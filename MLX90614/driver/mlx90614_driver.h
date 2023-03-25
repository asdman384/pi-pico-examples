/**
 * @author - Ashutosh Singh Parmar
 * @file - mlx90614_driver.h
 * @brief - This file contains declarations for main driver APIs for MLX90614 temperature sensor.
 * 
 * ************************************************************************************
 * ************************************************************************************
*/
#ifndef ML90614_DRIVER_H
#define ML90614_DRIVER_H

#include "mlx90614_driver_pal.h"

// default device address of MLX90614 sensor
#define MLX90614_I2CADDR 0x5A

// RAM
#define MLX90614_RAWIR1 0x04
#define MLX90614_RAWIR2 0x05
#define MLX90614_TA 0x06
#define MLX90614_TOBJ1 0x07
#define MLX90614_TOBJ2 0x08
// EEPROM
#define MLX90614_TOMAX 0x20
#define MLX90614_TOMIN 0x21
#define MLX90614_PWMCTRL 0x22
#define MLX90614_TARANGE 0x23
#define MLX90614_EMISS 0x24
#define MLX90614_CONFIG 0x25
#define MLX90614_ADDR 0x2E
#define MLX90614_ID1 0x3C
#define MLX90614_ID2 0x3D
#define MLX90614_ID3 0x3E
#define MLX90614_ID4 0x3F

#define MLX_ERROR_CHECK(err) if(!err)return 0

char InitializeMLX(uint8_t );

char MLXreadEmissivityReg(uint16_t *);

char MLXwriteEmissivityReg(uint16_t );

char MLXreadEmissivity(double * );

char MLXwriteEmissivity(double );

char MLXreadTemp(uint8_t , double * );

char MLXreadObjectTempF(double * );

char MLXreadAmbientTempF(double * );

char MLXreadObjectTempC(double * );

char MLXreadAmbientTempC(double * );

#endif
