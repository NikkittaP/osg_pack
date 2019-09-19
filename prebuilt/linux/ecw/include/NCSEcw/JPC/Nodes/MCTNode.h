/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:    MCTNode.h 
** CREATED:  20/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CMCTNode class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCMCTNODE_H
#define NCSJPCMCTNODE_H

#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H

namespace NCS {
namespace JPC {
	/**
	 * CMCTNode class - Multiple Component Transform.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CMCTNode: public SDK::CNode2D {
public:
		/** Default constructor, initialises members */
	CMCTNode();
		/** Virtual destructor */
	virtual ~CMCTNode();

		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
		/** 
		 * Write a BufferType line .
		 * @param		nCtx		Write context
		 * @param		pSrc		Source buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool WriteLine(ContextID nCtx, CBuffer2D *pSrc, UINT16 iComponent);
protected:

		/** 
		 * Read lines from the input.
		 * @param		nCtx		ContextID
		 * @param		pCtx		Pointer to Read context
		 * @param		pBuf0		Pointer to buffer0.
		 * @param		pBuf1		Pointer to buffer1.
		 * @param		pBuf2		Pointer to buffer2.
		 * @return      bool		true on succes, else false.
		 */
	bool ReadInputs(ContextID nCtx, Context *pCtx, CBuffer2D *pBuf0, CBuffer2D *pBuf1, CBuffer2D *pBuf2);
};
}
}

#endif // !NCSJPCMCTNODE_H
