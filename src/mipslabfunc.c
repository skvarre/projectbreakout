/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
// #include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "wasm_stubs.h" /* VIRTUALIZATIONS of system-specific addresses etc */
#include "mipslab.h"  /* Declatations for these labs */

// #define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
// #define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)
void display_change_to_command_mode(void);
void display_change_to_data_mode(void);
extern uint8_t buffer[4*128];

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

void display_change_to_command_mode(void) {
    PORTFCLR = 0x10;
}

void display_change_to_data_mode(void) {
    PORTFSET = 0x10;
}



/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

// uint8_t spi_send_recv(uint8_t data) {
// 	while(!(SPI2STAT & 0x08));
// 	SPI2BUF = data;
// 	while(!(SPI2STAT & 1));
// 	return SPI2BUF;
// }

uint8_t spi_send_recv(uint8_t data) {
    return EM_ASM_INT({
        return Module.spiSendRecv($0);
    }, data);
}
// void display_init(void) {
//         DISPLAY_CHANGE_TO_COMMAND_MODE;
// 	quicksleep(10);
// 	DISPLAY_ACTIVATE_VDD;
// 	quicksleep(1000000);

// 	spi_send_recv(0xAE);
// 	DISPLAY_ACTIVATE_RESET;
// 	quicksleep(10);
// 	DISPLAY_DO_NOT_RESET;
// 	quicksleep(10);

// 	spi_send_recv(0x8D);
// 	spi_send_recv(0x14);

// 	spi_send_recv(0xD9);
// 	spi_send_recv(0xF1);

// 	DISPLAY_ACTIVATE_VBAT;
// 	quicksleep(10000000);

// 	spi_send_recv(0xA1);
// 	spi_send_recv(0xC8);

// 	spi_send_recv(0xDA);
// 	spi_send_recv(0x20);

// 	spi_send_recv(0xAF);
//   start();
// }

void display_init(void) {
    emscripten_run_script("Module.initDisplay()");
}

EMSCRIPTEN_KEEPALIVE
void display_update(void) {
	// emscripten_log(EM_LOG_CONSOLE, "Updating display");
    EM_ASM({
        Module.updateDisplay($0, $1);
    }, buffer, sizeof(buffer));
}

// void display_update(void) {
// 	int i, j, k;
// 	int c;
// 	for(i = 0; i < 4; i++) {
// 		display_change_to_command_mode();
// 		spi_send_recv(0x22);
// 		spi_send_recv(i);

// 		spi_send_recv(0x0);
// 		spi_send_recv(0x10);

// 		display_change_to_data_mode();

// 		for(j = 0; j < 128; j++) {
// 			c = buffer[128*i+j];
// 			spi_send_recv(c);
// 		}
// 	}
// }
