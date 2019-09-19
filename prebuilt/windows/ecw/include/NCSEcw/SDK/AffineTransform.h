/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     AffineTransform.h
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CFile class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
**			 [01] 03Aug04 tfl  Added integer member to NCSJP2PCSBox
**			 [02] 07Dec04 tfl  Moved NCSGeodataUsage enum declaration to NCSECWClient.h
**			 [03] 17Dec04 tfl  Added static CFile::Shutdown() function called by CFile::Shutdown()
 *******************************************************/

#ifndef NCSAFFINETRANSFORM_H
#define NCSAFFINETRANSFORM_H

#include "NCSECWClient.h"
#include "NCSEcw/JP2/File.h" //for TIFF tag

namespace NCS {

class CAffineTransform 
{

public:
	IEEE8 fScaleX;
	IEEE8 fScaleY;
	IEEE8 fRotX;
	IEEE8 fRotY;
	IEEE8 fTransX;
	IEEE8 fTransY;
	
	CAffineTransform();
	explicit CAffineTransform(IEEE4 fPrecision);
	~CAffineTransform();

	bool GetFromFileInfo(NCSFileInfo &FileInfo, bool bRasterPixelIsArea);
	bool GetFromGeoTIFFModelTransformationTag(JP2::CFile::CPCSBox::TIFFTag *pTransformTag);
	bool PopulateFileInfo(NCSFileInfo &FileInfo, bool bRasterPixelIsArea);
	bool IsValid();
protected:
	void Standardize();
	IEEE8 m_fPrecision;
};
}

#endif
