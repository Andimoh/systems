/*-------------------------------------------------------------------------*
 *------*
 *---header.h---*
 *------*
 *--------------------------------------*
 *------*
 *---Version 1aJoseph Phillips---*
 *------*
 *-------------------------------------------------------------------------*/

#ifndef HEADER_H
#define HEADER_H
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 256

#define RANGE_LOWEST 0

#define RANGE_HIGHEST 32767

#define MIN_NUM_NUMBERS 0

#define MAX_NUM_NUMBERS 0x40000000

extern void countWithList(int numNums);

extern void countWithTree(int numNums);

extern int getNextNumber();

extern int lowest;

extern int highest;
