/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSTypes.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	General NCS typedefs
** EDITS:
** [01] sjc 30Apr00 Merged Mac SDK port
** [02]  ny 03Nov00 Mac port changes
** [03] tfl 02Jul04 Moved NCSCellType enum declaration from NCSEcwClient to here
 *******************************************************/

#include "NCSDefs.h"

#ifndef NCSTYPES_H
#define NCSTYPES_H

/*	Platform determination stuff.
	Should be moved to a better "central" location.
*/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ERSTYPES_H

/*
** Includes
*/

#if !defined(_WIN32_WCE)
	#include <sys/types.h>
#endif
    
#include <stdarg.h>

#if (defined(_WIN32) || defined(_WIN32_WCE))	//Pocket PC 2002

#include <windows.h>
#include <windef.h>
#include <winbase.h>

#if (_WIN32_WCE > 300) //Pocket PC 2002
#include <stddef.h>
#endif
#endif /* _WIN32 */
    

/*
** General data sizes
*/
#if (defined(_WIN32) || defined(_WIN32_WCE))	//pocket pc 2002
typedef signed char		INT8;		/*  8 bit signed integer		*/
typedef unsigned char	UINT8;		/*  8 bit unsigned integer		*/
typedef signed short	INT16;		/* 16 bit signed integer		*/
typedef unsigned short	UINT16;		/* 16 bit unsigned integer		*/
#if !defined(_BASETSD_H_)||defined(SWIG)
typedef signed int		INT32;		/* 32 bit signed integer		*/
typedef unsigned int	UINT32;		/* 32 bit unsigned integer		*/
typedef __int64 		INT64;		/* 64 bit signed integer		*/
typedef unsigned __int64 UINT64;	/* 64 bit unsigned integer		*/
typedef UINT8			BOOLEAN;	/* for use as a True/False or On/Off type */
#endif

#else	/* _WIN32 */

#ifdef POSIX

typedef signed char	INT8;		/*  8 bit signed integer		*/
typedef unsigned char	UINT8;		/*  8 bit unsigned integer		*/
typedef signed short	INT16;		/* 16 bit signed integer		*/
typedef unsigned short	UINT16;		/* 16 bit unsigned integer		*/
#ifndef _BASETSD_H_
typedef signed int	INT32;		/* 32 bit signed integer		*/
typedef unsigned int	UINT32;		/* 32 bit unsigned integer		*/
#endif
#define BYTE	UINT8

#if defined(LINUX)

#define DWORD UINT32
typedef struct tagRECT {
	long left;
	long top;
	long right;
	long bottom;
} RECT, *PRECT;

typedef int64_t		INT64;
typedef uint64_t	UINT64;

#elif defined(MACOSX)

typedef unsigned long long UINT64;
typedef signed long long INT64;
typedef int BOOLEAN;
#define DWORD UINT32
#define BYTE	UINT8
#ifndef NULL
#	define NULL 0
#endif
    
#endif //MACOSX
#if !(defined(_BASETSD_H_)||defined(_UTILITY_H)) && !(defined(MACOSX))
    typedef int BOOLEAN;		/* for use as a True/False or On/Off type */
#endif
#endif //POSIX

#endif	/* _WIN32 */

typedef float			IEEE4;		/* 4 byte IEEE floating point	*/
typedef double			IEEE8;		/* 8 byte IEEE floating point	*/
typedef time_t			DATETIME;	/* Datestamp					*/

#endif /* #ifndef ERSTYPES_H */

/*
** NCS Specific typedefs
*/
typedef UINT32			NCSBlockId;		/* Unique (per file) Block ID		*/
typedef UINT64			NCSClientUID;	/* Unique client ID (per server)	*/
typedef UINT64			NCSSequenceNr;	/* Packet sequence number			*/
typedef INT64			NCSTimeStampMs;	/* msec timestamp - wraps every 2^64 ms (10^6 years) */
typedef INT64			NCSTimeStampUs;	/* usec timestamp - wraps every 2^64 us (10^3 years) */



/*
** NCS Color typedefs : Note that these are compatible with win32 api calls for COLORREF
*/
typedef UINT32			NCSColor;

#define NCSColorRGB(r,g,b)          ((NCSColor)(((UINT8)(r)|((UINT16)((UINT8)(g))<<8))|(((UINT32)(UINT8)(b))<<16)))
#define NCSColorRGBA(r,g,b,a)		((NCSColor)(((UINT8)(r)|((UINT16)((UINT8)(g))<<8))|(((UINT32)(UINT8)(b))<<16))|(((UINT32)(UINT8)(a))<<24))
#define NCSColorGetRValue(nColor)   ((UINT8)(nColor))
#define NCSColorGetGValue(nColor)   ((UINT8)(((UINT16)(nColor)) >> 8))
#define NCSColorGetBValue(nColor)   ((UINT8)((nColor)>>16))
#define NCSColorGetAValue(nColor)	((UINT8)((nColor)>>24))

/*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*	*/

typedef enum {
	NCSCS_RAW	= 0,
	NCSCS_UTM	= 1,
	NCSCS_LL	= 2
} NCSCoordSys;

#ifdef __cplusplus

namespace NCS {

typedef enum {
	HttpMethodGet,
	HttpMethodPost,
	HttpMethodPut
} HttpMethod;

/**
 * NCSUUID - UUID class.
 * 
 * @author       Simon Cope
 * @version      $Revision: #2 $ $Author: ctapley $ $Date: 2017/12/03 $ 
 */	
class NCSECW_IMPEX UUID {
public:
	UINT8 m_UUID[16];
	
	UUID() {
		memset(m_UUID, 0, sizeof(m_UUID));
	};
	explicit UUID(const UINT8 Src[16]) {
		memcpy(m_UUID, Src, sizeof(m_UUID));
	};
	UUID(const UUID &Src) {
		*this = Src;
	};
	virtual ~UUID() {};

	const UUID &operator=(const UUID &Src) {
		memcpy(m_UUID, Src.m_UUID, sizeof(m_UUID));
		return *this;
	};
	NCS_INLINE bool operator==(const UUID &s) {
		return(memcmp(m_UUID, s.m_UUID, sizeof(m_UUID)) == 0);
	}
	NCS_INLINE bool operator!=(const UUID &s) {
		return(memcmp(m_UUID, s.m_UUID, sizeof(m_UUID)) != 0);
	}
};

	/** packet ID type */
typedef NCSBlockId PacketId;	

#ifndef SWIG
class NCSECW_IMPEX CPacketId {
public:
	CPacketId() { m_id = (NCSBlockId)-1; };
	CPacketId(const CPacketId &s) { *this = s; };
	CPacketId(const NCSBlockId &s) { *this = s; };
	NCS_INLINE CPacketId &operator=(const CPacketId &s) { m_id = s.m_id; return(*this); };
	NCS_INLINE CPacketId &operator=(const NCSBlockId &s) { m_id = s; return(*this); };
	NCS_INLINE operator NCSBlockId() const { return(m_id); };
protected:
	NCSBlockId m_id;
};
#endif // !SWIG

}

}
#endif

#endif /* #ifndef NCSTYPES_H */
