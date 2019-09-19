/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     SubBand.h
** CREATED:  13/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CSubBand class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
*******************************************************/

#ifndef NCSJPCSUBBAND_H
#define NCSJPCSUBBAND_H

#include "NCSEcw/JPC/TagTree.h"
#include "NCSEcw/JPC/Types/Types.h"
#include "NCSEcw/SDK/NodeTiler2D.h"
#include "NCSThreadPool.h"
//#include "NCSEcw/JPC/Nodes/CodeBlock.h"

// test temp
// defining NCSJPC_CB_MT does not work yet
//#define NCSJPC_CB_MT

namespace NCS {
	namespace JPC {

		//class CT1Coder;

		/**
		 * CSubBand class - the JPC codestream SubBand.
		 *
		 * @author       Simon Cope
		 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $
		 */
		class NCSECW_IMPEX CSubBand : public SDK::CNodeTiler2D {
		public:
			/** SubBand enumerated type */
			typedef enum {
				/** LowLow subband */
				LL = 0,
				/** HighLow - horizontally high-pass subband */
				HL = 1,
				/** LowHigh - vertically high-pass subband */
				LH = 2,
				/** HighHigh */
				HH = 3
			} Type;
			/** Precincts for this SubBand */
			class CPrecinct	*m_pPrecinct;
			void ResetDecodingStatus();
			/** Type of subband */
			Type	m_eType;

			/** Layer # each codeblock is first included in */
			CTagTree	m_LayerInclusion;
			/** # of zero bit planes for each codeblock */
			CTagTree	m_ZeroPlanes;

			/** CodeBlock data */
			class CCodeBlock *m_pCodeBlocks;
			//	std::vector<class CCodeBlock> *m_pCodeBlocks;

			// added on 21/03/2013
			static NCS::CThreadPool *sm_pCBWorker;
			static INT32	sm_nMaxThreads;
			static UINT32	sm_nCodeBlocks_TH;

			//CT1Coder *m_pT1Coder;

			/**
			 * Constructor
			 * @param pPrecinct		Parent precinct
			 * @param eType			SubBand type LL,LH,HL,HH
			 */
			CSubBand(class CPrecinct *pPrecinct, CSubBand::Type eType);
			/** Virtual destructor */
			virtual ~CSubBand();

			/**
			 * Get X0 of this node.
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetX0(class CResolution *pResolution, INT32 nPrecinct, CSubBand::Type eType);
			/**
			 * Get X0 of this node.
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetX0(class CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY, CSubBand::Type eType);

			/**
			 * Get Y0 of this node.
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetY0(class CResolution *pResolution, INT32 nPrecinct, CSubBand::Type eType);
			/**
			 * Get Y0 of this node.
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetY0(class CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY, CSubBand::Type eType);

			/**
			 * Get X1 of this node.
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetX1(class CResolution *pResolution, INT32 nPrecinct, CSubBand::Type eType);
			/**
			 * Get X1 of this node.
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetX1(class CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY, CSubBand::Type eType);

			/**
			 * Get Y1 of this node.
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetY1(class CResolution *pResolution, INT32 nPrecinct, CSubBand::Type eType);
			/**
			 * Get Y1 of this node.
			 * @return      INT32		Coordinate value.
			 */
			static INT32 sGetY1(class CResolution *pResolution, INT32 nPrecinctX, INT32 nPrecinctY, CSubBand::Type eType);

			/**
			 * Get codeblock Width.
			 * @return      INT32		Un-clipped Width of codeblocks.
			 */
			UINT32 GetCBWidth();
			/**
			 * Get codeblock Height.
			 * @return      INT32		Un-clipped height of codeblocks.
			 */
			UINT32 GetCBHeight();

			/**
			 * Get number of codeblocks wide.
			 * @return      INT32		Nr of codeblocks across.
			 */
			UINT32 NCS_INLINE GetNumCBWide() {
				return(m_NumCBWide);
			};
			/**
			 * Get number of codeblocks high.
			 * @return      INT32		Nr of codeblocks high.
			 */
			UINT32 NCS_INLINE GetNumCBHigh() {
				return(m_NumCBHigh);
			};

			/**
			* Get number of decoded codeblocks.
			* @return      INT32		Nr of decoded codeblocks across.
			*/
			UINT32 GetNumCBDecoded();

			/**
			* Increase number of decoded codeblocks.
			* @return      INT32		Nr of codeblocks across.
			*/
			void IncreaseNumCBDecoded();
			#if defined(FORCE_DECODE_WHOLE_REGION_IN_LARGE_PRECINCT)
			/**
			* Check if the required (by rect) code blocks have been done.
			* @return      bool		true if done, false otherwise.
			*/
			bool CheckCBDecoded(CRect2D& rectDst, bool bWholeSubband = false);
			#endif
			/**
			 * Get the orientation value from the given subband type
			 * @param		etype		NCS SubBand type.
			 * @return      UINT8		SubBand orientation value.
			 */
			static NCS_INLINE UINT8 GetOrient(CSubBand::Type eType) {
				switch (eType) {
				default:
				case CSubBand::LL: return(0);
				case CSubBand::LH: return(1);
				case CSubBand::HL: return(2);
				case CSubBand::HH: return(3);
				}
			};

			void DecodeCodeBlocks(UINT32 nRequestedQualityLevel, NCS::SDK::CBuffer2D::Type eType);
			void DecodeCodeBlocks(UINT32 nRequestedQualityLevel, NCS::SDK::CBuffer2D::Type eType, CRect2D& rectDst);

			UINT64 GetMemorySize();


#ifdef NCSJPC_CB_MT
			// added on 27/04/2014
			class CDecodeJob : public NCS::CThreadPool::CJob {
			public:
				CDecodeJob(NCS::JPC::CSubBand *pSubBand, NCS::SDK::CBuffer2D::Type eType, UINT32 nNode, NCS::SDK::CAtomicInt &JobDoneCount) : m_JobDoneCount(JobDoneCount) {
					m_pSubBand = pSubBand;
					m_eType = eType;
					m_nNode = nNode;
					m_pEvent = new NCS::CEvent();
					m_bDone = false;
					//m_bRet = false;
					NCS::CThreadPool::CJob::m_bDeleteThisJobOnceDone = true;
				};
				virtual ~CDecodeJob() {
					m_JobDoneCount++;
					delete m_pEvent;
					m_pEvent = NULL;
				};
				virtual void Work(void);
				virtual bool Done() { return(m_bDone); };
				//virtual bool Result() { return(m_bRet); };
				virtual bool Wait() {
					bool bRet = true;
					LockStatus();
					while (!Done() || IsRunning()) {
						UnLockStatus();
						if (m_pEvent)
							bRet = m_pEvent->Wait();
						else
							break;
						LockStatus();
					}
					UnLockStatus();
					return(bRet);
				};
				virtual void SetRunning(bool bRunning) {
					NCS::CThreadPool::CJob::SetRunning(bRunning);
					if (!bRunning) {
						if (m_pEvent)
							m_pEvent->Set();
					}
				};
			protected:
				NCS::JPC::CSubBand			*m_pSubBand;
				NCS::SDK::CBuffer2D::Type	m_eType;
				UINT32						m_nNode;
				bool						m_bDone;
				NCS::CEvent					*m_pEvent;
				NCS::SDK::CAtomicInt		&m_JobDoneCount;
			};

			class CDecodeJobs : public std::vector<CDecodeJob*> {
			public:
				CDecodeJobs();
				//CDecodeJobs() {};
				virtual ~CDecodeJobs();
				//virtual ~CDecodeJobs() { Wait(); };

				void AddJob(CDecodeJob *p);

				bool Wait(void);
				/*
				bool Wait(void) {
				bool bRet = true;
				for(int i = 0; i < (int)size(); i++) {
				(*this)[i]->Wait();
				bRet &= (*this)[i]->Result();
				delete (*this)[i];
				}
				clear();
				return bRet;
				}; */

				NCS::SDK::CAtomicInt &GetJobDoneCount(); // to add lock, changed on 27/04/2014

			private:
				NCS::SDK::CSpinLockImpl *m_pMutex;
				NCS::SDK::CAtomicInt m_JobDoneCount;
			};
			//CDecodeJobs m_CBJobs;

#endif

			// added on 27/04/2014
			static void MTInit();
			static void MTFini();
#ifdef NCSJPC_CB_MT 
			virtual void QueueJob(NCS::CThreadPool::CJob *p) {
				CSubBand::sm_pCBWorker->QueueJob(p);
			};
			virtual INT32 GetQueueSize() {
				return CSubBand::sm_pCBWorker->GetQueueSize();
			};
			virtual INT32 GetMaxThreadSize() {
				return CSubBand::sm_nMaxThreads;
			};
#endif

		protected:

			/**
			 * Get normal Node Width.
			 * @return      INT32		Un-clipped Width of codeblocks.
			 */
			virtual UINT32 GetNodeWidth(UINT16 iComponent = 0) {
				return(GetCBWidth());
			};
			/**
			 * Get normal Node Height.
			 * @return      INT32		Un-clipped height of codeblocks.
			 */
			virtual UINT32 GetNodeHeight(UINT16 iComponent = 0) {
				return(GetCBHeight());
			};
			/**
			 * Get number of nodes wide.
			 * @return      INT32		Nr of codeblocks across.
			 */
			virtual UINT32 GetNumNodesWide(UINT16 iComponent = 0) {
				return(GetNumCBWide());
			};
			/**
			 * Get number of nodes high.
			 * @return      INT32		Nr of codeblocks high.
			 */
			virtual UINT32 GetNumNodesHigh(UINT16 iComponent = 0) {
				return(GetNumCBHigh());
			};
			/**
			 * Get pointer to specific node.
			 * @param		UINT32		Node nr
			 * @return      CNode2D * Ptr to node.
			 */
			virtual CNode2D *GetNodePtr(UINT32 nNode, UINT16 iComponent = 0);

			void Release();

		private:
			void _DecodeCodeBlocks(UINT32 nRequestedQualityLevel, NCS::SDK::CBuffer2D::Type eType, UINT32 nNodeX0, UINT32 nNodeY0, UINT32 nNodeX1, UINT32 nNodeY1);

		private:
			UINT32 m_NumCBWide;
			UINT32 m_NumCBHigh;
			UINT32 m_NumCBDecoded;
			NCS::SDK::CSpinRWLockImpl *m_pRWMutex;


		};

	}
}

#endif // !NCSJPCSUBBAND_H
