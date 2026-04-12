// coms.h
#pragma once

#include <esp_now.h>
#include <WiFi.h>

static const uint8_t ESP_OUI[] = {0x18, 0xfe, 0x34};

// The maximum payload size for a single ESP-NOW transmission is 250 bytes.
typedef struct Message
{                   // The struct totals to 239 bytes, 11 bytes under the limit.
    char from[8];   // Sender's name -- up to 7 characters (null terminated)
    char text[230]; // Text messsage -- up to 229 characters (null terminated)
    bool isSOS;     // Whether the data is an SOS alert -- 1 byte
} Message;

extern Message msg;
extern esp_now_peer_info_t peer_info;

extern int rssi;
extern int channel;

void setupEspNow();
void addPeer();

// Callbacks
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void onReceived(const uint8_t *mac_addr, const uint8_t *incomingData, int len);

void promiscuous_rx_cb(void *buf, wifi_promiscuous_pkt_type_t type);
