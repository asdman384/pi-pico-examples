/**
 * @author - Ashutosh Singh Parmar
 * @file - mlx90614_driver_pal.c
 * @brief - This file contains platform abstraction layer APIs that are internally used by driver APIs.
 *
 * ************************************************************************************
 * ************************************************************************************
 */

#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mlx90614_driver_pal.h"

#define I2C_PORT i2c1

/**
 * @brief - This API is used to initialize I2C port.
 *
 * @param -
 * NONE
 *
 * @return - char
 * 1 : success
 * 0 : failed
 */
char InitializeI2C()
{
    i2c_init(I2C_PORT, 100000);
    return 1;
}

/**
 * @brief - This API is used to write one byte to I2C slave device.
 *
 * @param -
 * 1. uint8_t address : Address of the device
 * 2. uint8_t data : The 8 bits data to send to I2C slave device.
 *
 * @return - char
 * 1 : success
 * 0 : failed
 *
 * @note : This function should be such that device does not send a stop condition after ending transmission.
 * The device must send repeated start after sending one byte
 */
char WriteI2C(uint8_t address, uint8_t data)
{
    i2c_write_blocking(I2C_PORT, address, &data, 1, true);
    return 1;
}

/**
 * @brief - This API is used to write two bytes to I2C slave device, in one write operation.
 *
 * @param -
 * 1. uint8_t : Address of the device
 * 2. uint8_t data : Pointer to buffer where data to send is stored
 * 3. uint8_t num : Number to data bytes to send
 *
 * @return - char
 * 1 : success
 * 0 : failed
 */
char WriteI2C_mutli(uint8_t address, uint8_t *data, uint8_t num)
{
    i2c_write_blocking(I2C_PORT, address, data, num, false);
    return 1;
}

/**
 * @brief - This API is used to read two bytes from I2C slave device, in one read operation.
 *
 * @param -
 * 1. uint8_t address : Address of the device
 * 2. uint8_t * data : Pointer to array where data read from I2C slave device will be stored.
 * 3. uint8_t num : Number of data bytes to read
 *
 * @return - char
 * 1 : success
 * 0 : failed
 */
char ReadI2C_multi(uint8_t address, uint8_t *data, uint8_t num)
{
    i2c_read_blocking(I2C_PORT, address, data, num, false);
    return 1;
}

/**
 * @brief - This API is used to delay execution for given number of milli-seconds.
 *
 * @param -
 * 1. unsigned int delay : The delay in milliseconds
 *
 * @return -
 * NOTHING
 */
void DelayMS(unsigned int dl)
{
    sleep_ms(dl);
}

/**
 * @brief - This API is used to delay execution for given number of micro-seconds.
 *
 * @param -
 * 1. unsigned int delay : The delay in microseconds
 *
 * @return -
 * NOTHING
 */
void DelayUS(unsigned int dl)
{
    sleep_us(dl);
}
