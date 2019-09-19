#ifndef NCS_SDK_ATOMICS_INTERNAL_H
#define NCS_SDK_ATOMICS_INTERNAL_H

#include "NCSDefs.h"

// Does the compiler support C++11 atomics?  GCC 4.7, msvc110, clang3.1 
#if (__GNUC__ && ((__GNUC__ == 4 && __GNUC_MINOR__ >=7) || (__GNUC__ > 4))) ||\
	(_MSC_VER && (_MSC_VER >= 1700)) ||\
	(MACOSX && !NO_CPLUSPLUSLIB) ||\
    (IOS && !NO_CPLUSPLUSLIB)

#include <atomic>

namespace NCS {
namespace SDK {

#ifdef __GNUC__

typedef std::atomic_schar atomic_INT8;
typedef std::atomic_uchar atomic_UINT8;
typedef std::atomic_short atomic_INT16;
typedef std::atomic_ushort atomic_UINT16;
typedef std::atomic_int atomic_INT32;
typedef std::atomic_uint atomic_UINT32;
typedef std::atomic_llong atomic_INT64;
typedef std::atomic_ullong atomic_UINT64;


#else

typedef std::atomic_int8_t atomic_INT8;
typedef std::atomic_uint8_t atomic_UINT8;
typedef std::atomic_int16_t atomic_INT16;
typedef std::atomic_uint16_t atomic_UINT16;
typedef std::atomic_int32_t atomic_INT32;
typedef std::atomic_uint32_t atomic_UINT32;
typedef std::atomic_int64_t atomic_INT64;
typedef std::atomic_uint64_t atomic_UINT64;

#endif // __GNUC__

typedef std::memory_order memory_order;

} // SDK
} // NCS

#else

#ifndef BOOST_ATOMIC_NO_LIB
#define BOOST_ATOMIC_NO_LIB 1
#endif
#ifdef __GNUC__
#ifndef __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8
#define __GCC_HAVE_SYNC_COMPARE_AND_SWAP_8 1
#endif
#endif

#include <boost/atomic.hpp>

namespace NCS {
namespace SDK {

typedef boost::memory_order memory_order;

typedef boost::atomic_uint8_t atomic_UINT8;
typedef boost::atomic_int8_t atomic_INT8;
typedef boost::atomic_uint16_t atomic_UINT16;
typedef boost::atomic_int16_t atomic_INT16;
typedef boost::atomic_uint atomic_UINT32;
typedef boost::atomic_int atomic_INT32;
typedef boost::atomic_address atomic_ADDRESS;
typedef boost::atomic_bool atomic_bool;
typedef boost::atomic_wchar_t atomic_wchar;

#ifndef _WIN32_WCE

typedef boost::atomic_uint64_t atomic_UINT64;
typedef boost::atomic_int64_t atomic_INT64;

#else

// WINCE does not support InterlockedExchange64 so we need to emulate atomic [U]INT64's with a spinlock.
// boost's lockpool implementation for this is not workable.

template<typename IntegralType>
class atomic_base
{
public:
	atomic_base() : m_Lock(0), m_V(0) {};
	atomic_base(IntegralType v) : m_Lock(0), m_V(v) {};
	~atomic_base() {};
    
	void store(IntegralType v, memory_order order = NCS::SDK::memory_order::memory_order_seq_cst)
    {
		NCSCompilerBarrier();
		lock();
		m_V = v;
		NCSCompilerBarrier();
		unlock();
    }

    IntegralType load(memory_order order = NCS::SDK::memory_order::memory_order_seq_cst) const
    {
		NCSCompilerBarrier();
		IntegralType v;
        lock();
		v = m_V;
		NCSCompilerBarrier();
		unlock();
		return v;
    }

    IntegralType exchange(IntegralType v, memory_order order = NCS::SDK::memory_order::memory_order_seq_cst)
    {
		NCSCompilerBarrier();
		IntegralType original;
        lock();
		original = m_V;
		m_V = v;
		NCSCompilerBarrier();
		unlock();
		
        return original;
    }

    bool compare_exchange_weak(
        IntegralType & expected,
        IntegralType desired,
        memory_order success_order,
        memory_order failure_order)
    {
        return compare_exchange_strong(expected, desired, success_order, failure_order);
    }

    bool compare_exchange_strong(
        IntegralType & expected,
        IntegralType desired,
        memory_order success_order,
        memory_order failure_order)
    {
		NCSCompilerBarrier();
		bool success = false;
        lock();
		if(m_V == expected) {
			m_V = desired;
			success = true;
		}
		NCSCompilerBarrier();
		unlock();
        return success;
    }

    IntegralType fetch_add(IntegralType v, memory_order order = NCS::SDK::memory_order::memory_order_seq_cst)
    {
		NCSCompilerBarrier();
        IntegralType original;
        lock();
		original = m_V;
		m_V += v;
		NCSCompilerBarrier();
		unlock();
        return original;
    }

    IntegralType fetch_sub(IntegralType v, memory_order order = NCS::SDK::memory_order::memory_order_seq_cst)
    {
		NCSCompilerBarrier();
        IntegralType original;
        lock();
		original = m_V;
		m_V -= v;
		NCSCompilerBarrier();
		unlock();
        return original;
    }

    IntegralType fetch_and(IntegralType v, memory_order order = NCS::SDK::memory_order::memory_order_seq_cst)
    {
		NCSCompilerBarrier();
        IntegralType original;
        lock();
		original = m_V;
		m_V &= v;
		NCSCompilerBarrier();
		unlock();
        return original;
    }

    IntegralType fetch_or(IntegralType v, memory_order order = NCS::SDK::memory_order::memory_order_seq_cst)
    {
		NCSCompilerBarrier();
        IntegralType original;
        lock();
		original = m_V;
		m_V |= v;
		NCSCompilerBarrier();
		unlock();
        return original;
    }

    IntegralType fetch_xor(IntegralType v, memory_order order = NCS::SDK::memory_order::memory_order_seq_cst)
    {
		NCSCompilerBarrier();
        IntegralType original;
        lock();
		original = m_V;
		m_V ^= v;
		NCSCompilerBarrier();
		unlock();
        return original;
    }

    bool is_lock_free(void) { return false; }

	operator IntegralType() { return load(); }
	void operator=(IntegralType v) { store(v); } 
    IntegralType operator++(int) { return fetch_add(1); } 
    IntegralType operator++(void) { return fetch_add(1) + 1; } 
    IntegralType operator--(int) { return fetch_sub(1); } 
    IntegralType operator--(void) { return fetch_sub(1) - 1; } 
    IntegralType operator+=(IntegralType v) { return fetch_add(v) + v; } 
    IntegralType operator-=(IntegralType v) { return fetch_sub(v) - v; } 
	
	IntegralType operator&=(IntegralType v) { return fetch_and(v) & v; } 
    IntegralType operator|=(IntegralType v) { return fetch_or(v) | v; } 
    IntegralType operator^=(IntegralType v) { return fetch_xor(v) ^ v; } 

private:

	// lock / unlock is not really const, just pretending to be to satisfy the 
	// std::atomic<> interface
	void lock() const {
		for(;;) {
			UINT8 State = m_Lock.load();
			if(State == 0 && m_Lock.compare_exchange_strong(State, 1)) {
				return;
			}
		}
	}
	void unlock() const {
		m_Lock.store(0);
	}

	atomic_base(const atomic_base &s) {};
	atomic_base & operator=(const atomic_base &s) { return *this; };

	mutable atomic_UINT8 m_Lock;
    IntegralType m_V;
};

typedef atomic_base<UINT64> atomic_UINT64;
typedef atomic_base<INT64> atomic_INT64;

#endif // _WIN32_WCE

} // SDK
} // NCS


#endif

#endif // NCS_SDK_ATOMICS_INTERNAL_H
