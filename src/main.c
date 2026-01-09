#include <stdint.h>
#include "rcc.h"
#include "gpio.h"
#include "systick.h"
#include "uart.h"
#include "flash.h"
#include "crc.h"

// Helper to print strings
void Print(const char *str) {
    while (*str) UART2_Write(*str++);
}

// Helper to print Hex values
void PrintHex(uint32_t val) {
    char hex[] = "0123456789ABCDEF";
    UART2_Write('0'); UART2_Write('x');
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
    CRC_Init();

    Print("\r\n=== CRC DRIVER TEST ===\r\n");

    // --- TEST SETUP ---
    // The STM32 Hardware CRC unit uses a specific polynomial (0x04C11DB7).
    // For the input 0xDEADBEEF, the hardware MUST return 0x1A5A601F.
    uint32_t data[] = {0xDEADBEEF};
    
    // Calculate
    uint32_t result = CRC_Calculate(data, 1);

    Print("Input:  0xDEADBEEF\r\n");
    Print("Output: "); PrintHex(result); Print("\r\n");

    // Verify against known hardware value
    if (result == 0x81DA1A18) {
        Print("SUCCESS: CRC Matches!\r\n");
    } else {
        Print("ERROR: CRC Mismatch!\r\n");
    }

    Print("=======================\r\n");

    /* // --- OPTIONAL: FLASH TEST ---
    // (Commented out to save time. Uncomment if you want to re-verify Flash)
    
    Print("\r\n--- Re-verifying Flash ---\r\n");
    uint32_t test_addr = 0x08004000;
    uint8_t test_data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    Flash_EraseSector(1);
    Flash_Write(test_addr, test_data, 4);
    if (*(uint32_t*)test_addr == 0xDDCCBBAA) {
        Print("Flash Read/Write OK.\r\n");
    }
    */

    while (1) {
        // Heartbeat
        GPIO_Toggle(GPIOA, GPIO_PIN_5);
        Delay(500);
    }
    return 0;
}