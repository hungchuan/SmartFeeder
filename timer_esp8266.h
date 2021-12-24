#ifndef _TIMER_ESP8266_H
#define _TIMER_ESP8266_H

#ifdef __cplusplus
extern "C" {
#endif

void timer_init2(void);
void timer_handle(void);
void set_wifi_reconnect_count(uint32_t val);
uint32_t get_wifi_reconnect_count(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif //_NVM_H
