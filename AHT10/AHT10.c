#include "AHT10.h"
#include "IIC.h"
#include "Delay.h"


//��λ
void AHT10_RST()     
{
	IIC_start();
	IIC_write_byte(AHT10_Wr);
	IIC_Get_ack();				//����Ӧ��
	IIC_write_byte(0xba);	//������λָ��
	IIC_Get_ack();				//����Ӧ��
	IIC_stop(); 
}
//��������
void AHT10_Mea()		
{
	IIC_start();
	IIC_write_byte(AHT10_Wr);
	IIC_Get_ack();				//����Ӧ��
	IIC_write_byte(0xac);	//��������ָ��
	IIC_Get_ack();				//����Ӧ��
	IIC_write_byte(0x33);	//ָ�����
	IIC_Get_ack();				//����Ӧ��
	IIC_write_byte(0x00);
	IIC_Get_ack();				//����Ӧ��
	IIC_stop(); 
}
//��ȡAHT10״̬��
unsigned char AHT10_Status()         
{
	unsigned char byte_first;
	IIC_start();
	IIC_write_byte(AHT10_Rd);
	IIC_Get_ack();				//����Ӧ��
	byte_first = IIC_read_byte();	//��������
	IIC_NACK();										//��Ӧ�𣬲��ٽ��պ�������
	IIC_stop();
	return byte_first;
}
//�ж�AHT10У׼ʹ��	
unsigned char AHT10_CalEN(){
	unsigned char val = 0;
	val = AHT10_Status();
	if((val & 0x08) == 0x08)		//bit[3]ΪУ׼λ��1λ��У׼��0λδУ׼
		return 1;
	else 
		return 0;
}
//��ʼ��
void AHT10Init(){
	IIC_start();
	IIC_write_byte(AHT10_Wr);
	IIC_Get_ack();				//����Ӧ��
  IIC_write_byte(0xe1);	//��ʼ��ָ��
	IIC_Get_ack();				//����Ӧ��
  IIC_write_byte(0x08);	//��ʼ������
	IIC_Get_ack();				//����Ӧ��
  IIC_write_byte(0x00);
	IIC_Get_ack();				//����Ӧ��
	IIC_stop();
}
//��ʼ�����ж��Ƿ�У׼
void AHT10_Init()       
{
	Delay_ms(45);	//�ϵ�ʱ�ȴ�45����
	AHT10Init();		//���Գ�ʼ��
	while(AHT10_CalEN() == 0)//���δУ׼����λ���ٴ˳��Գ�ʼ����ֱ��У׼���
	{	//ע�⣬�����ʼ��δ��ɻ���ѭ������ʱ��������Ƿ��������򴫸����Ƿ���
		AHT10_RST();		//��λ
		Delay_ms(15);	//��λʱ�䲻����20ms
		AHT10Init();
	}
}
//��ȡ���ݲ�������
void AHT10_Read_Data(unsigned int *T,unsigned int *S) //ʪ�Ⱥ��¶�����
{																										//�����¶�2251,Ϊ22.51���϶ȣ�ʪ��30211��Ϊ30.211%
	unsigned char Data[6] = {0};//���������ֽ�����,��һ��״̬�֣������������ֽ�Ϊʪ�����ݣ����Ϊ�¶�����
	unsigned long retudata = 0;	//ʹ��һ��long int��������������
	unsigned char n = 0;				//��æ��������ֹһֱæµ�����ѭ��
	AHT10_Mea();			//��ʼ����
	Delay_ms(80);			//��ʱ�ȴ�����75����	
	while(((AHT10_Status() & 0x80) == 0x80))	//��æ�����æµ��ȴ�������300ms���˳�
	{
		Delay_ms(300);
		n++;
		if(n >= 100)break;
	}
  IIC_start();			
	IIC_write_byte(AHT10_Rd);				//д�������ָ��
	IIC_Get_ack();				//����Ӧ��
	for(n = 0;n < 6;n++){
		Data[n] = IIC_read_byte();		//״̬����
		if(n < 5){
			IIC_ACK();		//Ӧ��
		}	
		else{
			IIC_NACK();		//����Σ���Ӧ�𣬶�ȡ���
		}		
	}
	IIC_stop();	
	retudata = 0;
	retudata += Data[1];				//ԭʼʪ�����ݺϳ�
	retudata = (retudata<<8) + Data[2];
	retudata = ((retudata<<8) + Data[3])>>4;
	//retudata = retudata & 0x000fffff;
	//	*S = (retudata*1000.0/1024.0/1024.0)*1000;//���ݾ�ȷ����λС��
	*S = (retudata*1000.0/1024.0/1024.0)*10.0;
	retudata = 0;				//ԭʼ�¶����ݺϳ�
	retudata = ((Data[3] & 0x0f)<<8)|Data[4];
	retudata = (retudata<<8)|Data[5];
	//retudata = retudata & 0x000fffff;
	*T = (retudata*100.0*200.0/1024.0/1024.0 - 50.0*100);	//���ݾ�ȷ����λС��	
	
}

