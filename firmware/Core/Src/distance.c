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

extern int StartTimeUpEdge_Rear;
extern int EndTimeDownEdge_Rear;
extern int StartTimeUpEdge_Front;
extern int EndTimeDownEdge_Front;
extern float distance_rear;
extern float distance_front;
char message_rear[20] = "";
char message_front[20] = "";

//超声波测距模块
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim == &htim1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
		StartTimeUpEdge_Rear = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		EndTimeDownEdge_Rear = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
		distance_rear = (EndTimeDownEdge_Rear - StartTimeUpEdge_Rear) * 0.034 / 2;
	}
	if(htim == &htim1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
		StartTimeUpEdge_Front = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_4);
		EndTimeDownEdge_Front = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
		distance_front = (EndTimeDownEdge_Front - StartTimeUpEdge_Front) * 0.034 / 2;
	}
}
//倒车报警
void Distance_Rear(float Distance)
{
	if(Distance < 5){
		HAL_GPIO_WritePin(LED_Distance_GPIO_Port, LED_Distance_Pin, GPIO_PIN_RESET);
		HAL_Delay(500);
	}else{
		HAL_GPIO_TogglePin(LED_Distance_GPIO_Port, LED_Distance_Pin);
		HAL_Delay(100);
	}
}
//前后测距
void Fore_Rea_Ranging()
{
	//给Trig接口输入一小段高电平，代表测距开始
	HAL_GPIO_WritePin(Trig_rear_GPIO_Port, Trig_rear_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Trig_front_GPIO_Port, Trig_front_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(Trig_rear_GPIO_Port, Trig_rear_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Trig_front_GPIO_Port, Trig_front_Pin, GPIO_PIN_RESET);
	//设置计数器的值为0（防止开始值很接近65535而导致差值小于零）
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	//等待模块测量
	HAL_Delay(20);
	//距离小于15cm的报警系统（前无）
	if(distance_rear < 15){
	  Distance_Rear(distance_rear);
	}
	if(HAL_GPIO_ReadPin(LED_Distance_GPIO_Port, LED_Distance_Pin) == GPIO_PIN_RESET){
	  HAL_GPIO_WritePin(LED_Distance_GPIO_Port, LED_Distance_Pin, GPIO_PIN_SET);
	}
	//将数据显示在OLED上
	OLED_NewFrame();
	sprintf(message_rear, "DR=%.2fcm", distance_rear);
	OLED_PrintString(0, 0, message_rear, &font16x16, OLED_COLOR_NORMAL);
	sprintf(message_front, "DF=%.2fcm", distance_front);
	OLED_PrintString(0, 20, message_front, &font16x16, OLED_COLOR_NORMAL);
	OLED_ShowFrame();
	//减慢一下测量速度
	HAL_Delay(500);
}

//快速前后测距
void Fore_Rea_Ranging_Fast()
{
	//给Trig接口输入一小段高电平，代表测距开始
	HAL_GPIO_WritePin(Trig_rear_GPIO_Port, Trig_rear_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(Trig_front_GPIO_Port, Trig_front_Pin, GPIO_PIN_SET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(Trig_rear_GPIO_Port, Trig_rear_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Trig_front_GPIO_Port, Trig_front_Pin, GPIO_PIN_RESET);
	//设置计数器的值为0（防止开始值很接近65535而导致差值小于零）
	__HAL_TIM_SET_COUNTER(&htim1, 0);
	//等待模块测量
	HAL_Delay(20);
	//距离小于15cm的报警系统（前无）
	if(distance_rear < 15){
	  Distance_Rear(distance_rear);
	}
	if(HAL_GPIO_ReadPin(LED_Distance_GPIO_Port, LED_Distance_Pin) == GPIO_PIN_RESET){
	  HAL_GPIO_WritePin(LED_Distance_GPIO_Port, LED_Distance_Pin, GPIO_PIN_SET);
	}
	//将数据显示在OLED上
	OLED_NewFrame();
	sprintf(message_rear, "DR=%.2fcm", distance_rear);
	OLED_PrintString(0, 0, message_rear, &font16x16, OLED_COLOR_NORMAL);
	sprintf(message_front, "DF=%.2fcm", distance_front);
	OLED_PrintString(0, 20, message_front, &font16x16, OLED_COLOR_NORMAL);
	OLED_ShowFrame();
}
