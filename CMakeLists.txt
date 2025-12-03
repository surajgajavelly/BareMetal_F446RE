cmake_minimum_required(VERSION 3.10)

# Project Name
project(BareMetal_F446RE C)

# --- Options ---
option(BUILD_TESTS "Build host-based unit tests" ON)

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
    message(STATUS "Build Type: HOST (Unit Testing)")
    add_definitions(-DHOST_TESTING)
endif()

# --- Include Paths ---
include_directories(include)

# --- Subdirectories ---
add_subdirectory(src)

# Only build tests if we are NOT cross-compiling
if(NOT CMAKE_CROSSCOMPILING AND BUILD_TESTS)
    enable_testing()
    # Check if Unity exists before adding
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/unity/CMakeLists.txt")
        add_subdirectory(vendor)
        add_subdirectory(tests)
    else()
        message(WARNING "Unity not found in vendor/unity. Run 'git submodule update --init' to fix.")
    endif()
endif()

# Only build demo firmware if we ARE cross-compiling
# if(CMAKE_CROSSCOMPILING)
#     add_subdirectory(demo)
# endif()