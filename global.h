/* 
 * File:   global.h
 * Author: Lameguy64
 *
 * Created on March 13, 2018, 3:32 PM
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include "mainform.h"
#include "ContextClass.h"
#include "ConfigClass.h"

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#define VERSION "0.18b"

typedef struct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
} PIXEL;

extern ConfigClass config;
extern ContextClass project;
extern MainUI* ui;

void setActiveFace(int face);
void unsetFace();

#endif /* GLOBAL_H */

