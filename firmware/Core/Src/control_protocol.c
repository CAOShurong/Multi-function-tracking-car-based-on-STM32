#include "control_protocol.h"

bool ControlCommand_IsManualDrive(uint8_t action)
{
    return action >= 1u && action <= 4u;
}

bool ControlCommand_DriveExpired(
    uint8_t action, uint32_t now_ms, uint32_t last_ms, uint32_t timeout_ms
)
{
    uint32_t elapsed;

    if (!ControlCommand_IsManualDrive(action) || timeout_ms == 0u) {
        return false;
    }
    elapsed = now_ms - last_ms;
    return elapsed > timeout_ms;
}

bool ControlCommand_HoldForward(uint8_t action, float front_cm)
{
    /* Only Bluetooth/manual forward. Track/avoid have their own sensors.
       No-echo (0) and out-of-range readings must not brick the car. */
    if (action != 1u) {
        return false;
    }
    if (!(front_cm > 0.0f) || front_cm > CONTROL_RANGE_MAX_CM) {
        return false;
    }
    return front_cm < CONTROL_FWD_HOLD_CM;
}

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

static bool is_alpha(uint8_t byte)
{
    return (byte >= 'A' && byte <= 'Z') || (byte >= 'a' && byte <= 'z');
}

static char lower_ascii(char c)
{
    if (c >= 'A' && c <= 'Z') {
        return (char)(c + ('a' - 'A'));
    }
    return c;
}

static bool token_eq(const char *s, size_t n, const char *lit)
{
    size_t i;

    for (i = 0; lit[i] != '\0'; i++) {
        if (i >= n || lower_ascii(s[i]) != lit[i]) {
            return false;
        }
    }
    return i == n;
}

static bool parse_optional_value(
    const char *line, size_t len, size_t *i, unsigned int *parsed_value, bool *has_value
)
{
    unsigned int digits = 0;

    while (*i < len && is_space((uint8_t)line[*i])) {
        (*i)++;
    }
    if (*i < len && line[*i] == ',') {
        (*i)++;
        while (*i < len && is_space((uint8_t)line[*i])) {
            (*i)++;
        }
    }
    if (*i >= len || line[*i] < '0' || line[*i] > '9') {
        *has_value = false;
        *parsed_value = 0;
        return true;
    }
    *parsed_value = 0;
    while (*i < len && line[*i] >= '0' && line[*i] <= '9') {
        *parsed_value = *parsed_value * 10u + (unsigned int)(line[*i] - '0');
        if (*parsed_value > 255u) {
            return false;
        }
        digits++;
        (*i)++;
    }
    if (digits == 0 || digits > 3) {
        return false;
    }
    *has_value = true;
    return true;
}

static bool parse_named(const char *line, size_t len, uint8_t *action, uint8_t *value)
{
    size_t i = 0;
    size_t start;
    size_t n;
    unsigned int parsed_value = 0;
    bool has_value = false;
    const char *tok;

    while (i < len && is_space((uint8_t)line[i])) {
        i++;
    }
    start = i;
    while (i < len && is_alpha((uint8_t)line[i])) {
        i++;
    }
    n = i - start;
    if (n == 0) {
        return false;
    }
    if (!parse_optional_value(line, len, &i, &parsed_value, &has_value)) {
        return false;
    }
    while (i < len && is_space((uint8_t)line[i])) {
        i++;
    }
    if (i != len) {
        return false;
    }

    tok = line + start;
    if (token_eq(tok, n, "stop") || token_eq(tok, n, "halt") || token_eq(tok, n, "ting")
        || token_eq(tok, n, "tingzhi")) {
        *action = 0;
        *value = 0;
        return true;
    }
    if (token_eq(tok, n, "fwd") || token_eq(tok, n, "forward") || token_eq(tok, n, "go")
        || token_eq(tok, n, "w") || token_eq(tok, n, "qian") || token_eq(tok, n, "qianjin")) {
        *action = 1;
        *value = has_value ? (uint8_t)parsed_value : 3u;
        return true;
    }
    if (token_eq(tok, n, "back") || token_eq(tok, n, "backward") || token_eq(tok, n, "rev")
        || token_eq(tok, n, "s") || token_eq(tok, n, "hou") || token_eq(tok, n, "houtui")) {
        *action = 2;
        *value = has_value ? (uint8_t)parsed_value : 3u;
        return true;
    }
    if (token_eq(tok, n, "left") || token_eq(tok, n, "a") || token_eq(tok, n, "zuo")) {
        *action = 3;
        *value = has_value ? (uint8_t)parsed_value : 3u;
        return true;
    }
    if (token_eq(tok, n, "right") || token_eq(tok, n, "d") || token_eq(tok, n, "you")) {
        *action = 4;
        *value = has_value ? (uint8_t)parsed_value : 3u;
        return true;
    }
    if (token_eq(tok, n, "servo") || token_eq(tok, n, "duoji")) {
        *action = 5;
        *value = has_value ? (uint8_t)parsed_value : 2u;
        return true;
    }
    if (token_eq(tok, n, "avoid") || token_eq(tok, n, "oa") || token_eq(tok, n, "bizhang")) {
        *action = 6;
        *value = has_value ? (uint8_t)parsed_value : 1u;
        return true;
    }
    if (token_eq(tok, n, "track") || token_eq(tok, n, "t") || token_eq(tok, n, "xunji")) {
        *action = 7;
        *value = 0;
        return true;
    }
    return false;
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
    if (i < len && is_alpha((uint8_t)line[i])) {
        return parse_named(line, len, action, value);
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
