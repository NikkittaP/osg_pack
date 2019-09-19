/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     QCDMarker.h 
** CREATED:  12/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CQCDMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCQCDMARKER_H
#define NCSJPCQCDMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Types/QuantizationParameter.h"

namespace NCS {
namespace JPC {
	/**
	 * CQCDMarker class - the JPC QCD marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CQCDMarker: public CMarker, public CQuantizationParameter {
public:
		/** Default constructor, initialises members */
	CQCDMarker();
		/** Virtual destructor */
	virtual ~CQCDMarker();

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
		 * Assignment operator
		 * @param		src		QCC Marker.
		 * @return      CQCDMarker	Updated QCDMarker.
		 */
	class CQCDMarker& operator=( const class CQCCMarker& src );
};
}
}

#endif // !NCSJPCQCDMARKER_H
