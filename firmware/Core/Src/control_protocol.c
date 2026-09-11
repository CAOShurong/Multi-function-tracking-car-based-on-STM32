#include "control_protocol.h"

bool ControlCommand_IsValid(uint8_t action, uint8_t value)
{
    switch (action) {
    case 0:
    case 7:
        return true;
    case 1:
    case 2:
    case 3:
    case 4:
        return value <= 5;
    case 5:
        return value <= 4;
    case 6:
        return value <= 1;
    default:
        return false;
    }
}

static bool is_space(uint8_t byte)
{
    return byte == ' ' || byte == '\t';
}

static bool is_text_byte(uint8_t byte)
{
    /* Printable ASCII, including letters. Binary commands are 0-7 / 0-5. */
    return byte >= 0x20u && byte <= 0x7Eu;
}

bool ControlCommand_ParseAscii(const char *line, size_t len, uint8_t *action, uint8_t *value)
{
    size_t i = 0;
    unsigned int parsed_action = 0;
    unsigned int parsed_value = 0;
    unsigned int digits;

    if (line == NULL || action == NULL || value == NULL) {
        return false;
    }

    while (i < len && is_space((uint8_t)line[i])) {
        i++;
    }
    digits = 0;
    while (i < len && line[i] >= '0' && line[i] <= '9') {
        parsed_action = parsed_action * 10u + (unsigned int)(line[i] - '0');
        if (parsed_action > 255u) {
            return false;
        }
        digits++;
        i++;
    }
    if (digits == 0 || digits > 3) {
        return false;
    }

    while (i < len && is_space((uint8_t)line[i])) {
        i++;
    }
    if (i < len && line[i] == ',') {
        i++;
        while (i < len && is_space((uint8_t)line[i])) {
            i++;
        }
        digits = 0;
        while (i < len && line[i] >= '0' && line[i] <= '9') {
            parsed_value = parsed_value * 10u + (unsigned int)(line[i] - '0');
            if (parsed_value > 255u) {
                return false;
            }
            digits++;
            i++;
        }
        if (digits == 0 || digits > 3) {
            return false;
        }
    } else if (i < len && line[i] >= '0' && line[i] <= '9') {
        digits = 0;
        while (i < len && line[i] >= '0' && line[i] <= '9') {
            parsed_value = parsed_value * 10u + (unsigned int)(line[i] - '0');
            if (parsed_value > 255u) {
                return false;
            }
            digits++;
            i++;
        }
        if (digits == 0 || digits > 3) {
            return false;
        }
    }

    while (i < len && is_space((uint8_t)line[i])) {
        i++;
    }
    if (i != len) {
        return false;
    }

    *action = (uint8_t)parsed_action;
    *value = (uint8_t)parsed_value;
    return true;
}

void ControlRx_Init(ControlRx *rx)
{
    if (rx == NULL) {
        return;
    }
    rx->ascii_len = 0;
    rx->binary_len = 0;
}

bool ControlRx_Feed(ControlRx *rx, uint8_t byte, uint8_t *action, uint8_t *value)
{
    if (rx == NULL || action == NULL || value == NULL) {
        return false;
    }

    if (byte == '\r' || byte == '\n') {
        bool ok = false;
        if (rx->ascii_len > 0) {
            ok = ControlCommand_ParseAscii(
                (const char *)rx->ascii, rx->ascii_len, action, value
            );
        }
        rx->ascii_len = 0;
        rx->binary_len = 0;
        return ok;
    }

    if (is_text_byte(byte)) {
        rx->binary_len = 0;
        if (rx->ascii_len < CONTROL_RX_ASCII_MAX) {
            rx->ascii[rx->ascii_len++] = byte;
        } else {
            rx->ascii_len = 0;
        }
        return false;
    }

    rx->ascii_len = 0;
    if (rx->binary_len >= 2) {
        rx->binary_len = 0;
    }
    rx->binary[rx->binary_len++] = byte;
    if (rx->binary_len < 2) {
        return false;
    }
    rx->binary_len = 0;
    *action = rx->binary[0];
    *value = rx->binary[1];
    return true;
}
