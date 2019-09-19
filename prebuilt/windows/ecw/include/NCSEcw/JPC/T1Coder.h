/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:    T1Coder.h 
** CREATED:  12/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CT1Coder class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCT1CODER_H
#define NCSJPCT1CODER_H
#include "NCSDefs.h"
#include "NCSEcw/JPC/Types/Types.h"
#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H
#include "NCSEcw/JPC/Markers/QCDMarker.h"
#include "NCSMemoryIOStream.h"
#ifndef NCSJPCSEGMENT_H
#include "NCSEcw/JPC/Types/Segment.h"
#endif // NCSJPCSEGMENT_H
#include "NCSEcw/JPC/Nodes/SubBand.h"

namespace NCS {
namespace JPC {

	template <class T, int T1_SIGN_SHIFT, T T1_SIGN_MASK, T T1_VALUE_MASK> class T_T1Decoder;

	typedef T_T1Decoder<INT16, 15, INT16_MIN, INT16_MAX>  Dec16;
	typedef T_T1Decoder<INT32, 31, INT32_MIN, INT32_MAX>  Dec32; 
	typedef T_T1Decoder<INT64, 63, INT64_MIN, INT64_MAX> Dec64; 


	/**
	 * CT1Coder class - the JPC T1 Coder.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2017/12/03 $ 
	 */	
class NCSECW_IMPEX CT1Coder {
public:	
	const static int PREDICTABLE_TERMINATION;
	const static int PASS_RESET_CTX;
	const static int SEGMENT_SYMBOLS;
	const static int PASS_TERMINATION;
	const static int VERTICAL_CAUSAL_CTX;
	const static int SELECTIVE_CODING_BYPASS;

	explicit CT1Coder(bool bUseDefinedT_T1Decoder = false);
	virtual ~CT1Coder();

	bool Decode(JPC::CSubBand::Type eSBType,
				UINT8 roiShift, 
				UINT8 nBits,
				UINT8 nZeroBits,
				const std::vector<CSegment> &m_Segments,
				UINT32 nQualityLayers,
				SDK::CBuffer2D *pDecBuf,
				int Flags,
				CQCDMarker &Quantization,
				UINT8 nComponentBits,
				UINT8 nResolution,
				IEEE4 fReconstructionParameter);
	bool Encode(JPC::CSubBand::Type eSBType,
				UINT8 nBitsTotal,
				UINT8 &nZeroBits,
				std::vector<CSegment> &Segments,
				SDK::CBuffer2D *pDecBuf,
				UINT16 nRatio,
				UINT16 nLayers);
	static NCSTimeStampUs sm_usTotal;
	static NCSTimeStampUs sm_usLast;
	static UINT64 sm_nTotalSamples;
	static UINT64 sm_nLastSamples;

	Dec16 *m_Dec16;
	Dec32 *m_Dec32;
	Dec64 *m_Dec64;
	bool m_bUseDefinedT_T1Decoder;

	static void Init(); // for m_Data32INT16Pool, etc
	static void Fini(); // for m_Data32INT16Pool, etc

	class CDataPool
	{
	public:
		SDK::CBuffer2D *m_pData;
		CDataPool(UINT16 nSize, SDK::CBuffer2D::Type eType, UINT32 nHeapNumber) {
			m_nSize = nSize;
			m_pData = NULL;
			if (eType == SDK::CBuffer2D::BT_INT16)
				m_pData = new SDK::CBuffer2D(0, 0, m_nSize, m_nSize, SDK::CBuffer2D::BT_INT16, nHeapNumber); // for 8 bit
			else if (eType == SDK::CBuffer2D::BT_INT32)
				m_pData = new SDK::CBuffer2D(0, 0, m_nSize, m_nSize, SDK::CBuffer2D::BT_INT32, nHeapNumber); // for 16 bit
			// no support for other types
			m_bInUse = false;
		};
		~CDataPool() {
			delete m_pData;
			m_pData = NULL;
		};

		bool GetInUse(void) {
			return m_bInUse;
		};

		// through UnRefStream when related to m_Data32INT16Pool, etc
		void SetInUse(bool bInUse) {
			m_bInUse = bInUse;
		};

		SDK::CBuffer2D* GetData() {
			return m_pData;
		}

	private:
		UINT16 m_nSize;
		bool m_bInUse;
	};

	static CDataPool* GetDataPool(UINT16 nSize, SDK::CBuffer2D::Type eType);
	static void UnRefData(CDataPool *pDataPool);

protected:
	static std::vector< CDataPool* > m_Data32INT16Pool;
	static std::vector< CDataPool* > m_Data64INT16Pool;
	static std::vector< CDataPool* > m_Data128INT16Pool;
	static std::vector< CDataPool* > m_Data256INT16Pool;
	static std::vector< CDataPool* > m_Data32INT32Pool;
	static std::vector< CDataPool* > m_Data64INT32Pool;
	static std::vector< CDataPool* > m_Data128INT32Pool;
	static std::vector< CDataPool* > m_Data256INT32Pool;
	static CMutex m_DataPoolMutex;
};
}
}

#endif // !NCSJPCT1CODER_H
