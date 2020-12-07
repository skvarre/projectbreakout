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

/* Initialized for counting timeouts used for ball speed */
int timeoutcount = 0;
int timeoutcount2 = 0;

/* Keeps track of the menu pointer */
int menupointer = 0;

/* Keeps track of the current screen */
int state = 0;

/* Keep track of what letter is displaying in game over screen */
int lettercounter = 0;
int positioncounter = 0;
int moveposition = 0;

/* High score pixels */
uint8_t player1score[3][3];
uint8_t player2score[3][3];
uint8_t first[3][3];
uint8_t second[3][3];
uint8_t third[3][3];

/* Player names */
uint8_t player1[3][4];
uint8_t player2[3][4];

/* High score */
int score1 = 0;
int score2 = 0;
int score3 = 0;

/* Ball struct */
struct Ball {
  int x, y, b_dir, speed, score;
};

/* Structs holding the x, y and direction vales of both player balls */
struct Ball p1 = { 28, 21, 0, 7, 0 };
struct Ball p2 = { 99, 21, 0, 7, 0 };

/* Pointers to player balls */
struct Ball *ptr1 = &p1;
struct Ball *ptr2 = &p2;

/* Starting position of players in buffer array */
int pos = 409;
int pos2 = 480;

/* Player 1 coordinates */
int px = 25;
int py = 29;

/* Player 2 coordinates */
int px2 = 96;
int py2 = 29;

/* Variables used for check_pixel function */
int sx;
int sy;

/* Value of detected buttons */
int btn_status;

/* New direction according to paddle for player 1 */
int n_dir;
int* q = &n_dir;

/* New direction according to paddle for player 2 */
int n_dir2;
int* q2 = &n_dir2;

/* Array to be painted onto the screen */
uint8_t buffer[4*128];

/* Values used by paddle_hit function */
int values[9] = {3, 3, 5, 5, 0, 4, 4, 2, 2};

/* Paints players and balls to screen using the field template */
void lit(struct Ball p1, struct Ball p2, int px, int py, int px2, int py2){
  int k = 0;
  for(k; k < 512; k++){
    buffer[k] = field[k];
  }
  int i = 1;
  for(i; i >= 0; i--){
    int j = 1;
    for(j; j >= 0; j--){
      buffer[128*((p1.y+i)>>3)+(p1.x+j)] = (buffer[128*((p1.y+i)>>3)+(p1.x+j)] | (0x1 << ((p1.y+i) % 8)));
      buffer[128*((p2.y+i)>>3)+(p2.x+j)] = (buffer[128*((p2.y+i)>>3)+(p2.x+j)] | (0x1 << ((p2.y+i) % 8)));
    }
  }
  int l = 7;
  for(l; l >= 0; l--){
    buffer[128*(py>>3)+(px+l)] = (buffer[128*(py>>3)+(px+l)] | (0x1 << (py % 8)));
    buffer[128*(py2>>3)+(px2+l)] = (buffer[128*(py2>>3)+(px2+l)] | (0x1 << (py2 % 8)));
  }
}

/* Calculate new direction for ball according to player position */
int paddle_hit(int x, int px){
  int c = px - x + 7;
  if(c >= 0 && c <= 8){
    return values[c];
  }else{
    return -1;
  }
}

/* Kill a specified pixel on screen */
void unlit(int x, int y){
  field[128*(y>>3)+x] = (field[128*(y>>3)+x] & ~(0x1 << (y % 8)));
}


void tickscore(int score, struct Ball* ptr){
  if(ptr->speed > 1 && score%10 == 0){
    ptr->speed--;
  }
  int dig1 = score/100;
  int dig2 = (score/10) % 10;
  int dig3 = score % 10;
  int currentdig[3] = {dig1, dig2, dig3};
  int i = 0;
  int j = 0;
  for(i; i<3; i++){
    for(j; j<3; j++){
      //Tick score depending on who is playing.
      if(ptr == &p1){
        player1score[i][j] = numbers[currentdig[i]][j];
        field[186+j+i*4] = numbers[currentdig[i]][j];
      }else if(ptr == &p2){
        player2score[i][j] = numbers[currentdig[i]][j];
        field[443+j+i*4] = numbers[currentdig[i]][j];
      }
    }
    j=0;
  }
}

/* Find and destroy (unlit) a block according to one of its 10 coordinates */
void find_des(int x, int y, struct Ball* ptr){
if((y >= 2 && y <= 16) && ((x >= 2 && x <= 54) || (x >= 73 && x <= 125))){
    ptr->score++;
    tickscore(ptr->score,ptr);
    int i = 0;
    while(field[128*(y>>3)+(x+i)] & (0x1 << (y % 8))){
      unlit(x+i,y-1);
      unlit(x+i,y);
      unlit(x+i,y+1);
      i++;
    }
    i = 1;
    while(field[128*(y>>3)+(x-i)] & (0x1 << (y % 8))){
      unlit(x-i,y-1);
      unlit(x-i,y);
      unlit(x-i,y+1);
      i++;
    }
  }
}

/* Check if there is a pixel at this coordinate */
int pixel_check(int x, int y, int b_dir){
  int i = 0;
  for(i; i < 2; i++){
    if(b_dir == 0){
      if(buffer[128*(y>>3)+(x+i)] & (0x1 << (y % 8))){
        sx = x+i;
        sy = y;
        return 1;
      }
    }
    if(b_dir == 1){
      if(buffer[128*((y+i)>>3)+x] & (0x1 << ((y+i) % 8))){
        sx = x;
        sy = y+i;
        return 1;
      }
    }
  }
  return 0;
}

/* Collision detection that according to the ball direction sends it a specified way */
void coll_det(struct Ball* ptr, int* n_dir){
  switch(ptr->b_dir){

    // N
    case 0:
    if(pixel_check(ptr->x, ptr->y-1, 0)){find_des(sx,sy,ptr);ptr->b_dir = 1;break;}

    break;

    // S
    case 1:
    if(ptr->y == 27 && *n_dir != -1){ptr->b_dir = *n_dir;break;}
    if(ptr->y == 32){ptr->b_dir = 8; break;}

    if(ptr->y < 29){
      if(pixel_check(ptr->x, ptr->y+2, 0)){ptr->b_dir = 0;break;}
    }
    break;

    // NW 22.5 degrees
    case 2:
    if(pixel_check(ptr->x, ptr->y-1, 0)){find_des(sx,sy,ptr);ptr->b_dir = 6;break;}
    if(pixel_check(ptr->x-1, ptr->y, 1)){find_des(sx,sy,ptr);ptr->b_dir = 5;break;}
    break;

    // NE 22.5 degrees
    case 3:
    if(pixel_check(ptr->x, ptr->y-1, 0)){find_des(sx,sy,ptr);ptr->b_dir = 7;break;}
    if(pixel_check(ptr->x+2, ptr->y, 1)){find_des(sx,sy,ptr);ptr->b_dir = 4;break;}
    break;

    // NW
    case 4:
    if(pixel_check(ptr->x, ptr->y-1, 0)){find_des(sx,sy,ptr);ptr->b_dir = 6;break;}
    if(pixel_check(ptr->x-1, ptr->y, 1)){find_des(sx,sy,ptr);ptr->b_dir = 5;break;}

    break;

    // NE
    case 5:
    if(pixel_check(ptr->x, ptr->y-1, 0)){find_des(sx,sy,ptr);ptr->b_dir = 7;break;}
    if(pixel_check(ptr->x+2, ptr->y, 1)){find_des(sx,sy,ptr);ptr->b_dir = 4;break;}
    break;

    // SW
    case 6:
    if(ptr->y == 27 && *n_dir != -1){ptr->b_dir = *n_dir;break;}
    if(ptr->y == 32){ptr->b_dir = 8;break;}

    if(ptr->y < 29){
      if(pixel_check(ptr->x-1, ptr->y, 1)){find_des(sx,sy,ptr);ptr->b_dir = 7;break;}
      if(pixel_check(ptr->x, ptr->y+2, 0)){find_des(sx,sy,ptr);ptr->b_dir = 4;break;}
    }
    break;

    // SE
    case 7:
    if(ptr->y == 27 && *n_dir != -1){ptr->b_dir = *n_dir;break;}
    if(ptr->y == 32){ptr->b_dir = 8;break;}

    if(ptr->y < 29){
      if(pixel_check(ptr->x+2, ptr->y, 1)){find_des(sx,sy,ptr);ptr->b_dir = 6;break;}
      if(pixel_check(ptr->x, ptr->y+2, 0)){find_des(sx,sy,ptr);ptr->b_dir = 5;break;}
    }
    break;

    default:
    break;
  }
}

void reset( void ){
  int i = 0;
  for(i; i < 512; i++){
    field[i] = s_field[i];
  }

  ptr1->x=28;
  ptr1->y=21;
  ptr1->b_dir=0;
  ptr1->speed=7;
  ptr1->score=0;

  ptr2->x=99;
  ptr2->y=21;
  ptr2->b_dir=0;
  ptr2->speed=7;
  ptr2->score=0;

  pos = 409;
  pos2 = 480;

  px = 25;
  py = 29;

  px2 = 96;
  py2 = 29;
}

/* Update ball coordinates according to current direction */
void ball(struct Ball* ptr){

  switch(ptr->b_dir){

    case 0:
    ptr->y--;
    break;

    case 1:
    ptr->y++;
    break;

    case 2:
    ptr->x--;
    ptr->x--;
    ptr->y--;
    break;

    case 3:
    ptr->x++;
    ptr->x++;
    ptr->y--;
    break;

    case 4:
    ptr->x--;
    ptr->y--;
    break;

    case 5:
    ptr->x++;
    ptr->y--;
    break;

    case 6:
    ptr->x--;
    ptr->y++;
    break;

    case 7:
    ptr->x++;
    ptr->y++;
    break;

    case 8:
    gameover();
    reset();
    break;

    default:
    break;

  }

}


/* Some initializations */
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

/* Move players left or right */
void moveleft( void ) {
  if(!(field[pos-1] & 0x20)) {
    px--;
    pos--;
  }
}

/* Move players left or right */
void moveright( void ) {
  if(!(field[pos+8] & 0x20)){
    px++;
    pos++;
  }
}

/* Move players left or right */
void moveleftp2( void ) {
  if(!(field[pos2-1] & 0x20)) {
    px2--;
    pos2--;
  }
}

/* Move players left or right */
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

void updatescore(){
  int position = 0;
  //Validate that the player should be listed in High score
  //if(playerscore < score1){position++;}else{score1=playerscore;}
  //if(playerscore < score2){position++;}else{score2=playerscore;}
  //if(playerscore < score3){return;}else{score3=playerscore;}
  //Show name and score
  int i = 0;
  int j = 0;
  for(i; i<3; i++){
    for(j; j<4; j++){
      highscore[138+128*position+j+i*5] = player1[i][j];
      if(position==2){highscore[138+128*position+j+i*5] = highscore[138+128*position+j+i*5]+128;}
    }
    j=0;
  }
}

void score(){
  int i = 0;
  for(i; i<512; i++){
    buffer[i] = highscore[i];
  }
  i = 0;
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

void updategameover(){
  int i = 0;
  for(i; i<4; i++){
    buffer[163+i+moveposition] = letters[lettercounter][i] << 3;
    player1[positioncounter][i] = letters[lettercounter][i];
  }
}

void updategameoverpoints(){
  int i = 0;
  int j = 0;
  for(i; i<3; i++){
    for(j; j<3; j++){
      buffer[194+j+i*4] = player1score[i][j] << 3;
      if(state==2){
        buffer[322+j+i*4] = player2score[i][j] << 3;
      }
    }
    j = 0;
  }
}

void gameover(){
  int i = 0;
  for(i; i<512; i++){
    if(state==1){
      buffer[i] = gameoverscreen1[i];
    }else if(state==2){
      buffer[i] = gameoverscreen2[i];
    }
  }
  updategameover();
  updategameoverpoints();
  state=4; 
}

/* A simple AI that follows the ball according to x coodrinates */
void AI( void ){
  if(px2 < ptr2->x){
    px2++;
  }
  if(px2+7 > ptr2->x){
    px2--;
  }
}

/* ISR that updates 100 times a second, used as a screen update */
void user_isr( void ) {
  if((IFS(0)>>8) & 0x1){
    if(state == 1 || state == 2){
      if(p1.y == 27){n_dir = paddle_hit(p1.x, px);}
      if(p2.y == 27){n_dir2 = paddle_hit(p2.x, px2);}
      coll_det(ptr1, q);
      coll_det(ptr2, q2);
      lit(p1, p2, px, py, px2, py2);
      if(state==1){AI();}
    }
    IFS(0) &= ~0x100;
    timeoutcount++;
    timeoutcount2++;
    if(timeoutcount == ptr1->speed){ // 7 default starting speed
      if(state == 1 || state == 2){
        ball(ptr1);
      }
      timeoutcount = 0;
    }
    if(timeoutcount2 == ptr2->speed){ // 7 default starting speed
      if(state == 1 || state == 2){
        ball(ptr2);
      }
      timeoutcount2 = 0;
    }
    display_update();
  }
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
          lit(p1, p2, px, py, px2, py2);
        break;
        case 1:
          state = 2;
          lit(p1, p2, px, py, px2, py2);
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
    case 1:
      btn_status = getbtns();
      if(btn_status & 0x2){moveright();}
      if(btn_status & 0x4){moveleft();}
      quicksleep(100000);
    break;
    case 2:
       btn_status = getbtns();
      if(btn_status & 0x1){moveleftp2();}
      if(btn_status & 0x2){moveright();}
      if(btn_status & 0x4){moveleft();}
      if(btn_status & 0x8){moverightp2();}
      quicksleep(100000);
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
          state = 0;
          start();
          updatescore();
          quicksleep(1000000);
        }else{
          updategameover();
          quicksleep(1000000);
        }
      }
    break;
    default:
    break;
  }
}
