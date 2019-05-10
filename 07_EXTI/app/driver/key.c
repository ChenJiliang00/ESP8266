/*
 * key.c
 *
 *  Created on: 2018年8月10日
 *      Author: 小良哥
 */

#include "driver/key.h"
#include "driver/led.h"
#include "ets_sys.h"			// 回调函数

void KeyInitConfig(void)
{
	//1.设置按键所对应的GPIO0口为IO口模式 -- 详见文档3.3.1.1章节
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U,	FUNC_GPIO0);	// GPIO0设为IO口
	//2.由于GPIO0通过R3进行电源上拉，所以GPIO0默认为高电平状态，在这里要将内部拉高取消
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO0_U);
	//3.设定GPIO0引脚为输入模式
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(0));

}

void KeyExtiInitConfig(void)
{
	//1.关闭GPIO中断
	ETS_GPIO_INTR_DISABLE();
	//2.注册中断回调函数
	ETS_GPIO_INTR_ATTACH((ets_isr_t)KeyExtiInterruptFunction, NULL);
	//3.设置外部中断IO口以及触发方式
	gpio_pin_intr_state_set(GPIO_ID_PIN(0),GPIO_PIN_INTR_NEGEDGE);	// GPIO_0下降沿中断
	//4.打开GPIO中断
	ETS_GPIO_INTR_ENABLE();
}

//按键外部中断服务函数--需要函数声明
void KeyExtiInterruptFunction(void)
{
	static i = 1;
	uint32 gpio_status;
	uint32 GpioFlag;
	//1.读取GPIO中断状态
	gpio_status = GPIO_REG_READ(GPIO_STATUS_ADDRESS);
	//2.清除中断状态位
	GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status);
	//3.判断按键的中断状态
	GpioFlag = gpio_status & (0x01 << 0);
	os_printf("\r\n进入中断\r\n");
	if(GpioFlag == 1)
	{
		LED(i);
		i = !i;
		os_printf("\r\n按下按键\r\n");
	}
}



