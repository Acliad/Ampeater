/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "5x5_font.h"
#include "ILI9341_GFX.h"
#include "ILI9341_STM32_Driver.h"
#include "buttons.h"
#include "encoder.h"
#include "dac.h"
#include <stdio.h>
#include <string.h>


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define RS_HIGH_RANGE_OHMS  0.0909
#define RS_LOW_RANGE_OHMS   1.000
#define ISET_MAX_HIGH_AMPS  10.00
#define RES_HIGH_RANGE_AMPS 0.01 // Current set/read resolution in HIGH range
#define RES_LOW_RANGE_AMPS  0.001 // Current set/read resolution in LOW range
#define ISET_MAX_LOW_AMPS   1.000
#define ENCODER_MAX_VAL_HIGH ((uint32_t) (ENCODER_TICKS_PER_DETENT * \
                                         (ISET_MAX_HIGH_AMPS / RES_HIGH_RANGE_AMPS)))
#define ENCODER_MAX_VAL_LOW  ((uint32_t) (ENCODER_TICKS_PER_DETENT * \
                                         (ISET_MAX_LOW_AMPS / RES_LOW_RANGE_AMPS)))
#define ENCODER_MIN_VAL 0


#define NUM_DISPLAYED_DIGITS 4 // Max number of digits to display for readout
#define FORMATTER_HIGH_RNG "%d.%02d"
#define FORMATTER_LOW_RNG  "%d.%03d"
#define VALUE_CHAR_SCALER 4
#define INFO_CHAR_SCALER 3
// Y distance between new lines (in pixels)
#define VALUE_LCD_Y_OFFSET (CHAR_HEIGHT * VALUE_CHAR_SCALER + 5)
#define INFO_LCD_Y_OFFSET (CHAR_HEIGHT * INFO_CHAR_SCALER + 5)
// Start position for data labels
#define IMEAS_TXT_LCD_X_START 10 
#define IMEAS_TXT_LCD_Y_START 5 
#define ISET_TXT_LCD_X_START  10 
#define ISET_TXT_LCD_Y_START  (IMEAS_TXT_LCD_Y_START + \
                                VALUE_LCD_Y_OFFSET + INFO_LCD_Y_OFFSET)
// Start position for imeasured display data
#define IMEASURED_LCD_X_START (IMEAS_TXT_LCD_X_START)
#define IMEASURED_LCD_Y_START (IMEAS_TXT_LCD_Y_START + INFO_LCD_Y_OFFSET - 5)
// Start position for iset display data
#define ISET_LCD_X_START (ISET_TXT_LCD_X_START)
#define ISET_LCD_Y_START (ISET_TXT_LCD_Y_START + INFO_LCD_Y_OFFSET - 5)
#define OUTPUT_LCD_X_START 5
#define OUTPUT_LCD_Y_START 160
#define OUTPUT_ON_COLOR   GREEN
#define OUTPUT_OFF_COLOR  0x80E1
#define RANGE_LCD_X_START 165
#define RANGE_LCD_Y_START OUTPUT_LCD_Y_START
#define RANGE_HIGH_COLOR  ORANGE
#define RANGE_LOW_COLOR   CYAN
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define USE_LCD
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
typedef enum {
    LOW_RANGE = 1,
    HIGH_RANGE = 0
} RangeMode_t;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void displayInfo(uint32_t output_on, uint32_t range_mode, 
                    float iset, float imeasured);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    ButtonStates_TypeDef *buttons;
    ADC_HandleTypeDef *   adc;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
    ENCODER_init(ENCODER_MIN_VAL, ENCODER_MAX_VAL_LOW);
    DAC_init();
    buttons = BUTTONS_init();
    adc = HAL_ADC_getInstance();
    HAL_ADCEx_Calibration_Start(adc, ADC_SINGLE_ENDED);
    char out_buff[100] = {0};
    uint32_t output_on = 0;
    uint16_t vsense_raw = 0; // ADC reading from Vsense node
    uint16_t vfet_raw = 0; // ADC reading from VFET node
    RangeMode_t range_mode = LOW_RANGE;
    float vref = 3.3; // V TODO: Calculate using Vrefint
    float vRs = 0.0; // Voltage across sense resistor
    float imeasured = 0.0; // Measured sink current
    float iset = 0.0; // Set current
    float vset = 0.0; // DAC set voltage

#ifdef USE_LCD
    HAL_UART_MspDeInit(UART_getInstance(UART2));
    ILI9341_Init(); // initial driver setup to drive ili9341
    ILI9341_Set_Rotation(SCREEN_HORIZONTAL_2);
    ILI9341_Fill_Screen(BLACK);
    ILI9341_Draw_Text("Measured", 
                        IMEAS_TXT_LCD_X_START, IMEAS_TXT_LCD_Y_START,
                        WHITE, INFO_CHAR_SCALER, BLACK);
    ILI9341_Draw_Text("Set", 
                        ISET_TXT_LCD_X_START, ISET_TXT_LCD_Y_START,
                        WHITE, INFO_CHAR_SCALER, BLACK);
    ILI9341_Draw_Text("Output", 
                        OUTPUT_LCD_X_START, OUTPUT_LCD_Y_START,
                        WHITE, INFO_CHAR_SCALER, BLACK);
    ILI9341_Draw_Text("Range", 
                        RANGE_LCD_X_START, RANGE_LCD_Y_START,
                        WHITE, INFO_CHAR_SCALER, BLACK);
#endif

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1) {
        /***************************\
         *  Process button presses *
        \***************************/
        BUTTONS_update();

        // ---------- Range Selection ---------- 
        if (buttons->rangePressed) {
            if (range_mode == HIGH_RANGE) {
                // Set to LOW mode
                range_mode = LOW_RANGE;
                HAL_GPIO_WritePin(RANGE_SW_GPIO_Port, RANGE_SW_Pin, LOW_RANGE);
                // Scale the encoder value so that the output current stays the 
                // same (or clamps)
                ENCODER_setValue(ENCODER_getValue() * 
                                    ISET_MAX_HIGH_AMPS / ISET_MAX_LOW_AMPS);
            } else {
                // Set to HIGH mode
                range_mode = HIGH_RANGE;
                HAL_GPIO_WritePin(RANGE_SW_GPIO_Port, RANGE_SW_Pin, HIGH_RANGE);
                // Scale the encoder value so that the output current stays the 
                // same (or clamps)
                ENCODER_setValue(ENCODER_getValue() * 
                                    ISET_MAX_LOW_AMPS / ISET_MAX_HIGH_AMPS);
            }
        }

        // ---------- Output Toggle ---------- 
        if (buttons->togglePressed) output_on = !output_on;

        // ---------- Set Slew Rate ---------- 
        // TODO: Implement slew rate limiting

        /**************************\
         *    Update Iset value   *
        \**************************/
        if (range_mode == LOW_RANGE) {
            iset = ((float) ENCODER_getValue() / ENCODER_MAX_VAL_LOW) * ISET_MAX_LOW_AMPS;
            vset = iset * RS_LOW_RANGE_OHMS;
        } else {
            iset = ((float) ENCODER_getValue() / ENCODER_MAX_VAL_HIGH) * ISET_MAX_HIGH_AMPS;
            vset = iset * RS_HIGH_RANGE_OHMS;
        } 
        DAC_setVolts((output_on ? vset : 0.000));

        /**************************\
         *  Calcluate Isink value *
        \**************************/
        vsense_raw = ADC_readChannel(ADC_VSENSE_CHANNEL_IDX);
        vfet_raw = ADC_readChannel(ADC_VFET_CHANNEL_IDX);
        vRs = vref * (vsense_raw - vfet_raw) / ADC_MAX_READING;
        imeasured = vRs / 
            (range_mode == LOW_RANGE ? RS_LOW_RANGE_OHMS : RS_HIGH_RANGE_OHMS);
        
        /**************************\
         *     Update  display    *
        \**************************/
        displayInfo(output_on, range_mode, iset, imeasured);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
 * @brief Updates the user on the system states. iset and imeasured will always
 * display 4 digits. If range_mode is LOW, the output format will be x.xxx. If
 * range_mode is HIGH, the output will be xx.xx
 * 
 * @param output_on output state
 * @param range_mode output range
 * @param iset set output current
 * @param imeasured measured output current
 */
void displayInfo(uint32_t output_on, uint32_t range_mode, 
                    float iset, float imeasured) 
{
    static uint32_t output_on_last = 1;
    static uint32_t range_last = HIGH_RANGE;
    // Track digits so we can only redraw what's new. len = NUM_DIGITS + '.' + '\0' 
    static char imes_strbuff_last[NUM_DISPLAYED_DIGITS + 2] = {0}; 
    static char iset_strbuff_last[NUM_DISPLAYED_DIGITS + 2] = {0}; 

    char i_strbuff[NUM_DISPLAYED_DIGITS + 2]; // NUM_DIGITS + '.' + '\0' 
    uint32_t whole_part;
    uint32_t frac_part;
    // Precision factor is used to determine how many fractional digits to use
    uint32_t precision_factor = (range_mode == HIGH_RANGE) ? 100 : 1000;

    
    // Compute and display iset and imeasured
    const char* formatter = (range_mode == HIGH_RANGE) ? 
                                    FORMATTER_HIGH_RNG : FORMATTER_LOW_RNG;
    // ---------- imeasured ---------- 
    whole_part = (uint32_t) imeasured;
    frac_part = ((uint32_t) (imeasured * precision_factor)) % precision_factor;
    sprintf(i_strbuff, formatter, whole_part, frac_part);

    uint32_t cursor_x = IMEASURED_LCD_X_START;
    uint32_t cursor_y = IMEASURED_LCD_Y_START;
    for (int i = 0; i < NUM_DISPLAYED_DIGITS + 1; i++) {
        if (i_strbuff[i] != imes_strbuff_last[i]) {
            ILI9341_Draw_Char(i_strbuff[i], cursor_x, cursor_y, WHITE, VALUE_CHAR_SCALER, BLACK);
        }
        if (i_strbuff[i] == '.') {
            cursor_x += CHAR_WIDTH * VALUE_CHAR_SCALER * 1;
        } else {
            cursor_x += CHAR_WIDTH * VALUE_CHAR_SCALER;
        }

        imes_strbuff_last[i] = i_strbuff[i];
    }

    // ---------- iset ---------- 
    whole_part = (uint32_t) iset;
    frac_part = ((uint32_t) (iset * precision_factor)) % precision_factor;
    sprintf(i_strbuff, formatter, whole_part, frac_part);

    cursor_x = ISET_LCD_X_START;
    cursor_y = ISET_LCD_Y_START;
    for (int i = 0; i < NUM_DISPLAYED_DIGITS + 1; i++) {
        if (i_strbuff[i] != iset_strbuff_last[i]) {
            ILI9341_Draw_Char(i_strbuff[i], cursor_x, cursor_y, WHITE, VALUE_CHAR_SCALER, BLACK);
        }
        if (i_strbuff[i] == '.') {
            cursor_x += CHAR_WIDTH * VALUE_CHAR_SCALER * 1;
        } else {
            cursor_x += CHAR_WIDTH * VALUE_CHAR_SCALER;
        }

        iset_strbuff_last[i] = i_strbuff[i];
    }

    // Draw output state
    if (output_on != output_on_last) {
        uint32_t color = output_on == 1 ? OUTPUT_ON_COLOR : OUTPUT_OFF_COLOR;
        ILI9341_Draw_Text(output_on == 1 ? "ON " : "OFF", 
                            OUTPUT_LCD_X_START, OUTPUT_LCD_Y_START + INFO_LCD_Y_OFFSET,
                            color, VALUE_CHAR_SCALER, BLACK);
        output_on_last = output_on;
    }

    // Draw range state
    if (range_mode != range_last) {
        uint32_t color = range_mode == HIGH_RANGE ? 
                                        RANGE_HIGH_COLOR : RANGE_LOW_COLOR;
        ILI9341_Draw_Text(range_mode == HIGH_RANGE ? "High" : "Low ", 
                            RANGE_LCD_X_START, RANGE_LCD_Y_START + INFO_LCD_Y_OFFSET,
                            color, VALUE_CHAR_SCALER, BLACK);
        range_last = range_mode;
    }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1) {
    }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
