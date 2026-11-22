// Auto-scaffolded catalog TU for monolib/src/work/CWorkSystemCache

#include <harness_catalog.h>

#include "monolib/work/CWorkThread.hpp"
#include "monolib/math/CMat34.hpp"
#include "monolib/util/MemManager.hpp"

#include "monolib/work/CWorkSystemCache.hpp"
CWorkSystemCache* sCacheInstance; // name-guess: singleton at retail 0x806659C8; no recovered name in symbols.txt
extern "C" {

// _reslist_base<CCacheItem>::clearList() - walk nodes clearing only mNext.
 void SysCacheClearListLinks(_reslist_base<CCacheItem*>* list) {
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
    endNode = sCacheInstance->mCache.mStartNodePtr;
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
    CacheListNode* end = sCacheInstance->mCache.mStartNodePtr;
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

} // extern "C"

// _reslist_base<CCacheItem*>::~_reslist_base(): the real template destructor
// emits the retail FD3C vptr store, ring clear, array free and deleting path.
#pragma optimize_for_size on
template <> _reslist_base<CCacheItem*>::~_reslist_base() {
    SysCacheClearListLinks(this);
    if (unk1C == false && mList != 0) {
        delete[] mList;
        mList = 0;
    }
}
#pragma optimize_for_size off

bool CWorkSystemCache::wkStandbyLogin() {
    return CWorkThread::wkStandbyLogin();
}

#pragma optimize_for_size on
CWorkSystemCache::~CWorkSystemCache() {
    sCacheInstance = NULL;
}

void CWorkSystemCache::wkUpdate() {
    // Walk the singleton's resource list; logged-in items get released and
    // unlinked. The unlink helper takes &node and advances past the removed
    // entry, so the caller just resumes from the saved 'next' pointer.
    CacheListNode* node = sCacheInstance->mCache.mStartNodePtr->mNext;
    while (node != sCacheInstance->mCache.mStartNodePtr) {
        CacheListNode* next = node->mNext;
        SysCacheDecLoginRef(node->mItem);
        if (SysCacheItemWaitReady(node->mItem)) {
            if (node->mItem != NULL) {
                delete node->mItem;
                node->mItem = NULL;
            }
            SysCacheUnlinkFront(&sCacheInstance->mCache, CacheListIter(node));
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
    sCacheInstance = this;
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
// Compiler-emitted data owns the vtables, base lists, RTTI locators and
// RTTI name strings for CWorkSystemCache, reslist<CCacheItem*> and
// _reslist_base<CCacheItem*>.

// [.sbss] 0x806659C8-0x806659D0 (8B): singleton cache pointer + pad word.
u32 lbl_eu_806659CC;
