/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     NCSFileIOStream.h 
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CIOStream class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSFILEIOSTREAM_H
#define NCSFILEIOSTREAM_H

#ifndef NCSIOSTREAM_H
#include "NCSIOStream.h"
#endif // NCSIOSTREAM_H

#ifndef NCSUTIL_H
#include "NCSUtil.h"
#endif // NCSUTIL_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace NCS {

class NCSECW_IMPEX CFileIOStream: public CIOStream {
public:

	static bool GetInfo(CString sFilename, UINT64 *pnSize=NULL,
		UINT64 *pnLastAccessTime=NULL, UINT64 *pnLastWriteTime=NULL, UINT64 *pnCreationTime=NULL);

	CFileIOStream();
	virtual ~CFileIOStream();
	virtual CFileIOStream *Clone();
	
	virtual CError Open(const CString &FileName, bool bWrite = false);
	virtual CError Open(CString sFilename, bool bWrite, bool bCreateNew, bool bSharedWrite);
	virtual CError Open(const CString &FileName, UINT32 iFlags = NCS_FILE_READ);
	virtual CError Close();

	virtual bool Seek(INT64 offset, Origin origin = CURRENT);
	virtual bool Seek();
	virtual INT64 Tell();
	virtual INT64 Size();

	virtual bool Read(void* buffer, UINT32 count);
	virtual bool Read(void* buffer, UINT32 count, UINT32 *pBytesRead);
	virtual bool Read(INT64 offset, void* buffer, UINT32 count);
	virtual bool ReadMSB(UINT8 &Buffer);
	virtual bool Write(void* buffer, UINT32 count);
	virtual void FlushFileBuffers();
	


	static CError SetIOCallbacks(NCSError (NCS_CALL *pOpenACB)(char *szFileName, void **ppClientData),
#ifdef NCS_BUILD_UNICODE
									NCSError (NCS_CALL *pOpenWCB)(wchar_t *szFileName, void **ppClientData),
#endif
									NCSError (NCS_CALL *pCloseCB)(void *pClientData),
									NCSError (NCS_CALL *pReadCB)(void *pClientData, void *pBuffer, UINT32 nLength),
									NCSError (NCS_CALL *pReadAsyncCB)(void *pClientData, UINT64 nOffset, void *pBuffer, UINT32 nLength),
									NCSError (NCS_CALL *pReadCountCB)(void *pClientData, void *pBuffer, UINT32 nLength, UINT32 *pBytesRead),
									NCSError (NCS_CALL *pSeekCB)(void *pClientData, UINT64 nOffset, UINT32 nOrigin),
									NCSError (NCS_CALL *pTellCB)(void *pClientData, UINT64 *pOffset),
									NCSError (NCS_CALL *pFileSizeCB)(void *pClientData, INT64 *nFileSize));

	virtual bool Lock( INT64 nOffset, INT64 nBytes, bool bExclusive, bool bWait );
	virtual bool UnLock( INT64 nOffset, INT64 nBytes );

	virtual UINT64 GetLastAccessTime();
	virtual UINT64 GetLastWriteTime();
	virtual UINT64 GetCreationTime();
	
	virtual bool SetFileValidData(INT64 ValidDataLength);

	void SetStreamOffset(UINT64 nStreamOffset) {	// for NITF. added on 24/05/2013
		if (!m_nStreamOffset) {
			// if not set yet, do it
			m_nStreamOffset = nStreamOffset;
			//Seek( nStreamOffset, CIOStream::START );
			NCSFileSeekNative(m_hFile, m_nStreamOffset, CIOStream::START);
			m_nOffset = m_nStreamOffset;
		}
	};

private:
	UINT64			m_nStreamOffset; // default is 0, other value for NITF. added on 24/05/2013

protected:
	NCS_FILE_HANDLE m_hFile;
	CString m_sFilename;

	typedef struct {
		NCS_FILE_HANDLE hFile;
		bool	bInUse;
	} Handle;
	CMutex	m_mHandles;
	CEvent	m_eHandles;
	std::vector<Handle> m_Handles;

	INT64	m_nFileSize;

};

class NCSECW_IMPEX CFileIOStreamCallBacks : public CFileIOStream
{
public:
	CFileIOStreamCallBacks() : m_pClientData(NULL), m_iFlags(0) {};
	virtual ~CFileIOStreamCallBacks() {};
	virtual CError Open(const CString &FileName, UINT32 iFlags);
	virtual CError Close();
	virtual CFileIOStream *Clone();

	virtual bool Seek(INT64 offset, Origin origin = CURRENT);
	virtual INT64 Tell();
	virtual INT64 Size();
	
	virtual bool Read(void* buffer, UINT32 count);
	virtual bool Read(void* buffer, UINT32 count, UINT32 *pBytesRead);
	virtual bool Read(INT64 offset, void* buffer, UINT32 count);
	virtual bool ReadMSB(UINT8 &Buffer);
	
	static CError SetIOCallbacks(NCSError (NCS_CALL *pOpenACB)(char *szFileName, void **ppClientData),
#ifdef NCS_BUILD_UNICODE
									NCSError (NCS_CALL *pOpenWCB)(wchar_t *szFileName, void **ppClientData),
#endif
									NCSError (NCS_CALL *pCloseCB)(void *pClientData),
									NCSError (NCS_CALL *pReadCB)(void *pClientData, void *pBuffer, UINT32 nLength),
									NCSError (NCS_CALL *pReadAsyncCB)(void *pClientData, UINT64 nOffset, void *pBuffer, UINT32 nLength),
									NCSError (NCS_CALL *pReadCountCB)(void *pClientData, void *pBuffer, UINT32 nLength, UINT32 *pBytesRead),
									NCSError (NCS_CALL *pSeekCB)(void *pClientData, UINT64 nOffset, UINT32 nOrigin),
									NCSError (NCS_CALL *pTellCB)(void *pClientData, UINT64 *pOffset),
									NCSError (NCS_CALL *pFileSizeCB)(void *pClientData, INT64 *nFileSize));

	static bool HasIOCallbacksSet() {
		return(CFileIOStreamCallBacks::sm_pOpenACB != NULL ||
#ifdef NCS_BUILD_UNICODE
			   CFileIOStreamCallBacks::sm_pOpenWCB != NULL ||
#endif
			   CFileIOStreamCallBacks::sm_pCloseCB != NULL ||
			   CFileIOStreamCallBacks::sm_pReadCB != NULL ||
			   CFileIOStreamCallBacks::sm_pReadAsyncCB != NULL ||
			   CFileIOStreamCallBacks::sm_pReadCountCB != NULL ||
			   CFileIOStreamCallBacks::sm_pSeekCB != NULL ||
			   CFileIOStreamCallBacks::sm_pTellCB != NULL ||
			   CFileIOStreamCallBacks::sm_pFileSizeCB != NULL);
	}
	
	
protected:
	void *m_pClientData;
	UINT32 m_iFlags;

	static NCSError (NCS_CALL *sm_pOpenACB)(char *szFileName, void **ppClientData);
	static NCSError (NCS_CALL *sm_pOpenWCB)(wchar_t *szFileName, void **ppClientData);
	static NCSError (NCS_CALL *sm_pCloseCB)(void *pClientData);
	static NCSError (NCS_CALL *sm_pReadCB)(void *pClientData, void *pBuffer, UINT32 nLength);
	static NCSError (NCS_CALL *sm_pReadAsyncCB)(void *pClientData, UINT64 nOffset, void *pBuffer, UINT32 nLength);
	static NCSError (NCS_CALL *sm_pReadCountCB)(void *pClientData, void *pBuffer, UINT32 nLength, UINT32 *pBytesRead);
	static NCSError (NCS_CALL *sm_pSeekCB)(void *pClientData, UINT64 nOffset, UINT32 nOrigin);
	static NCSError (NCS_CALL *sm_pTellCB)(void *pClientData, UINT64 *pOffset);
	static NCSError (NCS_CALL *sm_pFileSizeCB)(void *pClientData, INT64 *nFileSize);
};

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CFileIOStream CNCSJPCFileIOStream;
#endif

#endif // !NCSFILEIOSTREAM_H
