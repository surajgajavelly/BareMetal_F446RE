#include "flash.h"

// --- Private Helper Functions ---

/**
 * @brief Waits for the Busy (BSY) flag to clear.
 * The Flash is slow (Erase takes ~1 second). We must block the CPU until it's done.
 */
static void flash_wait_for_busy(void) {
    while (FLASH->SR & FLASH_SR_BSY) {
        // Ideally, we add a timeout here to prevent infinite hangs.
        // For now, we trust the hardware.
    }
}

// --- Public Functions ---

void Flash_Unlock(void) {
    // Only unlock if it is currently locked
    if (FLASH->CR & FLASH_CR_LOCK) {
        FLASH->KEYR = FLASH_KEY1;
        FLASH->KEYR = FLASH_KEY2;
    }
}

void Flash_Lock(void) {
    FLASH->CR |= FLASH_CR_LOCK;
}

uint8_t Flash_EraseSector(uint8_t sector) {
    // 1. Wait for any pending operations
    flash_wait_for_busy();

    // 2. Unlock the Flash
    Flash_Unlock();

    // 3. Clear Previous Error Flags (Write 1 to clear)
    FLASH->SR |= FLASH_SR_ERRORS;

    // 4. Configure Erase
    // - Clear SNB (Sector Number) bits [6:3]
    // - Set new SNB
    // - Set SER (Sector Erase)
    // - Set PSIZE to x32 (2) just to be safe for commands
    uint32_t cr_val = FLASH->CR;
    cr_val &= ~(0xF << 3);        // Clear SNB
    cr_val |= (sector << 3);      // Set SNB
    cr_val |= FLASH_CR_SER;       // Set Sector Erase
    cr_val &= ~(3U << 8);         // Clear PSIZE
    cr_val |= FLASH_CR_PSIZE_32;  // Set PSIZE x32
    FLASH->CR = cr_val;

    // 5. Trigger the Erase (The "GO" button)
    FLASH->CR |= FLASH_CR_STRT;

    // 6. Wait for it to finish (Can take 1-2 seconds!)
    flash_wait_for_busy();

    // 7. Check for Errors
    uint8_t status = 0;
    if (FLASH->SR & FLASH_SR_ERRORS) {
        status = (uint8_t)FLASH->SR; // Return the error flags
    }

    // 8. Cleanup
    FLASH->CR &= ~FLASH_CR_SER; // Clear SER bit
    Flash_Lock();

    return status;
}

uint8_t Flash_Write(uint32_t address, uint8_t *data, uint32_t len) {
    // 1. Wait for pending ops
    flash_wait_for_busy();
    Flash_Unlock();

    // 2. Clear Errors
    FLASH->SR |= FLASH_SR_ERRORS;

    // 3. Set Programming Mode to x8 (Byte Access)
    // This allows us to write byte-by-byte safely.
    FLASH->CR &= ~(3U << 8);      // Clear PSIZE -> Becomes 00 (x8)
    FLASH->CR |= FLASH_CR_PG;     // Program Mode Enable

    // 4. Loop through data bytes
    for (uint32_t i = 0; i < len; i++) {
        // Write the byte directly to the address
        *(volatile uint8_t*)(address + i) = data[i];

        // Wait for the write to finish (Flash is slower than CPU)
        flash_wait_for_busy();
        
        // Check errors immediately
        if (FLASH->SR & FLASH_SR_ERRORS) {
            FLASH->CR &= ~FLASH_CR_PG; // Turn off PG
            Flash_Lock();
            return (uint8_t)FLASH->SR;
        }
    }

    // 5. Cleanup
    FLASH->CR &= ~FLASH_CR_PG; // Disable Program Mode
    Flash_Lock();
    
    return 0; // Success
}