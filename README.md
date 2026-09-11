# STM32F103 多功能循迹小车

**STM32F103 蓝牙小车：四路循迹、超声波避障、OLED。手机串口助手可发 `fwd 3` / `1,3`。源码在 [`firmware/`](firmware/)，不是只有 ZIP。**

Bluetooth-controlled STM32F103 robot car: variable-speed drive, line tracking,
ultrasonic obstacle avoidance, servo scan, reversing warning, OLED.

[![Firmware build](https://github.com/CAOShurong/Multi-function-tracking-car-based-on-STM32/actions/workflows/firmware.yml/badge.svg)](https://github.com/CAOShurong/Multi-function-tracking-car-based-on-STM32/actions/workflows/firmware.yml)
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
cmake --build build
```

Or open [`firmware/`](firmware/) in STM32CubeIDE (STM32F103C8Tx). Prebuilt ELF/HEX/BIN:
[Releases](https://github.com/CAOShurong/Multi-function-tracking-car-based-on-STM32/releases/latest).

The original [`Car_1.zip`](Car_1.zip) is a historical snapshot; `Debug/` build
outputs were not copied into source control.

<p align="center">
  <img src="./小车形象图.jpg" width="720" alt="Completed STM32 multifunction robot car">
</p>

## What is implemented

- two-motor direction and six-level speed control;
- five-position servo control;
- front and rear ultrasonic distance measurement;
- distance display on an SSD1315-compatible OLED;
- reversing-distance warning output on PC15;
- slow automatic obstacle avoidance with left/right scanning;
- four-sensor infrared line tracking, including cross intersections;
- Bluetooth commands as two raw bytes, a numeric line such as `1,3`, **or**
  a named line such as `fwd 3` / `w` / `qianjin` / `ting`;
- USART1 debug TX prints `front:12.3 rear:8.1` about five times a second
  (pipe into [termscope](https://github.com/CAOShurong/termscope));
- an AHT20 temperature/humidity driver that is included but not called by the
  current main loop.

## Build and flash

### STM32CubeIDE

1. Install STM32CubeIDE and the STM32CubeF1 firmware package. The project was
   generated against **STM32Cube FW_F1 V1.8.5**.
2. In STM32CubeIDE, choose **File → Open Projects from File System** and select
   the [`firmware/`](firmware/) directory.
3. Confirm the target is **STM32F103C8Tx**. The checked-in configuration uses an
   8 MHz external crystal and a 72 MHz system clock.
4. Build the `Car_1` project and flash it with an ST-Link or another supported
   programmer.
5. Connect the Bluetooth module to USART2. Send two raw command bytes, a
   numeric line such as `1,3`, or a named line such as `fwd 3` (Serial
   Bluetooth Terminal works).

### Headless CMake build

For a reviewable build without opening STM32CubeIDE, install CMake 3.20 or
newer, Ninja (or another CMake generator), and an `arm-none-eabi` GNU toolchain.
The original project used GNU Tools for STM32 12.3.rel1; hosted CI builds with
Ubuntu 24.04's Arm bare-metal package, and the repository is also checked with
Arm GNU Toolchain 15.2.rel1.

```sh
cmake -S . -B build -G Ninja \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_TOOLCHAIN_FILE=cmake/arm-none-eabi.cmake
cmake --build build --verbose
python tools/validate_firmware.py build/car-1.bin
```

The build produces `car-1.elf`, `car-1.hex`, `car-1.bin`, `car-1.map`, and a
disassembly listing. The validator checks image size, the initial stack
pointer, Thumb state, and that the reset handler lies inside the flash image.
CI also compiles a host-side protocol test that exhaustively checks all 65,536
possible two-byte Bluetooth commands, plus ASCII lines such as `1,3` and
named lines such as `fwd 3` / `ting`, before building the firmware.
Those are static build checks; they do not prove pin wiring, sensor polarity,
timing, motor behavior, or a successful flash on physical hardware.

### Download a CI-built image

Versioned firmware files are published on the
[Releases page](https://github.com/CAOShurong/Multi-function-tracking-car-based-on-STM32/releases).
Each release includes ELF, Intel HEX, raw BIN, and linker map files, plus
`SHA256SUMS`, the build toolchain versions, third-party notices, and a GitHub
build-provenance attestation. Verify a downloaded file with:

```sh
sha256sum --check SHA256SUMS
gh attestation verify car-1.bin \
  --repo CAOShurong/Multi-function-tracking-car-based-on-STM32
```

Use `car-1.hex` or `car-1.bin` only with an STM32F103C8Tx target and a suitable
programmer. A passing CI build is not a substitute for checking your board,
wiring, voltage levels, or motor power stage.

Motor power must not be sourced from an MCU GPIO pin. Use a motor driver and a
suitable external supply, join the grounds, and verify every sensor's voltage
before wiring it to a 3.3 V STM32 input. In particular, some ultrasonic modules
produce a 5 V echo signal and need level shifting.

## Bluetooth command protocol

USART2 is configured as **9600 baud, 8 data bits, no parity, 1 stop bit**.

Three encodings are accepted:

- **binary**: exactly two bytes `[action, value]`, as before;
- **numeric text**: a line `action,value` or `action value`, terminated by CR/LF.
  Example: `1,3` then Enter is forward at speed 3;
- **named text**: `fwd 3`, `w`, `stop`, `track`, `avoid`, or pinyin such as
  `qianjin` / `ting` / `xunji`. Drive words default to speed 3 if you omit
  the number. Case does not matter.

A phone serial terminal can send any of the text forms. Incomplete or
unknown lines are ignored.

Manual drive (`fwd` / `back` / `left` / `right`, actions 1–4) **expires 3
seconds** after the last complete UART command. If the Bluetooth link drops,
the motors stop. Repeat the drive line to keep moving, or send `track` /
`avoid` for unattended motion — those modes do not time out. `stop` and
`servo` are unchanged.

The receive callback publishes only complete commands. The main loop takes an
interrupt-protected snapshot and validates both bytes before changing motor
direction or PWM. An unknown action, an out-of-range value, or the
unimplemented fast-avoidance command stops the motors and centers the servo.

| action | names (English / WASD / 拼音) | value |
|---:|---|---|
| `0` | `stop` `halt` `ting` `tingzhi` | ignored |
| `1` | `fwd` `forward` `go` `w` `qian` `qianjin` | speed `0`–`5` (default 3) |
| `2` | `back` `backward` `rev` `s` `hou` `houtui` | speed `0`–`5` (default 3) |
| `3` | `left` `a` `zuo` | speed `0`–`5` (default 3) |
| `4` | `right` `d` `you` | speed `0`–`5` (default 3) |
| `5` | `servo` `duoji` | position `0`–`4` (default 2, center) |
| `6` | `avoid` `oa` `bizhang` | `0` stop, `1` slow avoidance (default 1); `2` is rejected until implemented |
| `7` | `track` `t` `xunji` | ignored |

`s` is WASD reverse, not stop — send `stop` or `ting` to halt.

The `action=6, value=2` path remains reserved, but its
`Auto_Obstacle_Avoidance_Fast()` implementation is currently empty. The command
therefore takes the safe-stop path instead of silently retaining an earlier
motor state.

## Confirmed peripheral map

This table is derived from [`Car_1.ioc`](firmware/Car_1.ioc) and the checked-in
source. Verify it against your own carrier board before applying power.

| function | STM32 pin / peripheral |
|---|---|
| Bluetooth serial | PA2 / PA3, USART2 TX/RX, 9600 baud |
| optional debug serial | PA9 / PA10, USART1 TX/RX, 115200 baud |
| OLED and AHT20 bus | PB10 / PB11, I2C2 SCL/SDA |
| motor PWM | PB6, TIM4 CH1 |
| servo PWM | PA6, TIM3 CH1 |
| left motor direction | PA4 / PA5 |
| right motor direction | PB14 / PB15 |
| rear ultrasonic trigger / echo | PC13 / PA8 |
| front ultrasonic trigger / echo | PC14 / PA11 |
| distance warning output | PC15 |
| four line sensors | PA15 / PB5 / PB7 / PB9 |

<p align="center">
  <img src="./各模块标注图.png" width="820" alt="Annotated module layout of the robot car">
</p>

## Repository map

```text
firmware/
├── Car_1.ioc             STM32CubeMX configuration
├── Core/                 application and generated initialization code
├── Drivers/              CMSIS and STM32F1 HAL sources with their licenses
└── .project/.cproject    STM32CubeIDE project metadata
Car_1.zip                 original project snapshot, including old build output
*.mp4                     three hardware demonstrations
*.jpg / *.png             build photographs and annotated module layout
```

The application modules are small enough to reuse independently: `motor.c`,
`distance.c`, `infrared.c`, `obstacle_avoidance.c`, `Servo.c`, `oled.c`, and
`aht20.c` are the best entry points.

## Demonstrations

- [Basic driving and control](./基本功能展示.mp4)
- [Infrared line tracking](./红外循迹功能展示.mp4)
- [Automatic obstacle avoidance](./自动避障功能展示.mp4)

## Known limitations

- fast obstacle avoidance is declared but not implemented;
- the two-byte protocol has no frame marker, checksum, or acknowledgement;
  manual drive (actions 1–4) now stops after 3 s without a new command, but
  `track` / `avoid` still run until another command arrives;
- the original author reports that line tracking does not pass sharp corners
  smoothly;
- the AHT20 driver is present but not integrated into the main loop;
- hosted CI compiles and links the firmware and validates the generated image,
  but no new physical-device flash test is claimed.

## 中文说明

这是一个基于 STM32F103C8Tx 的多功能小车完整工程。主要功能包括蓝牙调速与
方向控制、舵机五档转向、前后超声波测距、OLED 显示、倒车提醒、自动避障以及
四路红外循迹。过去源码只放在 ZIP 中；现在可以直接在 [`firmware/`](firmware/)
目录浏览，并用 STM32CubeIDE 导入构建。

蓝牙协议可以是两个原始字节 `[动作, 参数]`，也可以是手机串口助手发的文本
`1,3` 或 `fwd 3` / `w` / `qianjin` 加回车。`stop` 或 `ting` 停车，`track` 或
`xunji` 循迹，`avoid` 或 `bizhang` 避障。前进/后退/转向在 **3 秒内没有新指令**
时停车（蓝牙掉线保护）；循迹和避障不超时。USART1 会输出 `front:12.3 rear:8.1`
测距行。接线与完整命令表见上文。

如发现硬件组合、接线说明或代码方面的问题，请使用
[GitHub Issues](https://github.com/CAOShurong/Multi-function-tracking-car-based-on-STM32/issues)
提交可复现的信息。

## License status

Application source, photographs, and videos in this repository are MIT
([`LICENSE`](LICENSE)). The STM32 HAL and CMSIS trees keep their own files under
[`firmware/Drivers/`](firmware/Drivers/). The OLED driver also declares MIT in
its source header. See [third-party notices](THIRD_PARTY_NOTICES.md) for
component-by-component provenance.
