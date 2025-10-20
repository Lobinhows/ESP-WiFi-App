idf_component_register(SRCS "WiFi_App.c" "main.c" "RGB.c"
                        PRIV_REQUIRES spi_flash esp_wifi esp_http_server
                                        esp_driver_ledc nvs_flash
                        INCLUDE_DIRS ".")
