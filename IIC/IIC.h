#ifndef __IIC_H_
#define __IIC_H_

#include "stc8.h"

//*** <<< Use Configuration Wizard in Context Menu >>> ***
//硬件IIC和软件IIC选择
// <o> IIC selection <0=> Software I2C <1=> Hardware I2C
#define IIC_selection (0)

#define SW_IIC   	(IIC_selection==0)
#define HW_IIC  	(IIC_selection==1)

//*** <<< end of configuration section >>>    ***

#if HW_IIC
	sbit IIC_SDA = P1^4;      //数据线
	sbit IIC_SCL = P1^5;      //时钟线
#else
	sbit IIC_SDA = P3^4;      //数据线
	sbit IIC_SCL = P3^5;      //时钟线
#endif

//void delay_IIC(void);			//信号保持延时
extern void IIC_Init(void);			//总线初始化
extern void IIC_start(void);			//开始通信
extern void IIC_stop(void);			//结束通信
extern void IIC_Get_ack(void);		//接收应答信号，发送多个字节时使用
extern void IIC_ACK();	
extern void IIC_NACK();	
extern void IIC_write_byte(unsigned char Data);	//写入一个字节
extern unsigned char IIC_read_byte();						//读取一个字节
//void IIC_single_byte_write(unsigned char Daddr,unsigned char Waddr,unsigned char Data);	//向指定地址写入一个字节
//unsigned char IIC_single_byte_read(unsigned char Daddr,unsigned char Waddr);						//读取指定地址的数据

#endif