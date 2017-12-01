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
#include "soilpart.h"
#define ON	1
#define OFF	0

#define CLOCKWISE	1			// CW
#define COUNTERCLOCKWISE	0	// CCW

#define SERVER_IP_STR	"192.168.1.42"   //��������� �Ѱ� �����ؾ��� �κ� 
#define SERVER_PORT		50001
			


//-------------v�����κ�
unsigned int soil_gun_AdcValue = 0;
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
/*	while(wifiConnectTCPServer(SERVER_IP_STR,SERVER_PORT))
	{
		debugprint("Server connection fail.\r\n");
		
	}
	debugprint("Connected Server.\r\n");*/
	
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

	AdcInit(0);			// PORTF = ADC0   ������
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
			
			soil_gun_AdcValue	=	AdcRead();
		//	debugprint("soil_gun_AdcValue : %d\r\n", soil_gun_AdcValue);
		}
		
		Motor_Action(ON, COUNTERCLOCKWISE);
		
		if(soil_gun_AdcValue<=30) {//----------------------------soil value
			
			Motor_Action(ON, CLOCKWISE);
			_delay_ms(250);
			
			Motor_Action(OFF, CLOCKWISE);
			_delay_ms(15000);
			
			
	/*		Motor_Action(OFF, CLOCKWISE);
			_delay_ms(15000);
			
			Motor_Action(ON, COUNTERCLOCKWISE);
			_delay_ms(500);

			Motor_Action(OFF, COUNTERCLOCKWISE);
			_delay_ms(15000);
			
		//	_delay_ms(10000);*/
		}
		
		wifiMain();
		
		if ( isElapsed())
		{
			sprintf(strTemp,"soil : %d temp : %d humi : %d \n", soil_gun_AdcValue, tempc, humic);		//���� �ٽ����� �κ� 
			//sprintf(strTemp,"CDS : %d temp : %d humi : %d soil : %d \n", cds_gun_AdcValue, tempc, humic, soil_gun_AdcValue);		//���� �ٽ����� �κ� 
			wifiSendData(strTemp, strlen(strTemp));			//���� �ٽ����� �κ�  
			debugprint("TX:%s\r\n",strTemp);				//���� �ٽ����� �κ�  
		}
		
    }
}
