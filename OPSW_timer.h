#ifndef _OPSW_TIMER_H
#define _OPSW_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TMR_TICK
#define TMR_TICK         100    /* Timer tick = 100 msec, value in milliseconds*/  
#endif 


typedef enum {TMR_Cyclic, TMR_OneShot} T_TMR_Mode;	/* Timer modes */
typedef enum {
	TMR_OK = 250, TMR_InvalidId, TMR_NotActive, TMR_NoFreeEntry
	} T_TMR_Error;
typedef void (*T_TMR_Funcptr)(unsigned short param1);/* Timer callback function */
typedef unsigned char T_TMR_Timer;							/* Timer id */

void TMR_Init(void);
void TMR_TimerIntHandler(void);
T_TMR_Timer TMR_TimerAdd(  T_TMR_Mode mode,
					  T_TMR_Funcptr callbackfunction,
                           unsigned short param1   );
T_TMR_Error TMR_TimerStart(T_TMR_Timer timer_id, unsigned short msecDuration);
T_TMR_Error TMR_TimerStop(T_TMR_Timer timer_id);
T_TMR_Error TMR_TimerRemove(T_TMR_Timer timer_id);
unsigned long TMR_GetMSecCounter(void);
void TMR_Delay(unsigned short msecDelay);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //_OPSW_TIMER_H
