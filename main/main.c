/*
 * main.c
 *
 *  Created on: Oct 16, 2025
 *      Author: Lobinhows
 */
#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_http_server.h"
#include "esp_log.h"

#include "RGB.h"
#include "WiFi_App.h"
#include "nvs_flash.h"
#include "DHT22.h"
#include "wifi_reset_button.h"
#include "sntp_time_sync.h"

static const char TAG[] = "main";

void wifi_application_connected_events(void) {
    ESP_LOGI(TAG, "WiFi Application Connected!");
    sntp_time_sync_task_start();
}
 /**
  * RGB Set Colors
  *  WiFi Started -->  220, 240, 50
  *  HTTP Server Started --> 102, 200, 98
  *  WiFi connected --> 202, 88, 223
  *  !! Commom Anode LED used !!
  */

void app_main(void) {
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Start wifi
    wifi_app_start();

    // Configure WiFi reset button
    wifi_reset_button_config();

    DHT22_task_start();

    wifi_app_set_callback(&wifi_application_connected_events);

}
