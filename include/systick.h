#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

/* --- 1. Memory Map (ARM Cortex-M4 Standard) --- */
#define SCS_BASE            0xE000E000UL            // System Control Space Base
#define SYSTICK_BASE        (SCS_BASE + 0x0010UL)   // SysTick Base Address

/* --- 2. Register Structure --- */
typedef struct {
    volatile uint32_t CTRL;   // Offset: 0x00 (Control & Status)
    volatile uint32_t LOAD;   // Offset: 0x04 (Reload Value)
    volatile uint32_t VAL;    // Offset: 0x08 (Current Value)
    volatile uint32_t CALIB;  // Offset: 0x0C (Calibration)
} SysTick_TypeDef;

/* --- 3. Pointer Definition --- */
#define SysTick ((SysTick_TypeDef *) SYSTICK_BASE)

/* --- 4. Bit Definitions for CTRL Register --- */
#define SysTick_CTRL_ENABLE_Pos     0U
#define SysTick_CTRL_ENABLE_Msk     (1UL << SysTick_CTRL_ENABLE_Pos)  // Enable Counter

#define SysTick_CTRL_TICKINT_Pos    1U
#define SysTick_CTRL_TICKINT_Msk    (1UL << SysTick_CTRL_TICKINT_Pos) // Enable Interrupt

#define SysTick_CTRL_CLKSOURCE_Pos  2U
#define SysTick_CTRL_CLKSOURCE_Msk  (1UL << SysTick_CTRL_CLKSOURCE_Pos) // 1 = Processor Clock

/* --- 5. Public Functions --- */

/**
 * @brief Initialize SysTick to trigger an interrupt every 1ms
 * @param ticks: Number of clock cycles between interrupts
 */
void SysTick_Init(uint32_t ticks);

/**
 * @brief Interrupt Handler (Called automatically by CPU)
 */
void SysTick_Handler(void);

/**
 * @brief Returns the number of milliseconds since startup
 */
uint32_t GetTick(void);

/**
 * @brief Blocking delay in milliseconds
 */
void Delay(uint32_t ms);

#endif // SYSTICK_H