/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: FileRPCData.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: File RPC data header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_FILE_RPCDATA_H
#define NCS_API_FILE_RPCDATA_H
#ifndef SWIG
    #include "NCSDefs.h"
#else
    #define NCSECW_IMPEX
#endif
#include <vector>
typedef struct NCSRPCData NCSRPCData;
namespace NCS {
namespace API {

/**
 * @class	NCS::API::CFileRPCData
 *
 * @brief	RPC Data for a file.
 * @ingroup	simplifiedclasses
 * 
 * @date	19/02/2014
 */
class NCSECW_IMPEX CFileRPCData {
public:
	CFileRPCData();
	virtual ~CFileRPCData();
	CFileRPCData(const CFileRPCData &s);
#ifndef SWIG
	explicit CFileRPCData(const NCSRPCData *pRPCData);
	const CFileRPCData &operator=(const CFileRPCData &s);
	const CFileRPCData &operator=(const NCSRPCData *pRPCData);
	NCSRPCData *Get() const  { return m_pRPCData; };
#endif

	/**
	 * @fn	double CFileRPCData::GetErrorBias() const;
	 *
	 * @brief	Gets the error bias.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The error bias.
	 */
	double GetErrorBias() const;

	/**
	 * @fn	void CFileRPCData::SetErrorBias(double ERR_BIAS);
	 *
	 * @brief	Sets the error bias.
	 *
	 * @date	19/02/2014
	 *
	 * @param	ERR_BIAS	The error bias.
	 */
	void SetErrorBias(double ERR_BIAS);

	/**
	 * @fn	double CFileRPCData::GetErrorRandom() const;
	 *
	 * @brief	Gets the error random.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The error random.
	 */
	double GetErrorRandom() const;

	/**
	 * @fn	void CFileRPCData::SetErrorRandom(double ERR_RAND);
	 *
	 * @brief	Sets the error random.
	 *
	 * @date	19/02/2014
	 *
	 * @param	ERR_RAND	The error random.
	 */
	void SetErrorRandom(double ERR_RAND);

	/**
	 * @fn	double CFileRPCData::GetLineOffset() const;
	 *
	 * @brief	Gets the line offset.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The line offset.
	 */
	double GetLineOffset() const;

	/**
	 * @fn	void CFileRPCData::SetLineOffset(double LINE_OFF);
	 *
	 * @brief	Sets the line offset.
	 *
	 * @date	19/02/2014
	 *
	 * @param	LINE_OFF	The line off.
	 */
	void SetLineOffset(double LINE_OFF);

	/**
	 * @fn	double CFileRPCData::GetSampleOffset() const;
	 *
	 * @brief	Gets the sample offset.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The sample offset.
	 */
	double GetSampleOffset() const;

	/**
	 * @fn	void CFileRPCData::SetSampleOffset(double SAMP_OFF);
	 *
	 * @brief	Sets the sample offset.
	 *
	 * @date	19/02/2014
	 *
	 * @param	SAMP_OFF	The samp off.
	 */
	void SetSampleOffset(double SAMP_OFF);

	/**
	 * @fn	double CFileRPCData::GetGeodeticLatitudeOffset() const;
	 *
	 * @brief	Gets the geodetic latitude offset.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The geodetic latitude offset.
	 */
	double GetGeodeticLatitudeOffset() const;

	/**
	 * @fn	void CFileRPCData::SetGeodeticLatitudeOffset(double LAT_OFF);
	 *
	 * @brief	Sets the geodetic latitude offset.
	 *
	 * @date	19/02/2014
	 *
	 * @param	LAT_OFF	The lat off.
	 */
	void SetGeodeticLatitudeOffset(double LAT_OFF);

	/**
	 * @fn	double CFileRPCData::GetGeodeticLongitudeOffset() const;
	 *
	 * @brief	Gets the geodetic longitude offset.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The geodetic longitude offset.
	 */
	double GetGeodeticLongitudeOffset() const;

	/**
	 * @fn	void CFileRPCData::SetGeodeticLongitudeOffset(double LONG_OFF);
	 *
	 * @brief	Sets the geodetic longitude offset.
	 *
	 * @date	19/02/2014
	 *
	 * @param	LONG_OFF	The long off.
	 */
	void SetGeodeticLongitudeOffset(double LONG_OFF);

	/**
	 * @fn	double CFileRPCData::GetGeodeticHeightOffset() const;
	 *
	 * @brief	Gets the geodetic height offset.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The geodetic height offset.
	 */
	double GetGeodeticHeightOffset() const;

	/**
	 * @fn	void CFileRPCData::SetGeodeticHeightOffset(double HEIGHT_OFF);
	 *
	 * @brief	Sets the geodetic height offset.
	 *
	 * @date	19/02/2014
	 *
	 * @param	HEIGHT_OFF	The height off.
	 */
	void SetGeodeticHeightOffset(double HEIGHT_OFF);

	/**
	 * @fn	double CFileRPCData::GetLineScale() const;
	 *
	 * @brief	Gets the line scale.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The line scale.
	 */
	double GetLineScale() const;

	/**
	 * @fn	void CFileRPCData::SetLineScale(double LINE_SCALE);
	 *
	 * @brief	Sets the line scale.
	 *
	 * @date	19/02/2014
	 *
	 * @param	LINE_SCALE	The line scale.
	 */
	void SetLineScale(double LINE_SCALE);

	/**
	 * @fn	double CFileRPCData::GetSampleScale() const;
	 *
	 * @brief	Gets the sample scale.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The sample scale.
	 */
	double GetSampleScale() const;

	/**
	 * @fn	void CFileRPCData::SetSampleScale(double SAMP_SCALE);
	 *
	 * @brief	Sets the sample scale.
	 *
	 * @date	19/02/2014
	 *
	 * @param	SAMP_SCALE	The samp scale.
	 */
	void SetSampleScale(double SAMP_SCALE);

	/**
	 * @fn	double CFileRPCData::GetGeodeticLatitudeScale() const;
	 *
	 * @brief	Gets the geodetic latitude scale.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The geodetic latitude scale.
	 */
	double GetGeodeticLatitudeScale() const;

	/**
	 * @fn	void CFileRPCData::SetGeodeticLatitudeScale(double LAT_SCALE);
	 *
	 * @brief	Sets the geodetic latitude scale.
	 *
	 * @date	19/02/2014
	 *
	 * @param	LAT_SCALE	The lat scale.
	 */
	void SetGeodeticLatitudeScale(double LAT_SCALE);

	/**
	 * @fn	double CFileRPCData::GetGeodeticLongitudeScale() const;
	 *
	 * @brief	Gets the geodetic longitude scale.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The geodetic longitude scale.
	 */
	double GetGeodeticLongitudeScale() const;

	/**
	 * @fn	void CFileRPCData::SetGeodeticLongitudeScale(double LONG_SCALE);
	 *
	 * @brief	Sets the geodetic longitude scale.
	 *
	 * @date	19/02/2014
	 *
	 * @param	LONG_SCALE	The long scale.
	 */
	void SetGeodeticLongitudeScale(double LONG_SCALE);

	/**
	 * @fn	double CFileRPCData::GetGeodeticHeightScale() const;
	 *
	 * @brief	Gets the geodetic height scale.
	 *
	 * @date	19/02/2014
	 *
	 * @return	The geodetic height scale.
	 */
	double GetGeodeticHeightScale() const;

	/**
	 * @fn	void CFileRPCData::SetGeodeticHeightScale(double HEIGHT_SCALE);
	 *
	 * @brief	Sets the geodetic height scale.
	 *
	 * @date	19/02/2014
	 *
	 * @param	HEIGHT_SCALE	The height scale.
	 */
	void SetGeodeticHeightScale(double HEIGHT_SCALE);

	/**
	 * @fn	void CFileRPCData::GetLineNumeratorCoefficients(std::vector<double> &vector) const;
	 *
	 * @brief	Gets the line numerator coefficients.
	 *
	 * @date	19/02/2014
	 *
	 * @param [in,out]	vector	The vector of line numerator coefficients.
	 */
	void GetLineNumeratorCoefficients(std::vector<double> &vector) const;

	/**
	 * @fn	void CFileRPCData::SetLineNumeratorCoefficients(const std::vector<double> &vector);
	 *
	 * @brief	Sets the line numerator coefficients.
	 *
	 * @date	19/02/2014
	 *
	 * @param	vector	The vector of line numerator coefficients.
	 */
	void SetLineNumeratorCoefficients(const std::vector<double> &vector);

	/**
	 * @fn	void CFileRPCData::GetLineDenominatorCoefficients(std::vector<double> &vector) const;
	 *
	 * @brief	Gets the line denominator coefficients.
	 *
	 * @date	19/02/2014
	 *
	 * @param [in,out]	vector	The vector of line denominator coefficients.
	 */
	void GetLineDenominatorCoefficients(std::vector<double> &vector) const;

	/**
	 * @fn	void CFileRPCData::SetLineDenominatorCoefficients(const std::vector<double> &vector);
	 *
	 * @brief	Sets the line denominator coefficients.
	 *
	 * @date	19/02/2014
	 *
	 * @param	vector	The vector of line denominator coefficients.
	 */
	void SetLineDenominatorCoefficients(const std::vector<double> &vector);

	/**
	 * @fn	void CFileRPCData::GetSampleNumeratorCoefficients(std::vector<double> &vector) const;
	 *
	 * @brief	Gets the sample numerator coefficients.
	 *
	 * @date	19/02/2014
	 *
	 * @param [in,out]	vector	The vector of sample numerator coefficients.
	 */
	void GetSampleNumeratorCoefficients(std::vector<double> &vector) const;

	/**
	 * @fn	void CFileRPCData::SetSampleNumeratorCoefficients(const std::vector<double> &vector);
	 *
	 * @brief	Sets the sample numerator coefficients.
	 *
	 * @date	19/02/2014
	 *
	 * @param	vector	The vector of sample numerator coefficients.
	 */
	void SetSampleNumeratorCoefficients(const std::vector<double> &vector);

	/**
	 * @fn	void CFileRPCData::GetSampleDenominatorCoefficients(std::vector<double> &vector) const;
	 *
	 * @brief	Gets a sample denominator coefficients.
	 *
	 * @date	19/02/2014
	 *
	 * @param [in,out]	vector	The vector of sample denominator coefficients.
	 */
	void GetSampleDenominatorCoefficients(std::vector<double> &vector) const;

	/**
	 * @fn	void CFileRPCData::SetSampleDenominatorCoefficients(const std::vector<double> &vector);
	 *
	 * @brief	Sets the sample denominator coefficients.
	 *
	 * @date	19/02/2014
	 *
	 * @param	vector	The vector of sample denominator coefficients.
	 */
	void SetSampleDenominatorCoefficients(const std::vector<double> &vector);
            
private:
	void SetCoefficients(double *dest, const std::vector<double> &src) const;
	void GetCoefficients(std::vector<double> &srcVec, const double * src) const;
#ifndef SWIG
	NCSRPCData *m_pRPCData;
#endif
};
        
} // API
} // NCS

#endif // NCS_API_FILE_RPCDATA_H
