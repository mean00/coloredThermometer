
# Select platform and installation for extra arduino files AND toolchain (compiler...)
SET(PLATFORM_ARCHITECTURE "STM32F1")
# Must be named arduino
#  else the cmake code will look for ARDUINO
# while the name will be the name of the folder
IF(NOT PLATFORM_PATH)
    SET(PLATFORM_PATH "/home/fx/Arduino/hardware/stm32/STM32F1")
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

# USB_Composite
ADD_DEFINITIONS(-DUSB_EP_TYPE_ISO=USB_EP_T_ISO)
SET(USB_HID ${CMAKE_CURRENT_SOURCE_DIR}/USBComposite)
include_directories(${USB_HID})

##ADD_DEFINITIONS(-DSDA=PB7 -DSCL=PB6)

include_directories(MapleFreeRTOS1000//Source/include)
include_directories(MapleFreeRTOS1000//)
include_directories(WS2812)
include_directories(WSNumber)
include_directories(NTC)
include_directories(.)
link_directories( ${CUSTOM_LIB_DIR})
ADD_SUBDIRECTORY(MapleFreeRTOS1000)
ADD_SUBDIRECTORY(WS2812)
ADD_SUBDIRECTORY(WSNumber)
ADD_SUBDIRECTORY(NTC)

#
# USB Composite
#

SET(USB_SRCS USBComposite/USBComposite.cpp USBComposite/USBHID.cpp  USBComposite/usb_setup.cpp)
include_directories(USBComposite)

generate_arduino_firmware(ColoredThermo
                SKETCH thermoLed.ino
                SRCS myCode.cpp  tempDaemon.cpp   WSDisplay.cpp  ${USB_SRCS} thermoHID.cpp
                PORT ${ARDUINO_DEFAULT_PORT}
                BOARD_CPU ${ARDUINO_CPU}
                )
TARGET_LINK_LIBRARIES( ColoredThermo  ${libPrefix}FreeRTOS  ${libPrefix}WS2812 ${libPrefix}WSNumber ${libPrefix}NTC ${libPrefix}USBComposite)


# EOF
