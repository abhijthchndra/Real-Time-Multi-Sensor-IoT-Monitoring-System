/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f7xx_hal_msp.c
  * @brief        MSP Initialisation and De-Initialisation
  *
  *  Added:  HAL_ADC_MspInit / HAL_ADC_MspDeInit
  *          Configures PC0 as ADC1_IN10 (MQ gas sensor analogue input)
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/* ──────────────────────────────────────────────────────────────────────────
 *  Global MSP Init
 * ────────────────────────────────────────────────────────────────────────── */
void HAL_MspInit(void)
{
    /* USER CODE BEGIN MspInit 0 */
    /* USER CODE END MspInit 0 */

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_RCC_SYSCFG_CLK_ENABLE();

    /* USER CODE BEGIN MspInit 1 */
    /* USER CODE END MspInit 1 */
}

/* ──────────────────────────────────────────────────────────────────────────
 *  ADC MSP Init / DeInit  (NEW – MQ Gas Sensor on PC0 → ADC1_IN10)
 * ────────────────────────────────────────────────────────────────────────── */

/**
  * @brief  ADC MSP Initialization
  *         Enables ADC1 peripheral clock and configures PC0 as analogue pin.
  * @param  hadc  ADC handle pointer
  */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (hadc->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspInit 0 */
        /* USER CODE END ADC1_MspInit 0 */

        /* ADC1 peripheral clock */
        __HAL_RCC_ADC1_CLK_ENABLE();

        /* GPIOC clock (already enabled in MX_GPIO_Init, but safe to repeat) */
        __HAL_RCC_GPIOC_CLK_ENABLE();

        /**
         * ADC1 GPIO Configuration
         * PC0  ─────────────────→  ADC1_IN10   (MQ sensor AO)
         *
         * Wire:  MQ-module AO ─── PC0 (Arduino A1 header on Nucleo-F767ZI)
         * Note:  No pull resistor – analogue mode, internal pull disabled.
         */
        GPIO_InitStruct.Pin  = GPIO_PIN_0;
        GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

        /* USER CODE BEGIN ADC1_MspInit 1 */
        /* USER CODE END ADC1_MspInit 1 */
    }
}

/**
  * @brief  ADC MSP De-Initialization
  * @param  hadc  ADC handle pointer
  */
void HAL_ADC_MspDeInit(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        /* USER CODE BEGIN ADC1_MspDeInit 0 */
        /* USER CODE END ADC1_MspDeInit 0 */

        __HAL_RCC_ADC1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0);

        /* USER CODE BEGIN ADC1_MspDeInit 1 */
        /* USER CODE END ADC1_MspDeInit 1 */
    }
}

/* ──────────────────────────────────────────────────────────────────────────
 *  TIM3 MSP Init / DeInit  (unchanged – HC-SR04 echo on PB1 → TIM3_CH4)
 * ────────────────────────────────────────────────────────────────────────── */

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    if (htim_base->Instance == TIM3)
    {
        /* USER CODE BEGIN TIM3_MspInit 0 */
        /* USER CODE END TIM3_MspInit 0 */

        __HAL_RCC_TIM3_CLK_ENABLE();
        __HAL_RCC_GPIOB_CLK_ENABLE();

        /**
         * TIM3 GPIO Configuration
         * PB1  ─────────────────→  TIM3_CH4   (HC-SR04 ECHO)
         */
        GPIO_InitStruct.Pin       = GPIO_PIN_1;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
        GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* TIM3 interrupt */
        HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(TIM3_IRQn);

        /* USER CODE BEGIN TIM3_MspInit 1 */
        /* USER CODE END TIM3_MspInit 1 */
    }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
{
    if (htim_base->Instance == TIM3)
    {
        /* USER CODE BEGIN TIM3_MspDeInit 0 */
        /* USER CODE END TIM3_MspDeInit 0 */

        __HAL_RCC_TIM3_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_1);
        HAL_NVIC_DisableIRQ(TIM3_IRQn);

        /* USER CODE BEGIN TIM3_MspDeInit 1 */
        /* USER CODE END TIM3_MspDeInit 1 */
    }
}

/* ──────────────────────────────────────────────────────────────────────────
 *  UART MSP Init / DeInit  (unchanged – USART3 on PD8/PD9)
 * ────────────────────────────────────────────────────────────────────────── */

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef       GPIO_InitStruct    = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

    if (huart->Instance == USART3)
    {
        /* USER CODE BEGIN USART3_MspInit 0 */
        /* USER CODE END USART3_MspInit 0 */

        PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
        PeriphClkInitStruct.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
        if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
            Error_Handler();

        __HAL_RCC_USART3_CLK_ENABLE();
        __HAL_RCC_GPIOD_CLK_ENABLE();

        /**
         * USART3 GPIO Configuration
         * PD8  ─────────────────→  USART3_TX
         * PD9  ─────────────────→  USART3_RX
         */
        GPIO_InitStruct.Pin       = GPIO_PIN_8 | GPIO_PIN_9;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_NOPULL;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* USER CODE BEGIN USART3_MspInit 1 */
        /* USER CODE END USART3_MspInit 1 */
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART3)
    {
        /* USER CODE BEGIN USART3_MspDeInit 0 */
        /* USER CODE END USART3_MspDeInit 0 */

        __HAL_RCC_USART3_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_8 | GPIO_PIN_9);

        /* USER CODE BEGIN USART3_MspDeInit 1 */
        /* USER CODE END USART3_MspDeInit 1 */
    }
}

/* USER CODE BEGIN 1 */
/* USER CODE END 1 */
