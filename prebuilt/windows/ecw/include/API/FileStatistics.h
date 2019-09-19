/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: FileStatistics.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: File statistics header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_FILE_STATISTICS_H
#define NCS_API_FILE_STATISTICS_H
#ifndef SWIG
    #include "NCSDefs.h"
#else
    #define NCSECW_IMPEX
#endif
#include "BandStatistics.h"
#include "Error.h"

typedef struct NCSFileStatistics NCSFileStatistics;

namespace NCS {
namespace API {
        
/**
* @class	NCS::API::CFileStatistics
*
* @brief	Statistics for a dataset.
*
* @author	Intergraph Perth
* @date	8/08/2013
*/
class NCSECW_IMPEX CFileStatistics {
public:
	CFileStatistics();
	virtual ~CFileStatistics();
            
	CFileStatistics(const CFileStatistics &s);

	/**
	 * @fn	NCSError CFileStatistics::Init(int nBands, int *nHistogramBucketCounts);
	 *
	 * @brief	Initialises this object.
	 *
	 * @author	Intergraph Perth
	 * @date	19/02/2014
	 *
	 * @param	nBands						  	The number of bands.
	 * @param [in,out]	nHistogramBucketCounts	An array of the size of the buckets for each band.
	 *
	 * @return	CError of the result.
	 */
	CError Init(int nBands, int *nHistogramBucketCounts);
            
#ifndef SWIG
	explicit CFileStatistics(const NCSFileStatistics *pFileStatistics);
	NCSFileStatistics *Get() const { return m_pFileStatistics; };
	const CFileStatistics &operator=(const CFileStatistics &s);
	const CFileStatistics &operator=(const NCSFileStatistics *pFileStatistics);
#endif

	/**
	 * @fn	NCSError CFileStatistics::GetBandStatistics(unsigned int nBand,
	 * 		CBandStatistics *pBandStatistics) const;
	 *
	 * @brief	Gets the statistics for a band.
	 *
	 * @author	Intergraph Perth
	 * @date	19/02/2014
	 *
	 * @param	nBand				   	The requested band.
	 * @param [in,out]	pBandStatistics	If non-null, the band statistics.
	 *
	 * @return	NCSError of the result.
	 */
	CError GetBandStatistics(unsigned int nBand, CBandStatistics *pBandStatistics) const ;

	/**
	 * @fn	NCSError CFileStatistics::SetBandStatistics(unsigned int nBand,
	 * 		const CBandStatistics *pBandStatistics);
	 *
	 * @brief	Sets the statistics for a band.
	 *
	 * @author	Intergraph Perth
	 * @date	19/02/2014
	 *
	 * @param	nBand		   	The band.
	 * @param	pBandStatistics	The band statistics.
	 *
	 * @return	CError of the result.
	 */
	CError SetBandStatistics(unsigned int nBand, const CBandStatistics *pBandStatistics);

	/**
	 * @fn	unsigned int CFileStatistics::GetNumberOfBands() const;
	 *
	 * @brief	Gets the number of bands.
	 *
	 * @author	Intergraph Perth
	 * @date	19/02/2014
	 *
	 * @return	The number of bands.
	 */
	unsigned int GetNumberOfBands() const;
            
private:
#ifndef SWIG
	NCSFileStatistics *m_pFileStatistics;
#endif
};
        
} // API
} // NCS

#endif // NCS_API_FILE_STATISTICS_H

