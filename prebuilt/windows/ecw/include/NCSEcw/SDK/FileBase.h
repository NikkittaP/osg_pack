/********************************************************
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
**
** FILE:     FileBase.h $
** CREATED:  28/11/2002 3:27:34 PM
** AUTHOR:   Simon Cope
** PURPOSE:  CFile class header
** EDITS:    [xx] ddMmmyy NAME COMMENTS
**			 [01] 03Aug04 tfl  Added integer member to NCSJP2PCSBox
**			 [02] 07Dec04 tfl  Moved NCSGeodataUsage enum declaration to NCSECWClient.h
**			 [03] 17Dec04 tfl  Added static CFile::Shutdown() function called by CFile::Shutdown()
 *******************************************************/

#ifndef NCSFILEBASE_H
#define NCSFILEBASE_H

#ifndef NCSJP2DEFS_H
#include "NCSEcw/JPC/Types/Defs.h"
#endif // NCSJP2DEFS_H

#ifndef NCSECWCLIENT_H
#include "NCSECWClient.h"
#endif // NCSECWCLIENT_H

// Our own 64bit IOSTREAM class
#ifndef NCSIOSTREAM_H
#include "NCSIOStream.h"
#endif // NCSIOSTREAM_H

#ifndef NCSJPCBuffer2D_H
#include "NCSEcw/SDK/Buffer2D.h"
#endif //NCSJPCBuffer2D_H

#ifndef NCSJPCNode2D_H
#include "NCSEcw/SDK/Node2D.h"
#endif //NCSJPCNode2D_H

#ifndef NCSMUTEX_H
#include "NCSMutex.h"
#endif // NCSMUTEX_H

#ifndef NCSMULTILOCK_H
#include "NCSMultiLock.h"
#endif // NCSMULTILOCK_H

#include "NCSEcw/API/Transform.h"
#include "NCSEcw/SDK/GlobalInfo.h"

#ifndef NCS_SDK_ATOMIC_H
#include "NCSEcw/SDK/Atomic.h"
#endif

#define ECW_COMPRESS_SDK_LIMIT_MASK			0xfe43218a

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4251 4275)
#endif

namespace NCS {
class CView;

namespace SDK {
class CFileClient;

class NCSECW_IMPEX CFileClientJob : public NCS::CThreadPool::CJob {
public:
	CFileClientJob();
	explicit CFileClientJob(CFileClient *pClient);
	virtual ~CFileClientJob();
	CFileClientJob(const CFileClientJob &s);
	const CFileClientJob &operator=(const CFileClientJob &s);
	void operator<<(CFileClient *pClient);

	virtual void Work();

	CFileClient *Client() { return m_pClient; };
protected:
	CFileClient *m_pClient;
};

class NCSECW_IMPEX CFileClientAccess {
public:
	CFileClientAccess();
	explicit CFileClientAccess(CFileClient *pClient);
	virtual ~CFileClientAccess();
	CFileClientAccess(const CFileClientAccess &s);
	const CFileClientAccess &operator=(const CFileClientAccess &s);
	void operator<<(CFileClient *pClient);

	CFileClient *Client() { return m_pClient; };
protected:
	CFileClient *m_pClient;
};

/**
 * CFileBase class - the JP2 file class.
 * Note, the JP2File class is a superbox!
 * The JP2File is the base shared file object used by multiple CFile classes to 
 * access a JP2 file.
 * 
 * @author       Simon Cope
 * @version      $Revision: #3 $ $Author: ctapley $ $Date: 2017/12/03 $ 
 */
#ifndef SWIG
class NCSECW_IMPEX CFileBase: public CMutex, public NCS::CThreadPool::CJob {
public:
	/**
	 * CFileFactory class 
	 * The CFileFactory is a vector containing all the CFile object instances.
	 * There is only one instance of this class.
	 * 
	 * @author       Simon Cope
	 * @version      $Revision: #3 $ $Author: ctapley $ $Date: 2017/12/03 $ 
	 */
#ifndef SWIG
	class NCSECW_IMPEX CFileFactory: public std::vector<CFileBase *>, public CThread {
	public:
			/** Default constructor, initialises ECW library */
		CFileFactory();
			/** Virtual destructor, clean up */
		virtual ~CFileFactory();

			/** 
			* Open method - returns existing instance if already open
			* @param		ppFile			Returned CFileBase instance pointer.
			* @param		pURLPath		Local/remote path/URL.
			* @return      CError		NCS_SUCCESS or error code on failure.
			*/
		CError Open(CFileClient *pClient, const CString &URLPath, bool bWrite, const CString &sECWPHttpHeaders);

			/** 
			* Open method - returns existing instance if already open
			* @param		ppFile			Returned CFileBase instance pointer.
			* @param		pStream			Stream to open file from.
			* @return      CError		NCS_SUCCESS or error code on failure.
			*/
		CError Open(CFileClient *pClient, CIOStream *pStream);

			/** Close */
		CError Close(CFileClient *pClient, bool bFreeCache = false);

		CError CloseAll();

		bool OverBudget(float factor = 1.0);
		virtual INT64 GetDecompressionBytes();
		virtual INT64 GetCompressionBytes();

		void Purge(float factor = 1.0);
		NCS_INLINE CGlobalInfo &GlobalInfo() { return(m_EcwInfo); };

		bool QueueJob(NCS::CThreadPool::CJob *pJob);
		bool CancelJob(NCS::CThreadPool::CJob *pJob);
		bool IsThreadPoolRunning() { return(m_pWorkThreadPool && m_pWorkThreadPool->IsThreadPoolRunning()); };

		virtual void StartWorkers();
		virtual void StopWorkers();

		virtual void SignalPurger();

		NCS::CError PurgeUnusedMemory();

		virtual void LineRead();
	private:
		virtual void Work(void *p);
		virtual CError RemoveFile(CFileBase *pFile);

		NCS::CThreadPool *m_pWorkThreadPool;

		CGlobalInfo m_EcwInfo;
		CEvent		m_Event;
		CMutex		m_PurgeMutex;

		NCSTimeStampMs m_tsLastCloseDelayTime;
		class AtomicUINT32;
		AtomicUINT32 *m_pLineRead;
	};
#endif //SWIG
		/** ECW FileInfo Structure */
	NCSFileInfo m_FileInfo;
		/** Number of refs to this file */
	INT32				m_nRefs;

	std::vector<class CFileClient *>	m_Clients;

		/** IOStream */
	CIOStream *m_pStream;
		/** Did we allocate the stream? */
	bool	m_bOurStream;
		/** Currently processing ReceiveBlock() */
	bool	m_bInReceiveBlock;

		/** 
		 * Get the memory image of the file.
		 * @param		pLength		Returned length of image
		 * @return      void *		NCSMalloc()'d pointer to image.
		 */
	virtual void *GetMemImage(UINT32 &nLength) = 0;
		/** 
		 * Get length of a packet in the file.
		 * @param		nPacket		Absolute Packet number to determine length for
		 * @return      UINT32		Packet length or 0 on failure.
		 */
	virtual UINT32 GetPacketLength(NCSBlockId nPacket) = 0;
		/** 
		 * Read a packet from the file.
		 * @param		nPacket		Absolute Packet number to determine length for
		 * @param		pLength		Returned length of packet (header+data)
		 * @return      void *		NCSMalloc()'d pointer to read packet (header+data).
		 */
	virtual void *GetPacket(NCSBlockId nPacket, UINT32 &nLength) = 0;
		/** 
		 * Get number of a packet in the file.
		 * @return      UINT32		Number of Packets or 0 on failure.
		 */
	virtual UINT32 GetNrPackets() = 0;
#ifndef NCS_MOBILE
		/** 
		 * Set the compression parameters (dimensions, registration, bitdepth etc).
		 * @param		Info		FileInfo structure
		 * @return      CError	NCS_SUCCESS, or error on failure 
		 */
	virtual CError SetCompress(NCSFileInfo &Info) = 0;
		/** 
		 * Write the next line in BIL format into the file.
		 * @param		nCtx				Unique Context ID
		 * @param		pSrc				Source buffer
		 * @param		iComponent			Component index
		 * @return      bool				true on success, else false.
		 */
	virtual bool WriteLine(CNode2D::ContextID nCtx, CBuffer2D *pSrc, UINT16 iComponent) = 0;
#endif
		/** 
		 * Enable unlimited compression - NOTE YOU MUST COMPLY WITH THE EULA.
		 */
	static void SetOEMKey(const char *szCompanyName, const char *szKey);
	static bool GetOEMKey(char **szCompanyName, char**szKey);

#ifdef NCS_MOBILE
	static void SetOEMMobileDecodeKey(const char *szCompanyName, const char *szKey);
	static bool GetOEMMobileDecodeKey(char **szCompanyName, char**szKey);
#endif

	virtual bool OverBudget(float factor = 1.0);
		/** 
		 * Purge any unused buffers, codeblocks etc if memory useage exceeds acceptable limites
		 */
	virtual void Purge();

	virtual NCSFileType GetFileType() = 0;

	virtual NCSFileInfo *GetFileInfo() = 0;

#ifndef SWIG
	private:
		class NCSECW_IMPEX CFileFactoryHelper
		{
		public: 
			CFileFactoryHelper();
			virtual ~CFileFactoryHelper();
			void Init();
			void Fini();
			CFileFactory* Get();
		protected:
			CFileFactory *m_FileFactory;
		};
		static CFileFactoryHelper& sFilesHelper();
	public: 
	NCS_INLINE static CFileFactory &sFiles() {
		return *sFilesHelper().Get(); 
	};
    NCS_INLINE static void Init() {
		sFilesHelper().Init();
	}
	NCS_INLINE static void Fini(){
		sFilesHelper().Fini();
	}
	NCS_INLINE static SDK::CGlobalInfo &sGlobalInfo() {	return(sFiles().GlobalInfo()); };
	NCS_INLINE static SDK::CConfig &sConfig() { return(sFiles().GlobalInfo().Config()); };
	NCS_INLINE static SDK::CStats &sStats() { return(sFiles().GlobalInfo().Stats()); };
#endif

		/** 
		* Open method - returns existing instance if already open
		* @param		ppFile			Returned CFileBase instance pointer.
		* @param		pURLPath		Local/remote path/URL.
		* @return      CError		NCS_SUCCESS or error code on failure.
		*/
	static CError sOpen(CFileClient *pClient, const CString &URLPath, bool bWrite, const CString &sECWPHttpHeaders=NCS::CString());

		/** 
		* Open method - returns existing instance if already open
		* @param		ppFile			Returned CFileBase instance pointer.
		* @param		pStream			Stream to open file from.
		* @return      CError		NCS_SUCCESS or error code on failure.
		*/
	static CError sOpen(CFileClient *pClient, CIOStream *pStream);

		/** Close */
	static CError sClose(CFileClient *pClient, bool bFreeCache = false);

	virtual void ReceivedBlock(NCSBlockId nBlock, UINT8* pImage, UINT32 nBlockLength) = 0;


	virtual CError Reconnect();

// ONLY HERE UNTIL C++ify ECW
			/** Static list of files */
	CFileFactory &Files() { return sFiles(); };
	void RemoveClient(CFileClient *pClient);
	void PushClient(CFileClient *pClient);

	std::vector<NCS::API::CMinMaxTransform *> m_DefaultTransforms;

protected:

	virtual void Work();
	NCS::CMultiLock m_mClients;
		/** Unlimited compression key - see SetKeySize() */
	static char *sm_szOEMKey;
	static char *sm_szCompanyName;
#ifdef NCS_MOBILE
	static char *sm_szMobileDecodeKey;
	static char *sm_szMobileDecodeCompanyName;
#endif

	friend class CFileFactory;
	friend class NCS::CView;

			/** Default constructor, initialises members */
	CFileBase();
		/** Virtual Destructor */
	virtual ~CFileBase();

		/** 
		 * Open the JP2 file from the specified stream.
		 * @param		Stream		Stream of JP2 file to open
		 * @return      CError	NCS_SUCCESS or error code on failuer.
		 */
	virtual CError Open(CIOStream *pStream);
		/** 
		 * Close the JP2 file.
		 * @param		bFreeCache	Free the cache & delete file object if last close
		 * @return      CError	NCS_SUCCESS or error code on failuer.
		 */
	virtual CError Close();

		/** 
		 * Purge unused memory at specified purgestage
		 * @param		eStage		Purge stage to free memory at
		 */
	virtual void PurgeMemory(CGlobalInfo::Purge eStage, float factor = 1.0) = 0;
#if !defined(ECWP2_DISABLED)
	virtual CIOStream *CreateBlockStream(const CString &URLPath, const CString &HttpHeaders, bool bIsECWP3=true) = 0;
#else
    virtual CIOStream *CreateBlockStream(const CString &URLPath, const CString &HttpHeaders) = 0;
#endif
	virtual CError GetDefaultTransform(NCSCellType eType, UINT8 nBits, API::CMinMaxTransform **pTransform);

	virtual void GetClients(std::vector<CFileClientJob *> &Clients);
	virtual void GetClients(std::vector<CFileClientJob> &Clients);
	virtual CFileClientJob *GetClientJob(class CFileClient *pFileClient);
public:
	virtual void GetClients(std::vector<CFileClientAccess> &Clients);
	virtual bool QueueClient(class CFileClient *pFileClient);
	virtual bool CancelClient(class CFileClient *pFileClient);
		/** Last access for this file */
	NCS::SDK::CAtomicInt64	m_tsLastAccess;  // Used in FileBaseFactory to Purge older files first
};
#endif

class NCSECW_IMPEX CFileClient {
protected:
	CThreadPool::CJob *m_pInternalWorker;
public:
	CFileClient() { 
		m_pFile = NULL;  
		m_pInternalWorker = NULL;
		m_bIsOpen = false;
		m_nWorkScheduleCount = 0;
	};
	virtual ~CFileClient() { 
		if(m_pFile) {
			CFileBase::sClose(this, true); 
		}
	};
	CFileClient(const CFileClient &s) {
		*this = s;
	}
	const CFileClient &operator=(const CFileClient &s) {
		m_pFile = s.m_pFile;
		m_pInternalWorker = s.m_pInternalWorker;
		m_bIsOpen = s.m_bIsOpen;
		m_nWorkScheduleCount = s.m_nWorkScheduleCount;
		m_nFactoryAccessCount = s.m_nFactoryAccessCount;

		return *this;
	}

	NCS::CThreadPool::CJob *GetJob() {
		return m_pInternalWorker;
	}

	virtual void Queue() {
		if(m_pInternalWorker) {

			for(; ;) {
				if(!m_bIsOpen) {
					break;
				}
				m_pInternalWorker->LockStatus();
				if(m_pInternalWorker->IsRunning()) {
					m_pInternalWorker->SetNeedsRequeued(true);
					m_pInternalWorker->UnLockStatus();
					break;
				} else if(m_pInternalWorker->IsQueued()) {
					m_pInternalWorker->UnLockStatus();
					break;
				} else {
					m_pInternalWorker->UnLockStatus();
					if(m_pFile->QueueClient(this) || !CFileBase::sFiles().IsThreadPoolRunning()) {
						break;
					}
				}
			}
		}
	}

		/** 
		 * Get underlying CFileBase pointer.
		 * @return      CFileBase*	Return pointer to the CFileBase instance.
		 */
#ifndef SWIG
	CFileBase *GetFile() { return(m_pFile); };
	void SetFile(CFileBase *p) { m_pFile = p; };
#endif
	bool Set() { return(m_Event.Set()); };
	bool Reset() { return(m_Event.Reset()); };
	bool Wait(NCSTimeStampMs tsTimeout = -1) { return(m_Event.Wait(tsTimeout)); };

	virtual bool ReceivedBlock(NCSBlockId nBlock) {return(false);};

	virtual void Work() = 0;

	bool IsOpen() { return m_bIsOpen; };
	
	NCS::SDK::CAtomicInt m_nWorkScheduleCount;
	NCS::SDK::CAtomicInt m_nFactoryAccessCount;
protected:

	bool m_bIsOpen;
	
	bool QueueClient() {
		if(m_pInternalWorker && m_pFile) {
			return m_pFile->QueueClient(this);
		}
		return false;
	}
	bool CancelClient() {
		if(m_pInternalWorker && m_pFile) {
			return m_pFile->CancelClient(this);
		}
		return false;
	}
	bool IsQueuedOrWorking() {
		if(m_pInternalWorker) {
			return m_pInternalWorker->IsQueuedOrRunning();
		}
		return false;
	}

#ifndef SWIG
		/** Event - fired when something happens */
	CEvent	m_Event;
	CFileBase *m_pFile;

#endif //SWIG
};

} // SDK
} // NCS


#ifdef _MSC_VER
#pragma warning(pop)
#endif

#endif // !NCSJP2FILE_H
