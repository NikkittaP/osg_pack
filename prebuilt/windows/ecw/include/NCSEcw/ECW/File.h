/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     File.h 
** CREATED:  21Mar06 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CFile class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSECWFILE_H
#define NCSECWFILE_H

#ifndef NCSJP2DEFS_H
#include "NCSEcw/JPC/Types/Defs.h"
#endif // NCSJP2DEFS_H

#ifndef NCSMULTILOCK_H
#include "NCSMultiLock.h"
#endif //NCSMULTILOCK_H

#ifndef NCSECW_H
#include "NCSEcw/ECW/NCSEcw.h"
#endif // NCSECW_H

// Box class
#ifndef NCSSDKBOX_H
#include "NCSEcw/SDK/Box.h"
#endif // NCSSDKBOX_H

#ifndef NCSFILEBASE_H
#include "NCSEcw/SDK/FileBase.h"
#endif //NCSFILEBASE_H

#ifndef NCSGLOBALINFO_H
#include "NCSEcw/SDK/GlobalInfo.h"
#endif //NCSGLOBALINFO_H

#ifndef NCS_SDK_ATOMIC_H
#include "NCSEcw/SDK/Atomic.h"
#endif //NCS_SDK_ATOMIC_H

// ECW SuperBox class
#ifndef NCSECWSUPERBOX_H
#include "NCSEcw/ECW/SuperBox.h"
#endif // NCSECWSUPERBOX_H

// ECW ComponentDepthType
#ifndef NCSECWCOMPONENTDEPTHTYPE_H
#include "NCSEcw/ECW/ComponentDepthType.h"
#endif // NCSECWCOMPONENTDEPTHTYPE_H

#include "NCSThreadPool.h"
#include "NCSEcw/JPC/Nodes/SubBand.h"

#include <vector>
#include <map>
#include <deque>
#include <bitset>

#include "NCSEcw/SDK/NodeTiler2D_T.h"

#include "NCSEcw/API/HeaderEditor.h"

#include "NCSEcw/ECW/GeotiffTagBox.h"

#include "NCSEcw/SDK/spin_lock.h"

#include "NCSXmlUtils.h"

//#define USE_SCATTER_PRECINCT_READ_TASKS
//#define USE_CRATTER_SUBBAND_READ_TASKS

// NOTE - if you define COMP_WORKER you will need to muck around with 
// the tbb stuff in InputNode.h to make sure it is "hidden" like the rest of it
// - it was not done initially because COMP_WORKER was not defined so not used.
//#define COMP_WORKER // Use multiple threads to read the YUV bands

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4275)
#endif

namespace NCS {
	namespace SDK {
		
		class CBlockFile;
		class CECWP3IOStream;
		class ITileCompressorSource;
	}
	class CMemoryIOStream;
	class CFixedSizeMemoryPool;
namespace ECW {
#if !defined(ECWP2_DISABLED)
    class CECWPIOStream;
#endif
	class CWriter;
	class CUpdater;

	
/**
 * CFile class - the ECW file class.
 * The ECWFile is the base shared file object used by multiple CNCSFile classes to 
 * access an ECW file.
 * 
 * @author       Simon Cope
 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
 */
class NCSECW_IMPEX CFile: public CSuperBox, public SDK::CFileBase {
public:
	class CCachedBlock;
	class CSubBand;
	class CPrecinct;
	//class CPrecinctListReadAccessor;
	class CResolution;
	class CComponent;
	class CPrecinctList;
	class CPrecinctListReadAccessor;
	class CPrecinctListWriteAccessor;
	class CCachedBlockList;
	class COffsetCache;
	/*
	** The task scheduler is not currently used and is causing an assert
	** in debug mode.  See the following bug log for details: 
	** http://swengjira/browse/EMS-242 - "in debug mode you can often see an assert failed on exit"
	**
	** The tbb::task_scheduler_init class usage below appears to be incorrect - a static 
	** instance of tbb::task_scheduler_init is used and initialized with 1 thread.  
	**
	** The tbb::task_scheduler_init class reference states "For production code, the default value 
	** of -1 (automatic) should be used." in relation to the number of threads the task scheduler 
	** should be initialized with.
	**
	** The tbb::task_scheduler_init class reference also states "A thread may construct multiple 
	** task_scheduler_inits. Doing so does no harm because the underlying scheduler is reference counted."
	** So there may be no need to use a static instance of tbb::task_scheduler_init.
	*/
	//static tbb::task_scheduler_init sm_ts;

	// The type of request for a block in a file cache list
	typedef enum {
		BLOCK_INVALID	= 0,	// invalid block request
		BLOCK_REQUEST	= 1,	// Post a Request for the block
		BLOCK_CANCEL		= 2,	// Post a Cancel for the block
		BLOCK_RETURN		= 3		// Return the block in the cache list
	} NCSEcwBlockRequestMethod;

		// 00 10 20
		// 01 XX 21
		// 02 12 22
	typedef enum {
		NEIGHBOUR_00 = 0,
		NEIGHBOUR_10 = 1,
		NEIGHBOUR_20 = 2,
		NEIGHBOUR_01 = 3,
		NEIGHBOUR_21 = 4,
		NEIGHBOUR_02 = 5,
		NEIGHBOUR_12 = 6,
		NEIGHBOUR_22 = 7,
		SELF		 = 8
	} Neighbour;
	
	//only available for file version 3 or later
	struct UnusedFileSizeInfo
	{
		UINT64 m_nTotalFileSize;
		UINT64 n_nRealFileSizeInUse; // this is the size necessary for the file to use, (not including size of unused blocks)
		//UINT64 m_nFreedBoxesSize;
	};

	class NCSECW_IMPEX CSubBand : public SDK::CBuffer2D, public NCS::SDK::CSpinRWLock {
	public:
		UINT8	LEFT_BORDER;
		UINT8	RIGHT_BORDER;
		UINT8	TOP_BORDER;
		UINT8	BOTTOM_BORDER;

		// Required for ECWP
		bool Incomplete() { return m_bIncomplete; };
		void Incomplete( bool bIncomplete ) { m_bIncomplete = bIncomplete; };

		CSubBand();
		virtual ~CSubBand();
		virtual CSubBand &operator=(const CSubBand &s);

		NCS_INLINE virtual JPC::CSubBand::Type Type() { return(m_eSBType); };
		NCS_INLINE void Type(JPC::CSubBand::Type e) { 
				m_eSBType = e; 

				switch(e) {
					case JPC::CSubBand::LL:
							LEFT_BORDER = 0;
							TOP_BORDER = 0;
						break;
					case JPC::CSubBand::HL:
							RIGHT_BORDER = 0;
							TOP_BORDER = 0;
						break;
					case JPC::CSubBand::LH:
							LEFT_BORDER = 0;
							BOTTOM_BORDER = 0;
						break;
					case JPC::CSubBand::HH:
							RIGHT_BORDER = 0;
							BOTTOM_BORDER = 0;
						break;
				}
			};
		
		NCS_INLINE CPrecinct *Precinct() { return(m_pPrecinct); };
		NCS_INLINE void Precinct(CPrecinct *pPrecinct) { m_pPrecinct = pPrecinct; };
		CONDITIONAL_FORCEINLINE CResolution *Resolution(); // { return(Precinct()->Resolution()); };
		CONDITIONAL_FORCEINLINE CComponent *Component(); //  { return(Resolution()->Component()); };
		CONDITIONAL_FORCEINLINE CFile *File(); // { return(Component()->File()); };

		NCS_INLINE bool Zero() const { return(m_bIsZero); };
		NCS_INLINE void Zero(bool b) { m_bIsZero = b; };				

		NCS_INLINE bool HaveBorders() const { return(m_bHaveBorders); };
		NCS_INLINE void HaveBorders(bool b) { m_bHaveBorders = b; };
		
		CONDITIONAL_FORCEINLINE void SetupBorders(CPrecinct &P, CPrecinctListReadAccessor acc_w[9]); 
		
		CONDITIONAL_FORCEINLINE bool CopyNeighbourOverlap(CPrecinct &P, Neighbour f, CPrecinctListReadAccessor acc_w[9]);
		CONDITIONAL_FORCEINLINE bool CheckZeroNeighbourOverlap(CPrecinct &P, Neighbour f, CPrecinctListReadAccessor acc_w[9]);

	protected:
		// ECWP: indicates that the required block was missing when the subband was unpacked
		bool m_bIncomplete;

		JPC::CSubBand::Type	m_eSBType;
		bool	m_bHaveBorders;
		bool	m_bIsZero;
		CPrecinct *m_pPrecinct;

	};


#ifndef NCS_MOBILE
	class IWriterContextContext;
	class NCSECW_IMPEX IWriterContext : public NCS::CThread {
		IWriterContextContext *m_pContext;
	protected: 
		void InitializeMemoryPool(UINT32 nWidth, UINT32 nHeight, SDK::CBuffer2D::Type eType, size_t nMaxBuffersInQueue, NCS::SDK::CAtomicUnsignedInt64 *pMemoryCounter);
	public:
		
		IWriterContext();
		virtual ~IWriterContext();
		virtual void Work(void *pData) = 0;

		typedef struct {
			UINT16 nB;
			UINT8 nR;
			UINT32 nID;
			bool bIsAllOpacity;
			UINT32 nLLLength, nLHLength, nHLLength, nHHLength;
			void *pLL, *pLH, *pHL, *pHH;
		} PrecinctOutput;

		virtual void PrecinctDone(PrecinctOutput &Output) = 0;
		virtual void NULLPrecinctDone(NCSBlockId &nID, UINT16 nC) = 0;

		virtual void WriteStatus(const NCS::CString &sStatusText) = 0;


		virtual bool WriteError(NCS::CError eError, const NCS::CString &sErrorText) = 0;
		virtual bool Complete() = 0;
		NCS::CFixedSizeMemoryPool* GetInputMemoryPool();
		
		
	};


	class NCSECW_IMPEX CWriterContext : public IWriterContext {
	public:
		CWriterContext(ECW::CWriter *pWriter, ECW::CFile &File);
		virtual ~CWriterContext();

		virtual void Work(void *pData);
		
		virtual void PrecinctDone(PrecinctOutput &Output);
		virtual void NULLPrecinctDone(NCSBlockId &nID, UINT16 nC);

		virtual void WriteStatus(const NCS::CString &sStatusText);
		virtual bool WriteError(NCS::CError eError, const NCS::CString &sErrorText);

		virtual bool Complete();

		//static CFixedSizeMemoryPool *s_pInputMemoryPool;
	protected:
		friend class CPrecinct;
#if (defined(WIN32) || defined(LINUX))
		NCS_DLHANDLE m_xTBBMalloc;
#endif
		void SendStatus();
		NCS::SDK::CAtomicUnsignedInt64 m_nPrecinctsCompleted;
		NCS::SDK::CAtomicUnsignedInt64 m_nBlocksCompleted;
		NCS::SDK::CAtomicUnsignedInt64 m_nNULLBlocks;
		class CBlockPart;
		class CBlockAssembler;
		CBlockAssembler *m_pBlockParts;
		ECW::CWriter *m_pWriter;
		ECW::CFile &m_File;
		NCS::CMutex m_mCurrentStatus;
		NCS::CString m_sCurrentStatus;
		NCS::CEvent m_BlockReadyEvent;
	private:
		const CWriterContext &operator=(const CWriterContext &s);
	};
#endif

	class NCSECW_IMPEX CPrecinct: public SDK::CNode2D {
		void EnsureSubBand(CSubBand &subBand);
	public:
		typedef enum {
			NorthWest = 0,
			NorthEast = 1,
			SouthWest = 2,
			SouthEast = 3,
			OverlapNorth_Left = 4,
			OverlapNorth_Right = 5,
			OverlapSouth_Left = 6,
			OverlapSouth_Right = 7,
			OverlapEast_Top = 8,
			OverlapEast_Bottom = 9,
			OverlapWest_Top = 10,
			OverlapWest_Bottom = 11,
			OverlapNorthWest = 12,
			OverlapNorthEast = 13,
			OverlapSouthWest = 14,
			OverlapSouthEast = 15,
			All = 16
		} SourceRegion;

#define NCS_ECW_SOURCE_NW_MASK		(0x1 << ECW::CFile::CPrecinct::NorthWest)
#define NCS_ECW_SOURCE_NE_MASK		(0x1 << ECW::CFile::CPrecinct::NorthEast)
#define NCS_ECW_SOURCE_SW_MASK		(0x1 << ECW::CFile::CPrecinct::SouthWest)
#define NCS_ECW_SOURCE_SE_MASK		(0x1 << ECW::CFile::CPrecinct::SouthEast)
#define NCS_ECW_SOURCE_ONL_MASK		(0x1 << ECW::CFile::CPrecinct::OverlapNorth_Left)
#define NCS_ECW_SOURCE_ONR_MASK		(0x1 << ECW::CFile::CPrecinct::OverlapNorth_Right)
#define	NCS_ECW_SOURCE_OSL_MASK		(0x1 << ECW::CFile::CPrecinct::OverlapSouth_Left)
#define	NCS_ECW_SOURCE_OSR_MASK		(0x1 << ECW::CFile::CPrecinct::OverlapSouth_Right)
#define NCS_ECW_SOURCE_OET_MASK		(0x1 <<	ECW::CFile::CPrecinct::OverlapEast_Top)
#define NCS_ECW_SOURCE_OEB_MASK		(0x1 <<	ECW::CFile::CPrecinct::OverlapEast_Bottom)
#define NCS_ECW_SOURCE_OWT_MASK		(0x1 << ECW::CFile::CPrecinct::OverlapWest_Top)
#define NCS_ECW_SOURCE_OWB_MASK		(0x1 << ECW::CFile::CPrecinct::OverlapWest_Bottom)
#define NCS_ECW_SOURCE_ONW_MASK		(0x1 << ECW::CFile::CPrecinct::OverlapNorthWest)
#define NCS_ECW_SOURCE_ONE_MASK		(0x1 << ECW::CFile::CPrecinct::OverlapNorthEast)
#define NCS_ECW_SOURCE_OSW_MASK		(0x1 << ECW::CFile::CPrecinct::OverlapSouthWest)
#define NCS_ECW_SOURCE_OSE_MASK		(0x1 << ECW::CFile::CPrecinct::OverlapSouthEast)
#define NCS_ECW_SOURCE_ALL_MASK		(0x1 << ECW::CFile::CPrecinct::All)

		CPrecinct(CResolution *pResolution=NULL, CCachedBlock *pCB=NULL);
		CPrecinct(const CPrecinct &s);
		virtual ~CPrecinct();			

		virtual CPrecinct &operator=(const CPrecinct &s);

		virtual bool Init(CResolution *pResolution, NCSBlockId nId, bool bIsUpdate);//CCachedBlock *pCB);

		bool HaveLL();
		bool HaveLL(bool b);

		NCS_INLINE CResolution *Resolution() { return(m_pResolution); };
		void NCS_INLINE Resolution(CResolution *pResolution) { m_pResolution = pResolution; };
		CONDITIONAL_FORCEINLINE CComponent *Component();
		CONDITIONAL_FORCEINLINE CFile *File();

		void CachedBlock(CCachedBlock *p);

		CONDITIONAL_FORCEINLINE NCSBlockId GetNeighbourId(Neighbour f);
		//Internal meaning no borders. 
		virtual CSubBand &GetLLInternal();
		virtual CSubBand &GetLLInternal(UINT8 inCellBitDepth);
		virtual CSubBand &GetSubBandInternal(JPC::CSubBand::Type e);

		NCS_INLINE INT32 GetX() { return(m_nX); };
		NCS_INLINE INT32 GetY() { return(m_nY); };
		NCS_INLINE NCSBlockId GetId() { return(m_nId); };

		virtual CSubBand &SubBand(JPC::CSubBand::Type e, CPrecinctListReadAccessor acc_w[9]);
		virtual CSubBand &GetLL(CPrecinctListReadAccessor acc_w[9]);
		virtual CSubBand &GetHL(CPrecinctListReadAccessor acc_w[9]);
		virtual CSubBand &GetLH(CPrecinctListReadAccessor acc_w[9]);
		virtual CSubBand &GetHH(CPrecinctListReadAccessor acc_w[9]);


		virtual void PreErase(void);

		virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);
#ifndef NCS_MOBILE
		virtual bool Write(CBuffer2D &Src, std::bitset<4> &bInputHasData, bool &bComplete, IWriterContext *pWriterContext);
		virtual bool Write(CBuffer2D &Src, SourceRegion eRegion, bool bInputHasData, bool &bComplete, IWriterContext *pWriterContext);
		virtual bool PushLLUp(SDK::CBuffer2D &LL, IWriterContext *pWriterContext);
		virtual UINT8 *PackSubBand(CFile::CSubBand &SubBand, UINT32 &nPackedLength);
		void UpdateRequiredRegions(CResolution *pResolution);
		static void WriteLLOutput(const NCS::CString& tmpFolderPath, SDK::CBuffer2D& buffer, UINT32 nComponent,
			UINT32 nlevel, SDK::CRect2D& rect, bool bIsYUV);
#endif
		virtual CSubBand &SubBand(JPC::CSubBand::Type e);
#ifdef NODETILER2D_READCONST
		virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent) const;
#endif
		static void Init();
		static void Fini();

		class CInputBuffer : public NCS::SDK::CBuffer2D
		{
		public:
			CInputBuffer() : NCS::SDK::CBuffer2D() {
				m_bRestrictedViewSet = false;
				m_RestrictedX0 = 0;
				m_RestrictedY0 = 0;
				m_RestrictedX1 = 0;
				m_RestrictedY1 = 0;
				m_pMemoryCounter = &NCS::SDK::CBuffer2D::sm_nAllocCompressionBytes;
			}
			void SetRestrictedView(INT32 nX0, INT32 nY0, INT32 nX1, INT32 nY1) {
				m_bRestrictedViewSet = true;
				m_RestrictedX0 = nX0;
				m_RestrictedY0 = nY0;
				m_RestrictedX1 = nX1;
				m_RestrictedY1 = nY1;
			}
			NCS_INLINE INT32 GetRestrictedX0() const {
				if (m_bRestrictedViewSet) {
					return(m_RestrictedX0);
				}
				return(GetX0());
			};
			NCS_INLINE INT32 GetRestrictedY0() const {
				if (m_bRestrictedViewSet) {
					return(m_RestrictedY0);
				}
				return(GetY0());
			}
			NCS_INLINE INT32 GetRestrictedX1() const {
				if (m_bRestrictedViewSet) {
					return(m_RestrictedX1);
				}
				return(GetX1());
			}
			NCS_INLINE INT32 GetRestrictedY1() const {
				if (m_bRestrictedViewSet) {
					return(m_RestrictedY1);
				}
				return(GetY1());
			}
		protected:
			bool m_bRestrictedViewSet;
			INT32 m_RestrictedX0;
			INT32 m_RestrictedY0;
			INT32 m_RestrictedX1;
			INT32 m_RestrictedY1;
		};
		CInputBuffer *InputData() { return m_pEncodeData; };
	protected:
		static SDK::CBuffer2D *s_pTextureBuf_8;

		NCS::SDK::CAtomicInt m_bHaveLL;
		class CResolution *m_pResolution;

		friend class CCachedBlock;
		CSubBand m_LL;
		CSubBand m_LH;
		CSubBand m_HL;
		CSubBand m_HH;

		bool m_bBlockHasLL;

		INT32 m_nX;
		INT32 m_nY;
		NCSBlockId m_nId;

#ifdef USE_SCATTER_PRECINCT_READ_TASKS
		tbb::queuing_mutex	*m_pMutex; // not needed unless doing precinct_read_tasks
		tbb::empty_task *m_NeighbourSignalList[9];
#endif
#ifdef USE_CRATTER_SUBBAND_READ_TASKS
		int				m_nSubBandSignals[4];
		tbb::empty_task *m_SubBandSignalList[4][4];
#endif
		
		// Tile-based encoding members
		CInputBuffer *m_pEncodeData;
		UINT32 m_nReceivedRegions;
		// regions that are required to complete
		UINT32 m_nRequiredRegions;
		bool m_bIsUpdate;
		bool m_bHasReadOriginalPrecinct;
		bool *m_pInputDataMask;
		bool m_bHasInputData;

		// This class and the GenerateLoHoPair function should be
		// replaced with a 2D filter
		class CLOHIPair
		{
		public:
			CLOHIPair() {};
			~CLOHIPair() {};

			SDK::CBuffer2D &LO() { return m_LO; };
			SDK::CBuffer2D &HI() { return m_HI; };

			bool Parse(NCS::CIOStream &Stream) {
				return false;
			}
			bool UnParse(NCS::CIOStream &Stream) {
				return false;
			}
		protected:
			SDK::CBuffer2D m_LO;
			SDK::CBuffer2D m_HI;
		};

#ifndef NCS_MOBILE
		virtual bool GenerateLoHiPair(CInputBuffer *pInputData, CLOHIPair &LoHiPair);
		virtual bool WriteSubBands(CInputBuffer *pInputData);
		virtual bool WriteSubBandsSSE();
		
#ifdef NCS_X86_MMI_AVX2
		virtual bool WriteSubBandsFMA3();
#endif // NCS_X86_MMI_AVX2
#ifdef NCS_X86_MMI_AVX
		virtual bool WriteSubBandsAVX();
#endif // NCS_X86_MMI_AVX
		virtual bool WriteInit(IWriterContext *pWriterContext);
		virtual bool WriteInternal(CBuffer2D &Src, bool &bComplete, IWriterContext *pWriterContext);
	
#endif // NCS_MOBILE
	};

	class NCSECW_IMPEX CComponent: public SDK::CNode2D {
	public:
		CComponent(CFile *pFile, UINT16 c);
		virtual ~CComponent();

		void Index(UINT32 nComponent) {	m_nComponent = nComponent; };
		UINT32 Index(void) { return(m_nComponent); };
		NCS_INLINE CFile *File() { return(m_pFile); }
		NCS_INLINE CResolution *Resolution(UINT8 r) { return(m_Resolutions[r]); }
		NCS_INLINE UINT8 NrResolutions() { return((UINT8)m_Resolutions.size()); }

		virtual bool Link(ContextID nCtx, UINT32 nResolution, bool bEnableDither);
		virtual bool UnLink(ContextID nCtx, UINT32 nResolution);

		virtual bool Read(ContextID nCtx, CBuffer2D *pDst, UINT16 iComponent);

		virtual UINT8 BitDepth() const { return m_nCellBitDepth; };
		virtual bool IsAllOpacity() const { return m_bIsAllOpacity; };

		class NCSECW_IMPEX Context : public CNode2D::Context {
		public:
			Context() : m_bEnableDitherForView(false) {}
			bool m_bEnableDitherForView;
		};
		Context *GetComponentContext(ContextID nCtx) {
			return static_cast<Context*>(GetContext(nCtx));
		}
#ifdef COMP_WORKER
		typedef NCS::CThreadPool::CJob CWorkerJob;
		virtual void QueueJob(CWorkerJob *p) { m_Worker.QueueJob(p); };
	protected:
		NCS::CThreadPool m_Worker;
#else
	protected:
#endif
		virtual CNode2D::Context *NewContext() {
			return((CNode2D::Context*)new Context());
		}
		UINT32 m_nComponent;
		std::vector<CResolution*> m_Resolutions;
		CFile *m_pFile;
		UINT8 m_nCellBitDepth;
		bool m_bIsAllOpacity;
	};
	class NCSECW_IMPEX CCachedBlock: public SDK::CRect2D {
	public:
		CFile *m_pFile;
		NCSBlockId	m_nBlockNumber;	// block number being cached

		UINT32 m_nBlockX;
		UINT32 m_nBlockY;
		UINT32 m_nSidebands;
		
		UINT8	*m_pPacked;	// the packed ECW block that is currently cached (or NULL)
		UINT32	m_nPackedLength;	// Length of the packed block
		
		NCS::SDK::CAtomicInt64 m_nUsageCount;		// number of times in use by FileViews
		NCS::SDK::CAtomicInt64 m_nHitCount;			// number of times block has been read during inverse DWT operations
		bool	m_bHasLL;
		bool	m_bAligned;			// Packed block is aligned
		bool	m_bRequested;			// only TRUE if block request HAS BEEN SENT to the server
									// (so FALSE in every other situation, even if block not loaded yet)

		void SetFile(CFile *pFile) { m_pFile = pFile; };
		bool Packed() const { return(m_pPacked != NULL); };

		CCachedBlock(CFile *pFile = NULL, NCSBlockId nBlockNumber = 0);
		CCachedBlock(const CCachedBlock &s);

		virtual CCachedBlock &operator=(const CCachedBlock &s);

		void Init(CFile *pFile, NCSBlockId nBlockNumber);
		virtual ~CCachedBlock();

		NCSError UnPack(CSubBand &S) const;
		void FreePacked();
		void SetPacked(UINT8*p, UINT32 l, bool bAlign);
		bool InUse();
	
	};

	// replace NCSFileBlockOffsetEntry with CCachedOffset
	class NCSECW_IMPEX CCachedOffset {
	public:
		explicit CCachedOffset(NCSBlockId nBlockNumber = 0);
		CCachedOffset(const CCachedOffset &s);

		virtual CCachedOffset &operator=(const CCachedOffset &s);
		virtual ~CCachedOffset();

		UINT32		nLength;
		UINT64		nOffset;

		NCSBlockId	m_nBlockNumber;					// block number being cached
		NCS::SDK::CAtomicInt64 m_nUsageCount;		// number of times in use by FileViews
	};

	//typedef struct {
		//UINT32		nLength;
		//UINT64		nOffset;
	//} NCSFileBlockOffsetEntry;



	class NCSECW_IMPEX CQMFBox: public CBox {
	public:
			/** Box type */ 
		static UINT32	sm_nTBox;

			/** Default constructor, initialises members */
		CQMFBox();
			/** Virtual destructor */
		virtual ~CQMFBox();

		/**
		 * \fn	virtual CError CQMFBox::Parse(class CFileBase &FileBase, CIOStream &Stream);
		 *
		 * \brief	Parse the ECW box from the ECW file.
		 *
		 * \param [in,out]	FileBase	ECW file being parsed
		 * \param [in,out]	Stream  	IOStream to use to parse file.
		 *
		 * \return	CError	NCS_SUCCESS, or Error code on failure.
		**/
		virtual CError Parse(class CFileBase &FileBase, CIOStream &Stream);

		/**
		 * \fn	virtual CError CQMFBox::UnParse(class CFileBase &FileBase, CIOStream &Stream);
		 *
		 * \brief	Unparse the ECW box from the ECW file.
		 *
		 * \param [in,out]	FileBase	ECW file being parsed
		 * \param [in,out]	Stream  	IOStream to use to parse file.
		 *
		 * \return	CError	NCS_SUCCESS, or Error code on failure.
		**/
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);

		/**
		 * \fn	virtual CError CQMFBox::UpdateBoxDataLength(const UINT64 &nNewBoxLength,
		 * 		CIOStream &Stream);
		 *
		 * \brief	Updates the box data length.
		 *
		 * \param	nNewBoxLength 	Length of the new box.
		 * \param [in,out]	Stream	The stream.
		 *
		 * \return	Result.
		**/
		virtual CError UpdateBoxDataLength(const UINT64 &nNewBoxLength, CIOStream &Stream);

		CError UnparseToMemImage(class CFileBase &FileBase, CMemoryIOStream& memIOStream);

	private:
		UINT8* m_pMemImage;
		UINT64 m_nLengthForMemImage;
	};


	class NCSECW_IMPEX CSignatureBox: public CBox {
		public:
				/** Box type */ 
			static UINT32	sm_nTBox;
				/** ECW file signature */
			static UINT32	sm_ECWSignature;

				/** Signature as read from file */
			UINT32		m_nSignature;

				/** Version number */
			UINT32		m_nVersion;

				/** Default constructor, initialises members */
			CSignatureBox();
				/** Virtual destructor */
			virtual ~CSignatureBox();
				/** 
				 * Update the extended length of the box prior to writing
				 */
			virtual void UpdateXLBox(void);
				/** 
				 * Parse the signature box from the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError Parse(class CFileBase &FileBase, CIOStream&Stream);
				/** 
				 * UnParse the signature box to the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	};

		/**
		 * CFileTypeBox class - the ECW file type box class.  This box is always present.
		 * 
		 * @author       Simon Cope
		 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
		 */	
	class NCSECW_IMPEX CFileTypeBox: public CBox {
	public:
			/** Compatibility list entry, 4 chars as UINT32 */
		typedef UINT32 CCLEntry;
			/** Compatibility list */
		typedef std::list<CCLEntry> CCLList;

			/** Box type */
		static UINT32	sm_nTBox;
			/** ECW Brand */
		static UINT32	sm_ECWBrand;

			/** Brand as read from file */
		UINT32			m_Brand;
			/** Minor version */
		UINT32			m_nMinV;
			/** Compatibility list - other files may be readable as ECW, eg JPX files */
		CCLList	m_CLList;

			/** Default constructor, initialises members */
		CFileTypeBox();
			/** Virtual destructor */
		virtual ~CFileTypeBox();
			/** 
			 * Update the extended length of the box prior to writing
			 */
		virtual void UpdateXLBox(void);

			/** 
			 * Parse the file type box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream&Stream);
			/** 
			 * UnParse the file type box to the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	};
		/**
		 * CHeaderBox class - the ECW file header box class.  This box is always present.
		 * 
		 * @author       Simon Cope
		 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
		 */	
	class NCSECW_IMPEX CHeaderBox: public CSuperBox {
	public:
			/**
			 * CImageHeaderBox class - the ECW file image header box class.  This box is always present.
			 * 
			 * @author       Simon Cope
			 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
			 */	
		class NCSECW_IMPEX CImageHeaderBox: public CBox, public NCS::ECW::CComponentDepthType {
		public:
				/** Box type */
			static UINT32		sm_nTBox;
			static UINT8		sm_nVersion;

				/** Image Width */
			UINT32				m_nWidth;
				/** Image Height */
			UINT32				m_nHeight;
				/** Number of image components, eg RGB = 3 */
			UINT16				m_nComponents;
				/** Compression rate */
			UINT16				m_nCompressionRate;

			NCSBlockingFormat	m_nBlockingFormat;
			NCSCompressFormat	m_nCompressFormat;
			UINT16				m_nXBlockSize;
			UINT16				m_nYBlockSize;
			UINT8				m_nNrLevels;
			UINT8				m_nNrSidebands;

				/** Actual colorspace in image is known, 0 == known & ColourSpaceBox available, 1 == unkown */
			UINT8				m_nUnknownColorspace;
				/** Intellectual property, 0 = none, 1 - IPRBox present */
			UINT8				m_nIPR;

				/** Cell bits for MULTIBAND, RGB, YUV and GREYSCALE, currently support 8bit (default) and 16bit (progressing) */
				// Shao added to distingwish 8 and 16bit
			UINT8				m_nCellBitDepth;

				/** Default constructor, initialises members */
			CImageHeaderBox();
				/** Virtual destructor */
			virtual ~CImageHeaderBox();

				/** 
				 * Parse the image header box from the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError Parse(class CFileBase &FileBase, CIOStream&Stream);
				/** 
				 * UnParse the image header box to the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			//virtual CError UnParse(class CFile &ECWFile, CIOStream &Stream);
			virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);

		private:
			UINT64 CalculateDataSize(void);
		};
			/**
			 * CBitsPerComponentBox class - the ECW file bits per component box class.
			 * This box is only present if m_nBitsPerComponent in the IMageHeaderBox is 255, indicating
			 * each component may be a different bit depth (eg, 5/6/6 RGB).
			 * 
			 * @author       Simon Cope
			 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
			 */	
		class NCSECW_IMPEX CBitsPerComponentBox: public CBox {
		public:
				/** Box type */
			static UINT32	sm_nTBox;

				/** Bit depth for each component */
			std::vector<NCS::ECW::CComponentDepthType>	m_Bits;

				/** Default constructor, initialised members */
			CBitsPerComponentBox();
				/** Virtual destructor */
			virtual ~CBitsPerComponentBox();
				/** 
				 * Update the extended length of the box prior to writing
				 */
			virtual void UpdateXLBox(void);

				/** 
				 * Parse the bits per component box from the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError Parse(class CFileBase &FileBase, CIOStream &Stream);
				/** 
				 * UnParse the bits per component box to the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
		};

			/**
			 * CChannelDefinitionBox class - the ECW file channel definition box class.
			 * This box is optional, but is used for opacity stuff
			 * 
			 * @author       Simon Cope
			 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
			 */	

		class NCSECW_IMPEX CChannelDefinitionBox: public CBox {
		public:
				/** Channel type */
			typedef enum {
					/** Color value */
				COLOR			= 0,
					/** Opacity value */
				OPACITY			= 1,
					/** Premultiplied opacity value */
				PREMUL_OPACITY	= 2,
					/** Channel type is unknown */
				UNKNOWN			= 0xffff
			} Type;

				/** Color channel this channel is associated with */
			typedef enum {
					/** The entire image, eg RGBA */
				WHOLE_IMAGE		= 0,
					/** Red channel */
				R				= 1,
					/** Green channel */
				G				= 2,
					/** Blue channel */
				B				= 3,
					/** Luminance/Greyscale channel */
				Y				= 1,
					/** Cb channel */
				Cb				= 2,
					/** Cr channel */
				Cr				= 3,
					/** No channel associated */
				NONE			= 0xffff
			} Association;

				/** A channel definition */
			typedef struct {
					/** 0-based index of channel this definition is for */
				UINT16		m_iChannel;
					/** Channel type */
				Type		m_eType;
					/** Channel color component association */
				Association	m_eAssociation;
			} ChannelDefinition;

				/** Box type */
			static UINT32	sm_nTBox;

				/** Number of definitions */
			UINT16		m_nEntries;

				/** Channel Definitiona*/
			std::vector<ChannelDefinition>	m_Definitions;

				/** Default constructor, initialises members */
			CChannelDefinitionBox();
				/** Cirtual destructor */
			virtual ~CChannelDefinitionBox();

				/** 
				 * Get the ChannelDefinition for the specified channel index.
				 * @param		iChannel	Channel index
				 * @return      const ChannelDefinition*	ChannelDefinition, or NULL on failure.
				 */
			virtual const ChannelDefinition *GetDefinitition(UINT16 iChannel);
				/** 
				 * Update the extended length of the box prior to writing
				 */
			virtual void UpdateXLBox(void);
				/** 
				 * Parse the channel definition box from the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError Parse(class CFileBase &FileBase, CIOStream &Stream);
				/** 
				 * UnParse the channel definition box to the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
		};

			/** Box Type */
		static UINT32	sm_nTBox;

			/** ImageHeaderBox within HeaderBox */
		CImageHeaderBox m_ImageHeader;
			/** Optional BitsPerComponentBox within HeaderBox */
		CBitsPerComponentBox m_BitsPerComponent;
			/** Optional ChannelDefinitionBox within HeaderBox */
		CChannelDefinitionBox m_ChannelDefinition;
			/** Default contructor, initialises members */
		CHeaderBox();
			/** Virtual destructor */
		virtual ~CHeaderBox();
	};
		/**
		 * CIntellectualPropertyBox class - the ECW IPR box.  This box is optional.
		 * 
		 * @author       Simon Cope
		 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
		 */	
	class NCSECW_IMPEX CIntellectualPropertyBox: public CBox {
	public:
			/** Box type */ 
		static UINT32	sm_nTBox;

			/** Default constructor, initialises members */
		CIntellectualPropertyBox();
			/** Virtual destructor */
		virtual ~CIntellectualPropertyBox();
			/** 
			 * Parse the JPC box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream&Stream);
	};
		/**
		 * CXMLBox class - the ECW XML box.  This box is optional, and there may be more than one.
		 * 
		 * @author       Simon Cope
		 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
		 */	
	class NCSECW_IMPEX CXMLBox: public CBox {
	public:
			/** Box type */ 
		static UINT32	sm_nTBox;

			/** Default constructor, initialises members */
		CXMLBox();
			/** Virtual destructor */
		virtual ~CXMLBox();
			/** 
			 * Parse the XML box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream&Stream);
			/** 
			 * UnParse the XML box to the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	};
		/**
		 * CUUIDBox class - the ECW UUID box.  This box is optional, and there may be more than one.
		 * 
		 * @author       Simon Cope
		 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
		 */	
#if 0 // md_debug
	class NCSECW_IMPEX CUUIDBox: public CBox {
	public:
			/** Box type */ 
		static UINT32	sm_nTBox;
			/** UUID */
		UUID	m_UUID;
			/** UUID Box data */
		void *m_pData;
			/** UUID Box data Length*/
		INT32 m_nLength;

			/** Default constructor, initialises members */
		CUUIDBox();
			/** Virtual destructor */
		virtual ~CUUIDBox();

			/** 
			 * Parse the UUID box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream&Stream);
			/** 
			 * UnParse the UUID box to the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	};
		/**
		 * CUUIDInfoBox class - the ECW UUIDInfo box.  This box is optional, and there may be more than one.
		 * 
		 * @author       Simon Cope
		 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
		 */	
	class NCSECW_IMPEX CUUIDInfoBox: public CSuperBox {
	public:
			/**
			 * CUUIDListBox class - the ECW file UUIDList box class.
			 * This box is required within a UUIDInfo box.
			 * 
			 * @author       Simon Cope
			 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
			 */	
		class NCSECW_IMPEX CUUIDListBox: public CBox {
		public:
			
				/** Box type */
			static UINT32	sm_nTBox;
				/** Number of entries */
			UINT16		m_nEntries;
				/** UUIDs */
			std::vector<UUID> m_UUIDs;
			
				/** Default constructor, initialises members */
			CUUIDListBox();
				/** Virtual destructor */
			virtual ~CUUIDListBox();

				/** 
				 * Parse the UUID List box from the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError Parse(class CFileBase &FileBase, CIOStream&Stream);
				/**
				 * UnParse the UUID list box to the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
		};
			/**
			 * CDataEntryURLBox class - the ECW file DateEntryURL box class.
			 * This box is required within a UUIDInfo box.
			 * 
			 * @author       Simon Cope
			 * @version      $Revision: #5 $ $Author: ctapley $ $Date: 2018/07/10 $ 
			 */	
		class NCSECW_IMPEX CDataEntryURLBox: public CBox {
		public:
			
				/** Box type */
			static UINT32	sm_nTBox;

				/** Version */
			UINT8		m_nVersion;
				/** Flags */
			UINT8		m_Flags[3];
				/** Location URL */
			UINT8		*m_pLocation;

				/** Default constructor, initialises members */
			CDataEntryURLBox();
				/** Cirtual destructor */
			virtual ~CDataEntryURLBox();

				/** 
				 * Parse the Data Entry URL box from the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError Parse(class CFileBase &FileBase, CIOStream&Stream);
				/**
				 * UnParse the Data Entry URL box to the ECW file.
				 * @param		ECWFile		ECW file being parsed
				 * @param		Stream		IOStream to use to parse file.
				 * @return      CError	NCS_SUCCESS, or Error code on failure.
				 */
			virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
		};
			/** Box Type */
		static UINT32	sm_nTBox;
				/** UUIDListBox within UUIDInfo */
		CUUIDListBox m_List;
				/** DataEntryURLBox within the UUIDInfo */
		CDataEntryURLBox m_DE;

				/** Default constructor, initialises members */
		CUUIDInfoBox();
			/** Virtual destructor */
		virtual ~CUUIDInfoBox();
	};
#endif		
			
	class NCSECW_IMPEX CAssociationBox: public CSuperBox {
	public:
		static UINT32	sm_nTBox;

		/** Default constructor, initialises members */
		CAssociationBox();
		/** Virtual destructor */
		virtual ~CAssociationBox() {};

		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	};

#if 0 // md_debug
	class NCSECW_IMPEX CReaderRequirementsBox: public CBox {
	public:
		static UINT32	sm_nTBox;
		CReaderRequirementsBox();
			/** Virtual destructor */
		virtual ~CReaderRequirementsBox();
			/** Update the extended length for the box before writing */
		virtual void UpdateXLBox(void);
			/** 
			 * Parse the XML box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream&Stream);
			/** 
			 * UnParse the XML box to the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	};
#endif
	class NCSECW_IMPEX CLabelBox: public CBox {
	public:
		static UINT32	sm_nTBox;
		char *m_szLabel;

			/** Default constructor, initialises members */
		CLabelBox();
			/** Overloaded constructor, specify label */
		explicit CLabelBox(const char *szLabel);
			/** Virtual destructor */
		virtual ~CLabelBox();
			/** Update the extended length for the box before writing */
		virtual void UpdateXLBox(void);
			/**
			 * Set the label to be parsed out into the file.
			 * @param		szLabel		Label text
			 */
		virtual void SetLabel(const char* szLabel);
			/**
			 * Get the label text of this box, if any
			 * @return		const char *		Label text
			 */
		virtual const char *GetLabel() const;
			/** 
			 * Parse the XML box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream&Stream);
			/** 
			 * UnParse the XML box to the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	};

	class NCSECW_IMPEX CClientStatisticsECWBox : public CBox
	{
	public:
		static UINT32 sm_nTBox;
		
		/** Default constructor, initialises members */
		CClientStatisticsECWBox();
			/** Virtual destructor */
		virtual ~CClientStatisticsECWBox();

		void SetData(const NCSFileStatistics& statsData);

		const NCSFileStatistics* GetData();
			/** 
			 * Parse the XML box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream& Stream);
			/** 
			 * UnParse the XML box to the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	private:

		UINT32 CalculateStatsSizeInBytes();

		NCSFileStatistics* m_pStats;
	};

	class NCSECW_IMPEX CClientStatisticsV2ECWBox : public CBox
	{
	public:
		static UINT32 sm_nTBox;
		
		/** Default constructor, initialises members */
		CClientStatisticsV2ECWBox();
			/** Virtual destructor */
		virtual ~CClientStatisticsV2ECWBox();

		void SetData(const NCSFileStatistics& statsData);
		void SetOldClientStatisticsBox(const CClientStatisticsECWBox *pOldStatisticsBox);

		const NCSFileStatistics* GetData();
			/** 
			 * Parse the XML box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream& Stream);
			/** 
			 * UnParse the XML box to the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	private:
		const CClientStatisticsECWBox *m_oldStatisticsBox;
		UINT32 CalculateStatsSizeInBytes();

		NCSFileStatistics* m_pStats;
	};
	
	//auto filled box is used to store some fields that is automatically field by sdk after compression
	class NCSECW_IMPEX CAutoFilledBox : public CBox
	{
	public:
		static UINT32 sm_nTBox;
		
		/** Default constructor, initialises members */
		CAutoFilledBox();
			/** Virtual destructor */
		virtual ~CAutoFilledBox();
			/** 
			 * Parse the XML box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream& Stream);
			/** 
			 * UnParse the XML box to the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	};

	class NCSECW_IMPEX CFileMetaDataBox : public CXMLBox
	{
	public:
		static UINT32 sm_nTBox;
		
		/** Default constructor, initialises members */
		CFileMetaDataBox(const IXmlUtilsPtr &xmlUtils = CXmlUtils::Singleton());
			/** Virtual destructor */
		virtual ~CFileMetaDataBox();
			/** 
			 * Parse the XML box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream& Stream);
			/** 
			 * UnParse the XML box to the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	protected:
		IXmlUtilsPtr m_XmlUtils;
	};

	class NCSECW_IMPEX CRPCDataBox : public CBox
	{
	public:
		static UINT32 sm_nTBox;
		
		/** Default constructor, initialises members */
		CRPCDataBox();
			/** Virtual destructor */
		virtual ~CRPCDataBox();


		void SetData(const NCSRPCData& RPCData);

		const NCSRPCData* GetData();
			/** 
			 * Parse the XML box from the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError Parse(class CFileBase &FileBase, CIOStream& Stream);
			/** 
			 * UnParse the XML box to the ECW file.
			 * @param		ECWFile		ECW file being parsed
			 * @param		Stream		IOStream to use to parse file.
			 * @return      CError	NCS_SUCCESS, or Error code on failure.
			 */
		virtual CError UnParse(class CFileBase &FileBase, CIOStream &Stream);
	private:
		UINT32 CalculateStatsSizeInBytes();
		
		NCSRPCData* m_pRPCData;

	};

	typedef enum
	{
		GeoInfoModified = 1,
		FileMetaDataModified = 2,
		StatsModified = 4,
		RPCDataModified = 8,
		CustomBoxModidied = 16
	}
	ModificationFlag;


	UINT64 m_blockDataOffset; // offset to the block data related to the file

	GeotiffTagBox m_GeotiffTagBox;

		/** SignatureBox within ECWFile */
	CSignatureBox		m_Signature;
		/** FileTypeBox within ECWFile */
	CFileTypeBox		m_FileType;
		/** HeaderBox within ECWFile */
	CHeaderBox		m_Header;
		/** QMFBox  */
	CQMFBox	m_QMF;
		/** IntellectualPropertyBox within ECWFile */
	CIntellectualPropertyBox m_IPR;

	CFileMetaDataBox m_FileMetaData;

	CClientStatisticsECWBox m_ClientStatistics;
	CClientStatisticsV2ECWBox m_ClientStatisticsV2;

	CRPCDataBox m_RPCData;

	CAutoFilledBox m_AutoFilledData;

		/** Component index for each output channel - 1:1 unless a palette is present */
	std::vector<UINT16>	m_ChannelComponentIndex;

	/** Background Colours */
	std::vector<IEEE4> m_BackgroundColours;

		/** 
		 * Get the memory image of the file.
		 * @param		pLength		Returned length of image
		 * @return      void *		NCSMalloc()'d pointer to image.
		 */
	virtual void *GetMemImage(UINT32 &nLength);

		/** 
		 * Parse the superbox
		 */
	virtual CError ParseSuperBox();
		/** 
		 * UnParse the superbox
		 */
	virtual CError UnParseSuperBox();

		/** 
		 * Get length of a packet in the file.
		 * @param		nPacket		Absolute Packet number to determine length for
		 * @return      UINT32		Packet length or 0 on failure.
		 */
	virtual UINT32 GetPacketLength(NCSBlockId nPacket);
		/** 
		 * Read a packet from the file.
		 * @param		nPacket		Absolute Packet number to determine length for
		 * @param		pLength		Returned length of packet (header+data)
		 * @return      void *		NCSMalloc()'d pointer to read packet (header+data).
		 */
	virtual void *GetPacket(NCSBlockId nPacket, UINT32 &nLength);
		/** 
		 * Get number of a packet in the file.
		 * @return      UINT32		Number of Packets or 0 on failure.
		 */
	virtual UINT32 GetNrPackets();
#ifndef NCS_MOBILE
		/** 
		 * Set the compression parameters (dimensions, registration, bitdepth etc).
		 * @param		Info		FileInfo structure
		 * @return      CError	NCS_SUCCESS, or error on failure 
		 */
	virtual CError SetCompress(NCSFileInfo &Info);
		/** 
		 * Write the next line in BIL format into the file.
		 * @param		nCtx				Unique Context ID
		 * @param		pSrc				Source buffer
		 * @param		iComponent			Component index
		 * @return      bool				true on success, else false.
		 */
	virtual bool WriteLine(SDK::CNode2D::ContextID nCtx, SDK::CBuffer2D *pSrc, UINT16 iComponent);
#endif
		/** 
		 * Purge any unused buffers, codeblocks etc if memory useage exceeds acceptable limites
		 */
			/** 
		 * Get a pointer to a UUID box bsed on the specified UUID.  Iterate via the pLast paramter.
		 * @param		uuid		UUID to find UUID box for
		 * @param		pLast		Pointer to last returned UUID box (or NULL on first call), used to iterate through multiple UUID boxes with the same UUID
		 * @return      CUUIDBox*		Pointer to UUID Box.
		 */
#if 0	// md_debug
	CUUIDBox *GetUUIDBox(UUID uuid, CUUIDBox *pLast = NULL);
#endif
		/** 
		 * Get a pointer to an XML box.  Iterate via the pLast paramter.
		 * @param		pLast		Pointer to last returned XML box (or NULL on first call), used to iterate through multiple XML boxes
		 * @return      CXMLBox*		Pointer to XML Box.
		 */
	CXMLBox *GetXMLBox(CXMLBox *pLast = NULL);

		/** 
		 * Set the compression parameters (dimensions, registration, bitdepth etc).
		 * @param		Info		FileInfo structure
		 * @return      CError	NCS_SUCCESS, or error on failure 
		 */
//	virtual void Purge();

	virtual NCSFileType GetFileType() { return(NCS_FILE_ECW); };

	virtual NCSFileInfo *GetFileInfo();

	static CError GetVersionNumber(const CString& filePath, UINT32& versionNumber);
	//writes free box in place of current box. 
	static CError EraseBox(const NCS::SDK::CBox &box, NCS::SDK::CFileBase &FileBase, CIOStream &Stream);
	//call this function if the box has a variable length
	static CError UnParseVariableLengthBox(NCS::SDK::CBox &box, NCS::SDK::CFileBase &FileBase, CIOStream &Stream, UINT32 newDataLength);

	//check metadata 
	static NCSError CheckMetaData(const NCSFileMetaData& fileMetaData);

	//unparse the boxes such as filemetadatabox after encoding
	CError UnparseBoxesAfterEncoding(UINT64 oldOffset, UINT64 endOfCodeStream);

	NCSError SetFileMetaData(const NCSFileMetaData* pFileMetaData);

	//set the client stats
	CError SetClientStatistics(const NCSFileStatistics* pStats);

	//get the client stats
	CError GetClientStatistics(NCSFileStatistics** ppStats);

	//set the RPC data
	CError SetRPCData(const NCSRPCData* pRPCData);

	//get the RPC data
	CError GetRPCData(NCSRPCData** ppRPCData);

	//list all the current geotiff keys stored
	CError GetAllGeotiffKeyEntries(NCSGeoKeyEntry **ppGeoKeyIds, UINT32* count) const;

	/* geotiff keys define a model space in term of geotiff, such as projection, datum */
	CError GetGeotiffKey(geokey_t key, void *val, int index,
                         int count) const;
	CError SetGeotiffKey(geokey_t keyID, tagtype_t type, int count, va_list args);

	/* ModelTiePointTag, ModelPixelScaleTag or ModelTransformationTag defines the mapping between model space and raster space */
	CError SetGeotiffTag(unsigned short tag, int count, IEEE8* pValue);
	CError GetGeotiffTag(unsigned short tag, int *count, IEEE8** ppValue) const;

	CError FlushAllForEditing();

	/**
	 * \fn	NCS::CError GetUnusedFileSizeInfo(UnusedFileSizeInfo &UnusedFileInfo);
	 *
	 * \brief	Gets the unused file size information.
	 * 			
	 * 			First call to this function will generate the unused info which for a large dataset
	 * 			(e.g. ~30GB) will take a few minutes.
	 *
	 * \author	Intergraph Perth
	 * \date	22 Jan YYYY
	 *
	 * \param [in,out]	UnusedFileInfo	Information describing the unused portion of the file.
	 *
	 * \return	An error of the result.
	 */
	NCS::CError GetUnusedFileSizeInfo(UnusedFileSizeInfo &UnusedFileInfo);
	
	typedef void(*ReportCleanFunc)(double percentageCompleted);
	CError CleanTo(const NCS::CString& sOutputFilePath, ReportCleanFunc reportCleanFunc);


//	class CCachedBlockListHash {
//	public:
//		static bool equal( const NCSBlockId& j, const NCSBlockId& k ) { return(j == k); };
//		static size_t hash( const NCSBlockId& k ) { return((size_t)k); };
//	};
//	typedef tbb::concurrent_hash_map<NCSBlockId, CCachedBlock, CCachedBlockListHash> CCachedBlockList;

	QmfLevel	*m_pTopQmf;				// Pointer to the top level of the ECW QMF structure
	NCS::ECW::CFile *m_pOpacityFile;
	bool m_bReadOffsets;			// TRUE if the block offsets table has been read and is valid for the QMF
	bool m_bReadMemImage;			// TRUE if the NCSFile has a memory image of the header present
	bool m_bValid;					// File is currently valid (it has not changed on disk since open).
	// Client side information (not valid when file opened at the server end)
	NCSTimeStampMs	m_tLastSetViewTime;// Last time a SetView was done, used to decide when to purge files from cache
	bool		m_bAddTexture;		// TRUE if we need to add texture.
	CCachedBlockList	*m_pBlocks;
	bool		m_bTooBusyPurging;

	UINT32				m_nPackets;
	std::vector<std::vector<SDK::CBuffer2D> > m_BackgroundBuffers;
	UINT16 m_nStoredOverlap;
	//CCachedBlockListIterator pLastBlock;
	NCSPacket			*m_pSendPacket;		// Pointer to Send Packet - only one packet allocated & kept for life of File open
	NCSClientUID	m_nClientUID;		// Unique UID for this client file opened
	NCSSequenceNr	m_nServerSequence; // current maximum sequence number read from back from the server
	NCSSequenceNr	m_nClientSequence; // current sequence number client has issued

//	CCachedBlock m_Level0ZeroBlock;// a level 0 zero block containing all zeros for all bands and sidebands
//	CCachedBlock m_LevelnZeroBlock;// a > level 0 zero block (has one less sideband than level 0) 
	//UINT8	*m_pLevel0ZeroBlock;		
	//UINT8	*m_pLevelnZeroBlock;		

	// Client network related information (not valid when file opened at the server end)
	pNCSnetClient	m_pClientNetID;		// client network layer file channel NetID
	//bool	m_bSendInProgress;			// if TRUE, a send pack (request and/or cancel) has been made, and callback will be made once complete
	//UINT32	m_nRequestsXmitPending;		// if non-zero, number of block read requests waiting to send
										// (NOT requests already sent that responses are waiting for)
	//UINT32	m_nCancelsXmitPending;		// if non-zero, number of block cancel requests waiting to sent
	UINT32	m_nUnpackedBlockBandLength;	// length of one band in an unpacked block (always unpacked out to max X&Y block size)
	bool m_bIsConnected;				// Are we still connected to the server serving this file /**[07]**/
	bool	m_bIsCorrupt;						// File is corrupt - displayed message for user
	COffsetCache *m_pOffsetCache;
	bool		m_bFileIOError; //[17]

	UnusedFileSizeInfo *m_pUnusedFileSizeInfo;

	NCSTimeStampMs m_tsLastReconnectTry;  //[19] The time of the last attempt to reconnect to the IWS
	UINT32	m_nReconnectCount;
	NCS_INLINE INT32 NumComponents() { return (INT32)m_Components.size(); };
	NCS_INLINE CComponent *Component(UINT16 iComponent) { return(m_Components[iComponent]); };

	static NCSError EditReadInfo(const NCS::CString& pFilename, NCSEditInfo **ppInfo);
	static NCSError EditWriteInfoStream_v2(CIOStream& fileIOStream, NCSEditInfo *pInfo);
	static NCSError EditWriteInfo_v2(const NCS::CString& pFilename, NCSEditInfo *pInfo, void (*pProgressFunc)(UINT64 nTotal, UINT64 nWritten, void *pClientData), BOOLEAN (*pCancelFunc)(void *pClientData), void *pClientData);
	static NCSError EditWriteInfo_v3(const NCS::CString& pFilename, NCSEditInfo *pInfo, void (*pProgressFunc)(UINT64 nTotal, UINT64 nWritten, void *pClientData), BOOLEAN (*pCancelFunc)(void *pClientData), void *pClientData);
	NCSError UnparseEditInfo(const NCSEditInfo& editInfo, CIOStream& filestream);

	/**
	* How much memory in use for all ECW files
	* @return      UINT64		Memory in use in bytes
	*/
	static UINT64 sMemoryInUse();
protected:
	friend class NCS::SDK::CFileBase::CFileFactory;
#if !defined(ECWP2_DISABLED)
    friend class NCS::ECW::CECWPIOStream;
#endif
	friend class NCS::SDK::CECWP3IOStream;
	friend class NCS::SDK::CBlockFile;

	std::vector<CComponent*> m_Components;

		/** Default constructor, initialises members */
	CFile();
		/** Virtual Destructor */
	virtual ~CFile();

		/** 
		 * Open the ECW file for Parsing/UnParsing.
		 * @param		pFileName	Full path name of ECW file to open
		 * @param		bWrite		If true, opens for writing.
		 * @return      CError	NCS_SUCCESS or error code on failuer.
		 */
	//virtual CError Open(NCSTChar *pFileName, bool bWrite = false);
		
		/** 
		 * Open the ECW file from the specified stream.
		 * @param		Stream		Stream of ECW file to open
		 * @return      CError	NCS_SUCCESS or error code on failuer.
		 */
	virtual CError Open(CIOStream *pStream);
		/** 
		 * Close the ECW file.
		 * @param		bFreeCache	Free the cache & delete file object if last close
		 * @return      CError	NCS_SUCCESS or error code on failuer.
		 */
	virtual CError Close();

		/** 
		 * Purge unused memory at specified purgestage
		 * @param		eStage		Purge stage to free memory at
		 */
	virtual void PurgeMemory(SDK::CGlobalInfo::Purge eStage, float factor = 1.0);

	virtual UINT8 *ConstructZeroBlock(QmfLevel *pQmf,	UINT32 &nLength, bool bAligned);

	virtual bool GetFileBlockSize(NCSBlockId nBlock, UINT32 &nBlockLength, UINT64 &nBlockOffset );
	virtual UINT8 *ReadFileBlock(NCSBlockId nBlock, UINT32 &nBlockLength );

	CError CloseOpacityFile();

	virtual void ReceivedBlock(NCSBlockId nBlock, UINT8* pImage, UINT32 nBlockLength);

	virtual bool IsStreamSlowStorage();
private:
	friend class CView;
	friend class CUpdater;

	UINT16 m_ModifiedFlag;

	CError UnparseToMemImage();

	NCSError UnparseEditableBox(NCS::SDK::CBox& box, NCS::CIOStream& stream);

	CError CopyStream(CIOStream& inputStream, UINT32 nCopyLength, CIOStream& outputStream, UINT8* copy_buffer, UINT32 nBufferLength,
		ReportCleanFunc reportCleanFunc, UINT64 nRealFileSizeInUse);

#if !defined(ECWP2_DISABLED)
	virtual CIOStream *CreateBlockStream(const CString &, const CString &, bool bECWP3=true);
#else
    virtual CIOStream *CreateBlockStream(const CString &, const CString &);
#endif
};
}
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

//FIXME
extern "C" NCS::ECW::CFile::CCachedBlock *NCScbmReadViewBlock(QmfRegionLevel	*pQmfRegionLevel, UINT32 nBlockX, UINT32 nBlockY);
extern "C" int unpack_start_line_block( QmfRegionLevel *p_level, UINT32 x_block,
							NCS::ECW::CFile::CCachedBlock *p_packed_block, UINT32	lines_to_skip);

#if (defined(WIN32) || defined(LINUX))
typedef void* (*__tbbmalloc_scalable_aligned_malloc)(size_t, size_t);
typedef void(*__tbbmalloc_scalable_aligned_free)(void*);
#endif

#endif // !NCSECWFILE_H
