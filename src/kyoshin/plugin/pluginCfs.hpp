#pragma once

#include "monolib/vm/yvm2.h"

// Plugin script functions for the CFS (Common File System / script) subsystem.
// These are registered via pluginCfsRegist() and called from the VM.

extern "C" bool func_8004A2E8();
extern "C" void func_8004A400();

extern "C" void pluginCfsRegist();

// ml::CVec3 with the retail static zero object (symbol zero__Q22ml5CVec3).
namespace ml {
struct CVec3 {
    float x, y, z;
    static CVec3 zero;
};
} // namespace ml

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
// fillers so the four real methods land at retail slots: setPos 0x9C,
// getPosPtr 0xAC, setScale 0xC4, getScale 0xCC (declared slot n sits at
// vtable offset 8+4n; RTTI consumes emitted slots 0,4).
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
    virtual void vf_90() = 0; virtual void setPos(ml::CVec3* pos); // vtable offset 0x9C
    virtual void vf_98() = 0; virtual void vf_9C() = 0;
    virtual void vf_A0() = 0; virtual ml::CVec3* getPosPtr(); // vtable offset 0xAC
    virtual void vf_A8() = 0; virtual void vf_AC() = 0;
    virtual void vf_B0() = 0; virtual void vf_B4() = 0;
    virtual void vf_B8() = 0; virtual void setScale(float scale); // vtable offset 0xC4
    virtual void vf_C0() = 0; virtual float getScale();          // vtable offset 0xCC
    virtual void vf_D0() = 0; virtual void vf_D4() = 0;
    virtual void vf_D8() = 0; virtual void vf_DC() = 0;
    virtual void vf_E0() = 0; virtual void vf_E4() = 0;
    virtual void vf_E8() = 0; virtual void vf_EC() = 0;
    virtual void vf_F0() = 0; virtual void vf_F4() = 0;
    virtual void vf_F8() = 0; virtual void vf_FC() = 0;
    virtual void vf_100() = 0; virtual void vf_104() = 0;
    virtual void vf_108() = 0; virtual void vf_10C() = 0;
    virtual void* getTask();   // vtable slot 0x110 (RTTI consumes slots 0,4)
};

} // namespace cf

// Offset view of the CfGameManager fields written by setWarpArea:
// the optional fade timer stored as a float at +0x148.
struct CfGameManagerWarpView {
    u8 field_0x0[0x148];
    float warpFade;  // +0x148
};

namespace cf {

// Dispatch-only shim for the CfGameManager vtable: RTTI consumes emitted
// slots 0 and 4, so declared slot n sits at vtable offset 8+4n. 84 pure
// fillers (vf_00..vf_14C) put notifyPreload at offset 0x158, matching how
// the plugin functions dispatch their preload notifier.
class CfGameManagerVt158 {
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
    virtual void vf_90() = 0;
    virtual void setPos(ml::CVec3* pos);       // vtable offset 0x9C
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
    virtual void vf_110() = 0; virtual void vf_114() = 0;
    virtual void vf_118() = 0; virtual void vf_11C() = 0;
    virtual void vf_120() = 0; virtual void vf_124() = 0;
    virtual void vf_128() = 0; virtual void vf_12C() = 0;
    virtual void vf_130() = 0; virtual void vf_134() = 0;
    virtual void vf_138() = 0; virtual void vf_13C() = 0;
    virtual void vf_140() = 0; virtual void vf_144() = 0;
    virtual void vf_148() = 0; virtual void vf_14C() = 0;
    virtual void notifyPreload(bool enable);   // vtable offset 0x158
};

} // namespace cf

// Item implementation singleton returned by CItem_initItemImplInstances(item).
// Dispatch-only shim: fillers keep the real virtuals at their retail vtable
// slots (RTTI consumes emitted slots 0,4; declared slot n lands at offset
// 8+4n) -- 0x10 = item-id getter, 0x30 = weapon-slot getter, 0x34 =
// weapon-slot setter, matching how the plugin functions call them.
class CItemImplInstance {
public:
    virtual void vf_00() = 0; virtual void vf_04() = 0;
    virtual int getItemId(void* slot);         // vtable slot 0x10
    virtual void vf_14() = 0; virtual void vf_18() = 0;
    virtual void vf_1C() = 0; virtual void vf_20() = 0;
    virtual void vf_24() = 0; virtual void vf_28() = 0;
    virtual void vf_2C() = 0;
    virtual int getWeaponSlot(void* slot);     // vtable slot 0x30
    virtual void setWeaponSlot(void* slot, int id); // vtable slot 0x34
};

