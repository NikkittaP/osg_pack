/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSMalloc.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Memory handling routines.
** EDITS:
** [01] sjc 30Apr00 Merged Mac SDK port
** [02] sjc 15Jan02 Solaris port mods
** [03] tfl 02Jul04 Added util function to convert enum cell type to cell size
 *******************************************************/

#ifndef NCSMALLOC_H
#define NCSMALLOC_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif

#ifdef MACINTOSH

#include <stdlib.h>
#include <string.h>

#elif defined MACOSX
#include <stdlib.h>
#include <string.h>

#else /* MACINTOSH */

#include <malloc.h>

#if defined SOLARIS || defined LINUX || defined HPUX
#include <string.h>
#include <alloca.h>
#endif /* MACINTOSH */

#endif // SOLARIS

typedef struct {
#ifdef _WIN32
	HANDLE hMem;
	HANDLE hSize;
#endif
	void	*pData;
	UINT64  nSize;
} NCSSHMInfo;

/*
** Prototypes.
*/
void *NCSMalloc(UINT32 iSize, BOOLEAN bClear);
void *NCSRealloc(void *pPtr, UINT32 iSize, BOOLEAN bClear);
void NCSFree(void *pPtr);
INT32 NCSPhysicalMemorySize(void);
INT64 NCSPhysicalMemorySize64(void);
INT32 NCSPhysicalMemorySizeAvailable(void);	// Against bug EMS-407, added 02/11/2011
INT64 NCSPhysicalMemorySizeAvailable64(void); // Against bug EMS-407, added 02/11/2011
UINT16 NCSByteSwap16(UINT16 n);
UINT32 NCSByteSwap32(UINT32 n);
UINT64 NCSByteSwap64(UINT64 n);
void NCSByteSwapRange16(UINT16 *pDst, UINT16 *pSrc, INT32 nValues);
void NCSByteSwapRange32(UINT32 *pDst, UINT32 *pSrc, INT32 nValues);
void NCSByteSwapRange64(UINT64 *pDst, UINT64 *pSrc, INT32 nValues);
void NCSCopyMemory(UINT8 *pDst, UINT8 *pSrc, INT32 nLen);
void NCSZeroMemory(UINT8 *p, INT32 nLen);

#ifndef PALM
#define NCSMemSet(p, v, c)	memset((p), (v), (c))
#define NCSMemMove(d, s, c)	memmove((d), (s), (c))
#else	/* PALM */
#define NCSMemSet(p, v, c)	MemSet((void*)(p), (Int32)(c), (UInt8)(v))
#define NCSMemMove(d, s, c)	MemMove((void*)(d), (const void*)(s), (Int32)(c))
#endif	/* PALM */

/*
** Shared memory
*/
NCSSHMInfo *NCSMapSHM(UINT64 nMapSize, BOOLEAN bCreateUnique, char *pMapName);
void NCSUnmapSHM(NCSSHMInfo *pInfo);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif	/* NCSMALLOC_H */
