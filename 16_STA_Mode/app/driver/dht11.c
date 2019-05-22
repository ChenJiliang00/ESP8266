/*
 * dht11.c
 *
 *  Created on: 2019年1月15日
 *      Author: Marshal
 */

#include "driver/dht11.h"
#include "driver/delay.h"

// 全局变量
//==================================================================================
// DHT11_Data_Array[0] == 湿度_整数_部分
// DHT11_Data_Array[1] == 湿度_小数_部分
// DHT11_Data_Array[2] == 温度_整数_部分
// DHT11_Data_Array[3] == 温度_小数_部分
// DHT11_Data_Array[4] == 校验字节
// DHT11_Data_Array[5] == 【1:温度>=0℃】【0:温度<0℃】
//-----------------------------------------------------
u8 DHT11_Data_Array[6] = {0};	// DHT11数据数组
u8 DHT11_Data_Char[2][10]={0};	// DHT11数据字符串【行：湿/温度】【列：数据字符串】
// DHT11_Data_Char[0] == 【湿度字符串】
// DHT11_Data_Char[1] == 【温度字符串】
//==================================================================================

/*
 *函数名称：void Dht11OutputConfig(bool val)
 *函数功能：设置DHT11为输出模块
 *函数形参：bool val 高低电平
 *返  回  值：无
 *硬件连接：GPIO5
 * */
void Dht11OutputConfig(bool val)
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,	FUNC_GPIO5);	// GPIO5设为IO口
	GPIO_OUTPUT_SET(GPIO_ID_PIN(5), val);//主机发送高电平
}

/*
 *函数名称：void Dht11InputConfig(void)
 *函数功能：设置DHT11为输入模块
 *函数形参：无
 *返  回  值：无
 *硬件连接：GPIO5
 * */
void Dht11InputConfig(void)
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,	FUNC_GPIO5);	// GPIO5设为IO口
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(5));
}

/*
 *函数名称：u8 Dht11Star(void)
 *函数功能：判断DHT11模块释放存在
 *函数形参：无
 *返  回  值：
 *			0：存在  非0：不存在
 *硬件连接：GPIO5
 * */
u8 Dht11Star(void)
{
	u8 UsFlag = 0;
	Dht11OutputConfig(1);//
	DelayMs(1);

	//起始信号 -- 主机把数据总线拉低 T(18ms < T < 30ms)， 通知传感器准备数据
	Dht11OutputConfig(0);
	DelayMs(20);
	//释放总线
	Dht11OutputConfig(1);
	DelayUs(5);

	//接受响应信号
	//设置为输入模式，才能接收信号
	Dht11InputConfig();
	//等待接收响应信号70US以上
	while(GPIO_INPUT_GET(GPIO_ID_PIN(5)) == 1 && UsFlag < 70)
	{
		DelayUs(1);
		UsFlag ++;
	}
	if(UsFlag >= 70) return 1;//响应超时
	UsFlag = 0;

	//假如低电平时间太长，认为模块有问题
	while(GPIO_INPUT_GET(GPIO_ID_PIN(5)) == 0 && UsFlag < 200)
	{
		DelayUs(1);
		UsFlag ++;
	}
	if(UsFlag >= 200)
		return 1;//响应超时
	else
		return 0;
}

/*
 *函数名称：u8 DHT11ReadBit(void)
 *函数功能：DHT11模块独一位数据
 *函数形参：无
 *返  回  值：
 *			0/1
 *硬件连接：GPIO5
 * */
u8 DHT11ReadBit(void)
{
	u8 UsFlag = 0;	// 延时计时

	// 等待响应信息的低电平【最迟等150us】
	while( GPIO_INPUT_GET(GPIO_ID_PIN(5))==1 && UsFlag<150 )
	{
		DelayUs(1);		// 1us计时
		UsFlag++;
	}
	UsFlag = 0 ;		// 低电平计时开始
	// 数据位的低电平时长计时【最多200us】
	while( GPIO_INPUT_GET(GPIO_ID_PIN(5))==0 && UsFlag<120 )
	{
		DelayUs(1);
		UsFlag++;	// 低电平时长
	}
	// 数据位的低电平结束后，是数据位的高电平
	// 数据"0"的高电平时长 == [23～27us]
	// 数据"1"的高电平时长 == [68～74us]
	DelayUs(45);	// 跳过数据"0"的高电平部分

	// 延时45us后，检测信号线电平 此时电平为高就输出数据1 电平为低就输出数据0
	return GPIO_INPUT_GET(GPIO_ID_PIN(5));
}

/*
 *函数名称：u8 DHT11ReadByte(void)
 *函数功能：DHT11模块读一个字节数据
 *函数形参：无
 *返  回  值：
 *			返回一个字节数据
 *硬件连接：GPIO5
 * */
u8 DHT11ReadByte(void)
{
	u8 Bit = 0;	// 位计数
	u8 T_DHT11_Byte_Data = 0;	// DHT11字节数据
	for(Bit = 0; Bit<8; Bit++)		// 读取DHT11一个字节
	{
		T_DHT11_Byte_Data <<= 1;
		T_DHT11_Byte_Data |= DHT11ReadBit();	// 一位一位的读取
	}
	return T_DHT11_Byte_Data;	// 返回读取字节
}

/*
 *函数名称：u8 DHT11ReadDataComplete(void)
 *函数功能：DHT11模块读取所有数据
 *函数形参：无
 *返  回  值：
 *			0：数据读取成功
 *			非0：数据读取失败
 *硬件连接：GPIO5
 * */
u8 DHT11ReadDataComplete(void)
{
	u8 UsFlag = 0;	// 延时计时
	// 启动DHT11传输_成功
	//------------------------------------------------------------------------
	if(Dht11Star() == 0)	// DHT11：输出起始信号－＞接收响应信号
	{
		DHT11_Data_Array[0] = DHT11ReadByte();	// 湿度_整数_部分
		DHT11_Data_Array[1] = DHT11ReadByte();	// 湿度_小数_部分
		DHT11_Data_Array[2] = DHT11ReadByte();	// 温度_整数_部分
		DHT11_Data_Array[3] = DHT11ReadByte();	// 温度_小数_部分
		DHT11_Data_Array[4] = DHT11ReadByte();	// 校验字节

		// 如果此时是最后一位数据的高电平，则等待它结束
		while(GPIO_INPUT_GET(GPIO_ID_PIN(5))==1 && UsFlag < 100)
		{
			DelayUs(1);		// 1us计时
			UsFlag++;
		}
		UsFlag = 0 ;		// 低电平计时开始
		// 结束信号的低电平时长计时
		while(GPIO_INPUT_GET(GPIO_ID_PIN(5))==0 &&  UsFlag < 100)
		{
			DelayUs(1);		// 1us计时
			UsFlag++;
		}
		if(UsFlag >= 100)
			return 1;		// 返回1，表示：结束信号的低电平时长超时
		// 数据校验
		if(	DHT11_Data_Array[4] ==
			DHT11_Data_Array[0] + DHT11_Data_Array[1] +
			DHT11_Data_Array[2] + DHT11_Data_Array[3] )
		{
			// 读取DHT11数据结束，ESP8266接管DHT11信号线
			//-----------------------------------------------------------
			//os_delay_us(10);
			//DHT11_Signal_Output(1);	// DHT11信号线输出高(ESP8266驱动)
			// 判断温度是否为 0℃以上
			//----------------------------------------------
			if((DHT11_Data_Array[3]&0x80) == 0)
			{
				DHT11_Data_Array[5] = 1;		// >=0℃
			}
			else
			{
				DHT11_Data_Array[5] = 0;		// <0℃
				DHT11_Data_Array[3] &= 0x7F;	// 更正温度小数部分
			}
			return 0;	// 返回0，表示：温湿度读取成功
		}
		else return 3;		// 返回3，表示：校验错误
	}
	else return 2;		// 返回2，表示：启动DHT11传输，失败
}

/*
 *函数名称：void DHT11NUMChar(void)
 *函数功能：将DHT11读取的数据全部转为字符串
 *函数形参：无
 *返  回  值：无
 * */
void DHT11NUMChar(void)
{
	u8 C_char = 0;		// 字符计数
	// 湿度数据字符串
	if(DHT11_Data_Array[0]/100)			// 湿度整数的百位
		DHT11_Data_Char[0][C_char++] = DHT11_Data_Array[0]/100 + 48;
	if((DHT11_Data_Array[0]%100)/10)	// 湿度整数的十位
		DHT11_Data_Char[0][C_char++] = (DHT11_Data_Array[0]%100)/10 + 48;

	// 湿度整数的个位
	DHT11_Data_Char[0][C_char++] = DHT11_Data_Array[0]%10 + 48;
	DHT11_Data_Char[0][C_char++] = '.';		// 小数点
	// 湿度整数的小数
	DHT11_Data_Char[0][C_char++] = DHT11_Data_Array[1]%10 + 48;

	DHT11_Data_Char[0][C_char++] = ' ';		// ' '
	DHT11_Data_Char[0][C_char++] = '%';		// '%'
	DHT11_Data_Char[0][C_char++] = 'R';		// 'R'
	DHT11_Data_Char[0][C_char++] = 'H';		// 'H'
	DHT11_Data_Char[0][C_char] 	 =  0 ;		// 添0

	C_char = 0;		// 重置

	// 温度数据字符串
	if(DHT11_Data_Array[5]==0)			// 温度 < 0℃
		DHT11_Data_Char[1][C_char++] = '-';
	if(DHT11_Data_Array[2]/100)			// 湿度整数的百位
		DHT11_Data_Char[1][C_char++] = DHT11_Data_Array[2]/100 + 48;
	if((DHT11_Data_Array[2]%100)/10)	// 湿度整数的十位
		DHT11_Data_Char[1][C_char++] = (DHT11_Data_Array[2]%100)/10 + 48;

	// 湿度整数的个位
	DHT11_Data_Char[1][C_char++] = DHT11_Data_Array[2]%10 + 48;
	DHT11_Data_Char[1][C_char++] = '.';		// 小数点
	// 湿度整数的小数
	DHT11_Data_Char[1][C_char++] = DHT11_Data_Array[3]%10 + 48;

	DHT11_Data_Char[1][C_char++] = ' ';		// ' '
	DHT11_Data_Char[1][C_char++] = 'C';		// 'C'
	DHT11_Data_Char[1][C_char]   =  0 ;		// 添0
}



