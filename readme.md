# OPEN LIBRARIES FOR AUTONOMOUS NAVIGATION OF A SIX-WHEELED ROVER PROTOTYPE OVER MODERATELY ROUGH UNFAMILIAR TERRAIN WITH VISUAL NAVIGATION TARGET RECOGNITION

## Open Library for Low-Level Rover Motion Control ([TEAM]-DC)
## [TEAM]-DC-STM32

This is a complete project for the **STM32CubeIDE** environment. It is based on a software library created as part of a research program.

## Licensing

This project is distributed under dual licensing:

- MIT License (see LICENSE file in the repository)
- Open Simple License (Russian Open License) (see GENERAL_LICENSE-RF.md file in the repository)

You may use this project under the terms of either license.

##

The library can be used on autonomous mobile platforms on land and in space for geological and ecological exploration, study and development of hard-to-reach territories, monitoring of energy equipment conditions (including nuclear power plants) without taking it out of operation under conditions of poor communication and lack of current terrain maps.

The project provides:
*   Low-level rover motion control (conversion of high-level navigation commands into actuator signals).
*   Collection and transmission of feedback from sensors to the navigation system and computer vision for real-time route optimization.

<a id="minsystemrequirements"></a>
**Minimum System Requirements:**
*  Operating System: Ubuntu 22.04.
*  Processor: x86_64 or ARM64 (e.g., Nvidia Jetson).
*  RAM: 4 GB minimum, 8 GB recommended.
*  GPU: NVIDIA GPU with CUDA 11.0+ support (optional, but recommended).
*  Disk Space: 2 GB for library and models.

**Low-Level Hardware Requirements**
*  Microcontroller: STM32F407DISC.
*  Abstraction Level: HAL for STM32 F4 version.
*  Programming Environment: STM32CubeIDE version 1.16.1 or higher with default settings.

**Recommended Requirements (Tested Configuration)**
* Platform: Nvidia Jetson Orin NX Super.
* RAM: 16 GB.
*  Microcontroller: STM32F407DISC.
*  Abstraction Level: HAL for STM32 F4 version.
*  Programming Environment: STM32CubeIDE version 1.16.1 or higher with default settings.

**Key Module Files:**
*   `Core/Inc/motion_lib.h`
*   `Core/Src/motion_lib.c`

## 📖 Documentation

Full technical documentation is available in the [docs/](./docs/) directory:

*   [📄 Application Description](./docs/Application%20Description-MC-merged.pdf) — Application description.
*   [📄 Programmer's Guide](./docs/Programmer%27s%20Guide-MC-merged.pdf) — Programmer's guide for the motion control library.

## IMPORTANT NOTES

For the rover low-level motion control library to work, proper configuration and connection of the hardware-software interface and correct operation of the rover's radio electronics are required.\
**The choice of a specific hardware interface for the software library remains at the discretion of the user. When changing the microcontroller series, the library's operation cannot be guaranteed due to significant differences in HAL functions and architectures of chips in different STM32 generations.**
