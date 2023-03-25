cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(test_project C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()



# add test example
add_subdirectory(alarm)
add_subdirectory(serial_input)
add_subdirectory(test)
add_subdirectory(DS18B20)
add_subdirectory(MLX90614)
