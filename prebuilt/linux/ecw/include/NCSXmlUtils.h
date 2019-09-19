#pragma once

#include "NCSError.h"

#include <memory>

class NCSTiXmlHandle;

namespace NCS {

class IXmlUtils {
public:
	IXmlUtils();
	virtual ~IXmlUtils();

	virtual NCS::CError ExtractXMLField(NCSTiXmlHandle& rootXMLHandle, const char* xmlTag, NCSTChar** ppData) = 0;
};
typedef std::shared_ptr<IXmlUtils> IXmlUtilsPtr;

class CXmlUtils : public IXmlUtils {
public:
	CXmlUtils();
	virtual ~CXmlUtils();

	virtual NCS::CError ExtractXMLField(NCSTiXmlHandle& rootXMLHandle, const char* xmlTag, NCSTChar** ppData);

	static CXmlUtils &SingletonImpl();
	static IXmlUtilsPtr Singleton();
};

}