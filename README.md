coloredThermometer

This is a thermometer using colored leds to display the temperature
It is USB powered and can be queried over USB HID, see the pythonUI/pyThermometer.py example

The bill of material is as follows :
* STM32F1 (bluepill)
* 15 x WS2812b
* 1 3950 /100 k NTC thermistor

You can print the enclosure using the freecad file available in the enclosure folder

![screenshot](enclosure/pic.jpg?raw=true "front")

This project is using arduino-cmake-stm32 and freertos, pulled as git externals



