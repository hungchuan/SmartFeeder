#ifndef _THINKSPEAK_H
#define _THINKSPEAK_H

#ifdef __cplusplus
extern "C" {
#endif

void thingspeak_init(void);
void thingspeak_handle(void);
void thingspeak_set_mqtt_command(int cmd);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //_DOOR_H
