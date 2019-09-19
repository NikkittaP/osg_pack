#ifndef STL_CONCURRENT_QUEUE_H
#define STL_CONCURRENT_QUEUE_H

#include <list>
#include "NCSEcw/SDK/SpinLockInternal.h"

namespace NCS {
namespace SDK {
	
template<typename T>
class stl_concurrent_queue : public std::list<T>
{
public:
	typedef typename std::list<T>::iterator iterator;

	explicit stl_concurrent_queue(size_t nMaxSize = 1024) : m_nMaxSize(nMaxSize) {};
	virtual ~stl_concurrent_queue() {};

	void push(const T &Source) {
		m_Mutex.lock();
		while(std::list<T>::size() > m_nMaxSize) {
			m_Mutex.unlock();
			NCSSleep(1);
			m_Mutex.lock();
		}
		std::list<T>::push_back(Source);
		m_Mutex.unlock();
	}
	bool push_unique(const T &Source) {
		for(;;) {
			m_Mutex.lock();
			iterator it = std::list<T>::begin();
			while(it != std::list<T>::end()) {
				if(*it == Source) {
					m_Mutex.unlock();
					return false;
				}
				++it;
			}
			if(std::list<T>::size() > m_nMaxSize) {
				m_Mutex.unlock();
				NCSSleep(1);
				continue;
			}
			std::list<T>::push_back(Source);
			m_Mutex.unlock();
			return true;
		}
	}
	bool try_push_unique(const T &Source) {
		m_Mutex.lock();
		if(std::list<T>::size() > m_nMaxSize) {
			m_Mutex.unlock();
			return false;
		}

		iterator it = std::list<T>::begin();
		while(it != std::list<T>::end()) {
			if(*it == Source) {
				m_Mutex.unlock();
				return false;
			}
			++it;
		}
		std::list<T>::push_back(Source);
		m_Mutex.unlock();
		return true;
	}
	void pop(T &Destination) {
		m_Mutex.lock();
		while(std::list<T>::size() == 0) {
			m_Mutex.unlock();
			NCSSleep(1);
			m_Mutex.lock();
		}
		Destination = std::list<T>::front();
		std::list<T>::pop_front();
		m_Mutex.unlock();
	}

	bool try_push(const T &Source) {
		m_Mutex.lock();
		if(std::list<T>::size() < m_nMaxSize) {
			std::list<T>::push_back(Source);
			m_Mutex.unlock();
			return true;
		}
		m_Mutex.unlock();
		return false;
	}
	bool try_pop(T &Destination) {
		m_Mutex.lock();
		if(std::list<T>::size() > 0) {
			Destination = std::list<T>::front();
			std::list<T>::pop_front();
			m_Mutex.unlock();
			return true;
		}
		m_Mutex.unlock();
		return false;
	}

	iterator unsafe_begin() { return std::list<T>::begin(); };
	iterator unsafe_end() { return std::list<T>::end(); };
	
	size_t capacity() { return m_nMaxSize; };
	void set_capacity(size_t nCapacity) { m_nMaxSize = nCapacity; };
protected:
	CSpinLockImpl m_Mutex;
	size_t m_nMaxSize;
};

} // SDK
} // NCS

#endif // STL_CONCURRENT_QUEUE_H
