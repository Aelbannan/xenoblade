#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectModel.hpp"

namespace cf {
class CtrlNpc;
class CActParamAnimGame;
struct CfObjectMoveTargetC4;
class CfObjectMove;
}

namespace ml {
struct CVec3;
}

class CScn;  // monolib scene (CfGameManager owns the shared lbl_eu_80663E14 global)

// Base +0x9C implementation (defined in CfObjectModel.cpp as a forced-name
// free function: the retail symbol is Fv but the body reads r4 as a position
// vector). Plain global so the call-site reloc references the retail name
// (CfObjectMove::CfObject_UnkVirtualFunc19 forwards its hidden r4 through).
void CfObject_UnkVirtualFunc19__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, const ml::CVec3* vec);

// Cross-TU dispatch targets for the CfObjectMove mTarget6C0 wrappers (defined
// in kyoshin/cf/CtrlNpc.cpp, retail func_800948F8 family). extern "C" keeps
// the call relocs unmangled - plain C++ global decls get __F-suffix mangled
// at call sites (same scheme as CtrlNpc.hpp's func_800BE12C declarations).
extern "C" void func_800948F8(cf::CtrlNpc* self);
extern "C" void func_80094CE8(cf::CtrlNpc* self);
extern "C" void func_80094DF4(cf::CtrlNpc* self);
extern "C" void func_80094E44(cf::CtrlNpc* self);
extern "C" void func_80094EDC(cf::CtrlNpc* self);
extern "C" void func_80094FC8(cf::CtrlNpc* self);
// NPC movement-target action setter (CtrlNpc.cpp, retail func_8009377C):
// CfObject_UnkVirtualFunc49 tail-dispatches the incoming action id.
extern "C" void func_8009377C(cf::CtrlNpc* self, u32 param);
// Action-advance query on the NPC movement target (CtrlNpc.cpp, retail
// func_80094D1C / US 0x800956F4): whether the NPC may advance.
extern "C" int func_80094D1C(const cf::CtrlNpc* self);
// Region-library helpers (0x804B0xxx): attach/detach a region object to the
// shared manager global below. CfGimmickLock.hpp also declares these but is
// not included here (its func_800817BC return type conflicts with the
// CfGameManager unity header). func_804B0924 is the +0x60C region
// sub-object ctor called from the CfObjectMove ctor.
extern "C" void func_804B0924(void* region);
extern "C" void func_804B4BDC(void* manager, void* region);
extern "C" void func_804B4C7C(void* manager, void* region);
extern void* lbl_eu_80665958;   // region-library manager (sbss)
// CfObjectMove vtable (data TU; the destructor stores it explicitly, same
// scheme as CfGimmickLock.hpp's lbl_eu_80535900).
extern u8 lbl_eu_80529690[];
// Destructor-chain imports (defined in the region library, in
// kyoshin/cf/CActParamAnimGame.cpp and in CfObjectModel.cpp). extern "C"
// keeps the bl relocs at the unmangled retail names (same scheme as
// CfGimmickLock.hpp's __dt__Q22cf9CfGimmickFv declaration).
extern "C" void __dt__804B095C(void* self, int flag);
extern "C" void __ct__Q22cf17CActParamAnimGameFv(cf::CActParamAnimGame* self);
extern "C" void __dt__Q22cf17CActParamAnimGameFv(void* self, int flag);
extern "C" void __dt__Q22cf13CfObjectModelFv(void* self, int flag);
// CfGameManager resource loader (defined in CfGameManagerUnityHelpers.hpp as
// extern "C" with Unk8187CData* return; declared void* here to avoid pulling
// that unity-TU header into this TU).
extern "C" void* func_80081900__Q22cf13CfGameManagerFv(u32 first, u32 value, u32 resource);

// Retail data labels referenced by this unit.
extern const char lbl_eu_804FC550[];   // bdat column-name table (func_800BED6C / func_800BEE08)
extern u32 lbl_eu_80663E28;   // global flag word (bit 26) gating the func_800BC458 flag clear
// Global flag word (bits 25/9) gating func_800BC8D8's flag block. Non-volatile
// extern so the type agrees with CSystemWindow.hpp and the .sbss definition in
// CUICfManager.cpp; reads that must stay separate use explicit volatile casts
// in CfObjectMove.cpp (retail loads it twice there - see the double-lwz note).
extern u32 lbl_eu_80663E24;
extern u16 lbl_eu_80663E42;   // mode words checked by CfObject_UnkVirtualFunc26 (== 4 / == 1)
extern u16 lbl_eu_80663E44;
extern CScn* lbl_eu_80663E14;  // shared scene pointer passed to the func_80496288 time query
// Shared speed constant (retail unmangled name; the .cpp also defines a
// namespace-cf copy used by the resetMoveSpeed helper). Declared const so
// MWCC treats the SDA load as read-only and hoists/schedules it at retail's
// position (MWCC_CASES pair-copy / CArtsInfo store-order fix).
extern const float lbl_eu_80666A88;
// Shared camera-time constant (sdata2): CfObject_UnkVirtualFunc70 calls the
// EU helper with (lbl_eu_80666A94 - value). Const so MWCC keeps the SDA
// load at retail's position.
extern const float lbl_eu_80666A94;
// Movement-speed constants (sdata2, retail unmangled names) used by
// func_800BC68C (lbl_eu_80666AA4/AA8/AAC/AB0 + ml::epsilon) and
// CfObject_UnkVirtualFunc46 (lbl_eu_80666AD0 scale).
extern const float lbl_eu_80666AA4;
extern const float lbl_eu_80666AA8;
extern const float lbl_eu_80666AAC;
extern const float lbl_eu_80666AB0;
extern const float lbl_eu_80666AD0;
extern const float lbl_eu_80666AB4;   // CfObject_UnkVirtualFunc5 movement-rate constant
// CfObject_UnkVirtualFunc26 constants: lbl_eu_80666AD4 is the amount clamp
// threshold, lbl_eu_80666AD8 the sum-vector Y offset.
extern const float lbl_eu_80666AD4;
extern const float lbl_eu_80666AD8;
extern const float lbl_eu_8066A208;   // ml::epsilon (sdata2)
// Movement constants for func_800BC4CC: lbl_eu_80666A98 is the squared-
// distance threshold, lbl_eu_80666A9C the fallback distance, lbl_eu_80666AA0
// the addend applied when the camera/player distance check fails.
extern const float lbl_eu_80666A98;
extern const float lbl_eu_80666A9C;
extern const float lbl_eu_80666AA0;
// CfObject_UnkVirtualFunc4 movement-threshold constant (compared against the
// +0x98 sub-object's vtable +0x4C result).
extern const float lbl_eu_80666A8C;
// func_800BC9EC movement constants: lbl_eu_80666AB8/ABC are the scale/angle
// factors for the Sin/Cos sweep, lbl_eu_80666AC0 the fallback factor, and
// lbl_eu_80666AC4/AF20 the func_804BD94C tail factors.
extern const float lbl_eu_80666AB8;
extern const float lbl_eu_80666ABC;
extern const float lbl_eu_80666AC0;
extern const float lbl_eu_80666AC4;
extern const float lbl_eu_8066AF20;
// func_800BCFA0 approach-rate constants (selected by mFlags68 bit 1).
extern const float lbl_eu_80666AC8;
extern const float lbl_eu_80666ACC;

// Scene-time query (CfGameManager.cpp, retail unmangled name): returns the
// current time value from the shared scene object. extern "C" keeps the
// call-site reloc at the unmangled retail name (docs/MWCC_CASES.md §2).
extern "C" f32 func_80496288(void* scene);
// Minimal bdat imports (CfBdat.hpp cannot be included here: its
// getBdatStringColumnValue declaration conflicts with harness_catalog.hpp's).
// The static-member forms keep the retail mangled reloc names.
namespace cf {
class CfBdat {
public:
    static void* func_801422A8(u32 param1);
    static const char* func_801424A8(u16 index);
};
}  // namespace cf
// Retail sbss bdat globals used by func_800BCFA0.
extern void* lbl_eu_806640A8;   // spFldMapListFileData
extern u32 lbl_eu_80664184;     // cached fld-map row index
// One-arg call form of the CActParamAnim translation helper (defined 2-arg
// in kyoshin/action/CActParamAnim.cpp). Retail CfObject_UnkVirtualFunc19
// calls it with only r3 set. C linkage keeps the retail unmangled name.
extern "C" void func_8004B354(void* self);
// Second heap handle query (retail unmangled name, same family as
// func_80061FE8): used by CfObject_UnkVirtualFunc47's CtrlEnemy/CtrlNpc
// allocations.
extern "C" u32 func_80061FFC();
// Base +0xB4 forced-name implementation (defined in CfObjectModel.cpp as the
// CfObject member; the retail symbol is Fv but the body reads r4 as a
// position vector). extern "C" keeps the call-site reloc at the unmangled
// retail name (CfObject_UnkVirtualFunc26 forwards its hidden r4 through).
extern "C" void CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv(cf::CfObject* self, const ml::CVec3* vec);
// Base +0x178 forced-name implementation (defined in CfObjectModel.cpp as the
// CfObjectModel member; the retail symbol is Fv). CfObjectModel_UnkVirtualFunc1
// (CfObjectMove) calls it before tearing down its own state.
extern "C" void CfObjectModel_UnkVirtualFunc1__Q22cf13CfObjectModelFv(cf::CfObjectModel* self);
// Region-library helpers (retail unmangled names, defined outside this unit):
// func_804B0A7C is the +0x60C region sub-object release called from
// CfObjectModel_UnkVirtualFunc1 (CfObjectMove); func_800BB618 forwards a
// visibility flag to the +0x98 sub-object (CfObjectModel.cpp).
extern "C" void func_804B0A7C(u8* region);
extern "C" void func_800BB618(cf::CfObjectModel* self, u32 flag);
// Movement-position update (defined in this TU below; forward-declared here
// because CfObject_UnkVirtualFunc5 calls it). extern "C" keeps the call-site
// reloc at the unmangled retail name.
extern "C" void func_800BC8D8(cf::CfObjectMove* self);
// Movement-speed helper (defined in this TU below) and the status query used
// by func_800BC8D8. extern "C" keeps the call-site relocs unmangled.
extern "C" void func_800BC68C(cf::CfObjectMove* self, u32 arg);
extern "C" int func_800BC4CC(cf::CfObjectMove* self);
// Same-TU movement/position helpers (defined in CfObjectMove.cpp below;
// CfObject_UnkVirtualFunc4 dispatches func_800BCD04).
extern "C" void func_800BCD04(cf::CfObjectMove* self);
extern "C" void func_800BC9EC(cf::CfObjectMove* self);
extern "C" void func_800BCFA0(cf::CfObjectMove* self);
// Scene/manager queries used by func_800BC4CC / func_800BC9EC / func_800BCFA0.
extern "C" int CfRes_getD80Flag();
extern "C" void* func_80496264(void* obj, int index);
extern "C" int func_8007560C();
extern "C" bool func_800829B8__Q22cf13CfGameManagerFv();
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);
class UnkClass_800821F8;
extern "C" UnkClass_800821F8* func_800821F8__Q22cf13CfGameManagerFv();
// Region-library helper used by func_800BC4CC (region, target, flags, dist).
extern "C" int func_804B192C(void* region, void* target, int arg2, int arg3, f32 dist);
// +0x98 sub-object flag query (CfObjectModel.cpp, retail unmangled name) used
// by CfObject_UnkVirtualFunc4.
extern "C" u32 func_800BB934(cf::CfObjectModel* self);
// CActParamAnim helpers (defined in kyoshin/cf/CActParamAnimGame.cpp,
// retail unmangled names). func_8004CF00 releases a +0xC8 target;
// func_8004B6A4 / func_8004B624 attach model lists to the +0xC4 target;
// func_80051B84 family drives the +0xC4 target's page state (func_800BCFA0).
extern "C" void func_8004CF00(void* self);
extern "C" void func_8004B6A4(void* self, void* a, void* b);
extern "C" void func_80051B84(void* self);
extern "C" void func_80051BA0(void* self);
extern "C" void func_80051BDC(void* self);
extern "C" void func_80051BC4(void* self);
// Region-library helpers used by func_800BC9EC / func_800BCFA0.
extern "C" void func_804B0A74(void* region);
extern "C" void func_804B1164(void* region, void* out, const void* vec, void* out2);
// Collision/position stepper (defined in kyoshin/cf/CActParamAnimGame.cpp,
// retail unmangled name): 6 int-ish args + 5 float args.
extern "C" u32 func_804BD94C(void* a, void* b, u32 c, u32 d, u32 e, u32 f,
                              f32 g, f32 h, f32 i, f32 j, f32 k);
// Model sub-object flag setter (defined in realtimeevt/CREvtModel.cpp) used
// by func_800BCFA0's bdat-flag tail.
extern "C" void func_804C0254(void* self, int flag);
// Region position sync + model sub-object helpers used by func_800BCD04 /
// func_800BC9EC (func_804B0B54) and func_800BCFA0 (func_80484E5C).
extern "C" void func_804B0B54(void* region, const float* vec);
extern "C" void func_80484E5C(void* self, f32 value);
// nw4r math sweep functions used by func_800BC9EC (retail names keep the
// call relocs unmangled).
extern "C" f32 SinFIdx__Q24nw4r4mathFf(f32);
extern "C" f32 CosFIdx__Q24nw4r4mathFf(f32);
// Enemy-state query (mangled global C++ symbol func_800AD860__FPv; defined
// outside this repo's src tree) used by func_800BC9EC.
void* func_800AD860(void* obj);
// vtable +0x14C flag-word bit queries (defined in CfObjectModel.cpp as plain
// global C++ functions) used by func_800BCFA0's dispatch tail. extern "C":
// the retail symbols are the unmangled names.
extern "C" u32 func_800BAD98(cf::CfObject* obj);
extern "C" u32 func_800BADF8(cf::CfObject* obj);
extern "C" u32 func_800BADC8(cf::CfObject* obj);
extern "C" u32 func_800BAE28(cf::CfObject* obj);
// CtrlEnemy/CtrlNpc/CtrlPc/CtrlRemote/CtrlPad retail constructors (plain
// free functions in their own TUs; the symbol map links the unmangled names).
// Declared here because the owning headers are not included in this TU.
extern "C" void __ct__cf_CtrlEnemy(void* self, void* parent);
extern "C" void __ct__CtrlNpc(void* self, void* parent);
extern "C" void __ct__cf_CtrlPc(void* self, void* parent, u32 arg);
extern "C" void __ct__CtrlRemote(void* self, void* parent, int arg);
extern "C" void __ct__cf_CtrlPad(void* self, void* parent, int arg);

// bdat column lookup helper (defined in kyoshin/cf/CfBdat.cpp, retail
// unmangled name). Declared here because the canonical CModelDispEquip.hpp
// declaration conflicts with CfObjectModel.hpp's func_80495E60 (C-linkage
// overload) and CfBdat.hpp (read-only) does not declare it.
extern "C" u32 func_8014235C(u32 param1, const char* column, u32 param3);

// Retail unmangled import used by the CfObjectMove wrappers: voice-request
// helper (defined in kyoshin/cf/voice/CCharVoice.cpp). The CCharVoice member
// declaration takes three args; retail func_800BE898 forwards only `this`
// (addi r3,+0x28; b), so a one-arg C-ABI form is declared here to
// reproduce the call site exactly.
extern "C" bool func_802A109C(void* self);
// One-arg call form of the CActParamAnim translation helper (defined 2-arg
// in kyoshin/action/CActParamAnim.cpp). Retail CfObject_UnkVirtualFunc22
// calls it with only r3 set - r4 is left as the incoming vec, which the
// base call below consumes (same scheme as the CfObjectModel func_8004B354
// use). C linkage keeps the call-site reloc at the unmangled retail name.
extern "C" bool func_8004B40C(cf::CfObjectMoveTargetC4* self);
// One-arg call form of the camera-event flush helper (defined in
// kyoshin/cf/CfCamEvent.cpp, which declares it no-arg). Retail
// CfObject_UnkVirtualFunc22 calls it with the +0x98 sub-object in r3, so the
// arg-carrying form reproduces the null-check load landing in r3.
extern "C" void func_804876DC(cf::CfObjectModelSub98* sub);
// EU-named camera-time helper (retail CfObject_UnkVirtualFunc70 calls it
// with the +0x38 sub-object's vtable +0xE4 query result in r3 and a float
// in f1); the US build keeps the EU name for this reloc.
extern "C" void func_eu_8015D258(void* self, float value);
// No-arg call form of the CCharEffect slot-release helper (defined in
// kyoshin/cf/CCharEffect.cpp with a self param). Retail
// CfObjectMove_UnkVirtualFunc21 calls it with the vtable query result
// leftover in r3 - the call site passes no argument.
extern "C" void func_8015C100();
// Two-arg call forms of the CCharEffect slot helpers (defined in
// kyoshin/cf/CCharEffect.cpp with (CCharEffect*, u32)). Retail
// CfObjectMove_UnkVirtualFunc17/18/20 pass the +0x38 sub-object's vtable
// +0xE4 query result leftover in r3 and the incoming r4 as the index; C
// linkage keeps the call relocs at the unmangled retail names (same scheme
// as the func_8015C100 no-arg form).
extern "C" void func_8015C074(void* self, u32 idx);
extern "C" void func_8015C0B0(void* self, u32 idx);
extern "C" void func_8015BD24(void* self, u32 param);
// Two-arg call forms of the CCharEffect slot helpers (defined in
// kyoshin/cf/CCharEffect.cpp). func_8015C214 removes a pointer from the
// effect slot lists; func_8015C294 returns whether a slot is occupied.
// Retail CfObjectModel_UnkVirtualFunc18 / CfObjectMove_UnkVirtualFunc19
// forward the +0x38 sub-object's vtable +0xE4 query result leftover in r3
// plus the incoming r4 (same scheme as the func_8015C074 family above).
extern "C" void func_8015C214(void* self, void* p);
extern "C" bool func_8015C294(unsigned int* param1, int param2);
// CCharEffect slot-detach helper (defined in kyoshin/cf/CCharEffect.cpp
// with (CCharEffect*, CCharEffectVTableIf*); CCharEffect.hpp is not
// included here, so the canonical typed form is not visible). C linkage
// keeps the call-site reloc at the plain retail name (same scheme as the
// func_8015C074 family above).
extern "C" void func_8015C8F4(void* self, void* manager);
// Six-arg call form of the CCharEffect slot-register helper (defined in
// kyoshin/cf/CCharEffect.cpp with (CCharEffect*, u32, CCharEffectSlot*,
// u16, u16, u8); the call site only forwards register values, so the arg
// types below reproduce the retail call exactly - the s16 fourth arg emits
// the retail extsh at the call site).
extern "C" void func_8015BFCC(void* self, u32 a, u32 b, s16 c, u32 d, u32 e);
// CActParamAnim helpers (defined in kyoshin/action/CActParamAnim.cpp,
// retail unmangled names). func_8004B52C is the float-taking translation
// helper. func_8004B9D4 is called at the retail func_800BE12C site with
// only r3-r6 set (the incoming params); the canonical CModelDispEquip.hpp
// 5-arg declaration conflicts with CfObjectModel.hpp, so a 4-arg form is
// declared here to reproduce the call site exactly (same scheme as the
// func_8004B40C one-arg form above).
extern "C" bool func_8004B52C(void* self, float value);
extern "C" void func_8004B9D4(void* self, u32 a, u32 b, u32 c);
// CActParamAnim-view helpers used by func_800BD644 (defined in
// kyoshin/action/CActParamAnim.cpp / CActParamAnimGame.cpp, retail
// unmangled names; the canonical CModelDispEquip.hpp declarations take
// CActParamAnimView* which is not visible here - CfObjectMove.hpp's
// func_80495E60 C-linkage form conflicts with that header).
// func_8005A594 advances the view's animation model; func_8004B624
// attaches a model list + state with a vtable-query parameter;
// func_8004C5EC returns the C4 target's page id.
extern "C" void func_8005A594(void* self);
extern "C" void func_8004B624(void* self, void* object, void* state, u32 param);
extern "C" u32 func_8004C5EC(void* self);
// Heap query + MemManager allocate used by func_800BD644's +0xC8 target
// construction (defined in CfRes.cpp / monolib; same declarations as
// CfCamEvent.hpp, which is not included here).
extern "C" u32 func_80061FE8();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);
// +0xC8 target ctor (retail 0x8005A3FC, defined in
// kyoshin/cf/CActParamAnimGame.cpp): func_800BD644 constructs the target
// with the owning CfObjectMove as the parent.
extern "C" void __ct__8005A3FC(void* self, void* parent);
// Model sub-object helper used by func_800BD644's tail (defined in the
// monolib scene library, same family as func_804838DC in
// CfObjectModel.hpp).
extern "C" void func_80482918(cf::CfObjectModelSub98* model, int flag);
// CModelDisp release helper (defined in kyoshin/makecrystal/
// CModelDispMakeCrystal.cpp, retail unmangled name). Called by
// CfObjectMove_UnkVirtualFunc2 for each +0xC8/+0xCC target while the
// +0x6D8 model list is present.
extern "C" void func_8004B6BC(void* self, void* obj);
// CActParamAnim field setter (defined in kyoshin/action/CActParamAnim.cpp,
// retail unmangled name; the retail body is stw r4,0x8(r3); blr). C
// linkage keeps the call-site reloc at the plain retail name.
extern "C" void func_8004B730(void* self, void* value);// Battle-status setter (retail unmangled C symbol; canonical 5-arg form
// matching CfObjectActor.hpp / CtrlNpc.hpp / all call sites; the retail
// body only consumes r3-r6). Declared here so the CfObjectMove.cpp
// definition inherits C linkage without a local extern "C".
extern "C" void func_800BE12C(u8* obj, int a, int b, int c, int d);
// Base +0x9C / +0x64 forced-name implementations (defined in
// CfObjectModel.cpp as plain globals). extern "C" keeps the call-site
// relocs at the unmangled retail names (a plain C++ declaration makes MWCC
// append a __FP<params> mangling at the call site).
extern "C" void CfObject_UnkVirtualFunc22__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, const ml::CVec3* vec);
extern "C" void CfObject_UnkVirtualFunc66__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, int flag);
// Base +0x68 forced-name implementation (defined in CfObjectModel.cpp as a
// plain global). extern "C" keeps the call-site reloc at the unmangled
// retail name (a plain C++ declaration inside namespace cf makes MWCC
// append a __2cfF<params> mangling at the call site).
extern "C" void CfObject_UnkVirtualFunc29__Q22cf13CfObjectModelFv(cf::CfObjectModel* self);
// Base +0xA0 forced-name implementation (defined in CfObjectModel.cpp as a
// plain global taking the hidden float arg).
extern "C" void CfObject_UnkVirtualFunc70__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float value);

namespace cf {
    //min size: 0x71c
    // The retail vtable lives in a data TU (lbl_eu_80529690); the
    // compiler-generated __vt__Q22cf12CfObjectMove reloc name would drift,
    // so novtable suppresses the implicit vptr store and the ctor/dtor
    // assign the retail label explicitly (same scheme as COccCulling /
    // CfGimmickLock).
    class __declspec(novtable) CfObjectMove : public CfObjectModel {
    public:
        CfObjectMove();
        //vtable 1 (CfObject)
        virtual ~CfObjectMove();
        //vtable 1 (CfObjectMove)
        virtual void CfObjectMove_UnkVirtualFunc1();  //0x1C8
        virtual void CfObjectMove_UnkVirtualFunc2();  //0x1CC
        virtual void CfObjectMove_UnkVirtualFunc3();  //0x1D0
        virtual void CfObjectMove_UnkVirtualFunc4();  //0x1D4
        virtual void CfObjectMove_UnkVirtualFunc5();  //0x1D8
        virtual void CfObjectMove_UnkVirtualFunc6();  //0x1DC
        virtual void CfObjectMove_UnkVirtualFunc7();  //0x1E0
        virtual void CfObjectMove_UnkVirtualFunc8();  //0x1E4
        virtual int CfObjectMove_UnkVirtualFunc9();   //0x1E8 (retail returns int: 1 or the CtrlNpc action-advance query)
        virtual void CfObjectMove_UnkVirtualFunc10(); //0x1EC
        virtual void CfObjectMove_UnkVirtualFunc11(); //0x1F0
        virtual void CfObjectMove_UnkVirtualFunc12(); //0x1F4
        virtual void CfObjectMove_UnkVirtualFunc13(); //0x1F8
        virtual void CfObjectMove_UnkVirtualFunc14(); //0x1FC
        virtual void CfObjectMove_UnkVirtualFunc15(); //0x200
        virtual void CfObjectMove_UnkVirtualFunc16(); //0x204
        virtual void CfObjectMove_UnkVirtualFunc17(); //0x208
        virtual void CfObjectMove_UnkVirtualFunc18(); //0x20C
        virtual void CfObjectMove_UnkVirtualFunc19(); //0x210
        virtual void CfObjectMove_UnkVirtualFunc20(); //0x214
        virtual void CfObjectMove_UnkVirtualFunc21(); //0x218
        virtual void CfObjectMove_UnkVirtualFunc22(); //0x21C
        virtual void CfObjectMove_UnkVirtualFunc23(); //0x220

        //0x0: vtable
        //0x0-BE: CfObjectModel
        // Field layout starting at offset 0xBE:
        u8 _BE[2];              // 0xBE-0xBF
        void* mFieldC0;         // 0xC0-0xC3 (non-zero gates the +0x38 sub-object release in CfObjectModel_UnkVirtualFunc1)
        void* mTargetC4;         // 0xC4-0xC7
        void* mTargetC8;         // 0xC8-0xCB
        void* mTargetCC;         // 0xCC-0xCF
        u8 _D0[0x53C];           // 0xD0-0x60B
        u8 _60C_region[0xB4];   // 0x60C-0x6BF
        void* mTarget6C0;         // 0x6C0-0x6C3
        u32 mField6C4;           // 0x6C4-0x6C7 (func_800BE1A4 stores 1)
        s8 mField6C8;            // 0x6C8 (func_800BE1A4 stores -1)
        u8 mFlags6C9;             // 0x6C9
        s16 _6CA;               // 0x6CA-0x6CB (ctor stores -1)
        u16 field_6CC;          // 0x6CC-0x6CD (u16 read by func_800BE33C)
        s8 field_6CE;           // 0x6CE
        s8 field_6CF;            // 0x6CF
        s16 _6D0;               // 0x6D0-0x6D1 (ctor stores 0)
        u8 _6D2[2];             // 0x6D2-0x6D3
        void* mField6D4;         // 0x6D4-0x6D7 (model list released by func_800BE3E8)
        void* mField6D8;         // 0x6D8-0x6DB (CfObjectMove_UnkVirtualFunc2 model list)
        void* mField6DC;         // 0x6DC-0x6DF (CfObjectMove_UnkVirtualFunc23)
        void* mField6E0;         // 0x6E0-0x6E3 (CfObjectMove_UnkVirtualFunc23)
        float mField6E4;          // 0x6E4-0x6E7 (CfObject_UnkVirtualFunc57 stores f1)
        float mField6E8;          // 0x6E8-0x6EB (CfObjectMove_UnkVirtualFunc4)
        float mField6EC;          // 0x6EC-0x6EF (ctor stores lbl_eu_80666A88)
        float mMoveSpeed;         // 0x6F0-0x6F3
        u8 _6F4;                 // 0x6F4 (ctor stores 0)
        u8 _6F5[3];             // 0x6F5-0x6F7
        void* mField6F8[2];      // 0x6F8-0x6FF (slots walked by CfObjectModel_UnkVirtualFunc19)
        u32 mField700;           // 0x700-0x703 (ctor stores 0)
        void* mField704;         // 0x704-0x707 (CfObjectMove_UnkVirtualFunc2 clears with 0x6D8)
        u32 mField708;           // 0x708-0x70B (ctor stores 0)
        u16 field_70C[2];        // 0x70C-0x70F (bdat id array written by CfObject_UnkVirtualFunc45)
        u16 field_710[2];        // 0x710-0x713 (bdat index array read by func_800BED80 / func_800BEDC4)
        u8 _714;                 // 0x714
        u8 unk715[3];            // 0x715-0x717
        float mField718;         // 0x718-0x71B (ctor stores lbl_eu_80666A88)
    void CfObject_UnkVirtualFunc4();
    void CfObject_UnkVirtualFunc7();
    void CfObject_UnkVirtualFunc6();
    void CfObjectModel_UnkVirtualFunc1();
    void CfObjectModel_UnkVirtualFunc2();
    void CfObject_UnkVirtualFunc5();
    void CfObject_UnkVirtualFunc46();
    void CfObject_UnkVirtualFunc47();
    void CfObject_UnkVirtualFunc49();
    void CfObject_UnkVirtualFunc64(int flag);
    void CfObject_UnkVirtualFunc65();
    void CfObject_UnkVirtualFunc19();
    CfObjectMove* CfObject_UnkVirtualFunc22();  // covariant (retail callers read r3 after dispatch)
    void CfObject_UnkVirtualFunc25();
    void CfObject_UnkVirtualFunc26(u32 value, float amount);
    u32 CfObject_UnkVirtualFunc23();
    void CfObject_UnkVirtualFunc27(void* src);
    void CfObject_UnkVirtualFunc30();
    void CfObject_UnkVirtualFunc32();
    void CfObject_UnkVirtualFunc33(float amount);
    void CfObject_UnkVirtualFunc13();
    void CfObject_UnkVirtualFunc57();
    void* CObjectParam_UnkVirtualFunc2();
    void* CfObject_UnkVirtualFunc14(float value);
    void CfObject_UnkVirtualFunc15();
    void* CfObject_UnkVirtualFunc16(float value);
    void CfObject_UnkVirtualFunc17();
    void CfObjectModel_UnkVirtualFunc18();
    bool CfObject_UnkVirtualFunc9();
    void CfObject_UnkVirtualFunc10();
    void CfObject_UnkVirtualFunc61();
    void CfObject_UnkVirtualFunc62();
    void CfObject_UnkVirtualFunc12();
    void CfObject_UnkVirtualFunc66(int);
    void CfObjectModel_UnkVirtualFunc19();
    void* CfObjectModel_UnkVirtualFunc6(void* arg);
    void CfObject_UnkVirtualFunc37();
    void CfObject_UnkVirtualFunc38();
    void CfObject_UnkVirtualFunc39();
    void CfObject_UnkVirtualFunc40();
    void CfObject_UnkVirtualFunc42();
    void CfObject_UnkVirtualFunc43();
    void CfObject_UnkVirtualFunc45();
    void CfObject_UnkVirtualFunc70(float value);
    int CfObject_UnkVirtualFunc50();
    int CfObject_UnkVirtualFunc51();
    void CfObject_UnkVirtualFunc60();
    void CfObject_UnkVirtualFunc29(float value);
    void setMoveSpeed(float value);
    void resetMoveSpeed();
    void updatePos();
    void* getUnk54();
    int getSubState();
    void freeSub();
    void setSubFieldC(unsigned short val);
    int getSubFieldA();
    void setSubFieldA(unsigned short val);
    int getSubFieldE();
    void setSubFieldE(unsigned short val);
    void virtCall10();
    int nullsub_25();
    int nullsub_26();
    int nullsub_27();
    int nullsub_28();
    int isActive();
    void setBit6c9(unsigned long bit);
    cf::CfObjectMove* testFlag8();
    };
    // Vtable proxy for calling cf-chain vtable slot +0xC8 as a float-taking
    // virtual (retail CfObject_UnkVirtualFunc33 tail-calls it with a scaled
    // value; the base header declares the slot void). Extends the +0xB4
    // proxy; dummy slots pin the offset.
    class CfObjectMoveVtC8 : public CfObjectVtB4 {
    public:
        virtual void mB8(); virtual void mBC(); virtual void mC0(); virtual void mC4();
        virtual void mC8(float value);  // vtable +0xC8
    };
    // Layout of the object behind CfObjectMove::mTargetC4 (fields written by
    // CfObject_UnkVirtualFunc4 / 14 / 57 and read by 15 / 17).
    struct CfObjectMoveTargetC4 {
        u8 _pad000[0x388];      // 0x00-0x387
        float field_388;        // 0x388 (CfObject_UnkVirtualFunc14 write / 15 read)
        float field_38C;        // 0x38C (CfObject_UnkVirtualFunc17 read / 16 write)
        u8 _pad390[0x3A8 - 0x390];  // 0x390-0x3A7
        float field_3A8;        // 0x3A8 (CfObject_UnkVirtualFunc23 returns &field_3A8)
        float field_3AC;        // 0x3AC (func_800BCD04 position copy)
        float field_3B0;        // 0x3B0 (func_800BCD04 position copy)
        u8 _pad3B4[0x430 - 0x3B4];  // 0x3B4-0x42F
        float field_430;        // 0x430 (CfObject_UnkVirtualFunc57)
        float field_434;        // 0x434 (CfObject_UnkVirtualFunc4)
        float field_438;        // 0x438 (CfObject_UnkVirtualFunc57)
        float field_43C;        // 0x43C (CfObject_UnkVirtualFunc57)
        u8 _pad440[0x444 - 0x440];  // 0x440-0x443
        float field_444;        // 0x444 (func_800BCD04 writes to mField4C)
    };
    // View of the CtrlNpc movement-target fields CfObject_UnkVirtualFunc49
    // reads (the +0xC0 action-id word). CtrlNpc.hpp is not included here:
    // its func_800BE12C 5-arg declaration conflicts with the 4-arg form
    // this unit needs, so a local view is used (same scheme as
    // CfObjectMoveTargetC4).
    struct CfObjectMoveNpcView {
        u8 _pad[0xC0];       // 0x00-0xBF
        u16 field_C0;        // 0xC0 action id
    };
    // Vtable proxy for the CfObjectMove mTargetC4 object's slot +0x80: retail
    // CfObject_UnkVirtualFunc13 tail-calls it with a 0 arg and returns its int
    // result (the null path returns 1). Dummy slots pin the offset.
    class CfObjectMoveC4Vt80 {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40(); virtual void m44();
        virtual void m48(); virtual void m4C(); virtual void m50(); virtual void m54();
        virtual void m58(); virtual void m5C(); virtual void m60(); virtual void m64();
        virtual void m68(); virtual void m6C(); virtual void m70(); virtual void m74();
        virtual void m78(); virtual void m7C();
        virtual int m80(int arg);  // vtable +0x80
    };
    // Vtable proxy for the CfObjectModel+0x98 sub-object's slot +0x64 with an
    // explicit int arg: retail func_800BE0F8 forwards a 4-bit flag extracted
    // from mFlags6C9 (the base CfObjectModelSub98Vt proxy declares the slot
    // no-arg). Dummy slots pin the offset.
    class CfObjectMoveSub98Vt64 {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40(); virtual void m44();
        virtual void m48(); virtual void m4C(); virtual void m50(); virtual void m54();
        virtual void m58(); virtual void m5C(); virtual void m60();
        virtual void m64(int arg);  // vtable +0x64
    };
    // Vtable proxy for calling the CfObjectMove slot +0x108
    // (CfObject_UnkVirtualFunc46) with an explicit pointer arg (retail
    // CfObject_UnkVirtualFunc47 dispatches the newly constructed target
    // through it; the base header declares the slot Fv). Dummy slots pin
    // the offset (same scheme as CfObjectMoveC4Vt80).
    class CfObjectMoveVt108 {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40(); virtual void m44();
        virtual void m48(); virtual void m4C(); virtual void m50(); virtual void m54();
        virtual void m58(); virtual void m5C(); virtual void m60(); virtual void m64();
        virtual void m68(); virtual void m6C(); virtual void m70(); virtual void m74();
        virtual void m78(); virtual void m7C(); virtual void m80(); virtual void m84();
        virtual void m88(); virtual void m8C(); virtual void m90(); virtual void m94();
        virtual void m98(); virtual void m9C(); virtual void mA0(); virtual void mA4();
        virtual void mA8(); virtual void mAC(); virtual void mB0(); virtual void mB4();
        virtual void mB8(); virtual void mBC(); virtual void mC0(); virtual void mC4();
        virtual void mC8(); virtual void mCC(); virtual void mD0(); virtual void mD4();
        virtual void mD8(); virtual void mDC(); virtual void mE0(); virtual void mE4();
        virtual void mE8(); virtual void mEC(); virtual void mF0(); virtual void mF4();
        virtual void mF8(); virtual void mFC(); virtual void m100(); virtual void m104();
        virtual void m108(void* arg);  // vtable +0x108 (CfObject_UnkVirtualFunc46)
    };
    // Vtable proxy for calling the CfObjectMove slot +0x208 as an int-taking
    // virtual (retail CfObjectMove_UnkVirtualFunc22 loops a counter through
    // it; the base header declares the slot Fv). Inherits CfObjectModel so
    // the new virtuals land in the CfObjectMove vtable range; the dummy slots
    // pin 0x1C8..0x204 and m208 lands at +0x208.
    class CfObjectMoveVt208 : public CfObjectModel {
    public:
        virtual void m1C8(); virtual void m1CC(); virtual void m1D0(); virtual void m1D4();
        virtual void m1D8(); virtual void m1DC(); virtual void m1E0(); virtual void m1E4();
        virtual void m1E8(); virtual void m1EC(); virtual void m1F0(); virtual void m1F4();
        virtual void m1F8(); virtual void m1FC(); virtual void m200(); virtual void m204();
        virtual void m208(int arg);  // vtable +0x208
    };
    // Vtable proxies for calling CfObjectMove vtable slots as arg-carrying
    // virtuals (retail CfObject_UnkVirtualFunc37 dispatches +0x1D0 with a 0/1
    // flag and +0x18C with its hidden r4; the base header declares those slots
    // no-arg). Dummy slots pin the offsets (same scheme as CfObjectMoveVt208).
    class CfObjectMoveVt1D0 : public CfObjectModel {
    public:
        virtual void m1C8(); virtual void m1CC();
        virtual void m1D0(int arg);  // vtable +0x1D0 (CfObjectMove_UnkVirtualFunc3)
    };
    class CfObjectMoveVt18C : public CfObjectModel {
    public:
        virtual void m178(); virtual void m17C(); virtual void m180(); virtual void m184();
        virtual u32 m188();  // vtable +0x188 (CfObjectModel_UnkVirtualFunc5 returns a value in retail)
        virtual void m18C(void* arg);  // vtable +0x18C (CfObjectModel_UnkVirtualFunc6)
    };
    // Vtable proxy for calling a CfObjectMove vtable slot +0x144 with two
    // explicit args (retail CfObject_UnkVirtualFunc39/42 dispatch the bdat
    // result through it; the base header declares CfObject_UnkVirtualFunc61
    // no-arg). Extends the cf-chain sub-object proxy (CObjectParam root,
    // last slot +0xA8); dummy slots pin +0xAC..+0x140.
    class CfObjectMoveVt144 : public CfObjectModelSub98Vt {
    public:
        virtual void mAC(); virtual void mB0(); virtual void mB4(); virtual void mB8();
        virtual void mBC(); virtual void mC0(); virtual void mC4(); virtual void mC8();
        virtual void mCC(); virtual void mD0(); virtual void mD4(); virtual void mD8();
        virtual void mDC(); virtual void mE0(); virtual void mE4(); virtual void mE8();
        virtual void mEC(); virtual void mF0(); virtual void mF4(); virtual void mF8();
        virtual void mFC(); virtual void m100(); virtual void m104(); virtual void m108();
        virtual void m10C(); virtual void m110(); virtual void m114(); virtual void m118();
        virtual void m11C(); virtual void m120(); virtual void m124(); virtual void m128();
        virtual void m12C(); virtual void m130(); virtual void m134(); virtual void m138();
        virtual void m13C(); virtual void m140();
        virtual void m144(u32 a, u32 b);  // vtable +0x144 (CfObject_UnkVirtualFunc61)
    };
    // View of the CfObjectModel field_0x90 area: retail
    // CfObject_UnkVirtualFunc38 clears the +0x94 word. Cast on `self` with
    // the full offset pad, so field_94 lands at 0x94 (same scheme as
    // CfObjectMoveC4Flags).
    struct CfObjectMove90View {
        u8 _pad90[0x94];     // 0x00-0x93
        u32 field_94;        // 0x94-0x97
    };
    // View of the CfObject base word at +0x6C (func_800BE3E8 toggles bit 16;
    // CfObject.hpp exposes 0x6C-0x6F as opaque padding).
    struct CfObjectMoveFlags6C {
        u8 _pad[0x6C];
        u32 field_6C;  // 0x6C
    };
    // View of the CfObjectMove +0xA4/+0xA8/+0xAC floats (inside
    // CfObjectModel's opaque 0xA4-0xAF pad): func_800BC68C reads/writes
    // field_A8 as a movement speed; func_800BC8D8 and
    // CfObject_UnkVirtualFunc5 use all three as approach-rate terms.
    struct CfObjectMoveA8View {
        u8 _pad[0xA4];
        float field_A4;  // 0xA4
        float field_A8;  // 0xA8
        float field_AC;  // 0xAC
    };
    // Byte view of CfObjectMove::mFlags6C9 as bitfields (MWCC lays out the
    // first-declared field at the MSB). top3 spans word bits 24-26; clearing
    // it emits the retail rlwinm r0,r0,0,27,23 that func_800BE12C keeps (a
    // plain byte AND folds the mask to 27,31 via range analysis). mid4 spans
    // word bits 27-30 and b0 is word bit 31: the ctor's flag init assigns
    // mid4 = 10 and b0 = 0, which MWCC decomposes into the retail
    // li r3,5; rlwimi r0,r3,1,27,30; clrrwi r0,r0,1 sequence.
    struct CfFlags6C9 {
        u8 top3 : 3;   // word bits 24-26
        u8 mid4 : 4;   // word bits 27-30
        u8 b0 : 1;     // word bit 31
    };
    // Layout of the +0xC8/+0xCC target objects func_800BD644 constructs and
    // initializes (ctor __ct__8005A3FC): +0x4/+0x34 receive the +0x38
    // sub-object pointer, +0x378 the slot index, +0x4F4 the owner.
    struct CfObjectMoveC8View {
        u8 _pad00[0x4];           // 0x00-0x03
        void* field_4;            // 0x04 (self->mSubObj38)
        u8 _pad08[0x34 - 0x8];    // 0x08-0x33
        void* field_34;           // 0x34 (self->mSubObj38)
        u8 _pad38[0x378 - 0x38];  // 0x38-0x377
        u32 field_378;            // 0x378 (slot index)
        u8 _pad37C[0x4F4 - 0x37C];  // 0x37C-0x4F3
        void* field_4F4;          // 0x4F4 (owner CfObjectMove)
    };
    // Base pad that places the vptr at object+0x10 (retail double-hop reads
    // the vtable via lwz r12, 0x10(r3); same scheme as the file-scope
    // ObjVtIf view in CfObjectMove.cpp).
    class CfObjectMoveSubB0Shift {
    public:
        u8 _pad[0x10];
    };
    // Vtable proxy for the +0xB0 sub-object's slot +0x4C with an explicit
    // arg (retail CfObject_UnkVirtualFunc38 forwards its hidden r4; the
    // file-scope ObjVtIf view declares the slot no-arg, so this typed slot
    // is a separate class). Dummy slots pin the offset.
    class CfObjectMoveSubB0Vt4C : public CfObjectMoveSubB0Shift {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void vf14();
        virtual void vf18(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void vf28(); virtual void vf2C(); virtual void vf30(); virtual void vf34();
        virtual void _v038(); virtual void _v03C(); virtual void vf40(); virtual void _v044();
        virtual void vf48();
        virtual void _v04C(u32 arg);  // vtable +0x4C
    };
    // Vtable proxy for the +0xB0 sub-object's slot +0x3C with two explicit
    // args (retail CfObject_UnkVirtualFunc39/42 forward the bdat result and
    // a 0/1 flag; the file-scope ObjVtIf view declares the slot no-arg).
    // Dummy slots pin the offset (same scheme as CfObjectMoveSubB0Vt4C).
    class CfObjectMoveSubB0Vt3C : public CfObjectMoveSubB0Shift {
    public:
        virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void vf14();
        virtual void vf18(); virtual void _v01C(); virtual void _v020(); virtual void _v024();
        virtual void vf28(); virtual void vf2C(); virtual void vf30(); virtual void vf34();
        virtual void _v038();
        virtual void m3C(u32 a, u32 b);  // vtable +0x3C
    };
    // View of the CfObjectMove flags word at +0x68 (CfObject_UnkVirtualFunc5
    // clears bits through this separate view type so MWCC's alias analysis
    // treats the clamp-path clear as a fresh read - retail reloads mFlags68
    // (lwz r0,0x68) at each clamp-path clear).
    struct CfObjectMoveFlags68View {
        u8 _pad[0x68];
        u32 flags68;  // 0x68
    };
    // Vtable proxy for the +0x98 sub-object's slot +0x48 with an explicit
    // float arg (retail func_800BC8D8 forwards the movement value to the
    // slot; the CObjectParam base header declares it no-arg). Dummy slots
    // pin the offset (same scheme as CfObjectMoveSub98Vt64).
    class CfObjectMoveSub98Vt48 {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40(); virtual void m44();
        virtual void m48(float value);  // vtable +0x48
    };
    // Vtable proxy for the +0x98 sub-object's slot +0x50 with an explicit
    // int arg (retail func_800BE33C forwards its flag to the slot; the
    // CObjectParam base header declares it no-arg). Dummy slots pin the
    // offset (same scheme as CfObjectMoveSub98Vt64).
    class CfObjectMoveSub98Vt50 {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40(); virtual void m44();
        virtual void m48(); virtual void m4C();
        virtual void m50(int flag);  // vtable +0x50
    };
    // Vtable proxy for the +0x98 sub-object's slots +0xC4/+0xC8 (the sub's
    // retail vtable is the CModelDisp family layout): CfObject_UnkVirtualFunc45
    // stops the slot's animation model (mC8, one arg) then re-arms it with
    // the name and a 0 flag (mC4). Extends CfObjectModelSub98Vt (pins +0x54
    // .. +0xA8); dummy slots pin +0xAC..+0xC0.
    class CfObjectMoveSub98VtC4 : public CfObjectModelSub98Vt {
    public:
        virtual void mAC(); virtual void mB0(); virtual void mB4(); virtual void mB8();
        virtual void mBC(); virtual void mC0();
        virtual void mC4(void* animModel, u32 name, u32 flag);  // vtable +0xC4
        virtual void mC8(void* animModel);  // vtable +0xC8
    };
    // Vtable proxy for the mSubObj38 object's slot +0xE4 (an int-returning
    // query): retail CfObjectMove_UnkVirtualFunc21 calls it, tests the
    // result, then re-calls it. CfObjectSub38If stops at +0xAC, so dummy
    // slots pin the range up to +0xE4.
    class CfObjectSub38VtE4 : public CfObjectSub38If {
    public:
        virtual void _fB0(); virtual void _fB4(); virtual void _fB8(); virtual void _fBC();
        virtual void _fC0(); virtual void _fC4(); virtual void _fC8(); virtual void _fCC();
        virtual void _fD0(); virtual void _fD4(); virtual void _fD8(); virtual void _fDC();
        virtual void _fE0();
        virtual int _fE4();  // vtable +0xE4
    };
    // View of the +0xD0 CActParamAnimGame sub-object fields func_800BCFA0
    // touches: owner at +0x4E8, flag word at +0x4EC, speeds at +0x504/+0x50C,
    // and the u16 at +0x530.
    struct CfObjectMoveD0View {
        u8 _pad00[0x4E8];       // 0x00-0x4E7
        void* field_4E8;        // 0x4E8 (owner CfObjectMove)
        u32 field_4EC;          // 0x4EC
        u8 _pad4F0[0x504 - 0x4F0];  // 0x4F0-0x503
        float field_504;        // 0x504
        u8 _pad508[0x50C - 0x508];  // 0x508-0x50B
        float field_50C;        // 0x50C
        u8 _pad510[0x530 - 0x510];  // 0x510-0x52F
        u16 field_530;          // 0x530
    };
    // View of the +0x6C0 movement-target object's float fields func_800BC9EC
    // reads/writes (+0xC angle, +0x14 speed).
    struct CfObjectMove6C0View {
        u8 _pad[0xC];           // 0x00-0x0B
        float field_C;          // 0x0C
        u8 _pad10[0x14 - 0x10]; // 0x10-0x13
        float field_14;         // 0x14
    };
    // View of the +0x6B4 word inside the +0x60C region (func_800BC4CC toggles
    // bit 12 around the func_804B192C call).
    struct CfObjectMove6B4View {
        u8 _pad[0x6B4];         // 0x00-0x6B3
        u32 field_6B4;          // 0x6B4
    };
    // View of the scene object func_80496264 returns: its +0x10C position
    // vector is read by func_800BC4CC (the func_804B192C target).
    struct CfResScene10C {
        u8 _pad[0x10C];         // 0x00-0x10B
        float x;                // 0x10C
        float y;                // 0x110
        float z;                // 0x114
    };
    // View of the CfObjectModel +0xB4/+0xB8 words (CfObject_UnkVirtualFunc4
    // forwards them to the vtable +0x1AC slot; CfObjectModel declares the
    // range as opaque padding).
    struct CfObjectMoveB4View {
        u8 _pad[0xB4];          // 0x00-0xB3
        u32 field_B4;           // 0xB4
        u32 field_B8;           // 0xB8
    };
    // View of the CfObjectModel +0x94/+0x9C words (func_800BCFA0 forwards them
    // to the CActParamAnim helpers; CfObjectModel declares both opaque).
    struct CfObjectMove94View {
        u8 _pad[0x94];          // 0x00-0x93
        u32 field_94;           // 0x94
        u8 _pad98[0x9C - 0x98]; // 0x98-0x9B
        u32 field_9C;           // 0x9C
    };
    // View of the CfObjectMove +0x54 diff-vector area (func_800BCD04 /
    // func_800BC9EC store a position difference there as integer words;
    // CfObjectModel declares 0x54 as CfObjectSub54).
    struct CfObjectMove54View {
        u8 _pad[0x54];          // 0x00-0x53
        u32 field_54;           // 0x54
        u32 field_58;           // 0x58
        u32 field_5C;           // 0x5C
    };
    // View of the func_800AD860 result's +0x45CA u16 (func_800BC9EC tests its
    // bit 0).
    struct CfObjectMoveAD86View {
        u8 _pad[0x45CA];        // 0x00-0x45C9
        u16 field_45CA;         // 0x45CA
    };
    // Views of the +0x98 sub-object's +0x7A0/+0x7EC words and the object behind
    // +0x7EC (func_800BCFA0 clears a bit at +0x7A0, writes a byte at +0x2A of
    // the +0x7EC target and calls its vtable +0x1C).
    struct CfObjectMoveSub98View7A0 {
        u8 _pad[0x7A0];         // 0x00-0x79F
        u32 field_7A0;          // 0x7A0
    };
    struct CfObjectMoveSub98View7EC {
        u8 _pad[0x7EC];         // 0x00-0x7EB
        void* field_7EC;        // 0x7EC
    };
    struct CfObjectMoveSub98View31C {
        u8 _pad[0x31C];         // 0x00-0x31B
        u8 field_31C;           // 0x31C
    };
    struct CfObjectMove7ECView {
        u8 _pad[0x2A];          // 0x00-0x29
        u8 field_2A;            // 0x2A
    };
    // Vtable proxy for the func_800821F8 result (dynamic manager): retail
    // func_800BC4CC invokes its slot +0x60 and compares the result to self.
    // Dummy slots pin the offset.
    class CfDynMgrVt60 {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40(); virtual void m44();
        virtual void m48(); virtual void m4C(); virtual void m50(); virtual void m54();
        virtual void m58(); virtual void m5C();
        virtual void* m60();  // vtable +0x60
    };
    // Vtable proxy for calling the CfObjectMove slot +0x160 as an
    // int-returning virtual (retail CfObject_UnkVirtualFunc68 returns a value;
    // the base header declares the slot void). Extends the +0x14C proxy;
    // dummy slots pin +0x150..+0x15C.
    class CfObjectMoveVt160 : public CfObjectVt14C {
    public:
        virtual void m150(); virtual void m154(); virtual void m158(); virtual void m15C();
        virtual int m160();  // vtable +0x160
    };
    // Vtable proxy for calling the CfObjectMove slot +0x184 as a
    // pointer-returning virtual (retail CfObjectModel_UnkVirtualFunc4 returns
    // a value forwarded to func_8004B624; the base header declares it void).
    class CfObjectMoveVt184 : public CfObjectMoveVt160 {
    public:
        virtual void m164(); virtual void m168(); virtual void m16C(); virtual void m170();
        virtual void m174(); virtual void m178(); virtual void m17C(); virtual void m180();
        virtual void* m184();  // vtable +0x184
    };
    // Vtable proxy for calling the CfObjectMove slot +0x1A4 with an explicit
    // float arg (retail func_800BCFA0 passes lbl_eu_80666A94; the base header
    // declares CfObjectModel_UnkVirtualFunc12 no-arg).
    class CfObjectMoveVt1A4 : public CfObjectMoveVt184 {
    public:
        virtual void m188(); virtual void m18C(); virtual void m190(); virtual void m194();
        virtual void m198(); virtual void m19C(); virtual void m1A0();
        virtual void m1A4(float value);  // vtable +0x1A4
    };
    // Vtable proxy for calling the CfObjectMove slot +0x1AC with two explicit
    // args (retail CfObject_UnkVirtualFunc4 forwards the +0xB4/+0xB8 words;
    // the base header declares CfObjectModel_UnkVirtualFunc14 no-arg).
    class CfObjectMoveVt1AC : public CfObjectMoveVt1A4 {
    public:
        virtual void m1A8();
        virtual void m1AC(u32 a, u32 b);  // vtable +0x1AC
    };
    // Vtable proxy for calling the CfObjectMove slot +0x154 with an explicit
    // int arg (retail func_800BCFA0 passes 1; the header declares
    // CfObject_UnkVirtualFunc65 no-arg). m150 pins +0x150.
    class CfObjectMoveVt154 : public CfObjectVt14C {
    public:
        virtual void m150(int flag);  // vtable +0x150 (CfObject_UnkVirtualFunc64)
        virtual void m154(int flag);  // vtable +0x154 (CfObject_UnkVirtualFunc65)
    };
    // Vtable proxy for calling the CfObjectMove slot +0xA8 with an explicit
    // position-vector arg (retail func_800BCFA0 passes self+0x3C; the header
    // declares CfObject_UnkVirtualFunc22 no-arg). Dummy slots pin +0x08..+0xA4.
    class CfObjectMoveVtA8 {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40(); virtual void m44();
        virtual void m48(); virtual void m4C(); virtual void m50(); virtual void m54();
        virtual void m58(); virtual void m5C(); virtual void m60(); virtual void m64();
        virtual void m68(); virtual void m6C(); virtual void m70(); virtual void m74();
        virtual void m78(); virtual void m7C(); virtual void m80(); virtual void m84();
        virtual void m88(); virtual void m8C(); virtual void m90(); virtual void m94();
        virtual void m98(); virtual void m9C(); virtual void mA0(); virtual void mA4();
        virtual void mA8(const ml::CVec3* vec);  // vtable +0xA8
    };
    // Vtable proxy for the +0x98 sub-object's slot +0x4C as a float-returning
    // virtual (retail CfObject_UnkVirtualFunc4 compares the result against a
    // constant; the CObjectParam base header declares it BOOL).
    class CfObjectMoveSub98Vt4C {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40(); virtual void m44();
        virtual void m48();
        virtual float m4C();  // vtable +0x4C
    };
    // Vtable proxy for the +0x98 sub-object's slot +0x54 as an int-returning
    // virtual (retail func_800BCFA0 tests the result; the CfObjectModelSub98Vt
    // header declares it void).
    class CfObjectMoveSub98Vt54 {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40(); virtual void m44();
        virtual void m48(); virtual void m4C(); virtual void m50();
        virtual int m54();  // vtable +0x54
    };
    // Vtable proxy for the +0x98 sub-object's slot +0x9C with two explicit
    // args (retail func_800BCFA0 forwards a 0/1/2 index and 0; the
    // CfObjectModelSub98Vt header declares it no-arg).
    class CfObjectMoveSub98Vt9C {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18(); virtual void m1C(); virtual void m20(); virtual void m24();
        virtual void m28(); virtual void m2C(); virtual void m30(); virtual void m34();
        virtual void m38(); virtual void m3C(); virtual void m40(); virtual void m44();
        virtual void m48(); virtual void m4C(); virtual void m50(); virtual void m54();
        virtual void m58(); virtual void m5C(); virtual void m60(); virtual void m64();
        virtual void m68(); virtual void m6C(); virtual void m70(); virtual void m74();
        virtual void m78(); virtual void m7C(); virtual void m80(); virtual void m84();
        virtual void m88(); virtual void m8C(); virtual void m90(); virtual void m94();
        virtual void m98();
        virtual void m9C(u32 a, u32 b);  // vtable +0x9C
    };
    // Vtable proxy for the +0x7EC target object's slot +0x1C with an explicit
    // flag + float (retail func_800BCFA0 passes 1 and lbl_eu_8066AF20).
    class CfObjectMoveVt1C {
    public:
        virtual void m08(); virtual void m0C(); virtual void m10(); virtual void m14();
        virtual void m18();
        virtual void m1C(u32 a, f32 b);  // vtable +0x1C
    };
}

