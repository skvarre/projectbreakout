#include <emscripten.h>
#include "mipslab.h"

void main_loop() {
    labwork();
    user_isr();
    display_update();
}

int main() {
    labinit();
    start();
    emscripten_set_main_loop(main_loop, 0, 1);
    return 0;
}