/*
 * dht11.c
 *
 *  Created on: 2019��1��15��
 *      Author: Marshal
 */

#include "driver/dht11.h"
#include "driver/delay.h"

// ȫ�ֱ���
//==================================================================================
// DHT11_Data_Array[0] == ʪ��_����_����
// DHT11_Data_Array[1] == ʪ��_С��_����
// DHT11_Data_Array[2] == �¶�_����_����
// DHT11_Data_Array[3] == �¶�_С��_����
// DHT11_Data_Array[4] == У���ֽ�
// DHT11_Data_Array[5] == ��1:�¶�>=0�桿��0:�¶�<0�桿
//-----------------------------------------------------
u8 DHT11_Data_Array[6] = {0};	// DHT11��������
u8 DHT11_Data_Char[2][10]={0};	// DHT11�����ַ������У�ʪ/�¶ȡ����У������ַ�����
// DHT11_Data_Char[0] == ��ʪ���ַ�����
// DHT11_Data_Char[1] == ���¶��ַ�����
//==================================================================================

/*
 *�������ƣ�void Dht11OutputConfig(bool val)
 *�������ܣ�����DHT11Ϊ���ģ��
 *�����βΣ�bool val �ߵ͵�ƽ
 *��  ��  ֵ����
 *Ӳ�����ӣ�GPIO5
 * */
void Dht11OutputConfig(bool val)
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,	FUNC_GPIO5);	// GPIO5��ΪIO��
	GPIO_OUTPUT_SET(GPIO_ID_PIN(5), val);//�������͸ߵ�ƽ
}

/*
 *�������ƣ�void Dht11InputConfig(void)
 *�������ܣ�����DHT11Ϊ����ģ��
 *�����βΣ���
 *��  ��  ֵ����
 *Ӳ�����ӣ�GPIO5
 * */
void Dht11InputConfig(void)
{
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U,	FUNC_GPIO5);	// GPIO5��ΪIO��
	GPIO_DIS_OUTPUT(GPIO_ID_PIN(5));
}

/*
 *�������ƣ�u8 Dht11Star(void)
 *�������ܣ��ж�DHT11ģ���ͷŴ���
 *�����βΣ���
 *��  ��  ֵ��
 *			0������  ��0��������
 *Ӳ�����ӣ�GPIO5
 * */
u8 Dht11Star(void)
{
	u8 UsFlag = 0;
	Dht11OutputConfig(1);//
	DelayMs(1);

	//��ʼ�ź� -- ������������������ T(18ms < T < 30ms)�� ֪ͨ������׼������
	Dht11OutputConfig(0);
	DelayMs(20);
	//�ͷ�����
	Dht11OutputConfig(1);
	DelayUs(5);

	//������Ӧ�ź�
	//����Ϊ����ģʽ�����ܽ����ź�
	Dht11InputConfig();
	//�ȴ�������Ӧ�ź�70US����
	while(GPIO_INPUT_GET(GPIO_ID_PIN(5)) == 1 && UsFlag < 70)
	{
		DelayUs(1);
		UsFlag ++;
	}
	if(UsFlag >= 70) return 1;//��Ӧ��ʱ
	UsFlag = 0;

	//����͵�ƽʱ��̫������Ϊģ��������
	while(GPIO_INPUT_GET(GPIO_ID_PIN(5)) == 0 && UsFlag < 200)
	{
		DelayUs(1);
		UsFlag ++;
	}
	if(UsFlag >= 200)
		return 1;//��Ӧ��ʱ
	else
		return 0;
}

/*
 *�������ƣ�u8 DHT11ReadBit(void)
 *�������ܣ�DHT11ģ���һλ����
 *�����βΣ���
 *��  ��  ֵ��
 *			0/1
 *Ӳ�����ӣ�GPIO5
 * */
u8 DHT11ReadBit(void)
{
	u8 UsFlag = 0;	// ��ʱ��ʱ

	// �ȴ���Ӧ��Ϣ�ĵ͵�ƽ����ٵ�150us��
	while( GPIO_INPUT_GET(GPIO_ID_PIN(5))==1 && UsFlag<150 )
	{
		DelayUs(1);		// 1us��ʱ
		UsFlag++;
	}
	UsFlag = 0 ;		// �͵�ƽ��ʱ��ʼ
	// ����λ�ĵ͵�ƽʱ����ʱ�����200us��
	while( GPIO_INPUT_GET(GPIO_ID_PIN(5))==0 && UsFlag<120 )
	{
		DelayUs(1);
		UsFlag++;	// �͵�ƽʱ��
	}
	// ����λ�ĵ͵�ƽ������������λ�ĸߵ�ƽ
	// ����"0"�ĸߵ�ƽʱ�� == [23��27us]
	// ����"1"�ĸߵ�ƽʱ�� == [68��74us]
	DelayUs(45);	// ��������"0"�ĸߵ�ƽ����

	// ��ʱ45us�󣬼���ź��ߵ�ƽ ��ʱ��ƽΪ�߾��������1 ��ƽΪ�;��������0
	return GPIO_INPUT_GET(GPIO_ID_PIN(5));
}

/*
 *�������ƣ�u8 DHT11ReadByte(void)
 *�������ܣ�DHT11ģ���һ���ֽ�����
 *�����βΣ���
 *��  ��  ֵ��
 *			����һ���ֽ�����
 *Ӳ�����ӣ�GPIO5
 * */
u8 DHT11ReadByte(void)
{
	u8 Bit = 0;	// λ����
	u8 T_DHT11_Byte_Data = 0;	// DHT11�ֽ�����
	for(Bit = 0; Bit<8; Bit++)		// ��ȡDHT11һ���ֽ�
	{
		T_DHT11_Byte_Data <<= 1;
		T_DHT11_Byte_Data |= DHT11ReadBit();	// һλһλ�Ķ�ȡ
	}
	return T_DHT11_Byte_Data;	// ���ض�ȡ�ֽ�
}

/*
 *�������ƣ�u8 DHT11ReadDataComplete(void)
 *�������ܣ�DHT11ģ���ȡ��������
 *�����βΣ���
 *��  ��  ֵ��
 *			0�����ݶ�ȡ�ɹ�
 *			��0�����ݶ�ȡʧ��
 *Ӳ�����ӣ�GPIO5
 * */
u8 DHT11ReadDataComplete(void)
{
	u8 UsFlag = 0;	// ��ʱ��ʱ
	// ����DHT11����_�ɹ�
	//------------------------------------------------------------------------
	if(Dht11Star() == 0)	// DHT11�������ʼ�źţ���������Ӧ�ź�
	{
		DHT11_Data_Array[0] = DHT11ReadByte();	// ʪ��_����_����
		DHT11_Data_Array[1] = DHT11ReadByte();	// ʪ��_С��_����
		DHT11_Data_Array[2] = DHT11ReadByte();	// �¶�_����_����
		DHT11_Data_Array[3] = DHT11ReadByte();	// �¶�_С��_����
		DHT11_Data_Array[4] = DHT11ReadByte();	// У���ֽ�

		// �����ʱ�����һλ���ݵĸߵ�ƽ����ȴ�������
		while(GPIO_INPUT_GET(GPIO_ID_PIN(5))==1 && UsFlag < 100)
		{
			DelayUs(1);		// 1us��ʱ
			UsFlag++;
		}
		UsFlag = 0 ;		// �͵�ƽ��ʱ��ʼ
		// �����źŵĵ͵�ƽʱ����ʱ
		while(GPIO_INPUT_GET(GPIO_ID_PIN(5))==0 &&  UsFlag < 100)
		{
			DelayUs(1);		// 1us��ʱ
			UsFlag++;
		}
		if(UsFlag >= 100)
			return 1;		// ����1����ʾ�������źŵĵ͵�ƽʱ����ʱ
		// ����У��
		if(	DHT11_Data_Array[4] ==
			DHT11_Data_Array[0] + DHT11_Data_Array[1] +
			DHT11_Data_Array[2] + DHT11_Data_Array[3] )
		{
			// ��ȡDHT11���ݽ�����ESP8266�ӹ�DHT11�ź���
			//-----------------------------------------------------------
			//os_delay_us(10);
			//DHT11_Signal_Output(1);	// DHT11�ź��������(ESP8266����)
			// �ж��¶��Ƿ�Ϊ 0������
			//----------------------------------------------
			if((DHT11_Data_Array[3]&0x80) == 0)
			{
				DHT11_Data_Array[5] = 1;		// >=0��
			}
			else
			{
				DHT11_Data_Array[5] = 0;		// <0��
				DHT11_Data_Array[3] &= 0x7F;	// �����¶�С������
			}
			return 0;	// ����0����ʾ����ʪ�ȶ�ȡ�ɹ�
		}
		else return 3;		// ����3����ʾ��У�����
	}
	else return 2;		// ����2����ʾ������DHT11���䣬ʧ��
}

/*
 *�������ƣ�void DHT11NUMChar(void)
 *�������ܣ���DHT11��ȡ������ȫ��תΪ�ַ���
 *�����βΣ���
 *��  ��  ֵ����
 * */
void DHT11NUMChar(void)
{
	u8 C_char = 0;		// �ַ�����
	// ʪ�������ַ���
	if(DHT11_Data_Array[0]/100)			// ʪ�������İ�λ
		DHT11_Data_Char[0][C_char++] = DHT11_Data_Array[0]/100 + 48;
	if((DHT11_Data_Array[0]%100)/10)	// ʪ��������ʮλ
		DHT11_Data_Char[0][C_char++] = (DHT11_Data_Array[0]%100)/10 + 48;

	// ʪ�������ĸ�λ
	DHT11_Data_Char[0][C_char++] = DHT11_Data_Array[0]%10 + 48;
	DHT11_Data_Char[0][C_char++] = '.';		// С����
	// ʪ��������С��
	DHT11_Data_Char[0][C_char++] = DHT11_Data_Array[1]%10 + 48;

	DHT11_Data_Char[0][C_char++] = ' ';		// ' '
	DHT11_Data_Char[0][C_char++] = '%';		// '%'
	DHT11_Data_Char[0][C_char++] = 'R';		// 'R'
	DHT11_Data_Char[0][C_char++] = 'H';		// 'H'
	DHT11_Data_Char[0][C_char] 	 =  0 ;		// ��0

	C_char = 0;		// ����

	// �¶������ַ���
	if(DHT11_Data_Array[5]==0)			// �¶� < 0��
		DHT11_Data_Char[1][C_char++] = '-';
	if(DHT11_Data_Array[2]/100)			// ʪ�������İ�λ
		DHT11_Data_Char[1][C_char++] = DHT11_Data_Array[2]/100 + 48;
	if((DHT11_Data_Array[2]%100)/10)	// ʪ��������ʮλ
		DHT11_Data_Char[1][C_char++] = (DHT11_Data_Array[2]%100)/10 + 48;

	// ʪ�������ĸ�λ
	DHT11_Data_Char[1][C_char++] = DHT11_Data_Array[2]%10 + 48;
	DHT11_Data_Char[1][C_char++] = '.';		// С����
	// ʪ��������С��
	DHT11_Data_Char[1][C_char++] = DHT11_Data_Array[3]%10 + 48;

	DHT11_Data_Char[1][C_char++] = ' ';		// ' '
	DHT11_Data_Char[1][C_char++] = 'C';		// 'C'
	DHT11_Data_Char[1][C_char]   =  0 ;		// ��0
}



