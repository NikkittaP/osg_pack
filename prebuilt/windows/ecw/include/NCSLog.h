/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSLog.h
** CREATED:	Mon May 31 09:19:00 WST 1999
** AUTHOR: 	Doug Mansell
** PURPOSE:	NCS server logging interface
** EDITS:
** [01] 06-06-02 Changed the Registry key names to match IWS 1.7
** [02] 08-12-05 tfl lint fixes
 *******************************************************/


#ifndef NCSLOG_H
#define NCSLOG_H

#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif // NCSTYPES_H

#include "NCSILogger.h"


#define MOD_ECWSDK_UTIL	NCS_T("com.erdas.ecwsdk.util")
#define MOD_ECWSDK_CNET	NCS_T("com.erdas.ecwsdk.cnet")
#define MOD_ECWSDK_ECWJP2	NCS_T("com.erdas.ecwsdk.ecwjp2")
#define MOD_GENERIC_ERROR	NCS_T("com.erdas.generic.error")

#ifdef __cplusplus 
extern "C" {
#endif
#ifndef SWIG

#define NCS_SERVER_LOG_FILE_NAME_PREF		"Log Filename"		//[01]
#define NCS_SERVER_LOG_FILE_LEVEL_PREF		"Log Level"			//[01]
#define NCS_LOG_FILE_NAME_PREF				"IWS Log Filename"
#define NCS_LOG_FILE_LEVEL_PREF				"IWS Log Level"



void NCSLogGetConfig(char **pLogName, NCSLogLevel *pLogLevel);

void NCSLogA(const char *moduleName, NCSLogLevel eLevel, const char *szFormat, ...);
#ifdef NCS_BUILD_UNICODE
void NCSLogW(const wchar_t *moduleName, NCSLogLevel eLevel, const wchar_t *szFormat, ...);
#endif
#ifdef UNICODE

#define NCSLog NCSLogW

#else

#define NCSLog NCSLogA

#endif // UNICODE

void NCSLogSetServer(BOOLEAN bValue);



void NCSLogInit(void);
void NCSLogFini(void);

#endif //SWIG
#ifdef __cplusplus 
}

#ifndef NCSMUTEX_H
#include "NCSMutex.h"
#endif // NCSMUTEX_H

#ifndef NCSTHREAD_H
#include "NCSThread.h"
#endif // NCSTHREAD_H

namespace NCS {

class NCSECW_IMPEX CLog {
public:
	//register logger has to be called first, otherwise nothing will be logged
	//only the first registration (if it is not null) is recognized and any following call will be ignored.
	static void RegisterLogger(ILogger<wchar_t>* pLogger);
	static void RegisterLogger(ILogger<char>* pLogger);

#ifdef NCS_BUILD_UNICODE
	static void Log(const wchar_t* moduleName, NCSLogLevel logLevel, const wchar_t* pFormat, ...);
	static void LogInternal(const wchar_t* moduleName, NCSLogLevel logLevel, const wchar_t* pFormat, va_list va);
	static bool IsLogLevelEnabled(const wchar_t* moduleName, NCSLogLevel logLevel);
#endif


	static void Log(const char* moduleName, NCSLogLevel logLevel, const char* pFormat, ...);
	static void LogInternal(const char* moduleName, NCSLogLevel logLevel, const char* pFormat, va_list va);
	static bool IsLogLevelEnabled(const char *moduleName, NCSLogLevel logLevel);
	static void UpdateLogConfig(const NCS::CString &sLogFile, const CRollingParameters &logRollingParams, NCSLogLevel logLevel = NCS_INFO_LEVEL);
	static void UpdateLogConfig(const NCS::CString &sLogFile, const NCS::CString & sConfigFile);
protected:
	CLog() {};
	virtual ~CLog() {};

private:
	friend class CLogFunctionTracer;
	static void Init(const CString& logFilePath, const CRollingParameters &logRollingParams, NCSLogLevel logLevel);
	static void InitWithConfig(const CString& logFilePath, const CString& configFilePath);


	static NCS::CMutex	sm_Mutex;
	static NCS::ILogger<char>* sm_logger_c;
	static NCS::ILogger<wchar_t>* sm_logger_wc;
};

class NCSECW_IMPEX CLogFunctionTracer
{
public:
	CLogFunctionTracer(const NCSTChar *moduleName, const NCSTChar *functionName);
	virtual ~CLogFunctionTracer();
private:
	NCSTimeStampMs m_startTime;
	const NCSTChar* m_moduleName;
	const NCSTChar* m_functionName;

};

}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CLog CNCSLog;
typedef NCS::CLogFunctionTracer CNCSLogFunctionTracer;
#endif

#define LOG_FUNCTION_TRACER(moduleName, functionName) \
	CNCSLogFunctionTracer funcTracer(moduleName, functionName);


#define LOGERROR0( t, e )	CLog::Log(__FILE__, __LINE__, NCS_FATAL_LEVEL, "Error: %s %s(%ld)", t, NCSGetErrorText(e), e)
#define LOGERROR1( t, e )	CLog::Log(__FILE__, __LINE__, NCS_ERROR_LEVEL, "Error: %s %s(%ld)", t, NCSGetErrorText(e), e)

#endif //__cplusplus

#endif //NCSLOG_H

