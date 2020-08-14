#ifndef __IIC_H_
#define __IIC_H_

#include "stc8.h"

//*** <<< Use Configuration Wizard in Context Menu >>> ***
//Ӳ��IIC�����IICѡ��
// <o> IIC selection <0=> Software I2C <1=> Hardware I2C
#define IIC_selection (0)

#define SW_IIC   	(IIC_selection==0)
#define HW_IIC  	(IIC_selection==1)

//*** <<< end of configuration section >>>    ***

#if HW_IIC
	sbit IIC_SDA = P1^4;      //������
	sbit IIC_SCL = P1^5;      //ʱ����
#else
	sbit IIC_SDA = P3^4;      //������
	sbit IIC_SCL = P3^5;      //ʱ����
#endif

//void delay_IIC(void);			//�źű�����ʱ
extern void IIC_Init(void);			//���߳�ʼ��
extern void IIC_start(void);			//��ʼͨ��
extern void IIC_stop(void);			//����ͨ��
extern void IIC_Get_ack(void);		//����Ӧ���źţ����Ͷ���ֽ�ʱʹ��
extern void IIC_ACK();	
extern void IIC_NACK();	
extern void IIC_write_byte(unsigned char Data);	//д��һ���ֽ�
extern unsigned char IIC_read_byte();						//��ȡһ���ֽ�
//void IIC_single_byte_write(unsigned char Daddr,unsigned char Waddr,unsigned char Data);	//��ָ����ַд��һ���ֽ�
//unsigned char IIC_single_byte_read(unsigned char Daddr,unsigned char Waddr);						//��ȡָ����ַ������

#endif