#include "uart.h"

static void uart2_gpio_init(void) {
    RCC->AHB1ENR |= (1U << 0);
    // PA2 (TX) -> AF7
    GPIOA->MODER   &= ~(3U << 4); GPIOA->MODER   |= (2U << 4);
    GPIOA->AFR[0]  &= ~(0xFU << 8); GPIOA->AFR[0]  |= (7U << 8);
    // PA3 (RX) -> AF7
    GPIOA->MODER   &= ~(3U << 6); GPIOA->MODER   |= (2U << 6);
    GPIOA->AFR[0]  &= ~(0xFU << 12); GPIOA->AFR[0] |= (7U << 12);
}

void UART2_Init(void) {
    // 1. Enable Clock
    RCC->APB1ENR |= (1U << 17);

    // 2. Pins
    uart2_gpio_init();

    // 3. Reset
    USART2->CR1 = 0;

    // 4. BAUD RATE CALCULATION (Turbo Mode)
    // System Clock: 180 MHz
    // APB1 Clock:   45 MHz (SysClk / 4)
    // Target Baud:  115200
    
    uint32_t apb1_clk = 45000000U; // <--- 45 MHz
    uint32_t baud = 115200U;       // <--- 115200
    
    uint32_t brr = (apb1_clk + (baud/2U)) / baud; 
    USART2->BRR = brr;

    // 5. Enable
    USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
}

void UART2_Write(int ch) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (uint8_t)ch;
}

int UART2_Read(void) {
    while (!(USART2->SR & USART_SR_RXNE));
    return (int)(USART2->DR & 0xFF);
}