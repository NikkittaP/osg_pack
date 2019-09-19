/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     ECWPacketStream.h 
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CIOStream class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSECWPACKETSTREAM_H
#define NCSECWPACKETSTREAM_H

#ifndef NCSJPCPACKETIOSTREAM_H
#include "NCSPacketIOStream.h"
#endif // NCSJPCPACKETIOSTREAM_H
#ifndef NCSFILEIOSTREAM_H
#include "NCSFileIOStream.h"
#endif // NCSFILEIOSTREAM_H
#include "NCSEcw/ECW/File.h"
#ifndef NCSUTIL_H
#include "NCSUtil.h"
#endif // NCSUTIL_H

namespace NCS {
namespace ECW {

/**
 * CPacketStream class - 64bit ECW "Packet Stream" class.
 * This class is the Asynchronous IO mechanism used to access local ECW files.
 * 
 * @author       Simon Cope
 * @version      $Revision: #3 $ $Author: scope $ $Date: 2007/12/14 $ 
 */
class NCSECW_IMPEX CPacketStream: public SDK::PacketIOStream_T<CFileIOStream, NCSBlockId>, private CMutex {
public:


	//typedef SDK::HashList_T<NCSBlockId, CPacketStatus, (NCSBlockId)-1>::iterator CPacketStatusIterator;

		/** Constructor, initialises members */
	CPacketStream(ECW::CFile *pFile, const CString &URLPath);
		/** Virtual destructor, releases members */
	virtual ~CPacketStream();

		/** 
		 * Open the stream on the specified file.
		 * @param		pEcwpURL		ECWP URL
		 * @return      CError	NCS_SUCCESS, or error code on failure.
		 */
	virtual CError Open(const CString &FileName, UINT32 iFlags = NCS_FILE_READ);
		/** 
		 * Close the stream.
		 * @return      CError	NCS_SUCCESS, or error code on failure.
		 */
	virtual CError Close();
	
	virtual void RequestPacket(PacketId id);
	virtual void CancelPacket(PacketId id);

        virtual bool IsPacketStream() { return(true); }
	virtual CPacketStatus::Status GetPacketStatus(PacketId nPacket);
	virtual void SetPacketStatus(PacketId nPacket, CPacketStatus::Status eStatus);

	//virtual void ProcessReceivedPackets();
protected:
	class CJob: public SDK::CThreadPool::CJob {
	public:
		CJob(CPacketStream	*pStream, NCSBlockId id);
		virtual ~CJob();
		virtual void Work();
	protected:
		CPacketStream	*m_pStream;
		NCSBlockId		m_id;
	};
	ECW::CFile		*m_pFile;

//	typedef struct {
//		NCSPacket *pPacket;
//		INT32 nLength;
//	} ReceivedPacket;
//	std::list<ReceivedPacket*> m_ReceivedPackets;

		/** Precincts we need to request packets for */
	std::vector<PacketId> m_RequestPackets;
		/** Precincts we need to cancel packets for */
	std::vector<PacketId> m_CancelPackets;
};
}
}

#endif // !NCSECWPACKETSTREAM_H
