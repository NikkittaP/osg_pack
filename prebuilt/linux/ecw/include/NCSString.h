/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** This software is covered by US patent #6,442,298,
** #6,102,897 and #6,633,688.  Rights to use these patents 
** is included in the license agreements.
 *******************************************************/

#ifndef NCSSTRING_H
#define NCSSTRING_H

// shut MS compiler up!
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4786)
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#endif

#ifdef __cplusplus

#ifdef POSIX
#ifdef min
#undef min
#define min min
#endif
#ifdef max
#undef max
#define max max
#endif
#endif
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <map>

#include "NCSDefs.h"
#include "NCSMisc.h"

#ifdef NCS_BUILD_UNICODE
typedef std::wstring StdString;
#else
typedef std::string StdString;
#endif

namespace NCS {

/*
 * Important note: Changed NCSString not to inherit from std string to avoid duplicated linking problem in VC2010
 * when a client problem uses std string in a different source file. This is due to std string is implemented
 * as header only in VC2010. To keep the interface simple and remain backwards compability as much as possilbe,
 * only a handful  of functions of std string are exposed. At the same time, NCSString can be converted to the 
 * std string counterpart using the conversion operator. However, NCS::CString is not a std string and any method
 * or operators of std string but not of NCS::CString should not be used directly.
 */
class NCSECW_IMPEX CString
{
public:
	// size of Format() buffer.
	enum { _MAX_CHARS = 8096 };
	typedef StdString::size_type size_type;
	
	static size_t npos;

	/**
	 * Existing public functions from StdString
	 */
	const NCSTChar* c_str() const
	{
		return m_NativeString.c_str();
	}

	size_t find (const NCSTChar* s, size_t pos = 0) const
	{
		return m_NativeString.find(s, pos);
	}

	size_t length() const
	{
		return m_NativeString.length();
	}

	StdString& insert (size_t pos, const NCSTChar* s)
	{
		return m_NativeString.insert(pos, s);
	}

	bool empty() const
	{
		return m_NativeString.empty();
	}

	size_t size() const
	{
		return m_NativeString.size();
	}

	size_t rfind (const NCSTChar* s, size_t pos = npos) const
	{
		return m_NativeString.rfind(s, pos);
	}

	StdString substr (size_t pos = 0, size_t len = npos) const
	{
		return m_NativeString.substr(pos, len);
	}

	StdString& append (const StdString& str)
	{
		return m_NativeString.append(str);
	}

	StdString& append (const NCSTChar* s, size_t n)
	{
		return m_NativeString.append(s, n);
	}

	StdString& append (size_t n, NCSTChar c)
	{
		return m_NativeString.append(n, c);
	}

	size_t find_first_of (const NCSTChar& s, size_t pos = 0) const
	{
		return m_NativeString.find_first_of(s, pos);
	}

	size_t find_first_of (const StdString& s, size_t pos = 0) const
	{
		return m_NativeString.find_first_of(s, pos);
	}

	size_t find_last_of (const NCSTChar* s, size_t pos = npos) const
	{
		return m_NativeString.find_last_of(s, pos);
	}

	size_t find_last_of (const StdString& str, size_t pos = npos) const
	{
		return m_NativeString.find_last_of(str, pos);
	}

	size_t find_last_not_of (const NCSTChar* s, size_t pos = npos) const
	{
		return m_NativeString.find_last_not_of(s, pos);
	}

	size_t find_first_not_of (const NCSTChar* s, size_t pos = 0) const
	{
		return m_NativeString.find_first_not_of(s, pos);
	}

	void resize (size_t n)
	{
		return m_NativeString.resize(n);
	}
	
	void resize (size_t n, NCSTChar c)
	{
		return m_NativeString.resize(n, c);
	}

	StdString& erase (size_t pos = 0, size_t len = npos)
	{
		return m_NativeString.erase(pos, len);
	}

	const NCSTChar& operator[] (int pos) const
	{
		return m_NativeString[pos];
	}

	NCSTChar& operator[] (int pos) 
	{
		return m_NativeString[pos];
	}

	void reserve (size_t n = 0)
	{
		return m_NativeString.reserve(n);
	}
	
	int compare (const StdString& str) const
	{
		return m_NativeString.compare(str);
	}

	void clear()
	{
		return m_NativeString.clear();
	}

	int compare (size_t pos, size_t len, const NCSTChar* s) const
	{
		return m_NativeString.compare(pos, len, s);
	}
	
	
	CString& replace (size_t pos,  size_t len,  const StdString& str)
	{
		m_NativeString.replace(pos, len, str);
		return *this;
	}
	
	const StdString& ToStdString() const
	{
		return m_NativeString;
	}
	
	StdString& ToStdString() 
	{
		return m_NativeString;
	}
private:
	
	StdString m_NativeString;
public:

	CString();
	~CString();

	// various constructor flavours
	// Allowing implicit construction
	CString(const CString& arg) { m_NativeString.assign(arg.c_str()); }
#ifdef NCS_BUILD_UNICODE
	CString(const std::wstring& arg)	{ m_NativeString.assign(arg); }
	CString(const char* pArg);
	CString(const std::string& arg)	{ (*this) = arg.c_str(); }
#else
	CString(const std::string& arg)	{ m_NativeString.assign(arg); }
#endif
	CString(const NCSTChar* pArg)		{ m_NativeString.assign(pArg ? pArg : NCS_T("")); }

	// various assignment operator flavours
	CString& operator=(const NCSTChar* pArg)		{ m_NativeString.assign(pArg?pArg:NCS_T("")); return (*this); }
#ifdef NCS_BUILD_UNICODE
	CString& operator=(const char* pArg);
	CString& operator=(const std::wstring& arg)	{ m_NativeString.assign(arg); return (*this); }
	CString& operator=(const std::string& arg)	{ (*this)=arg.c_str(); return (*this); }
#else
	CString& operator=(const std::string& arg)	{ m_NativeString.assign(arg); return (*this); }
#endif
	CString& operator=(const CString& arg);

	CString& operator+=(const NCS::CString& arg)	{ (*this)+=arg.c_str(); return (*this); }
	CString& operator+=(const NCSTChar* pArg)	{ m_NativeString.append(pArg?pArg:NCS_T("")); return (*this); }
	//CString& operator+(const NCS::CString& arg)	{ m_NativeString + arg.ToStdString(); return (*this); }

	bool operator==(const NCS::CString& arg) const { return(m_NativeString == arg.m_NativeString); }
	bool operator==(const StdString& arg) const { return(m_NativeString == arg); }
	bool operator==(const NCSTChar* arg) const { return(m_NativeString == arg); }
	bool operator!=(const NCS::CString& arg) const { return(m_NativeString != arg.m_NativeString); }
	bool operator!=(const NCSTChar* arg) const { return (m_NativeString != arg); }
	bool operator<(const NCS::CString& arg) const { return(m_NativeString < arg.m_NativeString); }
	bool operator>=(const NCSTChar* arg) const { return (!(m_NativeString < arg)); }


#ifdef NCS_BUILD_UNICODE
	CString& operator+=(const char* pArg);
	CString& operator+=(const std::wstring& arg)	{ m_NativeString.append(arg); return (*this); }
	CString& operator+=(const std::string& arg)	{ (*this)+=arg.c_str(); return (*this); }
#else
	CString& operator+=(const std::string& arg)	{ m_NativeString.append(arg); return (*this); }
#endif

	CString& operator<<(const char* pArg);
#ifdef NCS_BUILD_UNICODE
	CString& operator<<(const wchar_t* pArg);
	CString& operator<<(const std::wstring& pArg);
#endif
	CString& operator<<(const std::string& pArg);
	CString& operator<<(const NCS::CString& pArg);
	CString& operator<<(UINT8 nArg);
	CString& operator<<(UINT16 nArg);
	CString& operator<<(UINT32 nArg);
	CString& operator<<(UINT64 nArg);
	CString& operator<<(INT8 nArg);
	CString& operator<<(INT16 nArg);
	CString& operator<<(INT32 nArg);
	CString& operator<<(INT64 nArg);
	CString& operator<<(IEEE4 fArg);
	CString& operator<<(IEEE8 dArg);

	// the essential MFC member functions
	int Format(const NCSTChar* szFormat,...);
#ifdef NCS_BUILD_UNICODE
	int Format(const char* szFormat,...);
#endif

	// this returns a reference so you can do things like this:
	// CString query = "abc";
	// query += CString().Format("%d %s",1,"abc");
	CString& FormatEx(const NCSTChar* szFormat,...);

		// trim the right hand of the string of whitespace characters
	CString& TrimRight(const NCSTChar* pszDelimiters = NCS_T(" \t\r\n") );

    // trim the right hand of the string of whitespace characters
    CString& TrimLeft(const NCSTChar* pszDelimiters = NCS_T(" \t\r\n") );

    // trim leading and trailing whitespace
    CString& TrimAll(const NCSTChar* pszDelimiters = NCS_T(" \t\r\n") );

	// convert to upper and lower case
	CString& MakeUpper();
	CString& MakeLower();
	bool EndsWith(const CString& sEnd, bool bCompareNoCase=true) const;

	//pass non-null pointer to the below functions if caller cares about validity of the conversion
	INT32 ToINT32(bool* pRes = NULL)const;
	INT64 ToINT64(bool* pRes = NULL)const;
	IEEE8 ToIEEE8(bool* pRes = NULL)const;

	bool CompareNoCase( const CString& input )const;
	bool CompareNoCase( const CString& input, size_t nChars )const;

	int Split(const CString& delimiter, std::vector<CString>& results, bool bIncludeEmpty=false)const;

	static int Split(const CString& input, const CString& delimiter, std::vector<CString>& results, bool bIncludeEmpty=false);
	static int CompareNoCase( const CString& first, const CString& second );
	static int CompareNoCase( const CString& first, const CString& second, size_t nChars );



#ifdef NCS_BUILD_UNICODE
	CString Substr(size_type pos = 0, size_type n = npos) const;

	NCS_DEPRECATED(const char *a_str());
	NCS_DEPRECATED(void a_str(char **szAscii) const);
	NCS_DEPRECATED(void a_str(char *szAscii, int nElements) const);
	NCS_DEPRECATED(void a_str(std::string &sAscii) const);
	std::string m_sAsciiString;
	std::string m_utf8String;
#else
	inline CString Substr(size_type pos = 0, size_type n = npos) const
	{
		return m_NativeString.substr( pos, n );
	}

	NCS_DEPRECATED(const char *a_str() const { return c_str(); });
	NCS_DEPRECATED(void a_str(char **szAscii) const);
	NCS_DEPRECATED(void a_str(char *szAscii, int nElements) const);
	NCS_DEPRECATED(void a_str(std::string &sAscii) const);
#endif
	operator const char*() { return(utf8_str()); };
#ifdef NCS_BUILD_UNICODE
	operator const wchar_t*() { return(c_str()); };
#endif

	int ReplaceAll( const CString& match, const CString& value, size_t nStartPos = 0 );

	static CString UrlUnEsc(const std::string& sUrlEscaped);
	static CString UrlUnEsc(const char *pszUrlEscaped);

	static std::string UrlEsc(const CString& src);

#ifdef NCS_BUILD_UNICODE
	static std::vector<UINT16> Utf16Encode(const std::wstring& wideString);
	static std::wstring Utf16Decode(const std::vector<UINT16>& utf16Str);

	static std::string Utf8Encode (const std::wstring &sValue);
	static std::wstring Utf8Decode (const std::string &sUtf8);
	
	static void Utf16Encode(std::vector<UINT16>& utf16Str, const std::wstring &sValue);
	static void Utf16Decode(std::wstring& wideString, const std::vector<UINT16> &sUtf16);

	static void Utf8Encode (std::string& utf8Str, const std::wstring &sValue);
	static void Utf8Decode (std::wstring& sUtf16, const std::string &sUtf8);

#endif

#ifdef NCS_BUILD_UNICODE
	std::vector<UINT16> utf16_str(){
		return Utf16Encode(m_NativeString);
	}
	const char* utf8_str() { 
		m_utf8String.clear();
		Utf8Encode(m_utf8String, std::wstring(c_str()));
		return m_utf8String.c_str(); 
	}
	void FromUtf16(const UINT16* pUtf16Str, size_t len) { if (pUtf16Str){ Utf16Decode(this->ToStdString(), std::vector<UINT16>(pUtf16Str, pUtf16Str+len));}}
	void FromUtf8(const char* cpUtf8){ if(cpUtf8){ Utf8Decode(this->ToStdString(),  std::string(cpUtf8)); } }	
#else
	const char* utf8_str() const { return c_str(); }
	std::vector<UINT16> utf16_str();
	void FromUtf8(const char* cpUtf8) { *this = cpUtf8; }
	void FromUtf16(const UINT16* pUtf16Str, size_t len);
#endif

	void utf8_str(char *szUTF8, int nElements) const;
	void utf8_str(char **szUTF8) const;
	void utf8_str(std::string &sUTF8) const;
	/**
	 * Escape characters that will interfer with xml.
	 *
	 * @param strIn The input string to escape.
	 * @param strOut encoded for insertion into xml.
	 */
	static void EscForXml( const CString& strIn, CString& strOut);
	/**
	 * Scan doubles from a string and push them into a vector.
	 * @note: it will accept normal and scientific notation of doubles and spaces as double separator
	 *
	 * @param src source string.
	 * @param doubleVec double vector to store the results.
	 * @return int successful conversion from the string scan to doubles
	 */
	static int ScanDoublesFromString(const char* src, std::vector<double>& doubleVec,  const char* extraSeparator = NULL);
};

typedef std::pair<NCS::CString, NCS::CString> StringPair;
typedef std::list<StringPair> StringPairList;
typedef std::map<NCS::CString, NCS::CString> StringKeyValueMap;

}

NCSECW_IMPEX NCS::CString operator+(const NCS::CString& lhs, const NCS::CString& rhs);

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CString CNCSString;
#endif

#endif //__cplusplus

#endif //NCSSTRING_H
