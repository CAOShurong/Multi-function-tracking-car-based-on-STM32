# Changelog

All notable repository changes are documented here. This project follows
[Semantic Versioning](https://semver.org/) for repository releases.

## [Unreleased]

### Changed

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
