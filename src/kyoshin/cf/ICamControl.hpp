#pragma once

/**
 * Real class tree for the camera-control pad interfaces.
 *
 * Retail vtable dump (US .data, from build/us/asm/kyoshin/cf/ICamControlRemote.s;
 * RTTI typestrs in .rodata 0x8050EBF0..0x8050EC48):
 *
 *   lbl_eu_80537F58, size 0x30 (RTTI lbl_eu_80662A00 "cf::ICamControlRemote",
 *     parent lbl_eu_80537F88 -> lbl_eu_80662A08 "cf::ICamControl"):
 *     +0x08 fetchPad  +0x0C isShortPress11  +0x10 isPressed12
 *     +0x14 isPadBit4  +0x18 isPadBit5  +0x1C isHeld2
 *     +0x20 isHeld3  +0x24 isHeld0  +0x28 isHeld1
 *     +0x2C isLongHold11
 *   lbl_eu_80537F10, size 0x30 (RTTI lbl_eu_806629F8 "cf::ICamControlGc",
 *     parent lbl_eu_80537F40 -> "cf::ICamControl"):
 *     +0x08 fetchPad  +0x0C isShortPress11  +0x10 isPressed12
 *     +0x14 isPadBit4  +0x18 isPadBit5  +0x1C CamGc_HeldBit19_DD8
 *     +0x20 CamGc_HeldBit20_E0C  +0x24 CamGc_HeldBit17_E40  +0x28 CamGc_HeldBit18_E74
 *     +0x2C isLongHold11
 *   lbl_eu_80537ED0, size 0x30 (RTTI lbl_eu_806629F0 "cf::ICamControlClassic",
 *     parent lbl_eu_80537F00 -> "cf::ICamControl"):
 *     +0x08 camClassicGetMainPad  +0x0C camClassicPressedBit4  +0x10 camClassicHeldPressedCombo
 *     +0x14 camClassicTriStateA  +0x18 camClassicTriStateB  +0x1C camClassicHeldBit12
 *     +0x20 camClassicHeldBit11  +0x24 camClassicHeldBit14  +0x28 camClassicHeldBit13
 *     +0x2C camClassicChainedOrBits
 *
 * Every table is RTTI + 0 + ten virtuals, and every leaf RTTI names
 * cf::ICamControl as its direct parent, so the tree is a flat 10-slot base
 * with three direct leaves. Slots 0-4 and 9 share bodies between Remote and
 * Gc, so they are the base implementations (defined once, in
 * ICamControlRemote.cpp); slots 5-8 differ per leaf, so they are pure on the
 * base and overridden in each leaf TU. The per-slot name below is the Remote
 * table's word (Remote is the default pad, index 0 in initCamControlInstances);
 * Gc/Classic bodies keep their logic under that name as true overrides.
 *
 * All classes are novtable: retail instances live in .data/.sbss, never
 * constructed from these TUs, so no TU may emit a __vt__ (same pattern as
 * cf::CHelp in kyoshin/help/CHelp.hpp).
 */

#include <types.h>

class CPad;

namespace cf {

class __declspec(novtable) ICamControl {
public:
    virtual CPad* fetchPad(); // vtable +0x08 - pad-status provider
    virtual u32 isShortPress11(); // vtable +0x0C
    virtual u32 isPressed12(int controllerId); // vtable +0x10
    virtual u32 isPadBit4(int controllerId); // vtable +0x14
    virtual u32 isPadBit5(int controllerId); // vtable +0x18
    virtual u32 isHeld2(int) = 0; // vtable +0x1C - per-leaf
    virtual u32 isHeld3(int) = 0; // vtable +0x20 - per-leaf
    virtual u32 isHeld0(int) = 0; // vtable +0x24 - per-leaf
    virtual u32 isHeld1(int) = 0; // vtable +0x28 - per-leaf
    virtual u32 isLongHold11(int); // vtable +0x2C
};

class __declspec(novtable) ICamControlRemote : public ICamControl {
public:
    virtual u32 isHeld2(int); // vtable +0x1C override
    virtual u32 isHeld3(int); // vtable +0x20 override
    virtual u32 isHeld0(int); // vtable +0x24 override
    virtual u32 isHeld1(int); // vtable +0x28 override
};

class __declspec(novtable) ICamControlGc : public ICamControl {
public:
    virtual u32 isHeld2(int); // vtable +0x1C override (retail DD8 body)
    virtual u32 isHeld3(int); // vtable +0x20 override (retail E0C body)
    virtual u32 isHeld0(int); // vtable +0x24 override (retail E40 body)
    virtual u32 isHeld1(int); // vtable +0x28 override (retail E74 body)
};

class __declspec(novtable) ICamControlClassic : public ICamControl {
public:
    virtual CPad* fetchPad(); // vtable +0x08 override (retail EA8 body)
    virtual u32 isShortPress11(); // vtable +0x0C override (retail EB0 body)
    virtual u32 isPressed12(int controllerId); // +0x10 (retail EE4 body)
    virtual u32 isPadBit4(int controllerId); // +0x14 (retail F34 body)
    virtual u32 isPadBit5(int controllerId); // +0x18 (retail F94 body)
    virtual u32 isHeld2(int); // vtable +0x1C override (retail FF4 body)
    virtual u32 isHeld3(int); // vtable +0x20 override (retail 5028 body)
    virtual u32 isHeld0(int); // vtable +0x24 override (retail 505C body)
    virtual u32 isHeld1(int); // vtable +0x28 override (retail 5090 body)
    virtual u32 isLongHold11(int); // vtable +0x2C override (retail 50C4 body)
};

} // namespace cf
