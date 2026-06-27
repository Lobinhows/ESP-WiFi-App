idf_component_register(SRCS "mqtt_demo_mutual_auth.c" "sntp_time_sync.c" "wifi_reset_button.c" "app_nvs.c" "DHT22.c" "http_server.c" "WiFi_App.c" "main.c" "RGB.c"
                        PRIV_REQUIRES spi_flash esp_wifi esp_http_server
                                      esp_driver_ledc nvs_flash app_update
                                      esp_timer fatfs coreMQTT posix_compat backoffAlgorithm
                                      posix_compat coreMQTT backoffAlgorithm
                        REQUIRES freertos
                        INCLUDE_DIRS "." 
                        EMBED_FILES webpage/app.css webpage/app.js webpage/favicon.ico webpage/index.html webpage/jquery-3.7.1.min.js)
                        
target_add_binary_data(${COMPONENT_TARGET} "certs/AmazonRootCA1.pem" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "certs/certificate.pem.crt" TEXT)
target_add_binary_data(${COMPONENT_TARGET} "certs/private.pem.key" TEXT)