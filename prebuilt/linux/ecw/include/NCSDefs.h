/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSDefs.h
** CREATED:	Tue Mar 2 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	General NCS defines
** EDITS:
** [01] sjc 30Apr00 Merged Mac SDK port
** [02]  ny 03Nov00 Merge WinCE/PALM SDK changes
** [03] tfl 14Jul04 Added radians/degrees conversion factors
 *******************************************************/

#ifndef NCSDEFS_H
#define NCSDEFS_H

#ifdef __cplusplus


#pragma once

 // Does the compiler support C++11 noexcept?  GCC 4.7, msvc110, clang3.1 
#if (__GNUC__ && ((__GNUC__ == 4 && __GNUC_MINOR__ >=7) || (__GNUC__ > 4))) ||\
	(_MSC_VER && (_MSC_VER > 1800)) ||\
	(MACOSX && !NO_CPLUSPLUSLIB) ||\
    (IOS && !NO_CPLUSPLUSLIB)
#define NCS_NOEXCEPT noexcept
#else
#define NCS_NOEXCEPT 
#endif

#define NCS_DEFAULT_COPY_CTOR(type) \
	type(type const&) = default; 

#define NCS_DEFAULT_COPY_ASSIGN(type) \
	type& operator=(type const&) = default;

#define NCS_DEFAULT_ALL_COPY(type) \
	NCS_DEFAULT_COPY_CTOR(type) \
	NCS_DEFAULT_COPY_ASSIGN(type)

#define NCS_DEFAULT_NOEXCEPT_MOVE_CTOR(type) \
	type(type&&) NCS_NOEXCEPT = default;

#define NCS_DEFAULT_NOEXCEPT_MOVE_ASSIGN(type) \
	type& operator=(type&&) NCS_NOEXCEPT = default;

#define NCS_DEFAULT_ALL_NOEXCEPT_MOVE(type) \
	NCS_DEFAULT_NOEXCEPT_MOVE_CTOR(type) \
	NCS_DEFAULT_NOEXCEPT_MOVE_ASSIGN(type)

#define NCS_DEFAULT_MOVE_CTOR(type) \
	type(type&&) = default;

#define NCS_DEFAULT_MOVE_ASSIGN(type) \
	type& operator=(type&&) = default;

#define NCS_DEFAULT_ALL_MOVE(type) \
	NCS_DEFAULT_MOVE_CTOR(type) \
	NCS_DEFAULT_MOVE_ASSIGN(type)

#define NCS_DELETE_COPY_CTOR(type) \
	type(type const&) = delete; 

#define NCS_DELETE_COPY_ASSIGN(type) \
	type& operator=(type const&) = delete;

#define NCS_DELETE_ALL_COPY(type) \
	NCS_DELETE_COPY_CTOR(type) \
	NCS_DELETE_COPY_ASSIGN(type)

#define NCS_DELETE_MOVE_CTOR(type) \
	type(type&&) = delete;

#define NCS_DELETE_MOVE_ASSIGN(type) \
	type& operator=(type&&) = delete;

#define NCS_DELETE_ALL_MOVE(type) \
	NCS_DELETE_MOVE_CTOR(type) \
	NCS_DELETE_MOVE_ASSIGN(type)

#define NCS_DELETE_ALL_COPY_AND_MOVE(type) \
	NCS_DELETE_ALL_MOVE(type) \
	NCS_DELETE_ALL_COPY(type)

#define NCS_DEFAULT_ALL_COPY_AND_MOVE(type) \
	NCS_DEFAULT_ALL_MOVE(type) \
	NCS_DEFAULT_ALL_COPY(type)


extern "C" {
#endif
/*********************************************************
 **	NCSECW Cache management values
**********************************************************/
    
#define ECWP_CACHE

#if (defined(_MSC_VER) && _MSC_VER >= 1800) || (defined(__cplusplus) && __cplusplus > 199711L)
#define NCS_OVERRIDE override
#else
#define NCS_OVERRIDE 
#endif 


//platform detection and translation to internal defines.
#if defined (__APPLE__) && defined(__MACH__)
    //It's either MACOSX or IOS. Find it out closer.
    #define MACOSX 1
    #define POSIX 1
    #define NCS_BUILD_UNICODE 1
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR == 1 || TARGET_OS_IPHONE == 1
        #define IOS 1
        #define NCS_MOBILE 1
    #elif TARGET_OS_MAC != 1
        #error Can not detect target platform.
    #endif
    
#endif
#ifdef __ANDROID__
	#undef NCS_BUILD_UNICODE
	#define POSIX 1
	#define LINUX 1
	#define NCS_MOBILE  1
	#define NCS_ANDROID 1  //this is supposed to be an internal flag
	#define _FILE_OFFSET_BITS 64
	//android google ndk doesn't support wide character
	#define _GLIBCXX_PERMIT_BACKWARD_HASH 1
#else
   	//android google ndk doesn't support wide character
	#ifndef NCS_BUILD_UNICODE
	// Currently all targeted platforms support UNICODE.  This should currently never be undefined.
	#define NCS_BUILD_UNICODE
	#endif
#endif

#ifdef __linux
    #define LINUX 1
    #define POSIX 1
    #define _FILE_OFFSET_BITS 64
#endif

#ifdef _WIN32_WCE
	#define NCS_MOBILE
#endif

#if defined (NCS_MOBILE)
    #define ECWP2_DISABLED
#endif
    
#if defined(POSIX) || (defined(_MSC_VER) && _MSC_VER > 1500 && !defined(_WIN32_WCE))
    #ifndef __STDC_LIMIT_MACROS
        #define __STDC_LIMIT_MACROS
    #endif
    #include <stdint.h>
#else
    
    #ifndef INT16_MIN
        #define INT16_MIN        (-32767i16 - 1)
    #endif
    
    #ifndef INT32_MIN
        #define INT32_MIN        (-2147483647i32 - 1)
    #endif
    
    #ifndef INT64_MIN
        #define INT64_MIN        (-9223372036854775807i64 - 1)
    #endif
    
    #ifndef INT16_MAX
        #define INT16_MAX        32767i16
    #endif
    
    #ifndef INT32_MAX
        #define INT32_MAX        2147483647i32
    #endif
    
    #ifndef INT64_MAX
        #define INT64_MAX        9223372036854775807i64
    #endif
#endif 
    
#ifdef _WIN32
    #include <windows.h>
    #define NCSWCHAR_IS_UTF16 1
	#ifndef _WIN32_WCE
		#define MULTIPLE_HEAPS
	#endif
#else
    #include <wchar.h>
    #include <wctype.h>
#endif

#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifdef POSIX
#ifndef NULL
#define NULL 0
#endif 

#endif

#ifndef X86
    #if defined(_WIN32) && (defined(_M_IX86) || defined(_M_X64)) // windows
        #define X86
    #elif defined(__GNUC__) && (defined(__i386__) || defined(__amd64__)) // gcc
        #define X86
    #elif defined(MACOSX) && (TARGET_CPU_X86 ==1 || TARGET_CPU_X86_64 ==1)
        #define X86
    #endif
#endif // X86

//TODO: we need to detect endianness properly for platforms other than MAC.


#if defined(SOLARIS) || TARGET_RT_BIG_ENDIAN == 1
#define NCSBO_MSBFIRST
#elif ((defined(_WIN32) || defined(LINUX)) && defined(X86)) || (defined(MACOSX) && TARGET_RT_LITTLE_ENDIAN == 1) || defined(_WIN32_WCE)
#define NCSBO_LSBFIRST
#elif defined (__ANDROID__)
	#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
		#define NCSBO_LSBFIRST
	#elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
		#define NCSBO_MSBFIRST
	#else
		#error "Endianness is not supported."
	#endif
#else
#error Endianness not defined for platform
#endif

#if !defined(_WIN32_WCE)&&!defined(UNALIGNED)
#define UNALIGNED
#endif	/* !_WIN32_WCE */

#ifndef MAX
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifdef _WIN32
    #define _USE_MATH_DEFINES
    #include <math.h>
    
    #ifdef _WIN64
        #define NCS_64BIT
    #endif
    #ifndef MAXDOUBLE
        #define MAXDOUBLE   DBL_MAX			//1.7976931348623158e+308
        #define NCS_FQNAN	_FPCLASS_QNAN	//0x0002
        #define NCS_NAN		_FPCLASS_SNAN
    #endif	/* !MAXDOUBLE */

    #if defined(_WIN32_WCE)
        #define NCS_NO_UNALIGNED_ACCESS
        #ifndef MB_TASKMODAL
            #define MB_TASKMODAL MB_APPLMODAL
        #endif
    #endif // _WIN32_WCE

#elif defined MACOSX

    #include <limits.h>
    #include <ctype.h>
    #include <math.h>
    #if TARGET_RT_64_BIT == 1
        #define NCS_64BIT
    #endif
    #define NCS_FQNAN	0x0002
    #define NCS_NAN		NAN
    #ifndef MAXDOUBLE
        #define MAXDOUBLE   DBL_MAX
    #endif
    //#define NCS_NO_UNALIGNED_ACCESS - this seems to be defined on Linux??

#elif defined SOLARIS || defined LINUX

    #ifdef _AMD64_
    #define NCS_64BIT
    #endif
#if !defined(NCS_ANDROID)
    #include <values.h>
#endif
    #include <limits.h>
    #include <ctype.h>
    #include <math.h>
    #ifndef MAXDOUBLE
    #define MAXDOUBLE   DBL_MAX			//1.7976931348623158e+308
    #endif	/* !MAXDOUBLE */

    #ifdef LINUX
        #define NCS_FQNAN	NAN	//0x0002
        #define NCS_NAN		NAN
    #elif defined(SOLARIS)
        #include <ieeefp.h>
        #define NCS_FQNAN	FP_QNAN
        #define NCS_NAN	FP_QNAN
    #else
        #error NAN not defined
    #endif

    #if defined(SOLARIS)
        // SPARC has slow BYTE bit ops
        #define NCS_SLOW_CPU_BYTE_OPS
    #endif

#else

#error DEFINE SYSTEM INCLUDES FOR TYPES

#endif	/* _WIN32 */

#ifndef M_PI
#define M_PI    3.14159265358979323846
#endif
    
#undef Deg2Rad
#undef Rad2Deg
#define Deg2Rad(x) (x * M_PI / 180.0)
#define Rad2Deg(x) (x * 180.0 / M_PI)

/*Coodinate system defines*/
#define NCS_LINEAR_COORD_SYS	"linear"
#define	NCS_FEET_FACTOR			0.30480061
/*[03]*/
#define NCS_RADIANS_TO_DEGREES	57.29577951L
#define NCS_DEGREES_TO_RADIANS	1.745329252E-2L	

#if defined(_WIN32)

    #ifdef NCS_MANAGED_BUILD

    #define NCS_EXPORT
    #define NCS_IMPORT
    #define NCS_GC __gc
    #define NCS_VALUE __value
    #define NCS_PUBLIC public
    #define NCS_PROTECTED NCS_PUBLIC
    #define NCS_PRIVATE NCS_PUBLIC
    #ifdef __cplusplus
    #using <mscorlib.dll>
    #endif //__cplusplus

    #else // _MANAGED

    #if defined(_LIB)||defined(NCSECW_STATIC_LIBS)||defined(SWIG)
    #define NCS_EXPORT
    #define NCS_IMPORT
    #else
    #define NCS_EXPORT __declspec(dllexport)
    #define NCS_IMPORT __declspec(dllimport)
    #endif


    #define NCS_GC
    #define NCS_VALUE
    #define NCS_PUBLIC public
    #define NCS_PROTECTED protected
    #define NCS_PRIVATE private

    #endif // _MANAGED

    #ifdef _WIN32_WCE
    #define NCS_CALL __cdecl
    #else
    #define NCS_CALL __cdecl
    #endif

    #define NCS_CB_CALL __cdecl

#else // _WIN32

#define NCS_EXPORT
#define NCS_IMPORT
#define NCS_GC
#define NCS_VALUE
#define NCS_PUBLIC public
#define NCS_PROTECTED protected
#define NCS_PRIVATE private

#define NCS_CALL
#define NCS_CB_CALL
#endif // _WIN32

#ifndef NCSECW_IMPEX
#ifdef NCSECW_EXPORTS
  #define NCSECW_IMPEX    NCS_EXPORT
#else
  #define NCSECW_IMPEX    NCS_IMPORT
#endif
#endif

#ifndef MAX_PATH
    #if defined SOLARIS || defined LINUX
        #ifdef PATH_MAX
        #define MAX_PATH	PATH_MAX
        #else
        #define MAX_PATH	1024
        #endif
    #elif defined MACOSX
        #define MAX_PATH 1024
    #else
        #define MAX_PATH	PATHNAMELEN
    #endif
#endif	/* !MAX_PATH */

#define NCSIsNullString(s) ((s) == (char *)0 || (*(s)) == '\0')
#define NCSIsNullStringW(s) ((s) == (wchar_t *)0 || (*(s)) == '\0')

#if	__ICL >= 700
#ifdef NCS_VECTOR_CC
//Note: need /QaxMiKW /Qvec_report3 /Qrestrict ICC flags to use vectorisation
#define NCS_RESTRICT restrict
#else
#define NCS_RESTRICT
#endif
#else
#define NCS_RESTRICT
#endif

#ifdef _OPENMP
#define NCS_OPENMP
#endif // _OPENMP

#ifdef NCS_OPENMP
#define NCS_VECTOR_CC
#endif // NCS_OPENMP

#ifndef NCS_INLINE
#ifdef SWIG
#define NCS_INLINE 
#elif _WIN32_WCE
#define NCS_BUILD_UNICODE
#define BOOST_NO_STD_LOCALE
#define NCS_INLINE __inline
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#elif _WIN32
#define NCS_INLINE __forceinline
#elif defined __GNUC__
#define NCS_INLINE __inline__
#elif __SUNPRO_CC
#define NCS_INLINE inline
#else
#define NCS_INLINE __inline
#endif // _WIN32
#endif // NCS_INLINE

#if defined(_WIN32) && !defined(NCSECW_STATIC_LIBS)
   #define CONDITIONAL_FORCEINLINE  __forceinline
#else
   #define CONDITIONAL_FORCEINLINE  
#endif

#include "NCSTypes.h"

/*
	Secure char* and wchar* copy functions. 
	When on the platform there is a secure counterpart it is used. Buffer size parameter is always taken and it is expected
	that strcpy_s functions will always be called from the SDK code and stuff using it. 
*/
#if defined (SECURE_FUNCTIONS_CPY_CAT) || !defined(SECURE_FUNCTIONS_CPY_CAT)
	#if defined(_WIN32) || (defined(POSIX) && defined(USE_SLIBC) && !defined(MACOSX))
		//basically do nothing but for completeness - define it. 
		#define strcpy_s		strcpy_s
		#define strncpy_s		strncpy_s
		#define strcat_s		strcat_s
		#define strncat_s		strncat_s

		#define wcscpy_s		wcscpy_s
		#define wcsncpy_s		wcsncpy_s
		#define wcscat_s		wcscat_s
		#define wcsncat_s		wcsncat_s

	#endif
	#if defined(MACOSX)
		//MACOSX has secure strlcpy and strlcat
		#define strcpy_s(strDestination,numberOfElements, strSource) strlcpy(strDestination, strSource, numberOfElements)
		#define strncpy_s(strDestination,numberOfElements, strSource, count) strlcpy(strDestination, strSource, numberOfElements)
		#define strcat_s(strDestination, numberOfElements, strSource) strlcat(strDestination, strSource, numberOfElements)
		#define strncat_s(strDestination, numberOfElements, strSource, count) strlcat(strDestination, strSource, numberOfElements)

		#define wcscpy_s(strDestination,numberOfElements, strSource) wcslcpy(strDestination, strSource, numberOfElements)
		#define wcsncpy_s(strDestination,numberOfElements, strSource, count) wcslcpy(strDestination, strSource, numberOfElements)
		#define wcscat_s(strDestination, numberOfElements, strSource) wcslcat(strDestination, strSource, numberOfElements)
		#define wcsncat_s(strDestination, numberOfElements, strSource, count) wcslcat(strDestination, strSource, numberOfElements)
	#endif
	#if (defined (POSIX) && !defined(USE_SLIBC) && !defined(MACOSX)) || defined(NCS_ANDROID)
		//no secure strcpy and strcat just ignore buffer parameter. 
		#define strcpy_s(strDestination,numberOfElements, strSource) strcpy(strDestination, strSource)
		#define strncpy_s(strDestination,numberOfElements, strSource, count) strncpy(strDestination, strSource, count)
		#define strcat_s(strDestination, numberOfElements, strSource) strcat(strDestination, strSource)
		#define strncat_s(strDestination, numberOfElements, strSource, count) strncat(strDestination, strSource, count)

		#define wcscpy_s(strDestination,numberOfElements, strSource) wcscpy(strDestination, strSource)
		#define wcsncpy_s(strDestination,numberOfElements, strSource, count) wcsncpy(strDestination, strSource, count)
		#define wcscat_s(strDestination, numberOfElements, strSource) wcscat(strDestination, strSource)
		#define wcsncat_s(strDestination, numberOfElements, strSource, count) wcsncat(strDestination, strSource, count)
	#endif
#endif
/*
	Secure char* and wchar* *printf* family of  functions.
	When on the platform there is a secure counterpart it is used. Buffer size parameter is always taken and it is expected
	that *printf* functions will always be called from the SDK code and stuff using it.
*/
#if defined (SECURE_FUNCTIONS_PRINTF) || !defined(SECURE_FUNCTIONS_PRINTF)
	#if (defined(_WIN32) && !defined(_WIN32_WCE)) || (defined(POSIX) && defined(USE_SLIBC))
		#define swprintf_s		swprintf_s
		#define vswprintf_s		vswprintf_s		
		#define sprintf_s		sprintf_s
		#define snprintf_s		_snprintf_s
		
		#define snwprintf_s		_snwprintf_s
		#define vsprintf_s		vsprintf_s
		
		#define vsnprintf_s		vsnprintf_s
		#define vsnwprintf_s	_vsnwprintf_s
		#define fscanf_s		fscanf_s
		#define sscanf_s		sscanf_s
		#define swscanf_s		swscanf_s
	#else //UNSECURE functions
	//TODO: null termination needs to be implemented manually. 
		#define swprintf_s		swprintf
		#if defined (_WIN32_WCE)
			#define sprintf_s		_snprintf
			#define vswprintf_s		vswprintf	
			#define snprintf_s(buffer,sizeOfBuffer,count,format,...)		_snprintf(buffer,count,format,__VA_ARGS__)
			#define snwprintf_s(buffer,sizeOfBuffer,count,format,...)		_snwprintf(buffer,count,format,__VA_ARGS__)
			#define vsnprintf_s(buffer,sizeOfBuffer,count,format,argptr)	_vsnprintf(buffer,count,format,argptr)
			#define vsnwprintf_s(buffer,sizeOfBuffer,count,format,argptr)	_vsnwprintf(buffer,count,format,argptr)
			#define vsprintf_s		_vsnprintf			
		#else
			#define sprintf_s		snprintf
			#define snprintf_s(buffer,sizeOfBuffer,count,format,...)		snprintf(buffer,count,format,__VA_ARGS__)
			#define snwprintf_s(buffer,sizeOfBuffer,count,format,...)		snwprintf(buffer,count,format,__VA_ARGS__)
			#define vsnprintf_s(buffer,sizeOfBuffer,count,format,argptr)	vsnprintf(buffer,count,format,argptr)
			#define vsnwprintf_s(buffer,sizeOfBuffer,count,format,argptr)	vswprintf(buffer,count,format,argptr)
			#define vsprintf_s		vsnprintf
		#endif
		#define fscanf_s		fscanf
		#define sscanf_s		sscanf
		#define swscanf_s		swscanf
	#endif
	
#endif
/*
	Secure char* and wchar* misc functions.
	When on the platform there is a secure counterpart it is used. Buffer size parameter is always taken and it is expected
	that misc functions will always be called from the SDK code and stuff using it.
*/
#if defined (SECURE_FUNCTIONS_MISC) || !defined(SECURE_FUNCTION_MISC)
	//functions common to POSIX (when not using SLIBS) and WINCE. 
	#if (defined(_WIN32_WCE) || (defined (POSIX) && !defined(USE_SLIBS)))
		
		typedef int errno_t;
		errno_t NCSInternalstrlwr_s(char *buffer, size_t size);
		errno_t NCSInternalwcslwr_s(wchar_t *buffer, size_t size);

		#define strlwr_s	NCSInternalstrlwr_s
		#define wcslwr_s	NCSInternalwcslwr_s
	#endif

	#if defined(_WIN32)  || (defined(POSIX) && defined(USE_SLIBC))
		
		#define gcvt_s			_gcvt_s
		#define mktemp_s		_mktemp_s
		
		#if defined(_WIN32_WCE)
			typedef int errno_t;
			errno_t NCSInternalfopen_s(FILE** file, const char *fileName, const char *mode);
			errno_t NCSInternalwfopen_s(FILE** file, const wchar_t *fileName, const wchar_t *mode);
			errno_t NCSInternalwcstombs_s(size_t *pReturnValue, char *mbstr, size_t sizeInBytes, const wchar_t *wcstr, size_t count);
			errno_t NCSInternalmbstowcs_s(size_t *pReturnValue, wchar_t *wcstr, size_t sizeInWords, const char *mbstr, size_t count);
			#define wcstombs_s      NCSInternalwcstombs_s
			#define mbstowcs_s      NCSInternalmbstowcs_s
			#define fopen_s         NCSInternalfopen_s
			#define wfopen_s		NCSInternalwfopen_s
			#define gmtime_s(tm,t)	_gmtime64_s(tm,(__time64_t *)t)
			#define ctime_s			_ctime_s
		#else 
			#define wfopen_s		_wfopen_s
			#define wcstombs_s		wcstombs_s
			#define mbstowcs_s		mbstowcs_s
			#define fopen_s			fopen_s
			#define gmtime_s		gmtime_s
			#define ctime_s			ctime_s
			//WIN
			#define strlwr_s		_strlwr_s
			#define wcslwr_s		_wcslwr_s
		#endif
		#define strtok_s		strtok_s
		#define wcstok_s		wcstok_s
		#define NCSputenv		_putenv
		#define NCSgetenv		getenv_s
		
	#else
		
		errno_t NCSInternalmktemp_s(char *buffer, size_t bufferSize);
		errno_t NCSInternalfopen_s(FILE** file, const char *fileName, const char *mode);
		errno_t NCSInternalwcstombs_s(size_t *pReturnValue, char *mbstr, size_t sizeInBytes, const wchar_t *wcstr, size_t count);
		errno_t NCSInternalmbstowcs_s(size_t *pReturnValue, wchar_t *wcstr, size_t sizeInWords, const char *mbstr, size_t count);
		errno_t NCSInternalgmtime_s(struct tm* _tm, const time_t* time);
		errno_t NCSInternalctime_s(char* buffer,size_t numberOfElements,const time_t *time);
		errno_t NCSInternalgetenv_s(size_t *pReturnValue, char* buffer,size_t numberOfElements,const char *varname);
		
		#define mktemp_s		NCSInternalmktemp_s
		#define fopen_s         NCSInternalfopen_s
		#define wcstombs_s      NCSInternalwcstombs_s
		#define mbstowcs_s      NCSInternalmbstowcs_s

		#define gmtime_s        NCSInternalgmtime_s
		#define ctime_s         NCSInternalctime_s
		#define strtok_s		strtok_r
		#define wcstok_s		wcstok

		#define NCSputenv		putenv
		#define NCSgetenv		NCSInternalgetenv_s

	#endif
#endif
/*
	Substitute POSIX names to use ISO C++ names. Currently only on Windows. 
*/
#define SUBSTITUTE_POSIX_NAMES
#ifdef SUBSTITUTE_POSIX_NAMES
#ifdef _WIN32

	#define NCSstricmp		_stricmp
	#define NCSstrnicmp		_strnicmp
	#define NCSstrcmp		strcmp
	#define NCSstrncmp		strncmp

	#define NCSwcsnicmp		_wcsnicmp
	#define NCSwcscmp		wcscmp
	#define NCSwcsncmp		wcsncmp
	#define NCSwcsicmp		_wcsicmp


	#define wcstoll		_wcstoi64
	#define strtoll		_strtoi64
	#define wcsdup		_wcsdup
	#define strdup		_strdup

#elif defined POSIX 

	#define NCSstricmp			strcasecmp
	#define NCSstrnicmp			strncasecmp
	#define NCSstrcmp			strcmp
	#define NCSstrncmp			strncmp
		
	#define NCSwcsicmp			wcscasecmp
	#define NCSwcsnicmp			wcsncasecmp
	#define NCSwcscmp			wcscmp
	#define NCSwcsncmp			wcsncmp

	#define wctoll wcstoll
	#define strtoll strtoll

	#define wcsdup wcsdup
	#define strdup strdup
 
#endif /* _WIN32 */

#endif //SUBSTITUTE_POSIX_NAMES 

/*
 * Unicode/ASCII agnostic macros
 * These reimplement the Win32 "tchar" concept in a platform independent way.  
 * Just enough for what we need, extend as necessary.  Use in lieu of <tchar.h>
 * in any code that requires Unicode support.
 */
    
#ifndef NCS_TCHAR
#define NCS_TCHAR
#define NCSToul(x,y,z) strtoul(x,y,z)

#ifdef NCS_BUILD_UNICODE

#ifndef SWIG
#define NCSWCharArrSize(arr) sizeof(arr)/sizeof(wchar_t)

#define NCSTChar wchar_t

#define NCS_T(x) L ## x
#define NCSTCharArrSize(arr) sizeof(arr)/sizeof(NCSTChar)
#define NCSTNCpy(x,xl,y,z) wcsncpy_s(x,xl,y,z)
#define NCSTCpy(x,xl,y) wcscpy_s(x,xl,y)
#define NCSTCat(x,xl,y) wcscat_s(x,xl,y)
#define NCSTCCmp(x,y) NCSwcscmp(x,y) //case sensitive comparison
#define NCSTCmp(x,y) NCSwcsicmp(x,y) //case insensitive comparison
#define NCSTNCmp(x,y,z) NCSwcsnicmp(x, y, z)
#define NCSTPrintf swprintf_s
#define NCSTScanf swscanf_s

#define NCSTLen(x) wcslen(x)

#define NCSTTol(x,y,z) wcstol(x,y,z)
#define NCSTTod(x,y) wcstod(x,y)
#define NCSwcstoul wcstoul(x, y, z)
#define NCSTToul(x,y,z) NCSwcstoul(x,y,z)
#define NCSTToll(x,y,z) wcstoll(x,y,z)
#define NCSTLwr wcslwr_s
#define NCSTStr wcsstr
#define NCSTDup(x) NCSStrDupT(x)

		
#ifdef _WIN32
#ifdef _WIN32_WCE
#define NCSWinGPA_T NCS_T
#else
#define NCSWinGPA_T(x) x
#endif
#endif


#if !defined(MACOSX)
#include <malloc.h>
#endif
#include <stdio.h>

static NCS_INLINE wchar_t *NCSA2WHelper(wchar_t *lpw, const char *lpa, int nChars)
{
	if (!lpw) {

		return lpw; // NULL. the caller may use NCSChar2WChar instead

	}

	lpw[0] = '\0';
#ifdef _WIN32
	MultiByteToWideChar(CP_ACP, 0, (LPSTR)lpa, -1, (LPWSTR)lpw, nChars);
#else
	mbstowcs(lpw, lpa, nChars);
#endif
	return lpw;
}

static NCS_INLINE char *NCSW2AHelper(char *lpa, const wchar_t *lpw, int nChars)
{
	if (!lpa) {
        return NULL;
	}
	lpa[0] = '\0';
#ifdef _WIN32
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)lpw, -1, (LPSTR)lpa, nChars, NULL, NULL);
#else
	wcstombs(lpa, lpw, nChars);
#endif
	return lpa;
}


#define OS_STRING(lpa)		(((lpa) == NULL) ? NULL : NCSA2WHelper((wchar_t*) alloca(((int)strlen(lpa)+1)*2*sizeof(wchar_t)), (lpa), ((int)strlen(lpa)+1)))
#define CHAR_STRING(lpw)	(((lpw) == NULL) ? NULL : NCSW2AHelper((char*) alloca(((int)wcslen(lpw)+1)*2), (lpw), ((int)wcslen(lpw)+1)*2))


#endif //SWIG

#else	/* NCS_BUILD_UNICODE */

/* Define agnostic macros to provide ASCII character support */
#define NCSCharArrSize(arr) sizeof(arr)
#define NCSTCharArrSize(arr) sizeof(arr)
#define NCSTCpy(x,xl,y) strcpy_s(x,xl,y)
#define NCSTNCpy(x,xl,y,z) strncpy_s(x,xl,y,z)


#define NCSTChar char
#define NCS_T 
#define NCSTCCmp(x,y) NCSstrcmp(x,y) //case sensitive comparison
#define NCSTCmp(x,y) NCSstricmp(x,y) //case insensitive comparison
#define NCSTNCmp(x,y,z) NCSstrnicmp(x, y, z)
#define NCSTPrintf snprintf
#define NCSTScanf sscanf
#define NCSTCat(x,xl,y) strcat_s(x,xl,y)
#define NCSTLen(x) strlen(x)

#define NCSTTol(x,y,z) strtol(x,y,z)
#define NCSTTod(x,y) strtod(x,y)
#define NCSTToul(x,y,z) NCSstrtoul(x,y,z)
#define NCSTToll(x,y,z) strtoll(x,y,z)
#define NCSTDup(x)	NCSStrDup(x)
#define NCSTLwr strlwr_s
#define NCSTStr strsstr

#ifdef _WIN32

#define NCSWinGPA_T(x) x
#endif

#define OS_STRING(lpa)		(lpa)
#define CHAR_STRING(lpw)	(char *)(lpw)

#endif	/* NCS_BUILD_UNICODE */

#endif /* NCS_TCHAR */

// NCSCompilerBarrier
#if defined(WIN32)

#if defined(__INTEL_COMPILER)
#define NCSCompilerBarrier() __memory_barrier()
#elif defined(_WIN32_WCE)
#include "Cmnintrin.h"
#define NCSCompilerBarrier() _ReadWriteBarrier()
#elif defined(_MSC_VER) && _MSC_VER >= 1310 && !defined(_WIN32_WCE)

// not used internally and _ReadWriteBarrier is now deprecated in _MSC_VER >= 1700
#define NCSCompilerBarrier()

#else
#define NCSCompilerBarrier()
#endif

#elif defined(__GNUC__)

#define NCSCompilerBarrier() __asm__ __volatile__ ("" ::: "memory")

#elif defined(__MACOSX)

#define NCSCompilerBarrier() OSMemoryBarrier()

#else 

#define NCSCompilerBarrier()

#endif



#if defined(_WIN32)&&!defined(_WIN32_WCE)&&((defined(_M_IX86) && defined(_MSC_FULL_VER) && (_MSC_FULL_VER >= 12008804))||(defined(_M_X64) && defined(_MSC_VER) && _MSC_VER >= 1400))

#define _DVEC_H_INCLUDED
#define _FVEC_H_INCLUDED
#define _IVEC_H_INCLUDED
#ifndef __INTRIN_H_
    #include <intrin.h>
#endif
//
// X86 "Multi Media Intrinsics" - ie, MMX, SSE, SSE2 optimisations
//
// Only defined if Win32, X86 and VC6 Processor Pack or newer _OR_ AMD64 and >= VS.NET 2005/AMD64 PlatformSDK compiler (ie, SSE intrinsics support in the compiler)
//
#define NCS_X86_MMI

#ifndef _M_X64
#define NCS_X86_MMI_MMX
#endif

#if (_MSC_VER >= 1500 || (__GNUC__ == 4 && __GNUC_MINOR__ >=3) || (__GNUC__ > 4) || __INTEL_COMPILER >= 1000)
    //FIXME MJS compiled out SSE code as its never worked
    //#define NCS_X86_MMI_SSSE3
    #define NCS_X86_MMI_SSE41
#endif

#if (_MSC_VER >= 1600 || (__GNUC__ == 4 && __GNUC_MINOR__ >=6) || (__GNUC__ > 4) || __INTEL_COMPILER >= 1100)
    #define NCS_X86_MMI_AVX
#endif
#if (_MSC_VER >= 1700 || (__GNUC__ == 4 && __GNUC_MINOR__ >=7) || (__GNUC__ > 4) || __INTEL_COMPILER >= 1300)
    #define NCS_X86_MMI_AVX2
#endif

#endif


#if defined(__ARM_NEON__)
    #include <arm_neon.h>
    #define NCS_ARM_NEON
#endif
	
#if defined (POSIX) && defined(X86) && ! (defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR == 1)
    #if !defined(NCS_ANDROID)  // for x86 builds of android.
    
        #include <x86intrin.h>
        #define NCS_X86_MMI
        #define NCS_X86_MMI_MMX
        #if !defined(LINUX) //On Linux we disable SSE4.1 because the compiler will generate code for SSE4.1 even for parts that we don't want if we enable it.
            #define NCS_X86_MMI_SSE41
        #endif
    #endif
#endif
#ifdef _WIN32_WCE
    #include <cmnintrin.h>
#endif

#if defined(_WIN32_WCE) && defined(_M_ARM)
	#define NCS_UNALIGNED __unaligned //Assume we have __unaligned support for WinCE. this could be wrong...
#endif



#ifdef __cplusplus
}
#endif
#ifdef __GNUC__
#define NCS_DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define NCS_DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define NCS_DEPRECATED(func) func
#endif

#endif /* NCSDEFS_H */
