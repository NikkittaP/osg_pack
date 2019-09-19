/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: FileType.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: File type header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_FILE_TYPE_H
#define NCS_API_FILE_TYPE_H

/** 
 *	@enum NCSFileType
 *	@ingroup enums
 *  Enumerated type for file types supported by the SDK.
 */
typedef enum {
	/** Unknown file type or no file open */
	NCS_FILE_UNKNOWN	=	0,
	/** The file is an ECW */
	NCS_FILE_ECW	=	1,
	/** The file is a Jpeg 2000 File */
	NCS_FILE_JP2	=	2
} NCSFileType;

#endif // NCS_API_FILE_TYPE_H