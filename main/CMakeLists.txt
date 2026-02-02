idf_component_register(SRCS "sntp_time_sync.c" "wifi_reset_button.c" "app_nvs.c" "DHT22.c" "http_server.c" "WiFi_App.c" "main.c" "RGB.c"
                        PRIV_REQUIRES spi_flash esp_wifi esp_http_server
                                        esp_driver_ledc nvs_flash app_update
                                        esp_timer
                        INCLUDE_DIRS "."
                        EMBED_FILES webpage/app.css webpage/app.js webpage/favicon.ico webpage/index.html webpage/jquery-3.7.1.min.js)
