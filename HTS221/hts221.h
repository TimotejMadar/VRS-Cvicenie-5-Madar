/*
 * hts221.h
 *
 *  Created on: 11. 11. 2026
 *      Author: tmote
 */

#ifndef HTS221_H_
#define HTS221_H_

#include <stdint.h>


#define HTS221_ADDR 0x5F

#define HTS221_WHO_AM_I 0x0F
#define HTS221_AV_CONF         0x10
#define HTS221_CTRL_REG1       0x20
#define HTS221_STATUS_REG      0x27
#define HTS221_HUMIDITY_OUT_L  0x28
#define HTS221_TEMP_OUT_L      0x2A
#define HTS221_CALIB_START     0x30


#define HTS221_WHO_AM_I_VALUE 0xBC

#define HTS221_CTRL1_PD        0x80
#define HTS221_CTRL1_BDU       0x04
#define HTS221_CTRL1_ODR_1HZ   0x01
#define HTS221_CTRL1_ODR_7HZ   0x02
#define HTS221_CTRL1_ODR_12HZ  0x03

#define HTS221_AV_CONF_VALUE   0x1B

#define HTS221_AUTO_INCREMENT  0x80

typedef uint8_t (*HTS221_ReadFn)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);
typedef uint8_t (*HTS221_WriteFn)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);


void HTS221_RegisterCallbacks(HTS221_ReadFn read, HTS221_WriteFn write);
uint8_t HTS221_Init(void);


uint8_t HTS221_ReadRegs(uint8_t reg, uint8_t *data, uint8_t len);
uint8_t HTS221_WriteRegs(uint8_t reg, uint8_t *data, uint8_t len);

float HTS221_GetTemperature(void);
float HTS221_GetHumidity(void);

#endif
