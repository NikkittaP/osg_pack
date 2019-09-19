/****************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
****************************************************/
// NCSGDTEPSGKey.h: interface for the CEPSGKey class.
//
//////////////////////////////////////////////////////////////////////

#ifndef NCSGDTEPSGKEY_H
#define NCSGDTEPSGKEY_H

#include <string>
#include <NCSTypes.h>

using std::string;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace NCS {
namespace GDT {

class NCSECW_IMPEX CEPSGKey  
{
public:
	CEPSGKey();
	virtual ~CEPSGKey();
	explicit CEPSGKey(string &Line);

	INT32 GetEPSG() const {return m_nEPSG;}
	string GetProjection() const {return m_Projection;}
	string GetDatum() const {return m_Datum;}
	string GetComment() const { return m_Comments; }

	void SetEPSG(INT32 nEpsg) {m_nEPSG = nEpsg;}
	void SetProjection(const string& strPrj ) { m_Projection = strPrj;}
	void SetDatum(const string& strDatum ) { m_Datum = strDatum;}
	void SetComment(const string& Comments ) { m_Comments = Comments;}

private:
	string m_Projection;
	string m_Datum;
	INT32  m_nEPSG;
	string m_Comments; 
};
}
}

#endif //NCSGDTEPSGKEY_H
