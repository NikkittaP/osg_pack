/******************************************************************************
 ** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
 **
 ** FILE: FileMetadata.h
 ** CREATED: <20/08/2013>
 ** PURPOSE: File metadata header file of ECW JPEG 2000 SDK
 **
 ******************************************************************************/
#ifndef NCS_API_FILE_METADATA_H
#define NCS_API_FILE_METADATA_H

#ifndef SWIG
    #include "NCSDefs.h"
#else
    #define NCSECW_IMPEX
#endif

typedef struct NCSFileMetaData NCSFileMetaData;

namespace NCS {
namespace API {

/**
 * @class	NCS::API::CFileMetadata
 *
 * @brief	File Metadata.
 * @ingroup	simplifiedclasses
 * 
 * @date	19/02/2014
 */
class NCSECW_IMPEX CFileMetadata {
public:
	CFileMetadata();
	virtual ~CFileMetadata();
	CFileMetadata(const CFileMetadata &s);

#ifndef SWIG
	const CFileMetadata &operator=(const CFileMetadata &s);
	const CFileMetadata &operator=(const NCSFileMetaData *pFileMetadata);
	NCSFileMetaData *Get() const { return m_pFileMetadata; };
#endif

	/**
	 * @fn	char CFileMetadata::*GetClassification() const;
	 *
	 * @brief	Gets the classification.
	 *
	 * @date	19/02/2014
	 *
	 * @return	null if it fails, else the classification.
	 */
	char *GetClassification() const;

	/**
	 * @fn	void CFileMetadata::SetClassification(const char *szClassification);
	 *
	 * @brief	Sets the classification.
	 *
	 * @date	19/02/2014
	 *
	 * @param	szClassification	The classification.
	 */
	void SetClassification(const char *szClassification);

	/**
	 * @fn	char CFileMetadata::*GetAcquisitionDate() const;
	 *
	 * @brief	Gets the acquisition date.
	 *
	 * @date	19/02/2014
	 *
	 * @return	null if it fails, else the acquisition date.
	 */
	char *GetAcquisitionDate() const;

	/**
	 * @fn	void CFileMetadata::SetAcquisitionDate(const char *szAcquisitionDate);
	 *
	 * @brief	Sets the acquisition date.
	 *
	 * @date	19/02/2014
	 *
	 * @param	szAcquisitionDate	Date of the acquisition.
	 */
	void SetAcquisitionDate(const char *szAcquisitionDate);

	/**
	 * @fn	char CFileMetadata::*GetAcquisitionSensorName() const;
	 *
	 * @brief	Gets the acquisition sensor name.
	 *
	 * @date	19/02/2014
	 *
	 * @return	null if it fails, else the acquisition sensor name.
	 */
	char *GetAcquisitionSensorName() const;

	/**
	 * @fn	void CFileMetadata::SetAcquisitionSensorName(const char *szAcquisitionSensorName);
	 *
	 * @brief	Sets the acquisition sensor name.
	 *
	 * @date	19/02/2014
	 *
	 * @param	szAcquisitionSensorName	Name of the acquisition sensor.
	 */
	void SetAcquisitionSensorName(const char *szAcquisitionSensorName);

	/**
	 * @fn	char CFileMetadata::*GetCompressionSoftware() const;
	 *
	 * @brief	Gets the compression software.
	 *
	 * @date	19/02/2014
	 *
	 * @return	null if it fails, else the compression software.
	 */
	char *GetCompressionSoftware() const;

	/**
	 * @fn	void CFileMetadata::SetCompressionSoftware(const char *szCompressionSoftware);
	 *
	 * @brief	Sets the compression software.
	 *
	 * @date	19/02/2014
	 *
	 * @param	szCompressionSoftware	The compression software.
	 */
	void SetCompressionSoftware(const char *szCompressionSoftware);

	/**
	 * @fn	char CFileMetadata::*GetAuthor() const;
	 *
	 * @brief	Gets the author.
	 *
	 * @date	19/02/2014
	 *
	 * @return	null if it fails, else the author.
	 */
	char *GetAuthor() const;

	/**
	 * @fn	void CFileMetadata::SetAuthor(const char *szAuthor);
	 *
	 * @brief	Sets the author.
	 *
	 * @date	19/02/2014
	 *
	 * @param	szAuthor	The author.
	 */
	void SetAuthor(const char *szAuthor);

	/**
	 * @fn	char CFileMetadata::*GetCopyright() const;
	 *
	 * @brief	Gets the copyright.
	 *
	 * @date	19/02/2014
	 *
	 * @return	null if it fails, else the copyright.
	 */
	char *GetCopyright() const;

	/**
	 * @fn	void CFileMetadata::SetCopyright(const char *szCopyright);
	 *
	 * @brief	Sets the copyright.
	 *
	 * @date	19/02/2014
	 *
	 * @param	szCopyright	The copyright.
	 */
	void SetCopyright(const char *szCopyright);

	/**
	 * @fn	char CFileMetadata::*GetCompany() const;
	 *
	 * @brief	Gets the company.
	 *
	 * @date	19/02/2014
	 *
	 * @return	null if it fails, else the company.
	 */
	char *GetCompany() const;

	/**
	 * @fn	void CFileMetadata::SetCompany(const char *szCompany);
	 *
	 * @brief	Sets the company.
	 *
	 * @date	19/02/2014
	 *
	 * @param	szCompany	The company.
	 */
	void SetCompany(const char *szCompany);

	/**
	 * @fn	char CFileMetadata::*GetEmail() const;
	 *
	 * @brief	Gets the email.
	 *
	 * @date	19/02/2014
	 *
	 * @return	null if it fails, else the email.
	 */
	char *GetEmail() const;

	/**
	 * @fn	void CFileMetadata::SetEmail(const char *szEmail);
	 *
	 * @brief	Sets the email address.
	 *
	 * @date	19/02/2014
	 *
	 * @param	szEmail	The email address.
	 */
	void SetEmail(const char *szEmail);

	/**
	 * @fn	char CFileMetadata::*GetAddress() const;
	 *
	 * @brief	Gets the address.
	 *
	 * @date	19/02/2014
	 *
	 * @return	null if it fails, else the address.
	 */
	char *GetAddress() const;

	/**
	 * @fn	void CFileMetadata::SetAddress(const char *szAddress);
	 *
	 * @brief	Sets the address.
	 *
	 * @date	19/02/2014
	 *
	 * @param	szAddress	The address.
	 */
	void SetAddress(const char *szAddress);

	/**
	 * @fn	char CFileMetadata::*GetTelephone() const;
	 *
	 * @brief	Gets the telephone.
	 *
	 * @date	19/02/2014
	 *
	 * @return	null if it fails, else the telephone.
	 */
	char *GetTelephone() const;

	/**
	 * @fn	void CFileMetadata::SetTelephone(const char *szTelephone);
	 *
	 * @brief	Sets the telephone.
	 *
	 * @date	19/02/2014
	 *
	 * @param	szTelephone	The telephone.
	 */
	void SetTelephone(const char *szTelephone);

private:

#ifndef SWIG
	NCSFileMetaData *m_pFileMetadata;
#ifdef NCS_BUILD_UNICODE
	char *m_szClassification;
	char *m_szAcquisitionDate;
	char *m_szAcquisitionSensorName;
	char *m_szCompressionSoftware; 
	char *m_szAuthor;
	char *m_szCopyright; 
	char *m_szCompany;
	char *m_szEmail;
	char *m_szAddress;
	char *m_szTelephone;
#endif
#endif
};


} // API
} // NCS

#endif // NCS_API_FILE_METADATA_H
