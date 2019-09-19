/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	Transform.h
** CREATED:	01/03/2012
** PURPOSE:	API for setting transformation on a NCS::CView
**
********************************************************/

#ifndef NCS_TRANSFORM_H
#define NCS_TRANSFORM_H

#include "NCSEcw/SDK/Buffer2D.h"

namespace NCS {
namespace API {

/**
 * \class	ITransform
 *
 * \brief	Purely virtual interface for implementing ecw/jp2 decoding transformations.
**/
class NCSECW_IMPEX ITransform {
public:
	virtual NCSError TransformLine(NCSCellType eSrcDataType, NCSCellType eSrcCellType, INT32 nSrcStep, void *pSrc, 
								   NCSCellType eDstDataType, NCSCellType eDstCellType, INT32 nDstStep, void *pDst, INT32 nCells) = 0;

	virtual NCSError Transform(NCSCellType eSrcDataType, INT32 nSrcStep, NCS::SDK::CBuffer2D &Src, 
							   NCSCellType eDstDataType, INT32 nDstStep, NCS::SDK::CBuffer2D &Dst) = 0;
    virtual ~ITransform(){
        
    }
};
typedef std::list<API::ITransform *> CTransformList;

class NCSECW_IMPEX CTransformListAccessor
{
public:
	CTransformListAccessor() : m_pMutex(NULL), m_pTransformList(NULL) {};
	virtual ~CTransformListAccessor() { Release(); };

	CTransformList *List() { return m_pTransformList; };
	bool Empty() { return  m_pTransformList == NULL; };
	void Setup(NCS::CMutex *pMutex, CTransformList *pTransformList);
	void Release();
protected:
	NCS::CMutex *m_pMutex;
	CTransformList *m_pTransformList;
};

class NCSECW_IMPEX CMinMaxTransform : public ITransform {
public:
	CMinMaxTransform(UINT8 nMin, UINT8 nMax);
	CMinMaxTransform(UINT16 nMin, UINT16 nMax);
	CMinMaxTransform(UINT32 nMin, UINT32 nMax);
	CMinMaxTransform(UINT64 nMin, UINT64 nMax);
	CMinMaxTransform(INT8 nMin, INT8 nMax);
	CMinMaxTransform(INT16 nMin, INT16 nMax);
	CMinMaxTransform(INT32 nMin, INT32 nMax);
	CMinMaxTransform(INT64 nMin, INT64 nMax);
	CMinMaxTransform(IEEE4 nMin, IEEE4 nMax);
	CMinMaxTransform(IEEE8 nMin, IEEE8 nMax);
	explicit CMinMaxTransform(NCSCellType eCellType);

	CMinMaxTransform(const CMinMaxTransform &s);
	const CMinMaxTransform &operator=(const CMinMaxTransform &s);

	virtual ~CMinMaxTransform() {};

	virtual NCSError TransformLine(NCSCellType eSrcDataType, NCSCellType eSrcCellType, INT32 nSrcStep, void *pSrc, 
								   NCSCellType eDstDataType, NCSCellType eDstCellType, INT32 nDstStep, void *pDst, INT32 nCells);

	virtual NCSError Transform(NCSCellType eSrcDataType, INT32 nSrcStep, NCS::SDK::CBuffer2D &Src, 
							   NCSCellType eDstDataType, INT32 nDstStep, NCS::SDK::CBuffer2D &Dst);


	void SetMin(UINT8 nMin);
	void SetMin(UINT16 nMin);
	void SetMin(UINT32 nMin);
	void SetMin(UINT64 nMin);
	void SetMin(INT8 nMin);
	void SetMin(INT16 nMin);
	void SetMin(INT32 nMin);
	void SetMin(INT64 nMin);
	void SetMin(IEEE4 fMin);
	void SetMin(IEEE8 dMin);

	void SetMax(UINT8 nMax);
	void SetMax(UINT16 nMax);
	void SetMax(UINT32 nMax);
	void SetMax(UINT64 nMax);
	void SetMax(INT8 nMax);
	void SetMax(INT16 nMax);
	void SetMax(INT32 nMax);
	void SetMax(INT64 nMax);
	void SetMax(IEEE4 fMax);
	void SetMax(IEEE8 dMax);

	union UScaleValue {
		UINT8  u8;
		INT8   i8;
		UINT16 u16;
		INT16  i16;
		UINT32 u32;
		INT32  i32;
		UINT64 u64;
		INT64  i64;
		IEEE4  ie4;
		IEEE8  ie8;
	};

	virtual NCSError Transform(NCSCellType eSrcDataType, INT32 nSrcStep, NCS::SDK::CBuffer2D &Src,
		NCSCellType eDstDataType, INT32 nDstStep, UScaleValue& dstMin, UScaleValue& dstMax, NCS::SDK::CBuffer2D &Dst);


	UScaleValue &Min() { return m_uScaleMin; };
	UScaleValue &Max() { return m_uScaleMax; };
protected:
	UScaleValue m_uScaleMin;
	UScaleValue m_uScaleMax;
};

} // API
} // NCS

#endif // NCS_TRANSFORM_H

