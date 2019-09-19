/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     NodeTiler2D_T.h
** CREATED:  13/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CSubBand class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCNodeTiler2D_T_H
#define NCSJPCNodeTiler2D_T_H

#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H

namespace NCS {
namespace SDK {
	/**
	 * NodeTiler2D_T tempplate class - manages tiled Nodes stored in a HashList_T<>.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #3 $ $Author: scope $ $Date: 2008/04/15 $ 
	 */	
template<class ReadAccessor, class WriteAccessor> class NCSECW_IMPEX NodeTiler2D_T: public CNode2D {
public:
		/** 
		 * Constructor 
		 */
	NodeTiler2D_T<ReadAccessor,WriteAccessor>() {
			m_pCtx = NULL;
		}
		/** Virtual destructor */
	virtual ~NodeTiler2D_T<ReadAccessor,WriteAccessor>() {};

		/** 
		 * Get normal Node Width.
		 * @return      INT32		Un-clipped Width of codeblocks.
		 */
	virtual UINT32 GetNodeWidth(UINT16 iComponent = 0) = 0;
		/** 
		 * Get normal Node Height.
		 * @return      INT32		Un-clipped height of codeblocks.
		 */
	virtual UINT32 GetNodeHeight(UINT16 iComponent = 0) = 0;
		/** 
		 * Get number of nodes wide.
		 * @return      INT32		Nr of codeblocks across.
		 */
	virtual UINT32 GetNumNodesWide(UINT16 iComponent = 0) = 0;
		/** 
		 * Get number of nodes high.
		 * @return      INT32		Nr of codeblocks high.
		 */
	virtual UINT32 GetNumNodesHigh(UINT16 iComponent = 0) = 0;
		/** 
		 * Get pointer to specific node.
		 * @param		UINT32		Node nr
		 * @return      CNode2D * Ptr to node.
		 */
	virtual bool GetNode(ReadAccessor &acc_r, UINT32 nNode, bool bIsUpdate, UINT16 iComponent = 0) = 0;
	virtual bool GetNode(WriteAccessor &acc_w, UINT32 nNode, bool bIsUpdate, UINT16 iComponent = 0) = 0;
		/** 
		 * UnLink a node as the unput to this node.
		 * @param		nCtx		Read context
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool UnLink(ContextID nCtx, UINT16 nInputs = 0) {
			bool bRet = true;
			m_pCtx = GetContext(nCtx);
			UINT32 nNodes = GetNumNodesWide() * GetNumNodesHigh();
			for(UINT32 n = 0; n < (UINT32)NCSMax(1, nInputs); n++) {
				for(UINT32 i = 0; i < nNodes; i++) {
					WriteAccessor acc_w;
					if(GetNode(acc_w, i, false, (UINT16)n)) {
						bRet &= acc_w->second.UnLink(nCtx);
					}
				}
			}
			return(CNode2D::UnLink(nCtx) && bRet);
		};

		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent) {
			bool bRet = false;
			UINT32 nNodesWide = GetNumNodesWide();
			UINT32 nNodesHigh = GetNumNodesHigh();
			INT32 nX = pDst->GetX0();
			INT32 nY = pDst->GetY0();
			
			m_pCtx = GetContext(nCtx);

			if(nNodesHigh && nNodesWide) {
				
				if(nNodesHigh == 1 && nNodesWide == 1) {
					ReadAccessor acc_r;
					if(GetNode(acc_r, 0, false, iComponent) && ((CNode2D&)acc_r->second).GetX0() <= nX && ((CNode2D&)acc_r->second).GetX1() >= pDst->GetX1() &&
						((CNode2D&)acc_r->second).GetY0() <= nY && ((CNode2D&)acc_r->second).GetY1() >= pDst->GetY1()) {
						return(((CNode2D&)acc_r->second).Read(nCtx, pDst, iComponent));
					}
				}
				UINT32 nNodeX0 = NCSFloorDiv(pDst->GetX0() - GetX0(), GetNodeWidth(iComponent));
				UINT32 nNodeY0 = NCSFloorDiv(pDst->GetY0() - GetY0(), GetNodeHeight(iComponent));
				UINT32 nNodeX1 = NCSCeilDiv(pDst->GetX1() - GetX0(), GetNodeWidth(iComponent));
				UINT32 nNodeY1 = NCSCeilDiv(pDst->GetY1() - GetY0(), GetNodeHeight(iComponent));

				for(UINT32 nNodeY = nNodeY0; nNodeY < nNodeY1; nNodeY++) {
					for(UINT32 nNodeX = nNodeX0; nNodeX < nNodeX1; nNodeX++) {
						UINT32 nNode = nNodeX + nNodeY * nNodesWide;
//#define NODETILER2D_READCONST						
#ifdef NODETILER2D_READCONST
						bool bRead = false;
						{
							ReadAccessor acc_r;
							// Try to get the node as readonly if possible
							// this will only succeed if the node already has its LL band
							if(GetNode(acc_r, nNode, false, iComponent) &&
								acc_r->second.Read(nCtx, pDst, iComponent))
							{
								bRet = true;
								bRead = true;
							}
						}
						if( !bRead )
#endif
						{
							WriteAccessor acc_w;
							if(GetNode(acc_w, nNode, false, iComponent)) {
								bRet = acc_w->second.Read(nCtx, pDst, iComponent);
							} else {
								bRet = false;
								break;
							}
						}
					}
				}
			}
			return(bRet);
		};
		/** 
		 * Write a BufferType line to the output.
		 * @param		nCtx		Context
		 * @param		pSrc		Source buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool WriteLine(ContextID nCtx, CBuffer2D *pSrc, UINT16 iComponent) {
			bool bRet = true;
			UINT32 nNodesWide = GetNumNodesWide();
			UINT32 nNodesHigh = GetNumNodesHigh();

			if(nNodesWide == 1 && nNodesHigh == 1) {
				ReadAccessor acc_r;
				if(GetNode(acc_r, 0, false, iComponent)) {
					bRet = ((CNode2D&)acc_r->second).WriteLine(nCtx, pSrc, iComponent);
				}
			} else {
				UINT32 nNodeHeight = GetNodeHeight();
				INT32 nNodeY = (pSrc->GetY0() - GetY0()) / nNodeHeight;
				UINT32 nNodesOffset = nNodeY * nNodesWide;

				nNodesWide += nNodesOffset;
				for(UINT32 nNode = nNodesOffset; nNode < nNodesWide; nNode++) {
					ReadAccessor acc_r;
					if(GetNode(acc_r, nNode, false, iComponent)) {
						bRet &= ((CNode2D&)acc_r->second).WriteLine(nCtx, pSrc, iComponent);
					}
				}
			}
			return(bRet);
		};
protected:
	Context	*m_pCtx;
};
}
}
#endif // !NCSJPCNodeTiler2D_T_H
