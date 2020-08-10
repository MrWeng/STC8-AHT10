#ifndef AHT10_H_
#define AHT10_H_

#define AHT10_Wr 0x70	 //д��ַ
#define AHT10_Rd 0x71  //����ַ

void Delay_ms(unsigned int ms);
void AHT10Init();			 //��ʼ��
void AHT10_Init();		 //��ʼ��+У׼��//ʹ��ǰ������ô˺���
void AHT10_RST();    	 //��λ
void AHT10_Mea();			 //��������
unsigned char AHT10_Status();//��ȡAHT10״̬�Ĵ���
unsigned char AHT10_CalEN(); //�ж�AHT10У׼ʹ��
void AHT10_Read_Data(unsigned int *T,unsigned int *S);//��ȡ���ݴ�������


#endif