#pragma once

#include "kyoshin/IUICf.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

class CMenuFade : public IUICf, public IWorkEvent, public IScnRender {
public:
    CMenuFade();
    virtual ~CMenuFade();
    void Init();
    void Term();
    void Move();
    void Draw();
    void cbRenderBefore();
    void func_80113E2C() { field_0x54 = 1; }

    // Fields (IUICf/CTTask at 0x00-0x53, IWorkEvent at 0x58, IScnRender at 0x5c)
    u8 field_0x54;                     // 0x54
    u8 pad55[3];                       // 0x55-0x57
    // IWorkEvent vtable at 0x58
    // IScnRender vtable at 0x5c
    CScn* mScn;                        // 0x60
    UnkClass_8045F564 mLayoutMem;      // 0x64
    nw4r::lyt::Layout* mLayout;        // 0x74
};
