add_executable(DS18B20
  OneWire.c
  DS18B20.c
  )

# generate the header file into the source tree as it is included in the RP2040 datasheet
pico_generate_pio_header(DS18B20 ${CMAKE_CURRENT_LIST_DIR}/OneWire.pio)

target_sources(DS18B20 PRIVATE 
    OneWire.c 
    DS18B20.c
    )

# pull in common dependencies
target_link_libraries(DS18B20
    pico_stdlib
    hardware_timer
    hardware_pio
 )

# enable usb
pico_enable_stdio_usb(DS18B20 1)
# disable uart
pico_enable_stdio_uart(DS18B20 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(DS18B20)

# add url via pico_set_program_url
# example_auto_set_url(DS18B20)
