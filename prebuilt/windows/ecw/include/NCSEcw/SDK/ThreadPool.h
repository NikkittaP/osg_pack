/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	ThreadPool.h
** CREATED:	10 Jan 2006
** AUTHOR: 	Simon Cope
** PURPOSE:	NCSJPC Thread pool class
** EDITS:
*******************************************************/
/** @file NCSEcw/SDK/ThreadPool.h */

#ifndef NCSSDKTHREADPOOL_H
#define NCSSDKTHREADPOOL_H

#include "NCSThreadPool.h"

namespace NCS {
namespace SDK {

/** @class CThreadPool
 *	@brief This class a thread pool class for NCSJPC derived from CThreadPool.
 */
	class NCSECW_IMPEX CThreadPool: public NCS::CThreadPool {
public:
	INT32	m_nRefs;
	CThreadPool();
	virtual ~CThreadPool();
};

}
}

#endif /* NCSSDKTHREADPOOL_H */
