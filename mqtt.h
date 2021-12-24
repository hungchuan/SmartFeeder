#ifndef _MQTT_H
#define _MQTT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef msg_Garage_ON
#define msg_Garage_ON "ON"
#endif

#ifndef msg_Garage_OFF
#define msg_Garage_OFF "OFF"
#endif

//char* Garage_ON = "ON"; 
//char* Garage_OFF = "OFF"; 

void mqtt_init(void);
void mqtt_handle(void);
void mqtt_check_connection(unsigned short usDummyParam);
void mqtt_set_publish(char* msg);
void mqtt_set_feeder_time(int num);
void mqtt_set_feeder_publish(int status);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //_MQTT_H

