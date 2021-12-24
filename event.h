#ifndef _EVENT_H
#define _EVENT_H

#ifdef __cplusplus
extern "C" {
#endif


/* Events that start a process in the main loop */
typedef enum    {
                    MN_WifiCheck,
                    MN_mqttCheck,
				MN_SaveSettings,	
				MN_DoorOff,
				MN_DoorDetect,
				MN_thingspeak_upload,
				MN_location,	
				MN_ADC,	
                    MN_End,    
                    MN_NULL = 0xFF				
                } T_MN_Event;

				
void Evnet_Init(void);
T_MN_Event Event_GetEvent(void **ptr);
Boolean Event_EventSend (T_MN_Event event, void *data);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //_EVENT_H