#pragma once

#include <types.h>

namespace cf {

// ---------------------------------------------------------------------------
// vtable dispatch views (never instantiated; no vtables emitted).
// With -RTTI on, MWCC reserves two leading vtable slots (offset-to-top +
// typeinfo), so the virtual at declared index N sits at vtable offset (N+2)*4.
// ---------------------------------------------------------------------------

// Object at CfObjectImplNpc14::field_98: vtable slots 0x28 (message string +
// flag) and 0x5c (float) are invoked by this TU.
class CfObjectImplNpc98 {
public:
    virtual void n00() = 0;                              // index 0
    virtual void n04() = 0;                              // index 1
    virtual void n08() = 0;                              // index 2
    virtual void n0C() = 0;                              // index 3
    virtual void n10() = 0;                              // index 4
    virtual void n14() = 0;                              // index 5
    virtual void n18() = 0;                              // index 6
    virtual void n1C() = 0;                              // index 7
    virtual void vf28(const char* str, int flag) = 0;    // index 8 -> vtable offset 0x28
    virtual void n2C() = 0;                              // index 9
    virtual void n30() = 0;                              // index 10
    virtual void n34() = 0;                              // index 11
    virtual void n38() = 0;                              // index 12
    virtual void n3C() = 0;                              // index 13
    virtual void n40() = 0;                              // index 14
    virtual void n44() = 0;                              // index 15
    virtual void n48() = 0;                              // index 16
    virtual void n4C() = 0;                              // index 17
    virtual void n50() = 0;                              // index 18
    virtual void n54() = 0;                              // index 19
    virtual void n58() = 0;                              // index 20
    virtual void vf5C(float v) = 0;                      // index 21 -> vtable offset 0x5c
};

// Object at CfObjectImplNpc::field_14: flag word at +0x64 (bit 0 tested),
// packed token at +0x70, id/flag at +0x90, message sub-object at +0x98.
struct CfObjectImplNpc14 {
    u8 _pad00[0x64];                                     // 0x00-0x63
    u32 field_64;                                        // 0x64
    u8 _pad68[0x70 - 0x68];                              // 0x68-0x6F
    u32 field_70;                                        // 0x70
    u8 _pad74[0x90 - 0x74];                              // 0x74-0x8F
    u32 field_90;                                        // 0x90
    u8 _pad94[0x98 - 0x94];                              // 0x94-0x97
    CfObjectImplNpc98* field_98;                         // 0x98
};

// NPC implementation object. Fields at +0x14 / +0x28 / +0x368; vtable slots
// 0x30 (command dispatch) and 0xDC are invoked by this TU.
class CfObjectImplNpc {
public:
    virtual ~CfObjectImplNpc();                          // index 0 -> vtable offset 0x08
    virtual void vf04() = 0;                             // index 1
    virtual void vf08() = 0;                             // index 2
    virtual void vf0C() = 0;                             // index 3
    virtual void vf10() = 0;                             // index 4
    virtual void vf14() = 0;                             // index 5
    virtual void vf18() = 0;                             // index 6
    virtual void vf1C() = 0;                             // index 7
    virtual void vf20() = 0;                             // index 8
    virtual void vf24() = 0;                             // index 9
    virtual void vf30(u32 a, u32 b) = 0;                 // index 10 -> vtable offset 0x30
    virtual void vf34() = 0;                             // index 11
    virtual void vf38() = 0;                             // index 12
    virtual void vf3C() = 0;                             // index 13
    virtual void vf40() = 0;                             // index 14
    virtual void vf44() = 0;                             // index 15
    virtual void vf48() = 0;                             // index 16
    virtual void vf4C() = 0;                             // index 17
    virtual void vf50() = 0;                             // index 18
    virtual void vf54() = 0;                             // index 19
    virtual void vf58() = 0;                             // index 20
    virtual void vf5C() = 0;                             // index 21
    virtual void vf60() = 0;                             // index 22
    virtual void vf64() = 0;                             // index 23
    virtual void vf68() = 0;                             // index 24
    virtual void vf6C() = 0;                             // index 25
    virtual void vf70() = 0;                             // index 26
    virtual void vf74() = 0;                             // index 27
    virtual void vf78() = 0;                             // index 28
    virtual void vf7C() = 0;                             // index 29
    virtual void vf80() = 0;                             // index 30
    virtual void vf84() = 0;                             // index 31
    virtual void vf88() = 0;                             // index 32
    virtual void vf8C() = 0;                             // index 33
    virtual void vf90() = 0;                             // index 34
    virtual void vf94() = 0;                             // index 35
    virtual void vf98() = 0;                             // index 36
    virtual void vf9C() = 0;                             // index 37
    virtual void vfA0() = 0;                             // index 38
    virtual void vfA4() = 0;                             // index 39
    virtual void vfA8() = 0;                             // index 40
    virtual void vfAC() = 0;                             // index 41
    virtual void vfB0() = 0;                             // index 42
    virtual void vfB4() = 0;                             // index 43
    virtual void vfB8() = 0;                             // index 44
    virtual void vfBC() = 0;                             // index 45
    virtual void vfC0() = 0;                             // index 46
    virtual void vfC4() = 0;                             // index 47
    virtual void vfC8() = 0;                             // index 48
    virtual void vfCC() = 0;                             // index 49
    virtual void vfD0() = 0;                             // index 50
    virtual void vfD4() = 0;                             // index 51
    virtual void vfD8() = 0;                             // index 52
    virtual void vfDC() = 0;                             // index 53 -> vtable offset 0xDC

    u8* func_800CA924();

    u8 _pad04[0x10];                                     // 0x04-0x13
    CfObjectImplNpc14* field_14;                         // 0x14
    u8 _pad18[0x10];                                     // 0x18-0x27
    u8 field_28[0x40];                                   // 0x28-0x67 (voice sub-object; address taken)
    u8 mUnk_0x68[4];                                     // 0x68-0x6B (returned by func_800CA924)
    u8 _pad6C[0x368 - 0x6C];                             // 0x6C-0x367
    s16 field_368;                                       // 0x368 (talk index)
};

} // namespace cf

// C-ABI imports (retail symbols are unmangled - keep linkage/signatures
// verbatim; same scheme as CfObjectImplPc.hpp / CfObjectImplEne.hpp).
extern "C" {
void func_800CA948(void* self);
void func_802A0B8C(void* self, void* owner);
void func_802A0E08(void* self);
void func_800AA318(u32 packed, u32* out0, u32* out1, u32* out2, u32* out3);
char* func_800AA5C0(void* handle);
void func_800CEBE0(void* self);
}

// rodata/data imports referenced by this unit (global scope: not mangled).
extern char lbl_eu_804FC7EC[];
extern f32 lbl_eu_804FC7B0[];
extern const char* lbl_eu_8052AA00[];
