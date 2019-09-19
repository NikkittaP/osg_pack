/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	Compess.h
** CREATED:	21 May 1999
** AUTHOR: 	TIS
** PURPOSE:	Public Interface library to compress ECW v2.0 image files
** EDITS:
** [01] tis	21May99 Created file
** [02] sjc 30Apr00	Merged Mac SDK port
**
********************************************************/

#ifndef NCSECWCOMPRESS_H
#define NCSECWCOMPRESS_H

#ifndef NCSECWCOMPRESSCLIENT_H
#include "NCSECWCompressClient.h"
#endif

#if !defined(_WIN32_WCE)
	#include <time.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif


/*
**	The EcwCompressionTask is used to simply do compressions.
**
**	Usage is:
**		ecw_compress_task_new()			- to create one of these structures
**		ecw_compress_task_parse_args()	- (optional step) parse command line args into task structure
**		ecw_compress_task_setup()		- will open input, and compute size details
**		ecw_compress_task_compress()	- do the actional compression. Skip this step to just get file sizes
**		ecw_compress_task_delete()		- to close the structure and any associated data
*/

#define MAX_THREAD_LINES 10

typedef struct EcwCompressionTask {
#ifdef MACINTOSH
		NCSTChar			szTmpDir[64];
#else	/* MACINTOSH */
	NCSTChar			szTmpDir[NCS_MAX_FILE_PATH_SIZE];
#endif	/* MACINTOSH */

		// private values. Do not use or modify. You must call the task_delete routine to free
		struct QmfLevel		*pQmf;
		NCS::CIOStream		*pStream;
		void				*pClient;
		IEEE4				**ppLines;			/* For Non-Threaded reads when converting RGB to YUV */

		NCSCompressFormat	eInternalFormat;
		NCSError			(*pReadCallback)(void *pClient, UINT32 nNextLine, IEEE4 **ppInputArray);	
		void				(*pStatusCallback)(void *pClient, UINT32 nCurrentLine);
		BOOLEAN				(*pCancelCallback)(void *pClient);
} EcwCompressionTask;

#ifdef __cplusplus
}
#endif

#endif	// NCSECWCOMPRESS_H
