/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	ECWP3Client.h
** CREATED:	01/03/2012
** PURPOSE:	Internal ECWP3 client API
**
********************************************************/

#ifndef ECWP3_CLIENT_H
#define ECWP3_CLIENT_H

#include "NCSUtil.h"
#include "NCSThread.h"
#include "NCSThreadPool.h"
#include "NCSMultiLock.h"

#include "NCScnet.h"
#include "NCSMd5.h"

#include "NCSMemoryIOStream.h"

#include <vector>
#include <list>

#include "NCSEcw/SDK/ecwp3packets.h"

#include "NCSECWClient.h"

//#define NCS_USE_CURL

#ifdef NCS_USE_CURL
typedef void CURL;
#endif

#define ECWP3_CLIENT_VERSION 1
#define MANUAL_LL

namespace NCS { namespace SDK {
	class CECWP3Client;
	class CBlockFile;
} }

namespace NCS {
namespace SDK {

class NCS_EXPORT CECWP3Client {
    CError GetResponseError(NCS::CMemoryIOStream &Stream);
public:
	class NCS_EXPORT IECWP3ClientApp {
	public:
		virtual void BlockRecv(NCSBlockId nID, void *pBlock, UINT32 nBlockLength, NCSError eError) = 0;
		virtual void BlockFailed(NCSBlockId nID, NCSError eError) = 0;
	};
	struct OpenInfo{
		CString sUrl;
#if defined ECWP_CACHE
		CString sLastModified;
		UINT8 * pCachedResponse;
		INT32 nCachedResponseLength;
		bool bCachedResponseStillValid;
		bool bNeedsFreeMemory;
#endif 
	};

	class CPacket {
	public:
		CPacket() : m_ePacketType(), m_nServerVersion(0), m_nPacketSize(0) {};
		virtual ~CPacket() {};
		virtual bool Read(CMemoryIOStream &Stream) {
			UINT16 nPreamble = 0;
			return(Stream.Read((void*)&nPreamble, sizeof(UINT16)) && nPreamble == ECWP3_PACKET_PREAMBLE &&
					Stream.Read((void*)&m_ePacketType, sizeof(UINT8)) &&
					Stream.Read((void*)&m_nServerVersion, sizeof(UINT8)) &&
					Stream.Read((void*)&m_nPacketSize, sizeof(UINT32)));
		}
		NCSecwp3PacketType m_ePacketType;
		UINT8 m_nServerVersion;
		UINT32 m_nPacketSize;
		static UINT16 m_nPreamble;
	};

	class CErrorResponsePacket : public CPacket {
	public:
		CErrorResponsePacket() : CPacket(), m_eErrorType(), m_nData(0) {};
		virtual ~CErrorResponsePacket() {};

		bool Read(CMemoryIOStream &Stream) {
			if(CPacket::Read(Stream) && m_ePacketType == ECWP3_ERROR_RESPONSE) {
				return(Stream.Read((void*)&m_eErrorType, sizeof(UINT8)) &&
						Stream.Read((void*)&m_nData, sizeof(UINT32)));
			}
			return false;
		}
		NCSecwp3ErrorType m_eErrorType;
		UINT32 m_nData;

		static CErrorResponsePacket *Parse(CMemoryIOStream &Stream) {
			CErrorResponsePacket *pPacket = new CErrorResponsePacket();
			if(pPacket) {
				if(pPacket->Read(Stream)) {
					return pPacket;
				}
				delete pPacket;
			}
			return NULL;
		}
	};

	class CGetResponsePacket : public CPacket {
	public:
		CGetResponsePacket() : CPacket(), m_pBlock(NULL), m_nBlockID(0), m_nBlockSize(0) {};
		virtual ~CGetResponsePacket() {
			if(m_pBlock) {
				NCSFree(m_pBlock);
			}
		}

		bool Read(CMemoryIOStream &Stream) {
			if(CPacket::Read(Stream) && m_ePacketType == ECWP3_GET_RESPONSE) {
				bool bRet = false;
				if(Stream.Read((void*)&m_nBlockID, sizeof(UINT32)) && 
					Stream.Read((void*)&m_nBlockSize, sizeof(UINT32))) {
					if (m_nBlockSize > 0 ){
						m_pBlock = NCSMalloc(m_nBlockSize, FALSE);
						bRet = Stream.Read((void*)m_pBlock, m_nBlockSize);
					}else{
						//NULL block is perfectly fine. 
						m_pBlock = NULL;
						bRet = true;
					}
				}
				return bRet;
			}
			return false;
		}
		static CGetResponsePacket *Parse(CMemoryIOStream &Stream) {
			CGetResponsePacket *pPacket = new CGetResponsePacket();
			if(pPacket) {
				if(pPacket->Read(Stream)) {
					return pPacket;
				}
				delete pPacket;
			}
			return NULL;
		}
		void *m_pBlock;
		UINT32 m_nBlockID;
		UINT32 m_nBlockSize;
	};

	class COpenResponsePacket : public CPacket {
	public:
		COpenResponsePacket() : CPacket(), m_nCID(0), m_pHeader(NULL), m_nHeaderSize(0), m_eFileType(ECWP3_FILETYPE_UNKNOWN), m_pRPCData(NULL) {};
		virtual ~COpenResponsePacket() {
			if(m_pRPCData) {
				NCSFree(m_pRPCData);
			}
			// header memory is managed by parent class
		}

		bool Read(CMemoryIOStream &Stream) {
			if(CPacket::Read(Stream) && m_ePacketType == ECWP3_OPEN_RESPONSE) {
				UINT8 eFileType = 0;
				UINT32 nStatsSize = 0;
				UINT32 nMetaDataSize = 0;
				UINT32 nRPCDataSize = 0;

				if(!Stream.Read((void*)&m_nCID, sizeof(UINT64))) {
					return false;
				}
				if(!Stream.Read((void*)&eFileType, sizeof(UINT8))) {
					return false;
				}
				m_eFileType = (NCSecwp3FileType)eFileType;
				if(!Stream.Read((void*)&m_nHeaderSize, sizeof(UINT32))) {
					return false;
				}
				m_pHeader = NCSMalloc(m_nHeaderSize, FALSE);
				if(!Stream.Read((void*)m_pHeader, m_nHeaderSize)) {
					return false;
				}
				
				if(!Stream.Read((void*)&nStatsSize, sizeof(UINT32))) {
					return false;
				}
				if(nStatsSize > 0) {
					UINT16* uint16Stats = (UINT16 *)NCSMalloc(nStatsSize+1, TRUE);
					if(!Stream.Read((void*)uint16Stats, nStatsSize)) {
						NCSFree(uint16Stats);
						return false;
					}
					m_sStatistics.clear();	
					m_sStatistics.FromUtf16(uint16Stats, nStatsSize/2);
					NCSFree(uint16Stats);	
				}

                
				if(!Stream.Read((void*)&nMetaDataSize, sizeof(UINT32))) {
					return false;
				}
				if(nMetaDataSize > 0) {
                    UINT16* uint16MetaData = (UINT16 *)NCSMalloc(nMetaDataSize+1, TRUE);
                    
                    if(!Stream.Read((void*)uint16MetaData, nMetaDataSize)) {
                        NCSFree(uint16MetaData);
                        return false;
                    }
                    
                    m_sMetaData.clear();
                    m_sMetaData.FromUtf16((UINT16*)uint16MetaData, nMetaDataSize/2);
                    NCSFree(uint16MetaData);
				}

				if(!Stream.Read((void*)&nRPCDataSize, sizeof(UINT32))) {
					return false;
				}
				if(nRPCDataSize > 0 && nRPCDataSize == sizeof(NCSRPCData)) {
					m_pRPCData = static_cast<NCSRPCData *>(NCSMalloc(sizeof(NCSRPCData), TRUE));
					if(!Stream.Read((void*)m_pRPCData, nRPCDataSize)) {
						return false;
					}
				}
				return true;
			}
			return false;
		}
		UINT64 m_nCID;
		void *m_pHeader;
		UINT32 m_nHeaderSize;
		NCSecwp3FileType m_eFileType;
		NCS::CString m_sMetaData;
		NCS::CString m_sStatistics;
		NCSRPCData *m_pRPCData;

		static COpenResponsePacket *Parse(CMemoryIOStream &Stream) {
			COpenResponsePacket *pPacket = new COpenResponsePacket();
			if(pPacket) {
				if(pPacket->Read(Stream)) {
					return pPacket;
				}
				delete pPacket;
			}
			return NULL;
		}
	};

	class CReconnectResponsePacket : public CPacket {
	public:
		CReconnectResponsePacket() : CPacket(), m_nCID(0) {};
		virtual ~CReconnectResponsePacket() {};

		bool Read(CMemoryIOStream &Stream) {
			if(CPacket::Read(Stream) && m_ePacketType == ECWP3_RECONNECT_RESPONSE) {
				return(Stream.Read((void*)&m_nCID, sizeof(UINT64)));
			}
			return false;
		}
		UINT64 m_nCID;

		static CReconnectResponsePacket *Parse(CMemoryIOStream &Stream) {
			CReconnectResponsePacket *pPacket = new CReconnectResponsePacket();
			if(pPacket) {
				if(pPacket->Read(Stream)) {
					return pPacket;
				}
				delete pPacket;
			}
			return NULL;
		}
	};

	class CCloseResponsePacket : public CPacket {
	public:
		CCloseResponsePacket() : CPacket(), m_nSuccess(0) {};
		virtual ~CCloseResponsePacket() {};

		bool Read(CMemoryIOStream &Stream) {
			if(CPacket::Read(Stream) && m_ePacketType == ECWP3_CLOSE_RESPONSE) {
				return(Stream.Read((void*)&m_nSuccess, sizeof(UINT8)));
			}
			return false;
		}
		UINT8 m_nSuccess;

		static CCloseResponsePacket *Parse(CMemoryIOStream &Stream) {
			CCloseResponsePacket *pPacket = new CCloseResponsePacket();
			if(pPacket) {
				if(pPacket->Read(Stream)) {
					return pPacket;
				}
				delete pPacket;
			}
			return NULL;
		}
	};

	CECWP3Client(IECWP3ClientApp *pApp, NCS::NET::IHttpRequestFactory *pRequestFactory);
	CECWP3Client(const CECWP3Client &rhs);
	virtual ~CECWP3Client();

	const CECWP3Client &operator=(const CECWP3Client &rhs);
	CError Open(OpenInfo& openInfo);
	bool IsConnected() { return m_bIsConnected; };
	NCS::CError Reconnect();
	bool Close();

	bool Request(std::list<NCSBlockId> &BlockIDs);
	bool Request(std::vector<CPacketId> *pBlocks);
	bool Request(CPacketId nID);
	bool Cancel(CPacketId nID);
	bool SendRequests();

	bool IsAuthenticated() 
	{ 
		if (m_bAuthenticatedUser) return true;
		else return false;
	}
	int GetResponseCode () {return m_nResponseCode; };
	void *GetHeader() { return m_pHeader; };
	int GetHeaderSize() { return m_nHeaderSize; };

	const CString &Statistics() { return m_sStatistics; };
	const CString &MetaData() { return m_sMetaData; };
	const NCSRPCData *RPCData() { return m_pRPCData; };

	static INT32 GetQueueSize();

	NCSecwp3FileType GetFileType() { return m_eFileType; };

	void AbleToCancelCurrentDownloads(bool bEnable) { m_bCancelCurrentDownloadsEnabled = bEnable; };

	static void Init();
	static void Fini();

	static bool SetMaxConcurrentDownloads(INT32 nThreadCount);
	// added on 13/02/2013
	void SetNumBlocksToRequest(size_t nBlocksToRequest);
protected:

	NCS::CError OpenInternal(OpenInfo &OpenInfo, const NCS::CString &sVirtualPath);

	class NCS_EXPORT CConnectionSwarm : public NCS::CThread {
	public:

		class NCS_EXPORT CConnection : public NCS::CThreadPool::CJob {
		public:
			CConnection();
			explicit CConnection(NCS::CString &sServer);
			explicit CConnection(NCS::CHashKey &Key);
			CConnection(const CConnection &s);
			virtual ~CConnection();

			virtual CConnection &operator=(const CConnection &s);

			void Enqueue(NCS::CString &sURL, CECWP3Client *pClient, NCSecwp3ServerCommand eCurrentRequestType);
			NCS::CHashKey Key() { return m_Key; };
			void Work();
			static int GetRequestCB(void *szNew, unsigned long nSize, void *szAllData, void *pUserData);
			bool IsAlive() { return m_bIsAlive; };			

			typedef std::map<NCS::CPacketId, bool> CGetBlockChecklist;
			CGetBlockChecklist *m_pGetBlockReceived;
			CGetBlockChecklist *m_pGetBlockCancel;
			
			NET::HttpConnectionCacheContext *m_ConnectionCacheContext;
			CECWP3Client *m_pClient;
			NCS::CString m_sURL;

			NCSecwp3ServerCommand m_eCurrentRequestType;

			bool CancelBlock(NCS::CPacketId ID);
			bool AbortDownloadCheck();
			bool Alive() { return m_bIsAlive; };

			void Touch() { m_tsLastAccess = NCSGetTimeStampMs(); };
			NCSTimeStampMs LastAccess() { return m_tsLastAccess; }; 
		protected:
			HINTERNET m_hSession;
			HINTERNET m_hConnection;
			NCS::CHashKey m_Key;

			NCS::CMutex m_BlockMutex;
			
			bool m_bIsAlive;
			NCSTimeStampMs m_tsLastAccess;
		};

		explicit CConnectionSwarm(int nSwarmSizePerServer);
		virtual ~CConnectionSwarm();

		CConnection *GetConnection(NCS::CString &sServer);
		void ReturnConnection(CConnection *Connection);

		void Work(void *pData);

		bool SetEvent() {
			return(m_Event.Set()); 
		};

		virtual bool Stop(bool bWait = true);

		bool Wait(NCSTimeStampMs tsTimeout = -1) {
			return(m_Event.Wait(tsTimeout)); 
		};
		bool Shutdown(bool bWait = true) {
			SetEvent();
			return(Stop(bWait)); 
		};

	protected:
		int m_nSwarmSizePerServer;
		class CConnectionQueue;
		class CConnectionMap;
		CConnectionMap *m_pConnections;

		CEvent	m_Event;
	};
	//CConnectionSwarm::CConnection *GetNextPacketsJob(size_t nMaxSize);
	void SignalDownloader();
	CConnectionSwarm::CConnection *GetConnection();
	void ReturnConnection(CConnectionSwarm::CConnection *pConnection);
	virtual CConnectionSwarm::CConnection *GetNextPacketsJob();
	std::vector<CConnectionSwarm::CConnection *> m_CurrentDownloadJobs;

	
	NCS::CMutex m_mCurrentDownloadJobs;

	class CDownloadScheduler;

	static const NCSTChar *s_pAcceptTypes[];

	

	class CDownloadScheduler : public NCS::CThreadPool
	{
	public:
		CDownloadScheduler( UINT32 nMaxThreads = 10, bool bStartThreads = false, bool bPoolIsJobOwner = true,
			bool bCheckIfAlreadyQueued=false, bool bCheckIfRunning=false );
		virtual ~CDownloadScheduler();

		bool UpdateThreadCount(INT32 nThreadCount);
		void RegisterClient(CECWP3Client *pClient);
		void UnRegisterClient(CECWP3Client *pClient);
		void GetServerNames(std::list<NCS::CString> &ServerNames);

		virtual inline void WorkFinished( CJob *pJob );

		virtual void Set() { NCS::CThreadPool::m_ResumeEvent.Set(); };
	protected:
		virtual inline bool QueueJobInt( CJob *pJob, bool bHighPriority );
		virtual inline CJob *GetNextJobInt();

		std::vector<CECWP3Client *> m_Clients;
		NCS::CMultiLock m_Mutex;
		size_t m_nClient;
		NCS::CMutex m_mClient;
#ifdef WIN32
		__forceinline void NextClientIndex(size_t &nI);
#else
		inline void NextClientIndex(size_t &nI) __attribute__((always_inline));

#endif
		// moved to ECWP3 client
		//size_t m_nBlocksToRequest;

	};

	static CDownloadScheduler *s_pDownloadSchedular;
	static CConnectionSwarm *s_pConnectionSwarm;
	static bool s_bWininetInitialised;

	bool ParseURL();
	void SetConnected(bool bIsConnected) { m_bIsConnected = bIsConnected; };
	bool m_bIsConnected;
	bool m_bIsOpen;

	void *m_pHeader;
	UINT32 m_nHeaderSize;
	bool m_bAuthenticatedUser;  //Whether or not the user has supplied authentication
	INT32 m_nResponseCode;
	UINT64 m_nClientID;
	UINT32 m_nServerVersion;
	
	NCS::CMultiLock m_mMutex;
	NCS::CMultiLock m_mReconnectMutex;


	std::list<CPacketId> m_RequestBlocks;
	
	NCS::CMutex m_mRequestBlocks;

	CString m_sStatistics;
	CString m_sMetaData;
	NCSRPCData *m_pRPCData;

	CString m_sProtocol;
	CString m_sImageURL;
	CString m_sServerName;
	CString m_sServerURL;
	CString m_sLayerPath;

	CString m_sUsername;
	CString m_sPassword;
	CString m_sCredentials;

	bool m_bAnonymous;
	bool m_bUseSSL;
	bool m_bCancelCurrentDownloadsEnabled;

	static NCS::CMutex s_mMutex;

	NCSecwp3FileType m_eFileType;

	IECWP3ClientApp *m_pApp;

	// due to server's limit set to 50 at CECWP3::CClient m_nMaxGetSize = 50
	// we may have to limit inputed value
	UINT32 m_nMaxBlocksToRequest; 
	size_t m_nBlocksToRequest;

	NCSTimeStampMs m_tsLastReconnectAttempt;
	bool m_bHeaderHasChangedOnServer;
#ifdef USE_LOCAL_BLOCK_FILE
	NCS::SDK::CBlockFile *m_pBlockFile;
#endif
	NCS::NET::IHttpRequestFactory *m_pRequestFactory;

	NCS::NET::IHttpRequestFactory *GetRequestFactory();
};

} // SDK
} // NCS

#endif
