/*
 * http_server.h
 *
 *  Created on: Oct 22, 2025
 *      Author: Lobinhows
 */
#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include "freertos/FreeRTOS.h"

#define OTA_UPDATE_PENDING      0
#define OTA_UPDATE_SUCCESSFUL   1
#define OTA_UPDATE_FAILED       -1

 /**
  * Connection status for WiFi
  */
typedef enum http_server_wifi_connect_status {
    NONE = 0,
    HTTP_WIFI_STATUS_CONNECTING,
    HTTP_WIFI_STATUS_FAILED,
    HTTP_WIFI_STATUS_SUCCESS,
    HTTP_WIFI_STATUS_DISCONNECTED
} http_server_wifi_connect_status_e;
 /**
  * Messages for HTTP monitor
  */
typedef enum http_server_message {
    HTTP_MSG_WIFI_CONNECT_INIT = 0,
    HTTP_MSG_WIFI_CONNECT_SUCCESS,
    HTTP_MSG_WIFI_CONNECT_FAIL,
    HTTP_MSG_WIFI_USER_DISCONNECT,
    HTTP_MSG_OTA_UPDATE_SUCCESS,
    HTTP_MSG_OTA_UPDATE_FAIL,
    HTTP_MSG_TIME_SERVER_INITIALIZED,
} http_server_message_e;

/**
 * Structure for message queue
 */
typedef struct http_server_queue_message {
    http_server_message_e msgID;
} http_server_queue_message_t;

/**
 * Sends message to the queue
 * @param msgID message ID from http_server_message_e enum.
 * @return pdTRUE if item was successfuly sent to queue, pdFALSE otherwise
 */
BaseType_t http_server_monitor_send_msg(http_server_message_e msgID);

/**
 * Starts HTTP Server
 */
void http_server_start(void);

/**
 * Stops HTTP Server
 */
void http_server_stop(void);

/**
 * Calls esp_restart upon successful firmware update
 */
void http_server_fw_update_reset_callback(void* arg);

#endif /* __HTTP_SERVER_H__ */