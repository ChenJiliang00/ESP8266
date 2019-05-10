/*
 * delay.c
 *
 *  Created on: 2018年8月5日
 *      Author: Marshal
 */
#include "driver/delay.h"

/*
 * 函数名称：void DealyUs(unsigned int data)
 * 函数功能：微秒延时函数
 * 函数形参：unsigned int data：延时时间
 * */
void DelayUs(unsigned int data)
{
	int i,j;
	for(i = 0; i < data; i ++)
	{
		for(j = 0; j < 26; j ++)
		{
			//ESP8266模块外部系统晶振26MHz
			//所以当在外部系统晶振频率下，每次运行时间为1/26000000S = 1/26000MS = 1/26US
			;
		}
	}
}

/*
 * 函数名称：void DelayMs(unsigned int data)
 * 函数功能：毫秒延时函数
 * 函数形参：unsigned int data：延时时间
 * */
void DelayMs(unsigned int data)
{
	int i,j;
	for(i = 0; i < data; i ++)
	{
		for(j = 0; j < 1000; j ++)
		{
			DelayUs(1);
		}
	}
}

/*
 * 函数名称：void os_DelayMs(unsigned int data)
 * 函数功能：毫秒延时函数
 * 函数形参：unsigned int data：延时时间
 * */
void os_DelayMs(uint32_t data)
{
	uint32_t i,j;
	for(i = 0; i < data; i ++)
	{
		for(j = 0; j < 1000; j ++)
		{
			os_delay_us(1);
		}
	}
}




