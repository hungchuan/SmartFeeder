
//#include <dos.h>
//#include "types.h"
//#include "cregion.h"

#include "general.h"

/* Static vars and definitions */
#define	CQ_MAX_ENTRIES	40

typedef struct {
  unsigned short	MsgCode;
  unsigned long	lparam;
  unsigned short	wparam;
  void *ptr;
  } T_CQ_EntryType;

static T_CQ_EntryType CQ_Queue[CQ_MAX_ENTRIES];
static unsigned short CQ_InsertPtr;
static unsigned short CQ_RemovePtr;
static unsigned short CQ_QueueDepth;


void CQ_Init(void)
{

  CQ_InsertPtr = 0;
  CQ_RemovePtr = 0;
  CQ_QueueDepth= 0;
}
//================================================================================================
T_CQ_Status CQ_QueueMessage (unsigned short	MsgCode,
			 							unsigned long	lparam,
			 							unsigned short	wparam,
			 							void *ptr)
{

//  CR_RegionMask SaveMask;
  T_CQ_Status Status = CQ_OperationOK;

//  SaveMask = CR_EnterCriticalRegion();

  if (CQ_QueueDepth < SIZEOF(CQ_Queue))
    	{
	   CQ_Queue[CQ_InsertPtr].MsgCode = MsgCode;
	   CQ_Queue[CQ_InsertPtr].lparam  = lparam;
	   CQ_Queue[CQ_InsertPtr].wparam = wparam;
	   CQ_Queue[CQ_InsertPtr].ptr = ptr;
	   CQ_InsertPtr = (CQ_InsertPtr + 1) % SIZEOF(CQ_Queue);
	   CQ_QueueDepth++;
	 	}
	else
		Status = CQ_QueueFull;

//  CR_ExitCriticalRegion (SaveMask);

  return Status;
}
//================================================================================================
T_CQ_Status CQ_PeekMessage (unsigned short *MsgCode,
			 						  unsigned long  *lparam,
			 						  unsigned short *wparam,
			 						  void **ptr)
{

//  CR_RegionMask SaveMask;
  T_CQ_Status Status = CQ_OperationOK;

//  SaveMask = CR_EnterCriticalRegion();

  if (CQ_QueueDepth > 0)
  	{
	   *MsgCode = CQ_Queue[CQ_RemovePtr].MsgCode;
	   *lparam  = CQ_Queue[CQ_RemovePtr].lparam;
	   *wparam  = CQ_Queue[CQ_RemovePtr].wparam;
	   *ptr    = CQ_Queue[CQ_RemovePtr].ptr;
	 	}
  else
  	Status = CQ_QueueEmpty;

//  CR_ExitCriticalRegion (SaveMask);

  return Status;
}
//================================================================================================
T_CQ_Status CQ_DeQueueMessage (unsigned short *MsgCode,
			 							  unsigned long *lparam,
			 							  unsigned short *wparam,
			 							  void **ptr	)
{

//  CR_RegionMask SaveMask;
  T_CQ_Status Status = CQ_OperationOK;


//  SaveMask = CR_EnterCriticalRegion();*/

  if (CQ_QueueDepth > 0)
  	{
	   *MsgCode  = CQ_Queue[CQ_RemovePtr].MsgCode;
	   *lparam   = CQ_Queue[CQ_RemovePtr].lparam;
	   *wparam  = CQ_Queue[CQ_RemovePtr].wparam;
	   *ptr  = CQ_Queue[CQ_RemovePtr].ptr;
	   CQ_RemovePtr = (CQ_RemovePtr + 1) % SIZEOF(CQ_Queue);
	   CQ_QueueDepth--;
	 	}
  else
  	Status = CQ_QueueEmpty;


//  CR_ExitCriticalRegion (SaveMask);

  return Status;
}

