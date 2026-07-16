#include "kyoshin/CUICfManager.hpp"

#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

#include <decomp.h>

// §17.6 whole-function asm: packed tail copy + -0x1A0/stmw frame not recoverable
// in high-level C++ (60% STRUCTURAL). User-approved "Fix it".

extern "C" {
CProcess* lbl_eu_80664054;
const char* lbl_eu_806621A8;
char lbl_eu_8052E404[];
char lbl_eu_8052E3BC[];
u32 __ptmf_null[3];
void __ct__8CProcessFv(CProcess*);
void* getWorkMem__17CWorkThreadSystemFv(void);
void* allocate__Q23mtl10MemManagerFUlUl(u32, u32);
CFileHandle* readFile__11CDeviceFileFUlPCcP10IWorkEventii(u32, const char*, IWorkEvent*, int, int);
void func_8044F154__11CDeviceFileFP11CFileHandlei(CFileHandle*, int);
void Regist__8CProcessFP8CProcessb(CProcess*, CProcess*, bool);
void func_8015704C(void*, const void*);
void func_8009D0B4();
void func_8009D514(cf::IFlagEvent*);
void func_801390E0(CFileHandle**);
void func_80139124(nw4r::lyt::ArcResourceAccessor*);
}

asm void CUICfManager::Init() {
    nofralloc
    stwu r1, -0x1a0(r1)
    mflr r0
    cmpwi r3, 0x0
    stw r0, 0x1a4(r1)
    stmw r22, 0x178(r1)
    mr r23, r3
    mr r5, r23
    beq lbl_80132EEC
    addi r5, r3, 0x54
lbl_80132EEC:
    lwz r3, 0x118(r3)
    li r6, 0x0
    lwz r4, lbl_eu_806621A8
    li r7, 0x0
    bl readFile__11CDeviceFileFUlPCcP10IWorkEventii
    stw r3, 0x114(r23)
    li r4, 0x3
    bl func_8044F154__11CDeviceFileFP11CFileHandlei
    bl getWorkMem__17CWorkThreadSystemFv
    mr r4, r3
    li r3, 0x54
    bl allocate__Q23mtl10MemManagerFUlUl
    cmpwi r3, 0x0
    mr r22, r3
    beq lbl_80132F7C
    bl __ct__8CProcessFv
    lis r3, lbl_eu_8052E404@ha
    lis r5, __ptmf_null@ha
    addi r3, r3, lbl_eu_8052E404@l
    stw r3, 0x10(r22)
    addi r5, r5, __ptmf_null@l
    lwz r0, 0x4(r5)
    lis r3, lbl_eu_8052E3BC@ha
    lwz r4, 0x0(r5)
    addi r3, r3, lbl_eu_8052E3BC@l
    stw r4, 0x3c(r22)
    stw r0, 0x40(r22)
    lwz r0, 0x8(r5)
    stw r0, 0x44(r22)
    lwz r0, 0x4(r5)
    lwz r4, 0x0(r5)
    stw r4, 0x48(r22)
    stw r0, 0x4c(r22)
    lwz r0, 0x8(r5)
    stw r0, 0x50(r22)
    stw r3, 0x10(r22)
lbl_80132F7C:
    stw r22, 0x144(r23)
    mr r3, r22
    li r5, 0x0
    lwz r4, lbl_eu_80664054
    bl Regist__8CProcessFP8CProcessb
    addi r27, r1, 0xdc
    addi r26, r1, 0x40
    addi r25, r1, 0x74
    addi r24, r1, 0xa8
    li r28, 0x0
    li r30, 0x0
    li r31, 0x7
    li r22, 0x34
lbl_80132FB0:
    clrlwi r0, r28, 24
    addi r3, r1, 0x40
    mulli r0, r0, 0x168
    sth r30, 0x10(r1)
    cmplw r3, r27
    stw r30, 0xc(r1)
    add r29, r23, r0
    bge lbl_80132FF4
    addi r0, r27, 0x33
    subf r0, r3, r0
    divwu r0, r0, r22
    mtctr r0
    bge lbl_80132FF4
lbl_80132FE4:
    sth r30, 0x4(r3)
    stw r30, 0x0(r3)
    addi r3, r3, 0x34
    bdnz lbl_80132FE4
lbl_80132FF4:
    stb r30, 0x9(r1)
    addi r3, r29, 0x150
    addi r4, r1, 0xc
    stw r30, 0xdc(r1)
    sth r30, 0xe0(r1)
    sth r30, 0xe2(r1)
    sth r30, 0xe4(r1)
    sth r30, 0xe6(r1)
    sth r30, 0xe8(r1)
    sth r30, 0xea(r1)
    stb r30, 0xec(r1)
    stb r30, 0xee(r1)
    stb r30, 0x12e(r1)
    stb r31, 0x8(r1)
    stb r31, 0x14c(r29)
    lbz r0, 0x9(r1)
    stb r0, 0x14d(r29)
    bl func_8015704C
    mr r4, r26
    addi r3, r29, 0x184
    bl func_8015704C
    mr r4, r25
    addi r3, r29, 0x1b8
    bl func_8015704C
    mr r4, r24
    addi r3, r29, 0x1ec
    bl func_8015704C
    lwz r0, 0xdc(r1)
    addi r28, r28, 0x1
    stw r0, 0x220(r29)
    cmplwi r28, 0x8
    lwz r0, 0xe4(r1)
    lwz r3, 0xe0(r1)
    stw r3, 0x224(r29)
    stw r0, 0x228(r29)
    lhz r0, 0xe8(r1)
    sth r0, 0x22c(r29)
    lwz r0, 0xea(r1)
    stw r0, 0x22e(r29)
    lwz r0, 0xf2(r1)
    lwz r3, 0xee(r1)
    stw r3, 0x232(r29)
    stw r0, 0x236(r29)
    lwz r0, 0xfa(r1)
    lwz r3, 0xf6(r1)
    stw r3, 0x23a(r29)
    stw r0, 0x23e(r29)
    lwz r0, 0x102(r1)
    lwz r3, 0xfe(r1)
    stw r3, 0x242(r29)
    stw r0, 0x246(r29)
    lwz r0, 0x10a(r1)
    lwz r3, 0x106(r1)
    stw r3, 0x24a(r29)
    stw r0, 0x24e(r29)
    lwz r0, 0x112(r1)
    lwz r3, 0x10e(r1)
    stw r3, 0x252(r29)
    stw r0, 0x256(r29)
    lwz r0, 0x11a(r1)
    lwz r3, 0x116(r1)
    stw r3, 0x25a(r29)
    stw r0, 0x25e(r29)
    lwz r0, 0x122(r1)
    lwz r3, 0x11e(r1)
    stw r3, 0x262(r29)
    stw r0, 0x266(r29)
    lwz r0, 0x12a(r1)
    lwz r3, 0x126(r1)
    stw r3, 0x26a(r29)
    stw r0, 0x26e(r29)
    lwz r0, 0x132(r1)
    lwz r3, 0x12e(r1)
    stw r3, 0x272(r29)
    stw r0, 0x276(r29)
    lwz r0, 0x13a(r1)
    lwz r3, 0x136(r1)
    stw r3, 0x27a(r29)
    stw r0, 0x27e(r29)
    lwz r0, 0x142(r1)
    lwz r3, 0x13e(r1)
    stw r3, 0x282(r29)
    stw r0, 0x286(r29)
    lwz r0, 0x14a(r1)
    lwz r3, 0x146(r1)
    stw r3, 0x28a(r29)
    stw r0, 0x28e(r29)
    lwz r0, 0x152(r1)
    lwz r3, 0x14e(r1)
    stw r3, 0x292(r29)
    stw r0, 0x296(r29)
    lwz r0, 0x15a(r1)
    lwz r3, 0x156(r1)
    stw r3, 0x29a(r29)
    stw r0, 0x29e(r29)
    lwz r0, 0x162(r1)
    lwz r3, 0x15e(r1)
    stw r3, 0x2a2(r29)
    stw r0, 0x2a6(r29)
    lwz r0, 0x16a(r1)
    lwz r3, 0x166(r1)
    stw r3, 0x2aa(r29)
    stw r0, 0x2ae(r29)
    blt lbl_80132FB0
    lmw r22, 0x178(r1)
    lwz r0, 0x1a4(r1)
    mtlr r0
    addi r1, r1, 0x1a0
    blr
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
    // Decl order: savedRet@0x8, gap, setItem stw-r1 home, idTable@0x28 (retail frame).
    volatile int savedRet;
    int pad0C;
    int pad10;
    int pad14;
    int pad18;
    int pad1C;
    int pad20;
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

            _reslist_node<u32>* startNode = (_reslist_node<u32>*)inst->unk128;
            int capacity = inst->unk13C;
            int i = 0;
            int byteOff = 0;
            // Keep pads "live" enough that MWCC does not DCE them entirely.
            pad0C = capacity;
            pad10 = byteOff;
            pad14 = i;
            pad18 = pad0C;
            pad1C = pad10;
            pad20 = pad14;
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
                _reslist_node<u32>* temp = (_reslist_node<u32>*)((u8*)inst->unk138 + i * 0xc);
                temp->setItem((u32)savedRet);
                temp->mNext = startNode;
                temp->mPrev = startNode->mPrev;
                startNode->mPrev->mNext = temp;
                startNode->mPrev = temp;
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
// Early r4 is NOT a fake-Fv arg — retail does `lhz r4, 0xc90(r3)` (mFlags).
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
void* func_800F6EC0(void* list, int index); // &slot → has +0x4 object ptr
void* func_800F6E98(void* list, int index); // *slot → object*
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
            *fp = (u16)(*fp & ~0x1); // rlwinm …,16,30
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
