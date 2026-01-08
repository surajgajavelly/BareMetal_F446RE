#ifndef FLASH_H
#define FLASH_H

#include <stdint.h>
#include "rcc.h" // For FLASH_R_BASE definition

// --- 1. Register Definitions (RM0390 Reference Manual) ---
typedef struct {
    volatile uint32_t ACR;      // Access Control Register (0x00)
    volatile uint32_t KEYR;     // Key Register (0x04)
    volatile uint32_t OPTKEYR;  // Option Key Register (0x08)
    volatile uint32_t SR;       // Status Register (0x0C)
    volatile uint32_t CR;       // Control Register (0x10)
    volatile uint32_t OPTCR;    // Option Control Register (0x14)
} FLASH_TypeDef;

// --- 2. Pointer Definition ---
// Connects the struct to the physical address 0x40023C00
#ifndef FLASH
#define FLASH ((FLASH_TypeDef *) FLASH_R_BASE)
#endif

// --- 3. Bit Definitions ---

// Status Register (SR) Flags - What's happening?
#define FLASH_SR_BSY        (1U << 16)  // Busy (1=Work in progress)
#define FLASH_SR_PGSERR     (1U << 7)   // Sequence Error
#define FLASH_SR_PGPERR     (1U << 6)   // Parallelism Error
#define FLASH_SR_PGAERR     (1U << 5)   // Alignment Error
#define FLASH_SR_WRPERR     (1U << 4)   // Write Protection Error
#define FLASH_SR_EOP        (1U << 0)   // End of Operation (Success)

// Error Mask: Handy to check all errors at once
#define FLASH_SR_ERRORS     (FLASH_SR_PGSERR | FLASH_SR_PGPERR | FLASH_SR_PGAERR | FLASH_SR_WRPERR)

// Control Register (CR) Bits - The Commands
#define FLASH_CR_LOCK       (1U << 31)  // Lock Bit (1=Locked)
#define FLASH_CR_STRT       (1U << 16)  // Start Command (Trigger)
#define FLASH_CR_PSIZE_32   (2U << 8)   // Program Size x32 (Required for 3.3V)
#define FLASH_CR_SER        (1U << 1)   // Sector Erase Mode
#define FLASH_CR_PG         (1U << 0)   // Programming Mode

// Unlock Keys (The "Secret Handshake")
#define FLASH_KEY1          0x45670123U
#define FLASH_KEY2          0xCDEF89ABU

// --- 4. Function Prototypes ---
void Flash_Unlock(void);
void Flash_Lock(void);
uint8_t Flash_EraseSector(uint8_t sector);
uint8_t Flash_Write(uint32_t address, uint8_t *data, uint32_t len);

#endif // FLASH_H