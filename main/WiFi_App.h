/*
 * WiFi_App.h
 *
 *  Created on: Oct 16, 2025
 *      Author: Lobinhows
 */
#ifndef WIFI_APP_H
#define WIFI_APP_H

#include "freertos/FreeRTOS.h"
#include "esp_wifi_types.h"
#include "esp_netif.h"

 // Access point settings
#define WiFi_AP_SSID    "ESP32_AP"  // AP Name
#define WiFi_AP_PASS    "quickpass" // AP Password
#define WiFi_AP_CHANNEL 1           // Channel of the 2.4GHz range
#define WiFi_AP_SSID_Visible    0   // Is AP visible?
#define WiFi_AP_MAX_CONNECTIONS 5   // Max connections to the AP
#define WiFi_AP_Beacon_Interval 100 // 100ms is recommended
#define WiFi_AP_IP      "192.168.0.1"
#define WiFi_AP_GATEWAY "192.168.0.1"
#define WiFi_AP_NETMASK "255.255.255.0" 
#define WiFi_AP_BANDWIDTH   WIFI_BW_HT20

#define WiFi_STA_POWER_SAVE WIFI_PS_NONE  // Power save not used
#define MAX_SSID_LENGTH 32          // IEEE Standard maximum
#define MAX_PASSWORD_LENGTH 64      // IEEE Standard maximum
#define MAX_CONNECTIONS_RETRIES 5

extern esp_netif_t* esp_netif_sta;
extern esp_netif_t* esp_netif_ap;


/**
 * Message IDs for WiFI application task
 */
typedef enum wifi_app_message {
    WIFI_APP_MSG_START_HTTP_SERVER = 0,
    WIFI_APP_MSG_CONNECTING_FROM_HTTP_SERVER,
    WIFI_APP_MSG_STA_CONNECTED_GOT_IP
}   wifi_app_message_e;

/**
 * Struct for message queue
 */
typedef struct wifi_app_queue_message {
    wifi_app_message_e msgID;
}   wifi_app_queue_message_t;

/**
 * Sends message to the queue
 * @param msgID message ID from wifi_app_message_e enum
 * @return pdTRUE if successfully sent to the queue, pdFALSE otherwise.
 */
BaseType_t wifi_app_send_message(wifi_app_message_e msgID);


/**
 * Starts WiFi RTOS task
 */
void wifi_app_start(void);

#endif // __WIFI_APP_H__