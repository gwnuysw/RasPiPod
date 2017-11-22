/*
 * WifiTest.c
 *
 * Created: 2016-04-01 ?�후 5:43:33
 *  Author: dogu
 */ 

 // wifi Client mode Test 
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "debug.h"
#include "Esp8266.h"
#include "timer.h"
#include "CDSsensor.h"
#include "adc.h"
#include "sensor.h"
//#include "soilpart.h"
//#define ON	1
//#define OFF	0

//#define CLOCKWISE	1			// CW
//#define COUNTERCLOCKWISE	0	// CCW

#define SERVER_IP_STR	"192.168.1.33"   //��������� �Ѱ� �����ؾ��� �κ� 
#define SERVER_PORT		50001
#define RED		0					
#define GREEN	1				


unsigned int cds_gun_AdcValue = 0;//-------------v�����κ�
//unsigned int soil_gun_AdcValue = 0;
volatile unsigned short temp;
volatile unsigned short humi;
extern int n_flag_getADC;//--------------^�����κ�

static void eventCallback(int eventType,uint8_t* rxBuff, int rxSize)
{
	if (eventType == EVENT_RX_DATA) 
	{
		debugprint("\r\nRX:%s\r\n", rxBuff);
	}
}


int main(void)
{
	uint8_t strTemp[256];
	unsigned char tempc, humic;   //��ȯ�� �µ� ���� ���� �ϴ� ����
	
	debugInit();
	wifiInit();
	
	sei();
		
	debugprint("wifi test start\r\n");
	_delay_ms(1000);
	
	wificlearUartBuff();
	
	// UART echo disable 
	wifiEchoDisable();
	wificlearUartBuff();
	
	// register callback function 
	wifiSetEventCallback(eventCallback);
	
	// Auto connection disable
	wifiAutoConnectDisable();
	wificlearUartBuff();
	
	wifiModeSet();
	
	wifiEnableDHCP();
	
	debugprint("\r\nAP list display:\r\n");
	wifiDisplayAPlist();
	
	// connect  AP 
	debugprint("\r\n");
	if ( !wifiConnectAP("CSNET-301", "gwnucomse"))
	{
		debugprint("AP connected.\r\n");
	}
	else
	{
		debugprint("AP connection fail.\r\n");
	}
	// display  local allocated IP 
	debugprint("\r\n Allocated local IP:\r\n");
	wifiDisplayLocalIP();
	debugprint("\r\n");

	// TCP remote server connection
	if( !wifiConnectTCPServer(SERVER_IP_STR,SERVER_PORT))
	{
		debugprint("Connected Server.\r\n");
	}
	else
	{
		debugprint("Server connection fail.\r\n");
	}				
	debugprint("\r\n");
	
	// Send Test message 
	strcpy(strTemp,"test msessage: hello\r\n");
//	wifiSendData(strTemp, sizeof("test msessage: hello\r\n"));
	
	 //���⼭���� ���� �����Ͱ� �����ȴ�. 
	TIMER_100mSInit ();
	setElapsedTime100mSUnit(100);

	debugprint("start Loop\r\n");	
	
	int counter = 0;
/*----------------led�κ�����*/
	debugInit();
	LED_Init ();		// PORTB = GREEN, PORTE = RED------------------------v���� �κ� LED CDS ����
	AdcInit(1);			// PORTF = ADC1
//	AdcInit(0);			// PORTF = ADC0
	TIMER_Init();
		
	sei();//-------------------------------------------^�����κ�
	
	SHT11_Init();  //humi temp ����
	
    while(1)
    {//TODO:: Please write your application code 
		
		temp = get_SHT11_data (TEMP);	
		
		humi = get_SHT11_data (HUMI); 	
		
		tempc = printValue (TEMP, temp);
		humic = printValue (HUMI, humi);
		
	/*--------led�κ� ����*/
		if(n_flag_getADC == 1)			//---------------------------------v���� ����, led���ۼ����κ�
		{
			n_flag_getADC = 0;
			
			cds_gun_AdcValue	=	AdcRead();		//�������߿�!!!!!!
	//		soil_gun_AdcValue	=	AdcRead();
			//			debugprint("gun_AdcValue : %d\r\n", gun_AdcValue);
		}
		
		
		
		
		//debugprint("gun_AdcValue : %d\r\n", gun_AdcValue);
		
		
		/*if(soil_gun_AdcValue<=50) {//----------------------------soil value
			Motor_Action(ON, COUNTERCLOCKWISE);
			_delay_ms(1000);

			Motor_Action(OFF, COUNTERCLOCKWISE);
			_delay_ms(3000);

			Motor_Action(ON, CLOCKWISE);
			_delay_ms(1000);
			
			Motor_Action(OFF, CLOCKWISE);
			_delay_ms(10000);
		}
		else if(soil_gun_AdcValue > 50) {
			continue;
		}//---------------------------------------------soil*/
		
		
		if (cds_gun_AdcValue < 640 && cds_gun_AdcValue >= 600)
		{	controlLED (GREEN, 0x80);	controlLED (RED, 0x00);	}
		else if (cds_gun_AdcValue < 600 && cds_gun_AdcValue >= 560)
		{	controlLED (GREEN, 0xC0);	controlLED (RED, 0x00);	}
		else if (cds_gun_AdcValue < 560 && cds_gun_AdcValue >= 520)
		{	controlLED (GREEN, 0xE0);	controlLED (RED, 0x00);	}
		else if (cds_gun_AdcValue < 520 && cds_gun_AdcValue >= 480)
		{	controlLED (GREEN, 0xF0);	controlLED (RED, 0x00);	}
		else if (cds_gun_AdcValue < 480 && cds_gun_AdcValue >= 440)
		{	controlLED (GREEN, 0xF8);	controlLED (RED, 0x00);	}
		else if (cds_gun_AdcValue < 440 && cds_gun_AdcValue >= 400)
		{	controlLED (GREEN, 0xFC);	controlLED (RED, 0x00);	}
		else if (cds_gun_AdcValue < 400 && cds_gun_AdcValue >= 360)
		{	controlLED (GREEN, 0xFE);	controlLED (RED, 0x00);	}
		else if (cds_gun_AdcValue < 360 && cds_gun_AdcValue >= 320)
		{	controlLED (GREEN, 0xFF);	controlLED (RED, 0x00);	}
		else if (cds_gun_AdcValue < 320 && cds_gun_AdcValue >= 280)
		{	controlLED (GREEN, 0xFF);	controlLED (RED, 0x80);	}
		else if (cds_gun_AdcValue < 280 && cds_gun_AdcValue >= 240)
		{	controlLED (GREEN, 0xFF);	controlLED (RED, 0xC0);	}	
		else if (cds_gun_AdcValue < 240 && cds_gun_AdcValue >= 200)
		{	controlLED (GREEN, 0xFF);	controlLED (RED, 0xE0);	}
		else if (cds_gun_AdcValue < 200 && cds_gun_AdcValue >= 160)
		{	controlLED (GREEN, 0xFF);	controlLED (RED, 0xF0);	}
		else if (cds_gun_AdcValue < 160 && cds_gun_AdcValue >= 120)
		{	controlLED (GREEN, 0xFF);	controlLED (RED, 0xF8);	}
		else if (cds_gun_AdcValue < 120 && cds_gun_AdcValue >= 80)
		{	controlLED (GREEN, 0xFF);	controlLED (RED, 0xFC);	}
		else if (cds_gun_AdcValue < 80 && cds_gun_AdcValue >= 40)
		{	controlLED (GREEN, 0xFF);	controlLED (RED, 0xFE);	}
		else if (cds_gun_AdcValue < 40 && cds_gun_AdcValue >= 0)
		{	controlLED (GREEN, 0xFF);	controlLED (RED, 0xFF);	}
		else
		{	controlLED (GREEN, 0x00);	controlLED (RED, 0x00);	}//-------------------------^�����κ�

		//������ ���� �κ�
        
		wifiMain();
		
		if ( isElapsed())
		{
			sprintf(strTemp,"CDS : %d temp : %d humi : %d \n", cds_gun_AdcValue, tempc, humic);		//���� �ٽ����� �κ� 
			//sprintf(strTemp,"CDS : %d temp : %d humi : %d soil : %d \n", cds_gun_AdcValue, tempc, humic, soil_gun_AdcValue);		//���� �ٽ����� �κ� 
			wifiSendData(strTemp, strlen(strTemp));			//���� �ٽ����� �κ�  
			debugprint("TX:%s\r\n",strTemp);				//���� �ٽ����� �κ�  
		}
		
    }
}
