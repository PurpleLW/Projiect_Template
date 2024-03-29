/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "Template.h"

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
	
  HAL_GPIO_WritePin(GPIOC, MOS12V_1_Pin|MOS12V_2_Pin|MOS12V_3_Pin|MOS12V_4_Pin
                          |MOS12V_5_Pin|MOS12V_6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
<<<<<<< HEAD
  HAL_GPIO_WritePin(GPIOB, RS485_S0_Pin|RS485_S1_Pin|GPIO_PIN_15, GPIO_PIN_RESET);
=======
  HAL_GPIO_WritePin(GPIOB, RS485_S0_Pin|RS485_S1_Pin, GPIO_PIN_RESET);
>>>>>>> e037807c5270ce72380d7a6187a961266f84d5ff

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, Gear_L_Pin|Gear_R_Pin, GPIO_PIN_RESET);

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

  /*Configure GPIO pins : PCPin PCPin PCPin PCPin
                           PCPin PCPin */
	
//	#if SWITCH_1_flag == 1
//		GPIO_InitStruct.Pin |= MOS12V_1_Pin;
//	#endif
//	
//	#if SWITCH_2_flag == 1
//		GPIO_InitStruct.Pin |= MOS12V_2_Pin;
//	#endif

//	#if SWITCH_3_flag == 1
//		GPIO_InitStruct.Pin |= MOS12V_3_Pin;
//	#endif

//	#if SWITCH_4_flag == 1
//		GPIO_InitStruct.Pin |= MOS12V_4_Pin;
//	#endif

//	#if SWITCH_5_flag == 1
//		GPIO_InitStruct.Pin |= MOS12V_5_Pin;
//	#endif

//	#if SWITCH_6_flag == 1
//		GPIO_InitStruct.Pin |= MOS12V_6_Pin;
//	#endif
  GPIO_InitStruct.Pin = MOS12V_1_Pin|MOS12V_2_Pin|MOS12V_3_Pin|MOS12V_4_Pin
                          |MOS12V_5_Pin|MOS12V_6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PBPin PBPin */
<<<<<<< HEAD
  GPIO_InitStruct.Pin = RS485_S0_Pin|RS485_S1_Pin|GPIO_PIN_15;
=======
  GPIO_InitStruct.Pin = RS485_S0_Pin|RS485_S1_Pin;
>>>>>>> e037807c5270ce72380d7a6187a961266f84d5ff
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PAPin PAPin */
  GPIO_InitStruct.Pin = Gear_L_Pin|Gear_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
