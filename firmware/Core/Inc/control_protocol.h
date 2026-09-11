#ifndef CONTROL_PROTOCOL_H
#define CONTROL_PROTOCOL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CONTROL_RX_ASCII_MAX 16u
/* Manual drive (fwd/back/left/right) expires without a new UART command. */
#define CONTROL_DRIVE_TIMEOUT_MS 3000u
/* Manual forward stops when the front ultrasonic reading is this close. */
#define CONTROL_FWD_HOLD_CM 15.0f
#define CONTROL_RANGE_MAX_CM 400.0f

bool ControlCommand_IsValid(uint8_t action, uint8_t value);
bool ControlCommand_IsManualDrive(uint8_t action);
bool ControlCommand_DriveExpired(
    uint8_t action, uint32_t now_ms, uint32_t last_ms, uint32_t timeout_ms
);
bool ControlCommand_HoldForward(uint8_t action, float front_cm);
bool ControlCommand_ParseAscii(const char *line, size_t len, uint8_t *action, uint8_t *value);

typedef struct {
    uint8_t ascii[CONTROL_RX_ASCII_MAX];
    uint8_t ascii_len;
    uint8_t binary[2];
    uint8_t binary_len;
} ControlRx;

void ControlRx_Init(ControlRx *rx);
bool ControlRx_Feed(ControlRx *rx, uint8_t byte, uint8_t *action, uint8_t *value);

#endif /* CONTROL_PROTOCOL_H */
