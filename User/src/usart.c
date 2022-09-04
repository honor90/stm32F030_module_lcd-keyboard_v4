#include "stm32f0xx.h"
#include "usart.h"
#include "LCD.h"
#include "GPIO_keyboard.h"

extern uint8_t but_code, SEND_but_code, LAST_but_code ;
uint8_t j;
/*****************************************************/
//Функция инициализации модуля USART
/*****************************************************/
void usart_installation(void)
{
		GPIOA->MODER   |= GPIO_MODER_MODER9_1;         
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;      
	
	  GPIOA->MODER   |= GPIO_MODER_MODER10_1;         
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR10;
	
		//Выводы 9 и 10 порта А как альтернативные функции//
    GPIOA->AFR[1]  |= 0x0110; 
		
		//Включение тактирования USART1//
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;		
		
		//Инициализация USART1//
    USART1->CR1 |= USART_CR1_UE;
    USART1->CR1 &= ~USART_CR1_M; 
    USART1->CR2 &= ~USART_CR2_STOP; 
    USART1->BRR = APBCLK/BAUDRATE; 
    USART1->CR1 |= USART_CR1_TE; 
    USART1->CR1 |= USART_CR1_RE; 
   USART1->CR1 |= USART_CR1_RXNEIE; 	
		
		//Разрешить прерывание от USART1//
    NVIC_EnableIRQ(USART1_IRQn);
}
/*****************************************************/
//Обработчик перывания модуля USART
/*****************************************************/
void USART1_IRQHandler(void)
{	
	static uint8_t i = 0;
	static uint8_t command = 0x00;
	
	if(USART1->ISR & USART_ISR_RXNE)
	{USART1->ISR &= ~USART_ISR_RXNE;
			   if (command == 0x00)
					 {
						 command = USART1->RDR;
						 
						
						 //if((command&0x1F) == 0x1F) p_lcd->cursor_new=0x00;
						 if((command&0x20)>>5 == 0) p_lcd->num_strings=0x01;
						 if((command&0x20)>>5 == 1) p_lcd->num_strings=0x02;
						 if(p_lcd->num_strings == 0x02) p_lcd->cursor_2 = command&0x1F;
						 if((command&0xC0)>>6 == 0x03 || (command&0xC0)>>6 == 0x00) p_lcd->side=0x00;
						 if((command&0xC0)>>6 == 0x01) p_lcd->side=RIGTH;
						 if((command&0xC0)>>6 == 0x02) p_lcd->side=LEFT;
						 
						 return;
					 }		
					 
				switch(p_lcd->num_strings)	
				{
					case 0x01:	//принять первую строку по USART  и записать в буфер	
//					if(p_lcd->flag_run_sr1)	{
//						/* если бегущая строка */
//						if(i<MAX_NEW)
//						{						
//							p_lcd->run_string[i] = USART1->RDR; 
//							i++;
//							p_lcd->len_str_run = i;	
//						}
//					}
//         else{
					 /* если простая строка */
						if(i<MAX_NEW)
						{							 
							p_lcd->strings_1_new[i] = USART1->RDR; 
							i++;
							p_lcd->len_new = i;	
						}	
				 
         if(USART1->RDR  == 0x00) //если последний байт 0х00 
				 {
					 p_lcd->flag_out_str1=1;//установить флаг наличия 1-й строки
				   
					 command=0x00;	//??
					 for(j=i-1; j < ID_MAX_NEW; j++) p_lcd->strings_1_new[j] = 0x90;
					 i=0;
				 }	
			 //}
				 
          break;
					case 0x02:  //принять вторую строку по USART и записать в буфер
						if(i<MAX_NEW)
						{
								p_lcd->strings_2_new[i] = USART1->RDR;
								i++;
								p_lcd->len_new = i;		
						}
         if(USART1->RDR == 0x00) //если последний байт 0х00
				 {
					 p_lcd->flag_out_str2=1;//установить флаг наличия 2-й строки	
				   
					 command=0x00;	//??
					 for(j=i-1; j < ID_MAX_NEW; j++) p_lcd->strings_2_new[j] = 0x90;
					 i=0;
				 }		
											 
          break;
				}					
}
	 if(USART1->ISR & USART_ISR_TXE) //отправить код нажатой кнопки
	{
//		USART1->ISR &= ~USART_ISR_TXE;
//		USART1->TDR = SEND_but_code;
//		USART1->CR1 &=~ USART_CR1_TXEIE; //запертить перерывание передачи
		
	}
		
}
