/********************************************************** 
** Copyright, 1998 - 2013, Intergraph Corporation. All rights reserved.
** 
** FILE:   	NCSRenderer.h
** CREATED:	12 Jan 2000
** AUTHOR: 	Mark Sheridan
** PURPOSE:	C++ class wrappers for the ECW library
** EDITS:
**
** [01] 07Dec00 jmp 
** [02] 12Feb01 jmp 
*******************************************************/
/** @file NCSRenderer.h */

//#if !defined(NCSRENDERER_H)
#ifndef NCSRENDERER_H
#define NCSRENDERER_H

#define TNCSRenderer NCS::TRenderer
#define NCS_HIST_AND_LUT_SUPPORT

#if defined(_WIN32) || defined(_WIN32_WCE)
#define DIB_FIX		//only define for Release with IWS 2.0
#endif //_WIN32

#include "NCSDefs.h"
#include "NCSFile.h"
#include "NCSMutex.h"
#include "NCSUtil.h"

extern "C" INT32 NCSCellSize(NCSCellType eCellType);


#if defined(MACINTOSH)
	#include <Quickdraw.h>
	#define COLORREF DWORD
	#define RECT Rect
	#define LPRECT Rect *
	#define HDC CGrafPtr
#endif

#if defined(_WIN32_WCE)
	#define _WIN32_WINNT 0x0400	// For MB_SERVICE_NOTIFICATION
	#define XMD_H
	#include <stdio.h>
#elif _WIN32
	#ifndef _WIN32_WINNT
	#define _WIN32_WINNT 0x0400	// For MB_SERVICE_NOTIFICATION
	#endif
	#define XMD_H
	#include <stdio.h> /**[04]**/
#if !defined(NCS_NO_JPG_RENDERER) 
	#include "jpeglib.h"
#endif // !NCS_NO_JPG_RENDERER
#elif defined( macintosh )
	#include <stdio.h>
	extern "C" {
		#include "jpeglib.h"
	}
	#include <string.h>

#endif

#define NCS_WIDTH_BYTES(bytes) (bytes+((bytes%4)?(4-bytes%4):0))
#define WIDTHBYTES(bits) (((bits) + 31) / 32 * 4)
#define IS_BETWEEN(a, b, c) (((a>=b)&&(a<=c))||((a>=c)&&(a<=b)))

#ifndef NCSDISPLAYENGINERWIN32_H
/*
** 4x4 Kernel Dither Macros & Defines
*/
#define _MR  32
#define _MG  32
#define _MB  32

#define _DX 4
#define _DY 4
#define _D  (_DX*_DY)
#define MAXC    256

#define _MIX(r,g,b) (((r)*_MG+(g))*_MB+(b))
#define _DITH(C,c,d)    (((unsigned)((_D*(C-1)+1)*c+d)) >> 12)

static UINT16 kernel8[_DY*_DX] = {
	0 * MAXC,  8 * MAXC,  2 * MAXC, 10 * MAXC,
	12 * MAXC,  4 * MAXC, 14 * MAXC,  6 * MAXC,
	3 * MAXC, 11 * MAXC,  1 * MAXC,  9 * MAXC,
	15 * MAXC,  7 * MAXC, 13 * MAXC,  5 * MAXC,
};
static UINT16 __d;

#define KERNEL_DITHER( X, Y, R, G, B )                     \
		(__d = kernel8[(((Y)&3)<<2) | ((X)&3)],             \
			m_pColorTable[_MIX(_DITH(_MR, (R), __d),    \
			_DITH(_MG, (G), __d),    \
			_DITH(_MB, (B), __d))])

#endif // !defined(NCSDISPLAYENGINERWIN32_H)

BOOLEAN IntersectRects(INT32 tlx1, INT32 tly1, INT32 brx1, INT32 bry1, INT32 tlx2, INT32 tly2, INT32 brx2, INT32 bry2);

#if !defined(NCS_NO_JPG_RENDERER) && !defined(_WIN32_WCE)
typedef struct JPEGOutputFuncTable {
	void (*CreateCompressFn)(jpeg_compress_struct *pJpegCompressObject);
	void (*StartCompressFn)(jpeg_compress_struct *pJpegCompressObject, boolean bWriteAllTables);
	void (*FinishCompressFn)(jpeg_compress_struct *pJpegCompressObject);
	void (*DestroyCompressFn)(jpeg_compress_struct *pJpegCompressObject);
	JDIMENSION (*WriteScanlinesFn)(jpeg_compress_struct *pJpegCompressObject, JSAMPARRAY DataArray, JDIMENSION nNrLines);
	void (*StdIODestFn)(jpeg_compress_struct *pJpegCompressObject, FILE *pOutputFile);
	void (*AbortCompressFn)(jpeg_compress_struct *pJpegCompressObject);
	jpeg_error_mgr * (*StdErrorFn)(jpeg_error_mgr * err);
	void (*SetQualityFn)(jpeg_compress_struct *pJpegCompressObject, int nQuality, boolean nForceBaseline);
	void (*SetDefaultsFn)(jpeg_compress_struct *pJpegCompressObject);
} JPEGOutputFuncTable;

void NCSErrorAbort (j_common_ptr cinfo);
void NCS_init_destination (j_compress_ptr cinfo);
boolean NCS_empty_output_buffer(j_compress_ptr cinfo);
void NCS_term_destination(j_compress_ptr cinfo);
#endif //Remove jpeg for wince builds

namespace NCS {

/** @class CRenderer
 *	@brief This class inherits from T and provides display routines for a Win32 environment.
 *
 *	CRenderer uses methods that take a Windows device context (HDC) argument
 *	to print and display views from an ECW file in either blocking or progressive mode.
 *	It is the easiest way to use the ECW JPEG 2000 SDK to get interactive ECW imagery 
 *	display into a native Windows C++ application.
 */
template<class T> class NCS_EXPORT TRenderer : public T
{
public:
	/**
	* Constructor.
	* Create a non transparent NCSRenderer object, set the background color to the system default.
	* @see ~TRenderer()
	*/
	TRenderer() {
		#ifdef _WIN32
			m_hBitmap = NULL;
			m_pbmInfo = (BITMAPINFO*)NULL;
			m_pPaletteEntries = NULL;
			m_pColorTable = NULL;
			m_nBackgroundColor = GetSysColor(COLOR_3DFACE);
			m_hOpenGLDLL = NULL;		/**[03]**/
		#elif defined(MACINTOSH) || defined(MACOSX)
			//m_hPixMap = NULL;
			//m_hLocalMemDC = NULL;
			//m_hCTable = NULL;
		#endif //_WIN32
			m_pRGBA = NULL;
			m_pRGBALocal = NULL;
			m_bUsingAlphaBand = false;
			m_pAlphaBlend = (NCS_FUNCADDR)NULL;
		#ifdef _WIN32
			m_hMSImg32DLL = NCSDlOpen(NCS_T("Msimg32.DLL"));
			if(m_hMSImg32DLL) {
				m_pAlphaBlend = (NCS_FUNCADDR)NCSDlGetFuncAddress(m_hMSImg32DLL, NCS_T("AlphaBlend"));
			}
		#endif

			m_nDCWidth = 0;														/**[07]**/
			m_nDCHeight = 0;													/**[07]**/
			m_nDCBitDepth = 0;													/**[07]**/
			m_bHaveInit = FALSE;
			m_bIsTransparent = TRUE;
			pCurrentViewSetInfo = NULL;
			m_bHaveReadImage = FALSE;
#ifndef POSIX
			m_bCreateNewDIB = TRUE;												/**[10]**/
#endif
		
			m_nRendererWidth = 0;
			m_nRendererHeight = 0;
			m_nRendererDatasetTLX = 0;
			m_nRendererDatasetTLY = 0;
			m_nRendererDatasetBRX = 0;
			m_nRendererDatasetBRY = 0;
			m_dRendererWorldTLX = 0.0;	/**[15]**/
			m_dRendererWorldTLY = 0.0;	/**[15]**/
			m_dRendererWorldBRX = 0.0;	/**[15]**/
			m_dRendererWorldBRY = 0.0;	/**[15]**/
			m_dAdjustedWorldTLX = 0;
			m_dAdjustedWorldTLY = 0;
			m_dAdjustedWorldBRX = 0;
			m_dAdjustedWorldBRY = 0;
			m_nAdjustedViewWidth = 0;
			m_nAdjustedViewHeight = 0;
			m_nRendererXOffset = 0;
			m_nRendererYOffset = 0;
			m_nAdjustedXOffset = 0;
			m_nAdjustedYOffset = 0;
			m_nBytesPerLine = 0;

			m_bAlternateDraw = FALSE;															//[06]

		#ifdef _WIN32
		#if !defined(_WIN32_WCE)
			BOOLEAN bAlternateDraw;																//[06]
			if (NCSPrefGetBoolean(NCS_T("UseAlternateDrawCode"), &bAlternateDraw) == NCS_SUCCESS) {	//[06]
				m_bAlternateDraw = bAlternateDraw;												//[06]
			}

			m_pBitmapImage = NULL;

			//Under Win98 using IE 5.5 SetDIBBitsToDevice doesn't work properly
			//instead need to use BitBlt. (Use wininet.dll to get the browser version).
			wchar_t m_pPath[MAX_PATH + 1 + sizeof("\\wininet.dll")];														
			UINT16 nMajor, nMinor, nRevision, nBuild;									
			if(::GetSystemDirectoryW(m_pPath, MAX_PATH))
			{								
				wcscat_s(m_pPath, NCSWCharArrSize(m_pPath), L"\\wininet.dll");
				NCSFileGetVersion(m_pPath, &nMajor, &nMinor, &nRevision, &nBuild);		

			}	
		#endif
		#endif //_WIN32

			NCSMutexInit(&m_DrawMutex);
			NCSMutexInit(&m_HistogramMutex);
		#ifdef NCS_HIST_AND_LUT_SUPPORT
			m_bCalcHistograms = false;
			m_nReadLine = 0;
			m_pHistograms = NULL;
			m_bApplyLUTs = false;
			m_bLutChanged = false;
			for(int b = 0; b < 3; b++) {
				m_LUTs[0][b] = (UINT8)b;
				m_LUTs[1][b] = (UINT8)b;
				m_LUTs[2][b] = (UINT8)b;
			}
		#endif // NCS_HIST_AND_LUT_SUPPORT
		};
	/**
	* Destructor.
	* Destroy a NCSRenderer object.
	* @see CRenderer()
	*/
	virtual ~TRenderer() {
			if (m_pRGBA)
			{
				NCSFree(m_pRGBA);
				m_pRGBA = NULL;
			}

			if (m_pRGBALocal)/**[04]**/
			{
				NCSFree(m_pRGBALocal);	/**[04]**/
				m_pRGBALocal = NULL;
			}
		#ifdef _WIN32
			DestroyDibAndPalette();

			if(m_hOpenGLDLL) {				/**[03]**/
				FreeLibrary(m_hOpenGLDLL);	/**[03]**/
			}								/**[03]**/
		#elif defined(MACINTOSH) || defined(MACOSX)
			//DestroyPixMapAndPallete();
		#endif
		#ifdef NCS_HIST_AND_LUT_SUPPORT
			NCSFree(m_pHistograms);
		#endif // NCS_HIST_AND_LUT_SUPPORTf
			NCSMutexFini(&m_DrawMutex);
			NCSMutexFini(&m_HistogramMutex);

		#ifdef _WIN32
			if(m_hMSImg32DLL)
			{
				NCSDlClose(m_hMSImg32DLL);
				m_hMSImg32DLL = NULL;
			}
		#endif //_WIN32
		};

		bool UsingAlphaBand() {
			if( m_pAlphaBlend && T::m_pnSetViewBandList && (T::m_nSetViewNrBands > 3) ) {
				if( NCSstrcmp( NCS_BANDDESC_AllOpacity, T::m_Bands[ T::m_pnSetViewBandList[T::m_nSetViewNrBands-1] ].szDesc)==0 ) {
					m_bUsingAlphaBand = true;
					return true;
				}
			}
			m_bUsingAlphaBand = false;
			return false;
		}

		bool HasAlpha( int &nAlphaBand ) {
			nAlphaBand = T::m_iAllOpacityBand;
			return(T::m_iAllOpacityBand >= 0);
		}

		/**
		* Set the view extents.
		* Set the view extents of the renderer object, in world coordinates. If the
		* view extents are out side the dataset extents, it adjusts the extents and draws
		* accordingly. After adjusting the extents, this makes a call to the underlying
		* SetView() in the base NCSFile class.
		* @param nBands The number of bands to view
		* @param pBandList A pointer to a list of band numbers
		* @param nWidth The output device view width
		* @param nHeight The output device view height
		* @param dWorldTLX The world top left X coordinate
		* @param dWorldTLY The world top left Y coordinate
		* @param dWorldBRX The world bottom right X coordinate
		* @param dWorldBRY The world bottom right Y coordinate
		* @return NCS_SUCCESS if successfull, or an NCSError value.
		*/
		virtual CError SetView ( INT32 nBands, INT32 *pBandList, 
						INT32 nWidth, INT32 nHeight,
						IEEE8 dWorldTLX, IEEE8 dWorldTLY,
						IEEE8 dWorldBRX, IEEE8 dWorldBRY )
		{
			NCSError nError = NCS_SUCCESS;
//OutputDebugStringA("S");
			if (T::m_bIsOpen) {

				if (nWidth <= 0 || nHeight <= 0) {
					return NCS_FILE_INVALID_SETVIEW;
				}

				//INT32 nAdjustedDeviceTLX, nAdjustedDeviceTLY, nAdjustedDeviceBRX, nAdjustedDeviceBRY;
				IEEE8 dAdjustedWorldTLX, dAdjustedWorldTLY, dAdjustedWorldBRX, dAdjustedWorldBRY;

				dAdjustedWorldTLX = dWorldTLX;
				dAdjustedWorldTLY = dWorldTLY;
				dAdjustedWorldBRX = dWorldBRX;
				dAdjustedWorldBRY = dWorldBRY;
				INT32 nAdjustedWidth = nWidth;
				INT32 nAdjustedHeight = nHeight;
				m_nAdjustedXOffset = 0;
				m_nAdjustedYOffset = 0;

				if(dAdjustedWorldTLX < OriginX()) {
					double dOriginalDeltaX = dAdjustedWorldBRX - dAdjustedWorldTLX;
					dAdjustedWorldTLX = OriginX();
					double dAdjustedDeltaX = dAdjustedWorldBRX - dAdjustedWorldTLX;
					nAdjustedWidth = (INT32)(((dAdjustedDeltaX / dOriginalDeltaX) * nAdjustedWidth)+0.5);
					m_nAdjustedXOffset = nWidth - nAdjustedWidth;
				}

				double dImageBRX = OriginX()+(CellIncrementX()*Width());
				if(dAdjustedWorldBRX > dImageBRX) {
					double dOriginalDeltaX = dAdjustedWorldBRX - dAdjustedWorldTLX;
					dAdjustedWorldBRX = dImageBRX;
					double dAdjustedDeltaX = dAdjustedWorldBRX - dAdjustedWorldTLX;
					nAdjustedWidth = (INT32)(((dAdjustedDeltaX / dOriginalDeltaX) * nAdjustedWidth)+0.5);
				}

				double dImageBRY = OriginY()+(CellIncrementY()*Height());
				if(CellIncrementY() < 0) {
					if(dAdjustedWorldTLY > OriginY()) {
						double dOriginalDeltaY = dAdjustedWorldTLY - dAdjustedWorldBRY;
						dAdjustedWorldTLY = OriginY();
						double dAdjustedDeltaY = dAdjustedWorldTLY - dAdjustedWorldBRY;
						nAdjustedHeight = (INT32)(((dAdjustedDeltaY / dOriginalDeltaY) * nAdjustedHeight)+0.5);
						m_nAdjustedYOffset = nHeight - nAdjustedHeight;
					}
					if(dAdjustedWorldBRY < dImageBRY) {
						double dOriginalDeltaY = dAdjustedWorldTLY - dAdjustedWorldBRY;
						dAdjustedWorldBRY = dImageBRY;
						double dAdjustedDeltaY = dAdjustedWorldTLY - dAdjustedWorldBRY;
						nAdjustedHeight = (INT32)(((dAdjustedDeltaY / dOriginalDeltaY) * nAdjustedHeight)+0.5);
					}
				} else {
					if(dAdjustedWorldTLY < OriginY()) {
						double dOriginalDeltaY = dAdjustedWorldBRY - dAdjustedWorldTLY;
						dAdjustedWorldTLY = OriginY();
						double dAdjustedDeltaY = dAdjustedWorldBRY - dAdjustedWorldTLY;
						nAdjustedHeight = (INT32)(((dAdjustedDeltaY / dOriginalDeltaY) * nAdjustedHeight)+0.5);
						m_nAdjustedYOffset = nHeight - nAdjustedHeight;
					}
					if(dAdjustedWorldBRY > dImageBRY) {
						double dOriginalDeltaY = dAdjustedWorldBRY - dAdjustedWorldTLY;
						dAdjustedWorldBRY = dImageBRY;
						double dAdjustedDeltaY = dAdjustedWorldBRY - dAdjustedWorldTLY;
						nAdjustedHeight = (INT32)(((dAdjustedDeltaY / dOriginalDeltaY) * nAdjustedHeight)+0.5);
					}
				}

				if (nAdjustedWidth <= 0 || nAdjustedHeight <= 0) {
					return NCS_FILE_INVALID_SETVIEW;
				}

				T::m_bSetViewModeIsWorld = true;

				
				if (nError == NCS_SUCCESS) {
		//			m_bHaveValidSetView = TRUE;
					m_nAdjustedViewWidth = nAdjustedWidth;
					m_nAdjustedViewHeight = nAdjustedHeight;
		//			T::m_nSetViewWidth = m_nAdjustedViewWidth;
		//			T::m_nSetViewHeight = m_nAdjustedViewHeight;
					//m_nAdjustedXOffset = 0;
					//m_nAdjustedYOffset = 0;

					// This is for the output file class (JPEG) so that it can write a "world" file from the adjusted extents.
					m_dAdjustedWorldTLX = dAdjustedWorldTLX;
					m_dAdjustedWorldTLY = dAdjustedWorldTLY;
					m_dAdjustedWorldBRX = dAdjustedWorldBRX;
					m_dAdjustedWorldBRY = dAdjustedWorldBRY;

					if( !T::m_bIsProgressive ) {
						m_dRendererWorldTLX = dAdjustedWorldTLX;
						m_dRendererWorldTLY = dAdjustedWorldTLY;
						m_dRendererWorldBRX = dAdjustedWorldBRX;
						m_dRendererWorldBRY = dAdjustedWorldBRY;
					}

					nError = T::SetView(nBands, pBandList, nAdjustedWidth, nAdjustedHeight, dAdjustedWorldTLX, dAdjustedWorldTLY, dAdjustedWorldBRX, dAdjustedWorldBRY);		
					UsingAlphaBand();

					m_bHaveReadImage = FALSE;
				}
				else
				{
					nError = NCS_ECW_ERROR;							/**[12]**/
				}

				if (nError != NCS_SUCCESS) {
			//		m_bHaveValidSetView = FALSE;
				} else {
		#ifdef NOTDEF
					//NCS_HIST_AND_LUT_SUPPORT
					m_bHaveValidSetView = m_bCalcHistograms ? SetupHistograms() : TRUE;
					if(!m_bHaveValidSetView) {
						nError = NCS_COULDNT_ALLOC_MEMORY;
					}
		#else // NCS_HIST_AND_LUT_SUPPORT
		//			m_bHaveValidSetView = TRUE;
		#endif // NCS_HIST_AND_LUT_SUPPORT
				}
			}
			else {
				nError = NCS_FILE_NOT_OPEN;
			}

			return nError;
		}

		/**
		* Set the view extents.
		* Set the view extents of the renderer object, in dataset units. If the
		* view extents are out side the dataset extents, it adjusts the extents and draws
		* accordingly. After adjusting the extents, this makes a call to the underlying
		* SetView() in the base NCSFile class.
		* @param nBands The number of bands to view
		* @param pBandList A pointer to a list of band numbers
		* @param nWidth The output device view width
		* @param nHeight The output device view height
		* @param nDatasetTLX The dataset top left X coordinate
		* @param nDatasetTLY The dataset top left Y coordinate
		* @param nDatasetBRX The dataset bottom right X coordinate
		* @param nDatasetBRY The dataset bottom right Y coordinate
		* @return NCS_SUCCESS if successfull, or an NCSError value.
		*/
		virtual CError SetView ( INT32 nBands, INT32 *pBandList, 
						INT32 nWidth, INT32 nHeight,
						INT32 nDatasetTLX, INT32 nDatasetTLY,
						INT32 nDatasetBRX, INT32 nDatasetBRY )
		{
			NCS::CError Error;

			if (T::m_bIsOpen) {
				INT32 nAdjustedDatasetTLX, nAdjustedDatasetTLY, nAdjustedDatasetBRX, nAdjustedDatasetBRY;
				INT32 nAdjustedDeviceTLX, nAdjustedDeviceTLY, nAdjustedDeviceBRX, nAdjustedDeviceBRY;

				T::m_bSetViewModeIsWorld = false;

				if (nWidth <= 0 || nHeight <= 0) {
					return NCS_FILE_INVALID_SETVIEW;
				}
				
				if (AdjustExtents(nWidth, nHeight, nDatasetTLX, nDatasetTLY, nDatasetBRX, nDatasetBRY,
							&nAdjustedDatasetTLX, &nAdjustedDatasetTLY, &nAdjustedDatasetBRX, &nAdjustedDatasetBRY,
							&nAdjustedDeviceTLX, &nAdjustedDeviceTLY, &nAdjustedDeviceBRX, &nAdjustedDeviceBRY) == NCS_SUCCESS) {

		//			m_bHaveValidSetView = TRUE;
					m_nAdjustedViewWidth = nAdjustedDeviceBRX - nAdjustedDeviceTLX;
					m_nAdjustedViewHeight = nAdjustedDeviceBRY - nAdjustedDeviceTLY;
		//			T::m_nSetViewWidth = m_nAdjustedViewWidth;
		//			T::m_nSetViewHeight = m_nAdjustedViewHeight;
					m_nAdjustedXOffset = nAdjustedDeviceTLX;
					m_nAdjustedYOffset = nAdjustedDeviceTLY;


					Error = T::SetView(nBands, pBandList, m_nAdjustedViewWidth, m_nAdjustedViewHeight,
											nAdjustedDatasetTLX, nAdjustedDatasetTLY, nAdjustedDatasetBRX, nAdjustedDatasetBRY);
					m_bHaveReadImage = FALSE;
					UsingAlphaBand();
				}

				if(Error != NCS_SUCCESS && Error != NCS_FILEIO_ERROR && Error != NCS_CONNECTION_LOST) {	//[17]
		#ifdef NOTDEF
					//NCS_HIST_AND_LUT_SUPPORT
				} else if(m_bCalcHistograms) {
					if(SetupHistograms() == FALSE) {
						nError = NCS_COULDNT_ALLOC_MEMORY;
					}
		#endif
				}
			}
			return Error;
		}

		/**
		* Read a line in BIL format.
		* Read a line of data, in Band Interleaved by Line format.
		* Optionally collect histograms based on value of m_bCalcHistograms
		* @param ppOutputLine A pointer to a list of buffers, to recieve a scanline of BILL image data
		* @see ReadLineRGB()
		* @see ReadLineBGR()
		* @see SetView()
		* @see ::NCSReadStatus
		* @return ::NCSReadStatus
		*/
		virtual NCSReadStatus ReadLineBIL(NCSCellType eOutputBufferType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL, NCSCellType eOuputDataType = NCSCT_NUMVALUES)
		{
			::NCSReadStatus eStatus = NCS_READ_FAILED;
			INT32 nReadLineWithOffset = m_nReadLine - m_nAdjustedYOffset;
			if(nReadLineWithOffset < m_nAdjustedViewHeight) {
				for(UINT16 nB = 0; nB < nBands; nB++) {
					memset(ppOutputLine[nB], 0x0, m_nAdjustedViewWidth * NCSCellSize(eOutputBufferType));
				}
				if(m_nReadLine < m_nAdjustedYOffset) {
					eStatus = NCS_READ_OK;
				} else if(nReadLineWithOffset < m_nAdjustedViewHeight) {
					if(m_nAdjustedXOffset > 0) {
						INT32 nXOffset = m_nAdjustedXOffset * NCSCellSize(eOutputBufferType);
						for(UINT16 nB = 0; nB < nBands; nB++) {
							UINT8 *pV = ((UINT8*)(ppOutputLine[nB]));
							pV += nXOffset;
						}
						eStatus = T::ReadLineBIL(eOutputBufferType, nBands, ppOutputLine, pLineSteps, eOuputDataType);
						for(UINT16 nB = 0; nB < nBands; nB++) {
							UINT8 *pV = ((UINT8*)(ppOutputLine[nB]));
							pV -= nXOffset;
						}
					} else {
						eStatus = T::ReadLineBIL(eOutputBufferType, nBands, ppOutputLine, pLineSteps, eOuputDataType);
					}
				}				
				m_nReadLine++;
				if(m_nReadLine >= m_nAdjustedViewHeight) {
					m_nReadLine = 0;
				}
			}
			return eStatus;
		}

		virtual ::NCSReadStatus ReadLineBIL (UINT8 **ppOutputLine)
		{
			::NCSReadStatus eStatus = T::ReadLineBIL(ppOutputLine);
		#ifdef NCS_HIST_AND_LUT_SUPPORT	
			if(m_bCalcHistograms && NCS_READ_OK == eStatus) {
				NCSSetViewInfo *pInfo = T::GetFileViewSetInfo();

				INT32 nBands = pInfo->nBands;

				NCSMutexBegin(&m_HistogramMutex);
				if(m_nReadLine++ == 0) {
					if(SetupHistograms() == FALSE) {
						m_nReadLine = 0;
						return(NCS_READ_FAILED);
					}
				}
				for(INT32 x = 0; x < T::m_nSetViewWidth; x++) {
					for(int b = 0; b < nBands; b++) {
						m_pHistograms[b][ppOutputLine[b][x]]++;
					}
				}
				if(m_nReadLine >= T::m_nSetViewHeight) {
					m_nReadLine = 0;
				}
				NCSMutexEnd(&m_HistogramMutex);
			} else if(NCS_READ_OK != eStatus) {
				m_nReadLine = 0;
			}
		#endif // NCS_HIST_AND_LUT_SUPPORT
			return(eStatus);
		}

		/**
		* Read a line in RGB format.
		* Read a line of data, in RGB format.
		* Optionally collect histograms based on value of m_bCalcHistograms
		* @param pRGBTriplet A pointer a buffer, to revieve a scanline of RGB packed image data
		* @see ReadLineBIL()
		* @see ReadLineBGR()
		* @see ::NCSReadStatus
		* @return ::NCSReadStatus
		*/
		virtual ::NCSReadStatus ReadLineRGB (UINT8 *pRGBTriplet)
		{
			::NCSReadStatus eStatus = T::ReadLineRGB(pRGBTriplet);
		#ifdef NCS_HIST_AND_LUT_SUPPORT
			if(m_bCalcHistograms && NCS_READ_OK == eStatus) {
				UINT8 *pRGB = pRGBTriplet;

				NCSMutexBegin(&m_HistogramMutex);
				if(m_nReadLine++ == 0) {
					if(SetupHistograms() == FALSE) {
						m_nReadLine = 0;
						return(NCS_READ_FAILED);
					}
				}
				for(INT32 x = 0; x < T::m_nSetViewWidth; x++) {
					m_pHistograms[0][*(pRGB++)]++;
					m_pHistograms[1][*(pRGB++)]++;
					m_pHistograms[2][*(pRGB++)]++;
				}
				if(m_nReadLine >= T::m_nSetViewHeight) {
					m_nReadLine = 0;
				}
				NCSMutexEnd(&m_HistogramMutex);
			} else if(NCS_READ_OK != eStatus) {
				m_nReadLine = 0;
			}
		#endif // NCS_HIST_AND_LUT_SUPPORT
			return(eStatus);
		}

		/**
		* Read a line in BGR format.
		* Read a line of data, in BGR format.
		* Optionally collect histograms based on value of m_bCalcHistograms
		* @param pRGBTriplet A pointer a buffer, to revieve a scanline of BGR packed image data
		* @see ReadLineRGB()
		* @see ReadLineBIL()
		* @see ::NCSReadStatus
		* @return ::NCSReadStatus
		*/
		virtual ::NCSReadStatus ReadLineBGR (UINT8 *pRGBTriplet)
		{
			::NCSReadStatus eStatus = T::ReadLineBGR(pRGBTriplet);
		#ifdef NCS_HIST_AND_LUT_SUPPORT
			if(m_bCalcHistograms && NCS_READ_OK == eStatus) {
				UINT8 *pBGR = pRGBTriplet;

				NCSMutexBegin(&m_HistogramMutex);
				if(m_nReadLine++ == 0) {
					if(SetupHistograms() == FALSE) {
						m_nReadLine = 0;
						return(NCS_READ_FAILED);
					}
				}

				for(INT32 x = 0; x < T::m_nSetViewWidth; x++) {
					m_pHistograms[2][*(pBGR++)]++;
					m_pHistograms[1][*(pBGR++)]++;
					m_pHistograms[0][*(pBGR++)]++;
				}
				if(m_nReadLine >= T::m_nSetViewHeight) {
					m_nReadLine = 0;
				}
				NCSMutexEnd(&m_HistogramMutex);
			} else if(NCS_READ_OK != eStatus) {
				m_nReadLine = 0;
			}
		#endif // NCS_HIST_AND_LUT_SUPPORT
			return(eStatus);
		}

#if defined( POSIX )
#else
		BOOLEAN	m_bCreateNewDIB;	// need to recreate DIB	/**[02]**/
		/**
		* Read the image into an internal buffer ready for drawing.
		* In progressive mode, when a RefreshUpdate() callback arrives
		* from the network, the client should call ReadImage() to transfer the
		* pending imagery from the network into an internal buffer. Once this
		* is done, the client can then call DrawImage() at any time to draw from
		* the internal buffer into the device. In non-progressive mode the
		* client should call ReadImage(), then immedeately call DrawImage()
		* to draw to the device.
		* @param pViewSetInfo A pointer to the NCSSetViewInfo struct passed to the RefreshUpdate() function
		* @see DrawImage()
		* @see RefreshUpdate()
		* @return NCS_SUCCESS if successfull, or an NCSError value.
		*/
		virtual NCSError ReadImage(NCSSetViewInfo *pViewSetInfo)
		{
			return ReadImage(pViewSetInfo->fTopX, pViewSetInfo->fLeftY, 
							pViewSetInfo->fBottomX, pViewSetInfo->fRightY,
							pViewSetInfo->nTopX, pViewSetInfo->nLeftY, 
							pViewSetInfo->nBottomX, pViewSetInfo->nRightY, 
							pViewSetInfo->nSizeX, pViewSetInfo->nSizeY );
		}

		/**
		* Read the image into an internal buffer ready for drawing.
		* In progressive mode, when a RefreshUpdate() callback arrives
		* from the network, the client should call ReadImage() to transfer the
		* pending imagery from the network into an internal buffer. Once this
		* is done, the client can then call DrawImage() at any time to draw from
		* the internal buffer into the device. In non-progressive mode the
		* client should call ReadImage(), then immedeately call DrawImage()
		* to draw to the device.
		* This overloaded function is called in progressive mode only.
		* @param dWorldTLX The view world top left X coordinate (must match the SetView() top left X).
		* @param dWorldTLY The view world top left Y coordinate (must match the SetView() top left Y).
		* @param dWorldBRX The view world bottom right X coordinate (must match the SetView() bottom right X).
		* @param dWorldBRY The view world bottom right Y coordinate (must match the SetView() bottom right Y).
		* @param nDatasetTLX The dataset top left X coordinate
		* @param nDatasetTLY The dataset top left Y coordinate
		* @param nDatasetBRX The dataset bottom right X coordinate
		* @param nDatasetBRY The dataset bottom right Y coordinate
		* @param nWidth The view width (must match the set view width).
		* @param nHeight The view height (must match the set view height).
		* @see DrawImage()
		* @see RefreshUpdate()
		* @return NCS_SUCCESS if successfull, or an NCSError value.
		*/
		virtual NCSError ReadImage(IEEE8 dWorldTLX, IEEE8 dWorldTLY, 
								   IEEE8 dWorldBRX, IEEE8 dWorldBRY, 
								   INT32 nDatasetTLX, INT32 nDatasetTLY, 
								   INT32 nDatasetBRX, INT32 nDatasetBRY, 
								   INT32 nWidth, INT32 nHeight)
		{//OutputDebugStringA("ReadImage0\r\n");
			//If we are in here we _MUST_ be in progressive mode
		#ifdef _MACINTOSH
			NCSTimeStampMs tsStart = NCSGetTimeStampMs();
		#endif //MACINTOSH

			if (!m_bHaveValidSetView)
			{
				NCS::CString sErrorText = "A valid view has not been set.";
				NCSFormatErrorText(NCS_ECW_ERROR, sErrorText.utf8_str());
				return NCS_ECW_ERROR;
			}

			m_bHaveReadImage = FALSE;

				// Assume the best, that this will not get realloced if the size is the same
			m_pRGBALocal = (UINT8 *)NCSRealloc(m_pRGBALocal, nWidth*nHeight*4, 1);

			::NCSReadStatus eStatus = NCS_READ_OK;
			UINT8 *pCopyRGBA = m_pRGBALocal;

		#if defined(MACINTOSH) || defined(MACOSX)
			for (int i=0; i<nHeight; i++) {	
				eStatus = ReadLineBGRA(pCopyRGBA);
				if (((i%10) == 0) && (i>=10)) {
						NCSThreadYield(); // Yeild this thread every 30 scanlines to free up cpu time for drawing.
				}
				if (eStatus != NCS_READ_OK) {
					break;
				}
				pCopyRGBA += nWidth*4;
			}
		#else
			//if (nHeight > 0)
			{
				SetupHistograms();

				pCopyRGBA += (nHeight-1)*(nWidth*4);
				for (int i=0; i<nHeight; i++) {	
					eStatus = TRenderer<T>::ReadLineBGRA((UINT32*)pCopyRGBA);
					if (((i%10) == 0) && (i>10)) {
						NCSThreadYield(); // Yeild this thread every 10 scanlines to free up cpu time for drawing.
					}
					if (eStatus != NCS_READ_OK) {
						break;
					}
					// calc histogram
					if(m_bCalcHistograms && m_pHistograms){
						UINT8* pRGBA = pCopyRGBA;
						for(INT32 x = 0; x < T::m_nSetViewWidth; x++) {
							// Changed so that all transparent pixels are ignored (not opaque pixels)
							//if(pRGBA[3] == 0){
							if(pRGBA[3] != 0){
								// Reading in BGR, so swap R and B
								m_pHistograms[2][pRGBA[0]]++;
								m_pHistograms[1][pRGBA[1]]++;
								m_pHistograms[0][pRGBA[2]]++;
							}
							pRGBA += 4;
						}
					}

					pCopyRGBA -= nWidth*4;
				}
			}
		#endif

			if (eStatus != NCS_READ_OK) {
				if (eStatus == NCS_READ_CANCELLED) {
					_RPT0(_CRT_WARN, "READ WAS CANCELLED in ReadLineBGR()!\n");
					return NCS_ECW_READ_CANCELLED;
				}
				else if (eStatus == NCS_READ_FAILED) {
					NCSFormatErrorText(NCS_ECW_ERROR, " ReadLineRGB() failed (returned NCS_READ_FAILED).");
					return NCS_ECW_ERROR;
				}
			}
			NCSMutexBegin(&m_DrawMutex);
			
				// Copy the rgb data and size info into the renderer's members
			if (m_pRGBALocal) {
				UINT8 *pTmp = m_pRGBA;
				m_pRGBA = m_pRGBALocal;
				m_pRGBALocal = pTmp;
			}
			m_nAdjustedViewWidth = nWidth;
			m_nAdjustedViewHeight = nHeight;
			
			m_nRendererWidth = nWidth;
			m_nRendererHeight = nHeight;

			m_nRendererDatasetTLX = nDatasetTLX;
			m_nRendererDatasetTLY = nDatasetTLY;
			m_nRendererDatasetBRX = nDatasetBRX;
			m_nRendererDatasetBRY = nDatasetBRY;
			m_dRendererWorldTLX = dWorldTLX;
			m_dRendererWorldTLY = dWorldTLY;
			m_dRendererWorldBRX = dWorldBRX;
			m_dRendererWorldBRY = dWorldBRY;

			m_bHaveReadImage = TRUE;

			m_bCreateNewDIB = TRUE; //force creation of a new DIB to load new image data /**[10]**/

			NCSMutexEnd(&m_DrawMutex);
			return NCS_SUCCESS;
		}

		/**
		* Read the image into an internal buffer ready for drawing.
		* In progressive mode, when a RefreshUpdate() callback arrives
		* from the network, the client should call ReadImage() to transfer the
		* pending imagery from the network into an internal buffer. Once this
		* is done, the client can then call DrawImage() at any time to draw from
		* the internal buffer into the device. In non-progressive mode the
		* client should call ReadImage(), then immedeately call DrawImage()
		* to draw to the device. 
		* This overloaded function is called in non-progressive mode only.
		* @param nWidth The view width (must match the set view width).
		* @param nHeight The view height (must match the set view height).
		* @see DrawImage()
		* @see RefreshUpdate()
		* @return NCS_SUCCESS if successfull, or an NCSError value.
		*/
		virtual NCSError ReadImage(INT32 nWidth, INT32 nHeight)
		{
			NCSError nError = NCS_SUCCESS;

			// If we are here we are _ALWAYS_ in NON-Progressive mode
			if (!m_bHaveValidSetView)
			{
				NCS::CString sErrorText = "A valid view has not been set.";
				NCSFormatErrorText(NCS_ECW_ERROR, sErrorText.utf8_str());
				return NCS_ECW_ERROR;
			}

			m_bHaveReadImage = FALSE;

			if (!T::m_bIsProgressive) {
				nWidth = m_nAdjustedViewWidth;
				nHeight = m_nAdjustedViewHeight;
			}

				// Assume the best, that this will not get realloced if the size is the same
			m_pRGBALocal = (UINT8 *)NCSRealloc(m_pRGBALocal, nWidth*nHeight*4, 1);

			::NCSReadStatus eStatus = NCS_READ_OK;
			UINT8 *pCopyRGBA = m_pRGBALocal;

		#if defined(MACINTOSH) || defined(MACOSX)
			for (int i=0; i<nHeight; i++) {
				eStatus = ReadLineBGRA(pCopyRGBA);

				if (eStatus != NCS_READ_OK) {
					break;
				}
				pCopyRGBA += nWidth*4;
			}
		#else
			pCopyRGBA += (nHeight-1)*(nWidth*4);
			for (int i=0; i<nHeight; i++) {	
				eStatus = TRenderer<T>::ReadLineBGRA((UINT32*)pCopyRGBA);

				if (eStatus != NCS_READ_OK) {
					break;
				}
				pCopyRGBA -= nWidth*4;
			}
		#endif

			if (eStatus != NCS_READ_OK) {
				if (eStatus == NCS_READ_CANCELLED) {
					_RPT0(_CRT_WARN, "READ WAS CANCELLED in ReadLineBGR()!\n");
				}
				else if (eStatus == NCS_READ_FAILED) {
		#ifdef _DEBUG																/**[12]**/
					//::MessageBoxA(NULL, "ReadLineRGB() returned an error!", "NCSFile class", MB_OK);
		#endif																				/**[12]**/
					nError = NCS_ECW_ERROR;													/**[12]**/
					NCSFormatErrorText(NCS_ECW_ERROR, "ReadLineRGB() returned an error!");	/**[12]**/								//[11]
				}
			}

			NCSMutexBegin(&m_DrawMutex);

			m_nRendererWidth = nWidth;
			m_nRendererHeight = nHeight;

			if (m_pRGBALocal) {
				UINT8 *pTmp = m_pRGBA;
				m_pRGBA = m_pRGBALocal;
				m_pRGBALocal = pTmp;
			}

			m_bHaveReadImage = TRUE;
			m_bCreateNewDIB = TRUE; //force creation of a new DIB to load new image data /**[10]**/

			NCSMutexEnd(&m_DrawMutex);

			return nError;
		}

		BOOLEAN CalcStretchBltCoordinates(INT32 nViewWidth, INT32 nViewHeight,
														IEEE8 dTLX, IEEE8 dTLY,
														IEEE8 dBRX, IEEE8 dBRY,
														IEEE8 outputDeviceCoords[4],
														IEEE8 outputImageCoords[4])
		{
			INT32 outputWidth, outputHeight;

			outputWidth = int(outputDeviceCoords[2] - outputDeviceCoords[0]);
			outputHeight = int(outputDeviceCoords[3] - outputDeviceCoords[1]);
			if (outputWidth != (nViewWidth-1) && outputHeight != (nViewHeight-1))
			{
				if ((outputWidth > (nViewWidth-1)) && (outputHeight > (nViewHeight-1)))	//zooming in
				{
					// Calculate the image extents of the sub area to be stretched from the main image, and only stretch that subset to the screen.
					IEEE8 outputScreenTLX = (outputDeviceCoords[0] < (IEEE8)0) ? (IEEE8)0 : outputDeviceCoords[0];
					IEEE8 outputScreenTLY = (outputDeviceCoords[1] < (IEEE8)0) ? (IEEE8)0 : outputDeviceCoords[1];
					IEEE8 outputScreenBRX = (outputDeviceCoords[2] > (IEEE8)(nViewWidth))  ? (IEEE8)(nViewWidth) : outputDeviceCoords[2];
					IEEE8 outputScreenBRY = (outputDeviceCoords[3] > (IEEE8)(nViewHeight)) ? (IEEE8)(nViewHeight) : outputDeviceCoords[3];

					IEEE8 outputImageWidth, outputImageHeight;
					outputImageWidth  = ((outputScreenBRX-outputScreenTLX) * (m_nRendererWidth))/(outputDeviceCoords[2]-outputDeviceCoords[0])  ;
					outputImageHeight = ((outputScreenBRY-outputScreenTLY) * (m_nRendererHeight))/(outputDeviceCoords[3]-outputDeviceCoords[1])  ;
					outputImageCoords[0] = ((outputScreenTLX-outputDeviceCoords[0])  * (outputImageWidth))/(outputScreenBRX-outputScreenTLX) ;
					outputImageCoords[1] = ((outputScreenTLY-outputDeviceCoords[1])  * (outputImageHeight))/(outputScreenBRY-outputScreenTLY) ;
					outputImageCoords[2] = outputImageCoords[0] + outputImageWidth;
					outputImageCoords[3] = outputImageCoords[1] + outputImageHeight;

					// Convert the truncated image coords back into world coords
					outputImageCoords[0] = floor(outputImageCoords[0]);
					outputImageCoords[1] = floor(outputImageCoords[1]);
					outputImageCoords[2] = ceil(outputImageCoords[2]);
					outputImageCoords[3] = ceil(outputImageCoords[3]);
					IEEE8 dNewOutputWorldTLX = m_dRendererWorldTLX + ((m_dRendererWorldBRX - m_dRendererWorldTLX)/((IEEE8)m_nRendererWidth)) * (outputImageCoords[0]);
					IEEE8 dNewOutputWorldTLY = m_dRendererWorldTLY + ((m_dRendererWorldBRY - m_dRendererWorldTLY)/((IEEE8)m_nRendererHeight)) * (outputImageCoords[1]);
					IEEE8 dNewOutputWorldBRX = m_dRendererWorldTLX + ((m_dRendererWorldBRX - m_dRendererWorldTLX)/((IEEE8)m_nRendererWidth)) * (outputImageCoords[2]);
					IEEE8 dNewOutputWorldBRY = m_dRendererWorldTLY + ((m_dRendererWorldBRY - m_dRendererWorldTLY)/((IEEE8)m_nRendererHeight)) * (outputImageCoords[3]);

					// Convert these new world coordinates into device coordinates.
					IEEE8 deviceCoords[4];
					deviceCoords[0] = (((outputDeviceCoords[2] - outputDeviceCoords[0]) * (dNewOutputWorldTLX-m_dRendererWorldTLX))/(m_dRendererWorldBRX - m_dRendererWorldTLX)) + outputDeviceCoords[0];
					deviceCoords[1] = (((outputDeviceCoords[3] - outputDeviceCoords[1]) * (dNewOutputWorldTLY-m_dRendererWorldTLY))/(m_dRendererWorldBRY - m_dRendererWorldTLY)) + outputDeviceCoords[1];
					deviceCoords[2] = (((outputDeviceCoords[2] - outputDeviceCoords[0]) * (dNewOutputWorldBRX-m_dRendererWorldTLX))/(m_dRendererWorldBRX - m_dRendererWorldTLX)) + outputDeviceCoords[0];
					deviceCoords[3] = (((outputDeviceCoords[3] - outputDeviceCoords[1]) * (dNewOutputWorldBRY-m_dRendererWorldTLY))/(m_dRendererWorldBRY - m_dRendererWorldTLY)) + outputDeviceCoords[1];
					outputDeviceCoords[0] = deviceCoords[0];
					outputDeviceCoords[1] = deviceCoords[1];
					outputDeviceCoords[2] = deviceCoords[2];
					outputDeviceCoords[3] = deviceCoords[3];

					return TRUE;
				}
				else	//Zooming out, so stretch whole image to only part of the device view
				{
					outputImageCoords[0] = 0;
					outputImageCoords[1] = 0;
					outputImageCoords[2] = m_nRendererWidth;
					outputImageCoords[3] = m_nRendererHeight;

					return TRUE;
				}
			}

			return FALSE;	//don't use StretchBlt
		}

		/**
		* Calculate the device coordinates of a given view
		* Internal only. Given the current view, calculate the device
		* coordinates of a new view based on the original.
		* PUT PARAMTERS IN!
		* @see AdjustExtents()
		* @return void.
		*/
		void inline CalculateDeviceCoords(
								INT32 nDeviceTLX, INT32 nDeviceTLY,INT32 nDeviceBRX, INT32 nDeviceBRY,
								IEEE8 dWorldTLX, IEEE8 dWorldTLY, IEEE8 dWorldBRX, IEEE8 dWorldBRY,
								IEEE8 dNewWorldTLX, IEEE8 dNewWorldTLY, IEEE8 dNewWorldBRX, IEEE8 dNewWorldBRY,
								INT32 *pnDeviceTLX, INT32 *pnDeviceTLY,	INT32 *pnDeviceBRX, INT32 *pnDeviceBRY)
		{
			*pnDeviceTLX = (INT32)(((dNewWorldTLX - dWorldTLX)/(dWorldBRX-dWorldTLX))*(IEEE8)(nDeviceBRX-nDeviceTLX)+0.5);
			*pnDeviceTLY = (INT32)(((dNewWorldTLY - dWorldTLY)/(dWorldBRY-dWorldTLY))*(IEEE8)(nDeviceBRY-nDeviceTLY)+0.5);
			*pnDeviceBRX = (INT32)(((dNewWorldBRX - dWorldTLX)/(dWorldBRX-dWorldTLX))*(IEEE8)(nDeviceBRX-nDeviceTLX)+0.5);
			*pnDeviceBRY = (INT32)(((dNewWorldBRY - dWorldTLY)/(dWorldBRY-dWorldTLY))*(IEEE8)(nDeviceBRY-nDeviceTLY)+0.5);
		}

		void calculateDeviceCoords(int nDeviceTLX, int nDeviceTLY,
												int nDeviceBRX, int nDeviceBRY,
												double dWorldTLX, double dWorldTLY,
												double dWorldBRX, double dWorldBRY,
												double outputDeviceCoords[4],
												double dRendererWorldTLX, double dRendererWorldTLY,
												double dRendererWorldBRX, double dRendererWorldBRY)
		{
			outputDeviceCoords[0] = ((dRendererWorldTLX - dWorldTLX)/(dWorldBRX-dWorldTLX))*(double)(nDeviceBRX-nDeviceTLX);
			outputDeviceCoords[1] = ((dRendererWorldTLY - dWorldTLY)/(dWorldBRY-dWorldTLY))*(double)(nDeviceBRY-nDeviceTLY);
			outputDeviceCoords[2] = ((dRendererWorldBRX - dWorldTLX)/(dWorldBRX-dWorldTLX))*(double)(nDeviceBRX-nDeviceTLX);
			outputDeviceCoords[3] = ((dRendererWorldBRY - dWorldTLY)/(dWorldBRY-dWorldTLY))*(double)(nDeviceBRY-nDeviceTLY);
		}

		void calculateImageCoords(double dDevice1TLX, double dDevice1TLY,
												double dDevice1BRX, double dDevice1BRY,
												double dImageWidth, double dImageHeight,
												double dDevice2TLX, double dDevice2TLY,
												double dDevice2BRX, double dDevice2BRY,
												double outputImageCoords[4]) {
			double outputImageWidth, outputImageHeight;

			outputImageWidth  = ((dDevice2BRX-dDevice2TLX) * (dImageWidth))/(dDevice1BRX-dDevice1TLX)  ;
			outputImageHeight = ((dDevice2BRY-dDevice2TLY) * (dImageHeight))/(dDevice1BRY-dDevice1TLY)  ;

			outputImageCoords[0] = ((dDevice2TLX-dDevice1TLX)  * (outputImageWidth))/(dDevice2BRX-dDevice2TLX) ;
			outputImageCoords[1] = ((dDevice2TLY-dDevice1TLY)  * (outputImageHeight))/(dDevice2BRY-dDevice2TLY) ;
			outputImageCoords[2] = outputImageCoords[0] + outputImageWidth;
			outputImageCoords[3] = outputImageCoords[1] + outputImageHeight;
		}

		/**
		* Calculate the screen extents that the DrawImage function will draw into
		* Internal only. Given the current view, calculate the device
		* coordinates of a new view based on the original.
		* PUT PARAMTERS IN!
		* @see DrawImage()
		* @return void.
		*/
		void DrawingExtents(LPRECT pClipRect,
										IEEE8 dWorldTLX, IEEE8 dWorldTLY, IEEE8 dWorldBRX, IEEE8 dWorldBRY,
										LPRECT pNewClipRect)		/**[09]**/

		{
			INT32 nDeviceTLX, nDeviceTLY, nDeviceBRX, nDeviceBRY;
			INT32 nDatasetTLX, nDatasetTLY, nDatasetBRX, nDatasetBRY;
			ConvertWorldToDataset(dWorldTLX, dWorldTLY, &nDatasetTLX, &nDatasetTLY);
			ConvertWorldToDataset(dWorldBRX, dWorldBRY, &nDatasetBRX, &nDatasetBRY);

			INT32 dAdjustedDatasetTLX, dAdjustedDatasetTLY, dAdjustedDatasetBRX, dAdjustedDatasetBRY;
			INT32 nAdjustedDeviceTLX, nAdjustedDeviceTLY, nAdjustedDeviceBRX, nAdjustedDeviceBRY;
			AdjustExtents(pClipRect->right - pClipRect->left, pClipRect->bottom - pClipRect->top,
						nDatasetTLX, nDatasetTLY, nDatasetBRX, nDatasetBRY,
						&dAdjustedDatasetTLX, &dAdjustedDatasetTLY, &dAdjustedDatasetBRX, &dAdjustedDatasetBRY,
						&nAdjustedDeviceTLX, &nAdjustedDeviceTLY, &nAdjustedDeviceBRX, &nAdjustedDeviceBRY);

			// Check to see if the renderers current bitmap extents, match this draw, if so do a direct blit
			if ((dAdjustedDatasetTLX == m_nRendererDatasetTLX) && 
				(dAdjustedDatasetTLY == m_nRendererDatasetTLY) &&
				(dAdjustedDatasetBRX == m_nRendererDatasetBRX) && 
				(dAdjustedDatasetBRY == m_nRendererDatasetBRY) &&
				(nAdjustedDeviceBRX - nAdjustedDeviceTLX == m_nRendererWidth) &&
				(nAdjustedDeviceBRY - nAdjustedDeviceTLY == m_nRendererHeight)) {
				
				pNewClipRect->left	 = m_nAdjustedXOffset + pClipRect->left;
				pNewClipRect->top    = m_nAdjustedYOffset + pClipRect->top;
				pNewClipRect->right  = pNewClipRect->left + m_nAdjustedViewWidth;
				pNewClipRect->bottom = pNewClipRect->top  + m_nAdjustedViewHeight;
			}
			else {
		//		INT32 nRendererDatasetWidth = m_nRendererDatasetBRX - m_nRendererDatasetTLX;
		//		INT32 nRendererDatasetHeight = m_nRendererDatasetBRY - m_nRendererDatasetTLY;
		//		INT32 nNewDatasetWidth = dAdjustedDatasetBRX - dAdjustedDatasetTLX;
		//		INT32 nNewDatasetHeight = m_nRendererDatasetBRY - m_nRendererDatasetTLY;

				// Calculate where the old world coordinates from the current rendered image, 
				// fit to the new screen world coordinates.
				CalculateDeviceCoords(pClipRect->left, pClipRect->top, pClipRect->right, pClipRect->bottom,
									dWorldTLX, dWorldTLY, dWorldBRX, dWorldBRY,
									m_dRendererWorldTLX, m_dRendererWorldTLY, m_dRendererWorldBRX, m_dRendererWorldBRY,
									&nDeviceTLX, &nDeviceTLY, &nDeviceBRX, &nDeviceBRY);

				pNewClipRect->left	 = nDeviceTLX + pClipRect->left;
				pNewClipRect->top    = nDeviceTLY + pClipRect->top;
				pNewClipRect->right  = pNewClipRect->left + abs(nDeviceBRX - nDeviceTLX);
				pNewClipRect->bottom = pNewClipRect->top  + abs(nDeviceBRY - nDeviceTLY);
			}
		}

		/**
		* Draw (blit) the internal buffer to the screen.
		* Draw ecw imagery to the screen, using the given extents. The extens do not
		* necessarilly have to match the SetView() extents. If they do, the entire image is
		* drawn. If they don't, only the intersection between the input extents and the
		* amount of imagery in the input buffer is draw.
		* @param hDeviceContext A win32 device context
		* @param pClipRect A pointer to a clip rect describing the width and height of the draw area.
		* @param dWorldTLX The top left X world coordinate of the device
		* @param dWorldTLY The top left Y world coordinate of the device
		* @param dWorldBRX The bottom right X world coordinate of the device
		* @param dWorldBRY The bottom right Y world coordinate of the device
		* @see ReadImage() 
		* @see SetView()
		* @return void.
		*/
		virtual NCSError DrawImage(HDC hDeviceContext, LPRECT pClipRect,
										IEEE8 dWorldTLX, IEEE8 dWorldTLY, IEEE8 dWorldBRX, IEEE8 dWorldBRY )
		{	//		OutputDebugStringA("DrawImage0\r\n");
//OutputDebugStringA("D");
			BOOLEAN bStretch = FALSE;
			BOOLEAN bSpecialBlt = FALSE;	/**[16]**/
			int width, height;
			int nRet;
			NCSError nError = NCS_SUCCESS;

			INT32 nDeviceTLX = 0;
			INT32 nDeviceTLY = 0;
			INT32 nDeviceBRX = 0;
			INT32 nDeviceBRY = 0;

			// Nothing open, nothing to draw
			if (!T::m_bIsOpen) {
				return NCS_FILE_NOT_OPEN;								/**[11]**/
			}

			NCSMutexBegin(&m_DrawMutex);

			width = pClipRect->right - pClipRect->left;
			height = pClipRect->bottom - pClipRect->top;

			
			if (T::m_bIsProgressive) {

					// Calculate where the old world coordinates from the current rendered image, 
					// fit to the new screen world coordinates.
					CalculateDeviceCoords(pClipRect->left, pClipRect->top, pClipRect->right, pClipRect->bottom,
										dWorldTLX, dWorldTLY, dWorldBRX, dWorldBRY,
										m_dRendererWorldTLX, m_dRendererWorldTLY, m_dRendererWorldBRX, m_dRendererWorldBRY,
										&nDeviceTLX, &nDeviceTLY, &nDeviceBRX, &nDeviceBRY);
				// Check to see if the renderers current bitmap extents, match this draw, if so do a direct blit
		#if defined(MACINTOSH) || defined(MACOSX)
				if(0) 	// For the mac always use the stretch case as it does not
						// have seperate draw function.
		#else
				if( ((dWorldTLX == m_dRendererWorldTLX) && 
					(dWorldTLY == m_dRendererWorldTLY) &&
					(dWorldBRX == m_dRendererWorldBRX) && 
					(dWorldBRY == m_dRendererWorldBRY) &&
					(width == m_nRendererWidth) &&
					(height == m_nRendererHeight)))
		#endif		
					{
					width = m_nAdjustedViewWidth;
					height = m_nAdjustedViewHeight;
					bStretch = FALSE;
				}
				else {
				//	INT32 nRendererDatasetWidth = m_nRendererDatasetBRX - m_nRendererDatasetTLX;
				//	INT32 nRendererDatasetHeight = m_nRendererDatasetBRY - m_nRendererDatasetTLY;
				//	INT32 nNewDatasetWidth = dAdjustedDatasetBRX - dAdjustedDatasetTLX;
				//	INT32 nNewDatasetHeight = m_nRendererDatasetBRY - m_nRendererDatasetTLY;

					// Calculate where the old world coordinates from the current rendered image, 
					// fit to the new screen world coordinates.
//					CalculateDeviceCoords(pClipRect->left, pClipRect->top, pClipRect->right, pClipRect->bottom,
//										dWorldTLX, dWorldTLY, dWorldBRX, dWorldBRY,
//										m_dRendererWorldTLX, m_dRendererWorldTLY, m_dRendererWorldBRX, m_dRendererWorldBRY,
//										&nDeviceTLX, &nDeviceTLY, &nDeviceBRX, &nDeviceBRY);

		#ifdef _WIN32
					//if supersampling have to do a stretch
					if (m_nRendererWidth == width && m_nRendererHeight == height)	/**[16]**/
					{
						if(dWorldTLX == m_dRendererWorldTLX && 
							dWorldTLY == m_dRendererWorldTLY &&
							dWorldBRX == m_dRendererWorldBRX && 
							dWorldBRY == m_dRendererWorldBRY) {

							bSpecialBlt = TRUE;
							bStretch = FALSE;
						} else {
							bStretch = TRUE;
						}
					} else
		#endif //_WIN32
						bStretch = TRUE;

					width = m_nAdjustedViewWidth;
					//height = m_nAdjustedViewHeight;
				}
			} else {
				// The non progressive case, just find the actual dataset units,
				//  calculate the actual device coords and do a stretch.

				CalculateDeviceCoords(pClipRect->left, pClipRect->top, pClipRect->right, pClipRect->bottom,
									dWorldTLX, dWorldTLY, dWorldBRX, dWorldBRY,
									m_dAdjustedWorldTLX, m_dAdjustedWorldTLY, m_dAdjustedWorldBRX, m_dAdjustedWorldBRY,
									&nDeviceTLX, &nDeviceTLY, &nDeviceBRX, &nDeviceBRY);

				bStretch = TRUE;
			}

			m_nBytesPerLine = NCS_WIDTH_BYTES(width*3*sizeof(UINT8));

			if (!T::m_bIsProgressive) {
				//ReadImage(m_nAdjustedViewWidth, m_nAdjustedViewHeight);
				ReadImage(m_dAdjustedWorldTLX, m_dAdjustedWorldTLY, m_dAdjustedWorldBRX, m_dAdjustedWorldBRY,
						  nDeviceTLX, nDeviceTLY, nDeviceBRX, nDeviceBRY,
						  m_nAdjustedViewWidth, m_nAdjustedViewHeight);

			}

			if (!m_pRGBA) {
				if (!T::m_bIsProgressive) {
		#if defined(_DEBUG) && !defined(_WIN32_WCE) 								/**[12]**/
					::MessageBoxA(NULL, "There is no RGB information to draw to window. ReadImage() must be called in non progressive mode before drawing can occur.", "NCSRenderer", MB_OK);
		#elif defined(_WIN32_WCE)
					::MessageBox(NULL, L"There is no RGB information to draw to window. ReadImage() must be called in non progressive mode before drawing can occur.", L"NCSRenderer", MB_OK);
		#endif																		/**[12]**/	
					nError = NCS_ECW_ERROR;											/**[12]**/
					NCSFormatErrorText(NCS_ECW_ERROR, "There is no RGB information to draw to window. ReadImage() must be called in non progressive mode before drawing can occur.");	/**[12]**/
				}
				// If its progressive, this just means we got a draw call back before a callback from the ecw lib, so its OK to do nothing.
				NCSMutexEnd(&m_DrawMutex);
				return nError;
			}

		#ifdef _WIN32
			// Copy the rgb buffer into the bitmap and blit it to the dc
			if (T::m_bIsProgressive) {
				CreateDIBAndPallete( hDeviceContext, m_nRendererWidth, m_nRendererHeight);
			} else {
				CreateDIBAndPallete(hDeviceContext, m_nAdjustedViewWidth, m_nAdjustedViewHeight);
			}
			
			int nSavedDC = SaveDC(hDeviceContext);

		#if !defined(_WIN32_WCE)
			SetStretchBltMode(hDeviceContext, COLORONCOLOR);
		#endif


			DWORD dwCaps = GetDeviceCaps(hDeviceContext, RASTERCAPS);
			nRet = GDI_ERROR;

		if(m_bUsingAlphaBand && !m_bAlternateDraw) {
			int nWidthDest, nHeightDest, nXOriginSrc(0), nYOriginSrc(0);
			if(!bStretch) {
				if (bSpecialBlt) {
					nWidthDest = abs(nDeviceBRX - nDeviceTLX);
					nHeightDest = abs(nDeviceBRY - nDeviceTLY);
				}else{
					nWidthDest  = m_nAdjustedViewWidth;
					nHeightDest = m_nAdjustedViewHeight;
				}
			}else{
				
				if (bSpecialBlt) {
					nWidthDest  = (pClipRect->right-pClipRect->left);//abs(nDeviceBRX - nDeviceTLX),
					nHeightDest = (pClipRect->bottom - pClipRect->top);//abs(nDeviceBRY - nDeviceTLY),
					nYOriginSrc = abs(m_pbmInfo->bmiHeader.biHeight) - m_nAdjustedViewHeight;
				}else 
				{	
					nWidthDest = abs(nDeviceBRX - nDeviceTLX);
					nHeightDest = abs(nDeviceBRY - nDeviceTLY);
				}
			}
			
			HDC hMemDC = CreateCompatibleDC(hDeviceContext);
			if(hMemDC) {
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, (HGDIOBJ)m_hBitmap);
				if(TRenderer<T>::StretchBlt(hDeviceContext,
							nDeviceTLX + pClipRect->left,
							nDeviceTLY + pClipRect->top,
							nWidthDest,nHeightDest, 
							hMemDC,
							nXOriginSrc, nYOriginSrc,
							//0, abs(m_pbmInfo->bmiHeader.biHeight) - m_nRendererHeight - 0,
							m_nRendererWidth,
							m_nRendererHeight
							))
				{
					nRet = m_nRendererHeight;
				} else {
					nRet = GDI_ERROR;
				}
				if(hOldBitmap) {
					SelectObject(hMemDC, (HGDIOBJ)hOldBitmap);
				}
				DeleteDC(hMemDC);
			}
		}else{
			if(!bStretch) { 
				if(dwCaps & RC_DIBTODEV && !m_bAlternateDraw) {
					if (bSpecialBlt) {														/**[16]**/
						nRet = SetDIBitsToDevice(hDeviceContext,							/**[16]**/
												nDeviceTLX + pClipRect->left,
												nDeviceTLY + pClipRect->top,
												abs(nDeviceBRX - nDeviceTLX),
												abs(nDeviceBRY - nDeviceTLY),
												0,
												0,
												0,
												abs(m_pbmInfo->bmiHeader.biHeight),
												m_pBitmapImage,
												m_pbmInfo,
												DIB_RGB_COLORS);
					}
					else
					{
						nRet = SetDIBitsToDevice(hDeviceContext,
												m_nAdjustedXOffset + pClipRect->left,
												m_nAdjustedYOffset + pClipRect->top,
												m_nAdjustedViewWidth,
												m_nAdjustedViewHeight,
												0,
												0,
												0,
												abs(m_pbmInfo->bmiHeader.biHeight),
												m_pBitmapImage,
												m_pbmInfo,
												DIB_RGB_COLORS);
					}
				}
				if(nRet == GDI_ERROR) {
					_RPT0(_CRT_WARN,"SetDIBitsToDevice Failed or not available\n");
					HDC hMemDC = CreateCompatibleDC(hDeviceContext);
				
					if(hMemDC) {
						HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, (HGDIOBJ)m_hBitmap);

						if (bSpecialBlt) {													/**[16]**/
							if(BitBlt(hDeviceContext,										/**[16]**/	
									nDeviceTLX + pClipRect->left,
									nDeviceTLY + pClipRect->top, 
									abs(nDeviceBRX - nDeviceTLX),
									abs(nDeviceBRY - nDeviceTLY),
									hMemDC,
									0, 
									abs(m_pbmInfo->bmiHeader.biHeight) - abs(nDeviceBRY - nDeviceTLY) - 0,
									SRCCOPY)) {
								nRet = m_nAdjustedViewHeight;
							} else {
								nRet = GDI_ERROR;
							}
						}
						else {
							if(BitBlt(hDeviceContext, 
									m_nAdjustedXOffset + pClipRect->left,
									m_nAdjustedYOffset + pClipRect->top, 
									m_nAdjustedViewWidth,
									m_nAdjustedViewHeight,
									hMemDC,
									0, 
									abs(m_pbmInfo->bmiHeader.biHeight) - m_nAdjustedViewHeight - 0,
									SRCCOPY)) {
								nRet = m_nAdjustedViewHeight;
							} else {
								nRet = GDI_ERROR;
							}
						}
						if(hOldBitmap) {
							SelectObject(hMemDC, (HGDIOBJ)hOldBitmap);
						}
						DeleteDC(hMemDC);
					}
				}
			} else {
				if(!m_bUsingAlphaBand && dwCaps & RC_STRETCHDIB && !m_bAlternateDraw) {
					nRet = StretchDIBits(hDeviceContext,
										nDeviceTLX + pClipRect->left,
										nDeviceTLY + pClipRect->top,
										abs(nDeviceBRX - nDeviceTLX),
										abs(nDeviceBRY - nDeviceTLY),
										0,
										0,
										m_nRendererWidth,
										m_nRendererHeight,
										m_pBitmapImage,
										m_pbmInfo,
										DIB_RGB_COLORS,
										SRCCOPY);
				}
				if(nRet == GDI_ERROR) {
					//_RPT0(_CRT_WARN,"StretchDIBits Failed or not available\n");
					HDC hMemDC = CreateCompatibleDC(hDeviceContext);
					if(hMemDC) {
						HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, (HGDIOBJ)m_hBitmap);

						if(TRenderer<T>::StretchBlt(hDeviceContext,
									nDeviceTLX + pClipRect->left,
									nDeviceTLY + pClipRect->top,
									abs(nDeviceBRX - nDeviceTLX),
									abs(nDeviceBRY - nDeviceTLY),
									hMemDC,
									0,
									abs(m_pbmInfo->bmiHeader.biHeight) - m_nRendererHeight - 0,
									m_nRendererWidth,
									m_nRendererHeight))
						{
							nRet = m_nRendererHeight;
						} else {
							nRet = GDI_ERROR;
						}
						if(hOldBitmap) {
							SelectObject(hMemDC, (HGDIOBJ)hOldBitmap);
						}
						DeleteDC(hMemDC);
					}
				}
			}
		} //end if	if(m_bUsingAlphaBand && !m_bAlternateDraw) 

			if(nRet == GDI_ERROR) {
				LPVOID lpMsgBuf;
				FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL, 
						GetLastError(),
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL);
				_RPT1(_CRT_WARN,"Stretch/SetDIBits Failed : %s", CHAR_STRING((wchar_t*)lpMsgBuf) );
			}

			// Fill in the background area.
			if (!m_bIsTransparent) {													/**[14]**/											
				if ((m_nAdjustedXOffset != 0) || (m_nAdjustedYOffset !=0) || 
					(m_nAdjustedViewWidth != pClipRect->right - pClipRect->left) || 
					(m_nAdjustedViewHeight != pClipRect->bottom - pClipRect->top)) {

					// Create a pen and solid brush to fill in the background
					HPEN hPen = ::CreatePen(PS_SOLID, 0, m_nBackgroundColor );
					HPEN hPenOld = (HPEN)SelectObject(hDeviceContext, hPen);
					HBRUSH hBrush = CreateSolidBrush(m_nBackgroundColor);
					HBRUSH hBrOld = (HBRUSH)SelectObject(hDeviceContext, hBrush);

					// Clip the image area.
					if (T::m_bIsProgressive)
					{
						RECT excludeRect;
						DrawingExtents(pClipRect, dWorldTLX, dWorldTLY, dWorldBRX, dWorldBRY, &excludeRect);
						ExcludeClipRect(hDeviceContext, excludeRect.left, excludeRect.top, excludeRect.right, excludeRect.bottom);
					}
					else
						ExcludeClipRect(hDeviceContext, nDeviceTLX, nDeviceTLY, nDeviceBRX, nDeviceBRY);

					// Clip the image area.
					/*ExcludeClipRect(hDeviceContext,
							(int)m_nAdjustedXOffset,
							(int)m_nAdjustedYOffset,
							(int)m_nAdjustedXOffset + m_nAdjustedViewWidth,
							(int)m_nAdjustedYOffset + m_nAdjustedViewHeight);*/

					::Rectangle(hDeviceContext, pClipRect->left, pClipRect->top, pClipRect->right, pClipRect->bottom);

					SelectObject(hDeviceContext, hBrOld);
					SelectObject(hDeviceContext, hPenOld);
					DeleteObject(hBrush);
					DeleteObject(hPen);
				}
			}

		#ifdef _DEBUG
					// Draw a RED rectangle around the plugin
		#if !defined(_WIN32_WCE)
					LOGBRUSH brush;
					brush.lbStyle = BS_HOLLOW;
					HBRUSH hBrush = CreateBrushIndirect(&brush);
		#else
					HBRUSH hBrush = CreateSolidBrush(RGB(255, 0, 0));
		#endif
					HPEN hPen = ::CreatePen( PS_SOLID, 1, RGB(255,0,0));
					HPEN hOldPen = (HPEN)SelectObject(hDeviceContext, hPen);
					HBRUSH hOldBrush = (HBRUSH)SelectObject(hDeviceContext, hBrush);

					::Rectangle(hDeviceContext, pClipRect->left, pClipRect->top, pClipRect->right, pClipRect->bottom);

					SelectObject(hDeviceContext, hOldBrush);
					SelectObject(hDeviceContext, hOldPen);
					DeleteObject(hOldBrush);
					DeleteObject(hPen);
		#endif

			RestoreDC(hDeviceContext, nSavedDC);
					
		#endif //_WIN32

		#if defined(MACINTOSH) || defined(MACOSX)
			Rect rectDest; //= ((GrafPtr)gpWindow)->portRect;
			Rect rectSource; //= ((GrafPtr)gpWindow)->portRect;
			long time;
			short i;
			BOOLEAN bResult=TRUE;

			// Copy the rgb buffer into the bitmap and blit it to the dc
			if (T::m_bIsProgressive) {
				//CreateDIBAndPallete( hDeviceContext, m_nRendererWidth, m_nRendererHeight);
				bResult = CreatePixMapAndPallete( (GrafPtr)hDeviceContext, m_nRendererWidth, m_nRendererHeight );
			} else {
				bResult = CreatePixMapAndPallete( (GrafPtr)hDeviceContext, m_nAdjustedViewWidth, m_nAdjustedViewHeight );
			}
			
			if( !bResult )
				return NCS_COULDNT_ALLOC_MEMORY;
			
			/*if( !bStretch ) {
				rectDest.left = m_nAdjustedXOffset + pClipRect->left;
				rectDest.top = m_nAdjustedYOffset + pClipRect->top;
				rectDest.right = m_nAdjustedViewWidth;
				rectDest.bottom = m_nAdjustedViewHeight;
			} else {*/
			
				GetPortBounds(m_hLocalMemDC, &rectSource);
				
				rectDest.left = nDeviceTLX;// + pClipRect->left;
				rectDest.top = nDeviceTLY;// + pClipRect->top;
				rectDest.right = nDeviceBRX;
				rectDest.bottom = nDeviceBRY;
						
			//}

			RGBColor	cblack = { 0x0000, 0x0000, 0x0000 };
			RGBColor	cwhite = { 0xFFFF, 0xFFFF, 0xFFFF };
			RGBForeColor( &cblack );
			RGBBackColor( &cwhite );

			CopyBits( GetPortBitMapForCopyBits( m_hLocalMemDC ), GetPortBitMapForCopyBits( hDeviceContext ),
				&rectSource, &rectDest, srcCopy, 0L);
			
			SInt16 qdError = QDError ();
			
			if( qdError == noErr ) {
				nError = NCS_SUCCESS;
			} else {
				nError = NCS_COULDNT_ALLOC_MEMORY;
			}

		#endif //MACINTOSH
			NCSMutexEnd(&m_DrawMutex);
//OutputDebugStringA("DrawImage1\r\n");
			return nError;
		}

		/**
		* Set the background color.
		* In non-transparent mode, specifies the color of the device background, 
		* which will get filled before drawing occurs.
		* @param nBackgroundColor A 32bit color reference
		* @see SetTransparent() 
		* @see GetTransparent()
		* @return void.
		*/
		virtual void SetBackgroundColor(COLORREF nBackgroundColor)
		{
			m_nBackgroundColor = nBackgroundColor;
		}

#endif //LINUX

		/**
		* Set the transparent property.
		* Specifies wheather or not the renderer is responsible for filling the
		* background before drawing imagery. When integrated with other applications
		* that draw layers, you most likely need to set transparency to FALSE and let
		* the application do the work. If implementing the renderer into a control on
		* its own, setting the transparency to TRUE allows the renderer to erase/paint
		* the client area before drawing, thus minimising the work the container must do.
		* @param bTransparent A boolean specifying the transparency value.
		* @see SetBackgroundColor() 
		* @see GetTransparent()
		* @return void.
		*/
		virtual void SetTransparent(BOOLEAN bTransparent)
		{
			m_bIsTransparent=bTransparent;
		}

		/**
		* Get the transparent property.
		* @param pbTransparent A pointer to a BOOLEAN value that will reveive the property on return
		* @see SetBackgroundColor()
		* @see SetTransparent()
		* @return void.
		*/
		virtual void GetTransparent(BOOLEAN *pbTransparent)
		{
			*pbTransparent = m_bIsTransparent;
		}

		#ifdef NCS_HIST_AND_LUT_SUPPORT

		#if defined(POSIX)
		#	define max(a,b) (b > a ? b : a)
		#endif

		/**
		* Setup the histogram bins.
		* @see CalcHistograms()
		* @see SetView()
		* @return BOOLEAN.
		*/
		virtual BOOLEAN SetupHistograms(void)
		{
			if(m_bCalcHistograms) {
				NCSMutexBegin(&m_HistogramMutex);
				NCSFree(m_pHistograms);
				m_pHistograms = (Histogram*)NCSMalloc(NCSMax(3, T::m_nNumberOfBands) * sizeof(Histogram), TRUE);

				if(!m_pHistograms) {
					NCSMutexEnd(&m_HistogramMutex);
					return(FALSE);
				}
				NCSMutexEnd(&m_HistogramMutex);
			}
			return(TRUE);
		}

		/**
		* Enable/disable histogram calculations.
		* @param bEnable A BOOLEAN value to enable/disable histogram calculations (call before SetView())
		* @see SetupHistograms()
		* @see SetView()
		* @return void.
		*/
		virtual BOOLEAN CalcHistograms(BOOLEAN bEnable)
		{
			m_bCalcHistograms = bEnable;
			return(m_bCalcHistograms);
		}

		/**
		* Get Calculated Histogram for a specific band.
		* @param nBand An INT32 band index to retrieve
		* @param Histogram A UINT32[256] Histogram array to fill
		* @see CalcHistograms()
		* @see SetView()
		* @see ReadLineBIL()
		* @return BOOLEAN.
		*/
		virtual BOOLEAN	GetHistogram(INT32 nBand, UINT32 Histogram[256])
		{
			NCSMutexBegin(&m_HistogramMutex);
			if(m_pHistograms && nBand < NCSMax(3, T::m_nNumberOfBands)) {
				memcpy(Histogram, m_pHistograms[nBand], sizeof(UINT32) * 256);
				NCSMutexEnd(&m_HistogramMutex);
				return(TRUE);
			}
			NCSMutexEnd(&m_HistogramMutex);
			return(FALSE);
		}

		virtual BOOLEAN ApplyLUTs(BOOLEAN bEnable)
		{
			m_bApplyLUTs = bEnable;
			return(m_bApplyLUTs);
		}

		virtual BOOLEAN	SetLUT(INT32 nBand, UINT8 Lut[256])
		{
			bool bChanged = false;
			if(nBand >= 0 && nBand < 3) {
				for(int i = 0; i < 256; i++) {
					if(m_LUTs[nBand][i] != Lut[i]) {
						bChanged = true;
						m_LUTs[nBand][i] = Lut[i];
					}
				}
		//		memcpy(m_LUTs[nBand], Lut, sizeof(Lut));
			} else {
				return(FALSE);
			}
			m_bLutChanged = bChanged;
			return(TRUE);
		}

		#endif // NCS_HIST_AND_LUT_SUPPORT

		virtual void FreeJPEGBuffer(UINT8 *pBuffer) {
			if (pBuffer)
				delete pBuffer;
		}

		virtual CError WriteWorldFile(char *pFilename)
		{
			// ARCView documentation states a world file is the first letter of the extension, 
			// the last letter of the extension and then a "w" so file.tif becomes file.tfw
			// file.bmp becomes file.bpw etc.

			FILE *pWorldFile = NULL;
			char *pWorldFileName = NULL;
			extern int errno;
			INT32 nLen;
			double dCellSizeX, dCellSizeY;
			INT32 nDatasetTLX, nDatasetTLY, nDatasetBRX, nDatasetBRY;

			if (!pFilename) {
				NCSFormatErrorText(NCS_INVALID_ARGUMENTS, "WriteWorldFile()");
				return NCS_INVALID_ARGUMENTS;
			}	

			// Work out the new file name
			pWorldFileName = NCSStrDup(pFilename);
			nLen = (INT32)strlen(pWorldFileName);
			pWorldFileName[nLen-2] = pWorldFileName[nLen-1];
			pWorldFileName[nLen-1] = 'w';
#if defined(WIN32) && !defined(_WIN32_WCE)
			errno_t err = fopen_s(&pWorldFile, pWorldFileName, "w+t");
			if(err != 0) {
				NCS::CString sErrorText;
				sErrorText.Format("Unable to open world file (%hs) errno: %d", pWorldFileName, err);
				return NCS::CError(NCS_INVALID_ARGUMENTS, NCS_FUNCTION, sErrorText);
			}
#else
			pWorldFile = fopen(pWorldFileName, "w+t");
#endif
			// Convert the renderer extents into dataset coordinates
			ConvertWorldToDataset(m_dAdjustedWorldTLX, m_dAdjustedWorldTLY, &nDatasetTLX, &nDatasetTLY);
			ConvertWorldToDataset(m_dAdjustedWorldBRX, m_dAdjustedWorldBRY, &nDatasetBRX, &nDatasetBRY);

			// This is a bit wally, but calculate the cell size based on the underling ecw file cell size
			// as a ratio to the output image size. This doesnt really work for geodetic datasets at the
			// extreme top and bottom of the world. Fix them later.
			dCellSizeX = CellIncrementX() / ((IEEE8)m_nAdjustedViewWidth/(IEEE8)(nDatasetBRX-nDatasetTLX));
			dCellSizeY = CellIncrementY() / ((IEEE8)m_nAdjustedViewHeight/(IEEE8)(nDatasetBRY-nDatasetTLY));

			if (pWorldFile != NULL) {
				NCS::CString sV;

				// Write the x cell size
				sV.Format("%lf\n", dCellSizeX);
				fwrite(sV.utf8_str(), sizeof(char), sV.length(), pWorldFile);
				sV.clear();

				// Rotation for row - not supported yet on ecw files.
				sV.Format("%lf\n", 0.0);
				fwrite(sV.utf8_str(), sizeof(char), sV.length(), pWorldFile);
				sV.clear();

				// Rotation for column - not supported yet on ecw files.
				sV.Format("%lf\n", 0.0);
				fwrite(sV.utf8_str(), sizeof(char), sV.length(), pWorldFile);
				sV.clear();

				// Write the y cell size
				sV.Format("%lf\n", dCellSizeY);
				fwrite(sV.utf8_str(), sizeof(char), sV.length(), pWorldFile);
				sV.clear();

				// Write the x origin value
				sV.Format("%lf\n", m_dAdjustedWorldTLX);
				fwrite(sV.utf8_str(), sizeof(char), sV.length(), pWorldFile);
				sV.clear();

				// Write the y origin value
				sV.Format("%lf\n", m_dAdjustedWorldTLY);
				fwrite(sV.utf8_str(), sizeof(char), sV.length(), pWorldFile);
				sV.clear();

				// Close the file
				fclose(pWorldFile);
			}
			else {
				NCSFormatErrorText(NCS_FILE_OPEN_FAILED, pWorldFileName, "");
				return NCS_FILE_OPEN_FAILED;
			}

			return NCS_SUCCESS;
		}

		static void ShutDown( void )
		{

		}

		virtual IEEE8 OriginX() const { return T::m_dOriginX;}
		virtual IEEE8 OriginY() const { return T::m_dOriginY;}
		virtual IEEE8 CellIncrementX() const { return T::m_dCellIncrementX;}
		virtual IEEE8 CellIncrementY() const { return T::m_dCellIncrementY;}
		virtual INT32 Width() const { return T::m_nWidth;	}
		virtual INT32 Height() const { return T::m_nHeight;	}

		// Need to overwrite the following functions if the underline image rotated or transformed.
		/**
		 *	Rectilinear conversion from world coordinates to dataset coordinates.
		 *
		 *	@param[in]	dWorldX				The world X coordinate.
		 *	@param[in]	dWorldY				The world Y coordinate.
		 *	@param[out]	pnDatasetX			A buffer for the output dataset X coordinate.
		 *	@param[out]	pnDatasetY			A buffer for the output dataset Y coordinate.
		 */
		NCSError ConvertWorldToDataset(IEEE8 dWorldX, IEEE8 dWorldY, INT32 *pnDatasetX, INT32 *pnDatasetY)
		{
			NCSError nError = NCS_SUCCESS;
			if (T::m_bIsOpen) {
				_ASSERT(m_dCellIncrementX!=0.0);
				_ASSERT(m_dCellIncrementY!=0.0);
				*pnDatasetX = (INT32)(((dWorldX - OriginX())/CellIncrementX()) - 0.5);
				*pnDatasetY = (INT32)(((dWorldY - OriginY())/CellIncrementY()) - 0.5);
			}
			else {
				*pnDatasetX = 0;
				*pnDatasetY = 0;
				nError = NCS_ECW_ERROR;
			}
			return nError;
		}

		/**
		 *	Rectilinear conversion from dataset coordinates to world coordinates.
		 *
		 *	@param[in]	nDatasetX			The dataset X coordinate.
		 *	@param[in]	nDatasetY			The dataset Y coordinate.
		 *	@param[out]	pdWorldX			A buffer for the output world X coordinate.
		 *	@param[out]	pdWorldY			A buffer for the output world Y coordinate.
		 */
		NCSError ConvertDatasetToWorld(INT32 nDatasetX, INT32 nDatasetY, IEEE8 *pdWorldX, IEEE8 *pdWorldY)
		{
			NCSError nError = NCS_SUCCESS;
			if (T::m_bIsOpen) {
				*pdWorldX = OriginX() + ((IEEE8)nDatasetX * CellIncrementX());
				*pdWorldY = OriginY() + ((IEEE8)nDatasetY * CellIncrementY());
			}
			else {
				*pdWorldX = 0.0;
				*pdWorldY = 0.0;
				nError = NCS_ECW_ERROR;
			}
			return nError;
		}

protected:
#ifdef _WIN32
		bool StretchBlt(
			HDC hdcDest,      // handle to destination DC
			int nXOriginDest, // x-coord of destination upper-left corner
			int nYOriginDest, // y-coord of destination upper-left corner
			int nWidthDest,   // width of destination rectangle
			int nHeightDest,  // height of destination rectangle
			HDC hdcSrc,       // handle to source DC
			int nXOriginSrc,  // x-coord of source upper-left corner
			int nYOriginSrc,  // y-coord of source upper-left corner
			int nWidthSrc,    // width of source rectangle
			int nHeightSrc   // height of source rectangle
			)
		{
#ifndef _WIN32_WCE
			if( m_pAlphaBlend && m_bUsingAlphaBand ) {
				BLENDFUNCTION bf;
				bf.BlendOp = AC_SRC_OVER;
				bf.BlendFlags = 0;
				bf.SourceConstantAlpha = 0xFF;  // 0x00 (transparent) through 0xFF (opaque)
												// want bitmap alpha, so no constant.
				bf.AlphaFormat = AC_SRC_ALPHA;  // Use bitmap alpha

				return (*(BOOL(__stdcall *)(HDC, int, int, int, int, HDC, int, int, int, int, BLENDFUNCTION))m_pAlphaBlend)
					(hdcDest,
					nXOriginDest,
					nYOriginDest,
					nWidthDest,
					nHeightDest,
					hdcSrc,
					nXOriginSrc,
					nYOriginSrc,
					nWidthSrc,
					nHeightSrc,
					bf)?true:false;
			} else {
#endif
				return ::StretchBlt(hdcDest,
					nXOriginDest,
					nYOriginDest,
					nWidthDest,
					nHeightDest,
					hdcSrc,
					nXOriginSrc,
					nYOriginSrc,
					nWidthSrc,
					nHeightSrc,
					SRCCOPY)?true:false;
#ifndef _WIN32_WCE
			}
#endif
		}

		BOOLEAN CreateDIBAndPallete(HDC hDeviceContext, INT32 nWidth, INT32 nHeight)
		{
			INT32 nColors = 0;
			BOOLEAN bChangeInSize = FALSE;
		#ifdef DIB_FIX
			INT32 nPrevDCBitDepth = m_nDCBitDepth;	//save previous depth for comparison	/**[10]**/
		#endif

			if ((m_nDCWidth!= nWidth) ||(m_nDCHeight != nHeight) || (m_pbmInfo == NULL)
		#ifdef NCS_HIST_AND_LUT_SUPPORT
				|| (m_bLutChanged && m_bApplyLUTs)
		#endif // NCS_HIST_AND_LUT_SUPPORT
				) {
				bChangeInSize = TRUE;
			}

			m_nDCWidth = nWidth;
			m_nDCHeight = nHeight;

			if( m_bUsingAlphaBand ) {
				// Always use a bit depth of 32 and let the Blit functions handle the conversion 
				m_nDCBitDepth = 32;
			} else {
				if(GetDeviceCaps(hDeviceContext, TECHNOLOGY) == DT_RASPRINTER) {
					// Printing
					m_nDCBitDepth = 24;
				} else {
					m_nDCBitDepth = GetDeviceCaps(hDeviceContext, PLANES) * GetDeviceCaps(hDeviceContext, BITSPIXEL);
					if(m_nDCBitDepth <= 8) {
						nColors = 256;
						m_nDCBitDepth = 8;
					}
				}
			}

			if (m_nDCBitDepth != nPrevDCBitDepth)
				bChangeInSize = TRUE;

			if (bChangeInSize) {
				if(m_hBitmap) {
					DeleteObject(m_hBitmap);
					m_hBitmap = NULL;
				}
				if(m_pbmInfo) {
					NCSFree(m_pbmInfo);
					m_pbmInfo = (BITMAPINFO*)NULL;
				}
			}	

		#ifdef DIB_FIX
			//If the bit depth hasn't changed and no new image data has arrived
			//we don't have to do anything further in this function.
			if (bChangeInSize == FALSE && nPrevDCBitDepth == m_nDCBitDepth && m_bCreateNewDIB == FALSE)		/**[10]**/
				return TRUE;														/**[10]**/	
		#endif

			if (bChangeInSize) {
				INT32 nHeaderSize = sizeof(BITMAPINFOHEADER) + nColors * sizeof(RGBQUAD);
				m_pbmInfo = (BITMAPINFO*)NCSMalloc(nHeaderSize + 3 * sizeof(DWORD), TRUE);
			}
			if(!m_pbmInfo) {
				DestroyDibAndPalette();
				return(FALSE);
			}
				// Note: Extra because of BITFIELDS for 16bit format
				// dib_header_size gets reset below if this format.
			m_pbmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
			m_pbmInfo->bmiHeader.biWidth = nWidth;
		#ifndef DIB_FIX
			m_pbmInfo->bmiHeader.biHeight = -nHeight;
		#else
			m_pbmInfo->bmiHeader.biHeight = nHeight;
		#endif
			m_pbmInfo->bmiHeader.biPlanes = 1;
			m_pbmInfo->bmiHeader.biBitCount = (WORD)m_nDCBitDepth;
			m_pbmInfo->bmiHeader.biSizeImage = NCS_WIDTH_BYTES((DWORD)nWidth * m_nDCBitDepth) * nHeight;
			m_pbmInfo->bmiHeader.biCompression = BI_RGB;
			
		#if !defined(_WIN32_WCE)
			if(m_nDCBitDepth == 16) {
				PIXELFORMATDESCRIPTOR pfd;
				HDC hPFDHDC = hDeviceContext;
				int i = 1;
				pfd.nSize = sizeof(pfd);

				/*
				** On some drivers DescribePixelFormat() always fails on memory DC's,
				** so we put this hacky check in to see if it's really 15bit (RGB555).
				** There doesn't seem to be any other way to find this out.
				*/
				if(!m_hOpenGLDLL) {
					/*
					** This may seem pretty pointless as we don't use the instance handle at all.
					** In fact, this is an optimisation to prevent GDI doing it itself every time
					** DescribePixelFormat() is called.  Otherwise opengl32.dll is loaded/unloaded 
					** every time we call DescribePixelFormat() - very sloooow.
					*/
					m_hOpenGLDLL = LoadLibraryW(NCS_T("opengl32.dll"));
				}
				if(DescribePixelFormat(hPFDHDC,
									i,
									sizeof(PIXELFORMATDESCRIPTOR),
									&pfd) == 0) {
					COLORREF cOrig;
					COLORREF cDst;
					RECT rect;

					GetClipBox(hDeviceContext, &rect);

					cOrig = GetPixel(hDeviceContext, rect.left, rect.top);
					cDst = SetPixel(hDeviceContext, rect.left, rect.top, RGB(0x00, 0x04, 0x00));
					if(cDst != 0x04) {
						m_nDCBitDepth = 15;
					}
					SetPixel(hDeviceContext, rect.left, rect.top, cOrig);
				} else {
					while(DescribePixelFormat(hPFDHDC,
											i,
											sizeof(PIXELFORMATDESCRIPTOR),
											&pfd)) {
						if(pfd.iPixelType == PFD_TYPE_RGBA) {
							INT32 bits = pfd.cRedBits + pfd.cGreenBits + pfd.cBlueBits;

							if(bits == 15) {
								/*
								** 5/5/5 RGB
								*/
								m_nDCBitDepth = 15;
								break;
							} else if(bits == 16) {
								/*
								** 5/6/5 RGB
								*/
								break;
							}
						}
						i++;
					}
				}
				if(hPFDHDC != hDeviceContext) {
					ReleaseDC(NULL, hPFDHDC);
					hPFDHDC = hDeviceContext;
				}
			}
		#endif

			if(m_nDCBitDepth == 16) {
				DWORD *pColors = (DWORD*)m_pbmInfo->bmiColors;
					
				m_pbmInfo->bmiHeader.biCompression = BI_BITFIELDS;

				/*
				** This is the only 16bit bitfield format supported under win95.
				*/
				pColors[0] = 0xf800;	/* 5 Red */
				pColors[1] = 0x07e0;	/* 6 Green */
				pColors[2] = 0x001f;	/* 5 Blue */
			} else if(m_nDCBitDepth <= 8) {
				LOGPALETTE lPalette;
				HPALETTE hPalette = NULL;

				lPalette.palNumEntries = 1;
				lPalette.palVersion = 0x300;
				lPalette.palPalEntry[0].peRed = 0;
				lPalette.palPalEntry[0].peGreen = 0;
				lPalette.palPalEntry[0].peBlue = 0;
				lPalette.palPalEntry[0].peFlags = NULL;

				if((hPalette = CreatePalette(&lPalette)) != NULL) {
					PALETTEENTRY peEntries[256];

					HPALETTE hOldPalette = SelectPalette(hDeviceContext, hPalette, TRUE);

					memset(peEntries, 0, sizeof(peEntries));
					GetPaletteEntries(hOldPalette, 0, 256, peEntries);

					for(int i = 0; i < 256; i++) {
						m_pbmInfo->bmiColors[i].rgbRed = peEntries[i].peRed;
						m_pbmInfo->bmiColors[i].rgbGreen = peEntries[i].peGreen;
						m_pbmInfo->bmiColors[i].rgbBlue = peEntries[i].peBlue;
						m_pbmInfo->bmiColors[i].rgbReserved = 0;

						if(m_pPaletteEntries && ((peEntries[i].peRed != m_pPaletteEntries[i].peRed) ||
												(peEntries[i].peGreen != m_pPaletteEntries[i].peGreen) ||
												(peEntries[i].peBlue != m_pPaletteEntries[i].peBlue) || 
												(peEntries[i].peFlags != m_pPaletteEntries[i].peFlags))) {
							if(m_pColorTable) {
								NCSFree(m_pColorTable);
								m_pColorTable = NULL;
							}
						}
					}
					
					if(!m_pPaletteEntries) {
						m_pPaletteEntries = (PALETTEENTRY*)NCSMalloc(sizeof(peEntries), TRUE);
					}
					if(!m_pColorTable) {
						if((m_pColorTable = (UINT8*)NCSMalloc(65536, FALSE)) != NULL) {
							int r, g, b;

							/* initialize color_table[], red_table[], etc */
							for (r = 0; r < _MR; r++) {
								UINT32 red = (INT32)(((double)r / (double)(_MR - 1)) * 255.0);

								for (g = 0; g < _MG; g++) {
									UINT32 green = (INT32)(((double)g / (double)(_MG - 1)) * 255.0);

									for (b = 0; b < _MB; b++) {
										UINT32 blue = (INT32)(((double)b / (double)(_MB - 1)) * 255.0);
															
										m_pColorTable[_MIX( r, g, b )] = (UINT8)GetNearestPaletteIndex(hOldPalette, 
																							RGB(red, green, blue));
									}
								}
							}
							memcpy(m_pPaletteEntries, peEntries, sizeof(peEntries));
						} else {
							DestroyDibAndPalette();
							return(FALSE);
						}
					}
					SelectPalette(hDeviceContext, hOldPalette, TRUE);
					hOldPalette = NULL;
					DeleteObject(hPalette);
					hPalette = NULL;
				}
				m_nDCBitDepth = 8;
			}
			if (bChangeInSize) {
				m_hBitmap = CreateDIBSection(hDeviceContext,
											m_pbmInfo,
											m_nDCBitDepth <= 8 ? DIB_PAL_COLORS : DIB_RGB_COLORS,
											(void **)&m_pBitmapImage,
											NULL, NULL);
			}

			if( !m_hBitmap || !m_pBitmapImage ) {
				return(FALSE);
			}

		#ifdef DIB_FIX
		//Only copy triplets to bitmap if bit depth changed from last call to this function
		//or new image data has arrived
		if (bChangeInSize == TRUE || nPrevDCBitDepth != m_nDCBitDepth || m_bCreateNewDIB == TRUE)		/**[10]**/
		{
		#endif
			switch(m_nDCBitDepth) {
				case 32:
		#ifndef NCS_HIST_AND_LUT_SUPPORT
						for(INT32 nLine = 0; nLine < nHeight; nLine++) {
							UINT8	*pRGBTriplets = (UINT8*)&(m_pRGBTriplets[nWidth * 3 * nLine]);
							UINT32	*pImagePtr32 = (UINT32*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 32) * nLine]);

							for(INT32 nCell = 0; nCell < nWidth; nCell++) {
								*(pImagePtr32++) = (UINT32)(*pRGBTriplets |
															(*(pRGBTriplets + 1) << 8) |
															(*(pRGBTriplets + 2) << 16));
								pRGBTriplets += 3;
							}
						}
		#else // NCS_HIST_AND_LUT_SUPPORT
					{
						if(!m_bApplyLUTs && !m_bUsingAlphaBand) {
							// Just do a mem copy
							for(INT32 nLine = 0; nLine < nHeight; nLine++) {
								UINT8	*pRGBA = (UINT8*)&(m_pRGBA[nWidth * 4 * nLine]);
								UINT32	*pImagePtr32 = (UINT32*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 32) * nLine]);

								memcpy( pImagePtr32, pRGBA, nWidth*4 );
							}
						} else {
							int i;
							UINT8 rLut[256];
							UINT8 gLut[256];
							UINT8 bLut[256];

							if(m_bApplyLUTs) {
								for(i = 0; i < 256; i++) {
									rLut[i] = (m_LUTs[2][i]);
									gLut[i] = (m_LUTs[1][i]);
									bLut[i] = (m_LUTs[0][i]);			
								}
							} else {
								for(i = 0; i < 256; i++) {
									rLut[i] = (UINT8)i;
									gLut[i] = (UINT8)i;
									bLut[i] = (UINT8)i;			
								}
							}
							for(INT32 nLine = 0; nLine < nHeight; nLine++) {
								UINT8	*pRGBA = (UINT8*)&(m_pRGBA[nWidth * 4 * nLine]);
								UINT32	*pImagePtr32 = (UINT32*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 32) * nLine]);

								for(INT32 nCell = 0; nCell < nWidth; nCell++) {
									UINT32 r = 0;
									UINT32 g = 0;
									UINT32 b = 0;
									UINT32 a = 0xFF;
									if( m_bUsingAlphaBand ) {
										a = *(pRGBA + 3);
										r = (rLut[(*pRGBA)]*a)/0xff;
										g = (gLut[(*(pRGBA + 1))]*a)/0xff;
										b = (bLut[(*(pRGBA + 2))]*a)/0xff;
									} else {
										r = rLut[(*pRGBA)];
										g = gLut[(*(pRGBA + 1))];
										b = bLut[(*(pRGBA + 2))];
									}

									*(pImagePtr32++) = (UINT32)( r | (g << 8) | (b << 16) | (a << 24) );

									pRGBA += 4;
								}
							}
						}
					}
		#endif // NCS_HIST_AND_LUT_SUPPORT
					break;
					
				case 24:
				default:
		#ifndef NCS_HIST_AND_LUT_SUPPORT
						for(INT32 nLine = 0; nLine < nHeight; nLine++) {
							UINT8 *pRGBTriplets = (UINT8*)&(m_pRGBTriplets[nWidth * 3 * nLine]);
							UINT8 *pImagePtr8 = (UINT8*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 24) * nLine]);

							memcpy(pImagePtr8, pRGBTriplets, nWidth * 3);
						}
		#else // NCS_HIST_AND_LUT_SUPPORT
						UINT16 rLut[256];
						UINT16 gLut[256];
						UINT16 bLut[256];	

						if(m_bApplyLUTs) {				
							for(int i = 0; i < 256; i++) {
								rLut[i] = ((UINT16)(m_LUTs[2][i])) >> 3;
								gLut[i] = (((UINT16)(m_LUTs[1][i])) & 0xf8) << 3;
								bLut[i] = (((UINT16)(m_LUTs[0][i])) & 0xf8) << 8;			
							}
						} else {
							for(int i = 0; i < 256; i++) {
								rLut[i] = (UINT16)i >> 3;
								gLut[i] = ((UINT16)i & 0xf8) << 3;
								bLut[i] = ((UINT16)i & 0xf8) << 8;			
							}
						}

						for(INT32 nLine = 0; nLine < nHeight; nLine++) {
							UINT8 *pRGBA = (UINT8*)&(m_pRGBA[nWidth * 4 * nLine]);
							UINT8 *pImagePtr8 = (UINT8*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 24) * nLine]);

							for(INT32 nCell = 0; nCell < nWidth; nCell++) {
								*(pImagePtr8) = (UINT8)rLut[*(pRGBA)];
								*(pImagePtr8 + 1) = (UINT8)gLut[*(pRGBA + 1)];
								*(pImagePtr8 + 2) = (UINT8)bLut[*(pRGBA + 2)];
								pImagePtr8 += 3;
								pRGBA+=4;
							}
						}
		#endif // NCS_HIST_AND_LUT_SUPPORT
					break;

				case 16:
		#ifndef NCS_HIST_AND_LUT_SUPPORT
							for(INT32 nLine = 0; nLine < nHeight; nLine++) {
								UINT8 *pRGBTriplets = (UINT8*)&(m_pRGBTriplets[nWidth * 3 * nLine]);
								UINT16 *pImagePtr16 = (UINT16*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 16) * nLine]);

								for(INT32 nCell = 0; nCell < nWidth; nCell++) {
									*(pImagePtr16++) = (UINT16)((*pRGBTriplets >> 3) |
																((*(pRGBTriplets + 1) & 0xf8) << 3) |
																((*(pRGBTriplets + 2) & 0xf8) << 8));
									pRGBTriplets += 3;
								}
							}
		#else // NCS_HIST_AND_LUT_SUPPORT
					{
						UINT16 rLut[256];
						UINT16 gLut[256];
						UINT16 bLut[256];	

						if(m_bApplyLUTs) {				
							for(int i = 0; i < 256; i++) {
								rLut[i] = ((UINT16)(m_LUTs[2][i])) >> 3;
								gLut[i] = (((UINT16)(m_LUTs[1][i])) & 0xf8) << 3;
								bLut[i] = (((UINT16)(m_LUTs[0][i])) & 0xf8) << 8;			
							}
						} else {
							for(int i = 0; i < 256; i++) {
								rLut[i] = (UINT16)i >> 3;
								gLut[i] = ((UINT16)i & 0xf8) << 3;
								bLut[i] = ((UINT16)i & 0xf8) << 8;			
							}
						}

						for(INT32 nLine = 0; nLine < nHeight; nLine++) {
							UINT8 *pRGBA = (UINT8*)&(m_pRGBA[nWidth * 4 * nLine]);
							UINT16 *pImagePtr16 = (UINT16*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 16) * nLine]);

							for(INT32 nCell = 0; nCell < nWidth; nCell++) {
								INT32 r = *pRGBA;
								INT32 g = *(pRGBA + 1);
								INT32 b = *(pRGBA + 2);

								*(pImagePtr16++) = (UINT16)(rLut[r] | gLut[g] | bLut[b]);
								pRGBA += 4;
							}
						}
					}
		#endif // NCS_HIST_AND_LUT_SUPPORT
					break;

				case 15:
		#ifndef NCS_HIST_AND_LUT_SUPPORT
						for(INT32 nLine = 0; nLine < nHeight; nLine++) {
							UINT8 *pRGBA = (UINT8*)&(m_pRGBA[nWidth * 3 * nLine]);
							UINT16 *pImagePtr16 = (UINT16*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 16) * nLine]);

							for(INT32 nCell = 0; nCell < nWidth; nCell++) {
								*(pImagePtr16++) = (UINT16)((*pRGBA >> 3) |
															((*(pRGBA + 1) & 0xf8) << 2) |
															((*(pRGBA + 2) & 0xf8) << 7));
								pRGBA += 4;
							}
						}
		#else // NCS_HIST_AND_LUT_SUPPORT
					{
						UINT16 rLut[256];
						UINT16 gLut[256];
						UINT16 bLut[256];	

						if(m_bApplyLUTs) {				
							for(int i = 0; i < 256; i++) {
								rLut[i] = ((UINT16)(m_LUTs[2][i])) >> 3;
								gLut[i] = (((UINT16)(m_LUTs[1][i])) & 0xf8) << 2;
								bLut[i] = (((UINT16)(m_LUTs[0][i])) & 0xf8) << 7;			
							}
						} else {
							for(int i = 0; i < 256; i++) {
								rLut[i] = (UINT16)i >> 3;
								gLut[i] = ((UINT16)i & 0xf8) << 2;
								bLut[i] = ((UINT16)i & 0xf8) << 7;			
							}
						}

						for(INT32 nLine = 0; nLine < nHeight; nLine++) {
							UINT8 *pRGBA = (UINT8*)&(m_pRGBA[nWidth * 4 * nLine]);
							UINT16 *pImagePtr16 = (UINT16*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 16) * nLine]);

							for(INT32 nCell = 0; nCell < nWidth; nCell++) {
								INT32 r = *pRGBA;
								INT32 g = *(pRGBA + 1);
								INT32 b = *(pRGBA + 2);

								*(pImagePtr16++) = (UINT16)(rLut[r] | gLut[g] | bLut[b]);
								pRGBA += 4;
							}
						}
					}
		#endif // NCS_HIST_AND_LUT_SUPPORT
					break;

				case 8:
		#ifdef NCS_HIST_AND_LUT_SUPPORT
						if(m_bApplyLUTs) {
							for(INT32 nLine = 0; nLine < nHeight; nLine++) {
								UINT8	*pRGBA = (UINT8*)&(m_pRGBA[nWidth * 4 * nLine]);
								UINT8	*pImagePtr8 = (UINT8*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 8) * nLine]);

								for(INT32 nCell = 0; nCell < nWidth; nCell++) {
									UINT8 b = *pRGBA;
									UINT8 g = *(pRGBA + 1);
									UINT8 r = *(pRGBA + 2);

									pRGBA += 4;

									*(pImagePtr8++) = KERNEL_DITHER(nCell, nLine, m_LUTs[2][r], m_LUTs[1][g], m_LUTs[0][b]);
								}
							}
						} else {
		#endif // NCS_HIST_AND_LUT_SUPPORT
							for(INT32 nLine = 0; nLine < nHeight; nLine++) {
								UINT8	*pRGBA = (UINT8*)&(m_pRGBA[nWidth * 4 * nLine]);
								UINT8	*pImagePtr8 = (UINT8*)&(m_pBitmapImage[WIDTHBYTES(nWidth * 8) * nLine]);

								for(INT32 nCell = 0; nCell < nWidth; nCell++) {
									UINT8 b = *pRGBA;
									UINT8 g = *(pRGBA + 1);
									UINT8 r = *(pRGBA + 2);

									pRGBA += 4;

									*(pImagePtr8++) = KERNEL_DITHER(nCell, nLine, r, g, b);
								}
							}	
		#ifdef NCS_HIST_AND_LUT_SUPPORT
						}
		#endif // NCS_HIST_AND_LUT_SUPPORT
						break;
			}
		#ifdef DIB_FIX
		}
		#endif

		#ifdef DIB_FIX
			m_bCreateNewDIB = FALSE;								/**[10]**/
		#endif
		#ifdef NCS_HIST_AND_LUT_SUPPORT
			m_bLutChanged = FALSE;
		#endif // NCS_HIST_AND_LUT_SUPPORT
			return(TRUE);
		}

		BOOLEAN DestroyDibAndPalette(void)
		{
			if(m_hBitmap) {
				DeleteObject(m_hBitmap);
				m_hBitmap = NULL;
			}
			if(m_pbmInfo) {
				NCSFree(m_pbmInfo);
				m_pbmInfo = NULL;
			}
			if(m_pColorTable) {
				NCSFree(m_pColorTable);
				m_pColorTable = NULL;
			}
			if(m_pPaletteEntries) {
				NCSFree(m_pPaletteEntries);
				m_pPaletteEntries = NULL;
			}
			return(TRUE);
		}

		#elif defined(MACINTOSH) || defined(MACOSX)
#ifdef NOT_DEF // This doesn't compile in OSX10.7
		// returns GDHandle that window resides on (most of it anyway)
		GDHandle GetWindowDevice (WindowPtr pWindow)
		{
			GrafPtr pgpSave;
			Rect rectWind, rectSect;
			long greatestArea, sectArea;
			GDHandle hgdNthDevice, hgdZoomOnThisDevice;
			
			GetPort (&pgpSave);
		#if TARGET_API_MAC_CARBON
			SetPortWindowPort (pWindow);
			GetPortBounds(GetWindowPort(pWindow), &rectWind);
		#else
			SetPort (pWindow);
			rectWind = pWindow->portRect;
		#endif
			LocalToGlobal ((Point*)& rectWind.top);	// convert to global coordinates
			LocalToGlobal ((Point*)& rectWind.bottom);
			hgdNthDevice = GetDeviceList ();
			greatestArea = 0;	 // initialize to 0
			// check window against all gdRects in gDevice list and remember 
			//  which gdRect contains largest area of window}
			while (hgdNthDevice)
			{
				if (TestDeviceAttribute (hgdNthDevice, screenDevice))
					if (TestDeviceAttribute (hgdNthDevice, screenActive))
					{
						// The SectRect routine calculates the intersection 
						//  of the window rectangle and this gDevice 
						//  rectangle and returns TRUE if the rectangles intersect, 
						//  FALSE if they don't.
						SectRect(&rectWind, &(**hgdNthDevice).gdRect, &rectSect);
						// determine which screen holds greatest window area
						//  first, calculate area of rectangle on current device
						sectArea = (long)(rectSect.right - rectSect.left) * (rectSect.bottom - rectSect.top);
						if ( sectArea > greatestArea )
						{
							greatestArea = sectArea;	// set greatest area so far
							hgdZoomOnThisDevice = hgdNthDevice;	// set zoom device
						}
						hgdNthDevice = GetNextDevice(hgdNthDevice);
					}
			} 	// of WHILE
			SetPort (pgpSave);
			return hgdZoomOnThisDevice;
		}

		BOOLEAN CreatePixMapAndPallete( GrafPtr pGPtr, INT32 nWidth, INT32 nHeight ) {

			BOOLEAN bChangeInSize = FALSE;

			if( (m_nDCWidth!= nWidth) || ( m_nDCHeight != nHeight) ){
				bChangeInSize = TRUE;
			}

			if( bChangeInSize ) {
				short wPixDepth = 32;//(**(GetPortPixMap(hDC))).pixelSize;
				Rect	tempRect1 = { 0,0,nHeight,nWidth };
				GDHandle hgdWindow = GetWindowDevice( (WindowPtr)pGPtr );  // may not work
				m_nDCWidth = nWidth;
				m_nDCHeight = nHeight;
				
				DestroyPixMapAndPallete();
				if (noErr != NewGWorld (&m_hLocalMemDC, wPixDepth, &tempRect1, NULL, hgdWindow, noNewDevice | keepLocal))
				{
					if (noErr != NewGWorld (&m_hLocalMemDC, wPixDepth, &tempRect1, NULL, hgdWindow, noNewDevice | useTempMem))
					{
						//SysBeep( 30 );
						m_hLocalMemDC = NULL;
						//BAD: we are out of mem
						return FALSE;
					}
				}
			}

			if ( bChangeInSize || m_bCreateNewDIB
		#ifdef NCS_HIST_AND_LUT_SUPPORT
				|| (m_bLutChanged && m_bApplyLUTs)
		#endif // NCS_HIST_AND_LUT_SUPPORT
				){
				INT32 nBytesPerRow;
				Ptr offBaseAddr = NULL;	/* Pointer to the off-screen pixel image */
				INT32 i,j,index;
				INT32 nLine;

				PixMapHandle hPixMap = GetGWorldPixMap(m_hLocalMemDC);
				offBaseAddr = GetPixBaseAddr( hPixMap );
				m_nDCBitDepth = 32;
				nBytesPerRow = ((*hPixMap)->rowBytes & 0x7FFF);//(nWidth * m_nDCBitDepth) / 8;
				
				m_bCreateNewDIB = FALSE;
		#ifdef NCS_HIST_AND_LUT_SUPPORT
				m_bLutChanged = FALSE;
		#endif // NCS_HIST_AND_LUT_SUPPORT
			
				// Fill PixMap with RGB data
				switch(m_nDCBitDepth) {
				case 32:
						for(nLine = 0; nLine < nHeight; nLine++) {
							UINT8	*pRGBTriplets = (UINT8*)&(m_pRGBTriplets[nWidth * 3 * nLine]);
							index = nLine*nBytesPerRow;
							for(j=0, i=0; (j < nBytesPerRow) && (i < (nWidth * 3)); j+=4, i+=3) {
								offBaseAddr[index+j] = 0;
								offBaseAddr[index+j+1] = pRGBTriplets[i+2];
								offBaseAddr[index+j+2] = pRGBTriplets[i+1];
								offBaseAddr[index+j+3] = pRGBTriplets[i+0];
							}
						}
					break;
					
		/*		case 24:	// other bit depths not tested and probably don't work
				default:
						for(nLine = 0; nLine < nHeight; nLine++) {
							UINT8 *pRGBTriplets = (UINT8*)&(m_pRGBTriplets[nWidth * 3 * nLine]);
							UINT8 *pImagePtr8 = (UINT8*)&(offBaseAddr[WIDTHBYTES(nWidth * 24) * nLine]);

							memcpy(pImagePtr8, pRGBTriplets, nWidth * 3);
						}
					break;

				case 16:
						for(nLine = 0; nLine < nHeight; nLine++) {
							UINT8 *pRGBTriplets = (UINT8*)&(m_pRGBTriplets[nWidth * 3 * nLine]);
							UINT16 *pImagePtr16 = (UINT16*)&(offBaseAddr[WIDTHBYTES(nWidth * 16) * nLine]);

							for(nCell = 0; nCell < nWidth; nCell++) {
								*(pImagePtr16++) = (UINT16)((*pRGBTriplets >> 3) |
															((*(pRGBTriplets + 1) & 0xf8) << 3) |
															((*(pRGBTriplets + 2) & 0xf8) << 8));
								pRGBTriplets += 3;
							}
						}
					break;

				case 15:
						for(nLine = 0; nLine < nHeight; nLine++) {
							UINT8 *pRGBTriplets = (UINT8*)&(m_pRGBTriplets[nWidth * 3 * nLine]);
							UINT16 *pImagePtr16 = (UINT16*)&(offBaseAddr[WIDTHBYTES(nWidth * 16) * nLine]);

							for(nCell = 0; nCell < nWidth; nCell++) {
								*(pImagePtr16++) = (UINT16)((*pRGBTriplets >> 3) |
															((*(pRGBTriplets + 1) & 0xf8) << 2) |
															((*(pRGBTriplets + 2) & 0xf8) << 7));
								pRGBTriplets += 3;
							}
						}
					break;

				case 8:
						for(nLine = 0; nLine < nHeight; nLine++) {
							UINT8	*pRGBTriplets = (UINT8*)&(m_pRGBTriplets[nWidth * 3 * nLine]);
							UINT8	*pImagePtr8 = (UINT8*)&(offBaseAddr[WIDTHBYTES(nWidth * 8) * nLine]);

							for(nCell = 0; nCell < nWidth; nCell++) {
								UINT8 b = *pRGBTriplets;
								UINT8 g = *(pRGBTriplets + 1);
								UINT8 r = *(pRGBTriplets + 2);

								pRGBTriplets += 3;
								
								// *(pImagePtr8++) = (b & ) & (g & ) & (r & );

								// *(pImagePtr8++) = KERNEL_DITHER(nCell, nLine, r, g, b);
							}
						}	
					break;*/
				default:
					return FALSE;
					break;
				}
			}

			return TRUE;
		}
			

		BOOLEAN DestroyPixMapAndPallete( void ) {

			//if( m_hCTable ) DisposeHandle( (Handle)m_hCTable );
			/*if(m_hPixMap) {
				if( (*m_hPixMap)->pmTable ) DisposeHandle( (Handle)(*m_hPixMap)->pmTable );
				if( (*m_hPixMap)->baseAddr ) DisposePtr( (*m_hPixMap)->baseAddr );
				DisposePixMap( m_hPixMap );//DeleteObject(m_hBitmap);
				m_hPixMap = NULL;
			}*/
			if( m_hLocalMemDC ) DisposeGWorld( m_hLocalMemDC );
			m_hLocalMemDC = NULL;
			
			return TRUE;
		}
#endif // NOT_DEF
		#endif // defined(MACINTOSH)
		/**
		* Adjust the extents, using input dataset extents.
		* The renderer object attemps to draw to a bitmap, irrespective of whether the extents
		* are outside the actual dataset extents or not. This method adjusts the extents to be
		* within the dataset and returns the relevant adjustments.
		* @param nWidth The output device view width
		* @param nHeight The output device view height
		* @param nInputDatasetTLX The unadjusted top left X input dataset coordinate
		* @param nInputDatasetTLY The unadjusted top left Y input dataset coordinate
		* @param nInputDatasetBRX The unadjusted bottom right X input dataset coordinate
		* @param nInputDatasetBRY The unadjusted bottom right Y input dataset coordinate
		* @param *pnAdjustedDatasetTLX [OUT] The output adjusted top left X dataset coordinate
		* @param *pnAdjustedDatasetTLY [OUT] The output adjusted top left Y dataset coordinate
		* @param *pnAdjustedDatasetBRX [OUT] The output adjusted bottom right X dataset coordinate
		* @param *pnAdjustedDatasetBRY [OUT] The output adjusted bottom right Y dataset coordinate
		* @param *pnAdjustedDeviceTLX [OUT] The output adjusted top left X device coordinate
		* @param *pnAdjustedDeviceTLY [OUT] The output adjusted top left Y device coordinate
		* @param *pnAdjustedDeviceBRX [OUT] The output adjusted bottom right X device coordinate
		* @param *pnAdjustedDeviceBRY [OUT] The output adjusted bottom right Y device coordinate
		* @see SetView()
		* @return NCS_SUCCESS if successfull, or an NCSError value.
		*/
		NCSError AdjustExtents(INT32 nWidth, INT32 nHeight,
		//NCSError inline CRenderer::AdjustExtents(INT32 nWidth, INT32 nHeight,
											INT32 nInputDatasetTLX, INT32 nInputDatasetTLY, INT32 nInputDatasetBRX, INT32 nInputDatasetBRY,
											INT32 *pnAdjustedDatasetTLX, INT32 *pnAdjustedDatasetTLY, INT32 *pnAdjustedDatasetBRX, INT32 *pnAdjustedDatasetBRY,
											INT32 *pnAdjustedDeviceTLX, INT32 *pnAdjustedDeviceTLY, INT32 *pnAdjustedDeviceBRX, INT32 *pnAdjustedDeviceBRY)
		{
			INT32 nDatasetTLX, nDatasetTLY, nDatasetBRX, nDatasetBRY;
		//	NCSError nError = NCS_SUCCESS;
			
			// The dataset extents and the required view extents don't intersect!
			if (!IntersectRects(nInputDatasetTLX, nInputDatasetTLY, 
								nInputDatasetBRX, nInputDatasetBRY, 
								0, 0, Width(), Height()) ){
				return NCS_FILE_INVALID_SETVIEW;
			}

			nDatasetTLX = nInputDatasetTLX;
			nDatasetTLY = nInputDatasetTLY;
			nDatasetBRX = nInputDatasetBRX;
			nDatasetBRY = nInputDatasetBRY;

			if ((nDatasetTLX < 0) || (nDatasetTLY < 0) ||
				(nDatasetBRX > Width()-1) || (nDatasetBRY > Height()-1)){

				INT32 nAdjustedDeviceTLX = 0;
				INT32 nAdjustedDeviceTLY = 0;
				INT32 nAdjustedDeviceBRX = nWidth;
				INT32 nAdjustedDeviceBRY = nHeight;
				INT32 nDeviceTLX = 0;
				INT32 nDeviceTLY = 0;
				INT32 nDeviceBRX = nWidth;
				INT32 nDeviceBRY = nHeight;
				INT32 nAdjustedDatasetTLX = nDatasetTLX;
				INT32 nAdjustedDatasetTLY = nDatasetTLY;
				INT32 nAdjustedDatasetBRX = nDatasetBRX;
				INT32 nAdjustedDatasetBRY = nDatasetBRY;

				// 4 sets of data, nAdjustedDevice, nDevice, nAdjustedDataset, nDataset
				if (nDatasetTLX < 0) {
					nAdjustedDatasetTLX = 0;
					nAdjustedDeviceTLX = (INT32)(((IEEE8)((nDeviceBRX - nDeviceTLX)*(nAdjustedDatasetTLX-nDatasetTLX))/(IEEE8)(nDatasetBRX - nDatasetTLX)) + (IEEE8)nDeviceTLX);
				}
				if (nDatasetTLY < 0) {
					nAdjustedDatasetTLY = 0;
					nAdjustedDeviceTLY = (INT32)(((IEEE8)((nDeviceBRY - nDeviceTLY)*(nAdjustedDatasetTLY-nDatasetTLY))/(IEEE8)(nDatasetBRY - nDatasetTLY)) + (IEEE8)nDeviceTLY);
				}
				if (nDatasetBRX > Width()) {
					nAdjustedDatasetBRX = Width() - 1;
					nAdjustedDeviceBRX = nDeviceBRX - (INT32)((IEEE8)(nDeviceBRX - nDeviceTLX)*(IEEE8)(nDatasetBRX - nAdjustedDatasetBRX)/(IEEE8)(nDatasetBRX - nDatasetTLX));
				}
				if (nDatasetBRY > Height()) {
					nAdjustedDatasetBRY = Height() - 1;
					nAdjustedDeviceBRY = nDeviceBRY - (INT32)((IEEE8)(nDeviceBRY - nDeviceTLY)*(IEEE8)(nDatasetBRY - nAdjustedDatasetBRY)/(IEEE8)(nDatasetBRY - nDatasetTLY));
				}

				*pnAdjustedDeviceTLX = nAdjustedDeviceTLX;
				*pnAdjustedDeviceTLY = nAdjustedDeviceTLY;
				*pnAdjustedDeviceBRX = nAdjustedDeviceBRX;
				*pnAdjustedDeviceBRY = nAdjustedDeviceBRY;

				*pnAdjustedDatasetTLX= nAdjustedDatasetTLX;
				*pnAdjustedDatasetTLY= nAdjustedDatasetTLY;
				*pnAdjustedDatasetBRX= nAdjustedDatasetBRX;
				*pnAdjustedDatasetBRY= nAdjustedDatasetBRY;
			}
			else {
				*pnAdjustedDeviceTLX = 0;
				*pnAdjustedDeviceTLY = 0;
				*pnAdjustedDeviceBRX = nWidth;
				*pnAdjustedDeviceBRY = nHeight;

				*pnAdjustedDatasetTLX= nDatasetTLX;
				*pnAdjustedDatasetTLY= nDatasetTLY;
				*pnAdjustedDatasetBRX= nDatasetBRX;
				*pnAdjustedDatasetBRY= nDatasetBRY;
			}
			return NCS_SUCCESS;
		}

		/**
		* Adjust the extents, using input world coordinates.
		* The renderer object attemps to draw to a bitmap, irrespective of whether the extents
		* are outside the actual dataset extents or not. This method adjusts the extents to be
		* within the dataset and returns the relevant adjustments.
		* @param nWidth The output device view width
		* @param nHeight The output device view height
		* @param dInputWorldTLX The unadjusted top left X world coordinate
		* @param dInputWorldTLY The unadjusted top left Y world coordinate
		* @param dInputWorldBRX The unadjusted bottom right X world coordinate
		* @param dInputWorldBRY The unadjusted bottom right Y world coordinate
		* @param *pdAdjustedWorldTLX [OUT] The output adjusted top left X world coordinate
		* @param *pdAdjustedWorldTLY [OUT] The output adjusted top left Y world coordinate
		* @param *pdAdjustedWorldBRX [OUT] The output adjusted bottom right X world coordinate
		* @param *pdAdjustedWorldBRY [OUT] The output adjusted bottom right Y world coordinate
		* @param *pnAdjustedDeviceTLX [OUT] The output adjusted top left X device coordinate
		* @param *pnAdjustedDeviceTLY [OUT] The output adjusted top left Y device coordinate
		* @param *pnAdjustedDeviceBRX [OUT] The output adjusted bottom right X device coordinate
		* @param *pnAdjustedDeviceBRY [OUT] The output adjusted bottom right Y device coordinate
		* @return NCS_SUCCESS if successfull, or an NCSError value.
		*/
		//NCSError inline CRenderer::AdjustExtents(INT32 nWidth, INT32 nHeight, 
		NCSError AdjustExtents(INT32 nWidth, INT32 nHeight, 
											IEEE8 dInputWorldTLX, IEEE8 dInputWorldTLY, IEEE8 dInputWorldBRX, IEEE8 dInputWorldBRY, 
											IEEE8 *pdAdjustedWorldTLX, IEEE8 *pdAdjustedWorldTLY, IEEE8 *pdAdjustedWorldBRX, IEEE8 *pdAdjustedWorldBRY,
											INT32 *pnAdjustedDeviceTLX, INT32 *pnAdjustedDeviceTLY, INT32 *pnAdjustedDeviceBRX, INT32 *pnAdjustedDeviceBRY)
		{
			IEEE8 dWorldTLX, dWorldTLY, dWorldBRX, dWorldBRY;
//			NCSError nError = NCS_SUCCESS;

		#ifdef NOTDEF
			// Need a double version of this!
			if (!IntersectRects(dInputWorldTLX, dInputWorldTLY, 
								dInputWorldBRX, dInputWorldBRY, 
								m_fOriginX, m_fOriginY, 
								m_fOriginX + (IEEE8)Width()*m_fCellIncrementX, 
								m_fOriginY + (IEEE8)Height()*m_fCellIncrementY) ){
				return NCS_FILE_INVALID_SETVIEW;
			}
		#endif

			dWorldTLX = dInputWorldTLX;
			dWorldTLY = dInputWorldTLY;
			dWorldBRX = dInputWorldBRX;
			dWorldBRY = dInputWorldBRY;

			if (!IS_BETWEEN(dWorldTLX, OriginX(), OriginX() + ((IEEE8)Width())*CellIncrementX())  ||
				!IS_BETWEEN(dWorldTLY, OriginY(), OriginY() + ((IEEE8)Height())*CellIncrementY()) ||
				!IS_BETWEEN(dWorldBRX, OriginX(), OriginX() + ((IEEE8)Width())*CellIncrementX())  ||
				!IS_BETWEEN(dWorldBRY, OriginY(), OriginY() + ((IEEE8)Height())*CellIncrementY()) ) {

				INT32 nAdjustedDeviceTLX = 0;
				INT32 nAdjustedDeviceTLY = 0;
				INT32 nAdjustedDeviceBRX = nWidth;
				INT32 nAdjustedDeviceBRY = nHeight;
				INT32 nDeviceTLX = 0;
				INT32 nDeviceTLY = 0;
				INT32 nDeviceBRX = nWidth;
				INT32 nDeviceBRY = nHeight;
				IEEE8 dAdjustedWorldTLX = dWorldTLX;
				IEEE8 dAdjustedWorldTLY = dWorldTLY;
				IEEE8 dAdjustedWorldBRX = dWorldBRX;
				IEEE8 dAdjustedWorldBRY = dWorldBRY;

				// Lat long inversion check
				if (CellIncrementY() < 0.0) {
					if (dWorldTLY > OriginY()) {
						dAdjustedWorldTLY = OriginY();
						nAdjustedDeviceTLY = (INT32)(((IEEE8)((nDeviceBRY - nDeviceTLY)*(dAdjustedWorldTLY-dWorldTLY))/(IEEE8)(dWorldBRY - dWorldTLY)) + (IEEE8)nDeviceTLY);
					}
					if (dWorldBRY < (OriginY() + (Height())*CellIncrementY())) {
						dAdjustedWorldBRY = (OriginY() + (Height())*CellIncrementY());
						nAdjustedDeviceBRY = nDeviceBRY - (INT32)((IEEE8)(nDeviceBRY - nDeviceTLY)*(IEEE8)(dWorldBRY - dAdjustedWorldBRY)/(IEEE8)(dWorldBRY - dWorldTLY));
					}
				} else {
					if (dWorldTLY < OriginY()) {
						dAdjustedWorldTLY = OriginY();
						nAdjustedDeviceTLY = (INT32)(((IEEE8)((nDeviceBRY - nDeviceTLY)*(dAdjustedWorldTLY-dWorldTLY))/(IEEE8)(dWorldBRY - dWorldTLY)) + (IEEE8)nDeviceTLY);
					}
					if (dWorldBRY > (OriginY() + (Height())*CellIncrementY())) {
						//dAdjustedWorldBRY = (m_dOriginX + (T::m_nWidth)*m_dCellIncrementX);
						dAdjustedWorldBRY = (OriginY() + (Height())*CellIncrementY());		/**[08]**/
						nAdjustedDeviceBRY = nDeviceBRY - (INT32)((IEEE8)(nDeviceBRY - nDeviceTLY)*(IEEE8)(dWorldBRY - dAdjustedWorldBRY)/(IEEE8)(dWorldBRY - dWorldTLY));
					}
				}
				// X is same for all coord systems
				if (dWorldBRX > (OriginX() + (Width())*CellIncrementX())) {
					dAdjustedWorldBRX = (OriginX() + (Width())*CellIncrementX());
					nAdjustedDeviceBRX = nDeviceBRX - (INT32)((IEEE8)(nDeviceBRX - nDeviceTLX)*(IEEE8)(dWorldBRX - dAdjustedWorldBRX)/(IEEE8)(dWorldBRX - dWorldTLX));
				}
				if (dWorldTLX < OriginX()) {
					dAdjustedWorldTLX = OriginX();
					nAdjustedDeviceTLX = (INT32)(((IEEE8)((nDeviceBRX - nDeviceTLX)*(dAdjustedWorldTLX-dWorldTLX))/(IEEE8)(dWorldBRX - dWorldTLX)) + (IEEE8)nDeviceTLX);
				}

				*pnAdjustedDeviceTLX = nAdjustedDeviceTLX;
				*pnAdjustedDeviceTLY = nAdjustedDeviceTLY;
				*pnAdjustedDeviceBRX = nAdjustedDeviceBRX;
				*pnAdjustedDeviceBRY = nAdjustedDeviceBRY;

				*pdAdjustedWorldTLX= dAdjustedWorldTLX;
				*pdAdjustedWorldTLY= dAdjustedWorldTLY;
				*pdAdjustedWorldBRX= dAdjustedWorldBRX;
				*pdAdjustedWorldBRY= dAdjustedWorldBRY;
			}
			else {
				*pnAdjustedDeviceTLX = 0;
				*pnAdjustedDeviceTLY = 0;
				*pnAdjustedDeviceBRX = nWidth;
				*pnAdjustedDeviceBRY = nHeight;

				*pdAdjustedWorldTLX= dWorldTLX;
				*pdAdjustedWorldTLY= dWorldTLY;
				*pdAdjustedWorldBRX= dWorldBRX;
				*pdAdjustedWorldBRY= dWorldBRY;
			}

			return NCS_SUCCESS;
		}

	// These are the adjusted extents for when the set view is outside the range of the data
	INT32 m_nAdjustedViewWidth;
	INT32 m_nAdjustedViewHeight;
	INT32 m_nAdjustedXOffset;
	INT32 m_nAdjustedYOffset;
	IEEE8 m_dAdjustedWorldTLX;
	IEEE8 m_dAdjustedWorldTLY;
	IEEE8 m_dAdjustedWorldBRX;
	IEEE8 m_dAdjustedWorldBRY;

	// This is the actual extents of the renderer.
	INT32 m_nRendererWidth;
	INT32 m_nRendererHeight;
	INT32 m_nRendererXOffset;
	INT32 m_nRendererYOffset;	
	INT32 m_nRendererDatasetTLX;
	INT32 m_nRendererDatasetTLY;
	INT32 m_nRendererDatasetBRX;
	INT32 m_nRendererDatasetBRY;
	IEEE8 m_dRendererWorldTLX;
	IEEE8 m_dRendererWorldTLY;
	IEEE8 m_dRendererWorldBRX;
	IEEE8 m_dRendererWorldBRY;

#ifdef _WIN32
	HBITMAP m_hBitmap;
	BITMAPINFO *m_pbmInfo;
	UINT8 *m_pColorTable;
	PALETTEENTRY *m_pPaletteEntries;
	HINSTANCE	m_hOpenGLDLL;	
	UINT8 *m_pBitmapImage;
#elif defined(MACINTOSH) || defined(MACOSX)
	//PixMapHandle m_hPixMap;
	//GWorldPtr m_hLocalMemDC;
	//CTabHandle m_hCTable;
#endif
	UINT8 *m_pRGBA;
	UINT8 *m_pRGBALocal;
	bool m_bUsingAlphaBand;
	NCS_FUNCADDR m_pAlphaBlend;
	NCS_DLHANDLE m_hMSImg32DLL;
	INT32 m_nDCWidth;
	INT32 m_nDCHeight;
	INT32 m_nDCBitDepth;
	INT32 m_nBytesPerLine;
	BOOLEAN m_bHaveInit;
#if defined( POSIX )
#else	
	COLORREF m_nBackgroundColor;
#endif
	BOOLEAN  m_bIsTransparent;
	NCSMutex m_DrawMutex;
	NCSSetViewInfo *pCurrentViewSetInfo;
	BOOLEAN m_bHaveReadImage;
	BOOLEAN m_bAlternateDraw;
#ifdef NCS_HIST_AND_LUT_SUPPORT
	BOOLEAN m_bCalcHistograms;
	INT32	m_nReadLine;
	typedef UINT32 Histogram[256];
	Histogram *m_pHistograms;
	NCSMutex m_HistogramMutex;
	
	BOOLEAN m_bApplyLUTs;
	BOOLEAN m_bLutChanged;
	UINT8 m_LUTs[3][256];
#endif //NCS_HIST_AND_LUT_SUPPORT
};

class NCS_EXPORT CRenderer : public TRenderer<CView> {
public:
	CRenderer();
	/**
	 *	Destructor.
	 */
	virtual ~CRenderer();
};
}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CRenderer CNCSRenderer;
#endif

#endif // !defined(NCSRENDERER_H)
