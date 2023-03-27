#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define WS2812_PIN 16
#define BUZZ_PIN 0
#define ONE_HOUR_ms 3600000
#define BUTTON_PIN 14

volatile alarm_id_t alarm_id;

void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}
void put_rgb(uint8_t red, uint8_t green, uint8_t blue)
{
    uint32_t mask = (green << 16) | (red << 8) | (blue << 0);
    put_pixel(mask);
}

int64_t fire_alarm_callback(alarm_id_t id, void *user_data)
{
    static int fade = 0;
    static int buzz_value = 0;
    static bool going_up = true;

    buzz_value = 1 - buzz_value; // invert on every call

    if (going_up)
    {
        if (++fade > 255)
        {
            fade = 255;
            going_up = false;
        }
        gpio_put(BUZZ_PIN, 0);
        put_rgb(0x00, 0x00, 0x00);
    }
    else
    {
        if (--fade < 0)
        {
            fade = 0;
            going_up = true;
        }
        gpio_put(BUZZ_PIN, buzz_value);
        put_rgb(fade, 0x00, fade);
    }

    return 1001;
}

int64_t reset_onboard_led()
{
    put_rgb(0x00, 0x00, 0x00);
    return 0;
}

void reset_alarm_callback(uint gpio, uint32_t events)
{
    if (alarm_id > 0)
    {
        cancel_alarm(alarm_id);
    }

    put_rgb(0x00, 0x70, 0x00);
    add_alarm_in_ms(100, reset_onboard_led, NULL, false);

    alarm_id = add_alarm_in_ms(ONE_HOUR_ms, fire_alarm_callback, NULL, true);
    printf("reset_alarm_callback at pin %d with event %d\n", gpio, events);
}

void init_onboard_led()
{
    uint offset = pio_add_program(pio0, &ws2812_program);
    ws2812_program_init(pio0, 0, offset, WS2812_PIN, 800000, true);
}

void init_button_interrupt(uint gpio, gpio_irq_callback_t callback)
{
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
    gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_FALL, 1, callback);
}

int main()
{
    // set_sys_clock_48mhz();
    // Initialise all I/O functions
    stdio_init_all();
    init_onboard_led();
    reset_onboard_led();
    // init buzzer
    gpio_init(BUZZ_PIN);
    gpio_set_dir(BUZZ_PIN, GPIO_OUT);

    init_button_interrupt(BUTTON_PIN, reset_alarm_callback);
    alarm_id = add_alarm_in_ms(ONE_HOUR_ms, fire_alarm_callback, NULL, true);

    while (1)
    {
        tight_loop_contents();
    }
}