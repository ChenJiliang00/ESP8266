/*
 * timer.c
 *
 *  Created on: 2018年8月14日
 *      Author: 小良哥
 */

#include "driver/delay.h"
#include "driver/led.h"
#include "driver/key.h"
#include "driver/uart.h"
#include "driver/timer.h"
#include "driver/dht11.h"

/*
 * 函数名称：void TimerInitConfig(uint32_t DelayMs,bool repeat_flag)
 * 函数功能：定时器初始化函数
 * 函数形参：
 *       uint32_t DelayMs   延时时间
 *       bool repeat_flag   是否重复
 *       		1：重复   0：不重复
 * 返回值：
 * */
os_timer_t os_timer_500ms;//定义一个定时器名称
void TimerInitConfig(uint32_t DelayMs,bool repeat_flag)
{
	os_timer_disarm(&os_timer_500ms);//关闭定时器
	os_timer_setfn(&os_timer_500ms,(os_timer_func_t *)TimerBackFunction,NULL);//设置定时器回调函数
	os_timer_arm (&os_timer_500ms, DelayMs, repeat_flag);
}

/*
 * 函数名称：void TimerBackFunction(void)
 * 函数功能：定时器回调函数
 * 函数形参：无
 * 返回值：   无
 * */
void TimerBackFunction(void)
{
	if(DHT11ReadDataComplete() == 0)		// 读取DHT11温湿度值
		{
			//-------------------------------------------------
			// DHT11_Data_Array[0] == 湿度_整数_部分
			// DHT11_Data_Array[1] == 湿度_小数_部分
			// DHT11_Data_Array[2] == 温度_整数_部分
			// DHT11_Data_Array[3] == 温度_小数_部分
			// DHT11_Data_Array[4] == 校验字节
			// DHT11_Data_Array[5] == 【1:温度>=0】【0:温度<0】
			//-------------------------------------------------
			// 温度超过30℃，LED亮
			//----------------------------------------------------
			if(DHT11_Data_Array[5]==1 && DHT11_Data_Array[2]>=30)
				GPIO_OUTPUT_SET(GPIO_ID_PIN(4),0);		// LED亮
			else
				GPIO_OUTPUT_SET(GPIO_ID_PIN(4),1);		// LED灭


			// 串口输出温湿度
			//---------------------------------------------------------------------------------
			if(DHT11_Data_Array[5] == 1)			// 温度 >= 0℃
			{
				os_printf("\r\n湿度 == %d.%d %RH\r\n",DHT11_Data_Array[0],DHT11_Data_Array[1]);
				os_printf("\r\n温度 == %d.%d ℃\r\n", DHT11_Data_Array[2],DHT11_Data_Array[3]);
			}
			else // if(DHT11_Data_Array[5] == 0)	// 温度 < 0℃
			{
				os_printf("\r\n湿度 == %d.%d %RH\r\n",DHT11_Data_Array[0],DHT11_Data_Array[1]);
				os_printf("\r\n温度 == -%d.%d ℃\r\n",DHT11_Data_Array[2],DHT11_Data_Array[3]);
			}

			// OLED显示温湿度
//			DHT11NUMChar();	// DHT11数据值转成字符串
		}
}

