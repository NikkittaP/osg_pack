/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     MESNode.h 
** CREATED:  20/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CNCSJPCDCShiftNode class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCDCSHIFTNODE_H
#define NCSJPCDCSHIFTNODE_H

#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H

	/**
	 * NCSJPCDCSHIFTNODE_H class - DC level shifting.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSJPC_EXPORT_ALL CNCSJPCDCShiftNode: public CNode2D2D {
public:
		/** Default constructor, initialises members */
	CNCSJPCDCShiftNode();
		/** Virtual destructor */
	virtual ~CNCSJPCDCShiftNode();

		/** 
		 * Initialise this node.
		 * @param		nCtx		Read context
		 * @param		JPC			CNCSJPC instance
		 * @param		iComponent	Index of component this node is for.
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool Init(ContextID nCtx, class CNCSJPC &JPC, UINT16 iComponent);

		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);

protected:

	class NCSJPC_EXPORT_ALL Context: public CNode2D2D::Context {
	public:
		UINT8	m_nSsiz;
		virtual ~Context() {};
	};

		/**
		 * Get the context for the given ContextID
		 * @param		nCtx		ContextID to retrive context
		 * @param		bAutoConstruct Autoconstruct a new context for this ContextID if one doesn't exist
		 * @return		Context*	Context for given ID.
		 */
	virtual CNode2D2D::Context *GetContext(ContextID nCtx, bool bAutoConstruct = true);
};

#endif // !NCSJPCDCSHIFTNODE_H
