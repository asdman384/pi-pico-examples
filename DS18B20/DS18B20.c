#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/timer.h"
#include "OneWire.h"

typedef struct DS18B20dev
{
    uint16_t family_code;
    uint16_t rom_crc;
    uint64_t serial_num;
    uint16_t temperature;
    uint8_t alarm_th;
    uint8_t alarm_tl;
    uint8_t config;
} DS18B20dev_t;

char txt[32];

int search_DS18_rom(DS18B20dev_t *devices[])
{
    uint64_t roms[16];
    int num_roms = oneWire_search_rom(roms);
    for (int i = 0; i < num_roms; i++)
    {
        devices[i] = (DS18B20dev_t *)malloc(sizeof(DS18B20dev_t));
        devices[i]->family_code = roms[i] & 0xFF;
        devices[i]->serial_num = roms[i] >> 8 & 0xFFFFFFFFFFFF;
        devices[i]->rom_crc = roms[i] >> 56 & 0xFF;
    }
    return num_roms;
}

void send_DS18_skip_rom()
{
    // send the skip rom command
    oneWire_write_byte(0xCC, true);
}

// Tells all devices to start the temperature conversion
// if conversion didn't start returns false.
// if wait is  true, the function will wait until
// the temperature conversion is completed. Returns false if timeout
bool convert_DS18_temp(bool wait)
{
    // send the skip rom command
    send_DS18_skip_rom();
    // send the convert temp command
    oneWire_write_byte(0x44, true);
    // wait a few microseconds to see if conversions started
    busy_wait_us_32(100);
    oneWire_wait_for_idle(true);

    return true;
}

void send_DS18_match_rom(DS18B20dev_t *dev)
{
    union
    {
        uint8_t a[8];
        uint64_t d;
    } u;
    // put the rom data together
    u.d = dev->serial_num << 8;
    u.a[0] = dev->family_code;
    u.a[7] = dev->rom_crc;
    // send the match rom command
    oneWire_write_byte(0x55, true);
    for (int i = 0; i < 8; i += 2)
    {
        oneWire_write_uint(((uint16_t)u.a[i + 1] << 8) + u.a[i], true);
    }
    // sprintf(txt, "\n%016llx", u.d);
    // print_d(txt);
}

bool get_DS18_scratch(DS18B20dev_t *dev)
{

    // send match rom to identify device
    send_DS18_match_rom(dev);
    union
    {
        uint8_t a[12];
        uint32_t l[3];
    } u;
    // send the read scratch command
    oneWire_write_byte(0xBE, true);

    // read back 9 bytes
    oneWire_status stat = oneWire_read_bytes(u.a, 9);
    if (stat != ONE_WIRE_NO_ERROR)
        return false;

    // store the scratch data in the dev struct
    dev->temperature = (u.a[1] << 8) | u.a[0];
    dev->alarm_th = u.a[2];
    dev->alarm_tl = u.a[3];
    dev->config = u.a[4];

    return true;
}

int main()
{
    stdio_init_all();

    // start a rom search to find all devices on the bus
    DS18B20dev_t *devices[10];
    int num_devs = search_DS18_rom(devices);
    if (num_devs == 0)
    {
        printf("No device responded \n");
    }
    if (num_devs == ONE_WIRE_SEARCH_ROM_FAILURE || num_devs >= 10)
    {
        printf("search_rom failed \n");
    }

    // print out devices
    for (int i = 0; i < num_devs; i++)
    {
        sprintf(txt, "\n%d DC = %02X", i, devices[i]->family_code);
        printf(txt);
        sprintf(txt, "\n%012llX", devices[i]->serial_num);
        printf(txt);
    }

    init_OneWire(); // start up the PIO state machine

    // counts of CRC errors
    int p = 0;
    int f = 0;

    while (1)
    {
        oneWire_reset(true);

        // issue command to convert temperature to all devices
        if (!convert_DS18_temp(true))
        {
            printf("Convert temp failed \n");
        }

        // for each device on the bus, read scratch and print temperature
        for (int i = 0; i < num_devs; i++)
        {
            oneWire_reset(true);
            if (get_DS18_scratch(devices[i]))
            {
                p++;
            }
            else
            {
                printf("Scratch Read Failed \n");
                sprintf(txt, "\%d n%x", devices[i]->temperature);
                printf(txt);
                f++;
            }

            float temp = (float)devices[i]->temperature / 16.0;

            sprintf(txt, "\n%d:%5.1f", i, temp);
            printf(txt);
            sprintf(txt, "\nF=%d\nP=%d", f, p);
            printf(txt);
        }
    }
}