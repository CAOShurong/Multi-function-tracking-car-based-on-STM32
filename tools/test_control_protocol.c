#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "control_protocol.h"

static bool expected_validity(uint8_t action, uint8_t value)
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

static bool parse_ok(const char *line, uint8_t *action, uint8_t *value)
{
    return ControlCommand_ParseAscii(line, strlen(line), action, value);
}

static bool feed_bytes(ControlRx *rx, const uint8_t *bytes, size_t n, uint8_t *action, uint8_t *value)
{
    bool published = false;
    for (size_t i = 0; i < n; i++) {
        uint8_t next_action;
        uint8_t next_value;
        if (ControlRx_Feed(rx, bytes[i], &next_action, &next_value)) {
            *action = next_action;
            *value = next_value;
            published = true;
        }
    }
    return published;
}

int main(void)
{
    uint8_t action;
    uint8_t value;
    ControlRx rx;

    for (unsigned int a = 0; a <= UINT8_MAX; ++a) {
        for (unsigned int v = 0; v <= UINT8_MAX; ++v) {
            assert(ControlCommand_IsValid((uint8_t)a, (uint8_t)v) ==
                   expected_validity((uint8_t)a, (uint8_t)v));
        }
    }

    assert(parse_ok("1,3", &action, &value) && action == 1 && value == 3);
    assert(parse_ok("1 3", &action, &value) && action == 1 && value == 3);
    assert(parse_ok(" 01, 03 ", &action, &value) && action == 1 && value == 3);
    assert(parse_ok("7", &action, &value) && action == 7 && value == 0);
    assert(parse_ok("0", &action, &value) && action == 0 && value == 0);
    assert(parse_ok("6,1", &action, &value) && action == 6 && value == 1);
    assert(!parse_ok("", &action, &value));
    assert(!parse_ok("1,", &action, &value));
    assert(!parse_ok("abc", &action, &value));
    assert(!parse_ok("1,3x", &action, &value));
    assert(!parse_ok("256,0", &action, &value));

    assert(parse_ok("stop", &action, &value) && action == 0 && value == 0);
    assert(parse_ok("STOP", &action, &value) && action == 0);
    assert(parse_ok("ting", &action, &value) && action == 0);
    assert(parse_ok("fwd", &action, &value) && action == 1 && value == 3);
    assert(parse_ok("fwd 5", &action, &value) && action == 1 && value == 5);
    assert(parse_ok("forward,2", &action, &value) && action == 1 && value == 2);
    assert(parse_ok("w", &action, &value) && action == 1 && value == 3);
    assert(parse_ok("qianjin,4", &action, &value) && action == 1 && value == 4);
    assert(parse_ok("s", &action, &value) && action == 2 && value == 3);
    assert(parse_ok("back,1", &action, &value) && action == 2 && value == 1);
    assert(parse_ok("a", &action, &value) && action == 3 && value == 3);
    assert(parse_ok("left 0", &action, &value) && action == 3 && value == 0);
    assert(parse_ok("d", &action, &value) && action == 4 && value == 3);
    assert(parse_ok("you,5", &action, &value) && action == 4 && value == 5);
    assert(parse_ok("servo", &action, &value) && action == 5 && value == 2);
    assert(parse_ok("duoji 0", &action, &value) && action == 5 && value == 0);
    assert(parse_ok("avoid", &action, &value) && action == 6 && value == 1);
    assert(parse_ok("bizhang,0", &action, &value) && action == 6 && value == 0);
    assert(parse_ok("track", &action, &value) && action == 7 && value == 0);
    assert(parse_ok("xunji", &action, &value) && action == 7);
    assert(!parse_ok("nope", &action, &value));
    assert(!parse_ok("fwd,", &action, &value));
    assert(!parse_ok("fwd 3x", &action, &value));

    ControlRx_Init(&rx);
    {
        const uint8_t binary[] = {1, 3};
        assert(feed_bytes(&rx, binary, sizeof(binary), &action, &value));
        assert(action == 1 && value == 3);
    }

    ControlRx_Init(&rx);
    {
        const uint8_t ascii[] = {'1', ',', '3', '\n'};
        assert(feed_bytes(&rx, ascii, sizeof(ascii), &action, &value));
        assert(action == 1 && value == 3);
    }

    ControlRx_Init(&rx);
    {
        const uint8_t crlf[] = {'5', ',', '2', '\r', '\n'};
        assert(feed_bytes(&rx, crlf, sizeof(crlf), &action, &value));
        assert(action == 5 && value == 2);
    }

    ControlRx_Init(&rx);
    {
        const uint8_t junk[] = {'h', 'i', '\n', 4, 2};
        assert(feed_bytes(&rx, junk, sizeof(junk), &action, &value));
        assert(action == 4 && value == 2);
    }

    ControlRx_Init(&rx);
    {
        const uint8_t letters[] = {'h', 'e'};
        assert(!feed_bytes(&rx, letters, sizeof(letters), &action, &value));
    }

    ControlRx_Init(&rx);
    {
        const uint8_t no_cmd[] = {'h', 'e', 'l', 'l', 'o', '\n'};
        assert(!feed_bytes(&rx, no_cmd, sizeof(no_cmd), &action, &value));
    }

    ControlRx_Init(&rx);
    {
        const uint8_t named[] = {'f', 'w', 'd', ' ', '3', '\n'};
        assert(feed_bytes(&rx, named, sizeof(named), &action, &value));
        assert(action == 1 && value == 3);
    }

    ControlRx_Init(&rx);
    {
        const uint8_t wasd[] = {'w', '\r', '\n'};
        assert(feed_bytes(&rx, wasd, sizeof(wasd), &action, &value));
        assert(action == 1 && value == 3);
    }

    assert(ControlCommand_IsManualDrive(1));
    assert(ControlCommand_IsManualDrive(4));
    assert(!ControlCommand_IsManualDrive(0));
    assert(!ControlCommand_IsManualDrive(7));
    assert(!ControlCommand_DriveExpired(1, 1000u, 1000u, 3000u));
    assert(!ControlCommand_DriveExpired(1, 3999u, 1000u, 3000u));
    assert(ControlCommand_DriveExpired(1, 4001u, 1000u, 3000u));
    assert(!ControlCommand_DriveExpired(7, 9000u, 1000u, 3000u));
    assert(!ControlCommand_DriveExpired(1, 9000u, 1000u, 0u));
    /* Unsigned tick wrap: last near 2^32-1, now small. */
    assert(ControlCommand_DriveExpired(2, 100u, 0xFFFFFFFFu - 4000u, 3000u));

    return 0;
}
