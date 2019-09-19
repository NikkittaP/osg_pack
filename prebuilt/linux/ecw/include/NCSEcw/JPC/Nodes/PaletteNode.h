/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     PaletteNode.h 
** CREATED:  20/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPaletteNode class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCPALETTENODE_H
#define NCSJPCPALETTENODE_H

#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H
#ifndef NCSJP2FILE_H
#include "NCSEcw/JP2/File.h"
#endif // NCSJP2FILE_H

namespace NCS {
namespace JPC {
	/**
	 * CPaletteNode class - Palette node.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CPaletteNode: public SDK::CNode2D {
public:
		/** Default constructor, initialises members */
	CPaletteNode();
		/** Virtual destructor */
	virtual ~CPaletteNode();

		/** 
		 * Link a node as the unput to this node.
		 * @param		nCtx		Read context
		 * @param		nInputs		Number of inputs to this node == 1
		 * @param		pInput		First CNode2D to use as input to this node.
		 * @param		Header		CHeaderBox
		 * @param		iComponent	Output Component index
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool Link(ContextID nCtx, UINT16 nInputs, CNode2D *pInput, JP2::CFile::CHeaderBox &Header, INT32 iComponent);

	/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
protected:

	class NCSECW_IMPEX Context: public CNode2D::Context {
	public:
		NCSCellType m_eType;
		void	*m_pEntries;
		Context();
		virtual ~Context();
	};
		/**
		 * Get the context for the given ContextID
		 * @param		nCtx		ContextID to retrive context
		 * @param		bAutoConstruct Autoconstruct a new context for this ContextID if one doesn't exist
		 * @return		Context*	Context for given ID.
		 */
	//virtual CNode2D::Context *GetContext(ContextID nCtx, bool bAutoConstruct = true);
	virtual CNode2D::Context *NewContext() {
				return((CNode2D::Context*)new Context());
			};
};
}
}

#endif // !NCSJPCPALETTENODE_H
