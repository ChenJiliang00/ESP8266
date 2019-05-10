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
int i = 1;
void TimerBackFunction(void)
{
//	static int i = 1;//Ϊʲô��̬�����������ã�����
	i = !i;
	LED(i);
	os_printf("\r\n��ʱ�����Դ���---LED�Ʒ�ת\r\n");
}

