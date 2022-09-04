#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "stm32f0xx.h"


void GPIO_keyboard_installation(void);
void GPIO_button(void);

#define  FIX_time   100
#define  REPEAT_time 30000
#define  DELAY_time  300000

#endif
