#ifdef ESP8266

#include "general.h"

#define TMR_TICK         1000000    /* Timer tick = 1 sec, value in microseconds*/  

Ticker secondTick;

int wifi_reconnect_count = 0;
void BlinkLed_ESP8266(void);
//======================================================================================================================
void Timer0_callback_1s()
{	
	BlinkLed_ESP8266();
	if (wifi_reconnect_count>0) wifi_reconnect_count--;	 
}
void timer_init2(void) {
    secondTick.attach(1,Timer0_callback_1s);
}
//======================================================================================================================
void timer_handle(void) {

}
//================================================================================================
void set_wifi_reconnect_count(uint32_t val)
{
	wifi_reconnect_count = val;
}
//================================================================================================
uint32_t get_wifi_reconnect_count(void)
{
	return wifi_reconnect_count;
}
//================================================================================================
#ifndef ledPin
#define ledPin 2 // GPIO2
#endif

int LED_Active = 0;
//int LED_Status=0;
int LED_Active2 = 0;

void BlinkLed_ESP8266(void)
{
	
    if (0==LED_Active)
    {
       LED_Active = 1;
    }
    else
    {
        LED_Active = 0;
    }

	digitalWrite(ledPin, LED_Active);	
}

#endif // _ESP8266_