/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     MQCoder.h 
** CREATED:  12/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CMQCoder class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCMQCODER_H
#define NCSJPCMQCODER_H

#include "NCSEcw/JPC/Types/Types.h"
#include "NCSMutex.h"

#define MQ_TAKE_AC_QEVAL // same speed with/without define this

//
// Inline certain functions for performance improvements
//
//#define NCSJPC_MQ_INLINE __declspec(noinline)
#define NCSJPC_MQ_INLINE NCS_INLINE
#define NCSJPC_MQC_NUMCTXS 19

#define T1_NUMCTXS_AGG 1
#define T1_NUMCTXS_ZC 9
#define T1_NUMCTXS_MAG 3
#define T1_NUMCTXS_SC 5
#define T1_NUMCTXS_UNI 1

#define T1_CTXNO_AGG 0
#define T1_CTXNO_ZC (T1_CTXNO_AGG+T1_NUMCTXS_AGG)
#define T1_CTXNO_MAG (T1_CTXNO_ZC+T1_NUMCTXS_ZC)
#define T1_CTXNO_SC (T1_CTXNO_MAG+T1_NUMCTXS_MAG)
#define T1_CTXNO_UNI (T1_CTXNO_SC+T1_NUMCTXS_SC)
#define T1_NUMCTXS (T1_CTXNO_UNI+T1_NUMCTXS_UNI)

namespace NCS {
namespace JPC {
	/**
	 * CMQCoder class - the JPC MQ Coder.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CMQCoder {
public:
	CMQCoder();
	virtual ~CMQCoder();

	void ResetStates();

	typedef struct _State {
		struct QeVAL {
#ifdef NCSBO_LSBFIRST
			UINT8 __pad[2];
			UINT16 nQEval;
#else
			UINT16 nQEval;
			UINT8 __pad[2];
#endif
		};		
		union {
			UINT32 nQEvalShift16;
			QeVAL Qe; 
		} U;
		UINT8	NPS[4]; //0 = MPS, 1 = LPS, 3 == nD

		UINT32 nQEvalShift2;	// Qe << 2
		UINT32 nQEvalShift18;	// Qe << 18
		UINT32 nQEvalShift161;	// Qe+Qe<<16
	} State;

	State m_Contexts[NCSJPC_MQC_NUMCTXS];
protected:
	static State sm_States[47*2];
	static NCS::CMutex sm_mStates;

	void SetState(UINT8 nCtx, int msb, int prob);
};

	/**
	 * CMQCoder class - the JPC MQ Coder.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CMQDecoder: public CMQCoder {
public:
	CMQDecoder();
	virtual ~CMQDecoder();
 
	void Init(UINT8 *pData, UINT32 nLen);
	bool Done(void) { return(m_pB[1] == 0); };
	//
	// C format: hhhh
	//
	// h = CHi - 16bit C High register
	//
	// CLo and CCt eliminated
	//

	//things to try:
	// decrefpass opt
	//  + data bit shift opt
	// sig pass flags opt
	static NCSJPC_MQ_INLINE INT32 Decode(register UINT32 &AC, register UINT8* &pB, State &S) {
				UINT8 nNPS;
				UINT8 nD = S.NPS[2];

				UINT32 nQEvalShift16 = S.U.nQEvalShift16;
				
				if (((UINT16)AC) >= S.U.Qe.nQEval) {			//(C >= Qe)?
					//AC = AC - (S.U.Qe.nQEval + nQEvalShift16);	//A-=Qe,C-=Qe
					AC = AC - (S.nQEvalShift161);	//A-=Qe,C-=Qe

					if((INT32)AC < 0) {							//(A & 0x8000)? AC>=0x80000000
						return((INT8)nD);
					}
					// MPS Exchange
					nNPS = (UINT8)(AC < nQEvalShift16);			//NPS = (A < Qe) ? 1 : 0
				} else {										//(C < Qe)
					//LPSExchange
					nNPS = (UINT8)(AC >= 2*nQEvalShift16);		//NPS = (A >= 2*Qe) ? 1 : 0
					AC = ((UINT16)AC) + nQEvalShift16;			//A = Qe
				}

				do {							// Renorm
					AC = (AC << 1) + pB[0];		// A <<= 1, C = (C << 1) + B
					pB += pB[1];				// B = Bnext
				} while(AC<0x80000000);			// Till A >= 0x8000

				nD = S.NPS[2 + nNPS];
				S = sm_States[S.NPS[nNPS]];	// Switch State
				return((INT8)nD);
			};

	//things to try:
	// decrefpass opt
	//  + data bit shift opt
	// sig pass flags opt
	// Non-CDP (Common Decoding Path) version
	// nQEvalShift16 = nQEval * 2^16
	static NCSJPC_MQ_INLINE INT32 Decode(register UINT32 &AC_Non_CDP, register UINT32 &D, register UINT8* &pB, State &S) {
		UINT8 nNPS;

		UINT32 nQEvalShift16 = S.U.nQEvalShift16;
		UINT32 AC = AC_Non_CDP + D; // the normal A and C
		bool bCLessThanQe = (UINT16)(D) < S.U.Qe.nQEval; // D before change. OK

#ifdef MQ_TAKE_AC_QEVAL
		//UINT32 nQEvalShift_AC = (nQEvalShift16 + S.U.Qe.nQEval); // nQEvalShift16 does not include sign. take away AC's nQEval
		//D -= nQEvalShift_AC; 
		D -= S.nQEvalShift161; 
#else
		D -= nQEvalShift16; // nQEvalShift16 does not include sign. take away A's nQEval
#endif
		//bool bCLessThanQe = (UINT16)(AC) < S.U.Qe.nQEval; // OK, as normal AC
		//bool bCLessThanQe = (UINT16)(AC_Non_CDP + D) < S.U.Qe.nQEval; // OK without putting back S.U.nQEvalShift16 (only when D -= nQEvalShift16), this is because nQEvalShift16 had left-shifted 16bit from nQEval
		////bool bCLessThanQe = (UINT16)(AC_Non_CDP + D) < S.U.Qe.nQEval; // Wrong without putting back S.U.nQEvalShift16+S.U.Qe.nQEval
		//bool bCLessThanQe = false; //// Wrong, when D -= nQEvalShift16 or when D -= nQEvalShift16 + S.U.Qe.nQEval at above

		if ((INT32)D >= 0) { // AC>=0x80000000
			if (!bCLessThanQe) {
#ifndef MQ_TAKE_AC_QEVAL
				D -= S.U.Qe.nQEval;	// C-=Qe. 
#endif
				return (INT8)S.NPS[2]; // symbol
			}
			else  {
				// LPS Exchange
				nNPS = (UINT8)(AC >= 2 * nQEvalShift16);		//NPS = (A >= 2*Qe) ? 1 : 0
				AC = ((UINT16)AC) + nQEvalShift16;			//A = Qe
			}
		} else {
			if (!bCLessThanQe) {							//(C >= Qe)?
#ifdef MQ_TAKE_AC_QEVAL
				//AC -= nQEvalShift_AC;
				AC -= S.nQEvalShift161;
#else
				AC = AC - (S.U.Qe.nQEval + nQEvalShift16);	//A-=Qe,C-=Qe
#endif
				// MPS Exchange
				nNPS = (UINT8)(AC < nQEvalShift16);			//NPS = (A < Qe) ? 1 : 0
			}
			else {
				// LPS Exchange
				nNPS = (UINT8)(AC >= 2 * nQEvalShift16);		//NPS = (A >= 2*Qe) ? 1 : 0
				AC = ((UINT16)AC) + nQEvalShift16;			//A = Qe
			}
		}

		do {							// Renorm
			AC = (AC << 1) + pB[0];		// A <<= 1, C = (C << 1) + B
			pB += pB[1];				// B = Bnext
		} while (AC<0x80000000);			// Till A >= 0x8000

		// update 
		{
			D = AC - 0x80000000; // 214,748,364,8 = MAXUINT32/2, 2^31
			AC_Non_CDP = AC - D;
		}

		UINT8 nD = S.NPS[2 + nNPS];
		S = sm_States[S.NPS[nNPS]];	// Switch State
		return((INT8)nD);
	};

	static NCSJPC_MQ_INLINE INT32 DecodeUNI(register UINT32 &AC, register UINT8* &pB) {
				UINT8 nD;

				if (((UINT16)AC) >= 0x5601) {			//(C >= Qe)?
					AC = AC - (0x56015601);	//A-=Qe,C-=Qe

					if((INT32)AC < 0) {							//(A & 0x8000)?
						return(0);
					}
					// MPS Exchange
					nD = (UINT8)(AC < 0x56010000);			//NPS = (A < Qe) ? 1 : 0
				} else {										//(C < Qe)
					//LPSExchange
					nD = (UINT8)(AC >= 0xAC020000);		//NPS = (A >= 2*Qe) ? 1 : 0
					AC = ((UINT16)AC) + 0x56010000;			//A = Qe
				}

				do {							// Renorm
					AC = (AC << 1) + pB[0];		// A <<= 1, C = (C << 1) + B
					pB += pB[1];				// B = Bnext
				} while(AC<0x80000000);			// Till A >= 0x8000

				return(nD);
			};
//	NCSJPC_MQ_INLINE INT32 Decode(register UINT32 &AC, register UINT8* &pB, UINT8 nCtx) {
//				return(Decode(AC, pB, m_Contexts[nCtx]));
//			};
//	NCSJPC_MQ_INLINE INT32 Decode(UINT8 nCtx) {
//				return(Decode(m_AC, m_pB, nCtx));
//			};

	UINT32 m_AC;
	UINT8 *m_pB;
protected:
	UINT8 *m_pBase;

	void BitIn();
	void SetupPB(UINT8 *pInput);

};


	/**
	 * CMQCoder class - the JPC MQ Coder.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CMQEncoder: public CMQCoder {
public:
	CMQEncoder();
	virtual ~CMQEncoder();

	void Init(UINT8 *pData, UINT32 nLen);
	void Flush();

		//
		// C format: 000llhhh
		//
		// h = CHi - 12bit C High register
		// l = CLo - 8bit C Low register
		// 0 = unused
		//
		// CCt is stored in a separate register
		//

		NCSJPC_MQ_INLINE void Encode(register UINT16 &A, register INT32 &C, INT32 d, UINT8 nCtx) {
				UINT16 nQEval = m_Contexts[nCtx].U.Qe.nQEval;
				UINT8 nD = m_Contexts[nCtx].NPS[0] & 0x1;//nQEval >> 15;
				//nQEval &= 0x7fff;

				if (nD == d) {	
					A = A - nQEval;
					if (A < 0x8000) {
						if (A >= nQEval) {
							C = C + nQEval;
							RenormMPS(A, C);
						} else {
							A = nQEval;
							RenormLPS(A, C);
						}
						m_Contexts[nCtx] = sm_States[(UINT8)m_Contexts[nCtx].NPS[0]];
					} else {
						C = C + nQEval;
					}
				} else {
					if (A < 2*nQEval) {
						A = A - nQEval;
						C = C + nQEval;
					} else {
						A = nQEval;
					}
					RenormLPS(A, C);
					m_Contexts[nCtx] = sm_States[m_Contexts[nCtx].NPS[1]];
				}
			};
		NCSJPC_MQ_INLINE void Encode(INT32 d, UINT8 nCtx) {
				Encode(m_A, m_C, d, nCtx);
			};
	void SetBits();

	INT32 m_C;
	UINT16 m_A;
	UINT16 m_nCT;
	INT32 m_nIndex;
	UINT8 *m_pB;

	void NCSJPC_MQ_INLINE RenormLPS(register UINT16 &A, register INT32 &C) {
			register UINT16 nCT = m_nCT;
			do {
				A <<= 1;
				C <<= 1;
				nCT--;
				if(nCT == 0) {
					ByteOut(C, nCT);
				}
			} while(A<0x8000);
			m_nCT = nCT;
		}
	void NCSJPC_MQ_INLINE RenormMPS(register UINT16 &A, register INT32 &C) {
			register UINT16 nCT = m_nCT;
			A <<= 1;
			C <<= 1;
			nCT--;
			if(nCT == 0) {
				ByteOut(C, nCT);
			}
			m_nCT = nCT;
		}
	/// <summary>
	/// Output a byte, doing bit-stuffing if necessary.
	/// After a 0xff byte, the next byte must be smaller than 0x90
	/// </summary>
	void NCSJPC_MQ_INLINE ByteOut(register INT32 &C, register UINT16 &nCT) {
		if (m_pB[m_nIndex]==0xff) {
			m_nIndex++;
			m_pB[m_nIndex] = (UINT8)(C >> 20);
			C &= 0xfffff;
			nCT = 7;
		} else {
			if ((C & 0x8000000)==0) {
				m_nIndex++;
				m_pB[m_nIndex] = (UINT8)(C >> 19);
				C &= 0x7ffff;
				nCT = 8;
			} else {
				m_pB[m_nIndex]++;
				if (m_pB[m_nIndex]==0xff) {
					C &= 0x7ffffff;
					m_nIndex++;
					m_pB[m_nIndex] = (UINT8)(C >> 20);
					C &= 0xfffff;
					nCT = 7;
				} else {
					m_nIndex++;
					m_pB[m_nIndex] = (UINT8)(C >> 19);
					C &= 0x7ffff;
					nCT = 8;
				}
			}
		}
	}
};
}
}

#endif // !NCSJPCMQCODER_H

