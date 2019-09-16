### Wifi led strip control and Infrared receiver bridge


### Install Arduino and ESP8266 board

Download Arduino IDE, [link](https://www.arduino.cc/en/main/software)

You can follow this oficial page to install ESP8266 board into arduino, [link](http://arduino.esp8266.com/Arduino/versions/2.0.0/doc/installing.html)


### PlatformIO

You can check the [getting started](https://platformio.org/get-started) page in the PlatformIO site to know how to setup the environment for your platform.

There is two different configurations, `irled` which is the firmware to turn on/off led strip and `test-irled` which is the firmware we use for hardware testing purposes.

Run normal firmware: 
```
pio run -t upload -e irled
```

Run test environment firmware: 
```
pio run -t upload -e test-irled
```
