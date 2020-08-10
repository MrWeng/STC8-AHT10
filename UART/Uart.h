#ifndef	_UART_H
#define	_UART_H

#include <intrins.h>
#include <STC8.h>

#define FOSC            11059200UL
#define BRT             (65536 - FOSC / 115200 / 4)

extern void UartInit();
extern void UartSend(char dat);
extern void UartSendStr(char *p);
#endif