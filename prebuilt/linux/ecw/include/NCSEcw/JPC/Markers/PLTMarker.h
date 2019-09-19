/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:    PLTMarker.h 
** CREATED:  19/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPLTMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPLTMARKER_H
#define NCSJPCPLTMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Types/PacketLengthType.h"
#include "NCSEcw/SDK/Node2D.h"
#include <vector>


namespace NCS {
namespace JPC {

	/**
	 * CPLTMarker class - the JPC PLT marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CPLTMarker: public CMarker {
public:
	/** Index of this marker WRT other PLT markers */
	UINT8		m_nZplt;

	/** Number of packet lengths in this marker
		*
		* m_nLengths was changed to a UINT32 instead of UINT16 as
		* it may wrap around if the file contains a large number of
		* packets. This can happen when dynamically generating PLT when no PLT is present. 
		*/
	UINT32		m_nLengths;

		/** Lengths in this PLT are cumulative */
	bool	m_bCumulativeLengths;

	bool		m_bDynamic;

	/** Length offsets when hashing index */
	UINT32 m_nFirstPacket;
	UINT64 m_nHeaderLengthOffset;
	UINT64 m_nDataLengthOffset;

	CPLTMarker();
	CPLTMarker(const CPLTMarker &Marker);
	
	virtual ~CPLTMarker();

		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream);
		/** 
		 * UnParse the marker to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);
		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual bool ParseLength(class CJPC &JPC, CIOStream &Stream, UINT16 iLength, bool bFilePPTs); // changed on 23/12/2011

		/** 
		 * Assignment operator.
		 * @param		src			Copy source.
		 * @return      void
		 */
	void operator=( const class CPLTMarker& src );
		/** 
		 * Allocate the lengths array vector.
		 */
	void AllocLengths(void);
	bool FreeLengths(void);
	std::vector<CPacketLengthType> *GetLengths();
	void AddLength(CPacketLengthType &Len);

protected:

	std::vector<CPacketLengthType>	*m_pLengths;

	NCS::CMutex m_Mutex_PLT;

};
}
}

#endif // !NCSJPCPLTMARKER_H
