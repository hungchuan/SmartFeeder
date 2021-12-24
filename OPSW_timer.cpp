#include <assert.h>
#include "general.h"


#define TMR_MAXTIMERS                     20		/* max active timers */
#define MIN_TICKS_FOR_CYCLIC_TIMERS       2
#define MIN_TICS_FOR_ONESHOOT_TIMERS      1


typedef struct {
	Boolean active;
	Boolean started;
	T_TMR_Mode mode;
	unsigned short duration;	/* in TMR_TICK Units */
	unsigned short counter;		/* in TMR_TICK Units */
	T_TMR_Funcptr func;			/* callback function */
	unsigned short param1;		/* function's parameter */
	} T_TMR_Entry;

static T_TMR_Entry TMR_Array[TMR_MAXTIMERS];		      /* List of all timers */
static T_TMR_Timer StartedTimersList[TMR_MAXTIMERS];  /* Array of started timers ID */
static unsigned short StartedTimersNumber;            /* Number of started timers */
static unsigned short RelativeCounter;
static unsigned long msecCounter;

static T_TMR_Timer TMR_Alloc(void);
static void TMR_Free(T_TMR_Timer timer_id);
void TMR_TimerIntHandler(void);
static void TMR_AddToStartedTimersList(T_TMR_Timer TimerID);
static void TMR_RemoveFromStartedTimersList(T_TMR_Timer TimerID);

#define TMR_Enable   timer_enable()
#define TMR_Disable  timer_disable()


void TMR_Init(void)
{
	int i;

   RelativeCounter = 0;
   StartedTimersNumber = 0;
   msecCounter = 0;

	/* Init module vars */
	for (i = 0;i < TMR_MAXTIMERS;i++ )
	  {
	  TMR_Array[i].active = False;
	  TMR_Array[i].started = False;
	  }

}  /* TMR_Init */
//================================================================================================
void IRAM_ATTR TMR_TimerIntHandler(void)
{
   unsigned short i;
   T_TMR_Timer NearestTimer;

   /* Disable TMR interrupt */
   TMR_Disable;

   msecCounter += TMR_TICK;

   if(StartedTimersNumber > 0)
   {
       NearestTimer = StartedTimersList[StartedTimersNumber-1];
       RelativeCounter++;

       if(! (--TMR_Array[NearestTimer].counter) )
       {
          /* Decrease (if it possible) all other started timers by RelativeCounter */
          for(i=0; i<StartedTimersNumber; i++)
          {
             if(TMR_Array[StartedTimersList[i]].counter > RelativeCounter)
                TMR_Array[StartedTimersList[i]].counter -= RelativeCounter;
             else
                TMR_Array[StartedTimersList[i]].counter = 1;
          }

          /* Clear RelativeCounter */
          RelativeCounter = 0;

          /* Decrease StartedTimersNumber = Remove serviced timer from the started timers list */
          StartedTimersNumber--;

          if(TMR_Array[NearestTimer].mode == TMR_Cyclic)
          {
             /* Restart serviced timer with required duration */
             TMR_Array[NearestTimer].counter = TMR_Array[NearestTimer].duration;

             /*
                Add serviced timer to the started timers list
                (sorted by counter list, last entry contain timer that will be service first)
             */
             TMR_AddToStartedTimersList(NearestTimer);
          }
          else
          {
             /* Clear start flag of the serviced timer */
             TMR_Array[NearestTimer].started = false;
          }

          /* Execute required routine */
          TMR_Array[NearestTimer].func(TMR_Array[NearestTimer].param1);

       }
   }

   /* Enable TMR interrupt */
   TMR_Enable;
 
}	/*TMR_TimerIntHandler */
//================================================================================================
T_TMR_Timer TMR_TimerAdd(  T_TMR_Mode mode,
					  T_TMR_Funcptr callbackfunction,
                           unsigned short param1   )
{
	T_TMR_Timer TimerId;

	/* Allocate a new entry and initialize it according to input parameters  */
	TimerId = TMR_Alloc();
	if (TimerId == TMR_MAXTIMERS)
		return(TMR_NoFreeEntry);

	TMR_Array[TimerId].mode = mode;
	TMR_Array[TimerId].func = callbackfunction;
	TMR_Array[TimerId].param1 = param1;

	TMR_Array[TimerId].active = True;

	return(TimerId);

}	/*TMR_TimerAdd */
//================================================================================================
T_TMR_Error TMR_TimerStart(T_TMR_Timer timer_id, unsigned short msecDuration)
{
	/* Check params validity */
	if (timer_id >= TMR_MAXTIMERS)
		return(TMR_InvalidId);
	if (!TMR_Array[timer_id].active)
		return(TMR_NotActive);


	/* Timer interrupt disable  */
	TMR_Disable;

   // if Timer already started, stop it.
   if(TMR_Array[timer_id].started)
   {
       /* Remove from started timers list */
       TMR_RemoveFromStartedTimersList(timer_id);

        /* Flag timer as stopped */
        //TMR_Array[timer_id].started = False;
   }

	/* Set duration             */
	TMR_Array[timer_id].duration = msecDuration / TMR_TICK;

   switch(TMR_Array[timer_id].mode)
   {
      case TMR_Cyclic:
         if(TMR_Array[timer_id].duration < MIN_TICKS_FOR_CYCLIC_TIMERS)
            TMR_Array[timer_id].duration = MIN_TICKS_FOR_CYCLIC_TIMERS;
      break;

      case TMR_OneShot:
         if(TMR_Array[timer_id].duration < MIN_TICS_FOR_ONESHOOT_TIMERS)
            TMR_Array[timer_id].duration = MIN_TICS_FOR_ONESHOOT_TIMERS;
      break;
   }

	/* Flag timer as started and reset counter */
	TMR_Array[timer_id].started = True;
	TMR_Array[timer_id].counter = TMR_Array[timer_id].duration;

   /* Add to started timers list */
   TMR_AddToStartedTimersList(timer_id);

	/* Timer interrupt enable */
	TMR_Enable;

	return(TMR_OK);

}	/*TMR_TimerStart */
//================================================================================================
T_TMR_Error TMR_TimerStop(T_TMR_Timer timer_id)
{
	/* Check params validity */
	if (timer_id >= TMR_MAXTIMERS)
		return(TMR_InvalidId);
	if (!TMR_Array[timer_id].active)
		return(TMR_NotActive);

	/* Timer interrupt disable  */
	TMR_Disable;

   /* Remove from started timers list */
   TMR_RemoveFromStartedTimersList(timer_id);

	/* Flag timer as stopped */
	TMR_Array[timer_id].started = False;

	/* Timer interrupt enable */
	TMR_Enable;

	return(TMR_OK);

}	/*TMR_TimerStop */
//================================================================================================
T_TMR_Error TMR_TimerRemove(T_TMR_Timer timer_id)
{
	/* Check params validity */
	if (timer_id >= TMR_MAXTIMERS)
		return(TMR_InvalidId);
	if (!TMR_Array[timer_id].active)
		return(TMR_NotActive);

   if(TMR_Array[timer_id].started)
      TMR_TimerStop(timer_id);

	TMR_Free(timer_id);

	return(TMR_OK);

}	/*TMR_TimerRemove */
//================================================================================================
static T_TMR_Timer TMR_Alloc(void)
{
	T_TMR_Timer id;

	/* Search for a free entry */
	for (id = 0;id < TMR_MAXTIMERS;id++ )
	  	{
	  	if (!TMR_Array[id].active)
	  		return(id);
	  	}

	/* No free entry */
	return(TMR_MAXTIMERS);

}
//================================================================================================
void TMR_Free(T_TMR_Timer timer_id)
{
	/* Timer interrupt disable  */
	TMR_Disable;

	/* Flag timer as inactive     */
	TMR_Array[timer_id].active = False;

	/* Timer interrupt enable   */
	TMR_Enable;
}
//================================================================================================
void IRAM_ATTR TMR_AddToStartedTimersList(T_TMR_Timer TimerID)
{
   int i, nRequiredEntry;

   /* Find required for TimerID entry */
   for(i=0; i<StartedTimersNumber; i++)
   {
      if(TMR_Array[StartedTimersList[i]].counter <= TMR_Array[TimerID].counter)
         break;
   }

   /* Store required for TimerID entry */
   nRequiredEntry = i;

   /* Shift remains entries */
   for(i=StartedTimersNumber; i>nRequiredEntry; i--)
      StartedTimersList[i] = StartedTimersList[i-1];

   /* Insert TimerID to required entry */
   StartedTimersList[nRequiredEntry] = TimerID;

   /* Increment number of active timers */
   StartedTimersNumber++;
}
//================================================================================================
void TMR_RemoveFromStartedTimersList(T_TMR_Timer TimerID)
{
   int i;

   /* Find required for TimerID */
   for(i=0; i<StartedTimersNumber; i++)
   {
      if(StartedTimersList[i] == TimerID)
      {
	      /* Decrease the number of started timers */
         StartedTimersNumber--;

         /* Shift remains entries */
         for( ; i<StartedTimersNumber; i++)
            StartedTimersList[i] = StartedTimersList[i+1];

         break;
      }
   }
}
//================================================================================================
unsigned long TMR_GetMSecCounter(void)
{
   return msecCounter;
}
//================================================================================================
void TMR_Delay(unsigned short msecDelay)
{
delay(msecDelay);
}
//================================================================================================
