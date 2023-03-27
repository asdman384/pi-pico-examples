add_executable(alarm
    alarm.c
)

# generate the header file into the source tree as it is included in the RP2040 datasheet
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
pico_generate_pio_header(alarm ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

# pull in common dependencies
target_link_libraries(alarm pico_stdlib)
target_link_libraries(alarm hardware_pio)

pico_enable_stdio_usb(alarm 1)
pico_enable_stdio_uart(alarm 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(alarm)
