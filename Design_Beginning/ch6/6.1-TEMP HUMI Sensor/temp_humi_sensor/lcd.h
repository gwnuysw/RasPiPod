/********************************************************************

 �� �� : lcd.h

 �ҽ� ���� :
 1) ����� ���� ��������� ����
 2) Delay ���� ��������� ����
 3) PORTB ����
 4) DDRB ����
 5) PORTE ����
 6) DDRE ����

********************************************************************/

#ifndef __LCD_H__
#define __LCD_H__

#include <avr/io.h>
#include <util/delay.h>
#include "gpio.h"

//#define LCD_CD_PORT		PORTK
//#define LCD_CD_DDR		DDRK
//#define LCD_DB_PORT		PORTC
//#define LCD_DB_DDR		DDRC

#define LCD_CD_PORT		K
#define LCD_DB_PORT		C

#define LCD_CD_PIN_NUM		3
#define LCD_DB_PIN_NUM		8

#define RS_PIN	0
#define RW_PIN	1
#define E_PIN	2

void write_Data ( unsigned char data );
void printString ( char *string );
void write_Command ( unsigned char command );
void LCD_Init (int controlPort, int rsPin, int rwPin, int ePin, int dataPort);


#endif	/* __LCD_H__ */