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
/*
int mytime = 0x5957;
int prime = 1234567;
char textstring[] = "text, more text, and even more text!";*/
int timeoutcount = 0;
int menupointer = 0;
int state = 0;

/* Position of players in array*/
int pos = 409;
int pos2 = 480;

/* Ball coordinates */
int x = 52;
int y = 26;

int b_dir = 4;
int* p = &b_dir;

/* Player 1 coordinates */
int px = 25;
int py = 29;

/* Player 2 coordinates */
int px2 = 96;
int py2 = 29;

int btn_status;

uint8_t buffer[4*128];

void lit(int x, int y, int px, int py, int px2, int py2){
  int k = 0;
  for(k; k < 512; k++){
    buffer[k] = field[k];
  }
  int i = 1;
  for(i; i >= 0; i--){
    int j = 1;
    for(j; j >= 0; j--){
      buffer[128*((y+i)>>3)+(x+j)] = (buffer[128*((y+i)>>3)+(x+j)] | (0x1 << ((y+i) % 8)));
    }
  }
  int l = 7;
  for(l; l >= 0; l--){
    buffer[128*(py>>3)+(px+l)] = (buffer[128*(py>>3)+(px+l)] | (0x1 << (py % 8)));
    buffer[128*(py2>>3)+(px2+l)] = (buffer[128*(py2>>3)+(px2+l)] | (0x1 << (py2 % 8)));
  }
}

void coll_det(int x, int y, int* p){
  switch(*p){

    // N
    case 0:
    if(buffer[128*((y-1)>>3)+x] & (0x1 << ((y-1) % 8))){b_dir = 1;}
    if(buffer[128*((y-1)>>3)+(x+1)] & (0x1 << ((y-1) % 8))){b_dir = 1;}
    break;

    // S
    case 1:
    if(y == 32){b_dir = 8; break;}
    if(buffer[128*((y+2)>>3)+x] & (0x1 << ((y+2) % 8))){b_dir = 0;}
    if(buffer[128*((y+2)>>3)+(x+1)] & (0x1 << ((y+2) % 8))){b_dir = 0;}
    break;

    // W
    case 2:
    if(buffer[128*(y>>3)+(x-1)] & (0x1 << (y % 8))){b_dir = 3;}
    if(buffer[128*((y+1)>>3)+(x-1)] & (0x1 << ((y+1) % 8))){b_dir = 3;}
    break;

    // E
    case 3:
    if(buffer[128*(y>>3)+(x+2)] & (0x1 << (y % 8))){b_dir = 2;}
    if(buffer[128*((y+1)>>3)+(x+2)] & (0x1 << ((y+1) % 8))){b_dir = 2;}
    break;

    // NW
    case 4:
    if(buffer[128*((y-1)>>3)+x] & (0x1 << ((y-1) % 8))){b_dir = 6;break;}
    if(buffer[128*((y-1)>>3)+(x+1)] & (0x1 << ((y-1) % 8))){b_dir = 6;break;}

    if(buffer[128*(y>>3)+(x-1)] & (0x1 << (y % 8))){b_dir = 5;break;}
    if(buffer[128*((y+1)>>3)+(x-1)] & (0x1 << ((y+1) % 8))){b_dir = 5;break;}
    break;


    // NE
    case 5:
    if(buffer[128*((y-1)>>3)+x] & (0x1 << ((y-1) % 8))){b_dir = 7;break;}
    if(buffer[128*((y-1)>>3)+(x+1)] & (0x1 << ((y-1) % 8))){b_dir = 7;break;}

    if(buffer[128*(y>>3)+(x+2)] & (0x1 << (y % 8))){b_dir = 4;break;}
    if(buffer[128*((y+1)>>3)+(x+2)] & (0x1 << ((y+1) % 8))){b_dir = 4;break;}
    break;


    // SW
    case 6:
    if(y == 32){b_dir = 8;break;}

    if(buffer[128*(y>>3)+(x-1)] & (0x1 << (y % 8))){b_dir = 7;break;}
    if(buffer[128*((y+1)>>3)+(x-1)] & (0x1 << ((y+1) % 8))){b_dir = 7;break;}

    if(buffer[128*((y+2)>>3)+x] & (0x1 << ((y+2) % 8))){b_dir = 4;break;}
    if(buffer[128*((y+2)>>3)+(x+1)] & (0x1 << ((y+2) % 8))){b_dir = 4;break;}
    break;

    // SE
    case 7:
    if(y == 32){b_dir = 8;break;}

    if(buffer[128*(y>>3)+(x+2)] & (0x1 << (y % 8))){b_dir = 6;break;}
    if(buffer[128*((y+1)>>3)+(x+2)] & (0x1 << ((y+1) % 8))){b_dir = 6;break;}

    if(buffer[128*((y+2)>>3)+x] & (0x1 << ((y+2) % 8))){b_dir = 5;break;}
    if(buffer[128*((y+2)>>3)+(x+1)] & (0x1 << ((y+2) % 8))){b_dir = 5;break;}
    break;

    default:
    break;

  }
}

void ball(int b_dir){

  switch(b_dir){

    case 0:
    y--;
    break;

    case 1:
    y++;
    break;

    case 2:
    x--;
    break;

    case 3:
    x++;
    break;

    case 4:
    x--;
    y--;
    break;

    case 5:
    x++;
    y--;
    break;

    case 6:
    x--;
    y++;
    break;

    case 7:
    x++;
    y++;
    break;

    case 8:
    break;

    default:
    break;

  }

}
/*
void lit_pad(int x, int y){
  int j = 7;
  for(j; j >= 0; j--){
    buffer[128*(y>>3)+(x+j)] = (buffer[128*(y>>3)+(x+j)] | (0x1 << (y % 8)));
  }
}*/

/* Interrupt Service Routine */
void user_isr( void ) {
  if((IFS(0)>>8) & 0x1){
    if(state == 1){
      coll_det(x, y, p);
      lit(x,y,px,py,px2,py2);
    }
    //display_update();
    IFS(0) &= ~0x100;
    timeoutcount++;
    if(timeoutcount == 10){
      if(state == 1){
        ball(b_dir);
      }
      timeoutcount = 0;
    }
    display_update();
  }
}

/* Lab-specific initialization goes here */
void labinit( void )
{
  volatile int* trise = (volatile int*) 0xbf886100;
  *trise&=255;
  *(trise + 0x10) = 0; /* setting porte to 0 */
  TRISD |= 0xfe0;
  TRISF |= 0x2;

  T2CON = 0x70; //Decides what scaler we want to use
  TMR2 = 0x0;
  PR2 = (80000000/256)/100;

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
  if(!(field[pos-1] & 0x20)) {
    px--;
    pos--;
  }
}

void moveright( void ) {
  if(!(field[pos+8] & 0x20)){
    px++;
    pos++;
  }
}

void moveleftp2( void ) {
  if(!(field[pos2-1] & 0x20)) {
    px2--;
    pos2--;
  }
}

void moverightp2( void ) {
  if(!(field[pos2+8] & 0x20)){
    px2++;
    pos2++;
  }
}

void start(){
  int i = 0;
  for(i; i<512; i++){
    buffer[i] = startscreen[i];
  }
  buffer[(385+43*menupointer)] = 31;
  buffer[(385+43*menupointer)+1] = 14;
  buffer[(385+43*menupointer)+2] = 4;
}

void score(){
  int i = 0;
  for(i; i<512; i++){
    buffer[i] = highscore[i];
  }
  int x = 3;
  int j = 0;
  for(x; x<11; x+3){
      buffer[131+128*j] = numbers[x];
      buffer[131+128*j+1] = numbers[x+1];
      buffer[131+128*j+2] = numbers[x+2];
      buffer[131+128*j+4] = 16;
      j++;
  }
}


/* This function is called repetitively from the main program */
void labwork( void ) {

  switch(state){

    case 0:
    if(getbtns() == 2){
      if(menupointer != 2){
        menupointer++;
        start();
        quicksleep(1000000);
        }
      }

    if(getbtns() == 4){
      if(menupointer != 0){
        menupointer--;
        start();
        quicksleep(1000000);
      }
    }

    if(getbtns() == 1){           // check if btn2/3/4 is pressed
      switch(menupointer){
        case 0:
          state = 1;
          lit(x, y, px, py, px2, py2);
        break;
        case 1:
        break;
        case 2:
          state = 3;
          score();
          quicksleep(1000000);
        break;
        default:
        break;
      }

    }
    break;
    case 1:

      btn_status = getbtns();
      if(btn_status & 0x1){moveleftp2();}
      if(btn_status & 0x2){moveright();}
      if(btn_status & 0x4){moveleft();}
      if(btn_status & 0x8){moverightp2();}
      quicksleep(100000);

    break;
    case 2:
    break;
    case 3:
      if(getbtns() & 0x1){
        state = 0;
        start();
        quicksleep(1000000);

      }
    break;
    default:
    break;
  }

}
