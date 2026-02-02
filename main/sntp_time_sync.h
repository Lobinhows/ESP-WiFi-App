/*
 * sntp_time_sync.h
 *
 *  Created on: Jan 29, 2026
 *      Author: Lobinhows
 */
#ifndef SNTP_TIME_SYNC_H
#define SNTP_TIME_SYNC_H

 /**
  * Starts NTP server synchronization task
  */
void sntp_time_sync_task_start(void);

/**
 * Returns local time if set
 * @return local time buffer
 */
char* sntp_tyme_sync_get_time(void);



#endif /* __SNTP_TIME_SYNC_H__ */