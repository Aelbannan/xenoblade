#pragma once

#include <types.h>
#include <monolib/math/CVec3.hpp>
#include <monolib/math/CMat33.hpp>
#include <monolib/math/CQuat.hpp>
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)
#include "kyoshin/cf/object/CfObject.hpp"  // owner of the 0x74/0xAC/0xCC/0x120/0x128/0x12C slots (unk164 / 73DDC / 74090 / 74230)

namespace cf {
struct CfCamFollow; // fwd decl for extern-C imports below (full layout later)
class CfObject;     // active camera-state object (func_8006E5A4 return)
} // namespace cf

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" bool func_800755B0(void*, int);
extern "C" bool func_8049EB60(void* obj);
extern "C" u32 func_8006BFBC(void* self);                          // CfCam sibling gate
extern "C" int func_8006BFC4(int controllerId, int unk); // CfCam sibling pad check
// Copies a 12-byte vector (dst, src); returns dst (retail: func_8004B3F0 leaves
// its first arg in r3). C linkage so call relocs keep the unmangled retail
// symbol name (MWCC mangles plain C++ global declarations).
extern "C" void* func_8004B3F0(void*, void*);
extern "C" int func_8006C1B0(void* arg);           // CfCam sibling (FULL_MATCH us-8006cc08)
extern "C" void func_8006B720(int arg1, int arg2); // CfCam sibling (us-8006c178)
extern "C" float lbl_eu_80527230[];
extern "C" float lbl_eu_80661BA8;
extern "C" float lbl_eu_80663DE4;     // .sbss float (retail 0x80663DE4)
extern "C" float lbl_eu_80663DE8;     // .sbss float (retail 0x80663DE8)
extern "C" float lbl_eu_8066632C;     // .sdata2 float constant
extern "C" float lbl_eu_80570A38[3];  // .bss vec3 (retail 0x80570A38)
// .bss float (retail 0x80570A44). Sized >8B so MWCC uses full lis/@l addressing
// (SDA threshold) - only [0] is ever referenced.
extern "C" float lbl_eu_80570A44[3];
extern "C" float lbl_eu_80570A50[3];  // .bss vec3 (retail 0x80570A50)
extern "C" float lbl_eu_80570A5C[3];  // .bss vec3 (retail 0x80570A5C)
extern "C" void func_804BE4B4(void*, int);
extern "C" void* func_804BE4E0(void*, int);
extern "C" float scaleByGlobal(float val);
extern "C" float lbl_eu_80661B60;     // .sdata float (retail 0x80661B60)
extern "C" float lbl_eu_80661B4C;     // .sdata float (retail 0x80661B4C)
extern "C" int lbl_eu_80661BA4;       // .sdata word (retail 0x80661BA4)
extern "C" u32 func_80061FE8();                          // mtl heap handle for cf allocations
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);
// func_8006F5C8 callees (flat retail names, same naming class)
extern "C" int func_80075640();
extern "C" int func_8006D72C(void* self);
extern "C" int func_80275378(void* self);
extern "C" int func_80275338(void* self);
extern "C" int func_802752B8(void* self);
extern "C" int func_802752F8(void* self);
extern "C" int CfRes_getE24Bit22();
extern "C" int func_8006EEE4();
extern "C" int func_801B0F8C();
extern "C" int func_8017FD44();
extern "C" int func_802751F8(void* self);
extern "C" int isSceneActive__Q22cf13CfGameManagerFv();
extern "C" float lbl_eu_80666358; // .sdata2 constant
extern "C" int func_8006C6B4(int self, int mask);        // CfCam sibling flag probe (0x1D4 word)
extern "C" void func_8006D734(void* self, void* src);     // CfCam sibling (retail 0x8006E18C)
extern "C" __declspec(noinline) u8* func_8006BF14(u8*, int); // CfCam sibling (retail 0x8006BF14), body at bottom of CfCam.cpp; noinline keeps the retail bl at call sites
// In-TU ctor helpers whose bodies MWCC would otherwise inline; noinline keeps
// the retail `bl func_8006BE*` calls in __ct__cf_CfCamFollow.
extern "C" __declspec(noinline) void func_8006BEF8(void* self);
extern "C" __declspec(noinline) void func_8006BEC0(void* self);
extern "C" __declspec(noinline) void func_8006BEE4(cf::CfCamFollow* self);
extern "C" __declspec(noinline) void func_8006BEFC(void* self);
extern "C" __declspec(noinline) void func_8006BF08(u16* self);
// .data lookup tables (retail 0x80527208 / 0x8052721C, 20 bytes each). Sized
// >8B so MWCC uses full lis/@l addressing (SDA threshold).
extern "C" float lbl_eu_80527208[5];
extern "C" float lbl_eu_8052721C[5];
// .sdata floats (retail 0x80661B3C, 0x80661B40, 0x80661B44, 0x80661B48,
// 0x80661B74). Scalar stores - only the .data tables below are indexed.
extern "C" float lbl_eu_80661B3C;
extern "C" float lbl_eu_80661B40;
extern "C" float lbl_eu_80661B44;
extern "C" float lbl_eu_80661B48;
extern "C" float lbl_eu_80661B74;
extern "C" float lbl_eu_806662B0;     // .sdata2 constant (retail 0x806662B0)
extern "C" float lbl_eu_806662B4;     // .sdata2 constant (retail 0x806662B4)
extern "C" float lbl_eu_806662D8;     // .sdata2 angle offset (retail 0x806662D8)
extern "C" float lbl_eu_806662E0;     // .sdata2 divisor (retail 0x806662E0)
extern "C" float const lbl_eu_806662E4;     // .sdata2 sine multiplier (retail 0x806662E4)
extern "C" double lbl_eu_806662E8;    // .sdata2 fmod wrap divisor, double (retail 0x806662E8)
extern "C" float lbl_eu_8066A200;     // .sdata2 pi/2 (retail 0x8066A200)
extern "C" u8 lbl_eu_80663DE0;        // .sdata byte gate (retail 0x80663DE0)
extern "C" void func_80059610(void*, float);        // retail 0x80059C78
extern "C" void func_8006BBF4(void* obj, u32 mask, int flag); // CfCam sibling flag set (retail 0x8006BBF4)
extern "C" void func_8006B948();                              // CfCam sibling (retail 0x8006B948)
extern "C" void func_8006BEC0(void* self);                    // CfCam sibling no-op (retail 0x8006BEC0)
extern "C" void func_8006C740(ml::CVec3* out, cf::CfCamFollow* self); // CfCam sibling (retail 0x8006CA1C); same signature as the in-TU def so call sites keep the unmangled bl
extern "C" void func_80073C7C(void* a, void* b, void* c, void* d, float f); // CfCam sibling (retail 0x80073C7C)
extern "C" float* func_800742FC(float* mtx, float scale);       // CfCam sibling (retail 0x800742FC)
extern "C" void func_800743A4(float* mtx, const float* v);    // CfCam sibling (retail 0x800743A4)
// extern-C-only views of in-TU C++ definitions (differing signature keeps the bl):
extern "C" void func_8006D6A8(void* dst, void* mtx, void* src); // in-TU def: (Vec*, const Mtx, const Vec*)
extern "C" void func_800743C0(void* mtx);                       // in-TU def: (nw4r::math::MTX34*)
extern "C" __declspec(noinline) void func_8006B8E4();
extern "C" void func_800598A8(void*, void*, void*); // retail 0x80059F10
extern "C" void nopFunc(void*);                     // retail 0x80059C74 (no-op stub)
// Flat retail C names (same naming class as lbl_* symbols) - CfCam cross-TU
// callees and siblings:
extern "C" float func_8006BAF0(void* self); // CfCam sibling (retail 0x8006C548); body sits under the C++-mangled name in CfCam.cpp so callers keep the bl
extern "C" float func_8006D3D0(const void* v); // CfCam sibling (retail 0x8006DE28); yaw from x/z; body under the C++-mangled name so callers keep the bl
// Call-through-overload views of in-TU bodies (differing signatures keep the
// bl instead of letting MWCC inline the local definition):
extern "C" float func_8006D380(const void* v); // spherical azimuth wrapper (retail 0x8006DD78)
extern "C" float func_8006D41C(double x);      // acos wrapper view (retail 0x8006D41C); body is the float overload
extern "C" float func_8006BB00(const void* v); // CfCam sibling (retail 0x8006C558): `b PSVECMag` tail call
extern "C" void func_8004CB80(f32* out, const f32* a, const f32* b); // vec3 subtract out = a - b (retail 0x8004CB80)
extern "C" float func_800A3EF4(float x);   // FSqrt-style sqrt with nw4r assert (retail 0x800A3EF4)
extern "C" f32 func_8004CC40(f32 a, f32 b); // sin wrapper; CfCam call sites pass 2 args (retail 0x8004D2D8)
extern "C" f32 func_8004BC28(f32 value);   // wrap angle into [-pi, pi) (retail 0x8004C300)
extern "C" float lbl_eu_80666368;          // .sdata2 step increment (retail 0x80666368)
extern "C" float lbl_eu_806662F8;          // .sdata2 follow-distance cap (retail 0x806662F8)
extern "C" float lbl_eu_8066636C;          // .sdata2 decay step (retail 0x8066636C)
extern "C" float lbl_eu_8066A1F8;          // .sdata2 pi (retail 0x8066A1F8)
extern char lbl_eu_80526324[];              // nw4r FSqrt assert source-file string (retail .data)
extern char lbl_eu_80526300[];              // nw4r FSqrt assert message string (retail .data)
extern char lbl_eu_805262F0[];              // nw4r assert source-file string (retail .data)
extern char lbl_eu_805262C8[];              // nw4r assert message string (retail .data)
extern const float lbl_eu_80666340;         // .sdata2 constant
// func_8006E884 cross-TU imports (flat retail names).
extern "C" int func_8007560C();             // gate probe
extern "C" void func_80071B78(void* self, float f);
extern "C" void* func_8006D6F8(void* obj);  // word-block source getter
extern "C" void* func_8006EEF0(void* obj);  // sub-object getter (+4 vec)
extern "C" int func_802753B8(void* self);   // cam-control register probe

namespace cf {

// NOTE: everything type-y lives in THIS single namespace block. MWCC 10140
// breaks qualified lookups (cf::X -> "undefined identifier") when types are
// declared across multiple re-openings of namespace cf, so later blocks were
// merged here on purpose.
struct CfCamFollow; // fwd decl for extern-C imports below (full layout later)
class CfObject;     // active camera-state object (func_8006E5A4 return)
struct CVoiceRec;   // owned by help/CHelp_Talk.hpp (func_8006DBD4 param)
// Base camera object (+0x00..+0x10). Retail table lbl_eu_805272E8
// (cf::CfCam: dtor + 25 virtuals through +0x6C; zero words are pure
// virtuals overridden by the leaves below).
// __declspec(novtable): the tables live in retail .data (this TU has no
// .data), never emitted here - the ctors store the symbols.txt labels
// manually (this->vtbl() = lbl_eu_...), same as CToken / CHelp. Virtual
// calls emit no relocs, so members mirror the flat retail words (CHelp
// pattern: func_802B7CBC). Pure signatures are forced by the leaf
// overrides (an override must match the base).
// Defined before CfCamFollow so the follow camera can really derive from it
// (the derived dtor must emit the retail `bl __dt__Q22cf5CfCamFv`).
class __declspec(novtable) CfCam {
public:
    virtual ~CfCam();                               // 0x08
    virtual void func_8006CC68(int arg) = 0;        // 0x0C (leaf: reset entry)
    virtual void* func_8006B6B8() = 0;              // 0x10 (leaf: this+0x1C)
    virtual void func_80074D60(void* src) = 0;      // 0x14 (leaf: copy 3 words to +0x28)
    virtual void func_8006CA2C(void* arg, float f) = 0; // 0x18 (leaf: vector prep)
    virtual void* func_80074D4C();                  // 0x1C (field_0x0C + 0x118)
    virtual void func_8006E884(float f) = 0;        // 0x20 (leaf: per-frame driver)
    virtual void func_80071B74() = 0;               // 0x24 (leaf: no-op)
    virtual void func_8006BFDC() = 0;               // 0x28 (leaf: constants reset)
    virtual void* func_80073C74(void* src) = 0;     // 0x2C (leaf: this+0x10 copy)
    virtual void* func_8006B6A0();                  // 0x30 (this+0x10)
    virtual void* func_8006B6B0() = 0;              // 0x34 (leaf: this+0x40)
    virtual void func_8006D7A8(void* src) = 0;      // 0x38 (leaf: state-block copy)
    virtual void func_8006C1BC(float f) = 0;        // 0x3C (leaf: store heading)
    virtual void func_8006BEF0(int arg);            // 0x40 (stores arg at +0x08)
    virtual void* func_80074D44();                  // 0x44 (returns 0)
    virtual void func_80073DDC(CfObject* src) = 0;  // 0x48 (leaf: state copy)
    virtual void func_800606AC();                   // 0x4C (pluginCam no-op)
    virtual void func_80060738();                   // 0x50 (pluginCam no-op)
    virtual void func_800607C4();                   // 0x54 (pluginCam no-op)
    virtual float func_80074D58() = 0;              // 0x58 (leaf: +0x1E0 getter)
    virtual void func_8006C16C(void* target) = 0;   // 0x5C (leaf: stash +0x164)
    virtual void* func_8006B6A8() = 0;              // 0x60 (leaf: +0x164 handle)
    virtual void func_800605D0();                   // 0x64 (pluginCam no-op)
    virtual void func_80060A08();                   // 0x68 (pluginCam no-op; CfCamFollow overrides)
    virtual void func_80060B84();                   // 0x6C (pluginCam no-op)

    // Overlay on the implicit vptr at +0 so the ctors can store the retail
    // table labels manually (novtable suppresses the compiler store).
    void*& vtbl() {
        return *reinterpret_cast<void**>(this);
    }

    u32 field_0x04;    // 0x04
    u32 field_0x08;    // 0x08
    void* field_0x0C;  // 0x0C ctor arg
    float getUnk4FC();
};

// Derived follow camera; base CfCam occupies +0x00..+0x10, follow extends
// through ~0x25A.
struct __declspec(novtable) CfCamFollow : CfCam {
    u8 unk10[0x1C - 0x10];
    u8 unk1C[0x28 - 0x1C];
    f32 field_0x28; // 0x28 camera position x
    f32 field_0x2C;
    f32 field_0x30; // 0x30 camera position z
    f32 field_0x34;
    u8 unk38[0x64 - 0x38];
    u32 field_0x64;        // 0x64 raw flag word
    u8 unk68[0x70 - 0x68]; // 0x68..0x70
    u8 unk70[0xC0]; // 0x70..0x130
    u8 unk130[0x160 - 0x130];
    void* unk160; // 0x160 ctor arg2
    CfObject* unk164; // 0x164 follow-target handle (ctor arg1; CfObject vtable slots 0x74/0xAC/0xCC/0x128)
    u8 unk168[0x180 - 0x168];
    f32 field_0x180;          // 0x180 snapshot source for unk1F8
    f32 field_0x184;          // 0x184 blended heading angle
    u8 unk188[0x1D4 - 0x188];
    u32 unk1D4; // 0x1D4
    u16 unk1D8; // 0x1D8
    u8 unk1DA[0x1DC - 0x1DA];
    f32 unk1DC;
    f32 unk1E0;
    f32 unk1E4;
    f32 unk1E8;
    f32 unk1EC;
    f32 unk1F0;
    f32 unk1F4;
    f32 unk1F8;
    f32 unk1FC;
    f32 unk200;
    f32 unk204;
    f32 unk208;
    f32 unk20C;
    f32 unk210;
    u32 unk214;
    f32 unk218;
    f32 unk21C;
    f32 unk220;
    f32 unk224;
    f32 unk228;
    f32 unk22C;
    f32 unk230;
    f32 unk234;
    f32 unk238;
    f32 unk23C;              // 0x23C follow/decay heading float
    f32 unk240;
    f32 unk244;
    f32 unk248;
    u32 unk24C;
    s16 unk250;
    u16 unk252;
    u16 unk254;
    s16 unk256;
    s16 unk258;
    u8 unk25A[0x4EC - 0x25A]; // padding
    u32 field_0x4EC;          // 0x4EC raw flag word

    void clearUnk04Bits(unsigned int mask);
    unsigned int getBit26_0x4EC();
    virtual ~CfCamFollow(); // retail __dt__Q22cf11CfCamFollowFv (defined outside this TU)

    // Retail table lbl_eu_80527260 (cf::CfCamFollow: 28 virtuals through
    // +0x74). Overrides redeclare the base slots they fill with the same
    // signature (a true override, not an append); the +0x70/+0x74 pair is
    // new on this class. Slots the leaf shares with the base
    // (0x1C/0x30/0x40/0x44/0x4C/0x50/0x54/0x64/0x6C) are inherited as-is.
    virtual void func_8006CC68(int arg);             // 0x0C
    virtual void* func_8006B6B8();                  // 0x10
    virtual void func_80074D60(void* src);          // 0x14
    virtual void func_8006CA2C(void* arg, float f); // 0x18
    virtual void func_8006E884(float f);            // 0x20
    virtual void func_80071B74();                   // 0x24
    virtual void func_8006BFDC();                   // 0x28
    virtual void* func_80073C74(void* src);         // 0x2C
    virtual void* func_8006B6B0();                  // 0x34
    virtual void func_8006D7A8(void* src);          // 0x38
    virtual void func_8006C1BC(float f);            // 0x3C
    virtual void func_80073DDC(CfObject* src);      // 0x48
    virtual float func_80074D58();                  // 0x58
    virtual void func_8006C16C(void* target);       // 0x5C
    virtual void* func_8006B6A8();                  // 0x60
    virtual void func_80073D8C(int cond);           // 0x68 (base holds func_80060A08 here)
    virtual void* func_80074A3C();                  // 0x70 (new: pad-action source)
    virtual int func_80074AA4(int id);              // 0x74 (new: pad-action dispatch)
};

// Layout view over the follow-camera state block copied wholesale by
// func_80073DDC from the dynamic-cast source object into CfCamFollow.
struct CfCamStateView {
    u8 pad00[0x10];
    ml::CVec3 field10;      // 0x10
    u8 pad1C[0x28 - 0x1C];
    ml::CVec3 field28;      // 0x28
    ml::CVec3 field34;      // 0x34
    ml::CVec3 field40;      // 0x40
    u8 pad4C[0x58 - 0x4C];
    ml::CVec3 field58;      // 0x58
    u8 pad64[0x1E0 - 0x64];
    f32 field1E0;           // 0x1E0
};


// cf::CfCamEvent - camera event state. Full definition lives in
// CfCamEvent.hpp (which includes this header); keep only the forward
// declaration here to avoid a redefinition in TUs that include both.
class CfCamEvent;

// Row-id halfword view of the active camera object (read at +0x8C).
struct CfCamRowView {
    u8 field_0x00[0x8C];
    u16 rowId; // +0x8C
};

// Flags-word view of UnkClass_800821F8 (bit 0x20000000 set at +0x4).
struct UnkClass800821F8FlagView {
    u8 field_0x00[4];
    u32 flags; // +0x04
};

} // namespace cf

// Additional retail C-ABI imports used by CfCam call sites (same naming
// class as the block above: unmangled func_* reloc names + lbl_eu_* data
// constants, so call relocs keep the retail symbol verbatim).
extern "C" {
void func_80070784(ml::CVec3* a, ml::CVec3* b);      // in-unit (retail 0x800711B0)
ml::CVec3* func_8004CBC8(ml::CVec3* vec);            // retail 0x8004D260 (normalize)
void func_8004B75C(float* destination, const float* source, float scale); // retail 0x8004BE34
void func_8004B738(float* destination, const float* source);              // retail 0x8004BE10
f32 func_8004CC68(f32 angle);                        // retail 0x8004D300 (SinFIdx wrapper)
f32 func_8004CC74(f32 angle);                        // retail 0x8004D30C (CosFIdx wrapper)
float func_80073F88(const f32* v);                   // in-TU def: (const ml::CVec3*); differing signature keeps the bl
void func_80073E74(const ml::CVec3* a, const ml::CVec3* b, f32* out, int flag2, int flag3, f32 heading); // in-TU def (defined below its first use); retail callers pass self->unk244 in f1 - body ignores it
void func_800733B8(ml::CVec3* out, cf::CfCamFollow* self, const ml::CVec3* dir,
                   const f32* angles, float f); // in-TU def (defined below its first use)
__declspec(noinline) void func_8006FFA8(cf::CfCamFollow* self, float* outA, float* outB); // in-TU def, same signature (mixed linkage keeps the unmangled bl); noinline keeps the retail bl instead of inlining the body
void func_80071730(void* dst, void* src);            // retail 0x8007215C (16-byte copy)
void func_80071AB0(cf::CfCamFollow* self, void* a, void* b, int c, float f1, float f2); // in-TU stub (defined below its first use)
__declspec(noinline) void func_800707C0(void* self, void* a, void* b); // in-TU stub, same signature (mixed linkage keeps the unmangled bl)
int CfRes_getD80Flag();                              // scene flag gate (CUIErrMesWin.hpp)
// func_8049603C: declared once in libs/monolib/src/scn/CScn_8049603C.hpp
// (ellipsis form - this TU calls it with no args).

void func_80070EBC(cf::CfCamFollow* self);           // in-TU def (defined below its first use), same signature (mixed linkage)
ml::CVec3* func_8004B79C(ml::CVec3* out, const ml::CVec3* v); // retail 0x8004BE74 (vec helper)
void func_8006C640(cf::CfCamFollow* self, u32 mask, int flag); // CfCam sibling (retail 0x8006D098)
int getNullPtrC__Q22cf13CfGameManagerFv(u32);          // CfGameManager state gate (retail 0x8007FE24)
void func_8006CB0C(void* out, cf::CfCamFollow* self, void* dir, void* sel); // follow-cam vector prep
int isTimerActive__Q22cf13CfGameManagerFv();             // CfGameManager gate (retail 0x8007F91C)
void func_8049EFF8(void* obj, f32 f, void* a, void* b);  // retail 0x8049EFF8 (pose/scale apply)
int func_8006D374(void* self);                           // CfCam sibling (retail 0x8006D374)
cf::CfCamFollow* func_80071CDC();                    // CfCam sibling (retail 0x80072708)
ml::CVec3* func_80071CE4(cf::CfCamFollow* self);     // CfCam sibling (retail 0x80072710)
__declspec(noinline) u8* func_80071CEC(cf::CfCamFollow* self); // CfCam sibling (retail 0x80072718), in-TU body at bottom of CfCam.cpp; noinline keeps the bl at call sites
void func_800C1DF0(...);                             // retail 0x800C2838 (variadic callback prologue)
ml::CVec3* func_800A3C48(ml::CVec3* v);              // retail 0x800A4510 (normalize)
int func_80275238(cf::CfCamFollow* self);            // retail 0x802776BC (cam-control register)
int func_80275278(cf::CfCamFollow* self);            // retail 0x802776FC (move register)
int func_804BE348(void*, void*, int, int, int);      // retail 0x804C24A4 (coli probe query; same type as CfObjectEnumList.hpp)
int func_804B5088(cf::CfCamFollow* self, ml::CVec3* a, ml::CVec3* b, int filter, int isFirst); // retail 0x804B91E4 (object enum probe; same type as CfObjectEnumList.hpp)
// Scene ground-probe result-list walkers (retail C-ABI; same shape as
// CtrlAct.hpp / CPartsChange.hpp).
int func_804BE4AC();
ml::CVec3* func_804BE50C(u32 index);
ml::CVec3* func_804BE520(int index);
int func_804BE2E8(void* vec, u32 mask, int a, int b);
float lbl_eu_806663CC;                               // .sdata2 constant
float lbl_eu_806663D0;                               // .sdata2 constant
float lbl_eu_806663D4;                               // .sdata2 constant
extern const float lbl_eu_806663BC;                  // .sdata2 constant
float lbl_eu_8066AF20;                               // .sdata2 constant
// The CfCam_ps.inl kernel body is visible in this TU; MWCC would otherwise
// inline it. noinline keeps the retail `bl func_8006C6E8` at call sites.
extern "C" __declspec(noinline) void func_8006C6E8(nw4r::math::VEC3* out,
                                                    const nw4r::math::VEC3* a,
                                                    const nw4r::math::VEC3* b);
// In-TU definition (CfCam.cpp) whose nw4r VEC3LenSq body MWCC would inline;
// noinline keeps the retail `bl func_8006DFC8` at call sites.
extern "C" __declspec(noinline) f32 func_8006DFC8(const nw4r::math::VEC3* vector);
float lbl_eu_80663DD4;                               // .sbss float (retail 0x80663DD4)
float lbl_eu_80663DD8;                               // .sbss float
extern const float lbl_eu_80666330;                             // .sdata2 constant
float lbl_eu_806662D4;                               // .sdata2 constant
float lbl_eu_80666370;                               // .sdata2 constant
float lbl_eu_80666374;                               // .sdata2 constant
float lbl_eu_80663DC8;                               // .sbss float
float lbl_eu_80663DCC;                               // .sbss float
float lbl_eu_8066635C;                               // .sdata2 constant
float lbl_eu_8066638C;                               // .sdata2 constant
float lbl_eu_80666324;                               // .sdata2 constant
float lbl_eu_8066A1FC;                               // .sdata2 constant (2*pi)
extern const float lbl_eu_80666308;                               // .sdata2 constant
extern const float lbl_eu_80666390;                               // .sdata2 constant
extern const float lbl_eu_806662CC;                               // .sdata2 constant
extern const float lbl_eu_8066A210;                               // .sdata2 constant (degrees-to-radians scale)
extern float lbl_eu_806662BC;                               // .sdata2 constant
extern float lbl_eu_806662C0;                               // .sdata2 constant
extern float lbl_eu_806662C4;                               // .sdata2 constant
extern float lbl_eu_806662C8;                               // .sdata2 constant
extern const float lbl_eu_80666318;                               // .sdata2 constant
float lbl_eu_80661B58;                               // .sdata float (retail 0x80661B58)
// .bss camera-defaults block (retail 0x80570A20): nine vec3 slots followed by
// trailing state words zeroed by the sinit.
struct CamDefaultsBlock {
    ml::CVec3 dir[9]; // 0x00..0x6c
    u8 rest[0x194];   // 0x6c..0x200
};
CamDefaultsBlock lbl_eu_80570A20;
float lbl_eu_80661B54;                               // .sdata float (retail 0x80661B54)
float lbl_eu_80661B5C;                               // .sdata float (retail 0x80661B5C)
float lbl_eu_80661B64;                               // .sdata float (retail 0x80661B64)
float lbl_eu_80661B68;                               // .sdata float (retail 0x80661B68)
float lbl_eu_80661B6C;                               // .sdata float (retail 0x80661B6C)
float lbl_eu_80661B70;                               // .sdata float (retail 0x80661B70)
float lbl_eu_80661B78;                               // .sdata float (retail 0x80661B78)
float lbl_eu_80661B80;                               // .sdata float (retail 0x80661B80)
float lbl_eu_80661B84;                               // .sdata float (retail 0x80661B84)
float lbl_eu_80661B88;                               // .sdata float (retail 0x80661B88)
float lbl_eu_80661BAC;                               // .sdata float (retail 0x80661BAC)
float lbl_eu_80663DDC;                               // .sdata float (retail 0x80663DDC)
extern unsigned int lbl_eu_80527244[];               // .data word table (retail 0x80527244)
extern const float lbl_eu_80666304;                  // .sdata2 constant
extern const float lbl_eu_806662F4;                  // .sdata2 constant
extern const float lbl_eu_806662FC;                  // .sdata2 constant
extern const float lbl_eu_8066630C;                  // .sdata2 constant
extern const float lbl_eu_80666310;                  // .sdata2 constant
extern const float lbl_eu_80666314;                  // .sdata2 constant
float lbl_eu_80661B38;                               // .sdata float (retail 0x80661B38)
u32 lbl_eu_80661B7C;                                  // .sdata word (retail 0x80661B7C)
float lbl_eu_80661B8C;                               // .sdata float (retail 0x80661B8C)
float lbl_eu_80661B90;                               // .sdata float (retail 0x80661B90)
float lbl_eu_80661B94;                               // .sdata float (retail 0x80661B94)
u8 lbl_eu_80661BB0;                                  // .sdata byte gate (retail 0x80661BB0)
extern const float lbl_eu_80666344;                  // .sdata2 constant
extern const float lbl_eu_80666348;                  // .sdata2 constant
extern const float lbl_eu_80666350;                  // .sdata2 constant
float lbl_eu_80661BA0;                               // .sdata float (retail 0x80661BA0)
float lbl_eu_80661B98;                               // .sdata float (retail 0x80661B98)
float lbl_eu_80661B9C;                               // .sdata float (retail 0x80661B9C)
u8 lbl_eu_80663DE1;                                  // .sdata byte (retail 0x80663DE1)
float lbl_eu_80663DD0;                               // .sbss float (retail 0x80663DD0)
float lbl_eu_80570A74[3];                            // .bss vec3 (retail 0x80570A74)
float lbl_eu_80570A80[3];                            // .bss vec3 (retail 0x80570A80)
extern char lbl_eu_804FB4F0[];                       // .data message string (retail 0x804FB4F0)
void func_8006CE18(cf::CfCamFollow* self);           // CfCam sibling: zero the 0x1D4 flag word (retail 0x8006D870)
int func_80074CD4(void* obj, u32 mask);              // CfCam sibling pad-action dispatcher (retail 0x80075568)
void* getActiveCameraObject__Q22cf13CfGameManagerFv();       // CfGameManager active-camera lookup (retail 0x80082BA0)
class UnkClass_800821F8;
UnkClass_800821F8* getCameraDataBlock__Q22cf13CfGameManagerFv(); // state object getter (retail 0x80082B7C); must match object/CfObjectMove.hpp's declaration
// .bss second camera-defaults block (retail 0x80527160): three stride-0x34
// records plus trailing space, keeping the original 0x100 footprint.
// Per-camera-id default entry in the 0x80527160 table (stride 0x34).
struct CamDefEntry {
    u32 field_0x00;
    ml::CVec3 unk04;
    ml::CVec3 unk10;
};
struct CamDefTable {
    CamDefEntry entries[3]; // 0x00..0xcc
    u8 tail[0x34];          // 0xcc..0x100
};
CamDefTable lbl_eu_80527160;
extern const char* lbl_eu_805271FC[];                // .data bdat-table-name entries (retail 0x805271FC)
float lbl_eu_80666328;                               // .sdata2 constant
void* func_8006D400(void* obj);                      // CfCam sibling (retail 0x8006D400)
float func_8006D408(void* pose);                     // CfCam sibling (retail 0x8006D408)
void* func_80496264(void* scene, int index);         // scene pose/xform block lookup
void func_8006D440(void* self);                      // CfCam sibling (retail 0x8006D440)
// Call-through-overload views of in-TU bodies (differing signatures keep the
// retail flat-name bl instead of a C++-mangled reloc).
void func_8006DFE0(void* self, const void* arg2, float* out1, float* out2,
                   int flag, float dist);
void func_8006E0C8(void* self, const void* arg2, float* out1, float* out2,
                   int flag);
extern const double lbl_eu_806662A8;                 // .sdata2 double constant (retail 0x806662A8)
extern const f32 lbl_eu_8066639C;                    // .sdata2 constant
extern const f32 lbl_eu_8066640C;                    // .sdata2 constant
extern const f32 lbl_eu_80666410;                    // .sdata2 constant
extern const f32 lbl_eu_80666414;                    // .sdata2 constant
extern const f32 lbl_eu_806663C8;                    // .sdata2 constant
extern const f32 lbl_eu_80666354;                    // .sdata2 constant
}

class CfCamEventManager;

// CfCam sibling imports used by func_8006C740 / func_8006D8D0 / func_80071754
// / func_8006E5D8 (same naming class as the blocks above: unmangled retail
// func_* names + lbl_eu_* data, so call relocs keep the retail symbol).
extern "C" {
CfCamEventManager* func_8006E59C();       // camera-event manager getter
cf::CfObject* func_8006E5A4(CfCamEventManager* mgr); // active camera-state getter
void func_8006E5AC(void*, void*);         // CfCam sibling block copy (dst, src)
int func_8006E5B8(void* self);            // CfCam sibling count (ignores its arg)
void func_8006E5C0(void*, void*);         // CfCam sibling word copy (dst, src)
void func_8006E5CC(void*, void*);         // CfCam sibling halfword copy (dst, src)
void func_8006C730(cf::CfCamFollow* self, u32 mask); // CfCam sibling flag set
int func_800B8920(void* obj);             // follow-target liveness probe
void* func_800BBC0C(void* obj);           // talk-source getter (takes an arg here)
void* func_8006C670(void* obj);           // CfCam sibling
unsigned int func_8006CA20(const void* self); // CfCam sibling (in-TU def has const CfCamFollow*)
void func_8006BC1C(void* self, int mask); // CfCam sibling flag clear
int func_80071A90(void* self);            // CfCam sibling
void func_80071A9C(void* self);            // CfCam sibling
int func_8006EEF8(void* self);            // CfCam sibling gate
int func_8006DC34(void* x);               // CfCam sibling probes
int func_8006DC40(void* x);
int func_8006DC4C(void* x, u32 mask);
int func_8006DC64(void* x);
int func_8006DC70(void* x);
float func_8006DC7C(void* x);
float func_8006DC84(void* x);
float func_8006DC8C(void* x);
int func_8006DC94(void* x);
int func_8004B848(void* x);               // vec helper probe
void func_8004B0B4(void* v);              // vec helper
void* func_8016FE34(void* source);        // object state getter
float lbl_eu_8066629C;                    // .sdata2 constant
float lbl_eu_80666300;                    // .sdata2 constant
float lbl_eu_8066631C;                    // .sdata2 constant
float lbl_eu_80666320;                    // .sdata2 constant
float lbl_eu_80666334;                    // .sdata2 constant
float lbl_eu_80666338;                    // .sdata2 constant
float lbl_eu_80570A2C[3];                 // .bss vec3 (retail 0x80570A2C)
u8 lbl_eu_80570A8C[];                     // .bss fallback camera-state buffer
// In-TU definitions (CfCam.cpp) whose differing signatures keep the bl at
// call sites (see the extern-C-only-views pattern above).
extern "C" void func_80071398(void* out, void* a, void* b, float t); // in-TU def: (Quaternion*, const Quaternion*, const Quaternion*, float)
extern "C" void* func_80071364(void* q);                            // in-TU def: (Quaternion*)
extern "C" int func_8006DBD4(cf::CVoiceRec* self, int flags);     // in-TU def (defined below its first use)
extern "C" __declspec(noinline) void func_80071694(ml::CMat33* out, const ml::CQuat* q); // in-TU def: quat -> 3x3 rotation matrix (mixed linkage keeps the unmangled bl)
}

// Imports used by func_8006DD58 / func_8006D450 / func_8006F9EC /
// func_8006FD3C (flat retail names).
extern "C" {
__declspec(noinline) int func_8006DF9C(void* obj);
__declspec(noinline) int func_8006DCA0(void* obj); // in-TU def (follow-cam arts gate)
int testResInfoFlag(unsigned int mask);
int func_800FE68C();
int func_8006DFA8(int obj);
int findObjectById__Fi(int arg);
int CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv(void* actor);
int func_8006DFBC(void* obj);
float func_800504BC(const f32* a, const f32* b); // vec dot product
float func_8006D410(float x);
extern const float lbl_eu_8066633C; // .sdata2 constant
extern const float lbl_eu_80666360; // .sdata2 constant
extern const float lbl_eu_8066634C; // .sdata2 constant
extern const float lbl_eu_80666364; // .sdata2 blend gain
}

// 48-byte / 12-word camera data block copied whole by func_8006EF1C.
struct CfCamWordBlock48 {
    u32 words[12];
};

extern "C" cf::CfCamFollow* __ct__cf_CfCamFollow(cf::CfCamFollow* self, void* arg1, void* arg2);
