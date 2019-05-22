#include "driver/apMode.h"

void ICACHE_FLASH_ATTR WifiSetAPModeInit(void)
{
	struct softap_config APConfig;		// AP�����ṹ��
	wifi_set_opmode(0x02);				// ����ΪAPģʽ�������浽Flash ����ĵ�6.2.1.3�½�

	os_memset(&APConfig, 0, sizeof(struct softap_config));	// ���ṹ������
	os_strcpy(APConfig.ssid, WiFiName);						// ����WiFi����
	os_strcpy(APConfig.password, WiFiPassWord);				//����WiFi����
	APConfig.ssid_len=os_strlen(WiFiName);					// ����ssid����(��SSID�ĳ���һ��)
	APConfig.channel=1;                      				// ͨ����1��13
	APConfig.authmode=AUTH_WPA2_PSK;           				// ���ü���ģʽ
	APConfig.ssid_hidden=0;                  				// ������SSID
	APConfig.max_connection=4;               				// ���������
	APConfig.beacon_interval=100;            				// �ű���ʱ��100��60000 ms

	wifi_softap_set_config(&APConfig);						// ����soft-AP�������浽Flash
}




