#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <stdio.h>
#include "oled.h"
#include "motor.h"
#include "Servo.h"
#include "obstacle_avoidance.h"

void ServoSetAngle(int type)
{
	if(type == 0){
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 24);
	}else if(type == 1){
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 19);
	}else if(type == 2){
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 14);
	}else if(type == 3){
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 9);
	}else if(type == 4){
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 5);
	}
}
