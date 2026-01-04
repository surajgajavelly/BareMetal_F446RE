#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "rcc.h"
#include "gpio.h"

/* --- 1. Memory Map --- */
// USART2 is on APB1 Bus (Base: 0x4000 4400)
#define USART2_BASE  (PERIPH_BASE + 0x4400)

/* --- 2. Register Structure --- */
typedef struct {
    volatile uint32_t SR;    // Status Register (0x00)
    volatile uint32_t DR;    // Data Register (0x04)
    volatile uint32_t BRR;   // Baud Rate Register (0x08)
    volatile uint32_t CR1;   // Control Register 1 (0x0C)
    volatile uint32_t CR2;   // Control Register 2 (0x10)
    volatile uint32_t CR3;   // Control Register 3 (0x14)
    volatile uint32_t GTPR;  // Guard Time & Prescaler (0x18)
} USART_TypeDef;

/* --- 3. Pointer Definition --- */
#define USART2 ((USART_TypeDef *) USART2_BASE)

/* --- 4. Bit Definitions --- */
// CR1 (Control Register 1)
#define USART_CR1_UE     (1U << 13) // USART Enable
#define USART_CR1_M      (1U << 12) // Word Length (0=8 bits, 1=9 bits)
#define USART_CR1_PCE    (1U << 10) // Parity Control Enable
#define USART_CR1_TE     (1U << 3)  // Transmitter Enable
#define USART_CR1_RE     (1U << 2)  // Receiver Enable
#define USART_CR1_RXNEIE (1U << 5)  // RX Not Empty Interrupt Enable

// SR (Status Register)
#define USART_SR_TXE     (1U << 7)  // Transmit Data Register Empty
#define USART_SR_RXNE    (1U << 5)  // Read Data Register Not Empty

/* --- 5. Function Prototypes --- */
void UART2_Init(void);
void UART2_Write(int ch);
int UART2_Read(void);

#endif // UART_H