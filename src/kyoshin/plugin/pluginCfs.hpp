#pragma once

#include "monolib/vm/yvm2.h"

// Plugin script functions for the CFS (Common File System / script) subsystem.
// These are registered via pluginCfsRegist() and called from the VM.

extern "C" bool func_8004A2E8();
extern "C" void func_8004A400();

extern "C" void pluginCfsRegist();

namespace cf {

class CfObject {
public:
    float CfObject_UnkVirtualFunc31();

    // TODO: add fields
};

// Dispatch-only shim for the party-player vtable. Never instantiated; all
// virtuals are pure (so no vtable/code is emitted). Slots 0x00-0x10C are
// unused fillers so getTask() lands at vtable slot 0x110, matching the
// retail party-object layout used by partyWarp's virtual call.
class unkPartyPlayerDispatch {
public:
    virtual void vf_00() = 0; virtual void vf_04() = 0;
    virtual void vf_08() = 0; virtual void vf_0C() = 0;
    virtual void vf_10() = 0; virtual void vf_14() = 0;
    virtual void vf_18() = 0; virtual void vf_1C() = 0;
    virtual void vf_20() = 0; virtual void vf_24() = 0;
    virtual void vf_28() = 0; virtual void vf_2C() = 0;
    virtual void vf_30() = 0; virtual void vf_34() = 0;
    virtual void vf_38() = 0; virtual void vf_3C() = 0;
    virtual void vf_40() = 0; virtual void vf_44() = 0;
    virtual void vf_48() = 0; virtual void vf_4C() = 0;
    virtual void vf_50() = 0; virtual void vf_54() = 0;
    virtual void vf_58() = 0; virtual void vf_5C() = 0;
    virtual void vf_60() = 0; virtual void vf_64() = 0;
    virtual void vf_68() = 0; virtual void vf_6C() = 0;
    virtual void vf_70() = 0; virtual void vf_74() = 0;
    virtual void vf_78() = 0; virtual void vf_7C() = 0;
    virtual void vf_80() = 0; virtual void vf_84() = 0;
    virtual void vf_88() = 0; virtual void vf_8C() = 0;
    virtual void vf_90() = 0; virtual void vf_94() = 0;
    virtual void vf_98() = 0; virtual void vf_9C() = 0;
    virtual void vf_A0() = 0; virtual void vf_A4() = 0;
    virtual void vf_A8() = 0; virtual void vf_AC() = 0;
    virtual void vf_B0() = 0; virtual void vf_B4() = 0;
    virtual void vf_B8() = 0; virtual void vf_BC() = 0;
    virtual void vf_C0() = 0; virtual void vf_C4() = 0;
    virtual void vf_C8() = 0; virtual void vf_CC() = 0;
    virtual void vf_D0() = 0; virtual void vf_D4() = 0;
    virtual void vf_D8() = 0; virtual void vf_DC() = 0;
    virtual void vf_E0() = 0; virtual void vf_E4() = 0;
    virtual void vf_E8() = 0; virtual void vf_EC() = 0;
    virtual void vf_F0() = 0; virtual void vf_F4() = 0;
    virtual void vf_F8() = 0; virtual void vf_FC() = 0;
    virtual void vf_100() = 0; virtual void vf_104() = 0;
    virtual void vf_108() = 0; virtual void vf_10C() = 0;
    virtual void* getTask();   // vtable slot 0x110
};

} // namespace cf

