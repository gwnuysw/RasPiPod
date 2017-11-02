/********************************************************************

 �� �� : sensor.c

 �ҽ� ���� :
 1) ���� ���� ��� ���� ����
 2) ���� ���� ��� �ʿ��� ���� ����
 3) ��Ʈ ����� ���� �� ��� ���� �Լ� ����
 4) SHT11 ������ TWI ����� ���� �ʱ�ȭ
 5) Connection reset
 6) Transmission start(���� ���� ��ȣ)
 7) ��/���� ������ ȹ��
  - ���� ��ġ ����
  - �µ� ��ġ ����
  - ���� �߻� ��, Connection reset
  - ������ ���� �������� ��/���� �� ���
 8) TWI ����� �̿��� ��/���� ����
  - Transmission start
  - �µ� ��û ��ȣ �۽�(���� ����̽� �ּ� �� �µ� Read  ���)
  - ���� ��û ��ȣ �۽�(���� ����̽� �ּ� �� ���� Read  ���)
  - ���� ���
  - ������ ����
 9) ������ �۽�
 10) ������ ����
 11) ��/���� �� ��� �Լ�(�����κ��� ������ ���� ��ȯ�� �ʿ�)

********************************************************************/

#include "sensor.h"

unsigned short val_temp, val_humi;
unsigned short SHT11_humi, SHT11_temp;
unsigned short error, checksum;
unsigned char sensing_type;


static void I2C_data_output (void)
{	pinMode(TEMP_PORT, DATA, OUTPUT);}
static void I2C_data_input()
{	pinMode(TEMP_PORT, DATA, INPUT);}
static void I2C_sck_output (void)
{	pinMode(TEMP_PORT, SCK, OUTPUT);}
static void I2C_sck_input (void)
{	pinMode(TEMP_PORT, SCK, INPUT);}
static void SET_DATA (void)
{	digitalWrite(TEMP_PORT, DATA, HIGH);}
static void CLR_DATA (void)
{	digitalWrite(TEMP_PORT, DATA, LOW);}
static int READ_DATA (void)
{	return digitalRead(TEMP_PORT, DATA);}
static void SET_SCK()
{	digitalWrite(TEMP_PORT, SCK, HIGH);}
static void CLR_SCK()
{	digitalWrite(TEMP_PORT, SCK, LOW);}
static int READ_I2CSCK_PIN()
{	return digitalRead(TEMP_PORT, SCK);}


void SHT11_Init (void)
{
	I2C_data_output ();		// DDRF |= 0x02;
	I2C_sck_output ();		// DDRF |= 0x01;
	Connection_reset ();
}

void Connection_reset (void)
{
	unsigned char i; 
  	SET_DATA();				// Initial state
	CLR_SCK();				// Initial state
  	for (i=0; i<9; i++)		// 9 SCK cycles
  	{
  		SET_SCK();
    	CLR_SCK();
  	}
  	Transmission_start();	// transmission start
}

void Transmission_start (void)
{  
	SET_DATA();                   //Initial state
	CLR_SCK();                    //Initial state
	asm("nop");asm("nop");

	SET_SCK();
	asm("nop");asm("nop");

	CLR_DATA();
	asm("nop");asm("nop");

	CLR_SCK();  
	asm("nop");asm("nop");

	SET_SCK();
	asm("nop");asm("nop");

	SET_DATA();		   
	asm("nop");asm("nop");

	CLR_SCK();
}

unsigned short get_SHT11_data (unsigned char type)
{
	sensing_type	=	type;

	error	=	0;
	// measure humidity
	error	+=	Measure (&SHT11_humi, &checksum, HUMI);
	// measure temperature
	error	+=	Measure (&SHT11_temp, &checksum, TEMP);

	if (error != 0)		// [Error] connection reset
		Connection_reset ();
	else				// Calculate humidity, temperature
		calc_SHT11 (SHT11_humi, SHT11_temp);

	if (sensing_type == HUMI)		return	val_humi;
	else if (sensing_type == TEMP)	return	val_temp;
	else 							return	0;
}

unsigned char Measure (unsigned short *p_value, unsigned short *p_checksum,
			unsigned char mode)
{ 
	unsigned short error	=	0;
	unsigned short SHT11_msb, SHT11_lsb;

	Transmission_start ();		//transmission start
	switch (mode)				//send command to sensor
	{
		case TEMP :
			error	+=	Write_byte (MEASURE_TEMP);
			break;
		case HUMI :
			error	+=	Write_byte (MEASURE_HUMI);
			break;
		default :
			break;	 
	}
	if (error != 0)
		return	error;

	I2C_data_input ();

	while (READ_DATA());

	I2C_data_input();

	SHT11_msb	=	Read_byte (ACK);		// read the first byte (MSB)
	SHT11_lsb	=	Read_byte (ACK);		// read the second byte (LSB)
	*p_value	=	(SHT11_msb * 256) + SHT11_lsb;
	*p_checksum	=	Read_byte (NOACK);	// read checksum

	return error;
}

unsigned char Write_byte (unsigned char value)
{ 
	unsigned char i, error	=	0;
	I2C_data_output ();
	for (i=0x80; i>0; i/=2)
	{
		if (i & value)	SET_DATA ();
		else		CLR_DATA ();

		SET_SCK ();
		asm("nop");asm("nop");
		CLR_SCK ();
	}
	SET_DATA ();
	I2C_data_input ();
	asm("nop");asm("nop");
	SET_SCK (); 
	error	=	READ_DATA ();

	CLR_SCK ();
	I2C_data_output ();

	return error;
}

unsigned char Read_byte(unsigned char ack)
{ 
	unsigned char i, val	=	0;
	I2C_data_input ();
	SET_DATA();
	asm("nop");asm("nop");

	for (i=0x80; i>0; i/=2)
	{
		SET_SCK();
		asm("nop");asm("nop");
		if (READ_DATA())
			val = (val | i); 
		CLR_SCK();
		asm("nop");asm("nop");
	}
	I2C_data_output();

	if (ack)	CLR_DATA();
	else		SET_DATA();

	SET_SCK();
	asm("nop");asm("nop"); 
	CLR_SCK();
	asm("nop");asm("nop"); 
	SET_DATA();

	return val;
}

void calc_SHT11 (unsigned short humidity, unsigned short temperature)
{ 
	const float C1	=	-4.0; 			// for 12 Bit
	const float C2	=	0.0405; 		// for 12 Bit
	const float C3	=	-0.0000028; 	// for 12 Bit
	const float T1	=	0.01; 			// for 12 Bit
	const float T2	=	0.00008; 		// for 12 Bit
 
	float rh_lin;		// Relative Humidity
	float rh_true; 		// Humidity Sensor RH/Temperature compensation
	float t_C; 			// Temperature
	float rh	=	(float)humidity;
	float t		=	(float)temperature;
    
	t_C			=	((t * 0.01) - 40) - 5;
	rh_lin		=	(C3 * rh * rh) + (C2 * rh) + C1;
	rh_true 	=	(t_C - 25) * (T1 + (T2 * rh)) + rh_lin;

	if (rh_true > 100)	rh_true = 100;
	if (rh_true < 0.1)	rh_true = 0.1;

//	val_temp	=	(unsigned short)(t_C*10);
//	val_humi	=	(unsigned short)(rh_true*10);
	val_temp	=	(unsigned short)(t_C);
	val_humi	=	(unsigned short)(rh_true);
}
