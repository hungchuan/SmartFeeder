#ifdef ESP32

#include "general.h"

hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

volatile uint32_t isrCounter = 0;
volatile uint32_t lastIsrAt = 0;

int wifi_reconnect_count = 0;


void IRAM_ATTR onTimer(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  isrCounter++;
  lastIsrAt = millis();
  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
  // It is safe to use digitalRead/Write here if you want to toggle an output
  TMR_TimerIntHandler();

}
//================================================================================================
void timer_init2(void) {
	
  TMR_Init();
  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();

  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, TMR_TICK*1000, true);

  // Start an alarm
  timerAlarmEnable(timer);
}
//================================================================================================
void timer_handle(void) {
  // If Timer has fired
  if (xSemaphoreTake(timerSemaphore, 0) == pdTRUE){
    uint32_t isrCount = 0, isrTime = 0;
    // Read the interrupt count and time
    portENTER_CRITICAL(&timerMux);
    isrCount = isrCounter;
    isrTime = lastIsrAt;
    portEXIT_CRITICAL(&timerMux);
	
    if (wifi_reconnect_count>0) wifi_reconnect_count--;

 	
    // Print it		
    //Serial.print("onTimer no. ");
    //Serial.print(isrCount);
    //Serial.print(" at ");
    //Serial.print(isrTime);
    //Serial.println(" ms");	

  }

}
//================================================================================================
void IRAM_ATTR timer_enable(void)
{
/*	
  timer = timerBegin(0, 80, true);

  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);

  // Set alarm to call onTimer function every second (value in microseconds).
  // Repeat the alarm (third parameter)
  timerAlarmWrite(timer, TMR_TICK*1000, true);
*/
  // Start an alarm
    if (timer) timerAlarmEnable(timer);

}
//================================================================================================
void IRAM_ATTR timer_disable(void)
{
    if (timer) {
      timerAlarmDisable(timer);
      // Stop and free timer
      //timerEnd(timer);
      //timer = NULL;
    }
}
//================================================================================================



//================================================================================================

//================================================================================================

//================================================================================================

//================================================================================================

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

//================================================================================================

#endif //ESP32
