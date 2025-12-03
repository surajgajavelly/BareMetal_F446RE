#include <stdint.h>

// A simple delay function so the CPU has something to do
void delay(uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        __asm("nop"); // Prevent compiler from removing this loop
    }
}

int main(void) {
    // 1. RCC Init will go here later
    // 2. GPIO Init will go here later

    // Super Loop
    while (1) {
        delay(100000);
    }

    return 0; // Should never allow the CPU to reach here
}