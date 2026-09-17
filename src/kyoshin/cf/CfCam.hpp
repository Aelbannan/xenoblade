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
class CfObject;     // active camera-state object (cfCam_getActiveObj return)
} // namespace cf

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

extern "C" bool func_800755B0(void*, int);
extern "C" bool func_8049EB60(void* obj);
extern "C" u32 cfCam_loadUnk08(void* self);                          // CfCam sibling gate
extern "C" int cfCam_testUnk04M(int controllerId, int unk); // CfCam sibling pad check
// Copies a 12-byte vector (dst, src); returns dst (retail: copyVec3Words leaves
// its first arg in r3). C linkage so call relocs keep the unmangled retail
// symbol name (MWCC mangles plain C++ global declarations).
extern "C" void* copyVec3Words(void*, void*);
extern "C" int cfCam_bit1At0x64(void* arg);           // CfCam sibling (FULL_MATCH us-8006cc08)
extern "C" void func_8006B720(int arg1, int arg2); // CfCam sibling (us-8006c178)
extern "C" float lbl_eu_80527230[];
extern "C" float lbl_eu_80663DE4;     // .sbss float (retail 0x80663DE4)
extern "C" float lbl_eu_80663DE8;     // .sbss float (retail 0x80663DE8)
extern "C" float lbl_eu_80570A38[3];  // .bss vec3 (retail 0x80570A38)
// .bss float (retail 0x80570A44). Sized >8B so MWCC uses full lis/@l addressing
// (SDA threshold) - only [0] is ever referenced.
extern "C" float lbl_eu_80570A44[3];
extern "C" float lbl_eu_80570A50[3];  // .bss vec3 (retail 0x80570A50)
extern "C" float lbl_eu_80570A5C[6];  // .bss 6 floats (retail 0x80570A5C, size 0x18)
extern "C" void func_804BE4B4(void*, int);
extern "C" void* func_804BE4E0(void*, int);
extern "C" float scaleByGlobal(float val);
extern "C" u32 CfRes_getHeapHandle();                          // mtl heap handle for cf allocations
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);
// func_8006F5C8 callees (flat retail names, same naming class)
extern "C" int func_80075640();
extern "C" int cfCam_testPad100(void* self);
extern "C" int func_80275378(void* self);
extern "C" int func_80275338(void* self);
extern "C" int func_802752B8(void* self);
extern "C" int func_802752F8(void* self);
extern "C" int CfRes_getE24Bit22();
extern "C" int cfCam_getE28Bit24();
extern "C" int func_801B0F8C();
extern "C" int func_8017FD44();
extern "C" int func_802751F8(void* self);
extern "C" int isSceneActive__Q22cf13CfGameManagerFv();
extern "C" int cfCam_maskUnk1D4(int self, int mask);        // CfCam sibling flag probe (0x1D4 word)
extern "C" void cfCam_resetHist(void* self, void* src);     // CfCam sibling (retail 0x8006E18C)
extern "C" __declspec(noinline) u8* cfCam_vec3AtIndex(u8*, int); // CfCam sibling (retail 0x8006BF14), body at bottom of CfCam.cpp; noinline keeps the retail bl at call sites
// In-TU ctor helpers whose bodies MWCC would otherwise inline; noinline keeps
// the retail `bl func_8006BE*` calls in __ct__cf_CfCamFollow.
extern "C" __declspec(noinline) void cfCam_nopBef8(void* self);
extern "C" __declspec(noinline) void cfCam_nopCtorSlot(void* self);
extern "C" __declspec(noinline) void cfCam_zeroUnk04(cf::CfCamFollow* self);
extern "C" __declspec(noinline) void cfCam_storeZeroU32(void* self);
extern "C" __declspec(noinline) void cfCam_storeZeroU16(u16* self);
// .data lookup tables (retail 0x80527208 / 0x8052721C, 20 bytes each). Sized
// >8B so MWCC uses full lis/@l addressing (SDA threshold).
extern "C" float lbl_eu_80527208[5];
extern "C" float lbl_eu_8052721C[5];
// .sdata floats (retail 0x80661B3C, 0x80661B40, 0x80661B44, 0x80661B48,
// 0x80661B74). Scalar stores - only the .data tables below are indexed.
extern "C" float lbl_eu_8066A200;     // .sdata2 pi/2 (retail 0x8066A200)
extern "C" u8 lbl_eu_80663DE0;        // .sdata byte gate (retail 0x80663DE0)
extern "C" void func_80059610(void*, float);        // retail 0x80059C78
extern "C" void cfCam_setClear04(void* obj, u32 mask, int flag); // CfCam sibling flag set (retail 0x8006BBF4)
extern "C" void cfCam_refreshBA8();                              // CfCam sibling (retail 0x8006B948)
extern "C" void cfCam_nopCtorSlot(void* self);                    // CfCam sibling no-op (retail 0x8006BEC0)
extern "C" void cfCam_getFollowSrc(ml::CVec3* out, cf::CfCamFollow* self); // CfCam sibling (retail 0x8006CA1C); same signature as the in-TU def so call sites keep the unmangled bl
extern "C" void cfCam_offsetFromDir(void* a, void* b, void* c, void* d, float f); // CfCam sibling (retail 0x80073C7C)
extern "C" float* cfCam_makeYRotMtx(float* mtx, float scale);       // CfCam sibling (retail 0x800742FC)
extern "C" void cfCam_setTransCol(float* mtx, const float* v);    // CfCam sibling (retail 0x800743A4)
// extern-C-only views of in-TU C++ definitions (differing signature keeps the bl):
extern "C" void cfCam_multMtxVec(void* dst, void* mtx, void* src); // in-TU def: (Vec*, const Mtx, const Vec*)
extern "C" void func_800743C0(void* mtx);                       // in-TU def: (nw4r::math::MTX34*)
extern "C" __declspec(noinline) void cfCam_loadTuneTbl();
extern "C" void func_800598A8(void*, void*, void*); // retail 0x80059F10
extern "C" void nopFunc(void*);                     // retail 0x80059C74 (no-op stub)
// Flat retail C names (same naming class as lbl_* symbols) - CfCam cross-TU
// callees and siblings:
extern "C" float cfCam_absFloat(void* self); // CfCam sibling (retail 0x8006C548); body sits under the C++-mangled name in CfCam.cpp so callers keep the bl
extern "C" float cfCam_yawFromVec(const void* v); // CfCam sibling (retail 0x8006DE28); yaw from x/z; body under the C++-mangled name so callers keep the bl
// Call-through-overload views of in-TU bodies (differing signatures keep the
// bl instead of letting MWCC inline the local definition):
extern "C" float cfCam_pitchFromVec(const void* v); // spherical azimuth wrapper (retail 0x8006DD78)
extern "C" float cfCam_acosF32(double x);      // acos wrapper view (retail 0x8006D41C); body is the float overload
extern "C" float cfCam_vecLength(const void* v); // CfCam sibling (retail 0x8006C558): `b PSVECMag` tail call
extern "C" void subVec3f(f32* out, const f32* a, const f32* b); // vec3 subtract out = a - b (retail 0x8004CB80)
extern "C" float func_800A3EF4(float x);   // FSqrt-style sqrt with nw4r assert (retail 0x800A3EF4)
extern "C" f32 atan2AnimFIdx(f32 a, f32 b); // sin wrapper; CfCam call sites pass 2 args (retail 0x8004D2D8)
extern "C" f32 wrapAnglePi(f32 value);   // wrap angle into [-pi, pi) (retail 0x8004C300)
extern "C" float lbl_eu_8066A1F8;          // .sdata2 pi (retail 0x8066A1F8)
extern char lbl_eu_80526324[];              // nw4r FSqrt assert source-file string (retail .data)
extern char lbl_eu_80526300[];              // nw4r FSqrt assert message string (retail .data)
extern char lbl_eu_805262F0[];              // nw4r assert source-file string (retail .data)
extern char lbl_eu_805262C8[];              // nw4r assert message string (retail .data)
// func_8006E884 cross-TU imports (flat retail names).
extern "C" int func_8007560C();             // gate probe
extern "C" void cfCam_nopVirtFloat(void* self, float f);
extern "C" void* cfCam_getPlus9C(void* obj);  // word-block source getter
extern "C" void* cfCam_getPlus3D8(void* obj);  // sub-object getter (+4 vec)
extern "C" int func_802753B8(void* self);   // cam-control register probe

namespace cf {

// NOTE: everything type-y lives in THIS single namespace block. MWCC 10140
// breaks qualified lookups (cf::X -> "undefined identifier") when types are
// declared across multiple re-openings of namespace cf, so later blocks were
// merged here on purpose.
struct CfCamFollow; // fwd decl for extern-C imports below (full layout later)
class CfObject;     // active camera-state object (cfCam_getActiveObj return)
// Owner view for cfCam_queryVoxArts's +0x04 sub-object (slot 0x30 -> query word).
// Owned by kyoshin/help/CHelp_Talk.hpp (CVoiceRec/CVoiceSub30: result of
// func_8016FE34) - repeated here so this TU stays self-contained;
// same shape as CHelp_Talk::func_802B86F0 (voice->field_04->vf30()).
struct CVoiceSub30 {
    virtual void* f00();
    virtual void* f01();
    virtual void* f02();
    virtual void* f03();
    virtual void* f04();
    virtual void* f05();
    virtual void* f06();
    virtual void* f07();
    virtual void* f08();
    virtual void* f09();
    virtual u32* vf30(); // index 10 -> vtable 0x30 (returns pointed-to query word)
};
struct CVoiceRec {
    u8 _pad00[0x4];
    CVoiceSub30* field_04; // 0x04
};
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
    virtual void* cfCam_getPlus1C() = 0;              // 0x10 (leaf: this+0x1C)
    virtual void cfCam_copyToPlus28(void* src) = 0;      // 0x14 (leaf: copy 3 words to +0x28)
    virtual void cfCam_updFollowPos(void* arg, float f) = 0; // 0x18 (leaf: vector prep)
    virtual void* cfCam_nestedPlus118();                  // 0x1C (field_0x0C + 0x118)
    virtual void func_8006E884(float f) = 0;        // 0x20 (leaf: per-frame driver)
    virtual void cfCam_nopVirt24() = 0;               // 0x24 (leaf: no-op)
    virtual void func_8006BFDC() = 0;               // 0x28 (leaf: constants reset)
    virtual void* cfCam_copyToPlus10(void* src) = 0;     // 0x2C (leaf: this+0x10 copy)
    virtual void* cfCam_getPlus10();                  // 0x30 (this+0x10)
    virtual void* cfCam_getPlus40() = 0;              // 0x34 (leaf: this+0x40)
    virtual void cfCam_initFollowS(void* src) = 0;      // 0x38 (leaf: state-block copy)
    virtual void cfCam_setFollowD(float f) = 0;        // 0x3C (leaf: store heading)
    virtual void cfCam_storeUnk08(int arg);            // 0x40 (stores arg at +0x08)
    virtual void* cfCam_returnZero();                  // 0x44 (returns 0)
    virtual void cfCam_copyCamState(CfObject* src) = 0;  // 0x48 (leaf: state copy)
    virtual void func_800606AC();                   // 0x4C (pluginCam no-op)
    virtual void func_80060738();                   // 0x50 (pluginCam no-op)
    virtual void func_800607C4();                   // 0x54 (pluginCam no-op)
    virtual float cfCam_get1E0Again() = 0;              // 0x58 (leaf: +0x1E0 getter)
    virtual void cfCam_setFollowTg(void* target) = 0;   // 0x5C (leaf: stash +0x164)
    virtual void* cfCam_loadUnk164() = 0;              // 0x60 (leaf: +0x164 handle)
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
    virtual void* cfCam_getPlus1C();                  // 0x10
    virtual void cfCam_copyToPlus28(void* src);          // 0x14
    virtual void cfCam_updFollowPos(void* arg, float f); // 0x18
    virtual void func_8006E884(float f);            // 0x20
    virtual void cfCam_nopVirt24();                   // 0x24
    virtual void func_8006BFDC();                   // 0x28
    virtual void* cfCam_copyToPlus10(void* src);         // 0x2C
    virtual void* cfCam_getPlus40();                  // 0x34
    virtual void cfCam_initFollowS(void* src);          // 0x38
    virtual void cfCam_setFollowD(float f);            // 0x3C
    virtual void cfCam_copyCamState(CfObject* src);      // 0x48
    virtual float cfCam_get1E0Again();                  // 0x58
    virtual void cfCam_setFollowTg(void* target);       // 0x5C
    virtual void* cfCam_loadUnk164();                  // 0x60
    virtual void func_80073D8C(int cond);           // 0x68 (base holds func_80060A08 here)
    virtual void* cfCam_getActivePad();                  // 0x70 (new: pad-action source)
    virtual int func_80074AA4(int id);              // 0x74 (new: pad-action dispatch)
};

// Layout view over the follow-camera state block copied wholesale by
// cfCam_copyCamState from the dynamic-cast source object into CfCamFollow.
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
void cfCam_seedProbeVec(ml::CVec3* a, ml::CVec3* b);      // in-unit (retail 0x800711B0)
ml::CVec3* normVec3(ml::CVec3* vec);            // retail 0x8004D260 (normalize)
void scaleVec3f(float* destination, const float* source, float scale); // retail 0x8004BE34
void addVec3f(float* destination, const float* source);              // retail 0x8004BE10
f32 sinAnimFIdx(f32 angle);                        // retail 0x8004D300 (SinFIdx wrapper)
f32 cosAnimFIdx(f32 angle);                        // retail 0x8004D30C (CosFIdx wrapper)
float cfCam_xzLength(const f32* v);                   // in-TU def: (const ml::CVec3*); differing signature keeps the bl
void cfCam_dirToAngles(const ml::CVec3* a, const ml::CVec3* b, f32* out, int flag2, int flag3, f32 heading); // in-TU def (defined below its first use); retail callers pass self->unk244 in f1 - body ignores it
void cfCam_buildOffset(ml::CVec3* out, cf::CfCamFollow* self, const ml::CVec3* dir,
                   const f32* angles, float f); // in-TU def (defined below its first use)
__declspec(noinline) void func_8006FFA8(cf::CfCamFollow* self, float* outA, float* outB); // in-TU def, same signature (mixed linkage keeps the unmangled bl); noinline keeps the retail bl instead of inlining the body
void cfCam_copyBlock16(void* dst, void* src);            // retail 0x8007215C (16-byte copy)
void cfCam_applyRelPos(cf::CfCamFollow* self, void* a, void* b, int c, float f1, float f2); // in-TU stub (defined below its first use)
__declspec(noinline) void func_800707C0(void* self, void* a, void* b); // in-TU stub, same signature (mixed linkage keeps the unmangled bl)
int CfRes_getD80Flag();                              // scene flag gate (CUIErrMesWin.hpp)
// func_8049603C: declared once in libs/monolib/src/scn/CScn_8049603C.hpp
// (ellipsis form - this TU calls it with no args).

void cfCam_tuneZoomDist(cf::CfCamFollow* self);           // in-TU def (defined below its first use), same signature (mixed linkage)
ml::CVec3* copyVec3f(ml::CVec3* out, const ml::CVec3* v); // retail 0x8004BE74 (vec helper)
void cfCam_setClear1D4(cf::CfCamFollow* self, u32 mask, int flag); // CfCam sibling (retail 0x8006D098)
int getNullPtrC__Q22cf13CfGameManagerFv(u32);          // CfGameManager state gate (retail 0x8007FE24)
void cfCam_prepFollowVec(void* out, cf::CfCamFollow* self, void* dir, void* sel); // follow-cam vector prep
int isTimerActive__Q22cf13CfGameManagerFv();             // CfGameManager gate (retail 0x8007F91C)
void func_8049EFF8(void* obj, f32 f, void* a, void* b);  // retail 0x8049EFF8 (pose/scale apply)
int cfCam_getSignBit04(void* self);                           // CfCam sibling (retail 0x8006D374)
cf::CfCamFollow* cfCam_getActiveCam();                    // CfCam sibling (retail 0x80072708)
ml::CVec3* cfCam_getPlus60(cf::CfCamFollow* self);     // CfCam sibling (retail 0x80072710)
__declspec(noinline) u8* cfCam_getPlus6C(cf::CfCamFollow* self); // CfCam sibling (retail 0x80072718), in-TU body at bottom of CfCam.cpp; noinline keeps the bl at call sites
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
extern float lbl_eu_8066AF20;                               // owned elsewhere (not this split)
// The CfCam_ps.inl kernel body is visible in this TU; MWCC would otherwise
// inline it. noinline keeps the retail `bl cfCam_psAddVec3` at call sites.
extern "C" __declspec(noinline) void cfCam_psAddVec3(nw4r::math::VEC3* out,
                                                    const nw4r::math::VEC3* a,
                                                    const nw4r::math::VEC3* b);
// In-TU definition (CfCam.cpp) whose nw4r VEC3LenSq body MWCC would inline;
// noinline keeps the retail `bl cfCam_vecLenSq` at call sites.
extern "C" __declspec(noinline) f32 cfCam_vecLenSq(const nw4r::math::VEC3* vector);
extern float lbl_eu_8066A1FC;                               // owned elsewhere (not this split)
extern const float lbl_eu_8066A210;                               // .sdata2 constant (degrees-to-radians scale)
// .bss camera-defaults block (retail 0x80570A20): nine vec3 slots followed by
// trailing state words zeroed by the sinit.
struct CamDefaultsBlock {
    ml::CVec3 dir[9]; // 0x00..0x6c
    u8 rest[0x194];   // 0x6c..0x200
};
// Owned by another TU (CfScript.o defines the 0xC bss instance); this TU
// only references it, so declare extern (a tentative here would wrongly
// emit 0x200 into this TU's .bss).
extern CamDefaultsBlock lbl_eu_80570A20;
extern const void* lbl_eu_80527244[7];               // .data joint-name table (retail 0x80527244)
float lbl_eu_80570A74[3];                            // .bss vec3 (retail 0x80570A74)
float lbl_eu_80570A80[3];                            // .bss vec3 (retail 0x80570A80)
extern char lbl_eu_804FB4F0[];                       // .data message string (retail 0x804FB4F0)
void cfCam_zero1D4Word(cf::CfCamFollow* self);           // CfCam sibling: zero the 0x1D4 flag word (retail 0x8006D870)
int cfCam_testWordMask(void* obj, u32 mask);              // CfCam sibling pad-action dispatcher (retail 0x80075568)
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
// Owned by another TU (CfCamEvent.o defines the .data instance); extern
// here so this TU's .bss does not gain a bogus 0x88 tentative.
extern CamDefTable lbl_eu_80527160;
extern const char* lbl_eu_805271FC[];                // .data bdat-table-name entries (retail 0x805271FC)
void* cfCam_loadPlus4(void* obj);                      // CfCam sibling (retail 0x8006D400)
float cfCam_getFloat1E0F(void* pose);                     // CfCam sibling (retail 0x8006D408)
void* func_80496264(void* scene, int index);         // scene pose/xform block lookup
void cfCam_setBit9_04(void* self);                      // CfCam sibling (retail 0x8006D440)
// Call-through-overload views of in-TU bodies (differing signatures keep the
// retail flat-name bl instead of a C++-mangled reloc).
void cfCam_updateHead(void* self, const void* arg2, float* out1, float* out2,
                   int flag, float dist);
void cfCam_updateYaw(void* self, const void* arg2, float* out1, float* out2,
                   int flag);
}

class CfCamEventManager;

// CfCam sibling imports used by cfCam_getFollowSrc / func_8006D8D0 / func_80071754
// / cfCam_pushStateToActive (same naming class as the blocks above: unmangled retail
// func_* names + lbl_eu_* data, so call relocs keep the retail symbol).
extern "C" {
CfCamEventManager* cfCam_getEventMgr();       // camera-event manager getter
cf::CfObject* cfCam_getActiveObj(CfCamEventManager* mgr); // active camera-state getter
void cfCam_copyWordA(void*, void*);         // CfCam sibling block copy (dst, src)
int cfCam_histCount(void* self);            // CfCam sibling count (ignores its arg)
void cfCam_copyWordB(void*, void*);         // CfCam sibling word copy (dst, src)
void cfCam_copyHalf(void*, void*);         // CfCam sibling halfword copy (dst, src)
void cfCam_andcUnk1D4(cf::CfCamFollow* self, u32 mask); // CfCam sibling flag set
int lookupWorkAtAddr(void* obj);             // follow-target liveness probe
void* func_800BBC0C(void* obj);           // talk-source getter (takes an arg here)
void* cfCam_loadUnkC4(void* obj);           // CfCam sibling
unsigned int cfCam_getBit1_4EC(const void* self); // CfCam sibling (in-TU def has const CfCamFollow*)
void cfCam_andcUnk04(void* self, int mask); // CfCam sibling flag clear
int cfCam_getBit12_04(void* self);            // CfCam sibling
void cfCam_clearBits1200(void* self);            // CfCam sibling
int cfCam_getBit9_04(void* self);            // CfCam sibling gate
int cfCam_getBit26_4EC(void* x);               // CfCam sibling probes
int cfCam_getBit24_4EC(void* x);
int cfCam_testMask4EC(void* x, u32 mask);
int cfCam_getBit25_4EC(void* x);
int cfCam_getBit23_4EC(void* x);
float cfCam_getFloat4FC(void* x);
float cfCam_getFloat4F8(void* x);
float cfCam_getFloat508(void* x);
int cfCam_getBit13_4EC(void* x);
int testAnimMoveBit(void* x);               // vec helper probe
void noopAnimQuat(void* v);              // vec helper
void* func_8016FE34(void* source);        // object state getter
float lbl_eu_80570A2C[3];                 // .bss vec3 (retail 0x80570A2C)
u8 lbl_eu_80570A8C[];                     // .bss fallback camera-state buffer
// In-TU definitions (CfCam.cpp) whose differing signatures keep the bl at
// call sites (see the extern-C-only-views pattern above).
extern "C" void func_80071398(void* out, void* a, void* b, float t); // in-TU def: (Quaternion*, const Quaternion*, const Quaternion*, float)
extern "C" void* cfCam_normQuat(void* q);                            // in-TU def: (Quaternion*)
extern "C" int cfCam_queryVoxArts(cf::CVoiceRec* self, int flags);     // in-TU def (defined below its first use)
extern "C" __declspec(noinline) void func_80071694(ml::CMat33* out, const ml::CQuat* q); // in-TU def: quat -> 3x3 rotation matrix (mixed linkage keeps the unmangled bl)
}

// Imports used by func_8006DD58 / cfCam_recomputeEyeTarget / cfCam_syncFollowSnapshot /
// func_8006FD3C (flat retail names).
extern "C" {
__declspec(noinline) int cfCam_getBit2_64(void* obj);
__declspec(noinline) int cfCam_artsGateClr(void* obj); // in-TU def (follow-cam arts gate)
int testResInfoFlag(unsigned int mask);
int func_800FE68C();
int cfCam_getInt90E4(int obj);
int findObjectById__Fi(int arg);
int CActorParam_UnkVirtualFunc22__Q22cf11CActorParamFv(void* actor);
int cfCam_getBit27_64(void* obj);
float dotVec3f(const f32* a, const f32* b); // vec dot product
float cfCam_mulSda2Const(float x);
}

// 48-byte / 12-word camera data block copied whole by cfCam_copyBlock48.
struct CfCamWordBlock48 {
    u32 words[12];
};

extern "C" cf::CfCamFollow* __ct__cf_CfCamFollow(cf::CfCamFollow* self, void* arg1, void* arg2);

// .sdata2 pool code symbols (added postprocess by UnitRules; storage lives
// in CfCam.cpp as const cam_ objects so values never fold, while these
// extern-const decls compile code loads with CSE like retail).
extern const float lbl_eu_8066629C;
extern const float lbl_eu_806662A0;
extern const float lbl_eu_806662B0;
extern const float lbl_eu_806662B4;
extern const float lbl_eu_806662B8;
extern const float lbl_eu_806662BC;
extern const float lbl_eu_806662C0;
extern const float lbl_eu_806662C4;
extern const float lbl_eu_806662C8;
extern const float lbl_eu_806662CC;
extern const float lbl_eu_806662D0;
extern const float lbl_eu_806662D4;
extern const float lbl_eu_806662D8;
extern const float lbl_eu_806662DC;
extern const float lbl_eu_806662E0;
extern const float lbl_eu_806662E4;
extern const float lbl_eu_806662F0;
extern const float lbl_eu_806662F4;
extern const float lbl_eu_806662F8;
extern const float lbl_eu_806662FC;
extern const float lbl_eu_80666300;
extern const float lbl_eu_80666304;
extern const float lbl_eu_80666308;
extern const float lbl_eu_8066630C;
extern const float lbl_eu_80666310;
extern const float lbl_eu_80666314;
extern const float lbl_eu_80666318;
extern const float lbl_eu_8066631C;
extern const float lbl_eu_80666320;
extern const float lbl_eu_80666324;
extern const float lbl_eu_80666328;
extern const float lbl_eu_8066632C;
extern const float lbl_eu_80666330;
extern const float lbl_eu_80666334;
extern const float lbl_eu_80666338;
extern const float lbl_eu_8066633C;
extern const float lbl_eu_80666340;
extern const float lbl_eu_80666344;
extern const float lbl_eu_80666348;
extern const float lbl_eu_8066634C;
extern const float lbl_eu_80666350;
extern const float lbl_eu_80666354;
extern const float lbl_eu_80666358;
extern const float lbl_eu_8066635C;
extern const float lbl_eu_80666360;
extern const float lbl_eu_80666364;
extern const float lbl_eu_80666368;
extern const float lbl_eu_8066636C;
extern const float lbl_eu_80666370;
extern const float lbl_eu_80666388;
extern const float lbl_eu_8066638C;
extern const float lbl_eu_80666390;
extern const float lbl_eu_80666394;
extern const float lbl_eu_80666398;
extern const float lbl_eu_8066639C;
extern const float lbl_eu_806663A0;
extern const float lbl_eu_806663A4;
extern const float lbl_eu_806663A8;
extern const float lbl_eu_806663AC;
extern const float lbl_eu_806663B0;
extern const float lbl_eu_806663B4;
extern const float lbl_eu_806663B8;
extern const float lbl_eu_806663BC;
extern const float lbl_eu_806663C0;
extern const float lbl_eu_806663C4;
extern const float lbl_eu_806663C8;
extern const float lbl_eu_806663CC;
extern const float lbl_eu_806663D0;
extern const float lbl_eu_806663D4;
extern const float lbl_eu_8066640C;
extern const float lbl_eu_80666410;
extern const float lbl_eu_80666414;
extern const double lbl_eu_806662A8;
extern const double lbl_eu_806662E8;
extern const float lbl_eu_80666374;
