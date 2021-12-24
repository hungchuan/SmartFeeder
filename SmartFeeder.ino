#include "general.h"


//Ryan begin
#ifndef ledPin
#define ledPin 2 // GPIO2
#endif

#ifndef button
#define button 0 //gpio to use to trigger delay
#endif

#ifndef gpio_zero_check
#define gpio_zero_check 15 //gpio to use to trigger delay
#endif

#ifndef gpio_motor
#define gpio_motor 13 //gpio to use to trigger delay
#endif

#ifndef gpio_ir
#define gpio_ir 16 //gpio to use to trigger delay
#endif

enum System_Status
{	 
 restart,
 start,
 feed0,
 feed1,
 feed2,
 feed3,
 feed4,
 feed5,
 feed6,
 turning
};

int e_system_status = restart;
int zero_check_status = 1;
int ir_return_value = 0;
int turn_time =0;
int feed_time = 0;
int feed_timeout_flag = False;


//T_MN_Event event;
static T_TMR_Timer TimerId = -1;

T_MN_Event event;
void *msg;


volatile byte LED_Active = LOW;

void IRAM_ATTR BlinkLed(unsigned short usDummy)
{   
   digitalWrite(ledPin, LED_Active);	   
   LED_Active = !LED_Active;
}
//===================================================================================================
void IamAlive(void)
{
   /* Init SW timer to blink led#0 every one second */
   if(TimerId == (T_TMR_Timer)-1)
		TimerId = TMR_TimerAdd(TMR_Cyclic, BlinkLed, 0);
	/* and start it */
	TMR_TimerStart(TimerId, 1000);
}
//===================================================================================================
void gpio_init()
{
  pinMode(ledPin, OUTPUT);
  pinMode(button, INPUT_PULLUP);    
  pinMode(gpio_zero_check, INPUT_PULLUP);  
  
  pinMode(gpio_motor, OUTPUT);
  digitalWrite(gpio_motor, LOW);	
  
  pinMode(gpio_ir, OUTPUT);
  digitalWrite(gpio_ir, HIGH);	  
  //pinMode(buttonPin, INPUT);
}
//===================================================================================================
void set_system_status(int status)
{
    e_system_status = status;
}
//===================================================================================================
void set_feed_timeout_flag(bool status)
{
    feed_timeout_flag = status;
}
//===================================================================================================
int Get_feed_time(void)
{
    return feed_time;
}
//===================================================================================================
void system_handle()
{
	switch (e_system_status)
	{
	   case  restart:
		   ir_return_value = adc_get_analog_value();
		   Serial.printf("adc_get_analog_value=%d\n",ir_return_value);
		   Serial.println();
		   Serial.printf("turn_time=%d\n",turn_time);
		   Serial.println();
		   
		   //if (turn_time < 20) break;
		   //if (ir_return_value>3600) ir_return_value = 0;
		   
		   if ((ir_return_value>5000)|| (turn_time>48)) //42
		   {
			 adc_timer_stop();
			 Serial.printf("turn_time=%d\n",turn_time);
			 Serial.println(); 
			 digitalWrite(gpio_motor, LOW);    
			   Serial.printf("adc_get_analog_value=%d\n",ir_return_value);
			 Serial.println(); 
			 feed_time=0;
			 //if (feed_time>6) feed_time = 0;
			 mqtt_set_feeder_time(feed_time);
			   Serial.printf("feed_time=%d\n",feed_time);
			 //Serial.println();	   
			 e_system_status = feed_time+feed0;
			 Serial.printf("e_system_status=%d\n",e_system_status);
			}

		break;
	   
	   case  start:
		  digitalWrite(gpio_motor, HIGH); 
		  zero_check_status=digitalRead(gpio_zero_check);
		  //Serial.println(zero_check_status);
		  if (0==zero_check_status)
		  {
		      Serial.printf("zero_check_status=%d\n",zero_check_status);
		  	//digitalWrite(gpio_motor, LOW);	
			e_system_status = restart;
			feed_time = 0;
			//mqtt_set_feeder_time(feed_time);
			adc_timer_start();
		  }
		break;	
		
		
        case  feed0:
	   case  feed1:
	   case  feed2:
	   case  feed3:
	   case  feed4:
	   case  feed5:   
	   case  feed6:   
	   	if (True==feed_timeout_flag)
	   	{
	   	  feed_timeout_flag = False;
		  turn_time = 0;
		  e_system_status = turning;
		  digitalWrite(gpio_motor, HIGH);
		  delay(2000);
		  adc_timer_start();	
	   	}

		break;
	   case  turning:
	   	ir_return_value = adc_get_analog_value();
		Serial.printf("adc_get_analog_value=%d\n",ir_return_value);
		Serial.println();
		Serial.printf("turn_time=%d\n",turn_time);
		Serial.println();

		//if (turn_time < 20) break;
		//if (ir_return_value>3600) ir_return_value = 0;
		
		if ((ir_return_value>5000)|| (turn_time>39)) //42
		{
		  adc_timer_stop();
		  Serial.printf("turn_time=%d\n",turn_time);
		  Serial.println();	
		  digitalWrite(gpio_motor, LOW);	
            Serial.printf("adc_get_analog_value=%d\n",ir_return_value);
		  Serial.println();	
		  feed_time++;
		  if (feed_time>6) feed_time = 1;
		  mqtt_set_feeder_time(feed_time);
            Serial.printf("feed_time=%d\n",feed_time);
		  //Serial.println();		
		  e_system_status = feed_time+feed0;
		  Serial.printf("e_system_status=%d\n",e_system_status);
		 }
		break;		

	}

}
//===================================================================================================
void setup() {
	
  gpio_init();  
  uart_init_0();
  Evnet_Init();
  timer_init2();
  nvm_init();

  wifi2_init(); 
  webpage_init(); //after wifi init
	 
  
  Serial.println();
  Serial.print("Hardware ID =");Serial.println(nvm_get_HW_ID());
  if (nvm_get_HW_ID()>1000)
  {
	  nvm_set_HW_ID(999);
	  Serial.print("Hardware ID =");Serial.println(nvm_get_HW_ID());
  }

  mqtt_init();
  //door_init();
  thingspeak_init();
  //device_init();
  wdt_init(); 
  adc_init();
  IamAlive();

  e_system_status = start;//restart;
  zero_check_status = HIGH;
  ir_return_value = 0;
  turn_time =0;
  feed_time = 0;
  feed_timeout_flag = False;
  mqtt_set_feeder_time(feed_time);

  while (0==digitalRead(gpio_zero_check))
  {
      digitalWrite(gpio_motor, HIGH); 

  }
  digitalWrite(gpio_motor, LOW); 

}
//====================================================================================================================
void loop() {
	wdt_handle();
	webpage_handleClient();
	uart_handle();
	mqtt_handle();
	system_handle();
	//adc_handle();


	/* Get new event */
	event = Event_GetEvent((void **)&msg);

	switch (event)
	{
		case MN_WifiCheck: 				/*	operator instruction */
              wifi2_handle();
	  	    break;
		case MN_mqttCheck: 				
              //mqtt_handle();	
		    break; 
		case MN_SaveSettings: 				/*	operator instruction */
               nvm_handle();
	  	    break; 
		case MN_thingspeak_upload:				
			 thingspeak_handle();
		    break;
		case MN_ADC:
			adc_handle();
			turn_time++;
			break;
/*
			
		case MN_DoorOff: 				
            door_turn_off_Anjin();
	  	    break; 	
		case MN_DoorDetect: 				
             door_handle();
	  	    break; 	

		case MN_location: 				
             device_handle();
	  	    break; 	
*/	  	    
	}

}
//====================================================================================================================

//====================================================================================================================
