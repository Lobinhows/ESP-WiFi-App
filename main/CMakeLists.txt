idf_component_register(SRCS "http_server.c" "WiFi_App.c" "main.c" "RGB.c"
                        PRIV_REQUIRES spi_flash esp_wifi esp_http_server
                                        esp_driver_ledc nvs_flash
                        INCLUDE_DIRS "."
                        EMBED_FILES webpage/app.css webpage/app.js webpage/favicon.ico webpage/index.html webpage/jquery-3.7.1.min.js)
