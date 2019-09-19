/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	GeotiffTagBox.h
** CREATED:	01/03/2012
** PURPOSE:	Internal GTIFF box for ECW declaration
**
********************************************************/

#ifndef _GEOTIFF_TAG_BOX_H_
#define _GEOTIFF_TAG_BOX_H_

#include "NCSECWClient.h"
#include "NCSECWHeaderEditor.h"
#include "NCSEcw/GTIF/geokeys.h"
#include "NCSEcw/GTIF/geo_public_defs.h"
#include "NCSIOStream.h"
#include "NCSEcw/SDK/Box.h"
#include <string>
#include <vector>

typedef struct gtiff GTIF;

namespace NCS {

namespace SDK
{
	class CFileBase;
}

namespace ECW {

typedef struct
{
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
}GeoReferenceInfo;

class GeotiffTagBox : public NCS::SDK::CBox
{
public:
	typedef enum
	{
		GeoFileInfo,
		GeotiffTagKey,
		MethodNone
	}SettingMethod;
	
	class InternalGeoTagStruct
	{
	public:
		explicit InternalGeoTagStruct(SettingMethod settingMethod);
		~InternalGeoTagStruct();

		GTIF* m_pGeotiff;
		UINT32 m_numberOfTiepoints;
		IEEE8* m_tiePoints;
	
		IEEE8* m_pixelScales;
		IEEE8* m_transformationMatrix;
		SettingMethod m_settingMethod;
	};

	static UINT32 sm_nTBox;
		
	/** Default constructor, initialises members */
	GeotiffTagBox();
		/** Virtual destructor */
	virtual ~GeotiffTagBox();

	virtual CError Parse(NCS::SDK::CFileBase &FileBase, CIOStream& Stream);

	virtual CError UnParse(NCS::SDK::CFileBase &FileBase, CIOStream &Stream);

	/* 
		The first set of API.
		Keep backwards capability with setting with existing SDK through GetFileInfo and SetFileInfo
	*/
	NCSError SetFileGeoInfo(const NCSFileInfo& fileInfo);
	NCSError SetEditGeoInfo(const NCSEditInfo& editInfo, IEEE8 fCWRotationDegrees);
	NCSError GetFileGeoInfo(NCSFileInfo& fileInfo, bool isEcwV3) const;

	/*
		The second set of API
		The client can finely control the definition of the coordintate system in details through get/set geotiff keys/tags 
	*/

	//list all the current geotiff keys stored
	void GetAllGeotiffKeyEntries(NCSGeoKeyEntry **ppGeoKeyIds, UINT32* count) const;

	/* geotiff keys define a model space in term of geotiff, such as projection, datum */
	NCSError GetGeotiffKey(geokey_t key, void *val, int index,
                         int count) const;
	NCSError SetGeotiffKey(geokey_t keyID, tagtype_t type, int count, va_list args);

	/* ModelTiePointTag, ModelPixelScaleTag or ModelTransformationTag defines the mapping between model space and raster space */
	NCSError SetGeotiffTag(unsigned short tag, int count, IEEE8* pValue);
	NCSError GetGeotiffTag(unsigned short tag, int *count, IEEE8** ppValue) const;

	// functions that is trying to convert geotiff key/tags into fileinfo and the other way around
	NCSError ConvertToNCSFileViewFileInfoEx(NCSFileInfo& fileInfo) const;
	NCSError ConvertToGeotiffKeyTags(const NCSFileInfo& fileInfo);
	

	/* 
	   The third set of API
	   The client can output geotiff box as a text
	 */
	NCSError ExportTo(std::string& geotiffBoxText);

private:
	NCSError SetGeoInfo(const GeoReferenceInfo& grInfo, bool isEditing);
	
	bool HasMinimumKeyTagSetting();

	UINT32 CalculateSizeInBytes(UINT32 tempASCIIDataLength);

	GTIF* RewriteInternalStructWithTagKeySetting();

	InternalGeoTagStruct* m_pGeotagStruct;

	bool m_bIsFirstKeySetting;

};

} //end of ECW
} //end of NCS


#endif