#pragma once

/**
 * @file code_800F42AC.hpp
 * @brief Class definition for the TU kyoshin/cf/code_800F42AC.
 *
 * This class contains methods from func_800F42AC through func_800F4798,
 * all of which are in the cf namespace and precede CfObjEnumList.
 *
 * FULL_MATCH symbols in this TU:
 *   func_800F477C -- getter for sub-object pointer at offset 0x0C
 */

#include "types.h"

namespace cf {

// Forward declaration for the sub-object type returned by func_800F477C.
// The returned object has fields at 0x42 (byte) and 0x78 (word flags).
struct CfUnknownSub;

/// Object class for code_800F42AC translation unit methods.
/// TODO: Rename once class identity is recovered via symbol recovery.
struct CfCode800F42AC {
    u8 pad_00[0xC];              // 0x00 - unknown padding
    CfUnknownSub* subObject;     // 0x0C - pointer to sub-object (CfGimmick or similar)
};

} // namespace cf

// ---------------------------------------------------------------------------
// Imported C-ABI symbols referenced by this TU (declared here so callers in
// code_800F42AC.cpp link to the retail addresses; the symbol map handles
// resolution).
// ---------------------------------------------------------------------------
extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);
extern "C" bool isGlobalCamFlagSet__Fi(int mask);
extern "C" int findObjectById__Fi(int id);
extern "C" void* func_8016FE34(void* src);
extern "C" int func_80148778(void* self, int id);
extern "C" void* func_80149154(void* self, u32 id);   // aligned with CAIAction.hpp (u32 param, per CBattleState.cpp def)
extern "C" void func_8009D018(u32 a, u32 b);
extern "C" void* func_80141270(u32 a);
// func_801412D0 is owned by kyoshin/CUIWindowManager.hpp.


// ---------------------------------------------------------------------------
// sdata2 floating point constants referenced via sda21/@sda21 by this TU.
// (f32/f64 small-data literals; values live in data, only the offset is
// encoded in the instruction.)
// ---------------------------------------------------------------------------
extern const f32 lbl_eu_80666E90; // 1.0f
extern const f64 lbl_eu_80666E98; // rounding add - positive
extern const f64 lbl_eu_80666EA0; // rounding add - negative
extern const f64 lbl_eu_80666EA8; // int->float conversion magic (0x4330000080000000)
extern const f32 lbl_eu_80666EB0; // constant A

/// Sub-object accessed through the 0x0C pointer. Field at 0x7C is a float
/// (used for ratio/percent computations).
namespace cf {
struct CfUnknownSub {
    u8 pad_00[0x42];             // 0x00 - unknown padding
    u8 field_42;                 // 0x42
    u8 pad_43[0x35];             // 0x43 - 0x78
    u32 field_78;                // 0x78 - word flags
    f32 field_7C;                // 0x7C - float value
};

// Object returned by func_80149154 in func_800F42AC; float value at 0x20.
struct Sc149154Ret {
    u8 pad_00[0x20];             // 0x00
    f32 field_20;                // 0x20
};

// Node reached at p+8; func_80148778 / func_80149154 operate on &data_08.
struct Sc48778 {
    u8 pad_00[0x8];              // 0x00
    u8 data_08[0x8];             // 0x08
};
} // namespace cf

// ---------------------------------------------------------------------------
// Object layout shared by func_800F4424 / func_800F4648 / func_800F42AC /
// func_800F449C / func_800F46C0 (offsets through 0x830).
// ---------------------------------------------------------------------------
struct ScMain {
    u32 field_00;                // 0x00 - id/slot value
    void* field_04;              // 0x04 - object pointer
    u32 field_08;                // 0x08
    cf::CfUnknownSub* sub;       // 0x0C - sub-object
    u32 field_10;                // 0x10
    u32 field_14;                // 0x14
    u8 data_18[0x40];            // 0x18 - zeroed buffer (memset 0x40)
    u32 field_58;                // 0x58
    s32 field_5C;                // 0x5C - initialized to -1
    u32 field_60;                // 0x60
    f32 f_64;                    // 0x64
    f32 f_68;                    // 0x68
    f32 f_6C;                    // 0x6C
    f32 f_70;                    // 0x70
    f32 f_74;                    // 0x74
    f32 f_78;                    // 0x78
    f32 f_7C;                    // 0x7C
    u16 s_80;                    // 0x80
    u16 s_82;                    // 0x82
    u32 flags_84;                // 0x84 - status flags
    u32 field_88;                // 0x88
    u32 field_8C;                // 0x8C
    u16 s_90;                    // 0x90
    u8 pad_92[2];                // 0x92
    u8 data_94[0x34];            // 0x94 - zeroed buffer (memset 0x34)
    u32 field_C8;                // 0xC8

    /// One element of the 5-entry table at 0xCC (stride 0xBC).
    struct ScSub {
        u32 field_00;            // +0x00
        u32 field_04;            // +0x04
        u8 data_08[0x40];        // +0x08 - zeroed buffer (memset 0x40)
        u32 field_48;            // +0x48
        s32 field_4C;            // +0x4C - initialized to -1
        u32 field_50;            // +0x50
        f32 f_54;                // +0x54
        f32 f_58;                // +0x58
        f32 f_5C;                // +0x5C
        f32 f_60;                // +0x60
        f32 f_64;                // +0x64
        f32 f_68;                // +0x68
        f32 f_6C;                // +0x6C
        u16 s_70;                // +0x70
        u16 s_72;                // +0x72
        u32 field_74;            // +0x74
        u32 field_78;            // +0x78
        u32 field_7C;            // +0x7C
        u16 s_80;                // +0x80
        u8 pad_82[2];            // +0x82
        u8 data_84[0x34];        // +0x84 - zeroed buffer (memset 0x34)
        u32 field_B8;            // +0xB8
    };
    ScSub subs[5];               // 0xCC - 5-entry table

    u8 pad_478[0x824 - (0xCC + 5 * 0xBC)];  // 0x478 - 0x824
    u32 flags_824;               // 0x824 - main status flags
    u8 zero_828[8];              // 0x828 - zero-init buffer (memset 0x8)
    f32 field_830;               // 0x830 - float value
};

// ---------------------------------------------------------------------------
// Object for func_800F4798: a pointer table at offset 0x00 (indexed by the
// function's 2nd argument) followed by a count field at 0x60C.
// ---------------------------------------------------------------------------
struct Sc4798Item {
    u8 pad_00[0x14];             // 0x00
    f32 v_14;                    // 0x14 - value returned (negated when active)
};
struct Sc4798 {
    Sc4798Item* items[0x183];    // 0x000 - pointer table
    s32 field_60C;               // 0x60C - comparison flag
};

// ---------------------------------------------------------------------------
// Object for func_800F46C0. `this` gives a pointer at 0x04; the 2nd argument
// is a sub-object with an embedded polymorphic member at 0x3E9C and a pointer
// field at 0x3F10.
//
// VirtBase_3E9C is the polymorphic type embedded at 0x3E9C. Its vtable holds
// 19 preceding virtuals plus the query function at slot 19 (vtable offset
// 0x4C). The virtuals are declared inline-empty so no vtable is emitted for
// this class; calls resolve through the existing object's vtable.
// ---------------------------------------------------------------------------
class VirtBase_3E9C {
public:
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual void v5() {}
    virtual void v6() {}
    virtual void v7() {}
    virtual void v8() {}
    virtual void v9() {}
    virtual void v10() {}
    virtual void v11() {}
    virtual void v12() {}
    virtual void v13() {}
    virtual void v14() {}
    virtual void v15() {}
    virtual void v16() {}
    virtual void* v17();        // slot 17 -> vtable offset 0x44 (MWCC adds 2 leading slots)
};
struct Sc46C0Other {
    u8 pad_000[0x3E9C];          // 0x000
    class VirtBase_3E9C mSub;    // 0x3E9C - polymorphic member (4-byte vptr)
    u8 pad_3EA0[0x3F10 - (0x3E9C + sizeof(VirtBase_3E9C))];  // 0x3EA0 - 0x3F10
    void* field_3F10;            // 0x3F10
};