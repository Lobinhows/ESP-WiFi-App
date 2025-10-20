/* 
 * rgb.h 
 * 
 *  Created on: Oct 15, 2025
 *      Author: Lobinhows        
 */
#ifndef RGB_LED_H
#define RGB_LED_H

//  RGB LED GPIOS 
#define RGB_R_Pin 12
#define RGB_G_Pin 13
#define RGB_B_Pin 14

//  RGB LED Channels
#define RGB_Channel_Num 3

//  RGB Configuration
typedef struct{
    int channel;
    int gpio;
    int mode;
    int timer_index;
} ledc_info_t;


//  Indicates WiFi has started
void rgb_led_wifi_started(void);

//  Indicates HTTP Server has started
void rgb_led_http_started(void);

//  Indicates WiFi is connected to acces point
void rgb_led_wifi_connected(void);


#endif /* RGB_LED_H */