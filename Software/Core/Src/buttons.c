/**
 * @file buttons.c
 * @author your name (you@domain.com)
 * @brief Configures and updates button states. Current buttons are Output Toggle, 
 *        Slew Rate, and Range Select
 * @version 0.1
 * @date 2022-05-07
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "buttons.h"
#include "string.h"
// TODO: Remove when done
// #include <stdio.h>
// #include <usart.h> 
// char out_buff[100] = {0};

const uint32_t ADC_BUTTON_THRESHOLDS[7] = {
    139, 360, 516, 725, 852, 1001, 1109};

const uint8_t ADC_VAL_BUTTON_MAP[7] = {
    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};

ButtonStates_TypeDef buttons;
ButtonStates_TypeDef buttons_prev;
ADC_HandleTypeDef* adc;

ButtonStates_TypeDef* BUTTONS_init(void){
    // Get ADC instance
    adc = HAL_ADC_getInstance();

    // Set initial values
    buttons = (const ButtonStates_TypeDef) {0};
    buttons_prev = (const ButtonStates_TypeDef) {0};;

    // Return
    return &buttons;
}

void BUTTONS_update(void) {
    uint16_t adc_val;
    uint8_t button_state = 0;

    adc_val = ADC_readChannel(ADC_BTN_CHANNEL_IDX);

    // Loop through the predefined thresholds until we find the one we're 
    // bigger than.
    for (int i = ARRAYSIZE(ADC_BUTTON_THRESHOLDS); i >= 0; i--) 
    {
        if (adc_val > ADC_BUTTON_THRESHOLDS[i]) {
            button_state = ADC_VAL_BUTTON_MAP[i];
            break;
        }
    }
    // sprintf(out_buff, "ADC Value: %lu\n\r", adc_val);
    // UART_print(out_buff);

    buttons.toggleActive   = button_state & TOGGLE_BUTTON_MASK;
    buttons.toggleChanged  = buttons.toggleActive != buttons_prev.toggleActive;
    buttons.toggleReleased = buttons.toggleChanged && !buttons.toggleActive;
    buttons.togglePressed  = buttons.toggleChanged && buttons.toggleActive;

    buttons.slewActive     = button_state & SLEW_BUTTON_MASK;
    buttons.slewChanged    = buttons.slewActive != buttons_prev.slewActive;
    buttons.slewReleased   = buttons.slewChanged && !buttons.slewActive;
    buttons.slewPressed    = buttons.slewChanged && buttons.slewActive;

    buttons.rangeActive    = button_state & RANGE_BUTTON_MASK;
    buttons.rangeChanged   = buttons.rangeActive != buttons_prev.rangeActive;
    buttons.rangeReleased  = buttons.rangeChanged && !buttons.rangeActive;
    buttons.rangePressed   = buttons.rangeChanged && buttons.rangeActive;

    // Copy states into previous states
    memcpy(&buttons_prev, &buttons, sizeof(ButtonStates_TypeDef));
}