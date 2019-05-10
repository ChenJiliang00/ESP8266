/*
 * delay.c
 *
 *  Created on: 2018��8��5��
 *      Author: Marshal
 */
#include "driver/delay.h"

/*
 * �������ƣ�void DealyUs(unsigned int data)
 * �������ܣ�΢����ʱ����
 * �����βΣ�unsigned int data����ʱʱ��
 * */
void DelayUs(unsigned int data)
{
	int i,j;
	for(i = 0; i < data; i ++)
	{
		for(j = 0; j < 26; j ++)
		{
			//ESP8266ģ���ⲿϵͳ����26MHz
			//���Ե����ⲿϵͳ����Ƶ���£�ÿ������ʱ��Ϊ1/26000000S = 1/26000MS = 1/26US
			;
		}
	}
}

/*
 * �������ƣ�void DelayMs(unsigned int data)
 * �������ܣ�������ʱ����
 * �����βΣ�unsigned int data����ʱʱ��
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
 * �������ƣ�void os_DelayMs(unsigned int data)
 * �������ܣ�������ʱ����
 * �����βΣ�unsigned int data����ʱʱ��
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




