#include "general.h"

static T_TMR_Timer ADC_CheckTimerId = -1;
static int analog_value = 0;

#define ANALOG_PIN_0   34
#define ADC_detect_time   200 //200 ms



void adc_init(void)
{
    pinMode(ANALOG_PIN_0,INPUT);
    
   if(ADC_CheckTimerId == (T_TMR_Timer)-1)
		ADC_CheckTimerId = TMR_TimerAdd(TMR_Cyclic, adc_event, 0);
	/* and start it */
	//TMR_TimerStart(ADC_CheckTimerId, ADC_detect_time);    
}

void adc_handle(void)
{
    //int analog_value = 0;
    analog_value = analogRead(ANALOG_PIN_0);
    //delay(100);
    Serial.printf("Current Reading on Pin(%d)=%d\n",ANALOG_PIN_0,analog_value);
    Serial.println();
}
//================================================================================================
void IRAM_ATTR adc_event(unsigned short usDummyParam)
{
    Event_EventSend(MN_ADC, (void *)NULL);
}
//================================================================================================
void adc_timer_start(void)
{
  TMR_TimerStart(ADC_CheckTimerId, ADC_detect_time);    
}
//================================================================================================
void adc_timer_stop(void)
{
  TMR_TimerStop(ADC_CheckTimerId);
  analog_value = 0;
}
//================================================================================================
int adc_get_analog_value(void)
{
  return analog_value;
}

