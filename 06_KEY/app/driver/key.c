/*
 * key.c
 *
 *  Created on: 2018��8��10��
 *      Author: Marshal
 */

#include "driver/key.h"

void KeyInitConfig(void)
{
	//1.���ð�������Ӧ��GPIO0��ΪIO��ģʽ -- ����ĵ�3.3.1.1�½�
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U,	FUNC_GPIO0);	// GPIO0��ΪIO��
	//2.����GPIO0ͨ��R3���е�Դ����������GPIO0Ĭ��Ϊ�ߵ�ƽ״̬��������Ҫ���ڲ�����ȡ��
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO0_U);
	//3.�趨GPIO0����Ϊ����ģʽ
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(0));
}
