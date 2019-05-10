/*
 * timer.c
 *
 *  Created on: 2018��8��14��
 *      Author: С����
 */

#include "driver/delay.h"
#include "driver/led.h"
#include "driver/key.h"
#include "driver/uart.h"
#include "driver/timer.h"
#include "driver/dht11.h"

/*
 * �������ƣ�void TimerInitConfig(uint32_t DelayMs,bool repeat_flag)
 * �������ܣ���ʱ����ʼ������
 * �����βΣ�
 *       uint32_t DelayMs   ��ʱʱ��
 *       bool repeat_flag   �Ƿ��ظ�
 *       		1���ظ�   0�����ظ�
 * ����ֵ��
 * */
os_timer_t os_timer_500ms;//����һ����ʱ������
void TimerInitConfig(uint32_t DelayMs,bool repeat_flag)
{
	os_timer_disarm(&os_timer_500ms);//�رն�ʱ��
	os_timer_setfn(&os_timer_500ms,(os_timer_func_t *)TimerBackFunction,NULL);//���ö�ʱ���ص�����
	os_timer_arm (&os_timer_500ms, DelayMs, repeat_flag);
}

/*
 * �������ƣ�void TimerBackFunction(void)
 * �������ܣ���ʱ���ص�����
 * �����βΣ���
 * ����ֵ��   ��
 * */
void TimerBackFunction(void)
{
	if(DHT11ReadDataComplete() == 0)		// ��ȡDHT11��ʪ��ֵ
		{
			//-------------------------------------------------
			// DHT11_Data_Array[0] == ʪ��_����_����
			// DHT11_Data_Array[1] == ʪ��_С��_����
			// DHT11_Data_Array[2] == �¶�_����_����
			// DHT11_Data_Array[3] == �¶�_С��_����
			// DHT11_Data_Array[4] == У���ֽ�
			// DHT11_Data_Array[5] == ��1:�¶�>=0����0:�¶�<0��
			//-------------------------------------------------
			// �¶ȳ���30�棬LED��
			//----------------------------------------------------
			if(DHT11_Data_Array[5]==1 && DHT11_Data_Array[2]>=30)
				GPIO_OUTPUT_SET(GPIO_ID_PIN(4),0);		// LED��
			else
				GPIO_OUTPUT_SET(GPIO_ID_PIN(4),1);		// LED��


			// ���������ʪ��
			//---------------------------------------------------------------------------------
			if(DHT11_Data_Array[5] == 1)			// �¶� >= 0��
			{
				os_printf("\r\nʪ�� == %d.%d %RH\r\n",DHT11_Data_Array[0],DHT11_Data_Array[1]);
				os_printf("\r\n�¶� == %d.%d ��\r\n", DHT11_Data_Array[2],DHT11_Data_Array[3]);
			}
			else // if(DHT11_Data_Array[5] == 0)	// �¶� < 0��
			{
				os_printf("\r\nʪ�� == %d.%d %RH\r\n",DHT11_Data_Array[0],DHT11_Data_Array[1]);
				os_printf("\r\n�¶� == -%d.%d ��\r\n",DHT11_Data_Array[2],DHT11_Data_Array[3]);
			}

			// OLED��ʾ��ʪ��
//			DHT11NUMChar();	// DHT11����ֵת���ַ���
		}
}

