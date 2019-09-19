/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCScnet.h
** CREATED:	Thu Mar 4 09:19:00 WST 1999
** AUTHOR: 	Doug Mansell
** PURPOSE:	IWS client side networking - public interface
** EDITS:
** [01] sjc 01Oct99 Protocol version define
** [02] sjc 25Feb00 Chenged NCS_PROTOCOL_VERSION to 3
** [03] ny  12May00 Added bIsConnected parameter to NCScnetSendPacket
**					to enable the propagation of loss of connection
**					error to the upper level
** [04] jmp 06Mar01 Added generic NCScnetPostURL
** [05] rar 14May01 Added NCSUrlStream.
** [06] rar 17-May-01 Modified NCSnetPacketRecvCB() to include a returned error code.
** [07] rar 28May01 Added sending of ping packet by client,
**					incremented version to 4.
** [08] cmt 12Mar10 Added opacity as appended stream, bumped protocol to v5
 *******************************************************/

#ifndef NCSCNET_H
#define NCSCNET_H


#include "NCSTypes.h"
#include "NCSErrors.h"
#ifdef __cplusplus
#include "NCSError.h"
#include "NCSHttpClient.h"
#endif
#ifdef NCSCNET3
	#ifdef __cplusplus
	#include "NCSUrlStream.h"
	#endif
#endif

#if defined(_WIN32)
typedef SIZE_T DWORD_PTR, *PDWORD_PTR;
typedef LPVOID HINTERNET;
#elif defined(POSIX)
typedef void* HINTERNET;
#else
Not implemented
#endif //_WIN32



#ifndef NCS_MAX_PACKET_SIZE
#define NCS_MAX_PACKET_SIZE (1024*1024)
#endif
#ifndef NCS_PROTOCOL_VERSION
/*
** Version of the wire protocol.  keep in sync with NCSsnet.h [01]
*/
#define NCS_PROTOCOL_VERSION	5 /**[07][08]**/
#endif

typedef void *pNCSnetClient;
typedef void NCSnetPacketRecvCB(void *, INT32, pNCSnetClient, void *, NCSError eError);	 /**[06]**/	// pPacket, nLength, pClient, pUserData, eError
typedef void NCSnetPacketSentCB(NCSError, void *, void *);					// eError, pPacket, pUserData

typedef int (*NCScnetWriteCallback)(void *szCurrent,
                                      unsigned long nSize,
                                      void *szAllData,
									  void *pUserData);

/////
#define MEMBLOCKSIZE 1024*100
#ifdef __cplusplus
extern "C" {
#endif

#ifdef NCS_BUILD_UNICODE
BOOLEAN NCScnetVerifyURLW(const wchar_t *szURL);
NCSError NCScnetCreateW(const wchar_t *szURL, void **ppClient, void *pPacketIn, int nLengthIn,
						void **ppPacketOut, INT32 *pLengthOut, NCSnetPacketRecvCB *pRecvCB, void *pRecvUserdata,
						wchar_t *pszHttpHeaders, BOOLEAN *bSentAuth);
NCSError NCScnetCreateExW(const wchar_t *szURL, void **ppClient, void *pPacketIn, int nLengthIn,
						void **ppPacketOut, INT32 *pLengthOut, NCSnetPacketRecvCB *pRecvCB, void *pRecvUserdata,
						const wchar_t *pszIISDLLName, wchar_t *pszHttpHeaders, BOOLEAN *bSentAuth);
NCSError NCScnetCreateProxyW(const wchar_t *szURL, void **ppClient, void *pPacketIn, int nLengthIn,
						void **ppPacketOut, INT32 *pLengthOut, NCSnetPacketRecvCB *pRecvCB, void *pRecvUserdata, const wchar_t *pszIISDLLName, 
						const wchar_t *pszUsername, const wchar_t *pszPassword, wchar_t *pszHttpHeaders, BOOLEAN *bSentAuth );
#endif


BOOLEAN NCScnetVerifyURLA(const char *szURL);
NCSError NCScnetCreateA(const char *szURL, void **ppClient, void *pPacketIn, int nLengthIn,
						void **ppPacketOut, INT32 *pLengthOut, NCSnetPacketRecvCB *pRecvCB, void *pRecvUserdata,
						char *pszHttpHeaders, BOOLEAN *bSentAuth);
NCSError NCScnetCreateExA(const char *szURL, void **ppClient, void *pPacketIn, int nLengthIn,
						void **ppPacketOut, INT32 *pLengthOut, NCSnetPacketRecvCB *pRecvCB, void *pRecvUserdata,
						const char *pszIISDLLName, char *pszHttpHeaders, BOOLEAN *bSentAuth);
NCSError NCScnetCreateProxyA(const char *szURL, void **ppClient, void *pPacketIn, int nLengthIn,
						void **ppPacketOut, INT32 *pLengthOut, NCSnetPacketRecvCB *pRecvCB, void *pRecvUserdata,
						const char *pszIISDLLName, const char *pszUsername, const char *pszPassword, char *pszHttpHeaders, BOOLEAN *bSentAuth );
#ifdef UNICODE

#define NCScnetVerifyURL NCScnetVerifyURLW
#define NCScnetCreate NCScnetCreateW
#define NCScnetCreateEx NCScnetCreateExW
#define NCScnetCreateProxy NCScnetCreateProxyW

#else

#define NCScnetVerifyURL NCScnetVerifyURLA
#define NCScnetCreate NCScnetCreateA
#define NCScnetCreateEx NCScnetCreateExA
#define NCScnetCreateProxy NCScnetCreateProxyA

#endif // UNICODE

void NCScnetSetSendBlocking(void *pConnection, BOOLEAN bOn);
BOOLEAN NCScnetSpawnThreads(void *pConnection);
void NCScnetSetRecvCB(pNCSnetClient pClient, NCSnetPacketRecvCB *pRecvCB, void *pUserdata);
void NCScnetSetSentCB(pNCSnetClient pClient, NCSnetPacketSentCB *pSentCB);
BOOLEAN NCS_EXPORT NCScnetIsPollingConnection(pNCSnetClient pClient);
BOOLEAN NCScnetSendPacket(pNCSnetClient pClient, void *pPacket, int nLength, void *pUserdata, BOOLEAN *bIsConnected);
void NCScnetCloseHandle(HINTERNET hInternet);
void NCScnetDestroy(pNCSnetClient pClient);
void NCScnetInit();
void NCScnetShutdown();
void NCScnetSetProxy(const wchar_t *pProxy, INT32 nPort, const wchar_t *pProxyBypass);

//
// Function specifically required for use by the IWS Proxy
//
#ifdef PROXYSERVER
void NCSUseWinHTTP( BOOL bValue );
#endif //PROXYSERVER

#ifdef __cplusplus
};
#endif

#endif
