#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "stm32f0xx.h"

/*****************************************************/
//
/*****************************************************/
#define button_up       (1<<0)    /* кнопка вверх */
#define button_down     (1<<1)    /* кнопка вниз */
#define button_right    (1<<2)    /* кнопка вправо */
#define button_left     (1<<3)    /* кнопка влево */
#define button_ok       (1<<4)    /* кнопка ОК */
#define button_esc      (1<<5)    /* кнопка ESC */
/*****************************************************/
//Структура для хранения старого состояния кнопок
/*****************************************************/
struct button_old{
uint8_t up;                       /* состояние кнопки вверх */
uint8_t down;                     /* состояние кнопки вниз */
uint8_t right;	                  /* состояние кнопки вправо */
uint8_t left;                     /* состояние кнопки влево */
uint8_t ok;                       /* состояние кнопки ОК */	
uint8_t esc;                      /* состояние кнопки ESC */	
};

void keyboard_installation(struct button_old *p_but);
void button(uint8_t but, uint8_t *p_but_old);

#endif
