/* mipslabwork.c

   This file written 2015 by F Lundevall
   Updated 2017-04-21 by F Lundevall

   This file should be changed by YOU! So you must
   add comment(s) here with your name(s) and date(s):

   This file modified 2017-04-31 by Ture Teknolog

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

int mytime = 0x5957;
int prime = 1234567;
char textstring[] = "text, more text, and even more text!";
int timeoutcount = 0;



/* Interrupt Service Routine */
void user_isr( void ) {
  if((IFS(0)>>8) & 0x1){
    IFS(0) &= ~0x100;
    timeoutcount++;
    if(timeoutcount == 10){
      time2string( textstring, mytime );
      display_string( 3, textstring );
      display_update();
      //tick( &mytime );
      timeoutcount = 0;
    }
  }
  if(IFS(0)>>15 & 0x1){
    IFS(0) &= ~0x8000;
    PORTE += 1;
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
  T2CONSET = 0x8000;

  PORTE = 0;
  IPC(3) &= 0x1b000000;
  IPC(2) = 31; //Set to highest priority
  IFS(0) &= ~0x100;
  //IEC(0) = 0x100; //Enable
  IEC(0) = 0x8100; //Enable interrupt 3
  enable_interrupt();
}

/* This function is called repetitively from the main program */
void labwork( void ) {
  //prime = nextprime( prime );
  //display_string( 0, itoaconv( prime ) );
  display_update();
}
