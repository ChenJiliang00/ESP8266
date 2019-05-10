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
#include "mem.h"				// 内存申请等函数

/*
 *	① 创建任务指针
 *	② 分配任务指针空间
 *	③ 创建任务
 *	④ 创建任务函数
 *	⑤ 给系统安排任务
 *	⑥ 编写任务函数(根据消息类型/消息参数实现相应功能)
 * */
#define	TEST_QUEUE_LEN		4	//消息队列深度
os_event_t	*testQueue;			//① 创建任务指针

//④ 创建任务  形参必须为s_event_t *类型
void test_task(os_event_t * Task_message)
{
	//⑤ 给系统安排任务
	os_printf("消息类型=%d，消息参数=%c\r\n",Task_message->sig, Task_message->par);
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
	uart_init(115200,115200);//设置串口波特率
	DelayMs(1000);			// 延时1秒
//	LedInitConfig();//LED灯初始化函数
//	KeyInitConfig();//按键初始化函数
//	KeyExtiInitConfig();//配置按键外部中断
//	TimerInitConfig(500,1);
//	hw_timer_set_func();
//	HwTimerInitConfig();
	os_printf("=============================================\r\n");
	os_printf("\t SDK version:\t%s", system_get_sdk_version());
	os_printf("\r\n嵌入式陈工个人编辑资料\r\n未经本人同意请勿私自传播\r\n");
	os_printf("\r\n系统任务调度代码\r\n");
	os_printf("\r\n带看门狗\r\n");
	os_printf("=============================================\r\n");

	//② 分配任务指针空间
	testQueue = (os_event_t*)os_malloc(sizeof(os_event_t)*TEST_QUEUE_LEN);
	//③ 创建任务函数
	system_os_task(test_task, USER_TASK_PRIO_0, testQueue, TEST_QUEUE_LEN);
	for(i = 0; i < 5; i ++)
	{
		system_soft_wdt_feed();//喂软件看门狗，防止程序跑偏

		os_DelayMs(1000);			// 延时1秒  系统延时函数
		os_printf("安排任务：Task = %d\r\n",i);

		// 调用任务(参数1=任务等级 / 参数2=消息类型 / 参数3=消息参数)
		// 注意：参数3必须为无符号整数，否则需要强制类型转换
		//---------------------------------------------------------------
		system_os_post(USER_TASK_PRIO_0, data ++, ch ++);
	}
	os_printf("任务创建完成\r\n");
}



