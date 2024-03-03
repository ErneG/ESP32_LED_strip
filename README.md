# ESP32 LED Control Web Server

This guide provides instructions on how to set up and flash your ESP32 board to control LEDs through a web server interface, using Arduino IDE.

## Prerequisites

Before you start, ensure you have the following:

-   Arduino IDE installed on your computer
-   An ESP32 board
-   A USB cable to connect the ESP32 to your computer

## Installation Steps

### 1. Setting Up Arduino IDE

1. **Install Arduino IDE** if you haven't already. You can download it from the [Arduino website](https://www.arduino.cc/en/software).

2. **Install ESP32 Board in Arduino IDE**:

    - Open Arduino IDE, go to `Tools` > `Board: "xxx"` > `Boards Manager...`
    - Search for `ESP32` and install the package by `Espressif Systems`.

3. **Select Your Board**:
    - Go to `Tools` > `Board manager` > `ESP32 Arduino` and select `ESP32 Dev Module`.

### 2. Install Required Libraries

The script uses several libraries that need to be installed:

-   `WiFi.h`
-   `ESPAsyncWebServer.h`
-   `FastLED.h`

To install these libraries:

-   Go to `Tools` > `Manage Libraries...`
-   Search for each library and install it along with its dependencies.

### 3. Install USB-to-UART Driver

If your computer does not recognize the ESP32 board as a serial device, you may need to install a driver:

-   Visit [Silicon Labs' USB to UART Bridge VCP Drivers page](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) to download and install the necessary drivers.

## Flashing the Board

1. **Connect your ESP32** to the computer using a USB cable.
2. Open the provided code in Arduino IDE.
3. **Modify WiFi Settings**: Replace `THE_NAME_OF_YOUR_WIFI_NETWORK` and `THE_PASSWORD_OF_YOUR_WIFI_NETWORK` with your WiFi credentials.
4. **Upload the Code**: Click the upload button in Arduino IDE to flash your ESP32.

## Post-Flashing Steps

After flashing the device, disconnect it from any power source and then reconnect it. Upon reconnecting, the board should create an access point. Connect to this network using the password you set in the code. On a device connected to this access point, open a web browser and navigate to `192.168.4.1`. You should be presented with the webpage.

## Code Modifications and Explanations

The provided code sets up an ESP32 web server to control LEDs via a web page. Here are some considerations and potential modifications:

### Additional Settings

At the top of the script, there are a few settings that might need changing:

-   **LED_PIN** is the pin to which the data wire is connected (D13 in this case).
-   **NUM_LEDS** is the number of LEDs you have connected to the controller.

It's crucial to **not power the LED strip directly from the board** as this can damage the board if you attempt to power a significant number of LEDs. The LED strip should be powered from a separate power source that supplies the required voltage for the LED strip. In this example, a 5V LED strip is powered from a battery pack.

### WiFi Mode

The code initializes the ESP32 as an Access Point (AP) with `WiFi.softAP(ssid, password);`. You may want to connect the ESP32 to an existing WiFi network instead. To do this, replace the `WiFi.softAP(ssid, password);` line with `WiFi.begin(ssid, password);` and ensure your SSID and password are for the network you wish to connect to.

### LED Configuration

Adjust `LED_PIN`, `NUM_LEDS`, `LED_TYPE`, and `COLOR_ORDER` according to your specific hardware setup.

### HTML Page

For larger projects or more complex web interfaces, consider serving the HTML from a file system like SPIFFS or LittleFS on the ESP32.

### Error Handling

For a more robust solution, consider adding detailed error responses and logging.

## Conclusion

After following these steps, your ESP32 should host a web server that allows you to control LEDs through a simple web interface. Modify the code as needed to fit your project.

## Parts List

For this project, you will need the following parts, all of which can be found on AliExpress by searching for the exact terms provided. Reference the accompanying picture for more details on how to assemble these components.

-   1x XL0401 diode
-   1x Type C PD trigger board
-   2x DC buck converter 2a
-   1x MOSFET switch 15a
-   1x ESP32 (any ESP32 board will do)
-   1x 3s BMS (Battery Management System)
-   3x 18650 cells

Ensure you have all the necessary parts before starting the project to avoid any interruptions during the build process.
