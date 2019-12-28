coloredThermometer

This is a thermometer using colored leds to display the temperature, with effects (breath, rainbow etc...).

It is USB powered and can be queried over USB HID to get the temperature, see the pythonUI/pyThermometer.py example.

The bill of material is as follows :
* STM32F103 (bluepill)
* 15 x WS2812b
* 1 x  3950 /100 k NTC thermistor

There is also an enclosure you can print, see the  freecad file available in the enclosure folder

![screenshot](enclosure/pic.jpg?raw=true "front")

This project is using arduino-cmake-stm32 and freertos, pulled as git externals.
It is way overkill in term of software design, but why try to optimize it.



