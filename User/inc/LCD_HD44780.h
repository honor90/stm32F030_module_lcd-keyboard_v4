#ifndef LCD_HD44780_H
#define LCD_HD44780_H
#include "stm32f0xx.h"

/*****************************************************/
//
/*****************************************************/
#define MAX_STR         20         /* ����� ������ ��� �������� ������ */
#define MAX_NEW         20         /* ����� ������ ��� �������� ����� (���������) ������ */
#define MAX_VIDEO_1     40         /* ����� ������ ��� ������ 1-� ������ */
#define MAX_VIDEO_2     40         /* ����� ������ ��� ������ 2-� ������ */
#define MAX_RUN         20         /* ����� ������ ��� �������� ������� ������ */
/*****************************************************/
//
/*****************************************************/
#define ID_MAX_STR      19         /* ����� ���������� �������� ������ �� ������� */
#define ID_MAX_NEW      19         /* ����� ���������� �������� ������ � ����� ������� */
#define ID_MAX_VIDEO_1  39         /* ����� ���������� �������� ������ ��� ������ 1-� ������ */
#define ID_MAX_VIDEO_2  39         /* ����� ���������� �������� ������ ��� ������ 2-� ������ */
#define ID_MAX_RUN      19         /* ����� ���������� �������� ������ � ������� ������� */

#define MAX_LEN_LCD     21
/*****************************************************/
//
/*****************************************************/
#define COUNT_SEC       60
#define COUNT_MINUTE     1         /* ������ ������������ (� �������) */
/*****************************************************/
//��������� ��� �������� � ��� ������ ��� ������ �� �������
/*****************************************************/
struct data_lcd{   
uint8_t strings_1[MAX_STR];        /* ����� ��� �������� � ��� 1-� ������ */
uint8_t strings_1_new[MAX_NEW];    /* ����� ��� �������� ����� 1-� ������, ��������� �� USART */	
uint8_t video_buff_1[MAX_VIDEO_1]; /* ����� ��� ������ 1-� ������ �� ������� */	
uint8_t strings_2[MAX_STR];        /* ����� ��� �������� � ��� 2-� ������ */
uint8_t strings_2_new[MAX_NEW];    /* ����� ��� �������� ����� 2-� ������, ��������� �� USART */	
uint8_t video_buff_2[MAX_VIDEO_2]; /* ����� ��� ������ 2-� ������ �� ������� */		
uint8_t run_string[MAX_RUN];	     /* ����� ��� �������� � ��� ������� ������ */
uint8_t len_str_1;                 /* ����� 1-� ������, �������� � ��� */
uint8_t len_str_2;                 /* ����� 2-� ������, �������� � ��� */
uint8_t len_str_run;               /* ����� ������� ������ */
uint8_t len_new;                   /* ����� ����� ������ */
uint8_t side;                      /* ����������� ������ ������� ������ */
uint8_t num_strings;               /* ����� ��������� ������ */
uint8_t cursor_new;
uint8_t cursor_1;                  /* ��������� ������� ��� ������ ������, �������� � ��� */
uint8_t cursor_2;                  /* ��������� ������� ��� ������ ������, �������� � ��� */
uint8_t flag_run_sr1;
uint8_t take_data;                 /* ��� ��������� ������ �� usart */
//uint8_t i;//����������� �������
uint8_t flag_out_str1;	
uint8_t flag_out_str2;	
};
/*****************************************************/
//���������� ��� ������ ������� ������������ �������
/*****************************************************/
struct time{
uint32_t tic_delay;
uint32_t mili_tic;
uint32_t tic;
uint32_t sec;
uint8_t minute;	
};

extern struct data_lcd  *p_lcd;
extern struct time  *t;


void portInstallation(void);
void HD44780_installation(void);
void installation_rus_alphabet(void);
void reloadHD44780(void);
void send_data(uint8_t data);
void command(uint8_t data);
void send_text(unsigned char * txt,uint8_t  st, uint8_t poz) ;
void record_strings(struct data_lcd * pLcd_st);
void circle_text(struct data_lcd *pLCD);


#endif


