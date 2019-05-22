/*
 * dht11.h
 *
 *  Created on: 2019年1月15日
 *      Author: Marshal
 */

#ifndef APP_INCLUDE_DRIVER_DHT11_H_
#define APP_INCLUDE_DRIVER_DHT11_H_

#include "ets_sys.h"
#include "osapi.h"
#include "user_interface.h"
#include "driver/uart.h"
#include "gpio.h"
#include "eagle_soc.h"
#include "driver/delay.h"

extern u8 DHT11_Data_Array[6];	// DHT11数据数组
extern u8 DHT11_Data_Char[2][10];	// DHT11数据字符串【行：湿/温度】【列：数据字符串】

void Dht11OutputConfig(bool val);
void Dht11InputConfig(void);
u8 Dht11Star(void);
u8 DHT11ReadBit(void);
u8 DHT11ReadByte(void);
u8 DHT11ReadDataComplete(void);
void DHT11NUMChar(void);

#endif /* APP_INCLUDE_DRIVER_DHT11_H_ */
