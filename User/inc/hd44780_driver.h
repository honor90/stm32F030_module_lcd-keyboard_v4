#ifndef HD44780_DRIVER_H
#define HD44780_DRIVER_H
#include "stm32f0xx.h"

/*****************************************************/
//
/*****************************************************/
#define PORT GPIOA
#define E    GPIO_ODR_11
#define RS   GPIO_ODR_15
/*****************************************************/
//«адержки
/*****************************************************/
#define DELAY           4          /* задержка дл€ данных и команд */
#define DELAY_CIRCLE    900
/*****************************************************/
//
/*****************************************************/
#define COUNT_SEC       60
#define COUNT_MINUTE     1         /* период перезагрузки (в минутах) */
/*****************************************************/
//переменные дл€ осчета времени перезагрузки диспле€
/*****************************************************/
struct time{
uint32_t tic_delay;
uint32_t mili_tic;
uint32_t tic;
uint32_t sec;
uint8_t minute;	                 
};


extern struct time  *t;

void delay(uint32_t time);
void portInstallation(void);
void HD44780_installation(void);
void installation_rus_alphabet(void);
void send_data(uint8_t data);
void command(uint8_t data);

#endif
