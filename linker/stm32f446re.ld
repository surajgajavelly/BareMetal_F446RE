/*
 * @file      stm32f446re.ld
 * @brief     Linker Script for STM32F446RE (Nucleo-64).
 * @details   Describes the memory layout of the device and how sections
 * (.text, .data, .bss) are mapped to Flash and RAM.
 *
 * **Memory Map Strategy:**
 * - FLASH (512KB): Holds Code (.text) and Read-Only Data (.rodata).
 * - RAM   (128KB): Holds Global Variables (.data, .bss) and Stack.
 * - CCM   (64KB):  Holds the Heap (Custom Allocation) for high-speed access.
 *
 * @author    Gajavelly Sai Suraj
 * @date      2026-01-10
 * @copyright MIT License
 */

/* Entry Point: The first instruction to execute (defined in startup_stm32.c) */
ENTRY(Reset_Handler)

/* Highest address of the user mode stack */
/* Stack grows downwards from the end of RAM */
_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Minimum size requirements for Heap and Stack checks */
_Min_Heap_Size = 0x200;      /* 512 Bytes */
_Min_Stack_Size = 0x400;     /* 1024 Bytes */

/* * 1. Physical Memory Definitions 
 * Based on STM32F446RE Reference Manual
 */
MEMORY
{
    /* Flash: Non-volatile memory for code (RX = Read/Execute) */
    FLASH (rx)      : ORIGIN = 0x08000000, LENGTH = 512K
    
    /* SRAM1 + SRAM2: Main generic RAM (XRW = Execute/Read/Write) */
    /* DMA *can* access this region. */
    RAM (xrw)       : ORIGIN = 0x20000000, LENGTH = 128K
    
    /* CCM (Core Coupled Memory): High-speed RAM connected directly to CPU D-Bus. */
    /* DMA *cannot* access this region. Perfect for Heap/Stack or math tables. */
    CCMRAM (xrw)    : ORIGIN = 0x10000000, LENGTH = 64K
}

/* * 2. Output Sections 
 * Defines how input object files are mapped to memory regions.
 */
SECTIONS
{
  /* --- Section: Interrupt Vector Table --- */
  /* Must be placed at the very start of Flash (0x08000000) */
  .isr_vector :
  {
    . = ALIGN(4);
    KEEP(*(.isr_vector)) /* Keep this section even if unused */
    . = ALIGN(4);
  } >FLASH

  /* --- Section: Program Code (.text) --- */
  .text :
  {
    . = ALIGN(4);
    *(.text)           /* Main code */
    *(.text*)          /* Sub-sections of code */
    *(.glue_7)         /* Glue ARM to Thumb code */
    *(.glue_7t)        /* Glue Thumb to ARM code */
    *(.eh_frame)

    KEEP (*(.init))
    KEEP (*(.fini))

    . = ALIGN(4);
    _etext = .;        /* Global Symbol: End of Code */
  } >FLASH

  /* --- Section: Read-Only Data (.rodata) --- */
  /* Constants, strings, const structs */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } >FLASH

  /* --- Section: Initialized Data (.data) --- */
  /* Critical: These variables have values (int x = 5). 
   * They must be saved in FLASH (LMA) but accessed in RAM (VMA). 
   * The 'AT> FLASH' command tells the linker to store the initialization values in Flash.
   */
  _sidata = LOADADDR(.data); /* Get load address in Flash */

  .data :
  {
    . = ALIGN(4);
    _sdata = .;        /* Global Symbol: Start of RAM Data */
    *(.data)
    *(.data*)

    . = ALIGN(4);
    _edata = .;        /* Global Symbol: End of RAM Data */
  } >RAM AT> FLASH

  /* --- Section: Uninitialized Data (.bss) --- */
  /* Global variables without values (int y). 
   * Startup code zeros this area.
   */
  .bss :
  {
    . = ALIGN(4);
    _sbss = .;         /* Global Symbol: Start of BSS */
    __bss_start__ = _sbss;
    *(.bss)
    *(.bss*)
    *(COMMON)

    . = ALIGN(4);
    _ebss = .;         /* Global Symbol: End of BSS */
    __bss_end__ = _ebss;
  } >RAM

  /* --- Section: Custom Heap (Mapped to CCM RAM) --- */
  /* NOLOAD: Tells the startup code NOT to zero/init this section. 
   * We manage this memory manually in 'my_allocator.c'.
   */
  .my_heap (NOLOAD) :
  {
    . = ALIGN(8);
    _sheap = .;        /* Global Symbol: Heap Start */
    *(.my_heap)        
    *(.my_heap*)
    . = ALIGN(8);
    _eheap = .;        /* Global Symbol: Heap End */
  } >CCMRAM            /* <--- Forced into fast CCM RAM */

  /* --- Section: Stack Safety Check --- */
  /* Ensures there is enough RAM left for the Stack after global variables */
  ._user_heap_stack :
  {
    . = ALIGN(8);
    PROVIDE ( end = . );
    PROVIDE ( _end = . );
    . = . + _Min_Heap_Size;
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM
}