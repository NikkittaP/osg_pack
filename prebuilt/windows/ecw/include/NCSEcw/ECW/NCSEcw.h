/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	ncsecw.h
** CREATED:	1 March 1999
** AUTHOR: 	SNS
** PURPOSE:	Glue functions to access ECW files from the NCS system
** EDITS:
** [01] sns	01Mar99 Created file
** [02] sns 02Apr99 Not doing Mutex locks during process shutdown
** [03] sns 09May99 Using new logic for callback interface
** [04] sjc 30Dec99 Changed Sanity check to global func from static
** [05] sjc 30Mar00 Modified to use Shared Memory for stats instead of
**					  Shared dataseg in dll.  Handles backwards compatibility.
**					  Means NCSEcw.dll can be anywhere in system and still
**					  use global caching stats
** [06] sjc 30Apr00	Merged Mac SDK port
** [07] ny  12May00	Added bIsConnected field to the NCSFileStruct structure
** [08] sjc 20Jun00 Out-of-Order block fix
** [09] rdh 04Oct00 Added NCSEcwReadLineType enum type
** [10] ddi 14Nov00	Moved NCSECWReadLineType to ecw.h
** [11]  ny 23Nov00	Added #include "NCSUtil.h" so that MACINTOSH is defined
** [12]  ny 06Dec00	Change #include "NCSUtil.h" to #include "NCSTypes.h"
** [13] jmp 23Jul01 Fix where IDWT thread can become suspended after a SetView.
** [14] mjs 26Nov01 Fixed old ermapper include warnings.
** [15] sjc 04Feb02 Added MAXOPEN
** [16] sjc 19Feb02 Exposed cache purge parameters
** [17] rar 17Jun02 Changed NCScbmReadFileBlockLocal() to check if a read failed due to network error
**						and if so set a flag (pNCSFile->bFileIOError).  This was needed so that if the
**						IWS and ecw_img knows the network file must be closed and reopened.
** [18] rar 19Jun02 Added mutex around NCScbmReadFileBlockLocal() and NCScbmGetFileBlockSizeLocal() to
**						prevent the IWS and ecw_img trying to read from the same file at the same time.
** [19] rar 02Sep03 Added option to try ecwp re-connection if connection lost
** [21] tfl 20Jan06 Added option to control maximum size of progressive views in NCSEcwInfo
** 
** NOTES
**
********************************************************/

#ifndef NCSECW_H
#define NCSECW_H

#include "NCSDefs.h" /**[12]**/
#if !defined(_WIN32_WCE)
	#include <time.h>
#endif
#ifdef MACINTOSH
#include <MacTypes.h>
#include <stat.h>
#include <stdio.h>
#else	/* MACINTOSH */
#if !defined(_WIN32_WCE)
	#include <sys/types.h>
	#include <sys/stat.h>
#endif
#endif	/* MACINTOSH */

#if defined(POSIX)
	#include <sys/types.h>
	#include <sys/stat.h>
#endif

#include <stdio.h>
#include "NCSThread.h"				// we need locks and threads
#include "NCSMutex.h"
#include "NCSPackets.h"				// Server Packets
#include "NCScnet.h"				// client network layer
#include "ECW.h"
#include "NCSECWClient.h"			// [03] to get public structure information
#include "NCSEcw/SDK/ThreadPool.h"
#include "NCSEcw/SDK/Stats.h"
//#include "NCSEcw/ECW/File.h"

#include <map>

#ifdef __cplusplus
extern "C" {
#endif



// If a File View currently has a view size (window size) that
// is smaller than this number, blocks will be cached,
// otherwise the blocks will be freed as soon as they are used

#define NCSECW_MAX_VIEW_SIZE_TO_CACHE	6000
#define MCSECW_MAX_CONCURRENT_FILE_READS 4

#define NCSECW_MAX_UNUSED_CACHED_FILES	10000	// Maximum number of files that can be open and unused
#define NCSECW_CACHED_BLOCK_POOL_SIZE	1000	// initial size of cached block pointers pool
#define NCSECW_MAX_SEND_PACKET_SIZE		(8*1024)	// maximum size of a packet to send
#define NCSECW_IDWT_QUEUE_GRANULARITY	32		// realloc the setview queue every this often

// Time related
#define NCSECW_PURGE_DELAY_MS			1000	// only purge cache after at least this time has passed since last purge
// These are stored in the global shared application structure, and can be changed by users
#define NCSECW_BLOCKING_TIME_MS			10000	// wait up to 10 seconds before giving up, for blocking clients
#define NCSECW_QUIET_WAIT_TIME_MS		10000	// wait up to 10 seconds before giving up, for blocking clients
#define	NCSECW_REFRESH_TIME_MS			500		// allow 0.5 seconds to pass before issuing a new refresh if new blocks have arrived
#define NCSECW_FILE_PURGE_TIME_MS		(30*60*1000)	// Ideally keep files for up to 30 minutes when idle
#define NCSECW_FILE_MIN_PURGE_TIME_MS	(30*1000)		// never purge files in less than 30 seconds

#define NCSECW_MAX_SETVIEW_PENDING	1	// maximum number of SetViews that can be pending
#define NCSECW_MAX_SETVIEW_CANCELS	3	// maximum number of SetViews that will be cancelled,
										// before forcing completion regardless of number pending
#define NCSECW_HIGHMAX_SETVIEW_PENDING 10	// unless this many setviews are pending, in which case
											// we flush pending's regardless (if we can)
#define NCSECW_MAX_OFFSET_CACHE		1024	// Offset cache size for ECW files with RAW block tables

#ifdef __cplusplus
}
#endif


#endif	// NCSECW_H
