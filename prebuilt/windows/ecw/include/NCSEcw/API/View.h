/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
**
** FILE:     View.h 
** CREATED:  14/03/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CFile class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
**			 [01] 07Sep04 tfl  Added support for configuring georeferencing use
**			 [02] 11Nov04 tfl  Added rotation member to ensure rotation data persists
**			 [03] 19Jan06 tfl  Changed tiled views vector to a single view-strip member
 *******************************************************/

#ifndef NCSVIEW_H
#define NCSVIEW_H

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4251)
#endif

#include "ECWJP2BuildNumber.h"

#ifndef NCSJPCDEFS_H
#include "NCSEcw/JPC/Types/Defs.h"
#endif // NCSJPCDEFS_H

#ifndef NCSECWCLIENT_H
#include "NCSECWClient.h"
#endif // NCSECWCLIENT_H

#ifndef NCSUTIL_H
#include "NCSUtil.h"
#endif // NCSUTIL_H
#ifndef NCSJPCBuffer2D_H
#include "NCSEcw/SDK/Buffer2D.h"
#endif // NCSJPCBuffer2D_H
#ifndef NCSSDKBOX_H
#include "NCSEcw/SDK/Box.h"
#endif // NCSSDKBOX_H
#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif // NCSJPCNode2D_H
#ifndef NCSFILEBASE_H
#include "NCSEcw/SDK/FileBase.h"
#endif // NCSJPCNode2D_H

#include "NCSEcw/API/Transform.h"
#include "NCSString.h"
#include "NCSEvent.h"


#ifndef NCS_MOBILE
	#include "NCSEcw/ECW/Compress.h"
	#include "NCSEcw/SDK/TimeCounters.h"
#endif

#include "NCSEcw/GTIF/geokeys.h"


#ifdef __GNUC__
// Disable the unused parameter warnings
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

namespace NCS {
	class CView;
	class CBlockIdQueue;
}

typedef NCSReadStatus (*NCSRefreshCallback)( NCS::CView *pFile);

struct NCSCompressClient;

namespace NCS {
namespace ECW {
	class GeotiffTagBox;
	class CReader;
#ifndef NCS_MOBILE
	class CWriter;
	class CUpdater;
#endif
}
namespace JP2 {
	class CReader;
	class CFile;
#ifndef NCS_MOBILE
	class CWriter;
#endif
}
namespace JPC {
	class CResampler;
}
namespace SDK {
	class CFileBase;
	class CRect2D;
}

/**
 * @class	NCS::CApplication
 * @ingroup classes
 * @brief	Scoped Application ECWJP2 SDK will stay alive which this object is constructed.
 * 			
 * 			Constructor calls NCSInit(), destructor calls NCSShutdown().  NCSInit/NCSShutdown are
 * 			reference counted.
 * 			
 * 			When using NCSEcw.dll on windows NCSInit() is called on DLL_PROCESS_ATTACH and 
 * 			NCSShutdown() is called on DLL_PROCESS_DETACH so explicitly calling NCSInit()/NCSShutdown()
 * 			or constructing a NCS::CApplication is not required for this configuration.
 *
 * @date	19/02/2014
 */
class NCSECW_IMPEX CApplication
{
public:
	CApplication();

	/**
	 * @fn	CApplication::CApplication(NCS::CString &sETCPath);
	 *
	 * @brief	Constructor.
	 *
	 * @date	19/02/2014
	 *
	 * @param [in,out]	sETCPath	Full pathname of the where the ecw_csv folder exists.
	 */
	explicit CApplication(NCS::CString &sETCPath);
	~CApplication();
private:
	CApplication(const CApplication &s) {};
	const CApplication &operator=(const CApplication &s) { return *this; };
};

/**
 * @class CView
 * @ingroup classes
 * @brief The CView class is the main class for reading and writing ECW and JPEG 2000 images.
 */
class NCSECW_IMPEX CView: public SDK::CFileClient {
public:
		/** Statistics enum */
	typedef enum {
			/** Global T1 Decoder US total */
		ST_CODEBLOCK_DECODER_US			= 0,	
			/** Global T1 Decoder samples total */
		ST_CODEBLOCK_DECODER_SAMPLES	= 1,	
			/** Global Codeblock read US total */
		ST_CODEBLOCK_READ_US			= 2,	
			/** Global Codeblock read samples total */
		ST_CODEBLOCK_READ_SAMPLES		= 3,
			/** DC Shift US total */
		ST_DCSHIFT_US					= 10,
			/** MCT US total */
		ST_MCT_US						= 20,
			/** DWT ROW US total */
		ST_DWT_ROW_US					= 30,
			/** DWT COL US total */
		ST_DWT_COL_US					= 31,
			/** DWT Samples total */
		ST_DWT_SAMPLES					= 32,
			/** SetView US total */
		ST_VIEW_SETVIEW_US				= 40,
			/** ReadLine US total */
		ST_VIEW_READLINE_US				= 41,
			/** Resample US total */
		ST_VIEW_RESAMPLE_US				= 42,
			/** Raw IO Read US total */
		ST_IO_READ_US					= 50,
			/** Raw IO Write US total */
		ST_IO_WRITE_US					= 51,
			/** Raw IO Seek US total */
		ST_IO_SEEK_US					= 52,
			/** Global Buffer Alloc US total */
		ST_BUF_ALLOC_US					= 60,	
			/** Global Buffer Free US total */
		ST_BUF_FREE_US					= 61,	
			/** Global Buffer bytes allocated total */
		ST_BUF_ALLOC_BYTES				= 62,	
			/** Global Buffer Copy US total */
		ST_BUF_COPY_US					= 63,	
			/** Global Buffer bytes copied total */
		ST_BUF_COPY_BYTES				= 64,	
			/** Global Buffer Clear US total */
		ST_BUF_CLEAR_US					= 65,	
			/** Global Buffer bytes cleared total */
		ST_BUF_CLEAR_BYTES				= 66,	
			/** File Compression Total MS */
		ST_COMPRESS_TOTAL_MS			= 80,	
			/** File Compression output size bytes total */
		ST_COMPRESS_OUTPUT_SIZE			= 81	
	} Statistic;

		/** JP2FileView parameters 
		* NOTE: THIS IS DEPRECATED.  USE THE STRING NAME VERSION INSTEAD
		*/
	typedef enum {
			/** Default, Baseline Profile 0 */
		JP2_COMPRESS_PROFILE_BASELINE_0		= 0, 
			/** Baseline Profile 1 */
		JP2_COMPRESS_PROFILE_BASELINE_1		= 1, 
			/** Profile 2 (unrestricted) */
		JP2_COMPRESS_PROFILE_BASELINE_2		= 2, 
			/** NITF NSIF BIIF NPJE Profile */
		JP2_COMPRESS_PROFILE_NITF_BIIF_NPJE = 3, 
			/** NITF NSIF BIIF EPJE Profile */
		JP2_COMPRESS_PROFILE_NITF_BIIF_EPJE = 4, 

			/** Resolution Levels.  UINT32 - Calculated so r=0 <= 64x64 */
		JP2_COMPRESS_LEVELS					= 20, 
			/** Quality Layers.  UINT32 - Default 1 */
		JP2_COMPRESS_LAYERS					= 21, 
			/** Precinct Width.  UINT32 - Default 64 or larger depending on file size */
		JP2_COMPRESS_PRECINCT_WIDTH			= 22, 
			/** Precinct Height.  UINT32 - Default 64 or larger depending on file size */
		JP2_COMPRESS_PRECINCT_HEIGHT		= 23, 
			/** Tile Width.  UINT32 - Default to image width given in SetFileInfo() */
		JP2_COMPRESS_TILE_WIDTH				= 24, 
			/** Tile Height.  UINT32 - Default to image height given in SetFileInfo() */
		JP2_COMPRESS_TILE_HEIGHT			= 25, 
			/** Output SOP.  bool - Default false */
		JP2_COMPRESS_INCLUDE_SOP			= 26, 
			/** Output EPH.  bool - Default true */
		JP2_COMPRESS_INCLUDE_EPH			= 27, 
			/** bool - Only write JPC codestream, no JP2 file wrapper */
		JP2_COMPRESS_CODESTREAM_ONLY		= 28, 
			/** bool - Enable/disable compression threaded read - Default true */
		JP2_COMPRESS_MT_READ				= 29, 

			/** LRCP progression */
		JP2_COMPRESS_PROGRESSION_LRCP		= 30, 
			/** RLCP progression */
		JP2_COMPRESS_PROGRESSION_RLCP		= 31, 
			/** v3.1 SDK Default: RPCL progression */
		JP2_COMPRESS_PROGRESSION_RPCL		= 32, 

			/** Control the precedence of georeferencing metadata from world files and embedded GML XML boxes and PCS UUID boxes */
		JP2_GEODATA_USAGE					= 40, 
			/** Control the precision employed by registration info (rounding errors can cause ridiculous values) */
		JP2_GEODATA_PRECISION_EPSILON       = 41,
			/** UINT32 - Defaults to all */
		JP2_DECOMPRESS_LAYERS				= 100, 
			/** IEEE4 - defaults to 0.0, range 0.0 <= r < 1.0 */
		JPC_DECOMPRESS_RECONSTRUCTION_PARAMETER = 101,
			/** Auto scale data up to read cell size, eg scale 1bit to 8bit when reading NCSCT_UINT8 */
		JPC_DECOMPRESS_AUTOSCALE_UP			= 102,

			/** Background colour for ecw v3 */
		ECW_BACKGROUND_COLOR			= 150,

			/** Options for both formats */
			/** resample method for decoding */
		NCS_RESAMPLE_METHOD			= 151,
			
		
		NCS_BLOCKING_TIME_MS				= 200,
		NCS_REFRESH_TIME_MS					= 201
	} Parameter;

#ifndef SWIG
	class NCSECW_IMPEX CReaderBase {//: public CThreadPool::CJob {
	public:
		explicit CReaderBase(CView *pView);
		virtual ~CReaderBase();
		virtual CError SetView(UINT32 nBands, UINT32 *pBandList, 
								   UINT32 nDatasetTLX, UINT32 nDatasetTLY,
								   UINT32 nDatasetBRX, UINT32 nDatasetBRY,
								   UINT32 nWidth, UINT32 nHeight,
								   IEEE8 dWorldTLX, IEEE8 dWorldTLY,
								   IEEE8 dWorldBRX, IEEE8 dWorldBRY) = 0; 
		virtual NCSReadStatus Read(SDK::CBuffer2DVector &Buffers, NCSCellType eOutputType) = 0;
		virtual NCSReadStatus ReadLine(NCSCellType eOutputBufferType, UINT16 nBands, void **ppOutputLine, NCSCellType eOuputDataType, UINT32 *pLineSteps) = 0;
		virtual void UpdateViewStats() = 0;

		virtual NCSFileType GetFileType() = 0;
				/** Copy the parameters from the m_Parameters vector to the File instance */
		virtual void SetFileParameters() {};

		virtual SDK::CFileBase *GetFile() { return(m_pView ? m_pView->GetFile() : NULL); };
		virtual CView *GetView() { return(m_pView); };

		virtual bool ReceivedBlock(NCSBlockId nBlock);
		virtual bool PushReceivedBlock(NCSBlockId nBlock);

		NCS_INLINE UINT32 &NextLine() { return(m_nNextLine); };
		NCS_INLINE void NextLine(UINT32 n) { m_nNextLine = n; };

		virtual void Work();

		// added on 31/01/2013
		virtual bool ReadCancel() = 0;

	protected:
		CView *m_pView;
			/** Scanline input buffer */
		SDK::CBuffer2D m_InputBuf;

		/** Next line in view to read */
		UINT32		m_nNextLine;

		CBlockIdQueue *m_pReceivedBlocks;

		NCSSetViewInfo m_LastRefreshedView;

		NCSReadStatus m_LastProgressiveReadStatus;
		//void Work(void *pData);
	};
#endif //SWIG

		/** The number of cells across the dataset */
	INT32 m_nWidth;				
		/** The number of lines down the dataset */
	INT32 m_nHeight;			
		/** The number of bands in the dataset */
	INT32 m_nNumberOfBands;	
		/** The actual compression ratio */	
	IEEE8 m_dCompressionRate;	
		/** The target compression ratio */
	IEEE8 m_dTargetCompressionRate;
		/** min data value for 16bit ECW compression, it is per-band now. changed on 11/08/2014 */
	//INT32 m_nEncodeDataMin;
	std::vector<INT32> m_nEncodeDataMinList;
		/** max data value for 16bit ECW compression, it is per-band now. changed on 11/08/2014 */
	//INT32 m_nEncodeDataMax;
	std::vector<INT32> m_nEncodeDataMaxList;
		/** What version format to generate - for ECW only*/ 
	UINT8 m_nFormatVersion;
		/** Cell bits for MULTIBAND, RGB, YUV and GREYSCALE, currently support 8bit (default) and 16bit (progressing) */
	UINT8 m_nCellBitDepth;
		/** Optional ECW huffman coding flag for 16 bits. more details see pack_data16() at ECW.h */
	UINT8 m_nECWhuffmancodingFlag;
		/** The X cell size in m_CellSizeUnits */
	IEEE8 m_dCellIncrementX;	
		/** The Y cell size in m_CellSizeUnits */
	IEEE8 m_dCellIncrementY;	
		/** The top left X origin of the dataset (world) */
	IEEE8 m_dOriginX;			
		/** The top left Y origin of the dataset (world) */
	IEEE8 m_dOriginY;		
		/** rotation */
	IEEE8 m_fCWRotationDegrees;
	    /** file metadata box */
	NCSFileMetaData* m_pFileMetaData;

		/** The GDT datum of the dataset */
	char *m_pDatum;				
		/** The GDT projection of the dataset */
	char *m_pProjection;		
		/** A pointer to the currently open filename in UNICODE*/
	NCS::CString m_Filename;	
		/* Currently open URL */
	CString m_URL;
		/** Cell units, meters, degrees or feet */
	NCSCellSizeUnitType m_CellSizeUnits;
		/** Cell type image should be read as */
	NCSCellType m_eCellType;		
		/** ColorSpace of image */
	NCSFileColorSpace m_eColorSpace;		
		/** BandInfo for each band */
	std::vector<NCSFileBandInfo> m_Bands;
		/** The clockwise rotation of the image in degrees [02] */
	IEEE8 m_dCWRotationDegrees; 
		/** Index of AllOpacity band */
	INT32 m_iAllOpacityBand;
		/** Flag to scale up to opacity band's max value. eg. 255 (8bit) or 65535 (16bit). */
	bool m_bAllOpacityBandScaleUpMaxValue;
		/** AllOpacity max value for opacity band, 255 or a number dependent on bits (when m_bAllOpacityBandScaleUpMaxValue = true). */
	UINT64 m_nAllOpacityBandScaleUpMaxValue;
		/** The number of bands in the current view */
	INT32 m_nSetViewNrBands;		
		/** A pointer to the band list for the current view */
	INT32 *m_pnSetViewBandList;		
		/** SetView() contains allopacity band */
	INT32 m_bSetViewHasAllOpacity;
		/** SetView() allopacity band number */
	INT32 m_nAllOpacityBandNumber;
		/** The current view width */
	INT32 m_nSetViewWidth;			
		/** The current view height */
	INT32 m_nSetViewHeight;			
		/** The current view world top left X */
	IEEE8 m_dSetViewWorldTLX;		
		/** The current view world top left Y */
	IEEE8 m_dSetViewWorldTLY;		
		/** The current view world bottom right X */
	IEEE8 m_dSetViewWorldBRX;		
		/** The current view world bottom right Y */
	IEEE8 m_dSetViewWorldBRY;		
		/** The current view dataset top left X */
	INT32 m_nSetViewDatasetTLX;		
		/** The current view dataset top left Y */
	INT32 m_nSetViewDatasetTLY;		
		/** The current view dataset bottom right X */
	INT32 m_nSetViewDatasetBRX;		
		/** The current view dataset bottom right Y */
	INT32 m_nSetViewDatasetBRY;		

	NCSResampleMethod m_eResampleMethod;

	INT32 m_bUpdateOpacity;
		/** Default constructor, initialises members */
	CView(); // changed also for 16bit
	CView(const CView &s);
		/** Virtual Destructor */
	virtual ~CView();

	virtual CView &operator=(const CView &s);
#ifdef NCS_BUILD_UNICODE
		/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		pURLPath	Full path name of JP2 file to open
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError Open(wchar_t *pURLPath, bool bProgressiveDisplay = false, bool bWrite = false);
#endif
		/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		pURLPath	Full path name of JP2 file to open
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError Open(char *pURLPath, bool bProgressiveDisplay = false, bool bWrite = false);
			/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		URLPath	Full path name of JP2 file to open
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError Open(const CString &URLPath, bool bProgressiveDisplay = false, bool bWrite = false);
		/** 
		 * Open the JP2 file for Parsing/UnParsing on the specified stream.  The stream object
		 * may be a custom subclass of CIOStream with overridden IO methods.  You should
		 * not usually maintain a reference or pointer to the stream object after calling this 
		 * method due to the way the SDK handles the destruction of NCSFile objects, which
		 * may lead to uncertainty as to the validity of the stream object over time.
		 * @param		pStream		Input stream on which to open the file
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */

	virtual CError Open(CIOStream *pStream, bool bProgressiveDisplay = false, NCSFileType eType = NCS_FILE_UNKNOWN);
		/** 
		 * Close the JP2 file
		 * @param		bFreeCache	Free the cached JP2 file data.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */

	virtual CError Close ( bool bFreeCache = false );

		/** 
		 * Set a view into the JP2 file for reading.
		 * @param		nBands			Number of bands in pBandList to read
		 * @param		pBandList		Array of band indices to read.
		 * @param		nWidth			Width of the view in pixels
		 * @param		nHeight			Height of the view in pixels
		 * @param		nDatasetTLX		Top left X dataset coordinate of view
		 * @param		nDatasetTLY		Top left Y dataset coordinate of view
		 * @param		nDatasetBRX		Bottom right X dataset coordinate of view
		 * @param		nDatasetBRY		Bottom right Y dataset voordinate of view
		 * @param		dWorldTLX		Top left X world coordinate of view (informative only)
		 * @param		dWorldTLY		Top left Y world coordinate of view (informative only)
		 * @param		dWorldBRX		Bottom right X world coordinate of view (informative only)
		 * @param		dWorldBRY		Bottom right Y world coordinate of view (informative only)
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError SetView(UINT32 nBands, UINT32 *pBandList, 
					   		  UINT32 nDatasetTLX, UINT32 nDatasetTLY,
							  UINT32 nDatasetBRX, UINT32 nDatasetBRY,
							  UINT32 nWidth, UINT32 nHeight,
							  IEEE8 dWorldTLX = 0.0, IEEE8 dWorldTLY = 0.0,
							  IEEE8 dWorldBRX = 0.0, IEEE8 dWorldBRY = 0.0);

	/**
	 * \fn	virtual CError NCS::CView::SetTransform(INT32 nBand, NCS::API::ITransform *pTransform);
	 *
	 * \brief	Sets the transform for a band.  The SDK will delete transforms that have already been applied to the band.
	 * 			If this pTransform already exists in the list the SDK will double delete the pointer and your program will 
	 * 			crash.
	 *
	 * \param	nBand			  	The band this Transform is for.
	 * \param [in,out]	pTransform	If non-null, the transform.
	 *
	 * \return	Error if unsuccessful
	**/
	virtual CError SetTransform(INT32 nBand, NCS::API::ITransform *pTransform);

	/**
	 * \fn	virtual CError NCS::CView::SetTransformList(INT32 nBand, NCS::API::CTransformList &TransformList);
	 *
	 * \brief	Sets the transforms for a band.  The SDK will delete transforms that have already been applied to the band.
	 * 			If a pointer to a transform already exists in the SDK it will be double deleted and your program will crash.
	 *
	 * \param	nBand			  	The band this Transform is for.
	 * \param 	TransformList		The list of transforms for the band.
	 *
	 * \return	Error if unsuccessful
	**/
	virtual CError SetTransformList(INT32 nBand, NCS::API::CTransformList &TransformList);

	/**
	 * \fn	virtual  NCS::CView::GetTransformList(INT32 nBand, NCS::API::CTransformListAccessor &Accessor);
	 *
	 * \brief	Gets an accessor that can access the internal CTransformList (std::list<ITransform*>) for a band.  This object is 
	 * 			scope locked so while it is in scope no rendering in the SDK can use the scale data.  If an invalid
	 * 			band was specified the internal CTransformList will be NULL.
	 *
	 * \param	nBand				  	The band.
	 * \param	[in, out] Accessor		The scope locked CTransformListAccessor.
	 * 
	 * \return	Error if unsuccessful
	**/
	virtual CError GetTransformList(INT32 nBand, API::CTransformListAccessor &Accessor);

	/**
	 * \fn	virtual CError NCS::CView::ClearTransforms();
	 *
	 * \brief	Clears the ITransforms for all bands.
	 *
	 * \return	Error if unsuccessful.
	**/
	virtual CError ClearTransforms();

	/**
	 * \fn	virtual CError NCS::CView::CalculateMinMaxTransforms(UINT16 nBands, INT32 *pBandList,
	 * 		std::vector<API::CMinMaxTransform *> &Transform, UINT32 nMaximumDimension = 1024,
	 * 		double dPercentClip = 99);
	 *
	 * \brief	Calculates the CMinMaxTransforms for a given bandlist (opacity band will be
	 * 			meaningless) with a default 99% clip.  The calculation will be performed on an overview, 
	 * 			the maximum pixel dimension of which is specified by nMaximumDimension.
	 *
	 * \param	nBands			 	The number of bands.
	 * \param [in,out]	pBandList	The bandlist to read.
	 * \param [in,out]	Transform	[in,out] If non-null, the Output CMinMaxTransforms.
	 * \param	dPercentClip	 	(optional) the percent clip.
	 * \param	nMaximumDimension	(optional) the maximum dimension.
	 *
	 * \return	Error if unsuccessful.
	**/
	virtual CError CalculateMinMaxTransforms(UINT16 nBands, INT32 *pBandList, std::vector<API::CMinMaxTransform *> &Transforms, double dPercentClip = 99, UINT32 nMaximumDimension = 512);

	/**
	 * \fn	virtual CError NCS::CView::SetMinMax(INT32 nBand, NCSCellType eCellType, INT32 nMin, INT32 nMax);
	 *
	 * \brief	Sets the min max for a band. This is optional when compressing 16bit ECW file.
	 *			The min and max values are helpful when being compressed 16bit ECW file has lower bit depth, say less than 14 bits.
	 *			Since min and max will be (when needed) converted to 16bit's values, eCellType
	 *			is needed. The cell type must be the one when counting min and max values.
	 *
	 * \param	nBand				The band number to set for.
	 * \param	eCellType			The cell type must be the one when counting min and max values.
	 * \param   nMin				The min pixel value to set.
	 * \param   nMax				The max pixel value to set.
	 *
	 * \return	Error if unsuccessful
	**/
	virtual CError SetMinMax(INT32 nBand, NCSCellType eCellType, INT64 nMin, INT64 nMax);

		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(UINT8 **ppOutputLine, NCSCellType eOuputDataType = NCSCT_UINT8);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(UINT16 **ppOutputLine, NCSCellType eOuputDataType = NCSCT_UINT16);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(UINT32 **ppOutputLine, NCSCellType eOuputDataType = NCSCT_UINT32);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(UINT64 **ppOutputLine, NCSCellType eOuputDataType = NCSCT_UINT64);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(INT8 **ppOutputLine, NCSCellType eOuputDataType = NCSCT_INT8);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(INT16 **ppOutputLine, NCSCellType eOuputDataType = NCSCT_INT16);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(INT32 **ppOutputLine, NCSCellType eOuputDataType = NCSCT_INT32);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(INT64 **ppOutputLine, NCSCellType eOuputDataType = NCSCT_INT64);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(IEEE4 **ppOutputLine, NCSCellType eOuputDataType = NCSCT_IEEE4);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(IEEE8 **ppOutputLine, NCSCellType eOuputDataType = NCSCT_IEEE8);
		/** 
		 * Read the next line in RGB UINT8 triplet format from the current view into the file.
		 * @param		pRGBTripler			Pointer to UINT8 buffer to receive RGB data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineRGB(UINT8 *pRGBTriplet);
		/** 
		 * Read the next line in BGR UINT8 triplet format from the current view into the file.
		 * @param		pBGRTripler			Pointer to UINT8 buffer to receive BGR data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBGR(UINT8 *pBGRTriplet);
		/** 
		 * Read the next line in RGBA UINT32 format from the current view into the file.
		 * @param		pRGBA				Pointer to UINT32 buffer to receive RGBA data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineRGBA(UINT32 *pRGBA);
		/** 
		 * Read the next line in BGRA UINT32 format from the current view into the file.
		 * @param		pBGRA				Pointer to UINT32 buffer to receive BGRA data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBGRA(UINT32 *pBGRA);
		/** 
		 * Read the next line in ARGB UINT32 format from the current view into the file.
		 * @param		pARGB				Pointer to UINT32 buffer to receive ARGB data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineARGB(UINT32 *pARGB);
		/** 
		 * Read the next line in ABGR UINT32 format from the current view into the file.
		 * @param		pABGR				Pointer to UINT32 buffer to receive ABGR data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineABGR(UINT32 *pABGR);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		eOutputBufferType	Output buffer type
		 * @param		nBands				Number of output bands
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @param		pLineSteps			Line steps, in CELLS.
		 * @param		eOuputDataType		Output datatype.
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBIL(NCSCellType eOutputBufferType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL, NCSCellType eOuputDataType = NCSCT_NUMVALUES);

		/** 
		 * Read the given buffers from the current view.
		 * @param		Buffers				Output buffers
		 * @param		eOutputType			Output data type
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus Read(SDK::CBuffer2DVector &Buffers, NCSCellType eOutputType);
#ifndef NCS_MOBILE
		/** 
		 * Write the next line in BIL format into the JP2 file.
		 * @param		eType				Output buffer type
		 * @param		nBands				Number of output bands
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @param		pLineSteps			Line steps, in CELLS.
		 * @return      CError			Write status code
		 */
	virtual CError WriteLineBIL(NCSCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL);
#endif
	/**
	 *	More data has become available and a refresh update should be done.  Deprecated, override RefreshUpdateEx() instead.
	 *
	 *	@param[in]	pViewSetInfo	Pointer to a SetViewInfo containing details on view the update is from.
	 */
	virtual void RefreshUpdate(NCSSetViewInfo *pViewSetInfo);
		/** 
		 * More data is available and a refresh update should be done.
		 * @param		pSetViewInfo		Pointer to SetViewInfo containing details on view the update is for
		 * @return      NCSReadStatus	Return the Read status code from the ReadLine*() call.
		 */
	virtual NCSReadStatus RefreshUpdateEx(NCSSetViewInfo *pViewSetInfo);

	/** 
	* \fn	virtual bool CView::ReadCancel();
	*
	* \brief	When closing a view, call this to cancel requested packets.
	*
	* \return	true if it succeeds, false if it fails.
	**/
	virtual bool ReadCancel();

#ifndef NCS_MOBILE
		
		/**
		* Open the ECW file for region update.
		* @param		pURLPath	Full path name of ECW file to open
		* @return      CError		NCS_SUCCESS or error code on failure.
		*/
	virtual CError OpenForUpdate(const CString &URLPath);

		/**
		* Update the ECW file.
		* @param		updateRect	rectangle of the region in image coordinates for update
		* @return      CError		NCS_SUCCESS or error code on failure.
		*/
	virtual CError Update(const SDK::CRect2D &updateRect);

	/*!
	 * \fn	virtual CError :::Write();
	 *
	 * \brief	Gets the write.
	 *
	 * \return	.
	 */
	virtual CError Write();	

	/**
	 * \fn	virtual CError :::WriteReadLine(UINT32 nNextLine, void **ppInputArray);
	 *
	 * \brief	Read input line for compression.
	 *
	 * \param	nNextLine				Which line to read (always in sequence).
	 * \param [in,out]	ppInputArray	Input as a BIL Buffer.
	 *
	 * \return	If there is an error
	**/
	virtual CError WriteReadLine(UINT32 nNextLine, void **ppInputArray);

	/**
	 * \fn	virtual NCSCellType :::WriteReadLineGetCellType()
	 *
	 * \brief	Get the line BIL Buffer type to be allocated for WriteReadLine Calls.
	 *
	 * \return	The Cell Type.
	**/
	virtual NCSCellType WriteReadLineGetCellType() {
		return NCSCT_IEEE4;
	}

	/**
	* \fn      virtual bool CView::WriteError(NCS::CError eError, const NCS::CString &sErrorText)
	*
	* \brief   Subclass must override this.  The Compressor calls this if an error occurs.
	*                The client can return true to attempt to continue (may not be possible) or 
	*                return false to cancel the compression.
	*
	* \param   eError            The error.
	* \param   sErrorText  The error text.
	*
	* \return  true to attempt to continue, false to cancel.
	**/
	virtual bool WriteError(NCS::CError eError, const NCS::CString &sErrorText) { return true; };

	/**
	* \enum    CompressInputType 
	*
	* \brief   Values that represent CompressInputType.
	**/
	typedef enum {
		TILE, // ECW Only
		LINE
	} CompressInputType;

	/**
	 * \struct	CompressionCounters
	 *
	 * \brief	Counters used to determine the progress of a compression.
	**/
	typedef struct {
		// The compression type
		CompressInputType eType;

		/**
		* \union	NCS::CView::CompressionCounters::ProgressElements
		 *
		 * \brief	Current progress of the compression job.
		**/
		union ProgressElements {
			UINT32 nCurrentLine;
			UINT64 nProcessedPrecincts;
		};
		union ProgressElements Progress;

		/**
		 * \union	NCS::CView::CompressionCounters::TotalElements
		 *
		 * \brief	Total elements in the compression job.
		**/
		union TotalElements {
			UINT32 nTotalLines;
			UINT64 nTotalPrecincts;
		};
		union TotalElements Totals;

		UINT64 nNULLBlocks;
		UINT64 nProcessedBlocks;
		UINT64 nTotalBlocks;
	} CompressionCounters;

	

	/**
	* \fn      virtual void CView::WriteStatus(IEEE4 fPercentComplete, const NCS::CString &sStatusText)
	*
	* \brief   Subclass must override this.  The Compressor periodically calls this to inform the 
	*                client of the compression status.
	*
	* \param   fPercentComplete  The percent complete.
	* \param   sStatusText             The status text.
	**/
	virtual void WriteStatus(IEEE4 fPercentComplete, const NCS::CString &sStatusText, const CompressionCounters &Counters) {  };

	/**
	* \fn      virtual bool CView::WriteCancel()
	*
	* \brief   Subclass must override this.  The Compressor periodically calls this to determine if
	*                the client wants to cancel the compression.  
	*
	* \return  true to cancel, false to continue.
	**/
	virtual bool WriteCancel();

	/**
	* \fn      virtual bool CView::WritePause()
	*
	* \brief   Subclass must override this.  The Compressor periodically calls this to determine if
	*                the client wants to pause the compression.  There is no dynamic "resume", the Compressor
	*                will save the intermediary data to disk and restarting the compression with the same
	*                input and output files will resume it.
	*
	* \return  true to pause, false to continue.
	**/
	virtual bool WritePause() { return false; };

	/**
	* \fn      virtual CompressInputType CView::GetCompressInputType()
	*
	* \brief   Gets the compress input type, subclass must override this.  Only the ECW 
	*          format supports the TILE based compression API.
	*
	* \return  The compress input type.
	**/
	virtual CompressInputType GetCompressInputType() { return CView::LINE; };

	/**
	* \fn      virtual bool CView::WriteReadTile(const NCS::SDK::CRect2D &Tile,
	*          SDK::CBuffer2DVector &Buffers)
	*
	* \brief   Gets a tile to compress, subclass must override this.  The callee must 
	*                honour the region specified by CRect2D Tile and the line stride of the buffers 
	*                (CBuffer2D::Stride()).  The region will never be outside the file input region, 
	*                however, the buffers will be for boundary tiles.
	*                
	*                If the image to compress has NULL regions (regions with no data at all) e.g. you 
	*                are mosaicing many images and the Tile regions does not intersect any the client may set 
	*                bIsRegionNULL to true and return true and the SDK can optimise the compression.
	*
	* \param   Tile              The tile region the SDK is requesting.
	* \param [in,out]      Buffers			The vector of buffers, one CBuffer2D per band.
	* \param [in,out]      bIsRegionNULL	Client will set to true if there is no input data for this region.
	*
	* \return  true if it succeeds, false if it fails.
	**/
	virtual bool WriteReadTile(const NCS::SDK::CRect2D &Tile, SDK::CBuffer2DVector &Buffers, bool &bIsRegionNULL) { return false; };

	/**
	 * \fn	virtual bool CView::SetupBackgroundColour(std::vector &BandColours)
	 *
	 * \brief	Sets up the background colour for NULL regions and regions with no data.  
	 * 			The compressor will call this function and if the client returns true, NULL
	 * 			regions will be filled with the colours set for that specific band.
	 *
	 * \param [in,out]	BandColours	List of colours of the bands.
	 *
	 * \return	true if it succeeds, false if it fails.
	**/
	virtual bool SetupBackgroundColour(std::vector<IEEE4> &BandColours) { return false; };

#ifndef SWIG
class NCSECW_IMPEX CWriterBase: public CThread {
	public:
		explicit CWriterBase(CView *pView) {
				m_pView = pView;
				m_bLowMemCompress = false;
				m_bCompressMTRead = true;//NCSGetNumPhysicalCPUs() > 1;
				m_nNextLine = 0;
				m_bReturnCompressionMemory = false;
				memset(&m_Counters, 0x0, sizeof(CView::CompressionCounters));
				m_Counters.eType = pView->GetCompressInputType();
				m_eResult = NCS_SUCCESS;
			};
		virtual ~CWriterBase() {
				m_pView = NULL;
				FreeCompressBuffers();
				if(m_bReturnCompressionMemory) {
					NCS::SDK::CFileBase::sConfig().m_nMaximumCacheSize += NCS::SDK::CFileBase::sConfig().m_nMaximumCompressionMemorySize;
					NCS::SDK::CFileBase::sConfig().m_nMaximumCompressionMemorySize = 0;
				}
				
			};
		virtual CError Open(const CString &URLPath, CIOStream *pStream) = 0;
		virtual NCSFileInfo *GetFileInfo() = 0;
		virtual CError WriteLineBIL(NCSCellType eType, UINT16 nBands, void **ppOutputLine, UINT32 *pLineSteps = NULL) = 0;
		virtual NCSFileType GetFileType() = 0;
				/** Copy the parameters from the m_Parameters vector to the File instance */
		virtual void SetFileParameters() {};

		virtual SDK::CFileBase *GetFile() { return(m_pView ? m_pView->GetFile() : NULL); };
		virtual CView *GetView() { return(m_pView); };

		NCS_INLINE UINT32 &NextLine() { return(m_nNextLine); };
		NCS_INLINE void NextLine(UINT32 n) { m_nNextLine = n; };

		virtual CError ConvertGeotiffToFileInfo(const NCS::ECW::GeotiffTagBox* pGeotiffConverter) = 0;

		virtual CError WriteTiled() { return NCS_COULDNT_PERFORM_COMPRESSION; };
		virtual void PushFreeBuffer(SDK::CBuffer2DVector *pBuffers) {
			m_CompressFreeQueue.PushBuffer(pBuffers);
		}
		virtual void SetWriteFailed(NCS::CError Error)  {
			m_eResult = Error;
		}
		virtual NCS::CError GetWriteCurrentStatus() {
			return m_eResult;
		}
		CView::CompressionCounters &Counters() { return m_Counters; };
		virtual bool NeedConvertRGBToYUV() { return false; };

		virtual bool CheckWriteErrorOrCancel()
		{
			if (m_pView->WriteCancel())
			{
				m_eResult = NCS_USER_CANCELLED_COMPRESSION;
				return true;
			}
			else
			{
				return (m_eResult != NCS_SUCCESS);
			}
		}
			
		
	    
	protected:
		friend class CView;
		NCS::CError m_eResult;
		bool m_bReturnCompressionMemory;
		virtual UINT64 EstimateMemoryRequired() = 0;
		NCS::CError ValidateMemoryRequirements();
		CView *m_pView;
			/** Compression buffer queues */
		SDK::CBuffer2DQueue m_CompressReadQueue;
		SDK::CBuffer2DQueue m_CompressFreeQueue;

		CView::CompressionCounters m_Counters;
			/** Multi-thread compression */
		bool m_bCompressMTRead;
			/** Performing low-memory compression */
		bool m_bLowMemCompress;

			/** Next line to write */
		UINT32		m_nNextLine;
	
		virtual void AllocCompressBuffers() {
			if(m_bCompressMTRead) {
				m_CompressFreeQueue.Alloc(m_bLowMemCompress ? 2 : MAX_THREAD_LINES, m_pView->m_nNumberOfBands);
			} else {
				m_CompressFreeQueue.Alloc(1, m_pView->m_nNumberOfBands);
			}
		};
		virtual void FreeCompressBuffers()	{
			Stop(true);
			// added on 08/09/2014 to force allow free below
			{
				m_CompressFreeQueue.Lock();
				for (UINT32 nQueue = 0; nQueue < m_CompressFreeQueue.size(); nQueue++){
					SDK::CBuffer2DVector *pBuffers = m_CompressFreeQueue.at(nQueue);
					if (pBuffers) {
						// set to it's owner to force free
						for (UINT32 nBuffer = 0; nBuffer < pBuffers->size(); nBuffer++){
							(*pBuffers)[nBuffer].SetFlag(SDK::CBuffer2D::AT_OWNER);
						}
					}
				}
				m_CompressFreeQueue.UnLock();
			}
			m_CompressFreeQueue.Free();
			m_CompressReadQueue.Free();
		};
		
		
	};
#endif  //SWIG
#endif  // NCS_MOBILE
	
	/** 
	 * Get default RGBA band list for image, for multiband images without proper band descriptions or that don't
	 * contain real RGB bands the SDK will guess the band order.
	 *
	 * @param		Bandlist	The number of bands set in pBandlist, will either be 3 or 4 depending on if the dataset
								has an opacity channel
	 * @return      NCSError	result.
	 */
	virtual NCS::CError GetDefaultRGBABandList(std::vector<UINT32> &Bandlist);

	/** 
	 * Get default RGB band list for image, for multiband images without proper band descriptions or that don't
	 * contain real RGB bands the SDK will guess the band order.
	 *
	 * @param		Bandlist	The number of bands set in pBandlist, will either be 3 or 4 depending on if the dataset
								has an opacity channel
	 * @return      NCSError	result.
	 */
	virtual NCS::CError GetDefaultRGBBandList(std::vector<UINT32> &Bandlist);

	/** 
	 * Get current ViewSetinfo structure.
	 * @return      NCSSetViewInfo*	Return pointer to the current SetViewInfo.
	 */
	virtual /*const*/ NCSSetViewInfo *GetFileViewSetInfo();
		/** 
		 * Get FileInfo structure.
		 * @return      NCSFileInfo*	Return pointer to the FileInfo.
		 */
	virtual /*const*/ NCSFileInfo *GetFileInfo();
		/** 
		 * Set FileInfo structure.
		 * @param		Info		New fileinfo - used to specify file info for compression
		 * @return      CError	Return pointer to the FileInfo.
		 */
	virtual CError SetFileInfo(NCSFileInfo &Info);

		/**
		  * Get the next UUID box with the specified UUID from the file.
		  * @param		uuid			UUID of UUID box to find.
		  * @param		pLast			Last UUID box found, else NULL for first
		  * @return		CBox*		Pointer to box if found, else NULL;
		  */
	virtual SDK::CBox *GetUUIDBox(UUID uuid, SDK::CBox *pLast = NULL);
		/**
		  * Get the next XML box from the file.
		  * @param		pLast			Last XML box found, else NULL for first
		  * @return		CBox*		Pointer to box if found, else NULL;
		  */
	virtual SDK::CBox *GetXMLBox(SDK::CBox *pLast = NULL);
		/**
		  * Get the next box of the specified type from the file.
		  * @param		nTBox			Box type to find
		  * @param		pLast			Last box found, else NULL for first
		  * @return		CBox*		Pointer to box if found, else NULL;
		  */
	virtual SDK::CBox *GetBox(UINT32 nTBox, SDK::CBox *pLast = NULL);
		/**
		  * Add a box to be written to the file on compression.

		  * The specified box's UnParse() method will be called to write the box into the file.
		  * The SDK will not take ownership of this memory address and will not attempt to delete it.
		  * @param		pBox			Box to be written to file
		  * @return		CError		returned error or NCS_SUCCESS;
		  */
	virtual CError AddBox(SDK::CBox *pBox);

		/**
		  * Get a GDT (ER Mapper) Projection/Datum pair for the given EPSG code, if available
		  * @param		nEPSGCode		EPSG Code to find projection/datum for
		  * @param		ppProjection	Returned GDT Projection name - free with NCSFree()
		  * @param		ppDatum			Returned GDT Datum name - free with NCSFree()
		  * @return		CError		Error code;
		  */
	static CError GetProjectionAndDatum(const INT32 nEPSGCode, char **ppProjection, char **ppDatum, bool bUseEpsgCodeIfFails = false);
		/**
		  * Get an EPSG code from the given GDT (ER Mapper) Projection/Datum pair if available
		  * @param		pProjection		GDT Projection name
		  * @param		pDatum			GDT Datum name
		  * @param		nEPSGCode		Returned EPSG Code or 0 on error
		  * @return		CError		Error code;
		  */
	static CError GetEPSGCode(const char *pProjection, const char *pDatum, INT32 *nEPSGCode);
		/**
		  * Get the custom GDT data path (for EPSG<-->ER Mapper projection/datum mapping)
		  * @return the custom path being used, or NULL
		  */
	static char *GetGDTPath();
		/**
		  * Set the custom GDT data path (for EPSG<-->ER Mapper projection/datum mapping)
		  * @param szPath the new GDT data path
		  */
	static void SetGDTPath(const char *szPath);
		/**
		  * Refresh the custom GDT data path, doing a search and validity check on 
		  * the current path value and several common GDT data locations
		  */
	static void DetectGDTPath();

	static bool AddCustomEPSGCodes(char *pMapping);

		/** 
		  * Set refresh callback function 
		  * @param		pCallback		Refresh callback function to use
		  * @return		CError		Error code;
		  */
	CError SetRefreshCallback(NCSReadStatus (*pCallback)(NCSFileView*));

		/** 
		 * Get a pointer to the underlying CIOStream.
		 * This can be used to read specific JP2 UUID/XML boxes etc.
		 * This may also be useful if the underlying stream object belongs to a 
		 * custom subclass of CIOStream that you have passed to the overloaded
		 * method CFile::Open(CIOStream *pStream, bool bProgressiveDisplay).
		 * @return      CIOStream*	Return pointer to the CIOStream, else NULL.
		 */
	CIOStream *GetStream();

		/**
		  * Get a statistic of NCSTimeStampMs type
		  * @param		eType				Statistic to find
		  * @param		Val					Statistic value
		  */		 
	virtual void GetStatistic(Statistic eType, NCSTimeStampMs &Val);
		/**
		  * Get a statistic of UINT64 type
		  * @param		eType				Statistic to find
		  * @param		Val					Statistic value
		  */		 
	virtual void GetStatistic(Statistic eType, UINT64 &Val);
		/**
		  * Reset the specified statistic to the default value
		  * @param		eType				Statistic to reset
		  */		 
	virtual void ResetStatistic(Statistic eType);
		/**
		  * Reset all statistics to their default values
		  */		 
	virtual void ResetStatistics();
		/**
		  * Set a parameter on the view.  These over-ride default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		eType			Parameter type to set.
		  * @param		...				variable arguments to get
		  * @return		CError			Error code
		  * @code
		  *   error = pView->SetParameter(JPC_DECOMPRESS_AUTOSCALE_UP, TRUE);
		  * @endcode
		  */
	virtual CError SetParameter(Parameter eType, ...);
		/**
		  * Set a parameter on the view.  These over-ride default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		sName			Fully-qualified Name of parameter (eg "JPC:DECOMPRESS:AUTOSCALE:UP").
										<h4>Available decode parameters:</h4>
										- "NCS:RESAMPLE:METHOD"

											Enum NCSResampleMethod. The resample method to use.  
										- "NCS:BLOCKING:TIME:MS"

											UINT32. The time the SDK will wait for blocks when decoding an ECW or JP2 file over ECWP in a non-progressive CView. 
										- "ECW:READER:DITHER:ENABLE"

											BOOL. If TRUE, the SDK will apply a dither texture on applicable Views for applicable ECW files on decode.  This should be disabled when transcoding from an ECW file.
										- "ECWP:HTTP:HEADERS"
											
											char *.  Optional HTTP headers to send to the ECWP server when decoding over ECWP.  The SDK will make a copy of the string.
										- "JPC:DECOMPRESS:AUTOSCALE:UP"
										
											BOOL. If TRUE the SDK will upscale from the stored range to the target range.  E.g. 1 bit AllOpacity will be scaled to 8bit when reading into an 8bit buffer (e.g. ReadLineRGBA).
										- "JP2:DECOMPRESS:LAYERS"
										
											UINT32.  The maximum number of quality layers to decode when decoding a JP2.
										- "JPC:DECOMPRESS:RECONSTRUCTION:PARAMETER"

											double. Arbitrary value usually chosen by the decoder between 0.0-1.0.
										<h4>Available encode parameters:</h4>
										- "ECW:BLOCKSIZE:X"

											INT32.  The x size of an ECW block in the wavelet pyramid.  Must be >= 64 and be a power of two.
										- "ECW:BLOCKSIZE:Y"

											INT32.  The y size of an ECW block in the wavelet pyramid.  Must be >= 64 and be a power of two.
										- "JP2:COMPRESS:PROFILE:NITF:BIIF:NPJE"

											BOOL.  The SDK will configure a JP2 for the NPJE profile.
										- "JP2:COMPRESS:PROFILE:NITF:BIIF:EPJE"

											BOOL.  The SDK will configure a JP2 for the EPJE profile.
										- "JP2:COMPRESS:PROGRESSION:LRCP"
										
											BOOL.  Use Layer, Reolution, Component, Precinct progression order in an output JP2.
										- "JP2:COMPRESS:PROGRESSION:RLCP"
										
											BOOL.  Use Reolution, Layer, Component, Precinct progression order in an output JP2.
										- "JP2:COMPRESS:PROGRESSION:RPCL"
										
											BOOL.  Use Reolution, Precinct, Component, Layer progression order in an output JP2.
										- "JP2:COMPRESS:INCLUDE:SOP"
										
											BOOL.  Include Start of Packet Marker in an output JP2.
										- "JP2:COMPRESS:INCLUDE:EPH"
										
											BOOL.  Include End of Packet Header Marker in an output JP2.
										- "JP2:COMPRESS:CODESTREAM:ONLY"
										
											BOOL.  Only write the JP2 codestream to the output JP2.
										- "JP2:COMPRESS:PRECINCT:WIDTH"
										
											INT32.  The precinct width in the output JP2.
										- "JP2:COMPRESS:PRECINCT:HEIGHT"
										
											INT32.  The precinct height in the output JP2.
										- "JP2:COMPRESS:LEVELS"
										
											INT32.  The number of resolution levels in the output JP2.
										- "JP2:COMPRESS:LAYERS"
										
											INT32.  The number of quality layers in the output JP2.
		  * @param		...				variable arguments to set
		  * @return		CError			Error code
		  * @code
		  *   error = pView->SetParameter("JPC:DECOMPRESS:AUTOSCALE:UP", TRUE);
		  * @endcode
		  */
	virtual CError SetParameter(const char *sName, ...);
	
		/**
		  * Get a parameter on the view.  These over-ride default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		eType			Parameter GetParameterInternaltype to set.
		  * @param		...				variable arguments to get
		  * @return		CError			Error code
		  * @code
		  *    int nAutoScaleUp = -1;
		  *    pView->GetParameterV(JPC_DECOMPRESS_AUTOSCALE_UP, &nAutoScaleUp);
		  * @endcode
		  */
	virtual CError GetParameter(Parameter eType, ...);
		/**
		  * Get a parameter on the view.  These over-ride default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		sName			Fully-qualified Name of parameter (eg "JPC:DECOMPRESS:AUTOSCALE:UP").
										<h4>Available decode parameters:</h4>
										- "JPC:DECOMPRESS:TILENR:X"

											UINT32 *. Number of Tiles on X axis. "TILES_X" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JPC:DECOMPRESS:TILENR:Y"

											UINT32 *. Number of Tiles on Y axis. "TILES_Y" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JPC:DECOMPRESS:TILESIZE:X"

											UINT32 *. Tile size on X axis. "TILE_WIDTH" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JPC:DECOMPRESS:TILESIZE:Y"

											UINT32 *. Tile size on Y axis. "TILE_HEIGHT" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JPC:DECOMPRESS:PRECINCTSIZE:X"

											char **. Precinct sizes for each resolution level (smaller to bigger level) on X axis. char *. "PRECINCT_SIZE_X" in ECWJP2 GDAL driver is equivalent to the string in the SDK.  Caller must free the string.
										- "JPC:DECOMPRESS:PRECINCTSIZE:Y"

											char **. Precinct sizes for each resolution level (smaller to bigger level) on Y axis. char *. "PRECINCT_SIZE_Y" in ECWJP2 GDAL driver is equivalent to the string in the SDK.  Caller must free the string.
										- "JPC:DECOMPRESS:CODEBLOCK:X"

											UINT32 *. Code block size on X axis. "CODE_BLOCK_SIZE_X" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JPC:DECOMPRESS:CODEBLOCK:Y"

											UINT32 *. Code block size on Y axis. "CODE_BLOCK_SIZE_Y" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JPC:DECOMPRESS:BITDEPTH"

											char **. List the bit depth (from JP2 SIZ Marker) per band. "PRECISION" in ECWJP2 GDAL driver is equivalent to the string in the SDK.  Caller must free the string.
										- "JPC:DECOMPRESS:RESOLUTION:LEVELS"

											UINT32 *. Resolution levels of the image pyramid. "RESOLUTION_LEVELS" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JP2:DECOMPRESS:LAYERS"

											UINT32 *. Number of packets in one precinct. A packet contains all compressed image data for a layer at a specific tile, component, resolution level and precinct. UINT32. "QUALITY_LAYERS" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JPC:DECOMPRESS:PROGRESSION:ORDER"

											char **. Progression order specifies the order of packets placed in a JPC stream, ie, among four dimensions: layer (L), component (C), resolution level (R) and precinct (P position). There are 5 orders LRCP, RLCP, RPCL, PCRL and CPRL. For instance, LRCP indicates Layer-Resolution Level-Component-Position order. char *. "PROGRESSION_ORDER" in ECWJP2 GDAL driver is equivalent to the string in the SDK.  Caller must free the string.
										- "JP2:TRANSFORMATION:TYPE"

											UINT32 *. Filter type for wavelet transformation. 0 represents 9x7 filter, 1 for 5x3 filter. UINT32. "TRANSFORMATION_TYPE" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JP2:DECOMPRESS:SOP:EXISTS"

											BOOL *. If TRUE, SOP marker is used. It marks the beginning of a packet within a JPC stream. Its usage is optional, but useful for error resilience. BOOLEAN. "USE_SOP" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JP2:DECOMPRESS:EPH:EXISTS"

											BOOL *. If TRUE, EPH marker is used. It marks the end of a packet header within bit stream or in the PPM or PPT marker segments. Its usage is optional. BOOLEAN. "USE_EPH" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "JP2:GML:JP2:BOX:EXISTS"

											BOOL *. If TRUE, the JP2 file contains georeferencing XML (GML) boxes. BOOLEAN. "GML_JP2_DATA" in ECWJP2 GDAL driver is equivalent to the string in the SDK
										- "ECW:READER:DITHER:ENABLE"

											BOOL *. If TRUE, the SDK will apply a dither texture on applicable Views for applicable ECW files on decode.
		  * @param		...				variable arguments to get
		  * @return		CError			Error code
		  * @code
		  *    int nAutoScaleUp = -1;
		  *    pView->GetParameter("JPC:DECOMPRESS:AUTOSCALE:UP", &nAutoScaleUp);
		  * @endcode
		  */
	virtual CError GetParameter(const char *sName, ...);
	
		/**
		  * Set the usage of geographical metadata when reading and writing JPEG 2000 files.
		  * @param		nGeodataUsage	NCSGeodataUsage enum value specifying the desired metadata types
		  */
	static void SetGeodataUsage(NCSGeodataUsage nGeodataUsage);

		/**
		  * Get the percent complete (from time of setview == 0%)
		  * @return		INT32			Returned Percent complete
		  */		 
	INT32 GetPercentComplete();

		/**
		  * Get the percent complete as a percentage of total blocks in the current view.
		  * @return		INT32			Returned Percent complete
		  */		 
	INT32 GetPercentCompleteTotalBlocksInView();

		/**
		  * Get the EPSG code associated with this view's projection and datum, if any.
		  * @return		INT32			EPSG code, or 0 if none applies.
		  */
	INT32 GetEPSGCode();

		/**
		  * Call this function to enable compression.
		  * NOTE: Verify you are in compliance with the appropriate license agreements.
		  * Calling this function signifies you accept the terms of the appropriate license.
		  */		 
	static void SetOEMKey(const char *szCompanyName, const char *szKey);

#ifdef NCS_MOBILE
		/**
		  * Call this function to set your decompression key for mobile devices
		  * NOTE: Verify you are in compliance with the appropriate license agreements.
		  * Calling this function signifies you accept the terms of the appropriate license.
		  */	
	static void SetOEMMobileDecompressionKey(const char *szCompanyName, const char *szKey);
#endif
		/**
		  * Get the type of the currently open file
		  * @return		NCSFileType			The file type.
		  */
	virtual NCSFileType GetFileType();

		/**
		  * Get the Mime type of the currently open file
		  * @return		The Mime Type string or NULL if error.
		  */
	char *GetFileMimeType();
	
		/**
		  * Return the Mime type of a file given the file type.
		  * @param		NCSFileType			The file type.
		  * @return		The Mime Type string or NULL if error.
		  */
	static char *GetFileMimeType(NCSFileType fileType);

	void _Lock() { m_Mutex.Lock(); };
	bool _TryLock() { return(m_Mutex.TryLock()); };
	void _UnLock() { m_Mutex.UnLock(); };

	/**
		  * Set Compress Client - Internal func for "C" API support only
		  * @param		pCompressClient	ECW Compress Client struct
		  * @return		CError		Error code;
		  */		 
	CError SetCompressClient(NCSCompressClient *pCompressClient);

	//FIXME TEMP public till ECW in C++
				/** ECW SetViewInfo Structure */
	NCSSetViewInfo m_CurrentView;
	IEEE8 m_dCellTLX, m_dCellTLY, m_dCellBRX, m_dCellBRY;
	bool m_bSetViewWorldCoordinates;

				/** Display update is progressive for this view */
	bool m_bIsProgressive;
	/** Timestamp representing last time a blocking read was called */
	NCSTimeStampMs	m_tsLastBlockTime; 
	/** Timestamp representing last time a RefreshUpdate was called */
	NCSTimeStampMs	m_tsLastRefreshUpdateTime;
	virtual void Work();

protected:
#ifndef SWIG
	friend class ECW::CReader;
#ifndef NCS_MOBILE
	friend class CWriterBase;
	friend class ECW::CWriter;
	friend class JP2::CWriter;
	friend class ECW::CUpdater;
#endif
	friend class JP2::CFile;
	friend class JP2::CReader;
	friend class SDK::CFileBase;
	friend class JPC::CResampler;
	friend class CViewParameterHelper;
		/** Set view mode is world? */
	bool m_bSetViewModeIsWorld;
	CError Close(bool bFreeCache, bool bDoNotReleaseFileInfo);

	class CTransformer : public NCS::CMultiLock
	{
	public:
		CTransformer();

		virtual ~CTransformer();

		NCS::CError Reset(NCS::CView *pView);
		NCS::CError Add(INT32 nBand, API::ITransform *pTransform);
		NCS::CError Set(INT32 nBand, API::ITransform *pTransform);
		NCS::CError SetList(INT32 nBand, NCS::API::CTransformList &TransformList);
		NCS::CError GetList(INT32 nBand, API::CTransformListAccessor &Accessor);
		void Clear();
		NCS::CError Transform(INT32 nBand, NCSCellType eSrcDataType, INT32 nSrcStep, NCS::SDK::CBuffer2D &Src, 
										   NCSCellType eDstDataType, INT32 nDstStep, NCS::SDK::CBuffer2D &Dst);

		const CTransformer &operator=(const CTransformer &s);
		bool CustomTransformsSet() { return m_bCustomTransformsSet; };
	protected:
		std::vector<NCS::API::CTransformList> m_Transforms;
		std::vector<API::ITransform *> m_DefaultTransforms;
		bool m_bCustomTransformsSet;
		bool m_bOwnsTransforms;
	private: 
		CTransformer(const CTransformer &s);
	};
	CTransformer m_Transformer;

		/** 
		 * Internal method to set a view into the JP2 file for reading.
		 * @param		nBands			Number of bands in pBandList to read
		 * @param		pBandList		Array of band indices to read.
		 * @param		nWidth			Width of the view in pixels
		 * @param		nHeight			Height of the view in pixels
		 * @param		nDatasetTLX		Top left X dataset coordinate of view
		 * @param		nDatasetTLY		Top left Y dataset coordinate of view
		 * @param		nDatasetBRX		Bottom right X dataset coordinate of view
		 * @param		nDatasetBRY		Bottom right Y dataset voordinate of view
		 * @param		dWorldTLX		Top left X world coordinate of view (informative only)
		 * @param		dWorldTLY		Top left Y world coordinate of view (informative only)
		 * @param		dWorldBRX		Bottom right X world coordinate of view (informative only)
		 * @param		dWorldBRY		Bottom right Y world coordinate of view (informative only)
		 * @param		bFormWorker		Is this call from the worker thread or external?
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError SetViewInternal(UINT32 nBands, UINT32 *pBandList, 
					   		  UINT32 nDatasetTLX, UINT32 nDatasetTLY,
							  UINT32 nDatasetBRX, UINT32 nDatasetBRY,
							  UINT32 nWidth, UINT32 nHeight,
							  IEEE8 dWorldTLX = 0.0, IEEE8 dWorldTLY = 0.0,
							  IEEE8 dWorldBRX = 0.0, IEEE8 dWorldBRY = 0.0, bool bFromWorker = false);
		/** 
		 * Set a pending view on the JP2 file for reading.
		 * Actual SetView() will occur from within worker thread when it processes the pending view.
		 * @param		nBands			Number of bands in pBandList to read
		 * @param		pBandList		Array of band indices to read.
		 * @param		nWidth			Width of the view in pixels
		 * @param		nHeight			Height of the view in pixels
		 * @param		nDatasetTLX		Top left X dataset coordinate of view
		 * @param		nDatasetTLY		Top left Y dataset coordinate of view
		 * @param		nDatasetBRX		Bottom right X dataset coordinate of view
		 * @param		nDatasetBRY		Bottom right Y dataset voordinate of view
		 * @param		dWorldTLX		Top left X world coordinate of view (informative only)
		 * @param		dWorldTLY		Top left Y world coordinate of view (informative only)
		 * @param		dWorldBRX		Bottom right X world coordinate of view (informative only)
		 * @param		dWorldBRY		Bottom right Y world coordinate of view (informative only)
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError SetPendingView(UINT32 nBands, UINT32 *pBandList, 
									 UINT32 nDatasetTLX, UINT32 nDatasetTLY,
									 UINT32 nDatasetBRX, UINT32 nDatasetBRY,
									 UINT32 nWidth, UINT32 nHeight,
									 IEEE8 dWorldTLX, IEEE8 dWorldTLY,
									 IEEE8 dWorldBRX, IEEE8 dWorldBRY);
		/** 
		 * Validates the view parameters for correctness.
		 * @param		nBands			Number of bands in pBandList to read
		 * @param		pBandList		Array of band indices to read.
		 * @param		nWidth			Width of the view in pixels
		 * @param		nHeight			Height of the view in pixels
		 * @param		nDatasetTLX		Top left X dataset coordinate of view
		 * @param		nDatasetTLY		Top left Y dataset coordinate of view
		 * @param		nDatasetBRX		Bottom right X dataset coordinate of view
		 * @param		nDatasetBRY		Bottom right Y dataset voordinate of view
		 * @param		dWorldTLX		Top left X world coordinate of view (informative only)
		 * @param		dWorldTLY		Top left Y world coordinate of view (informative only)
		 * @param		dWorldBRX		Bottom right X world coordinate of view (informative only)
		 * @param		dWorldBRY		Bottom right Y world coordinate of view (informative only)
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError ValidateViewParams(UINT32 nBands, UINT32 *pBandList, 
										 UINT32 nDatasetTLX, UINT32 nDatasetTLY,
										 UINT32 nDatasetBRX, UINT32 nDatasetBRY,
										 UINT32 nWidth, UINT32 nHeight,
										 IEEE8 dWorldTLX, IEEE8 dWorldTLY,
										 IEEE8 dWorldBRX, IEEE8 dWorldBRY);
			/** 
		 * Read the next line in RGB UINT8 triplet format from the current view into the file.
		 * @param		pRGBTripler			Pointer to UINT8 buffer to receive RGB data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineRGBInternal(UINT8 *pRGBTriplet);
		/** 
		 * Read the next line in BGR UINT8 triplet format from the current view into the file.
		 * @param		pBGRTripler			Pointer to UINT8 buffer to receive BGR data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBGRInternal(UINT8 *pBGRTriplet);
		/** 
		 * Read the next line in RGBA UINT32 format from the current view into the file.
		 * @param		pRGBA				Pointer to UINT32 buffer to receive RGBA data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineRGBAInternal(UINT32 *pRGBA);
		/** 
		 * Read the next line in BGRA UINT32 format from the current view into the file.
		 * @param		pBGRA				Pointer to UINT32 buffer to receive BGRA data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBGRAInternal(UINT32 *pBGRA);
		/** 
		 * Read the next line in ARGB UINT32 format from the current view into the file.
		 * @param		pARGB				Pointer to UINT32 buffer to receive ARGB data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineARGBInternal(UINT32 *pARGB);
		/** 
		 * Read the next line in ABGR UINT32 format from the current view into the file.
		 * @param		pABGR				Pointer to UINT32 buffer to receive ABGR data
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineABGRInternal(UINT32 *pABGR);
		/** 
		 * Read the next line in BIL format from the current view into the file.
		 * @param		eType				Output buffer type
		 * @param		nBands				Number of output bands
		 * @param		ppOutputLine		Array of buffer pointers, one buffer for each band
		 * @param		eOuputDataType		Output data type, e.g. you might want UINT8 data in an INT16 buffer									
		 * @param		pLineSteps			Line steps, in CELLS.
		 * @return      NCSReadStatus	Read status code
		 */
	virtual NCSReadStatus ReadLineBILInternal(NCSCellType eType, UINT16 nBands, void **ppOutputLine, NCSCellType eOuputDataType, UINT32 *pLineSteps = NULL);

		/**
		  * Fire a refresh update,
		  * @return		NCSReadStatus	RefreshUpdate Read status
		  */
	NCSReadStatus FireRefreshUpdate();
		/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		pURLPath	Full path name of JP2 file to open
		 * @param		pStream		IO Stream to open on
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError OpenForRead(const CString &URLPath, CIOStream *pStream, bool bProgressiveDisplay);
#ifndef NCS_MOBILE
		/** 
		 * Open the JP2 file for Parsing/UnParsing.
		 * @param		pURLPath	Full path name of JP2 file to open
		 * @param		pStream		IO Stream to open on
		 * @param		bProgressiveDisplay		Open for preogressive reading.
		 * @return      CError		NCS_SUCCESS or error code on failure.
		 */
	virtual CError OpenForWrite(const CString &URLPath, CIOStream *pStream, bool bProgressiveDisplay, NCSFileType eType);	
#endif

	CError OpenOpacityFileView(SDK::CFileBase *pFile, bool bProgressiveDisplay, NCSFileType eType);

		/** 
		 * Set FileInfo structure.
		 * @param		Info		New fileinfo - used to specify file info for compression
		 * @return      CError	Return pointer to the FileInfo.
		 */
	virtual CError SetFileInfoInternal(NCSFileInfo &Info);

		/** Add a dynamically allocated node to the dynamic node list so it is released */
	void AddDynamicNode(SDK::CNode2D *pNode) { m_DynamicNodes.push_back(pNode); };
		/**
		  * Set a parameter on the view.  These over-ride default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		sName			Fully-qualified Name of parameter (eg "JP2:COMPRESS:PROFILE:BASELINE0").
		  * @param		va				varargs list
		  * @return		CError			Error code
		  */
	virtual CError SetParameterInternal(const char *sName, va_list va);

		/**
		  * Get a parameter on the view.  Get the default settings, such as compression output profile,
		  * progression order, tiling etc
		  * @param		sName			Fully-qualified Name of parameter (eg "JP2:COMPRESS:PROFILE:BASELINE0").
		  * @param		va				varargs list
		  * @return		CError			Error code
		  */
	virtual CError GetParameterInternal(const char *sName, va_list va);

#ifndef NCS_MOBILE
	//Check if there is information setting problem before encoding
	CError CheckSettingsBeforeEncoding();
	bool m_bHasCheckedSetting;
	SDK::CRect2D* GetUpdateRectPtr();
#endif

	virtual UINT32 &NextLine();
	virtual void NextLine(UINT32 n);

	
	
	NCS::ECW::GeotiffTagBox* m_pGeotiffTagConverter;

	SDK::CNode2D::ContextID	m_CtxID;
	
		/** Connection has been lost to server (ECWP) */
	bool m_bConnectionLost;
		/** SetView is valid */
	bool m_bHaveValidSetView;
		/** File is open */
	bool m_bIsWrite;
		/** Autoscale data up on read */
	bool m_bAutoScaleUp;

		/** Do we have a pending setview */
	CMutex m_PendingViewMutex;
	INT32	m_nPendingViews;
	INT32	m_nCancelledViews;
		/** Progressive pending View */
	NCSSetViewInfo m_PendingView;

	/**
	 * ParameterValuePair class - holds compression parameters prior to CFile instance being created on the Open()
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #6 $ $Author: scope $ $Date: 2007/12/14 $ 
	 */
#ifndef SWIG
	class NCSECW_IMPEX ParameterValuePairBase {
	public:
		explicit ParameterValuePairBase(const char *pName) { m_pName = NCSStrDup(pName); };
		virtual ~ParameterValuePairBase() { NCSFree(m_pName); };
		virtual bool Is(const char *pName) { return(!NCSstricmp(pName, m_pName)); };
		virtual void GetValue(void *pValue) = 0;
	protected:
			/** Parameter name */
		char	*m_pName;
	};
	template<class T> class NCSECW_IMPEX ParameterValuePair: public ParameterValuePairBase {
	public:
			/** No-parameter constructor */
		explicit ParameterValuePair(const char *pName) :ParameterValuePairBase(pName) {};
			/** bool-parameter constructor */
		ParameterValuePair(const char *pName, T Value):ParameterValuePairBase(pName) { m_Value = Value; };
			/** Virtual destructor */
		virtual ~ParameterValuePair() {};
		
		virtual void GetValue(void *pValue) { *((T*)pValue) = m_Value; };
	protected:
			/** Parameter value */
		T		m_Value;
	};
#endif

		/** Number of blocks in cache at the last SetView() */
	UINT32	m_nBlocksAtLastRefreshUpdate;
		/** Current in RefreshUpdate() callback */
	bool	m_bInRefreshUpdate;
		/** Cancel the read */
	bool	m_bCancelRead;

		/** Force to cancel the read. added on 31/01/2013 */
	bool	m_bForceCancelRead;

	/** Refresh time, defaults to global NCSCFG_REFRESH_TIME_MS */
	UINT32 m_nRefreshTimeMS;
	/** Blocking delay, defaults to global NCSCFG_BLOCKING_TIME_MS */
	UINT32	m_nBlockingTimeMS;

		/** parameters held until open() */
	std::vector<ParameterValuePairBase*> m_Parameters;
		/** Copy the parameters from the m_Parameters vector to the CFile instance */
	void SetFileParameters();
		/** List of other JP2 box(es) to be included in the output file */
	SDK::CBox::CBoxList m_OtherBoxes;	

		/** Used when SetView is >4000 in either dimension - too large to cache */
	bool	m_bTiledView;
		/** dynamic Strip Height, added on 27/07/2012 */
	UINT32	m_nStripHeight;
		/** dynamic Strip Height for accurate calculation, added on 28/08/2012 */
	IEEE8	m_dStripHeight;	
		/** save the resolution level to decode when m_nStripHeight is updated, added on 31/10/2014 */
	UINT32	m_nResToDecode;
		/** This member stores a view "tile" (horizontal strip) when view > 4K x 4K */
	CView *m_pTiledView;
		/** Set the tiled views */
	CError SetTiledView(NCSSetViewInfo &View, UINT32 nReadLine);

		/** Used when reading extra opacity file */
	bool	m_bIsAppendedOpacityView;
		/** This member stores an opacity view */
	CView *m_pOpacityView;
		/** Set the opacity view */
	CError SetOpacityView(NCSSetViewInfo &View);

		/** Dynamic Nodes to free */
	SDK::CNode2DVector m_DynamicNodes;

		// The following private members are here to support the "C" API only.
		/** Progressive read callback for ECW decompression */
	NCSReadStatus (*m_pRefreshCallback)(NCSFileView *pNCSFileView);
		/** ECW Compress Client for "C" compression API */

		/** Compression Start timestamp */
	NCSTimeStampMs	m_tsCompressStart;
		/** Compression End timestamp */
	NCSTimeStampMs	m_tsCompressEnd;
		/** Compression Error status */
	CError		m_CompressError;

	CReaderBase *m_pReader;
	NCSCompressClient *m_pCompressClient;

#ifndef NCS_MOBILE


	CWriterBase *m_pWriter;
	SDK::CRect2D *m_pUpdateRect;
#endif
		/** Do Blocking mode wait */
	virtual void DoBlockingWait();
		/** Update the public file members */
	virtual bool UpdateFileInfo(void); 
		/** Update the public view members */
	virtual bool UpdateFileViewInfo(void);

		/** Delete all dynamic nodes */
	virtual void DeleteDynamicNodes(void);

		/** Update the view statistics */
	virtual void UpdateViewStats();

		/** Http Headers that are passed to cnet */
	NCS::CString m_sECWPHttpHeaders;

#endif //SWIG

public:
		/** Check if the ECWP connection is using streaming or polling mode */
	virtual bool IsConnected();
	virtual bool Reconnect();
	virtual bool IsConnectionPolling();

	/**	
	 *	Set the view on the open file.  This version takes world coordinates as input.
	 *
	 *	@param[in]	nBands				The number of bands to include in the view being set.
	 *	@param[in]	pBandList			An array of band indices specifying which bands to include and in which order.
	 *	@param[in]	nWidth				The width of the view to construct in dataset cells.
	 *	@param[in]	nHeight				The height of the view to construct in dataset cells.
	 *	@param[in]	dWorldTLX			The left of the view to construct in world coordinates.
	 *	@param[in]	dWorldTLY			The top of the view to construct in world coordinates.
	 *	@param[in]	dWorldBRX			The right of the view to construct in world coordinates.
	 *	@param[in]	dWorldBRY			The bottom of the view to construct in world coordinates.
	 *	@return							NCSError value, NCS_SUCCESS or any applicable error code
	 */
	virtual CError SetView ( INT32 nBands, INT32 *pBandList, 
					   INT32 nWidth, INT32 nHeight,
					   IEEE8 dWorldTLX, IEEE8 dWorldTLY,
					   IEEE8 dWorldBRX, IEEE8 dWorldBRY );
	/**	
	 *	Set the view on the open file.  This version takes dataset coordinates as input.
	 *
	 *	@param[in]	nBands				The number of bands to include in the view being set.
	 *	@param[in]	pBandList			An array of band indices specifying which bands to include and in which order.
	 *	@param[in]	nWidth				The width of the view to construct in dataset cells.
	 *	@param[in]	nHeight				The height of the view to construct in dataset cells.
	 *	@param[in]	nDatasetTLX			The left of the view to construct in dataset coordinates.
	 *	@param[in]	nDatasetTLY			The top of the view to construct in dataset coordinates.
	 *	@param[in]	nDatasetBRX			The right of the view to construct in dataset coordinates.
	 *	@param[in]	nDatasetBRY			The bottom of the view to construct in dataset coordinates.
	 *	@return							NCSError value, NCS_SUCCESS or any applicable error code
	 */
	virtual CError SetView ( INT32 nBands, INT32 *pBandList, 
					   INT32 nWidth, INT32 nHeight,
					   INT32 nDatasetTLX, INT32 nDatasetTLY,
					   INT32 nDatasetBRX, INT32 nDatasetBRY );

	/**
	 *	Rectilinear conversion from world coordinates to dataset coordinates.
	 *
	 *	@param[in]	dWorldX				The world X coordinate.
	 *	@param[in]	dWorldY				The world Y coordinate.
	 *	@param[out]	pnDatasetX			A buffer for the output dataset X coordinate.
	 *	@param[out]	pnDatasetY			A buffer for the output dataset Y coordinate.
	 */
	NCSError ConvertWorldToDataset(IEEE8 dWorldX, IEEE8 dWorldY, INT32 *pnDatasetX, INT32 *pnDatasetY);
	/**
	 *	Rectilinear conversion from dataset coordinates to world coordinates.
	 *
	 *	@param[in]	nDatasetX			The dataset X coordinate.
	 *	@param[in]	nDatasetY			The dataset Y coordinate.
	 *	@param[out]	pdWorldX			A buffer for the output world X coordinate.
	 *	@param[out]	pdWorldY			A buffer for the output world Y coordinate.
	 */
	NCSError ConvertDatasetToWorld(INT32 nDatasetX, INT32 nDatasetY, IEEE8 *pdWorldX, IEEE8 *pdWorldY);
	/**
	 *	Set a (void *) to a data structure containing any client data that must be accessed in the read callback.
	 *
	 *	@param[in]	pClientData			(void *) pointer to client data.
	 */
	void SetClientData(void *pClientData);
	/**	
	 *	Obtain any client data that has been established.
	 *
	 *	@return							(void *) pointer to client data.
	 */
	void *GetClientData();


	NCSError SetFileMetaData(const NCSFileMetaData* pFileMetaData);
	/**
	 * Set the client statistics on the file
	 * @param[in]	pStats			(void *) pointer to client stats.
	 */
	NCSError SetClientStatistics(const NCSFileStatistics* pStats);

	/**
	 * Get the client statistics on the file
	 * @param[in]	ppStats			(void *) pointer to client stats.
	 */
	NCSError GetClientStatistics(NCSFileStatistics** ppStats);

	/**
	 * Set the client statistics on the file
	 * @param[in]	pStats			(void *) pointer to client stats.
	 */
	NCSError SetRPCData(const NCSRPCData* pRPCData);

	/**
	 * Get the client statistics on the file
	 * @param[in]	ppStats			(void *) pointer to client stats.
	 */
	NCSError GetRPCData(NCSRPCData** ppRPCData);

	
	NCSError CheckFormatVersions(const std::string& sWhichData);

	/**
	 * Get all geotiff key entries on the file.
	 * @param[in, out]		ppGeoKeyIds 	a pointer to a geokeyentry array which is allocated and returned if the file contains any geokey data, or NULL if the file doesn't
	 * @param[in, out]		pCount 			a pointer to count of geokeyentry data
	 * @return      NCSError     NCS_SUCCESS, or error on failure
	 * @note		make sure *ppGeoKeyIds is initialised as NULL
	 */
	NCSError GetAllGeotiffKeyEntries(NCSGeoKeyEntry **ppGeoKeyIds, UINT32* pCount);

	/**
	 * Get individual key value frome the open fileview.
	 * @param[in]		key 			key id
	 * @param[in,out]	pVal 			a pointer to key value
	 * @param[in]		index			key index, usually is zero
	 * @param[in]	    count			value count. it is one if the key type is short or double and character count if the key type is ascii
	 * @return      NCSError     NCS_SUCCESS, or error on failure
	 */
	NCSError GetGeotiffKey(geokey_t key, void *pVal, int index,
                         int count);
	/** 
	 *	Set geotiff key for editing
	 *  @param[in]  keyID				The geotiff key. It should be in the definitions in geokeys.h
	 *  @param[in]  type				The type for the geotiff key.
	 *  @param[in]  count				count. Normally count is 1, one time to set one key. -1 for removal case.
	 *  @param[in]  varargs             key value(s).
	 *	@return					    NCSError value, NCS_SUCCESS or any applicable error code
	 */
	NCSError SetGeotiffKey(geokey_t keyID, tagtype_t type, int count, ...);

	NCSError SetGeotiffKeyInternal(geokey_t keyID, tagtype_t type, int count, va_list args);

	/** 
	 *	Set geotiff tag for editing
	 *  @param[in]  tag						The geotiff tag. It can only be GTIFF_PIXELSCALE, GTIFF_TIEPOINT,GTIFF_TRANSMATRIX.
	 *  @param[in]  count					value count that is stored in values.
	 *  @param[in]  values					value array. 
	 *	@return							NCSError value, NCS_SUCCESS or any applicable error code
	 */
	NCSError SetGeotiffTag(unsigned short tag, int count, IEEE8 *values);

	/**
	 * Get individual tag details frome the open fileview.
	 * @param[in]		tag 			The geotiff tag. It can only be GTIFF_PIXELSCALE, GTIFF_TIEPOINT,GTIFF_TRANSMATRIX.
	 * @param[in, out]       pCount			a pointer to value count returned by SDK.
	 * @param[in, out]		ppValue			a pointer to a value array which is allocated and return if the file has the values for the tag, otherwise NULL
	 * @return      NCSError     NCS_SUCCESS, or error on failure
	 * @note		make sure *ppValue is initialised as NULL
	 */
	NCSError GetGeotiffTag(unsigned short tag, int *pCount, IEEE8 **ppValue);


	NCSError EditFlushAll();

	/** 
	 *	Utility function.  Breaks down a URL string into protocol, hostname, and filename components.
	 *
	 *	@param[in]	pURLPath		The URL to be broken down and analyzed.
	 *	@param[out]	ppProtocol		A pointer to the protocol string resulting from the URL decomposition.
	 *	@param[out]	ppHost			A pointer to the hostname resulting from the URL decomposition.
	 *	@param[out]	ppFilename		A pointer to the filename resulting from the URL decomposition.
	 *	@return						BOOLEAN value, whether the input URL is a remote file.
	 */
	static BOOLEAN BreakdownURL(  char *pURLPath, 
							char **ppProtocol,
							char **ppHost, 
							char **ppFilename);
	/**	
	 *	Obtain meaningful error text from a returned error code.
	 *
	 *	@param[in]	nErrorNum		Error code
	 *	@return						(char *) value, an explanatory ASCII string for the error code
	 */
	static const char *FormatErrorText ( NCSError nErrorNum );

	virtual bool ReceivedBlock(NCSBlockId nBlock);

	/**
	*	Increment a specific type of time counter for the calling thread
	*
	*	@param[in]	type		compression time counter type
	*	@param[in]	timeLength	time increments
	*/
#ifndef NCS_MOBILE
	void IncrementThreadTimeCounters(TimeCounters::CompressionTimeCounterType type,
		const NCSTimeStampMs& timeLength)
	{
		if (m_pCompressionTimeCounters){
			m_pCompressionTimeCounters->IncrementThreadTimeCounters(type, timeLength);
		}
	}

	/**
	*	Summarize all different type of time counters for all threads. Call it when all counter is finished.
	*
	*/
	void SummarizeTimeCounters()
	{
		if (m_pCompressionTimeCounters){
			m_pCompressionTimeCounters->Summarize();
		}
	}

	/**
	*	Obtain the total time counters
	*
	*/
	const TimeCounters::CompressionTimeCounters& GetTotalTimeCounters();
#endif //NCS_MOBILE

	class CECWPCacheContent
	{
	public:
		typedef struct {
			NCS::CString m_sURL;
			INT64 m_nSize;
		} CEntry;
		std::list<CEntry> m_Entries;
	};
	static bool GetECWPCacheContent(CECWPCacheContent &Content);
	static bool PurgeECWPCache();
	static bool ReloadECWPCache();

	static NCS::CError PurgeUnusedMemory();

	

#ifndef SWIG
protected:
	CMutex m_Mutex;

	
private:
	void *m_pClientData;
#ifndef NCS_MOBILE
	TimeCounters *m_pCompressionTimeCounters;
#endif
#endif
};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // !NCSVIEW_H
