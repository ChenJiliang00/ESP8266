/*
 * led.c
 *
 *  Created on: 2018年8月10日
 *      Author: 小良哥
 */
#include "driver/led.h"
void LedInitConfig(void)
{
	//1.设置LED灯所对应的GPIO4口为IO口模式 -- 详见文档3.3.1.1章节
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U,	FUNC_GPIO4);	// GPIO4设为IO口
	//2.设置GPIO4初始化输出电平 -- 详见文档3.3.1.4和3.3.2.1章节
	GPIO_OUTPUT_SET(GPIO_ID_PIN(4), 1);//将GPIO4初始化为高点平状态--》灯灭
}


