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
#include "monolib/device/CDevice.hpp"
#include "monolib/lib/CLibLayout.hpp"
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
void func_8009D414(cf::IFlagEvent*);
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
// Vtable thunk for the secondary-subobject slot of func_80133324: the retail
// symbol mangles the thunk as a no-arg member (`Fv`) but the vtable entry
// passes its three hidden args (r4/r5/r6) straight through to the Fiii
// callee after adjusting `this` by -0x58. Emitted under the extern "C" flat
// name so the exact retail symbol is produced (OnFileEvent precedent); the
// in-class no-arg declaration stays declared-only.
extern "C" void func_80135FCC__12CUICfManagerFv(CUICfManager* self, int id, int a1, int a2) {
    func_80133324__12CUICfManagerFiii((CUICfManager*)((char*)self - 0x58), id, a1, a2);
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
void* func_8029A5DC(void* self, u32 parent, u32 arg2);
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

u32 func_80133770();
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
void func_80043E08(CUICfEnumListHolder*, int, int); // holder init (r4=0x20, r5=0x800)
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

// func_8013314C: when no file handle is loaded, copy the 12-byte event
// triplet (lbl_eu_8052E294) onto self+0x3C. The pair copy stores w1 before
// w0 (retail lwzu/lwz + reversed stw pair + trailing word).
extern u32 lbl_eu_8052E294[3];
extern "C" void func_8013314C(CUICfManager* self) {
    if (*(void**)((u8*)self + 0x114) != 0) {
        return;
    }
    const u32* src = (const u32*)lbl_eu_8052E294;
    u32 w0 = src[0];
    u32 w1 = src[1];
    u32* dst = (u32*)((u8*)self + 0x3C);
    dst[1] = w1;
    dst[0] = w0;
    dst[2] = src[2];
}
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
void* func_801355A0__Fv() {
    CUICfManager* m = (CUICfManager*)lbl_eu_80664054;
    return m == 0 ? 0 : (void*)((char*)m + 0x60);
}
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
// Retail linker name for the slot-reset helper below (flat); declaring it
// extern "C" here makes the C++ definition emit the flat retail symbol.
extern "C" int func_80130244(u8* base);

int func_80135610() {
    CUICfManager* m = static_cast<CUICfManager*>(lbl_eu_80664054);
    if (m == 0) {
        return 0;
    }
    return func_80130244(reinterpret_cast<u8*>(reinterpret_cast<CUICfManagerSlotView*>(m)->slots));
}
// func_80135630: like func_80135610 but forwards its first argument (the
// caller's this) as the src arg of the template-copy claim func_80130720.
// Declaring func_80130720 extern "C" makes its C++ definition emit the
// flat retail symbol.
extern "C" int func_80130720(u8* base, CUICfSrcCopyView* src);
extern "C" int func_80135630(CUICfSrcCopyView* src) {
    CUICfManager* m = static_cast<CUICfManager*>(lbl_eu_80664054);
    if (m == 0) {
        return 0;
    }
    return func_80130720(
        reinterpret_cast<u8*>(reinterpret_cast<CUICfManagerSlotView*>(m)->slots), src);
}
// func_80135654: forwards all five args to the 5-id slot claim
// func_80130960 after the null gate (retail shifts the arg registers up).
extern "C" int func_80130960(u8* base, u16 a1, u16 a2, u16 a3, u16 a4, u16 a5);
extern "C" int func_80135654(u16 a1, u16 a2, u16 a3, u16 a4, u16 a5) {
    CUICfManager* m = static_cast<CUICfManager*>(lbl_eu_80664054);
    if (m == 0) {
        return 0;
    }
    return func_80130960(
        reinterpret_cast<u8*>(reinterpret_cast<CUICfManagerSlotView*>(m)->slots),
        a1, a2, a3, a4, a5);
}
// func_80135694: forwards both args (id, state) to the slot claim
// func_80130B74 after the null gate (retail: or r5,r4,r4; or r4,r3,r3).
extern "C" int func_80130B74(u8* base, u16 id, u8 state);
extern "C" int func_80135694(u16 id, u8 state) {
    CUICfManager* m = static_cast<CUICfManager*>(lbl_eu_80664054);
    if (m == 0) {
        return 0;
    }
    return func_80130B74(
        reinterpret_cast<u8*>(reinterpret_cast<CUICfManagerSlotView*>(m)->slots), id, state);
}
// func_801356BC: forwards its first argument as the (u16) second arg of the
// sprintf-based slot claim func_80130D80 (same null-gate shape as the other
// slot helpers).
extern "C" int func_80130D80(u8* base, u16 arg1);
extern "C" int func_801356BC(u16 arg1) {
    CUICfManager* m = static_cast<CUICfManager*>(lbl_eu_80664054);
    if (m == 0) {
        return 0;
    }
    return func_80130D80(
        reinterpret_cast<u8*>(reinterpret_cast<CUICfManagerSlotView*>(m)->slots), arg1);
}
// func_801356E0: same two-arg forward to func_80130F98 (slot claim with id + arg).
extern "C" int func_80130F98(u8* base, u16 a1, u16 a2);
extern "C" int func_801356E0(u16 a1, u16 a2) {
    CUICfManager* m = static_cast<CUICfManager*>(lbl_eu_80664054);
    if (m == 0) {
        return 0;
    }
    return func_80130F98(
        reinterpret_cast<u8*>(reinterpret_cast<CUICfManagerSlotView*>(m)->slots), a1, a2);
}
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

// ---------------------------------------------------------------------------
// func_8012FFB4 (us-80130a88)
//
// Per-slot state machine for the free-menu system. Runs only when the task /
// global gate bits are clear, then dispatches on the slot's flag byte:
//   0 -> close the party window; 1 -> claim a tutorial slot; 2 -> open the
//   5-id selection window; 3 -> reset a block region; 4 -> open the id window;
//   5/6 -> open the message windows. The slot flag is then reset to 7 (free)
// and the slot list is re-sorted (func_80131820).
// ---------------------------------------------------------------------------
extern "C" void func_8012FFB4(u8* base) {
    volatile u32 savedRet;
    CUICfSlotDispView* slot;
    CUICfManagerCreateView* inst;
    int i;
    int byteOff;
    int capacity;
    CUICfListNode* startNode;

    // OR-combined guard: first disjunct emits the direct `bne end`, second
    // the retail branch-over `beq cont; b end` (MWCC_REFERENCE func_802AE004).
    if (func_800426F0__9CTaskGameFv(getInstance__9CTaskGameFv()) != 0 ||
        (lbl_eu_80663E28 & 0x00200000u) != 0) {
        return;
    }
    {
        // Dual SDA load (r0 and r3) so the rlwinm/andis pair read separate
        // loads; a plain single read would CSE (Move's volatile re-read).
        u32 g24_0 = lbl_eu_80663E24;
        u32 g24_1 = *(volatile u32*)&lbl_eu_80663E24;
        if ((g24_0 & 0x01000000u) == 0) {
            if ((g24_1 & 0xafe40000u) != 0) {
                return;
            }
        }
        {
            f32 v = func_8049603C(lbl_eu_80663E14)->field_C;
            f32 c = lbl_eu_806672C8;
            if ((c - v) < c) {
                return;
            }
        }
        if ((lbl_eu_80663E24 & 0x00000200u) != 0) {
            return;
        }
        if (cf::CfGameManager::func_800829B8() != 0) {
            return;
        }
        int busy;
        if (func_80293C10() != 0) {
            busy = 1;
        } else if (func_8029A658() != 0) {
            busy = 1;
        } else if (func_801B481C() != 0) {
            busy = 1;
        } else if (func_80122450() != 0) {
            busy = 1;
        } else {
            busy = func_80124B78() != 0;
        }
        if (busy != 0) {
            return;
        }

        slot = reinterpret_cast<CUICfSlotDispView*>(base);
        if (slot->field_0x00 == 7) {
            return;
        }
    switch (slot->field_0x00) {
    case 0:
        func_8013E8E0(1);
        break;
    case 1: {
        inst = (CUICfManagerCreateView*)lbl_eu_80664054;
        u8 idx = slot->field_0x01;
        if (inst != NULL) {
            u32 tempRet = (u32)func_8029A5DC(inst->field_0x144, inst->field_0x11C, idx);
            savedRet = tempRet;
            if (tempRet != 0) {
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
                    CUICfListNode* temp = (CUICfListNode*)((u8*)inst->field_0x138 + i * 0xc);
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
        break;
    }
    case 3:
        func_8013E424(reinterpret_cast<u8*>(&slot->block_04), 1);
        break;
    case 2: {
        // Shared 1-constant (retail li r8,1 + mr r9/r10 before the id loads).
        int one = 1;
        func_8013E2E0(slot->field_D8, slot->field_DA, slot->field_DC, slot->field_DE,
                      slot->field_E0, one, one, one, 0);
        break;
    }
    case 4:
        func_8013DA60(slot->field_E2, slot->field_E4, 1);
        break;
    case 5:
        func_8013D55C(slot->str_E6, 0, 1);
        break;
    case 6:
        func_8013D688(slot->str_E6, slot->str_126, 0, 1);
        break;
    default:
        break;
    }

    slot->field_0x00 = 7;
    func_80131820(base);
    }
}
// func_80130244 (us-80130d18): slot reset. When no busy gate is set and all
// init slots are free (state 7), clears the first in-use slot and rebuilds
// the slot list (func_801311B8) unless the 0xb40 byte is set.
int func_80130244(u8* base) {
    int flag = func_80293C10() != 0 || func_8029A658() != 0 || func_801B481C() != 0 ||
               func_80122450() != 0 || func_80124B78() != 0;
    if (flag == 0) {
        u8 i = 0;
        do {
            if (base[i * 0x168] != 7) {
                flag = 1;
                break;
            }
            i++;
        } while (i < 8);
    }
    if (flag != 0) {
        u8 j = 0;
        int changed = 0;
        do {
            if (base[j * 0x168] == 7) {
                base[j * 0x168] = 0;
                changed = 1;
                break;
            }
            j++;
        } while (j < 8);
        if (changed != 0 && base[0xb40] == 0) {
            func_801311B8(base);
        }
        return 1;
    }
    return 0;
}
// func_8013042C (us-80130f00): slot claim. Same busy-gate + all-free scan
// skeleton as func_80130960; the claim body sets flag 1 and the index byte
// at +1, then rebuilds the slot list unless the 0xb40 byte is set. The
// claim loop keeps `changed` uninitialized on the no-claim path (retail
// never materializes a 0 init; the flag is only read after a claim).
extern "C" int func_8013042C(u8* base, u8 index) {
    int flag;
    int gateBusy;
    if (func_80293C10() != 0) {
        gateBusy = 1;
    } else if (func_8029A658() != 0) {
        gateBusy = 1;
    } else if (func_801B481C() != 0) {
        gateBusy = 1;
    } else if (func_80122450() != 0) {
        gateBusy = 1;
    } else {
        gateBusy = func_80124B78() != 0;
    }
    if (gateBusy != 0) {
        flag = 1;
    } else {
        u8 i;
        for (i = 0; i < 8; i++) {
            if (base[i * 0x168] != 7) {
                flag = 1;
                break;
            }
        }
    }
    if (flag == 0) {
        return 0;
    }
    {
        // Retail fully unrolls the claim scan (8 explicit iterations; the
        // sibling claims compile to mtctr loops, but this one's 2-store body
        // keeps MWCC's per-iteration li/mulli/add/lbzx shape only when written
        // out). `changed` stays uninitialized on the no-claim path (retail
        // never materializes a 0 init; it is only read after a claim).
        int changed;
        u8 j = 0;
        if (base[j * 0x168] == 7) {
            u8* p = &base[j * 0x168];
            p[0] = 1;
            p[1] = index;
            changed = 1;
            goto claim_done;
        }
        j = 1;
        if (base[j * 0x168] == 7) {
            u8* p = &base[j * 0x168];
            p[0] = 1;
            p[1] = index;
            changed = 1;
            goto claim_done;
        }
        j = 2;
        if (base[j * 0x168] == 7) {
            u8* p = &base[j * 0x168];
            p[0] = 1;
            p[1] = index;
            changed = 1;
            goto claim_done;
        }
        j = 3;
        if (base[j * 0x168] == 7) {
            u8* p = &base[j * 0x168];
            p[0] = 1;
            p[1] = index;
            changed = 1;
            goto claim_done;
        }
        j = 4;
        if (base[j * 0x168] == 7) {
            u8* p = &base[j * 0x168];
            p[0] = 1;
            p[1] = index;
            changed = 1;
            goto claim_done;
        }
        j = 5;
        if (base[j * 0x168] == 7) {
            u8* p = &base[j * 0x168];
            p[0] = 1;
            p[1] = index;
            changed = 1;
            goto claim_done;
        }
        j = 6;
        if (base[j * 0x168] == 7) {
            u8* p = &base[j * 0x168];
            p[0] = 1;
            p[1] = index;
            changed = 1;
            goto claim_done;
        }
        j = 7;
        if (base[j * 0x168] == 7) {
            u8* p = &base[j * 0x168];
            p[0] = 1;
            p[1] = index;
            changed = 1;
        }
    claim_done:
        if (changed != 0 && base[0xb40] == 0) {
            func_801311B8(base);
        }
    }
    return 1;
}
// func_80130720 (us-801311f4): template-copy slot claim. Same
// gate/scan skeleton; the claim body sets slot flag 3 and copies four
// 0x34-byte blocks (src +0/0x34/0x68/0x9C -> slot +4/0x38/0x6C/0xA0) plus a
// word (src +0xD0 -> slot +0xD4) via the block-copy helper.
int func_80130720(u8* base, CUICfSrcCopyView* src) {
    int gateBusy;
    if (func_80293C10() != 0) {
        gateBusy = 1;
    } else if (func_8029A658() != 0) {
        gateBusy = 1;
    } else if (func_801B481C() != 0) {
        gateBusy = 1;
    } else if (func_80122450() != 0) {
        gateBusy = 1;
    } else {
        gateBusy = func_80124B78() != 0;
    }
    int flag;
    if (gateBusy != 0) {
        flag = 1;
    } else {
        for (u8 i = 0; i < 8; i++) {
            if (base[i * 0x168] != 7) {
                flag = 1;
                break;
            }
        }
    }
    if (flag == 0) {
        return 0;
    }
    int changed = 0;
    u8 j;
    for (j = 0; j < 8; j++) {
        if (base[j * 0x168] == 7) {
            CUICfSlotCopyView* slot =
                reinterpret_cast<CUICfSlotCopyView*>(base + j * 0x168);
            slot->field_0x00 = 3;
            changed = 1;
            func_8015704C(&slot->block1, &src->block1);
            func_8015704C(&slot->block2, &src->block2);
            func_8015704C(&slot->block3, &src->block3);
            func_8015704C(&slot->block4, &src->block4);
            slot->field_D4 = src->field_D0;
            break;
        }
    }
    if (changed != 0 && base[0xb40] == 0) {
        func_801311B8(base);
    }
    return 1;
}
// func_80130960 (us-80131434): 5-id slot claim. When the busy gates are
// clear and every init slot is free (state 7), fills the first free slot
// with flag 2 and the five u16 ids at +0xD8..+0xE0, then rebuilds the slot
// list unless the 0xb40 byte is set. Returns 1 when it claimed (or was
// busy), 0 when everything was already idle.
int func_80130960(u8* base, u16 a1, u16 a2, u16 a3, u16 a4, u16 a5) {
    // Gate chain: if/else-if form materializes the flag per branch (retail
    // emits li r0,1; b common per gate); a plain || chain would branch to a
    // shared flag-set instead (12B-spaced gates vs retail's 20B). The gate
    // result and the slot-scan busy flag are separate webs (retail colors the
    // gate value r0; one merged flag web would color both r3).
    int gateBusy;
    if (func_80293C10() != 0) {
        gateBusy = 1;
    } else if (func_8029A658() != 0) {
        gateBusy = 1;
    } else if (func_801B481C() != 0) {
        gateBusy = 1;
    } else if (func_80122450() != 0) {
        gateBusy = 1;
    } else {
        gateBusy = func_80124B78() != 0;
    }
    int flag;
    if (gateBusy != 0) {
        flag = 1;
    } else {
        // u8 counter: MWCC fully unrolls the 8-iteration scan keeping the
        // li/mulli index shape (retail); int would keep a tight loop instead.
        for (u8 i = 0; i < 8; i++) {
            if (base[i * 0x168] != 7) {
                flag = 1;
                break;
            }
        }
    }
    if (flag == 0) {
        return 0;
    } else {
        u8 j;
        int changed = 0;
        for (j = 0; j < 8; j++) {
            if (base[j * 0x168] == 7) {
                CUICfSlotIdWriteView* slot =
                    reinterpret_cast<CUICfSlotIdWriteView*>(base + j * 0x168);
                slot->field_0x00 = 2;
                changed = 1;
                slot->field_D8 = a1;
                slot->field_DA = a2;
                slot->field_DC = a3;
                slot->field_DE = a4;
                slot->field_E0 = a5;
                break;
            }
        }
        if (changed != 0 && base[0xb40] == 0) {
            func_801311B8(base);
        }
        return 1;
    }
}
// func_80130B74 (us-80131648): slot claim. When idle, fills the first free
// init slot (state 7) with flag 4, the passed id (u16 at +0xE2) and state
// (u8 at +0xE4), then rebuilds the slot list unless the 0xb40 byte is set.
int func_80130B74(u8* base, u16 id, u8 state) {
    int flag = func_80293C10() != 0 || func_8029A658() != 0 || func_801B481C() != 0 ||
               func_80122450() != 0 || func_80124B78() != 0;
    if (flag == 0) {
        u8 i = 0;
        do {
            if (base[i * 0x168] != 7) {
                flag = 1;
                break;
            }
            i++;
        } while (i < 8);
    }
    if (flag != 0) {
        u8 j = 0;
        int changed = 0;
        do {
            if (base[j * 0x168] == 7) {
                CUICfSlotWriteView* slot =
                    reinterpret_cast<CUICfSlotWriteView*>(base + j * 0x168);
                slot->field_0x00 = 4;
                slot->field_0xE2 = id;
                slot->field_0xE4 = state;
                changed = 1;
                break;
            }
            j++;
        } while (j < 8);
        if (changed != 0 && base[0xb40] == 0) {
            func_801311B8(base);
        }
        return 1;
    }
    return 0;
}
// func_80130D80 (us-80131854): single-arg slot claim that sprintf-formats
// the passed id into the slot's +0xE6 buffer. Same gate/scan skeleton as
// func_80130960; the claim body sets slot flag 5, formats the id, then sets
// changed (live across the sprintf, hence a saved register).
int func_80130D80(u8* base, u16 arg1) {
    int gateBusy;
    if (func_80293C10() != 0) {
        gateBusy = 1;
    } else if (func_8029A658() != 0) {
        gateBusy = 1;
    } else if (func_801B481C() != 0) {
        gateBusy = 1;
    } else if (func_80122450() != 0) {
        gateBusy = 1;
    } else {
        gateBusy = func_80124B78() != 0;
    }
    int flag;
    if (gateBusy != 0) {
        flag = 1;
    } else {
        for (u8 i = 0; i < 8; i++) {
            if (base[i * 0x168] != 7) {
                flag = 1;
                break;
            }
        }
    }
    if (flag == 0) {
        return 0;
    } else {
        int changed = 0;
        u8 j;
        for (j = 0; j < 8; j++) {
            if (base[j * 0x168] == 7) {
                CUICfSlotStrView* slot =
                    reinterpret_cast<CUICfSlotStrView*>(base + j * 0x168);
                slot->field_0x00 = 5;
                changed = 1;
                sprintf(slot->str, (const char*)lbl_eu_805000A8, arg1);
                break;
            }
        }
        if (changed != 0 && base[0xb40] == 0) {
            func_801311B8(base);
        }
        return 1;
    }
}
// func_80130F98 (us-80131a6c): two-arg slot claim that sprintf-formats both
// ids into the slot's +0xE6 and +0x126 buffers. Same gate/scan skeleton as
// func_80130D80; both sprintf calls share the pool-base format address (kept
// in a saved register across the calls).
int func_80130F98(u8* base, u16 a1, u16 a2) {
    int gateBusy;
    if (func_80293C10() != 0) {
        gateBusy = 1;
    } else if (func_8029A658() != 0) {
        gateBusy = 1;
    } else if (func_801B481C() != 0) {
        gateBusy = 1;
    } else if (func_80122450() != 0) {
        gateBusy = 1;
    } else {
        gateBusy = func_80124B78() != 0;
    }
    int flag;
    if (gateBusy != 0) {
        flag = 1;
    } else {
        for (u8 i = 0; i < 8; i++) {
            if (base[i * 0x168] != 7) {
                flag = 1;
                break;
            }
        }
    }
    if (flag == 0) {
        return 0;
    }
    int changed = 0;
    u8 j;
    for (j = 0; j < 8; j++) {
        if (base[j * 0x168] == 7) {
            CUICfSlotStrView* slot =
                reinterpret_cast<CUICfSlotStrView*>(base + j * 0x168);
            slot->field_0x00 = 6;
            changed = 1;
            sprintf(slot->str, (const char*)lbl_eu_805000A8, a1);
            sprintf(&slot->str[0x40], (const char*)lbl_eu_805000A8, a2);
            break;
        }
    }
    if (changed != 0 && base[0xb40] == 0) {
        func_801311B8(base);
    }
    return 1;
}
// ---------------------------------------------------------------------------
// func_801311B8 (us-80131c8c)
//
// Bubble-sort pass over the 8 init slots by flag byte (ascending; flag 7 =
// free sorts last). Each pass compares adjacent slots and, when out of
// order, swaps them through three stack templates (func_8015704C block
// copies + sprintf of the two id strings). Passes repeat until a pass makes
// no swap; the pass length shrinks by one each time (retail subfic limit).
// ---------------------------------------------------------------------------
extern "C" void func_801311B8(u8* base) {
    CUICfSwapSlot bufA;
    CUICfSwapSlot bufB;
    CUICfSwapSlot tpl;
    u8 start = 0;
    u8 didSwap;
    u8 i;

    do {
        didSwap = 0;
        for (i = 0; i < 7 - start; i++) {
            CUICfSwapSlot* slotI = reinterpret_cast<CUICfSwapSlot*>(base + i * 0x168);
            CUICfSwapSlot* slotJ = reinterpret_cast<CUICfSwapSlot*>(base + (i + 1) * 0x168);
            if (slotI->field_0 > slotJ->field_0) {
                didSwap = 1;

                // SWAP A: template <- slot i (3 blocks + tail, sprintf the ids).
                tpl.block1.unk04 = 0;
                tpl.block1.unk00 = 0;
                {
                    CUICfInitBlock* clear = &tpl.block2;
                    CUICfInitBlock* clearEnd = reinterpret_cast<CUICfInitBlock*>(&tpl.wordD4);
                    if (clear < clearEnd) {
                        u32 n = ((u8*)clearEnd - (u8*)clear + 0x33) / 0x34;
                        do {
                            clear->unk04 = 0;
                            clear->unk00 = 0;
                            ++clear;
                        } while (--n != 0);
                    }
                }
                tpl.wordD4 = 0;
                tpl.field_0 = slotI->field_0;
                tpl.field_1 = slotI->field_1;
                func_8015704C(&tpl.block1, &slotI->block1);
                func_8015704C(&tpl.block2, &slotI->block2);
                func_8015704C(&tpl.block3, &slotI->block3);
                func_8015704C(&tpl.block4, &slotI->block4);
                tpl.wordD4 = slotI->wordD4;
                tpl.ids[0] = slotI->ids[0];
                tpl.ids[1] = slotI->ids[1];
                tpl.ids[2] = slotI->ids[2];
                tpl.ids[3] = slotI->ids[3];
                tpl.ids[4] = slotI->ids[4];
                tpl.ids[5] = slotI->ids[5];
                tpl.byteE4 = slotI->byteE4;
                sprintf(tpl.strE6.b, lbl_eu_806621EC, slotI->strE6.b);
                sprintf(tpl.str126.b, lbl_eu_806621E8, slotI->str126.b);

                // SWAP B: buffer B <- slot j (full copy, sprintf the ids).
                bufB.block1.unk04 = 0;
                bufB.block1.unk00 = 0;
                {
                    CUICfInitBlock* clear = &bufB.block2;
                    CUICfInitBlock* clearEnd = reinterpret_cast<CUICfInitBlock*>(&bufB.wordD4);
                    if (clear < clearEnd) {
                        u32 n = ((u8*)clearEnd - (u8*)clear + 0x33) / 0x34;
                        do {
                            clear->unk04 = 0;
                            clear->unk00 = 0;
                            ++clear;
                        } while (--n != 0);
                    }
                }
                bufB.wordD4 = 0;
                bufB.field_0 = slotJ->field_0;
                bufB.field_1 = slotJ->field_1;
                func_8015704C(&bufB.block1, &slotJ->block1);
                func_8015704C(&bufB.block2, &slotJ->block2);
                func_8015704C(&bufB.block3, &slotJ->block3);
                func_8015704C(&bufB.block4, &slotJ->block4);
                bufB.wordD4 = slotJ->wordD4;
                bufB.ids[0] = slotJ->ids[0];
                bufB.ids[1] = slotJ->ids[1];
                bufB.ids[2] = slotJ->ids[2];
                bufB.ids[3] = slotJ->ids[3];
                bufB.ids[4] = slotJ->ids[4];
                bufB.ids[5] = slotJ->ids[5];
                bufB.byteE4 = slotJ->byteE4;
                sprintf(bufB.strE6.b, lbl_eu_806621EC, slotJ->strE6.b);
                sprintf(bufB.str126.b, lbl_eu_806621E8, slotJ->str126.b);

                // write-back A: slot i <- buffer B (raw copy, no sprintf).
                slotI->field_0 = bufB.field_0;
                slotI->field_1 = bufB.field_1;
                func_8015704C(&slotI->block1, &bufB.block1);
                func_8015704C(&slotI->block2, &bufB.block2);
                func_8015704C(&slotI->block3, &bufB.block3);
                func_8015704C(&slotI->block4, &bufB.block4);
                slotI->wordD4 = bufB.wordD4;
                slotI->ids[0] = bufB.ids[0];
                slotI->ids[1] = bufB.ids[1];
                slotI->ids[2] = bufB.ids[2];
                slotI->ids[3] = bufB.ids[3];
                slotI->ids[4] = bufB.ids[4];
                slotI->ids[5] = bufB.ids[5];
                slotI->byteE4 = bufB.byteE4;
                slotI->strE6 = bufB.strE6;
                slotI->str126 = bufB.str126;

                // SWAP C: buffer A <- template (block copies, sprintf the ids).
                bufA.block1.unk04 = 0;
                bufA.block1.unk00 = 0;
                {
                    CUICfInitBlock* clear = &bufA.block2;
                    CUICfInitBlock* clearEnd = reinterpret_cast<CUICfInitBlock*>(&bufA.wordD4);
                    if (clear < clearEnd) {
                        u32 n = ((u8*)clearEnd - (u8*)clear + 0x33) / 0x34;
                        do {
                            clear->unk04 = 0;
                            clear->unk00 = 0;
                            ++clear;
                        } while (--n != 0);
                    }
                }
                bufA.wordD4 = 0;
                bufA.field_0 = tpl.field_0;
                bufA.field_1 = tpl.field_1;
                func_8015704C(&bufA.block1, &tpl.block1);
                func_8015704C(&bufA.block2, &tpl.block2);
                func_8015704C(&bufA.block3, &tpl.block3);
                func_8015704C(&bufA.block4, &tpl.block4);
                bufA.wordD4 = tpl.wordD4;
                bufA.ids[0] = tpl.ids[0];
                bufA.ids[1] = tpl.ids[1];
                bufA.ids[2] = tpl.ids[2];
                bufA.ids[3] = tpl.ids[3];
                bufA.ids[4] = tpl.ids[4];
                bufA.ids[5] = tpl.ids[5];
                bufA.byteE4 = tpl.byteE4;
                sprintf(bufA.strE6.b, lbl_eu_806621EC, tpl.strE6.b);
                sprintf(bufA.str126.b, lbl_eu_806621E8, tpl.str126.b);

                // write-back C: slot j <- buffer A (raw copy, no sprintf).
                slotJ->field_0 = bufA.field_0;
                slotJ->field_1 = bufA.field_1;
                func_8015704C(&slotJ->block1, &bufA.block1);
                func_8015704C(&slotJ->block2, &bufA.block2);
                func_8015704C(&slotJ->block3, &bufA.block3);
                func_8015704C(&slotJ->block4, &bufA.block4);
                slotJ->wordD4 = bufA.wordD4;
                slotJ->ids[0] = bufA.ids[0];
                slotJ->ids[1] = bufA.ids[1];
                slotJ->ids[2] = bufA.ids[2];
                slotJ->ids[3] = bufA.ids[3];
                slotJ->ids[4] = bufA.ids[4];
                slotJ->ids[5] = bufA.ids[5];
                slotJ->byteE4 = bufA.byteE4;
                slotJ->strE6 = bufA.strE6;
                slotJ->str126 = bufA.str126;
            }
        }
        if (didSwap == 0) {
            break;
        }
        start++;
    } while (start < 7);
}
// func_80131820 (us-801322f4)
//
// Same bubble-sort pass as func_801311B8, but sorting by in-use status:
// a slot is swapped before its neighbour when it is in use (flag != 7) and
// the neighbour is free (flag == 7), so freed slots sink to the end. The
// comparison booleanizes each flag's != 7 (retail subfic/subi/or/srwi).
// ---------------------------------------------------------------------------
extern "C" void func_80131820(u8* base) {
    CUICfSwapSlot bufA;
    CUICfSwapSlot bufB;
    CUICfSwapSlot tpl;
    u8 start = 0;
    u8 didSwap;
    u8 i;

    do {
        didSwap = 0;
        for (i = 0; i < 7 - start; i++) {
            CUICfSwapSlot* slotI = reinterpret_cast<CUICfSwapSlot*>(base + i * 0x168);
            CUICfSwapSlot* slotJ = reinterpret_cast<CUICfSwapSlot*>(base + (i + 1) * 0x168);
            if ((slotI->field_0 != 7) > (slotJ->field_0 != 7)) {
                didSwap = 1;

                // SWAP A: template <- slot i (3 blocks + tail, sprintf the ids).
                tpl.block1.unk04 = 0;
                tpl.block1.unk00 = 0;
                {
                    CUICfInitBlock* clear = &tpl.block2;
                    CUICfInitBlock* clearEnd = reinterpret_cast<CUICfInitBlock*>(&tpl.wordD4);
                    if (clear < clearEnd) {
                        u32 n = ((u8*)clearEnd - (u8*)clear + 0x33) / 0x34;
                        do {
                            clear->unk04 = 0;
                            clear->unk00 = 0;
                            ++clear;
                        } while (--n != 0);
                    }
                }
                tpl.wordD4 = 0;
                tpl.field_0 = slotI->field_0;
                tpl.field_1 = slotI->field_1;
                func_8015704C(&tpl.block1, &slotI->block1);
                func_8015704C(&tpl.block2, &slotI->block2);
                func_8015704C(&tpl.block3, &slotI->block3);
                func_8015704C(&tpl.block4, &slotI->block4);
                tpl.wordD4 = slotI->wordD4;
                tpl.ids[0] = slotI->ids[0];
                tpl.ids[1] = slotI->ids[1];
                tpl.ids[2] = slotI->ids[2];
                tpl.ids[3] = slotI->ids[3];
                tpl.ids[4] = slotI->ids[4];
                tpl.ids[5] = slotI->ids[5];
                tpl.byteE4 = slotI->byteE4;
                sprintf(tpl.strE6.b, lbl_eu_806621EC, slotI->strE6.b);
                sprintf(tpl.str126.b, lbl_eu_806621E8, slotI->str126.b);

                // SWAP B: buffer B <- slot j (full copy, sprintf the ids).
                bufB.block1.unk04 = 0;
                bufB.block1.unk00 = 0;
                {
                    CUICfInitBlock* clear = &bufB.block2;
                    CUICfInitBlock* clearEnd = reinterpret_cast<CUICfInitBlock*>(&bufB.wordD4);
                    if (clear < clearEnd) {
                        u32 n = ((u8*)clearEnd - (u8*)clear + 0x33) / 0x34;
                        do {
                            clear->unk04 = 0;
                            clear->unk00 = 0;
                            ++clear;
                        } while (--n != 0);
                    }
                }
                bufB.wordD4 = 0;
                bufB.field_0 = slotJ->field_0;
                bufB.field_1 = slotJ->field_1;
                func_8015704C(&bufB.block1, &slotJ->block1);
                func_8015704C(&bufB.block2, &slotJ->block2);
                func_8015704C(&bufB.block3, &slotJ->block3);
                func_8015704C(&bufB.block4, &slotJ->block4);
                bufB.wordD4 = slotJ->wordD4;
                bufB.ids[0] = slotJ->ids[0];
                bufB.ids[1] = slotJ->ids[1];
                bufB.ids[2] = slotJ->ids[2];
                bufB.ids[3] = slotJ->ids[3];
                bufB.ids[4] = slotJ->ids[4];
                bufB.ids[5] = slotJ->ids[5];
                bufB.byteE4 = slotJ->byteE4;
                sprintf(bufB.strE6.b, lbl_eu_806621EC, slotJ->strE6.b);
                sprintf(bufB.str126.b, lbl_eu_806621E8, slotJ->str126.b);

                // write-back A: slot i <- buffer B (raw copy, no sprintf).
                slotI->field_0 = bufB.field_0;
                slotI->field_1 = bufB.field_1;
                func_8015704C(&slotI->block1, &bufB.block1);
                func_8015704C(&slotI->block2, &bufB.block2);
                func_8015704C(&slotI->block3, &bufB.block3);
                func_8015704C(&slotI->block4, &bufB.block4);
                slotI->wordD4 = bufB.wordD4;
                slotI->ids[0] = bufB.ids[0];
                slotI->ids[1] = bufB.ids[1];
                slotI->ids[2] = bufB.ids[2];
                slotI->ids[3] = bufB.ids[3];
                slotI->ids[4] = bufB.ids[4];
                slotI->ids[5] = bufB.ids[5];
                slotI->byteE4 = bufB.byteE4;
                slotI->strE6 = bufB.strE6;
                slotI->str126 = bufB.str126;

                // SWAP C: buffer A <- template (block copies, sprintf the ids).
                bufA.block1.unk04 = 0;
                bufA.block1.unk00 = 0;
                {
                    CUICfInitBlock* clear = &bufA.block2;
                    CUICfInitBlock* clearEnd = reinterpret_cast<CUICfInitBlock*>(&bufA.wordD4);
                    if (clear < clearEnd) {
                        u32 n = ((u8*)clearEnd - (u8*)clear + 0x33) / 0x34;
                        do {
                            clear->unk04 = 0;
                            clear->unk00 = 0;
                            ++clear;
                        } while (--n != 0);
                    }
                }
                bufA.wordD4 = 0;
                bufA.field_0 = tpl.field_0;
                bufA.field_1 = tpl.field_1;
                func_8015704C(&bufA.block1, &tpl.block1);
                func_8015704C(&bufA.block2, &tpl.block2);
                func_8015704C(&bufA.block3, &tpl.block3);
                func_8015704C(&bufA.block4, &tpl.block4);
                bufA.wordD4 = tpl.wordD4;
                bufA.ids[0] = tpl.ids[0];
                bufA.ids[1] = tpl.ids[1];
                bufA.ids[2] = tpl.ids[2];
                bufA.ids[3] = tpl.ids[3];
                bufA.ids[4] = tpl.ids[4];
                bufA.ids[5] = tpl.ids[5];
                bufA.byteE4 = tpl.byteE4;
                sprintf(bufA.strE6.b, lbl_eu_806621EC, tpl.strE6.b);
                sprintf(bufA.str126.b, lbl_eu_806621E8, tpl.str126.b);

                // write-back C: slot j <- buffer A (raw copy, no sprintf).
                slotJ->field_0 = bufA.field_0;
                slotJ->field_1 = bufA.field_1;
                func_8015704C(&slotJ->block1, &bufA.block1);
                func_8015704C(&slotJ->block2, &bufA.block2);
                func_8015704C(&slotJ->block3, &bufA.block3);
                func_8015704C(&slotJ->block4, &bufA.block4);
                slotJ->wordD4 = bufA.wordD4;
                slotJ->ids[0] = bufA.ids[0];
                slotJ->ids[1] = bufA.ids[1];
                slotJ->ids[2] = bufA.ids[2];
                slotJ->ids[3] = bufA.ids[3];
                slotJ->ids[4] = bufA.ids[4];
                slotJ->ids[5] = bufA.ids[5];
                slotJ->byteE4 = bufA.byteE4;
                slotJ->strE6 = bufA.strE6;
                slotJ->str126 = bufA.str126;
            }
        }
        if (didSwap == 0) {
            break;
        }
        start++;
    } while (start < 7);
}
// ---------------------------------------------------------------------------
// CUICfManager::CUICfManager (us-8013297c)
//
// The CTTask<CUICfManager> base ctor (inline) emits the CProcess ctor call
// plus the null move/draw PTMF copies (0x3c-0x50) and the vtable store. The
// body then finishes the event queue (double vtable store: the reslist base
// first, then the derived queue vtable), zero-inits the 8 init slots (the
// 3-block clear is the retail divwu-counted countdown), allocates the 0xC-node
// pool, and re-installs the move/draw PTMFs from lbl_eu_8052E288.
// ---------------------------------------------------------------------------
CUICfManager::CUICfManager(CScnNw4r* pScene, mtl::ALLOC_HANDLE mHandle) {
    mFileHandle = NULL;                       // 0x114
    unk118 = (int)mHandle;                    // 0x118
    unk11C = (u32)pScene;                     // 0x11C
    unk120 = 0;                               // 0x120

    CUICfEventQueue* q = reinterpret_cast<CUICfEventQueue*>(&unk124);
    q->vtable = lbl_eu_8052E3B0;
    q->nodes = NULL;                          // 0x138
    q->count = 0;                             // 0x13C
    q->ownsList = 0;                          // 0x140
    q->head = &q->startNode;                  // 0x128
    q->startNode.next = &q->startNode;
    q->startNode.prev = &q->startNode;
    q->vtable = lbl_eu_8052E398;

    unk144 = NULL;                            // 0x144
    mInitSlots[0].unk00[0] = 0;               // 0x148
    mInitSlots[0].unk00[1] = 0;               // 0x149

    for (u32 i = 0; i < 8; i++) {
        CUICfInitSlot& s = mInitSlots[i];
        // unk08's word pair, then the unk3C..unkA4 block clear (retail divwu
        // countdown over the 0x34 stride).
        s.unk08.unk04 = 0;
        s.unk08.unk00 = 0;
        CUICfInitBlock* clear = &s.unk3C;
        CUICfInitBlock* clearEnd = reinterpret_cast<CUICfInitBlock*>(&s.unkD8);
        if (clear < clearEnd) {
            u32 n = ((u8*)clearEnd - (u8*)clear + 0x33) / 0x34;
            do {
                clear->unk04 = 0;
                clear->unk00 = 0;
                clear++;
            } while (--n != 0);
        }
        s.unk00[0] = 7;
        s.unk00[1] = 0;
        s.unkD8.unk00 = 0;                    // +0xD4 word, +0xD8..
        s.unkD8.mid = CUICfInitTailChunk8();
        s.unkD8.unk0C = 0;
        s.unkD8.unk0E = 0;
    }

    unkC88[0] = 0;                            // 0xC8C
    mFlags = 0;                               // 0xC90
    unk138 = (CUICfListNode*)mtl::MemManager::allocate_array(
        0x90, func_80496004((CScn*)pScene));
    unk13C = 0xc;
    for (u32 i = 0; i < 0xc; i++) {
        unk138[i].next = NULL;
    }
}
void func_801336E4(){}
// func_80133770 (us-80134244): teardown/create of the base menu for Move's
// mFlags bit 0x1. Gates on the singleton + resource accessor, probes the
// busy chain (with the 0x20-resource / state-2 retry sound), then creates
// the menu via func_800FF6BC and pushes it onto the event queue (same
// reslist shape as the func_80133A08 family).
u32 func_80133770() {
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
    if (inst->field_0x5C == NULL) {
        inst->field_0xC90 = (u16)(inst->field_0xC90 | 0x1);
        return 0;
    }
    inst->field_0xC90 = (u16)(inst->field_0xC90 & 0xfffe);
    {
        int st = func_801359AC();
        if (st != 0) {
            if ((lbl_eu_80663E24 & 0x2) == 0) {
                if (func_8009CF8C(0x20) > 4 && st == 2) {
                    func_80138078(5);
                }
            }
            return 0;
        }
    }
    inst = (CUICfManagerCreateView*)lbl_eu_80664054;
    {
        u32 tempRet = (u32)func_800FF6BC((u8*)inst->field_0x144, inst->field_0x11C);
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
// func_80135708 (us-801361dc): busy/slot-state probe. Returns 1 when any
// menu-system busy gate is set or any init slot is not in state 7 (free).
int func_80135708() {
    CUICfManager* inst = (CUICfManager*)lbl_eu_80664054;
    if (inst == NULL) {
        return 0;
    }
    u8* base = (u8*)inst;
    base += 0x14c;
    if (func_80293C10() != 0 || func_8029A658() != 0 || func_801B481C() != 0 ||
        func_80122450() != 0 || func_80124B78() != 0) {
        return 1;
    }
    u8 i = 0;
    do {
        if (base[i * 0x168] != 7) {
            return 1;
        }
        i++;
    } while (i < 8);
    return 0;
}
// Minimal cf::CBattleManager view (the full CBattleManager.hpp pulls the
// object/chain headers; only getInstance is needed for func_801359AC).
namespace cf {
class CBattleManager {
public:
    static CBattleManager* getInstance();
};
}

// Float-returning vtable slot (vtable +0x128) of the player wrapper.
typedef float (*CUICfVFloatFn)(CUICfPlayerView*);

// func_801359AC (us-80136480): busy/slot-state probe. Returns 1 when any
// menu-system busy gate is set (sound/state gates first), 2 when the battle
// list, an enum-list fill, the player pose/state, the global settings bit,
// or any in-use slot (flag == 4) blocks the menus, else 0.
extern "C" int func_801359AC() {
    int count;
    int slotUsed;
    CUICfEnumListHolder holder;
    CUICfEnumList620* enumList;
    CUICfPlayerView* px;
    CUICfObj3F60View* battleObj;
    CUICfManager* inst;

    if (func_8008585C__Q22cf13CfGameManagerFv() != 0) {
        return 1;
    }
    if (func_8009CF8C(0x3508) != 0 && func_8009CF8C(0x20) <= 4) {
        return 1;
    }
    {
        cf::CBattleManager* bm = cf::CBattleManager::getInstance();
        CBattleListHead* blh = reinterpret_cast<CBattleListHead*>(bm);
        CUICfListLink* head = blh->head;
        count = 0;
        for (CUICfListLink* node = head->next; node != head; node = node->next) {
            count++;
        }
        if (count != 0) {
            return 2;
        }
    }
    func_80043E08(&holder, 0x20, 0x800);
    enumList = reinterpret_cast<CUICfEnumList620*>(func_80043F18(&holder));
    __ct__800FC32C(reinterpret_cast<u8*>(enumList), 3, 4, 0);
    enumList = reinterpret_cast<CUICfEnumList620*>(func_80043F18(&holder));
    if (enumList->count == 0) {
        __dt__80043E88(&holder, -1);
        return 2;
    }
    __dt__80043E88(&holder, -1);

    {
        u32 g24a = lbl_eu_80663E24;
        u32 g24b = *(volatile u32*)&lbl_eu_80663E24;
        if ((g24a & 0xafe40000u) != 0) {
            return 1;
        }
        if ((g24b & 0x4u) != 0) {
            return 1;
        }
    }
    if (func_80084BF4__Q22cf13CfGameManagerFv() != 0) {
        return 1;
    }
    {
        cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
        px = reinterpret_cast<CUICfPlayerView*>(player);
        if (player != NULL) {
            px = reinterpret_cast<CUICfPlayerView*>(reinterpret_cast<u8*>(player) - 0x3E9C);
        }
        if (px != NULL) {
            u8** vt = *reinterpret_cast<u8***>(px);
            f32 v = reinterpret_cast<CUICfVFloatFn>(vt[0x128 / 4])(px);
            if (v == lbl_eu_806672D0) {
                return 2;
            }
            if (px->field_3F60 != NULL) {
                battleObj = reinterpret_cast<CUICfObj3F60View*>(px->field_3F60);
                if ((battleObj->field_530 & 1) != 0) {
                    return 2;
                }
                {
                    u32 w4ec = battleObj->field_4EC;
                    if ((w4ec & (1 << 8)) != 0) {
                        return 2;
                    }
                    if ((w4ec & (1 << 15)) == 0 && (w4ec & (1 << 1)) == 0) {
                        return 2;
                    }
                }
            }
        }
    }
    if ((lbl_eu_80663E24 & 0x00000200u) != 0) {
        return 1;
    }
    if (func_8009CF8C(0x334b) == 0) {
        return 1;
    }
    if (func_8029A658() != 0) {
        return 1;
    }
    if (func_8011CD5C() != 0) {
        return 1;
    }
    if (CMenuArtsSelect_isCreated() != 0) {
        return 1;
    }
    if (func_80122450() != 0) {
        return 1;
    }
    {
        CUICfGlobalSettings* g = getUnk80664658();
        if ((g->field_214 & (1 << 19)) != 0) {
            return 2;
        }
    }
    inst = (CUICfManager*)lbl_eu_80664054;
    if (inst == NULL) {
        slotUsed = 0;
    } else {
        CUICfManagerSlotView* mv = reinterpret_cast<CUICfManagerSlotView*>(inst);
        u8 i;
        for (i = 0; i < 8; i++) {
            if (mv->slots[i].flag == 4) {
                slotUsed = 1;
                break;
            }
        }
    }
    if (slotUsed != 0) {
        return 2;
    }
    return 0;
}
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

// ---------------------------------------------------------------------------
// CUICfManager::OnFileEvent (us-80133c50)
//
// The retail symbol is the no-arg member `OnFileEvent__12CUICfManagerFv` but
// the body reads r4 as the CEventFile* from the IWorkEvent dispatcher (same
// hidden-arg ABI as func_80133324; the func_80135FBC thunk passes r4
// through). The body is emitted under the extern "C" free function declared
// in the .hpp so the exact Fv symbol name is produced; the class's member
// `void OnFileEvent()` stays declared-only and the thunk's bl binds here.
// ---------------------------------------------------------------------------
extern "C" bool OnFileEvent__12CUICfManagerFv(CUICfManager* self, CEventFile* evt) {
    CFileHandle* handle = self->mFileHandle;
    if (handle != evt->mFileHandle) {
        return false;
    }
    u8* fileData = static_cast<u8*>(handle->getData());

    nw4r::lyt::ArcResourceAccessor* accessor = CLibLayout::createArcResourceAccessor();
    self->mArcResourceAccessor = accessor;
    accessor->Attach(fileData, reinterpret_cast<const char*>(&lbl_eu_805000A8[3]));

    u8* fontData = static_cast<u8*>(accessor->GetResource(
        0x666e7461, reinterpret_cast<const char*>(&lbl_eu_805000A8[7]), NULL));
    u32 fontBytes = nw4r::ut::PackedFont::GetRequireBufferSize(fontData, lbl_8066DCF8, lbl_eu_806672C8);
    u8* fontBuf = static_cast<u8*>(
        mtl::MemManager::allocate_head(CDevice::getDevSys2Handle(), fontBytes, 0x20));
    self->mPackedFont60.Construct(fontBuf, fontBytes, fontData, lbl_8066DCF8);

    fontData = static_cast<u8*>(accessor->GetResource(
        0x666e7461, reinterpret_cast<const char*>(&lbl_eu_805000A8[0x1b]), NULL));
    fontBytes = nw4r::ut::PackedFont::GetRequireBufferSize(fontData, lbl_8066DCF8, lbl_eu_806672C8);
    fontBuf = static_cast<u8*>(
        mtl::MemManager::allocate_head(CDevice::getDevSys2Handle(), fontBytes, 0x20));
    self->mPackedFont9C.Construct(fontBuf, fontBytes, fontData, lbl_8066DCF8);

    fontData = static_cast<u8*>(accessor->GetResource(
        0x666e7461, reinterpret_cast<const char*>(&lbl_eu_805000A8[0x2f]), NULL));
    fontBytes = nw4r::ut::PackedFont::GetRequireBufferSize(fontData, lbl_8066DCF8, lbl_eu_806672C8);
    fontBuf = static_cast<u8*>(
        mtl::MemManager::allocate_head(CDevice::getDevSys2Handle(), fontBytes, 0x20));
    self->mPackedFontD8.Construct(fontBuf, fontBytes, fontData, lbl_8066DCF8);

    self->mFileHandle = NULL;
    cf::IFlagEvent* flagEvent = self;
    func_8009D0B4();
    func_8009D414(flagEvent);
    return true;
}

void CUICfManager::func_80135FBC() {
    // retail: subi r3,r3,0x54; b OnFileEvent__12CUICfManagerFv — secondary-subobject thunk
    reinterpret_cast<CUICfManager*>(reinterpret_cast<char*>(this) - 0x54)->OnFileEvent();
}

// retail: lwz r3,lbl_eu_80664054; cmpi r3,0; beq -> 0; addi r3,r3,0x9C
// returns the process sub-object at +0x9C, or null when the process is absent.
extern "C" void* func_801355BC() {
    if (lbl_eu_80664054 == 0) return 0;
    return (u8*)lbl_eu_80664054 + 0x9C;
}
