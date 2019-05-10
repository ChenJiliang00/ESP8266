/*
 * key.c
 *
 *  Created on: 2018年8月10日
 *      Author: Marshal
 */

#include "driver/key.h"

void KeyInitConfig(void)
{
	//1.设置按键所对应的GPIO0口为IO口模式 -- 详见文档3.3.1.1章节
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U,	FUNC_GPIO0);	// GPIO0设为IO口
	//2.由于GPIO0通过R3进行电源上拉，所以GPIO0默认为高电平状态，在这里要将内部拉高取消
	PIN_PULLUP_DIS(PERIPHS_IO_MUX_GPIO0_U);
	//3.设定GPIO0引脚为输入模式
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(0));
}
