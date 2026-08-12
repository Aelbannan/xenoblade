#pragma once

#include <types.h>
#include "kyoshin/cf/object/CObjectState.hpp"
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
    virtual void _v08();
    virtual void _v0C();
    virtual void _v10();
    virtual void _v14();
    virtual void _v18();
    virtual void _v1C();
    virtual void _v20();
    virtual void _v24();
    virtual void _v28();
    virtual void _v2C();
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual int _v44(const char* name);   // +0x44
};

// Character-model object reached through CtrlNpc::field_28 (CfObjectMove
// family, cf-chain vtable). Only the vtable slots and the state words this TU
// touches are declared; never instantiated here, so no vtable is emitted.
class CCtrlNpcChar {
public:
    // cf-chain vtable: MWCC places the Nth declared virtual at vtable offset
    // (N+2)*4 (entries 0/1 are the RTTI/offset-to-top slots). Declared indices
    // 7, 20, 21 therefore land at +0x24 (CObjectState_UnkVirtualFunc8),
    // +0x58 and +0x5C (the CfObject vfunc2/vfunc3 pair); index 41 lands at
    // +0xac (position getter consumed by func_800951A0). Indices 42..69 are
    // dummy pins for the +0x84 / +0x11C slots read by func_800950E8.
    virtual void _v08();
    virtual int _v0C(int);
    virtual void _v10(u32);
    virtual void _v14();
    virtual void _v18(int);
    virtual void _v1C();
    virtual void _v20(int);
    virtual int _v24(int arg);    // +0x24
    virtual void _v28();
    virtual int _v2C(int a, int b);   // +0x2C
    virtual void _v30();
    virtual void _v34();
    virtual void _v38();
    virtual void _v3C();
    virtual void _v40();
    virtual void _v44();
    virtual void _v48();
    virtual int _v4C();               // +0x4C (id fed to func_800B708C)
    virtual void _v50();
    virtual void _v54();
    virtual void _v58();          // +0x58
    virtual void _v5C();          // +0x5C
    virtual void _v60();
    virtual void _v64();
    virtual void _v68();
    virtual void _v6C();
    virtual void _v70();
    virtual int _v74();           // +0x74 (returns active/idle state)
    virtual void _v78();
    virtual void _v7C();
    virtual void _v80();
    virtual int _v84();           // +0x84
    virtual void _v88();
    virtual void _v8C();
    virtual void _v90();
    virtual void _v94();
    virtual void _v98();
    virtual void _v9C();
    virtual void _vA0();
    virtual void _vA4();
    virtual void _vA8();
    virtual CtrlNpcVec3W* _vAC();  // +0xac position getter
    virtual void _vB0();
    virtual void _vB4();
    virtual void _vB8();
    virtual void _vBC();
    virtual void _vC0();
    virtual void _vC4(f32 arg);       // +0xC4 (set heading/turn amount)
    virtual void _vC8();
    virtual f32 _vCC();               // +0xCC (returns a heading)
    virtual void _vD0();
    virtual void _vD4();
    virtual f32 _vD8();          // +0xd8 (returns a heading/turn amount)
    virtual void _vDC();
    virtual void _vE0();
    virtual void _vE4();
    virtual void _vE8();
    virtual void _vEC();
    virtual void _vF0();
    virtual void _vF4();
    virtual void _vF8();
    virtual void _vFC();
    virtual void _v100();
    virtual void _v104();
    virtual void _v108();
    virtual void _v10C();
    virtual void _v110();
    virtual void _v114();
    virtual void _v118();
    virtual int _v11C();          // +0x11C
    virtual void _v120();
    virtual void _v124();
    virtual void _v128();
    virtual void _v12C();
    virtual void _v130();
    virtual void _v134();
    virtual f32* _v138();         // +0x138 (returns a float pointer)
    virtual void _v13C();
    virtual f32 _v140();          // +0x140 (movement progress)
    virtual void _v144();
    virtual void _v148();
    virtual void _v14C();
    virtual void _v150();
    virtual void _v154();
    virtual void _v158();
    virtual void _v15C();
    virtual int _v160(ml::CVec3* arg);  // +0x160 (position-offset query)
    virtual void _v164();
    virtual void _v168();
    virtual void _v16C();
    virtual void _v170();
    virtual void _v174();
    virtual void _v178();
    virtual void _v17C();
    virtual void _v180();
    virtual void _v184();
    virtual void _v188();
    virtual void _v18C();
    virtual void _v190();
    virtual void _v194();
    virtual void _v198();
    virtual void _v19C();
    virtual void _v1A0();
    virtual void _v1A4();
    virtual void _v1A8();
    virtual void _v1AC(void* a, const char* name);   // +0x1AC
    virtual void _v1B0();
    virtual void _v1B4();
    virtual void _v1B8();
    virtual void _v1BC();
    virtual void _v1C0();
    virtual void _v1C4();
    virtual void _v1C8();
    virtual void _v1CC();
    virtual void _v1D0();
    virtual void _v1D4();
    virtual void _v1D8();
    virtual void _v1DC(int arg);  // +0x1DC

    u32 field_04;                 // 0x04
    u8 _pad08[0x74 - 0x08];       // 0x08..0x73
    u8* field_74;                 // 0x74 opaque text-object handle
    u8 _pad78[0x8C - 0x78];       // 0x78..0x8B
    u16 field_8C;                 // 0x8C npc-kind halfword
    u8 _pad8E[0x98 - 0x8E];       // 0x8E..0x97
    CCtrlNpcSearch* field_98;     // 0x98 name-index search helper
    u8 _pad9C[0xC4 - 0x9C];       // 0x9C..0xC3
    u32 field_C4;                 // 0xC4 flag read by the NPC state helpers
    u8 _padC8[0x4EC - 0xC8];      // 0xC8..0x4EB
    u32 field_4EC;                // 0x4EC flags word (bit 0x02000000 tested)
    u8 _pad4F0[0x6C4 - 0x4F0];    // 0x4F0..0x6C3
    u32 field_6C4;                // 0x6C4 state word (0x21..0x2A = active states)
};

// CtrlNpc's retail ctor (__ct__CtrlNpc) is a plain free function that stores
// the vtable pointers manually (MWCC_REFERENCE §4193 pattern: short-form ctor
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
    CCtrlNpcChar* field_28;        // 0x28 character object (CfObjectMove family)
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
    u32 field_E0;                  // 0xE0 target array element[0].x
    u32 field_E4;                  // 0xE4 target array element[0].y
    u32 field_E8;                  // 0xE8 target array element[0].z
    u32 field_EC[3];               // 0xEC..0xF7 element[1] of the target array
    u8 _padF8[0x158 - 0xF8];       // 0xF8..0x157
    f32 field_158;                 // 0x158
    s16 field_15C;                 // 0x15C
    u8 _pad15E[0x160 - 0x15E];     // 0x15E..0x15F
    u32 field_160;                 // 0x160
    u32 field_164;                 // 0x164
    s16 field_168;                 // 0x168
    u8 _pad16A[0x16C - 0x16A];     // 0x16A..0x16B
    s16 field_16C;                 // 0x16C
    s16 field_16E;                 // 0x16E
    u16 field_170;                 // 0x170
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
namespace ml { struct CVec3; }
namespace cf { class CCtrlMoveNpc; }

// Talk/page controller update helpers (func_80093F28).
extern "C" f32 func_80496288(void* scene);
extern "C" int func_801413DC(unsigned int arg0, int arg1);
extern "C" u32 func_8009CF8C(u32 resourceId);
extern "C" void* getFP__FPCc(const char* name);
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* col, int index);
extern "C" void func_8003AA34();
extern "C" void func_8013D07C(void* self, char* text, int flag);
extern "C" void func_8013D1E8(void* self);
extern "C" u32 func_80086DA0__Q22cf13CfGameManagerFv();
// C++-linkage decl: MWCC mangles the call to func_800B708C__Fi (actor id ->
// action source object; the result has a position getter at vtable +0xac).
void* func_800B708C(int id);

// bdat tables / strings used by func_80093F28.
extern void* lbl_eu_80663E14;             // scene singleton handle
// The page-hint halfwords sprintf'd into the bdat file name.
extern u16 lbl_eu_80663E42;
extern u16 lbl_eu_80663E44;
extern const char lbl_eu_804FBB0C[];      // column-name / format string base
extern const char* lbl_eu_80527A48[];     // 6 per-kind column names
extern const char* lbl_eu_80527A60[];     // per-row column names (index / 3)
extern const char* lbl_eu_80527A80[];     // per-row column names (index / 3)

// Character-object factory called by the free-function ctor; defined in
// CfObjectModel.cpp (retail func_800BBC0C). extern "C" keeps the call reloc
// unmangled (plain C++ global decls get __F-suffix mangled at call sites).
extern "C" cf::CfObject* func_800BBC0C(cf::CfObject* self);
// CCtrlMoveNpc ctor (retail symbol keeps the flat name); parent is passed in
// r4 at the retail call site even though the ctor body ignores it.
extern "C" void __ct__cf_CtrlMoveNpc(cf::CCtrlMoveNpc* self, cf::CtrlNpc* parent);
// Talk/page helper: takes the character object's field_C4 word, returns a
// page id (unsigned compare at the call site).
extern "C" u32 func_8004C5EC(u32 arg);

extern "C" void func_8019F6E8(cf::CCtrlMoveNpc* self, const ml::CVec3* vec, f32 scale, f32 paramB);
extern "C" int func_8019F8E0(cf::CCtrlMoveNpc* self);
extern "C" void func_80094A9C(cf::CtrlNpc* self, const ml::CVec3* vec,
                              f32 scale, f32 paramB);
extern "C" int func_800A5038(const ml::CVec3* sub, const ml::CVec3* v, f32 f1, f32 f2);
// Same-TU forward decl: keeps the call reloc from func_800966E8 flat.
extern "C" int func_800964EC(cf::CtrlNpc* self);
// Battle-status add/remove helper (same C symbol as CBattleManager.cpp).
extern "C" void func_800BE12C(void* parent, int handle, int a, int b, int c);
// CfGameManager helpers: the retail symbols keep the Fv suffix but the real
// call sites pass arguments / read the return (same scheme as CTaskREvent.hpp),
// so C linkage emits the literal retail names at the call sites.
extern "C" void func_80086D98__Q22cf13CfGameManagerFv(u16* first, u16* second);
extern "C" void func_80086DB0__Q22cf13CfGameManagerFv();
// Play-frame helper returning a u32 (CfGameManager.cpp defines it as a plain
// extern "C" free function returning lbl_eu_80663D90).
extern "C" u32 func_80086DB4__Q22cf13CfGameManagerFv();
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
extern const f32 lbl_eu_806666B0;
extern const f32 lbl_eu_806666B4;
extern const f32 lbl_eu_806666B8;
extern const f32 lbl_eu_806666BC;
extern const f32 lbl_eu_806666E0;
extern const f32 lbl_eu_806666E8;
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
