/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	NCSECWHeaderEditor.h
** CREATED:	01/03/2013
** PURPOSE:	C API for editing the headers of ECW/JP2 datasets.
**
********************************************************/

/** @file NCSECWHeaderEditor.h */

#ifndef NCS_ECW_HEADER_EDITOR
#define NCS_ECW_HEADER_EDITOR

#include "NCSECWClient.h"
#include "NCSEcw/GTIF/geokeys.h"
#include "NCSEcw/GTIF/geo_public_defs.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct NCSEditInfo
 * @ingroup structures
 * @brief Structure containing editable items for ECW and JPEG 2000 metadata.
 */
typedef struct {
	/** The cell size units for the pixel size, setting is not honoured. */
	NCSCellSizeUnitType	eCellSizeUnits; 
	/** The cell increment in x direction, may be negative, will never be zero. */
	IEEE8				fCellIncrementX;
	/** The cell increment in y direction, may be negative, will never be zero. */
	IEEE8				fCellIncrementY;
	/** The world origin x coordinate of the top left cell, in  NCSCellSizeUnitType*/
	IEEE8				fOriginX;
	/** The world origin y coordinate of the top left cell, in  NCSCellSizeUnitType*/
	IEEE8				fOriginY;
	/** The ER Mapper style datum string, e.g. "RAW" or "NAD27". Will never be NULL*/
	char				*szDatum;
	/** The ER Mapper style projection string, e.g. "RAW" or "WGS84". Will never be NULL */
	char				*szProjection;
	/** The epsg code, setting is not honoured. */
	int					EPSGcode;
	/** True if block offset table is compressed */
	BOOLEAN				bCompressedOffsetTable;
} NCSEditInfo;

/** 
 * @ingroup functions
 * @brief Read the NCSEditInfo from the ecw or jp2 file
 * @param		pFilename	Full path name of JP2 file to read
 * @param		ppInfo		ppInfo 	a pointer to NCSEditInfo which is allocated and returned 
 * @return      CError		NCS_SUCCESS or error code on failure.
 * @note        make sure *ppInfo is initialised as NULL
 */
NCSError NCS_CALL NCSReadEditInfo(const NCSTChar *pFilename, NCSEditInfo **ppInfo);

/**
 * Initialize a EditInfo structure.
 * @param		ppEditInfo 	A pointer to NCSEditInfo structre to be initialized.
 * @return      NCSError     NCS_SUCCESS, or error on failure
 * @note		Ensure *ppEditInfo is initialised as NULL and szDatum and szProjection are to freed before assigning new strings
 */
NCSError NCS_CALL NCSInitEditInfo(NCSEditInfo **ppEditInfo);


/**
* @ingroup functions
* @brief Copy NCSEditInfo data
* @param		pDst 	destination NCSEditInfo that needs to be copied into
* @param		pSrc  	source NCSEditInfo that needs to be copied from
* @return      NCSError     NCS_SUCCESS, or error on failure
* @note		*pDstRPCData will be allocated first, so make sure *ppDstMetaData is initialised as NULL
*/
NCSError NCS_CALL NCSCopyEditInfo(const NCSEditInfo *pSrc, NCSEditInfo **pDst);

/** 
 * @ingroup functions
 * @brief Write the NCSEditInfo to the ecw or jp2 file
 * @param		pFilename	Full path name of JP2 file to write
 * @param		pInfo		a pointer to NCSEditInfo which needs to be written
 * @param	    pProgressFunc callback function when writing is in progress
 * @param        pCancelFunc	  callback function when writing is in progress
 * @param		pClientData   client data that requires for the callbacks
 * @return      NCSError     NCS_SUCCESS, or error on failure 
 * @note        the callback functions will be used when changing bCompressedOffsetTable on ecw version 2 file
 */
NCSError NCS_CALL NCSWriteEditInfo(const NCSTChar *pFilename, NCSEditInfo *pInfo, void(*pProgressFunc)(UINT64 nTotal, UINT64 nWritten, void *pClientData), BOOLEAN(*pCancelFunc)(void *pClientData), void *pClientData);

/**
* @ingroup functions
* @brief Free the NCSEditInfo structure
* @param		pInfo 	a pointer to NCSEditInfo that needs to be freed
* @return      NCSError     NCS_SUCCESS, or error on failure
*/
NCSError NCS_CALL NCSFreeEditInfo(NCSEditInfo *pInfo);

/** 
 * @ingroup functions
 * @brief Open file for editing geotiff keys and tags
 * @param		pFilename	Full path name of ecw v3 file
 * @return      NCSFileView*	A pointer to the fileview for editing if it succeeds, NULL on error.
 */
NCSFileView* NCSEditOpenA(const char *pFilename);

/** 
 * @ingroup functions
 * @brief Open file for editing geotiff keys and tags
 * @param		pFilename	Full path name of ecw v3 file
 * @return      NCSFileView*	A pointer to the fileview for editing if it succeeds, NULL on error.
 */
NCSFileView* NCSEditOpenW(const wchar_t *pFilename);

#ifdef UNICODE

#define NCSEditOpen NCSEditOpenW

#else

#define NCSEditOpen NCSEditOpenA

#endif // UNICODE

/** 
 * @ingroup functions
 * @brief Set the NCSFileStatistics to the ecw or jp2 file
 * @param		pFilename	Full path name of JP2 file to write
 * @param		pStats		a pointer to NCSFileStatistics which needs to be set
 * @return      CError		NCS_SUCCESS or error code on failure.
 */
NCSError NCS_CALL NCSEditSetStatistics(NCSFileView *pNCSFileView,const NCSFileStatistics *pStats);


/** 
 * @ingroup functions
 * @brief Set the RPCData to the ecw or jp2 file
 * @param		pFilename	Full path name of JP2 file to write
 * @param		pRPCData	a pointer to NCSRPCData which needs to be set
 * @return      CError		NCS_SUCCESS or error code on failure.
 */
NCSError NCS_CALL NCSEditSetRPCData(NCSFileView *pNCSFileView,const NCSRPCData *pRPCData);

/** 
 * @ingroup functions
 * @brief Set the File metadata to the ecw or jp2 file
 * @param		pFilename	Full path name of JP2 file to write
 * @param		pFileMetaData	a pointer to NCSFileMetaData which needs to be set
 * @return      CError		NCS_SUCCESS or error code on failure.
 */
NCSError NCS_CALL NCSEditSetFileMetaData(NCSFileView *pNCSFileView,const NCSFileMetaData* pFileMetaData);

/** 
 * @ingroup functions
 * @brief Set geotiff key for editing
 * @param	pNCSFileView		A pointer to the fileview for editing
 * @param  keyID				The geotiff key. It should be in the definitions in geokeys.h
 * @param  type				The type for the geotiff key.
 * @param  count				count. Normally count is 1, one time to set one key. -1 for removal case.
 * @param  varargs             key value(s).
 * @return					    NCSError value, NCS_SUCCESS or any applicable error code
 */
NCSError NCSEditSetGeotiffKey(NCSFileView *pNCSFileView, geokey_t keyID, tagtype_t type, int count, ...);

/** 
 * @ingroup functions
 * @brief Set geotiff tag for editing
 * @param	pNCSFileView			A pointer to the fileview for editing
 * @param  tag						The geotiff tag. It can only be GTIFF_PIXELSCALE, GTIFF_TIEPOINT,GTIFF_TRANSMATRIX.
 * @param  count					value count that is stored in values.
 * @param  values					value array. 
 * @return							NCSError value, NCS_SUCCESS or any applicable error code
 */
NCSError NCSEditSetGeotiffTag(NCSFileView *pNCSFileView, unsigned short tag, int count, IEEE8* values);

/** 
* @ingroup functions
* @brief Flush all the settings to the file
* @param		pNCSFileView	a pointer to the opened fileview.
* @return      CError		NCS_SUCCESS or error code on failure.
*/
NCSError NCSEditFlushAll(NCSFileView *pNCSFileView);

/** 
* @ingroup functions
* @brief Close file for editing
* @param		pFilename	Full path name of the file
* @return      CError		NCS_SUCCESS or error code on failure.
*/
NCSError NCSEditClose(NCSFileView *pNCSFileView);

#ifdef __cplusplus
}
#endif

#endif