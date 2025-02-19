/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

/* Declare display-related functions from mipslabfunc.c */
#ifndef MIPSLAB_H
#define MIPSLAB_H
#include <stdint.h>
void display_image(int x, const uint8_t *data);
void display_init(void);
void display_string(int line, char *s);
void display_update(void);
void labinit(void);
void start(void);
void handleButton(int button);
void moveleft(void);
void moveright(void);
void user_isr(void);
uint8_t spi_send_recv(uint8_t data);


/* work around stuff */
extern void *stdin, *stdout, *stderr;

/* Declare lab-related functions from mipslabfunc.c */
char * itoaconv( int num );
void labwork(void);
int nextprime( int inval );
void quicksleep(int cyc);
void tick( unsigned int * timep );

/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
void display_debug( volatile int * const addr );

/* Declare bitmap array containing font */
extern const uint8_t const font[128*8];
/* Declare text buffer for display output */
extern char textbuffer[4][16];
extern uint8_t field[4*128];
extern uint8_t buffer[4*128];
extern uint8_t startscreen[4*128];
extern uint8_t highscore[4*128];
extern uint8_t gameoverscreen1[4*128];
extern uint8_t gameoverscreen2[4*128];
extern uint8_t player1[3][4];
extern uint8_t s_field[4*128];

extern uint8_t firstscore[3][3];
extern uint8_t secondscore[3][3];
extern uint8_t thirdscore[3][3];

extern uint8_t firstplayer[3][4];
extern uint8_t secondplayer[3][4];
extern uint8_t thirdplayer[3][4];
extern uint8_t bot[3][4];

extern const uint8_t numbers[10][3];
extern const uint8_t letters[25][4];

/* Declare functions written by students.
   Note: Since we declare these functions here,
   students must define their functions with the exact types
   specified in the laboratory instructions. */
/* Written as part of asm lab: delay, time2string */
void delay(int);
void time2string( char *, int );
/* Written as part of i/o lab: getbtns, getsw, enable_interrupt */
int getbtns(void);
int getbtn1(void);
int getsw(void);
// void enable_interrupt(void);
#endif