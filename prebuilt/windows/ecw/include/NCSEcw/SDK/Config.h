/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	Config.h
** CREATED:	15 Dec 2006
** AUTHOR: 	Simon Cope
** PURPOSE:	Ecw stats class header for SDK instrumentation
** EDITS:
** [01] sjc	15Dec06 Created file
** 
** NOTES
**
********************************************************/

#ifndef NCSSDKCONFIG_H
#define NCSSDKCONFIG_H

#include "NCSDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace NCS {
namespace SDK {

class NCS_EXPORT CConfig {
public:
	bool	m_bForceFileReopen;				// normally FALSE - only set to TRUE for server load testing
	bool	m_bNoTextureDither;				// normally FALSE - TRUE for compression, so input compressed files have no texture dither added
	bool	m_bForceLowMemCompress;			// Force a low memory compressioin

	UINT32	m_nMaximumOpen;					// [15] Max # files open in cache
	
	unsigned int	m_nPurgeDelay;			// [16] Time delay after last purge before new purge allowed
	unsigned int	m_nFilePurgeDelay;		// [16] Time delay between last view closing and file being purged from cache
	unsigned int	m_nMinFilePurgeDelay;		// [16] Min Time delay between last view closing and file being purged from cache

	UINT32			m_nMaxOffsetCache;		// [16] Max size of offset cache for ECW files with RAW block tables.

	bool			m_bEcwpReConnect;					// [19] normally FALSE - if TRUE the ecw library will try and reconnect if connection has been lost to IWS
	bool			m_bICCManage;					// [20] normally TRUE - if FALSE the ecw library does not do ICC management on READ
	UINT32			m_nMaxFileIOCache;				// [20] JP2 file IO cache size
	UINT32			m_nMaxProgressiveViewSize;		// [21] Maximum height and width of progressive mode views
	UINT32			m_nMaxFileReads;
	UINT32			m_nMaxJP2FileStreams;

	// Time wait for network server to respond with blocks
	unsigned int	m_nBlockingTime;			// Time in ms to block clients that don't support callbacks
	unsigned int	m_nRefreshTime;				// Time in ms to wait between refresh callbacks to smart clients

	UINT64			m_nMaximumCacheSize;				// Maximum allowed size of cache

	bool			m_bEnableAutoJ2IGeneration;
	
	bool			m_bPrintPurgeInfo;

	bool			m_bOptimizeForNonProgressive;		// Setting this will optimize the SDK for non progressive mode.  Progressive mode will no longer work.

	bool			m_bOptimizeCachePurgeForRandomMode;

	bool			m_bUseNearestNeighbour;

	bool			m_bUseBufferedIOStream;

	bool			m_bResilientDecoding;

	bool			m_bEnableECWPDiskCacheForSecure;

	bool			m_bEnableECWP3Client;
	UINT32			m_nMaxECWP3BlocksToRequest;
	UINT32			m_nMinECWP3BlocksToRequest;
	UINT32			m_nMaxECWP3ConcurrentDownloads;
	UINT32			m_nECWP3ConnectionTimeout;

	UINT32			m_nTileBasedCompressionThreads;
	UINT64			m_nMaximumCompressionMemorySize;
	NCS::CString	m_sCompressionTempDir;
	bool			m_bCompressionUseSSE;
	bool			m_bCompressionStoreECWOverlap;
	bool			m_bCompressionTiledSerialiseBlocks;
	NCSProjectionFormat			m_nProjectionFormat;
	bool			m_bDeleteViewTransforms;
	NCS::CString    m_sECWPClientHTTPUserAgent;	
	const std::vector<NCS::CString> &ECWPVirtualPaths() const { return m_ECWPIISVirtualPaths; }

	// GDAL captures 20% of above the lower resolution's image size. 
	// For instance, if a lower resolution image size is 1000 x 1000, the higher level is 2000 x 2000,
	// and the view size is 1200 x 1200, decoder will read up to this lower resolution. If the view size is 1201 x 1201, decoder will read up to the higher resolution.
	// Default will be true from now (was false) to be consistant with reading via GDAL.
	bool			m_bApplyLowerResolutionCaptureRate; 


	// TRUE for prefill the whole update region for ecw region update, FALSE to initial the whole update region with the background color
	// default to be false
	bool			m_bPrefillUpdateRegionEnabled;
	 
	UINT32			m_nMaxReadCountBeforePurgeCheck;
#ifdef MULTIPLE_HEAPS
	UINT32			m_nHeapsDecoding;
	UINT32			m_nHeapsEncoding;
#endif
	CConfig();
	virtual ~CConfig();

	CError SetConfig(NCSConfigType eType, va_list va);
	CError GetConfig(NCSConfigType eType, va_list va);

protected:
	std::vector<NCS::CString> m_ECWPIISVirtualPaths;
	NCS::CMultiLock m_Mutex;
};
}
}

#ifdef __cplusplus
};
#endif

#endif	// NCSSDKCONFIG_H
