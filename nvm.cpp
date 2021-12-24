#include "general.h"
/*
#include <Arduino.h>
#include <EEPROM.h>
#include "nvm.h"
*/

#define DIC_SAVING_INTERVAL         			5000
static void nvm_ApplySaveSettings(unsigned short usDummyParam);


int eeAddress = 0; //EEPROM address to start reading from
TDIC_SystemDictionary  SystemData;
bool nvm_save_flag = false;
static T_TMR_Timer ConfigSaveTimer = -1;
bool nvm_boot_flag = 0;


void nvm_init(void)
{
	nvm_save_flag = false;
	EEPROM.begin(512);
    delay(10); 
    Serial.println("Reading EEPROM....................");
    EEPROM.get(eeAddress, SystemData);	

    if (MAGIC_NUMBER != SystemData.magic_number)
    {
        SystemData.magic_number = MAGIC_NUMBER;
        nvm_clean_network();
        nvm_clean_SprayData();
    }
   

    SystemData.boot_count++;
    nvm_save_flag = true;
    nvm_boot_flag = 1;

    Serial.printf("SystemData.eep_ssid=%d\n",SystemData.eep_ssid);
    Serial.printf("SystemData.eep_pass=%d\n",SystemData.eep_pass);
	
    if(ConfigSaveTimer == (T_TMR_Timer)-1)
   	  ConfigSaveTimer = TMR_TimerAdd(TMR_Cyclic, nvm_ApplySaveSettings, 0);
    TMR_TimerStart(ConfigSaveTimer, DIC_SAVING_INTERVAL);	
}
//================================================================================================
static void IRAM_ATTR nvm_ApplySaveSettings(unsigned short usDummyParam)
{
   Event_EventSend(MN_SaveSettings, (void *)NULL);
}
//================================================================================================
void nvm_handle(void)
{
    if (true==nvm_save_flag)
	{
	  Serial.println("NVM saving...");
	  nvm_save_flag = false;
	  EEPROM.put(eeAddress, SystemData);
	  EEPROM.commit();
       Serial.println("NVM save done");
	}			
}
//================================================================================================
void nvm_clean_network(void)
{
	Serial.println("EEPROM_clean_network...");
	memset (SystemData.eep_ssid, 0, sizeof(SystemData.eep_ssid));
	memset (SystemData.eep_pass, 0, sizeof(SystemData.eep_pass));
	EEPROM.put(eeAddress, SystemData);
	EEPROM.commit();
}
//================================================================================================
void nvm_clean_SprayData(void)
{
	Serial.println("EEPROM clean Spray data................");
	SystemData.date = 0;
	SystemData.spraytime = 0;
	SystemData.boot_count = 0;
	EEPROM.put(eeAddress, SystemData);
	EEPROM.commit();
}
//================================================================================================
void nvm_set_save_flag(bool flag)
{
	nvm_save_flag = flag;
	Event_EventSend(MN_SaveSettings, (void *)NULL);	
}
//================================================================================================
void nvm_set_HW_ID(unsigned int id)
{
	SystemData.id = id;
	nvm_save_flag = true;
    Event_EventSend(MN_SaveSettings, (void *)NULL);	
}
//================================================================================================
unsigned int nvm_get_HW_ID(void)
{
	return SystemData.id;
}
//================================================================================================
unsigned int nvm_get_boot_count(void)
{
	return SystemData.boot_count;
}//================================================================================================
void nvm_set_ssid(char* ssid)
{
	strcpy(SystemData.eep_ssid,ssid);
	nvm_save_flag = true;
	Event_EventSend(MN_SaveSettings, (void *)NULL);	
}
//================================================================================================
void nvm_get_ssid(char* ssid)
{
	strcpy(ssid,SystemData.eep_ssid);
}
//================================================================================================
void nvm_set_password(char* pw)
{
	strcpy(SystemData.eep_pass,pw);
	nvm_save_flag = true;
	Event_EventSend(MN_SaveSettings, (void *)NULL);	
}
//================================================================================================
void nvm_get_password(char* pw)
{
	strcpy(pw,SystemData.eep_pass);
}
//================================================================================================
void nvm_clean_boot_flag(void)
{
	nvm_boot_flag = 0;
}
//================================================================================================
bool nvm_get_boot_flag(void)
{
	return nvm_boot_flag;
}
//================================================================================================
