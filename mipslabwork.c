/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include <string.h>
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
int prime = 1234567;
char textstring[] = "text, more text, and even more text!";
int timeoutcount = 0;

int pos = 409;

int x = 52;
int y = 26;

int px = 25;
int py = 29;

uint8_t buffer[4*128];

void lit(int x, int y, int w, int h){
  int k = 0;
  for(k; k < 512; k++){
    buffer[k] = field[k];
  }
  int i = h-1;
  for(i; i >= 0; i--){
    int j = w-1;
    for(j; j >= 0; j--){
      buffer[128*((y+i)>>3)+(x+j)] = (buffer[128*((y+i)>>3)+(x+j)] | (0x1 << ((y+i) % 8)));
    }
  }
}

int coll_det(int x, int y){
  if(buffer[128*(y>>3)+x] & (0x1 << (y % 8))) {
    return 1;
  }else{
    return 0;
  }
}

void lit_pad(int x, int y){
  int j = 7;
  for(j; j >= 0; j--){
    buffer[128*(y>>3)+(x+j)] = (buffer[128*(y>>3)+(x+j)] | (0x1 << (y % 8)));
  }
}

/* Interrupt Service Routine */
void user_isr( void ) {
  if((IFS(0)>>8) & 0x1){
    display_update();
    IFS(0) &= ~0x100;
    timeoutcount++;
    if(timeoutcount == 1){
      lit(x,y,2,2);
      display_update();
      if(y == 15){
        y = 27;
      }
      y--;
      timeoutcount = 0;
    }
  }
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile int* trise = (volatile int*) 0xbf886100;
  *trise&=255;
  *(trise + 0x10) = 0; /* setting porte to 0 */
  TRISD |= 0xfe0;

  T2CON = 0x70; //Decides what scaler we want to use
  TMR2 = 0x0;
  PR2 = (80000000/256)/10;

  PORTE = 0;
  //IPC(3) &= 0x1b000000;
  IPC(2) = 31; //Set to highest priority
  IFS(0) &= ~0x100;
  IEC(0) = 0x100; //Enable
  //IEC(0) = 0x8100; //Enable interrupt 3
  T2CONSET = 0x8000;
  enable_interrupt();
}

void moveleft( void ) {
    if(!(field[pos-1] & 0x20)){
    field[pos+7] = 0;
    field[pos-1] = 32;
    pos--;
  }
}

void moveright( void ) {
  if(!(field[pos+8] & 0x20)){
    field[pos] = 0;
    field[pos+8] = 32;
    pos++;
  }
}

/* This function is called repetitively from the main program */
void labwork( void ) {
  if(getbtns() == 2){           // check if btn2/3/4 is pressed
    moveright();
    quicksleep(100000);
  }
  if(getbtns() == 4){           // check if btn2/3/4 is pressed
    moveleft();
    quicksleep(100000);
  }
}
