#include "../include/structs.h"
#include "../include/defs.h"

#ifndef _SHELL_H
#define _SHELL_H

void shell();
void parse_command();
int run_command();


/* Avilable commands */

static void divideByZero();
static void echo();
static void help();
static void invalidOpcode();
static void clear_screen();
static void shortcuts();
static void mouse();



void auto_complete();
void clearCommand();
void erase_buffer();



#endif
