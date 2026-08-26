// CTaskGameEff: battle effect task (kyoshin/CTaskGameEff)
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
extern "C" void __dl__FPv(void*);
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/core/CViewRoot.hpp"
#include "monolib/core/CTaskManager.hpp"

extern u32 __ptmf_null[3];

__declspec(noinline) CTaskGameEff* __ct__CTaskGameEff(CTaskGameEff* pThis, CScn* scene) {
    __ct__8CProcessFv(pThis);

    // Sub-object vtables, kept as locals so MWCC holds the bases in registers
    // across the member stores.
    char* vtbl = reinterpret_cast<char*>(const_cast<u8*>(lbl_eu_80525BFC)); // final CTaskGameEff vtable
    u32 v54 = (u32)(vtbl + 0x24);      // 0x54 render-callback sub-vtable
    u32 v58 = (u32)(vtbl + 0x34);      // 0x58 render-callback sub-vtable
    u32 v70 = (u32)lbl_eu_80525C90;    // CEffRenderHighPrio vtable
    u32 v74i = (u32)lbl_eu_80525C84;   // scene-list interim vtable
    u32 v74f = (u32)lbl_eu_80525C6C;   // scene-list final vtable
    EffResListNode* node = &pThis->mSceneList.mStartNode;

    u32* p = reinterpret_cast<u32*>(pThis);

    // Interim CTTask<CTaskGameEff> vtable (overwritten by the final vptr).
    p[4] = reinterpret_cast<u32>(lbl_eu_80525CAC);

    f32 time = lbl_eu_80665D90;        // default effect time

    // NULL PTMF -> mMoveFunc (0x3C) / mDrawFunc (0x48): whole 12-byte
    // pointer-to-member assignments; MWCC picks the load/store schedule.
    CTaskGameEffPtmf* pMf = reinterpret_cast<CTaskGameEffPtmf*>(reinterpret_cast<char*>(pThis) + 0x3c);
    const CTaskGameEffPtmf* pNull = reinterpret_cast<const CTaskGameEffPtmf*>(__ptmf_null);
    *pMf = *pNull;
    u32 zero = 0;
    CTaskGameEffPtmf* pDf = reinterpret_cast<CTaskGameEffPtmf*>(reinterpret_cast<char*>(pThis) + 0x48);
    *pDf = *pNull;

    // Final vtable + member fields.
    p[4] = (u32)vtbl;
    p[0x15] = v54;
    p[0x16] = v58;
    p[0x17] = (u32)scene;           // 0x5C mScene
    p[0x18] = zero;                 // 0x60 mMemAlloc
    reinterpret_cast<u8*>(pThis)[0x64] = 1;  // mActive
    p[0x1A] = zero;                 // 0x68
    pThis->field_0x6C = time;       // 0x6C default effect time
    p[0x1C] = v70;                  // 0x70
    p[0x1D] = v74i;                 // 0x74 (interim)
    p[0x22] = zero;                 // 0x88 mSceneList.mList
    p[0x23] = zero;                 // 0x8C mSceneList.mCapacity
    reinterpret_cast<u8*>(pThis)[0x90] = (u8)zero;  // mSceneList.unk1C
    // Self-linked sentinel header node.
    pThis->mSceneList.mHead = node;   // 0x78
    node->mNext = node;               // 0x7C
    node->mPrev = node;               // 0x80
    p[0x1D] = v74f;                 // 0x74 (final)
    return pThis;
}

void* __dt__80044BB0(void* obj, int mode) {
    if (obj != 0 && mode > 0) {
        __dl__FPv(obj);
    }
    return obj;
}

// High-priority render callback dtor (retail __dt__Q212CTaskGameEff18CEffRenderHighPrioFv).
// Declared-only in the header so the containing dtor emits an out-of-line call.
CTaskGameEff::CEffRenderHighPrio::~CEffRenderHighPrio() {}

// reslist<CScn> destructor - standard MWCC virtual dtor pattern (retail
// __dt__reslist_CScn; complete-object flavor). The base-list dtor
// __dt___reslist_base_CScn is defined below (flat template mangling).
#pragma optimize_for_size on

// _reslist_base<CScn> deleting destructor (retail __dt___reslist_base_CScn).
// Old flat template mangling -> plain global function over the ResListCScn
// layout mirror (same shape as the matched CUIBattleManager reslist dtors).
// The C-linkage forward declaration makes the definition adopt the flat
// retail name instead of MWCC's parameterized mangling.
extern "C" void* __dt___reslist_base_CScn(ResListCScn* self, int mode);
__declspec(noinline) void* __dt___reslist_base_CScn(ResListCScn* self, int mode) {
    if (self != NULL) {
        self->mVtable = (void*)lbl_eu_80525C84;
        // Walk the node chain nulling each next pointer (nodes are
        // pool-allocated, not freed here), then reset the sentinel.
        EffResListNode* node = self->mHead->mNext;
        while (node != self->mHead) {
            EffResListNode* cur = node;
            node = cur->mNext;
            cur->mNext = NULL;
        }
        self->mHead->mNext = self->mHead;
        self->mHead->mPrev = self->mHead;
        if (self->unk1C == false && self->mList != NULL) {
            delete[] self->mList;
            self->mList = NULL;
        }
        if (mode > 0) {
            __dl__FPv(self);
        }
    }
    return self;
}

void* __dt__reslist_CScn(void* _this, int flags) {
    if (_this != NULL) {
        // Retail bl's straight to the out-of-line _reslist_base<CScn> dtor
        // (no vtable dispatch), then conditionally deletes the object.
        __dt___reslist_base_CScn((ResListCScn*)_this, 0);
        if (flags > 0) {
            __dl__FPv(_this);
        }
    }
    return _this;
}
#pragma optimize_for_size off

extern "C" void __dt__Q212CTaskGameEff18CEffRenderHighPrioFv(void* self,
                                                            int flags);
extern "C" void __dt__8CProcessFv(void* self, int flags);

// Retail dtor is a plain free function: destroy the scene list (guarded),
// the high-prio render callback, and the CProcess base, then free the object
// when the delete flag is positive. The redundant `if (self != 0)` re-check
// reproduces the retail's dead second beq.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void* __dt__12CTaskGameEffFv(void* self, int flags) {
    if (self != 0) {
        if ((u8*)self + 0x74 != 0) {
            __dt___reslist_base_CScn((ResListCScn*)((u8*)self + 0x74), 0);
        }
        __dt__Q212CTaskGameEff18CEffRenderHighPrioFv((u8*)self + 0x70, -1);
        if (self != 0) {
            __dt__8CProcessFv(self, 0);
        }
        if (flags > 0)
            __dl__FPv(self);
    }
    return self;
}
#pragma optimize_for_size off

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

    // Register render callbacks. Retail re-evaluates the null-guard at each
    // call site via the two-statement guard idiom.
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this) cb = &field_0x54;
    func_80495FC8(mScene, cb, 8);

    cb = reinterpret_cast<IScnRender*>(this);
    if (this) cb = &field_0x58;
    mScene->addRenderCB(cb, 1, 0);

    mScene->addRenderCB(&field_0x70, 0xe, 0);

    mtl::ALLOC_HANDLE mem2 = mtl::MemManager::getHandleMEM2();
    mSceneList.mList = (EffResListNode*)mtl::MemManager::allocate_array(0x30, mem2);

    // Zero the first word of each of the 4 effect-list slots (stride 0xC).
    // Access mSceneList.mList directly inside the loop so MWCC reloads it each pass.
    for (u32 j = 0; j < 4; j++) {
        reinterpret_cast<u32*>(mSceneList.mList)[j * 3] = 0;
    }
    mSceneList.mCapacity = 4;
}
#pragma optimize_for_size off

// func_800450CC: allocate a CTaskGameEff (0x94) from work memory, construct it
// with `scene`, register it under `parent`, then spin up the after-task factory
// under the scene root process. Returns the new task (or null if the allocation
// failed; Regist still runs on the null pointer - retail behaviour, same as the
// __ct__CTaskGameEffAfter factory).
// optimize_for_size on: retail saves r29-r31 via stmw, not individual stw.
#pragma optimize_for_size on
CTaskGameEff* func_800450CC(CProcess* parent, CScn* scene) {
    CTaskGameEff* task = (CTaskGameEff*)mtl::MemManager::allocate(0x94, CWorkThreadSystem::getWorkMem());
    if (task != nullptr) {
        task = __ct__CTaskGameEff(task, scene);
    }
    task->Regist(parent, false);
    __ct__CTaskGameEffAfter(CTaskManager::GetRootProcScn());
    return task;
}
#pragma optimize_for_size off

void CTaskGameEff::Term() {
    IScnRender* cb58 = reinterpret_cast<IScnRender*>(this);
    if (this) cb58 = &field_0x58;
    mScene->removeRenderCB(cb58);

    IScnRender* cb54 = reinterpret_cast<IScnRender*>(this);
    if (this) cb54 = &field_0x54;
    func_80495FDC(mScene, cb54, 8);

    mScene->removeRenderCB(&field_0x70);
    func_804CC154(&lbl_eu_8065FC18[0]);

    if (mMemAlloc != 0) {
        mtl::MemManager::deallocate((void*)mMemAlloc);
        mMemAlloc = 0;
    }
    lbl_eu_80663D40 = nullptr;
}



// func_80044FBC: toggle the effect singleton's visibility. Non-zero `enable`
// uses the default effect time constant (lbl_eu_80665D94) and sets bit 0x2 of
// field_0x68; zero uses the per-instance field_0x6C time and clears the bit.
void func_80044FBC(u32 enable) {
    CTaskGameEff* gTask = lbl_eu_80663D40;
    if (gTask == nullptr) return;
    f32 time = (enable != 0) ? lbl_eu_80665D94 : gTask->field_0x6C;
    func_804CBB14(lbl_eu_8065FC18, time);
    gTask = lbl_eu_80663D40;
    if (enable != 0) {
        gTask->field_0x68 |= 0x2;
    } else {
        gTask->field_0x68 &= ~0x2;
    }
}

extern "C" __declspec(noinline) void cbRenderBefore__12CTaskGameEffFv(void* self) {
    func_804CBB60(lbl_eu_8065FC18);
}

// func_80045044: flush GX state, fold this->mActive into bit 11 (0x800) of the
// effect-singleton flag word at lbl_eu_8065FC18, run the per-frame effect
// update pass, then flush GX state again.
#pragma optimize_for_size on
extern "C" __declspec(noinline) void func_80045044(CTaskGameEff* self, void* param) {
    CDeviceGX::getCacheInstance()->func_8044BE38();
    CViewRoot::func_80442DA8();
    // Bitfield assign folds to the retail lhz/rlwimi/sth insert of mActive
    // into bit 11 (0x800) of the singleton flag halfword.
    struct EffFlagBits {
        u16 unkHi : 4;
        u16 activeBit : 1;
        u16 unkLo : 11;
    };
    ((EffFlagBits*)&lbl_eu_8065FC18[0])->activeBit = self->mActive;
    func_804CBB84(&lbl_eu_8065FC18[0], param);
    func_804CBC90(&lbl_eu_8065FC18[0]);
    func_804CBD14(&lbl_eu_8065FC18[0]);
    func_804CBDB4(&lbl_eu_8065FC18[0]);
    func_804CC104(&lbl_eu_8065FC18[0]);
    CDeviceGX::getCacheInstance()->func_8044BE38();
    CViewRoot::func_80442DA8();
}
#pragma optimize_for_size off

void func_800450C8() {}

// func_8004513C: resolve an effect object by id and attach it to the battle
// host's +0x3E9C container. mode selects the resolver: 1 = the host
// container's vtable slot 0x220, 2 = func_800817BC, otherwise func_8008187C.
// The attach target is the caller's +0x3E9C container (or null itself).
#pragma optimize_for_size on
void* func_8004513C(EffHostObj* target, EffHostObj* host, u32 id, u32 mode) {
    void* obj;
    if (mode == 1) {
        obj = host->field_0x3E9C.vfn220(id);
    } else if (mode == 2) {
        obj = func_800817BC__Q22cf13CfGameManagerFv(id, 0);
    } else {
        obj = func_8008187C__Q22cf13CfGameManagerFv(id);
    }
    if (obj != nullptr) {
        // Retail folds the +0x3E9C container offset into the target variable
        // itself (null stays null), then attaches the resolved effect.
        u8* dst = (u8*)target;
        if (dst != nullptr) {
            dst += 0x3E9C;
        }
        func_800ACF78(obj, dst, 0);
    }
    return obj;
}
#pragma optimize_for_size off

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
// NOTE: func_80045284 and func_800452EC share one `optimize_for_size on`
// region - retail saves r30+r31 via stmw in both.
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

// func_800452EC: register `scene` with the effect task - push it onto the
// scene list (inlined reslist push_back incl. setItem's guarded item store)
// and attach the task's three render callbacks to the scene.
// optimize_for_size on: retail saves r30+r31 via stmw and keeps sp in r31
// (the inlined setItem try-block SP store targets 0x1c(r31)).
void func_800452EC(CScn* scene) {
    CTaskGameEff* gTask = lbl_eu_80663D40;
    if (gTask != nullptr && scene != nullptr) {
        gTask->mSceneList.push_back(scene);

        // Retail re-reads the singleton and null-guards each callback thunk.
        CTaskGameEff* gt = lbl_eu_80663D40;
        IScnRender* cb54 = reinterpret_cast<IScnRender*>(gt);
        if (gt != nullptr) cb54 = &gt->field_0x54;
        func_80495FC8(scene, cb54, 8);

        gt = lbl_eu_80663D40;
        IScnRender* cb58 = reinterpret_cast<IScnRender*>(gt);
        if (gt != nullptr) cb58 = &gt->field_0x58;
        scene->addRenderCB(cb58, 1, 0);

        gt = lbl_eu_80663D40;
        scene->addRenderCB(&gt->field_0x70, 0xe, 0);
    }
}

// func_800453EC: unregister the render callbacks for `scene` and unlink it
// from the effect-task's scene list. Finds the list node whose scene matches,
// detaches the task's callbacks, then unlinks the node.
void func_800453EC(CScn* scene) {
    CTaskGameEff* gTask = lbl_eu_80663D40;
    if (gTask == nullptr) return;
    if (scene == nullptr) return;

    EffResListNode* header = gTask->mSceneList.mHead;
    EffResListNode* node = header->mNext;
    while (node != header && node->mItem != scene) {
        node = node->mNext;
    }
    if (node == header) return;

    // Retail re-reads the singleton and null-guards each callback thunk.
    CTaskGameEff* gt = lbl_eu_80663D40;
    IScnRender* cb = reinterpret_cast<IScnRender*>(gt);
    if (gt != nullptr) cb = &gt->field_0x58;
    node->mItem->removeRenderCB(cb);

    gt = lbl_eu_80663D40;
    cb = reinterpret_cast<IScnRender*>(gt);
    if (gt != nullptr) cb = &gt->field_0x54;
    func_80495FDC(node->mItem, cb, 8);

    gt = lbl_eu_80663D40;
    node->mItem->removeRenderCB(&gt->field_0x70);

    EffResListNode* p = node->mPrev;
    EffResListNode* n = node->mNext;
    p->mNext = n;
    n->mPrev = p;
    node->mNext = nullptr;
}



// IWorkEvent/IScnRender vtable this-adjusting thunks (retail func_80045540..
// 80045558): subi the subobject pointer, tail-branch to the real impl.
extern "C" void* __dt__12CTaskGameEffFv(void*, int);
extern "C" void func_80045540(void* self) {
    cbRenderBefore__12CTaskGameEffFv((char*)self - 0x54);
}

extern "C" void func_80045548(void* self, int flags) {
    __dt__12CTaskGameEffFv((char*)self - 0x54, flags);
}

extern "C" void func_80045550(CTaskGameEff* self, void* param) {
    func_80045044(reinterpret_cast<CTaskGameEff*>(reinterpret_cast<char*>(self) - 0x58), param);
}

extern "C" void func_80045558(void* self, int flags) {
    __dt__12CTaskGameEffFv((char*)self - 0x58, flags);
}

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
