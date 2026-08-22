// Auto-scaffolded catalog TU for kyoshin/CTaskGameEff
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
extern "C" void __dl__FPv(void*);
#include "monolib/work/CProcess.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/device/CDeviceGX.hpp"
#include "monolib/core/CViewRoot.hpp"
#include "monolib/core/CTaskManager.hpp"

extern u32 __ptmf_null[3];

#pragma optimize_for_size on
__declspec(noinline) CTaskGameEff* __ct__CTaskGameEff(CTaskGameEff* pThis, CScn* scene) {
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
    p[0x22] = 0;                    // 0x88 mSceneList.mList
    p[0x23] = 0;                    // 0x8C mSceneList.mCapacity
    pThis->mSceneList.unk1C = false; // 0x90
    p[0x1E] = (u32)&pThis->mSceneList.mStartNode;               // 0x78 mStartNodePtr
    pThis->mSceneList.mStartNode.mNext = &pThis->mSceneList.mStartNode;  // 0x7C
    pThis->mSceneList.mStartNode.mPrev = &pThis->mSceneList.mStartNode;  // 0x80
    p[0x1D] = (u32)lbl_eu_80525C6C; // 0x74 (final)

    return pThis;
}
#pragma optimize_for_size off

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
// __dt___reslist_base_CScn is emitted by the template instantiation of
// _reslist_base<CScn>::~_reslist_base() triggered by CTaskGameEff's dtor.
#pragma optimize_for_size on
void* __dt__reslist_CScn(void* _this, int flags) {
    if (_this) {
        static_cast<_reslist_base<CScn>*>(_this)->~_reslist_base<CScn>();
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}
#pragma optimize_for_size off

extern "C" void __dt___reslist_base_CScn(void* self, int flags);
extern "C" void __dt__Q212CTaskGameEff18CEffRenderHighPrioFv(void* self,
                                                            int flags);
extern "C" void __dt__8CProcessFv(void* self, int flags);

// Retail dtor is a plain free function: destroy the scene list (guarded),
// the high-prio render callback, and the CProcess base, then free the object
// when the delete flag is positive. The redundant `if (self != 0)` re-check
// reproduces the retail's dead second beq.
#pragma optimize_for_size on
extern "C" void* __dt__12CTaskGameEffFv(void* self, int flags) {
    if (self != 0) {
        if ((u8*)self + 0x74 != 0) {
            __dt___reslist_base_CScn((u8*)self + 0x74, 0);
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
    mSceneList.mList = (_reslist_node<CScn>*)mtl::MemManager::allocate_array(0x30, mem2);

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

// func_800452EC: register `scene` with the effect task - push it onto the
// scene list (inlined reslist push_back incl. setItem's guarded item store)
// and attach the task's three render callbacks to the scene.
// func_80045284 uses optimize_for_size; this function matches without it.
void func_800452EC(CScn* scene) {
    CTaskGameEff* gTask = lbl_eu_80663D40;
    if (gTask != nullptr && scene != nullptr) {
        gTask->mSceneList.push_back(*scene);

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
// from the effect-task's scene list. Finds the list node whose scene matches.
void func_800453EC(CScn* scene) {
    CTaskGameEff* gTask = lbl_eu_80663D40;
    if (gTask == nullptr) return;
    if (scene == nullptr) return;

    _reslist_node<CScn>* header = &gTask->mSceneList.mStartNode;
    _reslist_node<CScn>* node = header->mNext;
    while (node != header && &node->mItem != scene) {
        node = node->mNext;
    }
    if (node == header) return;

    // Retail re-reads the singleton and null-guards each callback thunk.
    CTaskGameEff* gt = lbl_eu_80663D40;
    IScnRender* cb = reinterpret_cast<IScnRender*>(gt);
    if (gt != nullptr) cb = &gt->field_0x58;
    node->mItem.removeRenderCB(cb);

    gt = lbl_eu_80663D40;
    cb = reinterpret_cast<IScnRender*>(gt);
    if (gt != nullptr) cb = &gt->field_0x54;
    func_80495FDC(&node->mItem, cb, 8);

    gt = lbl_eu_80663D40;
    node->mItem.removeRenderCB(&gt->field_0x70);

    _reslist_node<CScn>* p = node->mPrev;
    _reslist_node<CScn>* n = node->mNext;
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
