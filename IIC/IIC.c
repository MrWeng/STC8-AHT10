#include "IIC.h"


/*
 *@brief 		Ӳ��IIC�ȴ�����
 *@param[in]	NULL
 *@author		MrWeng
 *@date			2020-8-10
 */
#if HW_IIC
void Wait(void)
{
    while (!(I2CMSST & 0x40));
    I2CMSST &= ~0x40;
}
#endif

/*
 *@brief 		IIC���߳�ʼ������
 *@param[in]	NULL
 *@author		MrWeng
 *@date			2020-8-10
 */
void IIC_Init(void)	
{
    IIC_SDA = 1;//�ͷ�IIC���ߵ������ߡ�
    IIC_SCL = 1;//�ͷ�IIC���ߵ�ʱ���ߡ�

}
/****************************************************************
ͨѶ��ʼ�źţ��� SCL ���Ǹߵ�ƽʱ SDA �ߴӸߵ�ƽ��͵�ƽ�л�����ʾͨѶ�Ŀ�ʼ��
*****************************************************************/
void IIC_start(void)  
{
#if HW_IIC
	I2CMSCR = 0x01;       //����START����
    Wait();
#else
   IIC_SDA = 1;//����
   IIC_SCL = 1;
   IIC_SDA = 0;
   IIC_SCL = 0;		//����ʱ���ߣ�׼����ʼʱ��
#endif
}
/****************************************************************
ͨѶֹͣ�źţ��� SCL �Ǹߵ�ƽʱ SDA���ɵ͵�ƽ��ߵ�ƽ�л�����ʾͨѶ��ֹͣ��
*****************************************************************/
void IIC_stop(void) 
{
#if HW_IIC
	I2CMSCR = 0x06;                             //����STOP����
	Wait();
#else
	IIC_SCL = 1;
	IIC_SDA = 0;
	IIC_SDA = 1;	//ͨѶֹͣ
	IIC_SCL = 0;
#endif	
}
/****************************************************************
//����Ӧ���źź���
*****************************************************************/
void IIC_Get_ack(void)
{
#if HW_IIC
	I2CMSCR = 0x03;                             //���Ͷ�ACK����
	Wait();
#else	
	IIC_SCL = 1;		//����ʱ���ߡ�
	IIC_SCL = 0;		//����ʱ���ߡ�
#endif
}
/****************************************************************
//����Ӧ���źź���
*****************************************************************/
//Ӧ��
void IIC_ACK()	
{ 
#if HW_IIC
	I2CMSST = 0x00;                             //����ACK�ź�
	I2CMSCR = 0x05;                             //����ACK����
	Wait();
#else
	IIC_SDA = 0;	//���������ߣ�Ӧ��
	IIC_SCL = 1;	//�����ھŸ�ʱ���źš�
	IIC_SCL = 0;	
#endif
}
//��Ӧ��
void IIC_NACK()
{ 
#if HW_IIC
	I2CMSST = 0x01;                             //����NAK�ź�
	I2CMSCR = 0x05;                             //����ACK����
	Wait();
#else
	IIC_SDA = 1;	//���������ߣ���Ӧ��
	IIC_SCL = 1;	//�����ھŸ�ʱ���źš�
	IIC_SCL = 0;	
#endif
}

/****************************************************************
//��IIC����д��һ���ֽڵ�����
*****************************************************************/
void IIC_write_byte(unsigned char Data)
{
#if HW_IIC
	I2CTXD = Data;                               //д���ݵ����ݻ�����
	I2CMSCR = 0x02;                             //����SEND����
	Wait();
#else
	unsigned char i;
	for(i=0;i<8;i++)//��λ����
	{
		if((Data & 0x80) == 0x80)
			IIC_SDA = 1;
		else
			IIC_SDA = 0;
			IIC_SCL = 1; //һ��ʱ���ź���������
			IIC_SCL = 0;
			Data = Data << 1;//��������һλ���Ѵθ�λ�������λ,Ϊд��θ�λ��׼��
	}
	IIC_SDA = 0;		//Ӧ����ǰ���ͣ�����Ӧ��
#endif
}
/****************************************************************
//��IIC���߶�ȡһ���ֽڵ����ݺ���
*****************************************************************/
unsigned char IIC_read_byte()
{
#if HW_IIC
	I2CMSCR = 0x04;                             //����RECV����
    Wait();
    return I2CRXD;
#else	
    unsigned char i;
    unsigned char Data = 0;       //����һ������Ĵ�����
	IIC_SCL = 0;	//������ʱ����
	IIC_SDA = 1;	//������������
    for(i = 0;i < 8;i++)
    {
			 Data = Data<<1;		//�������ֽڵ���������һλ��׼����ȡ���ݡ�
       IIC_SCL = 1;				//����ʱ���ߣ���ʼ��ȡ��һλ����
       if(IIC_SDA == 1)		//���������Ϊ��ƽ��ƽ��
				Data = Data|0x01;	//��������ֽڵ����λд1��
       IIC_SCL = 0;				//����ʱ���ߣ�һλ��ȡ���
    }
    return Data;					//���ض�ȡ��һ���ֽ����ݡ�
#endif
}
/*************************************************************************************************
void IIC_single_byte_write(unsigned char Daddr,unsigned char Waddr,unsigned char Data)
{//�������ַд��һ���ֽ����ݺ���
    IIC_start();//������ʼ�ź�
    IIC_write_byte(Daddr);//д���豸��ַ��д��
    IIC_Tack();//�ȴ��豸��Ӧ��
    IIC_write_byte(Waddr);//д��Ҫ�����ĵ�Ԫ��ַ��
    IIC_Tack();//�ȴ��豸��Ӧ��
    IIC_write_byte(Data);//д�����ݡ�
    IIC_Tack();//�ȴ��豸��Ӧ������ÿ��ֻд��һ���ֽڣ�����Ӧ��������
    IIC_stop();//����ֹͣ���š�
}
unsigned char IIC_single_byte_read(unsigned char Daddr,unsigned char Waddr)
{//�������ַ��ȡһ���ֽ����ݺ���
    unsigned char Data;//����һ������Ĵ�����

    IIC_start();//������ʼ�ź�
    IIC_write_byte(Daddr);//д���豸��ַ��д��
    IIC_Tack();//�ȴ��豸��Ӧ��
    IIC_write_byte(Waddr);//д��Ҫ�����ĵ�Ԫ��ַ��
    IIC_Tack();//�ȴ��豸��Ӧ��
    
    IIC_start();//������ʼ�ź�
    IIC_write_byte(Daddr+1);//д���豸��ַ��������
    IIC_Tack();//�ȴ��豸��Ӧ�� 
    Data=IIC_read_byte();//д�����ݡ�
    IIC_stop();//����ֹͣ���š�
    //-------------------���ض�ȡ������--------------------
    return Data;//���ض�ȡ��һ���ֽ����ݡ�
}
*************************************************************************************************/