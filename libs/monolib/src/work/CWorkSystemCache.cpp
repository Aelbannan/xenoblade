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
// stores this retail vtable manually (same pattern as CDeviceFileDvd).
extern "C" u32 lbl_eu_8056FC70[];
// reslist<CCacheItem> vtables (DOL data, defined at the bottom of this TU).
extern "C" u32 lbl_eu_8056FD3C[];
extern "C" u32 lbl_eu_8056FD24[];

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
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.rodata] 0x80524568-0x805245B0 (72B): RTTI name strings. MWCC's .rodata
// emission pads a non-last symbol to an 8-byte boundary when its end is
// 4-mod-8, so the retail [20][24][28] symbol split is unreproducible with
// three separate arrays. Instead the first string is padded to [24] and the
// first four bytes of the second name ("resl") ride inside it, with the
// remaining [20] following at +0x18; the combined bytes are byte-identical
// to retail and the third string lands at +0x2C. (The middle symbol's VALUE
// sits 4 bytes late; only typeid().name() output is affected, not the
// dynamic_cast base-list walk.)
extern "C" const char lbl_eu_80524568[24] = {
    0x43,0x57,0x6F,0x72,0x6B,0x53,0x79,0x73,0x74,0x65,0x6D,0x43,0x61,0x63,0x68,0x65,
    0x00,0x00,0x00,0x00, 0x72,0x65,0x73,0x6C,
};  // "CWorkSystemCache\0" + 3 pad + "resl"
extern "C" const char lbl_eu_8052457C[20] = {
    0x69,0x73,0x74,0x3C,0x43,0x43,0x61,0x63,0x68,0x65,0x49,0x74,0x65,0x6D,0x20,0x2A,
    0x3E,0x00,0x00,0x00,
};  // "ist<CCacheItem *>\0" + 2 pad (completes "reslist...")
extern "C" const char lbl_eu_80524594[28] = {
    0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x43,0x43,
    0x61,0x63,0x68,0x65,0x49,0x74,0x65,0x6D,0x20,0x2A,0x3E,0x00,
};  // "_reslist_base<CCacheItem *>\0"
DECOMP_FORCEACTIVE(CWorkSystemCache_cpp, lbl_eu_80524568);
DECOMP_FORCEACTIVE(CWorkSystemCache_cpp, lbl_eu_80524594);

// Foreign .sdata strings referenced by the locators below.
extern const char lbl_eu_8066B218[];
extern const char lbl_eu_8066B220[];
extern const char lbl_eu_805245C8[];

// Forward declarations for the .data block below (referenced by the .sdata
// locators above).
extern "C" u32 lbl_eu_8056FD10[];
extern "C" u32 lbl_eu_8056FD30[];
extern "C" u32 lbl_eu_8056FD24[];
extern "C" u32 lbl_eu_8056FD3C[];

// [.sdata] 0x80663B40-0x80663B78 (56B): RTTI locators + shared "ref"-style
// string pointers.
extern "C" void* lbl_eu_80663B40[2] = { (void*)lbl_eu_80524568, (void*)lbl_eu_8056FD10 };
extern "C" void* lbl_eu_80663B48[2] = { (void*)lbl_eu_8052457C, (void*)lbl_eu_8056FD30 };
extern "C" void* lbl_eu_80663B50[2] = { (void*)lbl_eu_80524594, 0 };
extern "C" void* lbl_eu_80663B58[2] = { (void*)lbl_eu_8066B218, 0 };
extern "C" void* lbl_eu_80663B60 = (void*)lbl_eu_8066B220;
extern "C" void* lbl_eu_80663B64 = (void*)lbl_eu_8066B220;
extern "C" void* lbl_eu_80663B68[2] = { (void*)lbl_eu_8066B220, 0 };
extern "C" void* lbl_eu_80663B70[2] = { (void*)lbl_eu_805245C8, 0 };
DECOMP_FORCEACTIVE(CWorkSystemCache_cpp, lbl_eu_80663B40);

// [.data] 0x8056FC70-0x8056FD48 (216B): CWorkSystemCache vtable + reslist
// RTTI base-list + reslist<CCacheItem*>/_reslist_base vtables.
extern "C" void* __RTTI__10IWorkEvent;
extern "C" void* __RTTI__11CWorkThread;
extern "C" void __dt__16CWorkSystemCacheFv();
extern "C" void wkUpdate__16CWorkSystemCacheFv();
extern "C" void wkStandbyLogin__16CWorkSystemCacheFv();
extern "C" void wkStandbyLogout__16CWorkSystemCacheFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl();
extern "C" void WorkEvent1__10IWorkEventFPvPCc();
extern "C" void OnFileEvent__10IWorkEventFP10CEventFile();
extern "C" void WorkEvent3__10IWorkEventFPv();
extern "C" void WorkEvent4__10IWorkEventFv();
extern "C" void OnPauseTrigger__10IWorkEventFb();
extern "C" void WorkEvent6__10IWorkEventFv();
extern "C" void WorkEvent7__10IWorkEventFv();
extern "C" void WorkEvent8__10IWorkEventFv();
extern "C" void WorkEvent9__10IWorkEventFv();
extern "C" void WorkEvent10__10IWorkEventFv();
extern "C" void WorkEvent11__10IWorkEventFv();
extern "C" void WorkEvent12__10IWorkEventFv();
extern "C" void WorkEvent13__10IWorkEventFv();
extern "C" void WorkEvent14__10IWorkEventFv();
extern "C" void WorkEvent15__10IWorkEventFv();
extern "C" void WorkEvent16__10IWorkEventFv();
extern "C" void WorkEvent17__10IWorkEventFv();
extern "C" void WorkEvent18__10IWorkEventFv();
extern "C" void WorkEvent19__10IWorkEventFv();
extern "C" void WorkEvent20__10IWorkEventFv();
extern "C" void WorkEvent21__10IWorkEventFv();
extern "C" void WorkEvent22__10IWorkEventFv();
extern "C" void WorkEvent23__10IWorkEventFv();
extern "C" void WorkEvent24__10IWorkEventFv();
extern "C" void WorkEvent25__10IWorkEventFv();
extern "C" void WorkEvent26__10IWorkEventFv();
extern "C" void WorkEvent27__10IWorkEventFv();
extern "C" void WorkEvent28__10IWorkEventFv();
extern "C" void WorkEvent29__10IWorkEventFv();
extern "C" void WorkEvent30__10IWorkEventFv();
extern "C" void WorkEvent31__10IWorkEventFv();

extern "C" u32 lbl_eu_8056FC70[40] = {
    (u32)&lbl_eu_80663B40, 0x00000000, (u32)&__dt__16CWorkSystemCacheFv,
    (u32)&WorkEvent1__10IWorkEventFPvPCc, (u32)&OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&WorkEvent3__10IWorkEventFPv, (u32)&WorkEvent4__10IWorkEventFv,
    (u32)&OnPauseTrigger__10IWorkEventFb,
    (u32)&WorkEvent6__10IWorkEventFv, (u32)&WorkEvent7__10IWorkEventFv,
    (u32)&WorkEvent8__10IWorkEventFv, (u32)&WorkEvent9__10IWorkEventFv,
    (u32)&WorkEvent10__10IWorkEventFv, (u32)&WorkEvent11__10IWorkEventFv,
    (u32)&WorkEvent12__10IWorkEventFv, (u32)&WorkEvent13__10IWorkEventFv,
    (u32)&WorkEvent14__10IWorkEventFv, (u32)&WorkEvent15__10IWorkEventFv,
    (u32)&WorkEvent16__10IWorkEventFv, (u32)&WorkEvent17__10IWorkEventFv,
    (u32)&WorkEvent18__10IWorkEventFv, (u32)&WorkEvent19__10IWorkEventFv,
    (u32)&WorkEvent20__10IWorkEventFv, (u32)&WorkEvent21__10IWorkEventFv,
    (u32)&WorkEvent22__10IWorkEventFv, (u32)&WorkEvent23__10IWorkEventFv,
    (u32)&WorkEvent24__10IWorkEventFv, (u32)&WorkEvent25__10IWorkEventFv,
    (u32)&WorkEvent26__10IWorkEventFv, (u32)&WorkEvent27__10IWorkEventFv,
    (u32)&WorkEvent28__10IWorkEventFv, (u32)&WorkEvent29__10IWorkEventFv,
    (u32)&WorkEvent30__10IWorkEventFv, (u32)&WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__16CWorkSystemCacheFv, (u32)&wkRender__11CWorkThreadFv,
    (u32)&wkRenderAfter__11CWorkThreadFv, (u32)&wkStandbyLogin__16CWorkSystemCacheFv,
    (u32)&wkStandbyLogout__16CWorkSystemCacheFv, (u32)&wkStandbyExceptionRetry__11CWorkThreadFUl,
};
extern "C" u32 lbl_eu_8056FD10[5] = {
    (u32)&__RTTI__10IWorkEvent, 0x00000000, (u32)&__RTTI__11CWorkThread, 0x00000000,
    0x00000000,
};
extern "C" u32 lbl_eu_8056FD24[3] = { (u32)&lbl_eu_80663B48, 0x00000000, (u32)&__dt__reslist_CCacheItem };
extern "C" u32 lbl_eu_8056FD30[3] = { (u32)&lbl_eu_80663B50, 0x00000000, 0x00000000 };
extern "C" u32 lbl_eu_8056FD3C[3] = { (u32)&lbl_eu_80663B50, 0x00000000, (u32)&__dt___reslist_base_CCacheItem };
DECOMP_FORCEACTIVE(CWorkSystemCache_cpp, lbl_eu_8056FC70);
DECOMP_FORCEACTIVE(CWorkSystemCache_cpp, lbl_eu_8056FD10);
DECOMP_FORCEACTIVE(CWorkSystemCache_cpp, lbl_eu_8056FD3C);

// [.sbss] 0x806659C8-0x806659D0 (8B): singleton cache pointer + pad word.
CWorkSystemCache* lbl_eu_806659C8;
u32 lbl_eu_806659CC;
