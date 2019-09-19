/******************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** CREATED:  16 Jan 2004
** AUTHOR:   Gary Noel
** PURPOSE:  Enable users to define own Epsg Pcs code projection/Datum Mappings
**			 Used in conjunction with file "PcsKeyProjDatum.dat" 
** EDITS:    [xx] ddMmmyy NAME COMMENTS
**			 [01] 03Aug04 tfl  Added GetGCSGeoKey fn along similar lines
**			 [02] 06Jan05 tfl  Major changes to fix GDT path location issues
**							   (made class singleton to improve efficiency of use, subclassed
**								class with path as static member, changed method names)
** 
** 
*******************************************************/
#ifndef NCSGDTEPSG_H
#define NCSGDTEPSG_H

#include "NCSGDTEPSGKey.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NCSTypes.h"
#include "NCSError.h"
#include "NCSGDTLocation.h"
#include "NCSString.h"

#include <vector>

#ifndef POSIX
#include  <unordered_map>
#endif

#include <map>

namespace NCS {
namespace GDT {

#define CNCSGDTEPSG NCS::GDT::CEPSG

class NCSECW_IMPEX CEPSG: public CLocation  
{
public:
	static CEPSG *Instance();
	static void Release();

	CError GetProjectionAndDatum(const INT32 nEPSG, char **pszProjection, char **pszDatum,
									bool bUseEpsgCodeIfFails//=false//If true and found no valid projection/datum pair, write both proj/dat as "epsg:xxxx" where xxxx denotes epsg code number.
									);
	INT32 GetEPSG(const char *szProjection, const char *szDatum);

	int GetKeys(std::vector<CEPSGKey>& Keys);
	int SetKeys(const std::vector<CEPSGKey>& Keys);
	bool AddCustomEPSGCodes(CString &sMapping);
	bool WriteToFile() const;// write to existing path
	bool WriteToFile(const char *szPath) const;

protected:
	CEPSG();
	virtual ~CEPSG() {}

	//Inherited and overridden from CLocation
	virtual bool IsValidPath(const char *szPath);

private:
	static CEPSG *sm_pInstance;
	static CMutex sm_mInstance;
	mutable CMutex m_InstanceMutex;
	bool m_bCustomKeysLoaded;
	NCSError LoadCustomKeys(const char *szPath);
	std::vector<CEPSGKey> m_Keys;


	typedef std::pair<std::string, std::string>	 STDstringPair;
#ifdef POSIX
	std::map<INT32, STDstringPair > m_mapEpsg2ProjDatPair;
#else
	std::unordered_map<INT32, STDstringPair >	 m_mapEpsg2ProjDatPair;
#endif
	struct less_2strs {
		bool operator()(const STDstringPair& left, const STDstringPair& right)const{
#ifdef _WIN32
			return (NCSstricmp(left.first.c_str(), right.first.c_str()) <0)
				&& (NCSstricmp(left.second.c_str(), right.second.c_str()) <0);
#else
            return (strcmp(left.first.c_str(), right.first.c_str()) <0)
				&& (strcmp(left.second.c_str(), right.second.c_str()) <0);
#endif
		}
	};
	std::map<std::pair<std::string, std::string>, INT32> m_mapProjDat2Epsg;	
};

}}

#endif //NCSGDTEPSG_H
