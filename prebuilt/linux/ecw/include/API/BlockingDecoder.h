/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: BlockingDecoder.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Blocking decoder header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_DECODER_H
#define NCS_API_DECODER_H
#ifndef SWIG
    #include "NCSDefs.h"
#else
    #define NCSECW_IMPEX
#endif
#include "DecoderBase.h"



namespace NCS {
namespace API {

/**
 * \class	NCS::API::CBlockingDecoder
 *
 * \brief	A decoder that waits a predetermined time for all ECWP data to arrive before drawing.  Local files draw instantly.  
 * \ingroup	simplifiedclasses
 * 
 * \author	Intergraph Perth
 * \date	8/08/2013
 */
class NCSECW_IMPEX CBlockingDecoder : public CDecoderBase {
public:
	CBlockingDecoder();
	CBlockingDecoder(const CBlockingDecoder &s);
	virtual ~CBlockingDecoder();

#ifndef SWIG
	const CBlockingDecoder &operator=(const CBlockingDecoder &s);
#endif

	/**
	 * \fn	virtual int CBlockingDecoder::GetWaitTimeMS();
	 *
	 * \brief	Gets wait time in milliseconds that the SDK will block for while waiting for ECWP data to be downloaded.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The wait time in milliseconds.
	 */
	virtual unsigned int GetWaitTimeMS();

	/**
	 * \fn	virtual void CBlockingDecoder::SetWaitTimeMS(int nMilliSecondsWaitTime);
	 *
	 * \brief	Sets wait time in milliseconds that the SDK will block for while waiting for ECWP data to be downloaded.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	nMilliSecondsWaitTime	The wait time in milliseconds.
	 */
	virtual void SetWaitTimeMS(unsigned int nMilliSecondsWaitTime);

	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::Open(const char *szUTF8Path);
	 *
	 * \brief	Opens an ECW/JP2 Dataset (local or over ECWP).
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	szUTF8Path	UTF8 encoded local path or ECWP Url.
	 *
	 * \return	Error of result.
	 */
	virtual NCS::CError Open(const char *szUTF8Path);

	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	 * 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
	 * 		NCSCellType eOutputBufferType, NCSCellType eOutputDataType, void *pBIL);
	 *
	 * \brief	Reads a region from an ECW or JP2 dataset in BIL format.  
	 * 			
	 * 			For regions less than 6000px x 6000px, if you need a region larger than
	 *			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	 * \param [in,out]	viewSize			Size of the output region in pixels.
	 * \param [in,out]	worldCoordinates	The world coordinates source region of the dataset to read.
	 * \param	eOutputBufferType			The actual buffer type passed in as pBIL.
	 * \param	eOutputDataType				The type of data to be stored in eOutputBufferType. This is usually the same as eOutputBufferType.
	 * \param [in,out]	pBIL				The output buffer.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates, 
						   NCSCellType eOutputBufferType, NCSCellType eOutputDataType, void *pBIL);
	
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
	* 		NCSCellType eOutputDataType, INT8 *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	worldCoordinates	The world coordinates source region of the dataset to read.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	* \param [in,out]	pBIL				The output buffer.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
		INT8 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
	* 		NCSCellType eOutputDataType, INT16 *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	worldCoordinates	The world coordinates source region of the dataset to read.
	* \param [in,out]	pBIL				The output buffer.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType. By default view cell type will be used. 
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
		INT16 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
	* 		NCSCellType eOutputDataType, INT32 *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	worldCoordinates	The world coordinates source region of the dataset to read.
	* \param [in,out]	pBIL				The output buffer.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
		INT32 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
	* 		NCSCellType eOutputDataType, INT64 *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	worldCoordinates	The world coordinates source region of the dataset to read.
	* \param [in,out]	pBIL				The output buffer.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
		INT64 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
	* 		NCSCellType eOutputDataType, float *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	worldCoordinates	The world coordinates source region of the dataset to read.
	* \param [in,out]	pBIL				The output buffer.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
		float *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
	* 		NCSCellType eOutputDataType, double *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	worldCoordinates	The world coordinates source region of the dataset to read.
	* \param [in,out]	pBIL				The output buffer.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates,
		double *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
	* 		NCSCellType eOutputBufferType, NCSCellType eOutputDataType, void *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	8/08/2013
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	imageCoordinates	The image coordinates source region of the dataset to read.
	* \param	eOutputBufferType			The actual buffer type passed in as pBIL.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	* \param [in,out]	pBIL				The output buffer.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
		NCSCellType eOutputBufferType, NCSCellType eOutputDataType, void *pBIL);
	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	 * 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
	 * 		NCSCellType eOutputBufferType, NCSCellType eOutputDataType, void *pBIL);
	 *
	 * \brief	Reads a region from an ECW or JP2 dataset in BIL format.  
	 * 			
	 * 			For regions less than 6000px x 6000px, if you need a region larger than
	 *			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	 * \param [in,out]	viewSize			Size of the output region in pixels.
	 * \param [in,out]	imageCoordinates	The image coordinates source region of the dataset to read.
	 * \param [in,out]	pBIL				The output buffer.
	 * \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates, 
		INT8 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
	* 		NCSCellType eOutputDataType, INT16 *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	imageCoordinates	The image coordinates source region of the dataset to read.
	* \param [in,out]	pBIL				The output buffer.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
		INT16 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
	* 		NCSCellType eOutputDataType, INT32 *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	imageCoordinates	The image coordinates source region of the dataset to read.
	* \param [in,out]	pBIL				The output buffer of INT32.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
		INT32 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);

	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
	* 		NCSCellType eOutputDataType, INT64 *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	imageCoordinates	The image coordinates source region of the dataset to read.
	* \param [in,out]	pBIL				The output buffer of INT64.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
		INT64 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
	* 		NCSCellType eOutputDataType, float *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	imageCoordinates	The image coordinates source region of the dataset to read.
	* \param [in,out]	pBIL				The output buffer of INT64.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
		float *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist,
	* 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
	* 		NCSCellType eOutputDataType, double *pBIL);
	*
	* \brief	Reads a region from an ECW or JP2 dataset in BIL format.
	*
	* 			For regions less than 6000px x 6000px, if you need a region larger than
	*			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	* \param [in,out]	viewSize			Size of the output region in pixels.
	* \param [in,out]	imageCoordinates	The image coordinates source region of the dataset to read.
	* \param [in,out]	pBIL				The output buffer of INT64.
	* \param	eOutputDataType				The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadRegionBandInterleaveLine(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates,
		double *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::ReadRegion32bpp(std::vector<unsigned int> &Bandlist,
	 * 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates, unsigned int *pRGBA);
	 *
	 * \brief	Reads a region from an ECW or JP2 dataset in 32bpp.  
	 * 			
	 * 			This is the same as NCS::CView::ReadLineRGBA, or NCS::CView::ReadLineBGRA, etc.  For regions less than 6000px x 6000px, if you need a region larger than
	 *			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	 * \param [in,out]	viewSize			Size of the output region in pixels.
	 * \param [in,out]	worldCoordinates	The world coordinates source region of the dataset to read.
	 * \param [in,out]	pRGBA				The output buffer.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError ReadRegion32bpp(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates, unsigned int *pRGBA);

	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::ReadRegion32bpp(std::vector<unsigned int> &Bandlist,
	 * 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates, unsigned int *pRGBA);
	 *
	 * \brief	Reads a region from an ECW or JP2 dataset in 32bpp.  
	 * 			
	 * 			This is the same as NCS::CView::ReadLineRGBA, or NCS::CView::ReadLineBGRA, etc.  For regions less than 6000px x 6000px, if you need a region larger than
	 *			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	 * \param [in,out]	viewSize			Size of the output region in pixels.
	 * \param [in,out]	imageCoordinates	The image coordinates source region of the dataset to read.
	 * \param [in,out]	pRGBA				The output buffer.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError ReadRegion32bpp(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates, unsigned int *pRGBA);

	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::ReadRegion24bpp(std::vector<unsigned int> &Bandlist,
	 * 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates, unsigned char *pRGB);
	 *
	 * \brief	Reads a region from an ECW or JP2 dataset in 24bpp.  
	 * 			
	 *			This is the same as NCS::CView::ReadLineRGB, or NCS::CView::ReadLineBGR, etc.  For regions less than 6000px x 6000px, if you need a region larger than
	 *			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	 * \param [in,out]	viewSize			Size of the output region in pixels.
	 * \param [in,out]	worldCoordinates	The world coordinates source region of the dataset to read.
	 * \param [in,out]	pRGB				The output buffer.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError ReadRegion24bpp(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates, unsigned char *pRGB);

	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::ReadRegion24bpp(std::vector<unsigned int> &Bandlist,
	 * 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates, unsigned char *pRGB);
	 *
	 * \brief	Reads a region from an ECW or JP2 dataset in 24bpp.  
	 * 			
	 *			This is the same as NCS::CView::ReadLineRGB, or NCS::CView::ReadLineBGR, etc.  For regions less than 6000px x 6000px, if you need a region larger than
	 *			this then either tile your decoding or use the scanline method (SetView, ReadLine, ReadLine, ReadLine...).
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	 * \param [in,out]	viewSize			Size of the output region in pixels.
	 * \param [in,out]	imageCoordinates	The image coordinates source region of the dataset to read.
	 * \param [in,out]	pRGB				The output buffer.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError ReadRegion24bpp(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates, unsigned char *pRGB);

	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::SetView(std::vector<unsigned int> &Bandlist,
	 * 		NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates);
	 *
	 * \brief	Sets a view to be read via subsequent ReadLine calls.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	 * \param [in,out]	viewSize			Size of the output region in pixels.
	 * \param [in,out]	worldCoordinates	The world coordinates source region of the dataset to read.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError SetView(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSWorldCoordinatesRect &worldCoordinates);

	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::SetView(std::vector<unsigned int> &Bandlist,
	 * 		NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates);
	 *
	 * \brief	Sets a view.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param [in,out]	Bandlist			The bandlist, size of which determines the band count.
	 * \param [in,out]	viewSize			Size of the output region in pixels.
	 * \param [in,out]	imageCoordinates	The image coordinates source region of the dataset to read.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError SetView(std::vector<unsigned int> &Bandlist, NCSViewSize &viewSize, NCSImageCoordinatesRect &imageCoordinates);

	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::ReadLineBandInterleaveLine(NCSCellType eOutputBufferType,
	 * 		NCSCellType eOutputDataType, void *pBIL);
	 *
	 * \brief	Reads line band interleave line.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	eOutputBufferType	The actual buffer type passed in as pBIL.
	 * \param	eOutputDataType		The type of data to be stored in eOutputBufferType. This is usually the same as eOutputBufferType.
	 * \param [in,out]	pBIL	 	The output scanline buffer.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError ReadLineBandInterleaveLine(NCSCellType eOutputBufferType, NCSCellType eOutputDataType, void *pBIL);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadLineBandInterleaveLine(NCSCellType eOutputBufferType,
	* 		NCSCellType eOutputDataType, INT8 *pBIL);
	*
	* \brief	Reads line band interleave line.
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	pBIL	 	The output scanline buffer.
	* \param	eOutputDataType		The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadLineBandInterleaveLine(INT8 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadLineBandInterleaveLine(NCSCellType eOutputBufferType,
	* 		NCSCellType eOutputDataType, INT16 *pBIL);
	*
	* \brief	Reads line band interleave line.
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	pBIL	 	The output scanline buffer.
	* \param	eOutputDataType		The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadLineBandInterleaveLine(INT16 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadLineBandInterleaveLine(NCSCellType eOutputBufferType,
	* 		NCSCellType eOutputDataType, INT32 *pBIL);
	*
	* \brief	Reads line band interleave line.
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	pBIL	 	The output scanline buffer.
	* \param	eOutputDataType		The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadLineBandInterleaveLine(INT32 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadLineBandInterleaveLine(NCSCellType eOutputBufferType,
	* 		NCSCellType eOutputDataType, INT64 *pBIL);
	*
	* \brief	Reads line band interleave line.
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param [in,out]	pBIL	 	The output scanline buffer.
	* \param	eOutputDataType		The type of data to be stored in eOutputBufferType.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadLineBandInterleaveLine(INT64 *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadLineBandInterleaveLine(NCSCellType eOutputBufferType,
	* 		NCSCellType eOutputDataType, float *pBIL);
	*
	* \brief	Reads line band interleave line.
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*
	* \param	eOutputDataType		The type of data to be stored in eOutputBufferType. This is usually the same as eOutputBufferType.
	* \param [in,out]	pBIL	 	The output scanline buffer.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadLineBandInterleaveLine(float *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	* \fn	virtual NCS::CError CBlockingDecoder::ReadLineBandInterleaveLine(NCSCellType eOutputBufferType,
	* 		NCSCellType eOutputDataType, double *pBIL);
	*
	* \brief	Reads line band interleave line.
	*
	* \author	Intergraph Perth
	* \date	05/01/2016
	*

	* \param [in,out]	pBIL	 	The output scanline buffer.
	*
	* \return	Error of the result.
	*/
	virtual NCS::CError ReadLineBandInterleaveLine(double *pBIL, NCSCellType eOutputDataType = NCSCT_NUMVALUES);
	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::ReadLine32bpp(unsigned int *pRGBA);
	 *
	 * \brief	Reads the next line from the current view (determined by your SetView call) in 32bpp.
	 * 			
	 * 			This is similar to NCS::CView::ReadLineRGBA
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param [in,out]	pRGBA	The output scanline buffer.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError ReadLine32bpp(unsigned int *pRGBA);

	/**
	 * \fn	virtual NCS::CError CBlockingDecoder::ReadLine24bpp(unsigned char *pRGB);
	 *
	 * \brief	Reads the next line from the current view (determined by your SetView call) in 24bpp.
	 * 			
	 * 			This is similar to NCS::CView::ReadLineRGB
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param [in,out]	pRGB	The output scanline buffer.
	 *
	 * \return	Error of the result.
	 */
	virtual NCS::CError ReadLine24bpp(unsigned char *pRGB);
};

} // API
} // NCS

#endif // NCS_API_DECODER_H