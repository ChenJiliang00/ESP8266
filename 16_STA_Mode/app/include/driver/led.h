/*
 * led.h
 *
 *  Created on: 2018年8月10日
 *      Author: 小良哥
 */

#ifndef APP_INCLUDE_DRIVER_LED_H_
#define APP_INCLUDE_DRIVER_LED_H_
#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "gpio.h"
#include "eagle_soc.h"
#include "driver/delay.h"

void LedInitConfig(void);
#define LED(x) GPIO_OUTPUT_SET(GPIO_ID_PIN(4), x) //LED宏定义

#endif /* APP_INCLUDE_DRIVER_LED_H_ */
