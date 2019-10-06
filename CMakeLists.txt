
# Select platform and installation for extra arduino files AND toolchain (compiler...)
SET(PLATFORM_ARCHITECTURE "STM32F1")
# Must be named arduino
#  else the cmake code will look for ARDUINO
# while the name will be the name of the folder
IF(NOT PLATFORM_PATH)
    SET(PLATFORM_PATH "/home/fx/Arduino/hardware/arduino/")
    MESSAGE(STATUS "Using default value for PLATFORM_PATH ${PLATFORM_PATH}")
ENDIF(NOT PLATFORM_PATH)
IF(NOT PLATFORM_TOOLCHAIN_PATH)
    # Use gcc 8.2.1 from MCU eclipse in nano mode
    SET(ARDUINO_USE_NEWLIB 1)
    SET(PLATFORM_TOOLCHAIN_PATH "/home/fx/Arduino_stm32/arm-none-eabi-gcc/download/gcc-arm-none-eabi-8.2.1-1.7/bin")
    #SET(PLATFORM_TOOLCHAIN_PATH "/home/fx/.arduino15/packages/arduino/tools/arm-none-eabi-gcc/4.8.3-2014q1/bin/")
    MESSAGE(STATUS "Using default value for PLATFORM_TOOLCHAIN_PATH ${PLATFORM_TOOLCHAIN_PATH}")
ENDIF(NOT PLATFORM_TOOLCHAIN_PATH)
IF(NOT CUSTOM_LIB_DIR)
    # Where you store your downloaded libs or manually installed
    SET(CUSTOM_LIB_DIR /home/fx/Arduino/libraries)
    MESSAGE(STATUS "Using default value for CUSTOM_LIB_DIR ${CUSTOM_LIB_DIR}")
ENDIF(NOT CUSTOM_LIB_DIR)


# and here we go
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
SET(CMAKE_TOOLCHAIN_FILE cmake/ArduinoToolchain.cmake)


cmake_minimum_required(VERSION 2.8)
Project("coloredThermometer" C CXX ASM)
# Board setup
set(ARDUINO_DEFAULT_BOARD genericSTM32F103C)        # Default Board ID, when not specified
set(ARDUINO_DEFAULT_PORT ttyACM0) # Default Port, when not specified
set(ARDUINO_CPU          STM32F103C8)
SET(ARDUINO_UPLOAD_METHOD DFUUpload)
set(ARDUINO_CPUMENU .menu.cpu.${ARDUINO_CPU})
SET(libPrefix ${ARDUINO_DEFAULT_BOARD}_)


#include_directories(SevSeg/src)

#generate_arduino_library( simpler_INA219
                        #SRCS  simpler_INA219/simpler_INA219.cpp
                        #BOARD ${ARDUINO_DEFAULT_BOARD})

#INCLUDE_DIRECTORIES(simpler_INA219)

# This reduces size by 10% or so
#ADD_DEFINITIONS("-ffunction-sections -fdata-sections ")
generate_arduino_firmware(ColoredThermo
                SKETCH thermoLed.ino
                SRCS myCode.cpp simple7seg.cpp
                PORT ${ARDUINO_DEFAULT_PORT}
                BOARD_CPU ${ARDUINO_CPU}
                )

# EOF
