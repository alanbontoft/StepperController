/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <math.h>
/* USER CODE END Includes */
#include <stdbool.h>
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
void setStepSize();
void rotate();
void enableDriver(bool enable);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOA
#define EN1_Pin GPIO_PIN_1
#define EN1_GPIO_Port GPIOA
#define VCP_TX_Pin GPIO_PIN_2
#define VCP_TX_GPIO_Port GPIOA
#define RST1_Pin GPIO_PIN_3
#define RST1_GPIO_Port GPIOA
#define STEP1_Pin GPIO_PIN_4
#define STEP1_GPIO_Port GPIOA
#define DIR1_Pin GPIO_PIN_5
#define DIR1_GPIO_Port GPIOA
#define EN2_Pin GPIO_PIN_6
#define EN2_GPIO_Port GPIOA
#define RST2_Pin GPIO_PIN_7
#define RST2_GPIO_Port GPIOA
#define EN3_Pin GPIO_PIN_0
#define EN3_GPIO_Port GPIOB
#define RST3_Pin GPIO_PIN_1
#define RST3_GPIO_Port GPIOB
#define STEP2_Pin GPIO_PIN_8
#define STEP2_GPIO_Port GPIOA
#define DIR2_Pin GPIO_PIN_9
#define DIR2_GPIO_Port GPIOA
#define MS1_Pin GPIO_PIN_10
#define MS1_GPIO_Port GPIOA
#define MS2_Pin GPIO_PIN_11
#define MS2_GPIO_Port GPIOA
#define MS3_Pin GPIO_PIN_12
#define MS3_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define VCP_RX_Pin GPIO_PIN_15
#define VCP_RX_GPIO_Port GPIOA
#define LD3_Pin GPIO_PIN_3
#define LD3_GPIO_Port GPIOB
#define STEP3_Pin GPIO_PIN_4
#define STEP3_GPIO_Port GPIOB
#define DIR3_Pin GPIO_PIN_5
#define DIR3_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
