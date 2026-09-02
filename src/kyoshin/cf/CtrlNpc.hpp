#pragma once

#include <types.h>
#include <revolution/MTX.h>  // nw4r::math::VEC3 (segment-projection helper decl)
#include "kyoshin/cf/object/CObjectState.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"
namespace cf {

// Word-triple input for func_80094DF4 (raw word copy into field_C8..field_D0);
// also the return shape of CCtrlNpcChar's position getter at vtable +0xac.
struct CtrlNpcVec3W {
    u32 x;   // 0x00
    u32 y;   // 0x04
    u32 z;   // 0x08
};

// Name-index search helper reached through CCtrlNpcChar::field_98 (vtable
// +0x44 query consumed by func_80093F28; returns -1 when the name is not
// found). Never instantiated here, so no vtable is emitted.
class CCtrlNpcSearch {
public:
    virtual int findNameIndex(const char* name);   // +0x44 retail (was _v44)

    u8 _pad00[0x7A0];
    u32 field_7A4;   // flags word (bit 0x00010000 tested by func_8009398C)
};

// Object referenced by CCtrlNpcChar::field_C4 (the NPC's C4 status object;
// retail null-tests the pointer and dereferences the +0x4EC flags word off
// it). Only the word this TU reads is declared.
struct CCtrlNpcC4Object {
    u8 _pad00[0x4EC];
    u32 field_4EC;   // flags word (bit 0x02000000 tested)
};

// Character-model object reached through CtrlNpc::field_28 (CfObjectMove
// family, cf-chain vtable). Real virtuals are on CfObject/CfObjectModel/
// CfObjectMove/CObjectState/CObjectParam - this struct is now a plain data
// view with no vtable; virtual calls go via the owning class.
struct CCtrlNpcChar {
    u32 field_04;
    u8 _pad08[0x68 - 0x08];       // 0x08..0x67
    u32 field_68;                 // 0x68 flags word (bits 0x00100000 / 0x2000 tested)
    u8 _pad6C[0x74 - 0x6C];       // 0x6C..0x73
    u8* field_74;                 // 0x74 opaque text-object handle
    u8 _pad78[0x8C - 0x78];       // 0x78..0x8B
    u16 field_8C;                 // 0x8C npc-kind halfword
    u8 _pad8E[0x98 - 0x8E];       // 0x8E..0x97
    CCtrlNpcSearch* field_98;     // 0x98 name-index search helper
    u8 _pad9C[0xC4 - 0x9C];       // 0x9C..0xC3
    CCtrlNpcC4Object* field_C4;   // 0xC4 C4 status object (null-tested)
    u8 _padC8[0x6C4 - 0xC8];      // 0xC8..0x6C3
    u32 field_6C4;                // 0x6C4 state word (0x21..0x2A = active states)
};
// View of CfObject at field_28 for field access (overlay, not inheritance)
inline CCtrlNpcChar* NpcCharView(void* obj) { return reinterpret_cast<CCtrlNpcChar*>(obj); }

// CtrlNpc's retail ctor (__ct__CtrlNpc) is a plain free function that stores
// the vtable pointers manually (MWCC_CASES §4193 pattern: short-form ctor
// + real C++ virtual dtor + novtable so no vtable/RTTI data is emitted here).
class __declspec(novtable) CtrlNpc {
public:
    virtual ~CtrlNpc();

    // Controller field layout (recovered from the unit's retail assembly).
    // The 0x30 region holds a CCtrlMoveNpc movement sub-object (see
    // CtrlMoveNpc.cpp); only the fields the CtrlNpc unit touches are named.
    u32 field_04;                  // 0x04
    f32 field_08;                  // 0x08
    f32 field_0C;                  // 0x0C
    f32 field_10;                  // 0x10
    f32 field_14;                  // 0x14
    u32 field_18;                  // 0x18
    u32 field_1C;                  // 0x1C
    u8 _pad20[0x24 - 0x20];        // 0x20..0x23
    u32 field_24;                  // 0x24
    cf::CfObjectMove* field_28;        // 0x28 character object (CfObjectMove family)
    u32 field_2C;                  // 0x2C
    u8 _sub30[0xAC - 0x30];        // 0x30..0xAB (CCtrlMoveNpc sub-object at 0x30)
    // Raw-word / float views of the movement-target words (ctor stores floats,
    // func_80094E44 / func_80094EDC copy the position getter's raw words).
    union { f32 f; u32 u; } field_AC;  // 0xAC
    union { f32 f; u32 u; } field_B0;  // 0xB0
    union { f32 f; u32 u; } field_B4;  // 0xB4
    s16 field_B8;                  // 0xB8
    s16 field_BA;                  // 0xBA
    s16 field_BC;                  // 0xBC
    s16 field_BE;                  // 0xBE busy counter
    u16 field_C0;                  // 0xC0
    u8 field_C2;                   // 0xC2
    u8 field_C3;                   // 0xC3
    u16 field_C4;                  // 0xC4
    u16 field_C6;                  // 0xC6
    u32 field_C8;                  // 0xC8
    u32 field_CC;                  // 0xCC
    u32 field_D0;                  // 0xD0
    f32 field_D4;                  // 0xD4
    f32 field_D8;                  // 0xD8
    u16 field_DC;                  // 0xDC
    s16 field_DE;                  // 0xDE
    // Movement target array at 0xE0..0xF7 (8 x 12-byte vec-word triples).
    // Written as raw words by some helpers, copied element-wise by
    // func_800948F8; readers needing floats reinterpret_cast to ml::CVec3.
    struct NpcTargetPos {
        u32 x;
        u32 y;
        u32 z;
    };
    union {
        NpcTargetPos field_E0[8];      // 0xE0..0xF7 movement targets
        u32 field_E0w[24];             // raw-word view
    };
    u8 _padF8[0x158 - 0x140];      // 0x140..0x157
    f32 field_158;                 // 0x158
    u16 field_15C;                 // 0x15C
    u8 _pad15E[0x160 - 0x15E];     // 0x15E..0x15F
    u32 field_160;                 // 0x160
    u32 field_164;                 // 0x164
    u16 field_168;                 // 0x168 event-window length (frames)
    u16 field_16A;                 // 0x16A elapsed movement time (frames*60 scale)
    s16 field_16C;                 // 0x16C
    s16 field_16E;                 // 0x16E
    s16 field_170;                 // 0x170
    u16 field_172;                 // 0x172
    u16 field_174;                 // 0x174
    u8 _pad176[0x178 - 0x176];     // 0x176..0x177
    f32 field_178;                 // 0x178
    u32 field_17C;                 // 0x17C saved bdat row index
};

} // namespace cf

// Forward decl for the ctor's factory call (func_800BBC0C takes a cf::CfObject).
namespace cf { class CfObject; }

// Cross-unit imports (unmangled retail names; global scope keeps the MWCC
// symbol/reloc name unmangled at the call sites).
namespace cf { class CCtrlMoveNpc; }

// Closest-point-on-segment projection helper (retail func_800A49E4, defined
// in code_800A3B24.cpp); signature must match the definition's mangling.
bool func_800A49E4(const nw4r::math::VEC3& a, const nw4r::math::VEC3& b,
                   const nw4r::math::VEC3& c, float r2);
// FSqrt-style sqrt with nw4r assert (retail func_800A3EF4).
extern "C" float func_800A3EF4(float x);
// XZ-plane segment length helper (code_800A3B24.cpp).
extern "C" f32 func_800A3DF8(const ml::CVec3& v);
// Ground-probe walk helper (code_800A3B24.cpp); third arg is a packed flag word.
extern "C" int func_800A72E0(const ml::CVec3* self, ml::CVec3* out, s32 flags,
                             f32 f1, f32 f2);
// Position-region copy helper (retail func_804B0B54). Same ABI as
// CfObjectMove.hpp / CfGimmickLock.hpp (void*, const float*).
extern "C" void func_804B0B54(void* region, const float* vec);
// CfGameManager play-frame helper (see CfObjectImplWalker.hpp).
extern "C" u32 getControllerWordA37C__Q22cf13CfGameManagerFv();
// Battle-status position getter (retail func_800BE0B0): returns a Vec*
// (the character object's +0x54 sub-object) fed straight into PSVECMag.
extern "C" void* func_800BE0B0(void* self);

// Unsigned-int -> double conversion scratch (MWCC 0x4330 idiom): build the
// 0x43300000-prefixed double on the stack, then subtract the magic constant
// lbl_eu_806666E8 instead of letting the compiler pool its own cookie.
union CtrlNpcCvtDbl {
    u32 w[2];   // w[0] = high word (big-endian)
    f64 d;
};

#include "kyoshin/plugin/ocBdat.hpp"  // getBdatStringColumnValue (owner)

// Talk/page controller update helpers (func_80093F28).
extern "C" int func_801413DC(unsigned int arg0, int arg1);
extern "C" u32 func_8009CF8C(u32 resourceId);
extern "C" void* getFP__FPCc(const char* name);
extern "C" void* func_8003AA34();
extern "C" void func_8013D07C(void* self, char* text, int flag);
extern "C" void func_8013D1E8(void* self);
extern "C" u32 getControllerWordA33C__Q22cf13CfGameManagerFv();
// C++-linkage decl: MWCC mangles the call to findObjectById__Fi (actor id ->
// action source object; the result has a position getter at vtable +0xac).
void* findObjectById(int id);

// Addressable raw-column holder for func_80093F28: full-word store of the
// getBdatStringColumnValue result, punned u8/u16 read on reload (retail's
// stw-then-lbz/lhz pairs around the column calls; same convention as
// CfEneColNarrow in CfObjectEne.hpp).
union NpcColNarrow {
    u32 w;
    u16 h;
    u8 b;
};

// bdat tables / strings used by func_80093F28.
// The page-hint halfwords sprintf'd into the bdat file name.
extern u16 lbl_eu_80663E42;
extern u32 lbl_eu_80663E28;   // global flags word (sbss)
extern u16 lbl_eu_80663E44;
extern const char lbl_eu_804FBB0C[];      // column-name / format string base
extern const char* lbl_eu_80527A48[];     // 6 per-kind column names
extern const char* lbl_eu_80527A60[];     // per-row column names (index / 3)
extern const char* lbl_eu_80527A80[];     // per-row column names (index / 3)

// Character-object factory called by the free-function ctor; defined in
// CfObjectModel.cpp (retail func_800BBC0C). extern "C" keeps the call reloc
// unmangled (plain C++ global decls get __F-suffix mangled at call sites).
// Signature matches the CfCam/CfCamEvent family form (void*(void*)) so the
// declarations coexist when those headers are visible; call sites cast.
extern "C" void* func_800BBC0C(void* obj);
// CCtrlMoveNpc ctor (retail symbol keeps the flat name); parent is passed in
// r4 at the retail call site even though the ctor body ignores it.
extern "C" void __ct__cf_CtrlMoveNpc(cf::CCtrlMoveNpc* self, cf::CtrlNpc* parent);
// Talk/page helper: takes the character object's field_C4 word, returns a
// page id (unsigned compare at the call site).
extern "C" u32 func_8004C5EC(void* arg);

extern "C" void func_8019F6E8(cf::CCtrlMoveNpc* self, const ml::CVec3* vec, f32 scale, f32 paramB);
extern "C" int func_8019F8E0(cf::CCtrlMoveNpc* self);
extern "C" void func_80094A9C(cf::CtrlNpc* self, const ml::CVec3* vec,
                              f32 scale, f32 paramB);
extern "C" int func_800A5038(const ml::CVec3* sub, const ml::CVec3* v, f32 f1, f32 f2);
// Same-TU forward decl: keeps the call reloc from func_800966E8 flat.
extern "C" int func_800964EC(cf::CtrlNpc* self);
// CfGameManager helpers: the retail symbols keep the Fv suffix but the real
// call sites pass arguments / read the return (same scheme as CTaskREvent.hpp),
// so C linkage emits the literal retail names at the call sites.
extern "C" void getControllerValues__Q22cf13CfGameManagerFv(u16* first, u16* second);
extern "C" u32 clearControllerState__Q22cf13CfGameManagerFv();
// Play-frame helper returning a u32 (CfGameManager.cpp defines it as a plain
// extern "C" free function returning lbl_eu_80663D90).
extern "C" u32 getFieldD90Value__Q22cf13CfGameManagerFv();
// Movement-timer helper defined in this TU (retail name unmangled); C linkage
// keeps the call reloc from func_80096488 matching retail.
extern "C" void func_8009565C(cf::CtrlNpc* self);

// Retail float constants (sdata2) referenced by the CtrlNpc unit.
extern const f32 lbl_eu_80666698;
extern const f32 lbl_eu_8066669C;
extern const f32 lbl_eu_806666A0;
extern const f32 lbl_eu_806666A4;
extern const f32 lbl_eu_806666A8;
extern const f32 lbl_eu_806666AC;
extern const f32 lbl_eu_806666C8;
extern const f32 lbl_eu_806666CC;
extern const f32 lbl_eu_806666D4;
extern const f32 lbl_eu_806666D8;
extern const f32 lbl_eu_806666DC;
extern const f32 lbl_eu_806666E4;
extern const f32 lbl_eu_8066A208;
extern const f32 lbl_eu_806666B0;
extern const f32 lbl_eu_806666B4;
extern const f32 lbl_eu_806666B8;
extern const f32 lbl_eu_806666BC;
extern const f32 lbl_eu_806666D0;
extern const f64 lbl_eu_806666C0;
extern const f32 lbl_eu_806666E0;
extern const f64 lbl_eu_806666E8;   // 0x4330 int->double magic constant
extern const f32 lbl_eu_806666F0;
extern const f32 lbl_eu_806666F4;
extern const f32 lbl_eu_806666F8;
extern const f32 lbl_eu_806666FC;
extern const f32 lbl_eu_80666704;
extern const f32 lbl_eu_80666700;
extern const f32 lbl_eu_80666708;
extern const f32 lbl_eu_8066670C;
extern const f32 lbl_eu_80666710;
extern const f32 lbl_eu_80666714;
extern const f32 lbl_eu_8066A1F8;
extern const f32 lbl_eu_8066A210;
// nw4r FSqrt warning strings (rodata).
extern const char lbl_eu_80526324[];
extern const char lbl_eu_80526300[];
// Global flags word (sbss) read by func_80094D1C.
extern u32 lbl_eu_80663E28;
// Retail vtables stored by the free-function ctor (data lives in another TU).
extern const u32 lbl_eu_80527BB0[];
extern const u32 lbl_eu_80527B38[];

// Member-function-pointer dispatch tables used by func_8009398C's per-action
// state handlers ((self->*tbl[field_BE])() -> __ptmf_scall in retail).
typedef void (cf::CtrlNpc::*CtrlNpcActionMfp)();
extern const CtrlNpcActionMfp lbl_eu_80527AE0[4];   // actions 1..2 handlers
extern const CtrlNpcActionMfp lbl_eu_80527AA0[2];   // action 4 handlers
extern const CtrlNpcActionMfp lbl_eu_80527AB8[3];   // action 5 handlers
extern const CtrlNpcActionMfp lbl_eu_80527B10[3];   // action 3 handlers

// CfGameManager singleton + presentation-flag probe (isGlobalCamFlagSet__Fi).
extern "C" void* getInstance__Q22cf13CfGameManagerFv();
bool isGlobalCamFlagSet(int mask);
// Search-helper helpers reached with CCtrlNpcChar::field_98.
namespace cf { struct CfObjectModelSub98; }
extern "C" float func_80484F18(cf::CCtrlNpcSearch* obj);
extern "C" void func_804876DC(cf::CfObjectModelSub98* sub);
