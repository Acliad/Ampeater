/**
 * @file encoder.h
 * @author irex
 * @brief Basic library for reading rotary encoder
 * @version 0.1
 * @date 2022-05-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __ENCODER_H__
#define __ENCODER_H__

#include "main.h"
#include "stm32l041xx.h"

#define ENCODER_TICKS_PER_DETENT 3 // Number of ticks (usually) registered per detent

/**
 * @brief Initialize the encoder. Sets up the interrupt callback based on 
 * appropriate pins.
 * 
 */
void ENCODER_init(int32_t min_value, int32_t max_value);

/**
 * @brief Returns the amount the encoder has moved since the last call to getIncrement.
 * 
 * @return int32_t amount encoder has changed since last call to getIncrement
 */
int32_t ENCODER_getIncrement(void);

/**
 * @brief Returns the current value of the rotary encoder. 
 * 
 * @return uint32_t current rotary encoder value
 */
int32_t ENCODER_getValue(void);

/**
 * @brief Sets the maximum value the encoder is allowed to hold
 * 
 * @param max max value of encoder
 */
void ENCODER_setMax(int32_t max);

/**
 * @brief Sets the current absolute value of the encoder
 * 
 * @param val value to set the encoder to
 */
void ENCODER_setValue(int32_t val);

/**
 * @brief Sets the minimum value the encoder is allowed to hold
 * 
 * @param min min value of encoder
 */
void ENCODER_setMin(int32_t min);

#endif