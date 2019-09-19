/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSMisc.h
** CREATED:	Wed Oct 13 09:19:00 WST 1999
** AUTHOR: 	David Hayward
** PURPOSE:	Miscellaneous prototypes of useful functions
**			for the public SDKs.
**
**			NOTE: Must be kept in sync with the private
**				  includes.
**
** EDITS:
** [01] 	08-12-05 tfl lint fixes
 *******************************************************/

#ifndef NCSMISC_H
#define NCSMISC_H

#ifdef __cplusplus
extern "C" {
#endif
#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif

#ifndef SWIG
#if !defined(_MSC_VER)
#include <fenv.h>
#define USE_STANDARD_C99_FENV
#endif

#ifdef _WIN32_WCE
#define NCSMin MIN
#define NCSMax MAX
#elif defined POSIX || defined NOMINMAX
#define NCSMin(a, b)   ((a) > (b) ? (b) : (a))
#define NCSMax(a, b)   ((a) < (b) ? (b) : (a))
#else
#define NCSMin min
#define NCSMax max
#endif // _WIN32_WCE


typedef enum {
	NCS_UNKNOWN	= 0,	/* Don't know what this platform is 	*/
	NCS_WINDOWS_9X	= 1,	/* Windows 9x (95, 98)			*/
	NCS_WINDOWS_NT	= 2,	/* Windows NT (NT4, 2000, 2003, 2008R1,R2, Vista, 7) */
	NCS_WINDOWS_CE	= 3,	/* Windows CE (CE, PocketPC)		*/
	NCS_MACINTOSH	= 4,	/* Macintosh (Sys 8/9)			*/
	NCS_MACINTOSH_OSX= 5,	/* Macintosh OSX			*/
	NCS_LINUX	= 6,	/* Linux				*/
	NCS_PALM	= 7,	/* PalmOS (v2+)				*/
	NCS_SOLARIS	= 8,	/* Solaris 2.5+				*/
	NCS_HPUX	= 9	/* HP-UX 11.0(64bit)			*/
} NCSPlatform;

void NCSFormatSizeText(INT64 nSizeBytes, char *buf, size_t bufSize);
NCSPlatform NCSGetPlatform(void);

#ifdef _WIN32

typedef enum {
	NCS_WINVER_00,
	NCS_WINVER_5X,
	NCS_WINVER_60,
	/* Windows 7 or Windows Server 2008 R2*/
	NCS_WINVER_61, 
	/* Windows 8 or Windows Server 2012*/
	NCS_WINVER_62,
	/* Windows 8.1 or Windows Server 2012 R2*/
	NCS_WINVER_63

} NCSWinVer;

NCSWinVer NCSGetWindowsVersion(void);

#endif


/*
**	Fast Float to UINT8 conversion logic
*/
#if defined(USE_STANDARD_C99_FENV)
#define NCS_FLT_ROUNDING_MODE_NEAREST FE_TONEAREST
#define NCS_FLT_ROUNDING_MODE_DOWN FE_DOWNWARD
#define NCS_FLT_ROUNDING_MODE_GET_ROUNDMODE(CURRENT_CONTROL) CURRENT_CONTROL = fegetround()
#define NCS_FLT_ROUNDING_MODE_INIT(ROUNDING_MODE) { int old_controlfp_val = fegetround(); fesetround(ROUNDING_MODE);
#define NCS_FLT_ROUNDING_MODE_FINI() fesetround(old_controlfp_val); }
#else
#define NCS_FLT_ROUNDING_MODE_NEAREST _RC_NEAR
#define NCS_FLT_ROUNDING_MODE_DOWN _RC_DOWN
#define NCS_FLT_ROUNDING_MODE_GET_ROUNDMODE(CURRENT_CONTROL) _controlfp_s(&CURRENT_CONTROL, 0, 0); CURRENT_CONTROL &= _MCW_RC;
#define NCS_FLT_ROUNDING_MODE_INIT(ROUNDING_MODE) { unsigned int old_controlfp_val; _controlfp_s(&old_controlfp_val, ROUNDING_MODE, _MCW_RC)
#define NCS_FLT_ROUNDING_MODE_FINI() _controlfp_s(&old_controlfp_val, _CW_DEFAULT, _MCW_RC); }
#endif


#ifndef NCS_MOBILE
#define FLT_TO_UINT8(a, b)								\
	   { a = (UINT8)_mm_cvtss_si32(_mm_load_ss(&b)); }
#define FLT_TO_INT32(a,b)								\
	   { a = _mm_cvtss_si32(_mm_load_ss(&b)); }
#else
//asm ("VCVTR.S32.F32 s0,s0 $1, %0": "=r" (dst) : "r" (src));
#define FLT_TO_UINT8(a, b)								\
    { a = (UINT8)lrint(b); }
#define FLT_TO_INT32(a,b)								\
    { a = (UINT32)lrint(b); }
#endif

static NCS_INLINE INT32 NCSfloatToInt32_RM(IEEE4 f) {
	INT32 i32;
	FLT_TO_INT32(i32, f);
	return(i32);
}

static NCS_INLINE INT32 NCSdoubleToInt32_RM(IEEE8 x) {
	return(NCSfloatToInt32_RM((IEEE4)x));
}

// Convert a float between 0.0 and 1.0 to an INT32
static NCS_INLINE INT32 NCSfloatToInt32_0_1(IEEE4 x)
{
    IEEE4 y = x + 1.f;
    return((*(INT32 *)&y) & 0x7FFFFF);	// last 23 bits
}

// Convert a float to an INT32
static NCS_INLINE INT32 NCSfloatToInt32(IEEE4 x)
{
	INT32 FltInt = *(INT32 *)&x;
	INT32 MyInt;
	INT32 mantissa = (FltInt & 0x07fffff) | 0x800000;
	INT32 exponent = 150 - ((FltInt >> 23) & 0xff);

	if (exponent < -(8*(int)sizeof(mantissa)-1)) {
		MyInt = (mantissa << (8*(int)sizeof(mantissa)-1));		      
	} else if(exponent < 0) {
		MyInt = (mantissa << -exponent);		      
	} else if(exponent > (8*(int)sizeof(mantissa)-1)) {
		MyInt = (mantissa >> (8*(int)sizeof(mantissa)-1));
	} else {
		MyInt = (mantissa >> exponent);
	}

	if (FltInt & 0x80000000)
		MyInt = -MyInt;
	return(MyInt);
}

// Convert a double to an INT32
static NCS_INLINE INT32 NCSdoubleToInt32(IEEE8 x)
{
	INT64 DblInt = *(INT64 *)&x;
	INT32 MyInt;
	INT64 mantissa = (DblInt & 0xfffffffffffff) | 0x10000000000000;
	INT32 exponent = (INT32)(1075 - ((DblInt >> 52) & 0x7ff));

	if (exponent < -(8*(int)sizeof(mantissa)-1)) {
		MyInt = (INT32)(mantissa << (8*(int)sizeof(mantissa)-1));		      
	} else if(exponent < 0) {
		MyInt = (INT32)(mantissa << -exponent);		      
	} else if(exponent > (8*(int)sizeof(mantissa)-1)) {
		MyInt = (INT32)(mantissa >> (8*(int)sizeof(mantissa)-1));
	} else {
		MyInt = (INT32)(mantissa >> exponent);
	}

	if (DblInt & 0x8000000000000000)
		MyInt = -MyInt;
	return(MyInt);
}

#if defined(WIN32) && !defined(_WIN64)
#include <float.h>
#elif defined(__GNUC__) && !defined(__x86_64__) && !defined(NCS_MOBILE)
#include <fpu_control.h>
#endif



#if defined(WIN32) && !defined(_WIN64)
#define NCS_CONTROL_FP_SINGLE_PRECISION_START	unsigned int old_controlfp_val; _controlfp_s(&old_controlfp_val, _PC_24, MCW_PC);
#elif defined(__GNUC__) && !defined(__x86_64__) && !defined(NCS_MOBILE)
#define NCS_CONTROL_FP_SINGLE_PRECISION_START	fpu_control_t fpu_oldcw, fpu_newcw;\
												_FPU_GETCW(fpu_oldcw);\
												fpu_newcw = (fpu_oldcw & ~_FPU_EXTENDED & ~_FPU_DOUBLE) | _FPU_SINGLE;\
												_FPU_SETCW(fpu_newcw);
#else
#define NCS_CONTROL_FP_SINGLE_PRECISION_START
#endif

#if defined(WIN32) && !defined(_WIN64)
#define NCS_CONTROL_FP_SINGLE_PRECISION_END		unsigned int _; _controlfp_s(&_,old_controlfp_val, MCW_PC);
#elif defined(__GNUC__) && !defined(__x86_64__) && !defined(NCS_MOBILE)
#define NCS_CONTROL_FP_SINGLE_PRECISION_END		_FPU_SETCW(fpu_oldcw);
#else 
#define NCS_CONTROL_FP_SINGLE_PRECISION_END
#endif

#define NCSfloatToInt64_RM(f) ((INT64)(f))
#define NCSfloatToInt64_0_1(x) ((INT64)(x))
#define NCSdoubleToInt64_RM(x) ((INT64)(x))
#define NCSfloatToInt64(x) ((INT64)(x))
#define NCSdoubleToInt64(x) ((INT64)(x))

// Get the 23bit mantissa from a float
static NCS_INLINE UINT32 NCSGetIEEE4Mantissa(IEEE4 x)
{
	union { IEEE4 fV; UINT32 nV; } FltInt = {x};
	return(FltInt.nV & 0x07fffff);
}

// Get the 8bit exponent from a float
static NCS_INLINE UINT8 NCSGetIEEE4Exponent(IEEE4 x)
{
	union { IEEE4 fV; UINT32 nV; } FltInt = {x};
	return((UINT8)((FltInt.nV >> 23) & 0xff));
}

// Get the 1bit sign from a float
static NCS_INLINE UINT8 NCSGetIEEE4Sign(IEEE4 x)
{
	union { IEEE4 fV; UINT32 nV; } FltInt = {x};
	return((UINT8)(FltInt.nV >> 31));
}

// Create a Float from a 23bit mantissa, 8bit exponent and 1bit sign.
static NCS_INLINE IEEE4 NCSGetIEEE4(UINT32 Mantissa, UINT8 Exponent, UINT8 Sign)
{
	union { IEEE4 fV; UINT32 nV; } FltInt;
	FltInt.nV = Mantissa | (Exponent << 23) | (Sign << 31);
	return(FltInt.fV);
}

// Get the 52bit mantissa from a float
static NCS_INLINE UINT64 NCSGetIEEE8Mantissa(IEEE8 x)
{
	union { IEEE8 dV; UINT64 nV; } FltInt = {x};
	return(FltInt.nV & 0xfffffffffffffLL);
}

// Get the 11bit exponent from a float
static NCS_INLINE UINT16 NCSGetIEEE8Exponent(IEEE8 x)
{
	union { IEEE8 dV; UINT64 nV; } FltInt = {x};
	return((UINT16)((FltInt.nV >> 52) & 0x7ff));
}

// Get the 1bit sign from a float
static NCS_INLINE UINT8 NCSGetIEEE8Sign(IEEE8 x)
{
	union { IEEE8 dV; UINT64 nV; } FltInt = {x};
	return((UINT8)(FltInt.nV >> 63));
}

// Create a Float from a 52bit mantissa, 11bit exponent and 1bit sign.
static NCS_INLINE IEEE8 NCSGetIEEE8(UINT64 Mantissa, UINT16 Exponent, UINT8 Sign)
{
	union { IEEE8 dV; UINT64 nV; } FltInt;
	FltInt.nV = Mantissa | (((UINT64)Exponent) << 52) | (((UINT64)Sign) << 63);
	return(FltInt.dV);
}

static NCS_INLINE INT32 NCSCeil(double a)
{
	if(a >= 0.0) {
		INT32 v = NCSdoubleToInt32(a);
		return(v + ((a != v) ? 1 : 0));
	} else {
		return(NCSdoubleToInt32(a));
	}
}

static NCS_INLINE INT64 NCSCeil64(double a)
{
	if(a >= 0.0) {
		INT64 v = NCSdoubleToInt64(a);
		return(v + ((a != v) ? 1 : 0));
	} else {
		return(NCSdoubleToInt64(a));
	}
}

static NCS_INLINE INT32 NCSFloor(double a)
{
	if(a >= 0.0) {
		return(NCSdoubleToInt32(a));
	} else {
		INT32 v = NCSdoubleToInt32(a);
		return(v - ((a != v) ? 1 : 0));		
	}
}

static NCS_INLINE INT64 NCSFloor64(double a)
{
	if(a >= 0.0) {
		return(NCSdoubleToInt64(a));
	} else {
		INT64 v = NCSdoubleToInt64(a);
		return(v - ((a != v) ? 1 : 0));		
	}
}

static NCS_INLINE INT32 NCSCeilDiv(INT32 n, INT32 d)
{
	if(d == 0) {
		return(0x7fffffff);
	} else if(n >= 0 && d > 0) {
		return((n / d + ((n % d) ? 1 : 0)));
	} else {
		return(n / d);
	}
}

static NCS_INLINE INT64 NCSCeilDiv64(INT64 n, INT64 d)
{
	if(d == 0) {
		return(0x7fffffffffffffffLL);
	} else if(n >= 0 && d > 0) {
		return((n / d + ((n % d) ? 1 : 0)));
	} else {
		return(n / d);
	}
}

static NCS_INLINE INT32 NCSFloorDiv(INT32 n, INT32 d)
{
	switch(d) {
		case 1: return(n); 
			//break;
		case 2: return(n >> 1); 
			//break;
		case 4: return(n >> 2); 
			//break;
		default:
				if(n < 0 || d < 0) {
					return((INT32)NCSFloor(n / (double)d));
				} else {
					return(n / d);
				}
			//break;
	}
}

static NCS_INLINE INT64 NCSFloorDiv64(INT64 n, INT64 d)
{
	switch(d) {
		case 1: return(n); break;
		case 2: return(n >> 1); break;
		case 4: return(n >> 2); break;
		default:
				if(n < 0 || d < 0) {
					return((INT64)NCSFloor64(n / (double)d));
				} else {
					return(n / d);
				}
			break;
	}
}

static NCS_INLINE UINT32 NCS2Pow(UINT32 n)
{
//	return(pow(2, n));
	return(1 << n);
}

static NCS_INLINE UINT64 NCS2Pow64(UINT64 n)
{
	return(((UINT64)1) << n);
}

static NCS_INLINE IEEE8 NCS2PowS(INT32 n)
{
//	return(pow(2, n));
	if(n >= 0) {
		return((IEEE8)(1 << n));
	} else {
		return(1.0 / (1 << -n));
	}
}

static NCS_INLINE INT32 NCSLog2(INT32 n)
{
    INT32 nLog;
    for(nLog = 0; n > 1; nLog++) {
        n >>= 1;
    }
    return nLog;
}

static NCS_INLINE INT64 NCSLog2_64(INT64 n)
{
    INT32 nLog;
    for(nLog = 0; n > 1; nLog++) {
        n >>= 1;
    }
    return nLog;
}

#ifdef __GNUC__
    
    static NCS_INLINE int NCSClz(long unsigned int n) { return __builtin_clzl(n); };

#endif

static NCS_INLINE size_t NCSLog2_szt(size_t n)
{
#if defined(WIN32) && !defined(_WIN32_WCE)

	size_t nLog = 0;
	if(n > 0) {
#ifdef _WIN64
		_BitScanReverse64((DWORD*)&nLog, n);
#else
		_BitScanReverse((DWORD*)&nLog, n);
#endif
	}
	return nLog;
	
#elif defined(__GNUC__)

	if(n == 0) return 0;
	return (sizeof(n) * 8) - NCSClz(n) - 1;

#else

	size_t nLog;
    for(nLog = 0; n > 1; nLog++) {
        n >>= 1;
    }
	return nLog;

#endif
}

static NCS_INLINE UINT64 NCSAbs(INT64 a)
{
//	return(abs(a));
	return((a < 0) ? -a : a);
}

static NCS_INLINE BOOLEAN NCSIsPow2(UINT32 nValue) 
{
	if(NCS2Pow(NCSLog2(nValue)) == nValue) {
		return(TRUE);
	}
	return(FALSE);
}




#endif //SWIG

#ifdef __cplusplus
}
#endif
#endif /* NCSMISC_H */
