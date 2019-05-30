/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

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
#define LEDCOL1_Pin GPIO_PIN_0
#define LEDCOL1_GPIO_Port GPIOA
#define LEDCOL2_Pin GPIO_PIN_1
#define LEDCOL2_GPIO_Port GPIOA
#define LEDCOL3_Pin GPIO_PIN_2
#define LEDCOL3_GPIO_Port GPIOA
#define LEDROW1_Pin GPIO_PIN_3
#define LEDROW1_GPIO_Port GPIOA
#define LEDROW2_Pin GPIO_PIN_4
#define LEDROW2_GPIO_Port GPIOA
#define LEDROW3_Pin GPIO_PIN_5
#define LEDROW3_GPIO_Port GPIOA
#define LEDROW4_Pin GPIO_PIN_6
#define LEDROW4_GPIO_Port GPIOA
#define BLINKER_Pin GPIO_PIN_12
#define BLINKER_GPIO_Port GPIOB
#define KEYCOL1_Pin GPIO_PIN_3
#define KEYCOL1_GPIO_Port GPIOB
#define KEYCOL2_Pin GPIO_PIN_4
#define KEYCOL2_GPIO_Port GPIOB
#define KEYCOL3_Pin GPIO_PIN_5
#define KEYCOL3_GPIO_Port GPIOB
#define KEYROW1_Pin GPIO_PIN_6
#define KEYROW1_GPIO_Port GPIOB
#define KEYROW2_Pin GPIO_PIN_7
#define KEYROW2_GPIO_Port GPIOB
#define KEYROW3_Pin GPIO_PIN_8
#define KEYROW3_GPIO_Port GPIOB
#define KEYROW4_Pin GPIO_PIN_9
#define KEYROW4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
