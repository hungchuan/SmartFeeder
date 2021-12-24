/*****************************************************************************
 *   type.h:  Type definition Header file for NXP LPC230x Family 
 *   Microprocessors
 *
 *   Copyright(C) 2006, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2006.09.01  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __TYPE_H__
#define __TYPE_H__


#ifndef Boolean
#define Boolean bool
#endif

#ifndef True
#define True true
#endif

#ifndef False
#define False false
#endif

#ifndef OPEN
#define OPEN HIGH
#endif

#ifndef CLOSE
#define CLOSE LOW
#endif

#ifndef UBYTE
#define UBYTE unsigned char
#endif

#ifndef UINT
#define UINT unsigned int
#endif

//###################################################
//##### 		macro define		            #####
//###################################################
#ifndef SIZEOF(x)
#define SIZEOF(x)	(sizeof(x)/sizeof(x[0]))
#endif





#endif  /* __TYPE_H__ */
