#include <stdint.h>
#include "rcc.h" // Include our new driver
#include "my_allocator.h" // Include custom allocator

// Simple delay function
void delay(uint32_t count) {
    for (uint32_t i = 0; i < count; i++) {
        __asm("nop");
    }
}

// Global pointer to ensure the optimizer doesn't delete our malloc
volatile uint32_t *test_ptr;

int main(void) {
    // 1. Initialize System Clock to 180 MHz
    RCC_Init();

    // 2. GPIO Init will go here later

    // 3. Test Allocation
    // We assign it to a global volatile variable so the compiler MUST perform this action.
    test_ptr = (uint32_t *)my_malloc(sizeof(uint32_t));

    // 4. Use the memory (Prove we can write to CCM RAM)
    if (test_ptr != NULL) {
        *test_ptr = 0xDEADBEEF;
    }
    

    // Super Loop
    while (1) {
        // We are now running at 180,000,000 cycles per second!
        delay(1000000); 
    }

    return 0;
}