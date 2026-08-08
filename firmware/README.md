# Firmware source

This directory contains the STM32CubeIDE project extracted from the repository's
original `Car_1.zip` snapshot so the code can be reviewed without downloading
and unpacking an archive.

The extraction intentionally excludes:

- `Debug/`, which contains compiled objects, dependency files, maps, listings,
  and the old ELF image;
- `Car_1 Debug.launch`, which is local IDE launch state.

The checked-in source inventory matches the 116 non-build files in the archive:
CubeIDE metadata, `Car_1.ioc`, `Core/`, `Drivers/`, and the linker script. The
original archive remains at the repository root for historical comparison.

Open this directory as an existing project in STM32CubeIDE. It targets
STM32F103C8Tx and records STM32Cube FW_F1 V1.8.5 in `Car_1.ioc`.
