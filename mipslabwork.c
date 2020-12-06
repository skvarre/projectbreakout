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

/* Keeps track of the menu pointer */
int menupointer = 0;

/* Keeps track of the current screen */
int state = 0;

/* Keep track of what letter is displaying in game over screen */
int lettercounter = 0;
int positioncounter = 0;
int moveposition = 0;

/* High score info */
uint8_t first[3][4];
uint8_t second[3][4];
uint8_t third[3][4];

/* Position of players in array*/
int pos = 409;
int pos2 = 480;

/* Ball coordinates */
int x = 28;
int y = 18;

int b_dir = 0;
int* p = &b_dir;

/* Player 1 coordinates */
int px = 25;
int py = 29;

/* Player 2 coordinates */
int px2 = 96;
int py2 = 29;

int btn_status;

int n_dir;
int* q = &n_dir;

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

int paddle_hit(int x, int px){
  int c = px - x;
  if(c >= -1 && c <= 1){
    return 4;
  }
  if(c >= -4 && c <= -2){
    return 0;
  }
  if(c >= -7 && c <= -5){
    return 5;
  }
  return -1;
}

void unlit(int x, int y){
  field[128*(y>>3)+x] = (field[128*(y>>3)+x] & ~(0x1 << (y % 8)));
}

void find_des(int x, int y, int* p){
  if((y >= 2 && y <= 16) && (x >= 2 && x <= 53)){
    int level;
    if(*p == 0 || *p == 4 || *p == 5){
      level = -1;
    }
    if(*p == 6 || *p == 7){
      level = 1;
    }
    int i = 0;
    while(field[128*(y>>3)+(x+i)] & (0x1 << (y % 8))){
      unlit(x+i,y-level);
      unlit(x+i,y);
      unlit(x+i,y+level);
      i++;
    }
    i = 1;
    while(field[128*(y>>3)+(x-i)] & (0x1 << (y % 8))){
      unlit(x-i,y-level);
      unlit(x-i,y);
      unlit(x-i,y+level);
      i++;
    }
  }
}

void coll_det(int x, int y, int* p, int* n_dir){
  switch(*p){

    // N
    case 0:
    if(buffer[128*((y-1)>>3)+x] & (0x1 << ((y-1) % 8))){find_des(x,y-1,p);b_dir = 1;break;}
    if(buffer[128*((y-1)>>3)+(x+1)] & (0x1 << ((y-1) % 8))){find_des(x+1,y-1,p);b_dir = 1;break;}
    break;

    // S
    case 1:
    if(y == 27 && *n_dir != -1){b_dir = *n_dir;break;}
    if(y == 32){b_dir = 8; break;}

    if(buffer[128*((y+2)>>3)+x] & (0x1 << ((y+2) % 8))){b_dir = 0;break;}
    if(buffer[128*((y+2)>>3)+(x+1)] & (0x1 << ((y+2) % 8))){b_dir = 0;break;}
    break;

    // W
    case 2:
    if(buffer[128*(y>>3)+(x-1)] & (0x1 << (y % 8))){b_dir = 3;break;}
    if(buffer[128*((y+1)>>3)+(x-1)] & (0x1 << ((y+1) % 8))){b_dir = 3;break;}
    break;

    // E
    case 3:
    if(buffer[128*(y>>3)+(x+2)] & (0x1 << (y % 8))){b_dir = 2;break;}
    if(buffer[128*((y+1)>>3)+(x+2)] & (0x1 << ((y+1) % 8))){b_dir = 2;break;}
    break;

    // NW
    case 4:
    if(buffer[128*((y-1)>>3)+x] & (0x1 << ((y-1) % 8))){find_des(x,y-1,p);b_dir = 6;break;}
    if(buffer[128*((y-1)>>3)+(x+1)] & (0x1 << ((y-1) % 8))){find_des(x+1,y-1,p);b_dir = 6;break;}

    if(buffer[128*(y>>3)+(x-1)] & (0x1 << (y % 8))){find_des(x-1,y,p);b_dir = 5;break;}
    if(buffer[128*((y+1)>>3)+(x-1)] & (0x1 << ((y+1) % 8))){find_des(x-1,y,p);b_dir = 5;break;}
    break;

    // NE
    case 5:
    if(buffer[128*((y-1)>>3)+x] & (0x1 << ((y-1) % 8))){find_des(x,y-1,p);b_dir = 7;break;}
    if(buffer[128*((y-1)>>3)+(x+1)] & (0x1 << ((y-1) % 8))){find_des(x+1,y-1,p);b_dir = 7;break;}

    if(buffer[128*(y>>3)+(x+2)] & (0x1 << (y % 8))){find_des(x+2,y,p);b_dir = 4;break;}
    if(buffer[128*((y+1)>>3)+(x+2)] & (0x1 << ((y+1) % 8))){find_des(x+2,y+1,p);b_dir = 4;break;}
    break;

    // SW
    case 6:
    if(y == 27 && *n_dir != -1){b_dir = *n_dir;break;}
    if(y == 32){b_dir = 8;break;}

    if(buffer[128*(y>>3)+(x-1)] & (0x1 << (y % 8))){find_des(x-1,y,p);b_dir = 7;break;}
    if(buffer[128*((y+1)>>3)+(x-1)] & (0x1 << ((y+1) % 8))){find_des(x-1,y+1,p);b_dir = 7;break;}

    if(buffer[128*((y+2)>>3)+x] & (0x1 << ((y+2) % 8))){find_des(x,y+2,p);b_dir = 4;break;}
    if(buffer[128*((y+2)>>3)+(x+1)] & (0x1 << ((y+2) % 8))){find_des(x+1,y+2,p);b_dir = 4;break;}
    break;

    // SE
    case 7:
    if(y == 27 && *n_dir != -1){b_dir = *n_dir;break;}
    if(y == 32){b_dir = 8;break;}

    if(buffer[128*(y>>3)+(x+2)] & (0x1 << (y % 8))){find_des(x+2,y,p);b_dir = 6;break;}
    if(buffer[128*((y+1)>>3)+(x+2)] & (0x1 << ((y+1) % 8))){find_des(x+2,y+1,p);b_dir = 6;break;}

    if(buffer[128*((y+2)>>3)+x] & (0x1 << ((y+2) % 8))){find_des(x,y+2,p);b_dir = 5;break;}
    if(buffer[128*((y+2)>>3)+(x+1)] & (0x1 << ((y+2) % 8))){find_des(x+1,y+2,p);b_dir = 5;break;}
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
    /*
    quicksleep(10000000);
    x = 28;
    y = 18;
    b_dir = 4;
    quicksleep(10000000);
    */
    break;

    default:
    break;

  }

}

/* Interrupt Service Routine */
void user_isr( void ) {
  if((IFS(0)>>8) & 0x1){
    if(state == 1){
      if(y == 27){n_dir = paddle_hit(x, px);}
      coll_det(x, y, p, q);
      lit(x,y,px,py,px2,py2);
    }
    IFS(0) &= ~0x100;
    timeoutcount++;
    if(timeoutcount == 7){ // 7 default starting speed
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

void updateScore(){
  int i = 0; 
  int j = 0;
  int end = 0;
  for(i; i<3; i++){
    if(i==2){end=128;}
    for(j; j<3; j++){
      buffer[131+128*i+j] = numbers[i+1][j] + end;
    }
    j = 0; 
    buffer[131+128*i+4] = 16 + end;
  }
}

void score(){
  int i = 0;
  for(i; i<512; i++){
    buffer[i] = highscore[i];
  }
  updateScore();
}

void updategameover(){
  uint8_t playername[3][4];
  int i = 0;
  for(i; i<4; i++){
    buffer[163+i+moveposition] = letters[lettercounter][i] << 3;
    playername[positioncounter][i] = letters[lettercounter][i];
  }
}

void updategameoverpoints(){
  /*PLACEHOLDER */
  int i = 0;
  int j = 0;
  int move = 0;
  for(i; i<3; i++){
    for(j; j<3; j++){
      buffer[194+j+i*4] = numbers[0][j] << 3;
    }
    j = 0;
  }
}

void gameover(){
  state = 4;
  int i = 0; 
  for(i; i<512; i++){
    buffer[i] = gameoverscreen[i];
  }
  updategameover();
  updategameoverpoints();
}


/* This function is called repetitively from the main program */
void labwork( void ) {
  /**
   * STATE 0: MAIN TITLE SCREEN
   * STATE 1: P1 GAME MODE
   * STATE 2: P2 GAME MODE
   * STATE 3: HIGH SCORE
   * STATE 4: GAME OVER SCREEN
   **/
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
    case 4: /* GAME OVER*/
      btn_status = getbtns();
      if(btn_status & 0x4){
        if(lettercounter == 0){lettercounter = 9;}
        lettercounter--;
        updategameover();
        quicksleep(1000000);
      }
      if(btn_status & 0x2){
        if(lettercounter == 8){lettercounter = -1;}
        lettercounter++;
        updategameover();
        quicksleep(1000000);
      }
      if(btn_status & 0x1){
        positioncounter++;
        if(positioncounter < 3){moveposition += 5;}
        if(positioncounter == 3){ 
          /* Set everything to zero and enter main screen*/
          positioncounter = 0; 
          moveposition = 0;
          lettercounter = 0;
        }
        updategameover();
        quicksleep(1000000);
      }
    break;
    default:
    break;
  }
}
