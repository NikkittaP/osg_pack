/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     Precinct.h
** CREATED:  11/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CPrecinct class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
*******************************************************/

#ifndef NCSJPCPRECINCT_H
#define NCSJPCPRECINCT_H

#ifndef NCSJPCSUBBAND_H
#include "NCSEcw/JPC/Nodes/SubBand.h"
#endif // NCSJPCSUBBAND_H
#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H
#include "NCSEcw/JPC/Progression.h"
#include "NCSEcw/JPC/Markers/POCMarker.h"
#include "NCSPacketIOStream.h"
#include "NCSThreadPool.h"

#include "NCSEcw/JPC/Packet.h" 
#include "NCSEcw/SDK/Atomic.h"

#define NCSJPC_KEEP_SMALL_PRECINCT_DECODE // used when m_bSmallPrecinct = true and decode whole subband

// define it to force decode whole region of a large precinct, but it is slower. Also calculating large amount of m_RectMap may crash, eg, zooming into pixel of Burrup_LL-GDA94.jp2
//#define FORCE_DECODE_WHOLE_REGION_IN_LARGE_PRECINCT

#ifdef DEBUG_MT_SUBBAND_DECODING
// debug for subband multithreading decoding
static INT16 CReadJobCount = 0;
static INT16 CReadJobDoing = 0;
static INT16 CReadJobDone = 0;
#endif

#include "NCSEcw/SDK/Atomic.h"
namespace NCS {

	namespace SDK {
		class CSpinLockImpl;
		class CSpinRWLockImpl;
	}

	namespace JPC {
		/**
		 * CPrecinct class - the JPC codestream Precinct.
		 *
		 * @author       Simon Cope
		 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $
		 */
		class NCSECW_IMPEX CPrecinct : public SDK::CNode2D {
			CError ParseBodies(CIOStream &Stream);
			void ResetResetDecodingStatus();
		public:
			enum EHeaderReadDoneStatus{
				HeaderReadDoneStatusNotDone=0,
				HeaderReadDoneStatusParsing = 1,
				HeaderReadDoneStatusHeadersParsed = 2,
				HeaderReadDoneStatusHeadersAndBodiesParsed = 3
			};
			/** Resolution level for this precinct */
			class CResolution *m_pResolution;
			/** Precinct number within this resolution */
			UINT32	m_nPrecinct;

			/** SubBands in this precinct */
			CSubBand *m_SubBands[4];

			/** Packet number for each layer in this precinct */
			std::vector<UINT32> m_Packets;

			/** Packets for each layer in this precinct */
			std::vector<CPacket*> m_pPackets; 

			UINT16	m_nProgressionLayer;
			/** Precinct has zero width or height */
			bool	m_bZeroSize;
			/** Flag for header read status */
			EHeaderReadDoneStatus	m_nHeaderReadDone;
			/** The first packet number in its own tile */
			UINT32  m_nFirstPacketNrInOwnTile; 

			/** temp stored stream for each packet in this precinct. This is used for ECWP3. */
			std::vector<CMemoryIOStream*> m_pStreams; 
			/** the smallest stored layer. This is used with m_pStreams. */
			UINT16 m_nSmallestStoredLayerNr;	
			/** the smallest parsed layer. This is used with m_pStreams. */
			UINT16 m_nSmallestParsedLayerNr;	

			CRect2D mInvalidRect;

			/**
			 * Constructor
			 * @param pResolution	Parent resolution level
			 * @param nPrecinct		Precinct number within resolution level
			 */
			CPrecinct(class CResolution *pResolution, UINT32 nPrecinct, bool bCreateSubBands = false);
			virtual ~CPrecinct();
			/**
			 * Get X0 of specified precinct.
			 * @param pResolution	Parent resolution level
			 * @param nPrecinct		Precinct number within resolution level
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetX0(CResolution *pResolution, INT32 nPrecinct);
			/**
			 * Get X0 of specified precinct.
			 * @param pResolution	Parent resolution level
			 * @param nPrecinctX	Precinct X within resolution level
			 * @param nPrecinctY	Precinct Y within resolution level
			 * @return      INT32	Coordinate value.
			 */
			static INT32 sGetX0(CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY);

			/**
			 * Get Y0 of specified precinct.
			 * @param pResolution	Parent resolution level
			 * @param nPrecinct		Precinct number within resolution level
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetY0(CResolution *pResolution, INT32 nPrecinct);
			/**
			 * Get Y0 of specified precinct.
			 * @param pResolution	Parent resolution level
			 * @param nPrecinctX	Precinct X within resolution level
			 * @param nPrecinctY	Precinct Y within resolution level
			 * @return      INT32	Coordinate value.
			 */
			static INT32 sGetY0(CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY);

			/**
			 * Get X1 of specified precinct.
			 * @param pResolution	Parent resolution level
			 * @param nPrecinct		Precinct number within resolution level
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetX1(CResolution *pResolution, INT32 nPrecinct);
			/**
			 * Get X1 of specified precinct.
			 * @param pResolution	Parent resolution level
			 * @param nPrecinctX	Precinct X within resolution level
			 * @param nPrecinctY	Precinct Y within resolution level
			 * @return      INT32	Coordinate value.
			 */
			static INT32 sGetX1(CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY);

			/**
			 * Get Y1 of specified precinct.
			 * @param pResolution	Parent resolution level
			 * @param nPrecinct		Precinct number within resolution level
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetY1(CResolution *pResolution, INT32 nPrecinct);
			/**
			 * Get Y1 of specified precinct.
			 * @param pResolution	Parent resolution level
			 * @param nPrecinctX	Precinct X within resolution level
			 * @param nPrecinctY	Precinct Y within resolution level
			 * @return      INT32	Coordinate value.
			 */
			static INT32 sGetY1(CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY);

			/**
			 * Read a BufferType line from the input.
			 * @param		nCtx		read context
			 * @param		pDst		Destination buffer.
			 * @param		iBand		Output band
			 * @return      bool		true on succes, else false.
			 */
			virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iBand) NCS_OVERRIDE;

			virtual CNode2D::Context *NewContext() NCS_OVERRIDE{
				return((CNode2D::Context*)new CPrecinct::Context());
			};
			UINT32 GetRequestedQualityLevel(ContextID nCtx);

			void AddRef();
			void UnRef();
			INT32 NrRefs();
			static bool HaveZeroRefs();


			/**
				 * Destroy the subbands
				 */
			virtual bool DestroySubBands();

			virtual void Request(CIOStream *pStream);
			virtual void Cancel(CIOStream *pStream);

			void SetContextQualityLevel(ContextID nCtx, UINT32 nLevel);
			virtual bool UnLink(ContextID nCtx, UINT16 nInputs);

			UINT64 GetMemorySize(); 
			bool CreatePackets();	

			bool CalcFirstPacketNr();	

			std::vector<UINT32>* GetPacketIDs();

			void ReceivedBlock(CMemoryIOStream &Stream, CProgression &p, CPacket *pHeader);

			void SetSubbandCreationState(UINT8 nCodeBlockRead);
			UINT8 GetSubbandCreationState(); 
			bool SetSubbandDecodeState(UINT8 nCodeBlockRead); 
			UINT8 GetSubbandDecodeState(); 

#ifdef FORCE_DECODE_WHOLE_REGION_IN_LARGE_PRECINCT
			bool SetReadRect(ContextID nCtx, CRect2D& Rect);
			CRect2D& GetReadRect(ContextID nCtx);
#endif // FORCE_DECODE_WHOLE_REGION_IN_LARGE_PRECINCT

			UINT32 m_nDecodedToLayer;
			friend class CSubBand;

			CPrecinct *m_pNext;
			CPrecinct *m_pPrev;
		protected:

			class NCSECW_IMPEX Context : public NCS::SDK::CNode2D::Context
			{
			public:
				UINT32 m_nRequestedQualityLevel;

				Context();

#ifdef FORCE_DECODE_WHOLE_REGION_IN_LARGE_PRECINCT
				// added on 29/04/2014
				void SetReadRect(CRect2D& Rect){
					m_Rect = Rect;
				};
				CRect2D& GetReadRect() {
					return m_Rect;
				}

			private:
				// so rect represents the required reading region for the precinct (x, y) in this view
				CRect2D m_Rect;
#endif // FORCE_DECODE_WHOLE_REGION_IN_LARGE_PRECINCT
			};

			static SDK::CAtomicInt 	sm_nZeroRefs;

			/** Packet Read error is stored here */
			CError	m_Error;
			SDK::CAtomicInt		m_nRefs;
			/**
			* Create the subbands
			*/
			virtual bool CreateSubBands(Context *pCtx, bool bRead, NCS::SDK::CBuffer2D::Type eType);
			virtual bool DecodeSubBands(Context *pCtx, NCS::SDK::CBuffer2D::Type eType);
			virtual bool DecodeSubBands(Context *pCtx, NCS::SDK::CBuffer2D::Type eType, CRect2D& rectDst);
			virtual bool ReadPackets(Context *pCtx);

			UINT8 m_nSubBandsCreation; 
			UINT8 m_nSubBandsDecode;
			bool m_bSmallPrecinct;

			NCS::SDK::CSpinLockImpl *m_pMutex;
			NCS::SDK::CSpinRWLockImpl *m_prwMutex;
			NCS::SDK::CSpinRWLockImpl *m_prwMutex_subband_creation; 
			NCS::SDK::CSpinRWLockImpl *m_prwMutex_subband_decode;

		};

		class NCSECW_IMPEX CMTPrecinct : public NCS::JPC::CPrecinct {
		public:
			CMTPrecinct(class CResolution *pResolution, UINT32 nPrecinct, bool bCreateSubBands = false);
			virtual ~CMTPrecinct();

			void Release(); 

			class CReadJob : public NCS::CThreadPool::CWaitableJob {
			public:
				CReadJob(NCS::JPC::CSubBand *pSubBand, NCS::SDK::CBuffer2D::Type eType, UINT32 nRequestedQualityLevel);
				CReadJob(NCS::JPC::CSubBand *pSubBand, NCS::SDK::CBuffer2D::Type eType, CRect2D& rectDst, UINT32 nRequestedQualityLevel);
				virtual ~CReadJob();

				virtual bool DoWork(void);

			protected:
				
				NCS::JPC::CSubBand			*m_pSubBand;
				NCS::SDK::CBuffer2D::Type	m_eType;
				NCS::SDK::CRect2D			m_rectDst;
				bool						m_bUseRectDst;
				UINT32 m_nRequestedQualityLevel; 
			};
		protected: 
			virtual bool DecodeSubBands(Context *pCtx, NCS::SDK::CBuffer2D::Type eType) NCS_OVERRIDE;
			virtual bool DecodeSubBands(Context *pCtx, NCS::SDK::CBuffer2D::Type eType, CRect2D& rectDst)  NCS_OVERRIDE;
		};

		class NCSECW_IMPEX CPrecinctMap : public NCS::SDK::CNode2D {
		public:

			CPrecinctMap();
			virtual ~CPrecinctMap();
			void Init(class CResolution *pResolution);
			void ResetProgressionLayer(void);
			CPrecinct *find_or_create(UINT32 x, UINT32 y, bool *bCreated = NULL);
			CPrecinct *find(UINT32 nPrecinct, bool bRef = false);
			CPrecinct *find(UINT32 x, UINT32 y, bool bRef = false);
			CPrecinct *findPacketPrecinct(UINT32 nPacket, bool bRef = false);
			bool empty(UINT32 nY);
			void Clear(bool bLock = true);
			UINT8 ClearIfZero();
			UINT32 GetNodeWidth() {
				return(m_nWidth);
			}
			UINT32 GetNodeHeight() {
				return(m_nHeight);
			}
			UINT32 GetMaxSize() {
				return(m_nWidth * m_nHeight);
			}
			__inline UINT32 size(void) {
				return(m_nItems);
			}
			bool remove(UINT32 nPrecinct, bool bMustRemove = true);
			bool remove(UINT32 x, UINT32 y, bool bMustRemove = true);
			bool remove(CPrecinct *p, bool bMustRemove = true);
			bool removeIfZero(UINT32 x, UINT32 y);
			bool PurgePrecincts();

			UINT32 remove_front();

			/**
			 * UnLink all precincts.
			 * @param		nCtx		Read context
			 * @return      bool		true on success, else false on error.
			 */
			virtual bool UnLink(SDK::CNode2D::ContextID nCtx, UINT16 nInputs = 0);

#ifdef FORCE_DECODE_WHOLE_REGION_IN_LARGE_PRECINCT
			// added on 29/04/2014 to store rect for intdividual precinct
			class NCSECW_IMPEX Context : public NCS::SDK::CNode2D::Context
			{
			public:
				Context();
				virtual ~Context();

				void Init(UINT32 nWidth, UINT32 nHeight);

				bool SetReadRect(UINT32 nNodeX, UINT32 nNodeY, CRect2D& Rect);
				CRect2D& GetReadRect(UINT32 nNodeX, UINT32 nNodeY);

			private:
				// rect[y][x], y = 0-CPrecinctMap::m_nHeight, x = 0-CPrecinctMap::m_nWidth
				// so rect represents the required reading region for the precinct (x, y) in this view
				typedef std::vector<CRect2D> CRect2DVector;
				std::vector< CRect2DVector > m_RectMap;
				CRect2D mInvalidRect;
				UINT32 m_nWidth;
				UINT32 m_nHeight;
			};

			CNode2D::Context *NewContext() {
				return((CNode2D::Context*)new Context());
			};

			Context *GetContext(ContextID nCtx) {
				Context *pCtx = (CPrecinctMap::Context*)CNode2D::GetContext(nCtx, true);
				return pCtx;
			};
#endif //FORCE_DECODE_WHOLE_REGION_IN_LARGE_PRECINCT	

			UINT64 GetMemorySize(); 
			bool CreatePackets();	
			bool HasRefs();	
			void AddRefs();
			class CPrecinctsFactory *m_pPrecinctsFactory;
			NCS::SDK::CSpinRWLockImpl *m_prwMutex_Map; 

		private:
			friend class CPrecinctsFactory;
			class NCSECW_IMPEX CPrecinctMapRow {
				NCS::SDK::CSpinRWLockImpl *m_prwMutex;
				NCS::SDK::CSpinRWLockImpl *m_pBackFrontMutex;
				bool PurgePrecinctIfZero(UINT32 x);
			public:
				typedef std::map<UINT32, CPrecinct*>::iterator CPrecinctMapColumnIterator;

				std::map<UINT32, CPrecinct*> m_Columns;

				CPrecinctMapRow();
				virtual ~CPrecinctMapRow();

				void Clear(bool bLock = true); // Shao changed on 08/11/2011 to speed up when lock is not needed
				UINT32 PurgePrecincts();
				bool PurgePrecinct(UINT32 x);
				CPrecinct* FindPrecinctForPacket(UINT32 nPacket, bool bRef);
				CPrecinct *FindPrecinct(UINT32 x, bool bRef);
				CPrecinct* FindOrCreatePrecinct(UINT32 x, UINT32 y, CPrecinctsFactory*pPrecinctsFactory, bool &bCreated);
				void ResetProgressionLayer();
				void AddRefs();
				bool HasRefs();
				bool IsEmpty();
				void MoveBack(CPrecinct *pPrecinct);

				CPrecinct *m_pFront;
				CPrecinct *m_pBack;
			};
			typedef std::vector<CPrecinctMapRow>::iterator CPrecinctMapRowIterator;

			class NCSECW_IMPEX ICProgressionOrderPacketCalculator
			{
			public:
				ICProgressionOrderPacketCalculator() {
					m_pPrecinctMap = NULL;
					m_pResolution = NULL;
					m_nComponents = 0;
					m_nWidth = 0;
					m_nHeight = 0;
					m_nLayers = 0;
				};
				virtual ~ICProgressionOrderPacketCalculator() {
					m_pPrecinctMap = NULL;
					m_pResolution = NULL;
				};

				virtual void Init(CPrecinctMap *pPrecinctMap);
				virtual void Set(UINT32 x, UINT32 y, CPrecinct *p) = 0;
			protected:
				class CPrecinctMap *m_pPrecinctMap;
				class CResolution *m_pResolution;
				UINT32 m_nComponents;
				UINT32 m_nWidth;
				UINT32 m_nHeight;
				INT32 m_nLayers;
			};

			class NCSECW_IMPEX CPacketCalculatorLRCP : public ICProgressionOrderPacketCalculator
			{
			public:
				CPacketCalculatorLRCP() : ICProgressionOrderPacketCalculator() {
					m_nLayerFactor = 0;
					m_nResolutionOffset = 0;
				};
				virtual ~CPacketCalculatorLRCP() {};

				virtual void Init(CPrecinctMap *pPrecinctMap);
				virtual void Set(UINT32 x, UINT32 y, CPrecinct *p);
			private:
				UINT32 m_nLayerFactor;
				UINT32 m_nResolutionOffset;
			};

			class NCSECW_IMPEX CPacketCalculatorRLCP : public ICProgressionOrderPacketCalculator
			{
			public:
				CPacketCalculatorRLCP() : ICProgressionOrderPacketCalculator() {
					m_nStartPrecinctNumber = 0;
				};
				virtual ~CPacketCalculatorRLCP() {};

				virtual void Init(CPrecinctMap *pPrecinctMap);
				virtual void Set(UINT32 x, UINT32 y, CPrecinct *p);
			private:
				UINT32 m_nStartPrecinctNumber;
			};

			class NCSECW_IMPEX CPacketCalculatorRPCL : public ICProgressionOrderPacketCalculator
			{
				UINT32 CalculateFirstPacketForPrecinct(CTilePartHeader *pMainTP, CPOCMarker::ProgressionOrder *pPO, INT32 nPrecinctX, INT32 nPrecinctY);
			public:
				CPacketCalculatorRPCL() : ICProgressionOrderPacketCalculator() {
					m_nStartPrecinctNumber = 0;
				};
				virtual ~CPacketCalculatorRPCL() {};

				virtual void Init(CPrecinctMap *pPrecinctMap);
				virtual void Set(UINT32 x, UINT32 y, CPrecinct *pPrecinct);
				
			private:
				UINT32 m_nStartPrecinctNumber;
			};

			class NCSECW_IMPEX CPacketCalculatorPCRL : public ICProgressionOrderPacketCalculator
			{
				UINT32 CalculateFirstPacketNumberForPrecinct(CTilePartHeader *pMainTP, CPOCMarker::ProgressionOrder *pPO, INT32 nPrecinctX, INT32 nPrecinctY);
			public:
				CPacketCalculatorPCRL() : ICProgressionOrderPacketCalculator(){
				};
				virtual ~CPacketCalculatorPCRL() {
				};

				virtual void Init(CPrecinctMap *pPrecinctMap);
				virtual void Set(UINT32 x, UINT32 y, CPrecinct *pPrecinct);
	
			};

			class NCSECW_IMPEX CPacketCalculatorCPRL : public ICProgressionOrderPacketCalculator
			{
			public:
				CPacketCalculatorCPRL() {
					m_nComponentOffset = 0;
					m_nResolutionOffset = 0;
				};
				virtual ~CPacketCalculatorCPRL() {};

				virtual void Init(CPrecinctMap *pPrecinctMap);
				virtual void Set(UINT32 x, UINT32 y, CPrecinct *p);
			private:
				UINT32 m_nComponentOffset;
				UINT32 m_nResolutionOffset;
			};

			UINT32 m_nWidth;
			UINT32 m_nHeight;
			class CResolution *m_pResolution;

			SDK::CAtomicUnsignedInt m_nItems;

			std::vector<CPrecinctMapRow *> m_Rows;
			ICProgressionOrderPacketCalculator *m_pProgressionOrderCalculator;

			};
		}
	}

#endif // !NCSJPCPRECINCT_H
