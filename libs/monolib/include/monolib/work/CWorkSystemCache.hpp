#pragma once

#include "monolib/work/CWorkThread.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/reslist.hpp"

// One entry stored in CWorkSystemCache's resource list. The list is a
// _reslist_base<CCacheItem*>, so each node holds a *pointer* to one of these.
class CCacheItem {
public:
    virtual ~CCacheItem();

    u8 field_0x4;                          // 0x4 (start of the name buffer)
    u8 pad_5[0x3c - 0x5];                  // 0x5..0x3c
    u32 field_0x3c;                        // 0x3c
    u32 field_0x40;                        // 0x40
    s32 field_0x44;                        // 0x44
    u8 field_0x48;                         // 0x48
    u8 pad_49[0x4c - 0x49];                // 0x49..0x4c
    u32 field_0x4c;                        // 0x4c
};

// POD mirror of _reslist_base<CCacheItem*>::iterator.
struct CacheListIter {
    CacheListIter(_reslist_node<CCacheItem*>* pNode) : mNode(pNode) {}
    CacheListIter(const CacheListIter& other) : mNode(other.mNode) {}

    _reslist_node<CCacheItem*>* mNode;
};

typedef _reslist_node<CCacheItem*> CacheListNode;

template <> _reslist_base<CCacheItem*>::~_reslist_base();

typedef reslist<CCacheItem*> CacheList;

// size: 0x1E8
// Real CWorkThread subclass: MWCC emits the 0xA0 vtable, base list and RTTI.
class CWorkSystemCache : public CWorkThread {
public:
    CWorkSystemCache(const char* pName, CWorkThread* pParent);
    virtual ~CWorkSystemCache();
    virtual void wkUpdate();        // 0x88
    virtual bool wkStandbyLogin();  // 0x94
    virtual bool wkStandbyLogout(); // 0x98

    DECL_WORKTHREAD_CREATE(CWorkSystemCache);

    //0x0: vtable
    //0x0-1c4: CWorkThread
    CacheList mCache;  // 0x1C4
    u32 unk1E4;        // 0x1E4
};

// Global singleton pointer to the cache instance (.sbss).
extern CWorkSystemCache* lbl_eu_806659C8;

// Retail-unmangled cache helpers (C linkage; defined in CWorkSystemCache.cpp).
extern "C" {
void SysCacheDecLoginRef(CCacheItem* item);
void SysCacheClearListLinks(_reslist_base<CCacheItem*>* list);
u32 SysCacheCountListNodes(void);
bool SysCacheFetchIndexedEntry(int index, u32* outField3C, u8** outField4Addr, u32* outField40);
// _reslist_base<CCacheItem*>::erase(iterator): unlink it, return the next node.
CacheListIter SysCacheUnlinkFront(CacheList* list, CacheListIter it);
bool SysCacheItemWaitReady(CCacheItem* item);
}
