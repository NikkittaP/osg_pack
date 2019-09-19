/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSTimeStamp.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Memory handling routines.
** EDITS:
 *******************************************************/

#ifndef NCSTIMESTAMP_H
#define NCSTIMESTAMP_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif

extern NCSTimeStampMs NCSGetTimeStampMs(void);
extern NCSTimeStampUs NCSGetTimeStampUs(void);

#ifdef __cplusplus
}
#endif

#endif /* NCSTIMESTAMP_H */
