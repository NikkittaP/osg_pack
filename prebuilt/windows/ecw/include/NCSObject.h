/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     NCSObject.h
** CREATED:  3/09/2001 11:46:57 AM
** AUTHOR:   Simon Cope
** PURPOSE:  Base CObject header.
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCSOBJECT_H
#define NCSOBJECT_H

#ifndef NCSTYPES_H
#include "NCSTypes.h"
#endif // NCSTYPES_H

#ifndef NCSDEFS_H
#include "NCSDefs.h"
#endif // NCSDEFS_H

#ifndef NCSTIMESTAMP_H
#include "NCSTimeStamp.h"
#endif // NCSTIMESTAMP_H

/**
 * CObject class.  This class tracks the objects creation time, and if logging is set to LOG_LEVEL3
 * then stats about the objects lifetime are logged with a DEBUG build.
 * 
 * @author       Simon Cope
 * @version      $Revision: #1 $ $Author: ctapley $ $Date: 2017/08/28 $ 
 *               
 * @since        2.0
 */
namespace NCS {

class NCSECW_IMPEX CObject {
public:
#ifndef SWIG
	class NCSECW_IMPEX CLife {
	public:
		NCS_INLINE CLife(NCSTimeStampMs &tsLife) {
			m_tsCreated = NCSGetTimeStampMs();
			m_ptsLife = &tsLife;
		};
		NCS_INLINE CLife(NCSTimeStampMs *ptsLife = NULL) {
			m_tsCreated = NCSGetTimeStampMs();
			m_ptsLife = ptsLife;
		};
		NCS_INLINE virtual ~CLife() {
			if(m_ptsLife) {
				*m_ptsLife += NCSGetTimeStampMs() - m_tsCreated;
			}
		};
		NCS_INLINE NCSTimeStampMs CreatedAt(void) {
			return(m_tsCreated);
		};
		NCS_INLINE NCSTimeStampMs TimeToNow(void) {
			return(NCSGetTimeStampMs() - m_tsCreated);
		};
	protected:
		NCSTimeStampMs m_tsCreated;
		NCSTimeStampMs *m_ptsLife;
	};
#endif
	/**
	 * Default Constructor.  Writes the current timestamp at construction time to the log file
	 * if logging set to LOG_LEVEL3 (DEBUG build only).
	 * 
	 * @see          #~CObject()
	 */
	CObject();
	/**
	 * Destructor.  Writes the objects lifetime in ms to the log file if loggin set to LOG_LEVEL3
	 * (DEBUG build only).
	 * 
	 * @see          #CObject()
	 */
	~CObject();

	/**
	 * Get the timestamp for when the objects was constructed.
	 * 
	 * @return       NCSTimeStampMs		Objects construction timestamp.
	 * @see          #TimeToNow()
	 * @see			 #~CObject()
	 */
	NCSTimeStampMs CreatedAt(void);
	/**
	 * Get the objects current lifetime, in ms (ie, current time - construction time in ms).
	 * 
	 * @return       NCSTimeStampMs		Objects current lifetime in ms.
	 * @see          #CreatedAt()
	 * @see			 #~CObject()
	 */
	NCSTimeStampMs TimeToNow(void);

private:
	/**
	 * ObjectLife object - this does the actual work.
	 * 
	 * @see          #CObject()
	 * @see			 #CreatedAt()
	 * @see			 #TimeToNow()
	 */
	//CLife		*m_pLife; 
	// This was changed by Russell.
	// It is only a temp solution but I had a good reason.
	// Ask me before changing back

	/**
	 * Time stamp of when the object was constructed.
	 * 
	 * @see          #CObject()
	 * @see			 #CreatedAt()
	 * @see			 #TimeToNow()
	 */
	NCSTimeStampMs m_tsCreate;
};
}

#ifndef NCS_NO_COMPAT_NAMES
typedef NCS::CObject CNCSObject;
#endif

#endif // NCSOBJECT_H

