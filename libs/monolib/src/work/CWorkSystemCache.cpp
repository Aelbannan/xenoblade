// Auto-scaffolded catalog TU for monolib/src/work/CWorkSystemCache

#include <harness_catalog.h>

#include "monolib/work/CWorkThread.hpp"
#include "monolib/math/CMat34.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/data_vtables.hpp"

#include "monolib/work/CWorkSystemCache.hpp"

// Retail CWorkSystemCache vtable (.data) - the class is novtable, so the ctor
// stores this retail vtable manually (same pattern as CDeviceFileDvd).
extern "C" u32 lbl_eu_8056FC70[];
// reslist<CCacheItem> vtables (DOL data, defined at the bottom of this TU).
extern "C" u32 lbl_eu_8056FD3C[];
extern "C" u32 lbl_eu_8056FD24[];

extern "C" {

// _reslist_base<CCacheItem>::clearList() - walk nodes clearing only mNext.
 void SysCacheClearListLinks(CacheList* list) {
    CacheListNode* walk = list->mStartNodePtr->mNext;
    while (walk != list->mStartNodePtr) {
        CacheListNode* node = walk;
        walk = walk->mNext;
        node->mNext = 0;
    }
    list->mStartNodePtr->mNext = list->mStartNodePtr;
}

// Count the nodes currently in the singleton cache list.
u32 SysCacheCountListNodes(void) {
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
bool SysCacheFetchIndexedEntry(int index, u32* outField3C, u8** outField4Addr, u32* outField40) {
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

// _reslist_base<CCacheItem*>::erase(iterator): unlink the node and return an
// iterator to its successor.
CacheListIter SysCacheUnlinkFront(CacheList* list, CacheListIter it) {
    CacheListNode* next = it.mNode->mNext;
    CacheListNode* prev = it.mNode->mPrev;
    prev->mNext = next;
    next->mPrev = prev;
    it.mNode->mNext = 0;
    return CacheListIter(next);
}

// Login-wait helper.
 bool SysCacheItemWaitReady(CCacheItem* ths) {
    if (ths->field_0x3c == 0 && ths->field_0x4c == 0) {
        return true;
    }
    return ths->field_0x44 <= 0;
}

// Copy ml::CMat34::identity into out.
void SysCacheStoreIdentityMat(ml::CMat34* out) {
    *out = ml::CMat34::identity;
}

// _reslist_base<CCacheItem>::~_reslist_base(int deleting)
#pragma optimize_for_size on
 void* __dt___reslist_base_CCacheItem(CacheList* ths, int deleting) {
    if (ths != 0) {
        ths->m_vtable = (u32)lbl_eu_8056FD3C;
        SysCacheClearListLinks(ths);
        if (ths->unk1C == 0) {
            if (ths->mList != 0) {
                delete[] ths->mList;
                ths->mList = 0;
            }
        }
        if (deleting > 0) {
            delete ths;
        }
    }
    return ths;
}
#pragma optimize_for_size off

// reslist<CCacheItem>::~reslist(int deleting)
#pragma optimize_for_size on
void* __dt__reslist_CCacheItem(CacheList* ths, int deleting) {
    if (ths != 0) {
        __dt___reslist_base_CCacheItem(ths, 0);
        if (deleting > 0) {
            delete ths;
        }
    }
    return ths;
}
#pragma optimize_for_size off

} // extern "C"

bool CWorkSystemCache::wkStandbyLogin() {
    return CWorkThread::wkStandbyLogin();
}

#pragma optimize_for_size on
CWorkSystemCache::~CWorkSystemCache() {
    lbl_eu_806659C8 = NULL;
    CacheList* cache = &mCache;
    if (cache != NULL) {
        __dt___reslist_base_CCacheItem(cache, 0);
    }
}

void CWorkSystemCache::wkUpdate() {
    // Walk the singleton's resource list; logged-in items get released and
    // unlinked. The unlink helper takes &node and advances past the removed
    // entry, so the caller just resumes from the saved 'next' pointer.
    CacheListNode* node = lbl_eu_806659C8->mCache.mStartNodePtr->mNext;
    while (node != lbl_eu_806659C8->mCache.mStartNodePtr) {
        CacheListNode* next = node->mNext;
        SysCacheDecLoginRef(node->mItem);
        if (SysCacheItemWaitReady(node->mItem)) {
            if (node->mItem != NULL) {
                delete node->mItem;
                node->mItem = NULL;
            }
            SysCacheUnlinkFront(&lbl_eu_806659C8->mCache, CacheListIter(node));
        }
        node = next;
    }
}

#pragma optimize_for_size on
bool CWorkSystemCache::wkStandbyLogout() {
    CacheListNode* node = mCache.mStartNodePtr->mNext;
    while (node != mCache.mStartNodePtr) {
        if (node->mItem != NULL) {
            delete node->mItem;
            node->mItem = NULL;
        }
        node = node->mNext;
    }
    SysCacheClearListLinks(&mCache);
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

extern "C" void SysCacheDecLoginRef(CCacheItem* ths) {
    if (ths->field_0x48 == 0) {
        ths->field_0x44 -= 1;
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

// Foreign .sdata strings referenced by the locators below.

// Forward declarations for the .data block below (referenced by the .sdata
// locators above).
extern "C" u32 lbl_eu_8056FD10[];
extern "C" u32 lbl_eu_8056FD30[];
extern "C" u32 lbl_eu_8056FD24[];
extern "C" u32 lbl_eu_8056FD3C[];

// [.sdata] 0x80663B40-0x80663B58 (24B): RTTI locators + shared "ref"-style
// string pointers (u32* slots; same bytes as void* pointer tables).
extern "C" u32 __RTTI__16CWorkSystemCache[2] = { (u32)lbl_eu_80524568, (u32)lbl_eu_8056FD10 };
extern "C" u32 lbl_eu_80663B48[2] = { (u32)lbl_eu_8052457C, (u32)lbl_eu_8056FD30 };
extern "C" u32 lbl_eu_80663B50[2] = { (u32)lbl_eu_80524594, 0 };

// [.data] 0x8056FC70-0x8056FD48 (216B): CWorkSystemCache vtable + reslist
// RTTI base-list + reslist<CCacheItem*>/_reslist_base vtables.
extern "C" u32 lbl_eu_8056FC70[40] = {
    (u32)&__RTTI__16CWorkSystemCache, 0x00000000, (u32)&__dt__16CWorkSystemCacheFv,
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

// [.sbss] 0x806659C8-0x806659D0 (8B): singleton cache pointer + pad word.
CWorkSystemCache* lbl_eu_806659C8;
u32 lbl_eu_806659CC;
