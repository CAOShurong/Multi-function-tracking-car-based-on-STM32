#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include <stdint.h>

void Stop(void);
void GoHead(uint8_t speed_level);
void GoBack(uint8_t speed_level);
void TurnLeft(uint8_t speed_level);
void TurnRight(uint8_t speed_level);


#endif /* INC_MOTOR_H_ */
