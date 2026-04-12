// Libraries required to use ESP-NOW
#include <WiFi.h>
#include <esp_now.h>
#include "esp_wifi.h"

// Header files
#include "coms.h"
#include "board_info.h"

void setup() 
{
  Serial.begin(9600);

  // ESP-NOW
  setupEspNow();
  addPeer();
}

void loop() 
{
  Serial.printf("%d\n", rssi);
  delay(1000); 
}