/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     NCSIOStream.h 
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CIOStream class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSIOSTREAM_H
#define NCSIOSTREAM_H

#ifndef NCSFILEIO_H
#include "NCSFileIO.h"
#endif // !NCSFILEIO_H

#include "NCSDefs.h"
#include "NCSError.h"

#ifndef NCSSTRING_H
#include "NCSString.h"
#endif

#include <vector>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace NCS {


class NCSECW_IMPEX CIOStream: public CError {
public:

	typedef enum {
		START		= NCS_FILE_SEEK_START,
		CURRENT		= NCS_FILE_SEEK_CURRENT,
		END			= NCS_FILE_SEEK_END
	} Origin;

	CIOStream();
	virtual ~CIOStream();

	virtual CIOStream *Clone() = 0;
	virtual UINT32 GetHttpResponseCode() { return m_nResponseCode; };

	virtual CError GetError() { return((CError)*this); };
	virtual const CString &GetName() { return(m_Name); };
	virtual bool IsWrite();

	virtual CError Open(const CString &Name, bool bWrite = false);
	virtual CError Close();
	
	virtual bool Seek(INT64 offset, Origin origin = CURRENT);
	virtual bool Seek();
	virtual INT64 Tell();
	virtual INT64 Size();

	virtual bool Mark();
	virtual bool Rewind();
	virtual bool UnMark();

	virtual bool IsBuffered() { return(false); }
	virtual bool IsPacketStream() { return(false); }
	virtual void ProcessReceivedPackets() {};

	virtual bool HasOpacityStreamAppended()  { return(m_bHasOpacityAppended); };
	virtual void SetOpacityStreamAppended(bool value)  {m_bHasOpacityAppended = value; };
	virtual bool IsOpacityStream()  { return(m_bIsOpacity); };
	virtual void SetOpacityStream(bool value)  {m_bIsOpacity = value; };

	virtual bool Read(void* buffer, UINT32 count);
	virtual bool Read(INT64 offset, void* buffer, UINT32 count);
	virtual bool Write(void* buffer, UINT32 count);

	virtual void FlushFileBuffers() { ; }
	
	virtual bool ReadMSB(UINT8 &Buffer) { return(Read(&Buffer, sizeof(Buffer))); };
	virtual bool ReadMSB(UINT16 &Buffer);
	virtual bool ReadMSB(UINT32 &Buffer);
	virtual bool ReadMSB(UINT64 &Buffer);
	virtual bool ReadMSB(INT8 &Buffer) { return(Read(&Buffer, sizeof(Buffer))); };
	virtual bool ReadMSB(INT16 &Buffer);
	virtual bool ReadMSB(INT32 &Buffer);
	virtual bool ReadMSB(INT64 &Buffer);
	virtual bool ReadMSB(IEEE4 &Buffer);
	virtual bool ReadMSB(IEEE8 &Buffer);
	NCS_INLINE virtual bool WriteMSB(UINT8 nValue) { return(Write(&nValue, sizeof(nValue))); };
	virtual bool WriteMSB(UINT16 nValue);
	virtual bool WriteMSB(UINT32 nValue);
	virtual bool WriteMSB(UINT64 nValue);
	NCS_INLINE virtual bool WriteMSB(INT8 nValue) { return(Write(&nValue, sizeof(nValue))); };
	virtual bool WriteMSB(INT16 nValue);
	virtual bool WriteMSB(INT32 nValue);
	virtual bool WriteMSB(INT64 nValue);
	virtual bool WriteMSB(IEEE4 fValue);
	virtual bool WriteMSB(IEEE8 dValue);
	virtual bool ReadLSB(UINT8 &Buffer) { return(Read(&Buffer, sizeof(Buffer))); };
	virtual bool ReadLSB(UINT16 &Buffer);
	virtual bool ReadLSB(UINT32 &Buffer);
	virtual bool ReadLSB(UINT64 &Buffer);
	virtual bool ReadLSB(INT8 &Buffer) { return(Read(&Buffer, sizeof(Buffer))); };
	virtual bool ReadLSB(INT16 &Buffer);
	virtual bool ReadLSB(INT32 &Buffer);
	virtual bool ReadLSB(INT64 &Buffer);
	virtual bool ReadLSB(IEEE4 &Buffer);
	virtual bool ReadLSB(IEEE8 &Buffer);
	virtual bool WriteLSB(UINT8 &Buffer) { return(Write(&Buffer, sizeof(Buffer))); };
	virtual bool WriteLSB(UINT16 nValue);
	virtual bool WriteLSB(UINT32 nValue);
	virtual bool WriteLSB(UINT64 nValue);
	virtual bool WriteLSB(INT8 &Buffer) { return(Write(&Buffer, sizeof(Buffer))); };
	virtual bool WriteLSB(INT16 nValue);
	virtual bool WriteLSB(INT32 nValue);
	virtual bool WriteLSB(INT64 nValue);
	virtual bool WriteLSB(IEEE4 fValue);
	virtual bool WriteLSB(IEEE8 dValue);	
	virtual bool ResetBitStuff();
	virtual bool FlushBitStuff();
	virtual bool Stuff(bool bBit);
	virtual bool UnStuff(bool &bBit);
	static NCS::CString GenerateUniqueName();
#ifndef NCS_NO_COMPAT_NAMES
	bool WriteUINT32(UINT32 nValue) {return WriteMSB(nValue);};
	bool WriteIEEE4(IEEE4 fValue) {return WriteMSB(fValue);};
	bool WriteUINT16(UINT16 nValue) {return WriteMSB(nValue);};
	bool WriteUINT64(UINT64 nValue) {return WriteMSB(nValue);};
	bool WriteIEEE8(IEEE8 dValue)  {return WriteMSB(dValue);};
	bool WriteUINT8(UINT8 nValue) {return WriteMSB(nValue);};
	bool ReadUINT32(UINT32& buffer) {return ReadMSB(buffer);};
	bool ReadIEEE4(IEEE4& buffer) {return ReadMSB(buffer);};
	bool ReadUINT16(UINT16& buffer) {return ReadMSB(buffer);};
	bool ReadUINT64(UINT64& buffer) {return ReadMSB(buffer);};
	bool ReadIEEE8(IEEE8& buffer) {return ReadMSB(buffer);};
	bool ReadUINT8(UINT8& buffer) {return ReadMSB(buffer);};
#endif

protected:
	CString			m_Name;

	BOOLEAN			m_bAuthenticatedUser;
	bool			m_bWrite;
	bool			m_bIsOpacity;
	bool			m_bHasOpacityAppended;
	std::vector<INT64>	m_Marks;
	INT64			m_nOffset;
	UINT32			m_nResponseCode;

	UINT16	m_nThisBytes;
	UINT8	m_nBitsLeft;
};

typedef std::vector<CIOStream *> CIOStreamVector;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CIOStream CNCSJPCIOStream;
#endif

#define NCSJP2_CHECKIO_BEGIN(e, s)	while(e == NCS_SUCCESS) { \
												CError *pErrorNCSJP2_CHECKIO = &e;\
												CIOStream *pStreamNCSJP2_CHECKIO = &s
#define NCSJP2_CHECKIO_ERROR_BEGIN(e)	while(e == NCS_SUCCESS) { \
												CError *pErrorNCSJP2_CHECKIO = &e
#define NCSJP2_CHECKIO(a)		if(pStreamNCSJP2_CHECKIO->a == false) { *pErrorNCSJP2_CHECKIO = *pStreamNCSJP2_CHECKIO; break; }
#define NCSJP2_CHECKIO_ERROR(a)	*pErrorNCSJP2_CHECKIO = a; if(*pErrorNCSJP2_CHECKIO != NCS_SUCCESS) break
#define NCSJP2_CHECKIO_END()		break; }

#endif // !NCSIOSTREAM_H
