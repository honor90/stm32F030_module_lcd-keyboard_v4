#include "stm32f0xx.h"
#include "sensing_keyboard.h"

int16_t bit;
uint8_t TSC_GROUP_count;


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define THRESHOLD 0x60
#define NUMBER_OF_CALIBRATION (10)
#define SOFT_DELAY (30)
//#define TIME_BEFORE_ADAPTATION (5000) /* 5s */
//#define TIME_BETWEEN_ADAPTATION (100) /* 100ms */
//#define REFERENCE_TAB_SIZE (3)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t Calibration_pin = 0; 
uint8_t Calibration_all_pin = 0; 
uint32_t AcquisitionValue = 0;
volatile uint32_t Count = 0;
uint8_t ReferenceAdaptation = 0;

uint8_t but_code, LAST_but_code ;
uint32_t count_but,  // время нажатого/отжатого  состояния без изменения == защита от дребезга
REPEAT_count;	// счетчик периода повтора

#define  FIX_time   100

void SoftDelay(void)
{
  Count = SOFT_DELAY;
  while(Count--);
}
/*****************************************************/
//Функция инициализации выводов, к которым подключена клавиатура
/*****************************************************/
void sensing_keyboard_installation(void)
{
  /* Включение тактирования GPIOA и GPIOВ*/
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
	
	
  GPIOA->OTYPER &=~ GPIO_OTYPER_OT_0; //кнопка
  GPIOA->OTYPER &=~ GPIO_OTYPER_OT_1; //кнопка
	GPIOA->OTYPER &=~ GPIO_OTYPER_OT_2; //кнопка
	GPIOA->OTYPER |= GPIO_OTYPER_OT_3;  //конденсатор
	
	GPIOA->OTYPER &=~ GPIO_OTYPER_OT_4; //кнопка
  GPIOA->OTYPER &=~ GPIO_OTYPER_OT_5; //кнопка
	GPIOA->OTYPER &=~ GPIO_OTYPER_OT_6; //кнопка
	GPIOA->OTYPER |= GPIO_OTYPER_OT_7;  //конденсатор
	
	GPIOB->OTYPER &=~ GPIO_OTYPER_OT_0; //кнопка
	GPIOB->OTYPER |= GPIO_OTYPER_OT_1;  //конденсатор
	
	GPIOA->MODER = (GPIOA->MODER  & ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1|GPIO_MODER_MODER2 | GPIO_MODER_MODER3|GPIO_MODER_MODER4 | GPIO_MODER_MODER5|GPIO_MODER_MODER6 | GPIO_MODER_MODER7))\
                | (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1|GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1|GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1|GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1); 
  GPIOA->AFR[0] = (GPIOA->AFR[0] & ~(GPIO_AFRL_AFRL0 | GPIO_AFRL_AFRL1|GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3|GPIO_AFRL_AFRL4 | GPIO_AFRL_AFRL5|GPIO_AFRL_AFRL6 | GPIO_AFRL_AFRL7))\
                     |(3<<(4*0))|(3<<(4*1)|3<<(4*2))|(3<<(4*3)|3<<(4*4))|(3<<(4*5)|3<<(4*6))|(3<<(4*7)); 

	GPIOB->MODER = (GPIOB->MODER  & ~(GPIO_MODER_MODER0 | GPIO_MODER_MODER1))\
                | (GPIO_MODER_MODER0_1 | GPIO_MODER_MODER1_1); 
  GPIOB->AFR[0] = (GPIOB->AFR[0] & ~(GPIO_AFRL_AFRL0 | GPIO_AFRL_AFRL1))\
                     |(3<<(4*0))|(3<<(4*1)); 


	  /* Включение тактирования TSC */
  RCC->AHBENR |= RCC_AHBENR_TSEN;

  /* Configure TSC */
  /* Note: there ain't bit definition in the product map file */
  /* (1) fPGCLK = fHCLK/32, pulse high = 2xtPGCLK,Master, pulse low = 2xtPGCLK */
  /*     Charge transfer cycle will be around 2.5µs */
  /*     TSC_CR_CTPH_0 = 1, TSC_CR_CTPL_0 = 1, TSC_CR_PGPSC_0 = 1, TSC_CR_PGPSC_2 = 1 */
  /*     Max count value = 16383 pulses, TSC_CR_MCV_1 = 1, TSC_CR_MCV_2 = 1 */

  TSC->CR = (5<<12) | (1<<28) | (1<<24) | (6<<5) | (1<<0); 
  TSC->IOHCR &= (uint32_t)(~((1<<0)|(1<<1)|(1<<2)|(1<<3))); 
  TSC->IER = (1<<0); // включить прерывания
  

  NVIC_SetPriority(TS_IRQn, 0);
  NVIC_EnableIRQ(TS_IRQn); 
	
	Calibration_all_pin=0;
	TSC_GROUP_count=0;
	Calibration_pin = 1; 	
	
	bit=0;  //поменять на bit=0
	TSC_GROUP_count=0;
	AcquisitionValue = 0; /* Reset Acquisition value */
	TSC->IOCCR  = (1<<bit);
	TSC->IOSCR  = (1<<3);
	TSC->IOGCSR = (1<<TSC_GROUP_count);
	SoftDelay();/* Wait to discharge sample capacitor before new acquisition */
  TSC->CR |= (1<<1);/* new acquisition, TSC_CR_START = 1 */
	
	count_but=FIX_time ;
}
/*****************************************************/
//Подпрограмма слежения за состоянием кнопок
/*****************************************************/
void sensing_button(void)
{
	static uint32_t NumberOfCalibration=0;
  uint8_t RefIndex=0;
  static uint8_t RefIndexStatic=0;
  //static uint32_t ReferenceTab[REFERENCE_TAB_SIZE];
	static uint32_t Reference[10];
	static uint32_t delta[10];
	static uint8_t i=0; // поменять на i=0
	static uint8_t flag=0;
	
	if(LAST_but_code != but_code && flag) {if(count_but) count_but--; }// считаем время нового кода
					
	if(count_but==0)
	{	// новый код длительный и не = 0
		switch(but_code)//разрешить прерывания передачи 1-го одиночного
		{
			case 0x00: USART1->TDR=0x40; break;
			case 0x01: USART1->TDR=0x08; break;
			case 0x02: USART1->TDR=0x20; break;
			case 0x04: USART1->TDR=0x04; break;	 
			case 0x05: USART1->TDR=0x10; break;
			case 0x06: USART1->TDR=0x02; break;
			case 0x09: USART1->TDR=0x01; break;
		}
		flag=0;
		count_but=FIX_time; 
		LAST_but_code= but_code;
	}	
	
		if(AcquisitionValue) /* check if there is a new acquisition value */
    {
      if(Calibration_all_pin) /* check if the calibration is done */
      {
        if((AcquisitionValue + THRESHOLD) < Reference[i]) /* Touch detected */
        {
					//delta[i]=(Reference[i]-AcquisitionValue)/2;

						but_code=bit;	
            flag=1;
					  REPEAT_count=FIX_time; 
        }
        else if(AcquisitionValue > (Reference[i] + THRESHOLD)) /* Need recalibration */
        {		
          Calibration_all_pin = 0;/* restart calibration */
          Reference[i] = 0;/* Reset reference */
        } 
        else /* no touch detected */
        { 
					if(REPEAT_count)REPEAT_count--;
					if(REPEAT_count==0)
					{
						flag=0;
						LAST_but_code=0xFF;
					}
					
          /* Reference adaptation */
//          if(ReferenceAdaptation)
//          {
//            ReferenceAdaptation=0;
//            RefIndexStatic%=REFERENCE_TAB_SIZE;
//            ReferenceTab[RefIndexStatic++] = AcquisitionValue;
//            
//            for(RefIndex=0;RefIndex<REFERENCE_TAB_SIZE;RefIndex++)
//            {
//               Reference[bit] += ReferenceTab[RefIndex];
//            }
//            Reference[bit] /= (REFERENCE_TAB_SIZE + 1);
//          }
        }
				i++;
				if(i==7)i=0; //поменять на i=0
      }
      else /* Calibration */
      {
        if(NumberOfCalibration < NUMBER_OF_CALIBRATION)
        {
          Reference[i] += AcquisitionValue;
          NumberOfCalibration++;					
        }
        else if(NumberOfCalibration == NUMBER_OF_CALIBRATION)
        {
          Reference[i] += AcquisitionValue;
          Reference[i] /= (NUMBER_OF_CALIBRATION + 1); /* Compute reference */
          NumberOfCalibration = 0; /* Reset number of calibration for nex time */
					
          Calibration_pin ++; /* Calibration Completed */
          
//          /* Fill reference table */
//          for(RefIndex=0;RefIndex<REFERENCE_TAB_SIZE;RefIndex++)
//          {
//             ReferenceTab[RefIndex] = Reference[bit];
//          }
						if(Calibration_pin == 8 )
						{
							Calibration_all_pin = 1;
							bit=-1; // поменять на bit=-1
							TSC_GROUP_count=0;
							TSC->IOSCR  = (1<<3);
							TSC->IOGCSR = (1<<TSC_GROUP_count);
							AcquisitionValue = 0; /* Reset Acquisition value */
						}
					
					i++;
					if(i==7)i=0; // поменять на i=0
        }
      }
			

				
			
		if(!Calibration_all_pin)
		{
			if(Calibration_pin == bit+1) bit++;
		}
		else 
		{			
			if(bit == 9)
			{
				bit=-1; // поменять на bit=-1
				TSC_GROUP_count=0;
				TSC->IOSCR  = (1<<3);
				TSC->IOGCSR = (1<<TSC_GROUP_count);
			}
			bit++;
		}
			
			
			if(bit == PIN_CAPACITOR_1) 
			{
				bit++; 
				Calibration_pin ++;
				TSC_GROUP_count++;
				TSC->IOGCSR = (1<<TSC_GROUP_count);
				TSC->IOSCR  = (1<<7);
			}
			
		
			if(bit == PIN_CAPACITOR_2 ) 
			{
				bit=9;
				TSC_GROUP_count++;
				TSC->IOGCSR = (1<<TSC_GROUP_count);
				TSC->IOSCR  = (1<<10);
			}
		
			TSC->IOCCR = (1<<bit);
			
      AcquisitionValue = 0; /* Reset Acquisition value */
      SoftDelay();/* Wait to discharge sample capacitor before new acquisition */
      TSC->CR |= (1<<1);/* new acquisition, TSC_CR_START = 1 */
    }
  }

void TS_IRQHandler(void)
{
  /* End of acquisition flag */
  if((TSC->ISR & (1<<0)) == (1<<0)) /* TSC_ISR_EOAF */
  {
    TSC->ICR = (1<<0);/* Clear flag, TSC_ICR_EOAIC = 1 */
		
		AcquisitionValue = TSC->IOGXCR[TSC_GROUP_count];
  }
}



