/*
 * key.c
 *
 *  Created on: 2018��8��10��
 *      Author: С����
 */

#include "driver/key.h"
#include "driver/led.h"
#include "ets_sys.h"			// �ص�����

void KeyInitConfig(void)
{
	//1.���ð�������Ӧ��GPIO0��ΪIO��ģʽ -- ����ĵ�3.3.1.1�½�
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U,	FUNC_GPIO0);	// GPIO0��ΪIO��
	//2.����GPIO0ͨ��R3���е�Դ����������GPIO0Ĭ��Ϊ�ߵ�ƽ״̬��������Ҫ���ڲ�����ȡ��
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO0_U);
	//3.�趨GPIO0����Ϊ����ģʽ
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(0));

}

void KeyExtiInitConfig(void)
{
	//1.�ر�GPIO�ж�
	ETS_GPIO_INTR_DISABLE();
	//2.ע���жϻص�����
	ETS_GPIO_INTR_ATTACH((ets_isr_t)KeyExtiInterruptFunction, NULL);
	//3.�����ⲿ�ж�IO���Լ�������ʽ
	gpio_pin_intr_state_set(GPIO_ID_PIN(0),GPIO_PIN_INTR_NEGEDGE);	// GPIO_0�½����ж�
	//4.��GPIO�ж�
	ETS_GPIO_INTR_ENABLE();
}

//�����ⲿ�жϷ�����--��Ҫ��������
void KeyExtiInterruptFunction(void)
{
	static i = 1;
	uint32 gpio_status;
	uint32 GpioFlag;
	//1.��ȡGPIO�ж�״̬
	gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	//2.����ж�״̬λ
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
	//3.�жϰ������ж�״̬
	GpioFlag = gpio_status & (0x01 << 0);
	os_printf("\r\n�����ж�\r\n");
	if(GpioFlag == 1)
	{
		LED(i);
		i = !i;
		os_printf("\r\n���°���\r\n");
	}
}



