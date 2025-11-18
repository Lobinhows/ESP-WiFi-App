/*
 * DHT22.h
 *
 *  Created on: Nov 12, 2025
 *      Author: Lobinhows
 */
#ifndef DHT22_H
#define DHT22_H

#define DHT_OK 0
#define DHT_CHECKSUM_ERROR -1
#define DHT_TIMEOUT_ERROR -2

#define DHT22_GPIO 23

 /**
  * @brief Start DHT22 sensor task
  */
void DHT22_task_start(void);

void errorHandler(int response);
int readDHT();
float getHumidity();
float getTemperature();


#endif /* __DHT22_H__ */