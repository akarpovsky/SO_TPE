#include "../include/fs.h"

superblock_t superblock;
extern int offset;

int acum = 0;

void fsInit() {

	int check_number = 0;
	ata_read(ATA0, &check_number, sizeof(check_number), 0, 0);

	kprintf("Checking filesystem ...\n");
	if (check_number != MAGIC_NUMBER) {
		kprintf("No filesystem found.\n");
		fsFormat();
	}

	kprintf("Loading filesystem ...\n");
	ata_read(ATA0, &superblock, sizeof(superblock_t), 0, 0);

	kprintf("Redundancy check ...\n");

	kprintf("Filesystem succesfully loaded.\n");

}

void fsFormat() {
	kprintf("Formating filesystem ...\n");
	superblock.check_number = MAGIC_NUMBER;
	int i = 0;
	for (i = 0; i < MAX_INODES; i++) {
		fsinitInode(&superblock.inodes[i], i);
	}
	ata_write(ATA0, &superblock, sizeof(superblock_t), 0, 0);

	fsSetup();
}

void fsinitInode(inode_t * inode, int i) {
	inode->inode_number = i;
	inode->type = FREE_INODE;

	int j = 0;
	for (j = 0; j < MAX_SECTORS; j++) {
		inode->sectors[j] = FIRST_SECTOR + (i * MAX_SECTORS) + j;
	}

	inode->count = 0;
	inode->next = inode->prev = NULL;
	inode->size = 0;
	inode->rev_no = 0;
	inode->status = FREE;
	inode->name[0] = 0;
}

void fsSetup() {

	// Create root directory in the first inode
	kprintf("Creating root directory \"/\" ...\n");
	int ans = fsMkdir(NULL, NULL);
	if (ans == NO_SPACE) {
		kprintf("FATAL: Not enough space for ROOT!");
		_debug();
	}
}

int fsMkdir(char * name, inode_t * inode) {

	int i = 0;

	if (inode != NULL) {
		if (name == NULL) {
			return NO_FILENAME;
		}
		if (inode->type != DIR_TYPE) {
			return NOT_DIR;
		}
		for (i = 0; i < MAX_INODES; i++) {
			if (superblock.inodes[i].type == FREE_INODE)
				break;
		}
		if (i == MAX_INODES) {
			return NO_SPACE;
		}

		strcpy(superblock.inodes[i].name, name);
	}
	superblock.inodes[i].type = DIR_TYPE;
	superblock.inodes[i].status = PRESENT;
	superblock.inodes[i].count++;
	superblock.inodes[i].parent = inode;

	int ans;
	if (inode != NULL) { // Not root inode
		if ((ans = fsAddEntry(inode, &superblock.inodes[i], name)) != OK) {
			return ans;
		}
	}

	if ((ans = fsAddEntry(&superblock.inodes[i], &superblock.inodes[i], "."))
			!= OK) {
		return ans;
	}
	if ((ans = fsAddEntry(&superblock.inodes[i],
			(inode == NULL ? &superblock.inodes[i] : inode), "..")) != OK) {
		return ans;
	}

	updateInode(&superblock.inodes[i]);
	return OK;

}

int fsMkfile(char * name, inode_t * inode) {

	int i = 0;

	if (inode != NULL) {
		if (name == NULL) {
			return NO_FILENAME;
		}
		if (inode->type != DIR_TYPE) {
			return NOT_DIR;
		}
		for (i = 0; i < MAX_INODES; i++) {
			if (superblock.inodes[i].type == FREE_INODE)
				break;
		}
		if (i == MAX_INODES) {
			return NO_SPACE;
		}

	}
	superblock.inodes[i].type = FILE_TYPE;
	superblock.inodes[i].status = PRESENT;
	superblock.inodes[i].count++;
	strcpy(superblock.inodes[i].name, name);
	superblock.inodes[i].parent = inode;

	int ans;

	if ((ans = fsAddEntry(inode, &superblock.inodes[i], name)) != OK) {
		return ans;
	}

	updateInode(&superblock.inodes[i]);
	return OK;

}

/* Adds the entry 'entry' to the directory 'dir' */
int fsAddEntry(inode_t * dir, inode_t * entry, char * entry_name) {
	fileentry_t fe;

	if (dir->type != DIR_TYPE || dir == NULL || entry == NULL || entry_name
			== NULL) {
		return ERROR;
	}

	if ((dir->size + sizeof(fileentry_t)) > MAX_FILE_SIZE) {
		return NOT_ENOUGH_SPACE_IN_DIR;
	}

	/* Check if the entry_name already exists */
	int i = 0;
	int dirtyPosition = 0;
	fileentry_t * checkEntry;
	while ((checkEntry = fsGetFileentry(dir, i++)) != NULL) {
		if(checkEntry->state == DIRTY){
			dirtyPosition = i;
			continue;
		}
		int aux = streq(checkEntry->name, entry_name);
		free(checkEntry);
		if (aux == TRUE)
			return FILE_ALREADY_EXISTS;
	}

	fe.position = ((dirtyPosition == 0) ? i-1 : dirtyPosition-1);
	fe.inode_number = entry->inode_number;
	fe.state = PRESENT;
	fe.type = ((entry->count <= 1) ? entry->type : LINK_TYPE);
	strcpy(fe.name, entry_name);
	if(dirtyPosition == 0){
		dir->size += sizeof(fileentry_t);
	}
	updateEntry(&fe, dir);
	updateInode(dir);
	return OK;

}

void updateEntry(fileentry_t * entry, inode_t * inode){
	int sector = (sizeof(fileentry_t) * entry->position) / SECTOR_SIZE;
	int offset = (entry->position * sizeof(fileentry_t)) - (sector * SECTOR_SIZE);
	ata_write(ATA0, entry, sizeof(fileentry_t), inode->sectors[sector], offset);
}

void updateInode(inode_t * inode) {
	int sector = ((int) inode - (int) &superblock) / SECTOR_SIZE;
	int offset = ((int) inode - (int) &superblock) - sector * SECTOR_SIZE;
	ata_write(ATA0, inode, sizeof(inode_t), sector, offset);
}

inode_t * getRootInode() {
	return &superblock.inodes[0];
}

fileentry_t * fsGetFileentry(inode_t * cwd_inode, int i) {
	if (sizeof(fileentry_t) * i >= cwd_inode->size)
		return NULL;
	fileentry_t * ans = (fileentry_t *) malloc(sizeof(fileentry_t));
	int sector = (sizeof(fileentry_t) * i) / SECTOR_SIZE;
	int offset = (i * sizeof(fileentry_t)) - (sector * SECTOR_SIZE);

	ata_read(ATA0, ans, sizeof(fileentry_t), cwd_inode->sectors[sector], offset);
	return ans;
}

inode_t * getInodeByNumber(int inode_number) {
	if (inode_number >= 0 && inode_number < MAX_INODES)
		return &superblock.inodes[inode_number];

	return NULL;
}

int fsRemove(inode_t * dir, fileentry_t * fileToRemove) {

	inode_t * oldVersion = &superblock.inodes[fileToRemove->inode_number];
	inode_t * newVersion = getFreeInode();
	fileentry_t newFile;
	strcpy(newFile.name, fileToRemove->name);
	newFile.state = ABSENT;
	newVersion->status = ABSENT;
	fsVersionCopy(dir, fileToRemove, &newFile, oldVersion, newVersion);
}



int fsRemoveHard(inode_t * dir, fileentry_t * fileToRemove) {

	/* First remove the entry
	 * Then remove myself
	 * Then remove my previous
	 */

	inode_t * inode = &superblock.inodes[fileToRemove->inode_number];
	cleanEntry(fileToRemove, dir);
	inode_t * prev;

	while(inode != NULL){
		prev = inode->prev;
		fsinitInode(inode,inode->inode_number);
		updateInode(inode);
		inode = prev;
	}

}

int fsRecursiveRemoveHard(inode_t * dir) {

	fileentry_t * checkEntry;
	int i = 0;
	while ((checkEntry = fsGetFileentry(dir, i++)) != NULL) {
		if((checkEntry->inode_number == dir->inode_number && checkEntry->type == DIR_TYPE) ||
				(checkEntry->inode_number == dir->parent->inode_number && checkEntry->type == DIR_TYPE))
			continue;

		if(checkEntry->type == FILE_TYPE){
			fsRemoveHard(dir,checkEntry);
		} else if(checkEntry->type == LINK_TYPE){
			cleanEntry(checkEntry, dir);
		}else{
			fsRecursiveRemoveHard(&superblock.inodes[checkEntry->inode_number]);
			fsRemoveHard(dir,checkEntry);
		}
	}

}

int fsRecursiveRemoveHardWrapper(inode_t * dir, fileentry_t * fileToRemove){

	if(fileToRemove->type == FILE_TYPE){
		fsRemoveHard(dir,fileToRemove); // Remove myself cause i am a file
		return OK;
	}else if(fileToRemove->type == LINK_TYPE){
		cleanEntry(fileToRemove, dir);
		return OK;
	}

	fsRecursiveRemoveHard(&superblock.inodes[fileToRemove->inode_number]);
	fsRemoveHard(dir, fileToRemove);
}

void cleanEntry(fileentry_t * entry, inode_t * dir){
	entry->inode_number = 0;
	entry->name[0] = 0;
	entry->state = DIRTY;
	updateEntry(entry,dir);
}

int fsVersionCopy(inode_t * dir, fileentry_t * oldEntry, fileentry_t * newEntry,
		inode_t * oldVersion, inode_t * newVersion) {

	newVersion->next = NULL;
	newVersion->prev = oldVersion;
	oldVersion->next = newVersion;
	newVersion->parent = oldVersion->parent;

	strcpy(newVersion->name, oldVersion->name);
	newEntry->inode_number = newVersion->inode_number;
	newEntry->position = oldEntry->position;
	newEntry->type = oldEntry->type;
	newVersion->rev_no = oldVersion->rev_no+1;
	printf("ver -%d- %d", oldVersion->rev_no, newVersion->rev_no);
	updateEntry(newEntry,dir);
	updateInode(dir);
	updateInode(oldVersion);
	updateInode(newVersion);
}

inode_t * getInodeForEntry(fileentry_t * currentFile){
	return &superblock.inodes[currentFile->inode_number];
}

inode_t * getFreeInode() {
	//Search for a free inode to store the new version
	int i = 0;
	for (i = 0; i < MAX_INODES; i++) {
		if (superblock.inodes[i].type == FREE_INODE)
			break;
	}
	if (i == MAX_INODES) {
		return NULL;
	}

	return &superblock.inodes[i];
}

inode_t * fsGetPrevVersion(inode_t * inode){
	return inode->prev;
}

int fsRevert(inode_t * dir, fileentry_t * entry, int version){

	inode_t * current_version = getInodeForEntry(entry);
	inode_t * oldVersion = current_version;
	while((oldVersion = fsGetPrevVersion(oldVersion)) != NULL){
		if(oldVersion->rev_no == version){
			inode_t * newVersion = getFreeInode();
			fileentry_t newFile;
			strcpy(newFile.name, oldVersion->name);
			newFile.state = oldVersion->status;
			newFile.inode_number = newVersion->inode_number;
			fsInodeCopy(newVersion, oldVersion);
			fsVersionCopy(dir, entry, &newFile, current_version, newVersion);
			return OK;
		}
	}
	return NO_VERSION;
}

void fsInodeCopy(inode_t * newVersion, inode_t * oldVersion){
	char sector[SECTOR_SIZE];
	int i = 0;
	newVersion->size = oldVersion->size;
	newVersion->rev_no = oldVersion->rev_no;
	newVersion->status = oldVersion->status;
	strcpy(newVersion->name, oldVersion->name);
	for (i = 0; i < MAX_SECTORS; i++) {
		ata_read(ATA0,sector,SECTOR_SIZE,oldVersion->sectors[i], 0);
		ata_write(ATA0,sector,SECTOR_SIZE,newVersion->sectors[i], 0);
	}
	updateInode(newVersion);

}
