// kyoshin/CTaskGameEvt.cpp
// CTaskGameEvt task wrapper: CProcess-derived task that hosts a scene render
// callback and a small state flag set.

#include "kyoshin/CTaskGameEvt.hpp"

#include "monolib/work/CWorkThreadSystem.hpp"

// CTTask<CTaskGameEvt> out-of-line Move/Draw/dtors (retail emits these as
// standalone functions; the inline CTTask header copy would mark them inline).
#pragma optimize_for_size on
template <>
CTTask<CTaskGameEvt>::~CTTask() {}
#pragma optimize_for_size off

template <>
void CTTask<CTaskGameEvt>::Move() {
    if (mMoveFunc) {
        (static_cast<CTaskGameEvt*>(this)->*mMoveFunc)();
    }
}

template <>
void CTTask<CTaskGameEvt>::Draw() {
    if (mDrawFunc) {
        (static_cast<CTaskGameEvt*>(this)->*mDrawFunc)();
    }
}

// ---------------------------------------------------------------------------
// Non-target glue stubs (preserved from scaffold; not part of the match set).
// ---------------------------------------------------------------------------

extern "C" int func_80295764(void* self);

extern "C" void OnFileEvent__12CTaskGameEvtFP10CEventFile(void* self) { ((void(*)(void*))func_80295764)((char*)self - 0x54); }

// Tail-call wrappers into the cf event-task helper calls.
void func_802956A4(void) { func_80165038(); }

void func_802956A8(void) { func_80164CFC(); }

extern "C" void cbRenderBefore__12CTaskGameEvtFv(void* self) { (void)self; }
extern "C" void __dt__12CTaskGameEvtFv(void*, int);

extern "C" int func_80295764(void* self) { (void)self; return 0; }

extern "C" void func_80295870(void* self) { ((void(*)(void*))__dt__12CTaskGameEvtFv)((char*)self - 0x54); }
extern "C" void func_80295878(void* self) { ((void(*)(void*))cbRenderBefore__12CTaskGameEvtFv)((char*)self - 0x58); }
extern "C" void func_80295880(void* self) { ((void(*)(void*))__dt__12CTaskGameEvtFv)((char*)self - 0x58); }

// ---------------------------------------------------------------------------
// Target members.
// ---------------------------------------------------------------------------

// Retail constructor `__ct__CTaskGameEvt` is a *stripped* symbol (no length
// mangling), reconstructed as a free function (cf. CTaskGameCf / CfTaskMain):
// it calls the CProcess base ctor, then performs the in-place vtable / NULL
// PTMF / field setup inline so every store matches retail ordering. Written as
// a free function (not a C++ member ctor) so no `__ct__12CTaskGameEvtFi`
// symbol is emitted and the interim CTTask vtable write can be reproduced.
#pragma optimize_for_size on
// C-linkage so the emitted symbol is exactly the retail `__ct__CTaskGameEvt`
// (stripped symbol; a C++ declaration would mangle to __ct__12CTaskGameEvtFi),
// and so create()'s call reloc references the retail name.
extern "C" __declspec(noinline) CTaskGameEvt* __ct__CTaskGameEvt(CTaskGameEvt* pThis, int arg) {
    __ct__8CProcessFv(pThis);

    // Final CTaskGameEvt vtable base + its two sub-vtable pointers (kept as the
    // label pointer so it is a first-class pointer live to the final 0x10 store;
    // retail holds the base in r6, sub-vtables in r5/r4, PTMF temps in r7/r8).
    char* vtbl = lbl_eu_80538C00;
    u32 v54 = (u32)(vtbl + 0x24);
    u32 v58 = (u32)(vtbl + 0xac);

    u32* p = reinterpret_cast<u32*>(pThis);

    // Interim CTTask<CTaskGameEvt> vtable (overwritten later).
    p[4] = reinterpret_cast<u32>(lbl_eu_80538CE8);

    // NULL PTMF -> mMoveFunc (0x3C) / mDrawFunc (0x48) in the retail store
    // order 0x40,0x3C,0x44 then 0x4C,0x48,0x50. Post-increment derefs of a local
    // pointer make MWCC fold the first access into `lwzu` (single base register
    // r9 via @ha/@l, offsets on the rest) instead of an extra `addi`-materialised
    // pointer - cf. MWCC_REFERENCE btm_sco_init lwzu shape. The stores into p can
    // alias the non-const global, forcing the 4..6/4..5 reloads between slots.
    const u32* src = __ptmf_null;
    u32 w0 = *src++;
    u32 w1 = *src++;
    p[0x10] = w1;        // 0x40 mMoveFunc[1]
    p[0xF] = w0;         // 0x3C mMoveFunc[0]
    u32 w2 = *src++;     // load [2] after the w1 store (retail lwz r7,8(r9) late)
    p[0x11] = w2;        // 0x44 mMoveFunc[2]
    src = __ptmf_null;
    w1 = *src++;
    w0 = *src++;
    p[0x13] = w0;        // 0x4C mDrawFunc[1]
    p[0x12] = w1;        // 0x48 mDrawFunc[0]
    w2 = *src++;
    p[0x14] = w2;        // 0x50 mDrawFunc[2]

    // Final CTaskGameEvt vtable + member fields.
    p[4] = (u32)vtbl;
    p[0x15] = v54;       // 0x54 field_54
    p[0x16] = v58;       // 0x58 mRenderCB (IScnRender vtable)
    p[0x17] = static_cast<u32>(arg); // 0x5C mScene
    p[0x18] = 0;             // 0x60 mFlags

    return pThis;
}
#pragma optimize_for_size off

#pragma optimize_for_size on
CTaskGameEvt::~CTaskGameEvt() {}
#pragma optimize_for_size off

void CTaskGameEvt::Init() {
    IScnRender* rp = reinterpret_cast<IScnRender*>(this); // default: null-this -> this(0)
    if (this) rp = reinterpret_cast<IScnRender*>(&mRenderCB); // override: this + 0x58
    mScene->addRenderCB(rp, 11, 0);
}

void CTaskGameEvt::Term() {
    IScnRender* rp = reinterpret_cast<IScnRender*>(this); // default: null-this -> this(0)
    if (this) rp = reinterpret_cast<IScnRender*>(&mRenderCB); // override: this + 0x58
    mScene->removeRenderCB(rp);
}

void CTaskGameEvt::Move() {
    u32 flags = mFlags;
    if ((flags & 0x2) && !(flags & 0x1) &&
        cf::CTaskGameCf::getInstance() != 0 &&
        func_80164C48() != 0) {
        if (func_80164954()) {
            mFlags |= 1;
        } else {
            mFlags &= ~1;
        }
    }
}

// Preserve empty Draw member (retail 4-byte body `blr`); the PTMF dispatch is
// handled by the CTTask<CTaskGameEvt>::Draw specialization above.
void CTaskGameEvt::Draw() {}

#pragma optimize_for_size on
// Retail symbol keeps the C-linkage Fv name although the source takes a parent
// and a scene arg (cf. CTaskGameCf / CTaskGamePic).
extern "C" CTaskGameEvt* create__12CTaskGameEvtFv(CProcess* pParent, int arg) {
    u32 handle = CWorkThreadSystem::getWorkMem();
    CTaskGameEvt* obj = (CTaskGameEvt*)mtl::MemManager::allocate(0x64, handle);
    if (obj) {
        obj = __ct__CTaskGameEvt(obj, arg);
    }
    obj->Regist(pParent, false);
    return obj;
}
#pragma optimize_for_size off