#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define WS2812_PIN 16
#define BUZZ_PIN 14
#define ONE_HOUR_ms 3600000

void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}
void put_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t mask = (green << 16) | (red << 8) | (blue << 0);
    put_pixel(mask);
}

int main()
{
    // Initialise all I/O functions
    stdio_init_all();

    // init onboard led
    uint offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, offset, WS2812_PIN, 800000, true);
    put_rgb(0x00, 0x70, 0x00);
    sleep_ms(100);
    put_rgb(0x00, 0x00, 0x00);

    // init buzzer
    gpio_init(BUZZ_PIN);
    gpio_set_dir(BUZZ_PIN, GPIO_OUT);

    int count = 0;
    sleep_ms(ONE_HOUR_ms);

    while (1)
    {
        if (count < 125)
        {
            if (count == 0)
            {
                put_rgb(0xff, 0x00, 0x00);
            }

            gpio_put(BUZZ_PIN, 0);
            sleep_ms(1);
            gpio_put(BUZZ_PIN, 1);
            sleep_ms(1);
            count++;
        }
        else
        {
            gpio_put(BUZZ_PIN, 0);
            while (count != 0)
            {
                put_rgb(count, 0x00, 0x00);
                sleep_ms(2);
                count--;
            }
        }
    }
}