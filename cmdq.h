
#ifndef _CQ_H
#define _CQ_H

/* Defines */
/* ------- */



/* Types */
/* ----- */

typedef enum {CQ_OperationOK, CQ_QueueFull, CQ_QueueEmpty} T_CQ_Status;

/* Prototypes */
/* ---------- */

void	CQ_Init(void);
T_CQ_Status CQ_QueueMessage (unsigned short	MsgCode,
			 						  unsigned long	lparam,
			 						  unsigned short	wparam,
			 						  void *ptr);
T_CQ_Status CQ_PeekMessage (unsigned short *MsgCode,
                            unsigned long  *lparam,
			 						 unsigned short *wparam,
			 						 void **ptr);
T_CQ_Status CQ_DeQueueMessage (unsigned short *MsgCode,
			 							 unsigned long *lparam,
			 							 unsigned short *wparam,
			 							 void **ptr	);


#endif /* _CQ_H */
