#include <intrins.h>
#include <STC8.h>
#include "Uart.h"
#include "Delay.h"
#include "AHT10.h"
#include "IIC.h"
unsigned char TH_Buffer[2][16] = {{"Temp:  .   C    "},{"Humi:  .    %   "}};

void main(void)
{
	unsigned int Temp = 0,Humi = 0;	//�¶Ⱥ�ʪ��
	UartInit();
	IIC_Init();
	ES = 1;
	EA = 1;
	UartSendStr("Uart Test !\r\n");
	
#if HW_IIC
	
	P_SW2 = 0x80;
	I2CCFG = 0xe0;                              //ʹ��I2C����ģʽ
	I2CMSST = 0x00;
#endif
	AHT10_Init();	//AHT10��ʼ��
	while(1)
	{
		AHT10_Read_Data(&Temp,&Humi);
		Delay_ms(1000);		//���ٲ������ܻ����𴫸������£��˴����Ͳ�������
		
		TH_Buffer[0][5] = Temp/1000 + 0x30;
		TH_Buffer[0][6] = (Temp%1000)/100 + 0x30;	
		TH_Buffer[0][8] = (Temp%100)/10 + 0x30;
		TH_Buffer[0][9] = Temp%10 + 0x30;	

		
		TH_Buffer[1][5] = Humi/1000 + 0x30;	
		TH_Buffer[1][6] = (Humi%1000)/100 + 0x30;
		TH_Buffer[1][8] = (Humi%100)/10 + 0x30;
		TH_Buffer[1][9] = Humi%10 + 0x30;
	
		UartSendStr(TH_Buffer);
		Delay_ms(1500);
	}

}
