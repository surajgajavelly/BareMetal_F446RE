#include <stdint.h>
#include "rcc.h" // Include our new driver

// Simple delay function
void delay(uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        __asm("nop");
    }
}

int main(void) {
    // 1. Initialize System Clock to 180 MHz
    RCC_Init();

    // 2. GPIO Init will go here later

    // Super Loop
    while (1) {
        // We are now running at 180,000,000 cycles per second!
        delay(1000000); 
    }

    return 0;
}