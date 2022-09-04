#include "stm32f0xx.h"
#include "GPIO_keyboard.h"

uint8_t but_code, SEND_but_code, LAST_but_code ;
uint32_t count_but,  // ����� ��������/��������  ��������� ��� ��������� == ������ �� ��������
	REPEAT_count,	// ������� ������� �������
	DELAY_count;	// ������� �������� ����� ��������
uint32_t flag=0;

/*****************************************************/
//������������ ������������� �������, � ������� ���������� ����������
/*****************************************************/
void GPIO_keyboard_installation(void)
{
RCC->AHBENR |= RCC_AHBENR_GPIOAEN;		

	//������������� ������ ������ K1...K7
GPIOA->MODER   &=~ (GPIO_MODER_MODER0 | GPIO_MODER_MODER1 | GPIO_MODER_MODER2 | GPIO_MODER_MODER3 | GPIO_MODER_MODER4| GPIO_MODER_MODER5 | GPIO_MODER_MODER6|GPIO_MODER_MODER8);	//�����
GPIOA->MODER  |=GPIO_MODER_MODER8_0;	//�����		   
GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR0 | GPIO_OSPEEDER_OSPEEDR1 | GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3 | GPIO_OSPEEDER_OSPEEDR4 | GPIO_OSPEEDER_OSPEEDR5 | GPIO_OSPEEDER_OSPEEDR6); //low
GPIOA->OTYPER  &=~ GPIO_OTYPER_OT_0|GPIO_OTYPER_OT_1|GPIO_OTYPER_OT_2|GPIO_OTYPER_OT_3|GPIO_OTYPER_OT_4|GPIO_OTYPER_OT_5|GPIO_OTYPER_OT_6;
GPIOA->PUPDR   |= GPIO_PUPDR_PUPDR0_0|GPIO_PUPDR_PUPDR1_0|GPIO_PUPDR_PUPDR2_0|GPIO_PUPDR_PUPDR3_0|GPIO_PUPDR_PUPDR4_0|GPIO_PUPDR_PUPDR5_0|GPIO_PUPDR_PUPDR6_0;
count_but=FIX_time ;
}
/*****************************************************/
//������������ �������� �� ���������� ������
/*****************************************************/
uint8_t but_code;

void GPIO_button(void)
{
	
but_code=GPIOA->IDR;		
but_code = (~but_code) & 0x7F;	//shan
if(LAST_but_code==but_code) {if(count_but) count_but--; }// ������� ����� ������ ����
			else {count_but=FIX_time ; 	// ������� ������� �� �����
			DELAY_count=DELAY_time;		// ������� �������� �� �����
			REPEAT_count=REPEAT_time;	// ������� ������� �� �����
			LAST_but_code=but_code;}

if(count_but==0 && LAST_but_code ){	// ����� ��� ���������� � �� = 0
	SEND_but_code=LAST_but_code;	//���������� ��� ��� �������
	if(DELAY_count==DELAY_time) USART1->TDR=  SEND_but_code;//��������� ���������� �������� 1-�� ����������
	if(DELAY_count) DELAY_count-- ; // �������� ������ ������� �������
	 else{	//�������� ���������
		if(REPEAT_count) REPEAT_count-- ; // �������� ������ ������� �������
		else 	{ USART1->TDR=  SEND_but_code;//��������� ���������� �������� ����������
			REPEAT_count=REPEAT_time;
			}
	  	
	      }
}

}

