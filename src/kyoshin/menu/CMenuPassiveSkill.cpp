// Auto-scaffolded catalog TU for kyoshin/menu/CMenuPassiveSkill
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/menu/CMenuPassiveSkill.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"
#include <revolution/GX.h>
#include <nw4r/lyt/lyt_drawInfo.h>

extern "C" void Init__17CMenuPassiveSkillFv() {}

extern "C" void Move__17CMenuPassiveSkillFv() {}

void CMenuPassiveSkill::Init() {}

// ---------------------------------------------------------------------------
// Term (us-80265b7c): wait for VI idle, drop this screen's render callback
// from the owning scene, tear down the three embedded widgets in reverse
// construction order (bgTex / titleAHelp / passiveSkill), clear the
// created-screen singleton and re-enable pad input.
// The null-check/adjust dance around the render pointer reproduces retail:
// a null this would pass the raw pointer, a real this passes the IScnRender
// secondary base at +0x58.
// ---------------------------------------------------------------------------
void CMenuPassiveSkill::Term() {
    CDeviceVI::waitForDrawDone();

    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        render = reinterpret_cast<IScnRender*>(reinterpret_cast<u8*>(this) + 0x58);
    }
    removeRenderCB__4CScnFP10IScnRender(mScnRef, render);

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_8026D920(mPassiveSkill);

    lbl_eu_80664878 = 0;
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

void CMenuPassiveSkill::Move() {}

// noinline: retail keeps the virtual dispatch as a real call (the thunks
// tail-branch to this symbol; an inline would fold the thunks to empty).
__declspec(noinline) void CMenuPassiveSkill::cbRenderBefore() {
    // Draw only while the game task is idle and the realtime-event busy bit
    // (bit 21 of the shared mode word) is clear.
    getInstance__9CTaskGameFv();
    if (func_800426F0__9CTaskGameFv() == 0 && (lbl_eu_80663E28 & 0x200000) == 0) {
        func_8013BE50();
        GXSetZMode(GX_DISABLE, GX_NEVER, GX_DISABLE);

        // Scoped draw-info: build it, drive all three widgets with it, then
        // let the scope-exit destructor run with the -1 subobject flag.
        nw4r::lyt::DrawInfo drawInfo;
        func_80137250(&drawInfo);
        func_801C3D7C(&mBgTex, &drawInfo);
        func_8026D8FC(mPassiveSkill, &drawInfo);
        func_801C4080(&mTitleAHelp, &drawInfo);
    }
}

// ---------------------------------------------------------------------------
// Factory (us-80265d40): single-instance guard - once a screen exists the
// factory returns NULL. Otherwise allocate 0x2B4 bytes from the work-thread
// region, construct it, register it as a child process and return the
// stored singleton pointer.
// ---------------------------------------------------------------------------
extern "C" CMenuPassiveSkill* func_802638D0(CProcess* parent, u32 arg) {
    if (lbl_eu_80664878 != 0) {
        return NULL;
    }

    CMenuPassiveSkill* proc = reinterpret_cast<CMenuPassiveSkill*>(
        mtl::MemManager::allocate(0x2B4, CWorkThreadSystem::getWorkMem()));
    if (proc != NULL) {
        proc = __ct__CMenuPassiveSkill(proc, arg);
    }
    lbl_eu_80664878 = reinterpret_cast<u32>(proc);

    proc->Regist(parent, false);
    return reinterpret_cast<CMenuPassiveSkill*>(lbl_eu_80664878);
}

extern "C" __declspec(noinline) CMenuPassiveSkill* __ct__CMenuPassiveSkill(CMenuPassiveSkill* self, u32 arg) {
    __ct__8CProcessFv(static_cast<CProcess*>(self));
    self->mScnRef = NULL;
    return self;
}

// ---------------------------------------------------------------------------
// Destructor (us-80265698), retail D2 form: written as a free function so the
// base-class destruction targets the game-side CProcess dtor wrapper
// __dt__800FED0C (a real member dtor would emit the library D1 symbol and an
// extra vtable store). Sub-objects are destroyed in reverse construction
// order with the -1 subobject flag, then conditional operator delete when
// flags > 0.
// ---------------------------------------------------------------------------
extern "C" CMenuPassiveSkill* __dt__17CMenuPassiveSkillFv(CMenuPassiveSkill* self, int flags) {
    if (self != NULL) {
        __dt__Q22UI13CPassiveSkillFv(self->mPassiveSkill, -1);
        __dt__11CTitleAHelpFv(&self->mTitleAHelp, -1);
        __dt__6CBgTexFv(&self->mBgTex, -1);
        __dt__800FED0C(self, 0);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// ---------------------------------------------------------------------------
// Per-frame readiness poll (us-80265dc4): once the background texture has
// loaded, the title/help widget is ready and the passive-skill layout finished
// loading, arm the help window, mark the screen ready, fire the 0x6d sound op
// and push the cursor state byte from the passive-skill sub-object into the
// title/help widget.
// ---------------------------------------------------------------------------
extern "C" void func_80263954(CMenuPassiveSkill* self) {
    if (func_801C3E34(&self->mBgTex) != 0 && func_801C4114(&self->mTitleAHelp) != 0 &&
        func_8026D9AC(self->mPassiveSkill) != 0) {
        func_801C412C(&self->mTitleAHelp);
        func_8026DA4C(self->mPassiveSkill);
        self->field_2AC = 1;

        func_80138078__FUl(0x6d);

        u8 curState = func_8026DB74(self->mPassiveSkill);
        func_801C41E8(&self->mTitleAHelp, curState);
    }
}

void stub_us_80265db4() {}

extern "C" void func_802639E4() {}

extern "C" void func_80263A34() {}

extern "C" void func_80263D3C() {}

extern "C" void func_80263D8C() {}

extern "C" void func_80263DE8() {}

extern "C" void func_80263E4C() {}

// Adjusted-this thunk: called through a secondary-base vtable entry
// (IScnRender at offset +0x58 within CMenuPassiveSkill). Retail is
// subi r3,r3,0x58 + tail-branch, so call through the adjusted pointer.
void CMenuPassiveSkill::func_80263EAC() {
    ((CMenuPassiveSkill*)((u8*)this - 0x58))->cbRenderBefore();
}

// Adjusted-this thunk for the destructor (same +0x58 adjustment). Direct
// extern-C call keeps the retail subi + tail-branch (no virtual dispatch,
// no delete flag).
void CMenuPassiveSkill::func_80263EB4() {
    __dt__17CMenuPassiveSkillFv((CMenuPassiveSkill*)((u8*)this - 0x58), 0);
}

extern unsigned long lbl_eu_80664878;
extern "C" unsigned long func_80263944(void) { return lbl_eu_80664878 != 0; }
