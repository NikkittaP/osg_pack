/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: BandStatistics.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Band statistics header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_BAND_STATISTICS_H
#define NCS_API_BAND_STATISTICS_H

typedef struct NCSBandStats NCSBandStats;
#ifndef SWIG
    #include "NCSDefs.h"
#else 
    #define NCSECW_IMPEX
#endif

#include "BandHistogram.h"
#include "NCSError.h"

namespace NCS {
namespace API {

/**
 * \class	NCS::API::CBandStatistics
 *
 * \brief	Statistics for a band.
 * \ingroup	simplifiedclasses
 *
 * \author	Intergraph Perth
 * \date	8/08/2013
 */
class NCSECW_IMPEX CBandStatistics {
public:
	CBandStatistics();
	virtual ~CBandStatistics();
#ifndef SWIG
	explicit CBandStatistics(NCSBandStats *pBandStatistics);
	NCSBandStats *Get() const { return m_pBandStatistics; };
    const CBandStatistics &operator=(const NCSBandStats *pFileBandInfo);
#endif

	/**
	 * \fn	float CBandStatistics::GetModeValue() const;
	 *
	 * \brief	Gets mode value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The mode value.
	 */
	float GetModeValue() const ;

	/**
	 * \fn	void CBandStatistics::SetModeValue(float fModeValue);
	 *
	 * \brief	Sets mode value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	fModeValue	The mode value.
	 */
	void SetModeValue(float fModeValue);

	/**
	 * \fn	float CBandStatistics::GetMinimumValue() const;
	 *
	 * \brief	Gets minimum value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The minimum value.
	 */
	float GetMinimumValue() const ;

	/**
	 * \fn	void CBandStatistics::SetMinimumValue(float fMinimumValue);
	 *
	 * \brief	Sets minimum value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	fMinimumValue	The minimum value.
	 */
	void SetMinimumValue(float fMinimumValue);

	/**
	 * \fn	float CBandStatistics::GetMaximumValue() const;
	 *
	 * \brief	Gets maximum value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The maximum value.
	 */
	float GetMaximumValue() const ;

	/**
	 * \fn	void CBandStatistics::SetMaximumValue(float fMaximumValue);
	 *
	 * \brief	Sets maximum value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	fMaximumValue	The maximum value.
	 */
	void SetMaximumValue(float fMaximumValue);

	/**
	 * \fn	float CBandStatistics::GetMeanValue() const;
	 *
	 * \brief	Gets mean value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The mean value.
	 */
	float GetMeanValue() const ;

	/**
	 * \fn	void CBandStatistics::SetMeanValue(float fMeanValue);
	 *
	 * \brief	Sets mean value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	fMeanValue	The mean value.
	 */
	void SetMeanValue(float fMeanValue);

	/**
	 * \fn	float CBandStatistics::GetMedianValue() const;
	 *
	 * \brief	Gets median value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The median value.
	 */
	float GetMedianValue() const ;

	/**
	 * \fn	void CBandStatistics::SetMedianValue(float fMedianValue);
	 *
	 * \brief	Sets median value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	fMedianValue	The median value.
	 */
	void SetMedianValue(float fMedianValue);

	/**
	 * \fn	float CBandStatistics::GetStandardDeviationValue() const;
	 *
	 * \brief	Gets standard deviation value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \return	The standard deviation value.
	 */
	float GetStandardDeviationValue() const ;

	/**
	 * \fn	void CBandStatistics::SetStandardDeviationValue(float fStandardDeviationValue);
	 *
	 * \brief	Sets standard deviation value.
	 *
	 * \author	Intergraph Perth
	 * \date	8/08/2013
	 *
	 * \param	fStandardDeviationValue	The standard deviation value.
	 */
	void SetStandardDeviationValue(float fStandardDeviationValue);

    /**
     * \fn	NCS::CError CBandStatistics::GetHistogram(CBandHistogram *pHistogram) const;
     *
     * \brief	Gets a histogram.
     *
     * \author	Intergraph Perth
     * \date	8/08/2013
     *
     * \param [in,out]	pHistogram	If non-null, the histogram.
     *
     * \return	The histogram.
     */
    NCS::CError GetHistogram(CBandHistogram *pHistogram) const;

    /**
     * \fn	NCS::CError CBandStatistics::SetHistogram(const CBandHistogram *pHistogram);
     *
     * \brief	Sets the histogram for this band.
     *
     * \author	Intergraph Perth
     * \date	8/08/2013
     *
     * \param	pHistogram	The histogram.
     *
     * \return	Error class
     */
    NCS::CError SetHistogram(const CBandHistogram *pHistogram); 

private:

#ifndef SWIG
	
	CBandStatistics(const CBandStatistics &s);
	const CBandStatistics &operator=(const CBandStatistics &s);
	NCSBandStats *m_pBandStatistics;
#endif

};

} // API
} // NCS

#endif // NCS_API_BAND_STATISTICS_H
