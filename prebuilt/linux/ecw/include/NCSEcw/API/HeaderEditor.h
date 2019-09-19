/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/

/** @file HeaderEditor.h */

#ifndef NCS_CHEADER_EDITOR
#define NCS_CHEADER_EDITOR

#include "NCSECWClient.h"
#include "NCSECWHeaderEditor.h"
#include "NCSString.h"

namespace NCS {

/*!
 * @class	CHeaderEditor
 * @ingroup classes
 * @brief	Header editor class to modify file, metadata, RPC and statistics information without having to rewrite the file.
 */
class NCSECW_IMPEX CHeaderEditor {
public:
	/** Default constructor, initialises members */
	CHeaderEditor();

	/** Destructor, clean up members */
	virtual ~CHeaderEditor();
	/** 
	* Get version number on the specified ecw or jp2 file
	* @return  version number    
	*/
	UINT32 GetVersion();
	/** 
	* Get file type on the specified ecw or jp2 file
	* @return  file type    
	*/
	NCSFileType GetFileType();
	/** 
	* Get number of bands on the specified ecw or jp2 file
	* @return  number of bands    
	*/
	UINT32 GetNumberOfBands();   
	/** 
	* Whether the specified ecw or jp2 file has opacity band
	* @return true for opacity band exists, otherwise false   
	*/
	bool HasOpacity(); 
    /** 
	* Get cell type on the specified ecw or jp2 file
	* @return cell type 
	*/
	NCSCellType GetCellType();
	/** 
	* Open the file path on the header editor. The method will read all information out if reading is successful.
	* @param		sFilePath	Full path name of ecw or jp2 file
	* @return      CError		NCS_SUCCESS or error code on failure.
	*/
	NCSError OpenFile(const NCS::CString& sFilePath);
	/** 
	* Set the NCSFileMetaData on the specified ecw or jp2 file. 
	* @param		pFileMetaData	a pointer to NCSFileMetaData that needs to be set
	* @return      CError		NCS_SUCCESS or error code on failure.
	*/
	NCSError SetFileMetaData(NCSFileMetaData* pFileMetaData);
	/** 
	* Get the NCSFileMetaData on the specified ecw or jp2 file. 
	* @return      NCSFileMetaData*		a pointer to NCSFileMetaData structure on the file
	*/
	const NCSFileMetaData* GetFileMetaData() const;
	/** 
	* Set the NCSFileStatistics on the specified ecw or jp2 file. 
	* @param		pStat	a pointer to NCSFileStatistics that needs to be set
	* @return      CError		NCS_SUCCESS or error code on failure.
	*/
	NCSError SetStatistics(const NCSFileStatistics *pStat);
	/** 
	* Get the NCSFileStatistics on the specified ecw or jp2 file. 
	* @return      NCSFileStatistics*		a pointer to NCSFileStatistics structure on the file
	*/
	const NCSFileStatistics* GetStatistics() const ;
	/** 
	* Set the NCSRPCData on the specified ecw or jp2 file. 
	* @param		pRPCData	a pointer to NCSRPCData that needs to be set
	* @return      CError		NCS_SUCCESS or error code on failure.
	*/
	NCSError SetRPCData(const NCSRPCData *pRPCData);
	/** 
	* Get the NCSRPCData on the specified ecw or jp2 file. 
	* @return      NCSRPCData*		a pointer to NCSRPCData structure on the file
	*/
	const NCSRPCData* GetRPCData() const ;
	/**
	 * Get all geotiff key entries
	 * @param		ppGeoKeyIds 	a pointer to a geokeyentry array which is allocated and returned if the file contains any geokey data, or NULL if the file doesn't
	 * @param		count 			count of geokeyentry data
	 * @return      NCSError     NCS_SUCCESS, or error on failure
	 * @note		make sure *ppGeoKeyIds is initialised as NULL
	 */
	NCSError GetAllGeotiffKeyEntries(NCSGeoKeyEntry **ppGeoKeyIds, UINT32* count);
	/**
	 * Get individual tag detail
	 * @param		tag 			The geotiff tag. It can only be GTIFF_PIXELSCALE, GTIFF_TIEPOINT,GTIFF_TRANSMATRIX.
	 * @param       pCount			a pointer to value count returned by SDK.
	 * @param		ppValue			a pointer to a value array which is allocated and return if the file has the values for the tag, otherwise NULL
	 * @return      NCSError     NCS_SUCCESS, or error on failure
	 * @note		make sure *ppValue is initialised as NULL
	 */
	NCSError GetGeotiffTag(unsigned short tag, int *pCount, IEEE8** ppValue);
	/**
	 * Get individual key value
	 * @param		key 			key id
	 * @param		val 			a pointer to key value
	 * @param		index			key index, usually is zero
	 * @param	    count			value count. it is one if the key type is short or double and character count if the key type is ascii
	 * @return      NCSError     NCS_SUCCESS, or error on failure
	 */
	NCSError GetGeotiffKey(geokey_t key, void *val, int index, int count);
	/** 
	 *	Set a Geotiff key in the header of the ECW v3 file.
	 *  @param[in]  tag				The geotiff tag. It can only be GTIFF_PIXELSCALE, GTIFF_TIEPOINT,GTIFF_TRANSMATRIX.
	 *  @param[in]  count			Value count that is stored in values.
	 *  @param[in]  values			Value array. 
	 *	@return						NCSError value, NCS_SUCCESS or any applicable error code
	 */
	NCSError SetGeotiffTag(unsigned short tag, int count, IEEE8* values);
	/** 
	 *	Set a geotiff key in the header of the ECW v3 file. This function uses a var_args style argument list, where the variant arguments are the key values.
	 *  @param[in]  keyID			The geotiff key. It should be in the definitions in geokeys.h
	 *  @param[in]  type			The type for the geotiff key.
	 *  @param[in]  count			Count. Normally count is 1, one time to set one key. -1 for removal case.
	 *	@return					    NCSError value, NCS_SUCCESS or any applicable error code
	 */
	NCSError SetGeotiffKey(geokey_t keyID, tagtype_t type, int count, ...);
	/**
	 * Flush all the settings to the file
	 */
	NCSError FlushAll();
	/**
	 * Close the file on the header editor
	 */
	NCSError Close();

private:
	NCSFileView* m_pFileView;
	
	void CleanUpInternalStructures();

	NCS::CString m_sFilePath;
	NCSFileStatistics* m_pFileStats;
	NCSRPCData* m_pRPCData;
	NCSFileMetaData* m_pFileMetaData;

	UINT32 m_nVersion;
	NCSFileType m_eFileType;
	UINT32 m_nNumberOfBands;   
	bool m_bHasOpacity; 
	NCSCellType m_eCellType;

};

} //namespace NCS



#endif
