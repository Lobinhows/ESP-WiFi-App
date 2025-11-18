/*
 * DHT22.c
 *
 *  Created on: Nov 12, 2025
 *      Author: Lobinhows
 */

#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp32/rom/ets_sys.h"

#include "DHT22.h"
#include "tasks_common.h"

#define DHT_MAX_DATA 5 


static const char* TAG = "DHT";
float humidity = 0.;
float temperature = 0.;


// Error handler
void errorHandler(int response) {
    switch (response) {
    case DHT_TIMEOUT_ERROR:
        ESP_LOGI(TAG, "Sensor Timeout\n");
        break;
    case DHT_CHECKSUM_ERROR:
        ESP_LOGI(TAG, "CheckSum error\n");
        break;
    case DHT_OK:
        break;
    default:
        ESP_LOGI(TAG, "Unkown error ocurred\n");
        break;
    }
}


/**
 * @brief Wait for signal level passed to the function.
 * Read it like "I want to wait X us to receive the passed signal level"
 * @param level Signal level you are waiting for
 * @param timeout_us Time to wait for signal level
 * @return False if timeout, true if signal changed.
 */
bool waitSignalLevel(int level, int timeout_us) {
    int64_t timer_start = esp_timer_get_time();
    while (gpio_get_level(DHT22_GPIO) != level) {
        if (esp_timer_get_time() - timer_start > timeout_us) {
            return false;
        }
    }
    return true;
}


/**
 * @brief Read bits received from sensor and return the equivalent byte
 * @return Byte formed from bits received if successful, -1 if timeout while reading bit
 */
uint8_t readByte() {
    uint8_t byte = 0;
    int idx = 7;
    for (int i = 0; i < 8; i++) {
        if (!waitSignalLevel(1, 55)) {
            ESP_LOGI(TAG, " Bit reading timeout!");
            return -1;                  // !!! Actualy it doesn't help to return -1 if timeout, instead, it will create a wrong reading.
        }
        uint64_t bit_start = esp_timer_get_time();
        if (!waitSignalLevel(0, 75)) {
            ESP_LOGI(TAG, " Bit reading timeout!");
            return -1;
        }
        int duration = esp_timer_get_time() - bit_start;
        // If bit equals 1
        if (duration > 35) {
            byte = byte | 1 << idx;
        }
        idx--;
    }
    return byte;
}




int readDHT() {

    uint8_t dhtData[DHT_MAX_DATA];

    for (int i = 0; i < DHT_MAX_DATA; i++) {
        dhtData[i] = 0;
    }

    gpio_set_direction(DHT22_GPIO, GPIO_MODE_OUTPUT);

    // Waking up sensor
    gpio_set_level(DHT22_GPIO, 0);
    ets_delay_us(1000);
    gpio_set_level(DHT22_GPIO, 1);
    ets_delay_us(30);

    // Receiving Sensor ACK
    gpio_set_direction(DHT22_GPIO, GPIO_MODE_INPUT);

    // Waiting 85us for level 0 (5 extra us to give a room for errors)
    if (!waitSignalLevel(1, 85)) {
        return DHT_TIMEOUT_ERROR;
    }
    if (!waitSignalLevel(0, 85)) {
        return DHT_TIMEOUT_ERROR;
    }

    // Receiving Sensor Data

    for (int i = 0; i < 5; i++) {
        dhtData[i] = readByte();
    }


    // DHT22 sends data as big endian float, scaled by 10. 
    // For example, if you received byte[0] = 0x01 and byte[2] = 0x44, it means 0x01 << 8 | 0x44
    // 0x0144 = 324, which is scaled by 10 so the value is actually 32.4

    if (dhtData[4] == ((dhtData[0] + dhtData[1] + dhtData[2] + dhtData[3]) & 0xFF)) {
        humidity = ((dhtData[0] * 256) + dhtData[1]) / 10.0f;
        // Temp's highest bit is a signal bit (0 = positive temp, 1 = negative temp)
        // Byte[i] & 0x80 compares most significative bit with 1. Byte[i] & 0x7f removes the signal bit
        temperature = (((dhtData[2] & 0x7F) * 256) + dhtData[3]) / 10.0f;
        if (dhtData[2] & 0x80) {
            temperature *= -1;
        }
        return DHT_OK;
    }
    else return DHT_CHECKSUM_ERROR;
}


float getHumidity() {
    return humidity;
}

float getTemperature() {
    return temperature;
}

/**
 * DHT22_task
 */
static void DHT22_task(void* pvParamenter) {
    // printf("Starting DHT task\n\n");

    for (;;) {

        int ret = readDHT();
        errorHandler(ret);

        // printf("Humi: %.1f\n", getHumidity());
        // printf("Temp: %.1f\n", getTemperature());

        // Wait at least 2 seconds to read again
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}



void DHT22_task_start(void) {
    xTaskCreatePinnedToCore(&DHT22_task, "DHT22_task", DHT22_TASK_STACK_SIZE, NULL, DHT22_TASK_PRIORITY, NULL, DHT22_TASK_CORE_ID);

}