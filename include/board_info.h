#pragma once

#include <Arduino.h>

#define MY_NAME "Athena"
static const uint8_t mac_peers[][ESP_NOW_ETH_ALEN] = 
{   // Add peers MAC address
    {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}    // Change to other board's MAC
};

#define NUM_PEERS (sizeof(mac_peers) / sizeof(mac_peers[0]))

#define UP_BTN_PIN      16
#define DOWN_BTN_PIN    17
#define RIGHT_BTN_PIN   18
#define LEFT_BTN_PIN    19
#define OK_BTN_PIN      23

#define BUZZER_PIN      4