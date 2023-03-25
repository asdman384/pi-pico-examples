/**
 * @author - Ashutosh Singh Parmar
 * @file - mlx90614_driver_pal.h
 * @brief - This file contains declarations for platform abstraction layer APIs
 * 
 * ************************************************************************************
 * ************************************************************************************
*/

#ifndef MLX90614_DRIVER_PAL_H
#define MLX90614_DRIVER_PAL_H

#include "mlx90614_driver_common.h"

char InitializeI2C();

char WriteI2C(uint8_t, uint8_t);

char WriteI2C_mutli(uint8_t, uint8_t *, uint8_t);

char ReadI2C_multi(uint8_t, uint8_t *, uint8_t);

void DelayMS(unsigned int);

void DelayUS(unsigned int);

#endif