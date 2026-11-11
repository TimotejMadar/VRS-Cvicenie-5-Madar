/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  /**I2C1 GPIO Configuration
  PB6   ------> I2C1_SCL
  PB7   ------> I2C1_SDA
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C1);
  LL_I2C_DisableOwnAddress2(I2C1);
  LL_I2C_DisableGeneralCall(I2C1);
  LL_I2C_EnableClockStretching(I2C1);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x00201D2B;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C1, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/* USER CODE BEGIN 1 */
#define I2C_TIMEOUT  10000U   // počet pokusov, kým to vzdáme (cca 10 ms)

/* Čaká, kým sa v registri ISR nastaví zadaný flag.
 * Vráti 0 = OK, 1 = chyba (senzor neodpovedal alebo vypršal čas). */
static uint8_t I2C1_WaitFlag(uint32_t flag)
{
    uint32_t timeout = I2C_TIMEOUT;

    while ((I2C1->ISR & flag) == 0)
    {
        if (I2C1->ISR & I2C_ISR_NACKF)      // senzor povedal "nie" (NACK)
        {
            LL_I2C_ClearFlag_NACK(I2C1);
            LL_I2C_ClearFlag_STOP(I2C1);    // pri NACK hardvér sám pošle STOP
            return 1;
        }
        if (--timeout == 0)                 // čakáme príliš dlho
        {
            return 1;
        }
    }
    return 0;
}


/* Prečíta 'len' bajtov zo senzora, začínajúc registrom 'reg'.
 * dev_addr = 7-bitová adresa senzora (napr. 0x5F)
 * Vráti 0 = OK, 1 = chyba. */
uint8_t I2C1_Read(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len)
{
    /* Fáza 1: START + adresa (zápis) + číslo registra, bez STOP */
    LL_I2C_HandleTransfer(I2C1, dev_addr << 1, LL_I2C_ADDRSLAVE_7BIT, 1,
                          LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

    if (I2C1_WaitFlag(I2C_ISR_TXIS)) return 1;
    LL_I2C_TransmitData8(I2C1, reg);
    if (I2C1_WaitFlag(I2C_ISR_TC)) return 1;

    /* Fáza 2: RESTART + adresa (čítanie), prijmeme 'len' bajtov, potom auto STOP */
    LL_I2C_HandleTransfer(I2C1, dev_addr << 1, LL_I2C_ADDRSLAVE_7BIT, len,
                          LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_RESTART_7BIT_READ);

    for (uint8_t i = 0; i < len; i++)
    {
        if (I2C1_WaitFlag(I2C_ISR_RXNE)) return 1;
        data[i] = LL_I2C_ReceiveData8(I2C1);
    }

    if (I2C1_WaitFlag(I2C_ISR_STOPF)) return 1;
    LL_I2C_ClearFlag_STOP(I2C1);
    return 0;
}

/* USER CODE END 1 */

