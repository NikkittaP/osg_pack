/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/


#ifndef NCSSDKECWPIOSTREAM_H
#define NCSSDKECWPIOSTREAM_H

#include "NCSTypes.h"
#include "NCSPacketIOStream.h"
#include "NCScnet.h"
#include "NCSEcw/ECW/NCSEcw.h"
#include "NCSEcw/SDK/ECWP3Client.h"

namespace NCS {
namespace SDK {

#ifdef ECWP_CACHE
class CECWPCache;
#endif
class CFileBase;
/**
 * CECWPIOStream class - 64bit ECWP IO "Stream" class.
 * This class is the IO mechanism used to access JP2 files via ECWP.
 * << and >> Operators are not implemented as we can't use exception handling for portability reasons.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: rrogers $ $Date: 2009/03/17 $ 
 */
class NCSECW_IMPEX CECWPIOStream: public CMutex , public SDK::PacketIOStream {
public:
		/** Constructor, initialises members */
	CECWPIOStream(CFileBase *pFile, const CString &URLPath, const CString &HttpHeaders);

		/** Virtual destructor, releases members */
	virtual ~CECWPIOStream();

	virtual CError Open(const CString &URLPath, bool bWrite);
	virtual CError Close();

	virtual CError Reconnect();

	virtual void RequestPacket(CPacketId id);
	virtual void CancelPacket(CPacketId id);
	// added on 04/02/2013
	virtual void RequestPackets(std::vector<CPacketId> &RequestPackets);
	virtual void CancelPackets(std::vector<CPacketId> &CancelPackets);

	virtual CError SendRequests();

	virtual void SetPacketStatus(CPacketId nPacket, CPacketStatus::Status eStatus);

	virtual void ProcessReceivedPackets();

	virtual bool ValidateIfReallyReceived(CPacketId nPacket) { return false; };


	virtual bool IsConnected()
	{
		return m_bIsConnected;
	}

	virtual bool IsPolling()
	{
		return m_bPollingConnection;
	}
	virtual INT32 GetVersion() { return m_nVersion; };

	virtual void ReceivedBlock(CPacketId nPacket, UINT8* p, UINT32 l);
	virtual NCS::CString GetHttpHeaders() { return m_sHttpHeaders; };

	virtual NCS::CError GetStatistics(NCSFileStatistics *pDstStats) { return NCS_UNSUPPORTED_FILE_TYPE_OR_VERSION; };
	virtual NCS::CError GetMetaData(NCSFileMetaData *pDstMetaData) { return NCS_UNSUPPORTED_FILE_TYPE_OR_VERSION; };
	virtual NCS::CError GetRPCData(NCSRPCData **ppDstRPCData) { return NCS_UNSUPPORTED_FILE_TYPE_OR_VERSION; };

protected:
	virtual CError OpenInternal(UINT8 **ppHeaderMemImage, 
								   UINT32 *pnHeaderMemImageLen, 
								   pNCSnetClient *pClientID, 
								   NCSClientUID		*pClientUID,
								   NCSnetPacketSentCB *pSentCB,
								   NCSnetPacketRecvCB *pRecvCB,
								   void *pCBData, 
								   NCSTChar *szUrlPath, const CString &HttpHeaders);

	pNCSnetClient	m_ClientNetID;
	NCSClientUID	m_ClientUID;
	bool			m_bIsConnected;
	bool			m_bSendInProgress;
	NCSSequenceNr	m_nClientSequence;
	NCSPacket		*m_pSendPacket;

	CString m_sHttpHeaders;
	bool m_bPollingConnection;
	int m_nReconnections;

	CString m_sURLPath;
	CFileBase *m_pFile;
	
	INT32 m_nVersion;

#ifdef ECWP_CACHE
	CECWPCache *m_pCache;
#endif

	typedef struct {
		NCSPacket *pPacket;
		INT32 nLength;
	} ReceivedPacket;
	std::list<ReceivedPacket*> m_ReceivedPackets;
	// TODO: improve mutexing (started but commented out because not tested)
	//CMutex m_mReceivedPackets;

	// added on 18/01/2013 to speed find operation
	/** Precincts we need to request packets for */
	//typedef tbb::concurrent_hash_map<CPacketId, bool> m_RequestPacketList;
	//m_RequestPacketList m_vRequestPackets;
		/** Precincts we need to cancel packets for */
	//typedef tbb::concurrent_hash_map<CPacketId, bool> m_CancelPacketList;
	//m_CancelPacketList m_vCancelPackets;
	std::list<NCSBlockId> m_vRequestPackets;
	std::list<NCSBlockId> m_vCancelPackets;

	/** Precincts we need to request packets for */
	std::vector<CPacketId> m_RequestPackets;
		/** Precincts we need to cancel packets for */
	std::vector<CPacketId> m_CancelPackets;

	CMutex m_mRequestPackets;

	void PacketRecvCB(NCSPacket *pPacket, INT32 nLength, pNCSnetClient pClientNetID, NCSError eError);	 
	void PacketSentCB(NCSError eError, UINT8 *pLastPacketSent);			

	static void sPacketRecvCB(NCSPacket *pPacket, INT32 nLength, pNCSnetClient pClientNetID,
		CECWPIOStream *pIOStream, NCSError eError);	 
	static void sPacketSentCB(NCSError eError, UINT8 *pLastPacketSent, CECWPIOStream *pIOStream);
};

class NCSECW_IMPEX CECWP3IOStream : public CECWPIOStream, public CECWP3Client::IECWP3ClientApp {
public:
	CECWP3IOStream(CFileBase *pFile, const CString &URLPath, const CString &HttpHeaders);
	virtual ~CECWP3IOStream();

	CError Open(const CString &URLPath, bool bWrite);
	CError Close();

	void RequestPacket(CPacketId id);
	void CancelPacket(CPacketId id);
	// added on 04/02/2013
	void RequestPackets(std::vector<CPacketId> &RequestPackets);
	void CancelPackets(std::vector<CPacketId> &CancelPackets);

	virtual CError Reconnect();

	CError SendRequests();

	virtual void BlockRecv(NCSBlockId nID, void *pBlock, UINT32 nBlockLength, NCSError eError);
	virtual void BlockFailed(NCSBlockId nID, NCSError eError);

	virtual bool IsConnected() { return(m_pECWP3Client && m_pECWP3Client->IsConnected()); }

	virtual NCS::CError GetStatistics(NCSFileStatistics *pDstStats);
	virtual NCS::CError GetMetaData(NCSFileMetaData *pDstMetaData);
	virtual NCS::CError GetRPCData(NCSRPCData **ppDstRPCData);

	// added on 20/02/2013
	NCS::SDK::CECWP3Client *GetECWP3Client() {return m_pECWP3Client;};
protected:
	NCS::SDK::CECWP3Client *m_pECWP3Client;

	NCS::CMutex m_mRequestBlocks;
	std::list<NCSBlockId> m_RequestBlocks;

	//typedef tbb::concurrent_hash_map<CPacketId, bool, CPacketIdHash> m_RequestBlockList;
	//m_RequestBlockList m_vRequestBlocks;
};

}} // namespace

#endif

