/**
 * @file encoder.c
 * @author irex
 * @brief Basic library for reading rotary encoder
 * @version 0.1
 * @date 2022-05-08
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "encoder.h"

#define DELTA 1 // Change by DELTA every pulse of the encoder

volatile int32_t _encoder_value;
volatile int32_t _encoder_delta;
int32_t _min_value;
int32_t _max_value;

static void _ENCODER_clampValue(void);

void ENCODER_init(int32_t min_value, int32_t max_value) {
    _encoder_value = 0;
    // Configure the GPIO pins for dual edge interrupt
    HAL_GPIO_DeInit(GPIOC, ENCODER_A_Pin | ENCODER_B_Pin);

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = ENCODER_A_Pin | ENCODER_B_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
    GPIO_InitStruct.Pull = GPIO_PULLUP;

    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Enable interrupt on the encoder pins
    HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);

    _min_value = min_value;
    _max_value = max_value;
}

int32_t ENCODER_getValue(void) {
    return _encoder_value;
}

int32_t ENCODER_getIncrement(void) {
    int32_t temp = _encoder_delta;
    _encoder_delta = 0;
    return temp;
}

void ENCODER_setMax(int32_t max) {
    _max_value = max;
    _ENCODER_clampValue();
}

void ENCODER_setMin(int32_t min) {
    _min_value = min;
    _ENCODER_clampValue();
}

void ENCODER_setValue(int32_t val) {
    _encoder_value = val;
    _ENCODER_clampValue();
}

// Interupt handler
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    uint32_t channel_a = HAL_GPIO_ReadPin(ENCODER_A_GPIO_Port, ENCODER_A_Pin);
    uint32_t channel_b = HAL_GPIO_ReadPin(ENCODER_B_GPIO_Port, ENCODER_B_Pin);

    // If interrupted on edge of channel A
    if (GPIO_Pin == ENCODER_A_Pin) {
        if (channel_a != channel_b) {
            _encoder_value += DELTA;
            _encoder_delta += DELTA;
        } else {
            _encoder_value -= DELTA;
            _encoder_delta -= DELTA;
        }
    } else { // If interrupted on edge of Pin B
        if (channel_a == channel_b) {
            _encoder_value += DELTA;
            _encoder_delta += DELTA;
        } else {
            _encoder_value -= DELTA;
            _encoder_delta -= DELTA;
        }
    }

    _ENCODER_clampValue();
}

static void _ENCODER_clampValue(void) {
    if (_encoder_value < _min_value) {
        _encoder_value = _min_value;
    }
    if (_encoder_value > _max_value) {
        _encoder_value = _max_value;
    }
}