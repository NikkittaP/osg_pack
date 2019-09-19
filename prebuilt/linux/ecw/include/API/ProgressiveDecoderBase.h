#pragma once

#ifndef SWIG
#include "NCSDefs.h"
#else
#define NCSECW_IMPEX
#endif
#include "API/DecoderBase.h"

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
class NCSECW_IMPEX CProgressiveDecoderBase : public CDecoderBase
{
public:
	CProgressiveDecoderBase();
	CProgressiveDecoderBase(const CProgressiveDecoderBase &s);
	virtual ~CProgressiveDecoderBase();

#ifndef SWIG
	CProgressiveDecoderBase &operator=(const CProgressiveDecoderBase &s);
#endif

	/**
	* \fn	virtual NCS::CError CProgressiveScanlineDecoder::Open(const char *szUTF8Path);
	*
	* \brief	Opens a ECW/JP2 Dataset at the given UTF8 encoded local path or ECWP URL.
	*
	* \author	Intergraph Perth
	* \date	13/08/2013
	*
	* \param	szUTF8Path	Full UTF8 encoded local path or ECWP URL.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError Open(const char *szUTF8Path);

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

	/**
	* \fn	void CProgressiveScanlineDecoder::SetBackgroundColour(unsigned int nRGBABGColour);
	*
	* \brief	Sets background colour.  This is the colour that is set on the ReadRegion output buffer, prior to drawing.
	*
	* \author	Intergraph Perth
	* \date	13/08/2013
	*
	* \param	nRGBABGColour	The colour.
	*/
	void SetBackgroundColour(const unsigned int nRGBABGColour);
	/**
	* \fn	unsigned int CProgressiveScanlineDecoder::GetRefreshTimeMS();
	*
	* \brief	Gets time in milliseconds to limit the frequency of RefreshUpdate calls in progressive views.
	*
	* \author	Intergraph Perth
	* \date	02/04/2015
	*
	* \return	The refresh time in milliseconds.
	*/
	unsigned int GetRefreshTimeMS();

	/**
	* \fn	void CProgressiveScanlineDecoder::SetRefreshTimeMS(unsigned int nMilliSecondsRefreshTime);
	*
	* \brief	Sets time in milliseconds to limit the frequency of RefreshUpdate calls in progressive views.
	*
	* \author	Intergraph Perth
	* \date	02/04/2015
	*
	* \param	nMilliSecondsRefreshTime	The refresh time in milliseconds.
	*/
	void SetRefreshTimeMS(const unsigned int nMilliSecondsRefreshTime);
protected:
#ifndef SWIG
	CCurrentProgressiveView *m_pProgressiveView;
	UINT32 m_nBackgroundColour;
#endif
};

} // API
} // NCS