#include "gpio.h"
#include "rcc.h" 

void GPIO_Init(void) {
    // 1. Enable Clock for GPIOA (Bit 0) and GPIOC (Bit 2)
    // We use the RCC struct defined in rcc.h to avoid redefinition errors
    RCC->AHB1ENR |= (1U << 0) | (1U << 2);

    // 2. Configure PA5 (LED) as Output
    // MODER: 00=Input, 01=Output, 10=Alt, 11=Analog
    // We want Bit 10=1, Bit 11=0 for Pin 5
    GPIOA->MODER &= ~(3U << (5 * 2)); // Clear bits
    GPIOA->MODER |=  (1U << (5 * 2)); // Set to Output (01)

    // 3. Configure PC13 (Button) as Input
    // MODER: 00=Input (Reset State)
    GPIOC->MODER &= ~(3U << (13 * 2)); 
    
    // 4. Enable Pull-Up for Button (PC13)
    // PUPDR: 00=None, 01=PU, 10=PD
    GPIOC->PUPDR &= ~(3U << (13 * 2));
    GPIOC->PUPDR |=  (1U << (13 * 2)); // Pull-Up
}

void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t pin, uint8_t state) {
    if (state == GPIO_PIN_SET) {
        // Write to the lower 16 bits of BSRR to SET the pin High
        GPIOx->BSRR = pin;
    } else {
        // Write to the upper 16 bits of BSRR to RESET the pin Low
        GPIOx->BSRR = (uint32_t)pin << 16;
    }
}

void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t pin) {
    // XOR the Output Data Register to flip the bit
    GPIOx->ODR ^= pin;
}

uint8_t GPIO_Read(GPIO_TypeDef *GPIOx, uint16_t pin) {
    // Read IDR, mask the specific pin
    return (GPIOx->IDR & pin) ? GPIO_PIN_SET : GPIO_PIN_RESET;
}