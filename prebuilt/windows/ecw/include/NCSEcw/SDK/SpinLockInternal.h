#ifndef NCS_SDK_SPINLOCK_INTERNAL_H
#define NCS_SDK_SPINLOCK_INTERNAL_H

#include "NCSDefs.h"
#include "NCSEcw/SDK/AtomicInternal.h"

#if defined(_MSC_VER)

#if defined(_M_AMD64) || defined(_M_IX86)
#include <intrin.h>
#define NCSPause() _mm_pause()
#define NCSYield() SwitchToThread()
#elif defined(_WIN32_WCE)
#define NCSPause() Sleep(0)
#define NCSYield() Sleep(0)
#else
#define NCSPause() YieldProcessor()
#define NCSYield() YieldProcessor()
#endif

#elif defined(MACOSX)

#include <unistd.h>
#include <pthread.h>

#define NCSPause() usleep(1)
#define NCSYield() pthread_yield_np()

#elif defined(__GNUC__)

#ifdef ANDROID
#include <pthread.h>
#define NCSPause()
#define NCSYield()
#else
#include <sched.h>
#define NCSPause()  __asm__ __volatile__ ("pause")
#define NCSYield() sched_yield()
#endif

#elif defined(__arm__)

#ifdef __CC_ARM
#define NCSPause()  __yield()
#define NCSYield()  __yield()
#else
#include <pthread.h>
#define NCSPause()  __asm__ __volatile__ ("yield")
#define NCSYield() pthread_yield_np()
#endif

#endif

namespace NCS {
namespace SDK {

class CSpinLockImpl
{
public:
	static const UINT8 UNLOCKED = 0;
	static const UINT8 LOCKED = 1;

	CSpinLockImpl() : m_MutexState(UNLOCKED) { };
	~CSpinLockImpl() { unlock(); };

	void unlock() {
		m_MutexState.store(UNLOCKED);
	}

	void lock() {
		int nI = 0;
		for(;;) {
			UINT8 State = m_MutexState.load();
			if(State == UNLOCKED && m_MutexState.compare_exchange_strong(State, LOCKED)) {
				return;
			}
			if(nI++ < 16) {
				NCSPause();
			} else {
				NCSYield();
			}
		}
	}

	bool try_lock() {
		UINT8 State = m_MutexState.load();
		if(State == UNLOCKED && m_MutexState.compare_exchange_strong(State, LOCKED)) {
			return true;
		}
		return false;
	}

	class CScopedLock {
	public:
		explicit CScopedLock(CSpinLockImpl &Lock) : m_pLock(&Lock) {
			m_pLock->lock();
		}
		explicit CScopedLock(CSpinLockImpl *pLock) : m_pLock(pLock) {
			m_pLock->lock();
		}
		virtual ~CScopedLock() {
			m_pLock->unlock();
		}
	private:
		CSpinLockImpl *m_pLock;
	};


	CSpinLockImpl(const CSpinLockImpl &s) { *this = s; };
	const CSpinLockImpl &operator=(const CSpinLockImpl &s) { m_MutexState.store(m_MutexState.load()); return *this; };
private:

	atomic_UINT8 m_MutexState;
};

class CSpinRWLockImpl
{
public:
	static const int UNLOCKED = 0;
	static const int WRITE_LOCKED = 1;
	static const int READ_LOCKED = 2;

	static const int FLAG_NONE = 0;
	static const int FLAG_WRITE_LOCK_REQUESTED = 1;

	class CScopedLock {
	public:
		CScopedLock(CSpinRWLockImpl &Lock, bool bWrite = true) {
			acquire(Lock, bWrite);
		}
		CScopedLock(CSpinRWLockImpl *pLock, bool bWrite = true) {
			acquire(pLock, bWrite);
		}
		virtual ~CScopedLock() {
			release();
		}
		
		void acquire(CSpinRWLockImpl *pLock, bool bWrite = true) {
			m_pLock = pLock;
			if(bWrite) {
				m_pLock->lock();
			} else {
				m_pLock->lock_read();
			}
		}
		void acquire(CSpinRWLockImpl &Lock, bool bWrite = true) {
			m_pLock = &Lock;
			if(bWrite) {
				m_pLock->lock();
			} else {
				m_pLock->lock_read();
			}
		}

		void release() { 
			if(m_pLock) {
				m_pLock->unlock(); 
				m_pLock = NULL;
			}
		};
	protected:
		CSpinRWLockImpl *m_pLock;
	};

	CSpinRWLockImpl() : m_MutexState(UNLOCKED), m_WriteLockRequestedFlag(FLAG_NONE) { };
	~CSpinRWLockImpl() { /*unlock();*/ };

	void unlock() {
		int State = m_MutexState.load();
		
		if((State&WRITE_LOCKED) != 0) {
			m_MutexState.fetch_sub(WRITE_LOCKED);
		} else {
			m_MutexState.fetch_sub(READ_LOCKED);
		}
	}

	void lock() {
		unsigned int nCount = 0;
		for(;;) {
			int Unlocked = UNLOCKED;
			int State = m_MutexState.load();
			if(State == Unlocked && m_MutexState.compare_exchange_weak(Unlocked, WRITE_LOCKED)) {
				m_WriteLockRequestedFlag = FLAG_NONE;
				return;
			}
			if(nCount++ < 16) {
				NCSPause();
			} 
			else {
				NCSYield();
				nCount = 0;
				
			}

			int FlagNone = FLAG_NONE;
			int FlagLockRequested = FLAG_WRITE_LOCK_REQUESTED;
			m_WriteLockRequestedFlag.compare_exchange_weak(FlagNone, FlagLockRequested);
		}
	}

	void lock_read() {
		int nCount = 0;
		for(;;) {
			int State = m_MutexState.load();
			if((State&WRITE_LOCKED) == 0) {
				if (m_WriteLockRequestedFlag.load() == FLAG_NONE) {
					int OldState = m_MutexState.fetch_add(READ_LOCKED);
					if ((OldState & WRITE_LOCKED) == 0) {
						return;
					}
					else {
						m_MutexState.fetch_sub(READ_LOCKED);
					}
				}
			}
			if(nCount++ < 16) {
				NCSPause();
			} else {
				NCSYield();
				nCount = 0;
			}
		}
	}

	bool try_lock() {
		int Unlocked = UNLOCKED;
		if(m_MutexState.compare_exchange_strong(Unlocked, WRITE_LOCKED)) {
			return true;
		}
		return false;
	}

	bool try_lock_read() {
		if (m_WriteLockRequestedFlag.load() == FLAG_NONE) {
			int State = m_MutexState.load();
			int Unlocked = UNLOCKED;
			if (m_MutexState.compare_exchange_strong(Unlocked, READ_LOCKED)) {
				return true;
			}
			else if ((State & (~WRITE_LOCKED)) != 0) {
				if (m_MutexState.compare_exchange_strong(State, State + READ_LOCKED)) {
					return true;
				}
			}
		}
		return false;
	}

	bool upgrade_to_writer() {
		int OneReader = READ_LOCKED;
		if(m_MutexState.compare_exchange_strong(OneReader, WRITE_LOCKED)) {
			return false;
		}
		unlock();
		lock();
		return true;
	}

	void downgrade_to_reader() {
		m_MutexState.fetch_add(WRITE_LOCKED);
	}

	CSpinRWLockImpl(const CSpinRWLockImpl &s) { *this = s; };
	const CSpinRWLockImpl &operator=(const CSpinRWLockImpl &s) { m_MutexState.store(m_MutexState.load()); return *this; };
private:
	atomic_INT32 m_MutexState;
	atomic_INT32 m_WriteLockRequestedFlag;
};

} // SDK
} // NCS

#endif // NCS_SDK_SPINLOCK_INTERNAL_H
