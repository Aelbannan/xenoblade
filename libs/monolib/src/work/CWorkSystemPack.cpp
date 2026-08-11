// Auto-scaffolded catalog TU for monolib/src/work/CWorkSystemPack

#include <harness_catalog.h>

#include <cstring>

#include "monolib/work/CWorkThread.hpp"
#include "monolib/util/CPathUtil.hpp"
#include "monolib/util/MemManager.hpp"

// ---------------------------------------------------------------------------
// CPackItem (monolib/core/CPackItem.hpp): load-state field at 0x78, base name
// at 0x4, and the member calls used below (isNotLoaded / update / lookupFile /
// the vtable+8 virtual used by func_eu_804E2340). The class is only ever used
// through pointers in this TU, so no vtable is emitted here.
// ---------------------------------------------------------------------------
class CPackItem {
public:
    // The 3rd param carries the CRI partition id (stored to field_0x68; -1 =
    // none). Retail callers pass it even though the retail symbol mangles as
    // the 2-param __ct__9CPackItemFPCci (defaulted arg, so MWCC omits it).
    CPackItem(const char* name, int partitionId, int arg3 = 0);
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
    u8 pad_4[0x2C - 0x4];
    int unk2C;                        // 0x2C - CArcItem load state (2 = ready)
};

// POD mirror of _reslist_node<CPackItem*>.
struct PackItemListNode {
    PackItemListNode* mNext; // 0x0
    PackItemListNode* mPrev; // 0x4
    CPackItem* mItem;        // 0x8
};

// POD mirror of _reslist_base<CPackItem*> (size 0x20).
struct PackItemList {
    u32 m_vtable;                 // 0x0
    PackItemListNode* mStartNodePtr; // 0x4
    PackItemListNode mStartNode;  // 0x8
    PackItemListNode* mList;      // 0x14
    int mCapacity;                // 0x18
    bool unk1C;                   // 0x1C
};

// POD mirror of _reslist_node<CArcItem*>.
struct ArcItemListNode {
    ArcItemListNode* mNext; // 0x0
    ArcItemListNode* mPrev; // 0x4
    CArcItem* mItem;        // 0x8
};

// POD mirror of _reslist_base<CArcItem*> (size 0x20).
struct ArcItemList {
    u32 m_vtable;                 // 0x0
    ArcItemListNode* mStartNodePtr; // 0x4
    ArcItemListNode mStartNode;   // 0x8
    ArcItemListNode* mList;       // 0x14
    int mCapacity;                // 0x18
    bool unk1C;                   // 0x1C
};

struct CWorkSystemPack {
    CWorkSystemPack(const char* pName, CWorkThread* pParent);
    ~CWorkSystemPack();
    static bool func_804DDDF4(const char* pName, void* pOut, u32* pFileId);
    static bool func_804DDFBC(int pExcept);
    static bool func_804DE08C();
    static bool func_804DE100();
    bool wkStandbyLogin();
    bool wkStandbyLogout();
    void wkUpdate();

    u32 m_vtable;           // 0x0 - CWorkSystemPack vtable (lbl_eu_8056FE58)
    u8 pad_4[0x50 - 0x4];
    int mType;              // 0x50 - CWorkThread::ThreadType
    u8 pad_54[0x1C4 - 0x54];
    PackItemList mPackList; // 0x1C4 - reslist<CPackItem*>
    ArcItemList mArcList;   // 0x1E4 - reslist<CArcItem*>
};

// Work-system singleton pointer (.sbss).
extern CWorkSystemPack* lbl_eu_80665A10;
// _reslist_base<CPackItem*> / _reslist_base<CArcItem*> vtables (DOL data).
extern u8 lbl_eu_8056FF48[];
extern u8 lbl_eu_8056FF24[];
// Pack work-memory region handle (.sdata) and pack string table (.rodata).
extern u32 lbl_eu_80663BC8;
extern const char lbl_eu_80524714[];
// CWorkSystemPack / reslist<CPackItem*> / reslist<CArcItem*> vtables (DOL data).
extern u8 lbl_eu_8056FE58[];
extern u8 lbl_eu_8056FF30[];
extern u8 lbl_eu_8056FF0C[];
// Static filename registration tables (set by the Save* helpers below).
extern const char* const* lbl_eu_80665A14;
extern const char* const* lbl_eu_80665A18;

extern "C" {

// _reslist_base<CPackItem*>::clearList() - unlink every node (retail func_804DDADC).
__attribute__((never_inline)) void func_804DDADC(PackItemList* list) {
    PackItemListNode* r5 = list->mStartNodePtr->mNext;
    while (r5 != list->mStartNodePtr) {
        PackItemListNode* r4 = r5;
        r5 = r5->mNext;
        r4->mNext = 0;
    }
    list->mStartNodePtr->mNext = list->mStartNodePtr;
}

// _reslist_base<CArcItem*>::clearList() (retail func_804DDBD8).
__attribute__((never_inline)) void func_804DDBD8(ArcItemList* list) {
    ArcItemListNode* r5 = list->mStartNodePtr->mNext;
    while (r5 != list->mStartNodePtr) {
        ArcItemListNode* r4 = r5;
        r5 = r5->mNext;
        r4->mNext = 0;
    }
    list->mStartNodePtr->mNext = list->mStartNodePtr;
}

// CArcItem::func_804DEC30() - C-linkage in the retail binary.
void func_804DEC30(CArcItem* self);

// _reslist_base<CPackItem*>::~_reslist_base(int deleting)
__attribute__((never_inline)) void* __dt___reslist_base_CPackItem(PackItemList* self, int deleting) {
    if (self != 0) {
        self->m_vtable = (u32)lbl_eu_8056FF48;
        func_804DDADC(self);
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

// reslist<CPackItem*>::~reslist(int deleting)
void* __dt__reslist_CPackItem(PackItemList* self, int deleting) {
    if (self != 0) {
        __dt___reslist_base_CPackItem(self, 0);
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}

// _reslist_base<CArcItem*>::~_reslist_base(int deleting)
__attribute__((never_inline)) void* __dt___reslist_base_CArcItem(ArcItemList* self, int deleting) {
    if (self != 0) {
        self->m_vtable = (u32)lbl_eu_8056FF24;
        func_804DDBD8(self);
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

// reslist<CArcItem*>::~reslist(int deleting)
void* __dt__reslist_CArcItem(ArcItemList* self, int deleting) {
    if (self != 0) {
        __dt___reslist_base_CArcItem(self, 0);
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}

// func_eu_804520D0 is extern "C" in CDeviceFileCri.hpp, but including that
// header would clash with this TU's C++ definition of func_804DDCD4.
int func_eu_804520D0(const char*);

// Register one static arc filename: normalize the path through func_eu_804520D0,
// construct a CArcItem from the work region, and push it onto the singleton's
// arc list (first free slot of the node array). The setItem try/catch shape
// reproduces the retail frame marker (stw r1, 0x1c) from the reslist template.
#pragma optimize_for_size on
__attribute__((never_inline)) void func_804DDF00(const char* pPath) {
    ml::FixStr<0x100> str = pPath;
    func_eu_804520D0(str.mString);

    CArcItem* newItem = (CArcItem*)mtl::MemManager::allocate(0x58, CWorkThreadSystem::getWorkMem());
    if (newItem != 0) {
        new (newItem) CArcItem(str.mString);
    }

    CWorkSystemPack* sys = lbl_eu_80665A10;
    int i = 0;
    ArcItemListNode* startNode = sys->mArcList.mStartNodePtr;
    while (i < sys->mArcList.mCapacity) {
        if (sys->mArcList.mList[i].mNext == 0) break;
        i++;
    }
    ArcItemListNode* temp = &sys->mArcList.mList[i];
    CArcItem** pSlot = &temp->mItem;
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
#pragma optimize_for_size off

// Register one static pkh filename: pick the lowest partition index not yet
// used by any pack item, construct the CPackItem (CRI partition id from the
// caller), and push it onto the singleton's pack list (first free slot).
#pragma optimize_for_size on
__attribute__((never_inline)) void func_804DDE3C(const char* pName, int pPartitionId) {
    CWorkSystemPack* sys = lbl_eu_80665A10;
    int i = 0;
    PackItemListNode* sentinel = sys->mPackList.mStartNodePtr;
    PackItemListNode* first = sentinel->mNext;
    while (true) {
        PackItemListNode* node = first;
        bool found = false;
        while (node != sentinel) {
            if (node->mItem->mAdxPartitionId == i) {
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
        new (newItem) CPackItem(pName, i, pPartitionId);
    }

    int j = 0;
    PackItemListNode* startNode = sys->mPackList.mStartNodePtr;
    while (j < sys->mPackList.mCapacity) {
        if (sys->mPackList.mList[j].mNext == 0) break;
        j++;
    }
    PackItemListNode* temp = &sys->mPackList.mList[j];
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
#pragma optimize_for_size off

} // extern "C"

#pragma optimize_for_size on
CWorkSystemPack::CWorkSystemPack(const char* pName, CWorkThread* pParent) {
    // Base thread init (placement-new: the mirror class has no CWorkThread base).
    new (this) CWorkThread(pName, pParent, 0);

    // CWorkSystemPack vtable, then the two reslists. Each list's inlined ctor
    // chain (_reslist_base then reslist) stores its vtable twice: base, then
    // derived (e.g. pack: lbl_eu_8056FF48 then lbl_eu_8056FF30).
    mPackList.mList = 0;
    m_vtable = (u32)lbl_eu_8056FE58;
    mPackList.m_vtable = (u32)lbl_eu_8056FF48;
    mPackList.mCapacity = 0;
    mPackList.unk1C = false;
    PackItemListNode* pStart = &mPackList.mStartNode;
    mPackList.mStartNodePtr = pStart;
    mPackList.mStartNode.mNext = pStart;
    mPackList.mStartNode.mPrev = pStart;
    mPackList.m_vtable = (u32)lbl_eu_8056FF30;

    mArcList.m_vtable = (u32)lbl_eu_8056FF24;
    mArcList.mList = 0;
    mArcList.mCapacity = 0;
    mArcList.unk1C = false;
    ArcItemListNode* aStart = &mArcList.mStartNode;
    mArcList.mStartNodePtr = aStart;
    mArcList.mStartNode.mNext = aStart;
    mArcList.mStartNode.mPrev = aStart;
    mArcList.m_vtable = (u32)lbl_eu_8056FF0C;

    lbl_eu_80665A10 = this;
    mType = CWorkThread::THREAD_CWORKSYSTEMPACK;

    // Node arrays: the arc list starts with one free slot, the pack list with 16.
    mArcList.mList = (ArcItemListNode*)mtl::MemManager::allocate_array(0xC, CWorkThreadSystem::getWorkMem());
    mArcList.mList[0].mNext = 0;
    mArcList.mCapacity = 1;

    mPackList.mList = (PackItemListNode*)mtl::MemManager::allocate_array(0xC0, CWorkThreadSystem::getWorkMem());
    for (int i = 0; i < 16; i++) {
        mPackList.mList[i].mNext = 0;
    }
    mPackList.mCapacity = 16;

    // Register the static arc / pkh filename tables (both NULL-terminated).
    if (lbl_eu_80665A18 != 0) {
        const char* const* p = lbl_eu_80665A18;
        while (*p != 0) {
            func_804DDF00(*p);
            p++;
        }
    }
    if (lbl_eu_80665A14 != 0) {
        const char* const* p = lbl_eu_80665A14;
        while (*p != 0) {
            func_804DDE3C(*p, -1);
            p++;
        }
    }
}
#pragma optimize_for_size off

CWorkSystemPack::~CWorkSystemPack() {
    // Clear the work-system singleton first, then tear down the two resource
    // lists (arc items at +0x1E4, pack items at +0x1C4) and the base thread.
    lbl_eu_80665A10 = 0;
    if (&mArcList != 0) {
        __dt___reslist_base_CArcItem(&mArcList, 0);
    }
    if (&mPackList != 0) {
        __dt___reslist_base_CPackItem(&mPackList, 0);
    }
    ((CWorkThread*)this)->CWorkThread::~CWorkThread();
}

// Walk the pack list, comparing each item's base name against pName. Returns
// the lookupFile() result (found flag) for the matching item, or 0 if no item
// matches.
__attribute__((never_inline)) bool func_804DDD54(const char* pName, const char* pPath, char** outPkbPath,
                   u32* outEntryId, u32* outIndex, u32* outFileId) {
    const char* pFile = ml::CPathUtil::getFilePtrFromPath(pPath);
    CWorkSystemPack* sys = lbl_eu_80665A10;
    PackItemListNode* sentinel = sys->mPackList.mStartNodePtr;
    PackItemListNode* node = sentinel->mNext;
    while (node != sentinel) {
        if (strcmp(node->mItem->mBaseName, pName) != 0) {
            node = node->mNext;
            continue;
        }
        return node->mItem->lookupFile(pFile, outPkbPath, outEntryId, outIndex,
                                       outFileId);
    }
    return false;
}

// Resolve a pack file id for pPath: first via the static lookup, then via the
// item search (returns the file id << 11). Returns -1 if neither matched.
s32 func_804DDCD4(const char* pName, const char* pPath) {
    // Declared last-to-first: MWCC assigns stack slots in reverse declaration
    // order, so this yields retail's 0x8/0xc/0x10/0x14/0x18 slot layout.
    u32 v18;  // 0x18
    u32 v14;  // 0x14
    u32 v10;  // 0x10
    u32 vC;   // 0xc - file id
    u32 v8;   // 0x8

    if (CWorkSystemPack::func_804DDDF4(pPath, &v8, &vC)) {
        return (s32)vC;
    }
    if (func_804DDD54(pName, pPath, (char**)&v18, &v14, &v10, &vC)) {
        return (s32)(vC << 11);
    }
    return -1;
}

// Login gate: walk the circular pack-item list (head at +0x1E8, nodes link
// via +0, data pointer at +8) and require every item's field_0x2C == 2.
bool CWorkSystemPack::func_804DE100() {
    CWorkSystemPack* sys = lbl_eu_80665A10;
    if (sys == 0) return false;
    u8* head = *(u8**)((u8*)sys + 0x1E8);
    u8* n = *(u8**)head;
    for (; n != head; n = *(u8**)n) {
        if (*(s32*)(*(u8**)(n + 8) + 0x2C) != 2) return false;
    }
    return true;
}

// Login gate: every arc item must be in load-state 2 (ready); if one isn't,
// try to advance it (func_804DEC30) and abort the login. Then ensure the pack
// work-memory region exists, and make sure every pack item is loaded
// (advancing with update() if not).
#pragma optimize_for_size on
bool CWorkSystemPack::wkStandbyLogin() {
    ArcItemListNode* sentinel = mArcList.mStartNodePtr;
    ArcItemListNode* node = sentinel->mNext;
    while (node != sentinel) {
        CArcItem* item = node->mItem;
        if (item->unk2C != 2) {
            func_804DEC30(item);
            return false;
        }
        node = node->mNext;
    }

    if (lbl_eu_80663BC8 == 0xFFFFFFFF) {
        lbl_eu_80663BC8 = mtl::MemManager::create(mtl::MemManager::getHandleMEM2(), 0x12000, lbl_eu_80524714);
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
#pragma optimize_for_size off

// Logout gate: notify every pack/arc item via its vtable+8 destroy hook (the
// nested double null-check mirrors the retail source shape), then clear both
// lists, release their node arrays and the pack work-memory region.
#pragma optimize_for_size on
bool CWorkSystemPack::wkStandbyLogout() {
    PackItemListNode* node = mPackList.mStartNodePtr->mNext;
    while (node != mPackList.mStartNodePtr) {
        CPackItem* item = node->mItem;
        if (item != 0) {
            if (item != 0) {
                item->func_0x8(1);
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
                aitem->func_0x8(1);
            }
            anode->mItem = 0;
        }
        anode = anode->mNext;
    }

    func_804DDBD8(&mArcList);
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

    mtl::MemManager::erase(lbl_eu_80663BC8);
    return ((CWorkThread*)this)->CWorkThread::wkStandbyLogout();
}
#pragma optimize_for_size off

const char* const* lbl_eu_80665A14;

// Store the PKH filenames array pointer for later retrieval by the work system.
void SavePkhFilenamesArrayPtr__15CWorkSystemPackFPCPCc(const char* const* pArray) {
    lbl_eu_80665A14 = pArray;
}

const char* const* lbl_eu_80665A18;

// Store the static arc filename string pointer for later retrieval by the work system.
void SaveStaticArcFilenameStringPtr__15CWorkSystemPackFPCPCc(const char* const* pFilenameStr) {
    lbl_eu_80665A18 = pFilenameStr;
}

// Search the pack list for an item whose base name matches pName.
int func_804DE010(const char* pName) {
    CWorkSystemPack* sys = lbl_eu_80665A10;
    if (sys == 0) return 0;
    PackItemListNode* sentinel = sys->mPackList.mStartNodePtr;
    PackItemListNode* node = sentinel->mNext;
    while (node != sentinel) {
        if (strcmp(node->mItem->mBaseName, pName) == 0) {
            return 1;
        }
        node = node->mNext;
    }
    return 0;
}

// Remove the pack item whose base name matches pName from the pack list,
// destroying the item (vtable+8 call) and unlinking its node.
void func_eu_804E2340(const char* pName) {
    CWorkSystemPack* sys = lbl_eu_80665A10;
    PackItemListNode* sentinel = sys->mPackList.mStartNodePtr;
    PackItemListNode* node = sentinel->mNext;
    while (node != sentinel) {
        if (strcmp(node->mItem->mBaseName, pName) != 0) {
            node = node->mNext;
            continue;
        }
        // Retail reloads node->mItem after the strcmp call (fresh load into r3)
        // and tests it with the nested double-if, so don't keep a local across
        // the call.
        if (node->mItem != 0) {
            if (node->mItem != 0) {
                node->mItem->func_0x8(1);
            }
            node->mItem = 0;
        }
        node->mPrev->mNext = node->mNext;
        node->mNext->mPrev = node->mPrev;
        node->mNext = 0;
        break;
    }
}

// Returns false if any pack item other than pExcept is currently mid-load
// (LOAD_STATE_LOADING_AHX_ADX_FILE == 2).
bool CWorkSystemPack::func_804DDFBC(int pExcept) {
    CPackItem* item;
    PackItemListNode* sentinel;
    PackItemListNode* node;
    if (lbl_eu_80665A10 == 0) return false;
    sentinel = lbl_eu_80665A10->mPackList.mStartNodePtr;
    node = sentinel->mNext;
    while (node != sentinel) {
        item = node->mItem;
        if ((u32)item != (u32)pExcept && item->mLoadState == 2) return false;
        node = node->mNext;
    }
    return true;
}

// Returns false while any pack item is still not loaded.
bool CWorkSystemPack::func_804DE08C() {
    CWorkSystemPack* sys = lbl_eu_80665A10;
    if (sys == 0) return false;
    PackItemListNode* node = sys->mPackList.mStartNodePtr->mNext;
    while (node != lbl_eu_80665A10->mPackList.mStartNodePtr) {
        if (node->mItem->isNotLoaded()) return false;
        node = node->mNext;
    }
    return true;
}

// Update the arc-item list, then the pack-item list.
void CWorkSystemPack::wkUpdate() {
    ArcItemListNode* node = lbl_eu_80665A10->mArcList.mStartNodePtr->mNext;
    while (node != lbl_eu_80665A10->mArcList.mStartNodePtr) {
        func_804DEC30(node->mItem);
        node = node->mNext;
    }
    PackItemListNode* pnode = lbl_eu_80665A10->mPackList.mStartNodePtr->mNext;
    while (pnode != lbl_eu_80665A10->mPackList.mStartNodePtr) {
        pnode->mItem->update();
        pnode = pnode->mNext;
    }
}