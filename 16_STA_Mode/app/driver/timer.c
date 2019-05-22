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
#include "user_interface.h"

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
	u8 WIFI_STA_Connect;			// WIFI接入状态标志
	u8 ipWIFI[4] = {0};				// 保存WiFi模块的IP地址信息
	struct ip_info ipESP8266;	// ESP8266的IP信息

	WIFI_STA_Connect = wifi_station_get_connect_status();
	/*
		STATION_IDLE = 0,			//STATION闲置
		STATION_CONNECTING,			//正在连接WIFI
		STATION_WRONG_PASSWORD,		//WIFI密码错误
		STATION_NO_AP_FOUND,		//未发现指定WIFI
		STATION_CONNECT_FAIL,		//连接失败
		STATION_GOT_IP		    	//获得IP，连接成功
	 */
	switch(WIFI_STA_Connect)
	{
		case STATION_IDLE:				os_printf("STATION闲置\r\n");		break;
		case STATION_CONNECTING:		os_printf("正在连接WIFI\r\n");		break;
		case STATION_WRONG_PASSWORD:	os_printf("WIFI密码错误\r\n");		break;
		case STATION_NO_AP_FOUND:		os_printf("未发现指定WIFI\r\n");	break;
		case STATION_CONNECT_FAIL:		os_printf("连接失败\r\n");			break;
		case STATION_GOT_IP:			os_printf("获得IP，连接成功\r\n");	break;
	}

	if(WIFI_STA_Connect == STATION_GOT_IP)//已连接到路由器，获取IP地址
	{
		// ① 获取IP地址  DHCP-Client是默认开启的
		wifi_get_ip_info(STATION_IF, &ipESP8266);// 查询WiFi的IP参数   详见文档6.2.1.46章节
		// ② 获取WiFi地址
		ipWIFI[0] = ipESP8266.ip.addr;		// IP地址高八位 == addr低八位
		ipWIFI[1] = ipESP8266.ip.addr>>8;	// IP地址次高八位 == addr次低八位
		ipWIFI[2] = ipESP8266.ip.addr>>16;	// IP地址次低八位 == addr次高八位
		ipWIFI[3] = ipESP8266.ip.addr>>24;	// IP地址低八位 == addr高八位
		//③ 输出WiFi的IP地址
		os_printf("ip地址 = %d.%d.%d.%d\r\n",ipWIFI[0],ipWIFI[1],ipWIFI[2],ipWIFI[3]);
		//④ 退出定时器回调函数
		os_timer_disarm(&os_timer_500ms);		// 关闭定时器
	}
}

