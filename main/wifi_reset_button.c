/*
 * wifi_reset_button.c
 *
 *  Created on: Jan 22, 2026
 *      Author: Lobinhows
 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "tasks_common.h"
#include "WiFi_App.h" 
#include "wifi_reset_button.h"

static const char TAG[] = "wifi_reset_button";

// Semaphore handle
SemaphoreHandle_t wifi_reset_semaphore = NULL;

/**
 * ISR Handler for WIFI reset button handler
 * @param arg parameter  which can be passed to the ISR handler
 */

void IRAM_ATTR wifi_reset_button_isr_handler(void* arg) {
    // Notify the button task
    xSemaphoreGiveFromISR(wifi_reset_semaphore, NULL);
}

/**
 * WiFi reset button task reacts to a button event by sending a message to the wifi app
 * to disconnect from wifi and clear the saved credentials
 * @param pvParam parameter which can be passed to the task
 */
void wifi_reset_button_task(void* pvParam) {
    for (;;) {
        if (xSemaphoreTake(wifi_reset_semaphore, portMAX_DELAY) == pdTRUE) {
            ESP_LOGI(TAG, "WIFI RESET BUTTON INTERRUPT OCURRED");

            // Send a message to disconnect and clear credentials
            wifi_app_send_message(WIFI_APP_MSG_USER_REQUESTED_STA_DISCONNECT);
            vTaskDelay(2000 / portTICK_PERIOD_MS);
        }
    }
}

void wifi_reset_button_config(void) {
    // Create binary semaphore
    wifi_reset_semaphore = xSemaphoreCreateBinary();

    // Configure the button and set directions
    gpio_set_pull_mode(WIFI_RESET_BUTTON, GPIO_PULLUP_ONLY);
    gpio_set_direction(WIFI_RESET_BUTTON, GPIO_MODE_INPUT);
    gpio_set_intr_type(WIFI_RESET_BUTTON, GPIO_INTR_NEGEDGE);

    // Create WiFi reset button task
    xTaskCreatePinnedToCore(&wifi_reset_button_task, "wifi_reset_button", WIFI_RESET_BUTTON_TASK_STACK_SIZE, NULL, WIFI_RESET_BUTTON_TASK_PRIORITY, NULL, WIFI_RESET_BUTTON_TASK_CORE_ID);

    // Install GPIO ISR service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);

    //Attach the interrupt routine
    gpio_isr_handler_add(WIFI_RESET_BUTTON, wifi_reset_button_isr_handler, NULL);

}