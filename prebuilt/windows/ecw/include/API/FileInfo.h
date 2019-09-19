/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: FileInfo.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: File info header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_FILE_INFO_H
#define NCS_API_FILE_INFO_H
#ifndef SWIG
    #include "NCSDefs.h"
#else
    #define NCSECW_IMPEX
	#ifdef _USE_32BIT_TIME_T
		typedef long time_t;
	#else
		typedef long long time_t;
	#endif
#endif
#include "FileBandInfo.h"
#include "FileColorSpace.h"
#include "CellType.h"
#include "CellSizeUnitType.h"
#include "NCSError.h"
#ifdef _WIN32_WCE
#include "time_CE.h"
#define time_t time_t_ce
#else
#include <ctime>
#endif

typedef struct NCSFileInfo NCSFileInfo;

namespace NCS {
namespace API {

/**
 * \class	NCS::API::CFileInfo
 *
 * \brief	Raster and geo-referencing information for a ECW or JP2 dataset.
 * \ingroup	simplifiedclasses
 * 
 * \author	Intergraph Perth
 * \date	9/08/2013
 */
class NCSECW_IMPEX CFileInfo {
public:
	CFileInfo();
	virtual ~CFileInfo();
	CFileInfo(const CFileInfo &s);

#ifndef SWIG
	const CFileInfo &operator=(const CFileInfo &s);
	const CFileInfo &operator=(const NCSFileInfo *pFileInfo);
	NCSFileInfo *Get() const { return m_pFileInfo; };
#endif

	/**
	 * \fn	unsigned int CFileInfo::GetSizeX() const;
	 *
	 * \brief	Gets width of dataset in pixels.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The width.
	 */
	unsigned int GetSizeX() const;

	/**
	 * \fn	void CFileInfo::SetSizeX(unsigned int nSizeX);
	 *
	 * \brief	Sets width of dataset in pixels for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nSizeX	The width.
	 */
	void SetSizeX(unsigned int nSizeX);

	/**
	 * \fn	unsigned int CFileInfo::GetSizeY() const;
	 *
	 * \brief	Gets height of dataset in pixels.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The height.
	 */
	unsigned int GetSizeY() const;

	/**
	 * \fn	void CFileInfo::SetSizeY(unsigned int nSizeY);
	 *
	 * \brief	Sets height of dataset in pixels for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nSizeY	The height.
	 */
	void SetSizeY(unsigned int nSizeY);

	/**
	 * \fn	unsigned int CFileInfo::GetNumberOfBands() const;
	 *
	 * \brief	Gets the number of bands.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The number of bands.
	 */
	unsigned int GetNumberOfBands() const;

	/**
	 * \fn	void CFileInfo::SetNumberOfBands(unsigned int nNumberOfBands);
	 *
	 * \brief	Sets the number of bands for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nNumberOfBands	Number of bands.
	 */
	void SetNumberOfBands(unsigned int nNumberOfBands);

	/**
	 * \fn	NCSCellSizeUnitType CFileInfo::GetCellSizeUnitType() const;
	 *
	 * \brief	Gets cell size unit type.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The cell size unit type.
	 */
	NCSCellSizeUnitType GetCellSizeUnitType() const;

	/**
	 * \fn	void CFileInfo::SetCellSizeUnitType(NCSCellSizeUnitType eCellSizeUnitType);
	 *
	 * \brief	Sets cell size unit type for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	eCellSizeUnitType	Type of the cell size unit.
	 */
	void SetCellSizeUnitType(NCSCellSizeUnitType eCellSizeUnitType);

	/**
	 * \fn	double CFileInfo::GetCellIncrementX() const;
	 *
	 * \brief	Gets cell increment x coordinate.  This is the width of one pixel in 
	 * 			world coordinates.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The cell increment x coordinate.
	 */
	double GetCellIncrementX() const;

	/**
	 * \fn	void CFileInfo::SetCellIncrementX(double dCellIncrementX);
	 *
	 * \brief	Sets cell increment x coordinate for encoding.  This is the width of 
	 * 			one pixel in world coordinates.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	dCellIncrementX	The cell increment x coordinate.
	 */
	void SetCellIncrementX(double dCellIncrementX);

	/**
	 * \fn	double CFileInfo::GetCellIncrementY() const;
	 *
	 * \brief	Gets cell increment y coordinate.  This is the height of one pixel in 
	 * 			world coordinates.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The cell increment y coordinate.
	 */
	double GetCellIncrementY() const;

	/**
	 * \fn	void CFileInfo::SetCellIncrementY(double dCellIncrementY);
	 *
	 * \brief	Sets cell increment y coordinate for encoding. This is the height of 
	 * 			one pixel in world coordinates.  
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	dCellIncrementY	The cell increment y coordinate.
	 */
	void SetCellIncrementY(double dCellIncrementY);

	/**
	 * \fn	double CFileInfo::GetOriginX() const;
	 *
	 * \brief	Gets the origin x coordinate in world coordinates.  This is the top
	 * 			left point of the top left pixel (0,0).
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The origin x coordinate.
	 */
	double GetOriginX() const;

	/**
	 * \fn	void CFileInfo::SetOriginX(double dOriginX);
	 *
	 * \brief	Sets origin x coordinate in world coordinates for encoding.  This 
	 * 			is the top left point of the top left pixel (0,0).
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	dOriginX	The origin x coordinate.
	 */
	void SetOriginX(double dOriginX);

	/**
	 * \fn	double CFileInfo::GetOriginY() const;
	 *
	 * \brief	Gets the origin y coordinate in world coordinates.  This is the top
	 * 			left point of the top left pixel (0,0).
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The origin y coordinate.
	 */
	double GetOriginY() const;

	/**
	 * \fn	void CFileInfo::SetOriginY(double dOriginY);
	 *
	 * \brief	Sets the origin y coordinate in world coordinates.  This is the top
	 * 			left point of the top left pixel (0,0).
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	dOriginY	The origin y coordinate.
	 */
	void SetOriginY(double dOriginY);

	/**
	 * \fn	const char CFileInfo::*GetDatum() const;
	 *
	 * \brief	Gets the datum string of the dataset.  Do not free this string.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	null if it fails, else the datum.
	 */
	const char *GetDatum() const;

	/**
	 * \fn	void CFileInfo::SetDatum(const char *szDatum);
	 *
	 * \brief	Sets the datum string of the dataset for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	szDatum	The datum.
	 */
	void SetDatum(const char *szDatum);

	/**
	 * \fn	const char CFileInfo::*GetProjection() const;
	 *
	 * \brief	Gets the projection string of the dataset.  Do not free
	 * 			this string.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	null if it fails, else the projection.
	 */
	const char *GetProjection() const;

	/**
	 * \fn	void CFileInfo::SetProjection(const char *szProjection);
	 *
	 * \brief	Sets the projection string of the dataset for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	szProjection	The projection.
	 */
	void SetProjection(const char *szProjection);

	/**
	 * \fn	int CFileInfo::GetEPSG() const;
	 *
	 * \brief	Gets the epsg code if it exists.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The epsg.
	 */
	int GetEPSG() const;

	/**
	 * \fn	void CFileInfo::SetEPSG(int nEPSG);
	 *
	 * \brief	Sets an epsg code for encoding.
	 * 			
	 * 			This internally modifies the datum and projection strings.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nEPSG	The epsg.
	 */
	void SetEPSG(int nEPSG);

	/**
	 * \fn	double CFileInfo::GetRotationDegrees() const;
	 *
	 * \brief	Gets rotation degrees.
	 * 			
	 * 			The SDK does not perform any rotations, this value is stored
	 * 			in the file for an application to use.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The rotation degrees.
	 */
	double GetRotationDegrees() const;

	/**
	 * \fn	void CFileInfo::SetRotationDegrees(double fRotationDegrees);
	 *
	 * \brief	Sets rotation degrees for encoding.
	 * 			
	 * 			The SDK does not perform any rotations, this value is stored
	 * 			in the file for an application to use.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	fRotationDegrees	The rotation in degrees.
	 */
	void SetRotationDegrees(double fRotationDegrees);

	/**
	 * \fn	NCSFileColorSpace CFileInfo::GetColorSpace() const;
	 *
	 * \brief	Gets the color space of the dataset.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The color space.
	 */
	NCSFileColorSpace GetColorSpace() const;

	/**
	 * \fn	void CFileInfo::SetColorSpace(NCSFileColorSpace eColorSpace);
	 *
	 * \brief	Sets the color space of the dataset for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	eColorSpace	The color space.
	 */
	void SetColorSpace(NCSFileColorSpace eColorSpace);

    /**
     * \fn	NCSCellType CFileInfo::GetCellType() const;
     *
     * \brief	Gets cell type of the dataset.
     *
     * \author	Intergraph Perth
     * \date	9/08/2013
     *
     * \return	The cell type.
     */
    NCSCellType GetCellType() const;

	/**
	 * \fn	void CFileInfo::SetCellType(NCSCellType eCellType);
	 *
	 * \brief	Sets cell type of the dataset for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	eCellType	Type of the cell.
	 */
	void SetCellType(NCSCellType eCellType);

	/**
	 * \fn	NCSError CFileInfo::GetBandInfo(unsigned int nBand, CFileBandInfo *pBandInfo) const;
	 *
	 * \brief	Gets band information for specified band.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nBand			 	The band.
	 * \param [in,out]	pBandInfo	If non-null, information describing the band.
	 *
	 * \return	An error of the result.
	 */
	NCS::CError GetBandInfo(unsigned int nBand, CFileBandInfo *pBandInfo) const;

	/**
	 * \fn	NCSError CFileInfo::SetBandInfo(unsigned int nBand, const CFileBandInfo *pBandInfo);
	 *
	 * \brief	Sets band information for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nBand	 	The band.
	 * \param	pBandInfo	Information describing the band.
	 *
	 * \return	An error of the result.
	 */
	NCS::CError SetBandInfo(unsigned int nBand, const CFileBandInfo *pBandInfo);

	/**
	 * \fn	unsigned char CFileInfo::GetFormatVersion() const;
	 *
	 * \brief	Gets format version of the ECW or JP2 dataset.  1 only for JP2,
	 * 			2 or 3 for ECW
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The format version.
	 */
	unsigned char GetFormatVersion() const;

	/**
	 * \fn	void CFileInfo::SetFormatVersion(unsigned char nFormatVersion);
	 *
	 * \brief	Sets format version for encoding.   1 only for JP2,
	 * 			2 or 3 for ECW, however, version 3 is only supported by SDK v5.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nFormatVersion	The format version.
	 */
	void SetFormatVersion(unsigned char nFormatVersion);

	/**
	 * \fn	unsigned char CFileInfo::GetCellBitDepth() const;
	 *
	 * \brief	Gets cell bit depth.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The cell bit depth.
	 */
	unsigned char GetCellBitDepth() const;

	/**
	 * \fn	void CFileInfo::SetCellBitDepth(unsigned char nCellBitDepth);
	 *
	 * \brief	Sets cell bit depth.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nCellBitDepth	Depth of the cell bit.
	 */
	void SetCellBitDepth(unsigned char nCellBitDepth);

	/**
	 * \fn	const time_t CFileInfo::GetCompressionDate() const;
	 *
	 * \brief	Gets compression date.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The compression date.
	 */
	const time_t GetCompressionDate() const;

	/**
	 * \fn	float CFileInfo::GetActualCompressionRate();
	 *
	 * \brief	Gets actual compression rate of the dataset.  
	 * 			
	 * 			This is calculated by the actual size of the dataset on disk and the logical decompressed 
	 * 			size of the dataset (width*height*bands*bytesperband)
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The actual compression rate.
	 */
	float GetActualCompressionRate();

	/**
	 * \fn	void CFileInfo::SetTargetCompressionRate(unsigned int nTargetCompressionRate) private: #ifndef SWIG NCSFileInfo *m_pFileInfo;
	 *
	 * \brief	Sets target compression rate for compression.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nTargetCompressionRate	Target compression rate.
	 */
	void SetTargetCompressionRate(unsigned int nTargetCompressionRate);
private:

#ifndef SWIG
	NCSFileInfo *m_pFileInfo;
#endif
};

} // API
} // NCS

#endif // NCS_API_FILE_INFO_H
