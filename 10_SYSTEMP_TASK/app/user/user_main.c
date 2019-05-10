/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2016 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS ESP8266 only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */

#include "ets_sys.h"
#include "osapi.h"

#include "user_interface.h"
#include "driver/uart.h"
#include "gpio.h"
#include "eagle_soc.h"
#include "driver/delay.h"
#include "driver/led.h"
#include "driver/key.h"
#include "driver/HwTimer.h"
#include "osapi.h"
#include "mem.h"				// �ڴ�����Ⱥ���

/*
 *	�� ��������ָ��
 *	�� ��������ָ��ռ�
 *	�� ��������
 *	�� ����������
 *	�� ��ϵͳ��������
 *	�� ��д������(������Ϣ����/��Ϣ����ʵ����Ӧ����)
 * */
#define	TEST_QUEUE_LEN		4	//��Ϣ�������
os_event_t	*testQueue;			//�� ��������ָ��

//�� ��������  �βα���Ϊs_event_t *����
void test_task(os_event_t * Task_message)
{
	//�� ��ϵͳ��������
	os_printf("��Ϣ����=%d����Ϣ����=%c\r\n",Task_message->sig, Task_message->par);
}

uint32 ICACHE_FLASH_ATTR user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 5;

            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;

            break;

        case FLASH_SIZE_16M_MAP_512_512:
            rf_cal_sec = 512 - 5;

            break;
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;

            break;

        case FLASH_SIZE_32M_MAP_512_512:
            rf_cal_sec = 1024 - 5;

            break;
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;

            break;

        case FLASH_SIZE_64M_MAP_1024_1024:
            rf_cal_sec = 2048 - 5;

            break;

        case FLASH_SIZE_128M_MAP_1024_1024:
            rf_cal_sec = 4096 - 5;

            break;
        default:
            rf_cal_sec = 0;

            break;
    }

    return rf_cal_sec;
}

void ICACHE_FLASH_ATTR user_rf_pre_init(void){}

/******************************************************************************
 * FunctionName : user_init
 * Description  : entry of user application, init user function here
 * Parameters   : none
 * Returns      : none
*******************************************************************************/
void ICACHE_FLASH_ATTR user_init(void)
{
	int i;
	char data = 0;
	char ch = 'a';
	uart_init(115200,115200);//���ô��ڲ�����
	DelayMs(1000);			// ��ʱ1��
//	LedInitConfig();//LED�Ƴ�ʼ������
//	KeyInitConfig();//������ʼ������
//	KeyExtiInitConfig();//���ð����ⲿ�ж�
//	TimerInitConfig(500,1);
//	hw_timer_set_func();
//	HwTimerInitConfig();
	os_printf("=============================================\r\n");
	os_printf("\t SDK version:\t%s", system_get_sdk_version());
	os_printf("\r\nǶ��ʽ�¹����˱༭����\r\nδ������ͬ������˽�Դ���\r\n");
	os_printf("\r\nϵͳ������ȴ���\r\n");
	os_printf("\r\n�����Ź�\r\n");
	os_printf("=============================================\r\n");

	//�� ��������ָ��ռ�
	testQueue = (os_event_t*)os_malloc(sizeof(os_event_t)*TEST_QUEUE_LEN);
	//�� ����������
	system_os_task(test_task, USER_TASK_PRIO_0, testQueue, TEST_QUEUE_LEN);
	for(i = 0; i < 5; i ++)
	{
		system_soft_wdt_feed();//ι������Ź�����ֹ������ƫ

		os_DelayMs(1000);			// ��ʱ1��  ϵͳ��ʱ����
		os_printf("��������Task = %d\r\n",i);

		// ��������(����1=����ȼ� / ����2=��Ϣ���� / ����3=��Ϣ����)
		// ע�⣺����3����Ϊ�޷���������������Ҫǿ������ת��
		//---------------------------------------------------------------
		system_os_post(USER_TASK_PRIO_0, data ++, ch ++);
	}
	os_printf("���񴴽����\r\n");
}



