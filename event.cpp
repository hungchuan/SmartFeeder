#include "general.h"

void Evnet_Init(void)
{
  CQ_Init();
}

//================================================================================================
T_MN_Event Event_GetEvent(void **ptr)
{
	T_CQ_Status status;
	unsigned short event;
	unsigned long  lparam;
	unsigned short wparam;
/*
   do
   {
		status = CQ_DeQueueMessage((unsigned short *)&event, &lparam, &wparam, ptr);
   }
   while(status == CQ_QueueEmpty);
*/
    status = CQ_DeQueueMessage((unsigned short *)&event, &lparam, &wparam, ptr);
	
	if (CQ_QueueEmpty == status)
      return ((T_MN_Event)0xff);

 	/* return event type  */
	return((T_MN_Event)event);
}
//================================================================================================
Boolean Event_EventSend (T_MN_Event event, void *data)
{
	T_CQ_Status status;
	unsigned long	lparam = 0;	/* dummy parameter */
	unsigned short	wparam = 0;	/* dummy parameter */
	Boolean retcode = True;

	status = CQ_QueueMessage((unsigned short)event, lparam, wparam, data);
	if (status == CQ_QueueFull)
		retcode = False;

	return(retcode);
}
