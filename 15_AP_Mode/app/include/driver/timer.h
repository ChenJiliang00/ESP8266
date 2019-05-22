/*
 * timer.h
 *
 *  Created on: 2018年8月14日
 *      Author: 小良哥
 */

#ifndef APP_INCLUDE_DRIVER_TIMER_H_
#define APP_INCLUDE_DRIVER_TIMER_H_

#include "ets_sys.h"
#include "osapi.h"

#include "user_interface.h"
#include "gpio.h"
#include "eagle_soc.h"
#include "osapi.h"

extern os_timer_t os_timer_500ms;

void TimerInitConfig(uint32_t DelayMs,bool repeat_flag);
void TimerBackFunction(void);

#endif /* APP_INCLUDE_DRIVER_TIMER_H_ */
