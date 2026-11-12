/*
 * hts221.c
 *
 *  Created on: 11. 11. 2026
 *      Author: tmote
 */


#include "hts221.h"


static HTS221_ReadFn  read_cb  = 0;
static HTS221_WriteFn write_cb = 0;


static float   T0_degC, T1_degC;
static int16_t T0_out,  T1_out;
static float   H0_rh,   H1_rh;
static int16_t H0_out,  H1_out;

void HTS221_RegisterCallbacks(HTS221_ReadFn read, HTS221_WriteFn write)
{
    read_cb  = read;
    write_cb = write;
}


uint8_t HTS221_ReadRegs(uint8_t reg, uint8_t *data, uint8_t len)
{
    if (len > 1) reg |= HTS221_AUTO_INCREMENT;
    return read_cb(HTS221_ADDR, reg, data, len);
}

uint8_t HTS221_WriteRegs(uint8_t reg, uint8_t *data, uint8_t len)
{
    if (len > 1) reg |= HTS221_AUTO_INCREMENT;
    return write_cb(HTS221_ADDR, reg, data, len);
}

static uint8_t HTS221_ReadCalibration(void)
{
    uint8_t c[16];

    if (HTS221_ReadRegs(HTS221_CALIB_START, c, 16) != 0)
        return 1;

    H0_rh = c[0] / 2.0f;
    H1_rh = c[1] / 2.0f;

    uint16_t t0_x8 = ((c[5] & 0x03) << 8) | c[2];
    uint16_t t1_x8 = ((c[5] & 0x0C) << 6) | c[3];
    T0_degC = t0_x8 / 8.0f;
    T1_degC = t1_x8 / 8.0f;

    H0_out = (int16_t)((c[7]  << 8) | c[6]);
    H1_out = (int16_t)((c[11] << 8) | c[10]);
    T0_out = (int16_t)((c[13] << 8) | c[12]);
    T1_out = (int16_t)((c[15] << 8) | c[14]);

    return 0;
}



uint8_t HTS221_Init(void)
{
	uint8_t who = 0;
	    uint8_t value;

	    if (read_cb == 0 || write_cb == 0)
	        return 0;

	    if (HTS221_ReadRegs(HTS221_WHO_AM_I, &who, 1) != 0) return 0;

	    if (who != HTS221_WHO_AM_I_VALUE) return 0;

	    value = HTS221_AV_CONF_VALUE;
	    if (HTS221_WriteRegs(HTS221_AV_CONF, &value, 1) != 0) return 0;

	    value = HTS221_CTRL1_PD | HTS221_CTRL1_BDU | HTS221_CTRL1_ODR_1HZ;
	    if (HTS221_WriteRegs(HTS221_CTRL_REG1, &value, 1) != 0) return 0;

	    if (HTS221_ReadCalibration() != 0) return 0;

	    return 1;
}


float HTS221_GetTemperature(void)
{
    uint8_t raw[2];
    HTS221_ReadRegs(HTS221_TEMP_OUT_L, raw, 2);
    int16_t t_out = (int16_t)((raw[1] << 8) | raw[0]);

    return T0_degC + (float)(t_out - T0_out) * (T1_degC - T0_degC) / (float)(T1_out - T0_out);
}

float HTS221_GetHumidity(void)
{
    uint8_t raw[2];
    HTS221_ReadRegs(HTS221_HUMIDITY_OUT_L, raw, 2);
    int16_t h_out = (int16_t)((raw[1] << 8) | raw[0]);

    float h = H0_rh + (float)(h_out - H0_out) * (H1_rh - H0_rh) / (float)(H1_out - H0_out);

    if (h < 0.0f)   h = 0.0f;
    if (h > 100.0f) h = 100.0f;
    return h;
}
