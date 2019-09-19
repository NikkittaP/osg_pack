/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSUtil.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Utility library header.
** EDITS:
** [01] ny  23May00 Modified NCSGetUserAndDomainName to be cross platform
** [02] ny  30Oct00 Merge CE/Palm modification in preparation for Macintosh port
** [03] ny  23Nov00 Mac port changes (Added NCSstrcasecmp)
** [04] sjc 28Aug01 Added NCSIsService() to detect if running as a service under WinNT/2K/XP Server.
** [05] sjc 11Mar02 Added NCSIsWebService() to detect if running as a web service under WinNT/2K/XP Server.
** [06] ajd 01Jul02 Added NCSNanVal() to generate NaNs
** [07] sjc 12Feb03 Added NCSLog2() to calculate log2() function
** [08] tfl 06Sep05 Added NCSErrorFiniThread() proto to quell compiler warning
 *******************************************************/

#ifndef NCSUTIL_H
#define NCSUTIL_H

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif
#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif
#ifndef NCSDYNAMICLIB_H
#include "NCSDynamicLib.h"
#endif
#ifndef NCSERRORS_H
#include "NCSErrors.h"
#endif
#ifndef NCSMALLOC_H
#include "NCSMalloc.h"
#endif
#ifndef NCSARRAY_H
#include "NCSArray.h"
#endif
#ifndef NCSTIMESTAMP_H
#include "NCSTimeStamp.h"
#endif
#ifndef NCSMUTEX_H
#include "NCSMutex.h"
#endif
#ifndef NCSTHREAD_H
#include "NCSThread.h"
#endif
#ifndef NCSPACKETS_H
#include "NCSPackets.h"
#endif
#ifndef NCSPREFS_H
#include "NCSPrefs.h"
#endif
#ifndef NCSFILEIO_H
#include "NCSFileIO.h"
#endif
#ifndef NCSMISC_H
#include "NCSMisc.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

void NCSErrorFiniThread(void); /**[08]**/
void NCSSleep(NCSTimeStampMs tsSleepTime);
char *NCSStrDup(const char *pString);
NCSTChar *NCSStrDupT(const NCSTChar *pString);
NCSError NCSFormatCoordStringsEN(double dEasting, double dNorthing, char **ppEastString, char **ppNorthString);
NCSError NCSFormatCoordStringsLL(double dEasting, double dNorthing, char **ppEastString, char **ppNorthString);
UINT32   NCSGetNumCPUs(void);
UINT32 NCSGetNumPhysicalCPUs(void);
#ifdef _WIN32
UINT32 NCSGetWebServerID(const char *szPath, DWORD nPathLen);
#endif
BOOLEAN NCSIsNaN(IEEE8 dDoubleValue);
BOOLEAN NCSIsFinite(IEEE8 dDoubleValue);
BOOLEAN NCSIsService(void);		/**[04]**/

BOOLEAN NCSIsIWS(void);
void NCSSetIsIWS( BOOLEAN bValue );

double NCSNanVal(void);	/**[06]**/
BOOLEAN NCSDoubleIsNan(double v);

void NCSUtf8Encode (const wchar_t *pSrc, char **ppDest);
void NCSUtf8Decode (const char *pSrc, wchar_t **ppDest);

#ifdef _WIN32

#define NCSMessageBox(Window, Text, Caption, Type) ::MessageBox(Window, Text, Caption, Type)
#define NCSMessageBoxA(Window, Text, Caption, Type) ::MessageBoxA(Window, Text, Caption, Type)

#endif /* _WIN32 */

#ifdef __cplusplus
}
#endif

#endif /* NCSUTIL_H */
