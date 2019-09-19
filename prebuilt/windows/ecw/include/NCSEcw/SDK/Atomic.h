/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	Atomic.h
** CREATED:	01/03/2013
** PURPOSE:	Public Atomic wrapper class
**
********************************************************/

#ifndef NCS_SDK_ATOMIC_H
#define NCS_SDK_ATOMIC_H

#ifdef _WIN32_WCE
#define NCS_DISABLE_TBB_ATOMIC
#endif

#include "NCSDefs.h"

namespace NCS {
namespace SDK {

class NCSECW_IMPEX CAtomicInt {
public:
	CAtomicInt(const int Value = 0);
	CAtomicInt(const CAtomicInt &s);
	virtual ~CAtomicInt();
	CAtomicInt &operator=(const CAtomicInt &s);
	operator int() const;
	CAtomicInt &operator=(const int &Value);
	int operator+=(const int &Value);
	int operator-=(const int &Value);
	int operator++();
	int operator--();
	int operator++(int);
	int operator--(int);
protected:
	class atomic_IMPL;
	class atomic_IMPL *m_pValue;
};

class NCSECW_IMPEX CAtomicUnsignedInt {
public:
	CAtomicUnsignedInt(const UINT32 Value = 0);
	CAtomicUnsignedInt(const CAtomicUnsignedInt &s);
	virtual ~CAtomicUnsignedInt();
	CAtomicUnsignedInt &operator=(const CAtomicUnsignedInt &s);
	operator UINT32() const;
	CAtomicUnsignedInt &operator=(const UINT32 &Value);
	UINT32 operator+=(const UINT32 &Value);
	UINT32 operator-=(const UINT32 &Value);
	UINT32 operator++();
	UINT32 operator--();
	UINT32 operator++(int);
	UINT32 operator--(int);
protected:
	class atomic_IMPL;
	atomic_IMPL *m_pValue;
};

class NCSECW_IMPEX CAtomicInt64 {
public:
	CAtomicInt64(const INT64 Value = 0);
	CAtomicInt64(const CAtomicInt64 &s);
	virtual ~CAtomicInt64();
	CAtomicInt64 &operator=(const CAtomicInt64 &s);
	operator INT64() const;
	CAtomicInt64 &operator=(const INT64 &Value);
	INT64 operator+=(const INT64 &Value);
	INT64 operator-=(const INT64 &Value);
	INT64 operator++();
	INT64 operator--();
	INT64 operator++(int);
	INT64 operator--(int);
protected:
	class atomic_IMPL;
	atomic_IMPL *m_pValue;
};

class NCSECW_IMPEX CAtomicUnsignedInt64 {
public:
	CAtomicUnsignedInt64(const UINT64 Value = 0);
	CAtomicUnsignedInt64(const CAtomicUnsignedInt64 &s);
	virtual ~CAtomicUnsignedInt64();
	CAtomicUnsignedInt64 &operator=(const CAtomicUnsignedInt64 &s);

	operator UINT64() const;
	CAtomicUnsignedInt64 &operator=(const UINT64 &Value);
	UINT64 operator+=(const UINT64 &Value);
	UINT64 operator-=(const UINT64 &Value);
	UINT64 operator++();
	UINT64 operator--();
	UINT64 operator++(int);
	UINT64 operator--(int);
protected:
	class atomic_IMPL;
	atomic_IMPL *m_pValue;
};

} // SDK
} // NCS

#endif // NCS_SDK_ATOMIC_H