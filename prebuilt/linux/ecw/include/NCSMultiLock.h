/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSPolygon.h
** CREATED:	29-Dec-2008
** AUTHOR: 	Russell Rogers
** PURPOSE:	Interface for shared locks.
*******************************************************/

#if !defined(NCSMULTILOCK_H)
#define NCSMULTILOCK_H

#include "NCSUtil.h"
#include "NCSMutex.h"
#include "NCSEvent.h"

#include <map>

namespace NCS {

class NCSECW_IMPEX CMultiLock
{
public:
	CMultiLock();
	virtual ~CMultiLock();

	virtual void LockShared();
	virtual bool TryLockShared();
	virtual void UnLockShared();
	virtual bool LockExclusive();
	virtual bool TryLockExclusive();
	virtual bool ConvertToExclusive();
	virtual void ConvertToShared();
	virtual void UnLockExclusive();
	
	virtual INT32 GetSharedCount() const;
	virtual bool IsLockedExclusively() const;

protected:
	CMutex m_mSharedCount;
	INT32 m_nSharedCount;

	CMutex m_Mutex;
	CEvent m_OK2WriteEvent;
	bool m_bLockedExclusively;
};

class NCSECW_IMPEX CSafeMultiLock : public CMultiLock
{
public:
	CSafeMultiLock();
	virtual ~CSafeMultiLock();

	virtual void LockShared();
	virtual bool TryLockShared();
	virtual void UnLockShared();
	virtual bool LockExclusive();
	virtual bool TryLockExclusive();
	virtual bool ConvertToExclusive();
	virtual void ConvertToShared();
	virtual void UnLockExclusive();

protected:
	class CSharedTIDMap;

	CSharedTIDMap *m_pSharedTID;
};

class NCSECW_IMPEX CSharedLock
{
public:
	explicit CSharedLock( CMultiLock *pMultiLock )
	{
		m_pMultiLock = pMultiLock;
		m_pMultiLock->LockShared();
	}
	explicit CSharedLock( CSafeMultiLock *pMultiLock )
	{
		m_pMultiLock = static_cast<CMultiLock*>(pMultiLock);
		m_pMultiLock->LockShared();
	}
	explicit CSharedLock( CMultiLock &MultiLock )
	{
		m_pMultiLock = &MultiLock;
		m_pMultiLock->LockShared();
	}
	explicit CSharedLock( CSafeMultiLock &MultiLock )
	{
		m_pMultiLock = static_cast<CMultiLock*>(&MultiLock);
		m_pMultiLock->LockShared();
	}
	virtual ~CSharedLock()
	{
		Release();
	}
	void Release() {
		if(m_pMultiLock) {
			m_pMultiLock->UnLockShared();
			m_pMultiLock = NULL;
		}
	}

private:
	CMultiLock *m_pMultiLock;

};

class NCSECW_IMPEX CExclusiveLock
{
public:
	explicit CExclusiveLock( CMultiLock *pMultiLock )
	{
		m_pMultiLock = pMultiLock;
		m_pMultiLock->LockExclusive();
	}
	explicit CExclusiveLock( CSafeMultiLock *pMultiLock )
	{
		m_pMultiLock = static_cast<CMultiLock*>(pMultiLock);
		m_pMultiLock->LockExclusive();
	}
	explicit CExclusiveLock( CMultiLock &MultiLock )
	{
		m_pMultiLock = &MultiLock;
		m_pMultiLock->LockExclusive();
	}
	explicit CExclusiveLock( CSafeMultiLock &MultiLock )
	{
		m_pMultiLock = static_cast<CMultiLock*>(&MultiLock);
		m_pMultiLock->LockExclusive();
	}
	virtual ~CExclusiveLock()
	{
		Release();
	}
	void Release() {
		if(m_pMultiLock) {
			m_pMultiLock->UnLockExclusive();
			m_pMultiLock = NULL;
		}
	}
private:
	CMultiLock *m_pMultiLock;

};

class NCSECW_IMPEX CMultiLockClient {
public:
	explicit CMultiLockClient(NCS::CMultiLock &Lock) : m_Lock(Lock)
	{
		m_Lock.LockShared();
		m_bIsShared = true;
	}

	explicit CMultiLockClient(NCS::CSafeMultiLock &Lock) : m_Lock(Lock)
	{
		m_Lock.LockShared();
		m_bIsShared = true;
	}

	bool UpgradeToExclusive() {
		bool bUnlocked = false;
		if (m_bIsShared) {
			bUnlocked = m_Lock.ConvertToExclusive();
			m_bIsShared = false;
		}
		return bUnlocked;
	}

	void DowngradeToShared() {
		if (!m_bIsShared) {
			m_bIsShared = true;
			m_Lock.ConvertToShared();
		}
	}

	virtual ~CMultiLockClient() {
		if (m_bIsShared) {
			m_Lock.UnLockShared();
		}
		else {
			m_Lock.UnLockExclusive();
		}
	}
protected:
	NCS::CMultiLock &m_Lock;
	bool m_bIsShared;
};

}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CMultiLock CNCSMultiLock;
typedef NCS::CExclusiveLock CNCSExclusiveLock;
typedef NCS::CSharedLock CNCSSharedLock;
#endif

#endif // !defined(NCSMULTILOCK_H)
