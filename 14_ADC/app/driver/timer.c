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
	//����ADC��������ֵ
	os_printf("ADC�����ֵ=%u\r\n", system_adc_read());
	//�����ADC��ֵ��Ӧ�ĵ�ѹֵ
	os_printf("ʵ�ʵ�ѹ=0.");
	os_printf("%u\r\n", system_adc_read()*100/1024);
}

