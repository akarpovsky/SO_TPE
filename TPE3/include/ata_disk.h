#ifndef _ATA_DISK_H
#define _ATA_DISK_H

#include "defs.h"
#include "structs.h"
#include "limits.h"
#include "proc.h"
#include "scheduler.h"

/**********************************
*
*  ATA_DISK.h: Driver de disco
*
***********************************/

/* I/O Ports used by winchester disk controller. */
#define ATA0 0x1f0
#define ATA1 0x170

#define WIN_REG0       0x0
#define WIN_REG1       0x1 // Error
#define WIN_REG2       0x2
#define WIN_REG3       0x3
#define WIN_REG4       0x4
#define WIN_REG5       0x5
#define WIN_REG6       0x6
#define WIN_REG7       0x7 // Command|Status
#define WIN_REG8       0x3f6


/* Winchester disk controller command bytes. */
#define WIN_IDENTIFY	0xEC
#define MEDIA_STATUS	0xDA
#define READ_DMA		0xC8
#define WIN_RECALIBRATE	0x10	/* command for the drive to recalibrate */
#define WIN_READ        0x20	/* command for the drive to read */
#define WIN_WRITE       0x30	/* command for the drive to write */
#define WIN_SPECIFY     0x91	/* command for the controller to accept params */

#define LBA_READ        WIN_READ
#define LBA_WRITE       WIN_WRITE

/* Parameters for the disk drive. */
#define SECTOR_SIZE      512	/* physical sector size in bytes */

/* Miscellaneous. */
#define MAX_ERRORS         4	/* how often to try rd/wt before quitting */
#define NR_DEVICES        10	/* maximum number of drives */
#define MAX_WIN_RETRY  10000	/* max # times to try to output to WIN */
#define PART_TABLE     0x1C6	/* IBM partition table starts here in sect 0 */
#define DEV_PER_DRIVE      5	/* hd0 + hd1 + hd2 + hd3 + hd4 = 5 */

typedef struct disk_cmd {
	int ata;
	int sector;
	int offset;
	int bytes;
	char * buffer;
} disk_cmd;

/*
 * Escribe en el disco ata, los bytes indicados por parametro dedesde msg al sector "sector" con el offset "offset".
 * El offset comienza a partir del 0 y cada sector tiene 512 bytes. En caso que offset >= 512, se normaliza a 512 e incrementa el
 * sector (offset / 512) veces.
 */
void ata_write(int ata, void* msg, int bytes, unsigned short sector, int offset);

/*
 * Lee del disco ata, los bytes indicados por parametro a msg desde el sector "sector" con el offset "offset".
 * El offset comienza a partir del 0 y cada sector tien 512 bytes. En caso que offset >= 512, se normaliza a 512 e incrementa el
 * sector (offset / 512) veces.
 */
void ata_read(int ata, void* msg, int bytes, unsigned short sector, int offset);

unsigned short ata_getStatusRegister(int ata);

void identifyDevice(int ata);

void ata_checkDrive(int ata);

unsigned short getErrorRegister(int ata);

/*
 * Noraliza sector y offset de acuerdo al tamaño maximo del offset de cada sector.
 * luego de esta funcion, 0 <= offset < 512
 * sector = sector + offset / 512
*/
void ata_normalize(unsigned short* sector, int* offset);

void sendComm(int ata, int rdwr, unsigned short sector);
void _read(int ata, char * ans, unsigned short sector, int offset, int count);
void _write(int ata, char * msg, int bytes, unsigned short sector, int offset);

unsigned short getDataRegister(int ata);
void writeDataToRegister(int ata, char upper, char lower);

void translateBytes(char ans[], unsigned short sector);

// Many of the declarations of this header file are deprecated and the important ones are declared in the .c
//
//#define ATA0 0x1f0
//#define ATA1 0x170
//
//#define WIN_REG0       0x0
//#define WIN_REG1       0x1 // Error
//#define WIN_REG2       0x2
//#define WIN_REG3       0x3
//#define WIN_REG4       0x4
//#define WIN_REG5       0x5
//#define WIN_REG6       0x6
//#define WIN_REG7       0x7 // Command|Status
//#define WIN_REG8       0x3f6
//
//
///* Winchester disk controller command bytes. */
//#define WIN_IDENTIFY	0xEC
//#define MEDIA_STATUS	0xDA
//#define READ_DMA		0xC8
//#define WIN_RECALIBRATE	0x10	/* command for the drive to recalibrate */
//#define WIN_READ        0x20	/* command for the drive to read */
//#define WIN_WRITE       0x30	/* command for the drive to write */
//#define WIN_SPECIFY     0x91	/* command for the controller to accept params */
//
//#define LBA_READ        WIN_READ
//#define LBA_WRITE       WIN_WRITE
//
///* Parameters for the disk drive. */
//#define SECTOR_SIZE      512	/* physical sector size in bytes */
//
//
//#define MAX_ERRORS         4	/* how often to try rd/wt before quitting */
//#define NR_DEVICES        10	/* maximum number of drives */
//#define MAX_WIN_RETRY  10000	/* max # times to try to output to WIN */
//#define PART_TABLE     0x1C6	/* IBM partition table starts here in sect 0 */
//#define DEV_PER_DRIVE      5	/* hd0 + hd1 + hd2 + hd3 + hd4 = 5 */
//
//#define		READ_DISK	0;
//#define		WRITE_DISK	1;
//#define		OKATA		2;
//#define		ERROR_ATA	3;
//
//// The following calls are used by our filesystem to write to the disk.
//
//
///*
//	INTERRUPTS MUST BE DISABLED BEFORE USING THIS CALL
//
//	 _disk_read
//	ata: ATA controller to use, but actually deprecated to mantain compatibility.
//	ans: Buffer to read in.
//	numreads: Number of sectors to read.
//	sector: Sector to start reading in.
//*/
//int _disk_read(int ata, char * ans, int numreads, unsigned int sector);
//
//
///*
//	INTERRUPTS MUST BE DISABLED BEFORE USING THIS CALL
//
//	 _disk_write
//	ata: ATA controller to use, but actually deprecated to mantain compatibility.
//	ans: Buffer to write in.
//	numreads: Number of sectors to write.
//	sector: Sector to start writing in.
//*/
//int _disk_write(int ata, char * msg, int numreads, unsigned int sector);
//
///*
//	400ns wait
//*/
//void _400ns();

#endif
