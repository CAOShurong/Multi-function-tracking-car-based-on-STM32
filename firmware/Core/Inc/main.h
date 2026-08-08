/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Trig_rear_Pin GPIO_PIN_13
#define Trig_rear_GPIO_Port GPIOC
#define Trig_front_Pin GPIO_PIN_14
#define Trig_front_GPIO_Port GPIOC
#define LED_Distance_Pin GPIO_PIN_15
#define LED_Distance_GPIO_Port GPIOC
#define Motor_Left4_Pin GPIO_PIN_4
#define Motor_Left4_GPIO_Port GPIOA
#define Motor_Left5_Pin GPIO_PIN_5
#define Motor_Left5_GPIO_Port GPIOA
#define Motor_Right14_Pin GPIO_PIN_14
#define Motor_Right14_GPIO_Port GPIOB
#define Motor_Right15_Pin GPIO_PIN_15
#define Motor_Right15_GPIO_Port GPIOB
#define Infrared_Right_Right_Pin GPIO_PIN_15
#define Infrared_Right_Right_GPIO_Port GPIOA
#define Infrared_Right_Pin GPIO_PIN_5
#define Infrared_Right_GPIO_Port GPIOB
#define Infrared_Left_Pin GPIO_PIN_7
#define Infrared_Left_GPIO_Port GPIOB
#define Infrared_Left_Left_Pin GPIO_PIN_9
#define Infrared_Left_Left_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
