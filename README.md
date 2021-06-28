# smart-plant


## ESP32 setup
https://www.youtube.com/watch?v=xPlN_Tk3VLQ

## Arduino IDE installation
Download from https://www.arduino.cc/en/main/software

```bash
tar -xvf arduino-1.8.13-linux64.tar.xz
./arduino-1.8.13/install.sh
```

Now we add the ESP32 to the Arduino board manager:
https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md

Copy the latest stable link to the board manager json:
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Open the Arduino IDE, open `File -> Preferences` and paste this URL into the field `Additional Board Manager URLs`.

Now we need to install the boards. Navigate to `Tools -> Board -> Boards Manager` and search for `esp32`. Install the latest version.

### Issues that needed fixing
At a later stage when compiling sketches I got the error
```
Traceback (most recent call last):
File "C:/msys32/sdk/esp-idf/components/esptool_py/esptool/esptool.py", line 24, in
import serial
ImportError: No module named serial
```

I found a solution to this [here](https://github.com/espressif/esptool/issues/171) and fixed it by installing pyserial on the system with
```bash
pip install pyserial
```

I also had a problem with not having access to the serial port `/dev/ttyUSB0` when attempting to upload sketches to the ESP32. I found a solution to this [here](https://www.arduino.cc/en/guide/linux#toc6). If we list the serial interfaces using `ls -l /dev` we can see that the usergroup for this interface is `dialout`. We can therefore add our user to this group with `sudo usermod -a -G dialout <user>`. After logging out and back in we should now be able to access the serial port through the Arduino IDE.

## Running an example sketch

1. Connect the ESP32 to the computer with a micro-USB cable
2. Open the sketch and select `Tools -> Board` and make sure to select `ESP32 Dev Module`.
3. Select `Tools -> Port` and make sure to select the right one. You can try disconnecting and reconnecting the cable to see which port is in use.
4. Press the upload sketch button and the LED on the board should now start blinking!
5. We can also open `Tools -> Serial Monitor` and select the same baud rate in the bottom right as in the code. We should now see the prints to the serial as the LED is blinking!

If you get the error `A fatal error occurred: Failed to connect to ESP32: Timed out waiting for packet header` you might have to press and hold the `BOOT` button on the ESP board while the sketch is uploading.

If the light is not blinking, or nothing happens, you might have to press the `EN` button on the board to restart it. The light should then start blinking.

# Using the DHT22 sensor
http://www.esp32learning.com/code/esp32-and-am2302-sensor-example.php

We can install libraries in two ways:

1. In the Arduino IDE we can select `Sketch -> Include Library -> Manage Libraries` and search for the library name and install it this way.
2. We can also download the library as a zip file from the https://github.com/ and then select `Sketch -> Include Library -> Install .ZIP Library...`

First add the DHT library from Adafruit:

DHT Sensor
https://github.com/adafruit/DHT-sensor-library


# Creating a web server

Async TCP
https://github.com/me-no-dev/AsyncTCP

ESP Async Web Server:
https://github.com/me-no-dev/ESPAsyncWebServer

Arduino JSON
https://github.com/bblanchon/ArduinoJson

Adafruit Sensor
https://github.com/adafruit/Adafruit_Sensor



https://medium.com/@raphaelpralat/example-of-json-rest-api-for-esp32-4a5f64774a05
