/**
 * @file buttons.h
 * @author irex
 * @brief Configures and updates button states. Current buttons are Output Toggle, 
 *        Slew Rate, and Range Select
 * @version 0.1
 * @date 2022-05-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include "gpio.h"
#include "adc.h"
#include "stdint.h"
#include "main.h"

#define ADC_CHANNEL_BTNS ADC_CHANNEL_9
#define TOGGLE_BUTTON_MASK 0b001
#define SLEW_BUTTON_MASK   0b010
#define RANGE_BUTTON_MASK  0b100


typedef struct __ButtonStatesTypeDef {
    uint8_t toggleActive;   // Output Toggle
    uint8_t toggleChanged;  // Output Toggle
    uint8_t toggleReleased; // Output Toggle
    uint8_t togglePressed;  // Output Toggle

    uint8_t slewActive;     // Slew Rate
    uint8_t slewChanged;    // Slew Rate
    uint8_t slewReleased;   // Slew Rate
    uint8_t slewPressed;    // Slew Rate

    uint8_t rangeActive;    // Range Select
    uint8_t rangeChanged;   // Range Select
    uint8_t rangeReleased;  // Range Select
    uint8_t rangePressed;   // Range Select
} ButtonStates_TypeDef;

/**
 * @brief Initialize the buttons and required hardware. Returns a struct that 
 *        tracks the state of the buttons. Call BUTTONS_update() to update the
 *        struct.
 * 
 * @return ButtonStates_TypeDef struct of button states
 */
ButtonStates_TypeDef* BUTTONS_init(void);

/**
 * @brief Update the current state of the buttons
 * 
 */
void BUTTONS_update(void);

#endif
