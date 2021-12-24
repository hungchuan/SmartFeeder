#include "general.h"
/*
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include "uart.h"
#include "nvm.h"
*/
//*-- Hardware Serial
String inputString = "";         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void uart_init_0(void)
{
  Serial.begin(_baudrate);	
  delay(100);	
}
//================================================================================================
void uart_handle(void)
{
  uart_command();
}
//====================================================================================================================
void uart_command(void)
{
	
  char command[50];
  char ch;
  unsigned int id;
  char command_1[20], command_2[20], new_data[20];
  int gpio_num, status;
  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:

    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\r') 
	{
        stringComplete = true;
        Serial.println();
    }
	else
	{
		Serial.print(inChar);
		inputString += inChar;		
	}
	
  }
  
    if (stringComplete) 
    {
        stringComplete = false;
		
		strcpy(command, inputString.c_str());
        inputString = "";		

		switch (command[0])
		{					
			case 's':
			    Serial.println("===Set up system===");
				if ((sscanf(command, "%s %s %s", command_1,command_2,new_data))==3)
				{
					Serial.print("command_1 =");
					Serial.println(command_1);

					Serial.print("command_2 =");
					Serial.println(command_2);
					
					Serial.print("new_data =");
					Serial.println(new_data);
					
					if(!strcmp (command_2,"ssid"))
					{
						nvm_set_ssid(new_data);
						Serial.print("New SSID = "); Serial.println(new_data);						
					}
					else if(!strcmp (command_2,"pw"))
					{
						nvm_set_password(new_data);
						Serial.print("New PW = "); Serial.println(new_data);						
					}

				}             			
			break;

			case 'o':
				Serial.println("setup gpio");
				if ((sscanf(command, "%s %s %s", command_1,command_2,new_data))==3)
				{
					gpio_num = atol(command_2);
					status = atol(new_data);
					
					Serial.print("command_1 =");
					Serial.println(command_1);

					Serial.print("gpio_num =");
					Serial.println(gpio_num);
					
					Serial.print("ststus =");
					Serial.println(status);
					
					pinMode(gpio_num, OUTPUT);
                    digitalWrite(gpio_num, status);					
				}
			break;
			
			case 'c':
                nvm_clean_SprayData();			
			break;
			
			case 'x':
                nvm_clean_SprayData();	
                nvm_clean_network();				
			break;		
			case 'd':
			    door_Close_Garage();
			break;	            
			case 'n':
				if ((sscanf(command, "%s %d", &ch,&id))==2)
				{
					nvm_set_HW_ID(id);
					Serial.print("New HW ID = "); Serial.println(nvm_get_HW_ID());
				}
					
			break;	

			case 'm':
				if ((sscanf(command, "%s %d", &ch,&id))==2)
				{
				    if (0==id)
                        {           
                            Serial.println("msg_Garage_OFF");
                            mqtt_set_publish(msg_Garage_OFF);
                        }
                        else if (1==id)
                        {
                            Serial.println("msg_Garage_ON");
                            mqtt_set_publish(msg_Garage_ON);	
                        }

				}
					
			break;

			case 'g':
                   float Lati;
                   float Long;
			    Serial.println("=== check distance ===");
				if ((sscanf(command, "%s %f %f", command_1,&Lati,&Long))==3)
				{
					Serial.print("command_1 =");
					Serial.println(command_1);

					Serial.print("command_2 =");
					Serial.println(Lati);
					
					Serial.print("new_data =");
					Serial.println(Long);

                         Serial.print("Distance =");
                         Serial.println(getDistanceGD(Home_lati,Home_long,Lati,Long));

				}             			
			break;

			case 't':
			    wifi_ping_test();
			break;	  
            
			default:
                Serial.println("o : set gpio");
			    Serial.println("h : Spray_Enable_Pin = high");
				Serial.println("l : Spray_Enable_Pin = low");
				Serial.println("s : set up ssid and pw ex: s ssid yourssid");
				Serial.println("c : clean spay data ");
				Serial.println("n id : set HW ID ");
			break;
			
		}


    }  
	
}