/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: FileBandInfo.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: File band info header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_FILE_BAND_INFO_H
#define NCS_API_FILE_BAND_INFO_H

#ifndef SWIG
    #include "NCSDefs.h"
#else
    #define NCSECW_IMPEX
#endif
typedef struct NCSFileBandInfo NCSFileBandInfo;

namespace NCS {
namespace API {

/**
 * \class	NCS::API::CFileBandInfo
 *
 * \brief	Band details.
 * \ingroup	simplifiedclasses
 * 
 * \author	Intergraph Perth
 * \date	9/08/2013
 */
class NCSECW_IMPEX CFileBandInfo {
public:
	CFileBandInfo();
	virtual ~CFileBandInfo();
	CFileBandInfo(const CFileBandInfo &s);
            
#ifndef SWIG
	const CFileBandInfo &operator=(const CFileBandInfo &s);
	const CFileBandInfo &operator=(const NCSFileBandInfo *pFileBandInfo);
	NCSFileBandInfo *Get() const { return m_pFileBandInfo; };
#endif

	/**
	 * \fn	unsigned char CFileBandInfo::GetBitDepth() const;
	 *
	 * \brief	Gets bit depth of the band.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	The bit depth.
	 */
	unsigned char GetBitDepth() const;

	/**
	 * \fn	void CFileBandInfo::SetBitDepth(unsigned char nBits);
	 *
	 * \brief	Sets bit depth of the band for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	nBits	The bits.
	 */
	void SetBitDepth(unsigned char nBits);

	/**
	 * \fn	bool CFileBandInfo::GetSigned() const;
	 *
	 * \brief	Gets whether or not the band is a signed data type.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	true if it succeeds, false if it fails.
	 */
	bool GetSigned() const;

	/**
	 * \fn	void CFileBandInfo::SetSigned(bool bSigned);
	 *
	 * \brief	Sets whether or not the band is a signed data type. For encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	bSigned	true if signed.
	 */
	void SetSigned(bool bSigned);

	/**
	 * \fn	const char CFileBandInfo::*GetDescription() const;
	 *
	 * \brief	Gets the band description.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \return	null if it fails, else the description.
	 */
	const char *GetDescription() const;

	/**
	 * \fn	void CFileBandInfo::SetDescription(const char *szDesc);
	 *
	 * \brief	Sets the band description for encoding.
	 *
	 * \author	Intergraph Perth
	 * \date	9/08/2013
	 *
	 * \param	szDesc	The description.
	 */
	void SetDescription(const char *szDesc);
            
private:
            
#ifndef SWIG
	NCSFileBandInfo *m_pFileBandInfo;
#endif
};
        
} // API
} // NCS

#endif // NCS_API_FILE_BAND_INFO_H
