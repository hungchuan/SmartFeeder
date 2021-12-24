#include "general.h"


static T_TMR_Timer mqttCheckTimerId = -1;


//MQTT=========================================================================
const char* mqtt_server = "192.168.68.116";
int mqtt_port = 1883;
const char* user_name = "mqtt"; 
const char* user_password = "mqtt";

// ESP8266訂閱的主題：收到 0 關閉 LED，1 打開LED
const char* topic_subscribe1 = "ha/1F/garage/switch"; 
const char* topic_subscribe2 = "ha/1F/garage/switch/ryan"; 
const char* topic_subscribe3 = "ha/1F/garage/switch/emma"; 
const char* topic_subscribe_switch_1 = "ha/1F/garage/switch/"; 
const char* topic_subscribe_gps_1 = "owntracks/mqtt/Ryan"; 
const char* topic_subscribe_gps_2 = "owntracks/mqtt/Emma"; 
const char* topic_publish = "ha/1F/garage/states";

char* Garage_ON = "ON"; 
char* Garage_OFF = "OFF"; 
char* Garage_states_msg = "";
int Garage_states = 0;
int prevGarage_states = 0;
bool topic_subscribe1_flag=false;
bool topic_subscribe2_flag=false;
bool topic_subscribe3_flag=false;



bool topic_publish_flag=false;
bool mqtt_connected_flag=false;
bool mqtt_Reconnected_flag=false;


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
//char msg[50];
char* mqtt_msg = "";
int value = 0;
int MQTT_reconnect = 0;

//StaticJsonBuffer jsonBuffer;
//StaticJsonDocument<200> doc;
DynamicJsonDocument doc(2048);


//=============================================================================

static void mqtt_callback(char* topic, byte* payload, unsigned int length); 
static void mqtt_reconnect(); 


void mqtt_init(void)
{
    
    topic_publish_flag=false;
    mqtt_connected_flag=false;
    mqtt_Reconnected_flag=false;
    
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(mqtt_callback);    
    if( mqttCheckTimerId == (T_TMR_Timer)-1)
		mqttCheckTimerId = TMR_TimerAdd(TMR_Cyclic, mqtt_check_connection, 0);
	/* and start it */
	TMR_TimerStart(mqttCheckTimerId, 10000); 
     //Event_EventSend(MN_mqttCheck, (void *)NULL);
}
//================================================================================================
void mqtt_handle(void)
{
    UBYTE i;
    if (true==wifi_get_connection())   
    {
        if (!client.connected()) 
        {
            mqtt_reconnect();
	   }
        else
        {
           mqtt_connected_flag = true;
        }
          
        if (true == mqtt_connected_flag)
		client.loop();  
        
	   if (true==topic_subscribe1_flag)
	   {
	       topic_subscribe1_flag = false;
					
		  Serial.println("topic_subscribe1_flag");
		  //client.publish(topic_publish, Garage_states_msg); // 發布MQTT主題與訊息		

		  if (0==Garage_states)
            {
		      door_Close_Garage();
                mqtt_set_publish(msg_Garage_OFF);
            }
		  else if (1==Garage_states)
		  {			
		      door_Open_Garage();
                mqtt_set_publish(msg_Garage_ON);
		  }
		  else if (2==Garage_states)
		  {
                if (true==device_ip_tracking(0))
                    return;
                Serial.println("Garage_open=====ryan");
                door_Open_Garage();
                mqtt_set_publish(msg_Garage_ON);
		  }
		  else if (3==Garage_states)
		  {
              if (true==device_ip_tracking(1))
                  return;
              Serial.println("Garage_open=====emma");
              door_Open_Garage();
              mqtt_set_publish(msg_Garage_ON);
		  }	          
        }

        if (true==topic_subscribe2_flag)
        {
           topic_subscribe2_flag = false;

           Serial.println("topic_subscribe2_flag");

           //client.publish(topic_publish, Garage_states_msg); // 發布MQTT主題與訊息
           
             
           if (1==Garage_states)
           {     
               if (true==device_ip_tracking(0))
                   return;
                               
               door_Open_Garage();
               mqtt_set_publish(msg_Garage_ON);
           }
           else
           {
               door_Close_Garage();
               mqtt_set_publish(msg_Garage_OFF);
           }                             
         }

        if (true==topic_subscribe3_flag)
        {
           topic_subscribe3_flag = false;
            
           Serial.println("topic_subscribe3_flag");

           //client.publish(topic_publish, Garage_states_msg); // 發布MQTT主題與訊息
           
             
           if (1==Garage_states)
           {         
               if (true==device_ip_tracking(1))
                   return;           
               
               door_Open_Garage();
               mqtt_set_publish(msg_Garage_ON);
           }
           else
           {
               door_Close_Garage();
               mqtt_set_publish(msg_Garage_OFF);
           }                             
         }


        if (true==topic_publish_flag)
        {
            topic_publish_flag = false;
            client.publish(topic_publish, Garage_states_msg);
        }
       
    }
}
//======================================================================================================================
void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
  payload[length] = '\0';  
  mqtt_msg = (char*)payload;

  Serial.print("Command from MQTT broker is : ");
  Serial.print(topic);
  Serial.println();
  Serial.print(mqtt_msg);  
  Serial.println();  
  
  if (true == mqtt_Reconnected_flag)
  {
      Serial.println("mqtt_Reconnected_flag = true; skip!!!");
      mqtt_Reconnected_flag = false;
      return;
  }

  if(strcmp(topic, topic_subscribe1) == 0)
  {
     if(strcmp(mqtt_msg, "ON") == 0)
	 {
		topic_subscribe1_flag = true;
		Garage_states=1;
          thingspeak_set_mqtt_command(1);
		//Garage_states_msg = msg_Garage_ON;	
          //Event_EventSend(MN_mqttCheck, (void *)NULL);
	 }
	 else if(strcmp(mqtt_msg, "OFF") == 0)
	 {
		topic_subscribe1_flag = true;
		Garage_states=0;
          thingspeak_set_mqtt_command(0);
		//Garage_states_msg = msg_Garage_OFF;	
          //Event_EventSend(MN_mqttCheck, (void *)NULL);
	 }
      else if(strcmp(mqtt_msg, "ryan") == 0)
      {
          topic_subscribe1_flag = true;
          Garage_states=2;
          thingspeak_set_mqtt_command(2);
      }
      else if(strcmp(mqtt_msg, "emma") == 0)
      {
          topic_subscribe1_flag = true;
          Garage_states=3;
          thingspeak_set_mqtt_command(3);
      }      
  }
  
  if(strcmp(topic, topic_subscribe2) == 0)
  {  
     if(strcmp(mqtt_msg, "ON") == 0)
	 {
		topic_subscribe2_flag = true;
		Garage_states=1;
		//Garage_states_msg = msg_Garage_ON;	
          //Event_EventSend(MN_mqttCheck, (void *)NULL);
	 }
	 else if(strcmp(mqtt_msg, "OFF") == 0)
	 {
		topic_subscribe2_flag = true;
		Garage_states=0;
		//Garage_states_msg = msg_Garage_OFF;	
          //Event_EventSend(MN_mqttCheck, (void *)NULL);
	 }  
  }  

  if(strcmp(topic, topic_subscribe3) == 0)
  {  
     if(strcmp(mqtt_msg, "ON") == 0)
	 {
		topic_subscribe3_flag = true;
		Garage_states=1;
		//Garage_states_msg = msg_Garage_ON;	
          //Event_EventSend(MN_mqttCheck, (void *)NULL);
	 }
	 else if(strcmp(mqtt_msg, "OFF") == 0)
	 {
		topic_subscribe3_flag = true;
		Garage_states=0;
		//Garage_states_msg = msg_Garage_OFF;	
          //Event_EventSend(MN_mqttCheck, (void *)NULL);
	 }  
  }  

  if(strcmp(topic, topic_subscribe_gps_1) == 0)
  {  
      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc,mqtt_msg);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
  
      float latitude    = doc["lat"];
      float longitude   = doc["lon"];	 
      Serial.print("lat = ");  
      Serial.println(latitude);        
      Serial.print("lon = ");  
      Serial.println(longitude);
      device_set_loction(0,latitude,longitude);
      Serial.print("Distance_GPS1= ");  
      Serial.println(getDistanceGD(Home_lati,Home_long,latitude,longitude));
      
      Event_EventSend(MN_location, (void *)NULL);      
  }  

  if(strcmp(topic, topic_subscribe_gps_2) == 0) 
  {  
      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc,mqtt_msg);

      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }
  
      float latitude    = doc["lat"];
      float longitude   = doc["lon"];	 
      Serial.print("lat = ");  
      Serial.println(latitude);        
      Serial.print("lon = ");  
      Serial.println(longitude);
      Serial.print("Distance_GPS2= ");  
      Serial.println(getDistanceGD(Home_lati,Home_long,latitude,longitude));	  
  }    
	
/*	
  int p =(char)payload[0]-'0'; 
  if(p==0) // MQTT 傳來 0 熄滅 D2 上的 LED
  {
	  Close_Garage();
  } 
  
  if(p==1) // MQTT 傳來 1 點亮 D2 上的 LED
  {
	  Open_Garage();
  }
*/
  Serial.println();
} //end callback
//=================================================================================================
void mqtt_reconnect() {
  // Loop until we're reconnected
//  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(),user_name,user_password)) {
      Serial.println("connected");
      client.subscribe(topic_subscribe1);
      //client.subscribe(topic_subscribe2);
      //client.subscribe(topic_subscribe3);      
      //client.subscribe(topic_subscribe_gps_1);      
      //client.subscribe(topic_subscribe_gps_2);         
      
      mqtt_connected_flag = true;
      mqtt_Reconnected_flag = true;
    } else {
      Serial.print("failed, rc=");
      Serial.println(client.state());
      mqtt_connected_flag = false;
      //Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      //delay(5000);
    }
//  }
}
//================================================================================================
void IRAM_ATTR mqtt_check_connection(unsigned short usDummyParam)
{
     topic_publish_flag = true;     
	//Event_EventSend(MN_mqttCheck, (void *)NULL);
}
//================================================================================================
void mqtt_set_publish(char* msg)
{
    Garage_states_msg = msg;	   
    topic_publish_flag = true;
    //Event_EventSend(MN_mqttCheck, (void *)NULL);    
    //client.publish(topic_publish, Garage_states_msg); 
}
//================================================================================================
void mqtt_set_garage_publish(int status)
{
    if (CLOSE==status)
    {
       Garage_states_msg = Garage_OFF;	    
    }
    else
    {
  	  Garage_states_msg = Garage_ON;
    }
    topic_publish_flag = true;
    //Event_EventSend(MN_mqttCheck, (void *)NULL);
}
//================================================================================================


