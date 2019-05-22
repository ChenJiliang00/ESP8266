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
#include "user_interface.h"

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
	u8 WIFI_STA_Connect;			// WIFI����״̬��־
	u8 ipWIFI[4] = {0};				// ����WiFiģ���IP��ַ��Ϣ
	struct ip_info ipESP8266;	// ESP8266��IP��Ϣ

	WIFI_STA_Connect = wifi_station_get_connect_status();
	/*
		STATION_IDLE = 0,			//STATION����
		STATION_CONNECTING,			//��������WIFI
		STATION_WRONG_PASSWORD,		//WIFI�������
		STATION_NO_AP_FOUND,		//δ����ָ��WIFI
		STATION_CONNECT_FAIL,		//����ʧ��
		STATION_GOT_IP		    	//���IP�����ӳɹ�
	 */
	switch(WIFI_STA_Connect)
	{
		case STATION_IDLE:				os_printf("STATION����\r\n");		break;
		case STATION_CONNECTING:		os_printf("��������WIFI\r\n");		break;
		case STATION_WRONG_PASSWORD:	os_printf("WIFI�������\r\n");		break;
		case STATION_NO_AP_FOUND:		os_printf("δ����ָ��WIFI\r\n");	break;
		case STATION_CONNECT_FAIL:		os_printf("����ʧ��\r\n");			break;
		case STATION_GOT_IP:			os_printf("���IP�����ӳɹ�\r\n");	break;
	}

	if(WIFI_STA_Connect == STATION_GOT_IP)//�����ӵ�·��������ȡIP��ַ
	{
		// �� ��ȡIP��ַ  DHCP-Client��Ĭ�Ͽ�����
		wifi_get_ip_info(STATION_IF, &ipESP8266);// ��ѯWiFi��IP����   ����ĵ�6.2.1.46�½�
		// �� ��ȡWiFi��ַ
		ipWIFI[0] = ipESP8266.ip.addr;		// IP��ַ�߰�λ == addr�Ͱ�λ
		ipWIFI[1] = ipESP8266.ip.addr>>8;	// IP��ַ�θ߰�λ == addr�εͰ�λ
		ipWIFI[2] = ipESP8266.ip.addr>>16;	// IP��ַ�εͰ�λ == addr�θ߰�λ
		ipWIFI[3] = ipESP8266.ip.addr>>24;	// IP��ַ�Ͱ�λ == addr�߰�λ
		//�� ���WiFi��IP��ַ
		os_printf("ip��ַ = %d.%d.%d.%d\r\n",ipWIFI[0],ipWIFI[1],ipWIFI[2],ipWIFI[3]);
		//�� �˳���ʱ���ص�����
		os_timer_disarm(&os_timer_500ms);		// �رն�ʱ��
	}
}

