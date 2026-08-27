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

class __declspec(novtable) CWorkSystemPack : public CWorkThread {
public:
    CWorkSystemPack(const char* pName, CWorkThread* pParent);
    ~CWorkSystemPack();
    static bool findPackResource(const char* pName, void* pOut, u32* pFileId);
    static bool isPackLoadIdle(int pExcept);
    static bool arePacksLoaded();
    static bool areArcsReady();
    bool wkStandbyLogin();
    bool wkStandbyLogout();
    void wkUpdate();

    // 0x0: vtable
    // 0x0-0x1c4: CWorkThread (mType at 0x50)
    PackItemList mPackList; // 0x1C4 - reslist<CPackItem*>
    ArcItemList mArcList;   // 0x1E4 - reslist<CArcItem*>
};

// Work-system singleton pointer (.sbss).
extern CWorkSystemPack* lbl_eu_80665A10;
// _reslist_base<CPackItem*> / _reslist_base<CArcItem*> vtables (DOL data).
extern u32 lbl_eu_8056FF48[4];
extern u32 lbl_eu_8056FF24[3];
// Pack work-memory region handle (.sdata) and pack string table (.rodata).
extern u32 lbl_eu_80663BC8[2];
extern const char lbl_eu_80524714[];
// CWorkSystemPack / reslist<CPackItem*> / reslist<CArcItem*> vtables (DOL data).
extern u32 lbl_eu_8056FE58[40];
extern u32 lbl_eu_8056FF30[3];
extern u32 lbl_eu_8056FF0C[3];
// Static filename registration tables (set by the Save* helpers below).
extern const char* const* lbl_eu_80665A14;
extern const char* const* lbl_eu_80665A18[2];

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
// optimize_for_size matches retail's stmw/lmw frame for the 2 saved regs.
#pragma push
#pragma optimize_for_size on
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
#pragma pop

// reslist<CPackItem*>::~reslist(int deleting)
// optimize_for_size matches retail's stmw/lmw frame.
#pragma push
#pragma optimize_for_size on
void* __dt__reslist_CPackItem(PackItemList* self, int deleting) {
    if (self != 0) {
        __dt___reslist_base_CPackItem(self, 0);
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}
#pragma pop

// _reslist_base<CArcItem*>::~_reslist_base(int deleting)
// optimize_for_size matches retail's stmw/lmw frame (same as CPackItem twin).
#pragma push
#pragma optimize_for_size on
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
#pragma pop

// reslist<CArcItem*>::~reslist(int deleting)
// optimize_for_size matches retail's stmw/lmw frame.
#pragma push
#pragma optimize_for_size on
void* __dt__reslist_CArcItem(ArcItemList* self, int deleting) {
    if (self != 0) {
        __dt___reslist_base_CArcItem(self, 0);
        if (deleting > 0) {
            delete self;
        }
    }
    return self;
}
#pragma pop

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

    CWorkSystemPack* sys = lbl_eu_80665A10;
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
#pragma optimize_for_size off

// Register one static pkh filename: pick the lowest partition index not yet
// used by any pack item, construct the CPackItem (CRI partition id from the
// caller), and push it onto the singleton's pack list (first free slot).
// Retail calls the 2-param CPackItem ctor with a THIRD (ignored) argument
// (the function's own arg2 forwarded), keeping r29 live across the allocate
// calls. Declared under the retail mangled name to reproduce that call.
extern "C" CPackItem* __ct__9CPackItemFPCci(CPackItem* mem, const char* name, int idx, int partition);

#pragma optimize_for_size on
__attribute__((never_inline)) void func_804DDE3C(const char* pName, int pPartitionId) {
    // No local singleton copy: retail reloads lbl_eu_80665A10 (SDA) at each
    // use site, keeping only pName/pPartitionId/i/newItem in callee-saved regs.
    int i = 0;
    PackItemListNode* sentinel = lbl_eu_80665A10->mPackList.mStartNodePtr;
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

    CWorkSystemPack* sys2 = lbl_eu_80665A10;
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
#pragma optimize_for_size off

} // extern "C"

// Inlined alloc+clear used by the ctor: going through the list pointer keeps
// MWCC reloading mList every iteration (retail shape).
#pragma optimize_for_size on
static void InitPackNodeArray(PackItemList* list, u32 bytes, int count) {
    list->mList = (PackItemListNode*)mtl::MemManager::allocate_array(bytes, CWorkThreadSystem::getWorkMem());
    for (int j = 0; j < count; j++) {
        list->mList[j].mNext = 0;
    }
    list->mCapacity = count;
}
#pragma optimize_for_size off

#pragma optimize_for_size on
// Base ctor runs in the init list (retail emits the plain __ct__11CWorkThread
// bl with capacity 0); the class is novtable so the retail vtable is stored
// manually (CWorkSystemCache pattern).
CWorkSystemPack::CWorkSystemPack(const char* pName, CWorkThread* pParent)
    : CWorkThread(pName, pParent, 0) {
    int i; // declared early: birth order keeps the loop index off the zero-reg copy path

    // CWorkSystemPack vtable, then the two reslists. Each list's inlined ctor
    // chain (_reslist_base then reslist) stores its vtable twice: base, then
    // derived (e.g. pack: lbl_eu_8056FF48 then lbl_eu_8056FF30).
    // Same shape as the matched CWorkSystemCache ctor: the sentinel-pointer
    // reads between the two vtable stores block MWCC's DSE of the base-vtable
    // / NULL-mList stores that the derived overwrite makes look dead.
    mPackList.mList = 0;
    *(u32**)this = (u32*)lbl_eu_8056FE58;
    mPackList.m_vtable = (u32)lbl_eu_8056FF48;
    mPackList.mCapacity = 0;
    mPackList.unk1C = false;
    mPackList.mStartNodePtr = &mPackList.mStartNode;
    mPackList.mStartNodePtr->mNext = &mPackList.mStartNode;
    mPackList.mStartNodePtr->mPrev = mPackList.mStartNodePtr->mNext;
    mPackList.m_vtable = (u32)lbl_eu_8056FF30;

    mArcList.m_vtable = (u32)lbl_eu_8056FF24;
    mArcList.mList = 0;
    mArcList.mCapacity = 0;
    mArcList.unk1C = false;
    mArcList.mStartNodePtr = &mArcList.mStartNode;
    mArcList.mStartNodePtr->mNext = &mArcList.mStartNode;
    mArcList.mStartNodePtr->mPrev = mArcList.mStartNodePtr->mNext;
    mArcList.m_vtable = (u32)lbl_eu_8056FF0C;

    lbl_eu_80665A10 = this;
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
    const char* const* p = lbl_eu_80665A18[0];
    if (p != 0) {
        const char* const* it = p - 1;
        while (*++it != 0) {
            func_804DDF00(*it);
        }
    }
    const char* const* q = lbl_eu_80665A14;
    if (q != 0) {
        const char* const* jt = q - 1;
        while (*++jt != 0) {
            func_804DDE3C(*jt, -1);
        }
    }
}
#pragma optimize_for_size off

// extern "C" free-function form: clear the work-system singleton, then tear
// down the two resource lists (mArcList +0x1E4, mPackList +0x1C4 — retail's
// addic. null-checks), the base CWorkThread (unchecked), and the flags-based
// delete; stmw/lmw frame via optimize_for_size.
#pragma push
#pragma optimize_for_size on
extern "C" void __dt__11CWorkThreadFv(void* self, int flags);
extern "C" void* __dt__15CWorkSystemPackFv(CWorkSystemPack* self, int flags) {
    if (self != 0) {
        lbl_eu_80665A10 = 0;
        if (&self->mArcList != 0) {
            __dt___reslist_base_CArcItem(&self->mArcList, 0);
        }
        if (&self->mPackList != 0) {
            __dt___reslist_base_CPackItem(&self->mPackList, 0);
        }
        __dt__11CWorkThreadFv(self, 0);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}
#pragma pop

// Walk the pack list, comparing each item's base name against pName. Returns
// the lookupFile() result (found flag) for the matching item, or 0 if no item
// matches.
// Retail symbol is unmangled - this helper has C linkage in the original.
// Retail's addic/subfe setnz bool tail is the optimize_for_size (-O4,s)
// codegen signature; the default -O4,p emits neg/or/rlwinm instead.
#pragma push
#pragma optimize_for_size on
extern "C" __declspec(noinline) bool func_804DDD54(const char* pName, const char* pPath, char** outPkbPath,
                   u32* outEntryId, u32* outIndex, u32* outFileId) {
    const char* pFile = ml::CPathUtil::getFilePtrFromPath(pPath);
    PackItemListNode* sentinel;
    PackItemListNode* node;
    sentinel = lbl_eu_80665A10->mPackList.mStartNodePtr;
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
#pragma pop

// Resolve a pack file id for pPath: first via the static lookup, then via the
// item search (returns the file id << 11). Returns -1 if neither matched.
// Static lookup: walk the singleton's circular pack-item list (head at
// +0x1E8, nodes link via +0, item pointer at +8) and forward to the first
// item's func_804DEC6C when the list is non-empty (retail unmangled reloc).
extern "C" bool func_804DEC6C(void* item, const char* pPath, void** pOutStartAddr, u32* pOutLength);

#pragma push
#pragma optimize_for_size on
s32 func_804DDCD4(const char* pName, const char* pPath) {
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
#pragma pop

// Static lookup: walk the singleton's circular pack-item list (head at
// +0x1E8, nodes link via +0, item pointer at +8) and forward to the first
// item's func_804DEC6C when the list is non-empty.
// __declspec(noinline) keeps retail's out-of-line bl - without it -inline auto
// folds this small helper into func_804DDCD4 below.
__declspec(noinline) bool CWorkSystemPack::findPackResource(const char* pName, void* pOut, u32* pFileId) {
    CWorkSystemPack* sys = lbl_eu_80665A10;
    u8* head = *(u8**)((u8*)sys + 0x1E8);
    u8* first = *(u8**)head;
    if (first != head) {
        return func_804DEC6C(*(void**)(first + 8), pName, (void**)pOut, pFileId);
    }
    return false;
}

// Login gate: walk the circular pack-item list (head at +0x1E8, nodes link
// via +0, data pointer at +8) and require every item's field_0x2C == 2.
bool CWorkSystemPack::areArcsReady() {
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

    mtl::MemManager::erase(lbl_eu_80663BC8[0]);
    return ((CWorkThread*)this)->CWorkThread::wkStandbyLogout();
}
#pragma optimize_for_size off

// Store the PKH filenames array pointer for later retrieval by the work system.
void SavePkhFilenamesArrayPtr__15CWorkSystemPackFPCPCc(const char* const* pArray) {
    lbl_eu_80665A14 = pArray;
}

// Store the static arc filename string pointer for later retrieval by the work system.
void SaveStaticArcFilenameStringPtr__15CWorkSystemPackFPCPCc(const char* const* pFilenameStr) {
    lbl_eu_80665A18[0] = pFilenameStr;
}

// Search the pack list for an item whose base name matches pName.
#pragma push
#pragma optimize_for_size on
int func_804DE010(const char* pName) {
    CWorkSystemPack* sys = lbl_eu_80665A10;
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
#pragma pop

// Remove the pack item whose base name matches pName from the pack list,
// destroying the item (vtable+8 call) and unlinking its node.
// optimize_for_size reproduces retail's stmw/lmw frame and the
// addic.-1/subfe setnz strcmp tail.
#pragma push
#pragma optimize_for_size on
void func_eu_804E2340(const char* pName) {
    // Don't cache the singleton in a local: retail only keeps 3 values live
    // (pName, sentinel, node) and saves r29-r31.
    PackItemListNode* sentinel = lbl_eu_80665A10->mPackList.mStartNodePtr;
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
#pragma pop

// Returns false if any pack item other than pExcept is currently mid-load
// (LOAD_STATE_LOADING_AHX_ADX_FILE == 2).
bool CWorkSystemPack::isPackLoadIdle(int pExcept) {
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
bool CWorkSystemPack::arePacksLoaded() {
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
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056FE58-0x8056FF58 (256B): CWorkSystemPack vtable (160B), its
// RTTI chain (20B) and the reslist<CArcItem*>/reslist<CPackItem*> vtable
// families (12B each + 16B tail).
namespace PackBlob {
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
extern "C" void wkUpdate__11CWorkThreadFv();
extern "C" void wkRender__11CWorkThreadFv();
extern "C" void wkRenderAfter__11CWorkThreadFv();
extern "C" void wkStandbyExceptionRetry__11CWorkThreadFUl();
extern "C" u32 __RTTI__10IWorkEvent;
extern "C" u32 __RTTI__11CWorkThread;
}
extern "C" void wkStandbyLogin__15CWorkSystemPackFv(); // defined below (member)
extern "C" void wkUpdate__15CWorkSystemPackFv();       // defined below (member)
extern "C" void wkStandbyLogout__15CWorkSystemPackFv(); // defined below (member)
extern "C" u32 lbl_eu_80663BD0[2];
extern "C" u32 lbl_eu_80663BD8[2];
extern "C" u32 lbl_eu_80663BE0[2];
extern "C" u32 lbl_eu_80663BE8[2];
extern "C" u32 lbl_eu_80663BF0[2];
extern "C" u32 lbl_eu_8056FEF8[5];
extern "C" u32 lbl_eu_8056FF18[3];
extern "C" u32 lbl_eu_8056FF3C[3];

extern "C" u32 lbl_eu_8056FE58[40] = {
    (u32)&lbl_eu_80663BD0, 0x00000000, (u32)&__dt__15CWorkSystemPackFv,
    (u32)&PackBlob::WorkEvent1__10IWorkEventFPvPCc,
    (u32)&PackBlob::OnFileEvent__10IWorkEventFP10CEventFile,
    (u32)&PackBlob::WorkEvent3__10IWorkEventFPv,
    (u32)&PackBlob::WorkEvent4__10IWorkEventFv,
    (u32)&PackBlob::OnPauseTrigger__10IWorkEventFb,
    (u32)&PackBlob::WorkEvent6__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent7__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent8__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent9__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent10__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent11__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent12__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent13__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent14__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent15__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent16__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent17__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent18__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent19__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent20__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent21__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent22__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent23__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent24__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent25__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent26__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent27__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent28__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent29__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent30__10IWorkEventFv,
    (u32)&PackBlob::WorkEvent31__10IWorkEventFv,
    (u32)&wkUpdate__15CWorkSystemPackFv,
    (u32)&PackBlob::wkRender__11CWorkThreadFv,
    (u32)&PackBlob::wkRenderAfter__11CWorkThreadFv,
    (u32)&wkStandbyLogin__15CWorkSystemPackFv,
    (u32)&wkStandbyLogout__15CWorkSystemPackFv,
    (u32)&PackBlob::wkStandbyExceptionRetry__11CWorkThreadFUl,
};
extern "C" u32 lbl_eu_8056FEF8[5] = {
    (u32)&PackBlob::__RTTI__10IWorkEvent, 0x00000000,
    (u32)&PackBlob::__RTTI__11CWorkThread, 0x00000000, 0x00000000,
};
extern "C" u32 lbl_eu_8056FF0C[3] = {
    (u32)&lbl_eu_80663BD8, 0x00000000, (u32)&__dt__reslist_CArcItem,
};
extern "C" u32 lbl_eu_8056FF18[3] = {
    (u32)&lbl_eu_80663BE0, 0x00000000, 0x00000000,
};
extern "C" u32 lbl_eu_8056FF24[3] = {
    (u32)&lbl_eu_80663BE0, 0x00000000, (u32)&__dt___reslist_base_CArcItem,
};
extern "C" u32 lbl_eu_8056FF30[3] = {
    (u32)&lbl_eu_80663BE8, 0x00000000, (u32)&__dt__reslist_CPackItem,
};
extern "C" u32 lbl_eu_8056FF3C[3] = {
    (u32)&lbl_eu_80663BF0, 0x00000000, 0x00000000,
};
extern "C" u32 lbl_eu_8056FF48[4] = {
    (u32)&lbl_eu_80663BF0, 0x00000000, (u32)&__dt___reslist_base_CPackItem, 0x00000000,
};

// [.rodata] 0x80524688-0x805246FC (116B): RTTI name pool.
extern "C" __declspec(align(4)) const char lbl_eu_80524688[16] = {
    0x43,0x57,0x6F,0x72,0x6B,0x53,0x79,0x73,0x74,0x65,0x6D,0x50,0x61,0x63,0x6B,0x00,
};
extern "C" __declspec(align(4)) const char lbl_eu_80524698[20] = {
    0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x43,0x41,0x72,0x63,0x49,0x74,0x65,0x6D,
    0x20,0x2A,0x3E,0x00,
};
extern "C" __declspec(align(4)) const char lbl_eu_805246AC[26] = {
    0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x43,0x41,
    0x72,0x63,0x49,0x74,0x65,0x6D,0x20,0x2A,0x3E,0x00,
};
extern "C" __declspec(align(4)) const char lbl_eu_805246C8[21] = {
    0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x3C,0x43,0x50,0x61,0x63,0x6B,0x49,0x74,0x65,
    0x6D,0x20,0x2A,0x3E,0x00,
};
extern "C" __declspec(align(4)) const char lbl_eu_805246E0[28] = {
    0x5F,0x72,0x65,0x73,0x6C,0x69,0x73,0x74,0x5F,0x62,0x61,0x73,0x65,0x3C,0x43,0x50,
    0x61,0x63,0x6B,0x49,0x74,0x65,0x6D,0x20,0x2A,0x3E,0x00,0x00,
};

// [.sdata] 0x80663BC8-0x80663BF8 (48B): region handle + RTTI locators.
extern "C" u32 lbl_eu_80663BC8[2] = { 0xFFFFFFFF, 0x00000000 };
extern "C" u32 lbl_eu_80663BD0[2] = { (u32)&lbl_eu_80524688, (u32)&lbl_eu_8056FEF8 };
extern "C" u32 lbl_eu_80663BD8[2] = { (u32)&lbl_eu_80524698, (u32)&lbl_eu_8056FF18 };
extern "C" u32 lbl_eu_80663BE0[2] = { (u32)&lbl_eu_805246AC, 0x00000000 };
extern "C" u32 lbl_eu_80663BE8[2] = { (u32)&lbl_eu_805246C8, (u32)&lbl_eu_8056FF3C };
extern "C" u32 lbl_eu_80663BF0[2] = { (u32)&lbl_eu_805246E0, 0x00000000 };

// [.sbss] 0x80665A10-0x80665A20 (16B).
CWorkSystemPack* lbl_eu_80665A10;
const char* const* lbl_eu_80665A14;
const char* const* lbl_eu_80665A18[2];
