/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: Config.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Config header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_CONFIG_H
#define NCS_API_CONFIG_H
#ifndef SWIG
    #include "NCSDefs.h"
#else
    #define NCSECW_IMPEX
#endif
#include "ProjectionFormat.h"
#include "NCSError.h"

namespace NCS {
namespace API {

/**
 * \class	NCS::API::CConfig
 *
 * \brief	Runtime Configuration options for the ECWJP2 SDK.
 * \ingroup	simplifiedclasses
 * 
 * \author	Intergraph Perth
 * \date	8/08/2013
 */
class NCSECW_IMPEX CConfig {
public:

	/**
	 * \fn	static void CConfig::PurgeUnusedMemory();
	 *
	 * \brief	Purge unused memory.  
	 * 			
	 * 			This call will close unused internal files and delete as much memory from its internal caches as possible.  For best
	 * 			results don't call this while decoders are reading datasets.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 */
	static void PurgeUnusedMemory();

	/**
	 * \fn	static bool CConfig::GetECWTextureDitheringEnabled();
	 *
	 * \brief	Is ECW Texture Dithering enabled?
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	true if it is enabled.
	 */
	static bool GetECWTextureDitheringEnabled();

	/**
	 * \fn	static void CConfig::SetECWTextureDitheringEnabled(bool bEnable);
	 *
	 * \brief	Sets whether or not ECW Texture Dithering is enabled.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	bEnable	true to enable, false to disable.
	 */
	static void SetECWTextureDitheringEnabled(bool bEnable);

	/**
	 * \fn	static unsigned int CConfig::GetMaximumCacheSizeMB();
	 *
	 * \brief	Gets the maximum memory cache size in megabytes.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The maximum memory cache size megabytes.
	 */
	static unsigned int GetMaximumCacheSizeMB();

	/**
	 * \fn	static void CConfig::SetMaximumCacheSizeMB(unsigned int nMB);
	 *
	 * \brief	Sets the maximum memory cache size megabytes.
	 * 			
	 * 			For iOS, Android, or WinCE this should be rather low (e.g. < 100MB)
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	nMB	The maximum memory cache size in megabytes.
	 */
	static void SetMaximumCacheSizeMB(unsigned int nMB);

	/**
	 * \fn	static unsigned int CConfig::GetUnusedFilePurgeTimeoutMS();
	 *
	 * \brief	Gets unused file purge timeout.
	 * 			
	 * 			This is the timeout before an internal factory file is purged when it is not in use.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The unused file purge timeout milliseconds.
	 */
	static unsigned int GetUnusedFilePurgeTimeoutMS();

	/**
	 * \fn	static void CConfig::SetUnusedFilePurgeTimeoutMS(unsigned int nMS);
	 *
	 * \brief	Sets unused file purge timeout.
	 *
	 * 			This is the timeout before an internal factory file is purged when it is not in use.
	 * 
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	nMS	The unused file purge timeout milliseconds.
	 */
	static void SetUnusedFilePurgeTimeoutMS(unsigned int nMS);

	/**
	 * \fn	static unsigned int CConfig::GetMaxmiumProgressiveViewSize();
	 *
	 * \brief	Gets maxmium progressive view size.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The maxmium progressive view size.
	 */
	static unsigned int GetMaxmiumProgressiveViewSize();

	/**
	 * \fn	static void CConfig::SetMaxmiumProgressiveViewSize(unsigned int nPx);
	 *
	 * \brief	Sets maxmium progressive view size.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	nPx	The maxmium progressive view size.
	 */
	static void SetMaxmiumProgressiveViewSize(unsigned int nPx);

	/**
	 * \fn	static bool CConfig::GetECWPCacheEnabled();
	 *
	 * \brief	Is the ECWP disk cache enabled?
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	true if it is, false if it isn't.
	 */
	static bool GetECWPCacheEnabled();

	/**
	 * \fn	static void CConfig::SetECWPCacheEnabled(bool bEnabled);
	 *
	 * \brief	Sets whether or not the ECWP disk cache is enabled.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	bEnabled	true to enable, false to disable.
	 */
	static void SetECWPCacheEnabled(bool bEnabled);

	/**
	 * \fn	static unsigned int CConfig::GetECWPCacheSizeMB();
	 *
	 * \brief	Gets ECWP disk cache size in megabytes.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The ECWP disk cache size megabytes.
	 */
	static unsigned int GetECWPCacheSizeMB();

	/**
	 * \fn	static void CConfig::SetECWPCacheSizeMB(unsigned int nMB);
	 *
	 * \brief	Sets ECWP disk cache size megabytes.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	nMB	The ECWP disk cache size megabytes.
	 */
	static void SetECWPCacheSizeMB(unsigned int nMB);

	/**
	 * \fn	static char CConfig::*GetECWPCacheLocation();
	 *
	 * \brief	Gets ECWP disk cache location.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	null if it fails, else the ECWP disk cache location.
	 */
	static char *GetECWPCacheLocation();

	/**
	 * \fn	static void CConfig::SetECWPCacheLocation(const char *szLocation);
	 *
	 * \brief	Sets the ECWP disk cache location.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	szLocation	The location.
	 */
	static void SetECWPCacheLocation(const char *szLocation);

	/**
	 * \fn	static unsigned int CConfig::GetECWPCacheCurrentSizeMB();
	 *
	 * \brief	Gets the current ECWP disk cache current size in megabytes.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The current ECWP disk cache current size in megabytes.
	 */
	static unsigned int GetECWPCacheCurrentSizeMB();

	/**
	 * \fn	static bool CConfig::GetECWPCacheSecureURL();
	 *
	 * \brief	Is the ECWP disk cache obfuscating the ECWP url?
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	true if it is, false if it isn't.
	 */
	static bool GetECWPCacheSecureURL();

	/**
	 * \fn	static void CConfig::SetECWPCacheSecureURL(bool bSecure);
	 *
	 * \brief	Sets whether or not the ECWP URL is obfuscated.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	bSecure	true to obsuscate.
	 */
	static void SetECWPCacheSecureURL(bool bSecure);

	/**
	 * \fn	static unsigned int CConfig::GetJP2MaxFileStreams();
	 *
	 * \brief	Gets the number of streams per file the decoder will use for jp2 files.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The jp2 maximum file streams.
	 */
	static unsigned int GetJP2MaxFileStreams();

	/**
	 * \fn	static void CConfig::SetJP2MaxFileStreams(unsigned int nStreams);
	 *
	 * \brief	Sets the number of streams per file the decoder will use for jp2 files.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	nStreams	The number of streams streams.
	 */
	static void SetJP2MaxFileStreams(unsigned int nStreams);

	/**
	 * \fn	static bool CConfig::GetResilientDecoding();
	 *
	 * \brief	Gets resilient decoding flag.  
	 * 			
	 *			If false then any errors encountered while decoding will cause the SDK to return an error
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	true if it resiliant, false if it isn't.
	 */
	static bool GetResilientDecoding();

	/**
	 * \fn	static void CConfig::SetResilientDecoding(bool bSet);
	 *
	 * \brief	Sets resilient decoding.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	bSet	true to set.
	 */
	static void SetResilientDecoding(bool bSet);

	/**
	 * \fn	static unsigned int CConfig::GetECWP3MaxConcurrentDownloads();
	 *
	 * \brief	Gets the maximum number of threads that will concurrently download data in ECWP v3.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The ECWP v3 maximum concurrent downloads.
	 */
	static unsigned int GetECWP3MaxConcurrentDownloads();

	/**
	 * \fn	static void CConfig::SetECWP3MaxConcurrentDownloads(unsigned int nMax);
	 *
	 * \brief	Sets the maximum number of threads that will concurrently download data in ECWP v3.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	nMax	The ECWP v3 maximum concurrent downloads.
	 */
	static void SetECWP3MaxConcurrentDownloads(unsigned int nMax);

	/**
	 * \fn	static unsigned int CConfig::GetECWP3MaxBlocksPerDownload();
	 *
	 * \brief	Gets the maximum number of blocks an ECWP v3 thread will request at once.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The ECWP v3 maximum blocks per download. 
	 */
	static unsigned int GetECWP3MaxBlocksPerDownload();

	/**
	 * \fn	static void CConfig::SetECWP3MaxBlocksPerDownload(unsigned int nMax);
	 *
	 * \brief	Sets the maximum number of blocks an ECWP v3 thread will request at once.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nMax	The ECWP v3 maximum blocks per download.
	 */
	static void SetECWP3MaxBlocksPerDownload(unsigned int nMax);

	/**
	 * \fn	static unsigned int CConfig::GetECWP3MinBlocksPerDownload();
	 *
	 * \brief	Gets the minimum number of blocks an ECWP v3 thread will request at once.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The ECWP v3 minimum blocks per download.
	 */
	static unsigned int GetECWP3MinBlocksPerDownload();

	/**
	 * \fn	static void CConfig::SetECWP3MinBlocksPerDownload(unsigned int nMin);
	 *
	 * \brief	Sets the minimum number of blocks an ECWP v3 thread will request at once.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nMin	The ECWP v3 minimum blocks per download.
	 */
	static void SetECWP3MinBlocksPerDownload(unsigned int nMin);

	/**
	 * \fn	static unsigned int CConfig::GetECWP3ClientConnectionTimeoutMS();
	 *
	 * \brief	Gets timeout in milliseconds that an unused ECWP v3 connection will linger before being purged.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The timeout in milliseconds.
	 */
	static unsigned int GetECWP3ClientConnectionTimeoutMS();

	/**
	 * \fn	static void CConfig::SetECWP3ClientConnectionTimeoutMS(unsigned int nMS);
	 *
	 * \brief	Sets timeout in milliseconds that an unused ECWP v3 connection will linger before being purged.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nMS	The timeout in milliseconds.
	 */
	static void SetECWP3ClientConnectionTimeoutMS(unsigned int nMS);

	/**
	 * \fn	static NCSProjectionFormat CConfig::GetProjectionFormat();
	 *
	 * \brief	Gets projection format used in NCSFileInfo::szProjection, NCSFileInfo::szDatum.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The projection format.
	 */
	static NCSProjectionFormat GetProjectionFormat();

	/**
	 * \fn	static void CConfig::SetProjectionFormat(NCSProjectionFormat eProjectionFormat);
	 *
	 * \brief	Sets projection format used in NCSFileInfo::szProjection, NCSFileInfo::szDatum.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	eProjectionFormat	The projection format.
	 */
	static void SetProjectionFormat(NCSProjectionFormat eProjectionFormat);

	/**
	 * \fn	static char *CConfig::GetECWPClientUserAgent();
	 *
	 * \brief	Gets ECWP client user agent.  Do not free this string.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The ECWP client user agent. 
	 */
	static char *GetECWPClientUserAgent();

	/**
	 * \fn	static void CConfig::SetECWPClientUserAgent(const char *szUTF8);
	 *
	 * \brief	Sets ECWP client user agent.  
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	szUTF8	The UTF 8.
	 */
	static void SetECWPClientUserAgent(const char *szUTF8);

	/**
	 * \fn	static NCS::CError CConfig::SetCustomEPSGToGDTMapping(const int nEPSG,
	 * 		const char *szDatum, const char *szProjection);
	 *
	 * \brief	Sets a custom epsg to gdt mapping.  
	 * 			
	 * 			Use this in lieu of a PcskeyProjDatum.dat file.  This function can override
	 * 			internal mappings (which is based on libgeotiff), or create custom mappings
	 * 			for your private custom CRSs.
	 *
	 * \author	Intergraph Perth
	 * \date	12/08/2013
	 *
	 * \param	nEPSG			The epsg code.  Must be > 0
	 * \param	szDatum			The ER Mapper datum string.
	 * \param	szProjection	The ER Mapper projection string.
	 *
	 * \return	An error of the result.
	 */
	static NCS::CError SetCustomEPSGToGDTMapping(const int nEPSG, const char *szDatum, const char *szProjection);
	/**
	* \fn	static unsigned int CConfig::GetDefaultWaitTimeMS();
	*
	* \brief	Gets wait time in milliseconds that the SDK will block for while waiting for ECWP data to be downloaded. Default for all decoders.
	*
	* \author	Intergraph Perth
	* \date	02/04/2015
	*
	* \return	The timeout in milliseconds.
	*/
	static unsigned int GetDefaultWaitTimeMS();
	/**
	* \fn	static void CConfig::SetDefaultWaitTimeMS(unsigned int nMS);
	*
	* \brief	Sets wait time in milliseconds that the SDK will block for while waiting for ECWP data to be downloaded. Default for all decoders.
	*
	* \author	Intergraph Perth
	* \date	9/08/2013
	*
	* \param	nMS	The time in milliseconds.
	*/
	static void SetDefaultWaitTimeMS(const unsigned int nMS);
	/**
	* \fn	static unsigned int CConfig::GetDefaultRefreshTimeMS();
	*
	* \brief	Gets time in milliseconds to limit the frequency of RefreshUpdate calls in progressive views. Default for all views.
	*
	* \author	Intergraph Perth
	* \date	02/04/2015
	*
	* \return	The timeout in milliseconds.
	*/
	static unsigned int GetDefaultRefreshTimeMS();
	/**
	* \fn	static void CConfig::SetDefaultRefreshTimeMS(unsigned int nMS);
	*
	* \brief	Sets time in milliseconds to limit the frequency of RefreshUpdate calls in progressive views. Default for all views.
	*
	* \author	Intergraph Perth
	* \date	02/04/2015
	*
	* \param	nMS	The time in milliseconds.
	*/
	static void SetDefaultRefreshTimeMS(const unsigned int nMS);

    CConfig();
    virtual ~CConfig();
};

} // API
} // NCS

#endif // NCS_API_DECODER_H