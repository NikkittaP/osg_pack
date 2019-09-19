/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	HuffmanCoder.h
** CREATED:	30 Jun 2004
** AUTHOR: 	Simon Cope
** PURPOSE:	CHuffmanCoder class header
** EDITS:
*******************************************************/

#ifndef NCSHUFFMANCODER_H
#define NCSHUFFMANCODER_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4786)
#endif

#include "NCSDefs.h"

#ifdef __cplusplus

#include <map>
#include <vector>

extern "C" {
#endif // __cplusplus

typedef struct _NCSHuffmanSymbol {
	//UINT16 nValue;
	UINT32 nValue; // can be used for upto 32 bit
	BOOLEAN bZeroRun;
} NCSHuffmanSymbol;

#ifdef __cplusplus
};
#include "NCSError.h"

namespace NCS {
namespace ECW {
/**
 * CHuffmanCoder class - ECW Huffman Coder/Decoder.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
 */
class CHuffmanCoder {
public:
	const static UINT32 _NUM_SYMBOL_VALUES;
	const static UINT16 _SIGN_MASK;
	const static UINT16 _RUN_MASK;
	const static INT16 _MAX_BIN_VALUE;
	const static INT16 _MIN_BIN_VALUE;
	const static UINT16 _VALUE_MASK;
	const static UINT16 _MAX_RUN_LENGTH;
	const static UINT8 _SMALL_SYMBOL;
	const static UINT8 _SMALL_SHIFT;

	// 16bit version
	const static UINT32 _NUM_SYMBOL_VALUES16;
	const static UINT32 _SIGN_MASK16;
	const static UINT32 _RUN_MASK16;
	const static INT32 _MAX_BIN_VALUE16;
	const static INT32 _MIN_BIN_VALUE16;
	const static UINT32 _VALUE_MASK16;
	const static UINT32 _MAX_RUN_LENGTH16;
	const static UINT8 _SMALL_SYMBOL16;
	const static UINT8 _SMALL_SHIFT16;

	/*
	**	Huffman structures and definitions
	*/
	class CTree;

	class CCodeNode {  
	public:
		typedef struct {
			class CCodeNode *m_p0Child;
			class CCodeNode *m_p1Child;
		} A;
		typedef union {
			A				m_P;
			class CCodeNode *m_Children[2];
		} U;
		U m_Children;

		NCSHuffmanSymbol m_Symbol;
		UINT32	m_nFrequency;
		class CCodeNode	*m_pPrev;  
		class CCodeNode	*m_pNext;  
		UINT32 	m_nCode;
		UINT8	m_nCodeBits;
		UINT16	m_nCodeBits16;
		bool	m_bInHistogram;

		CCodeNode();
		CCodeNode(const CCodeNode&);
		//CCodeNode(UINT8 **ppPacked, UINT32 &nNodes, UINT32 nState);
		virtual ~CCodeNode();

		void Pack(UINT8 **ppPacked, UINT32 &nNodes);
		void Pack8to8(UINT8 **ppPacked, UINT32 &nNodes);
		// 16bit version
		void Pack(UINT16 **ppPacked, UINT32 &nNodes);
		void Pack32to8(UINT8 **ppPacked, UINT32 &nNodes);
		void Pack16to8(UINT8 **ppPacked, UINT32 &nNodes);
		UINT8 SetCode(UINT32 nCode, UINT8 nCodeBits);
		UINT16 SetCode(UINT32 nCode, UINT16 nCodeBits);
		void BuildLut(CTree &tree);
	};
	class CTree: public CCodeNode {
	public:
		CCodeNode *m_pRoot;
		CCodeNode *m_Histogram[65536];
		UINT32 m_Histogram_LUT[65536];
		std::map<UINT32, CCodeNode *> m_HistogramList;
		
		bool mbUseHistogramLUT; // true if Pack_List used
		bool mbUseHistogramListVersion; // true if Pack_List used
		bool mbUseFrequencyListVersion; // true if Pack_List used

		CTree();
		virtual ~CTree();

		CError Pack(UINT8 **ppPacked, INT16 *pUnPacked, UINT32 nSymbols);
		CError Pack8to8(UINT8 **ppPacked, UINT8 *pUnPacked, UINT32 nSymbols);
		CError Pack16to8(UINT8 **ppPacked, INT16 *pUnPacked, UINT32 nSymbols);
		// 16bit version
		CError Pack(UINT16 **ppPacked, INT32 *pUnPacked, UINT32 nSymbols);
		CError Pack_List(UINT16 **ppPacked, INT32 *pUnPacked, UINT32 nSymbols);
		CError Pack32to8(UINT8 **ppPacked, INT32 *pUnPacked, UINT32 nSymbols);
		CError Pack_List32to8(UINT8 **ppPacked, INT32 *pUnPacked, UINT32 nSymbols);
#ifdef MULTIPLE_HEAPS
		static void *operator new(size_t size);
		static void operator delete(void *block);
	private: 
		UINT32 m_nHeapNumber;
#endif
	protected:
		CCodeNode *UnpackNode(UINT8 **ppPacked, UINT32 &nNodes);
		CCodeNode *UnpackNode(UINT16 **ppPacked, UINT32 &nNodes);
	private:
		//This is to speed up adding to the end of the list. 
		CCodeNode *m_pLastNode;
		NCS_INLINE CCodeNode *CreateNode_n_Link(UINT32 inSymbol) {
			CCodeNode *pNode = new CCodeNode();
			pNode->m_bInHistogram = TRUE;
			pNode->m_Symbol.nValue = inSymbol;
			pNode->m_nFrequency = 1;

			if(!m_pRoot) {
				m_pRoot = pNode;
				m_pLastNode = pNode;
			} else {
				CCodeNode *p = m_pLastNode;	
				p->m_pNext = pNode;
				pNode->m_pPrev = p;
				m_pLastNode = pNode;
			}

			return pNode;
		}
		NCS_INLINE CCodeNode *CreateNode_n_Link(UINT32 inIndex, UINT32 inSymbol) {
			CCodeNode *pNode = CreateNode_n_Link(inSymbol);
			pNode->m_nCode = inIndex; // temporarily borrowed

			return pNode;
		}
		NCS_INLINE void NodesReOrder(CCodeNode *pNode) {
			CCodeNode *p = pNode->m_pPrev;
			if(p && pNode->m_nFrequency > p->m_nFrequency) {
				// Higher frequency, move node left in list
				// unlink
				p->m_pNext = pNode->m_pNext;
				if(pNode->m_pNext) pNode->m_pNext->m_pPrev = p;
				if (m_pLastNode == pNode){
					m_pLastNode = p;
				}
				// find correct spot
				while(p && pNode->m_nFrequency > p->m_nFrequency) {
					p = p->m_pPrev;
				}
				if(p) {
					p->m_pNext->m_pPrev = pNode;
					pNode->m_pNext = p->m_pNext;
					pNode->m_pPrev = p;
					p->m_pNext = pNode;
				} else {
					pNode->m_pNext = m_pRoot;
					pNode->m_pPrev = NULL;
					m_pRoot->m_pPrev = pNode;
					m_pRoot = pNode;
				}
			}
		}

	};

//	typedef struct {
//		UINT16			m_Children;
//		UINT16			m_nValue;
//		UINT32 nPAD;
//	} State;
	class State {
	public:
		UINT16			m_Children;
		UINT16			m_nValue;
		UINT32 nPAD;
		NCS_INLINE State() 
		{ 
			nPAD = 0;
			m_Children = 0;
			m_nValue = 0;
		};
	};

	class State16 {
	public:
		UINT32			m_Children;
		UINT32			m_nValue;
		UINT32 nPAD;
		NCS_INLINE State16() 
		{ 
			nPAD = 0;
			m_Children = 0;
			m_nValue = 0;
		};
	};

	UINT16 m_nStates;
	State *m_pTable;
	State16 *m_pTable16; // for 16bit version
	UINT32 m_nNodes; // Shao added to prevent heap (m_pTable) corruption when file corrupted

		/** Default constructor. */
	CHuffmanCoder();
		/** virtual default destructor. */
	virtual ~CHuffmanCoder();

	CError Pack(UINT8 *pPacked, UINT32 *pPackedLength, INT16 *pUnPacked, UINT32 nRawLength);
	CError Pack8to8(UINT8 *pPacked, UINT32 *pPackedLength, UINT8 *pUnPacked, UINT32 nRawLength);
	// 16bit version
	CError Pack(UINT16 *pPacked, UINT32 *pPackedLength, INT32 *pUnPacked, UINT32 nRawLength);
	CError Pack(UINT8 *pPacked, UINT32 *pPackedLength, INT32 *pUnPacked, UINT32 nRawLength);
	CError Pack16to8(UINT8 *pPacked, UINT32 *pPackedLength, INT16 *pUnPacked, UINT32 nRawLength);
	
	// 8 bit
	CError UnPack(UINT8 *pPacked, UINT32 nPackedLength, INT16 *pUnPacked, UINT32 nUnpackedLength);
	CError UnPack8to8(UINT8 *pPacked, UINT32 nPackedLength, UINT8 *pUnPacked, UINT32 nUnpackedLength);
	// 16bit version
	CError UnPack(UINT16 *pPacked, UINT32 nPackedLength, INT32 *pUnPacked, UINT32 nUnpackedLength);
	CError UnPack(UINT8 *pPacked, UINT32 nPackedLength, INT32 *pUnPacked, UINT32 nUnpackedLength);
	CError UnPack_LUT(UINT8 *pPacked, UINT32 nPackedLength, INT32 *pUnPacked, UINT32 nUnpackedLength);
	CError UnPack8to16(UINT8 *pPacked, UINT32 nPackedLength, INT16 *pUnPacked, UINT32 nUnpackedLength);
private:
	CTree *m_pTree;

	CError Unpack(UINT8 **ppPacked);
	CError Unpack8to8(UINT8 **ppPacked);
	// 16bit version
	CError Unpack(UINT16 **ppPacked);
	CError Unpack8to32(UINT8 **ppPacked);
	CError Unpack8to16(UINT8 **ppPacked);
	// 8 bit
	CError Unpack(UINT8 **ppPacked, UINT32 &nNodes, UINT32 nState, UINT8 nCodeBits);
	CError Unpack8to8(UINT8 **ppPacked, UINT32 &nNodes, UINT32 nState, UINT8 nCodeBits);
	// 16bit version
	CError Unpack(UINT16 **ppPacked, UINT32 &nNodes, UINT32 nState, UINT16 nCodeBits); 
	CError Unpack8to32(UINT8 **ppPacked, UINT32 &nNodes, UINT32 nState, UINT16 nCodeBits); 
	CError Unpack8to16(UINT8 **ppPacked, UINT32 &nNodes, UINT32 nState, UINT8 nCodeBits);

	//
	void AssignTable(UINT64 &inTableSize, UINT64 &inTableBits, State16 *inTable, UINT32 *ioTable);
	void AssignTable16(UINT64 &inTableSize, UINT64 &inTableBits, State16 *inTable, UINT32 *ioTable);
	CError DecodeHuffmanSymbols(UINT8 *pPacked, INT32 *pOutput, INT32 &inWordCount,
		UINT64 &inTableSize, UINT64 &inTableBits, State16 *inTable, UINT32 *_inTable);

	//
	void AssignTable(UINT64 &inTableSize, UINT64 &inTableBits, State *inTable, State *ioTable);
	void AssignTable(UINT64 &inTableSize, UINT64 &inTableBits, State16 *inTable, State16 *ioTable);
	void AssignTable16(UINT64 &inTableSize, UINT64 &inTableBits, State16 *inTable, State16 *ioTable);
	void ValidateState(UINT64 &inTableBits, State16 *inTable, UINT32 &inCode, State16 **ioS);
	void ValidateState(UINT64 &inTableBits, State16 *inTable, UINT32 &inCode, State16 &ioS);
	CError DecodeHuffmanSymbols(UINT8 *pPacked, INT32 *pOutput, INT32 &inWordCount,
		UINT64 &inTableSize, UINT64 &inTableBits, State16 *inTable, State16 *_inTable);

};
}
}
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define NCS_HUFFMAN_MAX_RUN_LENGTH 0x7fff // 32767
#define NCS_HUFFMAN_SIGN_MASK	0x4000 // 16384
#define NCS_HUFFMAN_VALUE_MASK	0x3fff // 16383
#define NCS_HUFFMAN_RUN_MASK	0x8000 // 32768

// 16bit version
#define NCS_HUFFMAN_MAX_RUN_LENGTH16 0x7fffffff // 2147483647
#define NCS_HUFFMAN_SIGN_MASK16	0x40000000 // 1073741824
#define NCS_HUFFMAN_VALUE_MASK16 0x3fffffff // 1073741823
#define NCS_HUFFMAN_RUN_MASK16	0x80000000 // 2147483648

typedef struct {
	void	*pTree;
	UINT32	nBitsUsed;		
} NCSHuffmanState;

NCSError unpack_huffman(UINT8 *pPacked, UINT32 nPackedLength, INT16 *pUnPacked, UINT32 nUnpackedLength);
NCSError unpack_huffman8to8(UINT8 *pPacked, UINT32 nPackedLength, UINT8 *pUnPacked, UINT32 nUnpackedLength);
// 16bit version
NCSError unpack_huffman16(UINT16 *pPacked, UINT32 nPackedLength, INT32 *pUnPacked, UINT32 nUnpackedLength);
NCSError unpack_huffman8to32(UINT8 *pPacked, UINT32 nPackedLength, INT32 *pUnPacked, UINT32 nUnpackedLength);
NCSError unpack_huffman8to16(UINT8 *pPacked, UINT32 nPackedLength, INT16 *pUnPacked, UINT32 nUnpackedLength);

void unpack_huffman_init_state16(NCSHuffmanState *pState, UINT16 **ppPacked);
void unpack_huffman_init_state(NCSHuffmanState *pState, UINT8 **ppPacked);
void unpack_huffman_fini_state(NCSHuffmanState *pState);
NCSHuffmanSymbol *unpack_huffman_symbol(UINT8 **ppPacked, NCSHuffmanState *pState);
NCSHuffmanSymbol *unpack_huffman_symbol16(UINT16 **ppPacked, NCSHuffmanState *pState);
static NCS_INLINE BOOLEAN unpack_huffman_symbol_zero_run(NCSHuffmanSymbol *pSymbol) {
				return(pSymbol->bZeroRun);
			}
//static NCS_INLINE UINT16 unpack_huffman_symbol_zero_length(NCSHuffmanSymbol *pSymbol) {
static NCS_INLINE UINT32 unpack_huffman_symbol_zero_length(NCSHuffmanSymbol *pSymbol) {
				return(pSymbol->nValue);
			};
//static NCS_INLINE INT16 unpack_huffman_symbol_value(NCSHuffmanSymbol *pSymbol) {
static NCS_INLINE INT32 unpack_huffman_symbol_value(NCSHuffmanSymbol *pSymbol) {
				return(pSymbol->nValue);
			};

UINT32	pack_huffman(UINT8 *pPacked, INT16 *pUnPacked, UINT32 symbol_stream_length);
UINT32	pack_huffman8to8(UINT8 *pPacked, UINT8 *pUnPacked, UINT32 symbol_stream_length);
// 16bit version
UINT32	pack_huffman16(UINT16 *pPacked, INT32 *pUnPacked, UINT32 symbol_stream_length);
UINT32	pack_huffman32to8(UINT8 *pPacked, INT32 *pUnPacked, UINT32 symbol_stream_length);
UINT32	pack_huffman16to8(UINT8 *pPacked, INT16 *pUnPacked, UINT32 symbol_stream_length);
#ifdef __cplusplus
};
#endif

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif /* NCSHUFFMANCODER_H */
