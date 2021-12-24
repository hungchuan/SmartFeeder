#ifndef _GENERAL_H_
#define _GENERAL_H_

#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <EEPROM.h>
#include <Ticker.h>
#include <esp_task_wdt.h>
#include <PubSubClient.h>
#include <ArduinoJson.h> 
#include <HTTPClient.h>


#include "type.h"

#include "uart_0.h"
#include "webpage.h"
#include "nvm.h"
#include "wifi2.h"
#include "watchdog.h"
#include "OPSW_timer.h"
#include "cmdq.h"
#include "event.h"
#include "mqtt.h"

#include "door.h"
#include "distance.h"
#include "device.h"
#include "thingspeak.h"

#if defined (ESP32)

#include "timer_esp32.h"
#include <WiFi.h>
#include <WiFiMulti.h>
#include <WebServer.h>
#include <ESP32Ping.h>

#define INTERRUPT_ATTR IRAM_ATTR 

#elif defined (ESP8266) 

#include "timer_esp8266.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include <Ticker.h>
#define WIFI_AUTH_OPEN ENC_TYPE_NONE

#endif






#endif
