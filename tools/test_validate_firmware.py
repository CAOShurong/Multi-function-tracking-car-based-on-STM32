import struct
import unittest

from validate_firmware import FLASH_START, FLASH_SIZE, SRAM_END, validate_image


class ValidateFirmwareTests(unittest.TestCase):
    def test_accepts_valid_vector_table(self) -> None:
        image = struct.pack("<II", SRAM_END, FLASH_START + 9) + bytes(32)
        self.assertEqual(validate_image(image), [])

    def test_rejects_short_image(self) -> None:
        self.assertIn("shorter", validate_image(bytes(7))[0])

    def test_rejects_invalid_memory_addresses(self) -> None:
        image = struct.pack("<II", 0x10000000, FLASH_START + 8) + bytes(32)
        errors = validate_image(image)
        self.assertTrue(any("stack pointer" in error for error in errors))
        self.assertTrue(any("Thumb" in error for error in errors))

    def test_rejects_image_larger_than_flash(self) -> None:
        image = struct.pack("<II", SRAM_END, FLASH_START + 9) + bytes(FLASH_SIZE)
        self.assertTrue(any("flash" in error for error in validate_image(image)))


if __name__ == "__main__":
    unittest.main()
