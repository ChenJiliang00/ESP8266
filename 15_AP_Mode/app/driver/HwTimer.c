/*
 * HwTimer.c
 *
 *  Created on: 2018年9月10日
 *      Author: 小良哥
 */
#include "driver/HwTimer.h"
#include "driver/led.h"
#include "driver/uart.h"

/*
 * 函数名称：void HwTimerInterrupt(void)
 * 函数功能：硬件定时器回调函数
 * 函数形参：无
 * 返  回  值：无
 * */
void HwTimerInterrupt(void)
{
	static int LedFlag = 1;
	LedFlag = !LedFlag;
	LED(LedFlag);
	os_printf("硬件定时器中断!!!\r\n");
}

/*
 *
 * */
void HwTimerInitConfig(void)
{
//	hw_timer_init(0,1);	//使用FRC1中断源  FRC1_SOURCE[枚举为0],自动填装[0:不自动填装  1:自动填装]
	hw_timer_init(1,1);	//使用NMI 中断源  FRC1_SOURCE[枚举为0],自动填装[0:不自动填装  1:自动填装]
	hw_timer_set_func(HwTimerInterrupt);//设置HwTimerInterrupt()为定时器回调函数
	hw_timer_arm(1000000);//使能硬件定时器  1秒钟延时[单位：us  最大值:1677721us]
}




