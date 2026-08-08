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

extern float distance_front;
extern uint8_t receive[2];

void Auto_Obstacle_Avoidance_Slow()
{
	ServoSetAngle(2);
	while(1){
		Fore_Rea_Ranging_Fast();
		if(receive[0] == 0){
			Stop();
			ServoSetAngle(2);
			break;
		}
		Fore_Rea_Ranging_Fast();
		while(distance_front > 15){
			GoHead_2();
			Fore_Rea_Ranging_Fast();
			if(receive[0] == 0){
				Stop();
				ServoSetAngle(2);
				break;
			}
		}
		Fore_Rea_Ranging_Fast();
		if(distance_front < 15)
		{
			//前方有障碍，先后退一点，停下来先观察一下
			GoBack_2();
			HAL_Delay(1000);
			Stop();
			ServoSetAngle(0);
			HAL_Delay(1000);
			Fore_Rea_Ranging_Fast();
			HAL_Delay(1000);
			int distance_left = distance_front;
			ServoSetAngle(4);
			HAL_Delay(1000);
			Fore_Rea_Ranging_Fast();
			HAL_Delay(1000);
			int distance_right = distance_front;
			//先判断是否收到停止信号
			if(receive[0] == 0){
				Stop();
				ServoSetAngle(2);
				break;
			}
			if(distance_left > 15 && distance_right > 15){
				Fore_Rea_Ranging_Fast();
				obstacle_front();
			}else if(distance_left > 15 && distance_right <= 15){
				Fore_Rea_Ranging_Fast();
				obstacle_front_right();
			}else if(distance_left <= 15 && distance_right > 15){
				Fore_Rea_Ranging_Fast();
				obstacle_front_left();
			}else if(distance_left <= 15 && distance_right <= 15){
				Fore_Rea_Ranging_Fast();
				obstacle_front_left_right();
			}
		}
	}
}

void Auto_Obstacle_Avoidance_Fast()
{

}

void obstacle_front()
{
	ServoSetAngle(2);
	TurnLeft_3();
	HAL_Delay(1500);
	Stop();
	HAL_Delay(1500);
	Fore_Rea_Ranging_Fast();
	GoHead_2();
}

void obstacle_front_left()
{
	ServoSetAngle(2);
	TurnRight_3();
	HAL_Delay(1500);
	Stop();
	HAL_Delay(1500);
	Fore_Rea_Ranging_Fast();
	GoHead_2();
}

void obstacle_front_right()
{
	ServoSetAngle(2);
	TurnLeft_3();
	HAL_Delay(1500);
	Stop();
	HAL_Delay(1500);
	Fore_Rea_Ranging_Fast();
	GoHead_2();
}

void obstacle_front_left_right()
{
	int distance_left = 0;
	int distance_right = 0;
	while(distance_left < 15 && distance_right < 15){
		ServoSetAngle(2);
		GoBack_2();
		HAL_Delay(2000);
		Stop();
		ServoSetAngle(0);
		HAL_Delay(1000);
		Fore_Rea_Ranging_Fast();
		HAL_Delay(1000);
		distance_left = distance_front;
		ServoSetAngle(4);
		HAL_Delay(1000);
		Fore_Rea_Ranging_Fast();
		HAL_Delay(1000);
		distance_right = distance_front;
	}
	Quick_Turn_Right();
	HAL_Delay(1500);
	ServoSetAngle(2);
	Fore_Rea_Ranging_Fast();
	GoHead_2();
}

void GoHead_1()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 41);
}
void GoHead_2()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 49);
}
void GoHead_3()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
}
void GoBack_1()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 40);
}
void GoBack_2()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 49);
}
void GoBack_3()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
}
void TurnLeft_1()
{
	  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
}
void TurnLeft_2()
{
	  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
}
void TurnLeft_3()
{
	  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
}
void TurnRight_1()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 59);
}
void TurnRight_2()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 79);
}
void TurnRight_3()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
}
void Quick_Turn_Right()
{
	HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_SET);
	__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
}

void Quick_Turn_Left()
{
	  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
	  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_SET);
	  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 99);
}
