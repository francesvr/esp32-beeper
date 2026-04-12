# ESP32 Beeper
The ESP32 Beeper was originally an academic project for a Wireless Communication and Microcontroller class. 
It evolved into a fully functional peer-to-peer encrypted messaging device that works completely offline.  
  
Inspired by the Filipino movie *She's Dating the Gangster*, set in the late 1990s when beepers were widely 
used by Filipino young adults, the two devices are named Kenji and Athena after the film's main characters.  
  
Unlike real 1990s beepers which relied on one-way radio paging towers, the ESP32 Beeper communicates directly 
between two devices with no infrastructure. Every message is encrypted end-to-end using AES-128.   
  
  *See [coms.cpp](https://github.com/francesvr/esp32-beeper/blob/main/src/coms.cpp) and [coms.h](https://github.com/francesvr/esp32-beeper/blob/main/include/coms.h) for the full communication implementation.*  
  
**Board**: ESP32-WROOM-32E  
  Espressif v4.4  

## Features
- Write a message to peers
- Quick-send premade messages to peers
- Send SOS alerts to peers
- View the last received message
- Check peer signal strength (RSSI) and last active time
- End-to-end AES-128 encrypted communication
- Works completely offline


## Resources
- [ESP-NOW v4](https://docs.espressif.com/projects/esp-idf/en/release-v4.4/esp32/api-reference/network/esp_now.html#)
- [ESP WiFi](https://docs.espressif.com/projects/esp-idf/en/v4.0.4/api-reference/network/esp_wifi.html)

## Dependencies

- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [OneButton](https://github.com/mathertel/OneButton)
- [ESP-NOW](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_now.html)
