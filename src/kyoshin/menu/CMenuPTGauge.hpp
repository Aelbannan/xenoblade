#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn.hpp"
#include "monolib/work/CProcess.hpp"

// PTGauge widget state machine:
//   mPhase 0 = entry/appear, 1 = appearing anim, 2 = active, 3 = disappearing anim
//   mActionIdx indexes into the PTMF dispatch table lbl_eu_805323F8

// novtable: retail's dtor emits NO class-vtable store.
// Intermediate base mirroring the CTTask<IUICf> tail (ptmf slots at
// 0x3C/0x48). Deliberately does NOT inherit CProcess: its destructor calls
// __dt__8CProcessFv explicitly under a doubled self-check, so when MWCC
// inlines it into ~CMenuPTGauge we get retail's dead doubled beq before the
// base-dtor call, with no implicit duplicate.
class __declspec(novtable) PTGaugeTaskMid {
public:
    // Declared only; defined in the TU.
    virtual ~PTGaugeTaskMid();
    // MWCC 3-word pointer-to-member-function shape (func/this/delta).
    struct Ptmf {
        void* func;
        void* thisAdj;
        s32 delta;
    };
    u8 _04[0x38];   // 0x04-0x3C: CProcess body (vtable-relative region)
    Ptmf mMoveFunc; // 0x3C
    Ptmf mDrawFunc; // 0x48
};

class __declspec(novtable) CMenuPTGauge : public PTGaugeTaskMid {
public:
    void Init();
    void Move();
    void cbRenderBefore();

    // 0x00: base classes (CProcess at +0x00, CTTask<IUICf> region, IWorkEvent,
    // IScnRender vtable slot at +0x5c)
    u8 unk54[0xC]; // 0x54-0x60: IWorkEvent/IScnRender vtable slots
    CScn* mScn; // 0x60 -- owning scene; addRenderCB target in Init
    UnkClass_8045F564 mLayoutMem; // 0x64 -- layout memory region
    nw4r::lyt::Layout* mLayout; // 0x74 -- layout draw target
    nw4r::lyt::AnimTransform* mAnimDefault; // 0x78 -- main animation (PTGauge_00)
    nw4r::lyt::AnimTransform* mAnimOpen; // 0x7C -- open animation (PTGauge_01)
    nw4r::lyt::AnimTransform* mAnimClose; // 0x80 -- close animation (PTGauge_02)
    nw4r::lyt::AnimTransform* mAnimIdle; // 0x84 -- idle animation (PTGauge_03)
    nw4r::lyt::AnimTransform* mAnimSpecial; // 0x88 -- special animation (PTGauge_04)
    s32 mPhase; // 0x8C -- FSM phase (0=entry, 1=appearing, 2=active, 3=disappearing)
    s32 mActionIdx; // 0x90 -- substate / PTMF dispatch index (0, 1, 2)
    s32 mGaugeBase; // 0x94 -- gauge starting value
    s32 mGaugePrev; // 0x98 -- gauge display / previous value
    s32 unk9C; // 0x9C -- unused
    s32 mGaugeInit; // 0xA0 -- gauge panel initialised flag (non-zero = needs layout fixup)
    virtual ~CMenuPTGauge();
    void Term();
};

// Minimal abstract mirror of the font object returned by
// CDeviceFont::getFontInfo. Its slot 9 sits at vtable offset 0x24 and
// returns the u32 bound into the gauge's font pane. Never instantiated (no
// ctor), so no vtable is emitted; declaring the call as a genuine virtual
// member makes MWCC emit retail's native r12 dispatch sequence.
struct CMenuPTGaugeFont {
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0; // vtable offset 0x24
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" nw4r::lyt::ArcResourceAccessor* func_801355F4(); // shared ARC accessor (retail unmangled)
// Global pointer to the active CMenuPTGauge instance (sda21-relative in retail).
extern CMenuPTGauge* lbl_eu_806642D8;
