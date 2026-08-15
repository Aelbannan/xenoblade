// Auto-scaffolded catalog TU for monolib/src/work/CWorkSystemCache

#include <harness_catalog.h>

#include "monolib/work/CWorkThread.hpp"
#include "monolib/math/CMat34.hpp"
#include "monolib/util/MemManager.hpp"

// ---------------------------------------------------------------------------
// CCacheItem: one entry stored in CWorkSystemCache's resource list. The list
// is a _reslist_base<CCacheItem*> so each node holds a *pointer* to one of
// these objects (node->mItem at +0x8). The lookup helper func_804D8FDC reads
// fields at 0x3c / 0x40 and exposes the buffer pointer at +0x4.
// ---------------------------------------------------------------------------
class CCacheItem {
public:
    virtual void func_0x8(int flag);      // vtable+8
    u8 field_0x4;                          // 0x4
    u8 pad_5[0x3c - 0x5];                  // 0x5..0x3c
    u32 field_0x3c;                        // 0x3c
    u32 field_0x40;                        // 0x40
    s32 field_0x44;                        // 0x44
    u8 field_0x48;                         // 0x48
    u8 pad_49[0x4c - 0x49];                // 0x49..0x4c
    u32 field_0x4c;                        // 0x4c
};

// ---------------------------------------------------------------------------
// POD mirror of _reslist_node<CCacheItem*>: three pointers, 0xc bytes total.
// ---------------------------------------------------------------------------
struct CacheListNode {
    CacheListNode* mNext;     // 0x0
    CacheListNode* mPrev;     // 0x4
    CCacheItem* mItem;        // 0x8
};

// POD mirror of _reslist_base<CCacheItem*> (size 0x20).
struct CacheList {
    u32 m_vtable;                 // 0x0
    CacheListNode* mStartNodePtr; // 0x4
    CacheListNode mStartNode;     // 0x8
    CacheListNode* mList;         // 0x14
    int mCapacity;                // 0x18
    bool unk1C;                   // 0x1c
};

// Compact layout of the reslist header that CWorkSystemCache embeds at 0x1c4.
// It is literally a _reslist_base<CCacheItem*> (CacheList); the sentinel's
// mItem slot (0x1d4) is unused by the cache. mList/mCapacity/unk1C are the
// cache's array/count/flag.
struct CacheListHeader {
    u32 m_vtable;                  // 0x1c4 (reslist vtable slot)
    CacheListNode* mStartNodePtr;  // 0x1c8
    CacheListNode mStartNode;      // 0x1cc (sentinel: mNext, mPrev, mItem)
    CacheListNode* mList;          // 0x1d8
    int mCapacity;                 // 0x1dc
    bool unk1C;                    // 0x1e0
};

// Tiny struct used by the login-wait helper func_804D91D8 (fields 0x3c/0x44/0x4c).
struct CacheLoginState {
    u8 field_0x0[0x3c];
    u32 field_0x3c;   // 0x3c
    u8 field_0x40[0x4];
    s32 field_0x44;   // 0x44
    u8 field_0x48[0x4];
    u32 field_0x4c;   // 0x4c
};

class __declspec(novtable) CWorkSystemCache : public CWorkThread {
public:
    CWorkSystemCache(const char* pName, CWorkThread* pParent);
    virtual ~CWorkSystemCache();
    virtual void wkUpdate();       // 0x88
    virtual bool wkStandbyLogin(); // 0x94
    virtual bool wkStandbyLogout();// 0x98

    // 0x0: vtable
    // 0x0-0x1c4: CWorkThread
    CacheListHeader mCache;        // 0x1c4
};

// Global singleton pointer to the cache instance (.sbss).
extern CWorkSystemCache* lbl_eu_806659C8;
// Retail CWorkSystemCache vtable (.data) - the class is novtable, so the ctor
// stores this retail rodata vtable manually (same pattern as CDeviceFileDvd).
extern u32 lbl_eu_8056FC70[];
// reslist<CCacheItem> vtables (DOL data).
extern u8 lbl_eu_8056FD3C[];
extern u8 lbl_eu_8056FD24[];

extern "C" {

// Decrement a login-state refcount if it isn't flagged.
__attribute__((never_inline)) void func_804D91BC(CCacheItem* self);

// _reslist_base<CCacheItem>::clearList() - walk nodes clearing only mNext.
__attribute__((never_inline)) void func_804D8EC8(CacheList* list) {
    CacheListNode* r5 = list->mStartNodePtr->mNext;
    while (r5 != list->mStartNodePtr) {
        CacheListNode* r4 = r5;
        r5 = r5->mNext;
        r4->mNext = 0;
    }
    list->mStartNodePtr->mNext = list->mStartNodePtr;
}

// Count the nodes currently in the singleton cache list.
u32 func_804D8FB4(void) {
    CacheListNode* curNode;
    CacheListNode* endNode;
    u32 length = 0;
    endNode = lbl_eu_806659C8->mCache.mStartNodePtr;
    curNode = endNode->mNext;
    while (curNode != endNode) {
        length++;
        curNode = curNode->mNext;
    }
    return length;
}

// Look up the index-th node's item in the singleton cache list and write out
// its fields; returns true on a hit.
bool func_804D8FDC(int index, u32* outField3C, u8** outField4Addr, u32* outField40) {
    CacheListNode* end = lbl_eu_806659C8->mCache.mStartNodePtr;
    CacheListNode* node = end->mNext;
    int i = 0;
    while (node != end) {
        if (i == index) {
            *outField3C = node->mItem->field_0x3c;
            *outField4Addr = &node->mItem->field_0x4;
            *outField40 = node->mItem->field_0x40;
            return true;
        }
        node = node->mNext;
        i++;
    }
    return false;
}

// Unlink the front node of the list (sentinel) and write the new front to out.
void func_804D903C(CacheListNode** outFront, u32 unk, CacheListNode* sentinel) {
    (void)unk;
    CacheListNode* front = sentinel->mNext;
    CacheListNode* frontPrev = front->mPrev;
    CacheListNode* frontNext = front->mNext;
    frontPrev->mNext = frontNext;
    frontNext->mPrev = frontPrev;
    sentinel->mNext->mNext = 0;
    *outFront = frontNext;
}

// Login-wait helper.
__attribute__((never_inline)) bool func_804D91D8(CacheLoginState* self) {
    if (self->field_0x3c == 0 && self->field_0x4c == 0) {
        return true;
    }
    return self->field_0x44 <= 0;
}

// Copy ml::CMat34::identity into out.
void func_804D920C(ml::CMat34* out) {
    *out = ml::CMat34::identity;
}

// _reslist_base<CCacheItem>::~_reslist_base(int deleting)
#pragma optimize_for_size on
__attribute__((never_inline)) void* __dt___reslist_base_CCacheItem(CacheList* self, int deleting) {
    if (self != 0) {
        self->m_vtable = (u32)lbl_eu_8056FD3C;
        func_804D8EC8(self);
        if (self->unk1C == 0) {
            if (self->mList != 0) {
                delete[] self->mList;
                self->mList = 0;
            }
        }
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}
#pragma optimize_for_size off

// reslist<CCacheItem>::~reslist(int deleting)
#pragma optimize_for_size on
void* __dt__reslist_CCacheItem(CacheList* self, int deleting) {
    if (self != 0) {
        __dt___reslist_base_CCacheItem(self, 0);
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}
#pragma optimize_for_size off

} // extern "C"

bool CWorkSystemCache::wkStandbyLogin() {
    return CWorkThread::wkStandbyLogin();
}

#pragma optimize_for_size on
CWorkSystemCache::~CWorkSystemCache() {
    lbl_eu_806659C8 = NULL;
    CacheList* cache = (CacheList*)&mCache;
    if (cache != NULL) {
        __dt___reslist_base_CCacheItem(cache, 0);
    }
}

#pragma optimize_for_size on
void CWorkSystemCache::wkUpdate() {
    CacheListNode* node = lbl_eu_806659C8->mCache.mStartNodePtr->mNext;
    while (node != lbl_eu_806659C8->mCache.mStartNodePtr) {
        CacheListNode* next = node->mNext;
        func_804D91BC(node->mItem);
        if (func_804D91D8((CacheLoginState*)node->mItem)) {
            CCacheItem* p = node->mItem;
            if (p) {
                if (p) {
                    p->func_0x8(1);
                }
                node->mItem = 0;
            }
            CacheListNode* out;
            func_804D903C(&out, (u32)&lbl_eu_806659C8->mCache, (CacheListNode*)&node);
        }
        node = next;
    }
}

#pragma optimize_for_size on
bool CWorkSystemCache::wkStandbyLogout() {
    CacheListNode* node = mCache.mStartNodePtr->mNext;
    while (node != mCache.mStartNodePtr) {
        CCacheItem* p = node->mItem;
        if (p) {
            if (p) {
                p->func_0x8(1);
            }
            node->mItem = 0;
        }
        node = node->mNext;
    }
    func_804D8EC8((CacheList*)&mCache);
    if (mCache.unk1C == 0 && mCache.mList != NULL) {
        delete[] mCache.mList;
        mCache.mList = NULL;
    }
    mCache.mCapacity = 0;
    return CWorkThread::wkStandbyLogout();
}

CWorkSystemCache::CWorkSystemCache(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 0) {
    // Retail vtable store (novtable class -> stored manually, CDeviceFileDvd
    // pattern). The inlined _reslist_base/reslist ctor chain below keeps the
    // FD3C -> FD24 vtable-slot overwrite; the pointer-based sentinel reads
    // between the two stores block MWCC's dead-store elimination.
    *(u32**)this = (u32*)lbl_eu_8056FC70;
    mCache.m_vtable = (u32)lbl_eu_8056FD3C;
    mCache.mList = NULL;
    mCache.mCapacity = 0;
    mCache.unk1C = false;
    mCache.mStartNodePtr = &mCache.mStartNode;
    mCache.mStartNodePtr->mNext = &mCache.mStartNode;
    mCache.mStartNodePtr->mPrev = mCache.mStartNodePtr->mNext;
    mCache.m_vtable = (u32)lbl_eu_8056FD24;
    lbl_eu_806659C8 = this;
    mType = THREAD_CWORKSYSTEMCACHE;
    mCache.mList = (CacheListNode*)mtl::MemManager::allocate_array(
        0x180, CWorkThreadSystem::getWorkMem());
    for (int i = 0; i < 32; i++) {
        mCache.mList[i].mNext = 0;
    }
    mCache.mCapacity = 32;
}

extern "C" void func_804D91BC(CCacheItem* self) {
    if (*(u8*)((u8*)self + 0x48) == 0) {
        *(u32*)((u8*)self + 0x44) -= 1;
    }
}