/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     WorldFile.h 
** CREATED:  02/09/2004 3:27:34 PM
** AUTHOR:   Tom Lynch
** PURPOSE:  Utility class for reading six-value world files
** EDITS:    [xx]	ddMmmyy	NAME	COMMENTS
 *******************************************************/

#include "NCSEcw/SDK/AffineTransform.h"
#ifndef ERS_JP2_WORLD_FILE_EXTS
#define ERS_JP2_WORLD_FILE_EXTS	{ ".j2w" , ".jxw" , ".jfw" }  
#endif

namespace NCS {
namespace SDK {

class CWorldFile: public CAffineTransform
{
public:
	CWorldFile();
	explicit CWorldFile(IEEE8 fPrecision);
	~CWorldFile();

	bool Read(char *pWorldFilename);
	bool WriteMSB(char *pWorldFilename);
	bool ConstructWorldFilename(char *szRasterFilename, char **szWorldFilename);
private:
	static inline bool ScanValue(FILE *pFile, IEEE8 *pValue);
};

}
}
