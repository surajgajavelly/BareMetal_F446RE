#include <stdint.h>

/* Defines -------------------------------------------------------------------*/
#define SRAM_START  0x20000000U
#define SRAM_SIZE   (128U * 1024U) // 128KB
#define SRAM_END    ((SRAM_START) + (SRAM_SIZE))
#define STACK_START SRAM_END

/* Externals -----------------------------------------------------------------*/
extern uint32_t _etext;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;
extern uint32_t _sidata;

extern void main(void);

/* --- FIX 1: Add External Declaration for SysTick --- */
extern void SysTick_Handler(void);

/* Function Prototypes -------------------------------------------------------*/
void Reset_Handler(void);
void Default_Handler(void);

/* Vector Table --------------------------------------------------------------*/
uint32_t vectors[] __attribute__((section(".isr_vector"))) = {
    STACK_START,                   /* 0x0000 0000 : Initial Stack Pointer */
    (uint32_t)Reset_Handler,       /* 0x0000 0004 : Reset Handler */
    (uint32_t)Default_Handler,     /* 0x0000 0008 : NMI Handler */
    (uint32_t)Default_Handler,     /* 0x0000 000C : Hard Fault Handler */
    (uint32_t)Default_Handler,     /* 0x0000 0010 : MPU Fault Handler */
    (uint32_t)Default_Handler,     /* 0x0000 0014 : Bus Fault Handler */
    (uint32_t)Default_Handler,     /* 0x0000 0018 : Usage Fault Handler */
    0,                             /* 0x0000 001C : Reserved */
    0,                             /* 0x0000 0020 : Reserved */
    0,                             /* 0x0000 0024 : Reserved */
    0,                             /* 0x0000 0028 : Reserved */
    (uint32_t)Default_Handler,     /* 0x0000 002C : SVCall Handler */
    (uint32_t)Default_Handler,     /* 0x0000 0030 : Debug Monitor Handler */
    0,                             /* 0x0000 0034 : Reserved */
    (uint32_t)Default_Handler,     /* 0x0000 0038 : PendSV Handler */
    
    /* --- FIX 2: Point to the Real Handler --- */
    (uint32_t)SysTick_Handler,     /* 0x0000 003C : SysTick Handler */
    
    /* Add peripheral interrupts here (UART, DMA, etc.) later */
};

/* Functions -----------------------------------------------------------------*/
void Reset_Handler(void) {
    /* 1. Copy .data section from FLASH to RAM */
    uint32_t size = (uint32_t)&_edata - (uint32_t)&_sdata;
    uint8_t *pDst = (uint8_t*)&_sdata;    // RAM start
    uint8_t *pSrc = (uint8_t*)&_sidata;   // Flash start

    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = *pSrc++;
    }

    /* 2. Init .bss section to zero in RAM */
    size = (uint32_t)&_ebss - (uint32_t)&_sbss;
    pDst = (uint8_t*)&_sbss;

    for (uint32_t i = 0; i < size; i++) {
        *pDst++ = 0;
    }

    /* 3. Call main() */
    main();
}

void Default_Handler(void) {
    while (1);
}