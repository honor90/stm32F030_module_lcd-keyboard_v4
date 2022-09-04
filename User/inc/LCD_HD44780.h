#ifndef LCD_HD44780_H
#define LCD_HD44780_H
#include "stm32f0xx.h"

/*****************************************************/
//
/*****************************************************/
#define MAX_STR         20         /* длина буфера для хранения строки */
#define MAX_NEW         20         /* длина буфера для хранения новой (пришедшей) строки */
#define MAX_VIDEO_1     40         /* длина буфера для вывода 1-й строки */
#define MAX_VIDEO_2     40         /* длина буфера для вывода 2-й строки */
#define MAX_RUN         20         /* длина буфера для хранения бегущей строки */
/*****************************************************/
//
/*****************************************************/
#define ID_MAX_STR      19         /* номер последнего элемента буфера со строкой */
#define ID_MAX_NEW      19         /* номер последнего элемента буфера с новой строкой */
#define ID_MAX_VIDEO_1  39         /* номер последнего элемента буфера для вывода 1-й строки */
#define ID_MAX_VIDEO_2  39         /* номер последнего элемента буфера для вывода 2-й строки */
#define ID_MAX_RUN      19         /* номер последнего элемента буфера с бегущей строкой */

#define MAX_LEN_LCD     21
/*****************************************************/
//
/*****************************************************/
#define COUNT_SEC       60
#define COUNT_MINUTE     1         /* период перезагрузки (в минутах) */
/*****************************************************/
//структура для хранения в ОЗУ данных для вывода на дисплей
/*****************************************************/
struct data_lcd{   
uint8_t strings_1[MAX_STR];        /* Буфер для хранения в ОЗУ 1-й строки */
uint8_t strings_1_new[MAX_NEW];    /* Буфер для хранения новой 1-й строки, пришедшей по USART */	
uint8_t video_buff_1[MAX_VIDEO_1]; /* Буфер для вывода 1-й строки на дисплей */	
uint8_t strings_2[MAX_STR];        /* Буфер для хранения в ОЗУ 2-й строки */
uint8_t strings_2_new[MAX_NEW];    /* Буфер для хранения новой 2-й строки, пришедшей по USART */	
uint8_t video_buff_2[MAX_VIDEO_2]; /* Буфер для вывода 2-й строки на дисплей */		
uint8_t run_string[MAX_RUN];	     /* Буфер для хранения в ОЗУ бегущей строки */
uint8_t len_str_1;                 /* длина 1-й строки, хранимая в ОЗУ */
uint8_t len_str_2;                 /* длина 2-й строки, хранимая в ОЗУ */
uint8_t len_str_run;               /* Длина бегущей строки */
uint8_t len_new;                   /* длина новой строки */
uint8_t side;                      /* Направление вывода бегущей строки */
uint8_t num_strings;               /* Номер пришедшей строки */
uint8_t cursor_new;
uint8_t cursor_1;                  /* Положение курсора для первой строки, хранимое в ОЗУ */
uint8_t cursor_2;                  /* Положение курсора для второй строки, хранимое в ОЗУ */
uint8_t flag_run_sr1;
uint8_t take_data;                 /* Для пришедших данных по usart */
//uint8_t i;//индификатор массива
uint8_t flag_out_str1;	
uint8_t flag_out_str2;	
};
/*****************************************************/
//переменные для осчета времени перезагрузки дисплея
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


