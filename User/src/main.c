#include "stm32f0xx.h"
#include "LCD.h"
#include "hd44780_driver.h"
#include "usart.h"
#include "sensing_keyboard.h"

extern uint8_t but_code, LAST_but_code;


int main(void)
{
	
struct data_lcd	lcd;
struct time  tim;
p_lcd=&lcd;
t=&tim;
	


portInstallation();	
HD44780_installation();		
installation_rus_alphabet();
sensing_keyboard_installation();
usart_installation();

//Инициализация IWDG//
//		IWDG->KR = 0xCCCC;
//		IWDG->KR = 0x5555;
//		IWDG->PR = 0x0000;
//		IWDG->KR = 0xAAAA;	
	
  send_text((unsigned char *)"CPK-M2  ВИСАТ-СЕРВИС", 1, 0);
	send_text((unsigned char *)"  www.vsat-s.ru", 2, 0);


p_lcd->num_strings = 0;
p_lcd->cursor_new = 0;
p_lcd->cursor_1 = 0;
p_lcd->cursor_2 = 20;
p_lcd->len_str_1 = 0;
p_lcd->len_str_2 = 0;
p_lcd->len_new = 0;
p_lcd->flag_out_str1=0;
p_lcd->flag_out_str2=0;
p_lcd->side=0;
p_lcd->flag_run_sr1=0;	



t->mili_tic=0;
t->tic=0;
t->sec=0;
t->minute=0;	

RCC->APB2ENR |= RCC_APB2ENR_TIM1EN ;//Включение тактирования TIM1
//TIM1->CR1=TIM_CR1_OPM;	//одиночный 
//TIM1->ARR=40;		//время в мс для 50 и 60 гц надо кратно 10мс
TIM1->PSC=48000;	//1 кгц

	GPIOA->MODER   |=   GPIO_MODER_MODER8_0;   //PA8 - выход
	GPIOA->OSPEEDR |=   GPIO_OSPEEDER_OSPEEDR8; //Максимальная частота
	GPIOA->OTYPER  &=~  GPIO_OTYPER_OT_8;       //push-pull
  GPIOA->PUPDR   &=~  GPIO_PUPDR_PUPDR8_0;	//Без подягивающего резистора	

 // command(0x02);
	command(0xd3);

while(1)
	{
if (LAST_but_code != but_code){
	TIM1->ARR=60000 ;		//время в мс подсветки 60s=1min)
	TIM1->CNT=0;	//обнулить счетчик
	TIM1->CR1=TIM_CR1_OPM|TIM_CR1_CEN;	//одиночный 
	}
if(TIM1->CR1  &  TIM_CR1_CEN) GPIOA-> BSRR= 0x100; else  GPIOA->BRR=0x100;//зажечь или погасить подсветку PA8

		reloadHD44780();

		record_strings();	
		
    sensing_button();			
	}
}



