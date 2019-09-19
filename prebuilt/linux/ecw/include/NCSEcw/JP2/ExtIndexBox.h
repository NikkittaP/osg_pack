/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     ExtIndexBox.h 
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CBox class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJP2EXTINDEXBOX_H
#define NCSJP2EXTINDEXBOX_H

#ifndef NCSSDKBOX_H
#include "NCSEcw/SDK/Box.h"
#endif //NCSSDKBOX_H

namespace NCS {
namespace JP2 {
/**
 * CExtIndexBox class - JP2 Box compatible for External index J2I file.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
 */
	class NCSECW_IMPEX CExtIndexBox: public NCS::SDK::CBox {
	public:
		/** Box type */ 
	static UINT32	sm_nTBox;
		/** J2I file signature */
	static UINT32	sm_J2ISignature;

		/** Signature as read from file */
	UINT32		m_nSignature;
	UINT32		m_nExtIndexVersion;
	UINT32		m_nPackets;
		/** Modified timestamp of original JP2 file stream */
	INT32		m_nModTimeStamp;
	INT32		m_nMaxLength;
	INT32		m_nMaxDataLength;
	UINT64		m_nMaxOffset;
	UINT64		m_nMaxDataOffset;

	UINT8		m_nLengthBytes;
	UINT8		m_nDataLengthBytes;
	UINT8		m_nOffsetBytes;
	UINT8		m_nDataOffsetBytes;
	UINT32		m_nPacketStride;

	INT64		m_nIndexOffset;

		/** Default constructor, initialises members */
	CExtIndexBox();
		/** Virtual destructor */
	virtual ~CExtIndexBox();
		/** 
			* Update the extended length of the box prior to writing
			*/
	virtual void UpdateXLBox(void);

		/**
			* Parse the signature box from the JP2 file.
			* @param		JP2File		JP2 file being parsed
			* @param		Stream		IOStream to use to parse file.
			* @return      CError	NCS_SUCCESS, or Error code on failure.
			*/
	virtual CError Parse(class NCS::JPC::CJPC &JPC, CIOStream &Stream);
		/** 
			* UnParse the signature box to the JP2 file.
			* @param		JP2File		JP2 file being parsed
			* @param		Stream		IOStream to use to parse file.
			* @return      CError	NCS_SUCCESS, or Error code on failure.
			*/
	virtual CError UnParse(class NCS::JPC::CJPC &JPC, CIOStream &Stream);

	CError UnParse_NumPackets(class NCS::JPC::CJPC &JPC, CIOStream &Stream);	// Against bug EMS-407, added 18/11/2011
	CError Accumulation_Length(class NCS::JPC::CJPC &JPC, 
		UINT32 nStartTileNr, UINT32 nStartPacketNr, UINT32 nEndPacketNr);		// Against bug EMS-407, added 18/11/2011
	CError UnParse_Length(class NCS::JPC::CJPC &JPC, CIOStream &Stream);		// Against bug EMS-407, added 18/11/2011
	CError UnParse_Packets(class NCS::JPC::CJPC &JPC, CIOStream &Stream,
		UINT32 nStartTileNr, UINT32 nStartPacketNr, UINT32 nEndPacketNr);		// Against bug EMS-407, added 18/11/2011

	virtual NCS::JPC::CPacket *GetPacketHeader(UINT32 nPacket, CIOStream &Stream);
};
}
}
#endif // !NCSJP2EXTINDEXBOX_H
