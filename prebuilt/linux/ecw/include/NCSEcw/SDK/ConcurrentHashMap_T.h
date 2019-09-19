/********************************************************
** Copyright, 1998 - 2013, Intergraph Corporation. All rights reserved.
**
** FILE:     ConcurrentHashMap_T.h 
** CREATED:  02/09/2004 3:27:34 PM
** AUTHOR:   Chris Tapley	
** PURPOSE:  Replacement template class for TBB concurrent_hash_map
**			 Currently slower than TBB's implementation due to (according
**			 to vtune) slower atomic's (and derived spinlocks) and 
**			 memory allocations.
********************************************************/

#ifndef NCS_SDK_CONCURRENT_HASH_MAP_H
#define NCS_SDK_CONCURRENT_HASH_MAP_H

#include "NCSDefs.h"
#include "NCSMisc.h"

#include "ConcurrentBase_T.h"

#define NCS_MAX_TABLE_COUNT 22

namespace NCS {
namespace SDK {

template <typename Key_t, typename Item_t, typename ItemHash = DefaultKeyHash_T<Key_t>, typename Lock_t = CSpinRWLockImpl>
class ConcurrentHashMap_T : public ConcurrentContainerBase<Key_t, Item_t, Lock_t>
{
protected:
	typedef typename ConcurrentContainerBase<Key_t, Item_t, Lock_t>::ItemNode ItemNode;

public:
	typedef typename ConcurrentContainerBase<Key_t, Item_t, Lock_t>::accessor_type accessor_type;
	typedef typename ConcurrentContainerBase<Key_t, Item_t, Lock_t>::accessor accessor;
	typedef typename ConcurrentContainerBase<Key_t, Item_t, Lock_t>::const_accessor const_accessor;
#ifdef NOT_YET
	typedef typename ConcurrentContainerBase<Key_t, Item_t, Lock_t>::conditional_accessor conditional_accessor;
	typedef typename ConcurrentContainerBase<Key_t, Item_t, Lock_t>::conditional_accessor_pointer conditional_accessor_pointer;
#endif

	class iterator {
	public:
		~iterator() {};
		iterator(const iterator &s) { *this = s; };
		const iterator &operator=(const iterator &s) {
			m_pCurrentNode = s.m_pCurrentNode;
			m_pHashMap = s.m_pHashMap;
			m_nCurrentTable = s.m_nCurrentTable;
			m_nCurrentSlot = s.m_nCurrentSlot;
			return *this;
		}

		ItemNode *operator->() { return m_pCurrentNode; };
		bool operator==(const iterator &s) { 
			return (m_nCurrentTable == s.m_nCurrentTable && 
					m_nCurrentSlot == s.m_nCurrentSlot && 
					m_pHashMap == s.m_pHashMap);
		}
		bool operator!=(const iterator &s) { return !(*this == s); };

		void operator++(int) {
			m_pHashMap->find_next(*this);
		}
		void operator++() {
			m_pHashMap->find_next(*this);
		}

	protected:
		friend class ConcurrentHashMap_T;
		explicit iterator(ConcurrentHashMap_T<Key_t, Item_t, ItemHash, Lock_t> *pHashMap) : m_pCurrentNode(NULL), m_pHashMap(pHashMap), m_nCurrentTable(0), m_nCurrentSlot(0) {};
		iterator(ConcurrentHashMap_T<Key_t, Item_t, ItemHash, Lock_t> *pHashMap, size_t nTable, size_t nSlot) : m_pCurrentNode(NULL), m_pHashMap(pHashMap), m_nCurrentTable(nTable), m_nCurrentSlot(nSlot) {};
		

		bool try_acquire(ItemNode *pNode) {
			if(pNode->try_lock()) {
				release();
				m_pCurrentNode = pNode;
				return true;
			}
			return false;
		}
		void acquire(ItemNode *pNode) {
			release();
			m_pCurrentNode = pNode;
			m_pCurrentNode->lock();
		}
		void release() {
			if(m_pCurrentNode) {
				m_pCurrentNode->unlock();
				m_pCurrentNode = NULL;
			}
		}

		size_t &CurrentTable() { return m_nCurrentTable; };
		size_t &CurrentSlot() { return m_nCurrentSlot; };
		ItemNode *CurrentNode() { return m_pCurrentNode; };
	private:
		ItemNode *m_pCurrentNode;
		ConcurrentHashMap_T<Key_t, Item_t, ItemHash, Lock_t> *m_pHashMap;
		size_t m_nCurrentTable;
		size_t m_nCurrentSlot;
	};

protected:

	class ItemContainerList {
	public:
		explicit ItemContainerList(Key_t Key) : m_ItemNode(Key), m_pNext(NULL) {};
		Key_t Key() { return m_ItemNode.first; };
		ItemNode m_ItemNode;
		ItemContainerList *m_pNext;
	private:
		ItemContainerList(const ItemContainerList &s) {};
		const ItemContainerList &operator=(const ItemContainerList &s) {
			m_pNext = s.m_pNext;
			m_ItemNode = s.m_ItemNode;
		};
		ItemContainerList() : m_pNext(NULL) {};
	};

	class TableSlot : public Lock_t {
	public:
		
		TableSlot() : m_pItemContainerList(NULL), m_bNewlyGrown(true) {};
		explicit TableSlot(bool bIsInitial) : m_pItemContainerList(NULL), m_bNewlyGrown(!bIsInitial) {};
		~TableSlot() {
			if(m_pItemContainerList) {
				ItemContainerList *pItem = m_pItemContainerList;
				ItemContainerList *pItemNext = pItem->m_pNext;
				delete pItem;
				while(pItemNext != NULL) {
					pItem = pItemNext;
					pItemNext = pItem->m_pNext;
					delete pItem;
				}
			}
		}

		void set_null() { m_bNewlyGrown = false; }
		bool is_empty() { return(m_pItemContainerList == NULL); };
		bool is_newly_grown() { return(m_bNewlyGrown); };


		void rehash_to(TableSlot &Destination, size_t nTableInitialItem, size_t DestinationTableIndex, size_t DestinationItemIndex, int TableSize, UINT32 Mask, UINT32 nInitialShift) {
			Lock_t::lock();
			if(!is_empty()) {

				while(!is_empty()) {

					size_t LimitedHashKey = ItemHash::hash(m_pItemContainerList->Key()) & (Mask - 1);
					size_t TableIndex = NCSLog2_szt(LimitedHashKey);
					TableIndex = TableIndex < nInitialShift ? 0 : TableIndex - (nInitialShift-1);
					size_t ItemIndex = LimitedHashKey - nTableInitialItem;

					if(TableIndex == DestinationTableIndex && ItemIndex == DestinationItemIndex) {
						//NCS::CLog::Log("ConcurrentHashMap", NCS_DEBUG_LEVEL, "Found Key: %u", m_pItemContainerList->Key());
						ItemContainerList *pItem = m_pItemContainerList;
						m_pItemContainerList = m_pItemContainerList->m_pNext;
						pItem->m_pNext = Destination.m_pItemContainerList;
						Destination.m_pItemContainerList = pItem;
					} else {
						//NCS::CLog::Log("ConcurrentHashMap", NCS_DEBUG_LEVEL, "Missed Key: %u", m_pItemContainerList->Key());
						break;
					}
				}
				if(!is_empty()) {
					ItemContainerList *pItemPrev = m_pItemContainerList;
					ItemContainerList *pItem = m_pItemContainerList->m_pNext;
					while(pItem) {
						size_t LimitedHashKey = ItemHash::hash(pItem->Key()) & (Mask - 1);
						size_t TableIndex = NCSLog2_szt(LimitedHashKey);
						TableIndex = TableIndex < nInitialShift ? 0 : TableIndex - (nInitialShift - 1);
						size_t ItemIndex = LimitedHashKey - nTableInitialItem;

						if(TableIndex == DestinationTableIndex && ItemIndex == DestinationItemIndex) {
							//NCS::CLog::Log("ConcurrentHashMap", NCS_DEBUG_LEVEL, "Found Key: %u", pItem->Key());
							pItemPrev->m_pNext = pItem->m_pNext;
							pItem->m_pNext = Destination.m_pItemContainerList;
							Destination.m_pItemContainerList = pItem;
							pItem = pItemPrev->m_pNext;
						} else {
							//NCS::CLog::Log("ConcurrentHashMap", NCS_DEBUG_LEVEL, "Missed Key: %u", pItem->Key());
							pItemPrev = pItem;
							pItem = pItem->m_pNext;
						}
					}
				}
			}
			Lock_t::unlock();
		}

		void clear(atomic_INT64 &nItemCount) {
			Lock_t::lock();
			if(is_empty()) {
				Lock_t::unlock();
				return;
			}

			ItemContainerList *pItem = m_pItemContainerList;
			m_pItemContainerList = NULL;
			
			ItemContainerList *pItemNext = pItem->m_pNext;
			delete pItem;
			--nItemCount;
			while (pItemNext != NULL) {
				pItem = pItemNext;
				pItemNext = pItem->m_pNext;
				delete pItem;
				--nItemCount;
			}

			Lock_t::unlock();
		}
		bool find(accessor_type &acc_w, const Key_t &Key) {
begin_find:
			Lock_t::lock_read();

			ItemContainerList *pItem = m_pItemContainerList;
			if(is_empty()) {
				Lock_t::unlock();
				return false;
			}

			while(pItem != NULL) {
				if(ItemHash::equal(pItem->Key(), Key)) {
					for(int nI = 0;; nI++) {
						if(acc_w.try_acquire(pItem->m_ItemNode)) {
							break;
						} else if(nI > 20) {
							Lock_t::unlock();
							NCSYield();
							goto begin_find;
						}
						else{
							NCSPause();
						}
					}
					Lock_t::unlock();
					return true;
				}
				pItem = pItem->m_pNext;
			}
			Lock_t::unlock();
			return false;
		}

		bool maintenance_find(accessor_type &acc_w, const Key_t &Key) {
begin_maintenance_find:
			if(is_empty()) {
				return false;
			}
			ItemContainerList *pItem = m_pItemContainerList;
			while(pItem != NULL) {
				if(ItemHash::equal(pItem->Key(), Key)) {
					acc_w.release();
					for(int nI = 0;; nI++) {
						if(acc_w.try_acquire(pItem->m_ItemNode)) {
							break;
						} else if(nI > 20) {
							Lock_t::unlock();
							NCSYield();
							Lock_t::lock();
							goto begin_maintenance_find;
						}
						else{
							NCSPause();
						}
					}
					return true;
				}
				pItem = pItem->m_pNext;
			}
			return false;
		}

		bool find_next(iterator &it) {
begin_find_next:
			Lock_t::lock_read();
			
			if(is_empty()) {
				Lock_t::unlock();
				return false;
			}

			if(it.CurrentNode() == NULL) {
				for(int nI = 0;; nI++) {
					if(it.try_acquire(&m_pItemContainerList->m_ItemNode)) {
						break;
					} else if(nI > 20) {
						Lock_t::unlock();
						NCSYield();
						goto begin_find_next;
					}
					else{
						NCSPause();
					}
				}
				Lock_t::unlock();
				return true;
			} else {
				ItemContainerList *pItem = m_pItemContainerList;
				Key_t &Key = it.CurrentNode()->first;
				while(pItem != NULL) {
					if(ItemHash::equal(pItem->Key(), Key)) {
						it.release();
						if(pItem->m_pNext) {
							for(int nI = 0;; nI++) {
								if(it.try_acquire(&pItem->m_pNext->m_ItemNode)) {
									break;
								} else if(nI > 20) {
									Lock_t::unlock();
									NCSYield();
									goto begin_find_next;
								}
								else{
									NCSPause();
								}
							}
							Lock_t::unlock();
							return true;
						}
					}
					pItem = pItem->m_pNext;
				}
			}
			Lock_t::unlock();
			return false;
		}


		bool insert(accessor_type &acc_w, const Key_t &Key) {
begin_insert:
			Lock_t::lock_read();
			for(;;) {
				if(is_empty()) {
					// If we have to unlock during upgrade_to_writer, retest is_empty 
					if (Lock_t::upgrade_to_writer() && !is_empty()) {
						Lock_t::downgrade_to_reader();
						continue;
					}
					m_pItemContainerList = new ItemContainerList(Key);
					acc_w.acquire(m_pItemContainerList->m_ItemNode);
					Lock_t::unlock();
					return true;
				}
				ItemContainerList *pItem = m_pItemContainerList;
				while(pItem != NULL) {
					if(ItemHash::equal(pItem->Key(), Key)) {
						for(int nI = 0;; nI++) {
							if(acc_w.try_acquire(pItem->m_ItemNode)) {
								break;
							} else if(nI > 20) {
								Lock_t::unlock();
								NCSYield();
								goto begin_insert;
							}
							else{
								NCSPause();
							}
						}
						Lock_t::unlock();
						return false;
					}
					pItem = pItem->m_pNext;
				}
				if (Lock_t::upgrade_to_writer()) {
					pItem = m_pItemContainerList;
					while(pItem != NULL) {
						if(ItemHash::equal(pItem->Key(), Key)) {
							for(int nI = 0;; nI++) {
								if(acc_w.try_acquire(pItem->m_ItemNode)) {
									break;
								} else if(nI > 20) {
									Lock_t::unlock();
									NCSYield();
									goto begin_insert;
								}
								else{
									NCSPause();
								}
							}
							Lock_t::unlock();
							return false;
						}
						pItem = pItem->m_pNext;
					}
				}
				pItem = new ItemContainerList(Key);
				pItem->m_pNext = m_pItemContainerList;
				m_pItemContainerList = pItem;
				acc_w.acquire(m_pItemContainerList->m_ItemNode);
				Lock_t::unlock();
				return true;
			}
			return false;
		}

		bool erase(accessor_type &acc_w, const Key_t &Key) {
			Lock_t::lock();
			ItemContainerList *pItem = m_pItemContainerList;
			if(is_empty()) {
				Lock_t::unlock();
				return false;
			}
			if(ItemHash::equal(pItem->Key(), Key)) {
				m_pItemContainerList = m_pItemContainerList->m_pNext;
				Lock_t::unlock();
				acc_w.release();
				delete pItem;
				return true;
			}
			ItemContainerList *pItemPrev = pItem;
			pItem = pItem->m_pNext;
			while(pItem != NULL) {
				if(ItemHash::equal(pItem->Key(), Key)) {
					pItemPrev->m_pNext = pItem->m_pNext;
					Lock_t::unlock();
					acc_w.release();
					delete pItem;
					return true;
				}
				pItemPrev = pItem;
				pItem = pItem->m_pNext;
			}
			Lock_t::unlock();
			return false;
		}

		bool erase(const Key_t &Key) {
begin_erase:
			accessor acc_w;

			Lock_t::lock();
			ItemContainerList *pItem = m_pItemContainerList;
			if(is_empty()) {
				Lock_t::unlock();
				return false;
			}
			if(ItemHash::equal(pItem->Key(), Key)) {
				for(int nI = 0;; nI++) {
					if(acc_w.try_acquire(pItem->m_ItemNode)) {
						break;
					} else if(nI > 20) {
						Lock_t::unlock();
						NCSYield();
						goto begin_erase;
					}
					else{
						NCSPause();
					}
				}
				m_pItemContainerList = m_pItemContainerList->m_pNext;
				Lock_t::unlock();
				acc_w.release();
				delete pItem;
				return true;
			}
			ItemContainerList *pItemPrev = pItem;
			pItem = pItem->m_pNext;
			while(pItem != NULL) {
				if(ItemHash::equal(pItem->Key(), Key)) {
					for(int nI = 0;; nI++) {
						if(acc_w.try_acquire(pItem->m_ItemNode)) {
							break;
						} else if(nI > 20) {
							Lock_t::unlock();
							NCSYield();
							goto begin_erase;
						}
						else{
							NCSPause();
						}
					}
					pItemPrev->m_pNext = pItem->m_pNext;
					Lock_t::unlock();
					acc_w.release();
					delete pItem;
					return true;
				}
				pItemPrev = pItem;
				pItem = pItem->m_pNext;
			}
			Lock_t::unlock();
			return false;
		}

		ItemContainerList *m_pItemContainerList;
		bool m_bNewlyGrown;
	};


	class Table
	{
	public:
		Table(size_t nTableSize, bool bInitial, size_t nInitialItem) : m_Mutex(), m_nTableSize(nTableSize), m_pItems(NULL), m_nInitialItem(nInitialItem) {
			if(bInitial) {
				m_pItems = new TableSlot[m_nTableSize];
				for(size_t nI = 0; nI < m_nTableSize; nI++) {
					m_pItems[nI].set_null();
				}
			} else {
				m_pItems = new TableSlot[m_nTableSize];
			}
		}
		virtual ~Table() {
			delete[] m_pItems;
		}

		void clear(atomic_INT64 &nItemCount) {
			for (size_t nI = 0; nI < m_nTableSize; nI++) {
				m_pItems[nI].clear(nItemCount);
			}
		}
		
		TableSlot &get_slot(size_t nI) { return m_pItems[nI]; }

		Lock_t m_Mutex;
		size_t m_nTableSize;
		TableSlot *m_pItems;
		size_t m_nInitialItem;
		
	};

	void grow() {
		if(m_GrowShrinkMutex.try_lock()) {
			if(m_nNextTable < NCS_MAX_TABLE_COUNT) {
				size_t NextTableSize = (size_t)m_nMaxTableSlots;
				m_Tables[m_nNextTable] = new Table(NextTableSize, false, NextTableSize);
				m_nMaxTableSlots = m_nMaxTableSlots * 2;
				m_nNextTable++;
			}
			m_GrowShrinkMutex.unlock();
		}
	}

	typedef enum {
		FIND,
		INSERT,
		ERASE
	} Operation;

	void find_next(iterator &it) {
		if(!get_next(it)) {
			it.release();
			it = end();
		}
	}
	bool get_next(iterator &it) {

		size_t &nCurrentTable = it.CurrentTable();
		size_t &nCurrentSlot = it.CurrentSlot();

		Table *pT = m_Tables[nCurrentTable];
		TableSlot *pTableHeader = &(pT->get_slot(nCurrentSlot));

		while(pTableHeader->find_next(it) == false) {
			nCurrentSlot++;
			if(nCurrentSlot >= pT->m_nTableSize) {
				nCurrentSlot = 0;
				nCurrentTable++;
				if((int)nCurrentTable >= m_nNextTable) {
					return false;
				}
				pT = m_Tables[nCurrentTable];
			}			
			pTableHeader = &(pT->get_slot(nCurrentSlot));
		}
		return true;
	}

	bool internal_operation(accessor_type &acc, const Key_t &Key, Operation eOperation) {
begin_internal_operation:
		size_t HashedKey = ItemHash::hash(Key);

		// save nMaxTableSlots so we can tell if another thread triggered a grow(), in which case we need 
		// to rehash our particular node
		UINT32 nMaxTableSlots = m_nMaxTableSlots; 
		size_t LimitedHashKey = HashedKey & (nMaxTableSlots - 1); // limited to size of active tables

		size_t TableIndex = NCSLog2_szt(LimitedHashKey);
		TableIndex = TableIndex < m_nInitialShift ? 0 : TableIndex - (m_nInitialShift-1);

		Table &T = *(m_Tables[TableIndex]);
		size_t ItemIndex = LimitedHashKey - T.m_nInitialItem;
		
		TableSlot &TableHeader = T.get_slot(ItemIndex);
		TableHeader.lock_read();

		if(TableHeader.is_newly_grown()) {
			if(!TableHeader.upgrade_to_writer() || TableHeader.is_newly_grown()) {
				//NCS::CLog::Log("ConcurrentHashMap", NCS_DEBUG_LEVEL, "rehashing for VirtualIndex: %u; Initiating Key: %u; DT: %u; DI: %u", LimitedHashKey, Key, TableIndex, ItemIndex);
				TableHeader.set_null();
				size_t nTS = m_nFirstTableSize;
				size_t Prev_Parent_TableIndex = (size_t)-1;
				size_t Prev_Parent_ItemIndex = (size_t)-1;

				while(nTS < nMaxTableSlots) {
					size_t Parent_LimitedHashKey = HashedKey & (nTS - 1);
					size_t Parent_TableIndex = NCSLog2_szt(Parent_LimitedHashKey);
					Parent_TableIndex = Parent_TableIndex < m_nInitialShift ? 0 : Parent_TableIndex - (m_nInitialShift-1);
					Table &Parent_T = *(m_Tables[Parent_TableIndex]);
					size_t Parent_ItemIndex = Parent_LimitedHashKey - Parent_T.m_nInitialItem;
					if(!((Parent_TableIndex == TableIndex && Parent_ItemIndex == ItemIndex) ||(Prev_Parent_TableIndex == Parent_TableIndex && Prev_Parent_ItemIndex == Parent_ItemIndex))) {
						//NCS::CLog::Log("ConcurrentHashMap", NCS_DEBUG_LEVEL, "Rehash source Key: %u; T: %u; I: %u", Parent_LimitedHashKey, Parent_TableIndex, Parent_ItemIndex);
						m_Tables[Parent_TableIndex]->get_slot(Parent_ItemIndex).rehash_to(TableHeader, T.m_nInitialItem, TableIndex, ItemIndex, m_nFirstTableSize, nMaxTableSlots, m_nInitialShift);
					} else {
						//NCS::CLog::Log("ConcurrentHashMap", NCS_DEBUG_LEVEL, "Omitting rehash source Key: %u; T: %u; I: %u", Parent_LimitedHashKey, Parent_TableIndex, Parent_ItemIndex);
					}
					Prev_Parent_TableIndex = Parent_TableIndex;
					Prev_Parent_ItemIndex = Parent_ItemIndex;
					nTS *= 2;
				}
			}
		}
		TableHeader.unlock();
		switch(eOperation) {
			case INSERT: {
				// if the table has grown since we started the operation our item might have been rehashed
				// by another thread, so we have to start again.
				bool bTableGrown = false;
				if (nMaxTableSlots == m_nMaxTableSlots){
					bool bRet = false;
					m_GrowShrinkMutex.lock_read();
					if (nMaxTableSlots == m_nMaxTableSlots){
						
						if (TableHeader.insert(acc, Key)) {
							m_nItemCount++;
							if (m_nItemCount >= m_nMaxTableSlots) {
								m_GrowShrinkMutex.unlock();
								grow();
								return true;
							}
							bRet = true;
						}
						else {
							bRet = false;
						}
					}
					else{
						bTableGrown = true;
					}
					m_GrowShrinkMutex.unlock();
					if (bTableGrown){
						goto begin_internal_operation;
					}
					else{
						return bRet;
					}
				}
				else{
					goto begin_internal_operation;
				}
			}
			case FIND:
				if(TableHeader.find(acc, Key)) {
					return true;
				} else if(nMaxTableSlots != m_nMaxTableSlots) {
					// if the table has grown since we started the search our item might have been rehashed
					// by another thread, so we have to start again.
					goto begin_internal_operation;
				} else {
					return false;
				}
			case ERASE: {
					if(acc.empty() ? TableHeader.erase(Key) : TableHeader.erase(acc, Key)) {
						m_nItemCount--;
						return true;
					} else if(nMaxTableSlots != m_nMaxTableSlots) {
						// if the table has grown since we started the search our item might have been rehashed
						// by another thread, so we have to start again.
						goto begin_internal_operation;
					} else {
						return false;
					}
				}
			default:
				return false;
		}
	}
public:
	explicit ConcurrentHashMap_T(UINT32 nInitialShift = 10) : m_nInitialShift(nInitialShift), m_nFirstTableSize(1 << nInitialShift), m_nNextTable(1) {
		memset(m_Tables, 0x0, NCS_MAX_TABLE_COUNT * sizeof(Table*));
		m_Tables[0] = new Table(m_nFirstTableSize, true, 0);
		m_nMaxTableSlots = m_nFirstTableSize;
		m_nItemCount = 0;
	}
	virtual ~ConcurrentHashMap_T() {
		for(int nI = 0; nI < m_nNextTable; nI++) {
			delete m_Tables[nI];
		}
	}
	bool insert(accessor &acc_w, const Key_t &Key) {
		return internal_operation(acc_w, Key, ConcurrentHashMap_T::INSERT);
	}
	bool find(accessor &acc_w, const Key_t &Key) {
		return internal_operation(acc_w, Key, ConcurrentHashMap_T::FIND);
	}
	bool insert(const_accessor &acc_r, const Key_t &Key) {
		return internal_operation(acc_r, Key, ConcurrentHashMap_T::INSERT);
	}
	bool find(const_accessor &acc_r, const Key_t &Key) {
		return internal_operation(acc_r, Key, ConcurrentHashMap_T::FIND);
	}
#ifdef NOT_YET
	bool insert(conditional_accessor &acc_rw, const Key_t &Key) {
		return internal_operation(acc_rw, Key, ConcurrentHashMap_T::INSERT);
	}
	bool find(conditional_accessor &acc_rw, const Key_t &Key) {
		return internal_operation(acc_rw, Key, ConcurrentHashMap_T::FIND);
	}
#endif
	bool erase(const Key_t &Key) {
		accessor acc_w;
		return internal_operation(acc_w, Key, ConcurrentHashMap_T::ERASE);
	}
	bool erase(accessor &acc_w) {
		if (!acc_w.empty()) {
			return internal_operation(acc_w, acc_w->first, ConcurrentHashMap_T::ERASE);
		}
		return false;
	}

	void clear() { 
		for(int nI = 0; nI < m_nNextTable; nI++) {
			if(m_Tables[nI] != NULL) {
				m_Tables[nI]->clear(m_nItemCount);
			} else {
				break;
			}
		}
	}

	size_t size() { return (size_t)m_nItemCount; };

	iterator begin() {
		if(m_nItemCount > 0) {
			iterator it(this);
			if(get_next(it)) {
				return it;
			}
		}
		return end();
	}
	iterator end() {
		return iterator(this, (size_t)-1, (size_t)-1);
	}

protected:
	// Index	Start		End			Cummulative Virtual Size
	// 0		0			1023		1024
	// 1		1024		2047		2048
	// 2		2048		4095		4096
	// 3		4096		8191		8192
	// ...
	// 22		2147483648	4294967295	4294967296
	Table *m_Tables[NCS_MAX_TABLE_COUNT];

	atomic_INT64 m_nItemCount;
	atomic_UINT32 m_nMaxTableSlots;

	UINT32 m_nInitialShift;
	int m_nFirstTableSize;
	int m_nNextTable;
	Lock_t m_GrowShrinkMutex;
};

} // SDK
} // NCS


#endif // NCS_SDK_CONCURRENT_HASH_MAP_H