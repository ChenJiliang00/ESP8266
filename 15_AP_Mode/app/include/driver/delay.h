/*
 * delay.h
 *
 *  Created on: 2018Äê8ÔÂ5ÈÕ
 *      Author: Marshal
 */

#ifndef APP_INCLUDE_DRIVER_DELAY_H_
#define APP_INCLUDE_DRIVER_DELAY_H_

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include <stdio.h>

void ICACHE_FLASH_ATTR DelayUs(unsigned int data);
void ICACHE_FLASH_ATTR DelayMs(unsigned int data);
void os_DelayMs(uint32_t data);

#endif /* APP_INCLUDE_DRIVER_DELAY_H_ */
