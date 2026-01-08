#include <stdint.h>
#include "rcc.h"
#include "gpio.h"
#include "systick.h"
#include "uart.h"
#include "flash.h" // <--- The new driver

// Helper to print strings
void Print(const char *str) {
    while (*str) UART2_Write(*str++);
}

// Helper to print Hex values
void PrintHex(uint32_t val) {
    char hex[] = "0123456789ABCDEF";
    // Print "0x"
    UART2_Write('0'); UART2_Write('x');
    
    // Print 8 hex digits
    for(int i = 7; i >= 0; i--) {
        UART2_Write(hex[(val >> (i * 4)) & 0xF]);
    }
}

int main(void) {
    // 1. Initialize System
    RCC_Init();
    SysTick_Init(180000); 
    GPIO_Init();
    UART2_Init(); 

    Print("\r\n=== FLASH DRIVER TEST ===\r\n");

    // --- TEST SETUP ---
    // Sector 1 Start Address: 0x0800 4000
    uint32_t test_addr = 0x08004000;
    uint8_t test_data[] = {0xDE, 0xAD, 0xBE, 0xEF};
    
    // 2. READ BEFORE ERASE
    Print("1. Reading Sector 1 (Before Erase)...\r\n");
    uint32_t *pMem = (uint32_t*)test_addr;
    Print("   Value: "); PrintHex(*pMem); Print("\r\n");

    // 3. ERASE
    Print("2. Erasing Sector 1 (Wait 1s)...\r\n");
    uint8_t status = Flash_EraseSector(1);
    if(status != 0) {
        Print("   ERASE FAILED! Status: "); PrintHex(status); Print("\r\n");
        while(1); // Stop here on error
    }
    Print("   Erase Complete.\r\n");

    // 4. VERIFY ERASE (Should be 0xFFFFFFFF)
    if (*pMem == 0xFFFFFFFF) {
        Print("   Verified: Sector is empty.\r\n");
    } else {
        Print("   ERROR: Sector not empty!\r\n");
    }

    // 5. WRITE
    Print("3. Writing 0xDEADBEEF...\r\n");
    status = Flash_Write(test_addr, test_data, 4);
    if(status != 0) {
        Print("   WRITE FAILED! Status: "); PrintHex(status); Print("\r\n");
    } else {
        Print("   Write Success.\r\n");
    }

    // 6. READ BACK
    Print("4. Reading Back...\r\n");
    Print("   Value: "); PrintHex(*pMem); Print("\r\n");

    if (*pMem == 0xEFBEADDE) { // Little Endian check (EF BE AD DE)
        Print("   SUCCESS: Data Matches!\r\n");
        Print("=========================\r\n");
    } else {
        Print("   ERROR: Data Mismatch!\r\n");
    }

    while (1) {
        // Heartbeat
        GPIO_Toggle(GPIOA, GPIO_PIN_5);
        Delay(500);
    }
    return 0;
}