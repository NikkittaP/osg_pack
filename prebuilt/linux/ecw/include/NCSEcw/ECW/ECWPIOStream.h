/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/

#ifndef NCSECWECWPIOSTREAM_H
#define NCSECWECWPIOSTREAM_H
#if !defined(ECWP2_DISABLED)
#include "NCSEcw/SDK/ECWPIOStream.h"

namespace NCS {
namespace ECW {
class CFile;

/**
 * CECWPIOStream class - 64bit ECWP IO "Stream" class.
 * This class is the IO mechanism used to access JP2 files via ECWP.
 * << and >> Operators are not implemented as we can't use exception handling for portability reasons.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: rrogers $ $Date: 2009/03/17 $ 
 */
class NCSECW_IMPEX CECWPIOStream: public SDK::CECWPIOStream {

public:
		/** Constructor, initialises members */
	CECWPIOStream(ECW::CFile *pFile, const CString &URLPath, const CString &HttpHeaders);

	//TODO: fix and remove me
	virtual bool ValidateIfReallyReceived(CPacketId nPacket);

protected:

	CError OpenInternal(UINT8 **ppHeaderMemImage, 
								   UINT32 *pnHeaderMemImageLen, 
								   pNCSnetClient *pClientID, 
								   NCSClientUID		*pClientUID,
								   NCSnetPacketSentCB *pSentCB,
								   NCSnetPacketRecvCB *pRecvCB,
								   void *pCBData, 
								   NCSTChar *szUrlPath, const CString &HttpHeaders);

};
#ifdef NOT_DEF
class NCSECW_IMPEX CECWPIOStreamECWOpacity: public SDK::CECWPIOStream {
public:
		/** Constructor, initialises members */
	CECWPIOStreamECWOpacity(ECW::CFile *pFile, const CString &URLPath, const CString &HttpHeaders, void *pData, UINT32 nDataLen);

	virtual CError Open(const CString &URLPath, bool bWrite);
	virtual CError Close();

	virtual CError SendRequests();

	virtual void ProcessReceivedPackets();

	virtual void ReceivedBlock(CPacketId nPacket, UINT8* pImage, UINT32 nBlockLength);

	virtual void HookStream(pNCSnetClient ClientNetID, NCSClientUID ClientUID, NCS::CMutex *pParentStreamMutex) {
		m_ClientNetID = ClientNetID;
		m_ClientUID = ClientUID;
		m_pSendPacket = (NCSPacket *) NCSMalloc( NCSECW_MAX_SEND_PACKET_SIZE + 1 , FALSE);
		m_pParentStreamMutex = pParentStreamMutex;
	}

protected:

	ECW::CFile		*m_pFile;
	NCS::CMutex		*m_pParentStreamMutex;
};
#endif
}} // namespace
#else
#error This file should not be included if ECWP2_DISABLED
#endif //!defined(ECWP2_DISABLED)

#endif
