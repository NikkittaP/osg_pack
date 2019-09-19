/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     POCMarker.h 
** CREATED:  18/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPOCMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPOCMARKER_H
#define NCSJPCPOCMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Types/ProgressionOrderType.h"

namespace NCS {
namespace JPC {
	/**
	 * CPOCMarker class - the JPC POC marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CPOCMarker: public CMarker {
public:


		/** Information for each progression order change */
	typedef struct {
			/** Resolution level index (inclusive) for progression start */
		UINT8		m_nRSpoc;	
			/** Component index (inclusive) for progression start */
		UINT16		m_nCSpoc;
			/** Layer index (exclusive) for the end of progression  */
		UINT16		m_nLYEpoc;
			/** Resolution Level index (exclusive) for the end of progression */
		UINT8		m_nREpoc;
			/**	Component index (exclusive) for the end of progression */
		UINT16		m_nCEpoc;
			/** Progression order */
		CProgressionOrderType	m_Ppoc;
	} ProgressionOrder;

		/** The progression order changes */
	std::vector<ProgressionOrder> m_Progressions;

		/** Current active Progression index */
	UINT16	m_nCurProgression;

		/** Default constructor, initialises members */
	CPOCMarker();
		/** Virtual destructor */
	virtual ~CPOCMarker();

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

#endif // !NCSJPCPOCMARKER_H
