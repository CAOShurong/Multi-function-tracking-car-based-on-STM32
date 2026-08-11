#!/usr/bin/env python3
"""Validate the minimum static shape of an STM32F103C8 firmware image."""

from __future__ import annotations

import argparse
import struct
import sys
from pathlib import Path

FLASH_START = 0x08000000
FLASH_SIZE = 64 * 1024
SRAM_START = 0x20000000
SRAM_END = 0x20005000


def validate_image(data: bytes) -> list[str]:
    errors: list[str] = []
    if len(data) < 8:
        return ["image is shorter than the initial stack pointer and reset vector"]
    if len(data) > FLASH_SIZE:
        errors.append(f"image is {len(data)} bytes; STM32F103C8 flash is {FLASH_SIZE} bytes")

    stack_pointer, reset_vector = struct.unpack_from("<II", data)
    if not (SRAM_START < stack_pointer <= SRAM_END):
        errors.append(f"initial stack pointer 0x{stack_pointer:08x} is outside 20 KiB SRAM")
    if stack_pointer % 8:
        errors.append(f"initial stack pointer 0x{stack_pointer:08x} is not 8-byte aligned")

    if reset_vector & 1 == 0:
        errors.append(f"reset vector 0x{reset_vector:08x} does not select Thumb state")
    reset_address = reset_vector & ~1
    image_end = FLASH_START + len(data)
    if not (FLASH_START <= reset_address < image_end):
        errors.append(
            f"reset handler 0x{reset_address:08x} is outside image range "
            f"0x{FLASH_START:08x}..0x{image_end - 1:08x}"
        )
    return errors


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("image", type=Path, help="raw .bin firmware image")
    args = parser.parse_args(argv)
    try:
        data = args.image.read_bytes()
    except OSError as error:
        print(f"firmware validation: {error}", file=sys.stderr)
        return 2

    errors = validate_image(data)
    if errors:
        for error in errors:
            print(f"firmware validation: {error}", file=sys.stderr)
        return 1

    stack_pointer, reset_vector = struct.unpack_from("<II", data)
    print(
        f"firmware validation: ok; bytes={len(data)}; "
        f"stack=0x{stack_pointer:08x}; reset=0x{reset_vector:08x}"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
