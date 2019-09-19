/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:   	NCSMutex.h
** CREATED:	Thu Feb 25 09:19:00 WST 1999
** AUTHOR: 	Simon Cope
** PURPOSE:	NCS Mutex handling.
** EDITS:
** [01] 03Nov00	 ny	Merged WinCE/PALM SDK changes
** [02] 12Apr01	 ny	Added copy constructor for CMutex
 *******************************************************/

#ifndef NCSMUTEX_H
#define NCSMUTEX_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SWIG

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif
#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif
#ifndef NCSTIMESTAMP_H
#include "NCSTimeStamp.h"
#endif

#ifdef POSIX
#include <pthread.h>
#endif

/*
** Defines.
*/
#ifdef NOTDEF
#ifdef _WIN32

#define NCS_NULL_MUTEX { 0, 0, 0, 0, 0, 0 }

#elif defined POSIX

#define NCS_NULL_MUTEX  PTHREAD_MUTEX_INITIALIZER 

#else

ERROR: Define NCS_NULL_MUTEX for this platform

#endif /* _WIN32 */
#endif

typedef void *NCSMutex;

#define NCS_NULL_MUTEX (void*)NULL

/*
** Prototypes.
*/
void NCSMutexInit(NCSMutex *pMutex);
void NCSMutexFini(NCSMutex *pMutex);
void NCSMutexBegin(NCSMutex *pMutex);
BOOLEAN NCSMutexTryBegin(NCSMutex *pMutex);
void NCSMutexEnd(NCSMutex *pMutex);
#ifdef POSIX
pthread_mutex_t *NCSMutexGetNative(NCSMutex *pMutex);
#endif

/* Global (SYSTEM Wide) Locks */
#define NCS_GLOBAL_LOCK_NAME	"NCS GlobalLock"

#endif // SWIG

#ifdef __cplusplus
}

#ifndef NCS_NO_UTIL_LINK

namespace NCS {

// CMutex
class NCSECW_IMPEX CMutex {
public:
	CMutex();
	virtual ~CMutex();

	virtual void Lock(void);
	virtual bool TryLock(void);
	virtual void UnLock(void);
	
#ifdef POSIX
	pthread_mutex_t *GetNative();
#endif

private:
	class CMutexContext;
	CMutexContext *m_pContext;

};

#ifndef SWIG

// CMutexLock
class NCSECW_IMPEX CMutexLock {

private:
	CMutex	*m_pMutex;
public:
	explicit CMutexLock(CMutex *pmMutex) 
	{
		if( pmMutex ) {
			m_pMutex = pmMutex;
			m_pMutex->Lock();
		} else {
			m_pMutex = NULL;
		}
	}
#ifndef SWIG
	explicit CMutexLock(CMutex &mMutex) 
	{
		m_pMutex = &mMutex;
		m_pMutex->Lock();
	}
#endif
	virtual ~CMutexLock() {
		if( m_pMutex ) {
			m_pMutex->UnLock();
		}
	}
};

class NCSECW_IMPEX CGlobalLock {
public:
	CGlobalLock(const char *sName = NCS_GLOBAL_LOCK_NAME, bool bProcessLocal = false);
	virtual ~CGlobalLock();
private:
#ifdef _WIN32
	HANDLE	m_hLock;
#else
	static CMutex sm_mMutex;
#endif

	bool Lock(const char *sName);
	bool UnLock();
};
#endif // SWIG

}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CMutex CNCSMutex;
typedef NCS::CMutexLock CNCSMutexLock;
typedef NCS::CGlobalLock CNCSGlobalLock;
#endif

#endif //NCS_NO_UTIL_LINK

#endif
#endif /* NCSMUTEX_H */
