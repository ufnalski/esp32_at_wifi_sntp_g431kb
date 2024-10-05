# ESP-AT Wi-Fi SNTP for STM32 (STM32G431KB)
AT (from 'attention') commands were originally developed in 1981 for dial-up modems[^1]. Nowadays the idea is still being implemented in communication modems/modules. Sets of commands usually differ from device to device but the format of messages exchanged via UART is kept. Some common examples popular among DIYers include:
* GSM/GPRS modules such as SIM900[^2],
* BT/BLE modules such as AT-09/HM-10[^3],
* LoRa modules such as the ones from Reyax[^4],
* Wi-Fi modules such as ESP32 SoC flashed with a dedicated firmware.

This submission focuses on the last one. We will provide the network time to an STM32 uC that has no ETH peripheral. Such an approach, i.e. to have a separate uC/SoC responsible for wireless communication, is not uncommon in connected devices. A popular example of that is the Arduino UNO R4 WiFi board[^5].

[^1]: [Hayes AT command set](https://en.wikipedia.org/wiki/Hayes_AT_command_set) (Wikipedia)
[^2]: [Moduł SIM900 GSM GPRS shield dla UNO R3](https://sklep.msalamon.pl/produkt/modul-sim900-gsm-gprs-shield-dla-uno-r3/) (msalamon.pl)
[^3]: [Bluetooth 4.0 BLE - HM-10](https://botland.com.pl/moduly-bluetooth/8591-modul-bluetooth-40-ble-hm-10-33v5v-5904422311896.html) (Botland)
[^4]: [RYLR998 868/915MHz LoRa Antenna Transceiver Module](https://reyax.com//products/RYLR998) (Reyax)
[^5]: [UNO R4 WiFi](https://docs.arduino.cc/hardware/uno-r4-wifi/) (Arduino)

# My setup
* An ESP32-C6 evaluation board flashed with the newest ESP-AT firmware.
* A Nucleo board plus an OLED to talk to the ESP32-C6.
* A USB2UART converter for debugging plus the YAT serial terminal.
* A logic analyzer to be able to check the exchanged messages directly on the wires.

![ESP32-C6 AT SNTP in action](/Assets/Images/esp_at_sntp_in_action.jpg)

![ESP32-C6 AT SNTP OLED](/Assets/Images/esp_at_stm32_oled.jpg)

![ESP32-C6 AT SNTP logic analyzer](/Assets/Images/esp_at_logic_analyzer.JPG)

![ESP32-C6 AT SNTP YAT settings](/Assets/Images/esp_at_yat_settings.JPG)

![ESP32-C6 AT SNTP YAT terminal](/Assets/Images/esp_at_yat_terminal.JPG)

# Missing files?
Don't worry :slightly_smiling_face: Just hit Alt-K to generate /Drivers/CMCIS/ and /Drivers/STM32G4xx_HAL_Driver/ based on the .ioc file. After a couple of seconds your project will be ready for building.

> [!TIP]
> RTBM/RTFM[^6] - this will save you a lot of time. And yes, professionals do read manuals :slightly_smiling_face:

[^6]: [RTFM](https://en.wikipedia.org/wiki/RTFM) (Wikipedia)

# Manuals and tools from Espressif
* [What is ESP-AT](https://docs.espressif.com/projects/esp-at/en/latest/esp32c6/Get_Started/What_is_ESP-AT.html)
* [Hardware Connection](https://docs.espressif.com/projects/esp-at/en/latest/esp32c6/Get_Started/Hardware_connection.html)
* [Wi-Fi AT Commands](https://docs.espressif.com/projects/esp-at/en/latest/esp32c6/AT_Command_Set/Wi-Fi_AT_Commands.html)
* [ESP-AT (firmware)](https://github.com/espressif/esp-at)
* [Flash Download Tools for Windows](https://www.espressif.com/en/support/download/other-tools) or the cross-platform [esptool.py](https://github.com/espressif/esptool)

# More examples
* [ESP8266 WebServer using STM32 HAL](https://controllerstech.com/esp8266-webserver-using-stm32-hal/) (ControllersTech)
* [ESP32 z STM32 na komendach AT - kontynuujemy walkę razem z Chat-GPT 4o](https://www.youtube.com/watch?v=3Tv1ha1GIMQ) (Mateusz Salamon)

# Call for action
Create your own [home laboratory/workshop/garage](http://ufnalski.edu.pl/control_engineering_for_hobbyists/2024_dzien_popularyzacji_matematyki/Dzien_Popularyzacji_Matematyki_2024.pdf)! Get inspired by [ControllersTech](https://www.youtube.com/@ControllersTech), [DroneBot Workshop](https://www.youtube.com/@Dronebotworkshop), [Andreas Spiess](https://www.youtube.com/@AndreasSpiess), [GreatScott!](https://www.youtube.com/@greatscottlab), [ElectroBOOM](https://www.youtube.com/@ElectroBOOM), [Phil's Lab](https://www.youtube.com/@PhilsLab), [atomic14](https://www.youtube.com/@atomic14), [That Project](https://www.youtube.com/@ThatProject), [Paul McWhorter](https://www.youtube.com/@paulmcwhorter), [Max Imagination](https://www.youtube.com/@MaxImagination), and many other professional hobbyists sharing their awesome projects and tutorials! Shout-out/kudos to all of them!

> [!WARNING]
> Control engineering - do try this at home :sunglasses:

190+ challenges to start from: [Control Engineering for Hobbyists at the Warsaw University of Technology](http://ufnalski.edu.pl/control_engineering_for_hobbyists/Control_Engineering_for_Hobbyists_list_of_challenges.pdf).

Stay tuned!
