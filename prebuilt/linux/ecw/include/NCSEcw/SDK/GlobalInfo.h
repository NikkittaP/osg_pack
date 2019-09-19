/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	GlobalInfo.h
** CREATED:	15 Dec 2006
** AUTHOR: 	Simon Cope
** PURPOSE:	Ecw stats class header for SDK instrumentation
** EDITS:
** [01] sjc	15Dec06 Created file
** 
** NOTES
**
********************************************************/

#ifndef NCSECWINFO_H
#define NCSECWINFO_H

#include "NCSDefs.h"
#include "NCSEcw/SDK/Config.h"
#include "NCSEcw/SDK/Stats.h"
#include "NCSEcw/SDK/ThreadPool.h"

#ifdef __cplusplus
extern "C" {
#endif

namespace NCS {
namespace SDK {

class NCS_EXPORT CGlobalInfo: public CMutex {
public:
	typedef enum {
		STAGE_0	= 0,  // precincts in unref'd files
		STAGE_1 = 1,  // unused precincts in ref'd files
		STAGE_2 = 2,  // indexes in unref'd files
		STAGE_3 = 3,  // indexes in ref'd files
		STAGE_4 = 4,  // unused files
		NR_STAGES = 5
	} Purge;

	CGlobalInfo();
	virtual ~CGlobalInfo();


	NCS_INLINE CConfig &Config() { return(m_Config); };
	NCS_INLINE CStats &Stats() { return(m_Stats); };
	NCS_INLINE Purge PurgeStage() { return(m_ePurgeStage); };
	NCS_INLINE void PurgeStage(Purge eStage) { m_ePurgeStage = eStage; };
private:
	CConfig m_Config;
	CStats m_Stats;
	Purge m_ePurgeStage;
};

}
}

#ifdef __cplusplus
};
#endif

#endif	// NCSECWINFO_H
