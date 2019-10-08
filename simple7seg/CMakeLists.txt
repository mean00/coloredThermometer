
SET(SRCS simple7seg.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
generate_arduino_library(${libPrefix}7seg 
                            SRCS ${SRCS}
                            BOARD ${ARDUINO_DEFAULT_BOARD}
                        )
