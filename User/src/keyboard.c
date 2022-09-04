#include "stm32f0xx.h"
#include "keyboard.h"


/*****************************************************/
//Функция инициализации выводов, к которым подключена клавиатура
/*****************************************************/
void keyboard_installation(struct button_old *p_but)
{
	//K1...K6
	GPIOA->MODER &=~ (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER3_0 | GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0);   
	GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1 | GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5);
	GPIOA->OTYPER &=~ GPIO_OTYPER_OT_0|GPIO_OTYPER_OT_1|GPIO_OTYPER_OT_2|GPIO_OTYPER_OT_3|GPIO_OTYPER_OT_4|GPIO_OTYPER_OT_5;
  GPIOA->PUPDR |= GPIO_PUPDR_PUPDR0_0|GPIO_PUPDR_PUPDR1_0|GPIO_PUPDR_PUPDR2_0|GPIO_PUPDR_PUPDR3_0|GPIO_PUPDR_PUPDR4_0|GPIO_PUPDR_PUPDR5_0;

p_but->ok = (uint8_t)(~(GPIOA->IDR & button_ok ));
p_but->down = (uint8_t)(~(GPIOA->IDR & button_down ));
p_but->up = (uint8_t)(~(GPIOA->IDR & button_up ));
p_but->left = (uint8_t)(~(GPIOA->IDR & button_left ));
p_but->esc = (uint8_t)(~(GPIOA->IDR & button_esc ));
p_but->right = (uint8_t)(~(GPIOA->IDR & button_right ));
}
/*****************************************************/
//Функция слежения за состоянием одной кнопки
/*****************************************************/
void button(uint8_t but, uint8_t *p_but_old)
{
uint8_t count=0;
uint8_t flag=0; 	
if((uint8_t)(~(GPIOA->IDR & but )) != *p_but_old) {
for(count=0; count<255; count++) 
{if((uint8_t)(~(GPIOA->IDR & but )) != *p_but_old) flag++;
else flag--;
}	
if (flag > 250){	
			  while(!(USART1->ISR & USART_ISR_TC));
			  USART1->TDR = but;
}
while((uint8_t)(~(GPIOA->IDR & but )) != *p_but_old){};
while((uint8_t)(~(GPIOA->IDR & but )) != *p_but_old){};
while((uint8_t)(~(GPIOA->IDR & but )) != *p_but_old){};
}
}


