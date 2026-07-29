#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"

#include <nw4r/lyt.h>

// MWCC_REFERENCE §190: Do not inherit from IWorkEvent/IScnRender —
// the vtable pointers at 0x58/0x5c are raw data; inheriting pulls weak
// destructor stubs that blow the dtor shape vs retail.
class CMenuFade : public IUICf {
public:
    CMenuFade();
    virtual ~CMenuFade();
    void Init();
    void Term();
    void Move();
    void Draw();
    void cbRenderBefore();
    void func_80113E2C() { field_0x54 = 1; }

    // Fields
    // CProcess/CTTask/IUICf at 0x00-0x53
    u8 field_0x54;                              // 0x54
    u8 pad55[3];                                // 0x55-0x57
    u32 mIWorkEventVtbl;                        // 0x58 — IWorkEvent vtable pointer (raw)
    u32 mIScnRenderVtbl;                        // 0x5c — IScnRender vtable pointer (raw)
    CScn* mScn;                                 // 0x60
    UnkClass_8045F564 mLayoutMem;               // 0x64
    nw4r::lyt::Layout* mLayout;                 // 0x74
};
