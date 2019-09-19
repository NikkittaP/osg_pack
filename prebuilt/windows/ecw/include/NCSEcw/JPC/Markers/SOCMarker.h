/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     SOCMarker.h 
** CREATED:  11/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CSOCMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCSOCMARKER_H
#define NCSJPCSOCMARKER_H

#ifndef NCSJPCMARKER_H
#include "NCSEcw/JPC/Markers/Marker.h"
#endif // NCSJPCMARKER_H

namespace NCS {
namespace JPC {

	/**
	 * CSOCMarker class - the JPC SOC marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CSOCMarker: public CMarker {
public:
		/** Default constructor, initialises members */
	CSOCMarker();
		/** Virtual destructor */
	virtual ~CSOCMarker();

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


#endif // !NCSJPCSOCMARKER_H
