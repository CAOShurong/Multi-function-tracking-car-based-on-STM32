# Changelog

All notable repository changes are documented here. This project follows
[Semantic Versioning](https://semver.org/) for repository releases.

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

[0.1.0]: https://github.com/CAOShurong/Multi-function-tracking-car-based-on-STM32/releases/tag/v0.1.0
