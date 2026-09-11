# Changelog

All notable repository changes are documented here. This project follows
[Semantic Versioning](https://semver.org/) for repository releases.

## [Unreleased]

### Added

- Manual Bluetooth drive (actions 1–4) expires 3 s after the last complete
  UART command, so a dropped SPP link stops the motors. `track` and `avoid`
  do not time out. Host-tested; not newly flashed to hardware.
- Named Bluetooth lines such as `fwd 3`, WASD (`w`/`a`/`s`/`d`), and pinyin
  (`qianjin`, `ting`, `xunji`, `bizhang`) in addition to `1,3` and two raw
  bytes. Drive words default to speed 3; `stop`/`ting` halt. Host tests cover
  the aliases; this has not been newly flashed to hardware.
- USART1 (~5 Hz) prints `front:… rear:…` labelled distances for a serial
  plotter. Timeout is 20 ms so a disconnected debug UART does not stall the
  drive loop.
- Accept Bluetooth text lines such as `1,3` (CR/LF) in addition to the
  original two raw bytes, so Serial Bluetooth Terminal can drive the car.
  Non-numeric lines are ignored; binary two-byte commands still work.
  Printable ASCII is never treated as a two-byte binary frame.

### Changed

- Put a Chinese one-line pitch, CMake command, and MIT badge on the first screen.
- State that application files are MIT; HAL/CMSIS stay under their vendor licenses.
- Pin every third-party GitHub Action to an immutable commit while retaining
  version comments for reviewability.
- Add weekly Dependabot checks for GitHub Actions updates; automatic security
  fixes are also enabled in the repository settings.

## [0.1.1] - 2026-08-11

### Fixed

- Publish only complete two-byte UART commands to the control loop and take an
  interrupt-protected snapshot before dispatch.
- Reject unknown actions, out-of-range values, and the unimplemented fast
  obstacle-avoidance command before they can change motor direction while
  retaining an earlier PWM value.
- Pass speed levels into motor functions explicitly instead of rereading a
  UART-owned global buffer.

### Verification boundary

- A host-side test exhaustively checks all 65,536 command byte pairs, and the
  cross-compiled firmware is still validated for image and memory shape.
- No new physical-device flash, motor, sensor, or Bluetooth-link test is
  claimed for this release.

## [0.1.0] - 2026-08-11

### Added

- Browsable STM32CubeIDE source under `firmware/` while retaining the original
  `Car_1.zip` snapshot.
- A headless CMake/Ninja build for STM32F103C8Tx firmware.
- GitHub Actions compilation, image-shape validation, review artifacts, release
  assets, SHA-256 manifests, and build-provenance attestations.
- Pin maps, Bluetooth protocol documentation, safety notes, known limitations,
  and component-level third-party notices.

### Fixed

- Included `distance.h` in `obstacle_avoidance.c`, resolving an implicit
  declaration that prevented a modern Arm GNU Toolchain build.

### Verification boundary

The release is compiled, linked, and statically checked in CI. It has not been
newly flashed to physical hardware during this release process, so electrical,
sensor, motor, timing, and board-specific behavior remains unverified.

[Unreleased]: https://github.com/CAOShurong/Multi-function-tracking-car-based-on-STM32/compare/v0.1.1...HEAD
[0.1.1]: https://github.com/CAOShurong/Multi-function-tracking-car-based-on-STM32/compare/v0.1.0...v0.1.1
[0.1.0]: https://github.com/CAOShurong/Multi-function-tracking-car-based-on-STM32/releases/tag/v0.1.0
