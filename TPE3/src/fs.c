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

	}
	superblock.inodes[i].type = DIR_TYPE;
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
	superblock.inodes[i].count++;
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
	fileentry_t * checkEntry;
	while ((checkEntry = fsGetFileentry(dir, i++)) != NULL) {
		int aux = streq(checkEntry->name, entry_name);
		free(checkEntry);
		if (aux == TRUE)
			return FILE_ALREADY_EXISTS;
	}

	fe.position = i;
	fe.inode_number = entry->inode_number;
	fe.state = PRESENT;
	fe.type = ((entry->count <= 1) ? entry->type : LINK_TYPE);
	strcpy(fe.name, entry_name);

	int free_sector = dir->size / SECTOR_SIZE;
	int offset = dir->size - free_sector * SECTOR_SIZE;
	dir->size += sizeof(fileentry_t);
	ata_write(ATA0, &fe, sizeof(fe), dir->sectors[free_sector], offset);
	updateInode(dir);
	return OK;

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
	//	printf("inodeN: %d ; size: -%s- ; type: %d ; \n", ans->inode_number, ans->name, ans->type);

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
	fsVersionCopy(dir, fileToRemove, &newFile, oldVersion, newVersion);
}

int fsVersionCopy(inode_t * dir, fileentry_t * oldFile, fileentry_t * newFile,
		inode_t * oldVersion, inode_t * newVersion) {

	newVersion->next = NULL;
	newVersion->prev = oldVersion;
	oldVersion->next = newVersion;

	newVersion->rev_no = 1 + oldVersion->rev_no;
	newFile->inode_number = newVersion->inode_number;
	newFile->position = oldFile->position;
	newFile->type = oldFile->type;

	int sector = (sizeof(fileentry_t) * newFile->position) / SECTOR_SIZE;
	int offset = (newFile->position * sizeof(fileentry_t)) - (sector * SECTOR_SIZE);
	ata_read(ATA0, newFile, sizeof(fileentry_t), dir->sectors[sector], offset);

	updateInode(dir);
	updateInode(oldVersion);
	updateInode(newVersion);
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

