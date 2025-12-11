# Define the source files
set(SOURCES
    main.c
    startup_stm32.c
    rcc.c
    my_allocator.c
)

# Create the Firmware Executable
add_executable(firmware.elf ${SOURCES})

# Linker Settings (Crucial!)
# We use the script we created in linker/stm32f446re.ld
target_link_options(firmware.elf PRIVATE
    -T${CMAKE_SOURCE_DIR}/linker/stm32f446re.ld
    -Wl,-Map=${CMAKE_BINARY_DIR}/firmware.map
    -Wl,--gc-sections
)

# Generate the .bin file (for Flashing)
add_custom_command(TARGET firmware.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary firmware.elf firmware.bin
    COMMENT "Generating firmware.bin"
)