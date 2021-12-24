#include "general.h"


void wdt_init(void)
{
  Serial.println("Configuring WDT...");
  esp_task_wdt_init(WDT_TIMEOUT, true); //enable panic so ESP32 restarts
  esp_task_wdt_add(NULL); //add current thread to WDT watch	
}
//================================================================================================
void wdt_handle(void)
{
  esp_task_wdt_reset();
}
//================================================================================================
