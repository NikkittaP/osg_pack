#pragma once

#ifndef SWIG
#include "NCSDefs.h"
#else
#define NCSECW_IMPEX
#endif
#include "API/ProgressiveDecoderBase.h"

namespace NCS {
namespace API {

#ifndef SWIG
class CCurrentProgressiveView;
#endif

/**
* \class	NCS::API::CProgressiveScanlineDecoder
*
* \brief	A decoder that draws all available data into the provided buffer and then notifies the client when there is more data available.
* \ingroup	simplifiedclasses
*
* \author	Intergraph Perth
* \date	8/08/2013
*/
class NCSECW_IMPEX CProgressiveScanlineDecoder : public CProgressiveDecoderBase
{
public:
	CProgressiveScanlineDecoder();
	CProgressiveScanlineDecoder(const CProgressiveScanlineDecoder &s);
	virtual ~CProgressiveScanlineDecoder();

#ifndef SWIG
	CProgressiveScanlineDecoder &operator=(const CProgressiveScanlineDecoder &s);
#endif

	
	/**
	* \fn	NCS::CError CProgressiveScanlineDecoder::SetView(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldRect);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in 32bpp.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than this then you must use the CBlockingDecoder.
	*
	* \author	Intergraph Perth
	* \date	13/08/2013
	*
	* \param [in,out]	Bandlist 	The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize 	Size of the output region in pixels.
	* \param [in,out]	imageRect	The world coordinates source region of the dataset to read.
	*
	* \return	Error of the result.
	*/
	NCS::CError SetView(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldRect);

	/**
	* \fn	NCS::CError CProgressiveScanlineDecoder::SetView(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageRect);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in 32bpp.
	*
	*			For regions less than 6000px x 6000px, if you need a region larger than this then you must use the CBlockingDecoder.
	*
	* \author	Intergraph Perth
	* \date	13/08/2013
	*
	* \param [in,out]	Bandlist 	The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize 	Size of the output region in pixels.
	* \param [in,out]	imageRect	The image coordinates source region of the dataset to read.
	*
	* \return	Error of the result.
	*/
	NCS::CError SetView(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageRect);

	/**
	* \fn	NCS::CError CProgressiveScanlineDecoder::ReadScanLine32bpp(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageRect, unsigned int *pRGBAScanline);
	*
	* \brief	Reads a scanline from the current view in 32bpp.  This is called during a CProgressiveScanlineDecoder::RefreshUpdate event
	*
	*			For regions less than 6000px x 6000px, if you need a region larger than this then you must use the CBlockingDecoder.
	*
	* \author	Intergraph Perth
	* \date	13/08/2013
	*
	* \param [in,out]	pRGBAScanline	 	The output buffer for the scanline (NCSViewSize.Width * 4)
	*
	* \return	Error of the result.
	*/
	NCS::CError ReadScanLine32bpp(unsigned int *pRGBAScanline);

	/**
	* \fn	virtual void CProgressiveScanlineDecoder::RefreshUpdate(CCurrentView *pCurrentView)
	*
	* \brief	Refresh update callback override this function to receive notification when an update for the current view is available.
	*
	* \author	Intergraph Perth
	* \date	13/08/2013
	*
	* \param [in,out]	pCurrentView	The current view determined by the last ReadRegion call.
	*/
	virtual void RefreshUpdate(CCurrentView *pCurrentView) {};
private:
#ifndef SWIG
	UINT32 m_nCurrentLine;
#endif
};

} // API
} // NCS