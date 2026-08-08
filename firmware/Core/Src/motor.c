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

void Stop()
{
	  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
}

void GoHead()
{
	  if(receive[0] == 1){
		  //表明收到指令向前走
		  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
		  if(receive[1] == 0){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
		  }else if(receive[1] == 1){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 40);
		  }else if(receive[1] == 2){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 49);
		  }else if(receive[1] == 3){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
		  }else if(receive[1] == 4){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
		  }else if(receive[1] == 5){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
		  }
	  }
}

void GoBack()
{
	 if(receive[0] == 2){
		  //表明收到指令向后走
		  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
		  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
		  if(receive[1] == 0){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
		  }else if(receive[1] == 1){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 40);
		  }else if(receive[1] == 2){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 49);
		  }else if(receive[1] == 3){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
		  }else if(receive[1] == 4){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
		  }else if(receive[1] == 5){
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
		  }
	 }
}

void TurnLeft()
{
	 if(receive[0] == 3){
		  //表明收到指令向左转
		  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
		  if(receive[1] == 0){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
		  }else if(receive[1] == 1){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
		  }else if(receive[1] == 2){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
		  }else if(receive[1] == 3){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
		  }else if(receive[1] == 4){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
		  }else if(receive[1] == 5){
			  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
		  }
	 }
}

void TurnRight()
{
	 if(receive[0] == 4){
		  //表明收到指令向右转
		  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
		  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
		  if(receive[1] == 0){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
		  }else if(receive[1] == 1){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
		  }else if(receive[1] == 2){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
		  }else if(receive[1] == 3){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
		  }else if(receive[1] == 4){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
		  }else if(receive[1] == 5){
			  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
			  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
			  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
		  }
	  }
}
