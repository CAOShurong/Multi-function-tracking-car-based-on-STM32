#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

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

int main(void)
{
    for (unsigned int action = 0; action <= UINT8_MAX; ++action) {
        for (unsigned int value = 0; value <= UINT8_MAX; ++value) {
            assert(ControlCommand_IsValid((uint8_t)action, (uint8_t)value) ==
                   expected_validity((uint8_t)action, (uint8_t)value));
        }
    }

    return 0;
}
