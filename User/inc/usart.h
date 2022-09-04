#ifndef USART_H
#define USART_H
#include "stm32f0xx.h"

/*****************************************************/
//Значение скорости передачи USART
/*****************************************************/
#define APBCLK   48000000UL
#define BAUDRATE 115200UL


void usart_installation(void);

#endif
