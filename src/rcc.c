#include "rcc.h"
#include "flash.h"

void RCC_Init(void) {
    // 1. Enable HSE (High Speed External) Clock
    RCC->CR |= RCC_CR_HSEON;
    while (!(RCC->CR & RCC_CR_HSERDY)); // Wait for Crystal to start

    // 2. Enable Power Controller and Set Voltage Scale 1
    RCC->APB1ENR |= (1U << 28); // Enable PWREN
    PWR->CR |= PWR_CR_VOS_SCALE1; // High Performance Mode

    // 3. Configure Flash Latency (5 Wait States for 180 MHz)
    FLASH->ACR = FLASH_ACR_LATENCY_5WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

    // 4. Configure PLL (Phase Locked Loop)
    // Formula: (HSE / M) * N / P = 180 MHz
    // Input (HSE) = 8 MHz
    // M = 4   -> 8/4 = 2 MHz
    // N = 180 -> 2*180 = 360 MHz (VCO)
    // P = 2   -> 360/2 = 180 MHz (SYSCLK)
    // Q = 7   -> (USB requires 48 MHz, we'll deal with this later)
    
    RCC->PLLCFGR = (4U << 0) |           // PLLM
                   (180U << 6) |         // PLLN
                   (0U << 16) |          // PLLP (00 = /2)
                   RCC_PLLCFGR_PLLSRC_HSE | 
                   (7U << 24);           // PLLQ

    // 5. Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    while (!(RCC->CR & RCC_CR_PLLRDY)); // Wait for PLL to lock

    // 6. Configure Bus Prescalers
    // AHB  = 180 MHz (Div 1)
    // APB1 = 45 MHz  (Div 4)
    // APB2 = 90 MHz  (Div 2)
    RCC->CFGR |= (5U << 10) | (4U << 13); // PPRE1=DIV4, PPRE2=DIV2

    // 7. Select PLL as System Clock
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL); // Wait for switch
}