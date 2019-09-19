/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	Stats.h
** CREATED:	15 Dec 2006
** AUTHOR: 	Simon Cope
** PURPOSE:	Ecw stats class header for SDK instrumentation
** EDITS:
** [01] sjc	15Dec06 Created file
** 
** NOTES
**
********************************************************/

#ifndef NCSECWSTATS_H
#define NCSECWSTATS_H

#include "NCSDefs.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ECW_STATS_LOCK_NAME "NCS StatsLock"

	namespace NCS {
		namespace SDK {

			class NCS_EXPORT CStats {
				
			public:
				class CCounterSet;
				class NCS_EXPORT CRaw32Value {
					friend class CCounterSet;
				public:
					explicit CRaw32Value(class CRaw32Value *pParent = NULL) {
						m_nValue = 0;
						m_pValue = &m_nValue;
						m_pParent = pParent;
					};
					virtual ~CRaw32Value() {
						m_pValue = NULL;
						m_pParent = NULL;
					};
#if defined(_WIN32)
					void Init(ULONG &p) {
						m_pValue = (INT32*)&p;
					}
#endif
					NCS_INLINE INT32& operator++() {
						CRaw32Value *p = static_cast<CRaw32Value*>(this);
						while (p) {
							CStats::sIncrement(p->m_pValue, 1);
							p = p->m_pParent;
						}
						return(*m_pValue);
					};
					NCS_INLINE INT32 operator++(int) {
						INT32 v = *m_pValue;
						CRaw32Value *p = static_cast<CRaw32Value*>(this);
						while (p) {
							CStats::sIncrement(p->m_pValue, 1);
							p = p->m_pParent;
						}
						return(v);
					};
					NCS_INLINE INT32 operator+=(INT32 n) {
						CRaw32Value *p = static_cast<CRaw32Value*>(this);
						while (p) {
							CStats::sIncrement(p->m_pValue, n);
							p = p->m_pParent;
						}
						return(*m_pValue);
					};
					NCS_INLINE INT32 operator--() {
						CRaw32Value *p = static_cast<CRaw32Value*>(this);
						while (p) {
							CStats::sDecrement(p->m_pValue, 1);
							p = p->m_pParent;
						}
						return(*m_pValue);
					};

					NCS_INLINE INT32 operator--(int) {
						INT32 v = *m_pValue;
						CRaw32Value *p = static_cast<CRaw32Value*>(this);
						while (p) {
							CStats::sDecrement(p->m_pValue, 1);
							p = p->m_pParent;
						}
						return(v);
					};
					NCS_INLINE INT32 operator-=(INT32 n) {
						CRaw32Value *p = static_cast<CRaw32Value*>(this);
						while (p) {
							CStats::sDecrement(p->m_pValue, n);
							p = p->m_pParent;
						}
						return(*m_pValue);
					};
					NCS_INLINE operator const INT32&() { return(*m_pValue); };
					NCS_INLINE operator const INT32*() { return(m_pValue); };
				private:
					INT32 m_nValue;
					INT32 *m_pValue;
					class CRaw32Value *m_pParent;
				};
				class NCS_EXPORT CRaw64Value {
					friend class CCounterSet;
				public:
					//friend const INT64 operator-=(class CRaw64Value o, INT64 n);
					explicit CRaw64Value(class CRaw64Value *pParent = NULL) {
						m_nValue = 0;
						m_pValue = &m_nValue;
						m_pParent = pParent;
					};
					virtual ~CRaw64Value() {
						m_pValue = NULL;
						m_pParent = NULL;
					};
#if defined(_WIN32)&&!defined(NCS_MINDEP_BUILD)
					void Init(ULONGLONG &p) {
						m_pValue = (INT64*)&p;
					}
#endif
					NCS_INLINE CRaw64Value& operator++() {
						CRaw64Value *p = static_cast<CRaw64Value*>(this);
						while (p) {
							CStats::sIncrement64(p->m_pValue, 1);
							p = p->m_pParent;
						}
						return(*this);
					};
					NCS_INLINE INT64 operator++(int) {
						INT64 v = *this;
						CRaw64Value *p = static_cast<CRaw64Value*>(this);
						while (p) {
							CStats::sIncrement64(p->m_pValue, 1);
							p = p->m_pParent;
						}
						return(v);
					};
					NCS_INLINE CRaw64Value& operator+=(INT64 n) {
						CRaw64Value *p = static_cast<CRaw64Value*>(this);
						while (p) {
							CStats::sIncrement64(p->m_pValue, n);
							p = p->m_pParent;
						}
						return(*this);
					};
					NCS_INLINE CRaw64Value& operator--() {
						CRaw64Value *p = static_cast<CRaw64Value*>(this);
						while (p) {
							CStats::sDecrement64(p->m_pValue, 1);
							p = p->m_pParent;
						}
						return(*this);
					};
					NCS_INLINE INT64 operator--(int) {
						INT64 v = *this;
						CRaw64Value *p = static_cast<CRaw64Value*>(this);
						while (p) {
							CStats::sDecrement64(p->m_pValue, 1);
							p = p->m_pParent;
						}
						return(v);
					};
					NCS_INLINE CRaw64Value& operator-=(INT64 n) {
						CRaw64Value *p = static_cast<CRaw64Value*>(this);
						while (p) {
							CStats::sDecrement64(p->m_pValue, n);
							p = p->m_pParent;
						}
						return(*this);
					};
					NCS_INLINE operator const INT64&() { return(*m_pValue); };
					NCS_INLINE operator const INT64*() { return(m_pValue); };

				private:
					
					INT64 m_nValue;
					INT64 *m_pValue;
					class CRaw64Value *m_pParent;
				};

				class NCS_EXPORT CCounterSet{
				public: 
					class CCounterSetData;
				protected:
					CCounterSetData *m_pData;
					void RegisterCounter(unsigned long nCounterID, CRaw32Value& rawValue);
					void RegisterCounter(unsigned long nCounterID, CRaw64Value& rawValue);
				public: 
					virtual void Init() = 0;
					virtual void Fini();
					CCounterSet();
					virtual ~CCounterSet();
				};
				
				class NCS_EXPORT CCache :public CCounterSet {
				public:
					CRaw64Value Bytes;
					CRaw64Value SetViewHits;
					CRaw64Value SetViewMisses;
					CRaw64Value ReadHits;
					CRaw64Value ReadUnpackedHits;
					CRaw64Value ReadMisses;
					CRaw64Value ReadBypass;

					CRaw64Value ECWBytes;
					CRaw64Value ECWPackedBytes;
					CRaw64Value ECWUnPackedBytes;
					
					CRaw64Value SetViewECWHits;
					CRaw64Value SetViewECWMisses;
					CRaw64Value ReadECWHits;
					CRaw64Value ReadECWUnpackedHits;
					CRaw64Value ReadECWMisses;
					CRaw64Value ReadECWBypass;

					CRaw64Value JP2Bytes;
					CRaw64Value JP2PackedBytes;
					CRaw64Value JP2UnPackedBytes;
					
					CRaw64Value SetViewJP2Hits;
					CRaw64Value SetViewJP2Misses;
					CRaw64Value ReadJP2Hits;
					CRaw64Value ReadJP2UnpackedHits;
					CRaw64Value ReadJP2Misses;
					CRaw64Value ReadJP2Bypass;

					
					CRaw64Value ECWCachedPrecincts;
					CRaw64Value JP2CachedPrecincts;
					CRaw64Value ECWCachedBlocks;

					CCache();
					virtual ~CCache();
					virtual void Init() NCS_OVERRIDE;
				};
				class NCS_EXPORT CFiles :public CCounterSet {
				public:
					CRaw32Value Open;
					CRaw32Value Cached;
					CRaw32Value CacheHits;
					CRaw32Value CacheMisses;

					CRaw32Value ECWOpen;
					CRaw32Value ECWCached;
					CRaw32Value ECWCacheHits;
					CRaw32Value ECWCacheMisses;

					CRaw32Value JP2Open;
					CRaw32Value JP2Cached;
					CRaw32Value JP2CacheHits;
					CRaw32Value JP2CacheMisses;

					CFiles();
					virtual ~CFiles();
					virtual void Init() NCS_OVERRIDE;

				};
				class NCS_EXPORT CFileViews : public CCounterSet {
				public:
					CRaw32Value Open;
					CRaw32Value Cached;

					CRaw32Value ECWOpen;
					CRaw32Value ECWCached;

					CRaw32Value JP2Open;
					CRaw32Value JP2Cached;

					CFileViews();
					virtual ~CFileViews();
					virtual void Init() NCS_OVERRIDE;

				};
				class NCS_EXPORT CECWP : public CCounterSet {
				public:
					CRaw64Value RequestsSent;
					CRaw64Value CancelsSent;
					CRaw64Value BlocksReceived;
					CRaw64Value CancelledBlocksReceived;
					CRaw64Value RequestsXmitPending;
					CRaw64Value CancelsXmitPending;

					CRaw64Value ECWRequestsSent;
					CRaw64Value ECWCancelsSent;
					CRaw64Value ECWBlocksReceived;
					CRaw64Value ECWCancelledBlocksReceived;
					CRaw64Value ECWRequestsXmitPending;
					CRaw64Value ECWCancelsXmitPending;

					CRaw64Value JP2RequestsSent;
					CRaw64Value JP2CancelsSent;
					CRaw64Value JP2BlocksReceived;
					CRaw64Value JP2CancelledBlocksReceived;
					CRaw64Value JP2RequestsXmitPending;
					CRaw64Value JP2CancelsXmitPending;

					CECWP();
					virtual ~CECWP();
					virtual void Init() NCS_OVERRIDE;

				};
				class NCS_EXPORT CApps : public CCounterSet {
				public:
					CRaw32Value Open;

					CApps();
					virtual ~CApps();
					virtual void Init() NCS_OVERRIDE;
				};
				CFileViews FileViews;
				CFiles Files;
				CECWP ECWP;
				CCache Cache;
				CApps Apps;

				CStats();
				virtual ~CStats();
				void Init();
				void Fini();
			public:

				static void sIncrement(INT32 *pVal, INT32 n);
				static void sDecrement(INT32 *pVal, INT32 n);
				static void sIncrement64(INT64 *pVal, INT64 n);
				static void sDecrement64(INT64 *pVal, INT64 n);
			};
		}
	}


#ifdef __cplusplus
};
#endif

#endif	// NCSECWSTATS_H
