/*
 * ds18b20.h
 *
 *  Created on: 2019年1月13日
 *      Author: 小良哥
 */

#ifndef APP_INCLUDE_DRIVER_DS18B20_H_
#define APP_INCLUDE_DRIVER_DS18B20_H_

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "gpio.h"
#include "eagle_soc.h"
#include "driver/delay.h"

u8 Ds18b20InitConfig(void);
void Ds18b20WriteData(u8 data);
uint8 Ds18b20ReadByte(void);
void Ds18b20ChangTemp(void);
void Ds18b20ReadTempCom(void);
int Ds18b20ReadTemp(void);

#endif /* APP_INCLUDE_DRIVER_DS18B20_H_ */
