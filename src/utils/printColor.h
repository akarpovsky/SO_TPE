
#ifndef PRINTCOLOR_H
#define PRINTCOLOR_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../includes/message.h"

void printYellowColor(Msg_s answer);
void printGreenColor(Msg_s answer);
void printGreenBlink(Msg_s answer);
void printCyanColor(Msg_s answer);
void printRedColor(Msg_s answer);
void releasePrintColor(Msg_s answer);

#endif