#include "driver/staMode.h"

void ICACHE_FLASH_ATTR WifiSetSTAModeInit(void)
{
	/*
	 * ����ESP8266ΪSTAģʽ����·��������
	 * �� ����WiFi�ṹ��
	 * �� ����WiFiģ��ΪSTAģʽ
	 * �� �������ӵ�·������Ϣ
	 * �� ��WiFi���ñ��浽Flash
	 * */
	struct station_config staConfig;	// STA�����ṹ��

	wifi_set_opmode(0x01);				// ����ΪSTAģʽ�������浽Flash ����ĵ�6.2.1.3�½�
	// �ṹ�帳ֵ������STAģʽ����
	os_memset(&staConfig, 0, sizeof(struct station_config));	// STA�����ṹ�� = 0
	os_strcpy(staConfig.ssid, staWiFiName);						// ����WIFI��
	os_strcpy(staConfig.password, staWiFiPassWord);				// ����WIFI����

	wifi_station_set_config(&staConfig);	// ����STA�����������浽Flash
}


