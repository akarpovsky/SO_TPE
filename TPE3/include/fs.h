#ifndef _FS_H
#define _FS_H

#include "defs.h"
#include "ata_disk.h"

#define ABSENT		0
#define PRESENT     1
#define DIRTY		2
#define FREE		3

#define	FREE_INODE	1
#define DIR_TYPE	3
#define FILE_TYPE	4
#define LINK_TYPE	5


typedef struct inode_t inode_t;
typedef struct fileentry_t fileentry_t;
typedef struct superblock_t superblock_t;

typedef uint type_t;
typedef uint state_t;

struct fileentry_t{
	type_t		type;
	char		name[MAX_FILENAME];
	int			inode_number;
	uint		position;
	state_t		state;
};

struct inode_t{
	int			inode_number;
	int			count; // Number of references
	type_t		type;
	ushort		sectors[MAX_SECTORS];
	inode_t * 	next; // SVN
	inode_t *	prev; // SVN
	inode_t * 	parent;
	size_t		size;
	int			rev_no;
};

struct superblock_t{
	int	 check_number;
	inode_t inodes[MAX_INODES];
};

struct FILE{
	int	size;
	char * start;
	char * offset;
};

void fsInit(void);
void fsFormat(void);
void fsinitInode(inode_t * inode, int i);
void fsSetup();
int fsAddEntry(inode_t * dir, inode_t * entry, char * entry_name);
fileentry_t * fsGetFileentry(inode_t * cwd_inode, int i);
inode_t * getRootInode();
void updateInode(inode_t * inode);
int fsMkfile(char * name, inode_t * inode);
int fsMkdir(char * name, inode_t * inode);
inode_t * getInodeByNumber(int inode_number);
inode_t * getFreeInode();
int fsVersionCopy(inode_t * dir, fileentry_t * oldFile, fileentry_t * newFile,
		inode_t * oldVersion, inode_t * newVersion);
int fsRemove(inode_t * dir, fileentry_t * fileToRemove);


#endif
