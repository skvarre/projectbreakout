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

uint8_t buffer[4*128];

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

void moveleft( void ) {
  if(field[pos-1] == 0) {
    field[pos+7] = 0;
    field[pos-1] = 32;
    pos--;
    //quicksleep(200);
  }
}

void moveright( void ) {
  if(field[pos+8] == 0) {
  field[pos] = 0;
  field[pos+8] = 32;
  pos++;
  //quicksleep(200);
  }
}

void lit(int x, int y){
  int i = 0;
  for(i; i < 512; i++){
    buffer[i] = field[i];
  }
  buffer[128*(y>>3)+x] = (buffer[128*(y>>3)+x] | (0x1 << (y % 8)));
}

/*void scale(int x, int y, int w, int h){
  int i = h-1;
  int j = w-1;
  for(i; i >= 0; i--){
    for(j; j >= 0; j--){
      lit(x+j, y+i);
    }
  }
}*/



/* This function is called repetitively from the main program */
void labwork( void ) {

  if(getbtns() == 2){           // check if btn2/3/4 is pressed
    moveright();
    display_update();
    quicksleep(100000);
  }
  if(getbtns() == 4){           // check if btn2/3/4 is pressed
    moveleft();
    display_update();
    quicksleep(100000);
  }
  if(getbtns() == 1){
    int x = 28;
    int y = 26;
    while(1){
      lit(x,y);
      display_update();
      y--;
      quicksleep(10000000);
    }
  }

  if(IFS(0)){
    IFS(0) = 0x0;

  }

}
