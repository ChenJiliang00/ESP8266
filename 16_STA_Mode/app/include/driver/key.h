/*
 * key.h
 *
 *  Created on: 2018��8��10��
 *      Author: С����
 */

#ifndef APP_INCLUDE_DRIVER_KEY_H_
#define APP_INCLUDE_DRIVER_KEY_H_

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "gpio.h"
#include "eagle_soc.h"
#include "driver/delay.h"

void KeyInitConfig(void);
void KeyExtiInitConfig(void);
void KeyExtiInterruptFunction(void);
#define KEY (!!(GPIO_INPUT_GET(GPIO_ID_PIN(0))))

#endif /* APP_INCLUDE_DRIVER_KEY_H_ */
