/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	spin_lock.h
** CREATED:	01/03/2012
** PURPOSE:	Internal spin lock wrapper class
**
********************************************************/

#ifndef NCS_SDK_SPIN_LOCK_H
#define NCS_SDK_SPIN_LOCK_H

#include "NCSDefs.h"

namespace NCS {
namespace SDK {

class CSpinLockImpl;
class CSpinRWLockImpl;

class NCSECW_IMPEX CSpinLock {
public:
	CSpinLock();
	CSpinLock(const CSpinLock &s);
	virtual ~CSpinLock();

	const CSpinLock &operator=(const CSpinLock &s);

	bool try_lock();
    void lock();
    void unlock();

	class NCSECW_IMPEX CScopedLock {
	public:
		explicit CScopedLock(CSpinLock &Lock) : m_pLock(&Lock) {
			m_pLock->lock();
		}
		explicit CScopedLock(CSpinLock *pLock) : m_pLock(pLock) {
			m_pLock->lock();
		}
		virtual ~CScopedLock() {
			m_pLock->unlock();
		}
	protected:
		CSpinLock *m_pLock;
	};
	
protected:
	CSpinLockImpl *m_pLock;
};

class NCSECW_IMPEX CSpinRWLock {
public:
	CSpinRWLock();
	CSpinRWLock(const CSpinRWLock &s);
	virtual ~CSpinRWLock();

	const CSpinRWLock &operator=(const CSpinRWLock &s);

	bool try_lock(bool bWrite);
    void lock(bool bWrite);
    void unlock(bool bWrite);

	bool try_lock();
	bool try_lock_read();
	void lock();
	void lock_read();
	void unlock();

	bool upgrade_to_writer();
	void downgrade_to_reader();

	class NCSECW_IMPEX CScopedLock {
	public:
		CScopedLock(CSpinRWLock &Lock, bool bWrite) : m_pLock(&Lock), m_bWrite(bWrite) {
			m_pLock->lock(m_bWrite);
		}
		CScopedLock(CSpinRWLock *pLock, bool bWrite) : m_pLock(pLock), m_bWrite(bWrite) {
			m_pLock->lock(m_bWrite);
		}
		virtual ~CScopedLock() {
			m_pLock->unlock(m_bWrite);
		}
	protected:
		CSpinRWLock *m_pLock;
		bool m_bWrite;
	};
protected:
	CSpinRWLockImpl *m_pLock;
};

} // SDK
} // NCS

#endif // NCS_SDK_SPIN_LOCK_H
