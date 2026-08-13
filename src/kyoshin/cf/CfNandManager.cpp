// Auto-scaffolded catalog TU for kyoshin/cf/CfNandManager
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfNandManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CSaveLoad.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <revolution/os/OSCrc.h>

// Minimal view of cf::CBattleManager (the full CBattleManager.hpp pulls
// CSuddenCommu.hpp, which fails to compile in this TU's include chain). The
// party snapshot only calls getInstance() and reads the +0x194 word.
namespace cf {
class CBattleManager {
public:
    static CBattleManager* getInstance();
};
}

// forward declarations for scaffold thunk references
void __dt__Q22cf13CfNandManagerFv(void*);
void cbRenderBefore__Q22cf13CfNandManagerFv(void*);

// Constructor (retail flat symbol __ct__cf_CfNandManager). Takes the scene
// pointer create() forwards in r4, but the body ignores it (the retail ctor
// recomputes its own end bound for the memset loops). Stub body only;
// noinline so create() keeps the out-of-line call + live scene arg.
extern "C" __declspec(noinline) void* __ct__cf_CfNandManager(void* self, CScn* pScene) {
    (void)pScene;
    return self;
}

// Destructor of the CfNandEventQueue member embedded in CfNandManager at +0x60
// (retail placeholder name __dt__8023BFCC): zeroes the queue head/tail
// counters, then frees the block when the deleting flag is set.
extern "C" void* __dt__8023BFCC(CfNandEventQueue* self, int dealloc_flag) {
    if (self != nullptr) {
        self->mTail = 0;
        self->mHead = 0;
        if (dealloc_flag > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

cf::CfNandManager::~CfNandManager() {
    // Restore the three subobject vtables (retail stores them explicitly in
    // the ctor and dtor; MWCC emits no vtable stores for this class).
    u32 vtable = reinterpret_cast<u32>(lbl_eu_80536BBC);
    u32* slots = reinterpret_cast<u32*>(this);
    slots[4] = vtable;                   // +0x10 CTTask vtable
    mIWorkEventVtable = vtable + 0x24;   // +0x54 IWorkEvent vtable
    slots[0x16] = vtable + 0xac;         // +0x58 IScnRender vtable

    // Drop the D80 scene render callback while the singleton is live.
    CScn* scene = (CScn*)CfRes_getD80Flag();
    if (scene != 0) {
        scene->removeRenderCB(lbl_eu_80664768);
    }
    // Release the shared NAND resource buffer.
    if (lbl_eu_8066477C != 0) {
        mtl::MemManager::deallocate(lbl_eu_8066477C);
        lbl_eu_8066477C = 0;
    }
    // Clear the NAND state globals; the embedded event queue is torn down
    // automatically (inline ~CfNandEventQueue, null-checked member call).
    lbl_eu_80664768 = 0;
    lbl_eu_8066476C = 0;
    lbl_eu_8066476D = 0;
    lbl_eu_80664770 = 0;
    lbl_eu_80664772 = 0;
    lbl_eu_80664774 = 0;
}

void Init__Q22cf13CfNandManagerFv(void) {
    extern void func_804DA1B4(void*);
    func_804DA1B4(&lbl_eu_8065FD00);
}

void cf::CfNandManager::Term() {
    // Free the heap buffer, then clear the singleton pointer.
    if (field_0x5C != nullptr) {
        __dl__FPv(field_0x5C);
        field_0x5C = nullptr;
    }
    lbl_eu_80664768 = nullptr;
}

void cf::CfNandManager::Move() {
    // Skip the NAND completion pump while the global mode-flag bit is set.
    if (lbl_eu_80663E28 & 0x200000) {
        return;
    }
    func_804DA1CC(&lbl_eu_8065FD00);
    // Dispatch the +0x188 move hook through __ptmf_test/__ptmf_scall when set.
    if (__ptmf_test(&mMoveHook)) {
        (this->*mMoveHook)();
    }
}

extern char lbl_eu_80576CC0[];
extern "C" void* func_8023C1B4() { return lbl_eu_80576CC0; }

// Returns 1 while the NAND request queue is busy (head != tail) or the async
// flag is set; the result variable shape reproduces the retail li r3,0 / beqlr.
u32 func_8023C1C0() {
    u32 result = 0;
    if (lbl_eu_8065FD00.mHead != lbl_eu_8065FD00.mTail || lbl_eu_8066476C != 0) {
        result = 1;
    }
    return result;
}

// Target us-8023e320: build a 3-member party snapshot (player info + scale
// via the player vtable, static fallback data for missing slots) and run
// OSCalcCRC16 over the whole struct.
void func_8023C1F0(CfNandPartySnapshot* snapshot) {
    snapshot->field_30 = 0;
    if (cf::CBattleManager::getInstance() != 0) {
        snapshot->field_30 = ((CfNandBmView*)cf::CBattleManager::getInstance())->field_194;
    }
    for (s32 i = 0; i < 3; i++) {
        CfNandPlayerVt* player = (CfNandPlayerVt*)cf::CfGameManager::getPlayer(i);
        CfNandPartyEntry* entry = &snapshot->mEntry[i];
        if (player != 0) {
            CfNandPartyInfo* info = player->vf0AC();
            entry->field_00 = info->field_00;
            entry->field_04 = info->field_04;
            entry->field_08 = info->field_08;
            entry->field_0C = player->vf0D8();
        } else {
            entry->field_00 = lbl_eu_8057164C[0];
            entry->field_04 = lbl_eu_8057164C[1];
            entry->field_08 = lbl_eu_8057164C[2];
            entry->field_0C = lbl_eu_80663E54;
        }
    }
    OSCalcCRC16((const u8*)snapshot, 0x34);
}

void func_8023C2E4(){}

void func_8023C68C(){}

void func_8023C7C4(){}

void func_8023C93C(){}

void func_8023CD9C(){}

void* func_8023D3D8() { return nullptr; }

// NAND teardown: guards on the singleton's heap buffer. The `buf ? buf : 0`
// self-assignment reproduces the retail materialized-bool diamond (the buffer
// word is re-tested with its own cr0 before the main path). `zero` stays live
// across the __dl__ call, forcing the retail r31 save/reuse.
void* __dt__8023E448() {
    u8* buf = lbl_eu_80664768->field_0x5C;
    buf = buf != 0 ? buf : 0;
    if (buf == 0) {
        return 0;
    }
    u32 zero = 0;
    lbl_eu_80664772 = 1;
    void* result = func_8023D3D8();
    lbl_eu_80664772 = zero;
    if (lbl_eu_80664768->field_0x5C != 0) {
        __dl__FPv(lbl_eu_80664768->field_0x5C);
        lbl_eu_80664768->field_0x5C = (u8*)zero;
    }
    return result;
}

// OnFileEvent body (retail Fv symbol carries a hidden CEventFile* in r4: the
// IWorkEvent dispatcher calls the OnFileEvent vtable slot with this_sub in r3
// and the file event in r4; the OnFileEvent adjustor thunk rewrites r3, so the
// file is the only live argument here). The dummy first parameter keeps the
// file in r4 without touching the verbatim retail symbol name.
extern "C" u32 func_8023E4D4__Q22cf13CfNandManagerFv(void* unused, CEventFile* file) {
    lbl_eu_8066476C = 0;
    // Event types: 1 = load finished (kick the game manager), 4 = teardown.
    if (file->unk0 == 1 && file->field_14 != 0) {
        func_80084F50__Q22cf13CfGameManagerFv();
    } else if (file->unk0 == 4) {
        if (lbl_eu_80664768->field_0x5C != 0) {
            __dl__FPv(lbl_eu_80664768->field_0x5C);
            lbl_eu_80664768->field_0x5C = 0;
        }
    }
    return 1;
}

void func_8023E544(){}

extern "C" __declspec(noinline) void __dt__8023E63C(u8* self) {}

// Target us-80240c00: event-status dispatch for the NAND manager. Maps the
// event type to a status word written through *out. The r5 out-param is not
// used by this function (retail callers pass a byte slot in r5 and the word
// slot in r6). The field_180 flag bits (bit0/bit29) report 1, else 2. The
// case set covers 3..0x26 densely so MWCC emits the retail jump table
// (jumptable_eu_80536B2C).
void func_8023EABC(cf::CfNandManager* self, u32 type, u8* status, u32* out) {
    switch (type) {
    case 0x10:
    case 0x21:
        *out = ((self->field_180 & 1) || (self->field_180 & 0x20000000)) ? 1 : 2;
        break;
    case 0x1:
    case 0x2:
        *out = 3;
        break;
    case 0x3:
    case 0x8:
    case 0x9:
    case 0xA:
    case 0xE:
    case 0xF:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
    case 0x15:
    case 0x16:
    case 0x17:
    case 0x18:
    case 0x19:
    case 0x1A:
    case 0x1B:
    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x1F:
    case 0x20:
    case 0x22:
    case 0x23:
    case 0x24:
    case 0x25:
        break; // do nothing
    case 0x4:
    case 0x5:
    case 0xB:
    case 0xC:
    case 0xD:
    case 0x26:
        *out = ((self->field_180 & 1) || (self->field_180 & 0x20000000)) ? 1 : 2;
        break;
    case 0x6:
    case 0x7:
        *out = 4;
        break;
    }
}

extern "C" __declspec(noinline) void func_8023EB78(u8* self) {}

extern "C" void func_8023F288(u8* self) {
    // Retail materializes `busy = (head != tail) || (flag != 0)` into r5
    // (li r5,0; conditional li r5,1; cmpi r5,0) and skips the teardown when
    // busy — the ||-materialized inverse of the guard, not inline &&.
    bool busy = (lbl_eu_8065FD00.mHead != lbl_eu_8065FD00.mTail) ||
                (lbl_eu_8066476C != 0);
    if (!busy) {
        __dt__8023E63C(self);
        func_8023EB78(self);
    }
}

// Target us-802414a4: push two events {0, 2, 0, 0x20, 0, 0} and
// {cb, 0x28000, 0, 6, value, 0} onto the manager ring. Always returns 1.
u32 func_8023F2F4(u32 value, void (*cb)(u32, u32, u32, u32)) {
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = 0;
        entry->mTag = 2;
        entry->mField8 = 0;
        entry->mFieldC = 0x20;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = (u32)cb;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 6;
        entry->mFieldE = (u8)value;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    return 1;
}

void func_8023F3C0(){}

// Target us-8024177c: push two {word, tag, 0, fieldC, 0, 0} events onto the
// manager ring (tag 0x10000/fieldC 0x21, then tag 0/fieldC 0xd), each gated
// on the singleton being live. Always returns 1.
u32 func_8023F5CC(u32 word) {
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0x10000;
        entry->mField8 = 0;
        entry->mFieldC = 0x21;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0;
        entry->mField8 = 0;
        entry->mFieldC = 0xd;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    return 1;
}

void func_8023F690(){}

void func_8023F860(){}

// Target us-80241c14: zero a 0xA000-byte caller buffer, then push
// {word, 0xA000, buf, 3, byte, 0} onto the manager ring. Returns 1 when
// the singleton is live, else 0.
u32 func_8023FA64(u8* buf, u8 byte, u32 word) {
    u32 result;
    memset(buf, 0, 0xA000);
    if (lbl_eu_80664768 == 0) {
        result = 0;
    } else {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0xA000;
        entry->mField8 = (u32)buf;
        entry->mFieldC = 3;
        entry->mFieldE = byte;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
        result = 1;
    }
    return result;
}

// Push an event {word, 0x28000, 0, 1, byte, 0} onto the manager's ring.
// Returns 1 on success, 0 when the singleton is not set up.
u32 func_8023FB28(u8 byte, u32 word) {
    u32 result;
    CfNandEvent* entry;
    cf::CfNandManager* mgr = lbl_eu_80664768;
    if (mgr == 0) {
        result = 0;
    } else {
        entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                            mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 1;
        entry->mFieldE = byte;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
        result = 1;
    }
    return result;
}

// Push an event {word, 0x28000, 0, 0x10, 0, 0} onto the manager's ring.
// Returns 1 on success, 0 when the singleton is not set up.
u32 func_8023FBA0(u32 word) {
    if (lbl_eu_80664768 == 0) {
        return 0;
    }
    if (lbl_eu_80664768 != 0) {
        cf::CfNandManager* mgr = lbl_eu_80664768;
        CfNandEvent* entry =
            &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                        mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 0x10;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
    }
    return 1;
}

// Target us-80241dc8: push three {word, 0x28000, 0, 8, counter, 0} events
// onto the manager ring (counter = 1..3), after clearing the u16 event-status
// word. Returns whether the last push saw a live singleton.
u32 func_8023FC18(u32 word) {
    if (lbl_eu_80664768 == 0) {
        return 0;
    }
    lbl_eu_8066476E = 0;
    cf::CfNandManager* mgr;
    u32 result;
    u32 counter = 1;
    for (; counter <= 3; counter++) {
        mgr = lbl_eu_80664768;
        if (mgr == 0) {
            result = 0;
            continue;
        }
        CfNandEvent* entry = &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                                         mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0x28000;
        entry->mField8 = 0;
        entry->mFieldC = 8;
        entry->mFieldE = counter;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
        result = 1;
    }
    return result;
}

// Push an event {word, 0x10000, 0, 9, 0, 0} onto the manager's ring.
// Returns 1 on success, 0 when the singleton is not set up.
u32 func_8023FCCC(u32 word) {
    cf::CfNandManager* mgr = lbl_eu_80664768;
    u32 result;
    if (mgr == 0) {
        return 0;
    }
    if (mgr == 0) {
        result = 0;
    } else {
        CfNandEvent* entry =
            &mgr->mEventQueue.mRingBase[(mgr->mEventQueue.mHead + mgr->mEventQueue.mTail) %
                                        mgr->mEventQueue.mSize];
        entry->mWord = word;
        entry->mTag = 0x10000;
        entry->mField8 = 0;
        entry->mFieldC = 9;
        entry->mFieldE = 0;
        entry->mFieldF = 0;
        mgr->mEventQueue.mTail++;
        result = 1;
    }
    return result;
}

void func_8023FD4C(){}

void func_8023FEDC(){}

// Target us-80242188: CfNandManager factory (retail flat symbol
// create__Q22cf13CfNandManagerFv). Returns 0 while the singleton exists;
// otherwise allocates the 0x194-byte manager from work memory, runs the
// flat-name ctor, and registers it under pParent.
cf::CfNandManager* create__Q22cf13CfNandManagerFv(CProcess* pParent, CScn* pScene) {
    if (lbl_eu_80664768 != 0) {
        return 0;
    }
    cf::CfNandManager* mgr =
        (cf::CfNandManager*)mtl::MemManager::allocate(0x194, CWorkThreadSystem::getWorkMem());
    if (mgr != 0) {
        mgr = (cf::CfNandManager*)__ct__cf_CfNandManager(mgr, pScene);
    }
    mgr->Regist(pParent, false);
    return mgr;
}

// Returns whether a NAND file operation is in flight: reads the heap-buffer
// pointer at +0x5C of the CfNandManager singleton (null singleton -> 0).
u32 func_8024005C() {
    if (lbl_eu_80664768 != nullptr) {
        return lbl_eu_80664768->field_0x5C != 0;
    }
    return 0;
}

void func_80240084(){}

// Target us-80242518: (re)build the NAND resource buffers: two GX texture
// buffer sizes (a fixed 0xa4x0x74 tile and a VI-size buffer) merged into one
// tail allocation, halves stored in lbl_eu_8066477C / lbl_eu_80664780. Then
// re-arm the u16 counter, pull the D80 scene, and (re)register the manager's
// IScnRender (+0x58 subobject) with the scene.
void func_80240360() {
    if (lbl_eu_8066477C == 0) {
        u32 size1 = GXGetTexBufferSize(0xa4, 0x74, 4, 0, 0);
        u16 efbH = CDeviceVI::getRenderModeObj()->efbHeight;
        u32 size2 = GXGetTexBufferSize(CDeviceVI::getRenderModeObj()->fbWidth, efbH, 6, 0, 0);
        u8* buf = (u8*)mtl::MemManager::allocate_tail(mtl::MemManager::getHandleMEM2(), size1 + size2, 0x20);
        lbl_eu_8066477C = buf;
        lbl_eu_80664780 = buf + size1;
    }
    lbl_eu_80664770 = 1;
    CScn* scene = (CScn*)CfRes_getD80Flag();
    scene->addRenderCB(static_cast<IScnRender*>(lbl_eu_80664768), 4, 0);
}

void func_80240420() {
    // Teardown: drop the NAND manager's render callback from the D80 scene,
    // free the heap buffer, and clear the u16 counter.
    CScn* scene = (CScn*)CfRes_getD80Flag();
    if (scene != nullptr) {
        scene->removeRenderCB(static_cast<IScnRender*>(lbl_eu_80664768));
    }
    if (lbl_eu_8066477C != nullptr) {
        mtl::MemManager::deallocate(lbl_eu_8066477C);
        lbl_eu_8066477C = nullptr;
    }
    lbl_eu_80664770 = 0;
}

void cf::CfNandManager::cbRenderBefore() {
    // While the counter is armed (1), copy the EFB into the shared texture
    // buffer and flush it, then disarm.
    if (lbl_eu_80664770 == 1) {
        // §4 regalloc: declaration order maps 1st->r30, 2nd->r30 (reuse), 3rd->r31.
        u16 efbH;      // blocks 1-2 -> r30
        u32 size1;     // -> r30
        u16 efbH2;     // blocks 2+4 -> r31 (conflicts with size1)
        u32 size2;
        u16 fbW;
        efbH = CDeviceVI::getRenderModeObj()->efbHeight;
        fbW = CDeviceVI::getRenderModeObj()->fbWidth;
        GXSetTexCopySrc(0, 0, fbW, efbH);
        efbH2 = CDeviceVI::getRenderModeObj()->efbHeight;
        GXSetTexCopyDst(CDeviceVI::getRenderModeObj()->fbWidth, efbH2, (GXTexFmt)6, (GXBool)0);
        GXCopyTex(lbl_eu_80664780, (GXBool)0);
        GXPixModeSync();
        GXInvalidateTexAll();
        size1 = GXGetTexBufferSize(0xa4, 0x74, 4, (GXBool)0, 0);
        u16 h = CDeviceVI::getRenderModeObj()->efbHeight;
        u16 w = CDeviceVI::getRenderModeObj()->fbWidth;
        size2 = GXGetTexBufferSize(w, h, 6, (GXBool)0, 0);
        DCFlushRange(lbl_eu_8066477C, size1 + size2);
        lbl_eu_80664770 = 2;
    }
}

void OnFileEvent__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))func_8023E4D4__Q22cf13CfNandManagerFv)((char*)self - 0x54); }

void func_802405FC__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))__dt__Q22cf13CfNandManagerFv)((char*)self - 0x54); }

void func_80240604__Q22cf13CfNandManagerFv(void* self) { ((void(*)(void*))cbRenderBefore__Q22cf13CfNandManagerFv)((char*)self - 0x58); }

extern "C" void func_8024060C__Q22cf13CfNandManagerFv(cf::CfNandManager* self) { ((void(*)(void*))__dt__Q22cf13CfNandManagerFv)((char*)self - 0x58); }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<T> is declared in kyoshin/CTaskGameEff.hpp (via harness_catalog.hpp);
// specializations below emit the retail Move/Draw/dtor symbols.
namespace cf { class CfNandManager; }
template<> CTTask<cf::CfNandManager>::~CTTask() {}

// Dispatch the +0x3C move callback through __ptmf_test/__ptmf_scall when set.
template<>
void CTTask<cf::CfNandManager>::Move() {
    if (__ptmf_test(&mMoveFunc)) {
        (static_cast<cf::CfNandManager*>(this)->*mMoveFunc)();
    }
}

// Dispatch the +0x48 draw callback through __ptmf_test/__ptmf_scall when set.
template<>
void CTTask<cf::CfNandManager>::Draw() {
    if (__ptmf_test(&mDrawFunc)) {
        (static_cast<cf::CfNandManager*>(this)->*mDrawFunc)();
    }
}
extern "C" void sinit_802405F0(void) {}
