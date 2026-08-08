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
#include "infrared.h"

extern uint8_t receive[2];

void Track()
{
	while(1)
	{
		if(receive[0] == 0){
			Stop();
			ServoSetAngle(2);
			break;
		}
		if(//检测到都是白色
		(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 0)
		)
		{
			GoHead_1();
		}else if(//检测到都是黑色
		(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 1)
		)
		{
			HAL_Delay(50);
			if(//检测到都还是黑色
			(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 1) &&
			(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 1) &&
			(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 1) &&
			(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 1)
			)
			{
				Stop();
				break;

			}
			else
				;
		}else if(//只有偏右检测到黑色
		(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 0)
		)
		{
			Right_Gentel();
		}else if(//只有最右检测到黑色
		(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 1)
		)
		{
			Right_Hairpin();
		}else if(//右边两个都检测到黑色
		(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 1)
		)
		{
			Right_RightAngle();
		}else if(//只有偏左检测到黑色
		(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 0)
		)
		{
			Left_Gentel();
		}else if(//只有最左检测到黑色
		(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 0)
		)
		{
			Left_Hairpin();
		}else if(//左边两个都检测到黑色
		(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 0)
		)
		{
			Left_RightAngle();
		}else if(//左边两个和偏右，一共三个都检测到黑色
		(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 0)
		)
		{
			Left_Hairpin();
		}else if(//右边两个和偏左，一共三个都检测到黑色
		(HAL_GPIO_ReadPin(Infrared_Left_Left_GPIO_Port, Infrared_Left_Left_Pin) == 0) &&
		(HAL_GPIO_ReadPin(Infrared_Left_GPIO_Port, Infrared_Left_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Right_GPIO_Port, Infrared_Right_Pin) == 1) &&
		(HAL_GPIO_ReadPin(Infrared_Right_Right_GPIO_Port, Infrared_Right_Right_Pin) == 1)
		)
		{
			Right_Hairpin();
		}
		else{
			GoHead_1();
		}
	}
}

void Left_Gentel()
{
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
	HAL_Delay(10);
}

void Right_Gentel()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
	HAL_Delay(10);
}

void Left_RightAngle()
{
	HAL_Delay(100);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 75);
	HAL_Delay(250);
}

void Right_RightAngle()
{
	HAL_Delay(100);
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 75);
	HAL_Delay(250);
}

void Left_Hairpin()
{
	HAL_Delay(100);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
	HAL_Delay(350);
}

void Right_Hairpin()
{
	HAL_Delay(100);
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
	HAL_Delay(350);
}


