/* 
 * rgb.c 
 * 
 *  Created on: Oct 15, 2025
 *      Author: Lobinhows        
 */

#include <stdbool.h>
#include <driver/ledc.h>
#include "RGB.h"
#include <esp_private/esp_gpio_reserve.h>

bool g_pwm_init_handle = false;

ledc_info_t ledc_ch[RGB_Channel_Num];


static void rgb_pwm_init(void){
    int rgb_channel;

    // Red
    ledc_ch[0].channel = LEDC_CHANNEL_0;
    ledc_ch[0].gpio = RGB_R_Pin;
    ledc_ch[0].mode = LEDC_HIGH_SPEED_MODE;
    ledc_ch[0].timer_index = LEDC_TIMER_0;

    // Green
    ledc_ch[1].channel = LEDC_CHANNEL_1;
    ledc_ch[1].gpio = RGB_G_Pin;
    ledc_ch[1].mode = LEDC_HIGH_SPEED_MODE;
    ledc_ch[1].timer_index = LEDC_TIMER_0;

    // Blue
    ledc_ch[2].channel = LEDC_CHANNEL_2;
    ledc_ch[2].gpio = RGB_B_Pin;
    ledc_ch[2].mode = LEDC_HIGH_SPEED_MODE;
    ledc_ch[2].timer_index = LEDC_TIMER_0;

    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_8_BIT,
        .freq_hz = 100,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .timer_num = LEDC_TIMER_0
    };
    ledc_timer_config(&ledc_timer);

    for (rgb_channel = 0; rgb_channel<RGB_Channel_Num;rgb_channel++){
        ledc_channel_config_t ledc_config =
        {
            .channel = ledc_ch[rgb_channel].channel,
            .duty = 0,
            .hpoint =0,
            .gpio_num = ledc_ch[rgb_channel].gpio,
            .intr_type = LEDC_INTR_DISABLE,
            .speed_mode = ledc_ch[rgb_channel].mode,
            .timer_sel = ledc_ch[rgb_channel].timer_index
        };
        ledc_channel_config(&ledc_config);
        esp_gpio_revoke(BIT64(ledc_ch[rgb_channel].gpio));
    }

}

static void rgb_set_color(uint8_t r, uint8_t g, uint8_t b){
    // Value between 0 and 255 (8 bit )
    r = 255-r;
    g = 255-g;
    b = 255-b;
    ledc_set_duty(ledc_ch[0].mode, ledc_ch[0].channel, r);
    ledc_set_duty(ledc_ch[1].mode, ledc_ch[1].channel, g);
    ledc_set_duty(ledc_ch[2].mode, ledc_ch[2].channel, b);
    // esp_gpio_revoke(BIT64(12));
    
    ledc_update_duty(ledc_ch[0].mode, ledc_ch[0].channel);
    ledc_update_duty(ledc_ch[1].mode, ledc_ch[1].channel);
    ledc_update_duty(ledc_ch[2].mode, ledc_ch[2].channel);
    // esp_gpio_revoke(BIT64(13));

    // esp_gpio_revoke(BIT64(14));
}

void rgb_led_wifi_started(void){
    if (g_pwm_init_handle == false){
        rgb_pwm_init();
    }
    rgb_set_color(220, 240, 12);

}

void rgb_led_http_started(void){
    if (g_pwm_init_handle == false){
        rgb_pwm_init();
    }
    rgb_set_color(242, 95,6);
}

void rgb_led_wifi_connected(void){
    if (g_pwm_init_handle == false){
        rgb_pwm_init();
    }
    rgb_set_color(202, 48, 223);
}