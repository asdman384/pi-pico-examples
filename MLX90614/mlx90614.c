#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "driver/mlx90614_driver.h"
#include "display/ssd1306.h"

void init_sensor()
{
    InitializeMLX(MLX90614_I2CADDR);

    gpio_set_function(14, GPIO_FUNC_I2C); // i2c1 SDA
    gpio_set_function(15, GPIO_FUNC_I2C); // i2c1 SCL
    gpio_pull_up(14);                     // i2c1 SDA
    gpio_pull_up(15);                     // i2c1 SCL
}

ssd1306_t init_display()
{
    i2c_init(i2c0, 400000);
    gpio_set_function(0, GPIO_FUNC_I2C); // i2c0 SDA
    gpio_set_function(1, GPIO_FUNC_I2C); // i2c0 SCL
    gpio_pull_up(0);                     // i2c0 SDA
    gpio_pull_up(1);                     // i2c0 SCL

    ssd1306_t disp;
    disp.external_vcc = false;
    ssd1306_init(&disp, 64, 32, 0x3C, i2c0);
    ssd1306_clear(&disp);

    return disp;
}

int main(void)
{
    stdio_init_all(); // Initialise STD I/O for printing over serial
    init_sensor();

    ssd1306_t disp = init_display();
    const char *words[] = {
        "display",
        "test",
    };

    double emissivity;
    double ambient_temp;
    double object_temp;

    // MLXwriteEmissivity(0.85);
    char txt[32];

    while (1)
    {
        // MLXreadEmissivity(&emissivity);
        // MLXreadAmbientTempC(&ambient_temp);
        MLXreadObjectTempC(&object_temp);
        printf("e=%f, object_temp=%f\n", emissivity, object_temp);
        sprintf(txt, "TEMP:%.2f", object_temp);

        ssd1306_clear(&disp);
        ssd1306_draw_string(&disp, 1, 10, 1, txt);
        ssd1306_show(&disp);
        sleep_ms(100);

        // for (int y = -20; y < 52; y++)
        // {
        //     ssd1306_clear(&disp);
        //     ssd1306_draw_string(&disp, 15, 0 + y, 1, words[0]);
        //     ssd1306_draw_string(&disp, 15, 10 + y, 1.5, words[1]);
        //     ssd1306_show(&disp);
        //     sleep_ms(36);
        // }
    }
}
