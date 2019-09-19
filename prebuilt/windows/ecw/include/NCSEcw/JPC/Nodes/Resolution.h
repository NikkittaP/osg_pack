/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Resolution.h 
** CREATED:  12/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CResolution class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCRESOLUTION_H
#define NCSJPCRESOLUTION_H

#ifndef NCSJPCPRECINCT_H
#include "NCSEcw/JPC/Nodes/Precinct.h"
#endif // NCSJPCPRECINCT_H
#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H
#ifndef NCSJPCPLTMARKER_H
#include "NCSEcw/JPC/Markers/PLTMarker.h"
#endif // NCSJPCPLTMARKER_H

// tried on 21/03/2013
// turn on 12/05/2014
#ifndef NCSJPC_READ_MT_PRECINCT 
#define NCSJPC_READ_MT_PRECINCT
#endif

namespace NCS {
namespace SDK {
	class CSpinLockImpl;
	class CSpinRWLockImpl;
}
}

namespace NCS {
namespace JPC {
	/**
	 * CResolution class - the JPC Resolution level.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CResolution: public SDK::CNodeTiler2D {
public:
		/** Component for this resolution */
	class CComponent *m_pComponent;
		/** Resolution level */
	UINT8	m_nResolution;
		/** Compression target ratio */
	UINT16	m_nCompressionRatio;
		/** min data value for 16bit compression, on 11/10/2012 */
	INT32	m_nEncodeDataMin;
		/** max data value for 16bit compression, on 11/10/2012 */
	INT32	m_nEncodeDataMax;


	CPrecinctMap m_Precincts;

	UINT32			m_nEncoderPLTStartIndex;

	UINT32  m_nFirstPacketNrInOwnTile;
		/** 
		  * For PCRL and CPRL only
		  * The number packets between two neighbouring precincts on X 
		  * The size is depending on the current res and lower res 
		  * When resolution = 0, the size is 1, as no lower
		  * 			
		  * Res i-1:					x			x			x
		  *	| means corresponding		|			|			|
		  * Res i:						x	o	o	x	o	o	x	o
		  * The above shows the ratio between res i and i-1 is 3. The first packet number of precincts at position x at res i can be directly derived from x at res i-1 ([i] = p[i-1] + nLayers), as the order is XXRL (only L is between R). 
		  * There are three cycles shown as an example. Then m_nNrPacketsBetweenPrecinctsX[cycles][ratio-1], [3][2] in this example
		  */
    std::vector<std::vector<UINT32> >  m_nNrPacketsBetweenPrecinctsX; 
		/** 
		  * For PCRL and CPRL only
		  * The number packets between two neighbouring precincts on Y 
		  * The size is depending on the current res and lower res 
		  * When resolution = 0, the size is 1, as no lower
		  * The same principle, but on Y
		  */
    std::vector<std::vector<UINT32> >  m_nNrPacketsBetweenPrecinctsY; // added on 04/07/2012, changed on 13/07/2012
		/** 
		 * For PCRL and CPRL only
		 * The first packet number of each precinct of m_Precincts in its own tile. Only for m_nResolution = 0, 
		 * the other resolutions can be derived from m_nResolution = 0 and using m_nNrPacketsBetweenPrecinctsX and m_nNrPacketsBetweenPrecinctsY at the m_nResolution 
		 */
    std::vector<std::vector<UINT32> >  m_nFirstPrecinctPacketNrInOwnTile; // [y][x] added on 13/07/2012
		/** 
		 * For PCRL and CPRL only
		 * see if m_nFirstPrecinctPacketNrInOwnTile has been calculated
		 */
	bool m_bFirstPrecinctPacketNrInOwnTile; // added on 13/07/2012
		/** 
		 * constructor 
		 * @param pComponent	Parent component
		 * @param nResolution	Resolution level, 0-32
		 */
	CResolution(class CComponent *pComponent, UINT8 nResolution);
	virtual ~CResolution();

		/** 
		 * Get Precinct Width.
		 * @return      UINT32		Un-clipped Width of precincts.
		 */
	UINT32 NCS_INLINE GetPrecinctWidth() {
				return(m_PrecinctWidth);
			};
		/** 
		 * Get Precinct Height.
		 * @return      UINT32		Un-clipped height of precincts.
		 */
	UINT32 NCS_INLINE GetPrecinctHeight() {
				return(m_PrecinctHeight);
			};
		/** 
		 * Get number of precincts wide.
		 * @return      INT32		Nr of precincts across.
		 */
	UINT32 NCS_INLINE GetNumPrecinctsWide() {
				return(m_NumPrecinctsWide);
			};
		/** 
		 * Get number of precincts high.
		 * @return      UINT32		Nr of precincts high.
		 */
	UINT32 NCS_INLINE GetNumPrecinctsHigh() {
				return(m_NumPrecinctsHigh);
			};
		/** 
		 * Get number of precincts area (wide x high).
		 * @return      UINT32		Nr of precincts area.
		 */
	UINT32 NCS_INLINE GetNumPrecinctsArea() {
				return(m_NumPrecinctsArea);
			};

		/** 
		 * Get codeblock Width.
		 * @return      UINT32		Un-clipped Width of codeblocks.
		 */
	UINT32 NCS_INLINE GetCBWidth() {
				return(m_CBWidth);
			};
		/** 
		 * Get codeblock Height.
		 * @return      UINT32		Un-clipped height of codeblocks.
		 */
	UINT32 NCS_INLINE GetCBHeight() {
				return(m_CBHeight);
			};	
		/** 
		 * Get CPRL/PCRL/RPCL Progression resolution multiplier.
		 * @return      INT32		Progression resolution multiplier.
		 */
	INT32 NCS_INLINE GetResMultiplier() {
				return(m_ResMultiplier);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression XRsiz multiplier.
		 * @return      INT32		Progression XRsiz multiplier.
		 */
	INT32 NCS_INLINE GetXRsizResMultiplier() {
				return(m_XRsizResMultiplier);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression YRsiz multiplier.
		 * @return      INT32		Progression YRsiz multiplier.
		 */
	INT32 NCS_INLINE GetYRsizResMultiplier() {
				return(m_YRsizResMultiplier);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression DivX factor.
		 * @return      INT32		Progression DixX Factor.
		 */
	INT32 NCS_INLINE GetDivX() {
				return(m_DivX);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression DivY factor.
		 * @return      INT32		Progression DixY Factor.
		 */
	INT32 NCS_INLINE GetDivY() {
				return(m_DivY);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression X origin check OK test result.
		 * @return      INT32		Progression DixX Factor.
		 */
	bool NCS_INLINE XOriginCheckOk() {
				return(m_XOriginCheckOk);
			};
		/** 
		 * Get CPRL/PCRL/RPCL Progression Y origin check OK test result.
		 * @return      INT32		Progression DixY Factor.
		 */
	bool NCS_INLINE YOriginCheckOk() {
				return(m_YOriginCheckOk);
			};

		/** 
		 * Get X0 of top left precinct for this resolution.
		 * @return      INT32		Coordinate value.
		 */
	INT32 NCS_INLINE GetPX0() {
				return(m_PX0);
			};
		/** 
		 * Get Y0 of top left precinct for this resolution.
		 * @return      INT32		Coordinate value.
		 */
	INT32 NCS_INLINE GetPY0() {
				return(m_PY0);
			};

		/** 
		 * Read a BufferType line from the input.
		 * @param		nCtx		Read context
		 * @param		pDst		Destination buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
		/** 
		 * Write a BufferType line to the output.
		 * @param		nCtx		Context
		 * @param		pSrc		Source buffer.
		 * @param		iComponent	Output Component
		 * @return      bool		true on succes, else false.
		 */
	virtual bool WriteLine(ContextID nCtx, CBuffer2D *pSrc, UINT16 iComponent);

		/** 
		 * UnLink input nodes.
		 * @param		nCtx		Read context
		 * @return      bool		true on success, else false on error.
		 */
	virtual bool UnLink(ContextID nCtx, UINT16 nInputs = 0);

	virtual bool WritePrecinctLayerPLT(CJPC &JPC, CIOStream &Stream, CTilePartHeader &TilePart, CPLTMarker &PLT, CPacketLengthType &Len, INT64 &nSOTOffset, UINT32 &nBytes, UINT32 p, UINT32 l);
	virtual bool WriteLayerPLTs(CJPC &JPC, CIOStream &Stream, CTilePartHeader &TilePart, CPLTMarker &PLT, CPacketLengthType &Len, INT64 &nSOTOffset, UINT32 &nBytes, UINT32 l);
	virtual bool WritePrecinctLayerPacket(CJPC &JPC, CIOStream &Stream, UINT32 p, UINT32 l);
	virtual bool WriteLayerPackets(CJPC &JPC, CIOStream &Stream, UINT32 l);

	UINT64 GetMemorySize(); // Against bug EMS-407, added 01/11/2011
	bool CreatePackets();	// Against bug EMS-407, added 02/11/2011
	bool PrecinctValidation();	// added on 03/12/2011

	bool CalcFirstPacketNr(); // added on 13/07/2012
	bool CalcFirstPacketNr(UINT32 nRequPrecinctX, UINT32 nRequPrecinctY, UINT32 &nioFirstPacketNr); // used for PCRL and CPRL. added on 13/07/2012
	CResolution* GetLowerResolution(); // used for PCRL and CPRL. added on 13/07/2012
	bool CalcFirstPrecinctPacketNr(); // used for PCRL and CPRL. added on 13/07/2012
	bool PurgePrecincts(); // added on 07/09/2012
	bool remove_front_precincts(); // added on 30/12/2012
	UINT8 ClearIfZero(); // added on 07/09/2012

	 // moved on 12/12/2012
	UINT8 TraversePyramid_Precinct(ContextID nCtx,  
		UINT32 Flags,
		UINT32 x, UINT32 y, // precinct location
		UINT32 nFirstTilePacket,
		UINT32 &nBlocksTotal,
		UINT32 &nBlocksAvailable,
		UINT32 nMaxQualityLayer);
	UINT8 TraversePyramid(ContextID nCtx,  
		UINT32 Flags,
		INT32 nTopX,
		INT32 nLeftY,
		INT32 nBottomX,
		INT32 nRightY,
		UINT32 &nBlocksTotal,
		UINT32 &nBlocksAvailable,
		UINT32 nMaxQualityLayer);

	// added on 13/03/2013
	static void Init();
	static void Fini();

#ifdef NCSJPC_READ_MT_PRECINCT
	// added on 13/03/2013
	virtual void QueueJob(NCS::CThreadPool::CJob *p) {
		CResolution::sm_pPrecinctWorker->QueueJob(p);
	};

	class CReadJob: public NCS::CThreadPool::CWaitableJob {
	public:
		CReadJob(NCS::SDK::CNode2D::ContextID nCtx, NCS::JPC::CPrecinct *pPrecinct, NCS::SDK::CBuffer2D *pDst, UINT16 iSubBandType) { 
			m_nCtx = nCtx;
			m_pPrecinct = pPrecinct;
			m_pDst = pDst;
			m_iSubBandType = iSubBandType;

		};
		virtual bool DoWork(void) NCS_OVERRIDE {
			bool bRet = m_pPrecinct->Read(m_nCtx, m_pDst, m_iSubBandType);
			m_pPrecinct->UnRef();
			return bRet;
		}
		
	protected:
		NCS::SDK::CNode2D::ContextID	m_nCtx;
		NCS::JPC::CPrecinct				*m_pPrecinct;
		NCS::SDK::CBuffer2D				*m_pDst;
		UINT16							m_iSubBandType;
	};


#endif

protected:

	CBuffer2D	m_EncoderLL;
	CBuffer2D	m_EncoderLH;
	CBuffer2D	m_EncoderHL;
	CBuffer2D	m_EncoderHH;

	class NCSECW_IMPEX Context: public CNode2D::Context {
	public:
			/** 2-line Output Buffer */
		CBuffer2D m_OutputBuffer;

		SDK::CBuffer2DCache	m_State;
		/* only used for out of boundary lines (PSE0 changes input value) at GET_STATE_BUFFER() */
		SDK::CBuffer2DCache	m_State_PSE0;
		INT32			m_nStateU0;
		INT32			m_nStateU1;
		INT32			m_nStateV;

		CBuffer2D		m_IL2D_L;
		CBuffer2D		m_IL2D_H;
		UINT32 m_nRequestedQualityLevel;
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

		/** 
		 * Read a BufferType line from the given subband.
		 * @param		nCtx		Read Context
		 * @param		nX			X coordinate of node within it's resolution rect.
		 * @param		nY			Y coordinate of node within it's resolution rect.
		 * @param		nWidth		Number of pixels to read.
		 * @param		eType		Buffer type
		 * @param		pBuffer		Pointer to destination buffer.
		 * @param		eSubBandType SubBand index enum.
		 * @return      bool		true on succes, else false.
		 */
	bool ReadSubBandLine(ContextID nCtx, CBuffer2D *pDst, CSubBand::Type eSubBandType);
#ifdef NCSJPC_READ_MT_PRECINCT
	/** 
		 * Read a BufferType line from the given subband.
		 * @param		nCtx		Read Context
		 * @param		nX			X coordinate of node within it's resolution rect.
		 * @param		nY			Y coordinate of node within it's resolution rect.
		 * @param		nWidth		Number of pixels to read.
		 * @param		eType		Buffer type
		 * @param		pBuffer		Pointer to destination buffer.
		 * @param		eSubBandType SubBand index enum.
		 * @return      bool		true on succes, else false.
		 */
	bool ReadSubBandLineMT(ContextID nCtx, CBuffer2D *pDst, CSubBand::Type eSubBandType);
#endif
	/** 
		 * Write a BufferType line to the given subband.
		 * @param		nCtx		Context
		 * @param		pSrc		Source buffer.
		 * @param		eSubBandType SubBand index enum.
		 * @return      bool		true on succes, else false.
		 */
	bool WriteSubBandLine(ContextID nCtx, CBuffer2D *pSrc, CSubBand::Type eSubBandType);
		/** 
		 * Get normal Node Width.
		 * @return      INT32		Un-clipped Width of codeblocks.
		 */
	virtual UINT32 GetNodeWidth(UINT16 iComponent = 0) { 
			return(GetPrecinctWidth()); 
		};
		/** 
		 * Get normal Node Height.
		 * @return      INT32		Un-clipped height of codeblocks.
		 */
	virtual UINT32 GetNodeHeight(UINT16 iComponent = 0) { 
			return(GetPrecinctHeight()); 
		};
		/** 
		 * Get number of nodes wide.
		 * @return      INT32		Nr of codeblocks across.
		 */
	virtual UINT32 GetNumNodesWide(UINT16 iComponent = 0) { 
			return(GetNumPrecinctsWide()); 
		};
		/** 
		 * Get number of nodes high.
		 * @return      INT32		Nr of codeblocks high.
		 */
	virtual UINT32 GetNumNodesHigh(UINT16 iComponent = 0) { 
			return(GetNumPrecinctsHigh()); 
		};
		/** 
		 * Get pointer to specific node.
		 * @param		UINT32		Node nr
		 * @return      CNode2D * Ptr to node.
		 */
	virtual __inline CNode2D *GetNodePtr(UINT32 nNode, UINT16 iComponent = 0) {
					return NULL;
					//return(m_Precincts.find(nNode));
				}
	bool INTERLEAVE_2D(Context *pCtx, 
								INT32 u0,
								INT32 u1,
								INT32 v0,
								INT32 v1,
								CBuffer2D &a);
	bool SR_2D(Context *pCtx,
					  INT32 u0,
					  INT32 u1,
					  INT32 v0,
					  INT32 v1,
					  CBuffer2D &Dst);
	bool HOR_SR(Context *pCtx,
						CBuffer2D &a,
						INT32 u0,
						INT32 u1,
						INT32 v0,
						INT32 v1);
	CBuffer2D *GET_STATE_BUFFER(Context *pCtx, 
											INT32 u0, 
											INT32 u1, 
											INT32 v,
											CBuffer2D::Type eType);
	bool VER_SR_INPUT2(Context *pCtx,
							  INT32 u0, 
							  INT32 u1,
							  CBuffer2D *pDst,
							  CBuffer2D::Type eType);
	bool VER_SR(Context *pCtx,
						INT32 u0,
						INT32 u1,
						INT32 v0,
						INT32 v1,
						CBuffer2D &Dst);

	bool SD_2D_OUTPUT4(ContextID nCtx,
							   INT32 u0, 
							   INT32 u1, 
							   INT32 v,
							   CBuffer2D::Type eType);
	bool SD_2D(ContextID nCtx,
					   CBuffer2D *pSrc);
private:
	UINT32 m_CBWidth;
	UINT32 m_CBHeight;
	UINT32 m_PrecinctWidth;
	UINT32 m_PrecinctHeight;
	INT32 m_NumPrecinctsWide;
	INT32 m_NumPrecinctsHigh;
	INT32 m_NumPrecinctsArea; // added on 27/10/2011

	INT32 m_ResMultiplier;
	INT32 m_XRsizResMultiplier;
	INT32 m_YRsizResMultiplier;
	INT32 m_DivX;
	INT32 m_DivY;

	bool m_XOriginCheckOk;
	bool m_YOriginCheckOk;
	INT32 m_PX0;
	INT32 m_PY0;

	//tbb::spin_mutex *m_pMutex_Res; // Against bug EMS-407, added 30/11/2011
	NCS::SDK::CSpinRWLockImpl *m_prwMutex_Res;  // added 07/09/2012

	// added on 13/03/2013
protected:
	static NCS::CThreadPool *sm_pPrecinctWorker; 
	static int nReadJobsCreated;
	static int nReadJobsDone;
};
}
}

#endif // !NCSJPCRESOLUTION_H
