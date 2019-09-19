/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSThread.h
** CREATED:	Fri Feb 26 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Thread handling.
** EDITS:
** [01] 03Jan01	 ny	Added CThread wrapper class
** [02] 02May01	 ny	Added GetSysID() method
 *******************************************************/

#ifndef NCSTHREAD_H
#define NCSTHREAD_H

#ifndef NCSTTYPES_H
#include "NCSTypes.h"
#endif

#ifdef _WIN32
#ifndef _WIN32_WCE
#include <process.h>
#endif /* _WIN32_WCE */
#include <windows.h>
#ifdef _WIN32_WINNT
#include <winbase.h>
#endif
#elif defined POSIX
#include <pthread.h>
#endif

#ifndef SWIG

#ifndef NCSTIMESTAMP_H
#include "NCSTimeStamp.h"
#endif
#ifndef NCSMALLOC_H
#include "NCSMalloc.h"
#endif
#ifndef NCSMUTEX_H
#include "NCSMutex.h"
#endif
#include "NCSEvent.h"
#include "NCSString.h"
#endif // !SWIG

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SWIG

typedef enum {
	NCS_THREAD_PRI_IDLE				= 0,
	NCS_THREAD_PRI_BELOW_NORMAL			= 1,
	NCS_THREAD_PRI_NORMAL				= 2,
	NCS_THREAD_PRI_ABOVE_NORMAL			= 3,
	NCS_THREAD_PRI_REALTIME				= 4
} NCSThreadPriority;

typedef void *NCSThread;
#define NCS_NULL_THREAD_ID (void*)NULL

#if defined(_WIN32)
typedef UINT32 NCSThreadLSKey;
#elif defined(POSIX)
typedef pthread_key_t NCSThreadLSKey;
#else
#error: define NCSThreadLSKey for this platform
#endif

#ifndef POSIX
void NCSShowExceptionDialog( BOOLEAN bShow );
BOOLEAN NCSGetThreadExceptionStatus();
void NCSResetThreadExceptionStatus();
void NCSRethrowCaughtException( BOOLEAN bRethrow );
#endif

BOOLEAN NCSThreadSpawn(NCSThread *pThread, void (*pFunc)(void*), void *pData, BOOLEAN bCreateSuspended);
//void NCSThreadFreeInfo(NCSThread *pThread);
void NCSThreadExit(INT32 dwExitId);
void NCSThreadSuspend(void);
void NCSThreadResume(NCSThread *pThread);
//NCSThread *NCSThreadGetCurrent(void);
BOOLEAN NCSThreadIsRunning(NCSThread *pThread);
BOOLEAN NCSThreadIsSuspended(NCSThread *pThread);
BOOLEAN NCSThreadRun(NCSThread *pThread);
BOOLEAN NCSThreadSetName(NCSThread *pThread, const char *szName);
BOOLEAN NCSThreadYield(void);
BOOLEAN NCSThreadSetPriority(NCSThread *pThread, NCSThreadPriority pri);
NCSThreadPriority NCSThreadGetPriority(NCSThread *pThread);
BOOLEAN NCSThreadTerminate(NCSThread *pThread);

NCSThreadLSKey NCSThreadLSAlloc(void);
void NCSThreadLSFree(NCSThreadLSKey Key);
void NCSThreadLSSetValue(NCSThreadLSKey Key, void *pValue);
void *NCSThreadLSGetValue(NCSThreadLSKey Key);

#endif //SWIG

#ifdef __cplusplus
}

#ifndef NCS_NO_UTIL_LINK

#ifdef POSIX
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#endif
#include <list>
#include <algorithm>

namespace NCS {

class NCSECW_IMPEX CThreadID {
public:
	CThreadID();
	CThreadID(const CThreadID &s);
	virtual ~CThreadID();

	CThreadID &operator=(const CThreadID &s);
	operator size_t() const;
	bool operator==(const CThreadID &s) const;
	bool operator<(const CThreadID &s) const;
protected:
#if defined(WIN32)
	DWORD m_PlatformThreadIdImpl;
#elif defined(IOS) || defined(MACOSX)
	uint64_t m_PlatformThreadIdImpl;
#elif defined(POSIX)
	pthread_t m_PlatformThreadIdImpl;
#else
#error platform not implemented
#endif
};

// CThread
class NCSECW_IMPEX CThread: public CMutex {
public:
	static CThreadID GetCurrentThreadID();

	typedef enum {
		PRI_IDLE = 0,
		PRI_BELOW_NORMAL = 1,
		PRI_NORMAL = 2,
		PRI_ABOVE_NORMAL = 3,
		PRI_REALTIME = 4
	} Priority;

	void *m_pData;

	CThread();
	virtual ~CThread();
	virtual bool Spawn(void *pData=NULL, bool bCreateSuspended=false);
	
	virtual void Exit(INT32 dwExitId);
	virtual void Suspend(void);
	virtual void Resume();
	//virtual NCSThread *GetCurrent(void);
	virtual bool IsRunning();
	virtual bool IsSuspended();
	virtual bool ThreadYield(void);
	virtual bool SetPriority(Priority pri);
	virtual bool SetName(const NCS::CString &sName);
	virtual Priority GetPriority();
	virtual bool Terminate();
	virtual bool Run();
	virtual bool Wait( NCSTimeStampMs tsWait );
	virtual bool Stop(bool bWait = true);
	virtual bool AreThread();
	virtual void Work(void *pData) = 0;

//	virtual NCSThread GetID(void);

#ifdef _WIN32
	virtual DWORD GetSysID(void);
#elif defined POSIX
	virtual pthread_t *GetSysID(void);
#endif

protected:
	friend void ::NCSThreadExit(INT32 dwExitId);

#ifdef _WIN32
	DWORD			m_dwTID;
	HANDLE			m_hThread;

#elif defined(POSIX)
	
	pthread_t		m_thread;
	pthread_cond_t	m_suspend_condition;
	bool			m_bSuspended;
	bool			m_bThreadStarted;
    CMutex			m_mSuspendMutex;

#else 
ERROR: define NCS::CThread for this platform
#endif

	bool		m_bRunning;
	bool		m_bRun;
	CEvent	m_evtShutDown;
	NCS::CString m_sThreadName;

	static void sThreadFunc(void *pDataParam);
	void FreeInfo();
	virtual bool ApplyThreadName();
};

}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CThread CNCSThread;
#endif

#endif //NCS_NO_UTIL_LINK


#endif // __cplusplus

#endif /* NCSTHREAD_H */
