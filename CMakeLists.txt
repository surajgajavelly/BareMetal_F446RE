cmake_minimum_required(VERSION 3.10)

# Project Name
project(BareMetal_F446RE C)

# --- Standard C11 ---
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Strict compilation flags (Errors on warnings)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic -Werror -g")

# --- Detect Build Type (Host vs. Target) ---
if(CMAKE_CROSSCOMPILING)
    message(STATUS "Build Type: TARGET (STM32F446RE)")
    add_definitions(-DSTM32F446xx)
else()
    message(STATUS "Build Type: HOST (Not Supported for this project)")
endif()

# --- Include Paths ---
include_directories(include)

# --- Subdirectories ---
add_subdirectory(src)