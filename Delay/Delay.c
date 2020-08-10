#include "Delay.h"

/*
 *@brief 		1微秒延时基础函数
 *				STC-ISP软件生成
 *@param[in]	NULL
 *@author		MrWeng
 *@date			2020-8-7
 */
void Delay1us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 1;
	while (--i);
}

/*
 *@brief 		微秒延时函数
 *				
 *@param[in]	us：需要延时的微秒数
 *@author		MrWeng
 *@date			2020-8-7
 */
void Delay_us(unsigned short int us)
{
	unsigned short int i;
	for(i=0;i<us;i++)
		Delay1us();
}

/*
 *@brief 		1毫秒延时基础函数
 *				STC-ISP软件生成
 *@param[in]	NULL
 *@author		MrWeng
 *@date			2020-8-7
 */
void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	i = 15;
	j = 90;
	do
	{
		while (--j);
	} while (--i);
}

/*
 *@brief 		毫秒延时函数
 *				
 *@param[in]	ms：需要延时的毫秒数
 *@author		MrWeng
 *@date			2020-8-7
 */
void Delay_ms(unsigned short int ms)
{
	unsigned short int i;
	for(i=0;i<ms;i++)
		Delay1ms();
}
/*==========================================================*/



