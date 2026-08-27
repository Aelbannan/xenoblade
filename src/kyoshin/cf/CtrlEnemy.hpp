#pragma once

#include <types.h>
#include <nw4r/math/math_types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

namespace ml {
struct CVec3;
}

namespace cf {

class CCtrlMoveBase;

class CtrlBase {
public:
    virtual ~CtrlBase();
};

// Minimal CtrlAct declaration for this TU's dtor. The full CtrlAct.hpp header
// pulls in CtrlPc.hpp, whose two-arg extern "C" decl of func_80089B24
// conflicts with the one-arg delegate decls below (illegal overloading), so
// only the class is declared here.
class CtrlAct {
public:
    virtual ~CtrlAct();
};

// ---------------------------------------------------------------------------
// vtable-dispatch views. kyoshin builds with -RTTI on, so the virtual at
// declared index N sits at vtable byte offset (N+2)*4 (two leading RTTI
// slots). The classes are never instantiated, so no vtables are emitted and
// declared-only virtual bodies are never needed.
// ---------------------------------------------------------------------------

// Sub-object at battle-actor +0x04: vtable slot 0x30 (index 10) returns a
// word holder whose first word is the actor id (func_80174C98 out value).
class CtrlEnemyActor4 {
public:
    virtual void b008(); virtual void b00C(); virtual void b010();
    virtual void b014(); virtual void b018(); virtual void b01C();
    virtual void b020(); virtual void b024(); virtual void b028();
    virtual void b02C();
    virtual u32* b030(); // index 10 -> vtable offset 0x30
};

// Embedded move sub-object at battle-actor +0x3E9C (own vtable).
class CtrlEnemySubObj {
public:
    virtual void s008(u32 a);      // index 0  -> vtable 0x08
    virtual int s00C(u32 a);       // index 1  -> vtable 0x0c
    virtual void s010(u32 a);      // index 2  -> vtable 0x10
    virtual void s014();           // index 3
    virtual void s018();           // index 4
    virtual void s01C();           // index 5
    virtual void s020();           // index 6
    virtual void s024();           // index 7
    virtual void s028();           // index 8
    virtual void s02C();           // index 9
    virtual void s030();           // index 10
    virtual void s034();           // index 11
    virtual void s038();           // index 12
    virtual void s03C();           // index 13
    virtual void s040();           // index 14
    virtual void s044();           // index 15
    virtual void s048();           // index 16
    virtual void* s04C();          // index 17 -> vtable 0x4c
    virtual void s050(u32 a);      // index 18 -> vtable 0x50
    virtual void s054();           // index 19
    virtual void s058();           // index 20
    virtual void s05C();           // index 21
    virtual void s060();           // index 22
    virtual void s064();           // index 23
    virtual void s068();           // index 24
    virtual void s06C();           // index 25
    virtual void s070();           // index 26
    virtual void s074();           // index 27
    virtual void s078();           // index 28
    virtual void s07C();           // index 29
    virtual void s080();           // index 30
    virtual void s084();           // index 31
    virtual void s088();           // index 32
    virtual f32 s08C();            // index 33 -> vtable 0x8c
    virtual void s090();           // index 34
    virtual void s094();           // index 35
    virtual void s098();           // index 36
    virtual void s09C();           // index 37
    virtual void s0A0();           // index 38
    virtual void s0A4();           // index 39
    virtual void s0A8();           // index 40
    virtual void* s0AC();          // index 41 -> vtable 0xac
    virtual void s0B0();           // index 42
    virtual void s0B4();           // index 43
    virtual void s0B8();           // index 44
    virtual void s0BC();           // index 45
    virtual void s0C0();           // index 46
    virtual void s0C4(f32 f);      // index 47 -> vtable 0xc4
    virtual void s0C8();           // index 48
    virtual void s0CC();           // index 49
    virtual void s0D0();           // index 50
    virtual void s0D4();           // index 51
    virtual void s0D8();           // index 52
    virtual void s0DC();           // index 53
    virtual void s0E0();           // index 54
    virtual void s0E4();           // index 55
    virtual void s0E8();           // index 56
    virtual void s0EC();           // index 57
    virtual void s0F0();           // index 58
    virtual void s0F4();           // index 59
    virtual void s0F8();           // index 60
    virtual void s0FC();           // index 61
    virtual void s100();           // index 62
    virtual void s104();           // index 63
    virtual void s108();           // index 64
    virtual void s10C();           // index 65
    virtual void s110();           // index 66
    virtual void s114();           // index 67
    virtual void s118();           // index 68
    virtual void s11C();           // index 69
    virtual void s120();           // index 70
    virtual void s124();           // index 71
    virtual void s128();           // index 72
    virtual void* s12C(u32 a);     // index 73 -> vtable 0x12c
    virtual void s130();           // index 74
    virtual void s134();           // index 75
    virtual void s138();           // index 76
    virtual void s13C();           // index 77
    virtual f32 s140();            // index 78 -> vtable 0x140
};

// Position block returned by sub-object vtable slots: x/y/z floats.
struct CtrlEnemyPos {
    u8 _pad00[0x0C];               // 0x00-0x0B
    f32 x;                         // 0x0C
    u8 _pad10[0x1C - 0x10];        // 0x10-0x1B
    f32 y;                         // 0x1C
    u8 _pad20[0x2C - 0x20];        // 0x20-0x2B
    f32 z;                         // 0x2C
};

// 16-byte position block returned by sub-object vtable slot 0xAC (paired
// (x,y) then (z,w) - PS math reads the full 16 bytes).
struct CtrlEnemyPos4 {
    f32 e0, e1, e2, e3;            // +0, +4, +8, +C
};

// Battle-actor container rooted at CtrlEnemy::field_0x80 (func_8016FE34
// result). Declared with the full vtable so the dispatched slots below sit
// at their retail offsets.
class CtrlEnemyActor {
public:
    virtual void v008(); virtual void v00C(); virtual void v010();
    virtual void v014(); virtual void v018(); virtual void v01C();
    virtual void v020(); virtual void v024(); virtual void v028();
    virtual void v02C(); virtual void v030(); virtual void v034();
    virtual void v038(); virtual void v03C(); virtual void v040();
    virtual void v044(); virtual void v048(); virtual void v04C();
    virtual void v050(); virtual void v054(); virtual void v058();
    virtual void v05C(); virtual void v060(); virtual void v064();
    virtual void v068(); virtual void v06C(); virtual void v070();
    virtual void v074(); virtual void v078(); virtual void v07C();
    virtual void v080(); virtual void v084(); virtual void v088();
    virtual void v08C(); virtual void v090(); virtual void v094();
    virtual void v098(); virtual void v09C(); virtual void v0A0();
    virtual void v0A4(); virtual void v0A8(); virtual void v0AC();
    virtual void v0B0(); virtual void v0B4(); virtual void v0B8();
    virtual void v0BC(); virtual void v0C0(); virtual void v0C4();
    virtual void v0C8(); virtual void v0CC(); virtual void v0D0();
    virtual void v0D4(); virtual void v0D8(); virtual void v0DC();
    virtual int v0E0();            // index 54 -> vtable 0xe0 (battle id)
    virtual void v0E4(); virtual void v0E8(); virtual void v0EC();
    virtual void v0F0(); virtual void v0F4(); virtual void v0F8();
    virtual void v0FC(); virtual void v100(); virtual void v104();
    virtual int v108();            // index 64 -> vtable 0x108
    virtual void v10C(); virtual void v110(); virtual void v114();
    virtual void v118(); virtual void v11C(); virtual void v120();
    virtual void v124(); virtual void v128(); virtual void v12C();
    virtual void v130(); virtual void v134(); virtual void v138();
    virtual void v13C(); virtual void v140(); virtual void v144();
    virtual void v148(); virtual void v14C(); virtual void v150();
    virtual void v154(); virtual void v158(); virtual void v15C();
    virtual void v160(); virtual void v164(); virtual void v168();
    virtual void v16C(); virtual void v170(); virtual void v174();
    virtual void v178(); virtual void v17C(); virtual void v180();
    virtual void v184(); virtual void v188(); virtual void v18C();
    virtual void v190(); virtual void v194(); virtual void v198();
    virtual void v19C(); virtual void v1A0(); virtual void v1A4();
    virtual void v1A8(); virtual void v1AC(); virtual void v1B0();
    virtual void v1B4(); virtual void v1B8(); virtual void v1BC();
    virtual void v1C0(); virtual void v1C4(); virtual void v1C8();
    virtual void v1CC(); virtual void v1D0(); virtual void v1D4();
    virtual void v1D8(); virtual void v1DC(); virtual void v1E0();
    virtual void v1E4(); virtual void v1E8(); virtual void v1EC();
    virtual void v1F0(); virtual void v1F4(); virtual void v1F8();
    virtual void v1FC(); virtual void v200(); virtual void v204();
    virtual void v208(); virtual void v20C(); virtual void v210();
    virtual void v214(); virtual void v218(); virtual void v21C();
    virtual void v220(); virtual void v224(); virtual void v228();
    virtual void v22C(); virtual void v230(); virtual void v234();
    virtual void v238(); virtual void v23C(); virtual void v240();
    virtual void v244(); virtual void v248(); virtual void v24C();
    virtual void v250(); virtual void v254();
    virtual u32* v258();           // index 148 -> vtable 0x258 (state word)
    virtual void v25C();           // index 149
    virtual int v260();            // index 150 -> vtable 0x260
    virtual void v264();           // index 151
    virtual f32* v268();           // index 152 -> vtable 0x268
    virtual void v26C();           // index 153
    virtual f32* v270();           // index 154 -> vtable 0x270
    virtual void v274(); virtual void v278(); virtual void v27C();
    virtual void v280(); virtual void v284(); virtual void v288();
    virtual void v28C(); virtual void v290(); virtual void v294();
    virtual void v298(); virtual void v29C(); virtual void v2A0();
    virtual void v2A4(); virtual void v2A8(); virtual void v2AC();
    virtual void v2B0(); virtual void v2B4(); virtual void v2B8();
    virtual int v2BC();            // index 173 -> vtable 0x2bc
    virtual void v2C0();           // index 174
    virtual void v2C4(void* a, f32 b, f32 c, f32 d);  // index 175 -> 0x2c4
    virtual void v2C8(); virtual void v2CC(); virtual void v2D0();
    virtual void v2D4(); virtual void v2D8(); virtual void v2DC();
    virtual void v2E0(); virtual void v2E4();
    virtual void* v2E8();          // index 184 -> vtable 0x2e8
    virtual void v2EC(); virtual void v2F0(); virtual void v2F4();
    virtual void v2F8(); virtual void v2FC(); virtual void v300();
    virtual void v304(); virtual void v308(); virtual void v30C();
    virtual void v310(); virtual void v314(); virtual void v318();
    virtual void v31C(); virtual void v320(); virtual void v324();
    virtual void v328(); virtual void v32C(); virtual void v330();
    virtual void v334(); virtual void v338(); virtual void v33C();
    virtual void v340(); virtual void v344(); virtual void v348();
    virtual void v34C(); virtual void v350(); virtual void v354();
    virtual void v358(); virtual void v35C(); virtual void v360();
    virtual void v364(); virtual void v368(); virtual void v36C();
    virtual void v370(); virtual void v374(); virtual void v378();
    virtual void v37C(); virtual void v380(); virtual void v384();
    virtual void v388(); virtual void v38C(); virtual void v390();
    virtual void v394(); virtual void v398(); virtual void v39C();
    virtual void v3A0(); virtual void v3A4(); virtual void v3A8();
    virtual void v3AC(); virtual void v3B0(); virtual void v3B4();
    virtual void v3B8(); virtual void v3BC(); virtual void v3C0();
    virtual void v3C4(); virtual void v3C8(); virtual void v3CC();
    virtual void v3D0(); virtual void v3D4(); virtual void v3D8();
    virtual void v3DC(); virtual void v3E0(); virtual void v3E4();
    virtual void v3E8(); virtual void v3EC(); virtual void v3F0();
    virtual void v3F4(); virtual void v3F8(); virtual void v3FC();
    virtual void v400(); virtual void v404(); virtual void v408();
    virtual void v40C(); virtual void v410(); virtual void v414();
    virtual void v418(); virtual void v41C(); virtual void v420();
    virtual void v424(); virtual void v428(); virtual void v42C();
    virtual void v430(); virtual void v434(); virtual void v438();
    virtual void v43C(); virtual void v440(); virtual void v444();
    virtual void v448(); virtual void v44C(); virtual void v450();
    virtual void v454(); virtual void v458(); virtual void v45C();
    virtual void v460(); virtual void v464(); virtual void v468();
    virtual void v46C(); virtual void v470(); virtual void v474();
    virtual void v478(); virtual void v47C(); virtual void v480();
    virtual void v484(); virtual void v488(); virtual void v48C();
    virtual void v490(); virtual void v494(); virtual void v498();
    virtual void v49C(); virtual void v4A0(); virtual void v4A4();
    virtual void v4A8(); virtual void v4AC(); virtual void v4B0();
    virtual void v4B4(); virtual void v4B8(); virtual void v4BC();
    virtual void v4C0(); virtual void v4C4(); virtual void v4C8();
    virtual void v4CC(); virtual void v4D0(); virtual void v4D4();
    virtual void v4D8(); virtual void v4DC(); virtual void v4E0();
    virtual void v4E4(); virtual void v4E8(); virtual void v4EC();
    virtual void v4F0(); virtual void v4F4(); virtual void v4F8();
    virtual void v4FC(); virtual void v500(); virtual void v504();
    virtual void v508(); virtual void v50C(); virtual void v510();
    virtual void v514(); virtual void v518(); virtual void v51C();
    virtual void v520(); virtual void v524(); virtual void v528();
    virtual void v52C(); virtual void v530(); virtual void v534();
    virtual void v538(); virtual void v53C(); virtual void v540();
    virtual void v544(); virtual void v548(); virtual void v54C();
    virtual void v550(); virtual void v554(); virtual void v558();
    virtual void v55C(); virtual void v560(); virtual void v564();
    virtual void v568(); virtual void v56C(); virtual void v570();
    virtual void v574(); virtual void v578(); virtual void v57C();
    virtual void v580(); virtual void v584(); virtual void v588();
    virtual void v58C(); virtual void v590(); virtual void v594();
    virtual void v598(); virtual void v59C(); virtual void v5A0();
    virtual void v5A4(); virtual void v5A8(); virtual void v5AC();
    virtual void v5B0();
    virtual f32 v5B4();            // index 363 -> vtable 0x5b4

    CtrlEnemyActor4* field_04;        // 0x04 (id-word holder)
    u8 _pad08[0x15F0 - 0x08];         // 0x08-0x15EF
    u32 field_15F0;                   // 0x15F0
    u8 _pad15F4[0x3374 - 0x15F4];     // 0x15F4-0x3373
    u32 field_3374;                   // 0x3374 (flags)
    u8 _pad3378[0x3E9C - 0x3378];     // 0x3378-0x3E9B
    CtrlEnemySubObj mSub;             // 0x3E9C (embedded move sub-object)
    u8 _pad3EA0[0x3F60 - 0x3EA0];     // 0x3EA0-0x3F5F
    void* field_3F60;                 // 0x3F60
    u8 _pad3F64[0x45CA - 0x3F64];     // 0x3F64-0x45C9
    u16 field_45CA;                   // 0x45CA
};

// Minimal CBattleManager view for this TU: getInstance + vtable slot 0x28
// (battle-state probe, one u32 mask).
class CtrlEnemyBM {
public:
    virtual void m008(); virtual void m00C(); virtual void m010();
    virtual void m014(); virtual void m018(); virtual void m01C();
    virtual void m020(); virtual void m024();
    virtual int m028(u32 mask);      // index 8 -> vtable 0x28
};

// 8-byte enum-list holder (func_80043D90 ctor / __dt__80043E88 dtor); the
// list returned by func_80043F18 has the element count at +0x620.
struct CtrlEnemyEnumHolder {
    void* list;   // 0x0
    u32 handle;   // 0x4
};

struct CtrlEnemyEnumList {
    u8 _pad00[0x620];                // 0x00-0x61F
    u32 field_620;                   // 0x620 (count)
};

// Global gimmick settings view (getUnk80664658 result): flag word at +0x214.
struct CtrlEnemyGimmickGlobal {
    u8 _pad00[0x210];                // 0x00-0x20F
    u32 field_210;                   // 0x210
    u32 field_214;                   // 0x214 (flags)
};

// Battle-actor +0x3F60 target: u16 flag word at +0x530 (bit 0 probed).
struct CtrlEnemySubFlag {
    u8 _pad00[0x530];                // 0x00-0x52F
    u16 field_530;                   // 0x530
};

// Arts-info object returned by func_80149154: word at +0x10.
struct CtrlEnemyArtsInfo {
    u8 _pad00[0x10];                 // 0x00-0x0F
    u32 field_10;                    // 0x10
};

class __declspec(novtable) CtrlEnemy {
public:
    virtual ~CtrlEnemy();            // index 0  -> vtable 0x08
    virtual void v008() = 0;         // index 1
    virtual void v00C() = 0;         // index 2
    virtual void v010() = 0;         // index 3
    virtual void v014() = 0;         // index 4
    virtual void v018() = 0;         // index 5
    virtual void v01C() = 0;         // index 6
    virtual void v020() = 0;         // index 7
    virtual void v024() = 0;         // index 8
    virtual void v028() = 0;         // index 9
    virtual void v02C() = 0;         // index 10
    virtual void v030() = 0;         // index 11
    virtual void v034() = 0;         // index 12
    virtual void v038() = 0;         // index 13
    virtual void v03C() = 0;         // index 14
    virtual void v040() = 0;         // index 15
    virtual void v044() = 0;         // index 16
    virtual void v048() = 0;         // index 17
    virtual void v04C() = 0;         // index 18
    virtual void v050() = 0;         // index 19
    virtual int v058() = 0;          // index 20 -> vtable 0x58
    virtual void v05C() = 0;         // index 21
    virtual void v060() = 0;         // index 22 -> vtable 0x60
    virtual void v064() = 0;         // index 23
    virtual void v068() = 0;         // index 24
    virtual void v06C() = 0;         // index 25
    virtual void v070() = 0;         // index 26
    virtual void v074() = 0;         // index 27
    virtual void v078() = 0;         // index 28
    virtual void v07C() = 0;         // index 29
    virtual void v080() = 0;         // index 30
    virtual void v084() = 0;         // index 31
    virtual int v088() = 0;          // index 32 -> vtable 0x88
    virtual void v08C() = 0;         // index 33 -> vtable 0x8c

    // delegate helpers
    void delegateTo80089B24();
    void delegateTo80089E88();
    void delegateTo80089F68();

    // return-0 stubs
    int ret0_80088870();
    int ret0_80088878();
    int ret0_80088880();
    int ret0_80088888();
    int ret0_80088890();
    int ret0_80088898();
    int ret0_800888A0();
    int ret0_800888A8();
    int ret0_800888B0();
    int ret0_800888B8();
    int ret0_800888C0();
    int ret0_800888C8();
    int ret0_800888D0();
    int ret0_800888D8();
    int ret0_800888E0();
    int ret0_800888E8();

    // return-1 stubs
    int ret1_800888F4();
    int ret1_800888FC();

    // +0x00: implicit vptr (novtable: stored manually as lbl_eu_80527738)
    u32 field_0x04;                  // 0x04 (flags; ORed by the tick paths)
    u8 _pad08[0x0C - 0x08];          // 0x08-0x0B
    f32 field_0x0C;                  // 0x0C
    u8 _pad10[0x14 - 0x10];          // 0x10-0x13
    f32 field_0x14;                  // 0x14 (cooldown/counter, reset to 0.0)
    u8 _pad18[0x80 - 0x18];          // 0x18-0x7F
    CtrlEnemyActor* field_0x80;      // 0x80 (func_8016FE34 result, set by ctor)
    u8 mSub84[0x1E0 - 0x84];         // 0x84 move-controller sub-object
    void* field_0x1E0;               // 0x1E0
    u8 _pad1E4[0x200 - 0x1E4];       // 0x1E4-0x1FF
    u32 field_0x200;                 // 0x200 (flags)
};

// Minimal cf::CfObjectMove view for this TU's CfObject_UnkVirtualFunc60
// getter. The full CfObjectMove.hpp header declares that member with a void
// return, but retail emits `lfs f1, 0x6ec(r3)` - f1 is the FP return
// register, so the source must return float, and MWCC rejects a definition
// that widens the declared return type. The view is only used here, so it
// lives in this unit header instead (same convention as the CtrlActView
// family in CtrlAct.hpp).
class CfObjectMove {
public:
    float CfObject_UnkVirtualFunc60();

    // +0x00: vptr (not modelled - no virtuals in this view)
    u8 _00[0x6EC];              // 0x00
    float mField6EC;            // 0x6EC
};

} // namespace cf

// cf::CtrlEnemy retail vtable (.data).
extern u8 lbl_eu_80527738[];

// Retail data symbols (global scope: MWCC emits these unmangled).
extern f32 lbl_eu_80666570;    // 0.0f
extern f32 lbl_eu_80666574;    // 10.0f
extern f32 lbl_eu_80666578;    // 4.0f
extern f32 lbl_eu_8066657C;    // 1.0f
extern f32 lbl_eu_80666580;    // 0.15f
extern f32 lbl_eu_80666584;    // 100.0f
extern f64 lbl_eu_80666588;    // 2^52 + 2^31 (signed int -> double magic)

// C++-linkage imports (mangle to the retail __Fi forms).
void* findObjectById(int id);       // findObjectById__Fi
bool  isGlobalCamFlagSet(int mask);     // isGlobalCamFlagSet__Fi

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" {
void func_80089B24(void* self);  // one-arg delegate form
void func_80089E88(void* self);  // one-arg delegate form
void func_80089F68(void* self);  // one-arg delegate form
void __ct__800D10DC(void* self, void* parent);  // CtrlEnemy base ctor
void __ct__8008A104(void* self, void* parent);  // +0x84 sub-object ctor
void* func_8016FE34(void* source);              // voice/battle-list resolve
void func_8008B930(void* self);                 // +0x84 sub-object dtor (blr)
void func_80089398(cf::CCtrlMoveBase* self, ml::CVec3* dst,
                   const ml::CVec3* src, int flag);
int func_800890A8(cf::CCtrlMoveBase* self, ml::CVec3* out, u8* outFlag,
                  const ml::CVec3* src, int flag);
// getInstance__Q22cf14CBattleManagerFv is declared by
// kyoshin/cf/CBattleManagerApi.hpp (via cf/CtrlPc.hpp's include chain).
void* getInstance__Q22cf13CfGameManagerFv(void);
void* getPlayer__Q22cf13CfGameManagerFi(int idx);
int isSceneReadyForInput__Q22cf13CfGameManagerFv(void);
void func_80043D90(cf::CtrlEnemyEnumHolder* holder);
cf::CtrlEnemyEnumList* func_80043F18(cf::CtrlEnemyEnumHolder* holder);
void func_800F4A98(cf::CtrlEnemyEnumList* list, u32 a, u32 b);
void* func_800F6E98(cf::CtrlEnemyEnumList* list, int idx);
void __dt__80043E88(cf::CtrlEnemyEnumHolder* holder, int flags);
void* __ct__800FB044(cf::CtrlEnemyEnumList* list, f32 f, void* obj, int a);
void* __ct__800FAE3C(cf::CtrlEnemyEnumList* list, void* vec, int a);
void* __ct__800FD0B4(cf::CtrlEnemyEnumList* list, int a);
void* __ct__800FC4FC(cf::CtrlEnemyEnumList* list, f32 f, int a);
void func_800FB270(cf::CtrlEnemyEnumList* list, void* obj, f32 a, f32 b,
                   f32 c, int d);
int func_80148778(void* obj, int id);
void* func_80149154(void* obj, u32 id);   // aligned with CAIAction.hpp (u32 param, per CBattleState.cpp def)
void func_800D9CA0(void* mgr, void* target);
void func_8008B580(void* sub);
void func_8008A23C(void* sub);
void func_8008A2C8(void* sub);
int func_8008B934(void* sub, void* obj);
int func_8008B974(void* sub);
void* getUnk80664658(void);
}