/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     TLMMarker.h 
** CREATED:  19/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CTLMMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCTLMMARKER_H
#define NCSJPCTLMMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"

namespace NCS {
namespace JPC {
	/**
	 * CTLMMarker class - the JPC TLM marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CTLMMarker: public CMarker {
public:
		/** Pointer segments */
	typedef struct {
			/** Tile index of the ith tile part */
		UINT16		m_nTtlm;	
			/** Length in bytes of the tile part from the SOT to the last byte of the tile-pert datastream */
		UINT32		m_nPtlm;
	} PointerSegment;

	typedef struct {
		UINT8	m_nST : 2;
		UINT8	m_nSP : 2;
	} SizeofTP;

		/** Index of this marker WRT other TLM markers */
	UINT8		m_nZtlm;
		/** Sizeof Ttlm and Ptlm */
	SizeofTP	m_Stlm;
		/** Pointer Segments */
	std::vector<PointerSegment> m_Pointers;

		/** Default constructor, initialises members */
	CTLMMarker();
		/** Virtual destructor */
	virtual ~CTLMMarker();

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
		 * AssignTPOffset assign offsets for all tile-parts indicated by TLMs.
		 * @param		inTLMs			multi TLM.
		 * @param		inStartOffset	the first SOT offset.
		 * @param		ioTPOffsets		output offsets for all tile-parts.
		 * @param		Stream			IOStream to use to parse file, used to check if SOTs are the marker at the offset.
		 */
	static bool AssignTPOffset(std::vector<CTLMMarker> &inTLMs, UINT64 inStartOffset, 
		std::vector< std::vector<UINT64> > &ioTPOffsets, 
		CJPC &JPC, CIOStream &Stream);
};
}
}

#endif // !NCSJPCTLMMARKER_H
