/*
 * timer.c
 *
 *  Created on: 2018��8��14��
 *      Author: С����
 */

#include "driver/delay.h"
#include "driver/led.h"
#include "driver/key.h"
#include "driver/uart.h"
#include "driver/timer.h"
#include "driver/dht11.h"
#include "driver/apMode.h"

/*
 * �������ƣ�void TimerInitConfig(uint32_t DelayMs,bool repeat_flag)
 * �������ܣ���ʱ����ʼ������
 * �����βΣ�
 *       uint32_t DelayMs   ��ʱʱ��
 *       bool repeat_flag   �Ƿ��ظ�
 *       		1���ظ�   0�����ظ�
 * ����ֵ��
 * */
os_timer_t os_timer_500ms;//����һ����ʱ������
void TimerInitConfig(uint32_t DelayMs,bool repeat_flag)
{
	os_timer_disarm(&os_timer_500ms);//�رն�ʱ��
	os_timer_setfn(&os_timer_500ms,(os_timer_func_t *)TimerBackFunction,NULL);//���ö�ʱ���ص�����
	os_timer_arm (&os_timer_500ms, DelayMs, repeat_flag);
}

/*
 * �������ƣ�void TimerBackFunction(void)
 * �������ܣ���ʱ���ص�����
 * �����βΣ���
 * ����ֵ��   ��
 * */
void TimerBackFunction(void)
{
	struct ip_info ipESP8266;	// IP��Ϣ�ṹ��
	u8  ESP8266_IP[4];		// ���ʮ������ʽ����IP

	// ��ѯ����ӡESP8266�Ĺ���ģʽ
	switch(wifi_get_opmode())	// �������ģʽ
	{
		case 0x01:	os_printf("\nESP8266_Mode = Station\n");		break;
		case 0x02:	os_printf("\nESP8266_Mode = SoftAP\n");			break;
		case 0x03:	os_printf("\nESP8266_Mode = Station+SoftAP\n");	break;
	}

	// ��ȡESP8266_APģʽ�µ�IP��ַ
	//��APģʽ�£��������DHCP(Ĭ��)������δ����IP��ز�����ESP8266��IP��ַ=192.168.4.1��
	wifi_get_ip_info(SOFTAP_IF,&ipESP8266);	// ����2��IP��Ϣ�ṹ��ָ��

	// ESP8266_AP_IP.ip.addr==32λ������IP��ַ������ת��Ϊ���ʮ���Ƶ���ʽ
	ESP8266_IP[0] = ipESP8266.ip.addr;			// ���ʮ����IP�ĵ�һ���� <==> addr�Ͱ�λ
	ESP8266_IP[1] = ipESP8266.ip.addr>>8;		// ���ʮ����IP�ĵڶ����� <==> addr�εͰ�λ
	ESP8266_IP[2] = ipESP8266.ip.addr>>16;		// ���ʮ����IP�ĵ������� <==> addr�θ߰�λ
	ESP8266_IP[3] = ipESP8266.ip.addr>>24;		// ���ʮ����IP�ĵ��ĸ��� <==> addr�߰�λ

	// ��ӡESP8266��IP��ַ
	os_printf("ESP8266_IP = %d.%d.%d.%d\n",ESP8266_IP[0],ESP8266_IP[1],ESP8266_IP[2],ESP8266_IP[3]);
	// ��ѯ����ӡ�����WIFI���豸����
	os_printf("Number of devices connected to this WIFI = %d\n",wifi_softap_get_station_num());
}

