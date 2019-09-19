/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     NCSEvent.h 
** CREATED:  05/12/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  NCSJPCEvent class
** EDITS:    [01] 28-02-05 RAR Shifted to NCSUtil
**           [02] 08-12-05 tfl lint fixes
 *******************************************************/

#ifndef NCSEVENT_H
#define NCSEVENT_H

#include "NCSDefs.h"
#include "NCSTypes.h"
#include "NCSMutex.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

typedef void * NCSEvent;
NCSEvent NCSEventCreateEx(BOOLEAN bManualReset, BOOLEAN bInitialState, char *pLockName);
NCSEvent NCSEventCreate(void);
BOOLEAN NCSEventSet(NCSEvent event);
BOOLEAN NCSEventReset(NCSEvent event);
BOOLEAN NCSEventWait(NCSEvent event, NCSTimeStampMs tsTimeout);
void NCSEventDestroy(NCSEvent event);

#ifdef __cplusplus
}

namespace NCS {

// Segment class
class NCSECW_IMPEX CEvent {
public:
	/*
		bManualReset [in] Specifies whether a manual-reset or auto-reset event object is created.
							If true, then you must use the Reset function to manually reset
							the state to nonsignaled. If false, the system automatically resets
							the state to nonsignaled after a single waiting thread has been released.
	*/
	CEvent(bool bManualReset = false, bool bInitialState = false, char *pLockName = NULL);
	virtual ~CEvent();
	bool Set();
	bool Reset();
	bool Wait(NCSTimeStampMs tsTimeout = -1);

private:
#ifdef _WIN32
	HANDLE m_hEvent;
#elif defined( POSIX )
	CMutex m_Mutex;
	pthread_cond_t wait_cond;
	bool m_bSignaled;
	bool m_bManualReset;
#else
	bool m_bManualReset;
	char *m_pLockName;
	CMutex m_Mutex;
	bool m_bSignalled;
#endif
};
}
#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CEvent CNCSEvent;
#endif

#endif //__cplusplus

#endif // NCSEVENT_H
