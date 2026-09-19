#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectModel.hpp"
#include "kyoshin/cf/CfGameManagerApi.hpp"
#include "libs/monolib/src/scn/CScnItemModel.hpp"

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
// (CfObjectMove::CfObject_setMoveTargetVec forwards its hidden r4 through).
void CfObject_setMoveTargetVec__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, const ml::CVec3* vec);

// Cross-TU dispatch targets for the CfObjectMove mTarget6C0 wrappers (defined
// in kyoshin/cf/CtrlNpc.cpp, retail CtrlNpcInitMoveWindow family). extern "C" keeps
// the call relocs unmangled - plain C++ global decls get __F-suffix mangled
// at call sites (same scheme as CtrlNpc.hpp's CfObjectMove_setAnimModeArgs declarations).
extern "C" void CtrlNpcInitMoveWindow(cf::CtrlNpc* self);
extern "C" void CtrlNpcSetupAction(cf::CtrlNpc* self);
extern "C" void CtrlNpcStoreTargetActivate(cf::CtrlNpc* self);
extern "C" void func_80094E44(cf::CtrlNpc* self);
extern "C" void CtrlNpcArmMoveTarget5(cf::CtrlNpc* self);
extern "C" void CtrlNpcSetFlagActivate(cf::CtrlNpc* self);
// NPC movement-target action setter (CtrlNpc.cpp, retail func_8009377C):
// CfObject_forwardNpcAction tail-dispatches the incoming action id.
extern "C" void func_8009377C(cf::CtrlNpc* self, u32 param);
// Action-advance query on the NPC movement target (CtrlNpc.cpp, retail
// CtrlNpcCanAdvanceAction / US 0x800956F4): whether the NPC may advance.
extern "C" int CtrlNpcCanAdvanceAction(const cf::CtrlNpc* self);
// Region-library helpers (0x804B0xxx): attach/detach a region object to the
// shared manager global below. CfGimmickLock.hpp also declares these but is
// not included here (its func_800817BC return type conflicts with the
// CfGameManager unity header). ColiNodeInit is the +0x60C region
// sub-object ctor called from the CfObjectMove ctor.
extern "C" void ColiNodeInit(void* region);
extern "C" void func_804B4BDC(void* manager, void* region);
extern "C" void Coli_ListRemoveNode(void* manager, void* region);
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
extern const char lbl_eu_804FC550[];   // bdat column-name table (CfObjectMove_lookupBdatCol7 / CfObjectMove_lookupBdatCol11)
extern u32 lbl_eu_80663E28;   // global flag word (bit 26) gating the CfObjectMove_detachMoveRegion flag clear
// Global flag word (bits 25/9) gating func_800BC8D8's flag block. Non-volatile
// extern so the type agrees with CSystemWindow.hpp and the .sbss definition in
// CUICfManager.cpp; reads that must stay separate use explicit volatile casts
// in CfObjectMove.cpp (retail loads it twice there - see the double-lwz note).
extern u32 lbl_eu_80663E24;
extern u16 lbl_eu_80663E42;   // mode words checked by CfObject_UnkVirtualFunc26 (== 4 / == 1)
extern u16 lbl_eu_80663E44;
extern CScn* lbl_eu_80663E14;  // shared scene pointer passed to the Scn_GetFrameDelta time query
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
// CfObjectMove_updateSpeedA8 (lbl_eu_80666AA4/AA8/AAC/AB0 + ml::epsilon) and
// CfObject_setMoveTargetPtr (lbl_eu_80666AD0 scale).
extern const float lbl_eu_80666AA4;
extern const float lbl_eu_80666AA8;
extern const float lbl_eu_80666AAC;
extern const float lbl_eu_80666AB0;
extern const float lbl_eu_80666AD0;
extern const float lbl_eu_80666AB4;   // CfObject_UnkVirtualFunc5 movement-rate constant
// CfObject_UnkVirtualFunc26 constants: lbl_eu_80666AD4 is the amount clamp
// threshold, lbl_eu_80666AD8 the sum-vector Y offset.
extern const float lbl_eu_80666AD4;
extern const float lbl_eu_80666AD8[2];
extern const float lbl_eu_8066A208;   // ml::epsilon (sdata2)
// Movement constants for CfObjectMove_testMoveProximity: lbl_eu_80666A98 is the squared-
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
extern f32 lbl_eu_8066AF20; // f32 form: must match CtrlMoveBase.hpp (10197/10505 on mixed spellings)
// func_800BCFA0 approach-rate constants (selected by mFlags68 bit 1).
extern const float lbl_eu_80666AC8;
extern const float lbl_eu_80666ACC;

// Scene-time query (CfGameManager.cpp, retail unmangled name): returns the
// current time value from the shared scene object. extern "C" keeps the
// call-site reloc at the unmangled retail name (docs/MWCC_CASES.md sec 2).
extern "C" f32 Scn_GetFrameDelta(void* scene);
// Minimal bdat imports (CfBdat.hpp cannot be included here: its
// getBdatStringColumnValue declaration conflicts with harness_catalog.hpp's).
// The static-member forms keep the retail mangled reloc names.
namespace cf {
class CfBdat {
public:
    static u32 func_801422A8(u32 param1);
    static const char* getBdatStringEntry(u16 index);
    static void resetMapBdatFileDataPointers();
};
}  // namespace cf
// Retail sbss bdat globals used by func_800BCFA0.
extern void* lbl_eu_806640A8;   // spFldMapListFileData
extern u32 lbl_eu_80664184;     // cached fld-map row index
// One-arg call form of the CActParamAnim translation helper (defined 2-arg
// in kyoshin/action/CActParamAnim.cpp). Retail CfObject_setMoveTargetVec
// calls it with only r3 set. C linkage keeps the retail unmangled name.
extern "C" void setAnimSubPos(void* self);
// Second heap handle query (retail unmangled name, same family as
// CfRes_getHeapHandle): used by CfObject_createMoveTarget's CtrlEnemy/CtrlNpc
// allocations.
extern "C" u32 CfRes_getAllocHandle();
// CActParamAnim translation helper (defined in kyoshin/action/CActParamAnim.cpp
// as extern "C"). Retail CfObject_UnkVirtualFunc27 / UnkVirtualFunc29 call it
// with the +0xC4 target; C linkage keeps the retail unmangled name.
extern "C" void setTurnAngle(void* target, f32 value);
// Base +0xB4 forced-name implementation (defined in CfObjectModel.cpp as the
// CfObject member; the retail symbol is Fv but the body reads r4 as a
// position vector). extern "C" keeps the call-site reloc at the unmangled
// retail name (CfObject_UnkVirtualFunc26 forwards its hidden r4 through).
extern "C" void CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv(cf::CfObject* self, const ml::CVec3* vec);
// Base +0x178 forced-name implementation (defined in CfObjectModel.cpp as the
// CfObjectModel member; the retail symbol is Fv). CfObjectModel_UnkVirtualFunc1
// (CfObjectMove) calls it before tearing down its own state.
extern "C" void CfObjectModel_UnkVirtualFunc1__Q22cf13CfObjectModelFv(cf::CfObjectModel* self);
// Base +0x17C forced-name stub (retail symbol UnkVirtualFunc2; Model also
// exposes it as releaseModelList). Move's override body calls this Fv so the
// call-site reloc keeps the Unk retail name.
extern "C" void CfObjectModel_UnkVirtualFunc2__Q22cf13CfObjectModelFv(cf::CfObjectModel* self);
// Region-library helpers (retail unmangled names, defined outside this unit):
// ColiNodeReleaseBuffer is the +0x60C region sub-object release called from
// CfObjectModel_UnkVirtualFunc1 (CfObjectMove); CfModel_SyncVisFlag forwards a
// visibility flag to the +0x98 sub-object (CfObjectModel.cpp).
extern "C" void ColiNodeReleaseBuffer(u8* region);
extern "C" void CfModel_SyncVisFlag(cf::CfObjectModel* self, u32 flag);
// Movement-position update (defined in this TU below; forward-declared here
// because CfObject_UnkVirtualFunc5 calls it). extern "C" keeps the call-site
// reloc at the unmangled retail name.
extern "C" void func_800BC8D8(cf::CfObjectMove* self);
// Movement-speed helper (defined in this TU below) and the status query used
// by func_800BC8D8. extern "C" keeps the call-site relocs unmangled.
extern "C" void CfObjectMove_updateSpeedA8(cf::CfObjectMove* self, u32 arg);
extern "C" int CfObjectMove_testMoveProximity(cf::CfObjectMove* self);
// Same-TU movement/position helpers (defined in CfObjectMove.cpp below;
// CfObject_UnkVirtualFunc4 dispatches func_800BCD04).
extern "C" void func_800BCD04(cf::CfObjectMove* self);
extern "C" void func_800BC9EC(cf::CfObjectMove* self);
extern "C" void func_800BCFA0(cf::CfObjectMove* self);
// Scene/manager queries used by CfObjectMove_testMoveProximity / func_800BC9EC / func_800BCFA0.
extern "C" int CfRes_getD80Flag();
extern "C" void* Scn_FindCamItem(void* obj, int index);
extern "C" int func_8007560C();
extern "C" bool func_800829B8__Q22cf13CfGameManagerFv();
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int index);
// Camera-block getter (retail getCameraDataBlock__Q22cf13CfGameManagerFv,
// 0x80082B7C; returns the CfCamEventSlot-family object at CfGameManager+0xB0
// - see func_800784A0 in CfCamEvent_1.cpp, which returns manager->slots[idx]).
// Single winning decl lives in kyoshin/cf/CfGameManagerApi.hpp (included
// above); the old per-TU func_800821F8 placeholder name was a stale alias
// for this same symbol (hexdiff reloc drift in CfObjectMove_testMoveProximity).
// Region-library helper used by CfObjectMove_testMoveProximity (region, target, flags, dist).
extern "C" int ColiCheckMoveRadius(void* region, void* target, int arg2, int arg3, f32 dist);
// +0x98 sub-object flag query (CfObjectModel.cpp, retail unmangled name) used
// by CfObject_UnkVirtualFunc4.
extern "C" u32 CfModel_GetFlag7A8(cf::CfObjectModel* self);
// CActParamAnim helpers (defined in kyoshin/cf/CActParamAnimGame.cpp,
// retail unmangled names). tickAnimFrame releases a +0xC8 target;
// pushAnimNode / attachAnimObj attach model lists to the +0xC4 target;
// startAnimModeA family drives the +0xC4 target's page state (func_800BCFA0).
extern "C" void tickAnimFrame(void* self);
extern "C" void pushAnimNode(void* self, void* a, void* b);
extern "C" void startAnimModeA(void* self);
extern "C" void startAnimModeA2(void* self);
extern "C" void startAnimModeC(void* self);
extern "C" void startAnimModeB(void* self);
// Region-library helpers used by func_800BC9EC / func_800BCFA0.
extern "C" void ColiNodeSetWord8Rebuild(void* region);
extern "C" void func_804B1164(void* region, void* out, const void* vec, void* out2);
// Collision/position stepper (defined in kyoshin/cf/CActParamAnimGame.cpp,
// retail unmangled name): 6 int-ish args + 5 float args.
extern "C" u32 func_804BD94C(void* a, void* b, u32 c, u32 d, u32 e, u32 f,
                              f32 g, f32 h, f32 i, f32 j, f32 k);
// Model sub-object flag setter (defined in realtimeevt/CREvtModel.cpp) used
// by func_800BCFA0's bdat-flag tail.
extern "C" void ScnEnvLgt_EnableFlag100(void* self, int flag);
// Region position sync + model sub-object helpers used by func_800BCD04 /
// func_800BC9EC (ColiSetAxisBlockInverse) and func_800BCFA0 (simSetLeafDist7B0).
extern "C" void ColiSetAxisBlockInverse(void* region, const float* vec);
extern "C" void simSetLeafDist7B0(void* self, f32 value);
// nw4r math sweep functions used by func_800BC9EC (retail names keep the
// call relocs unmangled).
extern "C" f32 SinFIdx__Q24nw4r4mathFf(f32);
extern "C" f32 CosFIdx__Q24nw4r4mathFf(f32);
// Enemy-state query (mangled global C++ symbol func_800AD860__FPv; defined
// outside this repo's src tree) used by func_800BC9EC.
void* getEffOwner(void* obj);
// vtable +0x14C flag-word bit queries (defined in CfObjectModel.cpp as plain
// global C++ functions) used by func_800BCFA0's dispatch tail. extern "C":
// the retail symbols are the unmangled names.
extern "C" u32 CfObj_KindBit1(cf::CfObject* obj);
extern "C" u32 CfObj_KindBit3(cf::CfObject* obj);
extern "C" u32 CfObj_KindBit2(cf::CfObject* obj);
extern "C" u32 CfObj_KindBit7(cf::CfObject* obj);
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
// declaration conflicts with CfObjectModel.hpp's Scn_IsAnimActiveOrNull (C-linkage
// overload) and CfBdat.hpp (read-only) does not declare it.
extern "C" u32 func_8014235C(u32 param1, const char* column, u32 param3);

// Retail unmangled import used by the CfObjectMove wrappers: voice-request
// helper (defined in kyoshin/cf/voice/CCharVoice.cpp). The CCharVoice member
// declaration takes three args; retail func_800BE898 forwards only `this`
// (addi r3,+0x28; b), so a one-arg C-ABI form is declared here to
// reproduce the call site exactly.
extern "C" bool playGated(void* self);
// One-arg call form of the CActParamAnim translation helper (defined 2-arg
// in kyoshin/action/CActParamAnim.cpp). Retail CfObject_UnkVirtualFunc22
// calls it with only r3 set - r4 is left as the incoming vec, which the
// base call below consumes (same scheme as the CfObjectModel setAnimSubPos
// use). C linkage keeps the call-site reloc at the unmangled retail name.
extern "C" bool setAnimLastPos(cf::CfObjectMoveTargetC4* self);
// One-arg call form of the camera-event flush helper (defined in
// kyoshin/cf/CfCamEvent.cpp, which declares it no-arg). Retail
// CfObject_UnkVirtualFunc22 calls it with the +0x98 sub-object in r3, so the
// arg-carrying form reproduces the null-check load landing in r3.
extern "C" void scnImN4DynStart(cf::CfObjectModelSub98* sub);
// EU-named camera-time helper (retail CfObject_UnkVirtualFunc70 calls it
// with the +0x38 sub-object's vtable +0xE4 query result in r3 and a float
// in f1); the US build keeps the EU name for this reloc.
extern "C" void func_eu_8015D258(void* self, float value);
// No-arg call form of the CCharEffect slot-release helper (defined in
// kyoshin/cf/CCharEffect.cpp with a self param). Retail
// CfObjectMove_flushEffectSlots calls it with the vtable query result
// leftover in r3 - the call site passes no argument.
extern "C" void CharEffReleaseSlots0A10();
// Two-arg call forms of the CCharEffect slot helpers (defined in
// kyoshin/cf/CCharEffect.cpp with (CCharEffect*, u32)). Retail
// CfObjectMove_setEffectSlotBit/18/20 pass the +0x38 sub-object's vtable
// +0xE4 query result leftover in r3 and the incoming r4 as the index; C
// linkage keeps the call relocs at the unmangled retail names (same scheme
// as the CharEffReleaseSlots0A10 no-arg form).
extern "C" void func_8015C074(void* self, u32 idx);
extern "C" void CharEffMoveBit(void* self, u32 idx);
extern "C" void func_8015BD24(void* self, u32 param);
// Two-arg call forms of the CCharEffect slot helpers (defined in
// kyoshin/cf/CCharEffect.cpp). CharEffRemoveByPtr removes a pointer from the
// effect slot lists; func_8015C294 returns whether a slot is occupied.
// Retail CfObjectModel_UnkVirtualFunc18 / CfObjectMove_isEffectSlotUsed
// forward the +0x38 sub-object's vtable +0xE4 query result leftover in r3
// plus the incoming r4 (same scheme as the func_8015C074 family above).
extern "C" void CharEffRemoveByPtr(void* self, void* p);
extern "C" bool func_8015C294(unsigned int* param1, int param2);
// CCharEffect slot-detach helper (defined in kyoshin/cf/CCharEffect.cpp
// with (CCharEffect*, CCharEffectvtable*); CCharEffect.hpp is not
// included here, so the canonical typed form is not visible). C linkage
// keeps the call-site reloc at the plain retail name (same scheme as the
// func_8015C074 family above).
extern "C" void CharEffDetachBySubId(void* self, void* manager);
// Six-arg call form of the CCharEffect slot-register helper (defined in
// kyoshin/cf/CCharEffect.cpp with (CCharEffect*, u32, CCharEffectSlot*,
// u16, u16, u8); the call site only forwards register values, so the arg
// types below reproduce the retail call exactly - the s16 fourth arg emits
// the retail extsh at the call site).
extern "C" void CharEffRegisterSlot(void* self, u32 a, u32 b, s16 c, u32 d, u32 e);
// CActParamAnim helpers (defined in kyoshin/action/CActParamAnim.cpp,
// retail unmangled names). setTurnScale is the float-taking translation
// helper. func_8004B9D4 is called at the retail CfObjectMove_setAnimModeArgs site with
// only r3-r6 set (the incoming params); the canonical CModelDispEquip.hpp
// 5-arg declaration conflicts with CfObjectModel.hpp, so a 4-arg form is
// declared here to reproduce the call site exactly (same scheme as the
// setAnimLastPos one-arg form above).
extern "C" bool setTurnScale(void* self, float value);
extern "C" void func_8004B9D4(void* self, u32 a, u32 b, u32 c);
// CActParamAnim-view helpers used by CfObjectMove_ensureAnimTargets (defined in
// kyoshin/action/CActParamAnim.cpp / CActParamAnimGame.cpp, retail
// unmangled names; the canonical CModelDispEquip.hpp declarations take
// CActParamAnimView* which is not visible here - CfObjectMove.hpp's
// Scn_IsAnimActiveOrNull C-linkage form conflicts with that header).
// AnimGame_SetStateFlags advances the view's animation model; attachAnimObj
// attaches a model list + state with a vtable-query parameter;
// getAnimModelId returns the C4 target's page id.
extern "C" void AnimGame_SetStateFlags(void* self);
extern "C" void attachAnimObj(void* self, void* object, void* state, u32 param);
extern "C" u32 getAnimModelId(void* self);
// Heap query + MemManager allocate used by CfObjectMove_ensureAnimTargets's +0xC8 target
// construction (defined in CfRes.cpp / monolib; same declarations as
// CfCamEvent.hpp, which is not included here).
extern "C" u32 CfRes_getHeapHandle();
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);
// +0xC8 target ctor (retail 0x8005A3FC, defined in
// kyoshin/cf/CActParamAnimGame.cpp): CfObjectMove_ensureAnimTargets constructs the target
// with the owning CfObjectMove as the parent.
extern "C" void __ct__8005A3FC(void* self, void* parent);
// Model sub-object helper used by CfObjectMove_ensureAnimTargets's tail (defined in the
// monolib scene library, same family as simSetFlag2OnTree in
// CfObjectModel.hpp).
extern "C" void func_80482918(cf::CfObjectModelSub98* model, int flag);
// CModelDisp release helper (defined in kyoshin/makecrystal/
// CModelDispMakeCrystal.cpp, retail unmangled name). Called by
// CfObjectMove_detachModelList for each +0xC8/+0xCC target while the
// +0x6D8 model list is present.
extern "C" void releaseAnimObj(void* self, void* obj);
// CActParamAnim field setter (defined in kyoshin/action/CActParamAnim.cpp,
// retail unmangled name; the retail body is stw r4,0x8(r3); blr). C
// linkage keeps the call-site reloc at the plain retail name.
extern "C" void setAnimOwner(void* self, void* value);// Battle-status setter (retail unmangled C symbol; canonical 5-arg form
// matching CfObjectActor.hpp / CtrlNpc.hpp / all call sites; the retail
// body only consumes r3-r6). Declared here so the CfObjectMove.cpp
// definition inherits C linkage without a local extern "C".
extern "C" void CfObjectMove_setAnimModeArgs(u8* obj, int a, int b, int c, int d);
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
extern "C" void CfObject_setMoveHeadAngle__Q22cf13CfObjectModelFv(cf::CfObjectModel* self);
// Base +0xA0 forced-name implementation (defined in CfObjectModel.cpp as a
// plain global taking the hidden float arg).
extern "C" void CfObject_UnkVirtualFunc70__Q22cf13CfObjectModelFv(cf::CfObjectModel* self, float value);

// Call-site aliases onto already-real virtual names: the Model/Object base
// impls keep their UnkVirtual Fv linker symbols (hand-built vtables), but
// Move call sites spell the recovered slot names. Same arity; MWCC inlines.
inline void CfObjectModel_syncMoveTarget(cf::CfObjectModel* self, const ml::CVec3* vec) {
    CfObject_UnkVirtualFunc22__Q22cf13CfObjectModelFv(self, vec);
}
inline void CfObject_snapMoveTargetBase(cf::CfObject* self, const ml::CVec3* vec) {
    CfObject_UnkVirtualFunc25__Q22cf8CfObjectFv(self, vec);
}
inline void CfObjectModel_setPointEnabled(cf::CfObjectModel* self, int flag) {
    CfObject_UnkVirtualFunc66__Q22cf13CfObjectModelFv(self, flag);
}
inline void CfObjectModel_syncModelRate(cf::CfObjectModel* self, float value) {
    CfObject_UnkVirtualFunc70__Q22cf13CfObjectModelFv(self, value);
}

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
        virtual void CfObjectMove_dropMoveRequest();  //0x1C8
        virtual void CfObjectMove_detachModelList();  //0x1CC
        virtual void CfObjectMove_releaseSlotById(int arg);  //0x1D0
        virtual void CfObjectMove_recordMoveValue(float value);  //0x1D4
        virtual void CfObjectMove_getMovementRate();  //0x1D8
        // Wave-61: Unk primary flipped to behavior spelling. Fv linker body
        // CfObjectMove_UnkVirtualFunc6__Q22cf12CfObjectMoveFv (CtrlNpc.cpp)
        // and hand-built tables keep the Unk mangled name; Unk alias below.
        virtual void CfObjectMove_setTargetC4Arg(u32 arg);  //0x1DC
        // Retail ocUnit call sites leave args live into these bctrs (walkR
        // f1, CED0 mullw r4, CF48 stack vec r4, D060 r4-r7/f1, D570 r4,
        // D5DC r4-r6); the impls ignore them (CtrlNpc tails / direct
        // stores). Decls carry the retail arity; impls stay Fv-named free
        // functions with hidden args (recordMoveValue precedent).
        virtual void CfObjectMove_refreshNpcState(int modeA, int modeB, int count, const ml::CVec3* start, double angle);  //0x1E0
        // Retail ocUnit moveTo leaves r4/f1/r5 live into this bctr (vec,
        // frames*secPerFrame step, booleanized flag); the impl ignores them
        // (refreshNpcState precedent). Decl carries the retail arity; impl
        // stays an Fv-named free function with hidden args.
        virtual void CfObjectMove_updateNpcTarget(const ml::CVec3* vec, float step, int flag, int angle);  //0x1E4
        virtual int CfObjectMove_queryNpcAdvance();   //0x1E8 (retail returns int: 1 or the CtrlNpc action-advance query)
        // Wave-61: Unk primary flipped. Fv body
        // CfObjectMove_UnkVirtualFunc10__Q22cf12CfObjectMoveFv keeps Unk name.
        virtual void CfObjectMove_setNpcMoveRange(float value); //0x1EC
        // Wave-61 Unk aliases (non-virtual, inline): legacy Unk spellings for
        // the three Move slots renamed above. Same arity; MWCC inlines each
        // into the identical virtual dispatch.
        void CfObjectMove_UnkVirtualFunc10(float value) { CfObjectMove_setNpcMoveRange(value); } //0x1EC
        void CfObjectMove_UnkVirtualFunc6(u32 arg) { CfObjectMove_setTargetC4Arg(arg); } //0x1DC
        virtual void CfObjectMove_writeTargetField(int value); //0x1F0
        virtual void CfObjectMove_advanceNpcTarget(const ml::CVec3* vec); //0x1F4
        virtual void CfObjectMove_commandNpcTarget(int arg); //0x1F8
        virtual void CfObjectMove_restartNpcTarget(int a, int b, int c); //0x1FC
        // Wave-61: Unk primary flipped. Fv body
        // CfObjectMove_UnkVirtualFunc15__Q22cf12CfObjectMoveFv (code_800B06A4)
        // keeps the Unk mangled name; Unk alias below.
        virtual int CfObjectMove_hasActorScale(); //0x200 (retail returns int: tested != 0 in syncReloadObjectNameScale)
        int CfObjectMove_UnkVirtualFunc15() { return CfObjectMove_hasActorScale(); } //0x200
        virtual void CfObjectMove_attachEffectSlot(u32 a, u32 b, u32 c, u32 d, u32 e); //0x204
        virtual void CfObjectMove_setEffectSlotBit(u32 arg); //0x208
        virtual void CfObjectMove_transferSlotBits(u32 flag); //0x20C
        virtual int CfObjectMove_isEffectSlotUsed(u32 flag); //0x210
        virtual void CfObjectMove_signalEffectSlot(u32 arg); //0x214
        virtual void CfObjectMove_flushEffectSlots(); //0x218
        virtual void CfObjectMove_setAllEffectBits(); //0x21C
        virtual void* CfObjectMove_loadResourceById(u32 id); //0x220: retail 0x800BF1C4 takes id in r4, returns loader result (0 when mField6DC/mField6E0 NULL); forced-name impl CfObjectMove_loadResourceById__Q22cf12CfObjectMoveFv

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
        u32 mField6C4;           // 0x6C4-0x6C7 (CfObjectMove_resetAnimModeArgs stores 1)
        s8 mField6C8;            // 0x6C8 (CfObjectMove_resetAnimModeArgs stores -1)
        u8 mFlags6C9;             // 0x6C9
        s16 _6CA;               // 0x6CA-0x6CB (ctor stores -1)
        u16 field_6CC;          // 0x6CC-0x6CD (u16 read by CfObjectMove_setModelDisplayFlag)
        s8 field_6CE;           // 0x6CE
        s8 field_6CF;            // 0x6CF
        s16 _6D0;               // 0x6D0-0x6D1 (ctor stores 0)
        u8 _6D2[2];             // 0x6D2-0x6D3
        void* mField6D4;         // 0x6D4-0x6D7 (model list released by CfObjectMove_setModelListLock)
        void* mField6D8;         // 0x6D8-0x6DB (CfObjectMove_detachModelList model list)
        void* mField6DC;         // 0x6DC-0x6DF (CfObjectMove_loadResourceById)
        void* mField6E0;         // 0x6E0-0x6E3 (CfObjectMove_loadResourceById)
        float mField6E4;          // 0x6E4-0x6E7 (CfObject_UnkVirtualFunc57 stores f1)
        float mField6E8;          // 0x6E8-0x6EB (CfObjectMove_recordMoveValue)
        float mField6EC;          // 0x6EC-0x6EF (ctor stores lbl_eu_80666A88)
        float mMoveSpeed;         // 0x6F0-0x6F3
        u8 _6F4;                 // 0x6F4 (ctor stores 0)
        u8 _6F5[3];             // 0x6F5-0x6F7
        void* mField6F8[2];      // 0x6F8-0x6FF (slots walked by CfObjectModel_UnkVirtualFunc19)
        u32 mField700;           // 0x700-0x703 (ctor stores 0)
        void* mField704;         // 0x704-0x707 (CfObjectMove_detachModelList clears with 0x6D8)
        u32 mField708;           // 0x708-0x70B (ctor stores 0)
        u16 field_70C[2];        // 0x70C-0x70F (bdat id array written by CfObject_UnkVirtualFunc45)
        u16 field_710[2];        // 0x710-0x713 (bdat index array read by CfObjectMove_getBdatNameCol7 / CfObjectMove_getBdatNameCol11)
        u8 _714;                 // 0x714
        u8 unk715[3];            // 0x715-0x717
        float mField718;         // 0x718-0x71B (ctor stores lbl_eu_80666A88)
    // Wave-63: Fv bodies keep Unk linker names (lbl_eu_80529690). Do NOT
    // redeclare base behavior aliases (releaseModelSub/setModelVisible/...)
    // as Move members - that hides the base inline and breaks UVF6. Do NOT
    // declare C++ members whose mangled names collide with the Unk Fv bodies.
    // Wave-62: Unk primaries flipped on CfObject; Fv bodies keep Unk names.
    void CfObject_updateMoveRate();
    void CfObject_releaseMoveTargets();
    // Wave-61: CfObjectModel_UnkVirtualFunc2 override folded onto base
    // CfObjectModel_releaseModelList (+0x17C). Body remains the Fv free
    // function CfObjectModel_UnkVirtualFunc2__Q22cf12CfObjectMoveFv.
    void CfObject_setMoveTargetPtr(void* arg);
    void CfObject_setMoveBusyState(int flag);
    ml::CVec3* CfObject_getPosVector();
    void CfObject_setRotVec(void* src);
    void CfObject_setMoveYaw(float value);
    // Unk* below are non-colliding aliases onto base behavior names (base
    // still owns the Unk virtual / Fv linker symbol for these slots).
    void CfObject_UnkVirtualFunc33(float amount) { CfObject_applyMoveYaw(amount); }
    int CfObject_queryTargetState();
    void CfObject_UnkVirtualFunc57(float value) { CfObject_setMoveValue(value); }
    void* CfObject_pushRefreshValue(float value);
    float CfObject_getMoveSpeedRate();
    bool CfObject_isMoveActiveNow();
    void CfObject_setAnimSlotEntry(u32 a, u32 b);
    void CfObject_refreshSubB0();
    // Pure-virtual completions for CfObject base (W63 Model dropped these
    // as "vestigial"; Pc View embeds need Move non-abstract). Bodies are the
    // existing Fv free functions with matching mangled names.
    void CfObject_UnkVirtualFunc4(); //0x60
    void CfObject_UnkVirtualFunc7(); //0x6C
    void setPointEnabled(int flag); //0x158 override (base renamed; keeps CfObjectMove non-abstract)
    void CfObject_setMoveHeadAngle(float value);
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
    int isActive();
    void setBit6c9(unsigned long bit);
    cf::CfObjectMove* testFlag8();
    };
    // vtable proxy for calling cf-chain vtable slot +0xC8 as a float-taking
    // virtual (retail CfObject_UnkVirtualFunc33 tail-calls it with a scaled
    // value; the base header declares the slot void). Extends the +0xB4
    // proxy; dummy slots pin the offset.
    // Layout of the object behind CfObjectMove::mTargetC4 (fields written by
    // CfObject_UnkVirtualFunc4 / 14 / 57 and read by 15 / 17).
    struct CfObjectMoveTargetC4 {
        u8 _pad000[0x388];      // 0x00-0x387
        float field_388;        // 0x388 (CfObject_pushRefreshValue write / 15 read)
        float field_38C;        // 0x38C (CfObject_readRefreshValue read / 16 write)
        u8 _pad390[0x3A8 - 0x390];  // 0x390-0x3A7
        float field_3A8;        // 0x3A8 (CfObject_getPosVector returns &field_3A8)
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
    // View of the CtrlNpc movement-target fields CfObject_forwardNpcAction
    // reads (the +0xC0 action-id word). CtrlNpc.hpp is not included here:
    // its CfObjectMove_setAnimModeArgs 5-arg declaration conflicts with the 4-arg form
    // this unit needs, so a local view is used (same scheme as
    // CfObjectMoveTargetC4).
    struct CfObjectMoveNpcView {
        u8 _pad[0xC0];       // 0x00-0xBF
        u16 field_C0;        // 0xC0 action id
    };
    // vtable proxy for the CfObjectMove mTargetC4 object's slot +0x80: retail
    // CfObject_queryTargetState tail-calls it with a 0 arg and returns its int
    // result (the null path returns 1). Dummy slots pin the offset.
    // vtable proxy for the CfObjectModel+0x98 sub-object's slot +0x64 with an
    // explicit int arg: retail CfObjectMove_setMoveModeField forwards a 4-bit flag extracted
    // from mFlags6C9 (the base CfObjectModelSub98vt proxy declares the slot
    // no-arg). Dummy slots pin the offset.
    // vtable proxy for calling the CfObjectMove slot +0x108
    // (CfObject_setMoveTargetPtr) with an explicit pointer arg (retail
    // CfObject_createMoveTarget dispatches the newly constructed target
    // through it; the base header declares the slot Fv). Dummy slots pin
    // the offset (same scheme as CfObjectMoveC4vt80).
    // vtable proxy for calling the CfObjectMove slot +0x208 as an int-taking
    // virtual (retail CfObjectMove_setAllEffectBits loops a counter through
    // it; the base header declares the slot Fv). Inherits CfObjectModel so
    // the new virtuals land in the CfObjectMove vtable range; the dummy slots
    // pin 0x1C8..0x204 and m208 lands at +0x208.
    // vtable proxy for calling a CfObjectMove vtable slot +0x144 with two
    // explicit args (retail CfObject_UnkVirtualFunc39/42 dispatch the bdat
    // result through it; the base header declares CfObject_setAnimSlotEntry
    // no-arg). Extends the cf-chain sub-object proxy (CObjectParam root,
    // last slot +0xA8); dummy slots pin +0xAC..+0x140.
    // View of the CfObjectModel field_0x90 area: retail
    // CfObject_UnkVirtualFunc38 clears the +0x94 word. Cast on `self` with
    // the full offset pad, so field_94 lands at 0x94 (same scheme as
    // CfObjectMoveC4Flags).
    struct CfObjectMove90View {
        u8 _pad90[0x90];     // 0x00-0x8F
        u32 field_90;        // 0x90-0x93
        u32 field_94;        // 0x94-0x97
    };
    // View of the CfObject base word at +0x6C (CfObjectMove_setModelListLock toggles bit 16;
    // CfObject.hpp exposes 0x6C-0x6F as opaque padding).
    struct CfObjectMoveFlags6C {
        u8 _pad[0x6C];
        u32 field_6C;  // 0x6C
    };
    // View of the CfObjectMove +0xA4/+0xA8/+0xAC floats (inside
    // CfObjectModel's opaque 0xA4-0xAF pad): CfObjectMove_updateSpeedA8 reads/writes
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
    // it emits the retail rlwinm r0,r0,0,27,23 that CfObjectMove_setAnimModeArgs keeps (a
    // plain byte AND folds the mask to 27,31 via range analysis). mid4 spans
    // word bits 27-30 and b0 is word bit 31: the ctor's flag init assigns
    // mid4 = 10 and b0 = 0, which MWCC decomposes into the retail
    // li r3,5; rlwimi r0,r3,1,27,30; clrrwi r0,r0,1 sequence.
    struct CfFlags6C9 {
        u8 top3 : 3;   // word bits 24-26
        u8 mid4 : 4;   // word bits 27-30
        u8 b0 : 1;     // word bit 31
    };
    // Layout of the +0xC8/+0xCC target objects CfObjectMove_ensureAnimTargets constructs and
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
    // ObjReal view in CfObjectMove.cpp).
    // vtable proxy for the +0xB0 sub-object's slot +0x4C with an explicit
    // arg (retail CfObject_UnkVirtualFunc38 forwards its hidden r4; the
    // file-scope ObjReal view declares the slot no-arg, so this typed slot
    // is a separate class). Dummy slots pin the offset.
    // vtable proxy for the +0xB0 sub-object's slot +0x3C with two explicit
    // args (retail CfObject_UnkVirtualFunc39/42 forward the bdat result and
    // a 0/1 flag; the file-scope ObjReal view declares the slot no-arg).
    // Dummy slots pin the offset (same scheme as CfObjectMoveSubB0Real4C).
    // View of the CfObjectMove flags word at +0x68 (CfObject_UnkVirtualFunc5
    // clears bits through this separate view type so MWCC's alias analysis
    // treats the clamp-path clear as a fresh read - retail reloads mFlags68
    // (lwz r0,0x68) at each clamp-path clear).
    struct CfObjectMoveFlags68View {
        u8 _pad[0x68];
        u32 flags68;  // 0x68
    };
    // vtable proxy for the +0x98 sub-object's slot +0x48 with an explicit
    // float arg (retail func_800BC8D8 forwards the movement value to the
    // slot; the CObjectParam base header declares it no-arg). Dummy slots
    // pin the offset (same scheme as CfObjectMoveSub98vt64).
    // vtable proxy for the +0x98 sub-object's slot +0x50 with an explicit
    // int arg (retail CfObjectMove_setModelDisplayFlag forwards its flag to the slot; the
    // CObjectParam base header declares it no-arg). Dummy slots pin the
    // offset (same scheme as CfObjectMoveSub98vt64).
    // vtable proxy for the +0x98 sub-object's slots +0xC4/+0xC8 (the sub's
    // retail vtable is the CModelDisp family layout): CfObject_UnkVirtualFunc45
    // stops the slot's animation model (mC8, one arg) then re-arms it with
    // the name and a 0 flag (mC4). Extends CfObjectModelSub98vt (pins +0x54
    // .. +0xA8); dummy slots pin +0xAC..+0xC0.
    // vtable view of the CfObject-family sub-object at CfObjectModel+0x38
    // (mSubObj38): slots 0x8 (release-with-delete-flag), 0x2C (attach parent),
    // 0xA0 (activate) and 0xAC (release) are called by
    // CfObject_UnkVirtualFunc6 / CfObject_notifyEventDone. Dummy slots pin
    // the offsets (MWCC puts the Nth declared virtual at (N+1)*4).
    // vtable proxy for the mSubObj38 object's slot +0xE4 (an int-returning
    // query): retail CfObjectMove_flushEffectSlots calls it, tests the
    // result, then re-calls it. CfObject +0xAC getPosVector; former Sub38 pad deleted
    // slots pin the range up to +0xE4.
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
    // View of the +0xC4 target's flag word (CfObject_UnkVirtualFunc27 ORs in
    // the 0x20 bit when the incoming position moved on Z only).
    struct CfObjectMoveC4FlagsView {
        u8 _pad[0xC];           // 0x00-0x0B
        u32 flags;              // 0x0C
    };
    // View of the +0x6B4 word inside the +0x60C region (CfObjectMove_testMoveProximity toggles
    // bit 12 around the ColiCheckMoveRadius call).
    struct CfObjectMove6B4View {
        u8 _pad[0x6B4];         // 0x00-0x6B3
        u32 field_6B4;          // 0x6B4
    };
    // View of the scene object Scn_FindCamItem returns: its +0x10C position
    // vector is read by CfObjectMove_testMoveProximity (the ColiCheckMoveRadius target).
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
    // View of the getEffOwner result's +0x45CA u16 (func_800BC9EC tests its
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
    // +0xB0 sub-object: cf::CfResObjImpl (vptr at +0x10).
    // getCameraDataBlock result: cf::CfCam::cfCam_loadUnk164 (+0x60).
    // +0x7EC target: CActParam7ECTarget::func08 (u32, float).

}
