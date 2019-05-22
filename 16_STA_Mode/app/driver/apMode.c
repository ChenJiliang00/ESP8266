#include "driver/apMode.h"

void ICACHE_FLASH_ATTR WifiSetAPModeInit(void)
{
	struct softap_config APConfig;		// AP参数结构体
	wifi_set_opmode(0x02);				// 设置为AP模式，并保存到Flash 详见文档6.2.1.3章节

	os_memset(&APConfig, 0, sizeof(struct softap_config));	// 将结构体清零
	os_strcpy(APConfig.ssid, WiFiName);						// 设置WiFi名称
	os_strcpy(APConfig.password, WiFiPassWord);				//设置WiFi密码
	APConfig.ssid_len=os_strlen(WiFiName);					// 设置ssid长度(和SSID的长度一致)
	APConfig.channel=1;                      				// 通道号1～13
	APConfig.authmode=AUTH_WPA2_PSK;           				// 设置加密模式
	APConfig.ssid_hidden=0;                  				// 不隐藏SSID
	APConfig.max_connection=4;               				// 最大连接数
	APConfig.beacon_interval=100;            				// 信标间隔时槽100～60000 ms

	wifi_softap_set_config(&APConfig);						// 设置soft-AP，并保存到Flash
}




