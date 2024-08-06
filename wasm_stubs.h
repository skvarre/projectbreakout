#ifndef WASM_STUBS_H
#define WASM_STUBS_H

#include <stdint.h>
#include <emscripten.h>

// Stub for IFS register
extern uint32_t IFS_stub;
#define IFS(x) IFS_stub

// Stub for hardware interrupt enable/disable
#define enable_interrupt()
#define disable_interrupt()

// Stub for SPI2STAT, SPI2CON, SPI2BRG
extern uint32_t SPI2STAT_stub;
extern uint32_t SPI2CON_stub;
extern uint32_t SPI2BRG_stub;
extern uint32_t SPI2BUF_stub;
#define SPI2STAT SPI2STAT_stub
#define SPI2CON SPI2CON_stub
#define SPI2BRG SPI2BRG_stub
#define SPI2BUF SPI2BUF_stub

// Stub for TRISD, TRISF
extern uint32_t TRISD_stub;
extern uint32_t TRISF_stub;
#define TRISD TRISD_stub
#define TRISF TRISF_stub

// Stub for T2CON, TMR2, PR2, T2CONSET
extern uint32_t T2CON_stub;
extern uint32_t TMR2_stub;
extern uint32_t PR2_stub;
extern uint32_t T2CONSET_stub;
#define T2CON T2CON_stub
#define TMR2 TMR2_stub
#define PR2 PR2_stub
#define T2CONSET T2CONSET_stub

// Stub for PORTE
extern uint32_t PORTE_stub;
#define PORTE PORTE_stub

// Stub for PORTF, PORTG
extern uint32_t PORTF_stub;
extern uint32_t PORTG_stub;
#define PORTF PORTF_stub
#define PORTG PORTG_stub

//PORTFCLR, PORTFSET
extern uint32_t PORTFCLR_stub;
extern uint32_t PORTFSET_stub;
#define PORTFCLR PORTFCLR_stub
#define PORTFSET PORTFSET_stub

// Stub for IPC
#define IPC(x) 0

// Stub for IEC
extern uint32_t IEC_stub;
#define IEC(x) IEC_stub

// Stub for quicksleep
void quicksleep(int cyc);

#endif // WASM_STUBS_H