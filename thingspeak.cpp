#include "general.h"

//thingspeak begin
#define HOST_thingspeak 	"api.thingspeak.com" // ThingSpeak IP Address: 184.106.153.149
#define PORT_thingspeak 	80
String GET = "GET /update?api_key=FKVIRPB1Q0YK32FL";
String FIELD1 = "&field1=";
String FIELD2 = "&field2=";
String FIELD3 = "&field3=";
String FIELD4 = "&field4=";
String FIELD5 = "&field5=";
String FIELD6 = "&field6=";


String thingspeak_url = "http://api.thingspeak.com/update?api_key=FKVIRPB1Q0YK32FL";

//thingspeak end

static T_TMR_Timer thingspeak_upload_TimerId = -1;

void IRAM_ATTR thingspeak_upload_timeout(unsigned short usDummyParam);
void thingspeak_upload(void);    

int thingspeak_mqtt_command=0;
bool thingspeak_upload_flag=0;


void thingspeak_init(void)
{
    thingspeak_mqtt_command = 4;
    thingspeak_upload_flag = false;
    
    if(thingspeak_upload_TimerId == (T_TMR_Timer)-1)
        thingspeak_upload_TimerId= TMR_TimerAdd(TMR_Cyclic, thingspeak_upload_timeout, 0); 
    
    TMR_TimerStart(thingspeak_upload_TimerId, 60000);

}
//================================================================================================
void thingspeak_handle(void)
{
    if (true==nvm_get_boot_flag()||(true==thingspeak_upload_flag))
    {
        thingspeak_upload_flag = false;
        thingspeak_upload();
        nvm_clean_boot_flag();
    }

}
//======================================================================================================================
void thingspeak_upload(void) 
{

    if (false==wifi_get_connection())   
    {
        return;
    }

    Serial.println( "===update_to_thingspeak====" );  
    
    HTTPClient http;

    String url1 = thingspeak_url + FIELD1 + (int)nvm_get_boot_count() + FIELD2 + thingspeak_mqtt_command;

    http.begin(url1);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK)
    {
      String payload = http.getString();

      Serial.print("payload = ");
      Serial.println(payload);
    } 
    else 
    {

      Serial.println("connection failed");
    }
    http.end();

  
}

#if 0
void thingspeak_upload(void) 
{

  if (false==wifi_get_connection())   
  {
      return;
  }

  WiFiClient client;
  
   //initialize your AT command string
  //String cmd = "AT+CIPSTART=\"TCP\",\"";
  String cmd;
 
  
  char c_boot_count[5];  
  //String s_hour,s_minute; 
  
  Serial.println( "===update_to_thingspeak====" );  
  
  sprintf(c_boot_count,"%d",nvm_get_boot_count());    
  String s_boot_count(c_boot_count);
 
  Serial.print("SystemData.boot_count = "); Serial.println(nvm_get_boot_count());  
 
	if( !client.connect( HOST_thingspeak, PORT_thingspeak ) )
	{
		Serial.println( "connection failed" );
		return;
	}
	else
	{

		//build GET command, ThingSpeak takes Post or Get commands for updates, I use a Get
		cmd = GET;
		cmd += FIELD1;
		cmd += s_boot_count;

		//cmd += "\r\n";

		String getStr = cmd + " HTTP/1.1\r\n";

	    client.print( getStr );
	    client.print( "Host: api.thingspeak.com\n" );
	    client.print( "Connection: close\r\n\r\n" );
	    
      	delay(10);

      	client.stop();

	}
	

  
}
#endif
//================================================================================================
void IRAM_ATTR thingspeak_upload_timeout(unsigned short usDummyParam)
{
	Event_EventSend(MN_thingspeak_upload, (void *)NULL);
}
//================================================================================================
void thingspeak_set_mqtt_command(int cmd)
{
    thingspeak_mqtt_command = cmd;
    thingspeak_upload_flag = true;
}

//================================================================================================
