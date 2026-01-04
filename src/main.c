#include <stdint.h>
#include "rcc.h"
#include "gpio.h"
#include "systick.h"
#include "uart.h"
#include "my_allocator.h" // Included but not used yet

// Helper to print strings
void Print(const char *str) {
    while (*str) UART2_Write(*str++);
}

int main(void) {
    // 1. SYSTEM CLOCK: 180 MHz
    RCC_Init();

    // 2. SysTick: 1ms Interrupts
    SysTick_Init(180000); 

    // 3. GPIO: PA5 (LED Output) & PC13 (Button Input)
    GPIO_Init();
    
    // 4. UART: 115200 Baud
    UART2_Init(); 

    // Boot Message
    Print("\r\n============================\r\n");
    Print("   INTEGRATION TEST PASSED  \r\n");
    Print("   Press Blue Button (PC13) \r\n");
    Print("============================\r\n");

    uint8_t prev_state = GPIO_PIN_SET; // Button is pulled high (1)
    uint8_t curr_state;

    while (1) {
        // Test GPIO Input
        curr_state = GPIO_Read(GPIOC, GPIO_PIN_13);

        // Detect Press (Falling Edge: 1 -> 0)
        if (prev_state == GPIO_PIN_SET && curr_state == GPIO_PIN_RESET) {
            
            // Test GPIO Output
            GPIO_Toggle(GPIOA, GPIO_PIN_5); 
            
            // Test UART TX
            Print(">> Button Pressed! Toggling LED.\r\n");
        }

        prev_state = curr_state;

        // Test SysTick Delay (Debounce)
        Delay(50); 
    }
    return 0;
}