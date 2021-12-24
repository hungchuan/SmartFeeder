#include "general.h"
/*
#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include "webpage.h"
#include "nvm.h"
*/
/* Put your SSID & Password */
const char* ssid = "ESP32_AP";  // Enter SSID here
const char* password = "12345678";  //Enter Password here

char new_ssid[_ssid_len_];
char new_pw[_pw_len_];

#if defined (ESP32)
WebServer server(80);
#elif defined (ESP8266) 
ESP8266WebServer server(80);
#endif

String st;
String content;
int statusCode;

static void webpage_handleRoot();
static void webpage_setting_page();
static void webpage_garage_page();
static void webpage_cleareeprom_page();

void webpage_init(void)
{
    if (true==wifi_get_connection())
    {
        webpage_launchWeb(STA_Mode); 
    }
    else
    {
        webpage_setupAP();
    }
}
//================================================================================================
void webpage_handleClient(void)
{
  server.handleClient();	
}
//================================================================================================

void webpage_launchWeb(int webtype) {
	
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  
  webpage_createWebServer(webtype);
  
  // Start the server
  server.begin();
  Serial.println("Server started"); 
}
//================================================================================================
void webpage_setupAP(void) {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    Serial.println("no networks found");
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
     {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
      delay(10);
     }
  }
  Serial.println(""); 
  st = "<ol>";
  for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      st += "<li>";
      st += WiFi.SSID(i);
      st += " (";
      st += WiFi.RSSI(i);
      st += ")";
      st += (WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*";
      st += "</li>";
    }
  st += "</ol>";
  delay(100);
  
  //WiFi.softAP(ssid, password, 6);
  WiFi.softAP(ssid);
  Serial.println("softap");
  webpage_launchWeb(AP_Mode);
  Serial.println("over");
}
//================================================================================================
void webpage_createWebServer(int webtype)
{
  if ( webtype == AP_Mode ) {
    server.on("/", webpage_handleRoot);	
    server.on("/setting", [](){ webpage_setting_page();});	
    server.on("/garage", []() { webpage_garage_page();	});	
  } else if (webtype == STA_Mode) {
    server.on("/", webpage_handleRoot);	
    server.on("/cleareeprom", []() { webpage_cleareeprom_page(); });	
    server.on("/garage", []() { webpage_garage_page();	});    
  }
}
//================================================================================================
static void webpage_handleRoot()
{
    IPAddress ip = WiFi.softAPIP();
    String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
    content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP32 at ";
    content += ipStr;
    content += "<p>";
    content += st;
    content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' length=32><input name='pass' length=64><input type='submit'></form>";
    content += "</p><form method='get' action='garage'><label>Open garage </label><input type='submit'></form>";
	content += "</p><form method='get' action='cleareeprom'><label>clear eeprom </label><input type='submit'></form>";
    content += "</html>";
    server.send(200, "text/html", content);  	
}
//================================================================================================
static void webpage_setting_page()
{
    String qsid = server.arg("ssid");
    String qpass = server.arg("pass");
    if (qsid.length() > 0 && qpass.length() > 0) 
    {

        nvm_clean_network();
		  
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");
            
        Serial.println("writing eeprom ssid:");
	   strcpy(new_ssid, qsid.c_str());
	   strcpy(new_pw, qpass.c_str());

	   nvm_set_ssid(new_ssid);
  	   nvm_set_password(new_pw);
		  
        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
    } 
    else
    {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
    }
        server.send(statusCode, "application/json", content);
}
//================================================================================================
static void webpage_garage_page()
{
    Serial.println("Open Garage");
    //door_Open_Garage();
    content = "{\"Success\":\}";
    statusCode = 200;
    server.send(statusCode, "application/json", content); 
}
//================================================================================================
static void webpage_cleareeprom_page()
{
    Serial.println("Open Garage");
    //door_Open_Garage();
    content = "{\"Success\":\}";
    statusCode = 200;
    server.send(statusCode, "application/json", content); 
}
