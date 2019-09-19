/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Resample.h 
** CREATED:  12/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CResampler class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCRESAMPLE_H
#define NCSJPCRESAMPLE_H

#ifndef NCSJPCPRECINCT_H
#include "NCSEcw/JPC/Nodes/Precinct.h"
#endif // NCSJPCPRECINCT_H
#ifndef NCSJPCNodeTiler2D_H
#include "NCSEcw/SDK/NodeTiler2D.h"
#endif // NCSJPCNodeTiler2D_H
#ifndef NCSJPC_H
#include "NCSEcw/JPC/JPC.h"
#endif // NCSJPC_H
#ifndef NCSECWCLIENT_H
#include "NCSECWClient.h"
#endif // NCSECWCLIENT_H
#ifndef NCSJPCMCTNODE_H
#include "NCSEcw/JPC/Nodes/MCTNode.h"
#endif // NCSJPCMCTNODE_H
#ifndef NCSJP2FILE_H
#include "NCSEcw/JP2/File.h"
#endif // NCSJP2FILE_H

namespace NCS {

namespace API {

class CView;

} // API

namespace JPC {
	class CTileCacheManagerContext;
	/**
	 * CResampler class - Merges multiple tiles together, and resamples the output to the specified view..
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CResampler: public SDK::CNodeTiler2D {
	friend CTileCacheManagerContext;
public:
		/** Default constructor */
	CResampler();
		/** Virtual destructor */
	virtual ~CResampler();

		/** 
		 * Link this node to the view.
		 * @param		nCtx		Read context
		 * @param		JPC			Codestream object.
		 * @param		pView		Current View to resample to
		 & @param		pMCT		CMCTNode when MCT is required
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool Link(ContextID nCtx, CJPC &JPC, CView *pFileView, JP2::CFile::CHeaderBox &Header); // added on 14/12/2011

		/** 
		 * Link this node to the view.
		 * @param		nCtx		Read context
		 * @param		JPC			Codestream object.
		 * @param		pView		Current View to resample to
		 * @param		iComponent	Component index
		 * @param		iChannel	Channel index
		 & @param		pMCT		CMCTNode when MCT is required
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool Link(ContextID nCtx, CJPC &JPC, CView *pFileView, UINT16 iComponent, UINT16 iChannel, JP2::CFile::CHeaderBox &Header);
		/** 
		 * UnLink a node as the unput to this node.
		 * @param		nCtx		Read context
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool UnLink(ContextID nCtx, UINT16 nInputs = 0);
		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool UnLink(UINT32 nTile, ContextID nCtx, UINT16 nInputs = 0);
		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual NCSError Request(ContextID nCtx);
	virtual void UpdateViewStats();

	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);

	static UINT32 CalculateResolutionLevel(UINT32 nMaxResolution, INT32 nTopX, INT32 nLeftY, INT32 nBottomX, INT32 nRightY, UINT32 nSizeX, UINT32 nSizeY);
	static UINT32 CalculateResolutionLevel(CTilePartHeader *pTilePart, INT32 nTopX, INT32 nLeftY, INT32 nBottomX, INT32 nRightY, UINT32 nSizeX, UINT32 nSizeY, bool bForceNotToApplyLowerResolutionCaptureRate);
	static UINT32 CalculateResolutionLevel(CComponent *pComponent, INT32 nTopX, INT32 nLeftY, INT32 nBottomX, INT32 nRightY, UINT32 nSizeX, UINT32 nSizeY, bool bForceNotToApplyLowerResolutionCaptureRate);

	// added on 31/01/2013
	bool ReadCancel(ContextID nCtx);

	static bool CalResolutionDimension(UINT32 nRes, UINT32 &nioImageResXSize, UINT32 &nioImageResYSize, bool &bChanged);

	UINT32 m_nDecompressionLayers;
private:

	void MarkTileVisibility(ContextID nCtx, NCS::JPC::CJPC &JPC,  
		INT32 nDatasetTLX, INT32 nDatasetTLY, INT32 nDatasetBRX, INT32 nDatasetBRY, INT32 nSizeX, INT32 nSizeY); // Against bug EMS-407, added 08/12/2011

	bool AddResolutions(ContextID nCtx, NCS::JPC::CJPC &JPC, int nBands, 
		INT32 nDatasetTLX, INT32 nDatasetTLY, INT32 nDatasetBRX, INT32 nDatasetBRY, INT32 nSizeX, INT32 nSizeY);// Against bug EMS-407, added 08/12/2011

	bool LinkInternal(ContextID nCtx, UINT16 iComponent, UINT16 iChannel, JP2::CFile::CHeaderBox &Header); // Against bug EMS-407, added 14/12/2011


protected:

	class NCSECW_IMPEX Context: public CNode2D::Context {
	public:
		const NCSSetViewInfo *m_pView;

		class TileInput {
		public:
			class ChannelInput {
			public:
				INT32 m_nResSample;
				IEEE8 m_dResSampleFactorX;
				IEEE8 m_dResSampleFactorY;

				UINT32 m_nResolutionWidth;
				UINT32 m_nResolutionHeight;	

				UINT64 m_nMax;
				INT64 m_nMin;

				CNode2D		*m_pInput;

				ChannelInput();
			};
			std::vector<ChannelInput> m_CI;			
				// Multi-component transform, only if enabled in COD
			CMCTNode	*m_pMCT;
				// Color-space transformation (eg, YCrCb->sRGB), only if enabled
			CNode2D		*m_pCSC;

				/** Tile Visibility */
			bool	m_bVisible; // Against bug EMS-407, added 08/12/2011

				/** Tile m_nDeepestAllocatedResolution increased  */
			bool	m_bDeepestAllocatedResolutionIncreased; // Against bug EMS-407, added 08/12/2011

			

			TileInput();
		};

		std::vector<TileInput> m_Tiles;

		bool m_bUseNearestNeighbour;
		std::vector<INT32> m_nInputResolutionWidth;
		INT32 m_nTilesTLX; // visible top-left corner tile number on X. added on 23/11/2011 to reduce calculation for every line
		INT32 m_nTilesTLY; // visible top-left corner tile number on Y. added on 23/11/2011 to reduce calculation for every line
		INT32 m_nTilesBRX; // visible bottom-right corner tile number on X. added on 23/11/2011 to reduce calculation for every line
		INT32 m_nTilesBRY; // visible bottom-right corner tile number on Y. added on 23/11/2011 to reduce calculation for every line
		INT32 m_nTileNrTL; // visible top-left corner tile number. added on 23/11/2011 to reduce calculation for every line
		INT32 m_nTileNrBR; // visible bottom-right tile number. added on 23/11/2011 to reduce calculation for every line
		std::vector<double> m_dXMul; // added on 23/11/2011 to reduce calculation for every line
		std::vector<double> m_dYMul; // added on 23/11/2011 to reduce calculation for every line
		INT32 m_nPrevTileRowNr;	// previous tile row #, added on 22/12/2011 to reduce calculation for every line
		/** Current resolution being decoded for the view */
		INT32 m_nCurrViewResolution; // added 28/06/2012
		Context();
		virtual ~Context();
	};
	
	class NCSECW_IMPEX CTileCacheManager {
	private:
		CTileCacheManagerContext *m_pContext;
		UINT32 GetHeightOfTileToCache();

	public:
		CTileCacheManager(CResampler::Context* pCtx, UINT16 nBandsInFile, bool bBilinearResampling);
		
		bool TryReadCached(UINT32 nTileNumber, CBuffer2D *pDst, UINT16 iComponent);
		bool ReadIntoCacheAndBuffer(ContextID nCtx, UINT32 nTileNumber, CNode2D *pTileNode, UINT16 iComponent, CBuffer2D *pDst);
		void DestroyCache(UINT32 nTileNumber);
		virtual ~CTileCacheManager();
	};
		/** Clamp output data to valid range */
	bool m_bClamp;

	CJPC *m_pJPC;
	CView *m_pView;
	CTileCacheManager *m_pTileCacheManager;
	// added on 25/08/2012
	bool m_bDoCache;

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
	
	CNode2D *BuildInputChain(ContextID nCtx, CJPC &JPC, CView *pFileView, UINT16 iComponent, UINT16 iChannel, JP2::CFile::CHeaderBox &Header, CTilePartHeader *pTilePart); // changed on 20/12/2011
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
		 * @return      CNode2D * Ptr to node.
		 */
	virtual CNode2D *GetNodePtr(UINT32 nNode, UINT16 iComponent = 0);

	virtual bool ReadInternal(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
#ifdef NCSJPC_READ_MT_TILE // 12/03/2013
	virtual bool ReadInternalMT(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
	bool ReadInternalMT_MultiTile(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent, UINT32& nWidth, INT32 &nBufferOffset);
	bool ReadInternalMT_SingleTile(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
#endif

	bool LinkTile(ContextID nCtx, CTilePartHeader *pTilePart, JP2::CFile::CHeaderBox &Header,
		  Context::TileInput &ioTI); // Against bug EMS-407, added 14/12/2011

	bool LinkTile(ContextID nCtx, UINT16 iComponent, UINT16 iChannel, CTilePartHeader *pTilePart, JP2::CFile::CHeaderBox &Header,
		Context::TileInput &ioTI);  // Against bug EMS-407, added 14/12/2011

	bool CalcContextParameters(ContextID nCtx); // Against bug EMS-407, added 14/12/2011
	bool CalcContextParameters(ContextID nCtx, UINT16 iComponent, UINT16 iChannel); // Against bug EMS-407, added 14/12/2011


#ifdef NCSJPC_READ_MT_TILE
	class CJob :public NCS::CThreadPool::CWaitableJob{
	protected: 
		
		NCS::SDK::CNode2D::ContextID	m_nCtx;
		NCS::SDK::CNode2D				*m_pNode;
		NCS::SDK::CBuffer2D				*m_pDst;
		UINT16							m_iComponent;
		CTilePartHeader					*m_pMainTP;
	public: 
		CJob(NCS::SDK::CNode2D::ContextID nCtx, NCS::SDK::CNode2D *pNode, NCS::SDK::CBuffer2D *pDst, UINT16 iComponent, CTilePartHeader *pMainTP) : CThreadPool::CWaitableJob(), 
			m_nCtx(nCtx), m_pNode(pNode), m_pDst(pDst),m_iComponent(iComponent), m_pMainTP(pMainTP){
			
		}

		virtual bool DoWork(void) NCS_OVERRIDE{
			bool bRet = DoBaseWork();
			m_pDst->Release();
			delete m_pDst;
			m_pDst = NULL;
			if (m_pMainTP)
				m_pMainTP->UnRef_Read();
			return bRet;
		};
		
	protected:
		virtual bool  DoBaseWork() = 0;
	};
	class CReadJobCached : public CJob{
		CTileCacheManager *m_pTileCacheManager;
		INT32 m_nTileNumber;
	public: 
		CReadJobCached(NCS::SDK::CNode2D::ContextID nCtx, NCS::SDK::CNode2D *pNode, NCS::SDK::CBuffer2D *pDst, UINT16 iComponent, CTilePartHeader *pMainTP, CTileCacheManager *pTileCacheManager, INT32 nTileNumber) :
			CJob(nCtx, pNode, pDst, iComponent, pMainTP), m_pTileCacheManager(pTileCacheManager), m_nTileNumber(nTileNumber){

		}
	protected: 
		virtual bool  DoBaseWork() NCS_OVERRIDE; 
	};
	class CReadJob: public CJob {
	public:
		CReadJob(NCS::SDK::CNode2D::ContextID nCtx, NCS::SDK::CNode2D *pNode, NCS::SDK::CBuffer2D *pDst, UINT16 iComponent, CTilePartHeader *pMainTP):
			CJob(nCtx, pNode, pDst, iComponent, pMainTP){
			
		};
	protected: 
		virtual bool DoBaseWork(void) NCS_OVERRIDE {
			return m_pNode->Read(m_nCtx, m_pDst, m_iComponent);
		};
	};
#endif


};

}
}

#endif // !NCSJPCRESAMPLE_H
