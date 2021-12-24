#ifndef _TIMER_ESP32_H
#define _TIMER_ESP32_H

#ifdef __cplusplus
extern "C" {
#endif


void timer_init2(void);
void timer_handle(void);
void set_wifi_reconnect_count(uint32_t val);
uint32_t get_wifi_reconnect_count(void);
//void BlinkLed(void);
void timer_set_Garage_Off(uint32_t val);
uint32_t timer_get_Garage_Off(void);
void  timer_enable(void);
void  timer_disable(void);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //_NVM_H
