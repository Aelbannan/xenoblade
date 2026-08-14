#pragma once

#include <types.h>
#include <nw4r/math/math_types.h>
#include "monolib/math/CVec3.hpp"
#include "monolib/math/CMat34.hpp"
#include "monolib/math/CMat33.hpp"
#include "monolib/math/Random.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CtrlPc.hpp"

namespace cf {
class CtrlAct {
public:
    virtual ~CtrlAct();
};
} // namespace cf

// u32 -> double conversion magic: the retail code builds the 0x43300000-
// prefixed bit pattern in memory and subtracts the shared sdata2 magic
// double (lbl_eu_80666D78) so the pool reloc matches the retail constant
// instead of an MWCC-synthesised @N entry (CfMapEffectManager.hpp convention).
union F64Conv {
    u32 w[2];
    double d;
};

// 4-byte flag word at CtrlActView+0x58. MWCC lays bitfields out MSB-first:
// the first-declared field occupies the most significant bits, so
// mKind(28-31)/mPhase(26-27)/mAngleState(24-25)/mParam(16-23) map to the
// retail rlwimi masks (28,0-3), (26,4-5), (24,6-7), (16,8-15).
struct CtrlActFlags {
    u32 mKind : 4;        // 28-31 action kind (switch in func_800D5874)
    u32 mPhase : 2;       // 26-27 phase counter (incremented by func_800D3D34)
    u32 mAngleState : 2;  // 24-25 facing state (func_800D5D68)
    u32 mParam : 8;       // 16-23 action parameter
    u32 mBit15 : 1;       // 15
    u32 mBit14 : 1;       // 14 (0x4000, set by func_800D5F98)
    u32 mBit13 : 1;       // 13 (0x2000, latched by func_800D5F98)
    u32 : 13;             // 0-12
};

// self->mField28 view: vtable slot 0x138 returns a float vector (first
// float read as the height/timer value by func_800D3D34).
struct CtrlActViewSub28Vtbl {
    void* p00[0x138 / 4];
    float* (*fn_0x138)(void* self);  // 0x138
};
struct CtrlActViewSub28 {
    CtrlActViewSub28Vtbl* vtbl;      // 0x00
};

// player->mField3F60 (voice/battle target) view for the fields this TU
// touches: flag word at +0x4EC, timer/height at +0x4F8, threshold at +0x508.
struct CtrlActTargetView {
    u8 _00[0x4EC];
    u32 mField4EC;           // 0x4EC (bit 8 test by func_800D6720)
    u8 _4F0[0x4F8 - 0x4F0];
    f32 mField4F8;           // 0x4F8 (active timer gate, func_800D64E8)
    u8 _4FC[0x508 - 0x4FC];
    f32 mField508;           // 0x508 (height-delta threshold, func_800D6720)
    u8 _50C[0x510 - 0x50C];
    f32 mField510;           // 0x510
};

// player->vf167() (vtable slot 0x2A4) result view: the action/attack state
// block cleared and refilled by func_800D2A5C.
struct CtrlActSub2A4 {
    u32 mField0;             // 0x00
    u32 mField4;             // 0x04
    u8 _8[0x48 - 0x8];
    u32 mField48;            // 0x48
    u32 mField4C;            // 0x4C
    u32 mField50;            // 0x50
    f32 mField54;            // 0x54
    f32 mField58;            // 0x58
    f32 mField5C;            // 0x5C
    f32 mField60;            // 0x60
    f32 mField64;            // 0x64
    f32 mField68;            // 0x68
    f32 mField6C;            // 0x6C
    u16 mField70;            // 0x70
    u16 mField72;            // 0x72
    u32 mField74;            // 0x74
    u32 mField78;            // 0x78 (flag word: 0x40002000 base + kind bits)
    u32 mField7C;            // 0x7C
    u16 mField80;            // 0x80
    u8 _82[0xB8 - 0x82];
    u32 mFieldB8;            // 0xB8
};

// Player actor view (CtrlPc+0x5C) covering the fields/vtable slots this TU
// touches: slot 0x288 (getAtkParam base), slot 0x2A4 (action-state block) and
// slot 0x1C0 (height/radius probe returning a float vector).
struct CtrlActPlayerVtbl {
    void* p00[0x30 / 4];
    u32* (*fn_0x30)(void* self);              // mField4 sub-object slot 0x30
    void* r34[(0xF0 - 0x34) / 4];
    f32 (*fn_0xF0)(void* self);               // 0xF0 (returns a height scalar)
    void* rF4[(0x1C0 - 0xF4) / 4];
    float* (*fn_0x1C0)(void* self);           // 0x1C0 (returns float vector, [0] = height)
    void* r1C4[(0x1D0 - 0x1C4) / 4];
    float* (*fn_0x1D0)(void* self);           // 0x1D0 (returns float vector, [0] = height)
    void* r1D4[(0x23C - 0x1D4) / 4];
    f32 (*fn_0x23C)(void* self);              // 0x23C (returns a scalar scale factor)
    void* r240[(0x27C - 0x240) / 4];
    void* (*fn_0x27C)(void* self);            // 0x27C (arts set base, func_800D2D64)
    void* r280[(0x288 - 0x280) / 4];
    void* (*fn_0x288)(void* self);            // 0x288
    void* r28C[(0x2A4 - 0x28C) / 4];
    CtrlActSub2A4* (*fn_0x2A4)(void* self);   // 0x2A4
    void* r2A8[(0x2BC - 0x2A8) / 4];
    int (*fn_0x2BC)(void* self);              // 0x2BC (usable gate, func_800D11B0)
    void* r2C0[(0x5B4 - 0x2C0) / 4];
    f32 (*fn_0x5B4)(void* self);              // 0x5B4 (base facing angle, func_800D69D8)
};

// 32-byte battle-list entry (player +0x358C array, indexed modulo 0x3598).
struct CtrlActBattleEntry {
    u8 _00[0xD];
    u8 mFieldD;              // 0x0D kind byte (3 = use vf1C0 height probe)
    u8 _0E[0x18 - 0x0E];
    void* mField18;          // 0x18 sub-object (height float at +0x60)
};
struct CtrlActBattleEntryObj {
    u8 _00[0x60];
    f32 mField60;            // 0x60 (height value read by func_800D11B0)
};

struct CtrlActPlayerView {
    CtrlActPlayerVtbl* vtbl;     // 0x00
    CtrlPlayerSub4* mField4;     // 0x04 (sub-object, vf30 -> u32* word holder)
    void* mField8;               // 0x08 embedded arts container (vtbl pointer)
    u8 _0C[0x1530 - 0x0C];
    u32 mField1530;              // 0x1530
    u8 _1534[0x3374 - 0x1534];
    u32 mField3374;              // 0x3374 (bit 15 test by func_800D64E8)
    u8 _3378[0x3388 - 0x3378];
    u16 mField3388;              // 0x3388 (bit 4 test by func_800D11B0)
    u8 _338A[0x358C - 0x338A];
    CtrlActBattleEntry* mField358C;  // 0x358C (battle-list entry array base)
    u32 mField3590;              // 0x3590 (ring offset)
    u32 mField3594;              // 0x3594 (entry count)
    u32 mField3598;              // 0x3598 (ring modulus)
    u8 _359C[0x3E6C - 0x359C];
    u16 mField3E6C;              // 0x3E6C (bit 12 test by func_800D2D64)
    u8 _3E6E[0x3E9C - 0x3E6E];
    CVoiceOwnerIntfPc mSub3E9C;  // 0x3E9C embedded voice-owner interface
    u8 _3EA0[0x3F00 - 0x3EA0];
    u32 mField3F00;              // 0x3F00 (bit 1 / bit 2 tests)
    u8 _3F04[0x3F10 - 0x3F04];
    u32 mField3F10;              // 0x3F10 (battle-target actor id, func_800D2D64)
    u8 _3F14[0x3F60 - 0x3F14];
    CtrlActTargetView* mField3F60;  // 0x3F60 (voice/battle target)
    u8 _3F64[0x44D8 - 0x3F64];
    f32 mField44D8;              // 0x44D8 (proximity radius, func_800D755C)
};

// Action-source object (func_8016FE34 result): vtable slots 0x2BC (usable
// gate) and 0x5B4 (base facing), +0x04 sub-object, +0x3E9C voice-owner.
struct CtrlActSrcVtbl {
    void* p00[0x2BC / 4];
    int (*fn_0x2BC)(void* self);              // 0x2BC
    void* r2C0[(0x5B4 - 0x2C0) / 4];
    f32 (*fn_0x5B4)(void* self);              // 0x5B4
};
struct CtrlActSrc {
    CtrlActSrcVtbl* vtbl;        // 0x00
    CtrlPlayerSub4* mField4;     // 0x04
    u8 _08[0x3E9C - 0x08];
    CVoiceOwnerIntfPc mOwner3E9C;  // 0x3E9C embedded voice-owner interface
    u8 _3EA0[0x44D8 - 0x3EA0];
    f32 mField44D8;              // 0x44D8 (probe scale offset, func_800D5308)
    u8 _44DC[0x455A - 0x44DC];
    u16 mField455A;              // 0x455A (battle-list lock word, func_800D69D8)
};

// Attack-parameter record returned by getAtkParam (index*0x88 + 0x10).
struct CtrlActAtkParam {
    u8 _00[0x42];
    u8 mField42;               // 0x42 (kind gate, func_800D2D64)
    u8 _43[0x48 - 0x43];
    u16 mField48;              // 0x48 (arts id, func_800D2D64)
    u8 _4A[0x76 - 0x4A];
    u8 mField76;               // 0x76 (count; +1 written by func_800D2A5C)
};

// Player +0x08 embedded arts container: vtable slot 0x20 takes an id.
struct CtrlActArtsVtbl {
    void* p00[0x20 / 4];
    void (*fn_0x20)(void* self, int id);      // 0x20
};

// Battle-manager field views used by func_800D2D64.
struct CtrlActBmView {
    u8 _00[0x1A8];
    void* mField1A8;           // 0x1A8 chain region (func_80279778, func_800D11B0)
    u8 mField1AA;              // 0x1AA (range gate [1, 0x18])
};
struct CtrlActBattleSubView {
    u32 mField0;               // 0x00
    u8 _4[0x824 - 0x4];
    u32 mField824;             // 0x824 (bit 20 = 0x100000 test)
    u8 _828[0x219C - 0x828];
    u8 mField219C;             // 0x219C (fed to func_801A6A7C)
};

// func_800D2A5C argument view: actor id + kind byte + attack index.
struct CtrlActAtkArg {
    u32 mField0;               // 0x00 actor id
    u8 _4[0xD - 0x4];
    u8 mFieldD;                // 0x0D kind byte (2/3/0x54/0x55)
    u8 _E[0x12 - 0xE];
    s16 mField12;              // 0x12 attack index (signed)
    u8 _14[0x18 - 0x14];
    u32 mField18;              // 0x18
};

// Layout view of the player-control object (cf::CtrlPc-family) used by the
// CtrlAct action handlers. With -RTTI on, MWCC emits two leading vtable
// entries, so the Nth declared virtual sits at vtable offset (N+2)*4. The
// class is only used as a view (never instantiated), so no vtable is
// emitted.
class CtrlActView {
public:
    virtual void vf00();  virtual void vf01();  virtual void vf02();
    virtual void vf03();  virtual void vf04();  virtual void vf05();
    virtual void vf06();  virtual void vf07();  virtual void vf08();
    virtual void vf09();  virtual void vf10();  virtual void vf11();
    virtual void vf12();  virtual void vf13();  virtual void vf14();
    virtual void vf15();  virtual void vf16();  virtual void vf17();
    virtual void vf18();  virtual void vf19();  virtual void vf20();
    virtual void vf21();  virtual void vf22();
    virtual int vf23(ml::CVec3* a, f32* b, ml::CVec3* c, int d, int e); // 0x64
    virtual int vf24(ml::CVec3* out, const CVoicePos* pos);                  // 0x68
    virtual int vf25(ml::CVec3* out);                                        // 0x6C
    virtual int vf26(ml::CVec3* out, int flag);                              // 0x70
    virtual int vf27(ml::CVec3* out);                                        // 0x74
    virtual void vf28();
    virtual void vf29();  virtual int vf30();  virtual int vf31();
    virtual void vf32();  virtual void vf33();  virtual void vf34();
    virtual void vf35();  virtual void vf36();  virtual void vf37();
    virtual void vf38();  virtual void vf39();  virtual void vf40();
    virtual CVoicePos* vf41();  // 0xAC get own position

    u32 mField4;               // 0x04 (flag word, bit 1 set by func_800D2A5C)
    u8 _pad08[0x0C - 0x08];
    f32 mFieldC;               // 0x0C target angle
    f32 mField10;              // 0x10 computed facing
    f32 mField14;              // 0x14 aim/fx state value
    u32 mField18;              // 0x18 (attack counter written by func_800D2A5C)
    u8 _pad1C[0x28 - 0x1C];
    CtrlActViewSub28* mField28; // 0x28 (vtable slot 0x138 float source)
    u8 _pad2C[0x30 - 0x2C];
    ml::CVec3 mPos30;          // 0x30 (start of the 0x2C-byte block cleared in the handlers)
    f32 mField3C;              // 0x3C
    f32 mField40;              // 0x40
    f32 mField44;              // 0x44
    f32 mField48;              // 0x48
    f32 mField4C;              // 0x4C (facing angle written by func_800D3D34)
    f32 mField50;              // 0x50
    f32 mField54;              // 0x54 action timer
    CtrlActFlags mFlags58;     // 0x58
    CtrlActPlayerView* mPlayer;// 0x5C player actor object
    ml::CVec3 mPos60;          // 0x60 position block (func_800D69D8 walks it)
    f32 mField6C;              // 0x6C (facing reference, func_800D69D8)
    f32 mField70;              // 0x70 (facing blend value, func_800D11B0)
    u16 mField74;              // 0x74 (action state bits 1-3, func_800D6720)
    s16 mField76;              // 0x76 (countdown, func_800D11B0)
    u8 mField78;               // 0x78 (probe countdown, func_800D755C)
    u8 mField79;               // 0x79
    u8 mField7A;               // 0x7A (cleared by func_800D34D4 kind 9/10)
    u8 mField7B;               // 0x7B (probe counter, func_800D755C)
};

// Extended view of the +0x3E9C voice-owner interface exposing the vtable
// slot 0x1AC used by func_800D1CFC (the CVoiceOwnerIntfPc view in CtrlPc.hpp
// stops at 0xC4). Index 105 -> vtable 0x1AC.
class CtrlActVoiceOwnerIntf : public CVoiceOwnerIntfPc {
public:
    virtual void vf48();  virtual void vf49();  virtual void vf50();
    virtual void vf51();  virtual void vf52();  virtual void vf53();
    virtual void vf54();  virtual void vf55();  virtual void vf56();
    virtual void vf57();  virtual void vf58();  virtual void vf59();
    virtual void vf60();  virtual void vf61();  virtual void vf62();
    virtual void vf63();  virtual void vf64();  virtual void vf65();
    virtual void vf66();  virtual void vf67();  virtual void vf68();
    virtual void vf69();  virtual void vf70();  virtual void vf71();
    virtual void vf72();  virtual void vf73();  virtual void vf74();
    virtual void vf75();  virtual void vf76();  virtual void vf77();
    virtual void vf78();  virtual void vf79();  virtual void vf80();
    virtual void vf81();  virtual void vf82();  virtual void vf83();
    virtual void vf84();  virtual void vf85();  virtual void vf86();
    virtual void vf87();  virtual void vf88();  virtual void vf89();
    virtual void vf90();  virtual void vf91();  virtual void vf92();
    virtual void vf93();  virtual void vf94();  virtual void vf95();
    virtual void vf96();  virtual void vf97();  virtual void vf98();
    virtual void vf99();  virtual void vf100(); virtual void vf101();
    virtual void vf102(); virtual void vf103(); virtual void vf104();
    virtual void m1AC(u32 a, const char* b);  // 0x1AC
};

// Voice-owner vtable slots used by the action handlers with non-base
// signatures: 0x8C returns the actor height float, 0x9C takes a vec arg,
// 0xC8 takes a float arg (v47/0xC4 stays the base CVoiceOwnerIntfPc form).
struct CtrlActPosBlock {
    u8 _00[0xC];
    f32 mFieldC;             // 0x0C (anchor x)
    u8 _10[0x1C - 0x10];
    f32 mField1C;            // 0x1C (anchor y)
    u8 _20[0x2C - 0x20];
    f32 mField2C;            // 0x2C (anchor z)
};
struct CtrlActVoiceOwnerVtbl {
    void* p08[(0x8C - 0x08) / 4];
    f32 (*fn_0x8C)(void* self);                    // 0x8C (height float)
    void* r90[(0x9C - 0x90) / 4];
    void (*fn_0x9C)(void* self, ml::CVec3* out);   // 0x9C
    void* rA0[(0xC4 - 0xA0) / 4];
    void (*fn_0xC4)(void* self, f32 arg);          // 0xC4
    void (*fn_0xC8)(void* self, f32 arg);          // 0xC8
    void* rCC[(0x12C - 0xCC) / 4];
    CtrlActPosBlock* (*fn_0x12C)(void* self, int arg);  // 0x12C (anchor block)
    void* r130[(0x138 - 0x130) / 4];
    float* (*fn_0x138)(void* self);                // 0x138 (returns float vector, [0] used)
    void* r13C[(0x140 - 0x13C) / 4];
    f32 (*fn_0x140)(void* self);                   // 0x140 (returns a height scalar)
};
struct CtrlActVoiceOwnerView {
    CtrlActVoiceOwnerVtbl* vtbl;  // 0x00
};

// View of the action-source object (func_8016FE34 result): the embedded
// voice-owner region sits at +0x3E9C (mirror of CtrlRemote's sweep view).
struct CtrlActSweepView {
    u8 _00[0x3E9C];
    u8 mOwner3E9C[0x10];  // 0x3E9C voice-owner region (opaque)
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------
extern "C" void func_80174C24(void* obj, u32 flag);
extern "C" f32 func_80496288(void* scene);      // frame delta time
extern "C" void* func_8016FE34(void* r3);       // voice/battle-list resolve
extern "C" int func_8004C5EC(void* battleObj);  // battle-state page id
extern "C" void func_800BE12C(void* obj, int a, int b, int c, int d);
extern "C" void func_80174B4C(void* actor, u32 flags);
extern "C" int func_80174C98(void* actor, u32* val, int flags);
extern "C" int func_80148778(void* obj, int id);
// func_800D11B0 / func_800D69D8 extra imports (retail C-ABI names).
extern "C" f32 func_80190938();                    // battle-list fallback height
// Chain-start gate (chain/CChain.cpp); retail call site uses the plain name.
extern "C" int func_80279778(void* a, void* b);
// monolib coli segment probes (retail C-ABI names).
extern "C" int func_804B19CC(void* self, void* src, int a, int b);
extern "C" int func_804B4E10(void* a, void* b, void* c, int d, int e, int f);
extern "C" int func_804B526C(void* a, void* b, void* c, void* d, int e, int f, int g);
extern "C" int func_804B54D4(void* a, void* b, void* c, int d, int e);
// func_800D2D64 imports: arts-param lookups, battle-manager sub-views and the
// arts-id predicate. (getInstance__Q22cf14CBattleManagerFv is declared in
// CtrlPc.hpp as CBattleManagerView*.)
extern "C" void* func_80153CAC(void* base, int index);
extern "C" void* getArtsParamByIdx(void* base, int index);
extern "C" void* func_800EA444(void* bm);
extern "C" int func_801A6A7C(void* a, void* b);
extern "C" int func_80145C00(int val);
// Collision-query API: func_804BE398 is declared in CtrlMoveBase.hpp
// (retail C-ABI name; 6-arg probe passing two FP args after the GPR args).
extern "C" int func_804BE348(void* a, void* b, int c, int d, int e);
extern "C" int func_804BE4AC(void);
extern "C" void* func_804BE50C(u32 index);
extern "C" void* func_804BE520(int index);
extern "C" int func_804BE5A4(int a, int b);
// Attack-param table lookup (CArtsSet.cpp): base + index*0x88 + 0x10.
extern "C" void* getAtkParam(void* base, int index);
// Enum-list actor-id query (target 5 scan loop).
extern "C" int func_800F6E08(void* list);
// In-TU facing helper called by func_800D3D34 (retail plain C name).
extern "C" int func_800D5F98(CtrlActView* self, CtrlActSrc* src);

// C++-mangled retail helper func_800B708C__Fi (actor id -> action source).
void* func_800B708C(int id);

// cf::CfGameManager singleton accessor (mangles to getInstance__Q22cf13CfGameManagerFv)
// and the presentation/event flag probe func_8006EF04__Fi.
bool func_8006EF04(int mask);

// In-TU action-handler entry points: C linkage so the call relocs carry the
// retail plain names (func_800D5F98 convention).
extern "C" int func_800D34D4(CtrlActView* self);
extern "C" void func_800D3998(CtrlActView* self);
extern "C" void func_800D3D34(CtrlActView* self);
extern "C" void func_800D3FFC(CtrlActView* self);
extern "C" void func_800D4834(CtrlActView* self);
extern "C" void func_800D49F4(CtrlActView* self);
extern "C" void func_800D4F30(CtrlActView* self);
extern "C" void func_800D5308(CtrlActView* self);
extern "C" void func_800D56F0(CtrlActView* self);
extern "C" void func_800D5A2C(CtrlActView* self);
extern "C" void func_800D5D68(CtrlActView* self);
extern "C" void func_800D755C(CtrlActView* self, ml::CVec3* pos);
extern "C" int func_800D64E8(CtrlActView* self);
extern "C" int func_800D6720(CtrlActView* self, int flag);

// Scene object pointer fed to func_80496288 (.sbss).
extern void* lbl_eu_80663E14;

// sdata2 float constants (const so MWCC references the retail pool slots).
extern const f32 lbl_eu_80666CF8;   // 0.0f
extern const f32 lbl_eu_80666CFC;
extern const f32 lbl_eu_80666D00;
extern const f32 lbl_eu_80666D04;
extern const f32 lbl_eu_80666D08;
extern const f32 lbl_eu_80666D0C;
extern const f32 lbl_eu_80666D10;
extern const f32 lbl_eu_80666D14;
extern const f32 lbl_eu_80666D18;
extern const f32 lbl_eu_80666D1C;
extern const f32 lbl_eu_80666D20;
extern const f32 lbl_eu_80666D24;
extern const f32 lbl_eu_80666D28;
extern const f32 lbl_eu_80666D2C;
extern const f64 lbl_eu_80666D30;   // 0.1 (double)
extern const f32 lbl_eu_80666D38;
extern const f32 lbl_eu_80666D3C;
extern const f32 lbl_eu_80666D40;
extern const f32 lbl_eu_80666D44;
extern const f32 lbl_eu_80666D4C;
extern const f64 lbl_eu_80666D50;   // 2^52 (u32->double conversion magic)
extern const f32 lbl_eu_80666D58;
extern const f32 lbl_eu_80666D5C;
extern const f32 lbl_eu_80666D60;
extern const f32 lbl_eu_80666D64;
extern const f32 lbl_eu_80666D68;
extern const f32 lbl_eu_80666D6C;
extern const f32 lbl_eu_80666D70;
extern const f64 lbl_eu_80666D78;   // 2^52 (u32->double conversion magic)
extern const f32 lbl_eu_80666D80;
extern const f32 lbl_eu_80666D84;
extern const f32 lbl_eu_80666D88;
extern const f32 lbl_eu_80666D8C;
extern const f32 lbl_eu_80666D90;
extern const f32 lbl_eu_80666DB4;
extern const f32 lbl_eu_80666DB8;
extern const f32 lbl_eu_80666DBC;
extern const f32 lbl_eu_80666DC0;
extern const f32 lbl_eu_80666D94;
extern const f32 lbl_eu_80666D98;
extern const f32 lbl_eu_80666D9C;
extern const f32 lbl_eu_80666DA0;
extern const f32 lbl_eu_80666DA4;
extern const f32 lbl_eu_80666DA8;
extern const f32 lbl_eu_80666DAC;
extern const f32 lbl_eu_80666DB0;
extern const f32 lbl_eu_80666DC4;
extern const f32 lbl_eu_80666DA4;
extern const f32 lbl_eu_80666DA8;
extern const f32 lbl_eu_80666DAC;
extern const f32 lbl_eu_80666DB0;
extern const f32 lbl_eu_8066A1F8;   // pi
extern const f32 lbl_eu_8066A1FC;   // 2*pi
extern const f32 lbl_eu_8066A20C;
extern const f32 lbl_eu_8066A204;   // 0.7853982 (pi/4)

// 2-float blocks (base + f2 offset pairs for the facing-state table).
extern const f32 lbl_eu_80663EF8[2];   // sda2 (sda21 addressing)
extern const f32 lbl_eu_80573A20[4];   // .data (declared >8B to force lis/addi)

// 3-word enum-list filter table (.rodata) fed to func_800F4A98 (target 5).
extern const u32 lbl_eu_804FC810[3];

// Label string fed to the voice-owner slot 0x1AC (.rodata).
extern char lbl_eu_804FC81C[];

// Global collision-probe object pointer (.sbss; monolib coli unit).
extern void* lbl_eu_80665958;

// nw4r db Warning() file/format strings (.rodata; also used by CtrlNpc.cpp).
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];

// In-TU action entry points matched to retail (C linkage keeps the plain
// call reloc names).
extern "C" void func_800D11B0(CtrlActView* self);
extern "C" void func_800D69D8(CtrlActView* self, ml::CVec3* pos, CtrlActSrc* src, int flag);
