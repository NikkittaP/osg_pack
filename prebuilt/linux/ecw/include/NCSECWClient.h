/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	NCSECWClient.h
** CREATED:	08 May 1999
** PURPOSE:	Public client interface to ECW JP2 image decompression library
**
********************************************************/

/** @file NCSECWClient.h */

/**
 * @mainpage Overview
 *
 * @section intro_sec Introduction
 *
 * Welcome to the ERDAS ECW JPEG 2000 SDK. This SDK is aimed at developers who wish to compress 
 * or view extremely large (terabyte) sized images, typically for the GIS industry, into the ECW or 
 * ISO JPEG 2000 format. For more generic information on the file formats and the functionality of
 * the SDK, consult the "ECW JPEG 2000 User Guide" (PDF) installed with this software.
 * 
 * The SDK offers both a "C" and a "C++" interface into the encoder and decoder. Enumerations 
 * and data structures are shared between the two interfaces. Some functionality is only 
 * available in the C++ interface.
 * 
 * There are also several example applications that demonstrate usage of both APIs to accomplish the
 * same task.
 *
 * @section apidef Library API definitions
 *
 * @subsection definitions	Macros
 * Jump to the <a href="group__macros.html">macro</a> documentation for the C/C++ API.
 * @subsection enumerations Enumerations
 * Jump to the <a href="group__enums.html">enum</a> documentation for the C/C++ API.
 * @subsection structures	Structures
 * Jump to the <a href="group__structures.html">structure</a> documentation for the C/C++ API.
 * @subsection functionsC "C" Interface
 * Jump to the <a href="group__functions.html">function</a> documentation for the C API.
 *  @subsection functionsCpp "C++" Interface
 * Jump to the <a href="group__classes.html">class</a> documentation for the C++ API.
 *  @subsection functionsSimplifiedCpp Simplified "C++" Interface
 * Jump to the <a href="group__simplifiedclasses.html">class</a> documentation for the Simplified C++ API.
 *  @subsection functionsObjCAPI Objective-C Interface
 * Jump to the <a href="group__objcclasses.html">class</a> documentation for the Objective-C API.
 * 
 * @section examples Examples
 * Jump to the <a href="_examples.html">examples</a> documentation.
 */

/** 
 *	@defgroup structures Structures
 *	@brief These are the structures used when accesing files using the SDK to read, write, get and set image data or meta data from ECW or JPEG 2000 files.
 */

/** 
 *	@defgroup macros Macros
 *	@brief These are the macros and definitions used when accesing files using the SDK to read, write, get and set image data or meta data from ECW or JPEG 2000 files.
 */

/** 
 *	@defgroup enums Enumerations
 *	@brief These are the enumerations used when accesing files using the SDK to read, write, get and set image data or meta data from ECW or JPEG 2000 files.
 */

/** 
 *	@defgroup functions C API Functions
 *	@brief These are the gloabl functions used via "C" to encode, deocde, get and set image data or meta data from ECW or JPEG 2000 files.
 */

/** 
 *	@defgroup classes C++ API Classes
 *	@brief The main entry point for encoding and decoding images in the C++ API is the NCS::CView class.
 */

/** 
 *	@defgroup simplifiedclasses Simplified C++ API Classes
 *	@brief Simplified classes for decoding ECW/JP2 imagery with an API that will be consistent between releases.
 */

/**
*	@defgroup objcclasses Objective-C API Classes
*	@brief Objective-C API classes for decoding ECW/JP2 imagery with an API that will be consistent between releases.
*/
 
#ifndef NCSECWCLIENT_H
#define NCSECWCLIENT_H

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif
#ifndef NCSERRORS_H
#include "NCSErrors.h"
#endif

#include "NCSEcw/GTIF/geo_public_defs.h"
#include "NCSEcw/GTIF/geokeys.h"
#include "API/CellType.h"
#include "API/FileColorSpace.h"
#include "API/CellSizeUnitType.h"
#include "API/ResampleMethod.h"

/**
 * @def NCS_V3_COMPAT_NAMES
 * @ingroup macros
 * @brief Include the version 3 compatibility definitions for older code.
 */
#define NCS_V3_COMPAT_NAMES
#ifdef NCS_V3_COMPAT_NAMES
#include "NCSECWCompatibility.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ERS_WAVELET_DATASET_EXT
/**
 *	@def ERS_WAVELET_DATASET_EXT
 *	@ingroup macros
 *	@brief Allowed file extensions for ECW files.
 */
#define ERS_WAVELET_DATASET_EXT	".ecw"
#define ERS_WAVELET_DATASET_EXT_T	NCS_T(".ecw")
#endif

#ifndef ERS_JP2_DATASET_EXTS 
/**	
 *	@def ERS_JP2_DATASET_EXTS
 *	@ingroup macros
 *	@brief Allowed file extensions for JPEG 2000 files.
 */
#define ERS_JP2_DATASET_EXTS	{ ".jp2" , ".j2k" , ".j2c" , ".jpc", ".jpx", ".jpf" } /*[12]*/
#define ERS_JP2_DATASET_EXTS_T	{ NCS_T(".jp2"), NCS_T(".j2k"), NCS_T(".j2c"), NCS_T(".jpc"), NCS_T(".jpx"), NCS_T(".jpf") } /*[12]*/
#endif

#ifndef ERS_JP2_WORLD_FILE_EXTS
/**	
 *	@def ERS_JP2_WORLD_FILE_EXTS
 *	@ingroup macros
 *	@brief World filename extension definitions.
 */
#define ERS_JP2_WORLD_FILE_EXTS	{ ".j2w" , ".jxw" , ".jfw" }  
#endif

#ifndef ERSWAVE_VERSION_STRING
/**	
 *	@def ERSWAVE_VERSION_STRING
 *	@ingroup macros
 *	@brief Current version of the ECW format
 */
#define ERSWAVE_VERSION_STRING	"3.0"
#endif

#ifndef NCS_ECW_PROXY_KEY
#define NCS_ECW_PROXY_KEY		"ECWP Proxy"
#define NCS_ECW_PROXY_KEY_T		NCS_T("ECWP Proxy")
#endif

/** 
 *	@enum NCSReadStatus
 *	@ingroup enums
 *  @brief Enumerated type for the return status from read line routines.
 *	The application should treat CANCELLED operations as non-fatal,
 *	in that they most likely mean this view read was cancelled for
 *	performance reasons.
 */
typedef enum {
	/** Successful read */
	NCS_READ_OK			= 0,
	/** Read failed due to an error */
	NCS_READ_FAILED		= 1,
	/** Read was cancelled, either because a new SetView arrived or a 
	    library shutdown is in progress */
	NCS_READ_CANCELLED	= 2	
} NCSReadStatus;

#include "API/ProjectionFormat.h"
#include "API/FileType.h"

/** 
 *	@enum NCSConfigType
 *	@ingroup enums
 *	@brief Enumerated type for all the possible argument types for the SDK configuration function NCSEcwSetConfig
 */
typedef enum {
	/** BOOLEAN value, enable texturing when decoding ECW images, default is true. */
	NCSCFG_TEXTURE_DITHER			= 0,
	/** BOOLEAN value, whether to reopen file for each view opened. Default is false. */
	NCSCFG_FORCE_FILE_REOPEN		= 1,
	/** UINT32 value, target maximum memory cache size. Limited to 2GB on 32 bit platforms. This is deprecated in favour of NCSCFG_CACHE_MAXMEM_64. */
	NCSCFG_CACHE_MAXMEM				= 2,
	/** UINT32 value, target maximum number of files to have open to use for cache. */
	NCSCFG_CACHE_MAXOPEN			= 3,
	/** NCSTimeStampMs value, time an ECWP stream blocking read will wait before returning, default is 10000 ms. */
	NCSCFG_BLOCKING_TIME_MS			= 4,
	/** NCSTimeStampMs value, time delay between blocks arriving and the next refresh callback, default is 500 ms. */
	NCSCFG_REFRESH_TIME_MS			= 5,
	/** NCSTimeStampMs value, minimum time delay between last cache purge and the next one, default is 1000 ms. */
	NCSCFG_PURGE_DELAY_MS			= 6,
	/** NCSTimeStampMs value, time delay between last view closing and file being purged from cache, default is 1800000 ms. */
	NCSCFG_FILE_PURGE_DELAY_MS		= 7,
	/** NCSTimeStampMs value, minimum time delay between last view closing and file being purged from cache, default is 30000 ms. */
	NCSCFG_MIN_FILE_PURGE_DELAY_MS	= 8,
	/** CHAR* value, "server" name of ECWP proxy server. This feature is deprecated, do not use. */
	NCSCFG_ECWP_PROXY				= 9,
	/** BOOLEAN value, whether to force a low-memory compression. This feature is deprecated, do not use. */
	NCSCFG_FORCE_LOWMEM_COMPRESS	= 10,
	/** BOOLEAN value, whether to try to reconnect if ECWP connection is lost. Default is true. */
	NCSCFG_TRY_ECWP_RECONNECT		= 11,
	/** BOOLEAN value, whether to manage ICC profiles on file read. */
	NCSCFG_MANAGE_ICC			= 12,
	/** UINT32 value, number of cache bytes to use for JP2 file I/O caching - default 2^15 (32768). */
	NCSCFG_JP2_FILEIO_CACHE_MAXMEM	= 13,
	/** UINT32 value, maximum size for progressive file views. Default is 4000. */
	NCSCFG_MAX_PROGRESSIVE_VIEW_SIZE = 14,
	/** UINT64 value, target maximum memory cache size. Setting a large cache may impact performance negatively. Default is 25% of system memory. */
	NCSCFG_CACHE_MAXMEM_64			 = 15,
	/** UINT32 value, maximum number of concurrent file reads per file. */
	NCSCFG_FILE_MAXREADS			= 16,
	/** BOOLEAN value, enable the local ECWP client cache. Compressed blocks will be stored on disk in a local cache, and persist between sessions. Default is false. */
	NCSCFG_ECWP_CACHE_ENABLED		= 17,
	/** INT32 value, maximum size in MB on disk of local ECWP client cache. */
	NCSCFG_ECWP_CACHE_SIZE_MB		= 18,
	/** CHAR* value, location of local ECWP client cache directory. Default is the value of the TEMP environment variable. */
	NCSCFG_ECWP_CACHE_LOCATION		= 19,
	/** INT64 value, get the current size on disk of the local ECWP client cache. */
	NCSCFG_ECWP_CACHE_CURRENT_SIZE	= 20,
	/** BOOLEAN value, enable auto generation of .j2i index files on open. This is deprecated and has no effect, do not use. */
	NCSCFG_JP2_AUTOGEN_J2I			= 21,
	/** BOOLEAN value, enable optimization for non progressive use, progressive mode will no longer work. Only enable if blocking reads are used exclusively. */
	NCSCFG_OPTIMIZE_NON_PROGRESSIVE	= 22,
	/** BOOLEAN value, enable optimization of the SDK for random reading of files when purging the caches. */
	NCSCFG_OPTIMIZE_CACHE_PURGE_RANDOM_MODE = 23,
	/** BOOLEAN value, enable speed optimization using nearest neighbour interpolation instead of bilinear. Nearest neighbour is faster, but image quality is less. Default is true. */
	NCSCFG_OPTIMIZE_USE_NEAREST_NEIGHBOUR = 24,
	/** BOOLEAN value, force to use buffered I/O stream for JP2 file reading. */
	NCSCFG_USE_BUFFERED_IO_STREAM = 25,
	/** BOOLEAN value, force the reader to scale to the output buffer size */
	NCSCFG_USE_AUTO_SCALE_DOWN = 26,
	/** UINT32 value, number of streams in the pool, assists multi-threaded reading. */
	NCSCFG_JP2_MAX_FILE_STREAMS = 27,
	/** BOOLEAN value, enable resilient decoding, where the SDK tries to continue decoding even if the file contains an error (visible artefacts may be present in the decoded image). Set to false for fussy decoding, where the SDK will fail when it finds invalid information in the file. Default is true. */
	NCSCFG_RESILIENT_DECODING = 28,
	/** BOOLEAN value, enable the ECWP Disk cache for connections where a username and password are supplied, or the connection uses SSL (ECWPS). Default is false. */
	NCSCFG_ECWP_CACHE_ENABLE_SECURE = 29,
	/** BOOLEAN value, enable attempting to connect to server with ECWP version 3 protocol. Default is true. */
	NCSCFG_ECWP3_CLIENT = 30,
	/** UINT32 value, number of concurrent download sessions. */
	NCSCFG_ECWP3_CLIENT_MAX_CONCURRENT_DOWNLOADS = 31,
	/** UINT32 value, max number of blocks to request per download session. */
	NCSCFG_ECWP3_CLIENT_MAX_BLOCKS_PER_DOWNLOAD = 32,
	/** UINT32 value, min number of blocks to request per download session. */
	NCSCFG_ECWP3_CLIENT_MIN_BLOCKS_PER_DOWNLOAD = 33,
	/** UINT32 value, timeout in ms before a download session is closed. */
	NCSCFG_ECWP3_CLIENT_CONNECTION_TIMEOUT_MS = 34,
	/** CHAR* value, utf8 encoded path for temporary files when compressing. On "get", the result should be freed with NCSFree. */
	NCSCFG_COMPRESSION_TEMP_PATH_UTF8 = 35,
	/** wchar_t* value, get path for temporary files when compressing. On "get", the result should be freed with NCSFree. */
	NCSCFG_COMPRESSION_TEMP_PATH_WCHART = 36,
	/** UINT32 value, number of threads the tile based compressor will use. */
	NCSCFG_COMPRESSION_TILE_THREADS = 37,
	/** UINT64 value, bytes of memory the compressor can use.  Set to 0 to automatically calculate. */
	NCSCFG_COMPRESSION_MAXMEM = 38,
	/** BOOLEAN value, use CPU intrinsics (SSE, AVX, NEON) when compressing ECW files using the tile based API. This feature is deprecated, use NCSCFG_SUPPRESS_CPU_ENHANCEMENT instead. */
	NCSCFG_COMRESSION_USE_SSE = 39,
	/** UINT8 value, the format of returned projection information of ECW and JP2 files. See NCSProjectionFormat for return values. Default is NCS_PROJECTION_EPSG_FORMAT. */
	NCSCFG_PROJECTION_FORMAT = 40,
	/** BOOLEAN value, serialise the blocks for v3 ECW files compressed with the tiled compressor (makes decoding on slow IO faster). */
	NCSCFG_COMPRESSION_TILED_SERIALISE_BLOCKS = 41,
	/** BOOLEAN value, when ITransforms*'s are set in a NCSFile set this to FALSE to make the SDK not delete them (default TRUE). */
	NCSCFG_VIEW_ALWAYS_DELETE_TRANSFORMS = 42,
	/** (UTF-8 char *) value, the user agent that will be presented to web server.  On "get", the result should be freed with NCSFree. */
	NCSCFG_ECWP_CLIENT_HTTP_USER_AGENT = 43,
	/** BOOLEAN value, when a view is within 20 % range above its lower resolution, read up to that resolution, otherwise up to higher resolution if available. */
	NCSCFG_DECOMPRESSION_APPLY_LOWER_RES = 44,
	/** BOOLEAN value, set to TRUE to pre-fill the entire update region for ECW region update, FALSE to initialize the entire update region with the background color. */
	NCSCFG_ECW_PREFILL_UPDATE_REGION = 45,
	/** (UTF-8 char *) value, comma separated list of virtual directory paths to use in ECWP connections.  Default is "/erdas-iws/,/ecwp/". On "get", the result should be freed with NCSFree. */
	NCSCFG_ECWP_VIRTUAL_DIR_PATHS_UTF8 = 46,
	/** UINT32 value, number of read line operations before the SDK will check if the purger needs to be signalled. Default is 4096. */
	NCSCFG_MAX_READS_BEFORE_CACHE_SIZE_CHECK = 47, 
	/** UINT32 value, number of heaps from which memory will be allocated for encoding, zero to use process default heap. Heap for the next memory allocation will be selected in round robin manner. */
	NCSCFG_HEAPS_ENCODING = 48, 
	/** UINT32 value, number of heaps from which memory will be allocated for decoding, zero to use process default heap. Heap for the next memory allocation will be selected in round robin manner. */
	NCSCFG_HEAPS_DECODING = 49, 
	/** UINT32 value, suppress CPU enhancement.  See NCSSystemInfo.h for bitmask usage. Must be called before opening datasets. Default is false. */
	NCSCFG_SUPPRESS_CPU_ENHANCEMENT = 50
} NCSConfigType;

#define NCSCFG_JP2_MANAGE_ICC NCSCFG_MANAGE_ICC

/**
 *	@enum NCSGeodataUsage
 *	@ingroup enums
 *	@brief Enumerated type to describe the usage and precedence of geographic metadata in conjunction with JPEG 2000.
 */
typedef enum {
		/** Do not use geodata */
		JP2_GEODATA_USE_NONE				= 0,		//000000000
		/** Use GeoTIFF UUID box metadata only */
		JP2_GEODATA_USE_PCS_ONLY			= 1,		//000000001
		/** Use GML header box metadata only */
		JP2_GEODATA_USE_GML_ONLY			= 2,		//000000010
		/** Use world files only */
		JP2_GEODATA_USE_WLD_ONLY			= 4,		//000000100
		/** Use the GML box then the GeoTIFF box */
		JP2_GEODATA_USE_GML_PCS				= 10,		//000001010
		/** Use the world file then the GeoTIFF box */
		JP2_GEODATA_USE_WLD_PCS				= 12,		//000001100
		/** Use the GeoTIFF box then the GML box */
		JP2_GEODATA_USE_PCS_GML				= 17,		//000010001
		/** Use the world file then the GML box */
		JP2_GEODATA_USE_WLD_GML				= 20,		//000010100
		/** Use the GeoTIFF box then the world file */
		JP2_GEODATA_USE_PCS_WLD				= 33,		//000100001
		/** Use the GML box then the world file */
		JP2_GEODATA_USE_GML_WLD				= 34,		//000100010
		/** Use the world file, then the GML box, then the GeoTIFF box */
		JP2_GEODATA_USE_WLD_GML_PCS			= 84,		//001010100
		/** Use the GML box, then the world file, then the GeoTIFF box */
		JP2_GEODATA_USE_GML_WLD_PCS			= 98,		//001100010
		/** Use the world file, then the GeoTIFF box, then the GML box */
		JP2_GEODATA_USE_WLD_PCS_GML			= 140,		//010001100
		/** Use the GeoTIFF box, then the world file, then the GML box */
		JP2_GEODATA_USE_PCS_WLD_GML			= 161,		//010100001
		/** Use the GML box, then the GeoTIFF box, then the world file */
		JP2_GEODATA_USE_GML_PCS_WLD			= 266,		//100001010
		/** Use the GeoTIFF box, then the GML box, then the world file */
		JP2_GEODATA_USE_PCS_GML_WLD			= 273		//100010001
} NCSGeodataUsage;

/** 
 *	@struct NCSFileBandInfo
 *	@ingroup structures
 *	@brief Struct containing metadata for a specific band in the file.
 */
typedef struct NCSFileBandInfo{
	/** Bit depth used in band, including sign bit */
	UINT8	nBits;
	/** Whether band data is signed */
	BOOLEAN	bSigned;
	/** ASCII description of band, e.g. "Red" or "Band1" */
	char	*szDesc;
} NCSFileBandInfo;

/** 
 *	@addtogroup macros
 *	@brief These are the allowable ASCII descriptions for the type of a raster band.
 *  @{
 */
/** Red */
#define NCS_BANDDESC_Red							"Red"
/** Green */
#define NCS_BANDDESC_Green							"Green"
/** Blue */
#define NCS_BANDDESC_Blue							"Blue"
/** All */
#define NCS_BANDDESC_All							"All"
/** Red opacity channel */
#define NCS_BANDDESC_RedOpacity						"RedOpacity"
/** Green opacity channel */
#define NCS_BANDDESC_GreenOpacity					"GreenOpacity"
/** Blue opacity channel */
#define NCS_BANDDESC_BlueOpacity					"BlueOpacity"
/** All opacity channel (for every band) */
#define NCS_BANDDESC_AllOpacity						"AllOpacity"
/** Red opacity premultiplied*/
#define NCS_BANDDESC_RedOpacityPremultiplied		"RedOpacityPremultiplied"
/** Green opacity premultiplied*/
#define NCS_BANDDESC_GreenOpacityPremultiplied		"GreenOpacityPremultiplied"
/** Blue opacity premultiplied*/
#define NCS_BANDDESC_BlueOpacityPremultiplied		"BlueOpacityPremultiplied"
/** All opacity premultiplied*/
#define NCS_BANDDESC_AllOpacityPremultiplied		"AllOpacityPremultiplied"
/** Greyscale*/
#define NCS_BANDDESC_Greyscale						"Grayscale"
/** Greyscale opacity*/
#define NCS_BANDDESC_GreyscaleOpacity				"GrayscaleOpacity"
/** Greyscale opacity premultiplied*/
#define NCS_BANDDESC_GreyscaleOpacityPremultiplied	"GrayscaleOpacityPremultiplied"
/** Band numbner*/
#define NCS_BANDDESC_Band							"Band #%d"
/*@}*/

/**
 * @struct NCSFileMetaData
 * @ingroup structures
 * @brief Extended FileMetadata structure, only for ecw version 3 and above.
 */
typedef struct NCSFileMetaData
{
	/** image classificatioin */	
	NCSTChar* sClassification;
	/** Acquistion date on the image, the format should be YYYY-MM-DD */	
	NCSTChar* sAcquisitionDate;
	/** Acquisition Sensor Name */	
	NCSTChar* sAcquisitionSensorName;
	/** Software that is used to compress the image */
	NCSTChar* sCompressionSoftware; 
	/** Author who compress the image */
	NCSTChar* sAuthor; // updatable
	/** Copyright on the compressed image */
	NCSTChar* sCopyright; 
	/** Company which owns the compressed image */
	NCSTChar* sCompany; // updatable
	/** Contact email */
	NCSTChar* sEmail; // updatable
	/** Contact address */
	NCSTChar* sAddress;// updatable
	/** Contact phone */
	NCSTChar* sTelephone; // updatable
} NCSFileMetaData;

/**
 * @struct NCSRPCData
 * @ingroup structures
 * @brief Rapid Positioning Capability data, only for ECW version 3 files and higher.
 */
typedef struct NCSRPCData
{
	/** Error - Bias, should be >= 0 */
	IEEE8 ERR_BIAS;
	/** Error - Random, should be >= 0 */
	IEEE8 ERR_RAND;
	/** 	Line Offset, should be >= 0 */
	IEEE8 LINE_OFF;
	/** 	Sample Offset, should be >= 0 */
	IEEE8 SAMP_OFF;
	/** 	Geodetic Latitude Offset, range from -90 to +90 */
	IEEE8 LAT_OFF;
	/**  Geodetic Longitude Offset , range from -180 to +180 */
	IEEE8 LONG_OFF;
	/** 	Geodetic Height Offset */
	IEEE8 HEIGHT_OFF;
	/** 	Line Scale, should be > 0 */
	IEEE8 LINE_SCALE;
	/** Sample Scale, should be > 0 */
	IEEE8 SAMP_SCALE;
	/** Geodetic Latitude Scale, range is (0, 90] */
	IEEE8 LAT_SCALE;
	/** 	Geodetic Longitude Scale, range is (0, 180] */
	IEEE8 LONG_SCALE;
	/** Geodetic Height Scale, should be > 0 */
	IEEE8 HEIGHT_SCALE;
	/** Line Numerator Coefficients */
	IEEE8 LINE_NUM_COEFFS[20];
	/** Line Denominator Coefficients */
	IEEE8 LINE_DEN_COEFFS[20];
	/** Sample Numerator Coefficients */
	IEEE8 SAMP_NUM_COEFFS[20];
	/** Sample Denominator Coefficients */
	IEEE8 SAMP_DEN_COEFFS[20];
} NCSRPCData;

/** 
 *	@struct NCSFileInfo
 *	@ingroup structures
 *	@brief File info structure for ECW and JPEG 2000 files.
 *	The SDK function NCSGetViewFileInfo() returns a pointer to this	file info structure for a given view.
 */
typedef struct NCSFileInfo{
	/** Dataset cells in X direction */	
	UINT32	nSizeX;
	/** Dataset cells in X direction */
	UINT32	nSizeY;
	/** Number of bands in the file, e.g. 3 for a RGB file */
	UINT16	nBands;	
	/** Target compression rate, e,g, 20 == 20:1 compression.  May be zero */
	UINT16	nCompressionRate;
	/** Units used for pixel size */
    NCSCellSizeUnitType	eCellSizeUnits;
	/** Increment in eCellSizeUnits in X direction.  May be negative.  Never zero */
	IEEE8	fCellIncrementX;
	/** Increment in eCellSizeUnits in Y direction.  May be negative.  Never zero */
	IEEE8	fCellIncrementY;
	/** World X origin for top left corner of top left cell, in eCellSizeUnits */
	IEEE8	fOriginX;
	/** World Y origin for top left corner of top left cell, in eCellSizeUnits */
	IEEE8	fOriginY;
	/** Datum name string, can be in different format according to setting on NCSProjectionFormat. Never NULL */
	char	*szDatum;	
	/** Projection name string, can be in different format according to setting on NCSProjectionFormat. Never NULL */
	char	*szProjection;
	/** Clockwise rotation of image in degrees */
	IEEE8	fCWRotationDegrees;
	/** Color space of image */
	NCSFileColorSpace eColorSpace;
	/** Cell type of image samples */
    NCSCellType eCellType;
	/** A pointer to an array of band information structures for each band of the image */
	NCSFileBandInfo *pBands;
	/** What version on the jp2 or ecw file */ 
	UINT8	nFormatVersion;
	/** Optional cell bits for MULTIBAND, RGB, YUV and GREYSCALE, currently support 8 (default) and 16 bits (progressing) */
	UINT8	nCellBitDepth;
	/** Compression date, this field will be automatically filled after compression */	
	char* sCompressionDate;
	/** Actual copression rate, this field will be automatically filled after compression  */
	IEEE4	fActualCompressionRate;
	/** FileMetaData */
	NCSFileMetaData* pFileMetaData;
} NCSFileInfo;

/** 
 *	@struct NCSSetViewInfo
 *	@ingroup structures
 *	@brief Information about an open view into an ECW or JPEG 2000 image file.
 *	This structure contains updated information about the extents and processing status
 *	of an open file view.  NCSGetSetViewInfo() will return a pointer to this structure
 *	for an open file view.
 */
typedef struct NCSSetViewInfo{
	/** Client data */
	void	*pClientData;
	/** Number of bands to read */
	UINT32 nBands;				
	/** Array of band indices being read from the file - the size of this array is nBands */
	UINT32 *pBandList;
	/** Top left of the view in image coordinates */
	UINT32 nTopX, nLeftY;
	/** Bottom right of the view in image coordinates */
	UINT32 nBottomX, nRightY;
	/** Size of the view in pixels */
	UINT32 nSizeX, nSizeY;			
	/** Number of file blocks within the view area */
	UINT32 nBlocksInView;
	/** Number of these file blocks that are currently available */
	UINT32 nBlocksAvailable;
	/** Blocks of the file that were available at the time of the corresponding SetView */
	UINT32 nBlocksAvailableAtSetView;
	/** Number of blocks that were missed during the read of this view */
	UINT32 nMissedBlocksDuringRead;
	/** Top left of the view in world coordinates (if using SetViewEx) */
	IEEE8  fTopX, fLeftY;
	/** Bottom right of the view in world coordinates (if using SetViewEx) */ /*[02]*/
	IEEE8  fBottomX, fRightY;	
} NCSSetViewInfo;

/** 
 *	@struct NCSBandStats
 *	@ingroup structures
 *	@brief Statistics structure for a single band of raster data.
 */
typedef struct NCSBandStats{
		/** mode */
		IEEE4 fMode;
		/** minimum value on the band */
		IEEE4 fMinVal;
		/** max value on the band */
		IEEE4 fMaxVal;
		/** mean value on the band */
		IEEE4 fMeanVal;
		/** median value on the band */
		IEEE4 fMedianVal;
		/** standard deviation on the band */
		IEEE4 fStandardDev;
		/** histogram min value */
		IEEE4 fMinHist;
		/** histogram max value */
		IEEE4 fMaxHist;
		/** histogram bucket count */
		UINT32 nHistBucketCount;
		/** histogram on the band */
		UINT64* Histogram;
} NCSBandStats;

/** 
 *	@struct NCSFileStatistics
 *	@ingroup structures
 *	@brief Structure containin the statistics for an for ECW version 3 or higher file. A file may not necessarily contain statistics.
 */
typedef struct NCSFileStatistics
{
	/** The bands statistics. */
	NCSBandStats* BandsStats;
	/** The number of bands. */
	UINT32 nNumberOfBands;
} NCSFileStatistics;

/** 
 *	@struct NCSGeoKeyEntry
 *	@ingroup structures
 *	@brief Structure containing the Geokey entries for an ECW version 3 or higher file georeferencing information.
 */
typedef struct NCSGeoKeyEntry
{
	/** Identifier for the key */
	geokey_t keyId;
	/** Type of the key, can be TYPE_SHORT, TYPE_ASCII and TYPE_DOUBLE */
	tagtype_t keyType;
	/** Number of values, should be 1 for TYPE_SHORT and TYPE_DOUBLE, and string length for TYPE_ASCII */
	UINT32 valCount;
} NCSGeoKeyEntry;

/** 
 *	@typedef NCSFileView
 *	@ingroup structures
 *	@brief This type definition promotes properly transparent usage of the SDK structures.
 */
typedef struct NCSFileViewStruct NCSFileView;

/** 
 * @addtogroup functions
 *  @{
 */

 /**
 * @ingroup functions
 * Gets the initialisation counter value of the SDK. This value is incremented every time NCSInit is called
 * and is decremented every time NCSShutdown is called.
 */
extern INT32 NCS_CALL NCSGetInitCount();

/** 
 * @ingroup functions
 * Initialise the SDK library. Should not be called directly on Windows DLL as it is called automatically 
 * on DllLoad and DllUnload. Must be called explicitly for static librarys and on Unix-like systems.
 */
extern void NCS_CALL NCSInit(void);

/** 
 * @ingroup functions
 * Shutdown the SDK library and free all resources. Should not be called directly on Windows DLL as it is 
 * called automatically on DllLoad and DllUnload. Must be called explicitly for static librarys and on Unix-like systems.
 */
extern void NCS_CALL NCSShutdown(void);

/** 
 *	@brief Set custom functions to be used by the SDK library to open, close, read, seek and tell input files.
 *	This can be used to manage "wrapper" files which encapsulate the ordinary compressed files 
 *  handled by the SDK.  Only supported for decoding ECW/JP2 images.
 *	@ingroup functions
 *	@param	pOpenCB			Callback function for opening input files with utf8 string
 *	@param	pOpenWCB		Callback function for opening input files with wchar string
 *	@param	pCloseCB		Callback function for closing input files
 *	@param	pReadCB			Callback function for reading input files
 *	@param	pReadAsyncCB	Callback function for reading input files that may get called concurrently
 *	@param	pReadCountCB	Callback function for reading input files
 *	@param	pSeekCB			Callback function for seeking input files
 *	@param	pTellCB			Callback function for telling input files
 *	@param  pFileSizeCB		Callback function for getting the file size
 *	@return						NCSError value, NCS_SUCCESS or the code of any applicable error
 */
extern NCSError NCS_CALL NCSSetIOCallbacks(NCSError (NCS_CALL *pOpenCB)(char *szFileName, void **ppClientData),
#ifdef NCS_BUILD_UNICODE
							  NCSError (NCS_CALL *pOpenWCB)(wchar_t *szFileName, void **ppClientData),
#endif
							  NCSError (NCS_CALL *pCloseCB)(void *pClientData),
							  NCSError (NCS_CALL *pReadCB)(void *pClientData, void *pBuffer, UINT32 nLength),
							  NCSError (NCS_CALL *pReadAsyncCB)(void *pClientData, UINT64 nOffset, void *pBuffer, UINT32 nLength),
							  NCSError (NCS_CALL *pReadCountCB)(void *pClientData, void *pBuffer, UINT32 nLength, UINT32 *pBytesRead),
							  NCSError (NCS_CALL *pSeekCB)(void *pClientData, UINT64 nOffset, UINT32 nOrigin),
							  NCSError (NCS_CALL *pTellCB)(void *pClientData, UINT64 *pOffset),
							  NCSError (NCS_CALL *pFileSizeCB)(void *pClientData, INT64 *nFileSize));

/** 
 *	Reports if this is a local or remote file, and breaks URL down into sections
 *	@param	szUrlPath			The URL to be broken down and analysed
 *	@param	ppProtocol			The protocol of the URL (pointer to char *)
 *	@param	pnProtocolLength	The length of the protocol
 *	@param	ppHost				The hostname specified in the URL
 *	@param  pnHostLength		The length of the specified hostname
 *	@param	ppFilename			The filename specified by the URL
 *	@param	pnFilenameLength	The length of the specified filename
 *	@return							BOOLEAN value, if this is a local file
 */
extern BOOLEAN NCS_CALL NCSNetBreakdownUrlA( const char *szUrlPath,
						   char **ppProtocol,	int *pnProtocolLength,
						   char **ppHost,		int *pnHostLength,
						   char **ppFilename,	int *pnFilenameLength);

/** 
 *	Reports if this is a local or remote file, and breaks URL down into sections
 *	@param	szUrlPath			The URL to be broken down and analysed
 *	@param	ppProtocol			The protocol of the URL (pointer to char *)
 *	@param	pnProtocolLength	The length of the protocol
 *	@param	ppHost				The hostname specified in the URL
 *	@param  pnHostLength		The length of the specified hostname
 *	@param	ppFilename			The filename specified by the URL
 *	@param	pnFilenameLength	The length of the specified filename
 *	@return							BOOLEAN value, if this is a local file
 */
#ifdef NCS_BUILD_UNICODE
extern BOOLEAN NCS_CALL NCSNetBreakdownUrlW( const wchar_t *szUrlPath,
						   wchar_t **ppProtocol,	int *pnProtocolLength,
						   wchar_t **ppHost,		int *pnHostLength,
						   wchar_t **ppFilename,	int *pnFilenameLength);
#endif

#ifdef UNICODE
#define NCSNetBreakdownUrl NCSNetBreakdownUrlW
#else
#define NCSNetBreakdownUrl NCSNetBreakdownUrlA
#endif // UNICODE

/** 
 *	Opens a file view.  After calling this function, call GetViewFileInfo to obtain file metadata
 *	@param	szUrlPath			The location of the file on which to open a view
 *	@param	ppNCSFileView		The NCSFileView structure to initialise
 *	@param	pRefreshCallback	The refresh callback with which to handle progressive reads (may be NULL for the blocking interface)
 *	@return							NCSError value, NCS_SUCCESS or any applicable error code
 */
extern NCSError NCS_CALL NCSOpenFileViewA(const char *szUrlPath, NCSFileView **ppNCSFileView,
                       NCSReadStatus (*pRefreshCallback)(NCSFileView *pNCSFileView));

/** 
 *	Opens a file view.  After calling this function, call GetViewFileInfo to obtain file metadata
 *	@param	szUrlPath			The location of the file on which to open a view
 *	@param	ppNCSFileView		The NCSFileView structure to initialise
 *	@param	pRefreshCallback	The refresh callback with which to handle progressive reads (may be NULL for the blocking interface)
 *	@return							NCSError value, NCS_SUCCESS or any applicable error code
 */
extern NCSError NCS_CALL NCSOpenFileViewW(const wchar_t *szUrlPath, NCSFileView **ppNCSFileView,
                       NCSReadStatus (*pRefreshCallback)(NCSFileView *pNCSFileView));

#ifdef UNICODE
#define NCSOpenFileView NCSOpenFileViewW
#else
#define NCSOpenFileView NCSOpenFileViewA
#endif // UNICODE

/**	
 *	Closes a file view.  This can be called at any time after NCScbmOpenFileView is called to clean up an open file view.
 *	@param	pNCSFileView		The file view to close
 *	@return							NCSError value, NCS_SUCCESS or any applicable error code
 */
extern NCSError NCS_CALL NCSCloseFileView(NCSFileView *pNCSFileView);

/**	
 *	Closes a file view.  This can be called at any time after NCScbmOpenFileView is called to clean up an open file view.
 *	This version allows the developer to forcibly close a file and free the resources allocated to it.
 *	@param	pNCSFileView		The file view to close
 *	@param	bFreeCachedFile		Whether to force the freeing of the file's memory cache
 *	@return							NCSError value, NCS_SUCCESS or any applicable error code
 */
extern NCSError NCS_CALL NCSCloseFileViewEx(NCSFileView *pNCSFileView, BOOLEAN bFreeCachedFile);

/** 
 *	Populates a structure with information about an open image file.  Use this version when dealing with ECW files only.
 *	@param	pNCSFileView		The file view open on the file whose metadata is being obtained
 *	@param	ppNCSFileInfo		A pointer to a pointer to the NCSFileInfo struct to populate with the metadata
 *	@return								NCSError value, NCS_SUCCESS or any applicable error code
 */
extern NCSError NCS_CALL NCSGetViewFileInfo(NCSFileView *pNCSFileView, NCSFileInfo **ppNCSFileInfo);

/** 
 *	Obtains information about the current request to set the view, including statistics about the data being processed
 *	@param	pNCSFileView			The open file view the extents of which are currently being set
 *	@param	ppNCSSetViewInfo		The information about the view being set
 *	@return								NCSError value, either NCS_SUCCESS or an applicable error code
 */
extern NCSError NCS_CALL NCSGetSetViewInfo(NCSFileView *pNCSFileView, NCSSetViewInfo **ppNCSSetViewInfo);

/** 
 *	Sets the extents and band content of an open file view, and the output view size.  This function can be called at 
 *	any time after a successful call to NCScbmOpenFileView.  In progressive mode, multiple calls to NCScbmSetFileView 
 *	can be made, even if previous SetViews are still being processed, enhancing client interaction with the view.  After 
 *	the call to NCScbmSetFileView, the band list array pBandList can be freed if so desired.  It is used only during the 
 *  processing of the call, and not afterward.
 *	@param	pNCSFileView			The open file view to set.
 *	@param	nBands					The number of bands in the band list.
 *	@param	pBandList				An array of integers specifying which bands of the image to read, and in which order
 *	@param	nTLX					Left edge of the view in dataset cells
 *	@param	nTLY					Top edge of the view in dataset cells
 *	@param	nBRX					Right edge of the view in dataset cells
 *	@param	nBRY					Bottom edge of the view in dataset cells
 *	@param	nSizeX					Width of the view to be constructed from the image subset
 *	@param	nSizeY					Height of the view to be constructed from the image subset
 *	@return								NCSError value, NCS_SUCCESS or any applicable error code
 */
extern NCSError NCS_CALL NCSSetFileView(NCSFileView *pNCSFileView,
										   UINT32 nBands,					
										   UINT32 *pBandList,				
										   UINT32 nTLX, UINT32 nTLY,	
										   UINT32 nBRX, UINT32 nBRY,
										   UINT32 nSizeX, UINT32 nSizeY);	

/** 
 *	Sets the extents and band content of an open file view, and the output view size.  This function can be called at 
 *	any time after a successful call to NCSOpenFileView.  In progressive mode, multiple calls to NCSSetFileView
 *	can be made, even if previous SetViews are still being processed, enhancing client interaction with the view.  After 
 *	the call to NCSSetFileView, the band list array pBandList can be freed if so desired.  It is used only during the
 *  processing of the call, and not afterward.  This version also allows the calling program to specify world coordinates 
 *	for the view.
 *	@param	pNCSFileView		The open file view to set.
 *	@param  nBands				The number of band in the band list.
 *	@param	pBandList			An array of integers specifying which bands of the image to read, and in which order.
 *	@param	nTLX				Left edge of the view in dataset cells.
 *	@param	nTLY				Top edge of the view in dataset cells.
 *	@param	nBRX				Bottom edge of the view in dataset cells.
 *	@param	nBRY				Right edge of the view in dataset cells.
 *	@param	nSizeX				Width of the view to be constructed from the image subset.
 *	@param	nSizeY				Height of the view to be constructed from the image subset.
 *	@param	fWorldTLX			Left edge of the view in world coordinates.
 *	@param	fWorldTLY			Top edge of the view in world coordinates.
 *	@param	fWorldBRX			Right edge of the view in world coordinates.
 *	@param	fWorldBRY			Bottom edge of the view in world coordinates.
 *	@return						NCSError value, NCS_SUCCESS or any applicable error code.
 */
extern NCSError NCS_CALL NCSSetFileViewEx(NCSFileView *pNCSFileView,
											 UINT32 nBands,					
											 UINT32 *pBandList,				
											 UINT32 nTLX, UINT32 nTLY,	
											 UINT32 nBRX, UINT32 nBRY,
											 UINT32 nSizeX, UINT32 nSizeY,	
											 IEEE8 fWorldTLX, IEEE8 fWorldTLY,		
											 IEEE8 fWorldBRX, IEEE8 fWorldBRY);	

/** 
 *	Read line by line in BIL format.
 *	@param	pNCSFileView			The open file view from which to read view lines
 *	@param	ppOutputLine			The buffer into which to read the interleaved band information
 *	@return								NCSReadStatus value, NCS_READ_OK or any applicable error code
 */
extern NCSReadStatus NCS_CALL NCSReadViewLineBIL( NCSFileView *pNCSFileView, UINT8 **ppOutputLine);

/** 
 *	Read line by line in BIL format to different data types.  This extended version allows the client 
 *	program to read in view lines made up of cells with sample bitdepth other than 8-bit.
 *	@param	pNCSFileView			The open file view from which to read view lines
 *	@param	eType					The cell type of the view lines being read
 *	@param	ppOutputLine			The buffer into which to read the interleaved band information
 *	@return								NCSReadStatus value, NCS_READ_OK or any applicable error code
 */
extern NCSReadStatus NCS_CALL NCSReadViewLineBILEx( NCSFileView *pNCSFileView, NCSCellType eType, void **ppOutputLine);

/** 
 *	Read line by line in RGB format.
 *	@param	pNCSFileView			The open file view from which to read view lines
 *	@param	pRGBTriplets			The buffer into which to read the red-green-blue sample triplets.
 *	@return								NCSReadStatus value, NCS_READ_OK or any applicable error code
 */
extern NCSReadStatus NCS_CALL NCSReadViewLineRGB( NCSFileView *pNCSFileView, UINT8 *pRGBTriplets);

/** 
 *	Read line by line in BGR format.
 *	@param	pNCSFileView			The open file view from which to read view lines
 *	@param	pBGRTriplets			The buffer into which to read the blue-red-green sample triplets.
 *	@return								NCSReadStatus value, NCS_READ_OK or any applicable error code
 */
extern NCSReadStatus NCS_CALL NCSReadViewLineBGR( NCSFileView *pNCSFileView, UINT8 *pBGRTriplets);

/**
 *	Read line by line in RGBA format.  Samples are read into a buffer of UINT32 values, each value comprising
 *	the four bytes of a red-green-blue-alpha sample.  Alpha values will be zero if the input file is in ECW
 *	format as this format does not 'understand' alpha channels.  SDK programmers wanting to compress and 
 *	decompress data in four bands are advised to use multiband compression and NCSReadViewLineBil(Ex) to
 *	handle their data.
 *	@param	pNCSFileView			The open file view from which to read view lines
 *	@param	pRGBA					The buffer of packed UINT32 values.
 *	@return							NCSReadStatus value, NCS_READ_OK or any applicable error code.
 */
extern NCSReadStatus NCS_CALL NCSReadViewLineRGBA( NCSFileView *pNCSFileView, UINT32 *pRGBA);

/**	
 *	Read line by line in BGRA format.  Samples are read into a buffer of UINT32 values, each value comprising
 *	the four bytes of a red-green-blue-alpha sample.  Alpha values will be zero if the input file is in ECW
 *	format as this format does not 'understand' alpha channels.  SDK programmers wanting to compress and 
 *	decompress data in four bands are advised to use multiband compression and NCSReadViewLineBil(Ex) to
 *	handle their data.
 *	@param	pNCSFileView	The open file view from which to read view lines.
 *	@param	pBGRA			The buffer of packed UINT32 values.
 *	@return						NCSReadStatus value, NCS_READ_OK or any applicable error code.
 */
extern NCSReadStatus NCS_CALL NCSReadViewLineBGRA( NCSFileView *pNCSFileView, UINT32 *pBGRA);

/** 
 *	Return the major and minor versions of this SDK.
 *	@param	nMajor		The major version.
 *	@param	nMinor		The minor version.
 */
extern void NCS_CALL NCSGetLibVersion( INT32 *nMajor, INT32 *nMinor );

/**	
 *	Set global configuration parameters for the SDK. These parameters should be set before opening any file views.
 *	Example: UINT64 maxmem = 2048; NCSError error = NCSSetConfig(NCSCFG_CACHE_MAXMEM_64, maxmem);
 *	@param	eType	The configuration parameter to set, to be followed in the variable argument list by its desired value
 *	@return					NCSError value, NCS_SUCCESS or any applicable error code
 */
extern NCSError NCS_CALL NCSSetConfig(NCSConfigType eType, ...);

/**	
 *	Get configuration parameters for the SDK. This function is a var_args style function, the second argument type depends on the type parameter.
 *	Example: UINT64 maxmem = 0; NCSError error = NCSGetConfig(NCSCFG_CACHE_MAXMEM_64, &maxmem);
 *	@param	eType		The configuration parameter to obtain, to be followed in the variable argument list by a value buffer
 *	@return					NCSError value, NCS_SUCCESS or any applicable error code
 */
extern NCSError NCS_CALL NCSGetConfig(NCSConfigType eType, ...);

/**
 * Initialize a FileInfo structure.
 * @param		pDst			The file info structure to initialize.
 */
extern void NCS_CALL NCSInitFileInfo(NCSFileInfo *pDst);

/**
 * Free a FileInfo structure.
 * @param		pDst			The file info structure to initialize.
 */
extern void NCS_CALL NCSFreeFileInfo(NCSFileInfo *pDst);

/**
 * Copy a FileInfo structure.
 * @param	pDst	The destination file info structure.
 * @param	pSrc	The source file info structure to copy.
 */
extern void NCS_CALL NCSCopyFileInfo(NCSFileInfo *pDst, const NCSFileInfo *pSrc);

/**
 *	Get the file type of the file view (typically ECW or JP2).
 *	@param[in]	pNCSFileView		The file view
 *	@return		NCSFileType		The enum type of the file.
 */
extern NCSFileType NCS_CALL NCSGetFileType( NCSFileView *pNCSFileView );

/**
 *	Get the mime type of the file view (typically image/x-ecw or image/jp2)
 *	@param[in]	pNCSFileView	The file view.
 *	@return		NCSFileType        A char* string representing the mime type.
 */
extern char* NCS_CALL NCSGetFileMimeType( NCSFileView *pNCSFileView );

/**
 * Check if a FileInfo structure is georeferenced.
 * @param		pInfo			File info to test.
 * @return		BOOLEAN			TRUE if FileInfo is georeferenced (Non RAW/RAW).
 */
extern BOOLEAN NCS_CALL NCSIsFileInfoExGeoreferenced(NCSFileInfo *pInfo);

/**
 * Standardize a FileInfo structure
 * @param		pInfo			File info to standardize.
 */
extern void NCS_CALL NCSStandardizeFileInfoEx(NCSFileInfo *pInfo);

/**
 * Set the custom GDT data path (for EPSG<-->ER Mapper projection/datum mapping)
 * @param szPath the new GDT data path
 */
extern void NCS_CALL NCSSetGDTPath(const char *szPath);

/**
 * Get the custom GDT data path (for EPSG<-->ER Mapper projection/datum mapping)
 * @return the custom path being used, or NULL
 */
extern char* NCS_CALL NCSGetGDTPath(void);

/**
 * Get an EPSG Code for a specified GDT Datum/Projection pair.
 * @param		szDatum			GDT Datum name.
 * @param		szProjection	GDT Projection name.
 * @param		pnEPSG			Returned EPSG code.
 * @return		NCSError		NCS_SUCCESS, or error on failure.
 */
extern NCSError NCS_CALL NCSGetEPSGCode(const char *szDatum, const char *szProjection, INT32 *pnEPSG);

/**
 * Get an ER Mapper projection/datum pair from an EPSG code.
 * @param		nEPSGCode		EPSG code
 * @param		pszProjection	Returned ER Mapper projection string.
 * @param		pszDatum		Returned ER Mapper datum string.
 * @return		NCSError		NCS_SUCCESS, or error on failure.
 */
extern NCSError NCS_CALL NCSGetProjectionAndDatum(INT32 nEPSGCode, char **pszProjection, char **pszDatum);

/**
 * Refresh the custom GDT data path, doing a search and validity check on
 * the current path value and several common GDT data locations.
 */
extern void NCS_CALL NCSDetectGDTPath(void);

/**
 * Set the usage of geographical metadata when reading and writing JPEG 2000 files.
 * @param		nGeodataUsage	NCSGeodataUsage enum value specifying which metadata to use.
 */
extern void NCS_CALL NCSSetJP2GeodataUsage(NCSGeodataUsage nGeodataUsage);

/**
 * Get the statistics (if they exist) on the open file view.
 * @param		pNCSFileView	An open file view from which to retrieve the stats.
 * @param		ppStats 		A pointer to stats data which is allocated and returned if the file contains stats, or NULL if the file doesn't. 
 * @return     	NCSError		NCS_SUCCESS, or error on failure.
 * @note		Ensure *ppStats is initialised as NULL.
 */
extern NCSError NCS_CALL NCSGetViewStatistics(NCSFileView *pNCSFileView, NCSFileStatistics** ppStats);

/**
 * Initialize the statistics structure. If histBucketCounts is not NULL then NCSInitStatisticsHistograms is called implicitly.
 * @param		ppStats 			A pointer to stats data that needs to be initialized.
 * @param		numberOfBands 		The number of image bands.
 * @param		histBucketCounts	The list of histogram lengths.
 * @return      NCSError			NCS_SUCCESS, or error on failure.
 * @note		Ensure *ppStats is initialised as NULL.
 */
extern NCSError NCS_CALL NCSInitStatistics(NCSFileStatistics** ppStats, UINT32 numberOfBands,UINT32* histBucketCounts);

/**
 * Initialize the statistics structure and NCSInitStatisticsHistograms is called with an array of numberOfBands length filled
 * with nHistBucketCounts
 * @param		ppStats 	a pointer to stats data that needs to be initialized
 * @param		numberOfBands 	number of image bands 
 * @param		nHistBucketCounts histogram counts
 * @return      NCSError     NCS_SUCCESS, or error on failure
 * @note		Ensure *ppStats is initialised as NULL.
 */
extern NCSError NCS_CALL NCSInitStatisticsDefault(NCSFileStatistics** ppStats, UINT32 numberOfBands,UINT32 nHistBucketCounts);

/**
 * Initialize the histograms of the statistics structure.
 * @param		ppStats 			A pointer to stats data that needs to be initialized.
 * @param		histBucketCounts 	Histogram bucket counts.
 * @return      NCSError			NCS_SUCCESS, or error on failure.
 * @note		Ensure ppStats is initialised
 */
extern NCSError NCS_CALL NCSInitStatisticsHistograms(NCSFileStatistics* ppStats, UINT32* histBucketCounts);

/**
 * Free a statistics structure.
 * @param		pStats		Statistics data to be freed.
 * @return      NCSError	NCS_SUCCESS, or error on failure.
 */
extern NCSError NCS_CALL NCSFreeStatistics(NCSFileStatistics* pStats);

/**
 * Copy a statistics structure.
 * @param		pDstStats 	Destination stats data that needs to be copied into.
 * @param		pSrcStats 	Source stats data that needs to be copied from.
 * @return      NCSError    NCS_SUCCESS, or error on failure.
 * @note		*ppDstStats will be allocated first, ensure *ppStats is initialised as NULL.
 */
extern NCSError NCS_CALL NCSCopyStatistics(NCSFileStatistics** pDstStats, const NCSFileStatistics* pSrcStats);

/**
 * Initialize the file metadata structure.
 * @param		ppFileMetaData 	A pointer to FileMetaData structre to be initialized.
 * @return      NCSError     NCS_SUCCESS, or error on failure
 * @note		Ensure *ppFileMetaData is initialised as NULL.
 */
extern NCSError NCS_CALL NCSInitMetaData(NCSFileMetaData** ppFileMetaData);

/**
 * Copy a metadata strucutre.
 * @param		ppDstMetaData 	A pointer to a destination meta data structure to copy to.
 * @param		pSrcMetaData	A source meta data to copy from.
 * @return      NCSError		NCS_SUCCESS, or error on failure.
 * @note		*ppDstMetaData will be allocated first, ensure *ppDstMetaData is initialised as NULL.
 */	
extern NCSError NCS_CALL NCSCopyMetaData(NCSFileMetaData** ppDstMetaData, const NCSFileMetaData* pSrcMetaData);

/**
 * Free the metadata structure.
 * @param		pMetaData 	 Meta data that needs to be freed.
 * @return      NCSError     NCS_SUCCESS, or error on failure.
 */
extern NCSError NCS_CALL NCSFreeMetaData(NCSFileMetaData* pMetaData);

/**
 * Get the RPC data from an open file view.
 * @param		pNCSFileView	The open file view to get the RPCData from.
 * @param		ppRPCData 		A pointer to a RPC data structure which is allocated and returned if the file contains RPC data, or NULL if the file doesn't.
 * @return      NCSError		NCS_SUCCESS, or error on failure.
 * @note        Ensure the *ppRPCData pointer is initialised as NULL.
 */
extern NCSError NCS_CALL NCSGetViewRPCData(NCSFileView *pNCSFileView, NCSRPCData** ppRPCData);

/**
 * Copy an RPC data structure.
 * @param		pDstRPCData 	Destination RPC data that needs to be copied into.
 * @param		pSrcRPCData  	Source RPC data that needs to be copied from.
 * @return      NCSError		NCS_SUCCESS, or error on failure.
 * @note		*pDstRPCData will be allocated first, so ensure *ppDstMetaData is initialised as NULL.
 */
extern NCSError NCS_CALL NCSCopyRPCData(NCSRPCData** pDstRPCData, const NCSRPCData *pSrcRPCData);

/**
 * Free the RPC data structure.
 * @param		pRPCData 	 The meta data structure that needs to be freed.
 * @return      NCSError     NCS_SUCCESS, or error on failure.
 */
extern NCSError NCS_CALL NCSFreeRPCData(NCSRPCData* pRPCData);

/**
 * Get all geotiff key entries from the opened fileview.
 * @param		pNCSFileView 	The open file view from which to get geotiff key entries.
 * @param		ppGeoKeyIds 	A pointer to a geokeyentry array which is allocated and returned if the file contains any geokey data, or NULL if the file doesn't.
 * @param		count 			Count of geokeyentry data.
 * @return      NCSError		NCS_SUCCESS, or error on failure.
 * @note		Ensure *ppGeoKeyIds is initialised as NULL.
 */
extern NCSError NCS_CALL NCSGetAllGeotiffKeyEntries(NCSFileView *pNCSFileView, NCSGeoKeyEntry **ppGeoKeyIds, UINT32* count);

/**
 * Get individual key value frome the open fileview.
 * @param		pNCSFileView 	The open file view from which to get geotiff key.
 * @param		key 			Key id.
 * @param		val 			A pointer to key value.
 * @param		index			Key index, usually zero.
 * @param	    count			Value count. it is one if the key type is short or double and character count if the key type is ascii.
 * @return      NCSError		NCS_SUCCESS, or error on failure.
 */
extern NCSError NCS_CALL NCSGetGeotiffKey(NCSFileView *pNCSFileView, geokey_t key, void *val, int index, int count);

/**
 * Get individual tag details frome the open fileview.
 * @param		pNCSFileView 	The open file view from which to get the geotiff key.
 * @param		tag 			The geotiff tag. It can only be one of GTIFF_PIXELSCALE, GTIFF_TIEPOINT,GTIFF_TRANSMATRIX.
 * @param       pCount			A pointer to value count returned by SDK.
 * @param		ppValue			A pointer to a value array which is allocated and return if the file has the values for the tag, otherwise NULL.
 * @return      NCSError		NCS_SUCCESS, or error on failure.
 * @note		Ensure *ppValue is initialised as NULL.
 */
extern NCSError NCS_CALL NCSGetGeotiffTag(NCSFileView *pNCSFileView, unsigned short tag, int *pCount, IEEE8** ppValue);

/**
 * Purge unused memory immedeatley. This function blocks all pending file read operations, so will affect performance. 
 * For best results run it when there are no read operations in progress. Under normal circumstances, this function
 * is called internally and should never explicitly be called.
 * @return      NCSError     NCS_SUCCESS, or error on failure.
 */
extern NCSError NCS_CALL NCSPurgeUnusedMemory();

/**
 * Get a parameter on the fileview. The file view must have an open file to get the parameter.
 * @param		pNCSFileView	The open file view to get the parameter on.
 * @param		sName			Fully-qualified Name of parameter (eg "JP2:COMPRESS:PROFILE:BASELINE0").
 * @return      NCSError     	NCS_SUCCESS, or error on failure
 * @code
 *    int nAutoScaleUp = -1;
 *    NCSGetViewParameter(pFileView, "JPC:DECOMPRESS:AUTOSCALE:UP", &nAutoScaleUp);
 * @endcode
 */
extern NCSError NCS_CALL NCSGetViewParameter(NCSFileView *pNCSFileView, const char *sName, ...);

/**
 * Set a parameter on the fileview.  These over-ride default settings, such as compression output profile,
 * progression order, tiling etc. You should set parameters only on a valid open file.
 * @param		pNCSFileView	The open file view.
 * @param		sName			Fully-qualified Name of parameter (eg "JP2:COMPRESS:PROFILE:BASELINE0").
 * @return      NCSError     	NCS_SUCCESS, or error on failure
 * @code
 *   NCSSetViewParameter(pFileView, "JPC:DECOMPRESS:AUTOSCALE:UP", TRUE);
 * @endcode
 */
extern NCSError NCS_CALL NCSSetViewParameter(NCSFileView *pNCSFileView, const char *sName, ...);
/**
* Gets default RGBA bandlist.
* The SDK will attempt to detect RGBA bands and return them as 0,1,2,3.  If an opacity
* band is not present only three bands will be returned.  
* @param		pNCSFileView	The open file view.
* @param		pNumberOfBands	Number of bands returned. Memory needs to be allocated by the caller. 
* @param		pBandList	Band numbers. Memory needs to be allocated by the caller. 
* @return      NCSError     	NCS_SUCCESS, or error on failure
* @code
*   UINT32 pBandList[4];
*   UINT32 nNumberOfBands;
*   NCSGetDefaultRGBABandList(pFileView, &nNumberOfBands, &pBandList);
* @endcode
*/
extern NCSError NCS_CALL NCSGetDefaultRGBABandList(const NCSFileView *pNCSFileView, UINT32 * const pNumberOfBands, UINT32 (* const pBandList)[4]);

#ifdef NCS_BUILD_UNICODE
/**
* Convert a wchar string to utf8-encoded char string
* @param		pszDst 	Destination utf8-encode char string.
* @param		szSrc  	Source wchar string that needs to be converted.
* @return      NCSError		NCS_SUCCESS, or error on failure.
* @note		*pszDst will be allocated first, so ensure **pszDst is initialised as NULL.
*/
extern NCSError NCS_CALL NCSConvertWCharToUTF8(char** pszDst, const wchar_t* szSrc);

/**
* Convert utf8-encoded char string to a wchar string 
* @param		pszDst 	Destination wchar string.
* @param		szSrc  	Source wchar string that needs to be converted.
* @return      NCSError		NCS_SUCCESS, or error on failure.
* @note		*pszDst will be allocated first, so ensure **pszDst is initialised as NULL.
*/
extern NCSError NCS_CALL NCSConvertUTF8ToWChar(wchar_t** pszDst, const char* szSrc);
/** @} */
#endif

/**
* Initialize the filebandinfo structure.
* @param	ppFileBandInfo 	A pointer to FileBandInfo structure to be initialized.
* @param    nNumberOfBand	Number of band used for initialization
* @return   NCSError     NCS_SUCCESS, or error on failure
* @note		Ensure *ppFileBandInfo is initialised as NULL.
*/
extern NCSError NCS_CALL NCSInitFileBandInfo(NCSFileBandInfo** ppFileBandInfo, UINT32 nNumberOfBand);

/**
* Free the filebandinfo structure.
* @param	pFileBandInfo 	 pFileBandInfo that needs to be freed.
* @param    nNumberOfBand	Number of band used for initialization
* @return   NCSError     NCS_SUCCESS, or error on failure.
* @note		Ensure nNumberOfBand is the same used in NCSInitFileBandInfo.
*/
extern NCSError NCS_CALL NCSFreeFileBandInfo(NCSFileBandInfo* pFileBandInfo, UINT32 nNumberOfBand);

/**
* Parse the NCSFileStatistics from XML.  This does not Parse the histograms.
* @param		pDstStats 	initialised destination stats data to parse into
* @param		wszXML		source xml document to parse from
* @return      NCSError     NCS_SUCCESS, or error on failure
*/
extern NCSError NCS_CALL NCSStatisticsFromXML(NCSFileStatistics* pDstStats, const NCSTChar* wszXML);
/**
* UnParse the NCSFileStatistics to XML.  This does not UnParse the histograms.
* @param		pSrcStats 	source stats data to unparse from
* @param		wszXML		destination xml document to unparse to
* @return      NCSError     NCS_SUCCESS, or error on failure
* @note		*wszXML will be allocated first, make sure *ppStats is initialised as NULL and NCSFree it when you're finished with it.
*/
extern NCSError NCS_CALL NCSStatisticsToXML(const NCSFileStatistics* pSrcStats, NCSTChar** wszXML);

/**
* Parse the NCSFileMetaData from XML
* @param		pMetaData 	 meta data structure to store the parsed result
* @param		wszXMLData 	 The source XML document
* @return      NCSError     NCS_SUCCESS, or error on failure
*/
extern NCSError NCS_CALL NCSMetaDataFromXML(NCSFileMetaData* pMetaData, const NCSTChar *wszXMLData);
/**
* UnParse the NCSFileMetaData to XML
* @param		pMetaData 	 meta data structure to read from
* @param		wszXMLData 	 The destination XML document.  This memory will be alloc'd caller must NCSFree it
* @return      NCSError     NCS_SUCCESS, or error on failure
*/
extern NCSError NCS_CALL NCSMetaDataToXML(const NCSFileMetaData* pMetaData, NCSTChar **wszXMLData);

#ifdef __cplusplus
}
#endif

#endif	// NCSECWCLIENT_H
