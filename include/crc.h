#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include "rcc.h"

/* --- 1. Memory Map --- */
// CRC is on AHB1 Bus (Base: 0x4002 3000)
#define CRC_BASE      (AHB1PERIPH_BASE + 0x3000U)

/* --- 2. Register Structure --- */
typedef struct {
    volatile uint32_t DR;   // Data Register (0x00)
    volatile uint32_t IDR;  // Independent Data Register (0x04)
    volatile uint32_t CR;   // Control Register (0x08)
} CRC_TypeDef;

/* --- 3. Pointer Definition --- */
#define CRC           ((CRC_TypeDef *) CRC_BASE)

/* --- 4. Bit Definitions --- */
// RCC Clock Enable Bit for CRC (Bit 12 of AHB1ENR)
#define RCC_AHB1ENR_CRCEN   (1U << 12)

// CRC Control Register
#define CRC_CR_RESET        (1U << 0) // Reset bit

/* --- 5. Function Prototypes --- */

/**
 * @brief Initialize the CRC Engine (Enable Clock)
 */
void CRC_Init(void);

/**
 * @brief Calculate CRC32 for a buffer of data
 * @param data: Pointer to the data buffer
 * @param len: Number of 32-bit words (uint32_t) to check
 * @return The calculated CRC32 value
 */
uint32_t CRC_Calculate(uint32_t *data, uint32_t len);

#endif // CRC_H