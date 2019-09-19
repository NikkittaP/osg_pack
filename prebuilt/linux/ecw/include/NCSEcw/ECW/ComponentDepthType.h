/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     ComponentDepthType.h
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CSIZMarker class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSECWCOMPONENTDEPTHTYPE_H
#define NCSECWCOMPONENTDEPTHTYPE_H

// Our own 64bit IOSTREAM class
#ifndef NCSIOSTREAM_H
#include "NCSIOStream.h"
#endif // NCSIOSTREAM_H

namespace NCS {
namespace ECW {

	/**
	 * CComponentDepthType class - component depth/sign type
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: mdunne $ $Date: 2011/01/10 $ 
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
	virtual CError Parse(CIOStream &Stream);
		/** 
		 * UnParse to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CFile &ECWFile, CIOStream &Stream);

	INT32 GetMinValue() { return(m_bSigned ? (-1 * (signed)GetMaxValue() - 1) : 0); }; 
	INT32 GetMaxValue() { return(m_bSigned ? ((1 << (m_nBits - 1)) - 1) : ((1 << m_nBits) - 1)); };
	INT32 GetDCShift() { return(m_bSigned ? 0 : (INT32)((UINT32)(1 << (m_nBits - 1)))); }; 
};
}
}

#endif // !NCSECWCOMPONENTDEPTHTYPE_H
