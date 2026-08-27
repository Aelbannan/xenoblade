#pragma once

#include <types.h>
#include "monolib/math/CMat34.hpp"
#include "monolib/core/CSchedule.hpp"

// Effect object managed by CPcEffect07. Layout mirrors the CfObject family:
// vtable slots 0x9C (SetPos) and 0xBC (SetRot) take a CVec3&, flags at 0x68,
// aux field at 0xB0. Never instantiated in this TU, so the declared virtuals
// need no definitions and no vtable is emitted.
class CPcEffect07Obj {
public:
    virtual ~CPcEffect07Obj();            // 0x00
    virtual void vf04();                  // 0x04
    virtual void vf08();                  // 0x08
    virtual void vf0C();                  // 0x0C
    virtual void vf10();                  // 0x10
    virtual void vf14();                  // 0x14
    virtual void vf18();                  // 0x18
    virtual void vf1C();                  // 0x1C
    virtual void vf20();                  // 0x20
    virtual void vf24();                  // 0x24
    virtual void vf28();                  // 0x28
    virtual void vf2C();                  // 0x2C
    virtual void vf30();                  // 0x30
    virtual void vf34();                  // 0x34
    virtual void vf38();                  // 0x38
    virtual void vf3C();                  // 0x3C
    virtual void vf40();                  // 0x40
    virtual void vf44();                  // 0x44
    virtual void vf48();                  // 0x48
    virtual void vf4C();                  // 0x4C
    virtual void vf50();                  // 0x50
    virtual void vf54();                  // 0x54
    virtual void vf58();                  // 0x58
    virtual void vf5C();                  // 0x5C
    virtual void vf60();                  // 0x60
    virtual void vf64();                  // 0x64
    virtual void vf68();                  // 0x68
    virtual void vf6C();                  // 0x6C
    virtual void vf70();                  // 0x70
    virtual void vf74();                  // 0x74
    virtual void vf78();                  // 0x78
    virtual void vf7C();                  // 0x7C
    virtual void vf80();                  // 0x80
    virtual void vf84();                  // 0x84
    virtual void vf88();                  // 0x88
    virtual void vf8C();                  // 0x8C
    virtual void vf90();                  // 0x90
    virtual void SetPos(const ml::CVec3& pos);  // 0x9C
    virtual void vfA0();                  // 0xA0
    virtual void vfA4();                  // 0xA4
    virtual void vfA8();                  // 0xA8
    virtual void vfAC();                  // 0xAC
    virtual void vfB0();                  // 0xB0
    virtual void vfB4();                  // 0xB4
    virtual void vfB8();                  // 0xB8
    virtual void SetRot(const ml::CVec3& rot);  // 0xBC

    u8 _04[0x64];        // 0x04-0x67
    u32 mFlags68;        // 0x68
    u8 _6C[0x44];        // 0x6C-0xAF
    u32 mFieldB0;        // 0xB0
};

// u32 word-pair / f64 view for MWCC's 0x43300000 int->float conversion;
// subtract the sdata2 magic double lbl_eu_80667DF8 so the pool reloc matches
// retail instead of an MWCC @N constant.
union PcEffectF64Conv {
    u32 w[2];
    f64 d;
};

// Holder returned by the schedule sub-object's slot fetch; +0x94 points at the
// fixed-timestep CSchedule driven by func_804E3CDC.
struct PcEffectSchedHolder {
    u8 _00[0x94];
    CSchedule* mSched;   // 0x94
};

// Virtual-dispatch shim for the embedded player sub-object at +0x3E9C of the
// actor object at CPcEffect07::mField0C. Only vtable slot 0x220 (fetch the
// schedule holder for a slot id) is called from this TU; declaring it as a
// real virtual makes MWCC emit the retail lwzu + r12 virtual-call sequence.
// Never instantiated, so no vtable is emitted.
class PcEffectScheduleSub {
public:
    virtual void f00();   // 0x00
    virtual void f04();   // 0x04
    virtual void f08();   // 0x08
    virtual void f0C();   // 0x0C
    virtual void f10();   // 0x10
    virtual void f14();   // 0x14
    virtual void f18();   // 0x18
    virtual void f1C();   // 0x1C
    virtual void f20();   // 0x20
    virtual void f24();   // 0x24
    virtual void f28();   // 0x28
    virtual void f2C();   // 0x2C
    virtual void f30();   // 0x30
    virtual void f34();   // 0x34
    virtual void f38();   // 0x38
    virtual void f3C();   // 0x3C
    virtual void f40();   // 0x40
    virtual void f44();   // 0x44
    virtual void f48();   // 0x48
    virtual void f4C();   // 0x4C
    virtual void f50();   // 0x50
    virtual void f54();   // 0x54
    virtual void f58();   // 0x58
    virtual void f5C();   // 0x5C
    virtual void f60();   // 0x60
    virtual void f64();   // 0x64
    virtual void f68();   // 0x68
    virtual void f6C();   // 0x6C
    virtual void f70();   // 0x70
    virtual void f74();   // 0x74
    virtual void f78();   // 0x78
    virtual void f7C();   // 0x7C
    virtual void f80();   // 0x80
    virtual void f84();   // 0x84
    virtual void f88();   // 0x88
    virtual void f8C();   // 0x8C
    virtual void f90();   // 0x90
    virtual void f94();   // 0x94
    virtual void f98();   // 0x98
    virtual void f9C();   // 0x9C
    virtual void fA0();   // 0xA0
    virtual void fA4();   // 0xA4
    virtual void fA8();   // 0xA8
    virtual void fAC();   // 0xAC
    virtual void fB0();   // 0xB0
    virtual void fB4();   // 0xB4
    virtual void fB8();   // 0xB8
    virtual void fBC();   // 0xBC
    virtual void fC0();   // 0xC0
    virtual void fC4();   // 0xC4
    virtual void fC8();   // 0xC8
    virtual void fCC();   // 0xCC
    virtual void fD0();   // 0xD0
    virtual void fD4();   // 0xD4
    virtual void fD8();   // 0xD8
    virtual void fDC();   // 0xDC
    virtual void fE0();   // 0xE0
    virtual void fE4();   // 0xE4
    virtual void fE8();   // 0xE8
    virtual void fEC();   // 0xEC
    virtual void fF0();   // 0xF0
    virtual void fF4();   // 0xF4
    virtual void fF8();   // 0xF8
    virtual void fFC();   // 0xFC
    virtual void f100();  // 0x100
    virtual void f104();  // 0x104
    virtual void f108();  // 0x108
    virtual void f10C();  // 0x10C
    virtual void f110();  // 0x110
    virtual void f114();  // 0x114
    virtual void f118();  // 0x118
    virtual void f11C();  // 0x11C
    virtual void f120();  // 0x120
    virtual void f124();  // 0x124
    virtual void f128();  // 0x128
    virtual void f12C();  // 0x12C
    virtual void f130();  // 0x130
    virtual void f134();  // 0x134
    virtual void f138();  // 0x138
    virtual void f13C();  // 0x13C
    virtual void f140();  // 0x140
    virtual void f144();  // 0x144
    virtual void f148();  // 0x148
    virtual void f14C();  // 0x14C
    virtual void f150();  // 0x150
    virtual void f154();  // 0x154
    virtual void f158();  // 0x158
    virtual void f15C();  // 0x15C
    virtual void f160();  // 0x160
    virtual void f164();  // 0x164
    virtual void f168();  // 0x168
    virtual void f16C();  // 0x16C
    virtual void f170();  // 0x170
    virtual void f174();  // 0x174
    virtual void f178();  // 0x178
    virtual void f17C();  // 0x17C
    virtual void f180();  // 0x180
    virtual void f184();  // 0x184
    virtual void f188();  // 0x188
    virtual void f18C();  // 0x18C
    virtual void f190();  // 0x190
    virtual void f194();  // 0x194
    virtual void f198();  // 0x198
    virtual void f19C();  // 0x19C
    virtual void f1A0();  // 0x1A0
    virtual void f1A4();  // 0x1A4
    virtual void f1A8();  // 0x1A8
    virtual void f1AC();  // 0x1AC
    virtual void f1B0();  // 0x1B0
    virtual void f1B4();  // 0x1B4
    virtual void f1B8();  // 0x1B8
    virtual void f1BC();  // 0x1BC
    virtual void f1C0();  // 0x1C0
    virtual void f1C4();  // 0x1C4
    virtual void f1C8();  // 0x1C8
    virtual void f1CC();  // 0x1CC
    virtual void f1D0();  // 0x1D0
    virtual void f1D4();  // 0x1D4
    virtual void f1D8();  // 0x1D8
    virtual void f1DC();  // 0x1DC
    virtual void f1E0();  // 0x1E0
    virtual void f1E4();  // 0x1E4
    virtual void f1E8();  // 0x1E8
    virtual void f1EC();  // 0x1EC
    virtual void f1F0();  // 0x1F0
    virtual void f1F4();  // 0x1F4
    virtual void f1F8();  // 0x1F8
    virtual void f1FC();  // 0x1FC
    virtual void f200();  // 0x200
    virtual void f204();  // 0x204
    virtual void f208();  // 0x208
    virtual void f20C();  // 0x20C
    virtual void f210();  // 0x210
    virtual void f214();  // 0x214
    // NOTE: retail's vtable layout puts the slot fetch at byte offset 0x220;
    // MWCC counts two extra slots for this class shape, so the trailing
    // placeholders are omitted to land GetSlot on the retail offset.
    virtual PcEffectSchedHolder* GetSlot(u16 id);
};

// View of the actor object at CPcEffect07::mField0C with the embedded
// schedule sub-object at +0x3E9C.
struct PcEffectActorView {
    u8 pad[0x3E9C];
    PcEffectScheduleSub mSub;  // +0x3E9C
};

// PcEffect07 effect entries (stride 0x18):
//   +0x00: effect object pointer
//   +0x04: active flag (s16)
//   +0x06: aux s16 cleared together with active
//   +0x08: transform matrix pointer
//   +0x0C: slot ID (u8)
struct PcEffectEntry {
    PcEffectEntry();
    /* 0x00 */ CPcEffect07Obj* mObj;
    /* 0x04 */ s16 mActive;
    /* 0x06 */ s16 mField06;
    /* 0x08 */ ml::CMat34* mMat;
    /* 0x0C */ u8 mId;
    /* 0x0D */ u8 _0D[3];
    /* 0x10 */ u32 _10;
    /* 0x14 */ u8 _14;
    /* 0x15 */ u8 _15[3];
};

// Backward-scan record view: the negative-index scan walks 0x18-strided
// records starting at data+0x30 (record 2), testing the s16 at +0x14 - which
// aliases each entry's mActive.
struct PcEffectScanRec {
    /* 0x00 */ u8 _00[0x14];
    /* 0x14 */ s16 mActive;
    /* 0x16 */ u8 _16[2];
};

// PcEffect07 state blob (lbl_eu_80664398). Three entries at stride 0x18;
// offset 0x58 = maxEffects count.
struct PcEffectData {
    u8 _00[0xC];
    PcEffectActorView* mActor;  // 0x0C (aliases CPcEffect07::mField0C)
    PcEffectEntry entries[3];  // 0x10-0x58
    u8 maxEffects;             // 0x58
    u8 _59[3];
    f32 mField5C;              // 0x5C
    f32 mField60;              // 0x60
};

// Factory view over the global bank object: vtable slot 0x28 creates an
// effect object for the given effect id (0x700+n).
class PcEffectFactoryView {
public:
    virtual void f00();  // 0x00
    virtual void f04();  // 0x04
    virtual void f08();  // 0x08
    virtual void f0C();  // 0x0C
    virtual void f10();  // 0x10
    virtual void f14();  // 0x14
    virtual void f18();  // 0x18
    virtual void f1C();  // 0x1C
    virtual void f20();  // 0x20
    virtual void f24();  // 0x24
    virtual CPcEffect07Obj* Create(u32 id, s32 a, s32 b, s32 c);  // 0x28
};

// View of the schedule sub-object for vtable slot 0x12C (fetch the transform
// holder for a slot record). Separate from PcEffectScheduleSub, whose layout
// targets slot 0x220; never instantiated so no vtable is emitted.
class PcEffectSchedFetchSub {
public:
    virtual void f000();  // 0x000
    virtual void f004();  // 0x004
    virtual void f008();  // 0x008
    virtual void f00C();  // 0x00C
    virtual void f010();  // 0x010
    virtual void f014();  // 0x014
    virtual void f018();  // 0x018
    virtual void f01C();  // 0x01C
    virtual void f020();  // 0x020
    virtual void f024();  // 0x024
    virtual void f028();  // 0x028
    virtual void f02C();  // 0x02C
    virtual void f030();  // 0x030
    virtual void f034();  // 0x034
    virtual void f038();  // 0x038
    virtual void f03C();  // 0x03C
    virtual void f040();  // 0x040
    virtual void f044();  // 0x044
    virtual void f048();  // 0x048
    virtual void f04C();  // 0x04C
    virtual void f050();  // 0x050
    virtual void f054();  // 0x054
    virtual void f058();  // 0x058
    virtual void f05C();  // 0x05C
    virtual void f060();  // 0x060
    virtual void f064();  // 0x064
    virtual void f068();  // 0x068
    virtual void f06C();  // 0x06C
    virtual void f070();  // 0x070
    virtual void f074();  // 0x074
    virtual void f078();  // 0x078
    virtual void f07C();  // 0x07C
    virtual void f080();  // 0x080
    virtual void f084();  // 0x084
    virtual void f088();  // 0x088
    virtual void f08C();  // 0x08C
    virtual void f090();  // 0x090
    virtual void f094();  // 0x094
    virtual void f098();  // 0x098
    virtual void f09C();  // 0x09C
    virtual void f0A0();  // 0x0A0
    virtual void f0A4();  // 0x0A4
    virtual void f0A8();  // 0x0A8
    virtual void f0AC();  // 0x0AC
    virtual void f0B0();  // 0x0B0
    virtual void f0B4();  // 0x0B4
    virtual void f0B8();  // 0x0B8
    virtual void f0BC();  // 0x0BC
    virtual void f0C0();  // 0x0C0
    virtual void f0C4();  // 0x0C4
    virtual void f0C8();  // 0x0C8
    virtual void f0CC();  // 0x0CC
    virtual void f0D0();  // 0x0D0
    virtual void f0D4();  // 0x0D4
    virtual void f0D8();  // 0x0D8
    virtual void f0DC();  // 0x0DC
    virtual void f0E0();  // 0x0E0
    virtual void f0E4();  // 0x0E4
    virtual void f0E8();  // 0x0E8
    virtual void f0EC();  // 0x0EC
    virtual void f0F0();  // 0x0F0
    virtual void f0F4();  // 0x0F4
    virtual void f0F8();  // 0x0F8
    virtual void f0FC();  // 0x0FC
    virtual void f100();  // 0x100
    virtual void f104();  // 0x104
    virtual void f108();  // 0x108
    virtual void f10C();  // 0x10C
    virtual void f110();  // 0x110
    virtual void f114();  // 0x114
    virtual void f118();  // 0x118
    virtual void f11C();  // 0x11C
    virtual void f120();  // 0x120
    virtual void f124();  // 0x124
    virtual void f128();  // 0x128
    // Fetches the transform/schedule holder for a slot record (the caller's
    // PcEffectScanRec view of the entry array); result is stored as mMat.
    virtual PcEffectSchedHolder* GetHolder(u32 key);  // 0x12C
};

// Spawn-path record view: aliases entry i at bank + i*0x18 (entry starts at
// +0x10 of the record).
struct PcEffectSpawnRec {
    /* 0x10 */ CPcEffect07Obj* mObj;
    /* 0x14 */ s16 mActive;
    /* 0x16 */ u16 _16;
    /* 0x18 */ ml::CMat34* mMat;
    /* 0x1C */ u8 mId;
};

// Imports from other TUs used by this TU's free functions.
extern "C" void setTargetObj_(void* obj, void* target);
extern "C" void setChildB59__(void* obj, s8 val);
extern "C" void bindPartnerO_(CPcEffect07Obj* obj, PcEffectScheduleSub* sub, s32 flags);

// Effect-id -> wstring table index matrix (indexed by selector slot).
extern u32 lbl_eu_80533738[];
// sdata2 float constant (spawn cooldown reset value).
extern const f32 lbl_eu_80667DEC;

// External data symbols referenced by this TU (defined in other TUs).
extern PcEffectData* lbl_eu_80664398;   // PcEffect07 state (three effect slots)
extern u16 lbl_eu_805049F8[];           // wstring table of slot ids (index = id - 0x700)
extern const f32 lbl_eu_80667DF4;       // sdata2 float constant (schedule interval)
extern const f32 lbl_eu_80667DE8;       // sdata2 float constant (ctor initial value)
extern const f32 lbl_eu_80667DF0;       // sdata2 float constant (spawn reset value)
extern const f64 lbl_eu_80667DF8;       // sdata2 int->float conversion magic (0x4330000080000000)

// Spawn/attach an effect object into a slot (see CPcEffect07.cpp).
extern "C" s32 func_801B19F0(u8 type, s32 slotSel);

// 0x18-stride mat-slot view: the transform pointer of slot i sits at
// self + 0x18 + i*0x18 (overlaps PcEffectEntry.mMat).
struct PcEffectMatSlot {
    u8 _00[0x18];
    ml::CMat34* mMat;  // 0x18
};

// Twin vtable-pointer base subobjects (+0 / +4, retail lbl_eu_80533744 and
// lbl_eu_80533744+0x10). Ctors defined out-of-line in the .cpp so they inline
// into the CPcEffect07 constructor in retail order.
struct CPcEffect07VtA {
    void* mVT;
    CPcEffect07VtA();
};
struct CPcEffect07VtB {
    void* mVT;
    CPcEffect07VtB();
};

namespace cf {

class CPcEffect07 : public CPcEffect07VtA, public CPcEffect07VtB {
public:
    CPcEffect07(PcEffectActorView* actor);

    u32 mField08;                 // 0x08
    PcEffectActorView* mField0C;  // 0x0C: actor whose +0x3E9C sub-object fetches schedules
    PcEffectEntry mEntries[3];    // 0x10-0x58: effect entries (stride 0x18)
    u8 maxEffects;                // 0x58
    f32 mField5C;                 // 0x5C
    f32 mField60;                 // 0x60
    u8 _64[0x68 - 0x64];          // 0x64-0x67
};

} // namespace cf
