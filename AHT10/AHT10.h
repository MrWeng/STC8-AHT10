#ifndef AHT10_H_
#define AHT10_H_

#define AHT10_Wr 0x70	 //写地址
#define AHT10_Rd 0x71  //读地址

void Delay_ms(unsigned int ms);
void AHT10Init();			 //初始化
void AHT10_Init();		 //初始化+校准；//使用前必须调用此函数
void AHT10_RST();    	 //软复位
void AHT10_Mea();			 //触发测量
unsigned char AHT10_Status();//读取AHT10状态寄存器
unsigned char AHT10_CalEN(); //判断AHT10校准使能
void AHT10_Read_Data(unsigned int *T,unsigned int *S);//读取数据处理并返回


#endif