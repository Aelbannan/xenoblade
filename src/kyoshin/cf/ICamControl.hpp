#pragma once

/**
 * Real class tree for the camera-control pad interfaces.
 *
 * Retail vtable dump (US .data, from build/us/asm/kyoshin/cf/ICamControlRemote.s;
 * RTTI typestrs in .rodata 0x8050EBF0..0x8050EC48):
 *
 *   lbl_eu_80537F58, size 0x30 (RTTI lbl_eu_80662A00 "cf::ICamControlRemote",
 *     parent lbl_eu_80537F88 -> lbl_eu_80662A08 "cf::ICamControl"):
 *     +0x08 func_80274B28  +0x0C func_80274C20  +0x10 func_80274C68
 *     +0x14 func_80274B2C  +0x18 func_80274BA4  +0x1C func_80274CD4
 *     +0x20 func_80274D08  +0x24 func_80274D3C  +0x28 func_80274D70
 *     +0x2C func_80274DA4
 *   lbl_eu_80537F10, size 0x30 (RTTI lbl_eu_806629F8 "cf::ICamControlGc",
 *     parent lbl_eu_80537F40 -> "cf::ICamControl"):
 *     +0x08 func_80274B28  +0x0C func_80274C20  +0x10 func_80274C68
 *     +0x14 func_80274B2C  +0x18 func_80274BA4  +0x1C func_80274DD8
 *     +0x20 func_80274E0C  +0x24 func_80274E40  +0x28 func_80274E74
 *     +0x2C func_80274DA4
 *   lbl_eu_80537ED0, size 0x30 (RTTI lbl_eu_806629F0 "cf::ICamControlClassic",
 *     parent lbl_eu_80537F00 -> "cf::ICamControl"):
 *     +0x08 func_80274EA8  +0x0C func_80274EB0  +0x10 func_80274EE4
 *     +0x14 func_80274F34  +0x18 func_80274F94  +0x1C func_80274FF4
 *     +0x20 func_80275028  +0x24 func_8027505C  +0x28 func_80275090
 *     +0x2C func_802750C4
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
    virtual CPad* func_80274B28(); // vtable +0x08 - pad-status provider
    virtual u32 func_80274C20(); // vtable +0x0C
    virtual u32 func_80274C68(int controllerId); // vtable +0x10
    virtual u32 func_80274B2C(int controllerId); // vtable +0x14
    virtual u32 func_80274BA4(int controllerId); // vtable +0x18
    virtual u32 func_80274CD4(int) = 0; // vtable +0x1C - per-leaf
    virtual u32 func_80274D08(int) = 0; // vtable +0x20 - per-leaf
    virtual u32 func_80274D3C(int) = 0; // vtable +0x24 - per-leaf
    virtual u32 func_80274D70(int) = 0; // vtable +0x28 - per-leaf
    virtual u32 func_80274DA4(int); // vtable +0x2C
};

class __declspec(novtable) ICamControlRemote : public ICamControl {
public:
    virtual u32 func_80274CD4(int); // vtable +0x1C override
    virtual u32 func_80274D08(int); // vtable +0x20 override
    virtual u32 func_80274D3C(int); // vtable +0x24 override
    virtual u32 func_80274D70(int); // vtable +0x28 override
};

class __declspec(novtable) ICamControlGc : public ICamControl {
public:
    virtual u32 func_80274CD4(int); // vtable +0x1C override (retail DD8 body)
    virtual u32 func_80274D08(int); // vtable +0x20 override (retail E0C body)
    virtual u32 func_80274D3C(int); // vtable +0x24 override (retail E40 body)
    virtual u32 func_80274D70(int); // vtable +0x28 override (retail E74 body)
};

class __declspec(novtable) ICamControlClassic : public ICamControl {
public:
    virtual CPad* func_80274B28(); // vtable +0x08 override (retail EA8 body)
    virtual u32 func_80274C20(); // vtable +0x0C override (retail EB0 body)
    virtual u32 func_80274C68(int controllerId); // +0x10 (retail EE4 body)
    virtual u32 func_80274B2C(int controllerId); // +0x14 (retail F34 body)
    virtual u32 func_80274BA4(int controllerId); // +0x18 (retail F94 body)
    virtual u32 func_80274CD4(int); // vtable +0x1C override (retail FF4 body)
    virtual u32 func_80274D08(int); // vtable +0x20 override (retail 5028 body)
    virtual u32 func_80274D3C(int); // vtable +0x24 override (retail 505C body)
    virtual u32 func_80274D70(int); // vtable +0x28 override (retail 5090 body)
    virtual u32 func_80274DA4(int); // vtable +0x2C override (retail 50C4 body)
};

} // namespace cf
