add_executable(test
  test.c
)

# pull in common dependencies
target_link_libraries(test pico_stdlib)

pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 0)

# create map/bin/hex file etc.
pico_add_extra_outputs(test)

# add url via pico_set_program_url
# example_auto_set_url(test)