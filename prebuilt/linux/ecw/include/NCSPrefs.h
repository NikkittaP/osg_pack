/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSPrefs.h
** CREATED:	Fri Jun  4 18:05:04 WST 1999
** AUTHOR: 	Mark Sheridan
** PURPOSE:	NCS Preferences header.
** EDITS:
** [01]	 ny	31Oct00	Merge WinCE/PALM SDK changes
** [02] rar 10Jun04 Re-written using C++ and to use an XML file on non-windows platforms
 *******************************************************/

#ifndef NCSPREFS_H
#define NCSPREFS_H
#include "NCSUtil.h"
#ifdef POSIX
	#define NO_REGISTRY
#endif



#ifdef __cplusplus
extern "C" {
#endif

#include "NCSTypes.h"
#include "NCSErrors.h"
#include "NCSMutex.h"
#include "NCSMalloc.h"

#ifdef POSIX
#	define NCSPREF_DEFAULT_BASE_KEY "Image Web Server"
#	define NCSPREF_DEFAULT_BASE_KEY_T NCS_T("Image Web Server")
#else
#	define NCSPREF_DEFAULT_BASE_KEY "Software\\ERDAS\\Image Web Server"
#	define NCSPREF_DEFAULT_BASE_KEY_T NCS_T("Software\\ERDAS\\Image Web Server")
#endif

/* 
**
** Utility functions - create delete machine or user level keys
**
*/

// These initialization function are called by NCSUtilInit and NCSUtilFini repectively.
// They most only be called once.
void NCSPrefInit(void);
void NCSPrefFini(void);

// The return value from these function must always be checked.
// If NCS_SUCCESS is returned than there must be a subsequent
// call to the UnLock() once access to the key has finnished.
// If any of the Get/Set functions are used without first setting
// the Key than the default key (NCSPREF_DEFAULT_BASE_KEY) will
// be used.
NCSError NCSPrefCreateUserKeyLockA(const char *pKeyName );
NCSError NCSPrefCreateMachineKeyLockA(const char *pKeyName );
NCSError NCSPrefSetMachineKeyLockA(const char *pKeyName );
NCSError NCSPrefSetUserKeyLockA(const char *pKeyName );
NCSError NCSPrefSetMachineKeyLockROA(const char *pKeyName );
NCSError NCSPrefSetUserKeyLockROA(const char *pKeyName );

#ifdef NCS_BUILD_UNICODE
NCSError NCSPrefCreateUserKeyLockW(const wchar_t *pKeyName );
NCSError NCSPrefCreateMachineKeyLockW(const wchar_t *pKeyName );
NCSError NCSPrefSetMachineKeyLockW(const wchar_t *pKeyName );
NCSError NCSPrefSetUserKeyLockW(const wchar_t *pKeyName );
NCSError NCSPrefSetMachineKeyLockROW(const wchar_t *pKeyName );
NCSError NCSPrefSetUserKeyLockROW(const wchar_t *pKeyName );
#endif

#ifdef UNICODE

#define NCSPrefCreateUserKeyLock NCSPrefCreateUserKeyLockW
#define NCSPrefCreateMachineKeyLock NCSPrefCreateMachineKeyLockW
#define NCSPrefSetMachineKeyLock NCSPrefSetMachineKeyLockW
#define NCSPrefSetUserKeyLock NCSPrefSetUserKeyLockW
#define NCSPrefSetMachineKeyLockRO NCSPrefSetMachineKeyLockROW
#define NCSPrefSetUserKeyLockRO NCSPrefSetUserKeyLockROW

#else

#define NCSPrefCreateUserKeyLock NCSPrefCreateUserKeyLockA
#define NCSPrefCreateMachineKeyLock NCSPrefCreateMachineKeyLockA
#define NCSPrefSetMachineKeyLock NCSPrefSetMachineKeyLockA
#define NCSPrefSetUserKeyLock NCSPrefSetUserKeyLockA
#define NCSPrefSetMachineKeyLockRO NCSPrefSetMachineKeyLockROA
#define NCSPrefSetUserKeyLockRO NCSPrefSetUserKeyLockROA

#endif // UNICODE

void NCSPrefMachineUnLock();
void NCSPrefUserUnLock();

/* 
**
** Machine/Global level preference functions
** Base key is HKEY_LOCAL_MACHINE
**
*/
NCSError NCSPrefGetStringA(const char *pKeyName, char **pString );
NCSError NCSPrefSetStringA(const char *pKeyName, const char *pString );
NCSError NCSPrefGetDoubleA(const char *pKeyName, IEEE8 *pValue );
NCSError NCSPrefSetDoubleA(const char *pKeyName, IEEE8 dValue );
NCSError NCSPrefGetIntA(const char *pKeyName, INT32 *pValue );
NCSError NCSPrefSetIntA(const char *pKeyName, INT32 nValue);
NCSError NCSPrefSetBooleanA(const char *pKeyname, BOOLEAN bValue );
NCSError NCSPrefGetBooleanA(const char *pKeyName, BOOLEAN *pValue );
NCSError NCSPrefDeleteValueA(const char *pKeyName);

#ifdef NCS_BUILD_UNICODE
NCSError NCSPrefGetStringW(const wchar_t *pKeyName, char **pString );
NCSError NCSPrefSetStringW(const wchar_t *pKeyName, const char *pString );
NCSError NCSPrefGetDoubleW(const wchar_t *pKeyName, IEEE8 *pValue );
NCSError NCSPrefSetDoubleW(const wchar_t *pKeyName, IEEE8 dValue );
NCSError NCSPrefGetIntW(const wchar_t *pKeyName, INT32 *pValue );
NCSError NCSPrefSetIntW(const wchar_t *pKeyName, INT32 nValue);
NCSError NCSPrefSetBooleanW(const wchar_t *pKeyname, BOOLEAN bValue );
NCSError NCSPrefGetBooleanW(const wchar_t *pKeyName, BOOLEAN *pValue );
NCSError NCSPrefDeleteValueW(const wchar_t *pKeyName);
#endif

#ifdef UNICODE

#define NCSPrefGetString NCSPrefGetStringW
#define NCSPrefSetString NCSPrefSetStringW
#define NCSPrefGetDouble NCSPrefGetDoubleW
#define NCSPrefSetDouble NCSPrefSetDoubleW
#define NCSPrefGetInt NCSPrefGetIntW
#define NCSPrefSetInt NCSPrefSetIntW
#define NCSPrefSetBoolean NCSPrefSetBooleanW
#define NCSPrefGetBoolean NCSPrefGetBooleanW
#define NCSPrefDeleteValue NCSPrefDeleteValueW

#else

#define NCSPrefGetString NCSPrefGetStringA
#define NCSPrefSetString NCSPrefSetStringA
#define NCSPrefGetDouble NCSPrefGetDoubleA
#define NCSPrefSetDouble NCSPrefSetDoubleA
#define NCSPrefGetInt NCSPrefGetIntA
#define NCSPrefSetInt NCSPrefSetIntA
#define NCSPrefSetBoolean NCSPrefSetBooleanA
#define NCSPrefGetBoolean NCSPrefGetBooleanA
#define NCSPrefDeleteValue NCSPrefDeleteValueA

#endif // UNICODE

/* 
**
** User level preference functions
** Base key is HKEY_CURRENT_USER
**
*/
NCSError NCSPrefGetUserStringA(const char *pKeyName, char **pString );
NCSError NCSPrefSetUserStringA(const char *pKeyName, const char *pString );
NCSError NCSPrefGetUserDoubleA(const char *pKeyName, IEEE8 *pValue );
NCSError NCSPrefSetUserDoubleA(const char *pKeyName, IEEE8 dValue );
NCSError NCSPrefGetUserIntA(const char *pKeyName, INT32 *pValue );
NCSError NCSPrefSetUserIntA(const char *pKeyName, INT32 nValue );
NCSError NCSPrefSetUserBooleanA(const char *pKeyname, BOOLEAN bValue );
NCSError NCSPrefGetUserBooleanA(const char *pKeyName, BOOLEAN *pValue );
NCSError NCSPrefDeleteValueUserA(const char *pKeyName);

#ifdef NCS_BUILD_UNICODE
NCSError NCSPrefGetUserStringW(const wchar_t *pKeyName, char **pString );
NCSError NCSPrefSetUserStringW(const wchar_t *pKeyName, const char *pString );
NCSError NCSPrefGetUserDoubleW(const wchar_t *pKeyName, IEEE8 *pValue );
NCSError NCSPrefSetUserDoubleW(const wchar_t *pKeyName, IEEE8 dValue );
NCSError NCSPrefGetUserIntW(const wchar_t *pKeyName, INT32 *pValue );
NCSError NCSPrefSetUserIntW(const wchar_t *pKeyName, INT32 nValue );
NCSError NCSPrefSetUserBooleanW(const wchar_t *pKeyname, BOOLEAN bValue );
NCSError NCSPrefGetUserBooleanW(const wchar_t *pKeyName, BOOLEAN *pValue );
NCSError NCSPrefDeleteValueUserW(const wchar_t *pKeyName);
#endif

#ifdef UNICODE

#define NCSPrefGetUserString NCSPrefGetUserStringW
#define NCSPrefSetUserString NCSPrefSetUserStringW
#define NCSPrefGetUserDouble NCSPrefGetUserDoubleW
#define NCSPrefSetUserDouble NCSPrefSetUserDoubleW
#define NCSPrefGetUserInt NCSPrefGetUserIntW
#define NCSPrefSetUserInt NCSPrefSetUserIntW
#define NCSPrefSetUserBoolean NCSPrefSetUserBooleanW
#define NCSPrefGetUserBoolean NCSPrefGetUserBooleanW
#define NCSPrefDeleteValueUser NCSPrefDeleteValueUserW

#else

#define NCSPrefGetUserString NCSPrefGetUserStringA
#define NCSPrefSetUserString NCSPrefSetUserStringA
#define NCSPrefGetUserDouble NCSPrefGetUserDoubleA
#define NCSPrefSetUserDouble NCSPrefSetUserDoubleA
#define NCSPrefGetUserInt NCSPrefGetUserIntA
#define NCSPrefSetUserInt NCSPrefSetUserIntA
#define NCSPrefSetUserBoolean NCSPrefSetUserBooleanA
#define NCSPrefGetUserBoolean NCSPrefGetUserBooleanA
#define NCSPrefDeleteValueUser NCSPrefDeleteValueUserA

#endif // UNICODE

/* 
**
** New preference functions
**
*/
#define MACHINE_KEY TRUE
#define USER_KEY FALSE

NCSError NCSPrefGetStringExA(BOOLEAN bIsMachine, const char *pKeyName, const char *pValueName, char **pString );
NCSError NCSPrefSetStringExA(BOOLEAN bIsMachine, const char *pKeyName, const char *pValueName, const char *pString );
NCSError NCSPrefGetDoubleExA(BOOLEAN bIsMachine, const char *pKeyName, const char *pValueName, IEEE8 *pValue );
NCSError NCSPrefSetDoubleExA(BOOLEAN bIsMachine, const char *pKeyName, const char *pValueName, IEEE8 dValue );
NCSError NCSPrefGetIntExA(BOOLEAN bIsMachine, const char *pKeyName, const char *pValueName, INT32 *pValue );
NCSError NCSPrefSetIntExA(BOOLEAN bIsMachine, const char *pKeyName, const char *pValueName, INT32 nValue );
NCSError NCSPrefSetBooleanExA(BOOLEAN bIsMachine, const char *pKeyName, const char *pValueName, BOOLEAN bValue );
NCSError NCSPrefGetBooleanExA(BOOLEAN bIsMachine, const char *pKeyName, const char *pValueName, BOOLEAN *pValue );
NCSError NCSPrefDeleteValueExA(BOOLEAN bIsMachine, const char *pKeyName, const char *pValueName );

#ifdef NCS_BUILD_UNICODE
NCSError NCSPrefGetStringExW(BOOLEAN bIsMachine, const wchar_t *pKeyName, const wchar_t *pValueName, char **pString );
NCSError NCSPrefSetStringExW(BOOLEAN bIsMachine, const wchar_t *pKeyName, const wchar_t *pValueName, const char *pString );
NCSError NCSPrefGetDoubleExW(BOOLEAN bIsMachine, const wchar_t *pKeyName, const wchar_t *pValueName, IEEE8 *pValue );
NCSError NCSPrefSetDoubleExW(BOOLEAN bIsMachine, const wchar_t *pKeyName, const wchar_t *pValueName, IEEE8 dValue );
NCSError NCSPrefGetIntExW(BOOLEAN bIsMachine, const wchar_t *pKeyName, const wchar_t *pValueName, INT32 *pValue );
NCSError NCSPrefSetIntExW(BOOLEAN bIsMachine, const wchar_t *pKeyName, const wchar_t *pValueName, INT32 nValue );
NCSError NCSPrefSetBooleanExW(BOOLEAN bIsMachine, const wchar_t *pKeyName, const wchar_t *pValueName, BOOLEAN bValue );
NCSError NCSPrefGetBooleanExW(BOOLEAN bIsMachine, const wchar_t *pKeyName, const wchar_t *pValueName, BOOLEAN *pValue );
NCSError NCSPrefDeleteValueExW(BOOLEAN bIsMachine, const wchar_t *pKeyName, const wchar_t *pValueName );
#endif

#ifdef UNICODE

#define NCSPrefGetStringEx NCSPrefGetStringExW
#define NCSPrefSetStringEx NCSPrefSetStringExW
#define NCSPrefGetDoubleEx NCSPrefGetDoubleExW
#define NCSPrefSetDoubleEx NCSPrefSetDoubleExW
#define NCSPrefGetIntEx NCSPrefGetIntExW
#define NCSPrefSetIntEx NCSPrefSetIntExW
#define NCSPrefSetBooleanEx NCSPrefSetBooleanExW
#define NCSPrefGetBooleanEx NCSPrefGetBooleanExW
#define NCSPrefDeleteValueEx NCSPrefDeleteValueExW

#else

#define NCSPrefGetStringEx NCSPrefGetStringExA
#define NCSPrefSetStringEx NCSPrefSetStringExA
#define NCSPrefGetDoubleEx NCSPrefGetDoubleExA
#define NCSPrefSetDoubleEx NCSPrefSetDoubleExA
#define NCSPrefGetIntEx NCSPrefGetIntExA
#define NCSPrefSetIntEx NCSPrefSetIntExA
#define NCSPrefSetBooleanEx NCSPrefSetBooleanExA
#define NCSPrefGetBooleanEx NCSPrefGetBooleanExA
#define NCSPrefDeleteValueEx NCSPrefDeleteValueExA

#endif // UNICODE

#ifdef __cplusplus
}

#ifndef NCSUTIL_MIN_LINK

#include <iostream>
#include "NCSString.h"
#include "NCSError.h"

/*
 * A C++ interface for NCSPrefs
 * The C functions above are wrappers around this class.
 * 
 */
namespace NCS {

class NCSECW_IMPEX CPrefs : public CMutex
{
protected:
	CPrefs(){};
	virtual ~CPrefs(){};

public:
	class NCSECW_IMPEX CKey
	{
	protected:
		CKey(CMutex *pmMutex, const CString &sName);
	public:
		virtual ~CKey();

		virtual bool Get( const CString &sName, CString &sValue, const CString &sDefault=NCS::CString()) = 0;
		virtual bool Set( const CString &sName, const CString &sValue ) = 0;
		virtual bool Get( const CString &sName, int &nValue, int nDefault=0 ) = 0;
		virtual bool Set( const CString &sName, int nValue ) = 0;
		virtual bool Get( const CString &sName, double &dValue, double dDefault=0.0 ) = 0;
		virtual bool Set( const CString &sName, double dValue ) = 0;
		virtual bool Get( const CString &sName, bool &bValue, bool bDefault=false ) = 0;
		virtual bool Set( const CString &sName, bool bValue ) = 0;

		virtual bool DeleteValue( const CString &sName ) = 0;
		virtual bool DeleteSubKey( const CString &sSubKey, bool bSubKeys=false ) = 0;

		virtual CKey *OpenSubKey( const CString &sSubKey, bool bCreate = false, bool bReadOnly=false ) = 0;

		virtual CKey *GetFirstSubKey() = 0;
		virtual CKey *GetNextSubKey() = 0;

		virtual CString &GetKeyName();

	protected:
		CMutex *m_pmMutex;
		CString m_sName;
	};

	// The user is responsible for deleting the CKey instance returned by OpenKey.
	// Also access to the Prefs will be Lock to the current thread untill it is deleted.
	virtual CKey *OpenKey( const CString &sBaseKey = NCS::CString(NCSPREF_DEFAULT_BASE_KEY), bool bCreate=false, bool bReadOnly=false ) = 0;

	virtual bool DeleteKey( const CString &sKey ) = 0;

	static CPrefs* GetMachinePrefs();
	static CPrefs* GetUserPrefs();

	static void Fini();

protected:
	static CPrefs *s_pMachinePrefs;
	static CPrefs *s_pUserPrefs;

};

}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CPrefs CNCSPrefs;
#endif


#endif //NCSUTIL_MIN_LINK

#endif //__cplusplus

#endif /* NCSPREFS_H */

