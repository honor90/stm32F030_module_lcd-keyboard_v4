#include "stm32f0xx.h"
#include "hd44780_driver.h"


struct time  *t;
uint32_t count_ligth;
/*****************************************************/
//Обработчик прерывания SysTick, в котором отсчитывается время для задержек и время для перезагрузки дисплея
/*****************************************************/
void SysTick_Handler(void)
{
	if(t->tic_delay)	t->tic_delay--;
	t->mili_tic++;
	
	if(t->mili_tic==1000)
	{
		t->mili_tic=0;
		t->tic++;
	}
	
}
/*****************************************************/
//Функция задержки
/*****************************************************/
void delay(uint32_t time)
{	
	t->tic_delay = time;
	while(t->tic_delay){}	
}
/*****************************************************/
//Инициализация портов МК для работы с дисплеем
/*****************************************************/
void portInstallation(void)
{
	SysTick_Config(8000000/1000);
	
	RCC->AHBENR |= (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN);
	
	//????????????? ?????? ??? ?????????
//	GPIOA->MODER   |= GPIO_MODER_MODER0_0;   
//	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;
//  GPIOA->PUPDR   |= GPIO_PUPDR_PUPDR0_0;
	
	//RS
	GPIOA->MODER |= GPIO_MODER_MODER15_0;   
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;
	GPIOA->OTYPER &=~ GPIO_OTYPER_OT_15;
  GPIOA->PUPDR &=~ GPIO_PUPDR_PUPDR15_0;
/*??? ??????? */	
//	GPIOC->MODER |= GPIO_MODER_MODER11_0;   
//	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11;
//	GPIOC->OTYPER &=~ GPIO_OTYPER_OT_11;
//  GPIOC->PUPDR &=~ GPIO_PUPDR_PUPDR11_0;	

	//R/W
	GPIOB->MODER |= GPIO_MODER_MODER3_0;   
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;
	GPIOB->OTYPER &=~ GPIO_OTYPER_OT_3;
  GPIOB->PUPDR &=~ GPIO_PUPDR_PUPDR3_0;

	//E
	GPIOA->MODER |= GPIO_MODER_MODER11_0;   
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11;
	GPIOA->OTYPER &=~ GPIO_OTYPER_OT_11;
  GPIOA->PUPDR &=~ GPIO_PUPDR_PUPDR11_0;	

/*??? ??????? */
//	GPIOC->MODER |= GPIO_MODER_MODER12_0;   
//	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;
//	GPIOC->OTYPER &=~ GPIO_OTYPER_OT_12;
//  GPIOC->PUPDR &=~ GPIO_PUPDR_PUPDR12_0;	
	
	//DB4...DB7
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1 | GPIO_PUPDR_PUPDR6_1 | GPIO_PUPDR_PUPDR7_1);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);	
	GPIOB->OTYPER &=~ (GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7);
  GPIOB->MODER |= (GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);   

  GPIOB->ODR &=~ GPIO_ODR_3;
	delay(5000);
	delay(5000);
	delay(5000);
		delay(5000);
	delay(5000);
	delay(5000);
		delay(5000);
	delay(5000);
	delay(5000);
}
/*****************************************************/
//Функция отплавить команду
/*****************************************************/
void command(uint8_t data)
{
	PORT->ODR &=~ RS;  //RS=0
	
	PORT->ODR &=~ E;  //E=0

	
	GPIOB->ODR = data&0xF0;

//		if(data & 0x10) GPIOB->ODR |= GPIO_ODR_4; 
//		else GPIOB->ODR &=~ GPIO_ODR_4;
//		if(data & 0x20) GPIOB->ODR |= GPIO_ODR_5; 
//		else GPIOB->ODR &=~ GPIO_ODR_5;
//		if(data & 0x40) GPIOB->ODR |= GPIO_ODR_6; 
//		else GPIOB->ODR &=~ GPIO_ODR_6;
//		if(data & 0x80) GPIOB->ODR |= GPIO_ODR_7; 
//		else GPIOB->ODR &=~ GPIO_ODR_7;

	PORT->ODR |= E;  //E=1
  delay(1);
	
	//GPIOC->ODR |= GPIO_ODR_3;
	//GPIOB->MODER &=~ GPIO_MODER_MODER7;
	delay(1);
	//while((GPIOC->IDR & GPIO_IDR_7)>>7 == 0){}

	//GPIOB->MODER |= GPIO_MODER_MODER7_0;
	//GPIOC->ODR &=~ GPIO_ODR_3;
		
	PORT->ODR &=~ E;  //E=0
	delay(1);

	//while((GPIOC->IDR & GPIO_IDR_7)>>7 == 0){}
		
		GPIOB->ODR = (data&0x0f)<<4;

//		if(data & 0x01) GPIOB->ODR |= GPIO_ODR_4; 
//		else GPIOB->ODR &=~ GPIO_ODR_4;
//		if(data & 0x02) GPIOB->ODR |= GPIO_ODR_5; 
//		else GPIOB->ODR &=~ GPIO_ODR_5;
//		if(data & 0x04) GPIOB->ODR |= GPIO_ODR_6; 
//		else GPIOB->ODR &=~ GPIO_ODR_6;
//		if(data & 0x08) GPIOB->ODR |= GPIO_ODR_7; 
//		else GPIOB->ODR &=~ GPIO_ODR_7;
		
	PORT->ODR |= E;  //E=1
delay(2);
	//while((GPIOC->IDR & GPIO_IDR_7)>>7 == 0){}
		
	PORT->ODR &=~ E;  //E=0		
  delay(2);		
}
/*****************************************************/
//Функция инициализации дисплея
/*****************************************************/
void HD44780_installation(void)
{ 
	//Иницйиализация 4-х проводной шины
	PORT->ODR &=~ RS;  //RS=0
	
	PORT->ODR &=~ E;  //E=0
	
	GPIOB->ODR &=~ (GPIO_ODR_7 | GPIO_ODR_6 | GPIO_ODR_4);  
	GPIOB->ODR |= GPIO_ODR_5;// 0010
	PORT->ODR |= E;  //E=1
		
  delay(15);
	PORT->ODR &=~ E;  //E=0
	
  delay(15);
	
	GPIOB->ODR &=~ (GPIO_ODR_7 | GPIO_ODR_6 | GPIO_ODR_4);  
	GPIOB->ODR |= GPIO_ODR_5;// 0010
	PORT->ODR |= E;  //E=1

  delay(15);	
	PORT->ODR &=~ E;  //E=0

  delay(15);	
	GPIOB->ODR |= GPIO_ODR_7;
	GPIOB->ODR &=~ (GPIO_ODR_6 | GPIO_ODR_5 | GPIO_ODR_4);  //1000
	PORT->ODR |= E;  //E=1

  delay(15);	
	PORT->ODR &=~ E;  //E=0

  delay(15);	
	
	
	PORT->ODR &=~ RS;  //RS=0
	
	command(0x06); //	

	command(0x0C); 
	
}
/*****************************************************/
//Функция вывода символа
/*****************************************************/
void send_data(uint8_t data)
{
	PORT->ODR |= RS; //RS=1
	
	PORT->ODR &=~ E;  //E=0

	GPIOB->ODR = data&0xF0;
	
//		if(data & 0x10) GPIOB->ODR |= GPIO_ODR_4; 
//		else GPIOB->ODR &=~ GPIO_ODR_4;
//		if(data & 0x20) GPIOB->ODR |= GPIO_ODR_5; 
//		else GPIOB->ODR &=~ GPIO_ODR_5;
//		if(data & 0x40) GPIOB->ODR |= GPIO_ODR_6; 
//		else GPIOB->ODR &=~ GPIO_ODR_6;
//		if(data & 0x80) GPIOB->ODR |= GPIO_ODR_7; 
//		else GPIOB->ODR &=~ GPIO_ODR_7;

	PORT->ODR |= E;  //E=1
delay(1);
	//while((GPIOC->IDR & GPIO_IDR_7)>>7 == 0){}
	
	PORT->ODR &=~ E;  //E=0
delay(1);
	//while((GPIOC->IDR & GPIO_IDR_7)>>7 == 0){}


GPIOB->ODR = (data&0x0f)<<4;		
//	GPIOB->ODR |= (GPIO_ODR_4 | GPIO_ODR_5 | GPIO_ODR_6 | GPIO_ODR_7); 
//		if(data & 0x01) GPIOB->ODR |= GPIO_ODR_4; 
//		else GPIOB->ODR &=~ GPIO_ODR_4;
//		if(data & 0x02) GPIOB->ODR |= GPIO_ODR_5; 
//		else GPIOB->ODR &=~ GPIO_ODR_5;
//		if(data & 0x04) GPIOB->ODR |= GPIO_ODR_6; 
//		else GPIOB->ODR &=~ GPIO_ODR_6;
//		if(data & 0x08) GPIOB->ODR |= GPIO_ODR_7; 
//		else GPIOB->ODR &=~ GPIO_ODR_7;
		
	PORT->ODR |= E;  //E=1
delay(1);
	//while((GPIOC->IDR & GPIO_IDR_7)>>7 == 0){}

	PORT->ODR &=~ E;  //E=0

  delay(1);
}
/*****************************************************/
//Функция для записи русских букв в память дисплея
/*****************************************************/
void installation_rus_alphabet(void)
{ uint16_t i; 
	uint8_t code_rs[64]={0x1F,0x10,0x10,0x1E,0x11,0x11,0x1E,00,  /* Б */
              		     0x11,0x11,0x13,0x15,0x19,0x11,0x11,00,  /* И */
              		     0x06,0x09,0x09,0x09,0x09,0x09,0x11,00,  /* Л */
              		     0x1F,0x11,0x11,0x11,0x11,0x11,0x11,00,  /* П */
		                   0x00,0x04,0x0E,0x15,0x15,0x0E,0x04,00,  /*ф*/
              		     /*0x04,0x0E,0x15,0x15,0x15,0x0E,0x04,00,*/  /* Ф */
              		     0x11,0x11,0x11,0x0F,0x01,0x01,0x01,00,  /* Ч */
              		     0x10,0x10,0x10,0x1E,0x11,0x11,0x1E,00,  /* Ь */
              		     0x0F,0x11,0x11,0x0F,0x05,0x09,0x11,00}; /* Я */
	                     
command(0x40);
	for (i=0; i<64; i++)
	{
	send_data(code_rs[i]);
	}
}
