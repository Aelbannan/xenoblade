#pragma once

#include <types.h>

#include "kyoshin/IUICf.hpp"
#include "monolib/scn/IScnRender.hpp"
#include "kyoshin/CTitleAHelp.hpp"

class CProcess;

// Opaque kizuna-chart sub-object at +0x98 (spans 0x98..0x178). Raw storage:
// its constructor/destructor are called by hand from the .cpp.
struct UnkKizunaMenuSub98 {
    u8 data[0xE0];
};

// Opaque slot for the CPcKizunagram sub-object at +0x178 (sizeof 0x4C). Raw
// storage, constructed and destroyed by hand (see the .cpp).
struct UnkKizunaMenuPcSub {
    u8 data[0x4C];
};

// Kizuna gram (affinity chart) screen shown on the World Map.
// IUICf task + IScnRender render-callback subobject at offset +0x58.
//
// Layout (recovered from retail member offsets):
//   0x00: CTTask<IUICf> / IUICf base (0x54/0x55 flags)
//   0x58: IScnRender (render-callback subobject, vptr)
//   0x5C: CProcess* parent reference
//   0x60: CTitleAHelp
//   0x98: kizuna chart sub-object (opaque, CKizunagram 0xE0)
//  0x178: CPcKizunagram (opaque, 0x4C)
//  0x1C4: CLoad (raw storage, 0x30)
//  0x1F4: CFade (raw storage, 0x28)
//  0x21C: u8 state
class CMenuKizunagram : public IUICf, public IScnRender {
public:
    CMenuKizunagram(CProcess* parentRef, int b);
    virtual ~CMenuKizunagram();

    virtual void Init();
    virtual void Term();
    virtual void Move();
    virtual void cbRenderBefore(CScn* scene);

    /* 0x5C */ CProcess* mParentRef;
    /* 0x60 */ CTitleAHelp mTitleAHelp;
    /* 0x98 */ UnkKizunaMenuSub98 mSub98;
    /* 0x178 */ UnkKizunaMenuPcSub mPcKizunagram;
    /* 0x1C4 */ u8 mLoad[0x30];
    /* 0x1F4 */ u8 mFade[0x28];
    /* 0x21C */ u8 field_0x21C;
    /* 0x21D */ u8 field_0x21D;
    /* 0x21E */ u8 field_0x21E;
}; // size: 0x220 (allocation size used by KizunagramCreateSingleton)
