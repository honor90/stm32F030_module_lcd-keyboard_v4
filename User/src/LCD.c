#include "stm32f0xx.h"
#include "LCD.h"
#include "hd44780_driver.h"
#include "stdio.h"
struct data_lcd  *p_lcd;
	 uint8_t ls[64]={0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xa4,  /*�, �, �, �, �, �, �, �*/
                   0xa5,0xa6,0x4B,0x02,0x4D,0x48,0x4F,0x03,  /*�, �, �, �, �, �, �, �*/
                   0x50,0x43,0x54,0xa9,0xaa,0x58,0xe1,0xab,  /*�, �, �, �, �, �, �, �*/
                   0xac,0xe2,0xAD,0xae,0x06,0xAf,0xB0,0xB1,  /*�, �, �, �, �, �, �, �*/
                   0x61,0xb2,0xB3,0xb4,0xE3,0x65,0xB6,0xB7,  /*�, �, �, �, �, �, �, �*/
                   0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,  /*�, �, �, �, �, �, �, �*/
                   0x70,0x63,0xBF,0x79,0x04,0x78,0xE5,0xC0,  /*�, �, �, �, �, �, �, �*/
		               0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7}; /*�, �, �, �, �, �, �, �*/
/*****************************************************/
//������� ������������ ������� � ������� �����
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

		        send_text((unsigned char *)p_lcd->strings_1, 1, 0);
		        send_text((unsigned char *)p_lcd->strings_2, 2, 0);	
					}
			}
		}
}
/*****************************************************/
//������� �������� ����� �������
/*****************************************************/
void lcd_clr(void)
{
			command(0x01);
			command(0x0C);
}	
/*****************************************************/
//��������� ������� �� ������ ������
/*****************************************************/
void Cursor(uint8_t cursor)
{
	if(cursor <= 19)
	{
		command(0x0f); 
		command(0xC0+cursor);
	}
	else command(0x0C);
}
/*****************************************************/
//������� ������ ������� �� ������� � ������ ��������� � ��������� �������
/*****************************************************/
void send_text(uint8_t * txt,uint8_t  st, uint8_t poz) 
{  uint8_t var; 
	 uint8_t n;//��������� �������
	 
									 
									 
//������ � � � �(��� �������), ��� "��", ���� ��������� 
//� ����������� ������� ��������� �, � �������� ������
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
//������� { � } �������� �� ������� � ��������� ����� � ��������������		
//		switch (var)
//		{
//			case 0x7B:
//				var = 0xB1;
//			break;
//			case 0x7D:
//				var = 0xC7;
//			break;
//		}
		send_data(var);
		txt++;
	}
}
/*****************************************************/
//������������ ��� ������ �� �������
/*****************************************************/
void record_strings(void)
{
	uint8_t i=0;	
	
//		     if(pLcd_st->flag_run_sr1)
//		     {
//					send_text((unsigned char *)pLcd_st->strings_2, 2, pLcd_st->cursor_2); 
//			    pLcd_st->num_strings=0x01;
//			    pLcd_st->side=RIGTH;					 
//			    circle_text(pLcd_st);
//		     }	
	
			 	 if(p_lcd->flag_out_str1) //����� ������ ������
				 {
					 //lcd_clr();//�������� �������
					 if((p_lcd->side == RIGTH) | (p_lcd->side == LEFT)) circle_text();//������� ������� ������

					 //������� �� �������
					  
					 send_text((unsigned char *)p_lcd->strings_1_new, 1, 0); 
					 send_text((unsigned char *)p_lcd->strings_2, 2, 0);
					 Cursor(p_lcd->cursor_2);
					 
					 //���������� �������� ����� ������ � ����� ��� ������
           for(i =0; i < p_lcd->len_new-1; i++) p_lcd->strings_1[i] = p_lcd->strings_1_new[i];
           for(i = p_lcd->len_new-1; i < ID_MAX_STR; i++) p_lcd->strings_1[i] = 0x90;					 
           for(i=0; i < ID_MAX_NEW; i++) p_lcd->strings_1_new[i] = 0x90;	
				   
           p_lcd->len_str_1 = p_lcd->len_new;
           //pLcd_st->cursor_1 = pLcd_st->cursor_new;
           //p_lcd->cursor_new = 0;
           p_lcd->side=0x00;
					 
					 p_lcd->flag_out_str1=0;
				 }
				 
			 	 if(p_lcd->flag_out_str2) //����� ������ ������
				 {		 
					 //lcd_clr();//�������� �������
					 if((p_lcd->side == RIGTH) | (p_lcd->side == LEFT)) circle_text();//������� ������� ������
					 
					 //������� �� �������
           send_text((unsigned char *)p_lcd->strings_1, 1, 0);
					 send_text((unsigned char *)p_lcd->strings_2_new, 2, 0);
					 Cursor(p_lcd->cursor_2);
					 
					 //���������� �������� ����� ������ � ����� ��� ������
           for(i =0; i < p_lcd->len_new-1; i++) p_lcd->strings_2[i] = p_lcd->strings_2_new[i];
           for(i = p_lcd->len_new-1; i < ID_MAX_STR; i++) p_lcd->strings_2[i] = 0x90;
           for(i=0; i < ID_MAX_NEW; i++) p_lcd->strings_2_new[i] = 0x90;
            					 
           p_lcd->len_str_2 = p_lcd->len_new;
           //pLcd_st->cursor_2 = pLcd_st->cursor_new;
           //p_lcd->cursor_new = 0;
           p_lcd->side=0x00;
						
					 p_lcd->flag_out_str2=0;
				 } 
}
/*****************************************************/
//������� ������ ������� ������
/*****************************************************/
void circle_text(void)
{
	 uint8_t i;
	 uint8_t n;//��������� �������
	 uint8_t count;
	 uint8_t k;

switch(p_lcd->num_strings) //����������� ������ ������
{ 
/*****************************************************/
//����� 1-� ������
/*****************************************************/
case 0x01 : 
//��������� �������	
n=0x80;             
//if(pLCD->cursor_new>19) pLCD->cursor_new=19;
//n+=pLCD->cursor_new;


for (i=0; i < ID_MAX_VIDEO_1; i++) p_lcd->video_buff_1[i] = 0x90;	

switch (p_lcd->side)	//����������� ����������� ������
{ 
/*****************************************************/
//����� ������   
/*****************************************************/	
case RIGTH:	
//if(p_lcd->flag_run_sr1)	//����� ������� ������ ������������
//{for (i=0;	i < 20; i++) p_lcd->video_buff_1[i] = 0x90;	
//k=0;
//for (i=20; i < 20+p_lcd->len_str_run; i++)	{p_lcd->video_buff_1[i] = p_lcd->run_string[k];	k++;}       
//for(count=0; count < MAX_VIDEO_1; count++){        
//send_text((unsigned char *)p_lcd->strings_2, 2, 0);	
//command(n);	
//	command(0x0C);	
//	for(i=count; i < ID_MAX_VIDEO_1; i++)       
//	{          
//		if(p_lcd->video_buff_1[i]  >= 0xC0)     
//		{
//  		p_lcd->video_buff_1[i] -=0xC0;
//		  p_lcd->video_buff_1[i]=ls[p_lcd->video_buff_1[i]];
//		}
////������� { � } �������� �� ������� � ��������� ����� � ��������������
//		switch (p_lcd->video_buff_1[i])
//		{
//			case 0x7B:
//				p_lcd->video_buff_1[i] = 0xB1;
//			break;
//			case 0x7D:
//				p_lcd->video_buff_1[i] = 0xC7;
//			break;
//		}
//		send_data(p_lcd->video_buff_1[i]);//������� ������ �� �������
//	}
//	
//  delay(DELAY_CIRCLE);
//}	
//}	
//else {
	//����� ������ ������	

//���������� ����� � ������ ������ � ������ 
for (i=0; i < p_lcd->len_str_1-1; i++) p_lcd->video_buff_1[i] = p_lcd->strings_1[i];	
k=19;
for(i=0;	i < p_lcd->len_new-1; i++) {p_lcd->video_buff_1[k] = p_lcd->strings_1_new[i];	k++;} 	
//����� ������ �� �������
for(count=0; count<MAX_LEN_LCD; count++){
send_text((unsigned char *)p_lcd->strings_2, 2, 0);
  command(n);	
	command(0x0C);	
	for(i=count; i < ID_MAX_VIDEO_1; i++)
	{
		if(p_lcd->video_buff_1[i]  >= 0xC0)
		{
  		p_lcd->video_buff_1[i] -=0xC0;
		  p_lcd->video_buff_1[i]=ls[p_lcd->video_buff_1[i]];
		}
//������� { � } �������� �� ������� � ��������� ����� � ��������������
		switch (p_lcd->video_buff_1[i])
		{
			case 0x7B:
				p_lcd->video_buff_1[i] = 0xB1;
			break;
			case 0x7D:
				p_lcd->video_buff_1[i] = 0xC7;
			break;
		}
		send_data(p_lcd->video_buff_1[i]);//������� ������ �� �������
	}
  delay(DELAY_CIRCLE);		
}
//}
break;

/*****************************************************/
//����� �����
/*****************************************************/
case LEFT:

//���������� ����� � ������ ������ � ������ 	
for (i=0;	i < p_lcd->len_new-1; i++) p_lcd->video_buff_1[i] = p_lcd->strings_1_new[i];	
k=0;
for (i=21; i < 21+p_lcd->len_str_1-1; i++)	{p_lcd->video_buff_1[i] = p_lcd->strings_1[k];	k++;}
i = 0;
//����� ������ �� �������
for(count=MAX_LEN_LCD; count>0; count--){
send_text((unsigned char *)p_lcd->strings_2, 2, 0);	
command(n);
command(0x0C);		
	for(i=count; i < ID_MAX_VIDEO_1; i++)
	{
		if(p_lcd->video_buff_1[i]  >= 0xC0)
		{
  		p_lcd->video_buff_1[i] -=0xC0;
		  p_lcd->video_buff_1[i]=ls[p_lcd->video_buff_1[i]];
		}
//������� { � } �������� �� ������� � ��������� ����� � ��������������
		switch (p_lcd->video_buff_1[i])
		{
			case 0x7B:
				p_lcd->video_buff_1[i] = 0xB1;
			break;
			case 0x7D:
				p_lcd->video_buff_1[i] = 0xC7;
			break;
		}
		send_data(p_lcd->video_buff_1[i]);//������� ������ �� �������
	}
	
  delay(DELAY_CIRCLE);
}	
break;
}
break;

/*****************************************************/
//����� 2-� ������
/*****************************************************/
case 0x02 : 
//command(0x0C);
//��������� �������	
n=0xC0; 
//if(pLCD->cursor_new>19) pLCD->cursor_new=19;
//n+=pLCD->cursor_new;	

for (i=0; i < ID_MAX_VIDEO_2; i++) p_lcd->video_buff_2[i] = 0x90;	

switch (p_lcd->side)	
{ 
/*****************************************************/
//����� ������
/*****************************************************/	
case RIGTH:	
//���������� ����� � ������ ������ � ������ 	
for (i=0; i < p_lcd->len_str_2-1; i++) p_lcd->video_buff_2[i] = p_lcd->strings_2[i];	
k=19;
for(i=0;	i < p_lcd->len_new-1; i++) {p_lcd->video_buff_2[k] = p_lcd->strings_2_new[i];	k++;} 	
//����� ������ �� �������
for(count=0; count<MAX_LEN_LCD; count++){
send_text((unsigned char *)p_lcd->strings_1, 1, 0);
command(n);	
	command(0x0C);	
	for(i=count; i < ID_MAX_VIDEO_2; i++)
	{
		if(p_lcd->video_buff_2[i]  >= 0xC0)
		{
  		p_lcd->video_buff_2[i] -=0xC0;
		  p_lcd->video_buff_2[i]=ls[p_lcd->video_buff_2[i]];
		}
//������� { � } �������� �� ������� � ��������� ����� � ��������������
		switch (p_lcd->video_buff_2[i])
		{
			case 0x7B:
				p_lcd->video_buff_2[i] = 0xB1;
			break;
			case 0x7D:
				p_lcd->video_buff_2[i] = 0xC7;
			break;
		}
		send_data(p_lcd->video_buff_2[i]);//������� ������ �� �������
		
	}
  delay(DELAY_CIRCLE);	
}
break;

/*****************************************************/
//����� �����
/*****************************************************/
case LEFT:
//���������� ����� � ������ ������ � ������ 	
for (i=0;	i < p_lcd->len_new-1; i++) p_lcd->video_buff_2[i] = p_lcd->strings_2_new[i];	
k=0;
for (i=21; i < 21+p_lcd->len_str_2-1; i++)	{p_lcd->video_buff_2[i] = p_lcd->strings_2[k];	k++;}
//����� ������ �� �������	
i = 0;
for(count=MAX_LEN_LCD; count>0; count--){
send_text((unsigned char *)p_lcd->strings_1, 1, 0);	
command(n);	
command(0x0C);	
	for(i=count; i < ID_MAX_VIDEO_2; i++)
	{
		if(p_lcd->video_buff_2[i]  >= 0xC0)
		{
  		p_lcd->video_buff_2[i] -=0xC0;
		  p_lcd->video_buff_2[i]=ls[p_lcd->video_buff_2[i]];
		}
//������� { � } �������� �� ������� � ��������� ����� � ��������������
		switch (p_lcd->video_buff_2[i])
		{
			case 0x7B:
				p_lcd->video_buff_2[i] = 0xB1;
			break;
			case 0x7D:
				p_lcd->video_buff_2[i] = 0xC7;
			break;
		}
		send_data(p_lcd->video_buff_2[i]);//������� ������ �� �������
	}
	
  delay(DELAY_CIRCLE);	
}
break;
}
break;
}

}	

