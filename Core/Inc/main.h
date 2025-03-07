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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ad7616.h"
#include "RingBuffer.h"
#include "LSC_Airthmeitc.h"
#include "math.h"
#include "string.h"
#include <stdio.h>
#include <stdbool.h>
#include "arm_math.h"
#include "fft.h"
#include "ProRecData.h"
#include "Control.h"
#include "global.h"
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
#define AD7616_SER_Pin GPIO_PIN_2
#define AD7616_SER_GPIO_Port GPIOE
#define AD7616_RNG0_Pin GPIO_PIN_3
#define AD7616_RNG0_GPIO_Port GPIOE
#define AD7616_RNG1_Pin GPIO_PIN_4
#define AD7616_RNG1_GPIO_Port GPIOE
#define AD7616_RST_Pin GPIO_PIN_6
#define AD7616_RST_GPIO_Port GPIOE
#define Indicator_Light_Pin GPIO_PIN_13
#define Indicator_Light_GPIO_Port GPIOC
#define IGBT_Channel1_Pin GPIO_PIN_0
#define IGBT_Channel1_GPIO_Port GPIOC
#define sIGBT_Channel1_Pin GPIO_PIN_1
#define sIGBT_Channel1_GPIO_Port GPIOC
#define sKM_Channel1_Pin GPIO_PIN_2
#define sKM_Channel1_GPIO_Port GPIOC
#define KM_Channel1_Pin GPIO_PIN_3
#define KM_Channel1_GPIO_Port GPIOC
#define Test_Pin_Pin GPIO_PIN_1
#define Test_Pin_GPIO_Port GPIOA
#define AD7616_CONV_Pin GPIO_PIN_11
#define AD7616_CONV_GPIO_Port GPIOD
#define AD7616_BUSY_Pin GPIO_PIN_12
#define AD7616_BUSY_GPIO_Port GPIOD
#define AD7616_CHS2_Pin GPIO_PIN_13
#define AD7616_CHS2_GPIO_Port GPIOD
#define AD7616_CHS1_Pin GPIO_PIN_14
#define AD7616_CHS1_GPIO_Port GPIOD
#define AD7616_CHS0_Pin GPIO_PIN_15
#define AD7616_CHS0_GPIO_Port GPIOD
#define AD7616_CS_Pin GPIO_PIN_6
#define AD7616_CS_GPIO_Port GPIOC
#define AD7616_RD_Pin GPIO_PIN_7
#define AD7616_RD_GPIO_Port GPIOC
#define AD7616_WR_Pin GPIO_PIN_8
#define AD7616_WR_GPIO_Port GPIOC
#define AD7616_D15_Pin GPIO_PIN_1
#define AD7616_D15_GPIO_Port GPIOD
#define AD7616_D14_Pin GPIO_PIN_2
#define AD7616_D14_GPIO_Port GPIOD
#define AD7616_D13_Pin GPIO_PIN_3
#define AD7616_D13_GPIO_Port GPIOD
#define AD7616_D12_Pin GPIO_PIN_4
#define AD7616_D12_GPIO_Port GPIOD
#define AD7616_D11_Pin GPIO_PIN_5
#define AD7616_D11_GPIO_Port GPIOD
#define AD7616_D10_Pin GPIO_PIN_6
#define AD7616_D10_GPIO_Port GPIOD
#define AD7616_D9_Pin GPIO_PIN_7
#define AD7616_D9_GPIO_Port GPIOD
#define AD7616_D8_Pin GPIO_PIN_3
#define AD7616_D8_GPIO_Port GPIOB
#define AD7616_D7_Pin GPIO_PIN_4
#define AD7616_D7_GPIO_Port GPIOB
#define AD7616_D6_Pin GPIO_PIN_5
#define AD7616_D6_GPIO_Port GPIOB
#define AD7616_D5_Pin GPIO_PIN_6
#define AD7616_D5_GPIO_Port GPIOB
#define AD7616_D4_Pin GPIO_PIN_7
#define AD7616_D4_GPIO_Port GPIOB
#define AD7616_D3_Pin GPIO_PIN_8
#define AD7616_D3_GPIO_Port GPIOB
#define AD7616_D2_Pin GPIO_PIN_9
#define AD7616_D2_GPIO_Port GPIOB
#define AD7616_D1_Pin GPIO_PIN_0
#define AD7616_D1_GPIO_Port GPIOE
#define AD7616_D0_Pin GPIO_PIN_1
#define AD7616_D0_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
