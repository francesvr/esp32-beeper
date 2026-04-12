// main.cpp
#include <WiFi.h>
#include <esp_now.h>
#include "esp_wifi.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "Picopixel.h"
#include <OneButton.h>

#include "config.h"
#include "coms.h"
#include "state.h"
#include "ui.h"
#include "btn.h"
#include "power.h"
#include "beep.h"
#include "bitmaps.h"
#include "draw.h"
#include "_properties.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup()
{
  Serial.begin(9600);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR))
  {
    Serial.println("OLED FAIL");
    while (true);
  }

  display.clearDisplay();
  display.display();

  initButtons();

  ledcSetup(0, 1000, 8);
  ledcAttachPin(BUZZER_PIN, 0);

  setupEspNow();
  addPeer();

  Serial.println(MY_NAME);
  Serial.println(WiFi.macAddress());
}


void loop()
{
  tickButtons();
  updatePowerMode();
  updateScreen();

  if (alertReceived)
    playAlert();

  static unsigned long lastPing = 0;
  if (millis() - lastPing >= 5000)
  {
    sendPing();
    lastPing = millis();
  }


  // remove later

  static unsigned long lastRSSI = 0;

  if (millis() - lastRSSI >= 1000)
  {
    for (int i = 0; i < NUM_PEERS; i++)
    {
      bool stale = (millis() - peer_last_seen[i]) > 15000;
      Serial.printf("Peer %d | RSSI: %d dBm | Last seen: %lums ago %s\n",
        i,
        peer_rssi[i],
        millis() - peer_last_seen[i],
        stale ? "(offline?)" : ""
      );
    }
    lastRSSI = millis();
  }
}