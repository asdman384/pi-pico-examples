#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define WS2812_PIN 16

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

    uint offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, offset, WS2812_PIN, 800000, true);

    char userInput;

    while (1)
    {
        // Get User Input
        printf("Command (1 = on or 0 = off):\n");
        userInput = getchar();

        if (userInput == '1')
            put_rgb(0xff, 0xff, 0xff);
        else if (userInput == '0')
            put_rgb(0x00, 0x00, 0x00);
        else
            printf("invalid input\n");
    }
}