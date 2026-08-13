#include "kyoshin/CUICfManager.hpp"

// --- CTTask<CUICfManager> out-of-line specializations ---
// The canonical declared-only template emits no bodies; these explicit
// specializations produce the retail standalone Move/Draw/dtor symbols.
template<>
void CTTask<CUICfManager>::Move() {
    if (mMoveFunc) {
        (static_cast<CUICfManager*>(this)->*mMoveFunc)();
    }
}

template<>
void CTTask<CUICfManager>::Draw() {
    if (mDrawFunc) {
        (static_cast<CUICfManager*>(this)->*mDrawFunc)();
    }
}

template<>
CTTask<CUICfManager>::~CTTask() {}

#include "monolib/device/CDeviceFile.hpp"
#include "monolib/util/MemManager.hpp"

// Retail cf::IFlagEvent / IWorkEvent are trivially destructible (no retail
// __dt__ symbols); the empty out-of-line dtors here let MWCC elide the base-
// dtor calls from CUICfManager::~CUICfManager (reference: CTaskCulling dtor
// note — the body must be visible in the same TU). IWorkEvent's strong copy
// also lives in CTaskGame.cpp; the per-TU build tolerates the duplicate.
namespace cf {
IFlagEvent::~IFlagEvent() {}
}
IWorkEvent::~IWorkEvent() {}

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
void __dt__8CProcessFv(CProcess*, int);
void func_8015704C(CUICfInitBlock*, const CUICfInitBlock*);
void func_8009D0B4();
void func_8009D514(cf::IFlagEvent*);
void func_801390E0__FPP11CFileHandle(CFileHandle**);
void func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(nw4r::lyt::ArcResourceAccessor*);
// Deleting dtor (C-ABI import): the secondary-subobject thunks tail-call it
// with the adjusted `this`, so it is referenced through the flat retail name.
void __dt__12CUICfManagerFv(CUICfManager*);
}

IWorkEvent* CUICfManager::cfWorkEvent() {
    return static_cast<IWorkEvent*>(this);
}

// Virtual function thunks: adjust `this` and tail-call.
// us-80136a98
void CUICfManager::func_80135FC4() {
    __dt__12CUICfManagerFv((CUICfManager*)((char*)this - 0x54));
}

// us-80136aa0
void CUICfManager::func_80135FCC() {
    func_80133324__12CUICfManagerFv((CUICfManager*)((char*)this - 0x58), 0, 0, 0);
}

// us-80136aa8
void __dt__12CUICfManagerFv(void*); void func_80135FD4__12CUICfManagerFv(void* self) {
    __dt__12CUICfManagerFv((void*)((char*)self - 0x58));
}

void CUICfManager::Init() {
    CUICfInitProcess* process;
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

    mFileHandle = CDeviceFile::readFile(unk118, lbl_eu_806621A8, cfWorkEvent(), 0, 0);
    CDeviceFile::func_8044F154(mFileHandle, 3);

    process = static_cast<CUICfInitProcess*>(
        mtl::MemManager::allocate(0x54, CWorkThreadSystem::getWorkMem()));
    if (process != NULL) {
        __ct__8CProcessFv(reinterpret_cast<CProcess*>(process));
        process->vtable = lbl_eu_8052E404;
        ptmfWord1 = __ptmf_null[1];
        vtFinal = lbl_eu_8052E3BC;
        ptmfWord0 = __ptmf_null[0];
        process->callbacks[0] = ptmfWord0;
        process->callbacks[1] = ptmfWord1;
        ptmfWord2 = __ptmf_null[2];
        process->callbacks[2] = ptmfWord2;
        ptmfWord1 = __ptmf_null[1];
        ptmfWord0 = __ptmf_null[0];
        process->callbacks[3] = ptmfWord0;
        process->callbacks[4] = ptmfWord1;
        ptmfWord2 = __ptmf_null[2];
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
            clearCount = clearEnd - clearPtr;
            do {
                clearPtr->unk04 = 0;
                clearPtr->unk00 = 0;
                ++clearPtr;
            } while (--clearCount != 0);
        }

        tmpl.state.state = 0;
        tmpl.tail.unk00 = 0;
        // Overlay: retail sth zeros at +4..+0xE (copy still words at +4/+8).
        tailHalves = reinterpret_cast<u16*>(&tmpl.tail.mid);
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
    func_801390E0__FPP11CFileHandle(&mFileHandle);

    mtl::MemManager::deallocate(mPackedFont60.Destroy());
    mtl::MemManager::deallocate(mPackedFont9C.Destroy());
    mtl::MemManager::deallocate(mPackedFontD8.Destroy());

    func_80139124__FPQ34nw4r3lyt19ArcResourceAccessor(mArcResourceAccessor);
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
// Menu factories used by the func_80133A08-family create helpers below.
u32 func_8029BB24(CUICfUnk144*, u32, u32);
u32 func_8014A11C(CUICfUnk144*, u32, u32);
u32 func_8011CCE0(CUICfUnk144*, u32, u32);
u32 func_802AC494(CUICfUnk144*, u32, u32);
u32 __ct__CMenuItem(CUICfUnk144*, u32, u32, u32);
// Additional menu factories used by the func_80133A08-family create helpers.
u32 __ct__CMenuPause(CUICfUnk144*, u32);
// 6-arg factory: (ctx, scene, a, b, y, x) - func_80135464 forwards its two
// dead int params as args 3/4 (retail materializes them in the prologue).
u32 func_80113C84(CUICfUnk144*, u32, u32, u32, f32, f32);
u32 func_802514D4(CUICfUnk144*, u32);
// cpp-only imports: declared here rather than in the .hpp because other
// headers declare these same flat symbols with different signatures
// (func_80124B78: CMenuQstCnt.hpp int vs CMainMenu.hpp u32; func_8029A5DC:
// CMenuTutorial.hpp C++ member; func_8049603C: CTaskGame.hpp/code_80135FDC.hpp
// CScn* arg; getUnk80664658: CfGimmick.hpp vs CMainMenu.hpp already differ;
// lbl_eu_80663E14: CScn* in CTaskGame.hpp/code_80135FDC.hpp), so a header
// declaration would break TUs that co-include those headers.
extern "C" {
u32 func_80124B78();
void* func_8029A5DC(void* self, void* parent, u32 arg2);
CUICfCamView* func_8049603C(u8* scene);
CUICfGlobalSettings* getUnk80664658();
void* __ct__8011C1B8(void* ctx, u32 scene);
extern u8* lbl_eu_80663E14;
}

// Busy/state probe called by func_80134538 with the singleton as its arg
// (retail passes the just-loaded singleton through r3). noinline keeps
// the retail bl (MWCC would fold the trivial body otherwise).
int __declspec(noinline) func_80135D04(CUICfManagerCreateView* singleton) {
    u8 mode = (u8)lbl_eu_80664184;
    if (mode == 0x1a || mode == 0x1c) {
        return 1;
    }
    if (singleton->field_0x120 > 0) {
        return 1;
    }
    int st = func_801359AC();
    if (st != 0) {
        if (st == 2) {
            func_80138078(5);
        }
        return 1;
    }
    if (func_800FF738() != 0) {
        return 1;
    }
    if (func_80124B78() != 0) {
        return 1;
    }
    if (func_801B481C() != 0) {
        return 1;
    }
    if (func_80293C10() != 0) {
        return 1;
    }
    if (func_8029A658() != 0) {
        return 1;
    }
    if (getInstance__11CSysWinBuffFv() != 0) {
        return 1;
    }
    CUICfGlobalSettings* g = getUnk80664658();
    if ((g->field_214 & 0x40000) != 0) {
        func_80138078(5);
        return 1;
    }
    f32 v = func_8049603C(lbl_eu_80663E14)->field_C;
    f32 c = lbl_eu_806672C8;
    return (c - v) < c;
}
// Additional menu factories (CMenuGCItem / CMenuGetItem / CMenuSave TUs).
u32 __ct__CMenuGCItem(CUICfUnk144*, u32, u8);
u32 func_8014A064(CUICfUnk144*, u32, u16);
u32 func_8028E3B4(CUICfUnk144*, u32, u32, u32, u32);
void func_8013DA60(int, int, int);
void* func_8009EC9C(u16);
void func_800A21F8(void*, u16, int, int);
u8 func_801361E8(u32, const char*, u32);
u16 func_8013606C(char*, char*, u16);
void* lbl_eu_80573D18[];
u8 lbl_eu_805000A8[];
u16 lbl_eu_804FFFDC[];
}

// C++ mangling -> retail `func_8013B428__FUl`.
void func_8013B428(u32);

void CUICfManager::func_80133324(int id, int a1, int a2) {
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
    u8 res = (u8)func_801361E8((u32)tableVal, (char*)lbl_eu_805000A8 + 0x43, lbl_eu_80664050);
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

void func_8012FFB4(void*);
void func_80133770();
extern "C" u32 func_801338C8(CUICfManager*);
extern "C" {
u32 lbl_eu_80663E24;
u32 lbl_eu_80663E28;
f32 lbl_eu_806672CC;


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
 // &mInitSlots[0].unk04
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
    party = cf::CfGameManager::getPlayer(0);
    if (party == NULL) {
        goto after_enum;
    }

    func_80043D90(&holder);
    list = func_80043F18(&holder);
    func_800F4A98(list, 0x130, 0);
    party = cf::CfGameManager::getPlayer(0);
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

void func_8013314C(){}
void CUICfManager_func_33178(){}
int CUICfManager_prepareMenus(){
    CUICfManager* m = (CUICfManager*)lbl_eu_80664054;
    if (m == 0) return 0;
    return m->prepareMenus();
}
void CUICfManager_setTimeout30(){
    if (lbl_eu_80664054 != 0) {
        static_cast<CUICfManager*>(lbl_eu_80664054)->setTimeout30();
    }
}
void CUICfManager_setFlags(int value){
    if (lbl_eu_80664054 == 0) return;
    CUICfManager* manager = static_cast<CUICfManager*>(lbl_eu_80664054);
    manager->setFlagState(value != 0);
}
void func_801355A0__Fv(){}
void* CUICfManager_getPackedFont9C(){
    CUICfManager* m = (CUICfManager*)lbl_eu_80664054;
    return m == 0 ? 0 : m->getPackedFont9C();
}
void* CUICfManager_getPackedFontD8(){
    CUICfManager* m = (CUICfManager*)lbl_eu_80664054;
    return m == 0 ? 0 : m->getPackedFontD8();
}
void* CUICfManager_getField5C(){
    CUICfManager* m = (CUICfManager*)lbl_eu_80664054;
    return m == 0 ? 0 : m->getArcResourceAccessor();
}
void func_80135610(){}
void func_80135630(){}
void func_80135654(){}
void func_80135694(){}
void func_801356BC(){}
void func_801356E0(){}
extern "C" int func_80135898() {
    CUICfManager* inst = (CUICfManager*)lbl_eu_80664054;
    if (inst == 0) {
        return 0;
    }
    // Scan the init slots' unk04 flag for the "in use" value 4 (0x168 stride).
    CUICfSlotFlagView* slots = (CUICfSlotFlagView*)((u8*)inst + 0x14c);
    for (int i = 0; i < 8; i++) {
        if (slots[i].flag == 4) {
            return 1;
        }
    }
    return 0;
}
void CUICfManager_setFieldC8C(u8 value){
    CUICfManager* m = (CUICfManager*)lbl_eu_80664054;
    if (m == NULL)
        return;
    m->setFieldC8C(value);
}
void Draw__Q212CUICfManager5CTestFv(){}
void Move__Q212CUICfManager5CTestFv(){}
void Term__Q212CUICfManager5CTestFv(){}
void Init__Q212CUICfManager5CTestFv(){}

void func_8012FFB4(void*){}
void func_80130244(){}
extern "C" int func_8013042C(u8* base, u8 index) { return 0; }
void func_80130720(){}
void func_80130960(){}
void func_80130B74(){}
void func_80130D80(){}
void func_80130F98(){}
void func_801311B8(){}
void func_80131820(){}
void __ct__CUICfManager(){}
void func_801336E4(){}
void func_80133770(){}
// func_801338C8: create the menu via __ct__8011C1B8 and queue it
// (mFlags bit 0x2; busy-bit/accessor gates + scene-mode guard).
extern "C" u32 func_801338C8(CUICfManager* self) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
        return 0;
    }
    if (inst->field_0x5C == NULL) {
        inst->field_0xC90 = (u16)(inst->field_0xC90 | 0x2);
        return 0;
    }
    inst->field_0xC90 = (u16)(inst->field_0xC90 & 0xfffd);
    if ((u8)lbl_eu_80664184 <= 1 || (u8)lbl_eu_80664184 == 0x1d) {
        return 0;
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    {
        u32 tempRet = (u32)__ct__8011C1B8(inst->field_0x144, inst->field_0x11C);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}
// ---------------------------------------------------------------------------
// Menu-create + event-queue push helpers (func_80133A08 family).
//
// Shared skeleton (same slot walk as func_80133324's queue push):
//   1) null-check the lbl_eu_80664054 singleton,
//   2) call a menu factory; the result is kept in volatile savedRet@0x8
//      (spilled right after the call, reloaded for the setItem store and
//      again for the return value),
//   3) scan the 0xC-stride event-node array (unk138) for the first empty
//      slot (item == 0),
//   4) reslist-style setItem push; the try block forces the stw-r1 frame
//      home at 0x24 and the frame-link epilogue,
//   5) link the node into the circular list at unk128 and return savedRet.
// ---------------------------------------------------------------------------

// func_80134E50: create the option menu and queue it.
u32 func_80134E50(u32 arg) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = func_8029BB24(inst->field_0x144, inst->field_0x11C, arg);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}

// func_801352A4: create the get-item menu and queue it.
u32 func_801352A4(u32 arg) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = func_8014A11C(inst->field_0x144, inst->field_0x11C, arg);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}

// func_80133A08: create the quest-log menu and queue it (id narrowed to u16).
u32 func_80133A08(u32 arg) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = func_8011CCE0(inst->field_0x144, inst->field_0x11C, (u16)arg);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}

// func_80133D78: construct the base menu item and queue it.
u32 func_80133D78() {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = __ct__CMenuItem(inst->field_0x144, inst->field_0x11C, 0, 1);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}

// func_80134F2C: create the tutorial-list menu and queue it (id narrowed to u16).
u32 func_80134F2C(u32 arg) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = func_802AC494(inst->field_0x144, inst->field_0x11C, (u16)arg);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}

// func_80133B80: create the event menu via func_801109D8 and queue it
// (mFlags bit 0x10; busy-bit and accessor gates, same as Move's flag path).
extern "C" u32 func_80133B80() {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
        return 0;
    }
    if (inst->field_0x5C == NULL) {
        inst->field_0xC90 = (u16)(inst->field_0xC90 | 0x10);
        return 0;
    }
    inst->field_0xC90 = (u16)(inst->field_0xC90 & 0xffef);
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    {
        u32 tempRet = (u32)func_801109D8(inst->field_0x144, inst->field_0x11C, 0);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}
void func_80133CA0(){}
// us-8013492c: create the pause-style menu via CMenuPause's factory and queue
// it. The 3rd factory arg is the u8-narrowed first param, the 2nd is the
// caller's scene id or the manager's own unk11C when 0.
u32 func_80133E58(CUICfManager* self, u32 arg1, u32 arg2) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = __ct__CMenuItem(inst->field_0x144, arg1 != 0 ? arg1 : inst->field_0x11C,
                                      (u8)self, arg2);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}
void func_80133F48(){}
void func_80134024(){}
void func_80134100(){}
void func_801341D8(){}
void func_801342B0(){}
void func_80134388(){}
void func_80134460(){}
// us-8013500c: probe func_80135D04, then create the world-map menu via
// func_802514D4 and queue it.
u32 func_80134538() {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    if (func_80135D04(inst) != 0) {
        return 0;
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    {
        u32 tempRet = func_802514D4(inst->field_0x144, inst->field_0x11C);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}
// us-801350fc: create the pause menu via __ct__CMenuPause and queue it
// (gated on the lbl_eu_80663E28 busy bit).
u32 func_80134628() {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    if ((lbl_eu_80663E28 & 0x01000000u) != 0) {
        return 0;
    }
    {
        u32 tempRet = __ct__CMenuPause(inst->field_0x144, inst->field_0x11C);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}
void func_80134714(){}
void func_801347EC(){}
void func_801348C8(){}
void func_801349A0(){}
void func_80134A78(){}
// func_80134B50: create the save menu and queue it, forwarding both own args
// (retail keeps them in r6/r7 across the singleton check for the call).
u32 func_80134B50(u32 arg0, u32 arg1) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = func_8028E3B4(inst->field_0x144, inst->field_0x11C, 0, arg0, arg1);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}

// func_80134C34: create the save menu with fixed ctor args (1, 0, 1) and queue it.
u32 func_80134C34() {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = func_8028E3B4(inst->field_0x144, inst->field_0x11C, 1, 0, 1);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}

// func_80134D18: create the tutorial menu via func_8029A5DC and queue it.
// arg0 = slot index (u8), arg1 = scene id (or the manager's own when 0),
// arg2 = skip-reset flag (0 resets the slot via func_8013042C first).
extern "C" u32 func_80134D18(u32 a0, u32 a1, u32 a2) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    if (a2 == 0) {
        int reset = 0;
        if (inst != NULL) {
            reset = func_8013042C((u8*)inst + 0x14c, (u8)a0);
        }
        if (reset != 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    {
        u32 tempRet =
            (u32)func_8029A5DC(inst->field_0x144, a1 != 0 ? a1 : inst->field_0x11C, (u8)a0);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}
void func_8013500C(){}
// func_801350E4: create the GC-item menu and queue it (id narrowed to u8).
u32 func_801350E4(u32 arg) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = __ct__CMenuGCItem(inst->field_0x144, inst->field_0x11C, (u8)arg);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}

// func_801351C4: create the get-item menu and queue it (id narrowed to u16).
u32 func_801351C4(u32 arg) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = func_8014A064(inst->field_0x144, inst->field_0x11C, (u16)arg);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}

// func_80135380: create the multi get-item menu and queue it (id narrowed to u8).
u32 func_80135380(u32 arg) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = func_8014A1D4(inst->field_0x144, inst->field_0x11C, (u8)arg, 0);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}
// us-80135f38: create the fade menu via func_80113C84 and queue it. The two
// dead int params are forwarded as the factory's args 3/4 (retail materializes
// them with mr r5,r3 / mr r6,r4 at the prologue) and the float params are
// swapped (retail fmr f0,f1 / f1=f2 / f2=f0).
u32 func_80135464(u32 a, u32 b, f32 x, f32 y) {
    CUICfManagerCreateView* inst;
    volatile u32 savedRet;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    {
        u32 tempRet = func_80113C84(inst->field_0x144, inst->field_0x11C, a, b, y, x);
        savedRet = tempRet;
        if (tempRet == 0) {
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    i = 0;
    byteOff = 0;
    startNode = inst->field_0x128;
    capacity = inst->field_0x13C;
    goto slot_check;
slot_body:
    if (*(u32*)((u8*)inst->field_0x138 + byteOff) == 0) {
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
        CUICfListNode* temp = &inst->field_0x138[i];
        u32* ptr = &temp->item;
        if (ptr != 0) {
            try {
                *ptr = savedRet;
            } catch (...) {
                throw;
            }
        }
        temp->next = startNode;
        temp->prev = startNode->prev;
        startNode->prev->next = temp;
        startNode->prev = temp;
    }
    return savedRet;
}
void func_80135708(){}
extern "C" int func_801359AC() { return 0; }
void __dt__Q212CUICfManager5CTestFv(){}

// ---------------------------------------------------------------------------
// CUICfManager::~CUICfManager (us-80132dc8)
//
// Retail: the reslist-shaped event queue at +0x124 is cleared inline (vtable
// restore, sentinel walk nulling each node's mNext, then array-delete of the
// slot pool when the owns-flag is clear); the PackedFont members are
// destroyed with flag -1 and the CTTask base chain collapses to a direct
// __dt__8CProcessFv(this, 0). The doubled `if (q != NULL)` mirrors retail's
// two consecutive beqs against the single addic. on &mEventQueue (same shape
// as CUIBattleManager's __dt__reslist_IUIBattle).
// ---------------------------------------------------------------------------
CUICfManager::~CUICfManager() {
    if (this != NULL) {
        CUICfEventQueue* q = reinterpret_cast<CUICfEventQueue*>(&unk124);
        if (q != NULL) {
            if (q != NULL) {
                q->vtable = lbl_eu_8052E3B0;
                {
                    CUICfListNode* cur = q->head->next;
                    while (cur != q->head) {
                        CUICfListNode* prev = cur;
                        cur = cur->next;
                        prev->next = NULL;
                    }
                    q->head->next = q->head;
                    q->head->prev = q->head;
                }
                if (q->ownsList == 0 && q->nodes != NULL) {
                    delete[] q->nodes;
                    q->nodes = NULL;
                }
            }
        }
    }
}

void CUICfManager::func_80135FBC() {
    // retail: subi r3,r3,0x54; b OnFileEvent__12CUICfManagerFv — secondary-subobject thunk
    reinterpret_cast<CUICfManager*>(reinterpret_cast<char*>(this) - 0x54)->OnFileEvent();
}

extern "C" void func_801355BC() {}
