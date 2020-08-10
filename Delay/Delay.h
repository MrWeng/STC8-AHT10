#ifndef	_DELAY_H
#define	_DELAY_H

#include <STC8.h>
#include <intrins.h>
/*
 *@brief 		΢����ʱ����			
 *@param[in]	us����Ҫ��ʱ��΢����
 */
extern void Delay_us(unsigned short int us);

/*
 *@brief 		������ʱ����
 *@param[in]	ms����Ҫ��ʱ�ĺ�����
 */
extern void Delay_ms(unsigned short int ms);


#endif