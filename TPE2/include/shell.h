#ifndef _SHELL_H
#define _SHELL_H

#include "structs.h"
#include "defs.h"
#include "video.h"
#include "strings.h"
#include "scheduler.h"
#include "limits.h"
#include "memorym.h"

void shell();
void parse_command();
int run_command();


/* Avilable commands */

int divideByZero(int argc, char **argv);
int echo(int argc, char **argv);
int help(int argc, char **argv);
int invalidOpCode(int argc, char **argv);
int clear_screen(int argc, char **argv);
int shortcuts(int argc, char **argv);
int mouse(int argc, char **argv);
int top(int argc, char **argv);
int pkill(int argc, char **argv);
int imprimeUnos(int argc, char **argv);



void auto_complete();
void clearCommand();
void erase_buffer();



#endif
