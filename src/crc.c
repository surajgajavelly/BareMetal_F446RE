#include "crc.h"

void CRC_Init(void) {
    // 1. Enable CRC Clock on AHB1
    // The bit is defined in crc.h as (1U << 12)
    RCC->AHB1ENR |= RCC_AHB1ENR_CRCEN;
}

uint32_t CRC_Calculate(uint32_t *data, uint32_t len) {
    // 2. Reset the CRC calculation unit
    // This clears any previous results so we start fresh.
    CRC->CR |= CRC_CR_RESET;

    // 3. Feed the data
    // The CRC unit calculates the checksum cumulatively as we write to DR.
    for (uint32_t i = 0; i < len; i++) {
        CRC->DR = data[i];
    }

    // 4. Return result
    // Reading DR gives the final checksum.
    return CRC->DR;
}