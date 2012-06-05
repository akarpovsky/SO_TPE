#include "../include/io.h"

extern char color_p;
size_t offset = 0x0;
char wbuffer[20]; /*buffer para escribir*/

unsigned char _in(unsigned char port) {
	/* Reads a byte from an I/O port */
	char c;

	__asm__("in %%dx, %%al;" /* Assembler instructions */
			:"=a" (c) /* Output variables */
			:"d" (port) /* Input variabless */
	);

	return c;
}

void _memcpy(void* from, void* to, size_t nbytes) {
	/* Copies bytes from a buffer to another */
	size_t i;
	for (i = 0; i < nbytes; i++)
		*((char *) to + i) = *((char *) from + i);
}

void _out(unsigned char port, unsigned char c) {
	/* Writes a byte to an I/O port */

	__asm__("out %%al, %%dx;" /* Assembler instructions */
			: /* Output variables */
			:"d" (port), "a" (c) /* Input variabless */
	);
}

/* __write
 *
 * Recibe como parametros:
 * - File Descriptor
 * - Buffer del source
 * - Cantidad
 *
 **/
size_t __write(int fd, void * buffer, size_t count) {

	_SysCall(SYSCALL_WRITE, fd, buffer, count);

	return count;
}

size_t __kwrite(int fd, void * buffer, size_t count) {

	_SysCall(SYSCALL_KERNELWRITE, fd, buffer, count);

	return count;
}

/* __read
 *
 * Recibe como parametros:
 * - File Descriptor
 * - Buffer a donde escribir
 * - Cantidad
 *
 **/
size_t __read(int fd, void * buffer, size_t count) {

	_SysCall(SYSCALL_READ, fd, buffer, count);

	return count;

}

void sysread(int fd, void * buffer, size_t count) {
	if (fd == STDIN) {
		unsigned char c = getKey();
		_memcpy((void *) &c, buffer, count);
	}
}

void syswrite(int fd, void * buffer, size_t count) {
	int i;
	if (fd == STDOUT) {
		ttyScreen_t * screen = (ttyScreen_t*) getScreen(get_current_task());

		_memcpy(buffer, screen->buffer, count);
		writeInVideo(*((char *) buffer));

	}
}

void syskernelwrite(int fd, void * buffer, size_t count) {

			kwriteInVideo(*((char *) buffer));
//			*(video+offset++) = *((char *) buffer++); // Print char to screen
//			*(video+offset++) = WHITE_TXT; // Char format


}

int sscanf(char* str, char* fmt, ...) {
	int ret;
	void* param;
	char *cs, *cf, *cparam;

	ret = 0;
	param = &fmt + 1;

	for (cs = str, cf = fmt; *cf != 0 && *cs != 0; cf++) {
		if (*cf == '%') {
			switch (*(++cf)) {

			case 0:
				/* Formato inválido! Ignorar el símbolo y finalizar el loop */
				cf--;
				break;

			case '%':
				if (*cs == '%')
					cs++;
				else
					return ret;
				break;

			case 'd':
				if (isNumeric(*cs)) {
					**(int**) param = 0;
					do {
						**(int**) param *= 10;
						**(int**) param += (*cs - '0');
					} while (isNumeric(*(++cs)));

					param += sizeof(int*);
					ret++;

				} else
					return ret;

			case 's':
				*cf++; /* cf ahora tiene el delimitador para el string */
				cparam = *(char**) param;

				while ((*cs != *cf) && (*cs != 0))
					*((char*) cparam++) = *(cs++);

				*(char*) cparam++ = 0;

				if (*cs != 0)
					*cs++;

				param += sizeof(char*);
				ret++;
			}

		} else {
			if (*cs == *cf)
				cs++;
			else
				return ret;
		}

	}

	return ret;

}

void putc(char c) {
	char l = c;
	__write(STDOUT, &l, 1);
}

void kputc(char c) {
	char l = c;
	__kwrite(STDOUT, &l, 1);
}

char getc() {
	char c;
	__read(STDIN, &c, 1);
//	while (c == EMPTY_CHAR) {
//		kprintf("c");
//		__read(STDIN, &c, 1);
//	}
	return c;
}

void puts(char* str) {
	while (*str != 0)
		putc(*str++);
}

void kputs(char* str) {
	while (*str != 0)
		kputc(*str++);
}

void putd(int d) {
	if (d < 0) {
		putc('-');
		d = 0 - d;
	}
	putu(d);
}

void kputd(int d) {
	if (d < 0) {
		kputc('-');
		d = 0 - d;
	}
	kputu(d);
}

void putu(unsigned int u) {
	if (u >= 10)
		putu(u / 10);
	putc(u % 10 + '0');
}

void kputu(unsigned int u) {
	if (u >= 10)
		kputu(u / 10);
	kputc(u % 10 + '0');
}

void kprintf(char *fmt, ...) {
	//_Cli();
	void *param = &fmt + 1;
	char c;
	while ((c = *fmt++) != 0) {
		if (c != '%') {
			kputc(c);
		} else {
			c = *fmt++;
			switch (c) {
			case '%':
				kputc('%');
				break;

			case 'd':
				kputd(*(int*) param);
				param += sizeof(int*);
				break;

			case 'u':
				kputu(*(unsigned int*) param);
				param += sizeof(unsigned int*);
				break;

			case 'c':
				kputc((char) (*(int*) param));
				param += sizeof(int*);
				break;

			case 's':
				kputs(*(char**) param);
				param += sizeof(char**);
				break;
            case 'x':
            	kputx(* (int *) param);
            	param += sizeof(int*);
            	break;
			}
		}
	}
	//_Sti();
}

void kputx(int x){
	char *letters ="ABCDEF";
	char c;
	int num;
	if (x <= 0) {
	return;
	}
	num = x % 16;
	c = num > 9 ? letters[num - 10] : (num + '0');

	kputx(x / 16);
	kputc(c);
}

void putx(int x){
	char *letters ="ABCDEF";
	char c;
	int num;
	if (x <= 0) {
	return;
	}
	num = x % 16;
	c = num > 9 ? letters[num - 10] : (num + '0');

	putx(x / 16);
	putc(c);
}

void printf(char *fmt, ...)
{
	//_Cli();
	void *param = &fmt + 1;
	char c;
	while ((c = *fmt++) != 0) {
		if (c != '%') {
			putc(c);
		} else {
			c = *fmt++;
			switch(c){
				case '%':
					putc('%');
					break;

				case 'd':
					putd(* (int*) param);
					param += sizeof(int*);
                    break;

				case 'u':
                    putu(* (unsigned int*) param);
                    param += sizeof(unsigned int*);
                    break;

				case 'c':
					putc((char)(* (int*) param));
					param += sizeof(int*);
                    break;

                case 's':
                    puts(* (char**) param);
                    param += sizeof(char**);
                    break;
                case 'x':
                	putx(* (int *) param);
                	param += sizeof(int*);
                	break;
			}
		}
	}
	//_Sti();
}

void printfcolor(char color, char *fmt, ...) {
	//_Cli();
	char color_aux = color_p;
	color_p = color;
	void *param = &fmt + 1;
	char c;
	while ((c = *fmt++) != 0) {

		if (c != '%') {
			putc(c);
		} else {
			c = *fmt++;
			switch (c) {
			case '%':
				putc('%');
				break;

			case 'd':
				putd(*(int*) param);
				param += sizeof(int*);
				break;

			case 'u':
				putu(*(unsigned int*) param);
				param += sizeof(unsigned int*);
				break;

			case 'c':
				putc((char) (*(int*) param));
				param += sizeof(int*);
				break;

			case 's':
				puts(*(char**) param);
				param += sizeof(char**);
				break;
			}
		}
	}
	color_p = color_aux;
	//_Sti();
}

void kprintfcolor(char color, char *fmt, ...) {
	//_Cli();
	char color_aux = color_p;
	color_p = color;
	void *param = &fmt + 1;
	char c;
	while ((c = *fmt++) != 0) {
		if (c != '%') {
			kputc(c);
		} else {
			c = *fmt++;
			switch (c) {
			case '%':
				kputc('%');
				break;

			case 'd':
				kputd(*(int*) param);
				param += sizeof(int*);
				break;

			case 'u':
				kputu(*(unsigned int*) param);
				param += sizeof(unsigned int*);
				break;

			case 'c':
				kputc((char) (*(int*) param));
				param += sizeof(int*);
				break;

			case 's':
				kputs(*(char**) param);
				param += sizeof(char**);
				break;
			}
		}
	}
	color_p = color_aux;
	//_Sti();
}

char isNumeric(char c) {
	return (c >= '0' && c <= '9');

}

int getchar() {
	char rta = 0;

	__read(STDIN, &rta, 1);
	return rta;
}

void debugger(char c) {
	/*    screen.address[ttys[actualTTY].screen->wpos+=2] = c;*/
}
