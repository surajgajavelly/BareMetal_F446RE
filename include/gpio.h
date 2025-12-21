#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

// --- Memory Map Constants ---
#define GPIOA_BASE      0x40020000UL
#define GPIOC_BASE      0x40020800UL

// --- Register Definition Structure ---
typedef struct {
    volatile uint32_t MODER;    // Mode register
    volatile uint32_t OTYPER;   // Output type register
    volatile uint32_t OSPEEDR;  // Output speed register
    volatile uint32_t PUPDR;    // Pull-up/pull-down register
    volatile uint32_t IDR;      // Input data register
    volatile uint32_t ODR;      // Output data register
    volatile uint32_t BSRR;     // Bit set/reset register
    volatile uint32_t LCKR;     // Configuration lock register
    volatile uint32_t AFR[2];   // Alternate function registers (Low & High)
} GPIO_TypeDef;

// --- Pointer Definitions ---
#define GPIOA   ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOC   ((GPIO_TypeDef *) GPIOC_BASE)

// --- PIN Definitions ---
#define GPIO_PIN_0      (1U << 0)
#define GPIO_PIN_1      (1U << 1)
#define GPIO_PIN_2      (1U << 2)
#define GPIO_PIN_3      (1U << 3)
#define GPIO_PIN_4      (1U << 4)
#define GPIO_PIN_5      (1U << 5)
#define GPIO_PIN_6      (1U << 6)
#define GPIO_PIN_7      (1U << 7)
#define GPIO_PIN_8      (1U << 8)
#define GPIO_PIN_9      (1U << 9)
#define GPIO_PIN_10     (1U << 10)
#define GPIO_PIN_11     (1U << 11)
#define GPIO_PIN_12     (1U << 12)
#define GPIO_PIN_13     (1U << 13)
#define GPIO_PIN_14     (1U << 14)
#define GPIO_PIN_15     (1U << 15)

// --- Logic Levels ---
#define GPIO_PIN_SET    1
#define GPIO_PIN_RESET  0

// --- Function Prototypes ---
void GPIO_Init(void); // Ideally, pass arguments here like (GPIOx, Pin, Mode)
void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t pin, uint8_t state);
void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t pin);
uint8_t GPIO_Read(GPIO_TypeDef *GPIOx, uint16_t pin);

#endif // GPIO_H