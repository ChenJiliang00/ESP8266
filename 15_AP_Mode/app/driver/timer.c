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
#include "driver/apMode.h"

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
	struct ip_info ipESP8266;	// IP信息结构体
	u8  ESP8266_IP[4];		// 点分十进制形式保存IP

	// 查询并打印ESP8266的工作模式
	switch(wifi_get_opmode())	// 输出工作模式
	{
		case 0x01:	os_printf("\nESP8266_Mode = Station\n");		break;
		case 0x02:	os_printf("\nESP8266_Mode = SoftAP\n");			break;
		case 0x03:	os_printf("\nESP8266_Mode = Station+SoftAP\n");	break;
	}

	// 获取ESP8266_AP模式下的IP地址
	//【AP模式下，如果开启DHCP(默认)，并且未设置IP相关参数，ESP8266的IP地址=192.168.4.1】
	wifi_get_ip_info(SOFTAP_IF,&ipESP8266);	// 参数2：IP信息结构体指针

	// ESP8266_AP_IP.ip.addr==32位二进制IP地址，将它转换为点分十进制的形式
	ESP8266_IP[0] = ipESP8266.ip.addr;			// 点分十进制IP的第一个数 <==> addr低八位
	ESP8266_IP[1] = ipESP8266.ip.addr>>8;		// 点分十进制IP的第二个数 <==> addr次低八位
	ESP8266_IP[2] = ipESP8266.ip.addr>>16;		// 点分十进制IP的第三个数 <==> addr次高八位
	ESP8266_IP[3] = ipESP8266.ip.addr>>24;		// 点分十进制IP的第四个数 <==> addr高八位

	// 打印ESP8266的IP地址
	os_printf("ESP8266_IP = %d.%d.%d.%d\n",ESP8266_IP[0],ESP8266_IP[1],ESP8266_IP[2],ESP8266_IP[3]);
	// 查询并打印接入此WIFI的设备数量
	os_printf("Number of devices connected to this WIFI = %d\n",wifi_softap_get_station_num());
}

