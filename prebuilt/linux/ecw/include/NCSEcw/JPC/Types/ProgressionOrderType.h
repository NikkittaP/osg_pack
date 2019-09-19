/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     ProgressionOrderType.h 
** CREATED:  18/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CProgressionOrderType class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPROGRESSIONORDERTYPE_H
#define NCSJPCPROGRESSIONORDERTYPE_H

#include "NCSEcw/JPC/Markers/Marker.h"

namespace NCS {
namespace JPC {

	/**
	 * CProgressionOrderType class - the JPC Progression Order Type.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CProgressionOrderType {
public:
				/** Progression Order Type */
	typedef enum {
			/** Layer-Resolution-Component-Position */
		LRCP		 = 0,
			/** Resolution-Layer-Component-Position */
		RLCP		 = 1,
			/** Resolution-Position-Component-Layer */
		RPCL		 = 2,
			/** Position-Component-Resolution-Layer */
		PCRL		 = 3,
			/** Component-Position-Resolution-Layer */
		CPRL		 = 4
	} Type;

		/** The type */
	Type m_eType;

		/** Default constructor, initialises members */
	CProgressionOrderType();
		/** Virtual destructor */
	virtual ~CProgressionOrderType();

		/** 
		 * Parse the fields from the JPC codestream.
		 * @param		JPC			CJPC to use to parse file.
		 * @param		Stream		IOStream to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Parse(class CJPC &JPC, CIOStream &Stream);
		/** 
		 * UnParse the fields to the JPC codestream.
		 * @param		Stream		IOStream to use to parse file.
		 * @param		JPC			CJPC to use to parse file.
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError UnParse(class CJPC &JPC, CIOStream &Stream);
};
}
}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::JPC::CProgressionOrderType CNCSJPCProgressionOrderType;
#endif

#endif // !NCSJPCPROGRESSIONORDERTYPE_H
