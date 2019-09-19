/********************************************************
** Copyright 2003 Earth Resource Mapping Ltd.
** This document contains proprietary source code of
** Earth Resource Mapping Ltd, and can only be used under
** one of the three licenses as described in the 
** license.txt file supplied with this distribution. 
** See separate license.txt file for license details 
** and conditions.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     $Archive: /NCS/Source/C/NCSEcw/NCSJP2/NCSJPCT1Coder.cpp $
** CREATED:  21/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  NCSJPCT1Coder class implementation
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCT_T1CODER_H
#define NCSJPCT_T1CODER_H

//#pragma optimize( "", off )

//#include "NCSJPCT1Coder.h"
#include "NCSEcw/JPC/Nodes/SubBand.h"
#include "NCSEcw/JPC/Nodes/CodeBlock.h" // for NCSJPC_ENCODE_BLOCK_SIZE
#ifndef NCSJPCMQCODER_H
#include "NCSEcw/JPC/MQCoder.h"
#endif // NCSJPCMQCODER_H
#include <limits>

#ifndef NCSJPCT1CODER_H
#include "NCSEcw/JPC/T1Coder.h"
#endif //NCSJPCT1CODER_H

#include "VPU/VPU.h"

//#define NCSJPC_ENCODE_BLOCK_SIZE 64 

using namespace NCS;
//using namespace SDK; // caused many ambiguous symbol issues
//using namespace JPC; // caused some ambiguous symbol issues

#ifdef _WIN32
#define T1_INLINE __forceinline
#else
#define T1_INLINE NCS_INLINE
#endif

#define T1_C15 (1 << 15)

// Negate x if v is 0, equivalent to ((v)?(x):-(x))
#define T1_NEG_IF_0(v, x) ((2*(v)-1) * (x))
#define T1_NEG_IF_NEG(v, x)		((x)*(1|(v >> (sizeof(v)*8-1))))

// Negate x if v is 1, equivalent to ((v)?-(x):(x))
#define T1_NEG_IF_1(T, v, x) (((x)^((v)*((T)-1)))+(v))

// This macro was found to be incorrect in soime cases. 
// In previous versions the operation was:
// halve val by shifting, then add 1 if -ve e.g -18 -> -9 -> -8 
// Using this macro first meant that the operation was assuming a -ve sign flag
// (note - val is stored as absolute and has to have sign flag applied to it first).
//  negate then add 1 then shift e.g 
// 18 -> -18 -> -17 -> -9 which is different (and wrong) from -8 above.
// so the macro is replaced by T1_NEG_SHIFT_PLUS_1_IF_1 which 
// negates,shifts, and adds 1 if -ve (v is 1 for negate, 0 for dont negate, x is value)
// which is ((1) + (T1_NEG_IF_1(T, v, x)>>1)) which is ((1) + (-x)>>1)
// or 
// shifts if +ve (eg. ((0) + (T1_NEG_IF_1(T, v, x)>>1) which is x>>1

// but using the shift macro introduces errors in 16bit data, so have reverted it 
// pending more investigation...

#define T1_NEG_PLUS_1_IF_1(T, v, x) ((v)+T1_NEG_IF_1(T, v, x))
//#define T1_NEG_SHIFT_PLUS_1_IF_1(T, v, x) ((v)+(T1_NEG_IF_1(T, v, x)>>1))
//#define T1_NEG_SHIFT_PLUS_1_IF_1(T, (flags >> (T1_SGN_1_SHIFT + (0*3))) & 0x1, v);

#if defined(_WIN32)&&defined(_M_IX86)

int NCS_INLINE T1_ABS(int a) {
     __asm {
         mov eax, a;
         cdq;
         xor eax, edx;
         sub eax, edx;
     }
}

#else

#define T1_ABS(x)		(((x) < 0) ? -(x) : (x))

#endif

// NCSJPC_64BIT_FLAG_OPT currently broken
#undef NCSJPC_64BIT_FLAG_OPT 

#ifdef NCSJPC_64BIT_FLAG_OPT
typedef UINT64 T1_FLAG_TYPE;
#define NCSJPC_T1_FLAG_BUFTYPE	SDK::CBuffer2D::BT_INT64
#else
typedef UINT32 T1_FLAG_TYPE;
#define NCSJPC_T1_FLAG_BUFTYPE	SDK::CBuffer2D::BT_INT32
#endif

/*
const int NCS::JPC::CT1Coder::SELECTIVE_CODING_BYPASS	= (0x1 << 0);
const int CT1Coder::PASS_RESET_CTX			= (0x1 << 1);
const int CT1Coder::PASS_TERMINATION			= (0x1 << 2);
const int CT1Coder::VERTICAL_CAUSAL_CTX		= (0x1 << 3);
const int CT1Coder::PREDICTABLE_TERMINATION	= (0x1 << 4);
const int CT1Coder::SEGMENT_SYMBOLS			= (0x1 << 5);
*/

// []: Current stripe value locations
// Sxx: Significant bit (1 if value significant)
// N_x: Sign bit (1 if value neg)
// R_x:	Refine bit (1 if refining)
// V_x: Visit bit (1 if became significant on previous RefSig pass - cleared in cleanup pass)
//
// 32bit flags
//
// 00 01 02:	S00  S10  S20
// 03 04 05:	S01 [S11] S21
// 06 07 08:	S02 [S12] S22
// 09 10 11:	S03 [S13] S23
// 12 13 14:	S04 [S14] S24
// 15 16 17:	S05  S15  S25
// 18 19 20:	N_0 [N_1][R_1]
// 21 22 23:   [V_1][N_2][R_2]
// 24 25 26:   [V_2][N_3][R_3]
// 27 28 29:   [V_3][N_4][R_4]
// 30 31   :   [V_4] N_5
//
//
// 64bit flags
//
// H_x: HVD bit flag
//
// 00 01 02:	S00  S10  S20
// 03 04 05:	S01 [S11] S21
// 06 07 08:	S02 [S12] S22
// 09 10 11:	S03 [S13] S23
// 12 13 14:	S04 [S14] S24
// 15 16 17:	S05  S15  S25
// 18 19 20:        [H_1][R_1]
// 21 22 23:   [V_1][H_2][R_2]
// 24 25 26:   [V_2][H_3][R_3]
// 27 28 29:   [V_3][H_4][R_4]
// 30 31   :   [V_4]      ===
//    32 33:	===  N10  
// 34 35 36:	N01 [N11] N21
// 37 38 39:	N02 [N12] N22
// 40 41 42:	N02 [N13] N23
// 43 44 45:	N04 [N14] N24
// 46 47 48:         N15 
// 49 50 51:             
// 52 53 54:             
// 55 56 57:             
// 58 59 50:   
// 61 62 63:

// Significance propogation flags
#define T1_SIG_00 (1 << 0)
#define T1_SIG_10 (1 << 1)
#define T1_SIG_20 (1 << 2)
#define T1_SIG_01 (1 << 3)
#define T1_SIG_11 (1 << 4)
#define T1_SIG_21 (1 << 5)
#define T1_SIG_02 (1 << 6)
#define T1_SIG_12 (1 << 7)
#define T1_SIG_22 (1 << 8)
#define T1_SIG_03 (1 << 9)
#define T1_SIG_13 (1 << 10)
#define T1_SIG_23 (1 << 11)
#define T1_SIG_04 (1 << 12)
#define T1_SIG_14 (1 << 13)
#define T1_SIG_24 (1 << 14)
#define T1_SIG_05 (1 << 15)
#define T1_SIG_15 (1 << 16)
#define T1_SIG_25 (1 << 17)
#define T1_SIG_1 T1_SIG_11
#define T1_SIG_2 T1_SIG_12
#define T1_SIG_3 T1_SIG_13
#define T1_SIG_4 T1_SIG_14
// Refinement pass flags
#define T1_REFINE_1 (1 << 20)
#define T1_REFINE_2 (1 << 23)
#define T1_REFINE_3 (1 << 26)
#define T1_REFINE_4 (1 << 29)
// Significance pass visit flags
#define T1_VISIT_1 (1 << 21)
#define T1_VISIT_2 (1 << 24)
#define T1_VISIT_3 (1 << 27)
#define T1_VISIT_4 (1 << 30)
// Sign flags
#ifdef NCSJPC_64BIT_FLAG_OPT
#define T1_SGN_10 (((UINT64)1) << 32)
#define T1_SGN_01 (((UINT64)1) << 34)
#define T1_SGN_11 (((UINT64)1) << 35)
#define T1_SGN_21 (((UINT64)1) << 36)
#define T1_SGN_02 (((UINT64)1) << 37)
#define T1_SGN_12 (((UINT64)1) << 38)
#define T1_SGN_22 (((UINT64)1) << 39)
#define T1_SGN_03 (((UINT64)1) << 40)
#define T1_SGN_13 (((UINT64)1) << 41)
#define T1_SGN_23 (((UINT64)1) << 42)
#define T1_SGN_04 (((UINT64)1) << 43)
#define T1_SGN_14 (((UINT64)1) << 44)
#define T1_SGN_24 (((UINT64)1) << 45)
#define T1_SGN_15 (((UINT64)1) << 47)
#define T1_SGN_1 T1_SGN_11
#define T1_SGN_2 T1_SGN_12
#define T1_SGN_3 T1_SGN_13
#define T1_SGN_4 T1_SGN_14
#define T1_SGN_1_SHIFT 35
#else
#define T1_SGN_01 0
#define T1_SGN_11 0
#define T1_SGN_21 0
#define T1_SGN_02 0
#define T1_SGN_12 0
#define T1_SGN_22 0
#define T1_SGN_03 0
#define T1_SGN_13 0
#define T1_SGN_23 0
#define T1_SGN_04 0
#define T1_SGN_14 0
#define T1_SGN_24 0

#define T1_SGN_0 (1 << 18)
#define T1_SGN_1 (1 << 19)
#define T1_SGN_2 (1 << 22)
#define T1_SGN_3 (1 << 25)
#define T1_SGN_4 (1 << 28)
#define T1_SGN_5 (1 << 31)
#define T1_SGN_10 T1_SGN_0
#define T1_SGN_15 T1_SGN_5
#define T1_SGN_1_SHIFT 19
#endif
#define T1_SIG T1_SIG_11
#define T1_SGN T1_SGN_1
#define T1_REFINE T1_REFINE_1
#define T1_VISIT T1_VISIT_1

//
// Flags for locations around V1 (value at row 0 of stripe)
//
// D0 V0 D1
// H0 XX H1
// D2 V1 D3
#define T1_SIG_H0 T1_SIG_01
#define T1_SIG_H1 T1_SIG_21
#define T1_SIG_V0 T1_SIG_10
#define T1_SIG_V1 T1_SIG_12
#define T1_SIG_D0 T1_SIG_00
#define T1_SIG_D1 T1_SIG_20
#define T1_SIG_D2 T1_SIG_02
#define T1_SIG_D3 T1_SIG_22

// All horizontal+vertical bits
#define T1_SIG_HV (T1_SIG_V0|T1_SIG_H1|T1_SIG_V1|T1_SIG_H0)
// All 8 neighbour hor+ver+diag
#define T1_SIG_HVD (T1_SIG_HV|T1_SIG_D1|T1_SIG_D3|T1_SIG_D2|T1_SIG_D0)

#ifdef NCSJPC_64BIT_FLAG_OPT
#define T1_SIG_HVD64 (((UINT64)1) << 19) 
#else
#define T1_SIG_HVD64 0
#endif
#define T1_SIG_HVD64_1 T1_SIG_HVD64
#define T1_SIG_HVD64_2 (T1_SIG_HVD64 << 3)
#define T1_SIG_HVD64_3 (T1_SIG_HVD64 << 6)
#define T1_SIG_HVD64_4 (T1_SIG_HVD64 << 9)


//
// Sign bits
// Used for indexing the SC & SPB LUTs
//
//    V0 
// H0 XX H1
//    V1 
//
#define T1_SGN_V0 T1_SIG_00
#define T1_SGN_H0 T1_SIG_20
#define T1_SGN_V1 T1_SIG_02
#define T1_SGN_H1 T1_SIG_11 

#ifdef NCSJPC_64BIT_FLAG_OPT

#define T1_FLAGS_GET_EXT(f) (f)

#ifdef NCS_64BIT
#define T1_FLAGS_GET_NORMAL(f) T1_FLAGS_GET_EXT(f)
#else
#define T1_FLAGS_GET_NORMAL(f) ((UINT32)f)
#endif

#define T1_FLAGS_GETZC_CTX_NO(flags, j) ((flags >> 48 + (j * 4)) & 0xf)
#define T1_FLAGS_GET_MAG_CTX_NO(flags, j) ((UINT8)((T1_FLAGS_GET_NORMAL(flags) & (T1_SIG_HVD64 << (j*3))) >> (19 + j*3)))
UINT8 T1_INLINE T1_SC_SHUFFLE(T1_FLAG_TYPE flags0, T1_FLAG_TYPE flags1, T1_FLAG_TYPE flags2, int j)
{flags0;flags2;
	UINT32 f = (UINT32)(((flags1 >> (j*3)) & T1_SIG_HV) | ((flags1 >> (32 + j*3)) & (T1_SGN_V0|T1_SGN_H0|T1_SGN_V1|T1_SGN_H1)));
	return((UINT8)f);
}

#else

#define T1_FLAGS_GET_EXT(f) (f)
#define T1_FLAGS_GET_NORMAL(f) ((UINT32)f)
#define T1_FLAGS_GETZC_CTX_NO(flags, j) GetCtxNo_ZC(flags >> (j * 3))
#define T1_FLAGS_GET_MAG_CTX_NO(flags, j) /*T1_CTXNO_MAG+*/((UINT8)((T1_FLAGS_GET_NORMAL(flags)&(T1_SIG_HVD << (j*3))) != 0))
UINT8 T1_INLINE T1_SC_SHUFFLE(T1_FLAG_TYPE flags0, T1_FLAG_TYPE flags1, T1_FLAG_TYPE flags2, int j)
{
	int j3 = j * 3;
	UINT32 f = ((j == 0) ?												
			((((UINT32)flags1 >> (j3)) & T1_SIG_HV)/*HV bits*/|		
			 (((UINT32)flags1 >> (18+j3)) & 0x1)/*V0 sign*/|			
			 ((((UINT32)flags1 >> (22+j3)) & 0x1) << 6)/*V1 Sign*/|	
			 ((((UINT32)flags0 >> (19+j3)) & 0x1) << 2)/*H0 sign*/|	
			 ((((UINT32)flags2 >> (19+j3)) & 0x1) << 4)/*H1 sign*/)	
		:														
			((((UINT32)flags1 >> (j3)) & T1_SIG_HV)/*HV bits*/|		
			 (((UINT32)flags1 >> (16+j3)) & 0x1)/*V0 sign*/|			
			 ((((UINT32)flags1 >> (22+j3)) & 0x1) << 6)/*V1 Sign*/|	
			 ((((UINT32)flags0 >> (19+j3)) & 0x1) << 2)/*H0 sign*/|	
			 ((((UINT32)flags2 >> (19+j3)) & 0x1) << 4)/*H1 sign*/))	;
	return((UINT8)f);
}

#endif

#define T1_ENCODER_BUFSIZE (8*NCSJPC_ENCODE_BLOCK_SIZE*NCSJPC_ENCODE_BLOCK_SIZE) //32768

#define CBSIZE NCSJPC_ENCODE_BLOCK_SIZE
#define CBSTEP (CBSIZE * 4)
#define FLAG_STEP (CBSIZE+8)

class NCSECW_IMPEX CJPCCoderLUTs {
public:
	CJPCCoderLUTs() : m_plut_ctxno_zc(NULL){
		InitLUTs();
	}
	static UINT8 sm_lut_ctxno_zc0[512];
	static UINT8 sm_lut_ctxno_zc1[512];
	static UINT8 sm_lut_ctxno_zc2[512];
	static UINT8 sm_lut_ctxno_zc3[512];
	UINT8 *m_plut_ctxno_zc;
	static UINT8 sm_lut_ctxno_sc[256];
	static UINT8 sm_lut_spb[256];
	static bool	sm_bInit;

	UINT8 InitCtxNoZC(T1_FLAG_TYPE f, int orient) {
			int h,v,d,n,t,hv;
			n=0;
			t=h=((f&T1_SIG_H0)!=0)+((f&T1_SIG_H1)!=0);
			v=((f&T1_SIG_V0)!=0)+((f&T1_SIG_V1)!=0);
			d=((f&T1_SIG_D0)!=0)+((f&T1_SIG_D1)!=0)+((f&T1_SIG_D3)!=0)+((f&T1_SIG_D2)!=0);
			switch(orient) {
				case 2:
					h=v;v=t;
				case 0:
				case 1:
						if(!h){
							if(!v){
								if(!d){
									n=0;
								} else if(d==1){
									n=1;
								} else { 
									n=2;
								}
							} else if(v==1){
								n=3;
							} else {
								n=4;
							}
						}else if(h==1){
							if(!v){
								if(!d){
									n=5;
								}else{
									n=6;
								}
							} else{
								n=7;
							}
						}else{
							n=8;
						}
					break;
				case 3:
					hv=h+v;
					if(!d){
						if(!hv){
							n=0;
						}else if(hv==1){
							n=1;
						}else{
							n=2;
						}
					}else if(d==1){
						if(!hv){
							n=3;
						}else if(hv==1){ 
							n=4;
						}else{
							n=5;
						}
					}else if(d==2){
						if(!hv){
							n=6;
						}else{
							n=7;
						}
					}else{
						n=8;
					}
				break;
			}
			return (UINT8)(T1_CTXNO_ZC+n);
		}
	UINT8 InitCtxNoSC(T1_FLAG_TYPE f) {
			int hc,vc,n;
			n=0;
			hc=NCSMin(((f&(T1_SIG_H1|T1_SGN_H1))==T1_SIG_H1)+((f&(T1_SIG_H0|T1_SGN_H0))==T1_SIG_H0),1)-NCSMin(((f&(T1_SIG_H1|T1_SGN_H1))==(T1_SIG_H1|T1_SGN_H1))+((f&(T1_SIG_H0|T1_SGN_H0))==(T1_SIG_H0|T1_SGN_H0)),1);
			vc=NCSMin(((f&(T1_SIG_V0|T1_SGN_V0))==T1_SIG_V0)+((f&(T1_SIG_V1|T1_SGN_V1))==T1_SIG_V1),1)-NCSMin(((f&(T1_SIG_V0|T1_SGN_V0))==(T1_SIG_V0|T1_SGN_V0))+((f&(T1_SIG_V1|T1_SGN_V1))==(T1_SIG_V1|T1_SGN_V1)),1);
			if(hc<0){
				hc=-hc;vc=-vc;
			}
			if(!hc){
				if(vc==-1){
					n=1;
				}else if(!vc){
					n=0;
				}else{
					n=1;
				}
			}else if(hc==1){
				if(vc==-1){
					n=2;
				}else if(!vc){
					n=3;
				}else{
					n=4;
				}
			}
			return (UINT8)(T1_CTXNO_SC+n);
		}

	INT32 InitSPB(T1_FLAG_TYPE f) {
			int hc,vc,n;
			hc=NCSMin(((f&(T1_SIG_H1|T1_SGN_H1))==T1_SIG_H1)+((f&(T1_SIG_H0|T1_SGN_H0))==T1_SIG_H0),1)-NCSMin(((f&(T1_SIG_H1|T1_SGN_H1))==(T1_SIG_H1|T1_SGN_H1))+((f&(T1_SIG_H0|T1_SGN_H0))==(T1_SIG_H0|T1_SGN_H0)),1);
			vc=NCSMin(((f&(T1_SIG_V0|T1_SGN_V0))==T1_SIG_V0)+((f&(T1_SIG_V1|T1_SGN_V1))==T1_SIG_V1),1)-NCSMin(((f&(T1_SIG_V0|T1_SGN_V0))==(T1_SIG_V0|T1_SGN_V0))+((f&(T1_SIG_V1|T1_SGN_V1))==(T1_SIG_V1|T1_SGN_V1)),1);
			if(!hc&&!vc){
				n=0;
			}else{
				n=(!(hc>0||(!hc&&vc>0)));
			}
			return n;
		}
	void InitLUTs();

	T1_INLINE UINT8 GetCtxNo_ZC(T1_FLAG_TYPE f) {
			return m_plut_ctxno_zc[f&(T1_SIG_HVD)];
		}
	T1_INLINE UINT8 GetCtxNo_SC(T1_FLAG_TYPE f) {
			return sm_lut_ctxno_sc[f];
		}
};

// Some weird optimiser bug in the AMD64/PSDKv1400 compilers - this isn't performance critical 
// Moving this part into T_T1Coder.h also causes incorrect results
/*
#ifdef _MSC_VER
#pragma optimize( "", off )
#endif
void CJPCCoderLUTs::InitLUTs()	
{
	if(sm_bInit == false) {
		int i;

		for(i=0;i<512;++i) {
			sm_lut_ctxno_zc0[i] = InitCtxNoZC((T1_FLAG_TYPE)i,0);
		}
		for(i=0;i<512;++i) {
			sm_lut_ctxno_zc1[i] = InitCtxNoZC((T1_FLAG_TYPE)i,1);
		}
		for(i=0;i<512;++i) {
			sm_lut_ctxno_zc2[i] = InitCtxNoZC((T1_FLAG_TYPE)i,2);
		}
		for(i=0;i<512;++i) {
			sm_lut_ctxno_zc3[i] = InitCtxNoZC((T1_FLAG_TYPE)i,3);
		}
		for(i=0;i<256;i++) {
			sm_lut_ctxno_sc[i] = InitCtxNoSC((T1_FLAG_TYPE)i);
		}
		for(i=0;i<256;++i) {
			sm_lut_spb[i] = (UINT8)InitSPB((T1_FLAG_TYPE)i);
		}
		sm_bInit = true;
	}
}
#ifdef _MSC_VER
#pragma optimize( "", on )
#endif
*/

	/**
	 * CT1Coder class - the JPC T1 Coder.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #15 $ $Author: ctapley $ $Date: 2014/03/14 $ 
	 */	
//T1_SIGN_SHIFT ((sizeof(T)*8) - 1)
//#define T1_SIGN_MASK ((T)(((T)1) << T1_SIGN_SHIFT))
//#define T1_VALUE_MASK ((T)(~T1_SIGN_MASK))
namespace NCS {
namespace JPC {


template< class T, class U > struct K_Clear {
public:
	static const INT32 K_xSize = 1;
	static const INT32 K_ySize = 1;

	NCS_INLINE K_Clear(INT32 xStart, INT32 yStart, INT32 nStride, U *pBuf):_Buffer(xStart, yStart, nStride, pBuf), v(0x3) {};

	NCS_INLINE void operator()(INT32 x, INT32 y) {
		_Buffer(x,y) << v;
	};
private:
	typename T::AlignedBuffer _Buffer;
	T v;
};


//#define USE_DECODER_MACROS // Comment this out if you want functions instead (better for debug). Otherwise, speed is the same, as CMQDecoder::Decode using inline

//#ifdef USE_DECODER_MACROS
//#  define _mq_decode_(AC, pB, S, sym) _Decode(AC, pB, S, sym)
//#else // Do not use macros
//#  define _mq_decode_(AC, pB, S, sym) CMQDecoder::Decode(AC, pB, S, sym)
//#  define _mq_decode_nCDP_(AC, D, pB, S, sym) CMQDecoder::Decode(AC, D, pB, S, sym)
//#endif // USE_DECODER_MACROS

//things to try:
// decrefpass opt
//  + data bit shift opt
// sig pass flags opt
#define _Decode(AC, pB, S, sym)											\
{																		\
	UINT8 nNPS;															\
	UINT8 nD = S.NPS[2];												\
																		\
	UINT32 nQEvalShift16 = S.U.nQEvalShift16;								\
																				\
	if (((UINT16)AC) >= S.U.Qe.nQEval)	{			/*(C >= Qe)? */				\
		AC = AC - (S.U.Qe.nQEval + nQEvalShift16);	/*A-=Qe,C-=Qe*/				\
																				\
		if ((INT32)AC >= 0)  {							/*(A & 0x8000)? */		\
			/* MPS Exchange*/														\
			nNPS = (UINT8)(AC < nQEvalShift16);			/*NPS = (A < Qe) ? 1 : 0*/	\
																					\
			do {							/* Renorm */											\
				AC = (AC << 1) + pB[0];		/* A <<= 1, C = (C << 1) + B*/							\
				pB += pB[1];				/* B = Bnext*/											\
			} while (AC<0x80000000);			/* Till A >= 0x8000*/								\
																									\
			nD = S.NPS[2 + nNPS];																	\
			S = sm_States[S.NPS[nNPS]];	/* Switch State */											\
		}																			\
	}																				\
	else {										/*(C < Qe)*/						\
		/* LPSExchange*/															\
		nNPS = (UINT8)(AC >= 2 * nQEvalShift16);		/*NPS = (A >= 2*Qe) ? 1 : 0*/	\
		AC = ((UINT16)AC) + nQEvalShift16;			/*A = Qe*/						\
																					\
		do {							/* Renorm */											\
			AC = (AC << 1) + pB[0];		/* A <<= 1, C = (C << 1) + B*/							\
			pB += pB[1];				/* B = Bnext*/											\
		} while (AC < 0x80000000);			/* Till A >= 0x8000*/								\
																								\
		nD = S.NPS[2 + nNPS];																	\
		S = sm_States[S.NPS[nNPS]];	/* Switch State */											\
		sym = (INT8)nD; /*return((INT8)nD);*/													\
	}																						\
																							\
	sym = (INT8)nD; /*return((INT8)nD);*/													\
}


template <class T, int T1_SIGN_SHIFT, T T1_SIGN_MASK, T T1_VALUE_MASK> class NCSECW_IMPEX T_T1Decoder: public CJPCCoderLUTs, public CMQDecoder {
public:
	T_T1Decoder() {
			m_nBitPlane = 0;
			m_Scb = 0;
			m_bBypass = false;
			m_pData = NULL;
			m_bDataIsOwner = false;
	}

	virtual ~T_T1Decoder() {
		if (m_pData && m_bDataIsOwner) {
			delete m_pData;
			m_pData = NULL;
		}
	}

	void Init() {
		m_bBypass = false;
	};

	bool Decode(JPC::CSubBand::Type eSBType, 
				UINT8 roiShift, 
				UINT8 nBits,
				UINT8 nZeroBits,
				const std::vector<CSegment> &Segments,
				UINT32 nQualityLayers,
				SDK::CBuffer2D *pDecBuf,
				int Flags,
				CQCDMarker &Quantization,
				UINT8 nComponentBits,
				UINT8 nResolution,
				IEEE4 fReconstructionParameter,
				bool bUseDataPool)
		{
			NCSTimeStampUs tsBegin = CSystemInfo::GetTime();
			UINT32 nWidth = pDecBuf->GetWidth();
			UINT32 nHeight = pDecBuf->GetHeight();
			UINT8 orient = JPC::CSubBand::GetOrient(eSBType);
			m_Scb = (UINT8)Flags;

			SDK::CBuffer2D::Type eType = (sizeof(T) == sizeof(INT16)) ? SDK::CBuffer2D::BT_INT16 : ((sizeof(T) == sizeof(INT32)) ? SDK::CBuffer2D::BT_INT32 : SDK::CBuffer2D::BT_INT64);
			CT1Coder::CDataPool* pDataPool = NULL;
			if (bUseDataPool && (pDataPool = CT1Coder::GetDataPool(nWidth * 4, eType)) != NULL) { 
				m_pData = pDataPool->GetData();
				m_bDataIsOwner = false;
			}
			else {
				m_bDataIsOwner = true;
				//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::Decode failed to get data pool")); 
			}

			if (!m_pData) {
				m_pData = new SDK::CBuffer2D(0, 0, nWidth * 4, (1 + nHeight / 4), eType, SDK::CBuffer2D::HEAPDECODEJP2);
				m_bDataIsOwner = true;
				//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::Decode m_pData = NULL")); 
			} 
			//else 
				//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::Decode m_pData != NULL")); 

			if (m_bDataIsOwner && nWidth * 4 > m_pData->GetWidth() || (1 + nHeight / 4) > m_pData->GetHeight()) {
				m_pData->Alloc(0, 0, nWidth * 4, (1 + nHeight / 4), eType,SDK::CBuffer2D::HEAPDECODEJP2);
				m_bDataIsOwner = true;
			}
#ifndef NO_JP2_VPU
			T_VPU_apply<T, K_Clear>(m_pData->GetWidth(), m_pData->GetHeight(), m_pData->GetStride(), (T*)m_pData->GetPtr());
#else
			for (UINT32 k=0; k<nHeight; k+=4) {
				T *pData = (T*)m_pData->GetPtr(0, k >> 2);
				
				for (UINT32 i=0; i<nWidth; i++) {
					pData[i*4+0] = 0x3;
					pData[i*4+1] = 0x3;
					pData[i*4+2] = 0x3;
					pData[i*4+3] = 0x3;
				}
			}
	
#endif
			UINT32 h = 2 + (nHeight / 4) + ((nHeight % 4) ? 1 : 0);
			UINT32 w = nWidth + 2;
			if(h > m_Flags.GetHeight() || w > m_Flags.GetWidth()) {
				m_Flags.Alloc(0, 0, w, h, NCSJPC_T1_FLAG_BUFTYPE, SDK::CBuffer2D::HEAPDECODEJP2);
			}
			m_Flags.Clear();

			m_bBypass = false;

			int passno = nZeroBits * 3;
			CMQCoder::ResetStates();

			switch(orient) {
				case 0: m_plut_ctxno_zc = sm_lut_ctxno_zc0; break;
				case 1: m_plut_ctxno_zc = sm_lut_ctxno_zc1; break;
				case 2: m_plut_ctxno_zc = sm_lut_ctxno_zc2; break;
				case 3: m_plut_ctxno_zc = sm_lut_ctxno_zc3; break;
			}
	
			UINT16 nMSBs = nZeroBits;
			
			for (UINT32 segno = 0; segno<Segments.size() ; segno++) {
				const CSegment &Seg = Segments[segno];
				UINT8 nNumberOfPassesRequired = Seg.GetNumberOfPassesNeededToDecodeQualityLayer(nQualityLayers);
				UINT32 nDataLengthRequired = Seg.GetDataLengthNeededToDecodeQualityLayer(nQualityLayers);
				UINT8* pData = Seg.m_pData;
				if (nDataLengthRequired != Seg.m_nLength){
					pData = (UINT8*)NCSMalloc(nDataLengthRequired + 2, FALSE);
					if (nDataLengthRequired > 0){
						memcpy(pData, Seg.m_pData, nDataLengthRequired);
					}
					pData[nDataLengthRequired] = 0xFF;
					pData[nDataLengthRequired + 1] = 0xFF;
					nDataLengthRequired += 2;
				}
				if (pData) {

					if(((passno - nZeroBits * 3) >= 10) && (((passno + 2) % 3) != 2) && (Flags & CT1Coder::SELECTIVE_CODING_BYPASS)) {
						m_bBypass = true;
						const static NCS::CString sStreamName = NCS_T("T1_DecoderStream");
						m_Stream.Open(pData, nDataLengthRequired + 2,false,false,sStreamName);
					} else {
						m_bBypass = false;
						CMQDecoder::Init(pData, nDataLengthRequired);
					}
					

					for (UINT32 nSegPass = 0; nSegPass < nNumberOfPassesRequired; ++nSegPass) {
						m_nBitPlane = (INT8)(1+roiShift+nBits - 1 - (nZeroBits + (passno + nSegPass - (int)nZeroBits * 3 + 2) / 3));
						
						int passtype = (passno + nSegPass + 2) % 3;

						switch (passtype) {
							case 0:
									if(m_bBypass) {
										DecSigPassBypass(nWidth, nHeight);
									} else {
										DecSigPass(nWidth, nHeight);
									}
									nMSBs++;
								break;
							case 1:
									if(m_bBypass) {
										DecRefPassBypass(nWidth, nHeight);
									} else {
										DecRefPass(nWidth, nHeight);
									}
								break;
							case 2:
									DecClnPass(nWidth, nHeight, (Flags & CT1Coder::SEGMENT_SYMBOLS) ? true : false);

								break;
						}
						bool bLastSegPass = nSegPass == (UINT32)(nNumberOfPassesRequired - 1);
						bool bLastSegment = (segno == Segments.size() - 1) ;
						if (bLastSegment && bLastSegPass) {
							if(passtype < 1) {
								DecShiftPass(nWidth,nHeight);
							}
							ShiftData(m_nBitPlane-1, nWidth, nHeight);
						}
						if(Flags & CT1Coder::PASS_RESET_CTX) {
							CMQCoder::ResetStates();
						}
					}
					if(m_bBypass) {
						m_Stream.Close();
					}
				} 
				if (pData != Seg.m_pData){
					NCSFree(pData);
				}
				passno += Seg.m_nPasses;
			}

			switch(pDecBuf->GetType()) {
				case SDK::CBuffer2D::BT_INT16:
						Dequantize(pDecBuf, nWidth, nHeight, roiShift, (INT16*)NULL);
					break;
				case SDK::CBuffer2D::BT_INT32:
						Dequantize(pDecBuf, nWidth, nHeight, roiShift, (INT32*)NULL);
					break;
				case SDK::CBuffer2D::BT_INT64:
						Dequantize(pDecBuf, nWidth, nHeight, roiShift, (INT64*)NULL);
					break;
				case SDK::CBuffer2D::BT_IEEE4:
					{
						INT16 nMantissa = 0;
						INT16 nExponent = 0;

						if(Quantization.m_Sqc.m_eStyle == CQuantizationParameter::SCALAR_DERIVED) {
							nExponent = Quantization.m_SPqc[JPC::CSubBand::LL].m_Irreversible.m_nExponent;
							nMantissa = Quantization.m_SPqc[JPC::CSubBand::LL].m_Irreversible.m_nMantissa;
						} else if(eSBType == JPC::CSubBand::LL) {
							nMantissa = Quantization.m_SPqc[eSBType].m_Irreversible.m_nMantissa;
							nExponent = Quantization.m_SPqc[eSBType].m_Irreversible.m_nExponent;
						} else {
							INT32 i = 1 + (eSBType - 1) + 3 * (nResolution - 1);
							nMantissa = Quantization.m_SPqc[i].m_Irreversible.m_nMantissa;
							// FIXME - not 100% on this one, but by some fluke it seems to work for now... 
							nExponent = Quantization.m_SPqc[eSBType].m_Irreversible.m_nExponent;
						}
						float fScale = 1.0f;
						switch(eSBType) {
							case JPC::CSubBand::LL:
									fScale = (float)NCS2PowS(nComponentBits - nExponent) * (1.0f + (float)nMantissa / NCSJPC_2POW11);
								break;
							case JPC::CSubBand::LH:
									fScale = (float)NCS2PowS(nComponentBits + 0 - nExponent) * (1.0f + (float)nMantissa / NCSJPC_2POW11);
								break;
							case JPC::CSubBand::HL:
									fScale = (float)NCS2PowS(nComponentBits + 0 - nExponent) * (1.0f + (float)nMantissa / NCSJPC_2POW11);
								break;
							case JPC::CSubBand::HH:
									fScale = (float)NCS2PowS(nComponentBits + 0 - nExponent) * (1.0f + (float)nMantissa / NCSJPC_2POW11);
								break;
						}
						IEEE4 fOffset = fReconstructionParameter * (IEEE4)NCS2PowS(nBits - nMSBs);

						if(roiShift == 0) {
							for (UINT32 k=0; k<nHeight; k+=4) {
								UINT32 max_j = NCSMin(4, nHeight - k);
								IEEE4 *pDst = (IEEE4*)pDecBuf->GetPtr(0, k);
								T *pData = (T*)m_pData->GetPtr(0, k >> 2);
								INT32 nStride = pDecBuf->GetStride();
								T1_FLAG_TYPE *pFlags = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k/4+1);

								for (UINT32 i=0; i<nWidth; i++) {
									register T1_FLAG_TYPE flags = pFlags[i];
									for(UINT32 j = 0; j < max_j; j++) {
										if(flags & (T1_SIG << (j*3))) {
											T v = (T)(pData[i * 4 + j]);
											
											if(v == 0) {
												pDst[i + j * nStride] = 0;
											} else if(flags & (T1_SGN << (j*3))) {
												pDst[i + j * nStride] = (((1-v) >> 1) - fOffset) * fScale;
												
											} else {
												pDst[i + j * nStride] = ((v >> 1) + fOffset) * fScale;
												
											}
										} else {
											pDst[i + j * nStride] = 0;
										}
									}
								}
							}
						} else {
							INT32 nMax = 1 << (roiShift+1);
							IEEE8 dRShift = (IEEE8)(1 << roiShift); // to be divided by this value below (same as >>= roiShift)
							for (UINT32 k=0; k<nHeight; k+=4) {
								UINT32 max_j = NCSMin(4, nHeight - k);
								IEEE4 *pDst = (IEEE4*)pDecBuf->GetPtr(0, k);
								T *pData = (T*)m_pData->GetPtr(0, k >> 2);
								INT32 nStride = pDecBuf->GetStride();
								T1_FLAG_TYPE *pFlags = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k/4+1);

								for (UINT32 i=0; i<nWidth; i++) {
									register T1_FLAG_TYPE flags = pFlags[i];
									for(UINT32 j = 0; j < max_j; j++) {
										if(flags & (T1_SIG << (j*3))) {
											T v = (T)(pData[i * 4 + j]);
											IEEE8 dv = (IEEE8)v;
											if(v >= nMax ) {
												v >>= roiShift;
												dv /= dRShift;
											}
											if(v == 0) {
												pDst[i + j * nStride] = 0;
											} else if(flags & (T1_SGN << (j*3))) {
											
												pDst[i + j * nStride] = ((-dv / 2.0) - fOffset) * fScale; // this fixes p0_06.jpc
											} else {
												
												pDst[i + j * nStride] = ((dv / 2.0) + fOffset) * fScale;
											}
										} else {
											pDst[i + j * nStride] = 0;
										}
									}
								}
							}
						}
					}
					break;
			}

			/*
			for (int k=0; k<h; k+=4) {
				int max_j = NCSMin(4, h - k);
				int nFlagsStep = m_Flags.GetStride();
				T1_FLAG_TYPE *pFlags = (T1_FLAG_TYPE*)m_Flags.GetPtr(k+1, 1);
				HDC hDC = GetDC(NULL);

				for (int sm_i=0; sm_i<w; sm_i++, pFlags += nFlagsStep) {
					for(int n = 0; n < max_j; n++) {
						if(*(pFlags + n) & T1_SIG) {
							SetPixel(hDC, sm_i, k + n, RGB(0xff, 0x00, 0x00));
						} else {
							SetPixel(hDC, sm_i, k + n, RGB(0x00, 0xff, 0x00));
						}
					}
				}
				ReleaseDC(NULL, hDC);
			}*/

			// GetDataPool
			if (pDataPool)
				CT1Coder::UnRefData(pDataPool);

			CT1Coder::sm_usLast = CSystemInfo::GetTime() - tsBegin;
			CT1Coder::sm_usTotal += CT1Coder::sm_usLast;
			CT1Coder::sm_nLastSamples = nWidth * nHeight;
			CT1Coder::sm_nTotalSamples += CT1Coder::sm_nLastSamples;
			return(true);
		}
		

private:
	bool m_bBypass;

	SDK::CBuffer2D* m_pData;
	SDK::CBuffer2D m_Flags;
	bool m_bDataIsOwner;
 
	CMemoryIOStream m_Stream;
	INT8 m_nBitPlane;
	UINT8 m_Scb;

		//
		// Dequantize data, and fixup sign
		//
		// Data always stored as ABS value
		// Sign bit stored in flags
		//
		// Basic algorithm is:
		//
		// No ROI:
		//
		//		if(v < 0) {
		//			v += 1;
		//		}
		//
		// With ROI:
		//
		//		if(abs(v) >= 1 << roiShift) {
		//			v >>= roiShift;
		//		}
		//		if(v < 0) {
		//			v += 1;
		//		}
		//
	template<class U> void Dequantize(SDK::CBuffer2D *pDecBuf, 
										UINT32 nWidth, 
										UINT32 nHeight,
										UINT8 roiShift,
										U *pDummy) {
			INT32 nXO = pDecBuf->GetX0();
			INT32 nYO = pDecBuf->GetY0();
			if(roiShift == 0) {
				for (UINT32 k=0; k<nHeight; k+=4) {
					UINT32 max_j = NCSMin(4, nHeight - k);
					U *pDst = (U*)pDecBuf->GetPtr(nXO, nYO+k);
					T *pData = (T*)m_pData->GetPtr(0, k >> 2);
					INT32 nStride = pDecBuf->GetStride();
					
					T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k / 4 + 1);
 
					if(max_j == 4) {
						for (UINT32 i = 0; i<nWidth; i++, pFlags_ptr++) {
							UINT32 i4 = i * 4;
							register T1_FLAG_TYPE flags = *pFlags_ptr; // pFlags[i];
							T v;
							//ROW 0
							if(T1_FLAGS_GET_NORMAL(flags) & T1_SIG_11) {
								v = (T)(pData[i4 + 0]);
								pDst[i + 0 * nStride] = (U)T1_NEG_PLUS_1_IF_1(T, (flags >> (T1_SGN_1_SHIFT + (0))) & 0x1, v)>>1;// 0 = 0 * 3
							} else {
								pDst[i + 0 * nStride] = 0;
							}
							//ROW 1
							if(T1_FLAGS_GET_NORMAL(flags) & T1_SIG_12) {
								v = (T)(pData[i4 + 1]);
								pDst[i + 1 * nStride] = (U)T1_NEG_PLUS_1_IF_1(T, (flags >> (T1_SGN_1_SHIFT + (3))) & 0x1, v)>>1;// 3= 1 * 3
							} else {
								pDst[i + 1 * nStride] = 0;
							}
							//ROW 2
							if(T1_FLAGS_GET_NORMAL(flags) & T1_SIG_13) {
								v = (T)(pData[i4 + 2]);
								pDst[i + 2 * nStride] = (U)T1_NEG_PLUS_1_IF_1(T, (flags >> (T1_SGN_1_SHIFT + (6))) & 0x1, v)>>1;// 6= 2 * 3
							} else {
								pDst[i + 2 * nStride] = 0;
							}
							//ROW 3
							if(T1_FLAGS_GET_NORMAL(flags) & T1_SIG_14) {
								v = (T)(pData[i4 + 3]);
								pDst[i + 3 * nStride] = (U)T1_NEG_PLUS_1_IF_1(T, (flags >> (T1_SGN_1_SHIFT + (9))) & 0x1, v)>>1;// 9 = 3*3
							} else {
								pDst[i + 3 * nStride] = 0;
							}
						}
					} else {
						for (UINT32 i = 0; i<nWidth; i++, pFlags_ptr++) {
							UINT32 i4 = i * 4;
							register T1_FLAG_TYPE flags = *pFlags_ptr; // pFlags[i];
							for(UINT32 j = 0; j < max_j; j++) {
								UINT32 j3 = j * 3;
								if(flags & (T1_SIG << (j3))) {
									T v = (T)(pData[i4 + j]);
									pDst[i + j * nStride] = (U)T1_NEG_PLUS_1_IF_1(T, (flags >> (T1_SGN_1_SHIFT + (j3))) & 0x1, v)>>1;
								} else {
									pDst[i + j * nStride] = 0;
								}
							}
						}
					}
				}
			} else {
				INT32 nMax = 1 << (roiShift+1);
				for (UINT32 k=0; k<nHeight; k+=4) {
					UINT32 max_j = NCSMin(4, nHeight - k);
					U *pDst = (U*)pDecBuf->GetPtr(nXO, nYO+k);
					T *pData = (T*)m_pData->GetPtr(0, k >> 2);
					INT32 nStride = pDecBuf->GetStride();
				
					T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k / 4 + 1);

					for (UINT32 i = 0; i<nWidth; i++, pFlags_ptr++) {
						UINT32 i4 = i * 4;
						register T1_FLAG_TYPE flags = *pFlags_ptr; // pFlags[i];
						for(UINT32 j = 0; j < max_j; j++) {
							UINT32 j3 = j * 3;
							if(flags & (T1_SIG << (j3))) {
								T v = (T)(pData[i4 + j]);
								if(v >= nMax) {
									v >>= roiShift;
								}
								pDst[i + j * nStride] = (U)T1_NEG_PLUS_1_IF_1(T, (flags >> (T1_SGN_1_SHIFT + (j3))) & 0x1, v)>>1;
							} else {
								pDst[i + j * nStride] = 0;
							}						
						}
					}
				}
			}
		};
	void ShiftData(UINT8 nShift, INT32 nWidth, INT32 nHeight) {
			if(nShift > 0) {
				int k=0;
#ifdef NCS_X86_MMI
				bool bUseSSE3 = true; // test temp for SSE3Present
				if (bUseSSE3 && CSystemInfo::SSE3Present() && m_pData->GetType() == SDK::CBuffer2D::BT_INT16) {
					// we do not consider flags as ShiftData is the last Pass process
					__m128i count = _mm_set_epi16(0, 0, 0, 0, 0, 0, 0, nShift);
					for (; k<nHeight; k += 4) {
						__m128i *pData = (__m128i*)m_pData->GetPtr(0, k >> 2);
						int w = nWidth / 2;
						int i = 0;

						for (; i<w; i++) {
							__m128i d = _mm_load_si128(&pData[i]);
							d = _mm_sll_epi16(d, count);
							_mm_store_si128(&pData[i], d);
						}
						i *= 2;
						for (; i < nWidth; i++) {
							T *pData = (T*)m_pData->GetPtr(0, k >> 2);
							pData[i * 4 + 0] <<= nShift;
							pData[i * 4 + 1] <<= nShift;
							pData[i * 4 + 2] <<= nShift;
							pData[i * 4 + 3] <<= nShift;
						}
					}
				} else if (CSystemInfo::SSE2Present() && m_pData->GetType() == SDK::CBuffer2D::BT_INT16) {
					__m128i count = _mm_set_epi16(0, 0, 0, 0, 0, 0, 0, nShift);
					for (; k<nHeight; k+=4) {
						__m128i *pData = (__m128i*)m_pData->GetPtr(0, k >> 2);
						int w = nWidth / 2;
						int i = 0;

						for (; i<w; i++) {
							__m128i d = _mm_load_si128(&pData[i]);
							d = _mm_sll_epi16(d, count);
							_mm_store_si128(&pData[i], d);
						}
						i *= 2;
						for(; i < nWidth; i++) {
							T *pData = (T*)m_pData->GetPtr(0, k >> 2);
							pData[i*4+0] <<= nShift;
							pData[i*4+1] <<= nShift;
							pData[i*4+2] <<= nShift;
							pData[i*4+3] <<= nShift;
						}
					}
				} else 
#ifdef NCS_X86_MMI_MMX
				if(CSystemInfo::MMXPresent() && m_pData->GetType() == SDK::CBuffer2D::BT_INT16) {
					__m64 count = _mm_set_pi16(0, 0, 0, nShift);
					for (; k<nHeight; k+=4) {
						__m64 *pData = (__m64*)m_pData->GetPtr(0, k >> 2);
						int i = -nWidth;
						pData += nWidth;

						for (; i<0; i++) {
							__m64 d = pData[i];
							pData[i] = _mm_sll_pi16(d, count);
						}
					}
					_mm_empty();
				}
#endif
#endif //NCS_X86_MMI
				for(; k<nHeight; k+=4) {
					T *pData = (T*)m_pData->GetPtr(0, k >> 2);
					T1_FLAG_TYPE *pFlags = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k/4+1);
					int i = -nWidth;
					pData += 4*nWidth;
					pFlags += nWidth;

					for (; i<0; i++) {
						if(pFlags[i] & (T1_SIG_11|T1_SIG_12|T1_SIG_13|T1_SIG_14)) {
							pData[i*4] <<= nShift;
							pData[i*4+1] <<= nShift;
							pData[i*4+2] <<= nShift;
							pData[i*4+3] <<= nShift;
						}
					}
				}
			}
		};


	T1_INLINE void DecSigPassStepBypass(T1_FLAG_TYPE *fp, int j, UINT8 bVCC = false) {
			if (!(*fp&(T1_SIG << (j*3))) && (*fp&(T1_SIG_HVD << (j*3)))) {
				if(GetBit()) {
					int v=GetBit();
					UpdateFlags(fp, v, j, bVCC);
				}
				*fp|=T1_VISIT << (j*3);
			}
		}
	void DecSigPassBypass(int w, int h) {
			for (int k=0; k<h; k+=4) {
				int max_j = NCSMin(4, h - k);
				
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k / 4 + 1);

				if(max_j == 4) {
					for (int i = 0; i<w; i++, pFlags_ptr++) {
						DecSigPassStepBypass(pFlags_ptr, 0, (m_Scb & CT1Coder::VERTICAL_CAUSAL_CTX));
						DecSigPassStepBypass(pFlags_ptr, 1);
						DecSigPassStepBypass(pFlags_ptr, 2);
						DecSigPassStepBypass(pFlags_ptr, 3);
					}
				} else {
					for (int i = 0; i<w; i++, pFlags_ptr++) {
						DecSigPassStepBypass(pFlags_ptr, 0, (m_Scb & CT1Coder::VERTICAL_CAUSAL_CTX));
						for (int j=1; j< max_j; j++) {
							DecSigPassStepBypass(pFlags_ptr, j);
						}
					}
				}
			}
		}; 

	T1_INLINE void DecSigPassStep(register T1_FLAG_TYPE &f, register T1_FLAG_TYPE *fp,
		register int j,
		register UINT32 &AC,
		register UINT8 *&pB,
		State *Contexts,
		UINT8 bVCC = false) 
	{
		{
			INT32 nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(f)]);
			if (nSym) {
				f = T1_SC_SHUFFLE(fp[-1], fp[0], fp[1], j);
				nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
				int v = nSym ^ GetSPB(f);
				UpdateFlags(fp, v, j, bVCC);
			}
			*fp |= T1_VISIT << (j * 3);
		}
	}

	void DecSigPass(int w, int h) {
			register UINT32 AC = m_AC;
			register UINT8 *pB = m_pB;
			State Contexts[NCSJPC_MQC_NUMCTXS];
			memcpy(&Contexts[T1_CTXNO_ZC], &m_Contexts[T1_CTXNO_ZC], sizeof(State)*T1_NUMCTXS_ZC);
			memcpy(&Contexts[T1_CTXNO_SC], &m_Contexts[T1_CTXNO_SC], sizeof(State)*T1_NUMCTXS_SC);
			int k, h2 = h / 4;
			UINT8 bVCC = m_Scb & CT1Coder::VERTICAL_CAUSAL_CTX;
			
			//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecSigPass start AC = %ld"), m_AC);

			for(k=0; k<h2; k++) {
				
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1 + w, k + 1);
				pFlags_ptr -= w;

				INT32 nStride = m_Flags.GetStride();
				
				for (int i = -w; i < 0; i++, pFlags_ptr++) {
					register T1_FLAG_TYPE flags = *pFlags_ptr; // pFlags[i];
					if(T1_FLAGS_GET_NORMAL(flags) != 0) {
						//ROW 0
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG) << (0))) && (T1_FLAGS_GET_NORMAL(flags)&(T1_SIG_HVD << (0)))) { // 0 = 0 * 3
							INT32 nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (0))]);
							if (nSym) { // 0 = 0 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 0);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								// update neighbourhood flags
								UpdateFlags(flags, pFlags_ptr, v, 0, bVCC, nStride);
							} else {
								flags |= T1_VISIT << (0); // 0 = 0 * 3
							}
						}
						//ROW 1
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG) << (3))) && (T1_FLAGS_GET_NORMAL(flags)&(T1_SIG_HVD << (3)))) { // 3 = 1 * 3
							INT32 nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (3))]);
							if (nSym) { // 3 = 1 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 1);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 1, bVCC, nStride);
							} else {
								flags |= T1_VISIT << (3); // 3 = 1 * 3
							}
						}
						//ROW 2
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG) << (6))) && (T1_FLAGS_GET_NORMAL(flags)&(T1_SIG_HVD << (6)))) { // 6 = 2 * 3
							INT32 nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (6))]);
							if (nSym) { // 6 = 2 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 2);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 2, bVCC, nStride);
							} else {
								flags |= T1_VISIT << (6); // 6 = 2 * 3
							}
						}
						//ROW 3
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG) << (9))) && (T1_FLAGS_GET_NORMAL(flags)&(T1_SIG_HVD << (9)))) { // 9 = 3*3
							INT32 nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (9))]);
							if (nSym) { // 9 = 3*3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 3);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 3, bVCC, nStride);
							} else {
								flags |= T1_VISIT << (9); // 9 = 3*3
							}
						}
					
						*pFlags_ptr = flags;
					} 
					else {
						// Speed up when can skip over of zero neighbourhoods
						for (pFlags_ptr += 3, i += 3; i < 0 && T1_FLAGS_GET_NORMAL(*pFlags_ptr) == 0; pFlags_ptr += 3, i += 3);
						pFlags_ptr -= 3;
						i -= 3;
					}
				}
			}
			for (k *= 4; k<h; k+=4) {
				int max_j = NCSMin(4, h - k);
				
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1 + w, k / 4 + 1);								
				pFlags_ptr += (-w);

				for (int i = -w; i<0; i++, pFlags_ptr++) {
					if (T1_FLAGS_GET_NORMAL(*pFlags_ptr) == 0)
						continue;

					T1_FLAG_TYPE f = *pFlags_ptr >> (0); // 0 = 0 * 3
					if (!(f&T1_SIG) && (f&T1_SIG_HVD)) 
						DecSigPassStep(f, pFlags_ptr, 0, AC, pB, Contexts, bVCC);
					for (int j = 1; j < max_j; j++) {
						f = *pFlags_ptr >> (j * 3);
						if (!(f&T1_SIG) && (f&T1_SIG_HVD))
							DecSigPassStep(f, pFlags_ptr, j, AC, pB, Contexts);
					}
				}

			}

			m_AC = AC;
			m_pB = pB;
			memcpy(&m_Contexts[T1_CTXNO_ZC], &Contexts[T1_CTXNO_ZC], sizeof(State)*T1_NUMCTXS_ZC);
			memcpy(&m_Contexts[T1_CTXNO_SC], &Contexts[T1_CTXNO_SC], sizeof(State)*T1_NUMCTXS_SC);
		};

	T1_INLINE void DecRefPassStepBypass(T1_FLAG_TYPE *fp, T *dp, int j) {
			if (*fp & (T1_REFINE << (j*3))) {
				int v = GetBit();
				*dp = (T)((*dp << 1) - (v|1));
			}
		}
	void DecRefPassBypass(int w, int h) {
			for (int k=0; k<h; k+=4) {
				
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k / 4 + 1);
				
				T *pData = (T*)m_pData->GetPtr(0, k >> 2);

				for (int i = 0; i<w; i++, pFlags_ptr++) {
					int i4 = i * 4;
					DecRefPassStepBypass(pFlags_ptr, pData + i4 + 0, 0);
					DecRefPassStepBypass(pFlags_ptr, pData + i4 + 1, 1);
					DecRefPassStepBypass(pFlags_ptr, pData + i4 + 2, 2);
					DecRefPassStepBypass(pFlags_ptr, pData + i4 + 3, 3);
				}
			} 
		}
	T1_INLINE void DecRefPassStep(T1_FLAG_TYPE *fp, 
								  T *dp, 
								  int j, 
								  register UINT32 &AC,
								  register UINT8 *&pB,
								  register State &REF,
								  State *Contexts) {
			if (*fp & (T1_REFINE << (j*3))) {
				T t = *dp;
				int v;
				if((t) > 4) {
					t <<= 1;
					v = CMQDecoder::Decode(AC, pB, REF);
				} else {
					UINT8 nCtx = T1_FLAGS_GET_MAG_CTX_NO(*fp, j);
					t <<= 1;
					v = CMQDecoder::Decode(AC, pB, Contexts[nCtx]);
				}
				*dp = (T)((t) - (v|1));
			}
		}

	  void DecRefPass(int w, int h) {
			State Contexts[2];
			Contexts[0] = m_Contexts[T1_CTXNO_MAG+0];
			Contexts[1] = m_Contexts[T1_CTXNO_MAG+1];
			register UINT32 AC = m_AC;
			register UINT8 *pB = m_pB;

			//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecRefPass start AC = %ld"), m_AC);

			int k, h2 = h / 4;
			for(k=0; k<h2; k++) {
				
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k + 1);				
				T *pData = (T*)m_pData->GetPtr(0, k);
				register State REF = m_Contexts[T1_CTXNO_MAG+2];

				int v;
				for (int i = 0; i<w; i++, pFlags_ptr++) {
					register T1_FLAG_TYPE flags = *pFlags_ptr; // pFlags[i];
					if((T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE_1|T1_REFINE_2|T1_REFINE_3|T1_REFINE_4)) != 0) {
						//int i4 = i * 4;
						//ROW 0
						if (T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE << (0))) {// 0 = 0 * 3
							T t = *pData; // pData[i4];
							if((t) > 4) {
								t <<= 1;
								v = CMQDecoder::Decode(AC, pB, REF);
							} else {
								UINT8 nCtx = T1_FLAGS_GET_MAG_CTX_NO(flags, 0);
								t <<= 1;
								v = CMQDecoder::Decode(AC, pB, Contexts[nCtx]);
							}
							
							*pData = (T)((t)-(v | 1));
						}
						pData++;
						//ROW 1
						if (T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE << (3))) {// 3= 1 * 3
							T t = *pData; 
							if((t) > 4) {
								t <<= 1;
								v = CMQDecoder::Decode(AC, pB, REF);
							} else {
								UINT8 nCtx = T1_FLAGS_GET_MAG_CTX_NO(flags, 1);
								t <<= 1;
								v = CMQDecoder::Decode(AC, pB, Contexts[nCtx]);
							}
							*pData = (T)((t)-(v | 1));
						}
						pData++;
						//ROW 2
						if (T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE << (6))) {// 6= 2 * 3
							T t = *pData; 
							if((t) > 4) {
								t <<= 1;
								v = CMQDecoder::Decode(AC, pB, REF);
							} else {
								UINT8 nCtx = T1_FLAGS_GET_MAG_CTX_NO(flags, 2);
								t <<= 1;
								v = CMQDecoder::Decode(AC, pB, Contexts[nCtx]);
							}
							*pData = (T)((t)-(v | 1));
						}
						pData++;
						//ROW 3
						if (T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE << (3*3))) {// 9 = 3*3
							T t = *pData; 
							if((t) > 4) {
								t <<= 1;
								v = CMQDecoder::Decode(AC, pB, REF);
							} else {
								UINT8 nCtx = T1_FLAGS_GET_MAG_CTX_NO(flags, 3);
								t <<= 1;
								v = CMQDecoder::Decode(AC, pB, Contexts[nCtx]);
							}
							*pData = (T)((t)-(v | 1));
						}
						pData++;
					}
					else {
						pData += 4;
						// Speed up when can skip over of zero neighbourhoods
						for (pFlags_ptr += 2; i < 0 && T1_FLAGS_GET_NORMAL(*pFlags_ptr) == 0 ; pFlags_ptr += 2, i += 2, pData += 8); // OK, pData += 8 needs to test
						pFlags_ptr -= 2;
					}					
				}
				m_Contexts[T1_CTXNO_MAG+2] = REF;
			}
			for (k *= 4; k<h; k+=4) {
				int max_j = NCSMin(4, h - k);
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k / 4 + 1);
				T *pData = (T*)m_pData->GetPtr(0, k >> 2);
				register State REF = m_Contexts[T1_CTXNO_MAG+2];
				for (int i = 0; i<w; i++, pFlags_ptr++) {
					if (T1_FLAGS_GET_NORMAL(*pFlags_ptr) == 0) 
						continue;

					int i4 = i * 4;
					DecRefPassStep(pFlags_ptr, pData + i4, 0, AC, pB, REF, Contexts);
					for (int j=1; j< max_j; j++) {
						DecRefPassStep(pFlags_ptr, pData + i4 + j, j, AC, pB, REF, Contexts);
					}
				}
				m_Contexts[T1_CTXNO_MAG+2] = REF;
			}

			m_AC = AC;
			m_pB = pB;
			m_Contexts[T1_CTXNO_MAG+0] = Contexts[0];
			m_Contexts[T1_CTXNO_MAG+1] = Contexts[1];
		}
void DecShiftPass(int w, int h) {
			int k, h2 = h / 4;
			for(k=0; k<h2; k++) {
				
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k + 1);
				T *pData = (T*)m_pData->GetPtr(0, k);
			
				for (int i=0; i<w; i++) {
					register T1_FLAG_TYPE flags = *pFlags_ptr++; 
					if((T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE_1|T1_REFINE_2|T1_REFINE_3|T1_REFINE_4)) != 0) {
						UINT32 i4 = i * 4;
						//ROW 0
						if (T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE << (0))) {// 0 = 0 * 3
							pData[i4] <<= 1;
						}
						//ROW 1
						if (T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE << (3))) {// 3= 1 * 3
							pData[i4 + 1] <<= 1;
						}
						//ROW 2
						if (T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE << (6))) {// 6= 2 * 3
							pData[i4 + 2] <<= 1;
						}
						//ROW 3
						if (T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE << (9))) {// 9 = 3*3
							pData[i4 + 3] <<= 1;
						}
					}
				}
			}
			for (k *= 4; k<h; k+=4) {
				int max_j = NCSMin(4, h - k);
				T1_FLAG_TYPE *pFlags = (T1_FLAG_TYPE*)m_Flags.GetPtr(1, k/4+1);
				T *pData = (T*)m_pData->GetPtr(0, k >> 2);
				for (int i=0; i<w; i++) {
					int i4 = i * 4;
					register T1_FLAG_TYPE flags = pFlags[i];
					for (int j=0; j< max_j; j++) {
						if (T1_FLAGS_GET_NORMAL(flags) & (T1_REFINE << (j*3))) {
							pData[i4 + j] <<= 1;
						}
					}
				}
			}
		}

	T1_INLINE void DecClnPassStep(register T1_FLAG_TYPE *fp, 
								  register int j, 
								  register UINT32 &AC, 
								  register UINT8 *&pB, 
								  State *Contexts,
								  UINT8 bVCC = false) {
			if (!(*fp&((T1_SIG|T1_VISIT) << (j*3)))) {
				INT32 nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(*fp >> (j * 3))]);
				if (nSym) {
					UINT32 f = T1_SC_SHUFFLE(fp[-1], fp[0], fp[1], j);
					nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
					int v = nSym^ GetSPB(f);
					UpdateFlags(fp, v, j, bVCC);
				}
			}
		}
	T1_INLINE void DecClnPassStepPartial(register T1_FLAG_TYPE *fp, 
										register int j, 
										register UINT32 &AC, 
										register UINT8 *&pB, 
										State *Contexts,
										UINT8 bVCC = false) {
			UINT32 f = T1_SC_SHUFFLE(fp[-1], fp[0], fp[1], j);
			INT32 nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
			int v = nSym^GetSPB(f);
			UpdateFlags(fp, v, j, bVCC);
		}
	void DecClnPass(int w, int h, bool segsym) {
			register UINT32 AC = m_AC;
			register UINT8 *pB = m_pB;
			State Contexts[NCSJPC_MQC_NUMCTXS];
			memcpy(&Contexts[T1_CTXNO_ZC], &m_Contexts[T1_CTXNO_ZC], sizeof(State)*T1_NUMCTXS_ZC);
			memcpy(&Contexts[T1_CTXNO_SC], &m_Contexts[T1_CTXNO_SC], sizeof(State)*T1_NUMCTXS_SC);
			int k, h2 = h / 4;
			UINT8 bVCC = m_Scb & CT1Coder::VERTICAL_CAUSAL_CTX;

			//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass start AC = %ld"), m_AC);

#define USE_NON_CDP // undefining is previously version without skip 4 cols

// for flag != 0, undefining is to use CMQDecoder::Decode(, , ) otherwise CMQDecoder::Decode(, , , )
// Thr later however needs further tuning
//#define USE_NON_CDP_FLAG 

#ifndef USE_NON_CDP
			for(k=0; k<h2; k++) {
				
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1 + w, k + 1);
				pFlags_ptr -= w;
				INT32 nStride = m_Flags.GetStride();

				register State AGG = m_Contexts[T1_CTXNO_AGG];

				for (int i = -w; i<0; i++, pFlags_ptr++) {
					if (!T1_FLAGS_GET_NORMAL(*pFlags_ptr)) {
						INT32 nSym = CMQDecoder::Decode(AC, pB, AGG);
						//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 1 i = %d, AC= %ld, nSym= %d"), i, (INT32)AC, nSym);
						if (!nSym) {
							continue;
						}
						int runlen=DecodeUNI(AC, pB);
						runlen=(runlen<<1)|DecodeUNI(AC, pB);

						switch(runlen) {
							case 0:	
								DecClnPassStepPartial(pFlags_ptr, 0, AC, pB, Contexts, bVCC);
								DecClnPassStep(pFlags_ptr, 1, AC, pB, Contexts);
								DecClnPassStep(pFlags_ptr, 2, AC, pB, Contexts);
								DecClnPassStep(pFlags_ptr, 3, AC, pB, Contexts);
								break;
							case 1:
								DecClnPassStepPartial(pFlags_ptr, 1, AC, pB, Contexts);
								DecClnPassStep(pFlags_ptr, 2, AC, pB, Contexts);
								DecClnPassStep(pFlags_ptr, 3, AC, pB, Contexts);
								break;
							case 2:
								DecClnPassStepPartial(pFlags_ptr, 2, AC, pB, Contexts);
								DecClnPassStep(pFlags_ptr, 3, AC, pB, Contexts);
								break;
							case 3:
								DecClnPassStepPartial(pFlags_ptr, 3, AC, pB, Contexts);
								break;
						}
						*pFlags_ptr |= (*pFlags_ptr << (16)) & (T1_REFINE_1 | T1_REFINE_2 | T1_REFINE_3 | T1_REFINE_4);
					} else {
						register T1_FLAG_TYPE flags = *pFlags_ptr; // pFlags[i];
						//ROW 0
						INT32 nSym;
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG|T1_VISIT) << (0)))) {// 0 = 0 * 3
							nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (0))]);
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 2 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 0 = 0 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 0);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 0, bVCC, nStride);
							}
						}
						//ROW 1
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG|T1_VISIT) << (3)))) {// 3= 1 * 3
							nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (3))]);
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 3 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 3= 1 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 1);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 1, bVCC, nStride);
							}
						}
						//ROW 2
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG|T1_VISIT) << (6)))) {// 6= 2 * 3
							nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (6))]);
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 4 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 6= 2 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 2);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 2, bVCC, nStride);
							}
						}
						//ROW 3
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG|T1_VISIT) << (9)))) {// 9= 3 * 3
							nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (9))]);
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 5 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 9 = 3*3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 3);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 3, bVCC, nStride);
							}
						}
						flags &= ~(T1_VISIT_1|T1_VISIT_2|T1_VISIT_3|T1_VISIT_4);
						*pFlags_ptr = flags | ((flags << (16)) & (T1_REFINE_1 | T1_REFINE_2 | T1_REFINE_3 | T1_REFINE_4));
					}
				}
				m_Contexts[T1_CTXNO_AGG] = AGG;
			}
#else // USE_NON_CDP
#ifdef USE_NON_CDP_FLAG
			UINT32 D;
			UINT32 AC_Non_CDP;
			{
				AC_Non_CDP = AC;
				D = AC_Non_CDP - 0x80000000; // 2147483648 = MAXUINT32/2	
				AC_Non_CDP -= D;
			}

			INT32 C;
			//INT32 A;
			INT32 D16;

			for(k=0; k<h2; k++) {
				
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1 + w, k + 1);
				pFlags_ptr -= w;
				INT32 nStride = m_Flags.GetStride();

				register State AGG = m_Contexts[T1_CTXNO_AGG];

				for (int i = -w; i<0; i++, pFlags_ptr++) {
					if (!T1_FLAGS_GET_NORMAL(*pFlags_ptr)) {

						// when Non CDP used
						if (i < -3 && !T1_FLAGS_GET_NORMAL(pFlags_ptr[3]) && (((INT8)AGG.NPS[2]) & 1) == 0 && (UINT16)(D) >= AGG.U.Qe.nQEval) {

//#define USE_SKIP4_1
#ifndef USE_SKIP4_1
#define USE_SKIP4_2
#endif

#ifdef USE_SKIP4_1
							if (1) {
								C = (UINT16)AC;
								A = AC - C;
								
								D16 = (A - 0x80000000) >> 16; // to 16 bit for comparison without flip of next 4 cols

								
								if (C < D16)
									D16 = C;
							}
#endif USE_SKIP4_1

#ifdef USE_SKIP4_2
							{
								C = (UINT16)AC; 
								D16 = D >> 16; // to 16 bit for comparison without flip of next 4 cols 
								D16 = C < D16 ? C : D16;
							}
#endif USE_SKIP4_2

#ifdef NO_USE // do not work
							// calculate the next forth col's D and AC and check if still 
							bool bCLessThanQe = (UINT16)(D) < AGG.U.Qe.nQEval; // D before change. OK
							if (!bCLessThanQe) {
#ifdef MQ_TAKE_AC_QEVAL
								D -= (AGG.U.nQEvalShift16 + AGG.U.Qe.nQEval) << 2;
#else
								D -= AGG.U.nQEvalShift16 << 2;
#endif
								
								if ((INT32)D >= 0) {
									// can skip 4 cols, this is a big speed jump
#ifndef MQ_TAKE_AC_QEVAL
									D -= AGG.U.Qe.nQEval << 2;
#endif
									//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 11 ii = %d, i = %d, AC= %ld, nSym= %d"), ii, i + ii, (INT32)(AC_TMP - (ii + 1)*(AGG.U.nQEvalShift16 + AGG.U.Qe.nQEval)), (INT8)AGG.NPS[2]);

									AC = AC_Non_CDP + D;
									pFlags_ptr += 3;
									i += 3;
									continue;
								}
#ifdef MQ_TAKE_AC_QEVAL
								D += (AGG.U.nQEvalShift16 + AGG.U.Qe.nQEval) << 2; // Put back the change.
#else
								D += AGG.U.nQEvalShift16 << 2; // Put back the change.
#endif
							}
#endif NO_USE

							// calculate the next forth col's D and AC and check if still 
							D16 -= (INT32)((AGG.U.Qe.nQEval) << 2); // OK
							if ((INT32)D16 >= 0) {
								D -= (AGG.U.nQEvalShift16 + AGG.U.Qe.nQEval) << 2;
								AC = AC_Non_CDP + D;
								pFlags_ptr += 3;
								i += 3;
								continue;
							}

							

#ifdef NO_USE // though it works, but still cannot skip 4

							UINT8 nSkip = 0;
							for (int ii = 0; ii < 4; ii++) {
								// If can skip 4 cols, this is a big speed jump
								// calculate the next forth col's D and AC and check if still 
						
								D32 -= AGG.U.nQEvalShift16;

								if ((INT32)D32 >= 0) { // OK									
									//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 11 ii = %d, i = %d, AC= %ld, nSym= %d"), ii, i + ii, (INT32)(AC_Non_CDP + D - (ii+1) * (AGG.U.nQEvalShift16 + AGG.U.Qe.nQEval)), (INT8)AGG.NPS[2]);

									nSkip++;
								}
								else {
									D32 += AGG.U.nQEvalShift16; // Put back the change.
									break;
								}
							}

							if (nSkip != 0 && nSkip != 4) {
								nSkip = nSkip; // 
								NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass failed to skip 4 cols"));
							}
							if (nSkip >= 1) {
								D -= nSkip * (AGG.U.nQEvalShift16 + AGG.U.Qe.nQEval);
								AC = AC_Non_CDP + D;
								pFlags_ptr += (nSkip-1); // leave 1 at next loop
								i += (nSkip-1); // leave 1 at next loop
								continue;
							}
#endif // NO_USE							
						}

						INT32 nSym = CMQDecoder::Decode(AC_Non_CDP, D, pB, AGG); 
						AC = AC_Non_CDP + D;
						//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 1 i = %d, AC= %ld, nSym= %d"), i, (INT32)AC, nSym);

						if (!nSym) {
							continue;
						}
						int runlen=DecodeUNI(AC, pB);
						runlen=(runlen<<1)|DecodeUNI(AC, pB);

						switch(runlen) {
						case 0:	
							DecClnPassStepPartial(pFlags_ptr, 0, AC, pB, Contexts, bVCC);
							DecClnPassStep(pFlags_ptr, 1, AC, pB, Contexts);
							DecClnPassStep(pFlags_ptr, 2, AC, pB, Contexts);
							DecClnPassStep(pFlags_ptr, 3, AC, pB, Contexts);
							break;
						case 1:
							DecClnPassStepPartial(pFlags_ptr, 1, AC, pB, Contexts);
							DecClnPassStep(pFlags_ptr, 2, AC, pB, Contexts);
							DecClnPassStep(pFlags_ptr, 3, AC, pB, Contexts);
							break;
						case 2:
							DecClnPassStepPartial(pFlags_ptr, 2, AC, pB, Contexts);
							DecClnPassStep(pFlags_ptr, 3, AC, pB, Contexts);
							break;
						case 3:
							DecClnPassStepPartial(pFlags_ptr, 3, AC, pB, Contexts);
							break;
						}

						{
							//update AC_Non_CDP
							UpdateAC_D(AC, AC_Non_CDP, D, AGG);
						}
						*pFlags_ptr |= (*pFlags_ptr << (16)) & (T1_REFINE_1 | T1_REFINE_2 | T1_REFINE_3 | T1_REFINE_4);
					} else {
						register T1_FLAG_TYPE flags = *pFlags_ptr; // pFlags[i];
						//ROW 0
						INT32 nSym;
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG|T1_VISIT) << (0)))) {// 0 = 0 * 3
							nSym = CMQDecoder::Decode(AC_Non_CDP, D, pB, Contexts[GetCtxNo_ZC(flags >> (0))]);
							AC = AC_Non_CDP + D;
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 2 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);

							if (nSym) {// 0 = 0 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 0);
								nSym = CMQDecoder::Decode(AC_Non_CDP, D, pB, Contexts[GetCtxNo_SC(f)]);
								AC = AC_Non_CDP + D;
								//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 22 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 0, bVCC, nStride);
							}
						}
						//ROW 1
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG|T1_VISIT) << (3)))) {// 3= 1 * 3
							nSym = CMQDecoder::Decode(AC_Non_CDP, D, pB, Contexts[GetCtxNo_ZC(flags >> (3))]);
							AC = AC_Non_CDP + D;
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 3 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 3= 1 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 1);
								nSym = CMQDecoder::Decode(AC_Non_CDP, D, pB, Contexts[GetCtxNo_SC(f)]);
								AC = AC_Non_CDP + D;
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 1, bVCC, nStride);
							}
						}
						//ROW 2
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG|T1_VISIT) << (6)))) {// 6= 2 * 3
							nSym = CMQDecoder::Decode(AC_Non_CDP, D, pB, Contexts[GetCtxNo_ZC(flags >> (6))]);
							AC = AC_Non_CDP + D;
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 4 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 6= 2 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 2);
								nSym = CMQDecoder::Decode(AC_Non_CDP, D, pB, Contexts[GetCtxNo_SC(f)]);
								AC = AC_Non_CDP + D;
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 2, bVCC, nStride);
							}
						}
						//ROW 3
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG|T1_VISIT) << (9)))) {// 9= 3 * 3
							nSym = CMQDecoder::Decode(AC_Non_CDP, D, pB, Contexts[GetCtxNo_ZC(flags >> (9))]);
							AC = AC_Non_CDP + D;
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 5 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 9 = 3*3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 3);
								nSym = CMQDecoder::Decode(AC_Non_CDP, D, pB, Contexts[GetCtxNo_SC(f)]);
								AC = AC_Non_CDP + D;
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 3, bVCC, nStride);
							}
						}
						flags &= ~(T1_VISIT_1|T1_VISIT_2|T1_VISIT_3|T1_VISIT_4);
						*pFlags_ptr = flags | ((flags << (16)) & (T1_REFINE_1 | T1_REFINE_2 | T1_REFINE_3 | T1_REFINE_4));
					}
				}
				m_Contexts[T1_CTXNO_AGG] = AGG;
			}

			// update AC
			{
				AC_Non_CDP += D;
				AC = AC_Non_CDP;
			}
#else // !USE_NON_CDP_FLAG
			// not to use Non CDP MQ

			INT32 C;
			INT32 D16;

			INT32 nSym;
			for (k = 0; k<h2; k++) {
				
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1 + w, k + 1);
				pFlags_ptr -= w;
				INT32 nStride = m_Flags.GetStride();

				register State AGG = m_Contexts[T1_CTXNO_AGG]; //  Is there a multi-threading issue as m_Contexts[T1_CTXNO_AGG] needs update below?

				for (int i = -w; i<0; i++, pFlags_ptr++) {
					if (!T1_FLAGS_GET_NORMAL(*pFlags_ptr)) {
						C = (UINT16)AC; // C 16 bit
						if (i < -3 && C >= AGG.U.Qe.nQEval && !T1_FLAGS_GET_NORMAL(pFlags_ptr[3]) && (((INT8)AGG.NPS[2]) & 1) == 0) {

							{
								D16 = (INT32)(AC >> 16) - T1_C15; // from D16 = D >> 16 = (AC - 0x80000000) >> 16. to 16 bit for comparison without flip of next 4 cols. OK	
								D16 = C < D16 ? C : D16;
							}

							// calculate the next forth col's D and AC and check if still 
							{
								
								D16 -= (AGG.nQEvalShift2); 
								if (D16 >= 0) {
									AC -= (AGG.nQEvalShift18 + AGG.nQEvalShift2);
									pFlags_ptr += 3;
									i += 3;
									continue;
								}
							}
							
						}

						nSym = CMQDecoder::Decode(AC, pB, AGG);
						if (!nSym) {
							continue;
						}
						int runlen = DecodeUNI(AC, pB);
						runlen = (runlen << 1) | DecodeUNI(AC, pB);

						switch (runlen) {
						case 0:
							DecClnPassStepPartial(pFlags_ptr, 0, AC, pB, Contexts, bVCC);
							DecClnPassStep(pFlags_ptr, 1, AC, pB, Contexts);
							DecClnPassStep(pFlags_ptr, 2, AC, pB, Contexts);
							DecClnPassStep(pFlags_ptr, 3, AC, pB, Contexts);
							break;
						case 1:
							DecClnPassStepPartial(pFlags_ptr, 1, AC, pB, Contexts);
							DecClnPassStep(pFlags_ptr, 2, AC, pB, Contexts);
							DecClnPassStep(pFlags_ptr, 3, AC, pB, Contexts);
							break;
						case 2:
							DecClnPassStepPartial(pFlags_ptr, 2, AC, pB, Contexts);
							DecClnPassStep(pFlags_ptr, 3, AC, pB, Contexts);
							break;
						case 3:
							DecClnPassStepPartial(pFlags_ptr, 3, AC, pB, Contexts);
							break;
						}
						*pFlags_ptr |= (*pFlags_ptr << (16)) & (T1_REFINE_1 | T1_REFINE_2 | T1_REFINE_3 | T1_REFINE_4);
					}
					else {
						register T1_FLAG_TYPE flags = *pFlags_ptr; // pFlags[i];
						//ROW 0
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG | T1_VISIT) << (0)))) {// 0 = 0 * 3
							nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (0))]);
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 2 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 0 = 0 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 0);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 0, bVCC, nStride);
							}
						}
						//ROW 1
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG | T1_VISIT) << (3)))) {// 3= 1 * 3
							nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (3))]);
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 3 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 3= 1 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 1);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 1, bVCC, nStride);
							}
						}
						//ROW 2
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG | T1_VISIT) << (6)))) {// 6= 2 * 3
							nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (6))]);
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 4 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 6= 2 * 3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 2);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 2, bVCC, nStride);
							}
						}
						//ROW 3
						if (!(T1_FLAGS_GET_NORMAL(flags)&((T1_SIG | T1_VISIT) << (9)))) {// 9= 3 * 3
							nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_ZC(flags >> (9))]);
							//NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_INFO_LEVEL, NCS_T("T_T1Decoder::DecClnPass 5 i = %d, AC= %ld, nSym= %d"), i, AC, nSym);
							if (nSym) {// 9 = 3*3
								UINT32 f = T1_SC_SHUFFLE(pFlags_ptr[-1], flags, pFlags_ptr[1], 3);
								nSym = CMQDecoder::Decode(AC, pB, Contexts[GetCtxNo_SC(f)]);
								T1_FLAG_TYPE v = nSym^GetSPB(f);
								UpdateFlags(flags, pFlags_ptr, v, 3, bVCC, nStride);
							}
						}
						flags &= ~(T1_VISIT_1 | T1_VISIT_2 | T1_VISIT_3 | T1_VISIT_4);
						*pFlags_ptr = flags | ((flags << (16)) & (T1_REFINE_1 | T1_REFINE_2 | T1_REFINE_3 | T1_REFINE_4));
					}
				}
				m_Contexts[T1_CTXNO_AGG] = AGG;
			}
#endif //USE_NON_CDP_FLAG

#endif // USE_NON_CDP

			for (k *= 4; k<h; k+=4) {
				int max_j = NCSMin(4, h - k);
				
				T1_FLAG_TYPE *pFlags_ptr = (T1_FLAG_TYPE*)m_Flags.GetPtr(1 + w, k / 4 + 1);
				pFlags_ptr -= w;

				for (int i = -w; i<0; i++, pFlags_ptr++) {

					
					{ 
						DecClnPassStep(pFlags_ptr, 0, AC, pB, Contexts, bVCC);
						for (int j = 1; j < max_j; j++) {
							DecClnPassStep(pFlags_ptr, j, AC, pB, Contexts);
						}
					}
					*pFlags_ptr &= ~(T1_VISIT_1 | T1_VISIT_2 | T1_VISIT_3 | T1_VISIT_4);
					*pFlags_ptr |= (*pFlags_ptr << (16)) & (T1_REFINE_1 | T1_REFINE_2 | T1_REFINE_3 | T1_REFINE_4);
				}
			}
			if (segsym) {
				int v=0;
				v=DecodeUNI(AC, pB);
				v=(v<<1)|DecodeUNI(AC, pB);
				v=(v<<1)|DecodeUNI(AC, pB);
				v=(v<<1)|DecodeUNI(AC, pB);
				if (v!=0xa) {//FIXME
					fprintf(stderr, "warning: bad segmentation symbol\n");
				}
			}

			m_AC = AC;
			m_pB = pB;
			memcpy(&m_Contexts[T1_CTXNO_ZC], &Contexts[T1_CTXNO_ZC], sizeof(State)*T1_NUMCTXS_ZC);
			memcpy(&m_Contexts[T1_CTXNO_SC], &Contexts[T1_CTXNO_SC], sizeof(State)*T1_NUMCTXS_SC);
		}

		// update AC_Non_CDP
		T1_INLINE void UpdateAC_D(register UINT32 &AC, register UINT32 &AC_Non_CDP, register UINT32 &D, State &S) {
			AC_Non_CDP = AC;
			D = AC_Non_CDP - 0x80000000; // 2147483648 = MAXUINT32/2
			AC_Non_CDP -= D;
		}

	T1_INLINE void UpdateFlags(T1_FLAG_TYPE *pFlags, T1_FLAG_TYPE v, int j, UINT8 bVCC) {
			UpdateFlags(pFlags[0], pFlags, v, j, bVCC, m_Flags.GetStride());
		}
	static T1_INLINE void UpdateFlags(T1_FLAG_TYPE &flags, T1_FLAG_TYPE *pFlags, T1_FLAG_TYPE v, int j, UINT8 bVCC, INT32 nStride) {
			switch(j) {
				case 0:
						if(!bVCC) {
							pFlags[-nStride-1] |= T1_SIG_25|T1_SIG_HVD64_4;
							pFlags[-nStride] |= T1_SIG_15|(v & T1_SGN_15)|T1_SIG_HVD64_4;
							pFlags[-nStride+1] |= T1_SIG_05|T1_SIG_HVD64_4;		
						}
						pFlags[-1] |= T1_SIG_21|(v & T1_SGN_21)|T1_SIG_HVD64_1|T1_SIG_HVD64_2;
						flags |= T1_SIG_1|(v & T1_SGN_1)|T1_VISIT_1|T1_SIG_HVD64_2;
						pFlags[+1] |= T1_SIG_01|(v & T1_SGN_01)|T1_SIG_HVD64_1|T1_SIG_HVD64_2;
					break;
				case 1:
						pFlags[-1] |= T1_SIG_22|(v & T1_SGN_22)|T1_SIG_HVD64_1|T1_SIG_HVD64_2|T1_SIG_HVD64_3;
						flags |= T1_SIG_2|(v & T1_SGN_2)|T1_VISIT_2|T1_SIG_HVD64_1|T1_SIG_HVD64_3;
						pFlags[+1] |= T1_SIG_02|(v & T1_SGN_02)|T1_SIG_HVD64_1|T1_SIG_HVD64_2|T1_SIG_HVD64_3;
					break;
				case 2:
						pFlags[-1] |= T1_SIG_23|(v & T1_SGN_23)|T1_SIG_HVD64_2|T1_SIG_HVD64_3|T1_SIG_HVD64_4;
						flags |= T1_SIG_3|(v & T1_SGN_3)|T1_VISIT_3|T1_SIG_HVD64_2|T1_SIG_HVD64_4;
						pFlags[+1] |= T1_SIG_03|(v & T1_SGN_03)|T1_SIG_HVD64_2|T1_SIG_HVD64_3|T1_SIG_HVD64_4;
					break;
				case 3:
						pFlags[-1] |= T1_SIG_24|(v & T1_SGN_24)|T1_SIG_HVD64_3|T1_SIG_HVD64_4;
						flags |= T1_SIG_4|(v & T1_SGN_4)|T1_VISIT_4|T1_SIG_HVD64_3;
						pFlags[+1] |= T1_SIG_04|(v & T1_SGN_04)|T1_SIG_HVD64_3|T1_SIG_HVD64_4;
						{
							pFlags[+nStride-1] |= T1_SIG_20|T1_SIG_HVD64_1;
							pFlags[+nStride] |= T1_SIG_10|(v & T1_SGN_10)|T1_SIG_HVD64_1;				
							pFlags[+nStride+1] |= T1_SIG_00|T1_SIG_HVD64_1;
						}
					break;
			}
		}
	T1_INLINE INT32 GetSPB(T1_FLAG_TYPE f) {
			return -sm_lut_spb[f];
		}
	T1_INLINE INT32 GetBit() {
			bool bBit;
			m_Stream.UnStuff(bBit);
			return((INT8)-((UINT8)bBit));
		}
};


	/**
	 * CT1Coder class - the JPC T1 Coder.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #15 $ $Author: ctapley $ $Date: 2014/03/14 $ 
	 */	
template <class T, int T1_SIGN_SHIFT, T T1_SIGN_MASK, T T1_VALUE_MASK> class NCSECW_IMPEX T_T1Encoder: public CJPCCoderLUTs, public CMQEncoder {
public:
	T_T1Encoder() {
			m_pEncTmpBuf = NULL;
			m_nSigValues = 0;
			m_bAllSig = false;
			m_nBitPlane = 0;
			m_Scb = 0;
		}

	virtual ~T_T1Encoder() {
			NCSFree(m_pEncTmpBuf);
		}

	bool Encode(JPC::CSubBand::Type eSBType, 
				UINT8 nBitsTotal,
				UINT8 &nZeroBits,
				std::vector<CSegment> &Segments,
				SDK::CBuffer2D *pDecBuf,
				UINT16 nRatio,
				UINT16 nLayers) {

			int passno=0;
			int passtype;
			T nMax = (sizeof(T) == sizeof(INT16)) ? (T)SDK::CBuffer2D::MININT16 : (T)((sizeof(T) == sizeof(INT32)) ? (T)SDK::CBuffer2D::MININT32 : (T)SDK::CBuffer2D::MININT64);

			UINT32 nWidth = pDecBuf->GetWidth();
			UINT32 nHeight = pDecBuf->GetHeight();

			m_nSigValues = 0;
			memset(m_EncFlags, 0, sizeof(m_EncFlags));
			//memset(m_EncData, 0, sizeof(m_EncData));

			INT32 nDecStep = pDecBuf->GetStride(); 
			if(pDecBuf->GetType() == SDK::CBuffer2D::BT_INT16) {
				if(nRatio == 1) {
					for(UINT32 y = 0; y < nHeight; y++) {
						INT16 *pSrc = (INT16*)pDecBuf->GetPtr() + y * nDecStep;
						T *pDst = (T*)&m_EncData[(y % 4) +  (y / 4) * CBSTEP];

						for(UINT32 x = 0; x < nWidth; x++) {
							INT16 v = pSrc[x];
							if(v < 0) {
								nMax=NCSMax(nMax, (T)(-v));
								pDst[x * 4] = (T)((-v) | T1_SIGN_MASK);
							} else {
								nMax=NCSMax(nMax, (T)v);
								pDst[x * 4] = (T)v;
							}
						}
					}
				} else {
					IEEE4 fRatio = 1.0f / nRatio;
#ifdef NCS_X86_MMI
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127)
#endif
					if(CSystemInfo::SSE2Present() && (nWidth % 8) == 0 && (nHeight % 4) == 0 && sizeof(T) == sizeof(INT16)) {
#ifdef _MSC_VER
#pragma warning(pop)
#endif
						UINT32 nSubWidth = nWidth / 8;

						const __m128i _mm_r = _mm_set1_epi16((INT16)(fRatio * 0xffff));
						__m128i mmMax = _mm_set1_epi16(0);
						const __m128i _mm_nRatio = _mm_set1_epi16((INT16)nRatio);
						const __m128i _mmMask0 = _mm_set1_epi16(-1);//0xffff
						const __m128i _mmMask1 = _mm_set1_epi16(-32768/*0x8000*/);
						const __m128i _mmMask2 = _mm_set1_epi16((short)0x0001);
						for(UINT32 y = 0; y < nHeight; y += 4) {
							INT16 *pSrc = (INT16*)((INT16*)pDecBuf->GetPtr() + y * nDecStep);
							T *pDst = (T*)&m_EncData[(y % 4) +  (y / 4) * CBSTEP];

							for(UINT32 x = 0; x < nSubWidth; x++) {
								__m128i mm0 = _mm_loadu_si128(&((__m128i*)pSrc)[x]);
								__m128i mm4 = _mm_max_epi16(mm0, _mm_mullo_epi16(mm0, _mmMask0));//mm4 = abs(mm0)
								__m128i mm_Temp = _mm_and_si128(_mm_srai_epi16(_mm_mullo_epi16(mm4 , _mm_r), 15), _mmMask2); //mm2 = (((abs(nsrc) * r) >>15) & 0x1)
								mm_Temp = _mm_add_epi16(_mm_mulhi_epi16(mm4, _mm_r), mm_Temp); // += ((abs(nsrc) * r) >> 16)
								mm4 = _mm_mullo_epi16(mm_Temp, _mm_nRatio); // *= nRatio
								mmMax = _mm_max_epi16(mm4, mmMax);
								mm4 = _mm_or_si128(mm4, _mm_and_si128(mm0, _mmMask1));// or in sign bits

								__m128i mm1 = _mm_loadu_si128(&((__m128i*)(pSrc + nDecStep))[x]);
								__m128i mm5 = _mm_max_epi16(mm1, _mm_mullo_epi16(mm1, _mmMask0));//mm5 = abs(mm1)
								mm_Temp = _mm_and_si128(_mm_srai_epi16(_mm_mullo_epi16(mm5 , _mm_r), 15), _mmMask2); //mm2 = (((abs(nsrc) * r) >>15) & 0x1)
								mm_Temp = _mm_add_epi16(_mm_mulhi_epi16(mm5, _mm_r), mm_Temp); // += ((abs(nsrc) * r) >> 16)
								mm5 = _mm_mullo_epi16(mm_Temp, _mm_nRatio); // *= nRatio
								mmMax = _mm_max_epi16(mm5, mmMax);
								mm5 = _mm_or_si128(mm5, _mm_and_si128(mm1, _mmMask1));// or in sign bits

								__m128i mm2 = _mm_loadu_si128(&((__m128i*)(pSrc + 2*nDecStep))[x]);
								__m128i mm6 = _mm_max_epi16(mm2, _mm_mullo_epi16(mm2, _mmMask0));//mm6 = abs(mm2)
								mm_Temp = _mm_and_si128(_mm_srai_epi16(_mm_mullo_epi16(mm6 , _mm_r), 15), _mmMask2); //mm2 = (((abs(nsrc) * r) >>15) & 0x1)
								mm_Temp = _mm_add_epi16(_mm_mulhi_epi16(mm6, _mm_r), mm_Temp); // += ((abs(nsrc) * r) >> 16)
								mm6 = _mm_mullo_epi16(mm_Temp, _mm_nRatio); // *= nRatio
								mmMax = _mm_max_epi16(mm6, mmMax);
								mm6 = _mm_or_si128(mm6, _mm_and_si128(mm2, _mmMask1));// or in sign bits

								__m128i mm3 = _mm_loadu_si128(&((__m128i*)(pSrc + 3*nDecStep))[x]);
								__m128i mm7 = _mm_max_epi16(mm3, _mm_mullo_epi16(mm3, _mmMask0));//mm7 = abs(mm3)
								mm_Temp = _mm_and_si128(_mm_srai_epi16(_mm_mullo_epi16(mm7 , _mm_r), 15), _mmMask2); //mm2 = (((abs(nsrc) * r) >>15) & 0x1)
								mm_Temp = _mm_add_epi16(_mm_mulhi_epi16(mm7, _mm_r), mm_Temp); // += ((abs(nsrc) * r) >> 16)
								mm7 = _mm_mullo_epi16(mm_Temp, _mm_nRatio); // *= nRatio
								mmMax = _mm_max_epi16(mm7, mmMax);
								mm7 = _mm_or_si128(mm7, _mm_and_si128(mm3, _mmMask1));// or in sign bits

								// have (quantized, sign coded):
								// mm4  [s7  s6  s5  s4  s3  s2  s1  s0]
								// mm5  [s15 s14 s13 s12 s11 s10 s9  s8]
								// mm6  [s23 s22 s21 s20 s19 s18 s17 s16]
								// mm7  [s31 s30 s29 s28 s27 s26 s25 s24]
								//
								// need:
								// [s25 s17 s9 s1 s24 s16 s8 s0][s27 s19 s11 s3 s26 s18 s10 s2]...
								//		...[s29 s21 s13 s5 s28 s20 s12 s4][s31 s23 s15 s7 s30 s22 s14 s6]
				
								__m128i mmTemp0 = _mm_unpacklo_epi16(mm4, mm5);				// [s11 s3 s10 s2 s9 s1 s8 s0]
								__m128i mmTemp1 = _mm_unpacklo_epi16(mm6, mm7);				// [s27 s19 s26 s18 s25 s17 s24 s16]
								_mm_store_si128(&((__m128i*)pDst)[x*4], _mm_unpacklo_epi32(mmTemp0, mmTemp1));	// [s25 s17 s9 s1 s24 s16 s8 s0]
								_mm_store_si128(&((__m128i*)pDst)[x*4+1], _mm_unpackhi_epi32(mmTemp0, mmTemp1)); // [s27 s19 s11 s3 s26 s18 s10 s2]
	
								mmTemp0 = _mm_unpackhi_epi16(mm4, mm5);							// [s15 s7 s14 s6 s13 s5 s12 s4]
								mmTemp1 = _mm_unpackhi_epi16(mm6, mm7);							// [s31 s23 s30 s22 s29 s21 s28 s20]
								_mm_store_si128(&((__m128i*)pDst)[x*4+2], _mm_unpacklo_epi32(mmTemp0, mmTemp1));	// [s29 s21 s13 s5 s28 s20 s12 s4]
								_mm_store_si128(&((__m128i*)pDst)[x*4+3], _mm_unpackhi_epi32(mmTemp0, mmTemp1));	// [s31 s23 s15 s7 s30 s22 s14 s6]
							}
						}
						nMax = NCSMax((INT16)_mm_extract_epi16(mmMax, 7), NCSMax((INT16)_mm_extract_epi16(mmMax, 6), NCSMax((INT16)_mm_extract_epi16(mmMax, 5), NCSMax((INT16)_mm_extract_epi16(mmMax, 4), NCSMax((INT16)_mm_extract_epi16(mmMax, 3), NCSMax((INT16)_mm_extract_epi16(mmMax, 2), NCSMax((INT16)_mm_extract_epi16(mmMax, 1), (INT16)_mm_extract_epi16(mmMax, 0))))))));
#ifdef NCS_X86_MMI_MMX
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4127)
#endif
					} else if(CSystemInfo::SSEPresent() && (nWidth % 4) == 0 && (nHeight % 4) == 0 && sizeof(T) == sizeof(INT16)) {
#ifdef _MSC_VER
#pragma warning(pop)
#endif
						UINT32 nSubWidth = nWidth / 4;
						const __m64 _mm_r = _mm_set1_pi16((INT16)(fRatio * 0xffff));
						__m64 mmMax = _mm_set1_pi16(0);
						const __m64 _mm_nRatio = _mm_set1_pi16((INT16)nRatio);
						const __m64 _mmMask0 = _mm_set1_pi16(-1);//0xffff
						const __m64 _mmMask1 = _mm_set1_pi16(-32768/*0x8000*/);
						const __m64 _mmMask2 = _mm_set1_pi16((short)0x0001);
						for(UINT32 y = 0; y < nHeight; y += 4) {
							INT16 *pSrc = (INT16*)((INT16*)pDecBuf->GetPtr() + y * nDecStep);
							T *pDst = (T*)&m_EncData[(y % 4) +  (y / 4) * CBSTEP];

							for(UINT32 x = 0; x < nSubWidth; x++) {
								__m64 mm0 = ((__m64*)pSrc)[x];
								__m64 mm4 = _mm_max_pi16(mm0, _mm_mullo_pi16(mm0, _mmMask0));//mm4 = abs(mm0)
								__m64 mm_Temp = _mm_and_si64(_mm_srai_pi16(_mm_mullo_pi16(mm4 , _mm_r), 15), _mmMask2); //mm2 = (((abs(nsrc) * r) >>15) & 0x1)
								mm_Temp = _mm_add_pi16(_mm_mulhi_pi16(mm4, _mm_r), mm_Temp); // += ((abs(nsrc) * r) >> 16)
								mm4 = _mm_mullo_pi16(mm_Temp, _mm_nRatio); // *= nRatio
								mmMax = _mm_max_pi16(mm4, mmMax);
								mm4 = _mm_or_si64(mm4, _mm_and_si64(mm0, _mmMask1));// or in sign bits
					

								__m64 mm1 = ((__m64*)(pSrc + nDecStep))[x];
								__m64 mm5 = _mm_max_pi16(mm1, _mm_mullo_pi16(mm1, _mmMask0));//mm5 = abs(mm1)
								mm_Temp = _mm_and_si64(_mm_srai_pi16(_mm_mullo_pi16(mm5 , _mm_r), 15), _mmMask2); //mm2 = (((abs(nsrc) * r) >>15) & 0x1)
								mm_Temp = _mm_add_pi16(_mm_mulhi_pi16(mm5, _mm_r), mm_Temp); // += ((abs(nsrc) * r) >> 16)
								mm5 = _mm_mullo_pi16(mm_Temp, _mm_nRatio); // *= nRatio
								mmMax = _mm_max_pi16(mm5, mmMax);
								mm5 = _mm_or_si64(mm5, _mm_and_si64(mm1, _mmMask1));// or in sign bits

								__m64 mm2 = ((__m64*)(pSrc + 2*nDecStep))[x];
								__m64 mm6 = _mm_max_pi16(mm2, _mm_mullo_pi16(mm2, _mmMask0));//mm6 = abs(mm2)
								mm_Temp = _mm_and_si64(_mm_srai_pi16(_mm_mullo_pi16(mm6 , _mm_r), 15), _mmMask2); //mm2 = (((abs(nsrc) * r) >>15) & 0x1)
								mm_Temp = _mm_add_pi16(_mm_mulhi_pi16(mm6, _mm_r), mm_Temp); // += ((abs(nsrc) * r) >> 16)
								mm6 = _mm_mullo_pi16(mm_Temp, _mm_nRatio); // *= nRatio
								mmMax = _mm_max_pi16(mm6, mmMax);
								mm6 = _mm_or_si64(mm6, _mm_and_si64(mm2, _mmMask1));// or in sign bits

								__m64 mm3 = ((__m64*)(pSrc + 3*nDecStep))[x];
								__m64 mm7 = _mm_max_pi16(mm3, _mm_mullo_pi16(mm3, _mmMask0));//mm7 = abs(mm3)
								mm_Temp = _mm_and_si64(_mm_srai_pi16(_mm_mullo_pi16(mm7 , _mm_r), 15), _mmMask2); //mm2 = (((abs(nsrc) * r) >>15) & 0x1)
								mm_Temp = _mm_add_pi16(_mm_mulhi_pi16(mm7, _mm_r), mm_Temp); // += ((abs(nsrc) * r) >> 16)
								mm7 = _mm_mullo_pi16(mm_Temp, _mm_nRatio); // *= nRatio
								mmMax = _mm_max_pi16(mm7, mmMax);
								mm7 = _mm_or_si64(mm7, _mm_and_si64(mm3, _mmMask1));// or in sign bits

								// have (quantized, sign coded):
								// mm4  [s3  s2  s1  s0]
								// mm5  [s7  s6  s5  s4]
								// mm6  [s11 s10 s9  s8]
								// mm7  [s15 s14 s13 s12]
								//
								// need:
								// [s12 s8 s4 s0][s13 s9 s5 s1][s14 s10 s6 s2][s15 s11 s7 s3]
								
								__m64 mmTemp0 = _mm_unpacklo_pi16(mm4, mm5);				// [s5 s1 s4 s0]
								__m64 mmTemp1 = _mm_unpacklo_pi16(mm6, mm7);				// [s13 s9 s12 s8]
								((__m64*)pDst)[x*4] = _mm_unpacklo_pi32(mmTemp0, mmTemp1);	// [s12 s8 s4 s0]
								((__m64*)pDst)[x*4+1] = _mm_unpackhi_pi32(mmTemp0, mmTemp1); // [s13 s9 s5 s1]

								mmTemp0 = _mm_unpackhi_pi16(mm4, mm5);							// [s7 s3 s6 s2]
								mmTemp1 = _mm_unpackhi_pi16(mm6, mm7);							// [s15 s11 s14 s10]
								((__m64*)pDst)[x*4+2] = _mm_unpacklo_pi32(mmTemp0, mmTemp1);	// [s14 s10 s6 s2]
								((__m64*)pDst)[x*4+3] = _mm_unpackhi_pi32(mmTemp0, mmTemp1);	// [s15 s11 s7 s3]
							}
						}
						nMax = NCSMax((INT16)_mm_extract_pi16(mmMax, 3), NCSMax((INT16)_mm_extract_pi16(mmMax, 2), NCSMax((INT16)_mm_extract_pi16(mmMax, 1), (INT16)_mm_extract_pi16(mmMax, 0))));
						_mm_empty();
#endif //NCS_X86_MMI_MMX
					} else {
#endif //NCS_X86_MMI

						for(UINT32 y = 0; y < nHeight; y++) {
							INT16 *pSrc = (INT16*)pDecBuf->GetPtr() + y * nDecStep;
							T *pDst = (T*)&m_EncData[(y % 4) +  (y / 4) * CBSTEP];
							INT32 r = (INT32)(fRatio * 0xffff);

							for(UINT32 x = 0; x < nWidth; x++) {
								register INT16 nSrc = pSrc[x];
								register INT32 vAbs = (T1_ABS(nSrc) * r);
								vAbs = nRatio * ((vAbs >> 16) + ((vAbs >>15) & 0x1));// (((vAbs2+1) >> 15) & 0x1));
								pDst[x * 4] = (T)vAbs | (((T)nSrc & 0x8000) << (T1_SIGN_SHIFT - 15));
								nMax = NCSMax(nMax, (INT16)vAbs);
							}
						}
				//		FLT_TO_INT_FINI();
#ifdef NCS_X86_MMI
					}
#endif //NCS_X86_MMI
				}
			} else if(pDecBuf->GetType() == SDK::CBuffer2D::BT_INT32) {
				if(nRatio == 1) {
					for(UINT32 y = 0; y < nHeight; y++) {
						INT32 *pSrc = (INT32*)pDecBuf->GetPtr() + y * nDecStep;
						T *pDst = (T*)&m_EncData[(y % 4) +  (y / 4) * CBSTEP];
						
						for(UINT32 x = 0; x < nWidth; x++) {
							INT32 v = pSrc[x];
							if(v < 0) {
								nMax=NCSMax(nMax, (T)(-v));
								pDst[x * 4] = (T)((-v) | T1_SIGN_MASK);
							} else {
								nMax=NCSMax(nMax, (T)v);
								pDst[x * 4] = (T)v;
							}
						}
					}
				} else {
					
					IEEE4 fRatio = 1.0f / nRatio;
					IEEE4 fV;
					INT32 v;

					NCS_FLT_ROUNDING_MODE_INIT(NCS_FLT_ROUNDING_MODE_NEAREST);
					for(UINT32 y = 0; y < nHeight; y++) {
						INT32 *pSrc = (INT32*)pDecBuf->GetPtr() + y * nDecStep;
						T *pDst = (T*)&m_EncData[(y % 4) +  (y / 4) * CBSTEP];
						
						for(UINT32 x = 0; x < nWidth; x++) {
							fV = pSrc[x] * fRatio;
							FLT_TO_INT32(v, fV);
							v *= nRatio;

							if(v < 0) {
								nMax = NCSMax(nMax, (T)-v);
								pDst[x * 4] = (T)((-v) | T1_SIGN_MASK);
							} else {
								nMax=NCSMax(nMax, (T)v);
								pDst[x * 4] = (T)v;
							}
						}
					}
					NCS_FLT_ROUNDING_MODE_FINI();
				}
			} else {
				if(nRatio == 1) {
					for(UINT32 y = 0; y < nHeight; y++) {
						INT64 *pSrc = (INT64*)pDecBuf->GetPtr() + y * nDecStep;
						T *pDst = (T*)&m_EncData[(y % 4) +  (y / 4) * CBSTEP];
						
						for(UINT32 x = 0; x < nWidth; x++) {
							T v = (T)pSrc[x];
							if(v < 0) {
								nMax=NCSMax(nMax, (-v));
								pDst[x * 4] = (-v) | T1_SIGN_MASK;
							} else {
								nMax=NCSMax(nMax, v);
								pDst[x * 4] = v;
							}
						}
					}
				} else {
					IEEE8 dRatio = 1.0 / nRatio;

					NCS_FLT_ROUNDING_MODE_INIT(NCS_FLT_ROUNDING_MODE_NEAREST);
					for(UINT32 y = 0; y < nHeight; y++) {
						INT64 *pSrc = (INT64*)pDecBuf->GetPtr() + y * nDecStep;
						T *pDst = (T*)&m_EncData[(y % 4) +  (y / 4) * CBSTEP];
						
						for(UINT32 x = 0; x < nWidth; x++) {
							T v = (T)(nRatio * NCSdoubleToInt64_RM(pSrc[x] * dRatio));
							if(v < 0) {
								nMax = NCSMax(nMax, -v);
								pDst[x * 4] = (-v) | T1_SIGN_MASK;
							} else {
								nMax=NCSMax(nMax, v);
								pDst[x * 4] = v;
							}
						}
					}
					NCS_FLT_ROUNDING_MODE_FINI();
				}
			}
			UINT8 nBits = (UINT8)(nMax ? (NCSLog2_64(nMax) + 1) : 0);
			nZeroBits = nBitsTotal - nBits;

			m_nBitPlane = nBits - 1;
			passtype=2;

			CMQCoder::ResetStates();

			int orient = JPC::CSubBand::GetOrient(eSBType);

			switch(orient) {
				case 0: m_plut_ctxno_zc = sm_lut_ctxno_zc0; break;
				case 1: m_plut_ctxno_zc = sm_lut_ctxno_zc1; break;
				case 2: m_plut_ctxno_zc = sm_lut_ctxno_zc2; break;
				case 3: m_plut_ctxno_zc = sm_lut_ctxno_zc3; break;
			}
			if(!m_pEncTmpBuf) {
				m_pEncTmpBuf = (UINT8*)NCSMalloc(T1_ENCODER_BUFSIZE, TRUE);
			}
			m_bAllSig = false;
				// Num passes per layer
			IEEE4 fSegPasses = (m_nBitPlane * 3 + 1) / (IEEE4)nLayers;
			IEEE4 fTotalSegPasses = 0.0f;

			Segments.resize(nLayers);
			UINT16 nPassesOutput = 0;
			UINT32 nSegOffset = 0;
			int nLayer;
			CMQEncoder::Init(m_pEncTmpBuf, T1_ENCODER_BUFSIZE);
					
			for(nLayer = 0; nLayer < nLayers; nLayer++) {
				Segments[nLayer].m_nLength = 0;	
				Segments[nLayer].m_pData = NULL;
				Segments[nLayer].m_nPasses = 0;

				fTotalSegPasses += fSegPasses;
				UINT16 nThisSegPasses = (UINT16)NCSFloor(fTotalSegPasses - nPassesOutput);

				if(nLayer == nLayers - 1) {
					// Last layer, make sure we output all remaining passes!
					nThisSegPasses = (nBits - 1) * 3 + 1 - nPassesOutput;
				}
				if(nThisSegPasses >= 1) {
					for (int nSegPass = 0; m_nBitPlane >= 0 && nSegPass < nThisSegPasses; passno++, nSegPass++) {
						m_one = ((T)1) << m_nBitPlane;
						switch (passtype) {
							case 0:
									if(!m_bAllSig) {
										EncSigPass(nWidth, nHeight);
									}
								break;
							case 1:
									EncRefPass(nWidth, nHeight);
								break;
							case 2:
									if(!m_bAllSig) {
										EncClnPass(nWidth, nHeight);
					//FIXME					m_bAllSig = (m_nSigValues == nWidth * nHeight);
									}
								break;
						}
						if (++passtype==3) {
							passtype=0;
							m_nBitPlane--;
						}
					}
				}
				if((nLayer == nLayers - 1) || (passno == (nBits - 1) * 3 + 1)) {
					CMQEncoder::Flush();
				}
				Segments[nLayer].m_nLength = NCSMax(0, (T1_ENCODER_BUFSIZE + CMQEncoder::m_nIndex - 1) - (signed)nSegOffset);	
				if(Segments[nLayer].m_nLength || (passno - nPassesOutput)) {						
					if((nLayer < nLayers - 1)) {
						// This is so-called "Easy Termination", not always smallest length but fast to calculate
						if(m_pEncTmpBuf[nSegOffset + Segments[nLayer].m_nLength] == 0xff) {
							//Don't split escaped data 
							Segments[nLayer].m_nLength += 1;
						}
						if(CMQEncoder::m_nCT >= 5) {
							Segments[nLayer].m_nLength += 4;
						} else {
							Segments[nLayer].m_nLength += 5;
						} 
					}
					nSegOffset += Segments[nLayer].m_nLength;
					Segments[nLayer].m_nPasses = (UINT8)(passno - nPassesOutput);
					nPassesOutput = (UINT16)passno;
				}
			}
			nSegOffset= 0;
			for(nLayer = 0; nLayer < nLayers; nLayer++) {
			//	int nSegLen = Segments[nLayer].m_nLength;
				if(nLayer == nLayers - 1) {
					// Adjust final length to end of MQ coder buffer
					Segments[nLayer].m_nLength = (T1_ENCODER_BUFSIZE + CMQEncoder::m_nIndex - 1) - nSegOffset;
				}
				while(Segments[nLayer].m_nLength > 0 && ((m_pEncTmpBuf[nSegOffset + Segments[nLayer].m_nLength/*-1*/] == 0xff) || ((INT32)(nSegOffset + Segments[nLayer].m_nLength) > T1_ENCODER_BUFSIZE + CMQEncoder::m_nIndex - 1))) {
					Segments[nLayer].m_nLength--;
				}
				Segments[nLayer].m_pData = (UINT8*)NCSMalloc(Segments[nLayer].m_nLength, FALSE);
				memcpy(Segments[nLayer].m_pData, m_pEncTmpBuf+1 + nSegOffset, Segments[nLayer].m_nLength);
				nSegOffset += Segments[nLayer].m_nLength;
			}
			return(true);
		}

private:
	UINT32 m_nSigValues;
	bool m_bAllSig;
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4324)
#endif
#ifdef NCS_X86_MMI
	__m128i __force_alignment;
#endif
#ifdef _WIN32
	__declspec(align(16)) T m_EncData[(NCSJPC_ENCODE_BLOCK_SIZE) * 4 * (0 + NCSJPC_ENCODE_BLOCK_SIZE / 4)];
	__declspec(align(16)) T1_FLAG_TYPE m_EncFlags[(NCSJPC_ENCODE_BLOCK_SIZE+8) * (NCSJPC_ENCODE_BLOCK_SIZE+2)];
#else
        __attribute__((aligned(16))) T m_EncData[(NCSJPC_ENCODE_BLOCK_SIZE) * 4 * (0 + NCSJPC_ENCODE_BLOCK_SIZE / 4)];
        __attribute__((aligned(16))) T1_FLAG_TYPE m_EncFlags[(NCSJPC_ENCODE_BLOCK_SIZE+8) * (NCSJPC_ENCODE_BLOCK_SIZE+2)];
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

 
	UINT8 *m_pEncTmpBuf;
	INT8 m_nBitPlane;
	UINT8 m_Scb;
	T m_one;

	T1_INLINE void EncSigPassStep(T1_FLAG_TYPE *fp, T *dp, int j) {
			if ((*fp&(T1_SIG_HVD << (j*3))) && !(*fp&((T1_SIG|T1_VISIT) << (j*3)))) {
				if(*dp&m_one) {
					CMQEncoder::Encode(1, GetCtxNo_ZC(*fp >> (j*3)));
					UINT32 f = T1_SC_SHUFFLE(fp[-1], fp[0], fp[1], j);
					if(*dp & T1_SIGN_MASK) {
						CMQEncoder::Encode(1^GetSPB(f), GetCtxNo_SC(f));
						UpdateFlags(fp, 1, j);
					} else {
						CMQEncoder::Encode(0^GetSPB(f), GetCtxNo_SC(f));
						UpdateFlags(fp, 0, j);
					}
				} else {
					CMQEncoder::Encode(0, GetCtxNo_ZC(*fp >> (j*3)));
				}
				*fp|=T1_VISIT << (j*3);
			}
		}
	void EncSigPass(int w, int h) {
			int i, j, k;
			T one = m_one;
			for (k=0; k<h; k+=4) {
				int max_j = NCSMin(4, h - k);
				T *pData = (T*)&m_EncData[(k >> 2) * CBSTEP];
				T1_FLAG_TYPE *pFlags = (T1_FLAG_TYPE*)&m_EncFlags[4 + (k/4+1)*FLAG_STEP];

				if(max_j == 4) {
					register UINT16 A = m_A;
					register INT32 C = m_C;

					for (i=0; i<w; i++) {
						register T1_FLAG_TYPE flags = pFlags[i];

						if(flags != 0) {
							//ROW 0
							if((flags&(T1_SIG_HVD << (0))) && !(flags&((T1_SIG/*|T1_VISIT*/) << (0)))) {// 0 = 0 * 3
								if(pData[i*4+0]&one) {
									CMQEncoder::Encode(A, C, 1, GetCtxNo_ZC(flags >> (0)));// 0 = 0 * 3
									UINT32 f = T1_SC_SHUFFLE(pFlags[i-1], flags, pFlags[i+1], 0);
									UINT32 v = (UINT32)((pData[i*4+0] >> T1_SIGN_SHIFT) & 0x1);//& T1_SIGN_MASK;
									CMQEncoder::Encode(A, C, v^GetSPB(f), GetCtxNo_SC(f));
									pFlags[i-FLAG_STEP-1] |= T1_SIG_25;
									pFlags[i-FLAG_STEP] |= T1_SIG_15|(v << 31);
									pFlags[i-FLAG_STEP+1] |= T1_SIG_05;		
									pFlags[i-1] |= T1_SIG_21 << (0);// 0 = 0 * 3
									flags |= (T1_SIG|(v << 19)|T1_VISIT) << (0);// 0 = 0 * 3
									pFlags[i+1] |= T1_SIG_01 << (0);// 0 = 0 * 3
									m_nSigValues++;
								} else {
									CMQEncoder::Encode(A, C, 0, GetCtxNo_ZC(flags >> (0)));// 0 = 0 * 3
									flags |= T1_VISIT << (0);// 0 = 0 * 3
								}
							}
							//ROW 1
							if((flags&(T1_SIG_HVD << (3))) && !(flags&((T1_SIG/*|T1_VISIT*/) << (3)))) { // 3= 1 * 3
								if(pData[i*4+1]&one) {
									CMQEncoder::Encode(A, C, 1, GetCtxNo_ZC(flags >> (3)));// 3= 1 * 3
									UINT32 f = T1_SC_SHUFFLE(pFlags[i-1], flags, pFlags[i+1], 1);
									UINT32 v = (UINT32)((pData[i*4+1] >> T1_SIGN_SHIFT) & 0x1);//& T1_SIGN_MASK;
									CMQEncoder::Encode(A, C, v^GetSPB(f), GetCtxNo_SC(f));
									pFlags[i-1] |= T1_SIG_21 << (3);// 3= 1 * 3
									flags |= (T1_SIG|(v << 19)|T1_VISIT) << (3);// 3= 1 * 3
									pFlags[i+1] |= T1_SIG_01 << (3);// 3= 1 * 3
									m_nSigValues++;
								} else {
									CMQEncoder::Encode(A, C, 0, GetCtxNo_ZC(flags >> (3)));// 3= 1 * 3
									flags |= T1_VISIT << (3);// 3= 1 * 3
								}
							}
							//ROW 2
							if((flags&(T1_SIG_HVD << (6))) && !(flags&((T1_SIG/*|T1_VISIT*/) << (6)))) {// 6= 2 * 3
								if(pData[i*4+2]&one) {
									CMQEncoder::Encode(A, C, 1, GetCtxNo_ZC(flags >> (6)));// 6= 2 * 3
									UINT32 f = T1_SC_SHUFFLE(pFlags[i-1], flags, pFlags[i+1], 2);
									UINT32 v = (UINT32)((pData[i*4+2] >> T1_SIGN_SHIFT) & 0x1);//& T1_SIGN_MASK;
									CMQEncoder::Encode(A, C, v^GetSPB(f), GetCtxNo_SC(f));
									pFlags[i-1] |= T1_SIG_21 << (6);// 6= 2 * 3
									flags |= (T1_SIG|(v << 19)|T1_VISIT) << (6);// 6= 2 * 3
									pFlags[i+1] |= T1_SIG_01 << (6);// 6= 2 * 3
									m_nSigValues++;
								} else {
									CMQEncoder::Encode(A, C, 0, GetCtxNo_ZC(flags >> (6)));// 6= 2 * 3
									flags |= T1_VISIT << (6);// 6= 2 * 3
								}
							}
							//ROW 3
							if((flags&(T1_SIG_HVD << (9))) && !(flags&((T1_SIG/*|T1_VISIT*/) << (9)))) { // 9 = 3*3
								if(pData[i*4+3]&one) {
									CMQEncoder::Encode(A, C, 1, GetCtxNo_ZC(flags >> (9)));// 9 = 3*3
									UINT32 f = T1_SC_SHUFFLE(pFlags[i-1], flags, pFlags[i+1], 3);
									UINT32 v = (UINT32)((pData[i*4+3] >> T1_SIGN_SHIFT) & 0x1);//& T1_SIGN_MASK;
									CMQEncoder::Encode(A, C, v^GetSPB(f), GetCtxNo_SC(f));
									pFlags[i-1] |= T1_SIG_21 << (9);// 9 = 3*3
									flags |= (T1_SIG|(v << 19)|T1_VISIT) << (9);// 9 = 3*3
									pFlags[i+1] |= T1_SIG_01 << (9);// 9 = 3*3
									pFlags[i+FLAG_STEP-1] |= T1_SIG_20;
									pFlags[i+FLAG_STEP] |= T1_SIG_10|(v << 18);				
									pFlags[i+FLAG_STEP+1] |= T1_SIG_00;	
									m_nSigValues++;
								} else {
									CMQEncoder::Encode(A, C, 0, GetCtxNo_ZC(flags >> (9)));// 9 = 3*3
									flags |= T1_VISIT << (9);// 9 = 3*3
								}
							}
							pFlags[i] = flags;
						}
					}
					m_A = A;
					m_C = C;
				} else {
					for (i=0; i<w; i++) {
						for (j=0; j<max_j; j++) {
							EncSigPassStep(pFlags + i, pData + i*4 + j, j);
						}
					}
				}
			}
		}
	T1_INLINE void EncRefPassStep(T1_FLAG_TYPE *fp, T *dp, int j) {
			if(*fp&(T1_REFINE << (j*3))) {
				UINT8 nCtx = T1_CTXNO_MAG+2;
				T t = (*dp & T1_VALUE_MASK) >> m_nBitPlane;
				if(t < 4) {
					nCtx = T1_CTXNO_MAG+(UINT8)((*fp&(T1_SIG_HVD << (j*3))) != 0);
				}
				CMQEncoder::Encode((UINT32)t & 0x1, nCtx);
			}
		}

	T1_INLINE void EncRefPassStepSIG(T1_FLAG_TYPE *fp, T *dp, int j) {
			UINT8 nCtx = T1_CTXNO_MAG+2;
			T t = (*dp & T1_VALUE_MASK) >> m_nBitPlane;
			if(t < 8) {
				nCtx = T1_CTXNO_MAG+(UINT8)((*fp&(T1_SIG_HVD << (j*3))) != 0);
			}
			CMQEncoder::Encode((UINT32)t & 0x1, nCtx);
		}
	void EncRefPass(int w, int h) {
			int i, j, k;
			UINT8 nShift = m_nBitPlane;
			for (k=0; k<h; k+=4) {
				int max_j = NCSMin(4, h - k);

				T *pData = (T*)&m_EncData[(k >> 2) * CBSTEP];
				T1_FLAG_TYPE *pFlags = (T1_FLAG_TYPE*)&m_EncFlags[4 + (k/4+1)*FLAG_STEP];

				if(max_j == 4) {
					if(m_bAllSig) {
						for (i=0; i<w; i++) {
							EncRefPassStepSIG(pFlags + i, pData + i*4 + 0, 0);
							EncRefPassStepSIG(pFlags + i, pData + i*4 + 1, 1);
							EncRefPassStepSIG(pFlags + i, pData + i*4 + 2, 2);
							EncRefPassStepSIG(pFlags + i, pData + i*4 + 3, 3);
						}
					} else {
						register UINT16 A = m_A;
						register INT32 C = m_C;

						for (i=0; i<w; i++) {
							register T1_FLAG_TYPE flags = pFlags[i];
							if(flags&(T1_REFINE_1|T1_REFINE_2|T1_REFINE_3|T1_REFINE_4)) {
								if(flags&(T1_REFINE << (0))) { // 0 = 0 * 3
									UINT8 nCtx = T1_CTXNO_MAG+2;
									T t = (pData[i*4 + 0] & T1_VALUE_MASK) >> nShift;
									if(t < 4) {
										nCtx = T1_CTXNO_MAG+(UINT8)((flags&(T1_SIG_HVD << (0))) != 0);// 0 = 0 * 3
									}
									CMQEncoder::Encode(A, C, (UINT32)t & 0x1, nCtx);
								}
								if(flags&(T1_REFINE << (3))) {// 3= 1 * 3
									UINT8 nCtx = T1_CTXNO_MAG+2;
									T t = (pData[i*4 + 1] & T1_VALUE_MASK) >> nShift;
									if(t < 4) {
										nCtx = T1_CTXNO_MAG+(UINT8)((flags&(T1_SIG_HVD << (3))) != 0);// 3= 1 * 3
									}
									CMQEncoder::Encode(A, C, (UINT32)t & 0x1, nCtx);
								}
								if(flags&(T1_REFINE << (6))) {// 6= 2 * 3
									UINT8 nCtx = T1_CTXNO_MAG+2;
									T t = (pData[i*4 + 2] & T1_VALUE_MASK) >> nShift;
									if(t < 4) {
										nCtx = T1_CTXNO_MAG+(UINT8)((flags&(T1_SIG_HVD << (6))) != 0);// 6= 2 * 3
									}
									CMQEncoder::Encode(A, C, (UINT32)t & 0x1, nCtx);
								}
								if(flags&(T1_REFINE << (9))) {// 9 = 3*3
									UINT8 nCtx = T1_CTXNO_MAG+2;
									T t = (pData[i*4 + 3] & T1_VALUE_MASK) >> nShift;
									if(t < 4) {
										nCtx = T1_CTXNO_MAG+(UINT8)((flags&(T1_SIG_HVD << (9))) != 0);// 9 = 3*3
									}
									CMQEncoder::Encode(A, C, (UINT32)t & 0x1, nCtx);
								}
							}
						}
						m_A = A;
						m_C = C;
					}
				} else {
					for (i=0; i<w; i++) {
						for (j=0; j<max_j; j++) {
							EncRefPassStep(pFlags + i, pData + i*4 + j, j);
						}
					}
				}
			}
		}

	T1_INLINE void EncClnPassStep(T1_FLAG_TYPE *fp, T *dp, int j) {
			if (!(*fp&((T1_SIG|T1_VISIT) << (j*3)))) {
				if (*dp&m_one) {
					CMQEncoder::Encode(1, GetCtxNo_ZC(*fp >>(j*3)));
					UINT32 f = T1_SC_SHUFFLE(fp[-1], fp[0], fp[1], j);
					if(*dp & T1_SIGN_MASK) {//FIXME - can we mod SPB to precode sign bit?
						CMQEncoder::Encode(1^GetSPB(f), GetCtxNo_SC(f));
						UpdateFlags(fp, 1, j);
					} else {
						CMQEncoder::Encode(0^GetSPB(f), GetCtxNo_SC(f));
						UpdateFlags(fp, 0, j);
					}
				} else {
					CMQEncoder::Encode(0, GetCtxNo_ZC(*fp >> (j*3)));
				}
			}
		}
	T1_INLINE void EncClnPassStepPartial(T1_FLAG_TYPE *fp, T *dp, int j) {
			UINT32 f = T1_SC_SHUFFLE(fp[-1], fp[0], fp[1], j);
			if(*dp & T1_SIGN_MASK) {
				CMQEncoder::Encode(1^GetSPB(f), GetCtxNo_SC(f));
				UpdateFlags(fp, 1, j);
			} else {
				CMQEncoder::Encode(0^GetSPB(f), GetCtxNo_SC(f));
				UpdateFlags(fp, 0, j);
			}
		}
	void EncClnPass(int w, int h) {
			int i, j, k;
			INT8 nShift = m_nBitPlane;
			T one = m_one;
			for (k=0; k<h; k+=4) {
				int max_j = NCSMin(4, h - k);

				T *pData = (T*)&m_EncData[(k >> 2) * CBSTEP];
				T1_FLAG_TYPE *pFlags = (T1_FLAG_TYPE*)&m_EncFlags[4 + (k/4+1)*FLAG_STEP];

				if (max_j == 4) {
					register UINT16 A = m_A;
					register INT32 C = m_C;

					for (i=0; i<w; i++) {
						register T1_FLAG_TYPE flags = pFlags[i];
						if(flags) {
							//ROW 0
							if (!(flags&((T1_SIG|T1_VISIT) << (0)))) {// 0 = 0 * 3
								if(pData[i*4+0]&one) {
									CMQEncoder::Encode(A, C, 1, GetCtxNo_ZC(flags >> (0)));// 0 = 0 * 3
									UINT32 f = T1_SC_SHUFFLE(pFlags[i-1], flags, pFlags[i+1], 0);
									UINT32 v = (UINT32)((pData[i*4+0] >> T1_SIGN_SHIFT) & 0x1);//& T1_SIGN_MASK;
									CMQEncoder::Encode(A, C, v^GetSPB(f), GetCtxNo_SC(f));
									pFlags[i-FLAG_STEP-1] |= T1_SIG_25;
									pFlags[i-FLAG_STEP] |= T1_SIG_15|(v << 31);
									pFlags[i-FLAG_STEP+1] |= T1_SIG_05;		
									pFlags[i-1] |= T1_SIG_21 << (0);// 0 = 0 * 3
									flags |= (T1_SIG|(v << 19)) << (0);// 0 = 0 * 3
									pFlags[i+1] |= T1_SIG_01 << (0);// 0 = 0 * 3
									m_nSigValues++;
								} else {
									CMQEncoder::Encode(A, C, 0, GetCtxNo_ZC(flags >> (0)));// 0 = 0 * 3
								}
							}
							//ROW 1
							if (!(flags&((T1_SIG|T1_VISIT) << (3)))) {// 3= 1 * 3
								if(pData[i*4+1]&one) {
									CMQEncoder::Encode(A, C, 1, GetCtxNo_ZC(flags >> (3)));// 3= 1 * 3
									UINT32 f = T1_SC_SHUFFLE(pFlags[i-1], flags, pFlags[i+1], 1);
									UINT32 v = (UINT32)((pData[i*4+1] >> T1_SIGN_SHIFT) & 0x1);//& T1_SIGN_MASK;
									CMQEncoder::Encode(A, C, v^GetSPB(f), GetCtxNo_SC(f));
									pFlags[i-1] |= T1_SIG_21 << (3);// 3= 1 * 3
									flags |= (T1_SIG|(v << 19)) << (3);// 3= 1 * 3
									pFlags[i+1] |= T1_SIG_01 << (3);// 3= 1 * 3
									m_nSigValues++;
								} else {
									CMQEncoder::Encode(A, C, 0, GetCtxNo_ZC(flags >> (3)));// 3= 1 * 3
								}
							}
							//ROW 2
							if (!(flags&((T1_SIG|T1_VISIT) << (6)))) {// 6= 2 * 3
								if(pData[i*4+2]&one) {
									CMQEncoder::Encode(A, C, 1, GetCtxNo_ZC(flags >> (6)));// 6= 2 * 3
									UINT32 f = T1_SC_SHUFFLE(pFlags[i-1], flags, pFlags[i+1], 2);
									UINT32 v = (UINT32)((pData[i*4+2] >> T1_SIGN_SHIFT) & 0x1);//& T1_SIGN_MASK;
									CMQEncoder::Encode(A, C, v^GetSPB(f), GetCtxNo_SC(f));
									pFlags[i-1] |= T1_SIG_21 << (6);// 6= 2 * 3
									flags |= (T1_SIG|(v << 19)) << (6);// 6= 2 * 3
									pFlags[i+1] |= T1_SIG_01 << (6);// 6= 2 * 3
									m_nSigValues++;
								} else {
									CMQEncoder::Encode(A, C, 0, GetCtxNo_ZC(flags >> (6)));// 6= 2 * 3
								}
							}
							//ROW 3
							if (!(flags&((T1_SIG|T1_VISIT) << (9)))) {// 9 = 3*3
								if(pData[i*4+3]&one) {
									CMQEncoder::Encode(A, C, 1, GetCtxNo_ZC(flags >> (9)));// 9 = 3*3
									UINT32 f = T1_SC_SHUFFLE(pFlags[i-1], flags, pFlags[i+1], 3);
									UINT32 v = (UINT32)((pData[i*4+3] >> T1_SIGN_SHIFT) & 0x1);//& T1_SIGN_MASK;
									CMQEncoder::Encode(A, C, v^GetSPB(f), GetCtxNo_SC(f));
									pFlags[i-1] |= T1_SIG_21 << (9);// 9 = 3*3
									flags |= (T1_SIG|(v << 19)) << (9);// 9 = 3*3
									pFlags[i+1] |= T1_SIG_01 << (9);// 9 = 3*3
									pFlags[i+FLAG_STEP-1] |= T1_SIG_20;
									pFlags[i+FLAG_STEP] |= T1_SIG_10|(v << 18);				
									pFlags[i+FLAG_STEP+1] |= T1_SIG_00;	
									m_nSigValues++;
								} else {
									CMQEncoder::Encode(A, C, 0, GetCtxNo_ZC(flags >> (9)));// 9 = 3*3
								}
							}
							flags&=~((T1_VISIT<<0)|(T1_VISIT<<3)|(T1_VISIT<<6)|(T1_VISIT<<9));
							flags |= (flags << (16)) & (T1_REFINE_1|T1_REFINE_2|T1_REFINE_3|T1_REFINE_4);
							pFlags[i] = flags;
						} else {
							static UINT8 Lut[] = { 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0 };
							int runlen = Lut[((pData[i*4 + 0] >> nShift) & 0x1) |
											 ((pData[i*4 + 1] >> nShift) & 0x1) << 1 |
											 ((pData[i*4 + 2] >> nShift) & 0x1) << 2 |
											 ((pData[i*4 + 3] >> nShift) & 0x1) << 3];
							
							if (runlen==4) {
								CMQEncoder::Encode(A, C, 0, T1_CTXNO_AGG);
								continue;
							}
							CMQEncoder::Encode(A, C, 1, T1_CTXNO_AGG);
							CMQEncoder::Encode(A, C, runlen>>1, T1_CTXNO_UNI);
							CMQEncoder::Encode(A, C, runlen&1, T1_CTXNO_UNI);
							
							m_A = A;
							m_C = C;
							EncClnPassStepPartial(pFlags + i, pData + i*4 + runlen, runlen);
							for(j = runlen + 1; j < 4; j++) {
								EncClnPassStep(pFlags + i, pData + i*4 + j, j);
							}
							pFlags[i]|= (pFlags[i] << (16)) & (T1_REFINE_1|T1_REFINE_2|T1_REFINE_3|T1_REFINE_4);
							A = m_A;
							C = m_C;
						}
					}
					m_A = A;
					m_C = C;
				} else {
					for (i=0; i<w; i++) {
						for (j = 0; j < max_j; j++) {
							EncClnPassStep(pFlags + i, pData + i*4 + j, j);
						}
						pFlags[i]&=~((T1_VISIT<<0)|(T1_VISIT<<3)|(T1_VISIT<<6)|(T1_VISIT<<9));
						pFlags[i]|= (pFlags[i] << (16)) & (T1_REFINE_1|T1_REFINE_2|T1_REFINE_3|T1_REFINE_4);

					}
				}
			}
		}

	T1_INLINE void UpdateFlags(T1_FLAG_TYPE *pFlags, int v, int j) {
			if(j == 0) {
				pFlags[-FLAG_STEP-1] |= T1_SIG_25;
				pFlags[-FLAG_STEP] |= T1_SIG_15|(v << 31);
				pFlags[-FLAG_STEP+1] |= T1_SIG_05;		
			}
			int j3 = j * 3;
			pFlags[-1] |= T1_SIG_21 << (j3);
			pFlags[0] |= (T1_SIG|(v << 19)) << (j3);
			pFlags[+1] |= T1_SIG_01 << (j3);
			if(j == 3) {
				pFlags[FLAG_STEP-1] |= T1_SIG_20;
				pFlags[FLAG_STEP] |= T1_SIG_10|(v << 18);				
				pFlags[FLAG_STEP+1] |= T1_SIG_00;		
			}
			m_nSigValues++;
		}
	T1_INLINE INT32 GetSPB(T1_FLAG_TYPE f) {
			return sm_lut_spb[f];
		}
};

}
}

#endif // !NCSJPCT_T1CODER_H