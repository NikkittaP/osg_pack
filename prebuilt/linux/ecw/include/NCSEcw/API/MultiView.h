/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/


#ifndef NCSMULTIVIEW_H
#define NCSMULTIVIEW_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "NCSEcw/API/View.h"
#include "NCSEcw/SDK/Atomic.h"

namespace NCS {

class NCSECW_IMPEX CMultiView: public CView {
public:
	class CViewList;
	class Handle {
	public: 
		Handle() { m_id = (UINT64)-1; m_pMV = NULL; };
		virtual ~Handle() { release(); };
		void release() { m_pMV->ReleaseHandle(*this); };
	protected:
	friend class CMultiView;
		CMultiView *m_pMV;
		UINT64 m_id;
	};

	CMultiView();
	virtual ~CMultiView();

	virtual CError Close ( bool bFreeCache = false );

	virtual CError SetView(Handle &Id, UINT32 nBands, UINT32 *pBandList, 
					   		  UINT32 nDatasetTLX, UINT32 nDatasetTLY,
							  UINT32 nDatasetBRX, UINT32 nDatasetBRY,
							  UINT32 nWidth, UINT32 nHeight,
							  IEEE8 dWorldTLX = 0.0, IEEE8 dWorldTLY = 0.0,
							  IEEE8 dWorldBRX = 0.0, IEEE8 dWorldBRY = 0.0);
	virtual CError SetView (Handle &Id, INT32 nBands, INT32 *pBandList, 
					   INT32 nWidth, INT32 nHeight,
					   IEEE8 dWorldTLX, IEEE8 dWorldTLY,
					   IEEE8 dWorldBRX, IEEE8 dWorldBRY );
	virtual CError SetView (Handle &Id,  INT32 nBands, INT32 *pBandList, 
					   INT32 nWidth, INT32 nHeight,
					   INT32 nDatasetTLX, INT32 nDatasetTLY,
					   INT32 nDatasetBRX, INT32 nDatasetBRY );
	virtual NCSReadStatus ReadLineBIL(Handle Id, UINT8 **ppOutputLine);
	virtual NCSReadStatus ReadLineBIL(Handle Id, UINT16 **ppOutputLine);
	virtual NCSReadStatus ReadLineBIL(Handle Id, UINT32 **ppOutputLine);
	virtual NCSReadStatus ReadLineBIL(Handle Id, UINT64 **ppOutputLine);
	virtual NCSReadStatus ReadLineBIL(Handle Id, INT8 **ppOutputLine);
	virtual NCSReadStatus ReadLineBIL(Handle Id, INT16 **ppOutputLine);
	virtual NCSReadStatus ReadLineBIL(Handle Id, INT32 **ppOutputLine);
	virtual NCSReadStatus ReadLineBIL(Handle Id, INT64 **ppOutputLine);
	virtual NCSReadStatus ReadLineBIL(Handle Id, IEEE4 **ppOutputLine);
	virtual NCSReadStatus ReadLineBIL(Handle Id, IEEE8 **ppOutputLine);
	virtual NCSReadStatus ReadLineRGB(Handle Id, UINT8 *pRGBTriplet);
	virtual NCSReadStatus ReadLineBGR(Handle Id, UINT8 *pBGRTriplet);
	virtual NCSReadStatus ReadLineRGBA(Handle Id, UINT32 *pRGBA);
	virtual NCSReadStatus ReadLineBGRA(Handle Id, UINT32 *pBGRA);
	virtual NCSReadStatus ReadLineARGB(Handle Id, UINT32 *pARGB);
	virtual NCSReadStatus ReadLineABGR(Handle Id, UINT32 *pABGR);
	virtual NCSReadStatus ReadLineBIL(Handle Id, NCSCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL);
	virtual NCSReadStatus Read(Handle Id, SDK::CBuffer2DVector &Buffers, NCSCellType eOutputType);
protected:
	friend class Handle;
	virtual CError ReleaseHandle(Handle &Id);
private:
	CViewList *m_pViews;
	NCS::SDK::CAtomicUnsignedInt64 m_NextId;
};

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif //NCSMULTIVIEW_H