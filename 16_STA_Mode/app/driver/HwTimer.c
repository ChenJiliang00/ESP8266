/*
 * HwTimer.c
 *
 *  Created on: 2018��9��10��
 *      Author: С����
 */
#include "driver/HwTimer.h"
#include "driver/led.h"
#include "driver/uart.h"

/*
 * �������ƣ�void HwTimerInterrupt(void)
 * �������ܣ�Ӳ����ʱ���ص�����
 * �����βΣ���
 * ��  ��  ֵ����
 * */
void HwTimerInterrupt(void)
{
	static int LedFlag = 1;
	LedFlag = !LedFlag;
	LED(LedFlag);
	os_printf("Ӳ����ʱ���ж�!!!\r\n");
}

/*
 *
 * */
void HwTimerInitConfig(void)
{
//	hw_timer_init(0,1);	//ʹ��FRC1�ж�Դ  FRC1_SOURCE[ö��Ϊ0],�Զ���װ[0:���Զ���װ  1:�Զ���װ]
	hw_timer_init(1,1);	//ʹ��NMI �ж�Դ  FRC1_SOURCE[ö��Ϊ0],�Զ���װ[0:���Զ���װ  1:�Զ���װ]
	hw_timer_set_func(HwTimerInterrupt);//����HwTimerInterrupt()Ϊ��ʱ���ص�����
	hw_timer_arm(1000000);//ʹ��Ӳ����ʱ��  1������ʱ[��λ��us  ���ֵ:1677721us]
}




