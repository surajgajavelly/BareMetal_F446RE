#ifndef RCC_H
#define RCC_H

#include <stdint.h>

// --- 1. Base Addresses (From Datasheet) ---
#define PERIPH_BASE         0x40000000U
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000U)

#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800U)
#define FLASH_R_BASE        (AHB1PERIPH_BASE + 0x3C00U)
#define PWR_BASE            (PERIPH_BASE + 0x7000U)

// --- 2. Register Definitions (The Map) ---

// RCC (Reset and Clock Control)
typedef struct {
    volatile uint32_t CR;        // 0x00
    volatile uint32_t PLLCFGR;   // 0x04
    volatile uint32_t CFGR;      // 0x08
    volatile uint32_t CIR;       // 0x0C
    volatile uint32_t AHB1RSTR;  // 0x10
    volatile uint32_t AHB2RSTR;  // 0x14
    volatile uint32_t AHB3RSTR;  // 0x18
    volatile uint32_t Reserved0; // 0x1C (Gap in hardware)
    volatile uint32_t APB1RSTR;  // 0x20
    volatile uint32_t APB2RSTR;  // 0x24
    volatile uint32_t Reserved1; // 0x28
    volatile uint32_t Reserved2; // 0x2C
    volatile uint32_t AHB1ENR;   // 0x30
    volatile uint32_t AHB2ENR;   // 0x34
    volatile uint32_t AHB3ENR;   // 0x38
    volatile uint32_t Reserved3; // 0x3C
    volatile uint32_t APB1ENR;   // 0x40
    volatile uint32_t APB2ENR;   // 0x44
} RCC_TypeDef;

// PWR (Power Control)
typedef struct {
    volatile uint32_t CR;        // 0x00
    volatile uint32_t CSR;       // 0x04
} PWR_TypeDef;

// --- 3. Hardware Pointers ---
#define RCC   ((RCC_TypeDef *) RCC_BASE)
#define PWR   ((PWR_TypeDef *) PWR_BASE)

// Note: FLASH pointer is now defined in flash.h to avoid conflicts

// --- 4. Bit Definitions (The Knobs) ---

// RCC_CR
#define RCC_CR_HSEON        (1U << 16)
#define RCC_CR_HSERDY       (1U << 17)
#define RCC_CR_PLLON        (1U << 24)
#define RCC_CR_PLLRDY       (1U << 25)

// RCC_PLLCFGR
#define RCC_PLLCFGR_PLLSRC_HSE (1U << 22)

// RCC_CFGR
#define RCC_CFGR_SW_PLL     (2U << 0) // 10: PLL selected as system clock
#define RCC_CFGR_SWS_PLL    (2U << 2) // 10: PLL used as system clock

// FLASH_ACR (Kept here for RCC_Init usage)
#define FLASH_ACR_LATENCY_5WS (5U << 0) // 101: Five wait states
#define FLASH_ACR_PRFTEN      (1U << 8)
#define FLASH_ACR_ICEN        (1U << 9)
#define FLASH_ACR_DCEN        (1U << 10)

// PWR_CR
#define PWR_CR_VOS_SCALE1     (3U << 14) // 11: Scale 1 mode (<= 180 MHz)

// --- 5. Prototypes ---
void RCC_Init(void);

#endif // RCC_H