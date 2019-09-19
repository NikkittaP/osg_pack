/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Packet.h 
** CREATED:  18/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPacket class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPACKETHEADER_H
#define NCSJPCPACKETHEADER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#ifndef NCSJPCTYPES_H
#include "NCSEcw/JPC/Types/Types.h"
#endif // NCSJPCTYPES_H

namespace NCS {
namespace JPC {
	/**
	 * CPacket class - the JPC Packet Header Type.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CPacket {
	
public:
		/** Packet Number */
	PacketId	m_nPacket;
		/** Packet length */
	INT32	m_nLength;
		/** Total Packet data length */
	INT32	m_nDataLength;
		/** Packet offset */
	UINT64	m_nOffset;
		/** Absolute Packet Data offset in codestream */
	UINT64	m_nDataOffset;

		/** Default constructor, initialises members */
	CPacket();
	virtual ~CPacket();
	static CError ConsumeSOPMarkerIfPresent(CJPC &JPC, CIOStream &Stream, INT64 &nReadBytes);
		/** 
		 * Parse the fields from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError ParseHeader(class CJPC &JPC, 
								  CIOStream &Stream, 
								  class CProgression *pProgression,
								  bool bParseData = true,
								  bool *pbNonZeroLength = NULL);
	virtual CError UnParseHeader(CJPC &JPC, 
									CIOStream &Stream, 
									CProgression *pProgression, 
									bool bParseData);
	virtual CError ParseBody(class CJPC &JPC, 
								CIOStream &Stream, 
								class CProgression *pProgression,
								bool bNonZeroLength);	
	virtual CError UnParseBody(class CJPC &JPC, 
								  CIOStream &Stream, 
								  class CProgression *pProgression,
								  bool bNonZeroLength);

	// Against bug EMS-407, added 26/10/2011
	CPacket* Clone() const;

protected:
		/** 
		 * Parse A segment length from the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		pCB			CodeBlock this segment belongs too
		 * @param		nPassesIncluded Number of passes included in this segment
		 * @param		Length		Length of segment returned
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	CError ParseSegment(CIOStream &Stream, class CCodeBlock *pCB, UINT32 nPassesIncluded, UINT32 &Length);
	CError UnParseSegment(CIOStream &Stream, class CCodeBlock *pCB, UINT32 nPassesIncluded, UINT32 &Length);
};


//typedef std::list<CPacket> CPacketListVector;

	/**
	 * CPacketList class - self parsing list of JPC Packet Headers.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CPacketList {
public:
		/** Do we have offsets yet */
	bool	m_bHaveOffsets;
		/** Default constructor, initialises members */
	CPacketList();
		/** Virtual destructor */
	virtual ~CPacketList();

		/** 
		 * Parse the headers from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		bComplete   Progression order is complete
		 * @param		nBytes		Number of bytes of headers to parse
		 * @param		nHeaders	Total number of headers to parse, -1 == ignore
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream, class CTilePartHeader *pMainTP, bool &bComplete, UINT64 nBytes, INT32 nHeaders = -1);
		/** 
		 * Parse a header from the JPC codestream.
		 * @param		bComplete   Progression order is complete
		 * @param		nCurPacket	Current packet #
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	//virtual CError ParseHeader(bool &bComplete, UINT32 nCurPacket);
	virtual CError ParseHeader(class CTilePartHeader *pMainTP, bool &bComplete, UINT32 nCurPacket);
	
	virtual CError GenDynamicPLTs(class CTilePartHeader *pMainTP, class CTilePartHeaderBase *pTP); // changed on 06/07/2012
		/** 
		 * UnParse the headers to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);

	void clear() { m_Packets.clear(); };
	void push_back(CPacket &packet) { m_Packets.push_back(packet); };
protected:
	std::vector<CPacket> m_Packets;
		/** Codestream class pointer */
	class CJPC *m_pJPC;
		/** Pointer to IOStream */
	class CIOStream *m_pStream;
		/** Number of bytes of headers to read */
	UINT64 m_nBytes;
		/** Number of headers to read */
	INT32 m_nHeaders;
};
}
}
#endif // !NCSJPCPACKETHEADER_H
