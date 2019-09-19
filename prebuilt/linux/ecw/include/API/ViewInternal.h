/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: ViewInternal.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: View internal header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_VIEW_INTERNAL_H
#define NCS_API_VIEW_INTERNAL_H

#include "API/DecoderBase.h"
#include "API/ProgressiveDecoder.h"
#include "NCSFile.h"
#include "NCSRect.h"
#include "NCSEcw/SDK/Resample_T.h"
#include "NCSStringUtils.h"
#include "NCSEcw/SDK/Buffer2D.h"

#include <memory>
using namespace NCS;
using namespace API;

namespace NCS {
namespace SDK {

template<typename PixelT>
class CScanLineBuffer
{
public:
	explicit CScanLineBuffer(UINT32 nWidth = 0) : 
		m_ScanLineData(),
		m_nWidth(nWidth),
		m_bNeedsReset(true)
	{
	}
	virtual ~CScanLineBuffer()
	{
	}

	void Reset(UINT32 nWidth)
	{
		m_nWidth = nWidth;
		m_bNeedsReset = true;
	}

	PixelT *Get() {
		if (m_bNeedsReset) {
			NCS::SDK::CBuffer2D::GetDecompressionCounter() -= m_ScanLineData.size();
			m_ScanLineData.resize(m_nWidth);
			NCS::SDK::CBuffer2D::GetDecompressionCounter() += m_nWidth;
			m_bNeedsReset = false;
		}
		return &m_ScanLineData[0];
	}

protected:
	std::vector<PixelT> m_ScanLineData;
	UINT32 m_nWidth;
	bool m_bNeedsReset;
};

template<typename PixelT>
class CRectViewBuffer
{
public:
	CRectViewBuffer() :
		m_ScanLineBuffers(),
		m_nWidth(0),
		m_nHeight(0)
	{

	}

	CRectViewBuffer(UINT32 nWidth, UINT32 nHeight) :
		m_ScanLineBuffers(),
		m_nWidth(0),
		m_nHeight(0)
	{
		Reset(nWidth, nHeight);
	}

	virtual ~CRectViewBuffer()
	{
		
	}

	void Reset(UINT32 nWidth, UINT32 nHeight)
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_ScanLineBuffers.resize(m_nHeight);
		for (auto it = m_ScanLineBuffers.begin(); it != m_ScanLineBuffers.end(); ++it) {
			it->Reset(m_nWidth);
		}
	}

	PixelT *Get(UINT32 nRow) {
		if (nRow >= m_nHeight) {
			return NULL;
		}
		return m_ScanLineBuffers[nRow].Get();
	}

	UINT32 Width() { return m_nWidth; }
	UINT32 Height() { return m_nHeight; }
	UINT64 BufferSize() { 
		UINT64 nSize = m_nWidth;
		nSize *= m_nHeight;
		return nSize;
	}

protected:
	std::vector<CScanLineBuffer<PixelT> > m_ScanLineBuffers;
	UINT32 m_nWidth;
	UINT32 m_nHeight;
};

template<typename PixelT>
class CBufferQueue
{
public:
	explicit CBufferQueue(UINT32 nMaxQueueSize) : m_nMaxQueueSize(nMaxQueueSize)
	{

	}
	virtual ~CBufferQueue()
	{

	}

	std::shared_ptr<CRectViewBuffer<PixelT> > Get(UINT32 nRequestedBufferWidth, UINT32 nRequestedBufferHeight) {
		{
			NCS::CMutexLock Lock(m_Mutex);
			for (auto it = m_BufferList.begin(); it != m_BufferList.end(); ++it) {
				if ((*it)->Width() >= nRequestedBufferWidth && (*it)->Height() >= nRequestedBufferHeight) {
					std::shared_ptr<CRectViewBuffer<PixelT> > dequeuedItem = *it;
					m_BufferList.erase(it);
					return dequeuedItem;
				}
			}
		}
		return std::shared_ptr<CRectViewBuffer<PixelT> >(new CRectViewBuffer<PixelT>(nRequestedBufferWidth, nRequestedBufferHeight));
	}

	void Return(std::shared_ptr<CRectViewBuffer<PixelT> > &BufferItem) {
		
		if (BufferItem) {
			if (BufferItem->BufferSize() > 0) {
				NCS::CMutexLock Lock(m_Mutex);
				if (m_BufferList.size() < m_nMaxQueueSize) {
					m_BufferList.push_front(BufferItem);
				}
				else {
					// purge smallest
					for (auto it = m_BufferList.begin(); it != m_BufferList.end(); ++it) {
						if ((*it)->BufferSize() < BufferItem->BufferSize()) {
							*it = BufferItem;
							break;
						}
					}
				}
			}
			BufferItem.reset();
		}
	}

protected:
	std::list<std::shared_ptr<CRectViewBuffer<PixelT> > > m_BufferList;
	UINT32 m_nMaxQueueSize;
	NCS::CMutex m_Mutex;
};

}
}

class NCS::API::CCurrentProgressiveView 
{
	static NCS::SDK::CBufferQueue<UINT32> s_BufferCache;
public:
	CCurrentProgressiveView() :
		m_TmpViewBuffer(),
		m_CurrentViewBuffer(),
		m_nBackgroundColour(0),
		m_pDecoderBaseCallback(NULL),
		m_StringUtils(NCS::CStringUtils::Singleton())
	{
		memset(&m_CurrentView, 0x0, sizeof(NCSSetViewInfo));
	}
	virtual ~CCurrentProgressiveView()
	{
		NCS::CMutexLock _Lock(m_ViewClosing);

		s_BufferCache.Return(m_TmpViewBuffer);
		s_BufferCache.Return(m_CurrentViewBuffer);
	
		NCSFree(m_CurrentView.pBandList);
		m_CurrentView.pBandList = NULL;
	}

	bool CalculateDeviceCoords(int nDeviceTLX, int nDeviceTLY, int nDeviceWidth, int nDeviceHeight,
							   double dWorldTLX, double dWorldTLY, double dWorldBRX, double dWorldBRY,
							   double m_dCurrentWorldTLX, double m_dCurrentWorldTLY, double m_dCurrentWorldBRX, double m_dCurrentWorldBRY,
							   int *outputDeviceCoords)
	{
		double tlx, tly, brx, bry;

		tlx = ((m_dCurrentWorldTLX - dWorldTLX)/(dWorldBRX-dWorldTLX))*(double)(nDeviceWidth)  + (double)nDeviceTLX;
		tly = ((m_dCurrentWorldTLY - dWorldTLY)/(dWorldBRY-dWorldTLY))*(double)(nDeviceHeight) + (double)nDeviceTLY;
		brx = ((m_dCurrentWorldBRX - dWorldTLX)/(dWorldBRX-dWorldTLX))*(double)(nDeviceWidth)  + (double)nDeviceTLX;
		bry = ((m_dCurrentWorldBRY - dWorldTLY)/(dWorldBRY-dWorldTLY))*(double)(nDeviceHeight) + (double)nDeviceTLY;

		// Round - be careful to round in the right direction if the origin is negative.
		outputDeviceCoords[0] = (int)(tlx + (tlx < 0.0 ? -0.5 : 0.5));
		outputDeviceCoords[1] = (int)(tly + (tly < 0.0 ? -0.5 : 0.5));			
		outputDeviceCoords[2] = (int)(brx + (brx < 0.0 ? -0.5 : 0.5));
		outputDeviceCoords[3] = (int)(bry + (bry < 0.0 ? -0.5 : 0.5));
	
		return true;
	}

	bool calculateImageCoords( double dDevice1TLX, double dDevice1TLY, double dDevice1BRX, double dDevice1BRY,
                                double dImageWidth, double dImageHeight,
                                double dDevice2TLX, double dDevice2TLY, double dDevice2BRX, double dDevice2BRY,
								double *pOutputImageCoords)
	{
		double outputImageWidth, outputImageHeight;
	
		outputImageWidth  = ((dDevice2BRX-dDevice2TLX) * (dImageWidth))/(dDevice1BRX-dDevice1TLX)  ;
		outputImageHeight = ((dDevice2BRY-dDevice2TLY) * (dImageHeight))/(dDevice1BRY-dDevice1TLY)  ;
		pOutputImageCoords[0] = ((dDevice2TLX-dDevice1TLX)  * (outputImageWidth))/(dDevice2BRX-dDevice2TLX) ;
		pOutputImageCoords[1] = ((dDevice2TLY-dDevice1TLY)  * (outputImageHeight))/(dDevice2BRY-dDevice2TLY) ;
		pOutputImageCoords[2] = pOutputImageCoords[0] + outputImageWidth;
		pOutputImageCoords[3] = pOutputImageCoords[1] + outputImageHeight;
	
		return true;
	}

	void SetProgressiveCallback(CProgressiveDecoderBase *pDecoderBaseCallback)
	{
		m_pDecoderBaseCallback = pDecoderBaseCallback;
	}

	NCS::CError CopyFullCurrentView(unsigned int *pRGBA) {
		if (pRGBA != NULL) {
			UINT32 nLineSize = m_CurrentView.nSizeX * sizeof(UINT32);
			for (UINT32 nLine = 0; nLine < m_CurrentView.nSizeY; nLine++) {
				UINT32 *pLine = m_CurrentViewBuffer->Get(nLine);
				if (pLine == NULL) {
					return NCS::CError(NCS_COULDNT_ALLOC_MEMORY, NCS_FUNCTION, NCS::CString().FormatEx(NCS_T("Current View Buffer line %u was null"), nLine));
				}
				memcpy(pRGBA + (m_CurrentView.nSizeX * nLine), pLine, nLineSize);
			}
		}
		return NCS_SUCCESS;
	}

	NCS::CError CopyPartialCurrentView(int nWidth, int nHeight, double dWorldTLX, double dWorldTLY, double dWorldBRX, double dWorldBRY, unsigned int *pRGBA)
	{
		if (pRGBA != NULL) {
			CNCSRect_D NewView(dWorldTLX, dWorldTLY, dWorldBRX, dWorldBRY);
			NewView.Normalize();
			CNCSRect_D Intersect = m_CurrentViewRect.Common(NewView);
			if (Intersect.IsValid()) {
				double dCurrentViewCellSizeX = m_CurrentViewRect.DeltaX() / m_CurrentView.nSizeX;
				double dCurrentViewCellSizeY = m_CurrentViewRect.DeltaY() / m_CurrentView.nSizeY;

				double dNewViewCellSizeX = NewView.DeltaX() / nWidth;
				double dNewViewCellSizeY = NewView.DeltaY() / nHeight;

				int nSrcOffsetX, nSrcOffsetY, nDstOffsetX, nDstOffsetY;
				nSrcOffsetX = nSrcOffsetY = nDstOffsetX = nDstOffsetY = 0;

				if (fabs(dCurrentViewCellSizeX - dNewViewCellSizeX) < FLT_EPSILON && fabs(dCurrentViewCellSizeY - dNewViewCellSizeY) < FLT_EPSILON) {
					// pan
					int nCopyWidth, nCopyHeight;
					double dV = (m_CurrentView.fTopX - dWorldTLX) / dCurrentViewCellSizeX;
					int nV = (int)(dV + (dV < 0.0 ? -0.5 : 0.5));
					if (nV < 0) {
						nSrcOffsetX = abs(nV);
						nCopyWidth = nWidth - nSrcOffsetX;
					}
					else {
						nDstOffsetX = nV;
						nCopyWidth = nWidth - nDstOffsetX;
					}

					if (dWorldTLY < dWorldBRY) {
						dV = (m_CurrentView.fLeftY - dWorldTLY) / dCurrentViewCellSizeY;
					}
					else {
						dV = (dWorldTLY - m_CurrentView.fLeftY) / dCurrentViewCellSizeY;
					}
					nV = (int)(dV + (dV < 0.0 ? -0.5 : 0.5));
					if (nV < 0) {
						nSrcOffsetY = abs(nV);
						nCopyHeight = nHeight - nSrcOffsetY;
					}
					else {
						nDstOffsetY = nV;
						nCopyHeight = nHeight - nDstOffsetY;
					}

					UINT32 *pDst = pRGBA;
					for (int nL = 0; nL < nDstOffsetY; nL++) {
						for (int nC = 0; nC < nWidth; nC++) {
							*pDst++ = m_nBackgroundColour;
						}
					}
					for (int nL = 0; nL < nCopyHeight; nL++) {
						UINT32 *pLine = m_CurrentViewBuffer->Get(nL + nSrcOffsetY);
						if (pLine == NULL) {
							return NCS::CError(NCS_COULDNT_ALLOC_MEMORY, NCS_FUNCTION, NCS::CString().FormatEx(NCS_T("Current View Buffer line %u was null"), nL + nSrcOffsetY));
						}
						UINT32 *pSrc = pLine + nSrcOffsetX;

						for (int nC = 0; nC < nDstOffsetX; nC++) {
							*pDst++ = m_nBackgroundColour;
						}

						memcpy(pDst, pSrc, nCopyWidth * sizeof(UINT32));
						pDst += nCopyWidth;

						for (int nC = nDstOffsetX + nCopyWidth; nC < nWidth; nC++) {
							*pDst++ = m_nBackgroundColour;
						}
					}
					for (int nL = nDstOffsetY + nCopyHeight; nL < nHeight; nL++) {
						for (int nC = 0; nC < nWidth; nC++) {
							*pDst++ = m_nBackgroundColour;
						}
					}
				}
				else {
					// zoom

					// This is the complex bit, stretching the image. We need to work out:
					// A) The screen coordinates of where the available imagery maps to
					// B) Clip these screen coords to the actual screen area.
					// c) From the screen area, calculate the corresponding coords in the image from which we will do the stretch.

					// Calculate the actual SCREEN area to which the current image can be stretched, then clip it to the device coords.

					UINT32 *pDst = pRGBA;
					for (int nL = 0; nL < nHeight; nL++) {
						for (int nC = 0; nC < nWidth; nC++) {
							*pDst++ = m_nBackgroundColour;
						}
					}

					int dOutputDeviceCoords[4];
					CalculateDeviceCoords(0, 0, nWidth, nHeight,
						dWorldTLX, dWorldTLY, dWorldBRX, dWorldBRY,
						m_CurrentView.fTopX, m_CurrentView.fLeftY, m_CurrentView.fBottomX, m_CurrentView.fRightY,
						dOutputDeviceCoords);

					double outputScreenTLX = (dOutputDeviceCoords[0] < (double)0) ? (double)0 : dOutputDeviceCoords[0];
					double outputScreenTLY = (dOutputDeviceCoords[1] < (double)0) ? (double)0 : dOutputDeviceCoords[1];
					double outputScreenBRX = (dOutputDeviceCoords[2] > (double)(0 + nWidth)) ? (double)(0 + nWidth) : dOutputDeviceCoords[2];
					double outputScreenBRY = (dOutputDeviceCoords[3] > (double)(0 + nHeight)) ? (double)(0 + nHeight) : dOutputDeviceCoords[3];

					// Calculate the actual IMAGE coords that map to the screen extent area calculated above. 
					double pOutputImageCoords[4];
					calculateImageCoords(dOutputDeviceCoords[0], dOutputDeviceCoords[1], dOutputDeviceCoords[2], dOutputDeviceCoords[3],
						m_CurrentView.nSizeX, m_CurrentView.nSizeY,
						outputScreenTLX, outputScreenTLY, outputScreenBRX, outputScreenBRY,
						pOutputImageCoords);


					int nSrcR = 0;
					double dDstIncX = ((pOutputImageCoords[2] - pOutputImageCoords[0]) / (outputScreenBRX - outputScreenTLX));
					double dDstIncY = ((pOutputImageCoords[3] - pOutputImageCoords[1]) / (outputScreenBRY - outputScreenTLY));

					for (int nR = (int)outputScreenTLY; nR < (int)outputScreenBRY; nR++) {
						UINT32 nSrcLine = static_cast<UINT32>((static_cast<int>(nSrcR*dDstIncY) + static_cast<int>(pOutputImageCoords[1])));
						UINT32 *pLine = m_CurrentViewBuffer->Get(nSrcLine);
						if (pLine == NULL) {
							return NCS::CError(NCS_COULDNT_ALLOC_MEMORY, NCS_FUNCTION, NCS::CString().FormatEx(NCS_T("Current View Buffer line %u was null"), nSrcLine));
						}
						UINT32 *pSrc = pLine + static_cast<int>(pOutputImageCoords[0]);
						UINT32 *pDst = pRGBA + (nR * nWidth) + static_cast<int>(outputScreenTLX);

						NCS::SDK::Resample1Line_NearestNeighbour_INT<UINT32>(pDst, pSrc, 0, dDstIncX, (UINT32)(outputScreenBRX - outputScreenTLX), 0);
						nSrcR++;
					}
				}
			}
		}
		return NCS_SUCCESS;
	}

	virtual NCS::CError ReadScanlineFromCurrentView(unsigned int &scanLine, unsigned int *pRGBA) 
	{
		if (scanLine >= m_CurrentView.nSizeY) {
			return NCS::CError(NCS_INVALID_ARGUMENTS, NCS_FUNCTION, NCS::CString().FormatEx(NCS_T("scanline number %u outside view height %u"), scanLine, m_CurrentView.nSizeY));
		}
		UINT32 nLineSize = m_CurrentView.nSizeX * sizeof(UINT32);
		UINT32 *pLine = m_CurrentViewBuffer->Get(scanLine);
		memcpy(pRGBA, pLine, nLineSize);

		scanLine++;
		if (scanLine >= m_CurrentView.nSizeY) {
			scanLine = 0;
		}

		return NCS_SUCCESS;
	}

	virtual NCS::CError Read(NCS::CView *pView, int nBands, int *pBandlist, int nWidth, int nHeight, double dWorldTLX, double dWorldTLY, double dWorldBRX, double dWorldBRY, unsigned int *pRGBA)
	{
		NCS::CMutexLock _Lock(m_Mutex);

		bool bNeedSetView = true;
		NCS::CError Error;

		{
			if((UINT32)nBands == m_CurrentView.nBands && memcmp(pBandlist, m_CurrentView.pBandList, m_CurrentView.nBands*sizeof(INT32)) == 0) {

				if(dWorldBRX == m_CurrentView.fBottomX &&
				   dWorldTLY == m_CurrentView.fLeftY &&
				   dWorldBRY == m_CurrentView.fRightY &&
				   dWorldTLX == m_CurrentView.fTopX &&
				   (UINT32)nWidth == m_CurrentView.nSizeX &&
				   (UINT32)nHeight == m_CurrentView.nSizeY) {

					Error = CopyFullCurrentView(pRGBA);
					if (!Error.Success()) {
						return Error;
					}
					bNeedSetView = false;
				} else {
					Error = CopyPartialCurrentView(nWidth, nHeight, dWorldTLX, dWorldTLY, dWorldBRX, dWorldBRY, pRGBA);
				}
			}
		}
		if(bNeedSetView) {
			UINT64 nSize = nWidth;
			nSize *= nHeight;
			nSize *= sizeof(UINT32);
			
			if (!m_CurrentViewBuffer || !m_TmpViewBuffer || m_CurrentViewBuffer->BufferSize() != nSize) {
				NCS::CMutexLock _Lock(m_TmpViewBufferMutex);
			
				s_BufferCache.Return(m_TmpViewBuffer);
				s_BufferCache.Return(m_CurrentViewBuffer);
				m_TmpViewBuffer = s_BufferCache.Get(nWidth, nHeight);
				m_CurrentViewBuffer = s_BufferCache.Get(nWidth, nHeight);

			}
			Error = pView->SetView(nBands, pBandlist, nWidth, nHeight, dWorldTLX, dWorldTLY, dWorldBRX, dWorldBRY);
		}
		return Error;
	}

	virtual NCSReadStatus RefreshUpdateEx(NCS::CView *pView, NCSSetViewInfo *pViewSetInfo)
	{
		if (pViewSetInfo == NULL) {
			return NCS_READ_FAILED;
		}

		if (m_ViewClosing.TryLock()) {
			NCSReadStatus eReadStatus = NCS_READ_OK;
			try {
				{
					NCS::CMutexLock _Lock(m_TmpViewBufferMutex);
					for (UINT32 nL = 0; nL < pViewSetInfo->nSizeY && eReadStatus == NCS_READ_OK; nL++) {
						UINT32 *pLine = m_TmpViewBuffer->Get(nL);
						if (pLine == NULL) {
							eReadStatus = NCS_READ_FAILED;
						}
						eReadStatus = pView->ReadLineRGBA(pLine);
					}
				}

				if (eReadStatus == NCS_READ_OK) {
					{
						NCS::CMutexLock _Lock(m_Mutex);
						if (m_CurrentView.pBandList) {
							NCSFree(m_CurrentView.pBandList);
						}

						memcpy(&m_CurrentView, pViewSetInfo, sizeof(NCSSetViewInfo));

						m_CurrentView.pBandList = (UINT32 *)NCSMalloc(pViewSetInfo->nBands * sizeof(UINT32), FALSE);
						memcpy(m_CurrentView.pBandList, pViewSetInfo->pBandList, pViewSetInfo->nBands * sizeof(UINT32));

						m_CurrentViewRect.Set(m_CurrentView.fTopX, m_CurrentView.fLeftY, m_CurrentView.fBottomX, m_CurrentView.fRightY);
						m_CurrentViewRect.Normalize();
						std::swap(m_CurrentViewBuffer, m_TmpViewBuffer);
					}

					if (m_pDecoderBaseCallback) {
						CCurrentView CurrentView;
						CurrentView = pViewSetInfo;
						m_pDecoderBaseCallback->RefreshUpdate(&CurrentView);
					}
				}
			}
			catch (std::exception &ex) {
				NCS::CString setViewDetails = m_StringUtils->GetStringFromSetViewInfo(*pViewSetInfo);
				NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_ERROR_LEVEL, NCS_T("Failed to process view '%ls' from dataset '%ls' with error '%hs'"), setViewDetails.c_str(), pView->m_Filename.c_str(), ex.what());
				eReadStatus = NCS_READ_FAILED;
			}
			catch (...) {
				NCS::CString setViewDetails = m_StringUtils->GetStringFromSetViewInfo(*pViewSetInfo);
				NCS::CLog::Log(MOD_ECWSDK_ECWJP2, NCS_ERROR_LEVEL, NCS_T("Failed to process view '%ls' from dataset '%ls' with unknown error"), setViewDetails.c_str(), pView->m_Filename.c_str());
				eReadStatus = NCS_READ_FAILED;
			}
			m_ViewClosing.UnLock();
			return eReadStatus;
		}
		else{
			return NCS_READ_CANCELLED;
		}
	}
	void SetBackgroundColour(UINT32 nBackgroundColour) { m_nBackgroundColour = nBackgroundColour; };
protected:
	NCSSetViewInfo m_CurrentView;
	//Buffer used in preparation for the next RefreshUpdate. 
	std::shared_ptr<NCS::SDK::CRectViewBuffer<UINT32> > m_TmpViewBuffer;
	//Buffer currently in use by RefreshUpdate.  
	std::shared_ptr<NCS::SDK::CRectViewBuffer<UINT32> > m_CurrentViewBuffer;
	
	UINT32 m_nBackgroundColour;
	CNCSRect_D m_CurrentViewRect;

	NCS::CMutex m_Mutex;
	NCS::CMutex m_TmpViewBufferMutex;
	NCS::CMutex m_ViewClosing;
	
	CProgressiveDecoderBase *m_pDecoderBaseCallback;

	NCS::IStringUtilsPtr m_StringUtils;
};

class CDecoderBase::CView : public NCS::CView
{
public:
	CView() : m_pProgressiveView(NULL), NCS::CView() {};
	virtual ~CView() {};

	CView(const CView &s) 
	{
		*this = s;
	}

	CView &operator=(const CView &s)
	{
		((NCS::CView &)*this) = (NCS::CView &)s;

		m_pProgressiveView = s.m_pProgressiveView;

		return *this;
	}

	virtual NCSReadStatus RefreshUpdateEx(NCSSetViewInfo *pViewSetInfo)
	{
		if(m_pProgressiveView) {
			return m_pProgressiveView->RefreshUpdateEx(this, pViewSetInfo);
		}
		return NCS_READ_OK;
	}

	void SetProgressiveDecoder(CCurrentProgressiveView *pProgressiveView)
	{
		m_pProgressiveView = pProgressiveView;
	}
protected:
	CCurrentProgressiveView *m_pProgressiveView;
};

#endif // NCS_API_VIEW_INTERNAL_H
