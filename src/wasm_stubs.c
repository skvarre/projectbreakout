// wasm_stubs.c
#include "wasm_stubs.h"

uint32_t SPI2STAT_stub = 0;
uint32_t SPI2CON_stub = 0;
uint32_t SPI2BRG_stub = 0;
uint32_t SPI2BUF_stub = 0;
uint32_t PORTF_stub = 0;
uint32_t PORTFCLR_stub = 0;
uint32_t PORTFSET_stub = 0;
uint32_t PORTE_stub = 0;
uint32_t PORTG_stub = 0;
uint32_t TRISF_stub = 0;
uint32_t TRISG_stub = 0;

int getbtns(void) {
    return EM_ASM_INT({
        return Module.getButtons();
    });
}