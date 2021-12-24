#ifndef _WATCHDOG_H
#define _WATCHDOG_H

#ifdef __cplusplus
extern "C" {
#endif

#define WDT_TIMEOUT 30  //10 seconds WDT

void wdt_init(void);
void wdt_handle(void);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //_WATCHDOG_H
