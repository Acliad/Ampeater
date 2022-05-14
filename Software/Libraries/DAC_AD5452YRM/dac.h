/**
 * @file dac.h
 * @author irex
 * @brief Simple library for driving the AD5452YRM DAC
 * @version 0.1
 * @date 2022-05-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __DAC_H__
#define __DAC_H__

#include "main.h"
#include "spi.h"

#define DAC_VREF 1.024 
#define DAC_NUM_BITS 12
#define DAC_MAX_RAW ((1<<DAC_NUM_BITS) - 1)
#define DAC_DATA_MASK 0x3FFF

/**
 * @brief Initialze the DAC library
 * 
 */
void DAC_init(void);

/**
 * @brief Write the given value to the DAC. Value should be 12-bits
 * 
 * @param val 12-bit value to write to DAC
 */
void DAC_setRaw(uint16_t val);

/**
 * @brief Set the DAC output to the given voltage
 * 
 * @param volts voltage to set DAC output to
 */
void DAC_setVolts(float volts);

/**
 * @brief Converts the given voltage value into the closest raw value for a 12-bit DAC
 * 
 * @param volts value to convert
 * @return uint16_t converted value
 */
uint16_t DAC_volts2Raw(float volts);
#endif