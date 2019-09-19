#pragma once

#include "NCSECWClient.h"
#include "NCSString.h"
#include "NCSECWHeaderEditor.h"

#include <memory>

namespace NCS {

class NCSECW_IMPEX IStringUtils {
	NCS_DELETE_ALL_COPY_AND_MOVE(IStringUtils)
public:
	IStringUtils();
	virtual ~IStringUtils();

	virtual NCS::CString GetStringFromColorSpace(NCSFileColorSpace colorSpace) = 0;
	virtual NCS::CString GetStringFromCellSizeUnits(NCSCellSizeUnitType cellSizeUnits) = 0;
	virtual NCS::CString GetStringFromCellType(NCSCellType cellType) = 0;
	virtual NCS::CString GetStringFromFileBandInfo(const NCSFileBandInfo *pFileBandInfo, UINT16 bands) = 0;
	virtual NCS::CString GetStringFromFileInfo(const NCSFileInfo &fileInfo) = 0;
	virtual NCS::CString GetStringFromSetViewInfo(const NCSSetViewInfo &viewSetInfo) = 0;
	virtual CString GetStringFromEditInfo(const NCSEditInfo &editInfo) = 0;
	virtual std::string ConvertDoubleToConsistentString(const double &value) = 0;
};
typedef std::shared_ptr<IStringUtils> IStringUtilsPtr;

class NCSECW_IMPEX CStringUtils : public IStringUtils {
	NCS_DELETE_ALL_COPY_AND_MOVE(CStringUtils)
public:
	CStringUtils();
	virtual ~CStringUtils();

	virtual NCS::CString GetStringFromColorSpace(NCSFileColorSpace colorSpace);
	virtual NCS::CString GetStringFromCellSizeUnits(NCSCellSizeUnitType cellSizeUnits);
	virtual NCS::CString GetStringFromCellType(NCSCellType cellType);
	virtual NCS::CString GetStringFromFileBandInfo(const NCSFileBandInfo *pFileBandInfo, UINT16 bands);
	virtual NCS::CString GetStringFromFileInfo(const NCSFileInfo &fileInfo);
	virtual NCS::CString GetStringFromSetViewInfo(const NCSSetViewInfo &viewSetInfo);
	CString GetStringFromEditInfo(const NCSEditInfo &editInfo) override;
	virtual std::string ConvertDoubleToConsistentString(const double &value);

	void ErasePaddedZerosInExponent(std::string &doubleAsString);

	static CStringUtils &SingletonImpl();
	static IStringUtilsPtr Singleton();
};

}