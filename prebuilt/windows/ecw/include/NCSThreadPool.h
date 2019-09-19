/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSThreadPool.h
** CREATED:	2005
** AUTHOR: 	Russell Rogers
** PURPOSE:	NCS Thread Pool Class header.
** EDITS:
** [01] sjc 30Jan06 Moved to include, cleanup headers
**
 ********************************************************/

#if !defined(NCSTHREADPOOL_H)
#define NCSTHREADPOOL_H

#include "NCSUtil.h"
#include "NCSEvent.h"
#include "NCSString.h"

#include <vector>
#include <list>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251)
#endif

namespace NCS {
	class CWaitableJobsContext;
class NCSECW_IMPEX CThreadPool
{
public:
	class NCSECW_IMPEX CJob
	{
	public:
		CJob() { m_bIsRunning = false; m_bRun = true; m_bQueued = false; m_bDeleteThisJobOnceDone = false; m_bNeedsReQueued = false; m_bIsAwaitingRequeuing = false; };
		virtual ~CJob() { m_bIsRunning = false; m_bRun = false; m_bQueued = false; m_bDeleteThisJobOnceDone = false; }

		virtual void Work() = 0;
		virtual bool IsRunning();
		virtual bool Run() { return(m_bRun); };
		virtual void SetRunning(bool bRunning) { m_bIsRunning = bRunning; };
		virtual void SetRun(bool bRun) { m_bRun = bRun; };
		virtual bool IsQueued();
		virtual void SetQueued(bool bQueued) { m_bQueued = bQueued; };
		virtual void SetNeedsRequeued(bool bNeedsRequeued) { m_bNeedsReQueued = bNeedsRequeued; };
		virtual bool IsAwaitingRequeuing() { return m_bIsAwaitingRequeuing; };
		virtual void SetIsAwaitingRequeuing(bool bIsAwaitingRequeuing) { m_bIsAwaitingRequeuing = bIsAwaitingRequeuing; };
		virtual bool NeedsRequeued() { return m_bNeedsReQueued; };
		virtual bool IsQueuedOrRunning();
		//Used for identification of job types and their thread pools. 
		virtual int GetType(){ return 0; }
		bool IsWorker() { 
			return false; //(sm_tls_bIsWorker); 
		};
		void IsWorker(bool b) { 
			;//sm_tls_bIsWorker = b; 
		};

		void LockStatus();
		void UnLockStatus();
		bool DeleteJob() { return m_bDeleteThisJobOnceDone; };
	protected:
		bool	m_bIsRunning;
		bool	m_bRun;
		bool	m_bQueued;
		bool	m_bNeedsReQueued;
		bool	m_bIsAwaitingRequeuing;
		bool	m_bDeleteThisJobOnceDone;
		CMutex m_mStatus;
		//static CThread::T_Local<bool> sm_tls_bIsWorker;
	};
	class NCSECW_IMPEX CWaitableJob : public NCS::CThreadPool::CJob {
	public:
		CWaitableJob() :CJob(), m_bDone(false), m_bRet(false){
		};
		virtual ~CWaitableJob() {
		};
		virtual bool DoWork() = 0;
		virtual void Work(void) NCS_OVERRIDE{
			m_bRet = DoWork();
			m_bDone = true;
		};
		virtual bool Done() { return(m_bDone); };
		virtual bool Result() { return(m_bRet); };
		virtual bool Wait() {
			bool bRet = true;
			LockStatus();
			while (!Done() || IsRunning()) {
				UnLockStatus();
				bRet = m_Event.Wait();
				LockStatus();
			}
			UnLockStatus();
			return(bRet);
		};
		virtual void SetRunning(bool bRunning) {
			NCS::CThreadPool::CJob::SetRunning(bRunning);
			if (!bRunning) {
				m_Event.Set();
			}
		};
	private:
		bool							m_bDone;
		bool							m_bRet;
		NCS::CEvent						m_Event;

	};
	class NCSECW_IMPEX CWaitableJobs {
		public:
			CWaitableJobs();
			virtual ~CWaitableJobs();
			void AddJob(CWaitableJob *p);
			bool Wait(void);
		private:
			CWaitableJobsContext *m_pContext;
	};

	CThreadPool( UINT32 nMaxThreads = 10, bool bStartThreads = false, bool bPoolIsJobOwner = true,
		bool bCheckIfAlreadyQueued=false, bool bCheckIfRunning=false );
	
	virtual ~CThreadPool();

	bool QueueJob( CJob *pJob, bool bHighPriority=false );
	bool CancelJob( CJob *pJob);
	CJob *GetNextJob();
	INT32 GetQueueSize();
	bool IsWorking();
	void CancelJobs();
	virtual void StopWorking();
	virtual void ResumeWorking() { m_bThreadPoolRunning = true; };
	virtual bool IsThreadPoolRunning() { return m_bThreadPoolRunning; };
	virtual bool AreAnyThreadsRunning();

	INT32 GetThreadCount() { return(m_nThreadCount); };
	bool SetThreadCount(INT32 nThreadCount);
	INT32 GetMaxThreadCount() { return((INT32)m_Threads.size()); };
	virtual void SetName(const NCS::CString &sName);

	/*
	 * If you don't want to use the default Job List you can over-ride the follow
	 * three methods.
	 */
    virtual void WorkFinished( CJob *pJob );
protected:
	virtual void Wait(NCSTimeStampMs nWaitTimeMS) { m_ResumeEvent.Wait(nWaitTimeMS); };
    virtual bool QueueJobInt( CJob *pJob, bool bHighPriority );
    virtual CJob *GetNextJobInt();

	void ResumeThread();
	void StartThread();

	class NCSECW_IMPEX CWorkThread : public CThread
	{
	public:
		explicit CWorkThread(CThreadPool *pThreadPool);

		virtual ~CWorkThread();

		virtual void Work( void *pData );
	protected:
		CThreadPool *m_pThreadPool;
	};
	
	// Job list
	typedef std::list<CJob *> CJobList;
	typedef CJobList::iterator CJobListIterator;

	CJobList m_JobList;
	CMutex m_mJobList;

	CJobList m_RunningJobList;
	CMutex m_mRunningJobList;

	// Worker threads
	CEvent m_ResumeEvent;

	typedef std::vector<CWorkThread *> CWorkThreadList;
	typedef CWorkThreadList::iterator CWorkThreadListIterator;

	CWorkThreadList m_Threads;
	CMutex m_mThreads;
	UINT32 m_nThreadCount;
	INT32 m_nWorkingThreads;
	bool m_bPoolIsJobOwner;
	bool m_bCheckIfAlreadyQueued;
	bool m_bCheckIfRunning;
	bool m_bThreadPoolRunning;
	static const NCSTimeStampMs s_tsMaxWaitTime;
	NCS::CString m_sThreadPoolName;
};
class NCSECW_IMPEX CSchedulingThreadPool :public NCS::CThreadPool{
public:
	static const int JobType = 335583068;
	class NCSECW_IMPEX CJob : public CThreadPool::CJob{
	public:
		virtual NCSTimeStampMs GetEarliestSchedule() = 0;
		
		virtual int GetType() NCS_OVERRIDE{
			return CSchedulingThreadPool::JobType;
		}
	};
public:
	CSchedulingThreadPool(UINT32 nMaxThreads = 10, bool bStartThreads = false, bool bPoolIsJobOwner = true,
		bool bCheckIfAlreadyQueued = false, bool bCheckIfRunning = false) :CThreadPool(nMaxThreads, bStartThreads, bPoolIsJobOwner,
		bCheckIfAlreadyQueued, bCheckIfRunning){
	}
	virtual void Wait(NCSTimeStampMs nWaitTimeMS) NCS_OVERRIDE;
	virtual CThreadPool::CJob *GetNextJobInt() NCS_OVERRIDE;
};
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CThreadPool CNCSThreadPool;
typedef NCS::CThreadPool::CJob CNCSThreadPoolJob;
#define CNCSThreadPoolJob CJob
#endif

#endif // !defined(NCSTHREADPOOL_H)
