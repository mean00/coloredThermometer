
SET(SRCS WSDigit.cpp  WSNumber.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
generate_arduino_library(${libPrefix}WSNumber 
                            SRCS ${SRCS}
                            BOARD ${ARDUINO_DEFAULT_BOARD}
                        )
