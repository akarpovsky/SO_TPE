#include "../../include/ata_disk.h"


#define BIT0 (1)
#define BIT1 (1<<1)
#define BIT2 (1<<2)
#define BIT3 (1<<3)
#define BIT4 (1<<4)
#define BIT5 (1<<5)
#define BIT6 (1<<6)
#define BIT7 (1<<7)
#define BIT8 (1<<8)
#define BIT9 (1<<9)
#define BIT10 (1<<10)
#define BIT11 (1<<11)
#define BIT12 (1<<12)
#define BIT13 (1<<13)
#define BIT14 (1<<14)
#define BIT15 (1<<15)

#define DATA_PORT		BIT0 //Read/Write PIO data bytes on this port
#define ATAPI_PORT		BIT1 //Usually used for ATAPI devices
#define SECTOR_COUNT	BIT2 //Number of sectors to read/write
#define LBA_LO			BIT3 //Partial disc sector address
#define LBA_MID			BIT4 //Partial disc sector address
#define LBA_HI			BIT5 //Partial disc sector address
#define DRIVE_PORT		BIT6 //Used to select a drive and/or head
#define STATUS_PORT		BIT7 //Used to send commands or read the current status

#define ERR				BIT0 //Error made by user
#define	DRQ				BIT3 //Set when the drive is ready to transfer/recieve data
#define	SRV				BIT4 //Overlapped mode service request
#define	DF				BIT5 //Drive fault error
#define	RDY				BIT6 //When the bit is set, driver is ready
#define	BSY				BIT7 //Indicates the drive is preparing to send/recieve data

/* This defines are to check the sepecifications of the disk.
	#TODO: check size!*/
#define IS_REMOVABLE(D) (D & BIT7) ? kprintf("Is removable\n") : kprintf("Is not removable\n")
#define IS_ATA_DRIVE(D) (D & BIT15) ? kprintf("Is not ATA\n") : kprintf("Is ATA\n")

#define DMA_SUP(D) (D & BIT8) ? kprintf("DMA is supported\n") : kprintf("DMA is not supported\n")
#define LBA_SUP(D) (D & BIT9) ? kprintf("LBA is supported\n") : kprintf("LBA is not supported\n")
#define DMA_QUEUED_SUP(D) (D & BIT1) ? kprintf("DMA QUEUED supported\n") : kprintf("DMA QUEUED is not supported\n")

#define SECTOR_SIZE 512

int  hdddebug = 0;

int  lastsect = 0;

void translateBytes(char * ans, unsigned short databyte);
void writeDataToRegister (int ata, char upper, char lower);
unsigned short getDataRegister(int ata);

void _400ns() {
	_inb(0x3F6);
	_inb(0x3F6);
	_inb(0x3F6);
}

//Waits 400 nanoseconds to make sure the drive gets info before taking further actions.
int _drq_wait() {

	// _400ns();

	int test;
	while ((test = _inb(0x1F7)) && 1) {
		if(!(test & BSY))
		{
			if(test & DRQ)	{
				break;
			} else if(test & (ERR | DF))
			{
				if(test & ERR)	{
					kprintf("OUCH ERROR! %d %d %d\n", !!(test & DF), !!(test & ERR), lastsect);
					while(1);
				}

				return 1;
				break;
			}
		}
	}
	return 0;
}

//Says if disk is ready for another sector read/write
int _next_io() {
	int test;
	// _400ns();
	while ((test = _inb(0x1F7)) && 1) {
		if(!(test & BSY))
		{
			if(test & (ERR | DF))	{
				if(test & ERR)	{
					kprintf("IO Error %d %d %d\n", !!(test & DF), !!(test & ERR), lastsect );
					while(1);
				}
				return -1;
				break;
			} else  {
				break;
			}
		}
	}
	return 0;
}

//reads "numreads" sectors from disk starting at "sector"
int _disk_read(int ata, char * ans, int numreads, unsigned int sector){


	ata= ATA0;
	_outw(0x3F6, BIT2);

	int i = 0;
	for(i = 0; i < SECTOR_SIZE * numreads; ++i)
	{
		ans[i] = 0;
	}

	lastsect = sector;

	// _next_io();
	int retry = 1;
	while(retry) {
		unsigned long long addr = ((unsigned long long)sector) & 0x00000000ffffffff;

		//sets flags for disk reading
		_outb(0x1F1, 0x00);
		_outb(0x1F1, 0x00);
		_outb(0x1F2, 0x00);
		_outb(0x1F2, numreads);
		_outb(0x1F3, (unsigned char)(addr >> 24));
		_outb(0x1F3, (unsigned char)addr);
		_outb(0x1F4, 0);
		_outb(0x1F4, (unsigned char)(addr >> 8));
		_outb(0x1F5, 0);
		_outb(0x1F5, (unsigned char)(addr >> 16));
		_outb(0x1F6, 0);
		_outb(0x1F7, 0x24);

		retry = _drq_wait() == -1;
	}

	int b;
	unsigned short data;
	int errors = 0;
	int c = 0;
	for(i = 0; i < numreads;)
	{
		// kprintf("r3\n");
		for(b=0 ; b < SECTOR_SIZE ; b+=2, c+=2){
			data = getDataRegister(ata);
			translateBytes(ans+c, data);
		}
		++i;
		if(i == numreads)	{
			break;
		}
		retry = 1;
		while(retry) {
			retry = _next_io() == -1;
		}
	}


	return 1;
}

// Translate one word into two char
void translateBytes(char * ans, unsigned short databyte){
	ans[0] = databyte & 0x000000FF;
	ans[1] = (databyte >> 8) & 0x000000FF;
}

//writes "msg" in the "sector" of the disk, numreads indicates the amount
//of sectors the message will need
int _disk_write(int ata, char * msg, int numreads, unsigned int sector){


	// Sti();
	//_Halt();
	_outw(0x3F6, BIT2);

	lastsect = sector;
	// _next_io();

	ata=ATA0;
	int i = 0;

	int retry = 1;
	while(retry) {
	unsigned long long addr = ((unsigned long long)sector) & 0x00000000ffffffff;

	_outb(0x1F1, 0x00);
	_outb(0x1F1, 0x00);
	_outb(0x1F2, 0x00);
	_outb(0x1F2, numreads);
	_outb(0x1F3, (unsigned char)(addr >> 24));
	_outb(0x1F3, (unsigned char)addr);
	_outb(0x1F4, (unsigned char)(addr >> 32));
	_outb(0x1F4, (unsigned char)(addr >> 8));
	_outb(0x1F5, (unsigned char)(addr >> 40));
	_outb(0x1F5, (unsigned char)(addr >> 16));
	_outb(0x1F6, 0);
	_outb(0x1F7, 0x34);

		// kprintf("r1 %d\n", sector);
		retry = _drq_wait() == -1;
	}


	// kprintf("Write wait...\n");

	// kprintf("w1\n");
	_drq_wait();
	// kprintf("w2\n");

	// while (!(_inb(0x1F7) & DRQ)) {}


	// Now write all the sector
	int b;
	int c = 0;
	for(i = 0; i < numreads;)
	{
		// kprintf("w3\n");
		for (b=0; b<SECTOR_SIZE; b+=2, c+=2) {
			writeDataToRegister(ata, msg[c+1], msg[c]);
		}
		if(i == numreads)	{
			break;
		}
		++i;
		retry = 1;
		while(retry) {
			retry = _next_io() == -1;
		}
	}
	// Cli();
	return 1;
}

//writes the data in ATA language
void writeDataToRegister (int ata, char upper, char lower) {
	unsigned short out = 0;
	out = ((upper << 8) & 0xFF00) | (lower & 0xFF);

	_outw(ata + WIN_REG0, out);
}

//converts data from ATA language
unsigned short getDataRegister (int ata) {
	unsigned short ans;
	ans = _inw(ata + WIN_REG0);
	return ans;
}

void identifyDevice (int ata) {
	_out(ata + WIN_REG6, 0xA0);
	_out(ata + WIN_REG7, 0xEC);
}

// Check disk features
void check_drive(int ata) {
	ata = ATA0;
    identifyDevice(ata);

	unsigned short data = 0;
	unsigned short sector=12;
	int offset=0;
	int count=512;
	_next_io();
    int i;
 	short _60, _61, _100, _101, _102, _103;

    for(i=0;i<255;i++){
        data = getDataRegister(ata);
		if(i >= 27 && i <= 46)
		{
			if(data > 0)
			{
				kprintf("%c%c", (data & 0xFF00) >> 8, data & 0xFF);
			}
		}

		if(i == 60)
			_60 = data;
		if(i == 61)
			_61 = data;
    }
	kprintf("\n");
	kprintf("%d %d %d MB addressable\n", _61, _60, (_61 << 14 + _60) / 512);
}

//#define BIT(i)	(1 << (i))
//
//#define IS_REMOVABLE(D) 	kkprintf("Is %sremovable\n", (D & BIT(7)) ? "" : "not ")
//#define IS_ATA_DRIVE(D) 	kkprintf("Is %sATA\n", (D & BIT(15)) ? "not " : "")
//#define DMA_SUP(D) 			kkprintf("DMA is %ssupported\n", (D & BIT(8)) ? "" : "not ")
//#define LBA_SUP(D) 			kkprintf("LBA is %ssupported\n", (D & BIT(9)) ? "" : "not")
//#define DMA_QUEUED_SUP(D) 	kkprintf("DMA QUEUED supported\n", (D & BIT(1)) ? "" : "not ")
//
//void ata_normalize(unsigned short* sector, int* offset) {
//	if (*offset >= SECTOR_SIZE) {
//		*sector += (*offset / SECTOR_SIZE);
//		*offset %= SECTOR_SIZE;
//	}
//}
//
//void ata_read(int ata, void * msg, int bytes, unsigned short sector, int offset) {
//	// log(L_DEBUG, "reading from ATA disk [%d, %d]", sector, offset);
//	if (ata != ATA0 && ata != ATA1) {
//		//		log(L_ERROR, "Trying to read from an inexistent disk!! %d - [%d, %d]", ata, sector, offset);
//		//		errno = E_INVALID_ARG;
//		return;
//	}
//	char* ans = (char*) msg;
//	while (bytes != 0) {
//		if (offset >= SECTOR_SIZE) {
//			sector += (offset / SECTOR_SIZE);
//			offset %= SECTOR_SIZE;
//		}
//		int size;
//		if (offset + bytes > SECTOR_SIZE) { // read remaming part from the sector
//			size = SECTOR_SIZE - offset;
//			_read(ata, ans, sector, offset, size);
//			sector++;
//			offset = 0;
//			bytes -= size;
//			ans += size;
//		} else { // The remaning msg fits in the actual sector
//			size = bytes;
//			_read(ata, ans, sector, offset, size);
//			offset += size;
//			bytes = 0;
//			ans += size;
//		}
//	}
//}
//
//void _read(int ata, char * ans, unsigned short sector, int offset, int count) {
//	char tmp[SECTOR_SIZE];
//
//	sendComm(ata, LBA_READ, sector);
//	// Now read sector
//	int b;
//	unsigned short data;
//	for (b = 0; b < SECTOR_SIZE; b += 2) {
//		kkprintf("%d ", b);
//		data = getDataRegister(ata);
//		kkprintf("-%d-", data);
//		translateBytes(tmp + b, data);
//	}
//	kkprintf("%d ", b);
//	kkprintf("111112");
//	int i;
//	for (i = 0; i < count; i++) {
//		ans[i] = tmp[offset + i];
//	}
//}
//
//void translateBytes(char * ans, unsigned short databyte) {
//	ans[0] = databyte & 0xff;
//	ans[1] = (databyte >> 8) & 0xFF;
//
//}
//
//void ata_write(int ata, void * msg, int bytes, unsigned short sector,
//		int offset) {
//	//	log(L_DEBUG, "writing to ATA disk [%d, %d]", sector, offset);
//	if (ata != ATA0 && ata != ATA1) {
//		//		log(L_ERROR, "Trying to write to an inexistent disk!! %d - [%d, %d]", ata, sector, offset);
//		//		errno = E_INVALID_ARG;
//		return;
//	}
//	char* ans = (char*) msg;
//	while (bytes != 0) {
//		if (offset >= SECTOR_SIZE) {
//			sector += (offset / SECTOR_SIZE);
//			offset %= SECTOR_SIZE;
//		}
//		int size;
//		if (offset + bytes > SECTOR_SIZE) { // Fill sector
//			size = SECTOR_SIZE - offset;
//			_write(ata, ans, size, sector, offset);
//			sector++;
//			offset = 0;
//			bytes -= size;
//			ans += size;
//		} else { // The remaining msg fits in the actual sector
//			size = bytes;
//
//			_write(ata, ans, size, sector, offset);
//			offset += size;
//			bytes = 0;
//			ans += size;
//		}
//	}
//}
//
//void _write(int ata, char * msg, int bytes, unsigned short sector, int offset) {
//	char tmp[SECTOR_SIZE];
//	// Read current sector because ATA always writes a complete sector!
//	// Don't step previous values!
//	_read(ata, tmp, sector, 0, SECTOR_SIZE);
//
//	int i = 0;
//	for (i = 0; i < bytes; i++) {
//		tmp[offset + i] = msg[i];
//	}
//	sendComm(ata, LBA_WRITE, sector);
//	// Write updated sector
//	for (i = 0; i < SECTOR_SIZE; i += 2) {
//		writeDataToRegister(ata, tmp[i + 1], tmp[i]);
//	}
//}
//
//void writeDataToRegister(int ata, char upper, char lower) {
//	unsigned short out;
////	_Cli();
//	// Wait for driver's ready signal.
//	while (!(_portw_in(ata + WIN_REG7) & BIT(3)))
//		;
//	out = upper << 8 | (lower & 0xff);
//	_portw_out(ata + WIN_REG0, out);
////	_Sti();
//}
//
//unsigned short getDataRegister(int ata) {
//	unsigned short ans;
////	_Cli();
//	// Wait for driver's ready signal.
//	while (!(_portw_in(ata + WIN_REG7) & BIT(3)))
//	    ;
//	ans = _portw_in(ata + WIN_REG0);
////	_Sti();
//	return ans;
//}
//
//unsigned short getErrorRegister(int ata) {
////	_Cli();
//	unsigned short rta = _port_in(ata + WIN_REG1) & 0x00000FFFF;
////	_Sti();
//	return rta;
//}
//
//void sendComm(int ata, int rdwr, unsigned short sector) {
////	_Cli();
//	_port_out(ata + WIN_REG1, 0);
//	_port_out(ata + WIN_REG2, 0); // Set count register sector in 1
//
//	_port_out(ata + WIN_REG3, (unsigned char) sector); // LBA low
//	_port_out(ata + WIN_REG4, (unsigned char) (sector >> 8)); // LBA mid
//	_port_out(ata + WIN_REG5, (unsigned char) (sector >> 16)); // LBA high
//	_port_out(ata + WIN_REG6, 0xE0 | (ata << 4) | ((sector >> 24) & 0x0F)); // Set LBA bit in 1 and the rest in 0
//	// Set command
//	_port_out(ata + WIN_REG7, rdwr);
////	_Sti();
//}
//
//unsigned short ata_getStatusRegister(int ata) {
//	unsigned short rta;
////	_Cli();
//	rta = _port_in(ata + WIN_REG7) & 0x00000FFFF;
////	_Sti();
//	return rta;
//}
//
//void identifyDevice(int ata) {
////	_Cli();
//	_port_out(ata + WIN_REG6, 0);
//	_port_out(ata + WIN_REG7, WIN_IDENTIFY);
//
////	_Sti();
//}
//
//// Check disk features
//void ata_checkDrive(int ata) {
//	kkprintf("\n-----------------------\n");
//	kkprintf("Identifying device ");
//	switch (ata) {
//	case ATA0:
//		kkprintf("ATA0...");
//		break;
//	case ATA1:
//		kkprintf("ATA1...");
//		break;
//	}
//	kkprintf("\n");
//	identifyDevice(ata);
//	unsigned short data = 0;
//	int i;
//	for (i = 0; i < 255; i++) {
//		data = getDataRegister(ata);
//		switch (i) {
//		case 0:
//			//kkprintf("Data returned (%d): %d\n", i,data);
//			IS_REMOVABLE(data);
//			IS_ATA_DRIVE(data);
//			break;
//		case 49:
//			DMA_SUP(data);
//			LBA_SUP(data);
//			break;
//		case 83:
//			DMA_QUEUED_SUP(data);
//			break;
//		}
//	}
//}
