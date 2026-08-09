// Auto-scaffolded catalog TU for kyoshin/CTaskGameEff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "monolib/work/CProcess.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/core/CViewRoot.hpp"

extern u32 __ptmf_null[3];

#pragma optimize_for_size on
CTaskGameEff* __ct__CTaskGameEff(CTaskGameEff* pThis, CScn* scene) {
    __ct__8CProcessFv(pThis);

    u32* p = reinterpret_cast<u32*>(pThis);

    // Interim CTTask<CTaskGameEff> vtable (overwritten later).
    p[4] = reinterpret_cast<u32>(lbl_eu_80525CAC);

    // NULL PTMF -> mMoveFunc (0x3C) / mDrawFunc (0x48). Retail keeps the PTMF
    // base in a non-volatile register (r28) and uses additive loads, so access
    // __ptmf_null by index (not by incrementing a pointer) to reproduce that.
    p[0x10] = __ptmf_null[1];        // 0x40 mMoveFunc[1]
    p[0xF] = __ptmf_null[0];         // 0x3C mMoveFunc[0]
    p[0x11] = __ptmf_null[2];        // 0x44 mMoveFunc[2]
    p[0x13] = __ptmf_null[1];        // 0x4C mDrawFunc[1]
    p[0x12] = __ptmf_null[0];        // 0x48 mDrawFunc[0]
    p[0x14] = __ptmf_null[2];        // 0x50 mDrawFunc[2]

    // Final vtable base kept live in a register; sub-vtable offsets folded into
    // the 0x54/0x58 stores (retail adds 0x24/0x34 off the held base).
    p[4] = (u32)lbl_eu_80525BFC;
    p[0x15] = (u32)(lbl_eu_80525BFC + 0x24);   // 0x54 field_0x54
    p[0x16] = (u32)(lbl_eu_80525BFC + 0x34);   // 0x58 field_0x58
    p[0x17] = (u32)scene;           // 0x5C mScene
    p[0x18] = 0;                    // 0x60
    pThis->mActive = 1;             // 0x64
    p[0x1A] = 0;                    // 0x68
    pThis->field_0x6C = 0.0f;       // 0x6C
    p[0x1C] = (u32)lbl_eu_80525C90; // 0x70
    p[0x1D] = (u32)lbl_eu_80525C84; // 0x74 (interim)
    p[0x22] = 0;                    // 0x88
    p[0x23] = 0;                    // 0x8C
    pThis->field_0x90 = 0;          // 0x90
    p[0x1E] = (u32)&pThis->mHeaderNode;             // 0x78
    pThis->mHeaderNode.next = &pThis->mHeaderNode;  // 0x7C
    pThis->mHeaderNode.prev = &pThis->mHeaderNode;  // 0x80
    p[0x1D] = (u32)lbl_eu_80525C6C; // 0x74 (final)

    return pThis;
}
#pragma optimize_for_size off

void __dt__80044BB0(){}

void __dt__Q212CTaskGameEff18CEffRenderHighPrioFv(){}

// Base destructor stub (not in batch targets, keep for linking).
void __dt___reslist_base_CScn() {}

// Forward-declare base destructor with MWCC ABI signature (this, flags).
// extern "C" so call sites emit the unmangled retail symbol (no inlining: no
// in-TU body of this signature).
extern "C" void* __dt___reslist_base_CScn(void* _this, int flags);

// reslist<CScn> destructor - standard MWCC virtual dtor pattern.
// #pragma optimize_for_size on keeps stmw r30 instead of individual stw
// (same pattern as CTTask<CTaskGameEff>::~CTTask below).
#pragma optimize_for_size on
void* __dt__reslist_CScn(void* _this, int flags) {
    if (_this) {
        __dt___reslist_base_CScn(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}
#pragma optimize_for_size off

CTaskGameEff::~CTaskGameEff() {}

// Returns a global word from the sdata2/sdata pool (single lwz+sda21 reloc).
u32 func_80044DF4() { return (u32)lbl_eu_80663D40; }

#pragma optimize_for_size on
void CTaskGameEff::Init() {
    lbl_eu_80663D40 = this;

    void* size = func_804CB9F4();
    mMemAlloc = (u32)mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM1(),
                                                     (u32)size, 0x20);
    void* size2 = func_804CB9F4();
    func_804CBA00(lbl_eu_8065FC18, (void*)mMemAlloc, size2);
    func_804CBAA8(lbl_eu_8065FC18, mScene, 1);

    // Register render callbacks. Retail re-reads the singleton this-pointer and
    // null-guards all but the 0x54 callback.
    IScnRender* cb0x54 = &field_0x54;
    IScnRender* cb58 = reinterpret_cast<IScnRender*>(this);
    if (this) cb58 = &field_0x58;
    func_80495FC8(mScene, cb58, 8);

    IScnRender* cb70 = reinterpret_cast<IScnRender*>(this);
    if (this) cb70 = &field_0x70;
    mScene->addRenderCB(cb70, 1, 0);

    mScene->addRenderCB(cb0x54, 0xe, 0);

    mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
    mEffArray = mtl::MemManager::allocate_array(0x30, mem2);

    // Zero the first word of each of the 4 effect-list slots (stride 0xC).
    // Access mEffArray directly inside the loop so MWCC reloads it each pass.
    for (u32 j = 0; j < 4; j++) {
        reinterpret_cast<u32*>(mEffArray)[j * 3] = 0;
    }
    mEffCount = 4;
}
#pragma optimize_for_size off

void CTaskGameEff::Term() {}



void func_80044FBC__FUl(){}

void cbRenderBefore__12CTaskGameEffFv() {
    func_804CBB60(lbl_eu_8065FC18);
}

void func_80045044(){}

void func_800450C8() {}

void func_800450CC(){}

void func_8004513C(){}

// func_800451D8: resolve an effect by table index off the game-manager instance,
// dynamic-cast it to the base effect type, attach it to `manager`, and copy the
// manager's 0x304 data block. Returns the resolved effect (or 0).
// optimize_for_size on: retail prologue saves r30+r31 via stmw, not stw.
#pragma optimize_for_size on
void* func_800451D8(int index, void* manager) {
    if (cf::CfGameManager::getInstance() == nullptr) {
        return nullptr;
    }
    if (lbl_eu_80663D40 == nullptr) {
        return nullptr;
    }
    void* eff = func_8008187C__Q22cf13CfGameManagerFv(index);
    void* obj = __dynamic_cast(eff, 0, &lbl_eu_80661970, &lbl_eu_806618F0, 0);
    if (obj != nullptr && manager != nullptr) {
        func_800ACF78(obj, manager, 0);
        void* data = *(void**)((u8*)manager + 0x98);
        if (data != nullptr) {
            func_800ACEF8(obj, (u8*)data + 0x304);
        }
    }
    return obj;
}
#pragma optimize_for_size off

// Renders/updates effect resources via the global effect singleton at lbl_eu_8065FC18.
// First parameter (this) is unused; second parameter is forwarded to func_804CBB84.
// The cache/ViewRoot pair flushes GX state before and after the effect calls.
#pragma optimize_for_size on
void func_80045284(void* unused, void* param) {
    CDeviceGX::getCacheInstance()->func_8044BE38();
    CViewRoot::func_80442DA8();
    func_804CBB84(lbl_eu_8065FC18, param);
    func_804CBE48(lbl_eu_8065FC18);
    func_804CC104(lbl_eu_8065FC18);
    func_804CBEE8(lbl_eu_8065FC18);
    CDeviceGX::getCacheInstance()->func_8044BE38();
    CViewRoot::func_80442DA8();
}
#pragma optimize_for_size off

void func_800452EC(){}

// func_800453EC: unregister the render callbacks for `scene` and unlink it
// from the effect-task's scene list. Finds the list node whose scene matches.
void func_800453EC(CScn* scene) {
    CTaskGameEff* gTask = lbl_eu_80663D40;
    if (gTask == nullptr) return;
    if (scene == nullptr) return;

    EffListNode* header = gTask->mSceneList;
    EffListNode* node = header->next;
    while (node != header && node->scene != scene) {
        node = node->next;
    }
    if (node == header) return;

    // Retail re-reads the singleton and null-guards each callback thunk.
    CTaskGameEff* gt = lbl_eu_80663D40;
    IScnRender* cb = reinterpret_cast<IScnRender*>(gt);
    if (gt != nullptr) cb = &gt->field_0x58;
    node->scene->removeRenderCB(cb);

    gt = lbl_eu_80663D40;
    cb = reinterpret_cast<IScnRender*>(gt);
    if (gt != nullptr) cb = &gt->field_0x54;
    func_80495FDC(node->scene, cb, 8);

    gt = lbl_eu_80663D40;
    node->scene->removeRenderCB(&gt->field_0x70);

    EffListNode* p = node->prev;
    EffListNode* n = node->next;
    p->next = n;
    n->prev = p;
    node->next = nullptr;
}



bool func_80045540(){ return false; }

bool func_80045548(){ return false; }

bool func_80045550(){ return false; }

bool func_80045558(){ return false; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
// CTTask<CTaskGameEff> is declared in kyoshin/CTaskGameEff.hpp; the out-of-line
// template specializations below emit the retail Move/Draw/dtor symbols.

// CTTask<CTaskGameEff> constructor - out-of-line
template<>
CTTask<CTaskGameEff>::CTTask() : mMoveFunc(nullptr), mDrawFunc(nullptr) {}

// CTTask<CTaskGameEff>::~CTTask - dtor body is empty; compiler emits:
//   null check → CProcess::~CProcess(this, 0) → conditional operator delete
// #pragma optimize_for_size on keeps stmw r30 instead of individual stw.
#pragma optimize_for_size on
template<>
CTTask<CTaskGameEff>::~CTTask() {}
#pragma optimize_for_size off

// CTTask<CTaskGameEff>::Move - test PTMF at +0x3C, call if non-null
template<>
void CTTask<CTaskGameEff>::Move() {
    if (mMoveFunc) {
        (static_cast<CTaskGameEff*>(this)->*mMoveFunc)();
    }
}

// CTTask<CTaskGameEff>::Draw - test PTMF at +0x48, call if non-null
template<>
void CTTask<CTaskGameEff>::Draw() {
    if (mDrawFunc) {
        (static_cast<CTaskGameEff*>(this)->*mDrawFunc)();
    }
}
