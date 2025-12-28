#include <stdint.h>
#include <stddef.h>      // Required for NULL definition
#include "rcc.h"         // Clock Driver
#include "gpio.h"        // GPIO Driver
#include "my_allocator.h"// Memory Allocator
#include "systick.h"     // SysTick Driver 

// Global pointer for debugging/testing heap
volatile uint32_t *test_ptr;

int main(void) {
    // 1. Initialize System Clock to 180 MHz
    RCC_Init();

    // 2. Initialize GPIO (Setup PA5 and PC13)
    GPIO_Init();

    // 3. Initialize Heap (Setup free list in CCMRAM)
    allocator_init();

    // 4. Initialize SysTick (The Heartbeat)
    // Formula: Clock Freq / Target Freq = Ticks
    // 180,000,000 Hz / 1000 Hz (1ms) = 180,000 ticks
    SysTick_Init(180000); 

    // 5. Test Allocation
    test_ptr = (uint32_t *)my_malloc(sizeof(uint32_t));

    if (test_ptr != NULL) {
        *test_ptr = 0xDEADBEEF; // Write signature to verify memory works
    }
    
    // Super Loop
    while (1) {
        // Toggle LED (PA5)
        GPIO_Toggle(GPIOA, GPIO_PIN_5);
        
        // Professional Delay (500ms)
        // This is now accurate regardless of compiler optimization
        Delay(500); 
    }

    return 0;
}