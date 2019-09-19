/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     InputNode.h 
** CREATED:  20/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CInputNode class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSECWINPUTNODE_H
#define NCSECWINPUTNODE_H

#ifndef NCSJPCNodeTiler2D_H
#include "NCSEcw/SDK/NodeTiler2D.h"
#endif // NCSJPCNodeTiler2D_H

#include "NCSEcw/ECW/ECW.h"
#include "NCSEcw/ECW/File.h"
#include "NCSEcw/SDK/Resample_T.h"

namespace NCS {
namespace ECW {
	class CFile;
	/**
	 * CInputNode class - Reads ECW input, currently also resamples to view size.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2018/04/17 $ 
	 */	
class NCSECW_IMPEX CInputNode: public SDK::CNode2D {
public:
		/** Default constructor, initialises members */
	CInputNode();
		/** Virtual destructor */
	virtual ~CInputNode();

	virtual bool Init(ContextID nCtx, QmfRegion *pRegion, ECW::CFile *pFile, INT32 old_level, double &dXStart, double &dYStart, bool bNearestNeighbour, INT32 m_iAllOpacityBand, bool bEnableDither);
	virtual bool Fini(ContextID nCtx, QmfRegion *pRegion);

		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);


	virtual bool DeleteIncompleteNodes(ContextID nCtx);
#ifdef NOTUSED
		/** 
		 * Get normal Node Width.
		 * @return      INT32		Un-clipped Width of codeblocks.
		 */
	virtual UINT32 GetNodeWidth(UINT16 iComponent = 0);
		/** 
		 * Get normal Node Height.
		 * @return      INT32		Un-clipped height of codeblocks.
		 */
	virtual UINT32 GetNodeHeight(UINT16 iComponent = 0);
		/** 
		 * Get number of nodes wide.
		 * @return      INT32		Nr of codeblocks across.
		 */
	virtual UINT32 GetNumNodesWide(UINT16 iComponent = 0);
		/** 
		 * Get number of nodes high.
		 * @return      INT32		Nr of codeblocks high.
		 */
	virtual UINT32 GetNumNodesHigh(UINT16 iComponent = 0);
		/** 
		 * Get pointer to specific node.
		 * @param		UINT32		Node nr
		 * @return      CNode2D2D * Ptr to node.
		 */
	virtual CNode2D *GetNodePtr(UINT32 nNode, UINT16 iComponent = 0);
#endif
	bool m_bNearestNeighbour;
	class NCSECW_IMPEX Context: public CNode2D::Context {
	public:
		class CInputInfo {
		public:
			CInputInfo() {};
			virtual ~CInputInfo() {};
			SDK::CBuffer2DVector	m_Buffers;
#ifdef COMP_WORKER
			CReadJobs				m_Jobs;
#endif
		};

		struct QmfRegion		*m_pRegion;		
		std::vector<CInputInfo*> m_Inputs;
		UINT32					m_nCurrentInput;
		CRect2D					m_LastRead;
		INT32					m_X;
		INT32					m_Y;
		BOOLEAN					m_bManageICC;
		INT32					m_old_level;
		double					m_dStartX;
		double					m_dStartY;
		INT32					m_iAllOpacityBand;

		Context() : m_pRegion(NULL)
		{ 
			m_nCurrentInput = 0; 
			m_iAllOpacityBand = -1; 
			m_X = -1;
			m_Y = -1;
			m_bManageICC = FALSE;
			m_old_level = -1;
			m_dStartX = -1;
			m_dStartY = -1;
		};
		virtual ~Context() { while(m_Inputs.size()) { delete m_Inputs[0]; m_Inputs.erase(m_Inputs.begin()); } };
	};
	void SetLineResampler(const NCS::SDK::IResampleLinePtr &lineResampler);
	NCS::SDK::IResampleLinePtr GetLineResampler();
protected:
	class CFile *m_pFile;
	static CNode2DTracker	sm_Tracker;

	virtual CNode2D::Context *NewContext() {
		return((CNode2D::Context*)new Context());
	}
	NCS::SDK::IResampleLinePtr m_LineResampler;
};
}
}

#endif // !NCSECWINPUTNODE_H
