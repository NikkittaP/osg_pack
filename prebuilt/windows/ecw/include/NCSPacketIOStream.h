/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     NCSJP2IOStream.h 
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CECWPIOStream class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSPACKETIOSTREAM_H
#define NCSPACKETIOSTREAM_H

#ifndef NCSMEMORYIOSTREAM_H
#include "NCSMemoryIOStream.h"
#endif // NCSMEMORYIOSTREAM_H
#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif // NCSTYPES_H
#ifndef NCSTHREADPOOL_H
#include "NCSThreadPool.h"
#endif // NCSTHREADPOOL_H

namespace NCS {
namespace SDK {

class NCSECW_IMPEX PacketIOStream: public CMemoryIOStream {
public:
	class CStatusList;

	class NCSECW_IMPEX CPacketStatus {
	public:
		typedef enum {
			NONE		= 0,
			REQUESTED,
			RECEIVED,
			DECODED
		} Status;	

		CPacketId	m_nPacket;
		Status		m_eStatus;
	};	

	PacketIOStream();
	virtual ~PacketIOStream();

	virtual bool IsPacketStream() { return(true); };
	virtual void RequestPacket(CPacketId id) = 0;
	virtual void CancelPacket(CPacketId id) = 0;
	// added on 04/02/2013
	virtual void RequestPackets(std::vector<CPacketId> &RequestPackets) = 0;
	virtual void CancelPackets(std::vector<CPacketId> &CancelPackets) = 0;
	virtual CPacketStatus::Status GetPacketStatus(CPacketId nPacket);
	virtual void PurgePacketsByStatus(CPacketStatus::Status eStatus);
	virtual void SetPacketStatus(CPacketId nPacket, CPacketStatus::Status eStatus);
	virtual bool SetRequestedIfNone(CPacketId nPacket);

	virtual void LockStatus() {
		m_mStatusLock.Lock();
	}
	virtual void UnLockStatus() {
		m_mStatusLock.UnLock();
	}
	virtual bool TryLockStatus() {
		return m_mStatusLock.TryLock();
	}
protected:
	CStatusList *m_pPackets;
	CMutex m_mStatusLock;
};

}
}
#endif // !NCSPACKETIOSTREAM_H

