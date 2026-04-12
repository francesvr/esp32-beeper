// coms.cpp

// Libraries required to use ESP-NOW
#include <WiFi.h>
#include <esp_now.h>
#include "esp_wifi.h"

// Header Files
#include "coms.h"
#include "board_info.h"

static const char* PMK_KEY = "SharedPrime4U&1!";
static const char* LMK_KEY = "SharedL0cal4U&1!";

Message msg;
esp_now_peer_info_t peer_info;

int rssi = 0;
int channel = 0;


void setupEspNow()
{
    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) 
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register callbacks for receiving and sending ESP-NOW data
    esp_now_register_recv_cb(esp_now_recv_cb_t(onReceived));    // calls onReceived() when data is received
    esp_now_register_send_cb(esp_now_send_cb_t(onSent));        // calls onSent() when data is sent

    // ESP-NOW uses CCMP to encrypt packets 
    // Set PMK key to encrypt the LMK with AES-128
    esp_now_set_pmk((uint8_t *)PMK_KEY);

    // Enable promiscuous mode to capture all packets for RSSI reading
    // Only management frames are captured since ESP-NOW uses vendor-specific action frames
    wifi_promiscuous_filter_t filter = { .filter_mask = WIFI_PROMIS_FILTER_MASK_MGMT };

    esp_wifi_set_promiscuous_filter(&filter);           // Apply filter
    esp_wifi_set_promiscuous(true);                     // Turn on promiscuous mode
    esp_wifi_set_promiscuous_rx_cb(&promiscuous_rx_cb); // Calls promiscuous_rx_cb() when a packet is captured
}

void addPeer()
{
    // Loop through the list of peer MAC addresss
    for (int i = 0; i < NUM_PEERS; i++)
    {
        memcpy(peer_info.peer_addr, mac_peers[i], ESP_NOW_ETH_ALEN);    // Set peer MAC address
        memcpy(peer_info.lmk, LMK_KEY, ESP_NOW_KEY_LEN);                // Set LMK key to decrypt incoming messages
        peer_info.channel = 1;                                          // Set Wi-Fi channel
        peer_info.encrypt = true;                                       // Encrypt messages

        esp_err_t result = esp_now_add_peer(&peer_info);

        // Only add peer if the encrypted peer limit has not been reached
        if (result == ESP_ERR_ESPNOW_FULL)
        {
            Serial.println("Max peers reached.");
            break;
        }

        else if (result != ESP_OK)
        {
            Serial.println("Failed to add peer.");
        }
    }
}

// Sent callback
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Received callback
void onReceived(const uint8_t * mac_addr, const uint8_t *incomingData, int len)
{
    memcpy(&msg, incomingData, sizeof(msg));
}

// Promiscuous callback
void promiscuous_rx_cb(void *buf, wifi_promiscuous_pkt_type_t type) 
{
    // Action frame subtype value per IEEE 802.11 standard
    static const uint8_t ACTION_FRAME = 0xd0;
    
    // Map the raw bytes of the 802.11 MAC header
    typedef struct 
    {
        unsigned frame_ctrl: 16;    // Frame type and subtype -- action frame subtype is 0xd0
        unsigned duration_id: 16;   // Duration of channel reservation
        uint8_t dst[6];             // 1st address is set to the destination address
        uint8_t src[6];             // 2nd address is set to the source address
        uint8_t bcast[6];           // 3rd address is set to broadcast address (0xff:0xff:0xff:0xff:0xff:0xff)
        unsigned seq_ctrl: 16;      // Sequence number for packet ordering
        unsigned category: 8;       // ESP-NOW Category Code is 8 bits -- set to the value (127) for vendor-specific action frames
        uint8_t oui[3];             // Organization Identifier (0x18fe34) applied by Espressif
    } mac_header_t;

    // Raw packet from the Wi-Fi chip, containing RSSI and the 802.11 frame
    const wifi_promiscuous_pkt_t *raw = (wifi_promiscuous_pkt_t *)buf;
    // Read the raw bytes as a MAC header to access its fields
    const mac_header_t *hdr = (mac_header_t *)raw->payload;

    // Ignore if not an action frame
    if ((hdr->frame_ctrl & 0xFF) != ACTION_FRAME)
        return;

    // Ignore if not an ESP-NOW packet
    if (memcmp(hdr->oui, ESP_OUI, 3) != 0)
        return;

    // Update RSSI and channel if packet is from a registered peer
    for (int i = 0; i < NUM_PEERS; i++)
    {
        if (memcmp(hdr->src, mac_peers[i], ESP_NOW_ETH_ALEN) == 0)
        {
            rssi    = raw->rx_ctrl.rssi;
            channel = raw->rx_ctrl.channel;
            return;
        }
    }
}