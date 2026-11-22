// Auto-scaffolded catalog TU for monolib/src/work/CWorkSystemPack

#include <harness_catalog.h>

#include <cstring>

#include "monolib/work/CWorkThread.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkSystemPack.hpp"
#include "monolib/device/CDeviceFileCri.hpp" // canonical DevFile_SubstLangPath C++ decl

// ---------------------------------------------------------------------------
// CPackItem (monolib/core/CPackItem.hpp): load-state field at 0x78, base name
// at 0x4, and the member calls used below (isNotLoaded / update / lookupFile /
// the vtable+8 virtual used by removePackFile). The class is only ever used
// through pointers in this TU, so no vtable is emitted here.
// ---------------------------------------------------------------------------
class CPackItem {
public:
    CPackItem(const char* name, int partitionId);
    virtual ~CPackItem();              // vtable+0
    virtual bool func_0x4();           // vtable+4
    virtual bool func_0x8(int flag);   // vtable+8
    bool isNotLoaded();
    void update();
    bool lookupFile(const char* filename, char** outPkbPath, u32* outEntryId,
                    u32* outIndex, u32* outFileId);

    char mBaseName[32];                // 0x4
    u8 pad_0x24[0x64 - 0x24];          // 0x24..0x64: vtable + fields above partition id
    int mAdxPartitionId;               // 0x64
    u8 pad_0x68[0x78 - 0x68];
    s32 mLoadState;                    // 0x78 - CPackItem::LoadState
};

// CArcItem (monolib/core/CArcItem.hpp): local mirror exposing only the members
// this TU uses (load-state field at 0x2C, vtable+8 destroy hook, ctor).
struct CArcItem {
    CArcItem(const char* pFilename);
    virtual ~CArcItem();              // vtable+0
    virtual bool func_0x4();          // vtable+4
    virtual bool func_0x8(int flag);  // vtable+8
    void updateLoadState();
    bool findFileEntry(const char* pPath, void** pOutStartAddr, u32* pOutLength);
    u8 pad_4[0x2C - 0x4];
    int unk2C;                        // 0x2C - CArcItem load state (2 = ready)
};

// Cast-only destroy-hook interface: the retail vtable+8 slot is a destroy
// hook. Under -RTTI on the FIRST declared virtual lands at +8 (two hidden
// RTTI slots precede it), so this minimal one-virtual interface reproduces
// the retail `lwz r12, 8(r12)` dispatch that CPackItem/CArcItem's own
// {dtor, func_0x4, func_0x8} layout would push to +16.
struct CPackItemDestroyHook {
    virtual bool destroy(int flag);    // vtable+8
};
struct CArcItemDestroyHook {
    virtual bool destroy(int flag);    // vtable+8
};

// Work-system singleton pointer (.sbss).
// Pack work-memory region handle (.sdata) and pack string table (.rodata).
extern u32 lbl_eu_80663BC8[2];
extern const char lbl_eu_80524714[];

extern "C" {

// _reslist_base<CPackItem*>::clearList() - unlink every node (retail func_804DDADC).
 void func_804DDADC(PackItemListBase* list) {
    PackItemListNode* walk = list->mStartNodePtr->mNext;
    while (walk != list->mStartNodePtr) {
        PackItemListNode* node = walk;
        walk = walk->mNext;
        node->mNext = 0;
    }
    list->mStartNodePtr->mNext = list->mStartNodePtr;
}

// _reslist_base<CArcItem*>::clearList() (retail clearArcItemList).
 void clearArcItemList(ArcItemListBase* list) {
    ArcItemListNode* walk = list->mStartNodePtr->mNext;
    while (walk != list->mStartNodePtr) {
        ArcItemListNode* node = walk;
        walk = walk->mNext;
        node->mNext = 0;
    }
    list->mStartNodePtr->mNext = list->mStartNodePtr;
}


} // extern "C"

// The real reslist specializations emit the retail vtable/RTTI objects.
template <>
_reslist_base<CPackItem*>::~_reslist_base() {
    func_804DDADC(this);
    if (unk1C == false && mList != 0) {
        delete[] mList;
        mList = 0;
    }
}

// reslist<CPackItem*>::~reslist()
template <>
reslist<CPackItem*>::~reslist() {
}

// _reslist_base<CArcItem*>::~_reslist_base(int deleting)
template <>
_reslist_base<CArcItem*>::~_reslist_base() {
    clearArcItemList(this);
    if (unk1C == false && mList != 0) {
        delete[] mList;
        mList = 0;
    }
}

// reslist<CArcItem*>::~reslist()
template <>
reslist<CArcItem*>::~reslist() {
}

extern "C" {

// Register one static arc filename: normalize the path through DevFile_SubstLangPath,
// construct a CArcItem from the work region, and push it onto the singleton's
// arc list (first free slot of the node array). The setItem try/catch shape
// reproduces the retail frame marker (stw r1, 0x1c) from the reslist template.
 void registerArcFile(const char* pPath) {
    ml::FixStr<0x100> str = pPath;
    DevFile_SubstLangPath(str.mString);

    // Named pointer web parks &str in a callee-saved reg across the two
    // calls (retail shape); the allocation result feeds placement-new
    // directly so it never claims a saved register - MWCC's intrinsic
    // placement-new null guard is the cmpwi/beq retail shows.
    char* pStr = str.mString;
    // Split webs: 'raw' dies inside the new-expression, 'newItem' is born
    // from the placement-new result (the ctor's r3 return) so it survives
    // to the list insert without claiming a saved register.
    CArcItem* raw = (CArcItem*)mtl::MemManager::allocate(0x58, CWorkThreadSystem::getWorkMem());
    CArcItem* newItem = new (raw) CArcItem(pStr);

    CWorkSystemPack* sys = sWorkSystemPackInstance;
    int i = 0;
    int cap = sys->mArcList.mCapacity;
    ArcItemListNode* startNode = sys->mArcList.mStartNodePtr;
    while (i < cap) {
        if (sys->mArcList.mList[i].mNext == 0) break;
        i++;
    }
    CArcItem** pSlot = &sys->mArcList.mList[i].mItem;
    ArcItemListNode* temp = &sys->mArcList.mList[i];
    if (pSlot != 0) {
        try {
            *pSlot = newItem;
        } catch (...) {
            throw;
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
}

// Register one static pkh filename: pick the lowest partition index not yet
// used by any pack item, construct the CPackItem (CRI partition id from the
// caller), and push it onto the singleton's pack list (first free slot).
// Retail calls the 2-param CPackItem ctor with a THIRD (ignored) argument
// (the function's own arg2 forwarded), keeping r29 live across the allocate
// calls. Declared under the retail mangled name to reproduce that call.
extern "C" CPackItem* __ct__9CPackItemFPCci(CPackItem* mem, const char* name, int idx, int partition);
 void registerPackFile(const char* pName, int pPartitionId) {
    // No local singleton copy: retail reloads sWorkSystemPackInstance (SDA) at each
    // use site, keeping only pName/pPartitionId/i/newItem in callee-saved regs.
    int i = 0;
    PackItemListNode* sentinel = sWorkSystemPackInstance->mPackList.mStartNodePtr;
    while (true) {
        PackItemListNode* node = sentinel->mNext;
        bool found = false;
        while (node != sentinel) {
            if (i == node->mItem->mAdxPartitionId) {
                found = true;
                break;
            }
            node = node->mNext;
        }
        if (!found) break;
        i++;
    }

    CPackItem* newItem = (CPackItem*)mtl::MemManager::allocate(0x8C, CWorkThreadSystem::getWorkMem());
    if (newItem != 0) {
        newItem = __ct__9CPackItemFPCci(newItem, pName, i, pPartitionId);
    }

    CWorkSystemPack* sys2 = sWorkSystemPackInstance;
    int j = 0;
    int cap = sys2->mPackList.mCapacity;
    PackItemListNode* startNode = sys2->mPackList.mStartNodePtr;
    PackItemListNode* temp;
    while (j < cap) {
        if (sys2->mPackList.mList[j].mNext == 0) break;
        j++;
    }
    temp = &sys2->mPackList.mList[j];
    CPackItem** pSlot = &temp->mItem;
    if (pSlot != 0) {
        try {
            *pSlot = newItem;
        } catch (...) {
            throw;
        }
    }
    temp->mNext = startNode;
    temp->mPrev = startNode->mPrev;
    startNode->mPrev->mNext = temp;
    startNode->mPrev = temp;
}

} // extern "C"

// Inlined alloc+clear used by the ctor: going through the list pointer keeps
// MWCC reloading mList every iteration (retail shape).
static void InitPackNodeArray(PackItemList* list, u32 bytes, int count) {
    list->mList = (PackItemListNode*)mtl::MemManager::allocate_array(bytes, CWorkThreadSystem::getWorkMem());
    for (int j = 0; j < count; j++) {
        list->mList[j].mNext = 0;
    }
    list->mCapacity = count;
}
// Base ctor runs in the init list (retail emits the plain __ct__11CWorkThread
// bl with capacity 0); the class is novtable so the retail vtable is stored
// manually (CWorkSystemCache pattern).
CWorkSystemPack::CWorkSystemPack(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 0) {
    int i; // declared early: birth order keeps the loop index off the zero-reg copy path

    // The real polymorphic class emits its own retail vptr store in this
    // position; the reslist member ctors then run in declaration order.
    sWorkSystemPackInstance = this;
    mType = CWorkThread::THREAD_CWORKSYSTEMPACK;

    // Node arrays: the arc list starts with one free slot, the pack list with 16.
    mArcList.mList = (ArcItemListNode*)mtl::MemManager::allocate_array(0xC, CWorkThreadSystem::getWorkMem());
    mArcList.mList[0].mNext = 0;
    mArcList.mCapacity = 1;

    mPackList.mList = (PackItemListNode*)mtl::MemManager::allocate_array(0xC0, CWorkThreadSystem::getWorkMem());
    int j = 0;
    while (j < 16) {
        mPackList.mList[j].mNext = 0;
        j++;
    }
    mPackList.mCapacity = 16;

    // Register the static arc / pkh filename tables (both NULL-terminated).
    // 'it = p - 1; while (*++it)' reproduces retail's subi/lwzu walk rotation.
    const char* const* p = spStaticArcFilenames[0];
    if (p != 0) {
        const char* const* it = p - 1;
        while (*++it != 0) {
            registerArcFile(*it);
        }
    }
    const char* const* q = spPkhFilenamesArray;
    if (q != 0) {
        const char* const* jt = q - 1;
        while (*++jt != 0) {
            registerPackFile(*jt, -1);
        }
    }
}

CWorkSystemPack::~CWorkSystemPack() {
    sWorkSystemPackInstance = 0;
}

// Walk the pack list, comparing each item's base name against pName. Returns
// the lookupFile() result (found flag) for the matching item, or 0 if no item
// matches.
// Retail symbol is unmangled - this helper has C linkage in the original.
// Retail's addic/subfe setnz bool tail is the -O4,s (unit flag) signature.
extern "C" bool func_804DDD54(const char* pName, const char* pPath, char** outPkbPath,
                   u32* outEntryId, u32* outIndex, u32* outFileId) {
    const char* pFile = ml::CPathUtil::getFilePtrFromPath(pPath);
    PackItemListNode* sentinel;
    PackItemListNode* node;
    sentinel = sWorkSystemPackInstance->mPackList.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        if (!strcmp(node->mItem->mBaseName, pName)) {
            return node->mItem->lookupFile(pFile, outPkbPath, outEntryId,
                                           outIndex, outFileId);
        }
        node = node->mNext;
    }
    return false;
}

// Resolve a pack file id for pPath: first via the static lookup, then via the
// item search (returns the file id << 11). Returns -1 if neither matched.
// Static lookup: walk the singleton's circular pack-item list (head at
// +0x1E8, nodes link via +0, item pointer at +8) and forward to the first
// item's findFileEntry when the list is non-empty (retail unmangled reloc).
s32 findPackFileId(const char* pName, const char* pPath) {
    // Declared last-to-first: MWCC assigns stack slots in reverse declaration
    // order, so this yields retail's 0x8/0xc/0x10/0x14/0x18 slot layout.
    u32 v18;  // 0x18
    u32 v14;  // 0x14
    u32 v10;  // 0x10
    u32 vC;   // 0xc - file id
    u32 v8;   // 0x8

    if (CWorkSystemPack::findPackResource(pPath, &v8, &vC)) {
        return (s32)vC;
    }
    if (func_804DDD54(pName, pPath, (char**)&v18, &v14, &v10, &vC)) {
        return (s32)(vC << 11);
    }
    return -1;
}

// Static lookup: walk the singleton's circular pack-item list (head at
// +0x1E8, nodes link via +0, item pointer at +8) and forward to the first
// item's findFileEntry when the list is non-empty.
// keeps retail's out-of-line bl - without it -inline auto
// folds this small helper into findPackFileId below.
bool CWorkSystemPack::findPackResource(const char* pName, void* pOut, u32* pFileId) {
    CWorkSystemPack* sys = sWorkSystemPackInstance;
    u8* head = (u8*)sys->mArcList.mStartNodePtr;
    u8* first = *(u8**)head;
    if (first != head) {
        return ((ArcItemListNode*)first)->mItem->findFileEntry(pName, (void**)pOut, pFileId);
    }
    return false;
}

// Login gate: walk the circular pack-item list (head at +0x1E8, nodes link
// via +0, data pointer at +8) and require every item's field_0x2C == 2.
bool CWorkSystemPack::areArcsReady() {
    CWorkSystemPack* sys = sWorkSystemPackInstance;
    if (sys == 0) return false;
    u8* head = (u8*)sys->mArcList.mStartNodePtr;
    u8* n = *(u8**)head;
    for (; n != head; n = *(u8**)n) {
        if (((ArcItemListNode*)n)->mItem->unk2C != 2) return false;
    }
    return true;
}

// Login gate: every arc item must be in load-state 2 (ready); if one isn't,
// try to advance it (updateLoadState) and abort the login. Then ensure the pack
// work-memory region exists, and make sure every pack item is loaded
// (advancing with update() if not).
bool CWorkSystemPack::wkStandbyLogin() {
    ArcItemListNode* sentinel = mArcList.mStartNodePtr;
    ArcItemListNode* node = sentinel->mNext;
    while (node != sentinel) {
        CArcItem* item = node->mItem;
        if (item->unk2C != 2) {
            item->updateLoadState();
            return false;
        }
        node = node->mNext;
    }

    if (lbl_eu_80663BC8[0] == 0xFFFFFFFF) {
        lbl_eu_80663BC8[0] = mtl::MemManager::create(mtl::MemManager::getHandleMEM2(), 0x12000, lbl_eu_80524714);
    }

    PackItemListNode* pnode = mPackList.mStartNodePtr->mNext;
    while (pnode != mPackList.mStartNodePtr) {
        if (pnode->mItem->isNotLoaded()) {
            pnode->mItem->update();
            return false;
        }
        pnode = pnode->mNext;
    }

    return ((CWorkThread*)this)->CWorkThread::wkStandbyLogin();
}

// Logout gate: notify every pack/arc item via its vtable+8 destroy hook (the
// nested double null-check mirrors the retail source shape), then clear both
// lists, release their node arrays and the pack work-memory region.
bool CWorkSystemPack::wkStandbyLogout() {
    PackItemListNode* node = mPackList.mStartNodePtr->mNext;
    while (node != mPackList.mStartNodePtr) {
        CPackItem* item = node->mItem;
        if (item != 0) {
            if (item != 0) {
                ((CPackItemDestroyHook*)item)->destroy(1);
            }
            node->mItem = 0;
        }
        node = node->mNext;
    }

    ArcItemListNode* anode = mArcList.mStartNodePtr->mNext;
    while (anode != mArcList.mStartNodePtr) {
        CArcItem* aitem = anode->mItem;
        if (aitem != 0) {
            if (aitem != 0) {
                ((CArcItemDestroyHook*)aitem)->destroy(1);
            }
            anode->mItem = 0;
        }
        anode = anode->mNext;
    }

    clearArcItemList(&mArcList);
    if (mArcList.unk1C == 0 && mArcList.mList != 0) {
        delete[] mArcList.mList;
        mArcList.mList = 0;
    }
    mArcList.mCapacity = 0;

    func_804DDADC(&mPackList);
    if (mPackList.unk1C == 0 && mPackList.mList != 0) {
        delete[] mPackList.mList;
        mPackList.mList = 0;
    }
    mPackList.mCapacity = 0;

    mtl::MemManager::erase(lbl_eu_80663BC8[0]);
    return ((CWorkThread*)this)->CWorkThread::wkStandbyLogout();
}

// Store the PKH filenames array pointer for later retrieval by the work system.
void CWorkSystemPack::SavePkhFilenamesArrayPtr(const char* const* pArray) {
    spPkhFilenamesArray = pArray;
}

// Store the static arc filename string pointer for later retrieval by the work system.
void CWorkSystemPack::SaveStaticArcFilenameStringPtr(const char* const* pFilenameStr) {
    spStaticArcFilenames[0] = pFilenameStr;
}

// Search the pack list for an item whose base name matches pName.
int hasPackFile(const char* pName) {
    CWorkSystemPack* sys = sWorkSystemPackInstance;
    PackItemListNode* sentinel;
    PackItemListNode* node;
    if (sys == 0) return 0;
    sentinel = sys->mPackList.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        bool match = strcmp(node->mItem->mBaseName, pName) == 0;
        if (match) {
            return 1;
        }
        node = node->mNext;
    }
    return 0;
}

// Remove the pack item whose base name matches pName from the pack list,
// destroying the item (vtable+8 call) and unlinking its node.
void removePackFile(const char* pName) {
    // Don't cache the singleton in a local: retail only keeps 3 values live
    // (pName, sentinel, node) and saves r29-r31.
    PackItemListNode* sentinel = sWorkSystemPackInstance->mPackList.mStartNodePtr;
    PackItemListNode* node = sentinel->mNext;
    while (node != sentinel) {
        // Named bool with == 0 reproduces retail's addic.-1/subfe. setnz tail.
        bool match = strcmp(node->mItem->mBaseName, pName) == 0;
        if (match) {
            // Retail reloads node->mItem after the strcmp call (fresh load into
            // r3) and tests it with the nested double-if, so don't keep a local
            // across the call.
            if (node->mItem != 0) {
                if (node->mItem != 0) {
                    ((CPackItemDestroyHook*)node->mItem)->destroy(1);
                }
                node->mItem = 0;
            }
            node->mPrev->mNext = node->mNext;
            node->mNext->mPrev = node->mPrev;
            node->mNext = 0;
            break;
        }
        node = node->mNext;
    }
}

// Returns false if any pack item other than pExcept is currently mid-load
// (LOAD_STATE_LOADING_AHX_ADX_FILE == 2).
bool CWorkSystemPack::isPackLoadIdle(int pExcept) {
    CPackItem* item;
    PackItemListNode* sentinel;
    PackItemListNode* node;
    if (sWorkSystemPackInstance == 0) return false;
    sentinel = sWorkSystemPackInstance->mPackList.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        item = node->mItem;
        if ((u32)item != (u32)pExcept && item->mLoadState == 2) return false;
        node = node->mNext;
    }
    return true;
}

// Returns false while any pack item is still not loaded.
bool CWorkSystemPack::arePacksLoaded() {
    CWorkSystemPack* sys = sWorkSystemPackInstance;
    if (sys == 0) return false;
    PackItemListNode* node = sys->mPackList.mStartNodePtr->mNext;
    while (node != sWorkSystemPackInstance->mPackList.mStartNodePtr) {
        if (node->mItem->isNotLoaded()) return false;
        node = node->mNext;
    }
    return true;
}

// Update the arc-item list, then the pack-item list.
void CWorkSystemPack::wkUpdate() {
    ArcItemListNode* node = sWorkSystemPackInstance->mArcList.mStartNodePtr->mNext;
    while (node != sWorkSystemPackInstance->mArcList.mStartNodePtr) {
        node->mItem->updateLoadState();
        node = node->mNext;
    }
    PackItemListNode* pnode = sWorkSystemPackInstance->mPackList.mStartNodePtr->mNext;
    while (pnode != sWorkSystemPackInstance->mPackList.mStartNodePtr) {
        pnode->mItem->update();
        pnode = pnode->mNext;
    }
}

// [.sdata] 0x80663BC8-0x80663BD0 (8B): pack work-memory region handle.
extern "C" u32 lbl_eu_80663BC8[2] = { 0xFFFFFFFF, 0x00000000 };

// [.sbss] 0x80665A10-0x80665A20 (16B).
CWorkSystemPack* sWorkSystemPackInstance;
const char* const* spPkhFilenamesArray;
const char* const* spStaticArcFilenames[2];
