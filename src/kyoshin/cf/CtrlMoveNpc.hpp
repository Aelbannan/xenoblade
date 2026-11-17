#pragma once

#include <types.h>
#include <monolib/math/CVec3.hpp>
#include "kyoshin/cf/CtrlMoveBase.hpp"

namespace cf {

// CCtrlMoveNpc - NPC movement controller. Derived from CCtrlMoveBase (base
// occupies 0x00..0x4B; the secondary vptr at 0x48 is re-stored by this ctor,
// novtable like the base so the retail label lbl_eu_80532DB4 is installed by
// hand). Own fields begin at 0x4C; mStateFunc (0x4C) is a ptmf, null by
// default and set to a real state function by beginNpcMove.
//
// The ctor passes the owner pointer straight through to the base ctor
// (retail leaves r4 untouched before the base call).
class __declspec(novtable) CCtrlMoveNpc : public CCtrlMoveBase {
public:
    CCtrlMoveNpc(void* owner);

    int (CCtrlMoveNpc::*mStateFunc)();  // 0x4C state dispatch ptmf (12 bytes)
    ml::CVec3 mField58;     // 0x58 movement target (raw-copied from vec)
    u32 mField64;           // 0x64
    f32 mField68;           // 0x68
    f32 mField6C;           // 0x6C
    f32 mField70;           // 0x70
    f32 mField74;           // 0x74
    u32 mField78;           // 0x78
};

} // namespace cf
