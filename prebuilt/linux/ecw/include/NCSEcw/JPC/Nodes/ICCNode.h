/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     ICCNode.h 
** CREATED:  20/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CICC2RGBNode class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCICCNode2D_H
#define NCSJPCICCNode2D_H

#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H

#ifdef NCSJPC_USE_LCMS
#include "lcms.h"
#endif // NCSJPC_USE_LCMS

namespace NCS {
namespace JPC {
	/**
	 * CICC2RGBNode class - ICC profile to RGB colorspace conversion node using 
	 * the LittleCM library available from www.littlecms.com.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CICC2RGBNode: public SDK::CNode2D {
public:
		/** Default constructor, initialises members */
	//CICC2RGBNode(class CJPC &JPC, void *pICCProfile, UINT32 nLen);
	CICC2RGBNode(class CJPC &JPC, void *pICCProfile, UINT32 nLen, void *pTilePart); // changed on 29/04/2012
		/** Virtual destructor */
	virtual ~CICC2RGBNode();

#ifdef NCSJPC_USE_LCMS
		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(SDK::CNode2D::ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
protected:
	cmsHPROFILE m_ICCProfile;
	cmsHPROFILE m_sRGBProfile;
	cmsHTRANSFORM m_Transform;
	class CJPC *m_pJPC;

#endif //NCSJPC_USE_LCMS
};
}
}
#endif // !NCSJPCICCNode2D_H
