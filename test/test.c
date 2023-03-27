
#include <stdio.h>
#include "pico/stdlib.h"

#define BUTTON_PIN 14

int64_t fire_alarm_callback(alarm_id_t id, void *user_data)
{
    printf("alarm_callback %d fired!\n", (int)id);
    // Can return a value here in us to fire in the future
    return 1000000;
}

void button_callback(uint gpio, uint32_t events)
{
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
    stdio_init_all();
    printf("Hello Timer!\n");

    init_button_interrupt(BUTTON_PIN, button_callback);

    // Call alarm_callback in 2 seconds
    add_alarm_in_ms(2000, fire_alarm_callback, NULL, false);

    while (1)
    {
        tight_loop_contents();
    }

    return 0;
}
