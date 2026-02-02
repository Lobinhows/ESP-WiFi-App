/*
 * app_nvs.h
 *
 *  Created on: Jan 10, 2026
 *      Author: Lobinhows
 */
#ifndef APP_NVS_H
#define APP_NVS_H


 /**
  * Saves station mode WiFi credentials
  * @return ESP_OK if successful
  */
esp_err_t app_nvs_save_sta_creds(void);

/**
 * Loads previously saved credentials
 * @return true if previous credentials are found
 */
bool app_nvs_load_sta_creds(void);

/**
 * Clears station mode credentials from NVS
 * @return ESP_OK if successful
 */
esp_err_t app_nvs_clear_sta_creds(void);

#endif /* __APP_NVS_H__ */