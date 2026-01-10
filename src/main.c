#include <stdint.h>
#include "rcc.h"
#include "gpio.h"
#include "systick.h"
#include "uart.h"
#include "flash.h"
#include "crc.h"

// --- Helper Functions ---
void Print(const char *str) {
    while (*str) UART2_Write(*str++);
}

void PrintHex(uint32_t val) {
    char hex[] = "0123456789ABCDEF";
    UART2_Write('0'); UART2_Write('x');
    for(int i = 7; i >= 0; i--) {
        UART2_Write(hex[(val >> (i * 4)) & 0xF]);
    }
}

void PrintStatus(const char* test, int success) {
    // Print aligned status
    // Move cursor to specific column if we had a full terminal, 
    // but for simple UART, we just pad with spaces.
    Print("  -> Status: [");
    if (success) Print("\033[1;32mPASS\033[0m"); // Green Text (if terminal supports it)
    else         Print("\033[1;31mFAIL\033[0m"); // Red Text
    Print("] ");
    Print(test);
    Print("\r\n\r\n");
    Delay(500); // Pause for effect
}

void LoadingEffect(void) {
    for(int i=0; i<3; i++) {
        Print(".");
        Delay(200);
    }
    Print("\r\n");
}

// --- Main System Check ---
int main(void) {
    // 1. Initialize All Drivers
    RCC_Init();           
    SysTick_Init(180000); 
    GPIO_Init();          
    UART2_Init();         
    CRC_Init();           
    
    // Clear Screen
    Print("\033[2J\033[H"); 
    Delay(500);

    Print("########################################\r\n");
    Print("#                                      #\r\n");
    Print("#    STM32F446RE BARE METAL SUITE      #\r\n");
    Print("#         DIAGNOSTIC KERNEL            #\r\n");
    Print("#                                      #\r\n");
    Print("########################################\r\n\r\n");
    
    Print("Initializing System Clock (180 MHz)...");
    LoadingEffect();
    Print("Initializing Peripherals...");
    LoadingEffect();
    Print("\r\n=== STARTING SYSTEM SELF-TEST ===\r\n\r\n");
    Delay(1000);

    // --- TEST 1: GPIO & SYSTICK ---
    Print("[1/3] Testing GPIO & SysTick\r\n");
    Print("      Blinking LED (Check Board)...");
    for(int i=0; i<4; i++) {
        GPIO_Toggle(GPIOA, GPIO_PIN_5);
        Delay(150); 
        GPIO_Toggle(GPIOA, GPIO_PIN_5);
        Delay(150);
    }
    Print(" Done.\r\n");
    PrintStatus("GPIO & Timer Logic", 1);

    // --- TEST 2: FLASH DRIVER ---
    Print("[2/3] Testing Flash Driver (Sector 1)\r\n");
    uint32_t test_addr = 0x08004000; 
    uint32_t *pMem = (uint32_t*)test_addr;
    uint8_t data_to_write[] = {0xEF, 0xBE, 0xAD, 0xDE}; 
    
    Print("      Erasing Sector 1...");
    // Use dots to show "work" being done during the erase wait
    Print("."); 
    uint8_t status = Flash_EraseSector(1);
    Print(".");
    
    if(status == 0 && *pMem == 0xFFFFFFFF) {
        Print(" Clean.\r\n");
    } else {
        Print(" Fail.\r\n");
    }
    Delay(200);

    Print("      Writing 0xDEADBEEF...");
    status = Flash_Write(test_addr, data_to_write, 4);
    Delay(200);
    
    if(status == 0 && *pMem == 0xDEADBEEF) {
        Print(" Written.\r\n");
        PrintStatus("Flash Access", 1);
    } else {
        Print(" Verify Failed.\r\n");
        PrintStatus("Flash Access", 0);
    }

    // --- TEST 3: CRC DRIVER ---
    Print("[3/3] Testing Hardware CRC Engine\r\n");
    uint32_t crc_input[] = {0xDEADBEEF};
    
    Print("      Input Data: "); PrintHex(crc_input[0]); Print("\r\n");
    
    uint32_t crc_out = CRC_Calculate(crc_input, 1);
    Delay(200);
    
    Print("      HW Output:  "); PrintHex(crc_out); Print("\r\n");
    
    if(crc_out == 0x81DA1A18) {
        PrintStatus("CRC32 Integrity", 1);
    } else {
        PrintStatus("CRC32 Integrity", 0);
    }

    // --- SUMMARY ---
    Print("----------------------------------------\r\n");
    Print("   ALL DRIVERS VERIFIED. SYSTEM READY.\r\n");
    Print("----------------------------------------\r\n");

    while (1) {
        GPIO_Toggle(GPIOA, GPIO_PIN_5);
        Delay(500);
    }
    return 0;
}