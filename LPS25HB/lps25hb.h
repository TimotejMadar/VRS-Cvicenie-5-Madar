#ifndef LPS25HB_H
#define LPS25HB_H

#include <stdint.h>

#define LPS25HB_ADDR 0x5D

#define LPS25HB_WHO_AM_I 0x0F
#define LPS25HB_RES_CONF 0x10
#define LPS25HB_CTRL_REG1 0x20
#define LPS25HB_STATUS_REG 0x27
#define LPS25HB_PRESS_OUT_XL 0x28

#define LPS25HB_WHO_AM_I_VALUE 0xBD

#define LPS25HB_CTRL1_PD 0x80
#define LPS25HB_CTRL1_ODR_1HZ 0x10
#define LPS25HB_CTRL1_ODR_7HZ 0x20
#define LPS25HB_CTRL1_ODR_12HZ 0x30
#define LPS25HB_CTRL1_ODR_25HZ 0x40
#define LPS25HB_CTRL1_BDU 0x04

#define LPS25HB_RES_CONF_VALUE 0x0F

#define LPS25HB_AUTO_INCREMENT 0x80

typedef uint8_t (*LPS25HB_ReadFn)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);
typedef uint8_t (*LPS25HB_WriteFn)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);

void    LPS25HB_RegisterCallbacks(LPS25HB_ReadFn read, LPS25HB_WriteFn write);
uint8_t LPS25HB_Init(void);

uint8_t LPS25HB_ReadRegs(uint8_t reg, uint8_t *data, uint8_t len);
uint8_t LPS25HB_WriteRegs(uint8_t reg, uint8_t *data, uint8_t len);

float   LPS25HB_GetPressure(void);

float   LPS25HB_CalcAltitude(float p, float p0);

#endif
