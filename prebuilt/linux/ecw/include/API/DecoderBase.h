/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: DecoderBase.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Decoder base header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_DECODER_BASE_H
#define NCS_API_DECODER_BASE_H
#ifndef SWIG
    #include "NCSDefs.h"
#else
    #define NCSECW_IMPEX
#endif
#include "FileInfo.h"
#include "FileMetadata.h"
#include "FileStatistics.h"
#include "FileRPCData.h"
#include "CellType.h"
#include "FileType.h"
#include "Error.h"
#include "ViewSize.h"
#include "CurrentView.h"
#include "ImageCoordinatesRect.h"
#include "WorldCoordinatesRect.h"
#include "ResampleMethod.h"
#include "../NCSError.h"

#include <vector>

namespace NCS {
namespace API {
    class CView;
	class CBoxes;
/**
 * \class	NCS::API::CDecoderBase
 *
 * \brief	Parent class for decoders.
 * \ingroup	simplifiedclasses
 * 
 * \author	Intergraph Perth
 * \date	9/08/2013
 */
class NCSECW_IMPEX CDecoderBase
{
public:
	CDecoderBase();
	CDecoderBase(const CDecoderBase &s);
	virtual ~CDecoderBase();

#ifndef SWIG
	const CDecoderBase &operator=(const CDecoderBase &s);
#endif

	/**
	 * \fn	virtual NCS::CError CDecoderBase::Close();
	 *
	 * \brief	Close the decoder.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	An error of the result.
	 */
	virtual NCS::CError Close();

    /**
     * \fn	virtual NCS::CError CDecoderBase::Open(const char *szUTF8Path)=0;
     *
     * \brief	Opens the given size UTF 8 path.
     *
     * \author	Intergraph Perth
     * \date	9/08/2013
     *
     * \param	szUTF8Path	Full pathname of the UTF 8 file.
     *
     * \return	An error of the result.
     */
    virtual NCS::CError Open(const char *szUTF8Path)=0;

	/**
	 * \fn	virtual NCS::CError CDecoderBase::GetFileInfo(CFileInfo *pFileInfo);
	 *
	 * \brief	Gets file information.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param [in,out]	pFileInfo	Output.
	 *
	 * \return	An error of the result.
	 */
	virtual NCS::CError GetFileInfo(CFileInfo *pFileInfo);

	/**
	 * \fn	virtual NCS::CError CDecoderBase::GetFileStatistics(CFileStatistics *pFileStatistics);
	 *
	 * \brief	Gets file statistics.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param [in,out]	pFileStatistics	Output.
	 *
	 * \return	An error of the result.
	 */
	virtual NCS::CError GetFileStatistics(CFileStatistics *pFileStatistics);

	/**
	 * \fn	virtual NCS::CError CDecoderBase::GetFileMetadata(CFileMetadata *pFileMetadata);
	 *
	 * \brief	Gets file metadata.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param [in,out]	pFileMetadata	Output.
	 *
	 * \return	An error of the result.
	 */
	virtual NCS::CError GetFileMetadata(CFileMetadata *pFileMetadata);

	/**
	 * \fn	virtual NCS::CError CDecoderBase::GetFileRPCData(CFileRPCData *pFileRPCData);
	 *
	 * \brief	Gets file RPC data.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param [in,out]	pFileRPCData	Output.
	 *
	 * \return	An error of the result.
	 */
	virtual NCS::CError GetFileRPCData(CFileRPCData *pFileRPCData);

	/**
	 * \fn	virtual NCS::CError CDecoderBase::GetCurrentView(CCurrentView *pCurrentView);
	 *
	 * \brief	Gets current view.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param [in,out]	pCurrentView	Output.
	 *
	 * \return	An error of the result.
	 */
	virtual NCS::CError GetCurrentView(CCurrentView *pCurrentView);

	/**
	 * \fn	virtual NCSResampleMethod CDecoderBase::GetResampleMethod();
	 *
	 * \brief	Gets resample method.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The resample method.
	 */
	virtual NCSResampleMethod GetResampleMethod();

	/**
	 * \fn	virtual void CDecoderBase::SetResampleMethod(NCSResampleMethod eResampleMethod);
	 *
	 * \brief	Sets resample method.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	eResampleMethod	The resample method.
	 */
	virtual void SetResampleMethod(NCSResampleMethod eResampleMethod);

	/**
	 * \fn	virtual bool CDecoderBase::GetAutoScaleUp();
	 *
	 * \brief	Gets automatic scale up flag.
	 * 			
	 * 			This must be set for JP2 1bit opacity bands when reading in 24bpp or
	 * 			32bpp.  Also for 11 or 12 bit ECW or JP2 files if you want the output
	 * 			to be autoscaled to 16bit.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	true if it is enabled.
	 */
	virtual bool GetAutoScaleUp();

	/**
	 * \fn	virtual void CDecoderBase::SetAutoScaleUp(bool bAutoScaleUp);
	 *
	 * \brief	Sets automatic scale up flag.
	 * 			
	 * 			This must be set for JP2 1bit opacity bands when reading in 24bpp or
	 * 			32bpp.  Also for 11 or 12 bit ECW or JP2 files if you want the output
	 * 			to be autoscaled to 16bit.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	bAutoScaleUp	true to automatically scale up.
	 */
	virtual void SetAutoScaleUp(bool bAutoScaleUp);

	/**
	 * \fn	virtual NCS::CError CDecoderBase::SetPercentageClip(double dPercentageClip,
	 * 		int nMaximumDimension);
	 *
	 * \brief	Set a percentage clip on the dataset.
	 * 			
	 * 			This is usually used for non-8bit datasets when reading in with 24bpp
	 * 			or 32bpp functions.  A histogram is generated from a subsampled view
	 * 			of the dataset and then clipped by the percentage provided.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	dPercentageClip  	The percentage clip range: 1-100.
	 * \param	nMaximumDimension	The maximum dimension of the subsampled dataset which
	 * 								the histogram is generated from.
	 *
	 * \return	An error of the result.
	 */
	virtual NCS::CError SetPercentageClip(double dPercentageClip, int nMaximumDimension);
	/**
	* \fn	virtual NCS::CError CDecoderBase::GetTopLevelBoxes(CBoxes *pTopLevelBoxes);
	*
	* \brief	Gets top level boxes of current file
	*
	* \author	Intergraph Perth
	* \date	12/01/2016
	*
	* \param [in]	pTopLevelBoxes	The destination.
	*
	* \return	An error of the result.
	*/
	virtual NCS::CError GetTopLevelBoxes(CBoxes *pTopLevelBoxes);
	/**
	 * \fn	virtual NCS::CError CDecoderBase::GetDefaultRGBABandlist(std::vector<unsigned int> &Bandlist);
	 *
	 * \brief	Gets default RGBA bandlist.
	 * 			
	 * 			The SDK will attempt to detect RGBA bands and return them as 0,1,2,3.  If an opacity
	 * 			band is not present only three bands will be returned.  Three bands can be passed 
	 * 			in to ReadRegion32bpp call and the opacity band will prefilled with 0xff.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param [in,out]	Bandlist	The default RGBA bandlist.
	 *
	 * \return	An error of the result.
	 */
	virtual NCS::CError GetDefaultRGBABandlist(std::vector<unsigned int> &Bandlist);

	/**
	 * \fn	virtual NCS::CError CDecoderBase::GetDefaultRGBBandlist(std::vector<unsigned int> &Bandlist);
	 *
	 * \brief	Gets default RGB bandlist. 
	 * 			
	 * 			The SDK will attempt to detect RGB bands and return them as 0,1,2.  Opacity
	 * 			bands are ignored.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param [in,out]	Bandlist	The default RGB bandlist.
	 *
	 * \return	An error of the result.
	 */
	virtual NCS::CError GetDefaultRGBBandlist(std::vector<unsigned int> &Bandlist);

	/**
	 * \fn	virtual void CDecoderBase::SetOEMMobileDecodeKey(const char *szCompanyName,
	 * 		const char *szKey);
	 *
	 * \brief	Sets OEM mobile decode key.  
	 * 			
	 * 			On mobile platforms decoding of ECWP streams is unlimited, decoding local files requires key to be set. 
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	szCompanyName	Name of the company.
	 * \param	szKey		 	The ECWJP2 SDK Decompression key.
	 */
	virtual void SetOEMMobileDecodeKey(const char *szCompanyName, const char *szKey);

	/**
	 * \fn	virtual NCSFileType CDecoderBase::GetFileType();
	 *
	 * \brief	Gets file type.  To determine if the internal file is an ECW or JP2
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The file type.
	 */
	virtual NCSFileType GetFileType();

	/**
	 * \fn	virtual void CDecoderBase::SetFreeInternalOnClose(bool bFreeInternalOnClose);
	 *
	 * \brief	Sets whether to free internal file structures if the decoder has the last view 
	 *			on the file after decoder is closed. Default value is false if not being set.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	bFreeInternalOnClose	whether to free internal file structures.
	 */
	virtual void SetFreeInternalOnClose(bool bFreeInternalOnClose);

protected:
#ifndef SWIG
	class CView;
	CView *m_pView;
	bool m_bFreeInternalOnClose;
#endif
};

} // API
} // NCS

#endif // NCS_API_DECODER_BASE_H
