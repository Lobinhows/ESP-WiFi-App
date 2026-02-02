/*
 * wifi_reset_button.h
 *
 *  Created on: Jan 22, 2026
 *      Author: Lobinhows
 */
#ifndef WIFI_RESET_BUTTON_H
#define WIFI_RESET_BUTTON_H

 // Default interrupt flag
#define ESP_INTR_FLAG_DEFAULT   0

// WiFi Reset button GPIO
#define WIFI_RESET_BUTTON       0 // Boot button

/**
 * Configures WiFi reset button and interrupt
 */
void wifi_reset_button_config(void);

#endif /* __WIFI_RESET_BUTTON_H__ */