#pragma once

#include <types.h>

// Walker object + move-sub view, split out of CfObjectImplWalker.hpp so
// TUs that only need the classes (pluginUi/pluginBtl) do not inherit
// that header's C++-linkage function decls (they clash with the
// canonical extern "C" decls). Single definition: CfObjectImplWalker.hpp
// includes this file.

namespace cf {

class CfWalkField04;
class CfWalkField14;
class CfWalkBattleObj;
class CfWalkFieldE4;
struct CfWalkMovePos;

// Walker interface object.
// are declared; with -RTTI on MWCC reserves two leading vtable slots
// (offset-to-top + typeinfo), so declared index N sits at vtable offset
// (N+2)*4. The class is never instantiated here, so no vtable is emitted.
class CfObjectImplWalker {
public:
    virtual ~CfObjectImplWalker();              // index 0 -> vtable 0x8

    virtual void vf0C();                        // index 1
    virtual void vf10();                        // index 2
    virtual void vf14();                        // index 3
    virtual void vf18();                        // index 4
    virtual void vf1C();                        // index 5
    virtual void vf20();                        // index 6
    virtual void vf24();                        // index 7
    virtual void vf28();                        // index 8
    virtual void vf2C();                        // index 9
    virtual void vf30(u32 a, u32 b);            // index 10 -> vtable 0x30
    virtual void vf34(u32 a);                    // index 11 -> vtable 0x34
    virtual void vf38();                        // index 12
    virtual void vf3C();                        // index 13
    virtual u32 vf40(u32 a);                    // index 14 -> vtable 0x40
    virtual void vf44();                        // index 15
    virtual void* vf48();                       // index 16 -> vtable 0x48
    virtual void vf4C();                        // index 17
    virtual void vf50();                        // index 18
    virtual void vf54();                        // index 19
    virtual void vf58();                        // index 20
    virtual void vf5C();                        // index 21
    virtual void vf60();                        // index 22
    virtual void vf64();                        // index 23 -> vtable 0x64
    virtual void vf68();                        // index 24
    virtual void vf6C();                        // index 25
    virtual void vf70(u32 a);                    // index 26
    virtual void vf74();                        // index 27
    virtual void vf78();                        // index 28
    virtual void vf7C();                        // index 29
    virtual void vf80();                        // index 30
    virtual void vf84(u32 arg);                 // index 31 -> vtable 0x84
    virtual void vf88();                        // index 32
    virtual void vf8C();                        // index 33
    virtual void vf90();                        // index 34
    virtual void vf94();                        // index 35
    virtual void vf98();                        // index 36
    virtual u32 vf9C();                        // index 37 -> vtable 0x9C
    virtual void vfA0();                        // index 38
    virtual void vfA4();                        // index 39
    virtual void vfA8();                        // index 40
    virtual void vfAC();                        // index 41
    virtual void vfB0();                        // index 42
    virtual void vfB4();                        // index 43
    virtual void vfB8();                        // index 44
    virtual void vfBC();                        // index 45
    virtual void vfC0();                        // index 46
    virtual void vfC4();                        // index 47
    virtual void vfC8();                        // index 48
    virtual void vfCC();                        // index 49
    virtual void vfD0();                        // index 50
    virtual void vfD4();                        // index 51 -> vtable 0xD4
    virtual void vfD8();                        // index 52 -> vtable 0xD8
    virtual void vfDC();                        // index 53 -> vtable 0xDC
    virtual void vfE0();                        // index 54
    virtual class CfWalkFieldE4* vfE4();       // index 55 -> vtable 0xE4
    virtual void vfE8();                        // index 56
    virtual void vfEC();                        // index 57
    virtual void vfF0();                        // index 58
    virtual void vfF4();                        // index 59
    virtual void vfF8();                        // index 60
    virtual void vfFC();                        // index 61
    virtual void vf100();                       // index 62 -> vtable 0x100
    virtual void vf104();                       // index 63 -> vtable 0x104

    void* Walker_InnerPtr68();
    u32 Walker_GetField380();

    // Layout fields (vptr implicit at 0x00 from the virtuals above)
    class CfWalkField04* field_04;             // 0x04
    u8 _pad08[0x14 - 0x08];                    // 0x08-0x13
    class CfWalkField14* field_14;             // 0x14
    class CfWalkBattleObj* field_18;           // 0x18 (battle object base)
    u8 _pad1C[0x28 - 0x1C];                    // 0x1C-0x27
    u8 field_28[0x344];                        // 0x28-0x36B (opaque; address taken)
    u8 _pad36C[0x380 - 0x36C];                 // 0x36C-0x37F
    u32 field_380;                             // 0x380
    u32 field_384;                             // 0x384
    u32 field_388;                             // 0x388
    u32 field_38C;                             // 0x38C
    u32 field_390;                             // 0x390
    u32 field_394;                             // 0x394
    u32 field_398;                             // 0x398
    u8  field_39C;                             // 0x39C
    u8  _pad39D[0x3A0 - 0x39D];                // 0x39D-0x39F
    f32 field_3A0;                             // 0x3A0
};

// Move sub-object embedded at +0x3E9C of the battle object (secondary vtable).
class CfWalkMoveSub {
public:
    virtual void m08(u32 a);                   // index 0 -> vtable 0x8
    virtual u32 m0C(u32 a);                    // index 1 -> vtable 0xC
    virtual void m10(u32 a);                   // index 2 -> vtable 0x10
    virtual void m14();                        // index 3
    virtual void m18();                        // index 4
    virtual void m1C();                        // index 5
    virtual void m20();                        // index 6
    virtual void m24();                        // index 7
    virtual void m28();                        // index 8
    virtual void m2C();                        // index 9
    virtual void m30();                        // index 10
    virtual void m34();                        // index 11
    virtual void m38();                        // index 12
    virtual void m3C();                        // index 13
    virtual void m40();                        // index 14
    virtual void m44();                        // index 15
    virtual void m48();                        // index 16
    virtual u32 m4C();                        // index 17 -> vtable 0x4c
    virtual void m50(u32 a);                   // index 18 -> vtable 0x50
    virtual void m54();                        // index 19
    virtual void m58();                        // index 20
    virtual void m5C();                        // index 21
    virtual void m60();                        // index 22
    virtual void m64();                        // index 23
    virtual void m68();                        // index 24
    virtual void m6C();                        // index 25
    virtual void m70();                        // index 26
    virtual void m74();                        // index 27
    virtual void m78();                        // index 28
    virtual void m7C();                        // index 29
    virtual void m80();                        // index 30
    virtual void m84();                        // index 31
    virtual void m88();                        // index 32
    virtual void m8C();                        // index 33
    virtual void m90();                        // index 34
    virtual void m94();                        // index 35
    virtual void m98();                        // index 36
    virtual void m9C(void* a);                 // index 37 -> vtable 0x9C
    virtual void mA0();                        // index 38
    virtual void mA4();                        // index 39
    virtual void mA8();                        // index 40
    virtual void* mAC();                       // index 41 -> vtable 0xAC
    virtual void mB0();                        // index 42
    virtual void mB4();                        // index 43
    virtual void mB8();                        // index 44
    virtual void mBC();                        // index 45
    virtual void mC0();                        // index 46
    virtual void mC4();                        // index 47
    virtual void mC8();                        // index 48
    virtual void mCC();                        // index 49
    virtual void mD0();                        // index 50
    virtual void mD4();                        // index 51
    virtual void mD8();                        // index 52
    virtual void mDC();                        // index 53
    virtual void mE0();                        // index 54
    virtual void mE4();                        // index 55
    virtual void mE8();                        // index 56
    virtual void mEC();                        // index 57
    virtual void mF0();                        // index 58
    virtual void mF4();                        // index 59
    virtual void mF8();                        // index 60
    virtual void mFC();                        // index 61
    virtual void m100();                       // index 62
    virtual void m104();                       // index 63
    virtual void m108();                       // index 64
    virtual void m10C(u32 a);                  // index 65 -> vtable 0x10c
    virtual void* m110();                      // index 66 -> vtable 0x110 (returns this)
    virtual void m114();                       // index 67
    virtual void m118();                       // index 68
    virtual void m11C();                       // index 69
    virtual void m120();                       // index 70
    virtual void m124();                       // index 71
    virtual void m128();                       // index 72
    virtual void m12C();                       // index 73
    virtual void m130();                       // index 74
    virtual void m134(f32 a);                  // index 75 -> vtable 0x134
    virtual struct CfWalkMovePos* m138();      // index 76 -> vtable 0x138 (returns pos block)
    virtual void m13C();                       // index 77
    virtual void m140();                       // index 78
    virtual void m144();                       // index 79
    virtual void m148();                       // index 80
    virtual void m14C();                       // index 81
    virtual void m150();                       // index 82
    virtual void m154();                       // index 83
    virtual void m158();                       // index 84
    virtual void m15C();                       // index 85
    virtual void m160();                       // index 86
    virtual void m164();                       // index 87
    virtual void m168();                       // index 88
    virtual void m16C();                       // index 89
    virtual void m170();                       // index 90
    virtual void m174();                       // index 91
    virtual void m178();                       // index 92
    virtual void m17C();                       // index 93
    virtual void m180();                       // index 94
    virtual void m184();                       // index 95
    virtual void m188();                       // index 96
    virtual void m18C();                       // index 97
    virtual void m190();                       // index 98
    virtual void m194();                       // index 99
    virtual void m198();                       // index 100
    virtual void m19C();                       // index 101
    virtual void m1A0();                       // index 102
    virtual void m1A4();                       // index 103
    virtual void m1A8();                       // index 104
    virtual void m1AC(u32 a, const char* b);   // index 105 -> vtable 0x1AC
    virtual void m1B0();                       // index 106
    virtual void m1B4();                       // index 107
    virtual void m1B8();                       // index 108
    virtual void m1BC();                       // index 109
    virtual void m1C0();                       // index 110
    virtual void m1C4();                       // index 111
    virtual void m1C8();                       // index 112
    virtual void m1CC();                       // index 113
    virtual void m1D0();                       // index 114
    virtual void m1D4(f32 a);                  // index 115 -> vtable 0x1D4

    u8 _pad04[0x38 - 0x04];                    // 0x04-0x37
    void* field_38;                            // 0x38 (battle-object alias at +0x3ED4)
    u8 _pad3C[0x68 - 0x3C];                    // 0x3C-0x67
    u32 field_68;                              // 0x68 (battle-object view alias at +0x3F04)
    u8 _pad6C[0x74 - 0x6C];                    // 0x6C-0x73
    u32 field_74;                              // 0x74
    u8 _pad78[0x8C - 0x78];                    // 0x78-0x8B
    u16 field_8C;                              // 0x8C
    u8 _pad8E[0xC4 - 0x8E];                    // 0x8E-0xC3 (deeper fields undeclared)
};

// Position block returned by the move sub-object's m138() (vtable 0x138).
struct CfWalkMovePos {
    f32 field_0;                               // 0x0
};
} // namespace cf
