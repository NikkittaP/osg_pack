/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     QCCMarker.h 
** CREATED:  12/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CQCCMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCQCCMARKER_H
#define NCSJPCQCCMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Types/QuantizationParameter.h"

namespace NCS {
namespace JPC {
	/**
	 * CQCCMarker class - the JPC QCC marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2017/12/03 $ 
	 */	
class NCSECW_IMPEX CQCCMarker: public CMarker, public CQuantizationParameter {
public:
		/** Component index for this QCC */
	UINT16	m_nCqcc;

		/** Default constructor, initialises members */
	CQCCMarker();
		/** Copy constructor */
	explicit CQCCMarker(const class CQCDMarker& src);
		/** Virtual destructor */
	virtual ~CQCCMarker();

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
		 * @param		src		QCD Marker.
		 * @return      CQCCMarker	Updated QCCMarker.
		 */
	class CQCCMarker& operator=( const class CQCDMarker& src );
};
}
}

#endif // !NCSJPCQCCMARKER_H
