/*
 * tasks_common.h
 *
 *  Created on: Oct 16, 2025
 *      Author: Lobinhows
 */
#ifndef TASKS_COMMON_H
#define TASKS_COMMON_H

 // WiFi app task
#define WIFI_APP_TASK_STACK_SIZE    4096
#define WIFI_APP_TASK_PRIORITY      5
#define WIFI_APP_TASK_CORE_ID       0

// HTTP Server task
#define HTTP_SERVER_TASK_STACK_SIZE 8192
#define HTTP_SERVER_TASK_PRIORITY   4
#define HTTP_SERVER_TASK_CORE_ID    0

// HTTP Server Monitor task
#define HTTP_SERVER_MONITOR_STACK_SIZE  4096
#define HTTP_SERVER_MONITOR_PRIORITY    3
#define HTTP_SERVER_MONITOR_CORE_ID     0

#endif // __TASKS_COMMON_H__