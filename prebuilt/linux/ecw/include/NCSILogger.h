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


#ifndef NCS_ILOGGER_H
#define NCS_ILOGGER_H

#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif // NCSTYPES_H

#ifdef __cplusplus 
extern "C" {
#endif
#ifndef SWIG

/* supported logging level*/
typedef enum
{
	NCS_OFF_LEVEL = 60000, 
	NCS_FATAL_LEVEL = 50000,
	NCS_ERROR_LEVEL = 40000,
	NCS_WARN_LEVEL = 30000,
	NCS_INFO_LEVEL = 20000,
	NCS_DEBUG_LEVEL = 10000,
	NCS_TRACE_LEVEL = 0,
	NCS_ALL_LEVEL = NCS_TRACE_LEVEL,
	NCS_NOT_SET_LEVEL = -1
} NCSLogLevel;


#endif //SWIG
#ifdef __cplusplus
}


#ifndef NCSSTRING_H
#include "NCSString.h"
#endif // NCSSTRING_H

namespace NCS {

/* log rolling parameters. This is due to some of logging library such as log4cplus supports rolling */ 
class CRollingParameters {
	public:
		typedef enum {
			ROLLING_NONE,         //no rolling
			ROLLING_SIZE,		  //rolls when the current logfile exceeds a predefined size
			ROLLING_MONTHLY,      //roll monthly
			ROLLING_WEEKLY,		  //roll weekly
			ROLLING_DAILY,		  //roll daily
			ROLLING_TWICE_DAILY,  //roll twice daily
			ROLLING_HOURLY,		  //roll hourly
			ROLLING_MINUTELY	  //roll minutely
		} RollingType;

		/* rolling type */
		RollingType m_eRollingType;
		/* Maximum file size beyond which rolling happens when rolling type is set to ROLLING_SIZE */
		int m_nMaxFileSize;
		/* Maximum backup logfile that can be kept when rolling happens */
		int m_nMaxBackupIndex;

		CRollingParameters(RollingType eRollingType, int nMaxFileSize, int nMaxBackupIndex):
			m_eRollingType(eRollingType), m_nMaxFileSize(nMaxFileSize), m_nMaxBackupIndex(nMaxBackupIndex)
		{
		};
		CRollingParameters() : m_eRollingType(ROLLING_NONE), m_nMaxFileSize(0), m_nMaxBackupIndex(0) {};
		CRollingParameters(const CRollingParameters &s) {
			(*this) = s;
		};
		virtual ~CRollingParameters() {};

		const CRollingParameters &operator=(const CRollingParameters &s) {
			m_eRollingType = s.m_eRollingType;
			m_nMaxFileSize = s.m_nMaxFileSize;
			m_nMaxBackupIndex = s.m_nMaxBackupIndex;
			return(*this);
		};

		bool operator==(const CRollingParameters &s) {
			return(m_eRollingType == s.m_eRollingType && m_nMaxFileSize == s.m_nMaxFileSize &&
				   m_nMaxBackupIndex == s.m_nMaxBackupIndex);
		}
	};
class ILoggerBase{
protected: 
	bool m_hasBeenInitialised;
public : 
	ILoggerBase():m_hasBeenInitialised(false){

	}
	/**
	* Determine whether the logger has been initialised or not.
	* @return       bool		true for yes, false for no
	*/
	bool HasBeenInitialised() { return m_hasBeenInitialised; };
};
/*
 * Description: generic logger interface for client application to implement 
 */
template<class TCharType> class ILogger: public virtual ILoggerBase {
public:
	/**
	* Initialise the logger with direct parameters passed in. It can be called multiple times.
	* @note: set the m_hasBeenInitialised to true after successfully setting the logger
	* @param		logFilePath		full path of the log file
	* @param		logRollingParams		rolling paramters
	* @param		NCSLogLevel		log level
	*/
	virtual void Init(const CString& logFilePath, const CRollingParameters &logRollingParams, NCSLogLevel logLevel) = 0;
	/**
	* Initialise the logger with a configuration file. Some of logging library support complicated logging 
	* @note: set the m_hasBeenInitialised to true after successfully setting the logger
	* such as setting loglevel per module. All the configuration will be set using the configuration file
	* @param		logFilePath		full path of the log file
	* @param		configFilePath	full path of the logging configuration file
	*/
	virtual void InitWithConfig(const CString& logFilePath, const CString& configFilePath) = 0;
	/**
	* Determine whether the content need to be emitted to the logfile by given the modulename and loglevel
	* @param		moduleName		module name
	* @param		logLevel	loglevel
	* @return       bool		true for yes, false for no
	*/
	virtual bool IsLogLevelEnabled(const TCharType* moduleName, NCSLogLevel logLevel) = 0;
	/**
	* Log the message at the specific module against specific level
	* @param		moduleName		module name
	* @param		logLevel	loglevel
	* @message		message	    message to log
	*/
	virtual void Log(const TCharType* moduleName, NCSLogLevel logLevel, const TCharType* message) = 0;
	ILogger() :ILoggerBase(){
	
	}
	virtual ~ILogger() {};
private:
	ILogger(const ILogger<TCharType>& logger);
	ILogger<TCharType>& operator=(const ILogger<TCharType>& logger);
	
};

} // NCS

#endif //__cplusplus

#endif //NCSLOG_H

