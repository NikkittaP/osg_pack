/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: BandHistogram.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: Band histogram header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_BAND_HISTOGRAM_H
#define NCS_API_BAND_HISTOGRAM_H

#ifndef SWIG
#include "NCSDefs.h"

 /** 
 *	@struct NCSBandHistogram
 *	@ingroup structures
 *	@brief Struct containing histogram data for one band. 
 */
typedef struct NCSBandHistogram {
    /** histogram min value */
    IEEE4 fMinHist;
    /** histogram max value */
    IEEE4 fMaxHist;
    /** histogram bucket count */
    UINT32 nHistBucketCount;
    /** histogram on the band */
    UINT64* Histogram;
} NCSBandHistogram;

#else
#define NCSECW_IMPEX
#endif
#include <vector>

namespace NCS {
    namespace API {

        /**
         * \class	NCS::API::CBandHistogram
         *
         * \brief	Histogram for a band.
         * \ingroup	simplifiedclasses
         * 
         * A histogram represents counts of values that fall into certain buckets. Range that histogram describes is specified using fMinHist and fMaxHist. This range is divided to nHistBucketCount buckets. Each bucket has its derived minimum and maximum values. Each bucket contains number of pixels which band value falls within bucket's minimum and maximum values. 
         * \author	Intergraph Perth
         * \date	8/08/2013
         */
        class NCSECW_IMPEX CBandHistogram {
        public:

            CBandHistogram();
            virtual ~CBandHistogram();
#ifndef SWIG
            explicit CBandHistogram(NCSBandHistogram *pBandStatistics);
            NCSBandHistogram *Get() const { return m_pBandHistogram; };
            const CBandHistogram &operator=(const NCSBandHistogram *pBandHistogram);
#endif

            /**
             * \fn	float CBandHistogram::GetMinimumValue() const;
             *
             * \brief	Gets minimum value (start of the histogram range).
             *
             * \author	Intergraph Perth
             * \date	8/08/2013
             *
             * \return	The minimum value in the histogram for this band.
             */
            float GetMinimumValue() const ;

            /**
             * \fn	void CBandHistogram::SetMinimumValue(float fMinimumValue);
             *
             * \brief	Sets minimum value (start of the histogram range). Encoding or header editing only.
             *
             * \author	Intergraph Perth
             * \date	8/08/2013
             *
             * \param	fMinimumValue	The minimum value.
             */
            void SetMinimumValue(float fMinimumValue);

            /**
             * \fn	float CBandHistogram::GetMaximumValue() const;
             *
             * \brief	Gets maximum value (end of the histogram range).
             *
             * \author	Intergraph Perth
             * \date	8/08/2013
             *
             * \return	The maximum value (end of the histogram range) in the histogram for this band.
             */
            float GetMaximumValue() const ;

            /**
             * \fn	void CBandHistogram::SetMaximumValue(float fMaximumValue);
             *
             * \brief	Sets maximum value (end of the histogram range). Encoding or header editing only
             *
             * \author	Intergraph Perth
             * \date	8/08/2013
             *
             * \param	fMaximumValue	The maximum value.
             */
            void SetMaximumValue(float fMaximumValue);

            /**
             * \fn	void CBandHistogram::GetBucketValues(std::vector<unsigned long long> &bucketValues) const;
             *
             * \brief	Gets histogram values.
             *
             * \author	Intergraph Perth
             * \date	8/08/2013
             *
             * \param [in,out]	bucketValues	The histogram values.
             */
            void GetBucketValues(std::vector<unsigned long long> &bucketValues) const;

            /**
             * \fn	void CBandHistogram::SetBucketValues(const std::vector<unsigned long long> &bucketValues);
             *
             * \brief	Sets histogram values. Encoding or header editing only.
             *
             * \author	Intergraph Perth
             * \date	8/08/2013
             *
             * \param	bucketValues	The histogram values.
             */
            void SetBucketValues(const std::vector<unsigned long long> &bucketValues);
            
        private:
            
#ifndef SWIG
            
            CBandHistogram(const CBandHistogram &s);
            const CBandHistogram &operator=(const CBandHistogram &s);
            NCSBandHistogram *m_pBandHistogram;
#endif
            
        };
        
    } // API
} // NCS

#endif // NCS_API_BAND_HISTOGRAM_H
