/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "stdint.h"
/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc;

/* USER CODE BEGIN Private defines */
#define ADC_NUM_CHANNELS  3
#define ADC_NUM_BITS    12
#define ADC_MAX_READING ((1 << ADC_NUM_BITS) - 1)
/* USER CODE END Private defines */

void MX_ADC_Init(void);

/* USER CODE BEGIN Prototypes */
/**
 * @brief Return the ADC_HandleTypeDef pointer to the specified instance. 
 * Returns NULL pointer if instance is not found. 
 * 
 * @return ADC_HandleTypeDef* requested instance
 */
ADC_HandleTypeDef* HAL_ADC_getInstance(void);

/**
 * @brief Returns the value of the given ADC channel defined by the conversion 
 * sequence (not the hardware channel number).
 * 
 * @param channel_idx index of channel sequence to read (0 <= channel_idx < ADC_NUM_CHANNELS)
 * @return uint16_t The 16-bit value of the ADC reading
 */
uint16_t ADC_readChannel(uint32_t channel_idx);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
