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

void Stop(void)
{
	  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
}

void GoHead(uint8_t speed_level)
{
	  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	  if(speed_level == 0){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
	  }else if(speed_level == 1){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 40);
	  }else if(speed_level == 2){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 49);
	  }else if(speed_level == 3){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
	  }else if(speed_level == 4){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
	  }else if(speed_level == 5){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
	  }
}

void GoBack(uint8_t speed_level)
{
	  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
	  if(speed_level == 0){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
	  }else if(speed_level == 1){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 40);
	  }else if(speed_level == 2){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 49);
	  }else if(speed_level == 3){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
	  }else if(speed_level == 4){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
	  }else if(speed_level == 5){
		  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
	  }
}

void TurnLeft(uint8_t speed_level)
{
	  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
		  if(speed_level == 0){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
		  }else if(speed_level == 1){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
		  }else if(speed_level == 2){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
		  }else if(speed_level == 3){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
		  }else if(speed_level == 4){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
		  }else if(speed_level == 5){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
		  }
}

void TurnRight(uint8_t speed_level)
{
	  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
		  if(speed_level == 0){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
		  }else if(speed_level == 1){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
		  }else if(speed_level == 2){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
		  }else if(speed_level == 3){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
		  }else if(speed_level == 4){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
		  }else if(speed_level == 5){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
		  }
}
