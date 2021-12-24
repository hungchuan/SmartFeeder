#include "general.h"

#if defined (ESP32)
WiFiMulti wifiMulti;
#elif defined (ESP8266) 
ESP8266WiFiMulti wifiMulti;
#endif

//#define _wifi_Multi_

static T_TMR_Timer WifiCheckTimerId = -1;

char now_ssid[_ssid_len_];
char now_pw[_pw_len_];
bool WiFi_connected=false;


void wifi2_init(void)
{
  WiFi_connected = false;
  nvm_get_ssid(now_ssid);
  nvm_get_password(now_pw);	
    
  Serial.print("SSID: ");
  Serial.println(now_ssid);
  Serial.print("PASS: ");
  Serial.println(now_pw); 
  
  Serial.println("Connecting Wifi...");
  
  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_AP_STA);// WIFI_AP_STA  WIFI_STA
  

  Serial.println();
  Serial.print("Wait for WiFi... ");

  if (true==wifi2_connect())
  {
	  WiFi_connected = true;
  }
 
  if (false == WiFi_connected)
  {
	  WiFi.disconnect();

	  if (true==wifi2_connect())
	  {
		  WiFi_connected = true;
	  }  
  }	  

   WiFi.setAutoReconnect(true);
   WiFi.persistent(true);

   if(WifiCheckTimerId == (T_TMR_Timer)-1)
		WifiCheckTimerId = TMR_TimerAdd(TMR_Cyclic, wifi2_WifiCheck, 0);
	/* and start it */
	TMR_TimerStart(WifiCheckTimerId, 10000);

  delay(100);
  
}
//================================================================================================
void wifi2_handle()
{
    int wifi_status;

#ifdef _wifi_Multi_
    wifi_status = wifiMulti.run();		
#else	
    wifi_status = WiFi.status();
#endif	
    Serial.print("wifi status = ");    
    Serial.println(wifi_status);

    if (wifi_status != WL_CONNECTED) 
    {   
        WiFi.disconnect();
	   Serial.println("WiFi not connected!");
        WiFi_connected = false;         
        WiFi.reconnect();
	}
     else
     {
        WiFi_connected = true;
     }

}
//================================================================================================

bool wifi2_connect(void) {

  int c = 0;

  String esid(now_ssid);
  
  if ( esid.length() > 1 );
  else
  {
	 Serial.println("esid.length()<=1");
     return false;
  }	  
	  
  Serial.println("Waiting for Wifi to connect");  

#ifdef _wifi_Multi_
  wifiMulti.addAP(now_ssid, now_pw); 
#else  
  WiFi.begin(now_ssid, now_pw);
#endif

  while ( c < 20 ) {
#ifdef _wifi_Multi_
    if (wifiMulti.run() != WL_CONNECTED) {		
#else	
    if (WiFi.status() != WL_CONNECTED) {
#endif		
        delay(500);
        Serial.print(".");
    }
	else
	{
		Serial.println("");
		Serial.println("WiFi connected.");
		Serial.println("IP address: ");
		Serial.println(WiFi.localIP());  		
		return true;
	}
    c++;
  }

  return false;
  
} 
#if 0
bool wifi2_connect(void) {

  int c = 0;

  String esid(now_ssid);
  
  if ( esid.length() > 1 );
  else
  {
	 Serial.println("esid.length()<=1");
     return false;
  }	  
	  
  Serial.println("Waiting for Wifi to connect");  
  while ( c < 20 ) {
    while (wifiMulti.run() == WL_CONNECTED) { return true; } 
    Serial.print(".");
    delay(1000);
    c++;
  }
  Serial.println("");
  Serial.println("Connect timed out, opening AP");
  delay(1000);
  return false;
  
} 
#endif
//================================================================================================
void IRAM_ATTR wifi2_WifiCheck(unsigned short usDummyParam)
{
    Event_EventSend(MN_WifiCheck, (void *)NULL);
}
//================================================================================================
bool wifi_get_connection(void)
{
    return WiFi_connected;
}
//================================================================================================
void wifi_ping_test(void)
{
    bool success = Ping.ping("www.google.com",1);
    //bool success = Ping.ping("192.168.68.101",1);
    
    if(!success){
      Serial.println("Ping failed");
      return;
    }
    
    Serial.println("Ping succesful.");

}
//================================================================================================
bool wifi_ping(char* ip)
{
    char ping_ip[16];
    strcpy(ping_ip,ip);
    Serial.print("ip=");
    Serial.println(ping_ip);
    bool success = Ping.ping(ping_ip,3);

    if(!success){
      Serial.println("Ping failed");
    }
    else
    {
        Serial.println("Ping succesful.");
    }
    
    return success;
}
//================================================================================================



