
SET(SRCS WS2812.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
generate_arduino_library(${libPrefix}WS1812 
                            SRCS ${SRCS}
                            BOARD ${ARDUINO_DEFAULT_BOARD}
                        )
