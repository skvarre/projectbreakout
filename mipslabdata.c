/* mipslabdata.c
   This file compiled 2015 by F Lundevall
   from original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

char textbuffer[4][16];

const uint8_t const font[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};

uint8_t field[] = {
	255, 1, 109, 109, 109, 109, 109, 1,
	109, 109, 109, 109, 109, 1, 109, 109,
	109, 109, 109, 1, 109, 109, 109, 109,
	109, 1, 109, 109, 109, 109, 109, 1,
	109, 109, 109, 109, 109, 1, 109, 109,
	109, 109, 109, 1, 109, 109, 109, 109,
	109, 1, 109, 109, 109, 109, 109, 1,
	255, 0, 0, 60, 20, 20, 12, 0,
	0, 36, 60, 32, 0, 0, 255, 255,
	1, 109, 109, 109, 109, 109, 1,
	109, 109, 109, 109, 109, 1, 109, 109,
	109, 109, 109, 1, 109, 109, 109, 109,
	109, 1, 109, 109, 109, 109, 109, 1,
	109, 109, 109, 109, 109, 1, 109, 109,
	109, 109, 109, 1, 109, 109, 109, 109,
	109, 1, 109, 109, 109, 109, 109, 1, 255,

	255, 0, 219, 219, 219, 219, 219, 0,
	219, 219, 219, 219, 219, 0, 219, 219,
	219, 219, 219, 0, 219, 219, 219, 219,
	219, 0, 219, 219, 219, 219, 219, 0,
	219, 219, 219, 219, 219, 0, 219, 219,
	219, 219, 219, 0, 219, 219, 219, 219,
	219, 0, 219, 219, 219, 219, 219, 0,
	255, 0, 31, 17, 15, 0, 31, 17,
	15, 0, 31, 17, 15, 0, 255, 255,
	0, 219, 219, 219, 219, 219, 0, 219,
	219, 219, 219, 219, 0, 219, 219, 219,
	219, 219, 0, 219, 219, 219, 219, 219,
	0, 219, 219, 219, 219, 219, 0, 219,
	219, 219, 219, 219, 0, 219, 219, 219,
	219, 219, 0, 219, 219, 219, 219, 219,
	0, 219, 219, 219, 219, 219, 0, 255,

	255, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	255, 255, 1, 1, 61, 21, 21, 13,
	1, 1, 53, 53, 45, 1, 1, 255,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 255,

	255, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	255, 255, 0, 31, 17, 15, 0, 31,
	17, 15, 0, 31, 17, 15, 0, 255,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 255
};

uint8_t startscreen[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 254, 2, 2, 2, 2, 2, 132,
	120, 0, 0, 254, 2, 2, 2, 2,
	2, 132, 120, 0, 0, 254, 2, 2,
	2, 2, 2, 2, 0, 0, 248, 4,
	2, 2, 2, 2, 2, 4, 248, 0,
	0, 254, 0, 128, 64, 32, 16, 14,
	0, 0, 248, 4, 2, 2, 2, 2,
	2, 2, 4, 248, 0, 0, 254, 0,
	0, 0, 0, 0, 0, 0, 254, 0,
	0, 2, 2, 2, 2, 254, 2, 2,
	2, 2, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 255, 129, 129, 129, 129, 129, 66,
	60, 0, 0, 255, 1, 1, 1, 1,
	3, 6, 248, 0, 0, 255, 129, 129,
	129, 129, 129, 129, 0, 0, 255, 1,
	1, 1, 1, 1, 1, 1, 255, 0,
	0, 255, 1, 2, 4, 8, 16, 224,
	0, 0, 63, 64, 128, 128, 128, 128,
	128, 128, 64, 63, 0, 0, 63, 64,
	128, 128, 128, 128, 128, 64, 63, 0,
	0, 0, 0, 0, 0, 255, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 254, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2,
	114, 138, 138, 114, 2, 250, 18, 34,
	250, 2, 250, 170, 170, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2,
	254, 0, 0, 0, 0, 0, 0, 0,
	254, 2, 2, 2, 2, 2, 2, 2,
	10, 10, 250, 10, 10, 2, 122, 130,
	66, 130, 122, 2, 114, 138, 138, 114,
	2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 254, 0, 0, 0, 0,
	0, 0, 0, 254, 2, 2, 2, 2,
	2, 2, 2, 2, 250, 34, 34, 250,
	2, 250, 2, 114, 138, 170, 234, 2,
	250, 34, 34, 250, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 254, 0,

	0, 0, 0, 0, 0, 255, 128, 128,
	128, 128, 190, 138, 138, 132, 128, 190,
	160, 160, 160, 128, 188, 138, 138, 188,
	128, 142, 184, 142, 128, 190, 170, 170,
	128, 190, 138, 154, 164, 128, 128, 128,
	255, 0, 0, 0, 0, 0, 0, 0,
	255, 128, 190, 138, 138, 132, 128, 190,
	160, 160, 160, 128, 188, 138, 138, 188,
	128, 142, 184, 142, 128, 190, 170, 170,
	128, 190, 138, 154, 164, 128, 164, 170,
	170, 146, 128, 255, 0, 0, 0, 0,
	0, 0, 0, 255, 128, 128, 128, 128,
	128, 164, 170, 170, 146, 128, 156, 162,
	162, 162, 128, 156, 162, 162, 156, 128,
	190, 138, 154, 164, 128, 190, 170, 170,
	162, 128, 128, 128, 128, 128, 255, 0,

};

uint8_t highscore[] = {

	255, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 125, 17,
	17, 125, 1, 125, 1, 125, 69, 85, 117, 1,
	125, 17, 17, 125, 1, 1, 1, 1,
	73, 85, 85, 37, 1, 57, 69, 69,
	69, 1, 57, 69, 69, 69, 57, 1,
	125, 21, 53, 93, 1, 125, 85, 85,
	69, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 255,

	255, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 255,

	255, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 248, 168, 168, 80, 0,
	8, 248, 8, 0, 0, 248, 16, 32,
	248, 0, 232, 168, 168, 184, 0, 255,

	255, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 128, 128, 128, 128, 128,
	128, 128, 128, 190, 170, 170, 148, 128,
	188, 138, 138, 188, 128, 156, 162, 162,
	162, 128, 190, 136, 148, 162, 128, 255,
};

uint8_t gameoverscreen[] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 28, 34, 42, 58, 0, 60, 
	10, 10, 60, 0, 62, 4, 8, 4, 
	62, 0, 62, 42, 42, 34, 0, 0, 
	0, 0, 28, 34, 34, 28, 0, 30, 
	32, 30, 0, 62, 42, 42, 34, 0,
	62, 10, 26, 36, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	0, 248, 40, 40, 16, 0, 248, 0,
	0, 0, 248, 16, 32, 248, 0, 240,
	40, 40, 240, 0, 248, 16, 32, 16,
	248, 0, 248, 168, 168, 136, 0, 80,
	0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	248, 40, 40, 16, 0, 8, 248, 8,
	0, 144, 168, 168, 72, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

uint8_t player1[3][4] = {
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
};

const uint8_t numbers[10][3] = {
	{31, 17, 15}, //0 Index 0-2
	{0, 0, 31},   //1 Index 3-5
	{29, 21, 23}, //2 Index 6-8
	{21, 21, 31}, //3 Index 9-11
	{7, 4, 31},   //4 Index 12-14
	{23, 21, 29}, //5 Index 15-17
	{31, 21, 29}, //6 Index 18-20
	{3, 1, 31},   //7 Index 21-23
	{31, 21, 31}, //8 Index 24-26
	{7, 5, 31},   //9 Index 27-29
};

const uint8_t letters[9][4] = {
	{30, 5, 5, 30},    //A
	{31, 21, 21, 10},  //B
	{14, 17, 17, 17},  //C
	{31, 17, 17, 14},  //D
	{31, 21, 21, 17},  //E
	{31, 5, 5, 1},     //F
	{14, 17, 21, 29}, //G
	{31, 4, 4, 31},    //H
	{0, 31, 0, 0}      //I
};