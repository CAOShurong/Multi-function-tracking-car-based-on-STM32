# STM32F103 multifunction robot car

An STM32CubeIDE project for a Bluetooth-controlled robot car with variable-speed
drive, front and rear ultrasonic ranging, obstacle avoidance, four-sensor line
tracking, servo scanning, a reversing-distance warning output, and an OLED
status display.

The complete, browsable firmware now lives in [`firmware/`](firmware/). The
original [`Car_1.zip`](Car_1.zip) is retained as a historical snapshot; build
outputs from its `Debug/` directory were deliberately not copied into source
control.

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
- two-byte commands received over a 9600-baud Bluetooth serial link;
- an AHT20 temperature/humidity driver that is included but not called by the
  current main loop.

## Build and flash

1. Install STM32CubeIDE and the STM32CubeF1 firmware package. The project was
   generated against **STM32Cube FW_F1 V1.8.5**.
2. In STM32CubeIDE, choose **File → Open Projects from File System** and select
   the [`firmware/`](firmware/) directory.
3. Confirm the target is **STM32F103C8Tx**. The checked-in configuration uses an
   8 MHz external crystal and a 72 MHz system clock.
4. Build the `Car_1` project and flash it with an ST-Link or another supported
   programmer.
5. Connect the Bluetooth module to USART2 and send the two raw command bytes
   described below. ASCII text such as `"1,3"` is not the protocol.

Motor power must not be sourced from an MCU GPIO pin. Use a motor driver and a
suitable external supply, join the grounds, and verify every sensor's voltage
before wiring it to a 3.3 V STM32 input. In particular, some ultrasonic modules
produce a 5 V echo signal and need level shifting.

## Bluetooth command protocol

USART2 is configured as **9600 baud, 8 data bits, no parity, 1 stop bit**. Every
command is exactly two bytes: `[action, value]`.

| action byte | behavior | value byte |
|---:|---|---|
| `0` | stop and center servo | ignored |
| `1` | drive forward | speed level `0`–`5` |
| `2` | drive backward | speed level `0`–`5` |
| `3` | turn left | turn/speed level `0`–`5` |
| `4` | turn right | turn/speed level `0`–`5` |
| `5` | move servo | position `0`–`4` |
| `6` | obstacle mode | `0` stop, `1` slow avoidance, `2` fast avoidance |
| `7` | enter line-tracking mode | ignored |

The `action=6, value=2` path is reserved, but its
`Auto_Obstacle_Avoidance_Fast()` implementation is currently empty. This is a
known limitation, not a completed feature.

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
- the original author reports that line tracking does not pass sharp corners
  smoothly;
- the AHT20 driver is present but not integrated into the main loop;
- the repository has not been rebuilt on hosted CI because STM32CubeIDE is not
  available there; the source extraction and local inventory were verified,
  but this cleanup does not claim a new hardware flash test.

## 中文说明

这是一个基于 STM32F103C8Tx 的多功能小车完整工程。主要功能包括蓝牙调速与
方向控制、舵机五档转向、前后超声波测距、OLED 显示、倒车提醒、自动避障以及
四路红外循迹。过去源码只放在 ZIP 中；现在可以直接在 [`firmware/`](firmware/)
目录浏览，并用 STM32CubeIDE 导入构建。

蓝牙协议不是字符串，而是两个原始字节 `[动作, 参数]`。例如 `{1, 3}` 表示以
第 3 档前进，`{5, 2}` 表示舵机回到中间位置。接线与完整命令表见上文。

如发现硬件组合、接线说明或代码方面的问题，请使用
[GitHub Issues](https://github.com/CAOShurong/Multi-function-tracking-car-based-on-STM32/issues)
提交可复现的信息。

## License status

The STM32 HAL and CMSIS directories retain their own license files under
[`firmware/Drivers/`](firmware/Drivers/). A repository-wide license for the
application-specific source has not yet been declared, so do not assume rights
beyond those component licenses. Choosing that license requires an explicit
maintainer decision.
