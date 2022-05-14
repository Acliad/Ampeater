/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdint.h"
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
#define ENCODER_A_Pin GPIO_PIN_14
#define ENCODER_A_GPIO_Port GPIOC
#define ENCODER_A_EXTI_IRQn EXTI4_15_IRQn
#define ENCODER_B_Pin GPIO_PIN_15
#define ENCODER_B_GPIO_Port GPIOC
#define ENCODER_B_EXTI_IRQn EXTI4_15_IRQn
#define Vsense_R_Pin GPIO_PIN_0
#define Vsense_R_GPIO_Port GPIOA
#define Vsense_FET_Pin GPIO_PIN_1
#define Vsense_FET_GPIO_Port GPIOA
#define UART_TX_SPI_CS_LCD_Pin GPIO_PIN_2
#define UART_TX_SPI_CS_LCD_GPIO_Port GPIOA
#define UART_RX_SPI_DC_LCD_Pin GPIO_PIN_3
#define UART_RX_SPI_DC_LCD_GPIO_Port GPIOA
#define SPI_CS_DAC_Pin GPIO_PIN_4
#define SPI_CS_DAC_GPIO_Port GPIOA
#define SPI_SCK_Pin GPIO_PIN_5
#define SPI_SCK_GPIO_Port GPIOA
#define SPI_MISO_Pin GPIO_PIN_6
#define SPI_MISO_GPIO_Port GPIOA
#define SPI_MOSI_Pin GPIO_PIN_7
#define SPI_MOSI_GPIO_Port GPIOA
#define Vbtns_Pin GPIO_PIN_1
#define Vbtns_GPIO_Port GPIOB
#define SPI_nRST_LCD_Pin GPIO_PIN_9
#define SPI_nRST_LCD_GPIO_Port GPIOA
#define RANGE_SW_Pin GPIO_PIN_10
#define RANGE_SW_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */
// Channel index indicates what order the ADC channel is in. There are currently
// 3 channels: Vsense, Vense_fet, and btns, so these would be idx 0, 1, 2
#define ADC_VSENSE_CHANNEL_IDX 0
#define ADC_VFET_CHANNEL_IDX   1
#define ADC_BTN_CHANNEL_IDX    2

#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
