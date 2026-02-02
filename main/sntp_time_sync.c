#include "sntp_time_sync.h"
/*
 * sntp_time_sync.c
 *
 *  Created on: Jan 29, 2026
 *      Author: Lobinhows
 */

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lwip/apps/sntp.h"
#include "time.h"

#include "tasks_common.h"
#include "http_server.h"
#include "WiFi_App.h"
#include "sntp_time_sync.h"

static const char TAG[] = "sntp_time_sync";

// SNTP operating mode set 
static bool sntp_op_mode_set = false;

/**
 * Initialize SNTP service using SNTP_OPMODE_POLL mode
 */
static void sntp_time_sync_init_sntp(void) {
    ESP_LOGI(TAG, "Initializing SNTP service");

    if (!sntp_op_mode_set) {
        // Set operating mode
        sntp_setoperatingmode(SNTP_OPMODE_POLL);
        sntp_op_mode_set = true;
        sntp_setservername(0, "pool.ntp.org");

        // Initialize the servers
        sntp_init();

        // Let HTTP server know service is initialized
        http_server_monitor_send_msg(HTTP_MSG_TIME_SERVER_INITIALIZED);
    }
}

/**
 * Gets current time. If current time is not up to date, sntp_time_sync_init_sntp is called.
 */
static void sntp_time_sync_obtain_time(void) {
    time_t now = 0;
    struct tm time_info = { 0 };

    time(&now);
    localtime_r(&now, &time_info);
    // Check the time in case we need to (re)initialize
    // Bugged when updating over the air! (Apparently, time remains updated and never passes this IF, therefore never initializes sntp again)
    if (time_info.tm_year < (2026 - 1900)) {
        sntp_time_sync_init_sntp();
        // Set local time zone
        setenv("TZ", "<-03>3", 1);
        tzset();
    }
}

/**
 * SNTP time synchronization task
 * @param arg pvParam
 */
static void sntp_time_sync(void* pvParam) {
    while (1) {
        sntp_time_sync_obtain_time();
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }
    vTaskDelete(NULL);
}

char* sntp_tyme_sync_get_time(void) {
    static char time_buffer[100] = { 0 };

    time_t now = 0;
    struct tm time_info = { 0 };
    time(&now);
    localtime_r(&now, &time_info);

    if (time_info.tm_year < (2026 - 1900)) {
        ESP_LOGI(TAG, "Time is not set yet!");
    }
    else {
        strftime(time_buffer, sizeof(time_buffer), "%d/%m/%Y %H:%M:%S", &time_info);
        ESP_LOGI(TAG, "Current time info: %s", time_buffer);
    }
    return time_buffer;
}


void sntp_time_sync_task_start(void) {
    xTaskCreatePinnedToCore(&sntp_time_sync, "sntp_time_sync", SNTP_TIME_SYNC_TASK_STACK_SIZE, NULL, SNTP_TIME_SYNC_TASK_PRIORITY, NULL, SNTP_TIME_SYNC_TASK_CORE_ID);
}