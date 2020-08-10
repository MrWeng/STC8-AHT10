#include "IIC.h"


/*
 *@brief 		硬件IIC等待函数
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
 *@brief 		IIC总线初始化函数
 *@param[in]	NULL
 *@author		MrWeng
 *@date			2020-8-10
 */
void IIC_Init(void)	
{
    IIC_SDA = 1;//释放IIC总线的数据线。
    IIC_SCL = 1;//释放IIC总线的时钟线。

}
/****************************************************************
通讯起始信号：当 SCL 线是高电平时 SDA 线从高电平向低电平切换，表示通讯的开始；
*****************************************************************/
void IIC_start(void)  
{
#if HW_IIC
	I2CMSCR = 0x01;       //发送START命令
    Wait();
#else
   IIC_SDA = 1;//拉高
   IIC_SCL = 1;
   IIC_SDA = 0;
   IIC_SCL = 0;		//拉低时钟线，准备开始时钟
#endif
}
/****************************************************************
通讯停止信号：当 SCL 是高电平时 SDA线由低电平向高电平切换，表示通讯的停止。
*****************************************************************/
void IIC_stop(void) 
{
#if HW_IIC
	I2CMSCR = 0x06;                             //发送STOP命令
	Wait();
#else
	IIC_SCL = 1;
	IIC_SDA = 0;
	IIC_SDA = 1;	//通讯停止
	IIC_SCL = 0;
#endif	
}
/****************************************************************
//接收应答信号函数
*****************************************************************/
void IIC_Get_ack(void)
{
#if HW_IIC
	I2CMSCR = 0x03;                             //发送读ACK命令
	Wait();
#else	
	IIC_SCL = 1;		//拉高时钟线。
	IIC_SCL = 0;		//拉低时钟线。
#endif
}
/****************************************************************
//发送应答信号函数
*****************************************************************/
//应答
void IIC_ACK()	
{ 
#if HW_IIC
	I2CMSST = 0x00;                             //设置ACK信号
	I2CMSCR = 0x05;                             //发送ACK命令
	Wait();
#else
	IIC_SDA = 0;	//拉低数据线，应答
	IIC_SCL = 1;	//产生第九个时钟信号。
	IIC_SCL = 0;	
#endif
}
//非应答
void IIC_NACK()
{ 
#if HW_IIC
	I2CMSST = 0x01;                             //设置NAK信号
	I2CMSCR = 0x05;                             //发送ACK命令
	Wait();
#else
	IIC_SDA = 1;	//拉高数据线，非应答
	IIC_SCL = 1;	//产生第九个时钟信号。
	IIC_SCL = 0;	
#endif
}

/****************************************************************
//向IIC总线写入一个字节的数据
*****************************************************************/
void IIC_write_byte(unsigned char Data)
{
#if HW_IIC
	I2CTXD = Data;                               //写数据到数据缓冲区
	I2CMSCR = 0x02;                             //发送SEND命令
	Wait();
#else
	unsigned char i;
	for(i=0;i<8;i++)//八位数据
	{
		if((Data & 0x80) == 0x80)
			IIC_SDA = 1;
		else
			IIC_SDA = 0;
			IIC_SCL = 1; //一个时钟信号送入数据
			IIC_SCL = 0;
			Data = Data << 1;//数据左移一位，把次高位放在最高位,为写入次高位做准备
	}
	IIC_SDA = 0;		//应答处理前拉低，跳过应答
#endif
}
/****************************************************************
//从IIC总线读取一个字节的数据函数
*****************************************************************/
unsigned char IIC_read_byte()
{
#if HW_IIC
	I2CMSCR = 0x04;                             //发送RECV命令
    Wait();
    return I2CRXD;
#else	
    unsigned char i;
    unsigned char Data = 0;       //定义一个缓冲寄存器。
	IIC_SCL = 0;	//先拉低时钟线
	IIC_SDA = 1;	//再拉高数据线
    for(i = 0;i < 8;i++)
    {
			 Data = Data<<1;		//将缓冲字节的数据左移一位，准备读取数据。
       IIC_SCL = 1;				//拉高时钟线，开始读取下一位数据
       if(IIC_SDA == 1)		//如果数据线为高平电平。
				Data = Data|0x01;	//则给缓冲字节的最低位写1。
       IIC_SCL = 0;				//拉低时钟线，一位读取完成
    }
    return Data;					//返回读取的一个字节数据。
#endif
}
/*************************************************************************************************
void IIC_single_byte_write(unsigned char Daddr,unsigned char Waddr,unsigned char Data)
{//向任意地址写入一个字节数据函数
    IIC_start();//产生起始信号
    IIC_write_byte(Daddr);//写入设备地址（写）
    IIC_Tack();//等待设备的应答
    IIC_write_byte(Waddr);//写入要操作的单元地址。
    IIC_Tack();//等待设备的应答。
    IIC_write_byte(Data);//写入数据。
    IIC_Tack();//等待设备的应答。由于每次只写入一个字节，不对应答做处理
    IIC_stop();//产生停止符号。
}
unsigned char IIC_single_byte_read(unsigned char Daddr,unsigned char Waddr)
{//从任意地址读取一个字节数据函数
    unsigned char Data;//定义一个缓冲寄存器。

    IIC_start();//产生起始信号
    IIC_write_byte(Daddr);//写入设备地址（写）
    IIC_Tack();//等待设备的应答
    IIC_write_byte(Waddr);//写入要操作的单元地址。
    IIC_Tack();//等待设备的应答。
    
    IIC_start();//产生起始信号
    IIC_write_byte(Daddr+1);//写入设备地址（读）。
    IIC_Tack();//等待设备的应答。 
    Data=IIC_read_byte();//写入数据。
    IIC_stop();//产生停止符号。
    //-------------------返回读取的数据--------------------
    return Data;//返回读取的一个字节数据。
}
*************************************************************************************************/