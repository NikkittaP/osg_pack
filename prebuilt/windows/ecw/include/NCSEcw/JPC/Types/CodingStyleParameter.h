/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     CodingStyleParameter.h 
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CCodingStyleParameter class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCCODINGSTYLEPARAMETERS_H
#define NCSJPCCODINGSTYLEPARAMETERS_H

#include "NCSIOStream.h"

namespace NCS {
namespace JPC {
	/**
	 * CCODMarker class - the JPC COD marker.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CCodingStyleParameter {
public:

		/** SP Transformation Type */
	typedef enum {
			/** 9x7 irreversible filter */
		IRREVERSIBLE_9x7	= 0,
			/** 5x3 reversible filter */
		REVERSIBLE_5x3		= 1
	} TransformationType;

		/** SP Precinct Size */
	typedef struct {
			/** Precinct width exponent */
		UINT8		m_nPPx: 4;
			/** Precinct height exponent */
		UINT8		m_nPPy: 4;
	} PrecinctSize;

		/** Number of decomposition levels */
	UINT8						m_nLevels;
		/** Code block width exponent offset value */
	UINT8						m_nXcb;
		/** Code block height exponent offset value */
	UINT8						m_nYcb;
		/** CodeBlock coding pass style */
	UINT8						m_Scb;
		/** Wavelet transformation used */
	TransformationType			m_eTransformation;
		/** Precinct sizes */
	std::vector<PrecinctSize>	m_PrecinctSizes;

		/** Default constructor, initialises members */
	CCodingStyleParameter();
		/** Virtual destructor */
	virtual ~CCodingStyleParameter();

		/** 
		 * Parse the fields from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		bDefinedPrecincts	Precincts are explicitly defined in the CodingStyleParameter
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream, bool bDefinedPrecincts);
		/** 
		 * UnParse the fields to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		bDefinedPrecincts	Precincts are explicitly defined in the CodingStyleParameter
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream, bool bDefinedPrecincts);
};
}
}

#endif // !NCSJPCCODINGSTYLEPARAMETERS_H
