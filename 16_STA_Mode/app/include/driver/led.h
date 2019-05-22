/*
 * led.h
 *
 *  Created on: 2018��8��10��
 *      Author: С����
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
#define LED(x) GPIO_OUTPUT_SET(GPIO_ID_PIN(4), x) //LED�궨��

#endif /* APP_INCLUDE_DRIVER_LED_H_ */
