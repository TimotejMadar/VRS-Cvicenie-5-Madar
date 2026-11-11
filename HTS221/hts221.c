/*
 * hts221.c
 *
 *  Created on: 11. 11. 2026
 *      Author: tmote
 */


#include "hts221.h"


static HTS221_ReadFn  read_cb  = 0;
static HTS221_WriteFn write_cb = 0;

void HTS221_RegisterCallbacks(HTS221_ReadFn read, HTS221_WriteFn write)
{
    read_cb  = read;
    write_cb = write;
}

uint8_t HTS221_Init(void)
{
    uint8_t who = 0;

    if (read_cb == 0 || write_cb == 0)
        return 0;

    if (read_cb(HTS221_ADDR, HTS221_WHO_AM_I, &who, 1) != 0)
        return 0;

    if (who != HTS221_WHO_AM_I_VALUE)
        return 0;


    return 1;
}
