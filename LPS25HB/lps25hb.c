#include "lps25hb.h"
#include <math.h>

static LPS25HB_ReadFn  read_cb  = 0;
static LPS25HB_WriteFn write_cb = 0;

void LPS25HB_RegisterCallbacks(LPS25HB_ReadFn read, LPS25HB_WriteFn write)
{
    read_cb  = read;
    write_cb = write;
}

uint8_t LPS25HB_ReadRegs(uint8_t reg, uint8_t *data, uint8_t len)
{
    if (len > 1) reg |= LPS25HB_AUTO_INCREMENT;
    return read_cb(LPS25HB_ADDR, reg, data, len);
}

uint8_t LPS25HB_WriteRegs(uint8_t reg, uint8_t *data, uint8_t len)
{
    if (len > 1) reg |= LPS25HB_AUTO_INCREMENT;
    return write_cb(LPS25HB_ADDR, reg, data, len);
}

uint8_t LPS25HB_Init(void)
{
    uint8_t who = 0;
    uint8_t value;

    if (read_cb == 0 || write_cb == 0)
        return 0;

    if (LPS25HB_ReadRegs(LPS25HB_WHO_AM_I, &who, 1) != 0) return 0;
    if (who != LPS25HB_WHO_AM_I_VALUE) return 0;

    value = LPS25HB_RES_CONF_VALUE;
    if (LPS25HB_WriteRegs(LPS25HB_RES_CONF, &value, 1) != 0) return 0;

    value = LPS25HB_CTRL1_PD | LPS25HB_CTRL1_ODR_7HZ | LPS25HB_CTRL1_BDU;
    if (LPS25HB_WriteRegs(LPS25HB_CTRL_REG1, &value, 1) != 0) return 0;

    return 1;
}

float LPS25HB_GetPressure(void)
{
    uint8_t raw[3];
    LPS25HB_ReadRegs(LPS25HB_PRESS_OUT_XL, raw, 3);

    int32_t p_out = (int32_t)(((uint32_t)raw[2] << 24) |
                              ((uint32_t)raw[1] << 16) |
                              ((uint32_t)raw[0] << 8)) >> 8;

    return (float)p_out / 4096.0f;
}

float LPS25HB_CalcAltitude(float p, float p0)
{
    return 44330.0f * (1.0f - powf(p / p0, 1.0f / 5.255f));
}
