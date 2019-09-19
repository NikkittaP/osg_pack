/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     WriterTileJob.h
** CREATED:  10/01/2006 3:27:34 PM
** AUTHOR:   Yakun Wang
** PURPOSE:  CWriter class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
 *******************************************************/

#ifndef NCS_ECW_UPDATERCONTEXT_H_
#define NCS_ECW_UPDATERCONTEXT_H_

#ifndef NCSFILE_H
#include "NCSEcw/API/View.h"
#endif // NCSFILE_H


#include "NCSEcw/ECW/Compress.h"
#include "NCSEcw/SDK/Buffer2D.h"
#include "NCSMemoryIOStream.h"
#include "NCSFileIOStream.h"
#include "NCSString.h"
#include "NCSEcw/ECW/File.h"



namespace NCS {
	namespace ECW {
		/**
		 * CReader class - the JP2 file view class.
		 *
		 * @author       Yakun Wang
		 * @version      $Revision: #24 $ $Author: ywang $ $Date: 2014/03/04 $
		 */

		class CUpdater;
		class CFile;

#ifndef NCS_MOBILE
		class NCSECW_IMPEX CUpdaterContext : public CFile::IWriterContext {
		public:
			CUpdaterContext(ECW::CUpdater *pWriter, ECW::CFile &File);
			virtual ~CUpdaterContext();

			virtual void Work(void *pData);

			virtual void PrecinctDone(PrecinctOutput &Output);
			virtual void NULLPrecinctDone(NCSBlockId &nID, UINT16 nC);

			virtual void WriteStatus(const NCS::CString &sStatusText);


			virtual bool WriteError(NCS::CError eError, const NCS::CString &sErrorText);
			virtual bool Complete();

		protected:
			friend class CFile::CPrecinct;
#if (defined(WIN32) || defined(LINUX))
			NCS_DLHANDLE m_xTBBMalloc;
#endif
			void SendStatus();

			NCS::SDK::CAtomicUnsignedInt64 m_nPrecinctsCompleted;
			NCS::SDK::CAtomicUnsignedInt64 m_nBlocksCompleted;
			NCS::SDK::CAtomicUnsignedInt64 m_nNULLBlocks;
			class CBlockPart;
			class CBlockAssembler;
			CBlockAssembler *m_pBlockParts;
			ECW::CUpdater *m_pWriter;
			ECW::CFile &m_File;
			NCS::CMutex m_mCurrentStatus;
			NCS::CString m_sCurrentStatus;
			NCS::CEvent m_BlockReadyEvent;
		};
#endif

	}
}

#endif // !NCSECWWRITER_H
