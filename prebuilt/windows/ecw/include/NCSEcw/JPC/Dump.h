/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:    Dump.h 
** CREATED:  12/02/2003 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CDump class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSJPCDUMP_H
#define NCSJPCDUMP_H
#ifdef _DEBUG
#ifndef NCSJPCTYPES_H
#include "NCSEcw/JPC/Types/Types.h"
#endif // NCSJPCTYPES_H

#ifndef NCSLOG_H
#include "NCSLog.h"
#endif // NCSLOG_H
#ifndef NCSERROR_H
#include "NCSError.h"
#endif // NCSERROR_H

namespace NCS {
namespace SDK {
	/**
	 * CDump class - the JPC dumper class.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
	 */	
class NCSECW_IMPEX CDump: public CLog {
public:
		/** Default constructor, initialises members */
	CDump();
		/** Virtual destructor */
	virtual ~CDump();

#ifdef NCS_BUILD_UNICODE
		/** 
		 * Dump the formated varargs to stderr & log.
		 * @param		eLevel		LogLevel for dump
		 * @param		pFormat		Format string
		 * @param		...			Varargs for format string
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Dump(NCSLogLevel eLevel, wchar_t *pFormat, ...);
#endif
	/** 
		 * Dump the formated varargs to stderr & log.
		 * @param		eLevel		LogLevel for dump
		 * @param		pFormat		Format string
		 * @param		...			Varargs for format string
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Dump(NCSLogLevel eLevel, char *pFormat, ...);
private:
		/** 
		 * Dump the formated varargs to stderr & log.
		 * @param		eLevel		LogLevel for dump
		 * @param		pFormat		Format string
		 * @param		va			Varargs list
		 * @return      CError	NCS_SUCCESS, or Error code on failure.
		 */
	virtual CError Dump(NCSLogLevel eLevel, NCSTChar *pFormat, va_list va);
};
}
}

#endif // _DEBUG
#endif // !NCSJPCDUMP_H
