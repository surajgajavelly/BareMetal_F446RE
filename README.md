# BareMetal_F446RE: STM32F4 Driver Suite

![Build Status](https://img.shields.io/badge/build-passing-brightgreen)
![Platform](https://img.shields.io/badge/platform-STM32F446RE-blue)
![License](https://img.shields.io/badge/license-MIT-green)

This is a complete **Bare-Metal Driver Suite** for the STM32F446RE microcontroller, written entirely from scratch in C.

It bypasses vendor libraries (HAL/LL) to interact directly with hardware registers, demonstrating a deep understanding of computer architecture and embedded systems.

## 🚀 Key Features

* **No HAL / No Standard Peripheral Lib:** Every driver is written by reading the RM0390 Reference Manual and mapping memory addresses manually.
* **Register-Mapped Architecture:** Uses custom C structures to map peripheral registers (e.g., `RCC->CR`), mimicking the hardware layout exactly.
* **High Performance:** System clock manually configured to **180 MHz** (Maximum PLL speed) with correct Flash wait states and Voltage Scaling.
* **Professional Toolchain:** Built with **CMake**, **Docker**, and **GCC-ARM**, moving away from proprietary IDEs.
* **Unit Testing:** Integrated with **Unity** to test driver logic on the host machine before deploying to hardware.

## 🛠️ Technology Stack

| Category | Tool/Standard |
| :--- | :--- |
| **Hardware** | STM32F446RE (Nucleo-64) |
| **Language** | C (C11) |
| **Build System** | CMake |
| **Compiler** | `arm-none-eabi-gcc` |
| **Environment** | Docker (Ubuntu 22.04) |
| **Testing** | Unity Test Framework |
| **Style** | clang-format (LLVM) |

---

## Project Structure

```text
.
├── .gitignore
├── .clang-format
├── CMakeLists.txt       # Root Build Script
├── Dockerfile           # Reproducible Environment
├── Doxyfile             # Documentation Config
├── README.md
├── include/             # Hardware Register Maps
│   └── rcc.h            # RCC Register Definitions
├── src/                 # Driver Implementations
│   ├── main.c           # Entry Point
│   ├── rcc.c            # Clock Configuration Logic
│   └── startup_stm32.c  # Vector Table & Reset Handler
├── linker/
│   └── stm32f446re.ld   # Custom Memory Map
└── vendor/
    └── unity/           # Testing Framework
```
---

## How to Build

This project uses Docker to ensure the build works on any machine (Windows/Linux/Mac) without installing tools locally.

### Docker Build (Recommended)

This is the easiest and most reliable way to build, as it matches the CI environment.

1.  **Build the Docker image (One time setup):**
    ```bash
    docker build -t baremetal-dev .
    ```
2.  **Compile the Firmware:**
    ```bash
    docker run --rm -v $(pwd):/project baremetal-dev \
    sh -c "rm -rf build_target && \
           cmake -B build_target -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake && \
           make -C build_target"
    ```
---
## 🛠️ Technology Stack & Workflow

| Driver | Status | Description |
| :--- | :--- | :--- |
| RCC | ✅ Done | System Clock (PLL) config to 180 MHz. |
| GPIO | 🚧 Planned | Input/Output control. |
| UART | 🚧 Planned | Serial Console & CLI. |
| SysTick| 🚧 Planned | Millisecond delays. |

---
