#include "stm32f0xx.h"
#include "LCD_HD44780.h"

struct data_lcd  *p_lcd;
struct time  *t;

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
	SysTick_Config(48000000/100000);
	
	RCC->AHBENR |= (RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN);
	//RS
//	GPIOA->MODER |= GPIO_MODER_MODER15_0;   
//	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15;
//	GPIOA->OTYPER &=~ GPIO_OTYPER_OT_15;
//  GPIOA->PUPDR &=~ GPIO_PUPDR_PUPDR15_0;
	GPIOC->MODER |= GPIO_MODER_MODER11_0;   
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR11;
	GPIOC->OTYPER &=~ GPIO_OTYPER_OT_11;
  GPIOC->PUPDR &=~ GPIO_PUPDR_PUPDR11_0;	

	//R/W
	GPIOB->MODER |= GPIO_MODER_MODER3_0;   
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR3;
	GPIOB->OTYPER &=~ GPIO_OTYPER_OT_3;
  GPIOB->PUPDR &=~ GPIO_PUPDR_PUPDR3_0;

	//E
//	GPIOA->MODER |= GPIO_MODER_MODER7_0;   
//	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7;
//	GPIOA->OTYPER &=~ GPIO_OTYPER_OT_7;
//  GPIOA->PUPDR &=~ GPIO_PUPDR_PUPDR7_0;	
	GPIOC->MODER |= GPIO_MODER_MODER12_0;   
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR12;
	GPIOC->OTYPER &=~ GPIO_OTYPER_OT_12;
  GPIOC->PUPDR &=~ GPIO_PUPDR_PUPDR12_0;	
	
	//DB4...DB7
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR4_1 | GPIO_PUPDR_PUPDR5_1 | GPIO_PUPDR_PUPDR6_1 | GPIO_PUPDR_PUPDR7_1);
	GPIOB->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7);	
	GPIOB->OTYPER &=~ (GPIO_OTYPER_OT_4 | GPIO_OTYPER_OT_5 | GPIO_OTYPER_OT_6 | GPIO_OTYPER_OT_7);
  GPIOB->MODER |= (GPIO_MODER_MODER4_0 | GPIO_MODER_MODER5_0 | GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0);   

  GPIOB->ODR &=~ GPIO_ODR_3;
	delay(5000);
}
/*****************************************************/
//Функция инициализации дисплея
/*****************************************************/
void HD44780_installation(void)
{ 
	//Иницйиализация 4-х проводной шины
	GPIOC->ODR &=~ GPIO_ODR_11;  //RS=0
	GPIOC->ODR &=~ GPIO_ODR_12;  //E=0
	GPIOB->ODR &=~ (GPIO_ODR_7 | GPIO_ODR_6 | GPIO_ODR_4);  
	GPIOB->ODR |= GPIO_ODR_5;// 0010
	GPIOC->ODR |= GPIO_ODR_12;  //E=1
  delay(4);
	GPIOC->ODR &=~ GPIO_ODR_12;  //E=0
  delay(4);
	
	GPIOB->ODR &=~ (GPIO_ODR_7 | GPIO_ODR_6 | GPIO_ODR_4);  
	GPIOB->ODR |= GPIO_ODR_5;// 0010
	GPIOC->ODR |= GPIO_ODR_12;  //E=1
  delay(4);	
	GPIOC->ODR &=~ GPIO_ODR_12;  //E=0
  delay(4);	
	GPIOB->ODR |= GPIO_ODR_7;
	GPIOB->ODR &=~ (GPIO_ODR_6 | GPIO_ODR_5 | GPIO_ODR_4);  //1000
	GPIOC->ODR |= GPIO_ODR_12;  //E=1
  delay(4);	
	GPIOC->ODR &=~ GPIO_ODR_12;  //E=0
  delay(4);	
		
  command(0x0C); //00001111 // нет курсора
	command(0x01); //00000001 // очистить экран, курсор на первую позицию
	//command(0x06); //00000110	//курсор есть и он мигает
	command(0x04);//нет курсора
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
/*****************************************************/
//Функция очистить экран дисплея
/*****************************************************/
void lcd_clr(void)
{
			command(0x01);
			//command(0x0C);
}	
/*****************************************************/
//Функция перезагрезки дисплея с выводом строк
/*****************************************************/
void reloadHD44780(void)
{
		if(t->tic>=650)
		{
			t->sec++;
			t->tic=0;
		if(t->sec == COUNT_SEC)
			{
				t->minute++;
				t->sec=0;		
				if(t->minute == COUNT_MINUTE)
					{
						t->minute=0;		
		        HD44780_installation();
		        HD44780_installation();						
            installation_rus_alphabet();

		        send_text((unsigned char *)p_lcd->strings_1, 1, p_lcd->cursor_1);
		        send_text((unsigned char *)p_lcd->strings_2, 2, p_lcd->cursor_2);	
					}
			}
		}
}
/*****************************************************/
//Функция вывода символа
/*****************************************************/
void send_data(uint8_t data)
{
	GPIOC->ODR |= GPIO_ODR_11; //RS=1
	GPIOC->ODR &=~ GPIO_ODR_12;  //E=0
		if(data & 0x10) GPIOB->ODR |= GPIO_ODR_4; 
		else GPIOB->ODR &=~ GPIO_ODR_4;
		if(data & 0x20) GPIOB->ODR |= GPIO_ODR_5; 
		else GPIOB->ODR &=~ GPIO_ODR_5;
		if(data & 0x40) GPIOB->ODR |= GPIO_ODR_6; 
		else GPIOB->ODR &=~ GPIO_ODR_6;
		if(data & 0x80) GPIOB->ODR |= GPIO_ODR_7; 
		else GPIOB->ODR &=~ GPIO_ODR_7;

	GPIOC->ODR |= GPIO_ODR_12;  //E=1
	delay(2);
	GPIOC->ODR &=~ GPIO_ODR_12;  //E=0
  delay(2);
	GPIOB->ODR |= (GPIO_ODR_4 | GPIO_ODR_5 | GPIO_ODR_6 | GPIO_ODR_7); 
		if(data & 0x01) GPIOB->ODR |= GPIO_ODR_4; 
		else GPIOB->ODR &=~ GPIO_ODR_4;
		if(data & 0x02) GPIOB->ODR |= GPIO_ODR_5; 
		else GPIOB->ODR &=~ GPIO_ODR_5;
		if(data & 0x04) GPIOB->ODR |= GPIO_ODR_6; 
		else GPIOB->ODR &=~ GPIO_ODR_6;
		if(data & 0x08) GPIOB->ODR |= GPIO_ODR_7; 
		else GPIOB->ODR &=~ GPIO_ODR_7;
		
	GPIOC->ODR |= GPIO_ODR_12;  //E=1
  delay(2);
	GPIOC->ODR &=~ GPIO_ODR_12;  //E=0
  delay(2);
}
/*****************************************************/
//Функция отплавить команду
/*****************************************************/
void command(uint8_t data)
{
	GPIOC->ODR &=~ GPIO_ODR_11;  //RS=0
	GPIOC->ODR &=~ GPIO_ODR_12;  //E=0

		if(data & 0x10) GPIOB->ODR |= GPIO_ODR_4; 
		else GPIOB->ODR &=~ GPIO_ODR_4;
		if(data & 0x20) GPIOB->ODR |= GPIO_ODR_5; 
		else GPIOB->ODR &=~ GPIO_ODR_5;
		if(data & 0x40) GPIOB->ODR |= GPIO_ODR_6; 
		else GPIOB->ODR &=~ GPIO_ODR_6;
		if(data & 0x80) GPIOB->ODR |= GPIO_ODR_7; 
		else GPIOB->ODR &=~ GPIO_ODR_7;

	GPIOC->ODR |= GPIO_ODR_12;  //E=1
  delay(1);
	GPIOC->ODR &=~ GPIO_ODR_12;  //E=0
  delay(1);

		if(data & 0x01) GPIOB->ODR |= GPIO_ODR_4; 
		else GPIOB->ODR &=~ GPIO_ODR_4;
		if(data & 0x02) GPIOB->ODR |= GPIO_ODR_5; 
		else GPIOB->ODR &=~ GPIO_ODR_5;
		if(data & 0x04) GPIOB->ODR |= GPIO_ODR_6; 
		else GPIOB->ODR &=~ GPIO_ODR_6;
		if(data & 0x08) GPIOB->ODR |= GPIO_ODR_7; 
		else GPIOB->ODR &=~ GPIO_ODR_7;
		
	GPIOC->ODR |= GPIO_ODR_12;  //E=1
  delay(1);
	GPIOC->ODR &=~ GPIO_ODR_12;  //E=0
  delay(1);	
}

/*****************************************************/
//Функция вывода символа на дисплее с учетом кириллицы и положения курсора
/*****************************************************/
void send_text(uint8_t * txt,uint8_t  st, uint8_t poz) 
{  uint8_t var; 
	 uint8_t n;//положение курсора
	 uint8_t ls[64]={0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xa4,  /*А, Б, В, Г, Д, Е, Ж, З*/
                   0xa5,0xa6,0x4B,0x02,0x4D,0x48,0x4F,0x03,  /*И, й, К, Л, М, Н, О, П*/
                   0x50,0x43,0x54,0xa9,0xaa,0x58,0xe1,0xab,  /*Р, С, Т, У, Ф, Х, Ц, Ч*/
                   0xac,0xe2,0xAD,0xae,0x06,0xAf,0xB0,0xB1,  /*Ш, Щ, Ъ, Ы, Ь, Э, Ю, Я*/
                   0x61,0xb2,0xB3,0xb4,0xE3,0x65,0xB6,0xB7,  /*а, б, в, г, д, е, ж, з*/
                   0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,  /*и, й, к, л, м, н, о, п*/
                   0x70,0x63,0xBF,0x79,0x04,0x78,0xE5,0xC0,  /*р, с, т, у, ф, х, ц, ч*/
		               0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7}; /*ш, щ, ъ, ы, ь, э, ю, я*/
	 
									 
									 
//вопрос с Й и й(нет большой), нет "йо", есть маленикая 
//в стандартной таблице маленькая д, с выглядит полохо
	  switch(st) {case 0x01 : n=0x80; break;
                case 0x02 : n=0xC0; break;}
		if(poz>19) poz=19;
        n+=poz;					
				command(n); 
							
	while(*txt)
	{var = *txt;
		
		if(var  >= 0xC0)
		{
  		var -=0xC0;
		  var=ls[var];
		}
//Символы { и } заменены на большую и маленькую буквы Я соответственно		
		switch (var)
		{
			case 0x7B:
				var = 0xB1;
			break;
			case 0x7D:
				var = 0xC7;
			break;
		}
		send_data(var);
		txt++;
	}
}
/*****************************************************/
//Функция для вывода на дисплей
/*****************************************************/
void record_strings(struct data_lcd * pLcd_st)
{
	uint8_t i=0;	
	
		     if(p_lcd->flag_run_sr1)
		     {
					send_text((unsigned char *)pLcd_st->strings_2, 2, pLcd_st->cursor_2); 
					//pLcd_st->i =0;	
			    p_lcd->num_strings=0x01;
			    p_lcd->side=0x01;					 
			    circle_text(p_lcd);
		     }	
	
			 	 if(pLcd_st->flag_out_str1) //вывод первой строки
				 {
					 //pLcd_st->i =0;
					 lcd_clr();//очистить дисплей
					 if((pLcd_st->side == 0x01) | (pLcd_st->side == 0x02)) circle_text(pLcd_st);//вывести бегущую строку

					 //вывести на дисплей
					 send_text((unsigned char *)pLcd_st->strings_1_new, pLcd_st->num_strings, pLcd_st->cursor_new); 
					 send_text((unsigned char *)pLcd_st->strings_2, 2, pLcd_st->cursor_2); 
					 //переписать значение новой строки в буфер для старой
           for(i =0; i < pLcd_st->len_new-1; i++) pLcd_st->strings_1[i] = pLcd_st->strings_1_new[i];
           for(i = pLcd_st->len_new-1; i < ID_MAX_STR; i++) pLcd_st->strings_1[i] = 0x90;					 
           for(i=0; i < ID_MAX_NEW; i++) pLcd_st->strings_1_new[i] = 0x90;	

           //pLcd_st->i = 0;					 
           pLcd_st->len_str_1 = pLcd_st->len_new;
           pLcd_st->cursor_1 = pLcd_st->cursor_new;
           pLcd_st->cursor_new = 0;
           pLcd_st->side=0x00;
           pLcd_st->take_data = 0x00;
					 
					 pLcd_st->flag_out_str1=0;
				 }
				 
			 	 if(pLcd_st->flag_out_str2) //вывод второй строки
				 {
					 //pLcd_st->i =0;
					 lcd_clr();//очистить дисплей
					 if((pLcd_st->side == 0x01) | (pLcd_st->side == 0x02)) circle_text(pLcd_st);//вывести бегущую строку
					 
					 //вывести на дисплей
           send_text((unsigned char *)pLcd_st->strings_1, 1, pLcd_st->cursor_1);
					 send_text((unsigned char *)pLcd_st->strings_2_new, pLcd_st->num_strings, pLcd_st->cursor_new);
					 
					 //переписать значение новой строки в буфер для старой
           for(i =0; i < pLcd_st->len_new-1; i++) pLcd_st->strings_2[i] = pLcd_st->strings_2_new[i];
           for(i = pLcd_st->len_new-1; i < ID_MAX_STR; i++) pLcd_st->strings_2[i] = 0x90;
           for(i=0; i < ID_MAX_NEW; i++) pLcd_st->strings_2_new[i] = 0x90;
            	
           //pLcd_st->i =0;					 
           pLcd_st->len_str_2 = pLcd_st->len_new;
           pLcd_st->cursor_2 = pLcd_st->cursor_new;
           pLcd_st->cursor_new = 0;
           pLcd_st->side=0x00;
           pLcd_st->take_data = 0x00;
						
					 pLcd_st->flag_out_str2=0;
				 } 
}
/*****************************************************/
//Функция вывода бегущей строки
/*****************************************************/
void circle_text(struct data_lcd *pLCD)
{
	 uint8_t i;
	 uint8_t n;//положение курсора
	 uint8_t count;
	 uint8_t k;
	 uint8_t ls[64]={0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xa4,  /*А, Б, В, Г, Д, Е, Ж, З*/
                   0xa5,0xa6,0x4B,0x02,0x4D,0x48,0x4F,0x03,  /*И, й, К, Л, М, Н, О, П*/
                   0x50,0x43,0x54,0xa9,0xaa,0x58,0xe1,0xab,  /*Р, С, Т, У, Ф, Х, Ц, Ч*/
                   0xac,0xe2,0xAD,0xae,0x06,0xAf,0xB0,0xB1,  /*Ш, Щ, Ъ, Ы, Ь, Э, Ю, Я*/
                   0x61,0xb2,0xB3,0xb4,0xE3,0x65,0xB6,0xB7,  /*а, б, в, г, д, е, ж, з*/
                   0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,  /*и, й, к, л, м, н, о, п*/
                   0x70,0x63,0xBF,0x79,0x04,0x78,0xE5,0xC0,  /*р, с, т, у, ф, х, ц, ч*/
		               0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7}; /*ш, щ, ъ, ы, ь, э, ю, я*/
 	
switch(pLCD->num_strings) //определение номера строки
{ 
/*****************************************************/
//Вывод 1-й строки
/*****************************************************/
case 0x01 : 
//положение курсора	
n=0x80; 
if(pLCD->cursor_new>19) pLCD->cursor_new=19;
n+=pLCD->cursor_new;


for (i=0; i < ID_MAX_VIDEO_1; i++) pLCD->video_buff_1[i] = 0x90;	

switch (pLCD->side)	//определение направления вывода
{ 
/*****************************************************/
//Вывод справа
/*****************************************************/	
case 0x01:	
if(pLCD->flag_run_sr1)	//вывод бегущей строки комментариев
{for (i=0;	i < 20; i++) pLCD->video_buff_1[i] = 0x90;	
k=0;
for (i=20; i < 20+pLCD->len_str_run-1; i++)	{pLCD->video_buff_1[i] = pLCD->run_string[k];	k++;}
for(count=0; count < MAX_VIDEO_1; count++){
send_text((unsigned char *)pLCD->strings_2, 2, pLCD->cursor_2);	
command(n);	
	for(i=count; i < ID_MAX_VIDEO_1; i++)
	{
		if(pLCD->video_buff_1[i]  >= 0xC0)
		{
  		pLCD->video_buff_1[i] -=0xC0;
		  pLCD->video_buff_1[i]=ls[pLCD->video_buff_1[i]];
		}
//Символы { и } заменены на большую и маленькую буквы Я соответственно
		switch (pLCD->video_buff_1[i])
		{
			case 0x7B:
				pLCD->video_buff_1[i] = 0xB1;
			break;
			case 0x7D:
				pLCD->video_buff_1[i] = 0xC7;
			break;
		}
		send_data(pLCD->video_buff_1[i]);//вывести символ на дисплей
	}
	
  delay(220000);
}	
}	
else {//вывод первой строки	

//переписать новую и старую строку в буффер 
for (i=0; i < pLCD->len_str_1-1; i++) pLCD->video_buff_1[i] = pLCD->strings_1[i];	
k=20;
for(i=0;	i < pLCD->len_new-1; i++) {pLCD->video_buff_1[k] = pLCD->strings_1_new[i];	k++;} 	
//выбод буфера на дисплей
for(count=0; count<MAX_LEN_LCD; count++){
send_text((unsigned char *)pLCD->strings_2, 2, pLCD->cursor_2);
command(n);	
	for(i=count; i < ID_MAX_VIDEO_1; i++)
	{
		if(pLCD->video_buff_1[i]  >= 0xC0)
		{
  		pLCD->video_buff_1[i] -=0xC0;
		  pLCD->video_buff_1[i]=ls[pLCD->video_buff_1[i]];
		}
//Символы { и } заменены на большую и маленькую буквы Я соответственно
		switch (pLCD->video_buff_1[i])
		{
			case 0x7B:
				pLCD->video_buff_1[i] = 0xB1;
			break;
			case 0x7D:
				pLCD->video_buff_1[i] = 0xC7;
			break;
		}
		send_data(pLCD->video_buff_1[i]);//вывести символ на дисплей
	}
  delay(12000);		
}
}
//pLCD->i=0;
break;

/*****************************************************/
//Вывод слева
/*****************************************************/
case 0x02:

//переписать новую и старую строку в буффер 	
for (i=0;	i < pLCD->len_new-1; i++) pLCD->video_buff_1[i] = pLCD->strings_1_new[i];	
k=0;
for (i=21; i < 21+pLCD->len_str_1-1; i++)	{pLCD->video_buff_1[i] = pLCD->strings_1[k];	k++;}
i = 0;
//вывод буфера на дисплей
for(count=MAX_LEN_LCD; count>0; count--){
send_text((unsigned char *)pLCD->strings_2, 2, pLCD->cursor_2);	
command(n);	
	for(i=count; i < ID_MAX_VIDEO_1; i++)
	{
		if(pLCD->video_buff_1[i]  >= 0xC0)
		{
  		pLCD->video_buff_1[i] -=0xC0;
		  pLCD->video_buff_1[i]=ls[pLCD->video_buff_1[i]];
		}
//Символы { и } заменены на большую и маленькую буквы Я соответственно
		switch (pLCD->video_buff_1[i])
		{
			case 0x7B:
				pLCD->video_buff_1[i] = 0xB1;
			break;
			case 0x7D:
				pLCD->video_buff_1[i] = 0xC7;
			break;
		}
		send_data(pLCD->video_buff_1[i]);//вывести символ на дисплей
	}
	
  delay(12000);
}	

	
//pLCD->i=0;

break;
}

break;

/*****************************************************/
//Вывод 2-й строки
/*****************************************************/
case 0x02 : 
//положение курсора	
n=0xC0; 
if(pLCD->cursor_new>19) pLCD->cursor_new=19;
n+=pLCD->cursor_new;	

for (i=0; i < ID_MAX_VIDEO_2; i++) pLCD->video_buff_2[i] = 0x90;	

switch (pLCD->side)	
{ 
/*****************************************************/
//Вывод слева
/*****************************************************/	
case 0x01:	
//переписать новую и старую строку в буффер 	
for (i=0; i < pLCD->len_str_2-1; i++) pLCD->video_buff_2[i] = pLCD->strings_2[i];	
k=20;
for(i=0;	i < pLCD->len_new-1; i++) {pLCD->video_buff_2[k] = pLCD->strings_2_new[i];	k++;} 	
//выбод буфера на дисплей
for(count=0; count<MAX_LEN_LCD; count++){
send_text((unsigned char *)pLCD->strings_1, 1, pLCD->cursor_1);
command(n);	
	for(i=count; i < ID_MAX_VIDEO_2; i++)
	{
		if(pLCD->video_buff_2[i]  >= 0xC0)
		{
  		pLCD->video_buff_2[i] -=0xC0;
		  pLCD->video_buff_2[i]=ls[pLCD->video_buff_2[i]];
		}
//Символы { и } заменены на большую и маленькую буквы Я соответственно
		switch (pLCD->video_buff_2[i])
		{
			case 0x7B:
				pLCD->video_buff_2[i] = 0xB1;
			break;
			case 0x7D:
				pLCD->video_buff_2[i] = 0xC7;
			break;
		}
		send_data(pLCD->video_buff_2[i]);//вывести символ на дисплей
		
	}
  delay(12000);	
}
//pLCD->i=0;
break;

/*****************************************************/
//Вывод справа
/*****************************************************/
case 0x02:
//переписать новую и старую строку в буффер 	
for (i=0;	i < pLCD->len_new-1; i++) pLCD->video_buff_2[i] = pLCD->strings_2_new[i];	
k=0;
for (i=21; i < 21+pLCD->len_str_2-1; i++)	{pLCD->video_buff_2[i] = pLCD->strings_2[k];	k++;}
//выбод буфера на дисплей	
i = 0;
for(count=MAX_LEN_LCD; count>0; count--){
send_text((unsigned char *)pLCD->strings_1, 1, pLCD->cursor_1);	
command(n);	
	for(i=count; i < ID_MAX_VIDEO_2; i++)
	{
		if(pLCD->video_buff_2[i]  >= 0xC0)
		{
  		pLCD->video_buff_2[i] -=0xC0;
		  pLCD->video_buff_2[i]=ls[pLCD->video_buff_2[i]];
		}
//Символы { и } заменены на большую и маленькую буквы Я соответственно
		switch (pLCD->video_buff_2[i])
		{
			case 0x7B:
				pLCD->video_buff_2[i] = 0xB1;
			break;
			case 0x7D:
				pLCD->video_buff_2[i] = 0xC7;
			break;
		}
		send_data(pLCD->video_buff_2[i]);//вывести символ на дисплей
	}
	
  delay(12000);	
}
//pLCD->i=0;
break;
}
break;
}

}	


