/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PC14_OSC32_IN_Pin GPIO_PIN_14
#define PC14_OSC32_IN_GPIO_Port GPIOC
#define PC15_OSC32_OUT_Pin GPIO_PIN_15
#define PC15_OSC32_OUT_GPIO_Port GPIOC
#define PH0_OSC_IN_Pin GPIO_PIN_0
#define PH0_OSC_IN_GPIO_Port GPIOH
#define PH1_OSC_OUT_Pin GPIO_PIN_1
#define PH1_OSC_OUT_GPIO_Port GPIOH
#define Stepper1_DIR_Pin GPIO_PIN_8
#define Stepper1_DIR_GPIO_Port GPIOD
#define Stepper1_EN_Pin GPIO_PIN_9
#define Stepper1_EN_GPIO_Port GPIOD
#define Stepper2_DIR_Pin GPIO_PIN_8
#define Stepper2_DIR_GPIO_Port GPIOC
#define Stepper2_EN_Pin GPIO_PIN_9
#define Stepper2_EN_GPIO_Port GPIOC
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define SWCLK_Pin GPIO_PIN_14
#define SWCLK_GPIO_Port GPIOA
#define A1_Pin GPIO_PIN_1
#define A1_GPIO_Port GPIOD
#define B1_Pin GPIO_PIN_2
#define B1_GPIO_Port GPIOD
#define A2_Pin GPIO_PIN_3
#define A2_GPIO_Port GPIOD
#define B2_Pin GPIO_PIN_4
#define B2_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */
#define MY_CAN_ID (10)
#define GEAR_RATIO (239.0)  	//Gearbox ratio
#define ENCODER_RESOLUTION (34.0) // Number of encoder poles
#define TIM10_FREQUENCY (2000000.0) // Rate of counter increment, Hz
#define TIM7_IT_FREQUENCY (25.0)		// rate of interrupt, Hz
#define DC_PULSE_LOCK (1500)		// max DC PWM switch value
#define MY_WHEEL_ID_1 (11)
#define MY_WHEEL_ID_2 (12)
#define MY_WHEEL_ID_3 (13)

#define CONTROL_MODE_RAW (0)
#define CONTROL_MODE_PID (1)
#define CONTROL_MODE_ST (2)

#define POWER_SAVING_OFF (0)
#define POWER_SAVING_ON (1)

#define HEARTBEAT_OFF (0)
#define HEARTBEAT_ON (1)

#define dc_len 2
#define stp_len 2

#define adc_buffer_len 50 //necesssary to initialize the library

#include <motion_lib.h>

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
