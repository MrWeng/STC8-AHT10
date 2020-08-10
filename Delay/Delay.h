#ifndef	_DELAY_H
#define	_DELAY_H

#include <STC8.h>
#include <intrins.h>
/*
 *@brief 		微秒延时函数			
 *@param[in]	us：需要延时的微秒数
 */
extern void Delay_us(unsigned short int us);

/*
 *@brief 		毫秒延时函数
 *@param[in]	ms：需要延时的毫秒数
 */
extern void Delay_ms(unsigned short int ms);


#endif