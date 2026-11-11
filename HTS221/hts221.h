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
#define HTS221_WHO_AM_I_VALUE 0xBC


typedef uint8_t (*HTS221_ReadFn)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);
typedef uint8_t (*HTS221_WriteFn)(uint8_t dev_addr, uint8_t reg, uint8_t *data, uint8_t len);


void    HTS221_RegisterCallbacks(HTS221_ReadFn read, HTS221_WriteFn write);
uint8_t HTS221_Init(void);

#endif
