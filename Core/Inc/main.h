/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define IR_Sensor1_Pin GPIO_PIN_13
#define IR_Sensor1_GPIO_Port GPIOC
#define IR_Sensor2_Pin GPIO_PIN_2
#define IR_Sensor2_GPIO_Port GPIOC
#define IR_Sensor3_Pin GPIO_PIN_3
#define IR_Sensor3_GPIO_Port GPIOC
#define ENC2_A_Pin GPIO_PIN_0
#define ENC2_A_GPIO_Port GPIOA
#define ENC2_B_Pin GPIO_PIN_1
#define ENC2_B_GPIO_Port GPIOA
#define AX_12_Pin GPIO_PIN_2
#define AX_12_GPIO_Port GPIOA
#define Cinc_Pin GPIO_PIN_4
#define Cinc_GPIO_Port GPIOA
#define ENC1_A_Pin GPIO_PIN_5
#define ENC1_A_GPIO_Port GPIOA
#define Pumpa_Pin GPIO_PIN_1
#define Pumpa_GPIO_Port GPIOB
#define Magnet_Pin GPIO_PIN_2
#define Magnet_GPIO_Port GPIOB
#define M1_PWM_Pin GPIO_PIN_7
#define M1_PWM_GPIO_Port GPIOC
#define M1_Dir_Pin GPIO_PIN_8
#define M1_Dir_GPIO_Port GPIOA
#define M2_Dir_Pin GPIO_PIN_9
#define M2_Dir_GPIO_Port GPIOA
#define Kamera_TX_Pin GPIO_PIN_11
#define Kamera_TX_GPIO_Port GPIOA
#define ENC1_B_Pin GPIO_PIN_3
#define ENC1_B_GPIO_Port GPIOB
#define M2_PWM_Pin GPIO_PIN_6
#define M2_PWM_GPIO_Port GPIOB
#define IR_Sensor4_Pin GPIO_PIN_7
#define IR_Sensor4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
