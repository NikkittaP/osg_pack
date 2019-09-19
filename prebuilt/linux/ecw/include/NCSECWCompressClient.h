/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	NCSECWCompessClient.h
** CREATED:	Thu 19/08/1999
** AUTHOR: 	Simon Cope
** PURPOSE:	Public Interface library to compress ECW v2.0 image files
** EDITS:
** [01] sjc 08Dec1999	Updated comments
**
********************************************************/

/** @file NCSECWCompressClient.h */

#ifndef NCSECWCOMPRESSCLIENT_H
#define NCSECWCOMPRESSCLIENT_H

#ifndef NCSECWCLIENT_H
#include "NCSECWClient.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif
#ifndef NCSERRORS_H
#include "NCSErrors.h"
#endif

#include "NCSEcw/GTIF/geokeys.h"
#include "NCSEcw/GTIF/geo_public_defs.h"

#define ERSWAVE_VERSION 3
#define ERSWAVE_VERSION_STRING	"3.0"		/* should be in sync with the above */
	
#ifdef NCS_V3_COMPAT_NAMES
#include "NCSECWCompatibility.h"
#endif // NCS_V3_COMPAT_NAMES

/** 
 *  @enum NCSCompressFormat
 *  @ingroup enums
 *	An enumerated type specifying the format of the compressed data. Currently greyscale, RGB
 *	and luminance-chrominance formats are supported.
 */
typedef enum {
	/** The compressed data is unformatted */
	NCS_COMPRESS_NONE		= NCSCS_NONE,
	/** Greyscale format, single band */
	NCS_COMPRESS_UINT8		= NCSCS_GREYSCALE,
	/** JPEG standard YUV digital format, three band */
	NCS_COMPRESS_YUV		= NCSCS_YUV,
	/** Multiband format */
	NCS_COMPRESS_MULTI		= NCSCS_MULTIBAND,
	/** RGB images (converted to COMPRESS_YUV internally) */
	NCS_COMPRESS_RGB		= NCSCS_sRGB			
} NCSCompressFormat;

typedef enum {
	NCS_COMPRESS_HINT_NONE	= 0,
	NCS_COMPRESS_HINT_FAST	= 1,
	NCS_COMPRESS_HINT_BEST	= 2,
	NCS_COMPRESS_HINT_INTERNET = 3
} NCSCompressHint;

/** @def Maximum length of a datum definition string */
#define ECW_MAX_DATUM_LEN		16
#ifndef ECW_MAX_PROJECTION_LEN
/** @def Maximum length of a projection definition string */
#define ECW_MAX_PROJECTION_LEN	16
#endif

#ifndef NCS_MAX_FILE_PATH_SIZE
#define NCS_MAX_FILE_PATH_SIZE 4096
#endif

/** @def X dimension of the default (and preferred) block size */
#define X_BLOCK_SIZE	64
/** @def Y dimension of the default (and preferred) block size */
#define	Y_BLOCK_SIZE	64

/** 
 *	@enum NCSStringType
 *	@ingroup enums
 *  Enumerated type for the type of string used.
 */
typedef enum {
	/** UTF-8 encoded char */
	NCS_UTF8_CHAR,
	/** Native wchar_t* */
	NCS_WCHAR
} NCSStringType;

/** 
 *	@struct NCSCompressClient
 *	@ingroup structures
 *	@brief The main structure used to create an ECW or JPEG 2000 compression client.
 *	
 *	This structure contains information used when compressing raster data 
 *	to an output ECW file.  Most of the values have sensible defaults so users
 *	need only customise those of significance to their application.  Certain 
 *	of these values must always be specified in the client code, namely 
 *	fTargetCompression, nInOutSizeX, nInOutSizeY, nInputBands, and pReadCallback.
 *
 *	Once compression is complete certain statistics are calculated and added 
 *	to this structure.  These include the time taken, the output file size in 
 *	bytes, the actual compression ratio achieved, and the MB/s throughput of the 
 *	process.
 */
typedef struct NCSCompressClient {
	/*
	** These fields are populated by the compression client
	*/
	/** If this is specified but the output file is not, a default output filename will be created.
	 *	Otherwise this field is unused.
	 *
	 * Union of either wchar_t or UTF-8 encoded char distinguished by eInputOutputFileNameType
	 */
	union {
		wchar_t	wszInputFileName[NCS_MAX_FILE_PATH_SIZE];
		char	szInputFileName[NCS_MAX_FILE_PATH_SIZE];
	} uInputFileName;
	
	/** An output filename must be specified if no input filename is specified 
	 *
	 * Union of either wchar_t or UTF-8 encoded char distinguished by eInputOutputFileNameType
	 */
	union {
		wchar_t	wszOutputFileName[NCS_MAX_FILE_PATH_SIZE];
		char	szOutputFileName[NCS_MAX_FILE_PATH_SIZE];
	} uOutputFileName;
	
	/** The value of eInputOutputFileNameType controls the input and output file unions above */
	NCSStringType eInputOutputFileNameType;
	
	/** The target compression ratio - must be specified */
	IEEE4				fTargetCompression;
	/**	The compression format to use.  See the related enumerated type definition */
	NCSCompressFormat	eCompressFormat;
	/** 
	 *  A guideline for an appropriate compression scheme to use.  This currently has 
	 *	no effect, though the default value is COMPRESS_HINT_INTERNET.  Reserved for
	 *	future use, see the related enumerated type definition 
	 */
	NCSCompressHint		eCompressHint;
	/** X dimension of the block size to use.  Can be 64, 128, 256, 512, 1024, or 2048.
	 *	The default for these is set to 64 which produces preferred performance over the internet.
	 */
	UINT32				nBlockSizeX;
	/** Y dimension of the block size to use.  Can be 64, 128, 256, 512, 1024, or 2048.
	 *	The default for these is set to 64 which produces preferred performance over the internet.
	 */
	UINT32				nBlockSizeY;
	/** Number of cells of input data and compressed file in the X direction - must be specified */
	UINT32				nInOutSizeX;
	/** Number of cells of input data and compressed file in the Y direction - must be specified */
	UINT32				nInOutSizeY;
	/** Number of bands in the input data - must be specified */
	UINT32				nInputBands;
	/** Number of bands in the output file - should not generally be specified */
	UINT32				nOutputBands;
	/** Size of the input file in bytes - should not be specified, it will be determined automatically. */
	UINT64				nInputSize;
	/** Optional field specifying the cell size in the X direction in eCellSizeUnits */
	IEEE8				fCellIncrementX;
	/** Optional field specifying the cell size in the Y direction in eCellSizeUnits */
	IEEE8				fCellIncrementY;
	/** Optional field specifying the X world origin of the input data in eCellSizeUnits */
	IEEE8				fOriginX;
	/** Optional field specifying the Y world origin of the input data in eCellSizeUnits */
	IEEE8				fOriginY;
	/** Optional field specifying the units in which world cell sizes are specified, e.g. meters, feet */
	NCSCellSizeUnitType		eCellSizeUnits;
	/** ER Mapper GDT style datum string */
	char				szDatum[ECW_MAX_DATUM_LEN];
	/** ER Mapper GDT style projection string */
	char				szProjection[ECW_MAX_PROJECTION_LEN];
	/** What version format, 0 for jp2 file, or the correct version number for ecw file*/ 
	UINT8				nFormatVersion;
	/** Callback function used to obtain lines of band data from the input data - must be specified */
	BOOLEAN				(*pReadCallback) (struct NCSCompressClient *pClient,
									  UINT32 nNextLine,
									  IEEE4 **ppInputArray);
	/** Optional status callback function to track the progress of the compression process */
	void				(*pStatusCallback) (struct NCSCompressClient *pClient,
										UINT32 nCurrentLine);				
	/** Optional cancel callback function which can be used to cancel a compression process */
	BOOLEAN				(*pCancelCallback) (struct NCSCompressClient *pClient);
	/** (void *) Pointer to any private data you need to access in the three callback functions */
	void				*pClientData;
	/** Created by NCSEcwCompressOpen() */
	struct				EcwCompressionTask *pTask;

	/** The remaining fields are populated by NCSCompressClose() */
	
	/** Actual compression rate achieved - ratio of input data size to output file size */
	IEEE4				fActualCompression;
	/** Time taken to perform the complete compression, in seconds */
	IEEE8				fCompressionSeconds;
	/** MB/s throughput during the compression process */
	IEEE8				fCompressionMBSec;
	/** Total size of the output file in bytes */
	UINT64				nOutputSize;
	/** Optional cell bits for MULTIBAND, RGB, YUV and GREYSCALE, currently support 8 (default) and 16 bits */
	UINT8				nCellBitDepth;
	/** Information describing the file meta (ECW v3). */
	NCSFileMetaData*	pFileMetaData;
	/** A pointer to an array of band information structures for each band of the image */
	NCSFileBandInfo*    pBands;
} NCSCompressClient;

/** 
 * @ingroup functions
 * @brief	Allocate a new CompressionClient structure and fill in defaults set string type to char.
 * @return	A pointer to memory allocated to an NCSCompressClient structure
 */
NCSCompressClient * NCS_CALL NCSCompressAllocClientA(void);

/** 
 * @ingroup functions
 * @brief	Allocate a new CompressionClient structure and fill in defaults set string type to wchar
 * @return	A pointer to memory allocated to an NCSCompressClient structure
 */
NCSCompressClient * NCS_CALL NCSCompressAllocClientW(void);

#ifdef UNICODE

#define NCSCompressAllocClient NCSCompressAllocClientW

#else

#define NCSCompressAllocClient NCSCompressAllocClientA

#endif // UNICODE

/**	
 *	@ingroup functions
 *	@brief	Having set compression parameters, initialise the compression process.
 *	@param	pInfo	The NCSCompressClient structure containing the compression parameters 
 *	@param	bCalculateSizesOnly	Which output statistics to calculate 
 *	@return	NCSError value, NCS_SUCCESS or any applicable error code
 */
NCSError NCS_CALL NCSCompressOpen(NCSCompressClient *pInfo, BOOLEAN bCalculateSizesOnly);

/**	
 * @ingroup functions
 * @brief	Start the compression process based on these parameters
 * @param	pInfo	The NCSCompressClient structure for this compression process
 * @return	NCSError value, NCS_SUCCESS or any applicable error code
 */
NCSError NCS_CALL NCSCompress(NCSCompressClient *pInfo);

/**	
 * @ingroup functions
 * @brief Start the compression process based on these parameters
 * @param		pInfo				The NCSCompressClient structure for this compression process
 * @param		pClientStats        The NCSFileStatistics structure for this compression process
 * @return		NCSError value, NCS_SUCCESS or any applicable error code
 */
NCSError NCS_CALL NCSCompressSetStatistics(NCSCompressClient *pInfo, const NCSFileStatistics* pClientStats);

/**	
 * @ingroup functions
 * @brief Start the compression process based on these parameters
 * @param	pInfo		The NCSCompressClient structure for this compression process
 * @param	pRPCData	The NCSRPCData structure for this compression process
 * @return	NCSError value, NCS_SUCCESS or any applicable error code
 */
NCSError NCS_CALL NCSCompressSetRPCData(NCSCompressClient *pInfo, const NCSRPCData* pRPCData);

/** 
 *  @ingroup functions
 *	@brief Set geotiff key for compression. This is a var_args function, the variable arguments are the Geotiff keys to get.
 *	@param	pInfo				The NCSCompressClient structure for this compression process
 *  @param	keyID				The geotiff key. It should be in the definitions in geokeys.h
 *  @param	type				The type for the geotiff key.
 *  @param	count				Key count. Normally count is 1, one time to set one key, -1 for removal case.
 *  @param	...					The keys to set.
 *	@return						NCSError value, NCS_SUCCESS or any applicable error code
 */
NCSError NCSCompressSetGeotiffKey(NCSCompressClient *pInfo, geokey_t keyID, tagtype_t type, int count, ...);

/** 
 *	@ingroup functions
 *	@brief	Set geotiff tag for compression
 *	@param	pInfo			The NCSCompressClient structure for this compression process
 *  @param	tag				The geotiff tag. It can only be GTIFF_PIXELSCALE, GTIFF_TIEPOINT,GTIFF_TRANSMATRIX.
 *  @param	count			value count that is stored in values.
 *  @param	values			value array. 
 *	@return					NCSError value, NCS_SUCCESS or any applicable error code
 */
NCSError NCSCompressSetGeotiffTag(NCSCompressClient *pInfo, unsigned short tag, int count, IEEE8 *values);

/** 
 *	@ingroup functions
 *	@brief	Having completed compression, close, calculate output statistics, and clean up
 *	@param	pInfo				The NCSCompressClient structure for this compression process
 *	@return						NCSError value, NCS_SUCCESS or any applicable error code
 */
NCSError NCS_CALL NCSCompressClose(NCSCompressClient *pInfo);

/**	
 *	@ingroup functions
 *	@brief Free the memory allocated to the NCSCompressClient structure
 *	@param	pInfo				The structure to free
 *	@return						NCSError value, NCS_SUCCESS or any applicable error code
 */
NCSError NCS_CALL NCSCompressFreeClient(NCSCompressClient *pInfo);

/** 
 *	@ingroup functions
 *	@brief Call this function to specify your organisation's unique OEM compression key
 *	@param	szCompanyName		Your organization's company name.
 *	@param	szKey				Your organisation's unique key.
 *	@return						NCSError value, NCS_SUCCESS or any applicable error code.
 */
void NCS_CALL NCSCompressSetOEMKey(const char *szCompanyName, const char *szKey);

#ifdef __cplusplus
}
#endif

#endif /* NCSECWCOMPRESSCLIENT_H */
