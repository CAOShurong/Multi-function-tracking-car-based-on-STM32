# Third-party notices and provenance

This repository combines application-specific robot-car logic, generated
STM32Cube project files, ST and Arm platform code, and an external OLED driver.
The licenses are not uniform.

| Component | Location | Declared terms |
|---|---|---|
| STM32F1 HAL | `firmware/Drivers/STM32F1xx_HAL_Driver/` | The bundled `LICENSE.txt` says BSD-3-Clause applies when the component is received without wider package terms. |
| CMSIS Core | `firmware/Drivers/CMSIS/` | Apache License 2.0, preserved in the bundled `LICENSE.txt`. |
| STM32F1 CMSIS device files | `firmware/Drivers/CMSIS/Device/ST/STM32F1xx/` | The bundled `LICENSE.txt` identifies Apache-2.0. |
| STM32Cube-generated startup, initialization, and support files | `firmware/Core/`, `firmware/Core/Startup/`, and the linker script | File headers identify STMicroelectronics and refer to the STM32Cube package terms. The project declares STM32Cube FW_F1 v1.8.5; its [component license table](https://github.com/STMicroelectronics/STM32CubeF1/blob/v1.8.5/License.md) is the upstream provenance record. |
| Baud Dance / keysking OLED and font driver | `firmware/Core/Src/oled.c`, `firmware/Core/Src/font.c`, and matching headers | The source header declares "MIT License" and identifies the Baud Dance/keysking tutorial driver. A separate copyright notice or complete license file was not present in the imported project; this incomplete provenance is retained as a known licensing gap rather than silently replaced. |

No repository-wide license has been declared for the application-specific
robot-car source, photographs, or videos. The component notices above do not
grant rights to otherwise unlicensed material.
