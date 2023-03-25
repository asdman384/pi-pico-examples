add_executable(MLX90614
  driver/mlx90614_driver.c
  driver/mlx90614_driver_pal.c
  display/ssd1306.c
  mlx90614.c
)


# pull in common dependencies
target_link_libraries(MLX90614
    pico_stdlib
    hardware_i2c
 )

# enable usb
pico_enable_stdio_usb(MLX90614 1)
# disable uart
pico_enable_stdio_uart(MLX90614 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(MLX90614)

# add url via pico_set_program_url
# example_auto_set_url(MLX90614)
