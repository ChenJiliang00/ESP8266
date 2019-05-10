/*
 * ds18b20.c
 *
 *  Created on: 2019年1月13日
 *      Author: 小良哥
 */
#include "driver/ds18b20.h"
#include "c_types.h"
#include "driver/delay.h"
/*
 *函数名称：u8 Ds18b20InitConfig(void)
 *函数功能：ds18b20模块初始化
 *函数形参：无
 *返  回  值：
 *			0：da18b20存在且正常
 *			其他值：ds18b20异常
 * */
u8 Ds18b20InitConfig(void)
{
	u16 cnt = 0;
	//1.设置LED灯所对应的GPIO4口为IO口模式 -- 详见文档3.3.1.1章节
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,	FUNC_GPIO13);	// GPIO13设为IO口
	//2.主机首先发出一个 480-960 微秒的低电平脉冲
	GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0);//主机发送低电平
	DelayUs(700);//低电平持续时间700US
	//3.然后释放总线变为高电平
	GPIO_OUTPUT_SET(GPIO_ID_PIN(13),1);//主机发送高电平
	//3.1 主机检测不到GPIO13的低电平，视为没有DS18B20模块
	while((GPIO_INPUT_GET(GPIO_ID_PIN(13)) == 1) && (cnt < 200))
	{
		DelayUs(1);
		cnt ++;
	}
	if(cnt >= 200) return 1;

	cnt = 0;

	//3.2 主机检测到GPIO13的低电平，但是低电平持续时间太长，则认为DS18B20模块异常
	while((GPIO_INPUT_GET(GPIO_ID_PIN(13)) == 0) && (cnt < 480))
	{
		DelayUs(1);
		cnt ++;
	}
	if(cnt >= 480) return 1;

	return 0;//成功返回0
}

/*
 * 函数名称：void Ds18b20WriteData(u8 data)
 * 函数功能：DS18B20写一个字节数据
 * 函数形参：
 * 			u8 data 写入的数据
 * 返  回  值：无
 * */
void Ds18b20WriteData(u8 data)
{
	u8 i,j;
	for(i = 0; i < 8; i ++)
	{
		//1.写周期一开始做为主机先把总线拉低 1 微秒表示写周期开始
		GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0);//主机发送低电平
		j ++;//一行代码1微秒
		//2.从低位开始写入数据
		GPIO_OUTPUT_SET(GPIO_ID_PIN(13), data & 0x01);
		DelayUs(80);//写周期最少为 60 微秒，最长不超过 120 微秒
		//释放总线为高电平
		GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 1);
		data = data >> 1;
	}
}

/*
 * 函数名称：uint8 Ds18b20ReadByte(void)
 * 函数功能：DS18B20读一个字节数据
 * 函数形参：无
 * 返  回  值：
 * 			byte：获取到的数据
 * */
uint8 Ds18b20ReadData(void)
{
	uint8 byte, data;
	int i, j;
	for (j = 8; j > 0; j--)
	{
		GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0);		//先将总线拉低1us
		i++;
		GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 1);	  	//然后释放总线
		i++;
		i++;	  //延时6us等待数据稳定
		data = GPIO_INPUT_GET(GPIO_ID_PIN(13));	 	//读取数据，从最低位开始读取
		/*将byte左移一位，然后与上右移7位后的bi，注意移动之后移掉那位补0。*/
		byte = (byte >> 1) | (data << 7);
		os_delay_us(48); //读取完之后等待48us再接着读取下一个数
	}
	return byte;
}

/*
 * 函数名称：void Ds18b20ChangTemp(void)
 * 函数功能：让18b20开始转换温度
 * 函数形参：无
 * 返  回  值：无
 * */
void Ds18b20ChangTemp(void)
{
	Ds18b20Init();
	DelayMs(1);
	Ds18b20WriteData(0xcc);		//跳过ROM操作命令
	Ds18b20WriteData(0x44);	    //温度转换命令
//	Delay1ms(100);	//等待转换成功，而如果你是一直刷着的话，就不用这个延时了

}

/*
 * 函数名称：void Ds18b20ReadTempCom(void)
 * 函数功能：发送读取温度命令
 * 函数形参：无
 * 返  回  值：无
 * */
void Ds18b20ReadTempCom(void)
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteData(0xcc);	 //跳过ROM操作命令
	Ds18b20WriteData(0xbe);	 //发送读取温度命令
}

/*
 * 函数名称：int Ds18b20ReadTemp(void)
 * 函数功能：读取温度
 * 函数形参：无
 * 返  回  值：
 * 			温度值
 * */
int Ds18b20ReadTemp(void)
{
	int temp = 0;
	uint8 tmh, tml;
	Ds18b20ChangTemp();			 	//先写入转换命令
	Ds18b20ReadTempCom();			//然后等待转换完后发送读取温度命令
	tml = Ds18b20ReadData();		//读取温度值共16位，先读低字节
	tmh = Ds18b20ReadData();		//再读高字节
	temp = tmh;
	temp <<= 8;
	temp |= tml;
	return temp;
}







