# --- Source Files ---
set(SOURCES
    main.c
    startup_stm32.c
    rcc.c
    gpio.c
    my_allocator.c
    systick.c
    uart.c
)

# --- Create Executable ---
add_executable(firmware.elf ${SOURCES})

# --- Compile Definitions ---
# Force the allocator to use the static array (Bare Metal mode)
target_compile_definitions(firmware.elf PRIVATE 
    HEAP_BACKEND=1
)

# --- Linker Settings ---
target_link_options(firmware.elf PRIVATE
    -T${CMAKE_SOURCE_DIR}/linker/stm32f446re.ld
    -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map
    -Wl,--gc-sections
    ##--specs=nosys.specs 
)

# --- Post-Build: Generate Binary ---
# This converts the ELF (debuggable) into BIN (flashable)
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary firmware.elf firmware.bin
    COMMENT "Generating firmware.bin"
)