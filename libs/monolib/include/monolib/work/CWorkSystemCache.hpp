#pragma once

#include "monolib/work/CWorkThread.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

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

// POD mirror of _reslist_node<CCacheItem*>: three pointers, 0xc bytes total.
struct CacheListNode {
    CacheListNode* mNext;     // 0x0
    CacheListNode* mPrev;     // 0x4
    CCacheItem* mItem;        // 0x8
};

// POD mirror of _reslist_base<CCacheItem*>::iterator.
struct CacheListIter {
    CacheListIter(CacheListNode* pNode) : mNode(pNode) {}
    CacheListIter(const CacheListIter& other) : mNode(other.mNode) {}

    CacheListNode* mNode;
};

// POD mirror of _reslist_base<CCacheItem*> (size 0x20). The vtable word is a
// raw slot because the reslist vtables are hand-laid retail data
// (lbl_eu_8056FD3C / lbl_eu_8056FD24) defined in CWorkSystemCache.cpp.
struct CacheList {
    u32 m_vtable;                 // 0x0
    CacheListNode* mStartNodePtr; // 0x4
    CacheListNode mStartNode;     // 0x8 (sentinel; mItem unused)
    CacheListNode* mList;         // 0x14
    int mCapacity;                // 0x18
    bool unk1C;                   // 0x1c
};

// size: 0x1E8
// __declspec(novtable): the retail vtable is the hand-laid lbl_eu_8056FC70
// (blob dissolve), stored explicitly by the ctor.
class __declspec(novtable) CWorkSystemCache : public CWorkThread {
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
void SysCacheClearListLinks(CacheList* list);
u32 SysCacheCountListNodes(void);
bool SysCacheFetchIndexedEntry(int index, u32* outField3C, u8** outField4Addr, u32* outField40);
// _reslist_base<CCacheItem*>::erase(iterator): unlink it, return the next node.
CacheListIter SysCacheUnlinkFront(CacheList* list, CacheListIter it);
bool SysCacheItemWaitReady(CCacheItem* item);
}
