#include <stdint.h>
#include "rcc.h" // Include our new driver
#include "gpio.h" // Include GPIO driver
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

    // 2. Initialize GPIO (Enable Clocks, Setup PA5 and PC13)
    GPIO_Init();

    // 3. Initialize Heap (Setup free list)
    allocator_init();

    // 4. Test Allocation
    // This should land in CCMRAM (0x10000000) based on your Linker Script
    test_ptr = (uint32_t *)my_malloc(sizeof(uint32_t));

    if (test_ptr != NULL) {
        *test_ptr = 0xDEADBEEF;
    }
    

    // Super Loop
    while (1) {
        // Toggle LED (PA5)
        GPIO_Toggle(GPIOA, GPIO_PIN_5);
        
        // Delay (~500ms at 180MHz)
        delay(5000000); 
    }

    return 0;
}