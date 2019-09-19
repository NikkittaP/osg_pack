/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     NCSBufferedIOStream.h
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CIOStream class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSBUFFEREDIOSTREAM_H
#define NCSBUFFEREDIOSTREAM_H

#ifndef NCSIOSTREAM_H
#include "NCSIOStream.h"
#endif // NCSIOSTREAM_H

#ifndef NCSUTIL_H
#include "NCSUtil.h"
#endif // NCSUTIL_H

namespace NCS {

class NCSECW_IMPEX CBufferedIOStream: public CIOStream {
public:
	explicit CBufferedIOStream(UINT32 nMaxIOCache = 4096);
	virtual ~CBufferedIOStream();
	virtual CBufferedIOStream *Clone();

	virtual CError Open(const CString &Name, bool bWrite = false);
	virtual CError Open(const CString &Name, bool bWrite, bool bCreateNew, bool bSharedWrite);
	virtual CError Open(CIOStream *pChild);
	virtual CError Close();

	virtual bool Seek(INT64 offset, Origin origin = CURRENT);
	virtual bool Seek();
	virtual INT64 Tell();
	virtual INT64 Size();

	virtual bool Read(void* buffer, UINT32 count);
	virtual bool Read(UINT8 &Buffer);
	virtual bool Write(void* buffer, UINT32 count);

	UINT32 GetIOCacheSize();
	void SetIOCacheSize(UINT32 nBytes);
	virtual bool IsBuffered() { return(true); }

protected:
	CIOStream *m_pChild;

	UINT8 *m_pIOCache;
	bool   m_bCached;

	bool   m_bInternallyOwnedChild;

	INT64  m_nCached_Start;
	UINT32 m_nIOWriteCache;
	UINT32 m_iIOReadCache;
	UINT32 m_nMaxIOCache;
	INT64	m_nFileSize;
	bool   m_bIsSharedWrite;

	virtual __inline void WriteFlush() {
			if(m_bWrite && m_pIOCache && m_nIOWriteCache) {
				if(m_pChild->Write(m_pIOCache, m_nIOWriteCache)) {
					m_nOffset += m_nIOWriteCache;
					m_nFileSize = NCSMax(m_nFileSize, m_nOffset);
					m_nIOWriteCache = 0;
				} else {
					*static_cast<CError*>(this) = *m_pChild;
				}
			} 
		}
	virtual __inline void ReadFlush() {
				// Current just invalidate the cache.
			m_iIOReadCache = GetIOCacheSize();
		}

};
}

#endif // !NCSBUFFEREDIOSTREAM_H
