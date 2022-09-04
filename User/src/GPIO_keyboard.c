#include "stm32f0xx.h"
#include "GPIO_keyboard.h"

uint8_t but_code, SEND_but_code, LAST_but_code ;
uint32_t count_but,  // время нажатого/отжатого  состояния без изменения == защита от дребезга
	REPEAT_count,	// счетчик периода повтора
	DELAY_count;	// счетчик задержки перед повтором
uint32_t flag=0;

/*****************************************************/
//Подпрограмма инициализации выводов, к которым подключена клавиатура
/*****************************************************/
void GPIO_keyboard_installation(void)
{
RCC->AHBENR |= RCC_AHBENR_GPIOAEN;		

	//Инициализация выводв кнопок K1...K7
GPIOA->MODER   &=~ (GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3 | GPIO_MODER_MODER4| GPIO_MODER_MODER5 | GPIO_MODER_MODER6|GPIO_MODER_MODER8);	//входы
GPIOA->MODER  |=GPIO_MODER_MODER8_0;	//выход		   
GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1 | GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6); //low
GPIOA->OTYPER  &=~ GPIO_OTYPER_OT_0|GPIO_OTYPER_OT_1|GPIO_OTYPER_OT_2|GPIO_OTYPER_OT_3|GPIO_OTYPER_OT_4|GPIO_OTYPER_OT_5|GPIO_OTYPER_OT_6;
GPIOA->PUPDR   |= GPIO_PUPDR_PUPDR0_0|GPIO_PUPDR_PUPDR1_0|GPIO_PUPDR_PUPDR2_0|GPIO_PUPDR_PUPDR3_0|GPIO_PUPDR_PUPDR4_0|GPIO_PUPDR_PUPDR5_0|GPIO_PUPDR_PUPDR6_0;
count_but=FIX_time ;
}
/*****************************************************/
//Подпрограмма слежения за состоянием кнопок
/*****************************************************/
uint8_t but_code;

void GPIO_button(void)
{
	
but_code=GPIOA->IDR;		
but_code = (~but_code) & 0x7F;	//shan
if(LAST_but_code==but_code) {if(count_but) count_but--; }// считаем время нового кода
			else {count_but=FIX_time ; 	// счетчик нажатия по новой
			DELAY_count=DELAY_time;		// счетчик задержки по новой
			REPEAT_count=REPEAT_time;	// счетчик повтора по новой
			LAST_but_code=but_code;}

if(count_but==0 && LAST_but_code ){	// новый код длительный и не = 0
	SEND_but_code=LAST_but_code;	//запоминаем код для отсылки
	if(DELAY_count==DELAY_time) USART1->TDR=  SEND_but_code;//разрешить прерывания передачи 1-го одиночного
	if(DELAY_count) DELAY_count-- ; // начинаем отсчет времени повтора
	 else{	//задержка кончилась
		if(REPEAT_count) REPEAT_count-- ; // начинаем отсчет времени повтора
		else 	{ USART1->TDR=  SEND_but_code;//разрешить прерывания передачи повторного
			REPEAT_count=REPEAT_time;
			}
	  	
	      }
}

}

