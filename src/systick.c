#include "systick.h"

// Volatile is MANDATORY. 
// This variable is modified by an Interrupt (hardware) and read by Main (software).
// Without volatile, the compiler might optimize the read loop and assume the value never changes.
static volatile uint32_t ms_ticks = 0;

void SysTick_Init(uint32_t ticks) {
    // 1. Disable SysTick during configuration
    SysTick->CTRL = 0;

    // 2. Set the Reload Value
    // Note: The counter counts from N down to 0, so it takes N+1 cycles.
    // We subtract 1 to be exact.
    SysTick->LOAD = ticks - 1;

    // 3. Clear Current Value (Writing any value clears it)
    SysTick->VAL = 0;

    // 4. Enable SysTick
    // - CLKSOURCE=1: Use Processor Clock (180 MHz)
    // - TICKINT=1:   Enable Interrupts
    // - ENABLE=1:    Start Counting
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
                    SysTick_CTRL_TICKINT_Msk   | 
                    SysTick_CTRL_ENABLE_Msk;
}

// This function name is SPECIAL. 
// The linker script (.ld) and startup code expect exactly "SysTick_Handler".
// When the timer hits 0, the CPU pauses main() and jumps here.
void SysTick_Handler(void) {
    ms_ticks++;
}

uint32_t GetTick(void) {
    return ms_ticks;
}

void Delay(uint32_t ms) {
    uint32_t start_time = GetTick();
    
    // Wait until the difference between current time and start time is >= ms
    // This handles variable overflow automatically due to unsigned math.
    while ((GetTick() - start_time) < ms) {
        // Do nothing (Blocking wait)
    }
}