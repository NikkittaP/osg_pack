/********************************************************
*** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     rGNMarker.h 
** CREATED:  12/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CRGNMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCRGNMARKER_H
#define NCSJPCRGNMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"

namespace NCS {
namespace JPC {
	/**
	 * CRGNMarker class - the JPC RGN marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CRGNMarker: public CMarker {
public:
	typedef enum {
		IMPLICIT	= 0
	} Style;
		/** Index of component this region is for */
	UINT16		m_nCrgn;
		/** Style of the region */
	Style		m_eSrgn;
		/** Resion style parameter */
	UINT8		m_nSPrgn;

		/** Default constructor, initialises members */
	CRGNMarker();
		/** Virtual destructor */
	virtual ~CRGNMarker();

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


#endif // !NCSJPCRGNMARKER_H
