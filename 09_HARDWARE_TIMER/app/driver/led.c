/*
 * led.c
 *
 *  Created on: 2018��8��10��
 *      Author: С����
 */
#include "driver/led.h"
void LedInitConfig(void)
{
	//1.����LED������Ӧ��GPIO4��ΪIO��ģʽ -- ����ĵ�3.3.1.1�½�
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U,	FUNC_GPIO4);	// GPIO4��ΪIO��
	//2.����GPIO4��ʼ�������ƽ -- ����ĵ�3.3.1.4��3.3.2.1�½�
	GPIO_OUTPUT_SET(GPIO_ID_PIN(4), 1);//��GPIO4��ʼ��Ϊ�ߵ�ƽ״̬--������
}


