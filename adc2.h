#ifndef _ADC2_H
#define _ADC2_H

#ifdef __cplusplus
extern "C" {
#endif

void adc_init(void);
void adc_handle(void);
void IRAM_ATTR adc_event(unsigned short usDummyParam);
void adc_timer_start(void);
void adc_timer_stop(void);
int adc_get_analog_value(void);



#ifdef __cplusplus
} // extern "C"
#endif

#endif 

