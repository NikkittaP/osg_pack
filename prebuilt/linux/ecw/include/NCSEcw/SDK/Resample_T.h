/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	Resample_T.h
** CREATED:	01/03/2012
** PURPOSE:	Consolidated resampling for ECW and JP2 decoders
**
********************************************************/

#ifndef NCS_SDK_RESAMPLE_T
#define NCS_SDK_RESAMPLE_T

#if defined(POSIX) && !defined(MACOSX) && !defined(NCS_ANDROID)
#include <fpu_control.h>
#endif

#include "NCSEcw/SDK/Buffer2D.h"

namespace NCS {
namespace SDK {

template <class BT>
static bool Resample1Line_NearestNeighbour_INT(BT *pDst, BT *pSrc, double dStartX, double dIncX, UINT32 nWidth, int nShift)
{
	register UINT32 number_x = nWidth;
	register UINT64 x_offset = ((UINT64) (dStartX * 0x10000) << 16);
	// This gives us 1/(2^16) accuracy per line, which should be fine
	register UINT64 increment_x = ((UINT64) (dIncX * 0x10000) << 16);
	register BT *p_in = pSrc;
	register BT *p_out = pDst;

	for(UINT32 x = 0; x < number_x; x++) {
		((BT*)p_out)[x] = (BT)(*(p_in + (UINT32) (x_offset >> 32))) >> nShift;
		x_offset += increment_x;
	}
	return(true);
}

template <class BT>
static bool Resample1Line_NearestNeighbour_FP(BT *pDst, BT *pSrc, double dStartX, double dIncX, UINT32 nWidth, int nShift)
{
	register UINT32 number_x = nWidth;
	register UINT64 x_offset = ((UINT64) (dStartX * 0x10000) << 16);
	// This gives us 1/(2^16) accuracy per line, which should be fine
	register UINT64 increment_x = ((UINT64) (dIncX * 0x10000) << 16);
	register BT *p_in = pSrc;
	register BT *p_out = pDst;
	register INT32 nDiv = NCS2Pow(nShift);

	for(UINT32 x = 0; x < number_x; x++) {
		((BT*)p_out)[x] = (BT)(*(p_in + (UINT32) (x_offset >> 32))) / nDiv;
		x_offset += increment_x;
	}
	return(true);
}

#define BILINEAR_INTERP_2x2_FLT(o, v00, v01, v10, v11, dx, dy) \
									o = (IEEE4)((v00 + (v01 - v00) * (dx)) + \
											((v10 + (v11 - v10) * (dx)) - \
											 (v00 + (v01 - v00) * (dx))) * (dy))
#define BILINEAR_INTERP_2x2_DBL(o, v00, v01, v10, v11, dx, dy) \
									o = (IEEE8)((v00 + (v01 - v00) * (dx)) + \
											((v10 + (v11 - v10) * (dx)) - \
											 (v00 + (v01 - v00) * (dx))) * (dy))
#define BILINEAR_INTERP_2x2_INT(o, v00, v01, v10, v11, dx, dy) \
								{ \
									float f;\
									BILINEAR_INTERP_2x2_FLT(f, v00, v01, v10, v11, dx, dy);\
									o = NCSfloatToInt32_RM(f); \
								}
#define BILINEAR_INTERP_2x2_INT64(o, v00, v01, v10, v11, dx, dy) \
								{ \
									double f;\
									BILINEAR_INTERP_2x2_DBL(f, v00, v01, v10, v11, dx, dy);\
									o = NCSfloatToInt64_RM(f); \
								}
#define BILINEAR_INTERP_2(v0, v1, d)	((v0) + ((v1) - (v0)) * (d))

template<class BT>
static bool Resample1Line_Bilinear_INT(BT *pDst, BT *pSrc1, BT *pSrc2, int nShift, double dStartX, double dIncX, double dY, INT32 nWidth)
{
	register INT32 number_x = nWidth;
	register BT *p_out = pDst;
	register BT *p_in = pSrc1;
	register BT *p_in2 = pSrc2;

	double dX = dStartX;
	dIncX -= 0.5;
	for(register INT32 x = 0; x < number_x; x++) {
		//dX = dStartX + x * dIncX;
		dX += 0.5;
		double dXF = dX - (INT32)dX;
		INT32 nX1 = NCSFloor(dX);
		INT32 nX0 = nX1-1;
		
		BILINEAR_INTERP_2x2_INT(((BT*)p_out)[x], ((BT*)p_in)[nX0], ((BT*)p_in)[nX1], ((BT*)p_in2)[nX0], ((BT*)p_in2)[nX1], dXF, dY);
		((BT*)p_out)[x] >>= nShift;
		
		dX += dIncX;
	}

	return true;
}

template<class BT>
static bool Resample1Line_Bilinear_FP(BT *pDst, BT *pSrc1, BT *pSrc2, int nShift, double dStartX, double dIncX, double dY, INT32 nWidth)
{
	register INT32 number_x = nWidth;
	register BT *p_out = pDst;
	register BT *p_in = pSrc1;
	register BT *p_in2 = pSrc2;
	register INT32 nDiv = NCS2Pow(nShift);

	double dX = dStartX;
	dIncX -= 0.5;
	for(register INT32 x = 0; x < number_x; x++) {
		dX += 0.5;
		double dXF = dX - (INT32)dX;
		INT32 nX1 = NCSFloor(dX);
		INT32 nX0 = nX1-1;

		BILINEAR_INTERP_2x2_FLT(((IEEE4*)p_out)[x], ((IEEE4*)p_in)[nX0], ((IEEE4*)p_in)[nX1], ((IEEE4*)p_in2)[nX0], ((IEEE4*)p_in2)[nX1], dXF, dY);
		((IEEE4*)p_out)[x] /= nDiv;
		
		dX += dIncX;
	}
	return true;
}

class IResampleLine
{
public:
	virtual ~IResampleLine() {}

	virtual void Bilinear(void *pDst, void *pSrc1, void *pSrc2, int nShift, double dStartX, double dIncX, double dY, INT32 nWidth) = 0;
	virtual void NearestNeighbour(void *pDst, void *pSrc, double dStartX, double dIncX, UINT32 nWidth, int nShift) = 0;
	virtual int GetSize() = 0;
};
typedef std::shared_ptr<IResampleLine> IResampleLinePtr;

template<typename T>
class CIntegralResampleLine : public IResampleLine
{
public:
	CIntegralResampleLine() {}
	virtual ~CIntegralResampleLine() {}

	virtual void Bilinear(void *pDst, void *pSrc1, void *pSrc2, int nShift, double dStartX, double dIncX, double dY, INT32 nWidth) {
		NCS::SDK::Resample1Line_Bilinear_INT<T>(static_cast<T*>(pDst), static_cast<T*>(pSrc1), static_cast<T*>(pSrc2), nShift, dStartX, dIncX, dY, nWidth);
	}
	virtual void NearestNeighbour(void *pDst, void *pSrc, double dStartX, double dIncX, UINT32 nWidth, int nShift) {
		NCS::SDK::Resample1Line_NearestNeighbour_INT<T>(static_cast<T*>(pDst), static_cast<T*>(pSrc), dStartX, dIncX, nWidth, nShift);
	}
	virtual int GetSize() {
		return sizeof(T);
	}
};

} // NCS
} // SDK

#endif // NCS_SDK_RESAMPLE_T
