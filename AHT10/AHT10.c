#include "AHT10.h"
#include "IIC.h"
#include "Delay.h"


//软复位
void AHT10_RST()     
{
	IIC_start();
	IIC_write_byte(AHT10_Wr);
	IIC_Get_ack();				//接收应答
	IIC_write_byte(0xba);	//发送软复位指令
	IIC_Get_ack();				//接收应答
	IIC_stop(); 
}
//触发测量
void AHT10_Mea()		
{
	IIC_start();
	IIC_write_byte(AHT10_Wr);
	IIC_Get_ack();				//接收应答
	IIC_write_byte(0xac);	//触发测量指令
	IIC_Get_ack();				//接收应答
	IIC_write_byte(0x33);	//指令参数
	IIC_Get_ack();				//接收应答
	IIC_write_byte(0x00);
	IIC_Get_ack();				//接收应答
	IIC_stop(); 
}
//读取AHT10状态字
unsigned char AHT10_Status()         
{
	unsigned char byte_first;
	IIC_start();
	IIC_write_byte(AHT10_Rd);
	IIC_Get_ack();				//接收应答
	byte_first = IIC_read_byte();	//接收数据
	IIC_NACK();										//非应答，不再接收后续数据
	IIC_stop();
	return byte_first;
}
//判断AHT10校准使能	
unsigned char AHT10_CalEN(){
	unsigned char val = 0;
	val = AHT10_Status();
	if((val & 0x08) == 0x08)		//bit[3]为校准位，1位已校准，0位未校准
		return 1;
	else 
		return 0;
}
//初始化
void AHT10Init(){
	IIC_start();
	IIC_write_byte(AHT10_Wr);
	IIC_Get_ack();				//接收应答
  IIC_write_byte(0xe1);	//初始化指令
	IIC_Get_ack();				//接收应答
  IIC_write_byte(0x08);	//初始化参数
	IIC_Get_ack();				//接收应答
  IIC_write_byte(0x00);
	IIC_Get_ack();				//接收应答
	IIC_stop();
}
//初始化并判断是否校准
void AHT10_Init()       
{
	Delay_ms(45);	//上电时等待45毫秒
	AHT10Init();		//尝试初始化
	while(AHT10_CalEN() == 0)//如果未校准，则复位，再此尝试初始化，直到校准完成
	{	//注意，如果初始化未完成会死循环，此时请检查接线是否存在问题或传感器是否损坏
		AHT10_RST();		//复位
		Delay_ms(15);	//复位时间不超过20ms
		AHT10Init();
	}
}
//读取数据并处理返回
void AHT10_Read_Data(unsigned int *T,unsigned int *S) //湿度和温度数据
{																										//例如温度2251,为22.51摄氏度，湿度30211，为30.211%
	unsigned char Data[6] = {0};//读入六个字节数据,第一个状态字，后面两个半字节为湿度数据，最后为温度数据
	unsigned long retudata = 0;	//使用一个long int类型来处理数据
	unsigned char n = 0;				//读忙计数，防止一直忙碌造成死循环
	AHT10_Mea();			//开始测量
	Delay_ms(80);			//延时等待至少75毫秒	
	while(((AHT10_Status() & 0x80) == 0x80))	//读忙，如果忙碌则等待，超过300ms则退出
	{
		Delay_ms(300);
		n++;
		if(n >= 100)break;
	}
  IIC_start();			
	IIC_write_byte(AHT10_Rd);				//写入读数据指令
	IIC_Get_ack();				//接收应答
	for(n = 0;n < 6;n++){
		Data[n] = IIC_read_byte();		//状态数据
		if(n < 5){
			IIC_ACK();		//应答
		}	
		else{
			IIC_NACK();		//第五次，非应答，读取完毕
		}		
	}
	IIC_stop();	
	retudata = 0;
	retudata += Data[1];				//原始湿度数据合成
	retudata = (retudata<<8) + Data[2];
	retudata = ((retudata<<8) + Data[3])>>4;
	//retudata = retudata & 0x000fffff;
	//	*S = (retudata*1000.0/1024.0/1024.0)*1000;//数据精确到三位小数
	*S = (retudata*1000.0/1024.0/1024.0)*10.0;
	retudata = 0;				//原始温度数据合成
	retudata = ((Data[3] & 0x0f)<<8)|Data[4];
	retudata = (retudata<<8)|Data[5];
	//retudata = retudata & 0x000fffff;
	*T = (retudata*100.0*200.0/1024.0/1024.0 - 50.0*100);	//数据精确到两位小数	
	
}

