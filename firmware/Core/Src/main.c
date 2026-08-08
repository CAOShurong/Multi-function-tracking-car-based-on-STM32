/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "oled.h"
#include "motor.h"
#include "Servo.h"
#include "obstacle_avoidance.h"
#include "distance.h"
#include "infrared.h"
#include "aht20.h"
#include "font.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t receive[2];
uint8_t get;
int StartTimeUpEdge_Rear = 0;
int EndTimeDownEdge_Rear = 0;
int StartTimeUpEdge_Front = 0;
int EndTimeDownEdge_Front = 0;
float distance_rear = 0;
float distance_front = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	  HAL_UART_Transmit_IT(&huart2, receive, sizeof(receive));
	  HAL_UART_Receive_IT(&huart2, receive, sizeof(receive));
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  //GPIO口初始化
  HAL_GPIO_WritePin(Motor_Left4_GPIO_Port, Motor_Left4_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Motor_Left5_GPIO_Port, Motor_Left5_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(Motor_Right15_GPIO_Port, Motor_Right15_Pin, GPIO_PIN_SET);
  HAL_GPIO_WritePin(Motor_Right14_GPIO_Port, Motor_Right14_Pin, GPIO_PIN_RESET);
  //TIM4控制直流电机 1KHZ
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  //TIM3控制舵机 50HZ
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  __HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 14);
  //开启UART2轮询接收模式
  HAL_UART_Receive_IT(&huart2, receive, sizeof(receive));
  //设置轮子初始速度
  __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
  //OLED初始化
  HAL_Delay(20);
  OLED_Init();
  //启动TIM1定时器计数
  HAL_TIM_Base_Start(&htim1);
  //启动定时器输入捕获IC，测量结束时TIM_CHANNEL_2引发中断
  HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);
  HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_4);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //前后测量
	  Fore_Rea_Ranging();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	  //蓝牙接收数据，配置四个轮子
	  if(receive[0] == 0){
		  ServoSetAngle(2);
		  Stop();
	  }
	  else if(receive[0] == 1){
		  //表明收到指令向前走
		  GoHead();
	  }else if(receive[0] == 2){
		  //表明收到指令向后走
		  GoBack();
	  }else if(receive[0] == 3){
		  //表明收到指令向左转
		  TurnLeft();
	  }else if(receive[0] == 4){
		  //表明收到指令向右转
		  TurnRight();
	  }else if(receive[0] == 5){
		  //表明收到舵机控制指令
		  ServoSetAngle(receive[1]);
	  }else if(receive[0] == 6){
		  //表明收到自动避障指令
		  if(receive[1] == 0){
			  Stop();
		  }else if(receive[1] == 1){
			  Auto_Obstacle_Avoidance_Slow();
		  }else if(receive[1] == 2){
			  Auto_Obstacle_Avoidance_Fast();
		  }
	  }else if(receive[0] == 7){
		  Track();
	  }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
