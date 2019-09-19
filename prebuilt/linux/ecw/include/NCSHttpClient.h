#pragma once

#include "NCSCnetCallBacks.h"
#include "NCSError.h"


namespace NCS {
namespace NET {

typedef NCS::CString HeaderNameType;
typedef NCS::CString HeaderValueType;
typedef std::map<HeaderNameType, HeaderValueType> HeadersType;

class NCSECW_IMPEX HttpConnectionCacheContext {
public:
	HttpConnectionCacheContext();
	virtual ~HttpConnectionCacheContext();
};

class IHttpResponse {
public:
	virtual ~IHttpResponse() {}

	virtual NCS::CError GetResponseData(void*pResponseData) = 0;
	virtual NCS::CError GetResponseData(void **pResponseData, UINT32 *pLength) = 0;
	virtual NCS::CError GetResponseData(void **pResponseData, UINT32 *pLength, NCScnetWriteCallback pWriteCB, int nDataReadSize, void *pUserData) = 0;
	virtual bool GetHeaderValue(const HeaderNameType&  headerName, HeaderValueType& headerValue) = 0;
	virtual INT32 GetHttpResponseCode() = 0;
	virtual NCS::CError GetLastError() = 0;

	virtual INT32 GetContentLength() = 0;
	virtual NCS::CString GetContentType() = 0;

	virtual bool GetIsAuthorized() = 0;
	virtual void SetIsAuthorized(bool isAuth) = 0;
};

class IHttpRequest {
public:
	virtual ~IHttpRequest() {}
	virtual void SetUrl(const NCS::CString &sUrl) = 0;
	virtual void SetRequestBody(const void *pBody, const UINT32 nBodyLength) = 0;
	virtual void SetHttpMethod(HttpMethod httpMethod) = 0;
	virtual void SetUseCache(bool bValue) = 0;
	virtual void SetHeader(const HeaderNameType& headerName, const HeaderValueType& headerValue) = 0;
	virtual void SetSuppressDialog(bool bValue) = 0;
	virtual void SetConnectionTimeout(int nTimeoutInSeconds) = 0;
	virtual IHttpResponse *SendHttpRequest(HttpConnectionCacheContext* pHttpConnectionCacheContext = NULL) = 0;
};

class IHttpRequestFactory {
public:
	virtual ~IHttpRequestFactory() {}
	virtual IHttpRequest *GetHttpRequest() = 0;
	virtual HttpConnectionCacheContext *CreateConnectionCacheContext() = 0;
	
};

class NCSECW_IMPEX CHttpRequestFactory : public IHttpRequestFactory {
public:
	CHttpRequestFactory();
	virtual ~CHttpRequestFactory();

	virtual IHttpRequest *GetHttpRequest();
	virtual HttpConnectionCacheContext *CreateConnectionCacheContext();
};

}
}