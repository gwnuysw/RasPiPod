/********************************************************************

 �� �� : lcd.h

 �ҽ� ���� :
 1) ����� ���� ������� ����
 2) Delay ���� ������� ����
 3) ���ͷ�Ʈ ���� ������� ����                                                
 4) GPIO ���� ��������� ����
 5) TEMP ��Ʈ �� �� ����
 6) ACK/ NOACK ��� �� ����
 7) Addr	Code(Command)	r/w
	000		0001  			1
	000		0010  			1
	000		0011  			1
	000		0011  			0
	000		1111  			0
 8) �µ��� ������ ��Ÿ���� ���ڿ� ���� ����
 9) ��/���� ������ I2C ���� �Լ��� ������Ÿ���� ����
 
********************************************************************/

#ifndef	__LIB_SENSOR_H__
#define	__LIB_SENSOR_H__

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "gpio.h"

#define TEMP_PORT		F
#define SCK		5
#define DATA	2

#define NOACK	0
#define ACK   	1

// SHT11 I2C Command
                            		// Addr	    Code(command)   r/w
#define MEASURE_TEMP		0x03	// 000   	0001  			1
#define MEASURE_HUMI		0x05	// 000   	0010  			1
//#define READ_STATUS_REG	0x07	// 000   	0011  			1
//#define WRITE_STATUS_REG	0x06	// 000   	0011  			0
//#define RESET				0x1e   	// 000   	1111  			0

enum { TEMP, HUMI };

void SHT11_Init (void);
void Connection_reset (void);
void Transmission_start (void);
unsigned short get_SHT11_data (unsigned char type);
unsigned char Write_byte (unsigned char value);
unsigned char Read_byte (unsigned char ack);
unsigned char Measure (unsigned short *p_value, 
						unsigned short *p_checksum,	unsigned char mode);
void calc_SHT11 (unsigned short p_humidity ,unsigned short p_temperature);

#endif	/* __LIB_SENSOR_H__ */
