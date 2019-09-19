/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     NCSJP2IOStream.h 
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CIOStream class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSMEMORYIOSTREAM_H
#define NCSMEMORYIOSTREAM_H

#ifndef NCSIOSTREAM_H
#include "NCSIOStream.h"
#endif // NCSIOSTREAM_H

namespace NCS {

class NCSECW_IMPEX CMemoryIOStream: public CIOStream {
public:
	explicit CMemoryIOStream(bool bSeekable = true);
	virtual ~CMemoryIOStream();

	virtual CMemoryIOStream *Clone();
	virtual CError Open(const void *pMemory, UINT32 nMemoryLength, bool bOwnStream = false, bool bWrite = false, const NCS::CString& sName = CIOStream::GenerateUniqueName());
	virtual CError Open(CIOStream &Stream, UINT32 nMemoryLength, const NCS::CString& sName = CIOStream::GenerateUniqueName());
	virtual CError Close();

	virtual bool Seek(INT64 offset, Origin origin = CURRENT);
	virtual bool Seek();
	virtual INT64 Size();

	virtual bool Read(void* buffer, UINT32 count);
	virtual bool Read(INT64 offset, void* buffer, UINT32 count);
	virtual bool Write(void* buffer, UINT32 count);

	virtual bool IsPacketStream() { return(false); };
	virtual bool IsBuffered() { return(true); };
	virtual bool IsWrite() { return(m_bWrite); };
	virtual void *GetMemory() { return(m_pMemory); };

	// added on 13/02/2013
	virtual UINT32 GetMemoryLength() { return(m_nMemoryLength); };
	virtual bool IsOwnMemory() { return(m_bOwnMemory); };

protected:
	void *m_pMemory;
	UINT32 m_nMemoryLength;
	bool	m_bSeekable;
	bool	m_bOwnMemory;
};
}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CMemoryIOStream CNCSJPCMemoryIOStream;
#endif

#endif // !NCSMEMORYIOSTREAM_H
