add_executable(serial_input
  serial_input.c
)

# generate the header file into the source tree as it is included in the RP2040 datasheet
file(MAKE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/generated)
pico_generate_pio_header(serial_input ${CMAKE_CURRENT_LIST_DIR}/ws2812.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/generated)

# pull in common dependencies
target_link_libraries(serial_input pico_stdlib)
target_link_libraries(serial_input hardware_pio)

# enable usb
pico_enable_stdio_usb(serial_input 1)
# disable uart
pico_enable_stdio_uart(serial_input 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(serial_input)

# add url via pico_set_program_url
# example_auto_set_url(serial_input)
