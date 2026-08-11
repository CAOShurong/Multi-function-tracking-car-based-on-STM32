#ifndef CONTROL_PROTOCOL_H
#define CONTROL_PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

bool ControlCommand_IsValid(uint8_t action, uint8_t value);

#endif /* CONTROL_PROTOCOL_H */
