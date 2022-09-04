#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "stm32f0xx.h"

/*****************************************************/
//
/*****************************************************/
#define button_up       (1<<0)    /* ������ ����� */
#define button_down     (1<<1)    /* ������ ���� */
#define button_right    (1<<2)    /* ������ ������ */
#define button_left     (1<<3)    /* ������ ����� */
#define button_ok       (1<<4)    /* ������ �� */
#define button_esc      (1<<5)    /* ������ ESC */
/*****************************************************/
//��������� ��� �������� ������� ��������� ������
/*****************************************************/
struct button_old{
uint8_t up;                       /* ��������� ������ ����� */
uint8_t down;                     /* ��������� ������ ���� */
uint8_t right;	                  /* ��������� ������ ������ */
uint8_t left;                     /* ��������� ������ ����� */
uint8_t ok;                       /* ��������� ������ �� */	
uint8_t esc;                      /* ��������� ������ ESC */	
};

void keyboard_installation(struct button_old *p_but);
void button(uint8_t but, uint8_t *p_but_old);

#endif
