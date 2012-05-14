#ifndef _SHELL_H
#define _SHELL_H

#include "structs.h"
#include "defs.h"
#include "video.h"
#include "strings.h"
#include "scheduler.h"
#include "limits.h"

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

static void top();
static void pkill();
static void imprimeUnos();




void auto_complete();
void clearCommand();
void erase_buffer();



#endif
