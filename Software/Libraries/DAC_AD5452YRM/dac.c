/**
 * @file dac.c
 * @author irex
 * @brief Simple library for driving the AD5452YRM DAC
 * @version 0.1
 * @date 2022-05-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "dac.h"

SPI_HandleTypeDef* spi;

void DAC_init(void) {
    spi = SPI_getInstance();
}

void DAC_setRaw(uint16_t val) {
    // DAC input shift register for 12-bit looks like:
    // ---------------------------------------------------------------------------------
    // | C1 | C0 | B11 | B10 | B9 | B8 | B7 | B6 | B5 | B4 | B3 | B2 | B1 | B0 | x | x |
    // ---------------------------------------------------------------------------------
    // Bitshift left for the two empty LSB in the shift register
    val = val << 2;
    // Mask off the control bits (which should be 0 for falling edge clock)
    val = val & DAC_DATA_MASK;
    uint8_t val_arr[2] = {val >> 8, val & 0xFF};
    // Could be sent using 16-bit mode, but the LCD uses 8-bit and I don't think
    // It's worth the overhead. 
    HAL_GPIO_WritePin(SPI_CS_DAC_GPIO_Port, SPI_CS_DAC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit(spi, val_arr, 2, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(SPI_CS_DAC_GPIO_Port, SPI_CS_DAC_Pin, GPIO_PIN_SET);
}

void DAC_setVolts(float volts) {
    uint16_t raw_val = DAC_volts2Raw(volts);
    DAC_setRaw(raw_val);
}

uint16_t DAC_volts2Raw(float volts) {
    if (volts < 0) {
        return 0;
    } else if (volts > DAC_VREF) {
        return DAC_MAX_RAW;
    } else {
    return (uint16_t) ((volts * DAC_MAX_RAW) / DAC_VREF);
    }
}