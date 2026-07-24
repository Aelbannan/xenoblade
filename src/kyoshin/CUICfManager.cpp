#include "kyoshin/CUICfManager.hpp"

#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

// High-level Init (no asm void). Best ~94.8% HIGH_MATCH.
// Tail: scalars + u8 mid[8] + lone u32@+0x0E + pair-unroll chunks; zeros via u16* overlay.

struct CUICfInitProcess {
    u8 unk00[0x10];
    void* vtable;
    u8 unk14[0x28];
    u32 callbacks[6];
};

extern "C" {
CProcess* lbl_eu_80664054;
const char* lbl_eu_806621A8;
char lbl_eu_8052E404[];
char lbl_eu_8052E3BC[];
u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
void func_8015704C(CUICfInitBlock*, const CUICfInitBlock*);
void func_8009D0B4();
void func_8009D514(cf::IFlagEvent*);
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);
}

static IWorkEvent* cfWorkEvent(CUICfManager* self) {
    IWorkEvent* workEvent = reinterpret_cast<IWorkEvent*>(self);
    if (self != NULL) {
        workEvent = reinterpret_cast<IWorkEvent*>(reinterpret_cast<u8*>(self) + 0x54);
    }
    return workEvent;
}

void CUICfManager::Init() {
    CUICfInitProcess* process;
    u8* ptmfBase;
    char* vtFinal;
    u32 ptmfWord1;
    u32 ptmfWord0;
    u32 ptmfWord2;
    // Retail stack: state@+0x8, block0@+0xC, blocks[3]@+0x40, tail@+0xDC.
    CUICfInitTemplates tmpl;
    CUICfInitBlock* clearEnd;
    CUICfInitBlock* initBlock1Ptr;
    CUICfInitBlock* initBlock2Ptr;
    CUICfInitBlock* initBlock3Ptr;
    CUICfInitBlock* clearPtr;
    u32 blockStride;
    u32 clearCount;
    u8 i;

    mFileHandle = CDeviceFile::readFile(unk118, lbl_eu_806621A8, cfWorkEvent(this), 0, 0);
    CDeviceFile::func_8044F154(mFileHandle, 3);

    process = static_cast<CUICfInitProcess*>(
        mtl::MemManager::allocate(0x54, CWorkThreadSystem::getWorkMem()));
    if (process != NULL) {
        __ct__8CProcessFv(reinterpret_cast<CProcess*>(process));
        ptmfBase = (u8*)__ptmf_null;
        process->vtable = lbl_eu_8052E404;
        ptmfWord1 = *(u32*)(ptmfBase + 4);
        vtFinal = lbl_eu_8052E3BC;
        ptmfWord0 = *(u32*)(ptmfBase + 0);
        process->callbacks[0] = ptmfWord0;
        process->callbacks[1] = ptmfWord1;
        ptmfWord2 = *(u32*)(ptmfBase + 8);
        process->callbacks[2] = ptmfWord2;
        ptmfWord1 = *(u32*)(ptmfBase + 4);
        ptmfWord0 = *(u32*)(ptmfBase + 0);
        process->callbacks[3] = ptmfWord0;
        process->callbacks[4] = ptmfWord1;
        ptmfWord2 = *(u32*)(ptmfBase + 8);
        process->callbacks[5] = ptmfWord2;
        process->vtable = vtFinal;
    }
    unk144 = reinterpret_cast<CUICfUnk144*>(process);
    reinterpret_cast<CProcess*>(process)->Regist(lbl_eu_80664054, false);

    // Retail: r27=&tail end, r26/r25/r24 = blocks, r22 = 0x34 stride.
    clearEnd = reinterpret_cast<CUICfInitBlock*>(&tmpl.tail);
    initBlock1Ptr = &tmpl.blocks[0];
    initBlock2Ptr = &tmpl.blocks[1];
    initBlock3Ptr = &tmpl.blocks[2];
    blockStride = sizeof(CUICfInitBlock);
    for (i = 0; i < 8; ++i) {
        CUICfInitSlot& slot = mInitSlots[i];
        u16* tailHalves;
        u8* tailBytes;

        tmpl.block0.unk04 = 0;
        tmpl.block0.unk00 = 0;
        // Fresh &blocks[0] + (end+stride-1-start)/stride (retail clear shape).
        clearPtr = &tmpl.blocks[0];
        if (clearPtr < clearEnd) {
            clearCount =
                (reinterpret_cast<u8*>(clearEnd) + (blockStride - 1) - reinterpret_cast<u8*>(clearPtr)) /
                blockStride;
            do {
                clearPtr->unk04 = 0;
                clearPtr->unk00 = 0;
                clearPtr = reinterpret_cast<CUICfInitBlock*>(reinterpret_cast<u8*>(clearPtr) +
                                                             blockStride);
            } while (--clearCount != 0);
        }

        tmpl.state.state = 0;
        tmpl.tail.unk00 = 0;
        // Overlay: retail sth zeros at +4..+0xE (copy still words at +4/+8).
        tailHalves = reinterpret_cast<u16*>(reinterpret_cast<u8*>(&tmpl.tail) + 4);
        tailHalves[0] = 0;
        tailHalves[1] = 0;
        tailHalves[2] = 0;
        tailHalves[3] = 0;
        tailHalves[4] = 0;
        tailHalves[5] = 0;
        tailBytes = reinterpret_cast<u8*>(&tmpl.tail);
        tailBytes[0x10] = 0;
        tailBytes[0x12] = 0;
        tailBytes[0x52] = 0;
        tmpl.state.mode = 7;
        slot.unk04 = tmpl.state.mode;
        slot.unk05 = tmpl.state.state;

        func_8015704C(&slot.unk08, &tmpl.block0);
        func_8015704C(&slot.unk3C, initBlock1Ptr);
        func_8015704C(&slot.unk70, initBlock2Ptr);
        func_8015704C(&slot.unkA4, initBlock3Ptr);
        slot.unkD8.unk00 = tmpl.tail.unk00;
        slot.unkD8.mid = tmpl.tail.mid;
        slot.unkD8.unk0C = tmpl.tail.unk0C;
        slot.unkD8.unk0E = tmpl.tail.unk0E;
        slot.unkD8.rest0 = tmpl.tail.rest0;
        *reinterpret_cast<CUICfInitTailChunk40View*>(&slot.unkD8.rest1) =
            *reinterpret_cast<CUICfInitTailChunk40View*>(&tmpl.tail.rest1);
    }
}

void CUICfManager::Term() {
    func_801390E0(&mFileHandle);

    mtl::MemManager::deallocate(mPackedFont60.Destroy());
    mtl::MemManager::deallocate(mPackedFont9C.Destroy());
    mtl::MemManager::deallocate(mPackedFontD8.Destroy());

    func_80139124(mArcResourceAccessor);
    mArcResourceAccessor = NULL;
    unk118 = -1;

    cf::IFlagEvent* flagEvent = this;
    func_8009D0B4();
    func_8009D514(flagEvent);

    unk144->unk39 = 1;
    lbl_eu_80664054 = NULL;
}

// ---------------------------------------------------------------------------
// func_80133324__12CUICfManagerFv
//
// Retail mangles this as a no-arg CUICfManager member (`Fv`) but the body
// reads r4/r5/r6 as real event-dispatch arguments and never touches `this`
// (all state comes from the `lbl_eu_80664054` singleton). Declared as a free
// `extern "C"` function so the ABI is r3=<unused self>, r4=id, r5=a1, r6=a2.
//
// Event-queue insert is the same shape as reslist::push_back -> setItem
// (try/catch forces -0x80 / mr r31,r1 / stw r1 epilogue). Leaf body order
// matches retail text: 221 -> 312c -> 7fc -> 22 -> 609.
#include "monolib/util/reslist.hpp"

extern "C" {
int lbl_eu_80664050;
int func_80138138(int);
u32 func_8013B87C(u8);
int func_8014A1D4(void*, u32, u8, int);
void func_8013DA60(int, int, int);
void* func_8009EC9C(u16);
void func_800A21F8(void*, u16, int, int);
u32 func_801361E8(void*, char*, u32);
u16 func_8013606C(char*, char*, u16);
void* lbl_eu_80573D18[];
u8 lbl_eu_805000A8[];
u16 lbl_eu_804FFFDC[];
}

// C++ mangling -> retail `func_8013B428__FUl`.
void func_8013B428(u32);

void func_80133324__12CUICfManagerFv(CUICfManager* self, int id, int a1, int a2) {
    // Decl order: savedRet@0x8, gap, setItem stw-r1 home@0x24, idTable@0x28 (retail frame).
    volatile int savedRet;
    CUICfIdTable idTable;
    u8 codePersist;

    if (a1 == a2) {
        goto end;
    }

    if (id >= 0x798) {
        goto ge798;
    }

    if (id >= 0x221) {
        goto ge221;
    }

    if (id >= 0x28) {
        goto end;
    }
    if (id >= 0x22) {
        goto range_22_27;
    }
    goto end;

ge221:
    if (id == 0x608) {
        goto end;
    }
    if (id >= 0x608) {
        goto range_609_797;
    }
    goto range_221_607;

ge798:
    if (id >= 0x312c) {
        goto ge312c;
    }
    if (id >= 0x805) {
        goto end;
    }
    if (id >= 0x7fc) {
        goto range_7fc_804;
    }
    goto end;

ge312c:
    if (id >= 0x31f4) {
        goto end;
    }
    goto range_312c_31f3;

range_221_607: {
    u32 check = (u32)(a1 - 0xfe);
    int off = id - 0x220;
    lbl_eu_80664050 = off;
    if (check > 1) {
        goto end;
    }

    int idx = func_80138138(off);
    void* tableVal = lbl_eu_80573D18[idx];
    u8 res = (u8)func_801361E8(tableVal, (char*)lbl_eu_805000A8 + 0x43, lbl_eu_80664050);
    if (res == 2) {
        goto end;
    }

    func_8013DA60(lbl_eu_80664050, 1, 0);

    idTable = *(const CUICfIdTable*)lbl_eu_804FFFDC;
    {
        int target = lbl_eu_80664050;
        u8 i = 0;
        goto id_check;
    id_body:
        if ((u16)idTable.ids[i] == target) {
            func_8013B428(0xb9);
            goto end;
        }
        i++;
    id_check:
        if (idTable.ids[i] != 0) {
            goto id_body;
        }
    }
    goto end;
}

range_312c_31f3: {
    if (a1 == 0) {
        goto end;
    }
    if (a2 != 0) {
        goto end;
    }

    // Retail: addi r3,id,-0x312c / li r0,0xc8 / clrlwi. / clrlwi into codePersist (r30).
    // Soft-cap: MWCC colors diff/code as r0/r3 (and codePersist r27) vs retail r3/r0 (r30).
    {
        u32 diff = id - 0x312c;
        u32 code = 0xc8;
        if ((u8)diff != 0) {
            code = (u8)diff;
        }
        codePersist = (u8)code;
    }

    func_8013B87C(codePersist);

    CUICfManager* inst = (CUICfManager*)lbl_eu_80664054;
    if (inst != NULL) {
        int tempRet = (int)func_8014A1D4(inst->unk144, inst->unk11C, codePersist, 1);
        savedRet = tempRet;
        if (tempRet != 0) {
            inst = (CUICfManager*)lbl_eu_80664054;

            // Decl capacity before startNode so Chaitin colors capacity=r7, startNode=r8;
            // assign startNode then capacity to keep retail load order (296 then 316).
            int i = 0;
            int byteOff = 0;
            int capacity;
            CUICfListNode* startNode;
            startNode = (CUICfListNode*)inst->unk128;
            capacity = inst->unk13C;
            goto slot_check;
        slot_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto slot_found;
            }
            byteOff += 0xc;
            i++;
        slot_check:
            if (i < capacity) {
                goto slot_body;
            }
        slot_found:
            {
                // Expand setItem so temp lands in r4 before savedRet reload (addic. r3,r4,8).
                CUICfListNode* temp = (CUICfListNode*)((u8*)inst->unk138 + i * 0xc);
                u32* ptr = &temp->item;
                if (ptr != 0) {
                    try {
                        *ptr = (u32)savedRet;
                    } catch (...) {
                        throw;
                    }
                }
                temp->next = startNode;
                temp->prev = startNode->prev;
                startNode->prev->next = temp;
                startNode->prev = temp;
            }
        }
    }

    {
        u16 ret2 = func_8013606C((char*)lbl_eu_805000A8 + 0x4d, (char*)lbl_eu_805000A8 + 0x5b,
                                   codePersist);
        if (ret2 != 0) {
            int i = 1;
            goto party_check;
        party_body:
            {
                void* r = func_8009EC9C((u16)i);
                func_800A21F8(r, ret2, 0, 0);
                i++;
            }
        party_check:
            if (i <= 8) {
                goto party_body;
            }
        }
    }
    goto end;
}

range_7fc_804: {
    int rel = id - 0x7fc;
    if (rel == 0) {
        goto case_7fc;
    }
    if (rel == 1) {
        goto case_7fd;
    }
    goto end;
}

case_7fc:
    if (a1 == 0x64) {
        func_8013B428(0xc8);
        goto end;
    }
    if (a1 >= 0x32) {
        func_8013B428(0xc7);
        goto end;
    }
    if (a1 >= 1) {
        func_8013B428(0xc6);
    }
    goto end;

case_7fd:
    if (a1 == 0x96) {
        func_8013B428(0xc5);
        goto end;
    }
    if (a1 >= 0x64) {
        func_8013B428(0xc4);
        goto end;
    }
    if (a1 >= 0x32) {
        func_8013B428(0xc3);
    }
    goto end;

range_22_27:
    if (a1 < 0xfa0) {
        goto end;
    }
    func_8013B428((u8)(id + 0x81));
    if (a1 < 0x1f40) {
        goto end;
    }
    func_8013B428(0xa8);
    func_8013B428(0xa9);
    goto end;

range_609_797:
    if (a2 == 0) {
        if (a1 > 0) {
            func_8013B428(0x9f);
            func_8013B428(0xa0);
            func_8013B428(0xa1);
            func_8013B428(0xa2);
        }
    }
    if (a1 < 5) {
        goto end;
    }
    func_8013B428(0x9e);
    goto end;

end:
    return;
}

// ---------------------------------------------------------------------------
// CUICfManager::Move
//
// Early r4 is NOT a fake-Fv arg -- retail does `lhz r4, 0xc90(r3)` (mFlags).
// Bitflag-driven create/teardown against lbl_eu_80664054, then optional
// enum-list proximity spawn, then mark/clear walks of the menu queue.
// ---------------------------------------------------------------------------

#include "monolib/device/CDeviceVI.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

extern "C" {
u32 lbl_eu_80663E24;
u32 lbl_eu_80663E28;
f32 lbl_eu_806672CC;

void func_801338C8(CUICfManager*);
void func_80133770();
void* __ct__CMenuKeyAssign(void*, u32);
void* func_801109D8(void*, u32, void*); // create menu; r5=0 or enum object*
void* func_8011E4C4(void*, u32);
void* __ct__CMenuBattleMode(void*, u32);
void* __ct__CMenuLvUp(void*, u32);

// Opaque 8-byte holder around a CfObjEnumList* (func_80043D90 / __dt__80043E88).
struct CUICfEnumListHolder {
    void* list; // 0x0
    u32 handle; // 0x4
};

void func_80043D90(CUICfEnumListHolder*);
void* func_80043F18(CUICfEnumListHolder*); // returns holder->list
void __dt__80043E88(CUICfEnumListHolder*, s16);
void func_800F4A98(void* list, int type, int);
void* __ct__800FB044(void* list, f32, void* obj, int);
void* func_80496264(void* obj, int index);
void* func_800F6EC0(void* list, int index); // &slot -> has +0x4 object ptr
void* func_800F6E98(void* list, int index); // *slot -> object*
int func_800B8920(void*);
int func_8013A4B4(void* a, void* b, void* c);
void func_8012FFB4(void*); // &mInitSlots[0].unk04
}

typedef void* (*CUICfVPtrFn)(void*);

void CUICfManager::Move() {
    void* savedRet18;
    void* savedRet14;
    void* savedRet10;
    void* savedRet0C;
    void* savedRet08;
    volatile void** home18;
    volatile void** home14;
    volatile void** home10;
    volatile void** home0C;
    volatile void** home08;
    CUICfEnumListHolder holder;
    f32 posA[3];
    f32 posB[3];
    f32 posC[3];
    _reslist_node<CUICfMenuItem*>* pending[18];
    CUICfManager* inst;
    int i;
    int byteOff;
    int capacity;
    _reslist_node<void*>* startNode;
    _reslist_node<void*>* temp;
    void* party;
    void* list;
    void* partyHandle;
    void* qpos;
    void* pose;
    void* slot;
    void* createdArg;
    _reslist_node<CUICfMenuItem*>* head;
    _reslist_node<CUICfMenuItem*>* it;
    _reslist_node<CUICfMenuItem*>* walk;
    _reslist_node<CUICfMenuItem*>* node;
    _reslist_node<CUICfMenuItem*>* prev;
    _reslist_node<CUICfMenuItem*>* next;
    CUICfMenuItem* item;
    int pendingCount;
    int needWait;
    int limit;
    int canUnroll;
    int nextCount;
    int idx;

    home18 = (volatile void**)&savedRet18;
    home14 = (volatile void**)&savedRet14;
    home10 = (volatile void**)&savedRet10;
    home0C = (volatile void**)&savedRet0C;
    home08 = (volatile void**)&savedRet08;
    u16 flags = mFlags;

    if ((flags & 0x2) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xfffd);
        }
        func_801338C8(this);
        goto after_flags;
    }
    if ((flags & 0x1) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & ~0x1); // rlwinm ...,16,30
        }
        func_80133770();
        goto after_flags;
    }
    if ((flags & 0x4) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xfffb);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x4);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xfffb);
        inst = (CUICfManager*)lbl_eu_80664054;
        inst->mFlags = (u16)(inst->mFlags | 0x8);
        inst = (CUICfManager*)lbl_eu_80664054;
        inst->mFlags = (u16)(inst->mFlags | 0x10);
        inst = (CUICfManager*)lbl_eu_80664054;
        inst->mFlags = (u16)(inst->mFlags | 0x20);
        inst = (CUICfManager*)lbl_eu_80664054;
        inst->mFlags = (u16)(inst->mFlags | 0x40);
        inst = (CUICfManager*)lbl_eu_80664054;
        inst->mFlags = (u16)(inst->mFlags | 0x80);
        goto after_flags;
    }
    if ((flags & 0x8) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xfff7);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x8);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xfff7);
        inst = (CUICfManager*)lbl_eu_80664054;
        *home18 = __ct__CMenuKeyAssign(inst->unk144, inst->unk11C);
        if (*home18 != NULL) {
            inst = (CUICfManager*)lbl_eu_80664054;
            i = 0;
            byteOff = 0;
            startNode = (_reslist_node<void*>*)inst->unk128;
            capacity = inst->unk13C;
            goto push8_check;
        push8_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto push8_found;
            }
            byteOff += 0xc;
            i++;
        push8_check:
            if (i < capacity) {
                goto push8_body;
            }
        push8_found:
            temp = (_reslist_node<void*>*)((u8*)inst->unk138 + i * 0xc);
            temp->setItem((void*)*home18);
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }
        goto after_flags;
    }
    if ((flags & 0x10) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xffef);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x10);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xffef);
        inst = (CUICfManager*)lbl_eu_80664054;
        *home14 = func_801109D8(inst->unk144, inst->unk11C, NULL);
        if (*home14 != NULL) {
            inst = (CUICfManager*)lbl_eu_80664054;
            i = 0;
            byteOff = 0;
            startNode = (_reslist_node<void*>*)inst->unk128;
            capacity = inst->unk13C;
            goto push10_check;
        push10_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto push10_found;
            }
            byteOff += 0xc;
            i++;
        push10_check:
            if (i < capacity) {
                goto push10_body;
            }
        push10_found:
            temp = (_reslist_node<void*>*)((u8*)inst->unk138 + i * 0xc);
            temp->setItem((void*)*home14);
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }
        goto after_flags;
    }
    if ((flags & 0x20) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xffdf);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x20);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xffdf);
        inst = (CUICfManager*)lbl_eu_80664054;
        *home10 = func_8011E4C4(inst->unk144, inst->unk11C);
        if (*home10 != NULL) {
            inst = (CUICfManager*)lbl_eu_80664054;
            i = 0;
            byteOff = 0;
            startNode = (_reslist_node<void*>*)inst->unk128;
            capacity = inst->unk13C;
            goto push20_check;
        push20_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto push20_found;
            }
            byteOff += 0xc;
            i++;
        push20_check:
            if (i < capacity) {
                goto push20_body;
            }
        push20_found:
            temp = (_reslist_node<void*>*)((u8*)inst->unk138 + i * 0xc);
            temp->setItem((void*)*home10);
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }
        goto after_flags;
    }
    if ((flags & 0x40) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xffbf);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x40);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xffbf);
        inst = (CUICfManager*)lbl_eu_80664054;
        *home0C = __ct__CMenuBattleMode(inst->unk144, inst->unk11C);
        if (*home0C != NULL) {
            inst = (CUICfManager*)lbl_eu_80664054;
            i = 0;
            byteOff = 0;
            startNode = (_reslist_node<void*>*)inst->unk128;
            capacity = inst->unk13C;
            goto push40_check;
        push40_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto push40_found;
            }
            byteOff += 0xc;
            i++;
        push40_check:
            if (i < capacity) {
                goto push40_body;
            }
        push40_found:
            temp = (_reslist_node<void*>*)((u8*)inst->unk138 + i * 0xc);
            temp->setItem((void*)*home0C);
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }
        goto after_flags;
    }
    if ((flags & 0x80) != 0) {
        {
            volatile u16* fp = &mFlags;
            *fp = (u16)(*fp & 0xff7f);
        }
        inst = (CUICfManager*)lbl_eu_80664054;
        if (inst == NULL) {
            goto after_flags;
        }
        if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
            goto after_flags;
        }
        if (inst->mArcResourceAccessor == NULL) {
            inst->mFlags = (u16)(inst->mFlags | 0x80);
            goto after_flags;
        }
        inst->mFlags = (u16)(inst->mFlags & 0xff7f);
        inst = (CUICfManager*)lbl_eu_80664054;
        *home08 = __ct__CMenuLvUp(inst->unk144, inst->unk11C);
        if (*home08 != NULL) {
            inst = (CUICfManager*)lbl_eu_80664054;
            i = 0;
            byteOff = 0;
            startNode = (_reslist_node<void*>*)inst->unk128;
            capacity = inst->unk13C;
            goto push80_check;
        push80_body:
            if (*(u32*)((u8*)inst->unk138 + byteOff) == 0) {
                goto push80_found;
            }
            byteOff += 0xc;
            i++;
        push80_check:
            if (i < capacity) {
                goto push80_body;
            }
        push80_found:
            temp = (_reslist_node<void*>*)((u8*)inst->unk138 + i * 0xc);
            temp->setItem((void*)*home08);
            temp->mNext = startNode;
            temp->mPrev = startNode->mPrev;
            startNode->mPrev->mNext = temp;
            startNode->mPrev = temp;
        }
    }

after_flags:
    // Retail: dual SDA load + rlwinm/rlwimi on bits 6|21, then bit 13 alone.
    {
        u32 g0 = lbl_eu_80663E24;
        u32 g1 = *(volatile u32*)&lbl_eu_80663E24;
        u32 bits = __rlwinm(g0, 0, 6, 6);
        bits = __rlwimi(bits, g0, 0, 21, 21);
        if (bits != 0) {
            goto after_enum;
        }
        if (__rlwinm(g1, 0, 13, 13) != 0) {
            goto after_enum;
        }
    }
    party = cf::CfGameManager::func_80082D54(0);
    if (party == NULL) {
        goto after_enum;
    }

    func_80043D90(&holder);
    list = func_80043F18(&holder);
    func_800F4A98(list, 0x130, 0);
    party = cf::CfGameManager::func_80082D54(0);
    {
        void** vt = *reinterpret_cast<void***>(party);
        partyHandle = reinterpret_cast<CUICfVPtrFn>(vt[0xAC / 4])(party);
    }
    list = func_80043F18(&holder);
    __ct__800FB044(list, lbl_eu_806672CC, partyHandle, 0);

    pose = func_80496264((void*)unk11C, -1);
    posA[0] = *reinterpret_cast<f32*>((u8*)pose + 0x10c);
    posA[1] = *reinterpret_cast<f32*>((u8*)pose + 0x110);
    posA[2] = *reinterpret_cast<f32*>((u8*)pose + 0x114);
    posB[0] = *reinterpret_cast<f32*>((u8*)pose + 0x138);
    posB[1] = *reinterpret_cast<f32*>((u8*)pose + 0x13c);
    posB[2] = *reinterpret_cast<f32*>((u8*)pose + 0x140);

    i = 0;
    goto enum_check;
enum_body:
    list = func_80043F18(&holder);
    slot = func_800F6EC0(list, i);
    partyHandle = *(void**)((u8*)slot + 4);
    if (partyHandle == NULL) {
        goto enum_next;
    }
    {
        void** vt = *reinterpret_cast<void***>(partyHandle);
        if (reinterpret_cast<int (*)(void*)>(vt[0x160 / 4])(partyHandle) == 0) {
            goto enum_next;
        }
    }
    if (func_800B8920(partyHandle) == 0) {
        goto enum_next;
    }
    list = func_80043F18(&holder);
    slot = func_800F6EC0(list, i);
    partyHandle = *(void**)((u8*)slot + 4);
    {
        void** vt = *reinterpret_cast<void***>(partyHandle);
        qpos = reinterpret_cast<CUICfVPtrFn>(vt[0xAC / 4])(partyHandle);
    }
    posC[0] = *reinterpret_cast<f32*>((u8*)qpos + 0);
    posC[1] = *reinterpret_cast<f32*>((u8*)qpos + 4);
    posC[2] = *reinterpret_cast<f32*>((u8*)qpos + 8);
    if (func_8013A4B4(posA, posB, posC) == 0) {
        goto enum_next;
    }
    list = func_80043F18(&holder);
    createdArg = func_800F6E98(list, i);
    func_801109D8(unk144, unk11C, createdArg);
enum_next:
    i++;
enum_check:
    list = func_80043F18(&holder);
    if ((u32)i < *(u32*)((u8*)list + 0x620)) {
        goto enum_body;
    }
    __dt__80043E88(&holder, -1);

after_enum:
    // Mark-from-head (retail reloads head only inside the set-loop).
    head = (_reslist_node<CUICfMenuItem*>*)unk128;
    walk = head->mNext;
    it = walk;
    goto mark_scan_check;
mark_scan_body:
    if (it->mItem->unk55 != 0) {
        goto mark_set_check;
    }
    if (mInitSlots[0].unk00[1] == 0) {
        goto mark_scan_next;
    }
    goto mark_set_check;
mark_set_body:
    walk->mItem->unk55 = 1;
    walk = walk->mNext;
mark_set_check:
    head = (_reslist_node<CUICfMenuItem*>*)unk128;
    if (walk != head) {
        goto mark_set_body;
    }
    goto mark_done;
mark_scan_next:
    it = it->mNext;
mark_scan_check:
    if (it != head) {
        goto mark_scan_body;
    }
mark_done:

    // Collect: snapshot idx, bump count, indexed store (retail stwx schedule).
    head = (_reslist_node<CUICfMenuItem*>*)unk128;
    node = head->mNext;
    pendingCount = 0;
    needWait = 1;
    goto collect_check;
collect_body:
    item = node->mItem;
    if (item->unk54 == 0 && mInitSlots[0].unk00[0] == 0) {
        goto collect_next;
    }
    if (needWait != 0) {
        CDeviceVI::waitForDrawDone();
        needWait = 0;
    }
    item = node->mItem;
    idx = pendingCount;
    item->unk39 = 1;
    pendingCount = idx + 1;
    *(_reslist_node<CUICfMenuItem*>**)((u8*)pending + (idx << 2)) = node;
collect_next:
    node = node->mNext;
collect_check:
    head = (_reslist_node<CUICfMenuItem*>*)unk128;
    if (node != head) {
        goto collect_body;
    }

    i = 0;
    if (pendingCount <= 0) {
        goto unlink_done;
    }
    if (pendingCount <= 8) {
        goto unlink_tail;
    }
    limit = pendingCount - 8;
    canUnroll = 0;
    if (pendingCount < 0) {
        goto unlink_unroll_test;
    }
    if (pendingCount > (int)0x7FFFFFFE) {
        goto unlink_unroll_test;
    }
    canUnroll = 1;
unlink_unroll_test:
    if (canUnroll == 0) {
        goto unlink_tail;
    }
    goto unlink_unroll_check;
unlink_unroll_body:
    for (byteOff = 0; byteOff < 8; byteOff++) {
        temp = (_reslist_node<void*>*)pending[i + byteOff];
        prev = (_reslist_node<CUICfMenuItem*>*)((_reslist_node<CUICfMenuItem*>*)temp)->mPrev;
        next = (_reslist_node<CUICfMenuItem*>*)((_reslist_node<CUICfMenuItem*>*)temp)->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        ((_reslist_node<CUICfMenuItem*>*)temp)->mNext = NULL;
    }
    i += 8;
unlink_unroll_check:
    if (i < limit) {
        goto unlink_unroll_body;
    }
unlink_tail:
    goto unlink_tail_check;
unlink_tail_body:
    node = pending[i];
    prev = node->mPrev;
    next = node->mNext;
    prev->mNext = next;
    next->mPrev = prev;
    node->mNext = NULL;
    i++;
unlink_tail_check:
    if (i < pendingCount) {
        goto unlink_tail_body;
    }
unlink_done:

    mInitSlots[0].unk00[0] = 0;
    mInitSlots[0].unk00[1] = 0;
    func_8012FFB4(&mInitSlots[0].unk04);

    nextCount = (int)unk120 - 1;
    unk120 = (u32)nextCount;
    if (nextCount < 0) {
        unk120 = 0;
    }
}

// LLM-HARNESS-BEGIN: us-80133c20
extern "C" void func_8013314C() {}
// LLM-HARNESS-END: us-80133c20
// LLM-HARNESS-BEGIN: us-80133c4c
extern "C" void func_80133178() {}
// LLM-HARNESS-END: us-80133c4c
// LLM-HARNESS-BEGIN: us-801345bc
extern "C" int func_80133AE8() { if (lbl_eu_80664054 == 0) return 0; if (*reinterpret_cast<volatile int *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0x5c) == 0) { volatile unsigned short *flags = reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90); *flags = (unsigned short)(*flags | 0x4); return 0; } volatile unsigned short *flags = reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90); *flags = (unsigned short)(*flags & 0xfffb); *reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90) = (unsigned short)(*reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90) | 0x8); *reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90) = (unsigned short)(*reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90) | 0x10); *reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90) = (unsigned short)(*reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90) | 0x20); *reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90) = (unsigned short)(*reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90) | 0x40); *reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90) = (unsigned short)(*reinterpret_cast<volatile unsigned short *>(reinterpret_cast<unsigned char *>(lbl_eu_80664054) + 0xc90) | 0x80); return 0; }
// LLM-HARNESS-END: us-801345bc
// LLM-HARNESS-BEGIN: us-80136024
extern "C" void func_80135550() { if (lbl_eu_80664054 != 0) *(int*)((char*)lbl_eu_80664054 + 0x120) = 30; }
// LLM-HARNESS-END: us-80136024
// LLM-HARNESS-BEGIN: us-8013603c
extern "C" void func_80135568(int value) { if (lbl_eu_80664054 == 0) return; if (value != 0) *(unsigned char *)((char *)lbl_eu_80664054 + 0x149) = 1; else *(unsigned char *)((char *)lbl_eu_80664054 + 0x148) = 1; *(unsigned short *)((char *)lbl_eu_80664054 + 0xc90) = 0; }
// LLM-HARNESS-END: us-8013603c
// LLM-HARNESS-BEGIN: us-80136074
extern "C" void func_801355A0__Fv() {}
// LLM-HARNESS-END: us-80136074
// LLM-HARNESS-BEGIN: us-80136090
extern "C" void* func_801355BC() { if (lbl_eu_80664054 == 0) return 0; return reinterpret_cast<char*>(lbl_eu_80664054) + 0x9c; }
// LLM-HARNESS-END: us-80136090
// LLM-HARNESS-BEGIN: us-801360ac
extern "C" void* func_801355D8() { if (lbl_eu_80664054 == 0) return 0; return reinterpret_cast<char*>(lbl_eu_80664054) + 0xd8; }
// LLM-HARNESS-END: us-801360ac
// LLM-HARNESS-BEGIN: us-801360c8
extern "C" void* func_801355F4() { void* p = lbl_eu_80664054; if (p == 0) return 0; return *(void**)((char*)p + 0x5c); }
// LLM-HARNESS-END: us-801360c8
// LLM-HARNESS-BEGIN: us-801360e4
extern "C" void func_80135610() {}
// LLM-HARNESS-END: us-801360e4
// LLM-HARNESS-BEGIN: us-80136104
extern "C" void func_80135630() {}
// LLM-HARNESS-END: us-80136104
// LLM-HARNESS-BEGIN: us-80136128
extern "C" void func_80135654() {}
// LLM-HARNESS-END: us-80136128
// LLM-HARNESS-BEGIN: us-80136168
extern "C" void func_80135694() {}
// LLM-HARNESS-END: us-80136168
// LLM-HARNESS-BEGIN: us-80136190
extern "C" void func_801356BC() {}
// LLM-HARNESS-END: us-80136190
// LLM-HARNESS-BEGIN: us-801361b4
extern "C" void func_801356E0() {}
// LLM-HARNESS-END: us-801361b4
// LLM-HARNESS-BEGIN: us-8013636c
extern "C" void func_80135898() {}
// LLM-HARNESS-END: us-8013636c
// LLM-HARNESS-BEGIN: us-8013646c
extern "C" void func_80135998(u8 value) {
    u8 *obj = (u8 *)lbl_eu_80664054;
    if (obj == NULL)
        return;
    obj[0xc8c] = value;
}
// LLM-HARNESS-END: us-8013646c
// LLM-HARNESS-BEGIN: us-80136904
extern "C" void Draw__Q212CUICfManager5CTestFv() {}
// LLM-HARNESS-END: us-80136904
// LLM-HARNESS-BEGIN: us-80136908
extern "C" void Move__Q212CUICfManager5CTestFv() {}
// LLM-HARNESS-END: us-80136908
// LLM-HARNESS-BEGIN: us-8013690c
extern "C" void Term__Q212CUICfManager5CTestFv() {}
// LLM-HARNESS-END: us-8013690c
// LLM-HARNESS-BEGIN: us-80136910
extern "C" void Init__Q212CUICfManager5CTestFv() {}
// LLM-HARNESS-END: us-80136910

// LLM-HARNESS-BEGIN: us-80130a88
extern "C" void func_8012FFB4(void*) {}
// LLM-HARNESS-END: us-80130a88
// LLM-HARNESS-BEGIN: us-80130d18
extern "C" void func_80130244() {}
// LLM-HARNESS-END: us-80130d18
// LLM-HARNESS-BEGIN: us-80130f00
extern "C" void func_8013042C() {}
// LLM-HARNESS-END: us-80130f00
// LLM-HARNESS-BEGIN: us-801311f4
extern "C" void func_80130720() {}
// LLM-HARNESS-END: us-801311f4
// LLM-HARNESS-BEGIN: us-80131434
extern "C" void func_80130960() {}
// LLM-HARNESS-END: us-80131434
// LLM-HARNESS-BEGIN: us-80131648
extern "C" void func_80130B74() {}
// LLM-HARNESS-END: us-80131648
// LLM-HARNESS-BEGIN: us-80131854
extern "C" void func_80130D80() {}
// LLM-HARNESS-END: us-80131854
// LLM-HARNESS-BEGIN: us-80131a6c
extern "C" void func_80130F98() {}
// LLM-HARNESS-END: us-80131a6c
// LLM-HARNESS-BEGIN: us-80131c8c
extern "C" void func_801311B8() {}
// LLM-HARNESS-END: us-80131c8c
// LLM-HARNESS-BEGIN: us-801322f4
extern "C" void func_80131820() {}
// LLM-HARNESS-END: us-801322f4
// LLM-HARNESS-BEGIN: us-8013297c
extern "C" void __ct__CUICfManager() {}
// LLM-HARNESS-END: us-8013297c
// LLM-HARNESS-BEGIN: us-801341b8
extern "C" void func_801336E4() {}
// LLM-HARNESS-END: us-801341b8
// LLM-HARNESS-BEGIN: us-80134244
extern "C" void func_80133770() {}
// LLM-HARNESS-END: us-80134244
// LLM-HARNESS-BEGIN: us-8013439c
extern "C" void func_801338C8(CUICfManager*) {}
// LLM-HARNESS-END: us-8013439c
// LLM-HARNESS-BEGIN: us-801344dc
extern "C" void func_80133A08() {}
// LLM-HARNESS-END: us-801344dc
// LLM-HARNESS-BEGIN: us-80134654
extern "C" void func_80133B80() {}
// LLM-HARNESS-END: us-80134654
// LLM-HARNESS-BEGIN: us-80134774
extern "C" void func_80133CA0() {}
// LLM-HARNESS-END: us-80134774
// LLM-HARNESS-BEGIN: us-8013484c
extern "C" void func_80133D78() {}
// LLM-HARNESS-END: us-8013484c
// LLM-HARNESS-BEGIN: us-8013492c
extern "C" void func_80133E58() {}
// LLM-HARNESS-END: us-8013492c
// LLM-HARNESS-BEGIN: us-80134a1c
extern "C" void func_80133F48() {}
// LLM-HARNESS-END: us-80134a1c
// LLM-HARNESS-BEGIN: us-80134af8
extern "C" void func_80134024() {}
// LLM-HARNESS-END: us-80134af8
// LLM-HARNESS-BEGIN: us-80134bd4
extern "C" void func_80134100() {}
// LLM-HARNESS-END: us-80134bd4
// LLM-HARNESS-BEGIN: us-80134cac
extern "C" void func_801341D8() {}
// LLM-HARNESS-END: us-80134cac
// LLM-HARNESS-BEGIN: us-80134d84
extern "C" void func_801342B0() {}
// LLM-HARNESS-END: us-80134d84
// LLM-HARNESS-BEGIN: us-80134e5c
extern "C" void func_80134388() {}
// LLM-HARNESS-END: us-80134e5c
// LLM-HARNESS-BEGIN: us-80134f34
extern "C" void func_80134460() {}
// LLM-HARNESS-END: us-80134f34
// LLM-HARNESS-BEGIN: us-8013500c
extern "C" void func_80134538() {}
// LLM-HARNESS-END: us-8013500c
// LLM-HARNESS-BEGIN: us-801350fc
extern "C" void func_80134628() {}
// LLM-HARNESS-END: us-801350fc
// LLM-HARNESS-BEGIN: us-801351e8
extern "C" void func_80134714() {}
// LLM-HARNESS-END: us-801351e8
// LLM-HARNESS-BEGIN: us-801352c0
extern "C" void func_801347EC() {}
// LLM-HARNESS-END: us-801352c0
// LLM-HARNESS-BEGIN: us-8013539c
extern "C" void func_801348C8() {}
// LLM-HARNESS-END: us-8013539c
// LLM-HARNESS-BEGIN: us-80135474
extern "C" void func_801349A0() {}
// LLM-HARNESS-END: us-80135474
// LLM-HARNESS-BEGIN: us-8013554c
extern "C" void func_80134A78() {}
// LLM-HARNESS-END: us-8013554c
// LLM-HARNESS-BEGIN: us-80135624
extern "C" void func_80134B50() {}
// LLM-HARNESS-END: us-80135624
// LLM-HARNESS-BEGIN: us-80135708
extern "C" void func_80134C34() {}
// LLM-HARNESS-END: us-80135708
// LLM-HARNESS-BEGIN: us-801357ec
extern "C" void func_80134D18() {}
// LLM-HARNESS-END: us-801357ec
// LLM-HARNESS-BEGIN: us-80135924
extern "C" void func_80134E50() {}
// LLM-HARNESS-END: us-80135924
// LLM-HARNESS-BEGIN: us-80135a00
extern "C" void func_80134F2C() {}
// LLM-HARNESS-END: us-80135a00
// LLM-HARNESS-BEGIN: us-80135ae0
extern "C" void func_8013500C() {}
// LLM-HARNESS-END: us-80135ae0
// LLM-HARNESS-BEGIN: us-80135bb8
extern "C" void func_801350E4() {}
// LLM-HARNESS-END: us-80135bb8
// LLM-HARNESS-BEGIN: us-80135c98
extern "C" void func_801351C4() {}
// LLM-HARNESS-END: us-80135c98
// LLM-HARNESS-BEGIN: us-80135d78
extern "C" void func_801352A4() {}
// LLM-HARNESS-END: us-80135d78
// LLM-HARNESS-BEGIN: us-80135e54
extern "C" void func_80135380() {}
// LLM-HARNESS-END: us-80135e54
// LLM-HARNESS-BEGIN: us-80135f38
extern "C" void func_80135464() {}
// LLM-HARNESS-END: us-80135f38
// LLM-HARNESS-BEGIN: us-801361dc
extern "C" void func_80135708() {}
// LLM-HARNESS-END: us-801361dc
// LLM-HARNESS-BEGIN: us-80136480
extern "C" void func_801359AC() {}
// LLM-HARNESS-END: us-80136480
// LLM-HARNESS-BEGIN: us-801367d8
extern "C" void func_80135D04() {}
// LLM-HARNESS-END: us-801367d8
// LLM-HARNESS-BEGIN: us-80136a34
extern "C" void __dt__Q212CUICfManager5CTestFv() {}
// LLM-HARNESS-END: us-80136a34
