/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     NCSError.h
** CREATED:  28/08/2001 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CError class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSERROR_H
#define NCSERROR_H

#ifndef NCSLOG_H
#include "NCSLog.h"
#endif // NCSLOG_H

#ifndef NCSERRORS_H
#include "NCSErrors.h"
#endif // NCSERRORS_H

#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif // NCSTYPES_H

#ifdef SWIG

#define NCSECW_IMPEX

#else

#if defined(_MSC_VER)
#define NCS_FUNCTION __FUNCTION__
#elif defined(__GNUC__)
#define NCS_FUNCTION __FUNCTION__
#else
#error Define a suitable function macro for your compiler
#endif

#endif

namespace NCS {

/**
 * \class	CError
 *
 * \brief	Error wrapper class.
 */
class NCSECW_IMPEX CError {
public:
	/**
	 * \fn	CError::CError()
	 *
	 * \brief	Default constructor.  Initialises error to success
	 */
	CError() : m_eError(NCS_SUCCESS), m_sText(), m_pLocation((char *)"") {};

	/**
	 * \fn	CError::CError(const NCSError eError)
	 *
	 * \brief	Constructor.  Error with no message
	 *
	 * \param	eError	The error state.
	 */
	CError(const NCSError eError) : m_eError(eError), m_sText(), m_pLocation((char *)"") {};
	CError(const NCSError eError, const char *pLocation) : m_eError(eError), m_pLocation(pLocation) {};
	CError(const NCSError eError, const char *pLocation, const char *pText) : m_eError(eError), m_sText(pText), m_pLocation(pLocation) {};
	~CError();

#ifndef SWIG
	

#ifdef NCS_BUILD_UNICODE
	CError(const NCSError eError, const char *pLocation, const wchar_t *pText) :
			m_eError(eError), m_sText(pText), m_pLocation(pLocation) {};
#endif
	CError(const NCSError eError, const char *pLocation, const NCS::CString &sText) :
			m_eError(eError), m_sText(sText), m_pLocation(pLocation) {};
	CError(const CError &Error);

	

	NCS::CString GetErrorMessage() const;
	char *GetErrorMessage(char *pFormat, ...) const;
	wchar_t *GetErrorMessage(wchar_t *pFormat, ...) const;
	NCS::CString GetErrorText() const { return m_sText; }
	
	void Set(const NCSError eError, const char *pLocation, const wchar_t *pText);
	void Set(const NCSError eError, const char *pLocation, const NCS::CString &sText);
    //Creates an error from NCSError and message from TLS if exists
    static CError CreateFromNCSError(NCSError eError);
	class CContext {
	public:
		CContext(const char *szLocation, const NCS::CString &sText) : m_szLocation(szLocation), m_sText(sText) {};
#ifdef NCS_BUILD_UNICODE
		CContext(const char *szLocation, const wchar_t *wszText = NULL) : m_szLocation(szLocation), m_sText(wszText) {};
#endif
		CContext(const char *szLocation, const char *szText = NULL) : m_szLocation(szLocation), m_sText(szText) {};
		virtual ~CContext() {};

		const char *Location() const { return m_szLocation; };
		const NCS::CString &Text() const { return m_sText; };
	protected:
		const char *m_szLocation;
		NCS::CString m_sText;
	};
	const CError &operator+=(const CError::CContext &sContext);

	void Log(NCSLogLevel eLevel = NCS_ERROR_LEVEL, const NCSTChar* szModuleName = MOD_GENERIC_ERROR);

	CError& operator =(const CError &Error);
	operator const NCSError&() { return(m_eError); }

	inline bool operator ==( const CError &Error ) {
			return(m_eError == Error.m_eError);
		};
	inline bool operator ==( const NCSError eError ) {
			return(m_eError == eError);
		};
	inline bool operator !=( const CError &Error ) {
			return(m_eError != Error.m_eError);
		};
	inline bool operator !=( const NCSError eError ) {
			return(m_eError != eError);
		};
#endif

	void Set(const NCSError eError, const char *pLocation);
	void Set(const NCSError eError, const char *pLocation, const char *pText);


	char *GetErrorMessageReport() const;
	NCSError GetErrorNumber(void) const { return(m_eError); }
	const char *GetErrorLocation() const { return m_pLocation; }
	bool Success() const {
		return (m_eError == NCS_SUCCESS);
	}

private:

#ifndef SWIG
	NCSError		m_eError;
	NCS::CString	m_sText;
	const char		*m_pLocation;
#endif
};

}
#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CError CNCSError;
#endif


#define NCSERROR(e)				NCS::CError(e, NCS_FUNCTION)
#define NCSERRORLOG(Error, e)	{\
									Error = NCS::CError(e, NCS_FUNCTION);\
									Error.Log(NCS_FATAL_LEVEL);\
								}
#define NCSERRORTXT(e, t)		NCS::CError(e, NCS_FUNCTION, t)
#define NCSERRORLOGTXT(Error, e, t)	{\
									Error = NCS::CError(e, NCS_FUNCTION, t);\
									Error.Log(NCS_FATAL_LEVEL);\
								}
#endif // NCSERROR_H
