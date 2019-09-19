#ifndef NCS_SDK_CONCURRENT_BASE_H
#define NCS_SDK_CONCURRENT_BASE_H

#include "NCSEcw/SDK/SpinLockInternal.h"

namespace NCS {
namespace SDK {




template<typename Key_t>
struct DefaultKeyHash_T {
	static size_t hash(const Key_t& t) { return((size_t)t * (size_t)1375261239); }
	static bool equal(const Key_t& a, const Key_t& b) { return a == b; }
};

template <typename Key_t, typename Item_t, typename Lock_t>
class ConcurrentContainerBase
{
public:
	class ItemNode : public Lock_t
	{
	public:
		explicit ItemNode(Key_t key) : first(key), second() {};
		~ItemNode() {};
		ItemNode(const ItemNode &s) { (*this) = s; };
		ItemNode &operator=(const ItemNode &s) {
			first = s.first;
			second = s.second;
			return(*this);
		}
		Key_t first;
		Item_t second;
	private:
		ItemNode() : first(), second() {};
	};

	class accessor_type {
	public:
		virtual void release() = 0;
		virtual bool try_acquire(ItemNode &Node) = 0;
		virtual void acquire(ItemNode &Node) = 0;
		virtual bool empty() const = 0;
	};
    class accessor;
	class const_accessor : public accessor_type
	{
        friend class accessor;
	public:
		const_accessor() : m_pNode(NULL) {};
		virtual ~const_accessor() { release(); };

		virtual bool try_acquire(ItemNode &Node) {
			release();
			if(Node.try_lock_read()) {
				m_pNode = &Node;
				return true;
			}
			return false;
		}
		virtual void acquire(ItemNode &Node) {
			release();
			Node.lock_read();
			m_pNode = &Node;
		}
		virtual void release() {
			if(m_pNode) {
				m_pNode->unlock();
				m_pNode = NULL;
			}
		}
		virtual bool empty() const { return m_pNode == NULL; };
		const ItemNode *operator->() const { return m_pNode; };
		bool upgrade_to_writer(accessor &s){
			bool bRet = true;
			if (m_pNode){
				s.m_pNode = m_pNode;
				bRet = m_pNode->upgrade_to_writer();
				m_pNode = NULL;
			}
			return bRet;
		}
	protected:
		ItemNode *m_pNode;
	private:
		const_accessor(const const_accessor &s) : m_pNode(NULL) {}
		const_accessor &operator=(const const_accessor &s) { m_pNode = NULL; return *this; }
	};

	class accessor : public accessor_type
	{
		friend class const_accessor;
	public:
		accessor() : m_pNode(NULL) {};
		virtual ~accessor() { release(); };

		virtual bool try_acquire(ItemNode &Node) {
			release();
			if(Node.try_lock()) {
				m_pNode = &Node;
				return true;
			}
			return false;
		}
		virtual void acquire(ItemNode &Node) {
			release();
			Node.lock();
			m_pNode = &Node;
		}
		virtual void release() {
			if(m_pNode) {
				m_pNode->unlock();
				m_pNode = NULL;
			}
		}
		virtual bool empty() const { return m_pNode == NULL; };
        void downgrade_to_reader(const_accessor &s){
            if (m_pNode){
                s.m_pNode = m_pNode;
                m_pNode->downgrade_to_reader();
                m_pNode = NULL;
            }
        }
		ItemNode *operator->() { return m_pNode; };
	protected:
		ItemNode *m_pNode;
	private:
		accessor(const accessor &s) : m_pNode(NULL) {}
		accessor &operator=(const accessor &s) { m_pNode = NULL; return *this; }
	};
#ifdef NOT_YET
	class conditional_accessor : public accessor_type
	{
	public:
		conditional_accessor() : m_pNode(NULL), m_bIsWrite(false) {};
		virtual ~conditional_accessor() { release(); };

		virtual bool try_acquire(ItemNode &Node) {
			release();
			if(Node.try_lock_read()) {
				Node.unlock();
				if(Node.second.NeedsWriteAccessor()) {
					if(Node.try_lock()) {
						m_bIsWrite = true;
						m_pNode = &Node;
						return true;
					}
				} else {
					m_pNode = &Node;
					return true;
				}
			}
			return false;
		}
		virtual void acquire(ItemNode &Node) {
			release();
			if(Node.second.NeedsWriteAccessor()) {
				m_bIsWrite = true;
				Node.lock();
				m_pNode = &Node;
			} else {
				Node.lock_read();
				m_pNode = &Node;
			}
		}
		virtual void release() {
			if(m_pNode) {
				m_pNode->unlock();
				m_pNode = NULL;
			}
		}
		virtual bool empty() { return m_pNode == NULL; };
		ItemNode *operator->() { return m_pNode; };
		bool IsWriteLocked() { return m_bIsWrite; };
	protected:
		ItemNode *m_pNode;
		bool m_bIsWrite;
	};


	class conditional_accessor_pointer : public accessor_type
	{
	public:
		conditional_accessor_pointer() : m_pNode(NULL), m_bIsWrite(false) {};
		virtual ~conditional_accessor_pointer() { release(); };

		virtual bool try_acquire(ItemNode &Node) {
			release();
			if(Node.try_lock_read()) {
				if(Node.second->NeedsWriteAccessor()) {
					Node.unlock();
					if(Node.try_lock()) {
						m_bIsWrite = true;
						m_pNode = &Node;
						return true;
					}
				} else {
					m_pNode = &Node;
					return true;
				}
			}
			return false;
		}
		virtual void acquire(ItemNode &Node) {
			release();
			Node.lock_read();
			if(Node.second->NeedsWriteAccessor()) {
				Node.unlock();
				Node.lock();
				m_bIsWrite = true;
				m_pNode = &Node;
			} else {
				m_pNode = &Node;
			}
		}
		virtual void release() {
			if(m_pNode) {
				m_pNode->unlock();
				m_pNode = NULL;
			}
		}
		virtual bool empty() { return m_pNode == NULL; };
		ItemNode *operator->() { return m_pNode; };
		bool IsWriteLocked() { return m_bIsWrite; };
	protected:
		ItemNode *m_pNode;
		bool m_bIsWrite;
	};
#endif
};

} // SDK
} // NCS

#endif // NCS_SDK_CONCURRENT_BASE_H