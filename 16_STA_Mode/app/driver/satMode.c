#include "driver/staMode.h"

void ICACHE_FLASH_ATTR WifiSetSTAModeInit(void)
{
	/*
	 * 配置ESP8266为STA模式连接路由器步骤
	 * ① 创建WiFi结构体
	 * ② 设置WiFi模块为STA模式
	 * ③ 配置连接的路由器信息
	 * ④ 将WiFi设置保存到Flash
	 * */
	struct station_config staConfig;	// STA参数结构体

	wifi_set_opmode(0x01);				// 设置为STA模式，并保存到Flash 详见文档6.2.1.3章节
	// 结构体赋值，配置STA模式参数
	os_memset(&staConfig, 0, sizeof(struct station_config));	// STA参数结构体 = 0
	os_strcpy(staConfig.ssid, staWiFiName);						// 设置WIFI名
	os_strcpy(staConfig.password, staWiFiPassWord);				// 设置WIFI密码

	wifi_station_set_config(&staConfig);	// 设置STA参数，并保存到Flash
}


