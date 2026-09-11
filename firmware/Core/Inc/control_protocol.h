#ifndef CONTROL_PROTOCOL_H
#define CONTROL_PROTOCOL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CONTROL_RX_ASCII_MAX 16u

bool ControlCommand_IsValid(uint8_t action, uint8_t value);
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
