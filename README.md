# MQTT LED Display

A quick-glance information panel, controlled via MQTT.
This little device features a string of 5x WS2812B LEDs, and a 128x64 OLED for more detailed information, and can be used as a convenient little tool around the house. 

## Features
* 5 rotating pages displaying detailed information
* 1x progress bar page for scalar information
* customisable text, subtext and icons per page
* Support for ~80% of the material design icon library
* Support for individual LED control (HSV Colour, and flashing LED functionality)

## Potential Usage Cases:
* 3D printer progress monitor
* Energy monitor
* Temperature Display

## Hardware
Currently there is no PCB for this design. The "Hardware" directory stores the schematics for construction however.

## 3D Printing
The casing provided is intended to be 3D printed. There are no specific support/ infil requirements to achieve a successful print. 

## Credits
Kyle Swinburne

Built from the examples provided by ThingPulse: [Library](https://github.com/ThingPulse/esp8266-oled-ssd1306), [Project](https://github.com/ThingPulse/esp8266-weather-station)

## License
Copyright 2020 Kyle Swinburne

The hardware portion of this project is licensed under the TAPR Open Hardware License (www.tapr.org/OHL). The "license" folder within this repository contains a copy of this license in plain text format.
