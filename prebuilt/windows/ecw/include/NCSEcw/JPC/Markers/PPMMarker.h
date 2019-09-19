/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     PPMMarker.h 
** CREATED:  18/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPPMMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPPMMARKER_H
#define NCSJPCPPMMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Packet.h"

namespace NCS {
namespace JPC {
	/**
	 * CPPMMarker class - the JPC PPM marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CPPMMarker: public CMarker {
public:
		/** Packet header info within a tile part */
	typedef struct {
			/** Packet length(s) for tile-part */
//		CPacketList	m_Headers;
			/** nBytes of packet-header to parse */
		UINT32			m_nNppmTotal;
			/** nBytes of packet-header remaining to parse in next PPM */
		UINT32			m_nNppmLeft;
			/** Offset to first packet header in this tile part */
		INT64			m_nOffset;
	} TilePartPackets;

		/** Index of this marker WRT other PPM markers */
	UINT8		m_nZppm;
		/** Packet headers, each tile-part in this PPM */
	std::vector<TilePartPackets> m_TileParts;

		/** Default constructor, initialises members */
	CPPMMarker();
		/** Virtual destructor */
	virtual ~CPPMMarker();

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
};
}
}

#endif // !NCSJPCPPMMARKER_H
