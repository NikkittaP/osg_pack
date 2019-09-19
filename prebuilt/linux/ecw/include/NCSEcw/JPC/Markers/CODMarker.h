/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     CODMarker.h 
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CCODMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCCODMARKER_H
#define NCSJPCCODMARKER_H

#include "NCSEcw/JPC/Markers/Marker.h"
#include "NCSEcw/JPC/Markers/COCMarker.h"
#include "NCSEcw/JPC/Types/CodingStyleParameter.h"
#include "NCSEcw/JPC/Types/ProgressionOrderType.h"

namespace NCS {
namespace JPC {
	/**
	 * CCODMarker class - the JPC COD marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CCODMarker: public CMarker {
public:
		/** Coding Style flags for Scod field */
	typedef struct {
		bool	bDefinedPrecincts: 1;
		bool	bSOPMarkers: 1;
		bool	bEPHMarkers: 1;
	} CodingStyle;

		/** SG Parameter values */
	typedef struct {
			/** Progression Order */
		CProgressionOrderType	m_ProgressionOrder;
			/** Number of layers, 1-65535 */
		UINT16					m_nLayers;
			/** Multi-Component Transform */
		bool					m_bMCT;
	} StyleParameters;

		/** Coding style for all components */
	CodingStyle			m_Scod;
		/** SG Coding style parameters */
	StyleParameters		m_SGcod;
		/** SP Coding style parameters */
	CCodingStyleParameter	m_SPcod;

		/** Default constructor, initialises members */
	CCODMarker();
		/** Virtual destructor */
	virtual ~CCODMarker();

		/** 
		 * Parse the marker from the JPC codestream.
		 * @param		JPC			CJPCto use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC&JPC, CIOStream &Stream);
		/** 
		 * UnParse the marker to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPCto use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);

		/** 
		 * Assignment operator
		 * @param		src		COC Marker.
		 * @return      CCODMarker	Updated CODMarker.
		 */
	class CCODMarker& operator=( const class CCOCMarker& src );
};

}
}

#endif // !NCSJPCCODMARKER_H
