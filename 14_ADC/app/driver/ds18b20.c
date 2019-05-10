/*
 * ds18b20.c
 *
 *  Created on: 2019��1��13��
 *      Author: С����
 */
#include "driver/ds18b20.h"
#include "c_types.h"
#include "driver/delay.h"
/*
 *�������ƣ�u8 Ds18b20InitConfig(void)
 *�������ܣ�ds18b20ģ���ʼ��
 *�����βΣ���
 *��  ��  ֵ��
 *			0��da18b20����������
 *			����ֵ��ds18b20�쳣
 * */
u8 Ds18b20InitConfig(void)
{
	u16 cnt = 0;
	//1.����LED������Ӧ��GPIO4��ΪIO��ģʽ -- ����ĵ�3.3.1.1�½�
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTCK_U,	FUNC_GPIO13);	// GPIO13��ΪIO��
	//2.�������ȷ���һ�� 480-960 ΢��ĵ͵�ƽ����
	GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0);//�������͵͵�ƽ
	DelayUs(700);//�͵�ƽ����ʱ��700US
	//3.Ȼ���ͷ����߱�Ϊ�ߵ�ƽ
	GPIO_OUTPUT_SET(GPIO_ID_PIN(13),1);//�������͸ߵ�ƽ
	//3.1 ������ⲻ��GPIO13�ĵ͵�ƽ����Ϊû��DS18B20ģ��
	while((GPIO_INPUT_GET(GPIO_ID_PIN(13)) == 1) && (cnt < 200))
	{
		DelayUs(1);
		cnt ++;
	}
	if(cnt >= 200) return 1;

	cnt = 0;

	//3.2 ������⵽GPIO13�ĵ͵�ƽ�����ǵ͵�ƽ����ʱ��̫��������ΪDS18B20ģ���쳣
	while((GPIO_INPUT_GET(GPIO_ID_PIN(13)) == 0) && (cnt < 480))
	{
		DelayUs(1);
		cnt ++;
	}
	if(cnt >= 480) return 1;

	return 0;//�ɹ�����0
}

/*
 * �������ƣ�void Ds18b20WriteData(u8 data)
 * �������ܣ�DS18B20дһ���ֽ�����
 * �����βΣ�
 * 			u8 data д�������
 * ��  ��  ֵ����
 * */
void Ds18b20WriteData(u8 data)
{
	u8 i,j;
	for(i = 0; i < 8; i ++)
	{
		//1.д����һ��ʼ��Ϊ�����Ȱ��������� 1 ΢���ʾд���ڿ�ʼ
		GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0);//�������͵͵�ƽ
		j ++;//һ�д���1΢��
		//2.�ӵ�λ��ʼд������
		GPIO_OUTPUT_SET(GPIO_ID_PIN(13), data & 0x01);
		DelayUs(80);//д��������Ϊ 60 ΢�룬������� 120 ΢��
		//�ͷ�����Ϊ�ߵ�ƽ
		GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 1);
		data = data >> 1;
	}
}

/*
 * �������ƣ�uint8 Ds18b20ReadByte(void)
 * �������ܣ�DS18B20��һ���ֽ�����
 * �����βΣ���
 * ��  ��  ֵ��
 * 			byte����ȡ��������
 * */
uint8 Ds18b20ReadData(void)
{
	uint8 byte, data;
	int i, j;
	for (j = 8; j > 0; j--)
	{
		GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 0);		//�Ƚ���������1us
		i++;
		GPIO_OUTPUT_SET(GPIO_ID_PIN(13), 1);	  	//Ȼ���ͷ�����
		i++;
		i++;	  //��ʱ6us�ȴ������ȶ�
		data = GPIO_INPUT_GET(GPIO_ID_PIN(13));	 	//��ȡ���ݣ������λ��ʼ��ȡ
		/*��byte����һλ��Ȼ����������7λ���bi��ע���ƶ�֮���Ƶ���λ��0��*/
		byte = (byte >> 1) | (data << 7);
		os_delay_us(48); //��ȡ��֮��ȴ�48us�ٽ��Ŷ�ȡ��һ����
	}
	return byte;
}

/*
 * �������ƣ�void Ds18b20ChangTemp(void)
 * �������ܣ���18b20��ʼת���¶�
 * �����βΣ���
 * ��  ��  ֵ����
 * */
void Ds18b20ChangTemp(void)
{
	Ds18b20Init();
	DelayMs(1);
	Ds18b20WriteData(0xcc);		//����ROM��������
	Ds18b20WriteData(0x44);	    //�¶�ת������
//	Delay1ms(100);	//�ȴ�ת���ɹ������������һֱˢ�ŵĻ����Ͳ��������ʱ��

}

/*
 * �������ƣ�void Ds18b20ReadTempCom(void)
 * �������ܣ����Ͷ�ȡ�¶�����
 * �����βΣ���
 * ��  ��  ֵ����
 * */
void Ds18b20ReadTempCom(void)
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteData(0xcc);	 //����ROM��������
	Ds18b20WriteData(0xbe);	 //���Ͷ�ȡ�¶�����
}

/*
 * �������ƣ�int Ds18b20ReadTemp(void)
 * �������ܣ���ȡ�¶�
 * �����βΣ���
 * ��  ��  ֵ��
 * 			�¶�ֵ
 * */
int Ds18b20ReadTemp(void)
{
	int temp = 0;
	uint8 tmh, tml;
	Ds18b20ChangTemp();			 	//��д��ת������
	Ds18b20ReadTempCom();			//Ȼ��ȴ�ת������Ͷ�ȡ�¶�����
	tml = Ds18b20ReadData();		//��ȡ�¶�ֵ��16λ���ȶ����ֽ�
	tmh = Ds18b20ReadData();		//�ٶ����ֽ�
	temp = tmh;
	temp <<= 8;
	temp |= tml;
	return temp;
}







