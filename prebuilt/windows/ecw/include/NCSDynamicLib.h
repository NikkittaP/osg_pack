/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSdynamicLib.h
** CREATED:	Thursday 2 March 2000
** AUTHOR: 	Mark Sheridan
** PURPOSE:	Dynamic library loading routines
** EDITS:
** [01] 03Nov00	 ny	Merge WinCE/PALM SDK changes with Mac port changes
 *******************************************************/

#ifndef NCSDYNAMICLIB_H
#define NCSDYNAMICLIB_H

#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32

#define NCS_DLEXT ".dll"
#define NCS_DLEXT_NO_DOT "dll"
#define NCS_FUNCADDR FARPROC
#define NCS_DLHANDLE void *
#define NCSDL_NULL_HANDLE NULL

#elif defined PALM

#define NCS_DLEXT ".shl"
#define NCS_DLEXT_NO_DOT "shl"
typedef void (*NCS_FUNCADDR)(void);
#define NCS_DLHANDLE UInt16
#define NCSDL_NULL_HANDLE 0

#elif defined MACINTOSH

#define NCS_DLEXT ".dll"
#define NCS_DLEXT_NO_DOT "dll"
typedef void (*NCS_FUNCADDR)(void);
#define NCS_DLHANDLE void *
#define NCSDL_NULL_HANDLE NULL

#elif defined POSIX

#ifdef HPUX
#define NCS_DLEXT ".sl"
#define NCS_DLEXT_NO_DOT "sl"
#else
#define NCS_DLEXT ".so"
#define NCS_DLEXT_NO_DOT "so"
#endif
typedef void (*NCS_FUNCADDR)(void);
#define NCS_DLHANDLE void *
#define NCSDL_NULL_HANDLE NULL

#else

	ERROR: Platform not supported.

#endif	/* _WIN32 */

NCS_DLHANDLE NCSDlOpenA( const char *pLibraryName );
NCS_DLHANDLE NCSDlOpenW( const wchar_t *pLibraryName );
void NCSDlClose( NCS_DLHANDLE NCSLibararyHandle );
NCS_FUNCADDR NCSDlGetFuncAddressA(NCS_DLHANDLE NCSLibararyHandle, const char *pFunctionName);
NCS_FUNCADDR NCSDlGetFuncAddressW(NCS_DLHANDLE NCSLibararyHandle, const wchar_t *pFunctionName);
BOOLEAN NCSDlFuncExistsA(const char *pLibraryName, const char *pFunctionName);
BOOLEAN NCSDlFuncExistsW(const wchar_t *pLibraryName, const wchar_t *pFunctionName);
char *NCSDlErrorA(void);
wchar_t *NCSDlErrorW(void);

#ifdef UNICODE

#define NCSDlOpen NCSDlOpenW
#define NCSDlGetFuncAddress NCSDlGetFuncAddressW
#define NCSDlFuncExists NCSDlFuncExistsW
#define NCSDlError NCSDlErrorW

#else

#define NCSDlOpen NCSDlOpenA
#define NCSDlGetFuncAddress NCSDlGetFuncAddressA
#define NCSDlFuncExists NCSDlFuncExistsA
#define NCSDlError NCSDlErrorA

#endif

#ifdef __cplusplus
}
#endif

#endif
