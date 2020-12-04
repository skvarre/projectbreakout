#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(){
    return (PORTD & 0xf00) >> 8;
}

int getbtns(){
    return (PORTD & 0xe0) >> 5;
}

int getbtn1(){
    return (PORTF & 0x2) >> 1;
}
