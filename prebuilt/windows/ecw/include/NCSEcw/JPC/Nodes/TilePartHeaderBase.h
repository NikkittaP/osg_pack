/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     TilePartHeader.h 
** CREATED:  13/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CTilePartHeaderBase class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
** EDITS:    [01] 19/07/2012 Juliang Shao This is copied from TilePartHeader.h
				  It will be for other tile part, rather than main tile part, 
				  which still use TilePartHeader.h, but inheritaged from TilePartHeaderBase.h
 *******************************************************/

#ifndef NCSJPCTILEPARTHEADER_BASE_H
#define NCSJPCTILEPARTHEADER_BASE_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Markers/SOTMarker.h"
#include "NCSEcw/JPC/Markers/CODMarker.h"
#include "NCSEcw/JPC/Markers/COCMarker.h"
#include "NCSEcw/JPC/Markers/QCDMarker.h"
#include "NCSEcw/JPC/Markers/QCCMarker.h"
#include "NCSEcw/JPC/Markers/RGNMarker.h"
#include "NCSEcw/JPC/Markers/POCMarker.h"
#include "NCSEcw/JPC/Markers/SODMarker.h"
#include "NCSEcw/JPC/Markers/PLTMarker.h"
#include "NCSEcw/JPC/Markers/PPTMarker.h"
#include "NCSEcw/JPC/Markers/COMMarker.h"
#include "NCSEcw/JPC/Nodes/Component.h"
#include "NCSEcw/JPC/Progression.h"
#include "NCSEcw/JPC/Types/Types.h"
#include "NCSEcw/JPC/JPC.h"

namespace NCS {
namespace JPC {
	/**
	 * CTilePartHeaderBase class - the JPC codestream Tile-Part Header.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #21 $ $Author: jshao $ $Date: 2012/07/18 $ 
	 */	
class NCSECW_IMPEX CTilePartHeaderBase {
public:
		// Tile-Part absolute index within codestream
	UINT32				m_nZtp;
		/** pointer to main JPC */
	CJPC*				m_pJPC;

		/** Tile-part header is valid */
	bool				m_bValid;

		/** SOC start of codestream Marker - required */
	CSOTMarker			m_SOT;

		/** POC progression order change marker - optional
		 * NOTE: Over-rides any main-header POC and any COD Marker 
		 */
	CPOCMarker			m_POC;

		/** SOD start of data */
	CSODMarker			m_SOD;

		/** Packet length(s) for this tile-part */
	std::vector<CPLTMarker> m_PLTs;
		/** Hashed index to PLTs, in mod 32768 */
	std::vector<UINT32> m_PLTIndex;

		/** Packet header(s) for this tile-part */
	std::vector<CPPTMarker> m_PPTs;
		/** Are the PPT's from the file or dynamically created? */
	bool				m_bFilePPTs;

		/** offset to current stop of the bytes m_nBytes */
	UINT64				m_nDynamicPLTOffset; // 16/03/2012

		/** Are the PLT dynamically created? */
	bool				m_bPLTDynamic;	// added on 25/06/2012

	UINT32				m_FirstPacketNrInTP; // the first packet number for this tile part referred to the tile, added on 08/07/2012

		/** Encoder PLT Stream */
//	CIOStream		*m_pEncoderPLTFile;

		/** Default constructor, initialises members */
	explicit CTilePartHeaderBase(CJPC *pJPC);
		/** Virtual destructor */
	virtual ~CTilePartHeaderBase();

	void InitBase();

	void ResetBase();
		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		inOption	the required tile part ranges 
		 * @param		inTileNrStart: start tile number [0, CBuffer2D::MAXUINT32)
		 * @param		inTileNrEnd: end tile number [0, CBuffer2D::MAXUINT32), inclusive
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream,  
		UINT32 inTileNrStart = 0, UINT32 inTileNrEnd = NCSJPC_INIT_VALUE, INT8 nResolutionLevel = -1); //CBuffer2D::MAXUINT32);
		/** 
		 * UnParse the marker to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);
		/**
		 * Get the available number of packets for this tile part
		 * @return		UINT32	Packet count
		 */
	UINT32 GetNrPLTsPackets();

	bool FreePLTsLength(bool bForced = false);	// Against bug EMS-407, added 22/11/2011

	void Lock(void);	// added on 04/01/2012
	void UnLock(void);	// added on 04/01/2012
	void AddPLT(CPLTMarker &PLT);	// added on 26/06/2012

	bool LoadAvailablePackets(CTilePartHeader *pMainTP, UINT32 inNumPLTs, CPLTMarker *inPLTs);		// added on 26/06/2012
	bool UnloadAvailablePackets(CTilePartHeader *pMainTP, UINT32 inNumPLTs, CPLTMarker *inPLTs);	// added on 18/07/2012
		/**
		 * Get the sequential Packet Header for the specified packet number
		 * @param		nPacket		Packet number
		 * @return		CPacket	Packet Header struct (may be unparsed)
		 */
	CPacket *_GetSequentialPacketHeader(UINT32 nPacket, UINT32 &nioPrevPLTnr, bool bPurge = true);

	bool SeekPacket(CJPC &JPC, CTilePartHeaderBase *pTP, CPLTMarker &inPLT, UINT32 &inPLTNr, UINT32 &inNr, std::vector<CPacketLengthType> *pLengths,
		CPacket *pioPH); // Added on 13/12/2011
	bool SeekPacket(CIOStream &Stream, UINT64 nOffset, CJPC &JPC, CTilePartHeader *pMainTP, CTilePartHeaderBase *pTP, UINT32 nPacket, CPLTMarker &inPLT, UINT32 &inPLTNr, UINT32 &inNr, std::vector<CPacketLengthType> *pLengths,
		CPacket *pioPH);	// Added on 13/12/2011

	// added on 16/11/2012
	bool CalcPLTIndex();

protected:

	NCS::CMutex m_Mutex_TP;// Against bug EMS-407, changed 21/12/2011, as spin lock for short term

};
}
}


#endif // !NCSJPCTILEPARTHEADER_BASE_H
