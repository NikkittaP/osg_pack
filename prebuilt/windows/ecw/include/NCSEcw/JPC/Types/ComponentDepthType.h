/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     ComponentDepthType.h
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CSIZMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCCOMPONENTDEPTHTYPE_H
#define NCSJPCCOMPONENTDEPTHTYPE_H

#include "NCSEcw/JPC/Markers/Marker.h"

namespace NCS {
namespace JPC {

	/**
	 * CComponentDepthType class - component depth/sign type
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CComponentDepthType {
public:
		/** Component bit depth */
	UINT8		m_nBits;	
		/** Component values are signed */
	bool		m_bSigned;

		/** Default constructor, initialises members */
	CComponentDepthType();
		/** Virtual destructor */
	virtual ~CComponentDepthType();

		/** 
		 * Parse from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream);
		/** 
		 * UnParse to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);

	INT32 GetMinValue() { return(m_bSigned ? (-1 * (signed)GetMaxValue() - 1) : 0); }; 
	INT32 GetMaxValue() { return(m_bSigned ? ((1 << (m_nBits - 1)) - 1) : ((1 << m_nBits) - 1)); };
	INT32 GetDCShift() { return(m_bSigned ? 0 : (INT32)((UINT32)(1 << (m_nBits - 1)))); }; 
};
}
}

#endif // !NCSJPCCOMPONENTDEPTHTYPE_H
