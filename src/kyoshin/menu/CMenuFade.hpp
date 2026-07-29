#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"

#include <nw4r/lyt.h>

// Local IScnRender declaration WITHOUT an explicit destructor.
// The real IScnRender (in IScnRender.hpp) has `virtual ~IScnRender(){}`
// which causes MWCC to emit a null-check branch in derived-class dtors.
// Omitting the dtor here gives IScnRender an implicitly-declared trivial
// destructor, which suppresses the branch while keeping the vtable layout
// identical (cbRenderBefore is the only other virtual; slot order is the
// same modulo the dtor slot which the linker fills from retail data).
class IScnRender {
public:
    virtual void cbRenderBefore();
};

// MWCC_REFERENCE §190: Do not inherit from IWorkEvent (out-of-line dtor
// pulls weak stubs). Intermediate base holds the 0x54-0x5b fields so that
// IScnRender lands at retail offset 0x5c, giving correct non-primary
// base-cast null-check codegen in Term().
class CMenuFadeBase : public IUICf {
public:
    u8 field_0x54;                              // 0x54
    u8 pad55[3];                                // 0x55-0x57
    u32 mIWorkEventVtbl;                        // 0x58 — IWorkEvent vtable (raw)
};

class CMenuFade : public CMenuFadeBase, public IScnRender {
public:
    CMenuFade(CScn* pScn, int p5, int p6, float f1, float f2, float f3);
    virtual ~CMenuFade();
    void Init();
    void Term();
    void Move();
    void Draw();
    void cbRenderBefore();
    void func_80113E2C() { field_0x54 = 1; }

    // IScnRender at 0x5c (public non-primary base)
    CScn* mScn;                                 // 0x60
    UnkClass_8045F564 mLayoutMem;               // 0x64
    nw4r::lyt::Layout* mLayout;                 // 0x74
    nw4r::lyt::AnimTransform* field_0x78;       // 0x78
    f32 field_0x7C;                             // 0x7C
    f32 field_0x80;                             // 0x80
    f32 field_0x84;                             // 0x84
    f32 field_0x88;                             // 0x88
    u32 field_0x8C;                             // 0x8C
    u32 field_0x90;                             // 0x90
    u8 field_0x94;                              // 0x94
    u8 pad_0x95[3];                             // 0x95-0x97
    u32 field_0x98;                             // 0x98
};
