#ifndef _SHELL_H
#define _SHELL_H

#include "structs.h"
#include "defs.h"
#include "video.h"
#include "strings.h"
#include "scheduler.h"
#include "limits.h"
#include "memorym.h"

#define NUM_COMMANDS 19
#define LINEBUF_LEN 10


typedef struct command_t { 
        char name[LINEBUF_LEN]; 
        char args[LINEBUF_LEN - 2]; 
        int argc;
        char * argv[];
 
} command_t;

void shell();
void parse_command(command_t * c);
int run_command(command_t * c);


/* Avilable commands */

int divideByZero(int argc, char *argv);
int echo(int argc, char *argv);
int help(int argc, char *argv);
int invalidOpCode(int argc, char *argv);
int clear_screen(int argc, char *argv);
int shortcuts(int argc, char *argv);
int mouse(int argc, char *argv);
int top(int argc, char *argv);
int pkill(int argc, char *argv);
int imprimeUnos(int argc, char *argv);
int imprimeDos(int argc, char *argv);
int pagefault(int argc, char * argv);
int mallocs(int argc, char * argv);
int cd(int argc, char * argv);
int ls(int argc, char * argv);
int rm(int argc, char * argv);
int mv(int argc, char * argv);
int cp(int argc, char * argv);
int mkdir(int argc, char * argv);
int touch(int argc, char * argv);
int cd(int argc, char * argv);
int lsRemoved(int argc, char * argv);
int rmHard(int argc, char * argv);
int rmRecursive(int argc, char * argv);
int vh(int argc, char *argv);
int revert(int argc, char *argv);
int ln(int argc, char *argv);

void auto_complete(command_t * command);
void clearCommand(command_t * command);
void erase_buffer();
char * getcwd();



#endif
