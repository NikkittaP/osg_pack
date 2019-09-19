/********************************************************** 
** Copyright, 1998 - 2014, Intergraph Corporation. All rights reserved.
** 
** FILE:   	MemoryPool.h
** CREATED:	01/03/2012
** PURPOSE:	Internal Memory pooling classes.  You have to be very
**			careful with memory pooling as there are only a few
**			cases where we are better than the runtime.  Also,
**			if we are better than the msvc runtime in one instance,
**			we still may be slower than the linux runtime.
**
********************************************************/

#include "NCSEcw/SDK/Buffer2D.h"
#include "NCSEcw/SDK/stl_concurrent_queue.h"

namespace NCS {

class CFixedSizeMemoryPool
{
public:
	CFixedSizeMemoryPool() 
		: m_nWidth(0), 
		  m_nHeight(0), 
		  m_eType(SDK::CBuffer2D::BT_IEEE4), 
		  m_nMaxBuffersInQueue(1024),
		  m_pMemoryCounter(&SDK::CBuffer2D::sm_nAllocCompressionBytes) {};

	CFixedSizeMemoryPool(UINT32 nWidth, UINT32 nHeight, SDK::CBuffer2D::Type eType, size_t nMaxBuffersInQueue, NCS::SDK::CAtomicUnsignedInt64 *pMemoryCounter) 
		: m_nWidth(0), 
		  m_nHeight(0), 
		  m_eType(SDK::CBuffer2D::BT_IEEE4), 
		  m_nMaxBuffersInQueue(1024),
		  m_pMemoryCounter(&SDK::CBuffer2D::sm_nAllocCompressionBytes)
	{
		Init(nWidth, nHeight, eType, nMaxBuffersInQueue, pMemoryCounter);
	}
	void Init(UINT32 nWidth, UINT32 nHeight, SDK::CBuffer2D::Type eType, size_t nMaxBuffersInQueue, NCS::SDK::CAtomicUnsignedInt64 *pMemoryCounter)
	{
		m_nWidth = nWidth;
		m_nHeight = nHeight;
		m_eType = eType;
		m_nMaxBuffersInQueue = nMaxBuffersInQueue;
		m_pMemoryCounter = pMemoryCounter;

		INT32 nTypeSize = SDK::CBuffer2D::Size(m_eType);
		m_nStride = nTypeSize * m_nWidth;
		m_nStride = NCSCeilDiv(m_nStride, NCS::SDK::CBuffer2D::s_nMemoryAlignment) * NCS::SDK::CBuffer2D::s_nMemoryAlignment; // original
		m_nBufferSize = m_nStride * m_nHeight;
		m_nStride /= nTypeSize;

		m_Queue.set_capacity(m_nMaxBuffersInQueue);
	};
	virtual ~CFixedSizeMemoryPool() 
	{
		Purge();
	}
	void *Get() 
	{		
		void *pBuffer = NULL;
		if(!m_Queue.try_pop(pBuffer)) {
			//pBuffer = _aligned_malloc(m_nBufferSize, 128);
#if defined(WIN32) && !defined(_WIN32_WCE)
			pBuffer = _aligned_malloc(m_nBufferSize, 16);
#elif defined(NCS_MOBILE)
			pBuffer = NCSMalloc(m_nBufferSize, TRUE);
#elif defined(POSIX)
			posix_memalign(&pBuffer, 16, m_nBufferSize);
#else
#error define your platforms aligned memory allocator
#endif
			*(m_pMemoryCounter) += m_nBufferSize;
		}
		return pBuffer;
	}

	void Return(void *pBuffer) 
	{
		if(!m_Queue.try_push(pBuffer)) {
#if defined(WIN32) && !defined(_WIN32_WCE)
			_aligned_free(pBuffer);
#elif defined(NCS_MOBILE)
			NCSFree(pBuffer);
#elif defined(POSIX)
			free(pBuffer);
#else
#error define your platforms aligned memory deallocator
#endif
			*(m_pMemoryCounter) -= m_nBufferSize;
			pBuffer = NULL;
		}
	}

	void Purge()
	{
		void *pBuffer = NULL;
		while(m_Queue.try_pop(pBuffer)) {
#if defined(WIN32) && !defined(_WIN32_WCE)
			_aligned_free(pBuffer);
#elif defined(NCS_MOBILE)
			NCSFree(pBuffer);
#elif defined(POSIX)
			free(pBuffer);
#else
#error define your platforms aligned memory deallocator
#endif
			*(m_pMemoryCounter) -= m_nBufferSize;
			pBuffer = NULL;
		}
	}
	
	INT32 Stride() { return m_nStride; };
	UINT32 BufferSize() { return m_nBufferSize; };
	SDK::CBuffer2D::Type Type() { return m_eType; };
	UINT32 Width() { return m_nWidth; };
	UINT32 Height() { return m_nHeight; };

protected:
	UINT32 m_nWidth, m_nHeight;
	SDK::CBuffer2D::Type m_eType;
	size_t m_nMaxBuffersInQueue;
	INT32 m_nStride;
	UINT32 m_nBufferSize;
	NCS::SDK::CAtomicUnsignedInt64 *m_pMemoryCounter;
	NCS::SDK::stl_concurrent_queue<void *> m_Queue;
};

} // NCS