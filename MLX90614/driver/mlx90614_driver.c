/**
 * @author - Ashutosh Singh Parmar
 * @file - mlx90614_driver.c
 * @brief - This file contains main driver APIs for MLX90614 temperature sensor.
 * 
 * ************************************************************************************
 * ************************************************************************************
*/

#include "pico/stdlib.h"
#include "mlx90614_driver.h"

static uint8_t device_addr;

//*****************************************************************************************************************************

/**
 * @brief : This API is used for CRC8 generation
 * 
 * @params:
 * 1. uint8_t * addr :
 * 2. uint8_t len
 * 
 * @return: uint8_t 
 * The CRC checksum
*/
static uint8_t MLXcrc8(uint8_t *addr, uint8_t len)
// The PEC calculation includes all bits except the START, REPEATED START, STOP,
// ACK, and NACK bits. The PEC is a CRC-8 with polynomial X8+X2+X1+1.
{
  uint8_t crc = 0;
  while (len--) 
  {
    uint8_t inbyte = *addr++;
    for (uint8_t i = 8; i; i--)
    {
      uint8_t carry = (crc ^ inbyte) & 0x80;
      crc <<= 1;
      if (carry)
        crc ^= 0x7;
      inbyte <<= 1;
    }
  }
  return crc;
}

/**
 * @brief : This API is used to read 16 bits from the MLX6014 internal register
 * 
 * @params:
 * 1. uint8_t reg_addr : The address of the register to read from
 * 2. uint16_t val : Pointer to variable where read value will be stored
 * 
 * @return : char
 * '1' Success
 * '0' Failed
*/
static char MLXread16(uint8_t reg_addr, uint16_t * val) 
{
  uint8_t buff[3];

  //Writing the register address
  MLX_ERROR_CHECK( WriteI2C(device_addr, reg_addr) );

  //Reading the 3 bytes
  MLX_ERROR_CHECK( ReadI2C_multi(device_addr, buff, 3) );

  *(val) = buff[0];
  *(val) |= buff[1] << 8;

  return 1;
}

/**
 * @brief : This API is used to write 16 bits to MLX90614
 * 
 * @params :
 * 1. uint8_t reg_addr : The register address
 * 2. uint16_t val : The 16 bits value to be written
 * 
 * @returns : char
 * '1' Success
 * '0' Failed
*/
static char MLXwrite16(uint8_t reg_addr, uint16_t val)
{
  uint8_t pec;
  uint8_t pecbuf[4];

  uint8_t sendBuff[4];
  sendBuff[0] = reg_addr;
  sendBuff[1] = val & 0XFF;
  sendBuff[2] = val >> 8;
  

  pecbuf[0] = device_addr << 1;
  pecbuf[1] = reg_addr;
  pecbuf[2] = val & 0xff;
  pecbuf[3] = val >> 8;
  pec = MLXcrc8(pecbuf, sizeof pecbuf);

  sendBuff[3] = pec;

  MLX_ERROR_CHECK( WriteI2C_mutli(device_addr, sendBuff, 4) );

  return 1;
}

//****************************************************************************************************************************

/**
 * @brief : This funciton is used to initialize the sensor module. If 'MLX90614_I2C_INITIALIZE' macro is defined in 
 * the file 'mlx90614_driver_common.h' then, this funciton will internally call the I2C port initializtion API from PAL library. 
 * 
 * @params:
 * 1. uint8_t addr : The 7 bits I2C address of the device
 * 
 * @returns : char 
 * '0' Failed
 * '1' Sucess
*/
char InitializeMLX(uint8_t addr)
{
    #ifdef MLX90614_INITIALIZE_I2C
    MLX_ERROR_CHECK(InitializeI2C());
    #endif

    device_addr = addr;

    return 1;
}

/**
 * @brief Read the raw value from the emissivity register
 * 
 * @param : 
 * 1. uint16_t * val : Pointer to variable where value will be stored
 *
 * @returns : char
 * '1' Success
 * '0' Failed
 */
char MLXreadEmissivityReg(uint16_t * val)
{
    MLX_ERROR_CHECK( MLXread16(MLX90614_EMISS, val) );
    return 1;
}

/**
 * @brief Write the raw unscaled emissivity value to the emissivity register
 *
 * @params
 * 1. uint16_t ereg : The unscaled emissivity value
 * 
 * @returns : char
 * '1' Success
 * '0' Failed
 */
char MLXwriteEmissivityReg(uint16_t ereg) 
{
  MLX_ERROR_CHECK( MLXwrite16(MLX90614_EMISS, 0) );
  DelayMS(10);
  MLX_ERROR_CHECK( MLXwrite16(MLX90614_EMISS, ereg) );
  DelayMS(10);
  return 1;
}

/**
 * @brief Read the emissivity value from the sensor's register and scale
 * 
 * @params:
 * 1. double * val : Pointer to variable where emissivity value (ranging from 0.1 - 1.0) will be stored.
 *
 * @return : char
 * '1' Success
 * '0' Failed
 */
char MLXreadEmissivity(double * val)
{
  uint16_t ereg;

  MLX_ERROR_CHECK( MLXreadEmissivityReg(&ereg) );
  *val = ((double)ereg) / 65535.0;

  return 1;
}

/**
 * @brief Set the emissivity value
 *
 * @param 
 * 1. double emissivity : The emissivity value to use, between 0.1 and 1.0
 * 
 * @returns : char
 * '1' Success
 * '0' Failed
 */
char MLXwriteEmissivity(double emissivity)
{
  uint16_t ereg = (uint16_t)(0xffff * emissivity);

  MLX_ERROR_CHECK( MLXwriteEmissivityReg(ereg) );

  return 1;
}

/**
 * @brief : This API is used to read temperature
 * 
 * @param :
 * 1. uint8_t reg : The temperature register
 * 2. double * temp : Pointer to variable where temperature will be stored
 * 
 * @returns : char
 * '1' success
 * '0' failed
*/
char MLXreadTemp(uint8_t reg, double * temp)
{
    uint16_t val;

    MLX_ERROR_CHECK( MLXread16(reg, &val) );
    *temp = val;

    *(temp) *= .02;
    *(temp) -= 273.15;
  
    return 1;
}

/**
 * @brief Get the current temperature of an object in degrees Farenheit
 * 
 * @params:
 * 1. double * temp : Pointer to variable where object temperature will be stored
 *
 * @return : char
 * '1' Success
 * '0' Failed
 */
char MLXreadObjectTempF(double * temp) 
{
    MLX_ERROR_CHECK( MLXreadTemp(MLX90614_TOBJ1, temp) );
    *temp = ( (*temp) * 9 / 5) + 32;
    return 1;
}

/**
 * @brief Get the current ambient temperature in degrees Farenheit
 *
 * * @params:
 * 1. double * temp : Pointer to variable where ambient temperature will be stored
 * 
 * @return : char
 * '1' Success
 * '0' Failed
 */
char MLXreadAmbientTempF(double * temp) 
{
    MLX_ERROR_CHECK( MLXreadTemp(MLX90614_TA, temp) );
    *temp = ( (*temp) * 9 / 5) + 32;
    return 1;
}

/**
 * @brief Get the current ambient temperature in degrees Celcius
 *
 * * @params:
 * 1. double * temp : Pointer to variable where object temperature will be stored
 * 
 * @return : char
 * '1' Success
 * '0' Failed
 */
char MLXreadObjectTempC(double * temp) 
{
  MLX_ERROR_CHECK( MLXreadTemp(MLX90614_TOBJ1, temp) );
  return 1;
}

/**
 * @brief Get the current ambient temperature in degrees Celcius
 *
 * * @params:
 * 1. double * temp : Pointer to variable where ambient temperature will be stored
 * 
 * @return : char
 * '1' Success
 * '0' Failed
 */
char MLXreadAmbientTempC(double * temp)
{
    MLX_ERROR_CHECK( MLXreadTemp(MLX90614_TA, temp) );
    return 1;
}