// (CfObjectMove_setAnimModeArgs now has a single unified decl on CfObjectMove.hpp; only one
// form exists, so no pre-include rename is needed here.)
// (single-owner-decl cleanup: all six pre-include renames are gone.
// getArtsSlotRC / getArtsParamRC2 resolve via the single owner decl pair on
// chain/CChainActorList.hpp (CAIAction.hpp's foreign copies removed);
// CtrlObjectParam_GetCharEntry resolves via the aligned u32 owner-form decls (CfObjectPc.hpp
// / CVision.hpp; defined in CtrlObjectParam.cpp); getTableValueByPair__Q2..Fv and
// IdTable_SumValues share aligned extern "C" decls between CChainActorList.hpp /
// CfGameManagerUnityHelpers.hpp / CChainTimer.hpp; ml::pi keeps its
// owner decl on action/CActParamAnim.hpp, with this TU's copies aligned to
// CChain.hpp's const form (const is load-bearing for chain-TU scheduling).)
#include "kyoshin/cf/object/CAIAction.hpp"
#include "kyoshin/menu/code_8018C5FC.hpp"
#include "kyoshin/code_8025FB10.hpp"
#include "kyoshin/cf/chain/CChainActorList.hpp"
#include "kyoshin/cf/chain/CChain.hpp"
// ml::pi: CChain.hpp's const f32 form is kept (load-bearing for
// chain-TU scheduling); CfObjectEne.hpp and this TU's local decl are aligned
// to it, so no pre-include rename is needed.
#include "kyoshin/cf/CBattleManagerImports.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include "kyoshin/code_8028130C.hpp" // SysWinLog_PollBattleStart (C++ linkage)
#include "monolib/scn/CScn.hpp"
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"
#include "kyoshin/cf/CPcEffect07.hpp"
#include "monolib/math/CVec3.hpp"
#include "kyoshin/cf/CfCamSymbols.hpp"
// BMIf removed: CVision real vt used via mVision.
#include "kyoshin/UnkClass_805764CC.hpp"
#include "kyoshin/cf/object/CfObjectPc.hpp"
// (CfModel_SyncVisFlag/CfObjectMove_initAnimTarget/getUnk80664658/BasicSound + the C-ABI import
// names below now resolve via their single owner-header decls; only the
// other agents' guards (lbl_eu_*) remain.)
#include "kyoshin/cf/object/CfObjectEne.hpp"
// (getCameraDataBlock__Q22cf13CfGameManagerFv: CfSoundMan.hpp now carries the
// single winning decl from CfGameManagerApi.hpp - no pre-include rename.)
#include "kyoshin/cf/CfSoundMan.hpp"
// The include/kyoshin/cf/CfGameManager.hpp C-ABI import tail conflicts with
// the cfsys/chain walker decls already visible in this TU for the few names
// it shares; guard only those (call sites bind to the global-scope decls,
// same retail symbol names). (Scn_GetFrameDelta now has a single unified decl.)
// (updateBattleManager: CfGameManager.hpp's copy is gone; single decl on
// kyoshin/cf/CBattleManager.hpp, included via CBattleManager.hpp above.)
// (getEffOwner: CfObjectMove.hpp and CfGameManager.hpp now carry the
// identical void*(void*) C++-linkage form - no guard needed.)
#include "kyoshin/cf/CfGameManager.hpp"
// (UIWin_BuildFlagBuf is owned by kyoshin/CUIWindowManager.hpp; single decl.)
#include "monolib/work.hpp"
#include "kyoshin/cf/code_800F42AC.hpp"
#include "kyoshin/CfEnumListHolder.hpp"
#include "kyoshin/cf/CfObjectEnumList.hpp"
#include "kyoshin/CUICfManager.hpp"
#include "kyoshin/cf/CSuddenCommu.hpp"

// Retail C++ symbol getEffOwner____FPv (0x800AE12C); the plain C++ name
// getEffOwner__ mangles to it (same declaration as CChainActorEne.cpp).
void* getEffOwner__(void* obj);

// Arts-data row returned by CtrlObjectParam_GetCharEntry: the per-entry table scanned by
// scanArtsEntries starts at +0x1C (passed to CtrlObjectParam_GetArtsStatsRow). (CtrlObjectParam_GetCharEntry
// itself resolves via the aligned u32 owner-form decls: CfObjectPc.hpp /
// CVision.hpp; defined in CtrlObjectParam.cpp - no local copy needed.)
extern void CtrlObjectParam_GetArtsStatsRow(UNKTYPE* base, u32 idx);

// Status add/remove helper (retail CfObjectMove_setAnimModeArgs). The shared headers carry
// two incompatible extern "C" forms ((u8*,...) and (void*,...)); this TU uses
// the u8* form throughout, declared once here.

// Game state check function (name already contains __Fi; extern "C" prevents
// MWCC from double-mangling it to isGlobalCamFlagSet__Fi__Fi).


// Initialise all vision slots/effects (retail initVisionSlots). Free function,
// not a CVision member - declared here because CVision.hpp stays opaque.
// C linkage: the retail symbols for these reset helpers are unmangled, and
// they are called as free functions (not member calls) so the call relocs
// keep the retail names.
extern "C" void initVisionSlots(cf::CVision* self);
extern "C" void __ct__80192C10(cf::UnkClass_80192BF4* self);
extern "C" void ChainTmr_Reset_D1A4(cf::UnkClass_800D8DBC* self);
extern "C" void SuddenCommuResetPairingState(cf::CSuddenCommu* self);
// Generic-object holder register/deregister (retail unmangled: the static
// member UnkClass_805764CC::getInstance would mangle to
// getInstance__17UnkClass_805764CCFv; the Fv-suffix here is the retail
// symbol as-is).
extern "C" UnkClass_805764CC* getInstance__Fv();
extern "C" void registerFactoryEvent__FPvPQ22cf13IFactoryEvent(UnkClass_805764CC* holder, cf::IFactoryEvent* ev);
extern "C" void unlinkFactoryById(UnkClass_805764CC* holder, cf::IFactoryEvent* ev);
// Cross-actor id lookup used by func_800F3970's dispatch table (/100 scale).
// (getTableValueByPair__Q2..Fv resolves via the single extern "C" s32 owner decl on
// chain/CChainActorList.hpp, included above - no local copy needed.)

// (Former CBmVFn2C4/CBmVFn2BC manual-slot typedefs removed: slot 0x2C4
// is CfObjectActor::CActorParam_addHate (override form) and slot
// 0x2BC is CActorParam::CActorParam_isBattleLocked; call sites use the
// real virtuals directly.)
// (Former CBmBS27Fn manual-slot typedef removed: slot 0x70 on the
// +0x3ED4 holder is CfObject::CfObject_notifyEventDone; call sites use the
// real virtual directly.)

#include "kyoshin/cf/CtrlActApi.hpp"

extern "C" {
extern const u8 __RTTI__Q22cf7CtrlAct[];
extern void __dt__Q22cf7CtrlActFv();
extern void CtrlNpcAlwaysFalse();
extern void parseActEntry();
extern void aimVoiceOwner();
extern void updateActMain();
extern void runArtsAttack();
extern void handleActKind();
extern void CtrlEnemy_ret0_800888E8();
extern void CtrlEnemy_ret0_800888E0();
extern void CtrlEnemy_ret0_800888D8();
extern void CtrlEnemy_ret0_800888D0();
extern void CtrlEnemy_ret0_800888C8();
extern void CtrlEnemy_ret0_800888C0();
extern void CtrlEnemy_ret0_800888B8();
extern void CtrlEnemy_ret0_800888B0();
extern void CtrlEnemy_ret0_800888A8();
extern void CtrlEnemy_ret0_800888A0();
extern void CtrlEnemy_ret0_80088898();
extern void CtrlEnemy_ret0_80088890();
extern void CtrlEnemy_ret0_80088888();
extern void CtrlEnemy_ret0_80088880();
extern void CtrlEnemy_ret0_80088878();
extern void CtrlEnemy_ret0_80088870();
extern void CtrlEnemy_ret1_800888F4();
extern void CtrlEnemy_emptyStub_800888F0();
extern void runVisionResetWindows();
extern void runVisionTierWindows();
extern void gateVisionByFusionParam();
}
// Retail .data head as typed defs (MWCC emits file-scope .data in declaration
// order ahead of function switch tables, so these land at retail 0x0/0x90).
// lbl_eu_8052B080 (0x90 = 36 words) is cf::CtrlAct's vtable: word +0 is RTTI,
// +4 the offset-to-top and +8 the dtor; +0x0C/+0x8C are the pure virtuals
// named in CtrlAct.hpp. The 16 return-0 stubs at +0x18..+0x54 have only
// placeholder names in symbols.txt (searched us/eu/jp, RTTI and strings).
// lbl_eu_8052B110 (0xC) is the default-handler PMF triplet
// {delta=0, vindex=-1, func} copied wholesale by initBattleActorState.
__attribute__((aligned(8))) __attribute__((used)) const void* lbl_eu_8052B080[36] = {
    (const void*)__RTTI__Q22cf7CtrlAct,
    (const void*)0,
    (const void*)__dt__Q22cf7CtrlActFv,
    (const void*)0,
    (const void*)CtrlNpcAlwaysFalse,
    (const void*)ctrlActIsFlagBit15Clear,
    (const void*)CtrlEnemy_ret0_800888E8,
    (const void*)CtrlEnemy_ret0_800888E0,
    (const void*)CtrlEnemy_ret0_800888D8,
    (const void*)CtrlEnemy_ret0_800888D0,
    (const void*)CtrlEnemy_ret0_800888C8,
    (const void*)CtrlEnemy_ret0_800888C0,
    (const void*)CtrlEnemy_ret0_800888B8,
    (const void*)CtrlEnemy_ret0_800888B0,
    (const void*)CtrlEnemy_ret0_800888A8,
    (const void*)CtrlEnemy_ret0_800888A0,
    (const void*)CtrlEnemy_ret0_80088898,
    (const void*)CtrlEnemy_ret0_80088890,
    (const void*)CtrlEnemy_ret0_80088888,
    (const void*)CtrlEnemy_ret0_80088880,
    (const void*)CtrlEnemy_ret0_80088878,
    (const void*)CtrlEnemy_ret0_80088870,
    (const void*)parseActEntry,
    (const void*)aimVoiceOwner,
    (const void*)updateActMain,
    (const void*)ctrlActComputeAimVec,
    (const void*)ctrlActAimAtTarget,
    (const void*)ctrlActFaceTargetVec,
    (const void*)ctrlActProbeTargetReset,
    (const void*)ctrlActCheckBlockedProbe,
    (const void*)ctrlActDispatchEntry,
    (const void*)runArtsAttack,
    (const void*)CtrlEnemy_ret1_800888F4,
    (const void*)CtrlEnemy_emptyStub_800888F0,
    (const void*)handleActKind,
    (const void*)0
};
__attribute__((used)) const void* lbl_eu_8052B110[3] = {
    (const void*)0,
    (const void*)-1,
    (const void*)runVisionResetWindows
};


// ============================================================
// Retail .sdata2 pool (0xC0, lbl_eu_80666DD0..lbl_eu_80666E8C) as typed
// definitions in retail order. Non-const to match the existing
// `extern "C" f32/f64` declarations (header + body); the explicit
// section attribute keeps them in .sdata2. Code already references
// most of these by name; the rest (E34/E38/E3C/E40/E48/E50/E88)
// back the float/double literals MWCC would otherwise pool anonymously.
extern "C" {
__declspec(section ".sdata2") __attribute__((used)) float lbl_eu_80666DD0 = 0.6f;
__declspec(section ".sdata2") __attribute__((used)) float lbl_eu_80666DD4 = 1.0f;
// The zero constant at +0xC occupies the alignment gap before the double.
__declspec(section ".sdata2") __attribute__((used)) double lbl_eu_80666DE0 = 4503601774854144.0;
__attribute__((used)) const float lbl_eu_80666DE8 = 0.5f;
__declspec(section ".sdata2") __attribute__((used)) double lbl_eu_80666E08 = 4503599627370496.0;
__declspec(section ".sdata2") __attribute__((used)) float lbl_eu_80666E10 = 0.024543693f;
}

// ============================================================
// Retail .sdata pointer tables (6 pairs, 0x30, __RTTI__Q22cf15CfObjectImplEne..78).
// Cross-TU members stay extern (804FC7F8/8052AEC8 live in other objects);
// the reslist name strings are defined with .rodata below. The three .data
// tables (BD20/BD38/BD5C) have no decomp emitter (reslist.hpp gap: MWCC
// emits only the vtables/RTTI here), so those three members stay UNDEF
// extern refs by retail name; data-diff ignores relocs once bytes match.
// ============================================================
// BA0-region note: an earlier revision split the 34 content bytes into
// sized pieces to dodge an MWCC pad quirk; unsized arrays pack correctly
// so the retail names stand directly (sdata below references them).
extern const char lbl_eu_804FCB70[];
extern const char lbl_eu_804FCB84[];
extern const char lbl_eu_804FCBA0[];
extern const char lbl_eu_804FCBC4[];
extern const char lbl_eu_804FCBE4[];
extern "C" u8 lbl_eu_804FC7F8[];
extern "C" u8 lbl_eu_8052AEC8[];
extern "C" u8 lbl_eu_8052BD20[];
extern "C" u8 lbl_eu_8052BD38[];
extern "C" u8 lbl_eu_8052BD5C[];
__attribute__((used)) const void* lbl_eu_80661D60[2] = { lbl_eu_804FCB84, lbl_eu_8052BD38 };
__attribute__((used)) const void* lbl_eu_80661D68[2] = { lbl_eu_804FCBA0, 0 };
__attribute__((used)) const void* lbl_eu_80661D70[2] = { lbl_eu_804FCBC4, lbl_eu_8052BD5C };
__attribute__((used)) const void* lbl_eu_80661D78[2] = { lbl_eu_804FCBE4, 0 };

// ============================================================
// Retail .rodata (sBattleEventTable..lbl_eu_804FCBE4) as typed definitions
// in retail order. Tables are word-exact transcriptions of retail;
// comments note the float/string views code uses. Strings are unsized
// arrays: MWCC 4-packs string starts, reproducing the retail gaps exactly
// (explicit sizes take a stricter path that pads after >32-byte runs).
// first table carries aligned(8) (section alignment); the rest pack
// tightly like retail. Linkage matches the existing declarations:
// 804FC828/804FCA3C/804FCAE4 are `extern "C"`; the rest are C++ globals.
// ============================================================
extern "C" {
// 804FC828: 40 battle-event entries
// and entry 30 at +0x168).
__declspec(section ".rodata") __attribute__((aligned(8)))
BattleTableEntry sBattleEventTable[40] = {
    {0x00000003u, 100, 0, 0x00, 0x00000032u},
    {0x00000003u, 50, 0, 0x00, 0x00000032u},
    {0x00000003u, 50, 0, 0x01, 0x00000000u},
    {0x00000000u, 5, 0, 0x00, 0x00000000u},
    {0x00000000u, 10, 0, 0x00, 0x00000000u},
    {0x00000001u, 0, 5, 0x01, 0x00000000u},
    {0x00000001u, 25, 5, 0x01, 0x00000000u},
    {0x00000000u, 50, 0, 0x00, 0x00000000u},
    {0x00000000u, 5, 0, 0x00, 0x00000000u},
    {0x00000000u, -25, 0, 0x00, 0x00000000u},
    {0x00000000u, -10, 0, 0x00, 0x00000000u},
    {0x00000000u, 0, 0, 0x00, 0x00000000u},
    {0x00000000u, -100, 0, 0x00, 0x00000000u},
    {0x00000000u, -200, 0, 0x00, 0x00000000u},
    {0x00000000u, -100, 0, 0x00, 0x00000000u},
    {0x00000000u, -25, 0, 0x00, 0x00000000u},
    {0x00000000u, -40, 0, 0x00, 0x00000000u},
    {0x00000003u, 999, 0, 0x00, 0x00000000u},
    {0x00000003u, 100, 0, 0x00, 0x00000000u},
    {0x00000003u, 50, 0, 0x00, 0x00000000u},
    {0x00000003u, 25, 0, 0x00, 0x00000000u},
    {0x00000003u, 20, 0, 0x00, 0x00000000u},
    {0x00000000u, 0, 0, 0x00, 0x00000000u},
    {0x3DCCCCCDu, 15948, -52, 0xCD, 0x00000000u},
    {0x0000003Du, 0, 0, 0x3D, 0x0000003Du},
    {0x0000003Du, 0, 0, 0x3E, 0x0000004Du},
    {0x0000003Fu, 0, 0, 0x40, 0x0000004Eu},
    {0x00000040u, 0, 0, 0x40, 0x0000004Fu},
    {0x00000042u, 0, 0, 0x42, 0x00000042u},
    {0x0000004Cu, 0, 0, 0x4C, 0x0000004Cu},
    {0x00000044u, 0, 0, 0x44, 0x00000044u},
    {0x00000046u, 0, 0, 0x46, 0x00000046u},
    {0x0000004Au, 0, 0, 0x4A, 0x0000004Au},
    {0x00000048u, 0, 0, 0x48, 0x00000048u}
};

} // extern "C"
// 804FCA08: 13 x u32 attack sound-id table (code indexes by element type).
__declspec(section ".rodata") __attribute__((used))
u32 sAttackSoundIdTable[13] = {
    0x00000000u,
    0x000001ADu,
    0x000001AAu,
    0x000001B0u,
    0x000001B6u,
    0x000001BAu,
    0x000001B5u,
    0x000001B7u,
    0x000001B8u,
    0x000001B9u,
    0xFFECFFF6u,
    0x0000000Fu,
    0x001E0000u
};

extern "C" {
// 804FCA3C: 6 x s16 (code indexes by actor vfunc result).
extern const s16 sStatusCountTable[6] = {0, 0, 10, 15, 30, 0};

} // extern "C"
// 804FCA48: 36 x u32 rate table (tail words are 8.0f/4.0f/2.0f views).
extern const u32 sBattleEffectParamTable[36] = {
    0x00000000u,
    0x00000100u,
    0x00000020u,
    0x00000100u,
    0x00000020u,
    0x00000100u,
    0x00000000u,
    0x00000020u,
    0x00000100u,
    0x00000000u,
    0x00000064u,
    0x41000000u,
    0x40800000u,
    0x40000000u,
    0x00000000u,
    0x00000064u,
    0x41000000u,
    0x40800000u,
    0x40000000u
};

// 804FCAD8: 3 x u32 (code uses [2] as the int 100; never as float).
extern const u32 sDamageRateThresholds[3] = {0, 0, 100};

extern "C" {
// 804FCAE4: 3 x float rate table {8.0, 4.0, 2.0}.
extern const float sDamageRates[3] = {8.0f, 4.0f, 2.0f};

} // extern "C"
// 804FCAF0: 32 x u32 arts-power table; last 4 words are the
// "cf::CfObjectEff" name string code-adjacent in retail.
__declspec(section ".rodata") __attribute__((used))
u32 sArtsPowerTable[32] = {
    0x00000050u,
    0x0000003Cu,
    0x00000028u,
    0x00000019u,
    0x0000000Au,
    0x00000058u,
    0x00000059u,
    0x0000005Au,
    0x0000005Bu,
    0x0000005Cu,
    0x00000002u,
    0x00000005u,
    0x00000028u,
    0x00000019u,
    0x00000064u,
    0x00000006u,
    0x00000004u,
    0x00000007u,
    0x00000008u,
    0x00000009u,
    0x00000005u,
    0x00000125u,
    0x00000126u,
    0x00000127u,
    0x00000128u,
    0x00000129u,
    0x0000012Au,
    0x63663A3Au,  // "cf::CfObjectEff" (retail-adjacent name string)
    0x43664F62u,
    0x6A656374u,
    0x45666600u
};

// Strings as unsized arrays: MWCC 4-packs string starts, reproducing the
// retail gaps exactly (explicit sizes take a stricter path that pads after
// >32-byte runs). Sizes below are content+NUL: B70=19, B84=28, BA0=34,
// BC4=29, BE4=36.
__attribute__((used))
const char lbl_eu_804FCB70[] = "cf::CBattleManager";
__attribute__((used))
const char lbl_eu_804FCB84[] = "reslist<cf::IBattleEvent *>";
__attribute__((used))
const char lbl_eu_804FCBA0[] = "_reslist_base<cf::IBattleEvent *>";
__attribute__((used))
const char lbl_eu_804FCBC4[] = "reslist<cf::CfObjectActor *>";
__attribute__((used))
const char lbl_eu_804FCBE4[] = "_reslist_base<cf::CfObjectActor *>";

// ============================================================
// Explicit template specializations: _reslist_base / reslist
// destructors for CfObjectActor* and IBattleEvent*
// Must appear before any ODR-use of these templates so they
// override the inline definitions in reslist.hpp.
// ============================================================

// _reslist_base<CfObjectActor*>::~_reslist_base()
// Clears the intrusive linked list, frees the backing buffer if not owned.
// Uses the retail MWCC pattern: old = cur; cur = cur->next; old->next = 0
template <>
_reslist_base<cf::CfObjectActor*>::~_reslist_base() {
    // The virtual destructor itself installs the base-class vtable.
    _reslist_node<cf::CfObjectActor*>* old;
    _reslist_node<cf::CfObjectActor*>* cur = mStartNodePtr->mNext;
    while (cur != mStartNodePtr) {
        old = cur;
        cur = cur->mNext;
        old->mNext = nullptr;
    }
    mStartNodePtr->mNext = mStartNodePtr;
    mStartNodePtr->mPrev = mStartNodePtr;

    if (!unk1C && mList != nullptr) {
        __dla__FPv(mList);
        mList = nullptr;
    }
}

// _reslist_base<IBattleEvent*>::~_reslist_base()
template <>
_reslist_base<cf::IBattleEvent*>::~_reslist_base() {
    // The virtual destructor itself installs the base-class vtable.
    _reslist_node<cf::IBattleEvent*>* old;
    _reslist_node<cf::IBattleEvent*>* cur = mStartNodePtr->mNext;
    while (cur != mStartNodePtr) {
        old = cur;
        cur = cur->mNext;
        old->mNext = nullptr;
    }
    mStartNodePtr->mNext = mStartNodePtr;
    mStartNodePtr->mPrev = mStartNodePtr;

    if (!unk1C && mList != nullptr) {
        __dla__FPv(mList);
        mList = nullptr;
    }
}

// Global battle manager instance pointer (defined later in this file)
// Voice action helper:
void* MapFx_ResolveOwner(void*);

// Attack sound-id table (retail .rodata 0x804FCA08; 10 known entries of the
// 0x34-byte symbol). Defined with the retail name so the lwzx reloc binds.
extern u32 sAttackSoundIdTable[];

// Re-arm every enumerated kind-0x20 object (battle-capable actors) once the
// battle lists empty: re-run setup, refresh the battle status and reset the
// status counter. Shared tail of FactoryEvent2 / unregisterBattleActor.
static inline void resetEnumeratedBattleActors() {
    CfEnumListHolder holder;
    startEnumObjects(holder.get(), 0x20, 0);

    for (u32 i = 0; i < ((cf::CVisionEnumList*)holder.get())->count; i++) {
        cf::CActorParam* actor = (cf::CActorParam*)MapFx_ResolveOwner(getObjectAt(holder.get(), i));
        actor->CActorParam_setupActorState();
        actor->CActorParam_refreshBattleStatus();
        actor->CActorParam_initStatusCounter();
    }

}

namespace cf{
    //Plays attack sound effects when a player character hits an enemy?
    void applyBattleDamage(CfObjectPc* pObjectPc, CfObjectEne* pObjectEne){
        CActorParam_UnkStruct1* param = pObjectPc->CActorParam_getBattleStats();
        BattleObjAccessor* ene = (BattleObjAccessor*)pObjectEne;

        if(param->mFlagsArray[0].flags & CActorParam_UnkStruct1::FLAG_BIT_1){
            CfSoundMan::playActorSound(0, 0x192, 0, 0, lbl_eu_80666DD0);
            CfSoundMan::playActorSound(0, 0x191, 0, 0, lbl_eu_80666DD0);
        }else if(param->mFlagsArray[0].flags & 0x2000){
            CfSoundMan::playActorSound(0, 0x1C5, 0, 0, lbl_eu_80666DD0);
        }else{
            CActorParam_UnkStruct2* sub = param->unk50;

            if(sub != nullptr){
                if(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_24){
                    if((s32)pObjectEne->CActorParam_getActorType() == 1 ||
                    (s32)pObjectEne->CActorParam_getActorType() == 2){
                        if(!(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                            CfSoundMan::playActorSound(0, 0x1B4, 0, 0, lbl_eu_80666DD0);
                            return;
                        }
                    }else if(!(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                        CfSoundMan::playActorSound(0, 0x1B5, 0, 0, lbl_eu_80666DD0);
                        return;
                    }
                }else if((sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_25)
                && !(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_9)){
                    CfSoundMan::playActorSound(0, 0x1B5, 0, 0, lbl_eu_80666DD0);
                    return;
                }
                
                u16 type = sub->unk40;
                if((s32)type == 1){
                    if(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_8){
                        CfSoundMan::playActorSound(0, 0x1AF, 0, 0, lbl_eu_80666DD0);
                    }else{
                        //likely an inline
                        if(pObjectEne != nullptr && (ene->field_3f00 & 0x2)){
                            void* arts = CtrlObjectParam_GetCharEntry(ene->field_3f28);
                            CtrlObjectParam_GetArtsStatsRow((u8*)arts + 0x1c, 1);
                        }

                        if((s32)pObjectEne->CActorParam_getActorType() == 1 ||
                        (s32)pObjectEne->CActorParam_getActorType() == 2){
                            CfSoundMan::playActorSound(0, 0x1AE, 0, 0, lbl_eu_80666DD0);
                        }else{
                            CfSoundMan::playActorSound(0, 0x1AD, 0, 0, lbl_eu_80666DD0);
                        }
                    }
                }else if((s32)type == 2){
                    if(sub->unk78 & CActorParam_UnkStruct1::FLAG_BIT_8){
                        CfSoundMan::playActorSound(0, 0x1AC, 0, 0, lbl_eu_80666DD0);
                    }else{
                        //likely an inline
                        if(pObjectEne != nullptr && (ene->field_3f00 & 0x2)){
                            void* arts = CtrlObjectParam_GetCharEntry(ene->field_3f28);
                            CtrlObjectParam_GetArtsStatsRow((u8*)arts + 0x1c, 1);
                        }

                        if((s32)pObjectEne->CActorParam_getActorType() == 1 ||
                        (s32)pObjectEne->CActorParam_getActorType() == 2){
                            CfSoundMan::playActorSound(0, 0x1AB, 0, 0, lbl_eu_80666DD0);
                        }else{
                            CfSoundMan::playActorSound(0, 0x1AA, 0, 0, lbl_eu_80666DD0);
                        }
                    }
                }else{
                    u32 soundId = sAttackSoundIdTable[type];
                    CfSoundMan::playActorSound(0, soundId, 0, 0, lbl_eu_80666DD0);
                }
            }
        }
    }

    CBattleManager::CBattleManager() : unk84(0) {
        mtl::ALLOC_HANDLE heapIndex = CWorkThreadSystem::getWorkMem();
        mActorList1.reserve(heapIndex, 64);
        mActorList2.reserve(heapIndex, 8);
        mActorList3.reserve(heapIndex, 56);
        mBattleEventList.reserve(heapIndex, 4);
        UnkClass_805764CC* classPtr = getInstance__Fv();
        registerFactoryEvent__FPvPQ22cf13IFactoryEvent(classPtr, (cf::IFactoryEvent*)this);
        // Retail tail of the ctor: zero the trailing f32 timer slot.
        mVision.unk261C4.unk74 = lbl_eu_80666DDC;
    }

    CBattleManager::~CBattleManager(){
        // Deregister from the holder. MWCC emits the vptr store from the
        // virtual destructor declaration (retail symbol
        // __vt__Q22cf14CBattleManager); retail calls getInstance__Fv /
        // unlinkFactoryById with C linkage.
        unlinkFactoryById(getInstance__Fv(), this);
    }

// Static member definition (storage for the singleton pointer)
CBattleManager* cf::CBattleManager::spInstance = nullptr;

CBattleManager* CBattleManager::getInstance() {
    return cf::CBattleManager::spInstance;
}

    void CBattleManager::getBattleStateA(){
        cf::CBattleManager::spInstance = new (mtl::MemManager::getHandleMEM1()) CBattleManager();
    }

    void CBattleManager::getBattleStateB(){
        if(cf::CBattleManager::spInstance != nullptr){
            delete cf::CBattleManager::spInstance;
            cf::CBattleManager::spInstance = nullptr;
        }
    }

    void CBattleManager::resetBattleState(){
        mActorList1.clear();
        mActorList2.clear();
        mActorList3.clear();
        initVisionSlots(&mVision);
        __ct__80192C10(&unk19C);
        mChain.CChain_ResetChainState();
        ChainTmr_Reset_D1A4(&unk20C8);
        SuddenCommuResetPairingState(&mSuddenCommu);
        // Single read of the named lbl_eu_80666DDC global (retail does one lfs from
        // lbl_eu_80666DDC, then reuses f0 for all three stfs). Naming the
        // global (instead of writing 0) keeps the sda21 reloc on the retail
        // symbol rather than an MWCC pool constant @N.
        mSuddenCommu.resetPairTimer();
        unk88 = lbl_eu_80666DDC;
        unk8C = 0;
        unk90 = lbl_eu_80666DDC;
        mVision.unk261C4.unk70 = 0;
        unk94.clear();
        setPartyMaskFlag(2, 0);
    }

    void CBattleManager::FactoryEvent2(CfObject* obj){
        // Removed-object callback from the factory manager; obj->unk64 holds
        // the object's spawn-kind flags.
        if(obj->unk64 & 0x20){
            this->CBattleMan_EmitVisionEvent(__dynamic_cast(obj, 0, &__RTTI__Q22cf11CfObjectEff, &__RTTI__Q22cf8CfObject, 0));
            return;
        }

        // Kinds 0x02 / 0x04 are battle actors: drop them from the lists.
        if(!(obj->unk64 & 0x02) && !(obj->unk64 & 0x04)) return;

        cf::CfObjectActor* target = (cf::CfObjectActor*)MapFx_ResolveOwner(obj);

        // If the target is a member of mActorList1, unregister it everywhere.
        if(mActorList1.find((CfObjectActor*)target) != mActorList1.end()){
            mActorList1.removeAll((CfObjectActor*)target);
            CChain_NotifyStateChange(&mChain, (CfObjectActor*)target);

            if(target->unk64 & 0x02){
                mActorList2.remove((CfObjectActor*)target);
            }else{
                mActorList3.remove((CfObjectActor*)target);
            }

            if(((cf::CBattleManager*)cf::CBattleManager::spInstance)->mActorList3.size() != 0) return;
            CBattleMan_OnActorsEmpty(this);
            if(((cf::CBattleManager*)cf::CBattleManager::spInstance)->mActorList1.size() != 0) return;

            for(int i = 0; i < 3; i++){
                void* player = getPlayer__Q22cf13CfGameManagerFi(i);
                void* action = MapFx_ResolveOwner(player);
                if(action != nullptr){
                    ((cf::CObjectState*)((cf::CActorState*)action)->unk4)->clearStateFlags8(0x400000);
                    ((cf::CObjectState*)((cf::CActorState*)action)->unk4)->clearStateFlags8(0x800000);
                    ((cf::CObjectState*)((cf::CActorState*)action)->unk4)->clearStateFlags8(0x80000000);
                    dispatchItemLoad(action, 0x8000000);
                    dispatchItemLoad(action, 0x10000000);
                }
            }

            setPartyMaskFlag(1, 1);
            // Clear the bit-28 global state flag (rlwinm mask 29..27).
            gmPauseFlags &= ~0x8;
            CChain_ForwardVoiceClear(&mChain);

            resetEnumeratedBattleActors();
        }
    }

}

bool CBattleMan_ListHasValue(void* self, unsigned int value) { struct Node { Node* next; unsigned int unused; unsigned int value; }; struct Manager { unsigned char unused[8]; Node* list; }; Manager* manager = static_cast<Manager*>(self); Node* sentinel = manager->list; Node* current = sentinel->next; while (current != sentinel && current->value != value) current = current->next; return current != sentinel; }
void cf::CBattleManager::CBattleMan_ClearStateMask(u32 mask) {
    unk84 &= ~mask;
}
void* CBattleMan_NextListItem(void* self) {
    SimpleListNode* p = static_cast<Func800EA384_Self*>(self)->listHead;
    if (p->next == p) return nullptr;
    return static_cast<SimpleListNode*>(*reinterpret_cast<void* volatile*>(&static_cast<Func800EA384_Self*>(self)->listHead))->next->data;
}
struct ActorSub3E9CView { u8 _00[0x3e9c]; u8 sub; };
void* CBattleMan_FindNextActorRef(void* self, void* val) {
    if (val == nullptr) return nullptr;

    SimpleListNode* sentinel = static_cast<Func800EA384_Self*>(self)->listHead;
    void* data;
    SimpleListNode* cur = sentinel->next;
    
    goto cond;
    
    loop:
    cur = cur->next;
    
    cond:
    if (cur == sentinel) goto end;
    
    data = cur->data;
    if (data != nullptr) {
        data = &((ActorSub3E9CView*)data)->sub;
    }
    if (data != val) goto loop;
    
    end:
    if (cur == sentinel) goto notfound;
    
    data = (void*)cur->next;
    if ((SimpleListNode*)data == sentinel) goto notfound;
    return ((SimpleListNode*)data)->data;
    
    notfound:
    return nullptr;
}
void cf::CBattleManager::CBattleMan_TickVision() { mVision.func_801A6BCC(); }
void* cf::CBattleManager::CBattleMan_QueryVision() {
    if (cf::CBattleManager::spInstance != nullptr) {
        return ((cf::CVision*)&this->mVision)->func_801A70DC();
    }
    return nullptr;
}
extern "C" void* getNonZeroSlot1(void*);
void* cf::CBattleManager::CBattleMan_FetchVisionObj() {
    return cf::CBattleManager::spInstance ? getNonZeroSlot1(&mVision) : 0;
}
void cf::CBattleManager::CBattleMan_SetTimers(float a, float b, unsigned long c) {
    extern void refreshBattleActors(cf::CBattleManager*);
    unk88 = b;
    unk8C = c;
    unk90 = a;
    refreshBattleActors(this);
}
// NB: refreshBattleActors stays declared inline here (not in CBattleManager.hpp):
// CVision.cpp re-declares it as (cf::CBattleManager*, f32, int) and
// CChainTime.hpp (concurrent agent edit) now declares (cf::CBattleManager*,
// f32, int) repo-wide; MWCC rejects a second visible (float, u32) form as
// overload (error 10197). The (f32, int) form is ABI-identical to the
// original (float, u32) C-linkage decl (same 4-byte args, same symbol).
extern "C" void refreshBattleActors(cf::CBattleManager*, f32, int);

#pragma schedule off
void cf::CBattleManager::CBattleMan_ResetTimer() {
    unk88 = lbl_eu_80666DDC;
    refreshBattleActors(this, lbl_eu_80666DD4, unk8C);
}
#pragma schedule on
void cf::CBattleManager::CBattleMan_EmitVisionEvent(void*) { mVision.detachVisionEffect(); }
// func_800EC918 (retail 0x800ED400, 0x6E1C) - main battle-event processor.
// The callers below pass ABI-compatible args; the typed definition follows.
// (unmangled), so they must be declared extern "C" for reloc-name matching.
// isBattleStatusActive / findBattleStatusEntry / Counter_TestBit are already declared in
// object/CAIAction.hpp / CVision.hpp (included via CBattleManager.hpp).
extern "C" int IdTable_GetSum(void*, int, void*);
// Retail C wrapper (this += 0x18, tail-call): the actor is forwarded in r4 as
// the Chain_FindOrCreateActor key.
extern "C" bool CChain_chkActorList(void* chain, cf::CfObjectActor* actor);
extern "C" void* findTopBattleStatusEntry(void*, u32);
extern "C" int isBattleEventKind1(int);
extern "C" int classifyBattleCommand(int);
extern "C" int isBattleEventKind3(int);
extern "C" int isBattleStatusKind0(int);
extern "C" int mtRand__Q22ml4mathFi(int n);
extern "C" f32 lbl_eu_80666DF4;  // 15.0f
extern "C" f32 lbl_eu_80666DD0;  // 0.6f
// 0.01f (existing lbl_80666DD8 without eu_ is a different symbol)
extern "C" f32 lbl_eu_80666DD8;   // 0.01f

// --- updateBattleManager imports (retail-unmangled names) ---
extern "C" void driveVisionRingFx(cf::CVision* self);
extern "C" void PartyGaugeTickDecay(cf::UnkClass_8018C5FC* self);
extern "C" void updatePtStateTimer(cf::UnkClass_80192BF4* self);
extern "C" void refreshPtStateAccum(cf::UnkClass_80192BF4* self);
// (memberized: CChain_UpdatePerFrame -> CChain::UpdatePerFrame; declaration moved into class CChain)
extern "C" void ChainTmr_Update_D20C(cf::UnkClass_800D8DBC* self);
extern "C" void SysWinBuff_EmptyStubB20(void* self);
void SysWinLog_PollBattleEnd();
extern "C" s32 isGimmickActive__Q22cf13CfGameManagerFv(void* self);

// --- registerBattleActor imports
extern "C" void CCharVoiceMan_ResetAndAppend();
extern "C" void CCharVoiceMan_LatchPendingVoiceId(void* actor);
void SysWinLog_ResetState();

extern "C" f32 lbl_eu_80666DF8;  // 25.0f
extern "C" f32 lbl_eu_80666E00;  // 100.0f
extern "C" f32 lbl_eu_80666E18;  // 50.0f
extern "C" f32 lbl_eu_80666E1C;  // 0.25f
extern "C" f32 lbl_eu_80666E20;  // 200.0f
extern "C" f32 lbl_eu_80666E2C;  // -1.0f
extern "C" f32 lbl_eu_80666E34;  // 10.0f
extern "C" f32 lbl_eu_80666E54;  // 32.0f
extern "C" f64 lbl_eu_80666E58;  // 0.5 (f64)
extern "C" f64 lbl_eu_80666E60;  // -0.5 (f64)
extern "C" f32 lbl_eu_80666E7C;  // 75.0f
extern "C" f32 lbl_eu_80666E80;  // 2.5f
extern "C" f32 lbl_eu_80666E84;  // 0.1f
extern "C" const f32 lbl_eu_80666DE8;  // 0.5f
extern "C" f32 lbl_eu_80666E30;  // 0.9f
extern "C" f32 lbl_eu_80666DFC;  // 2.0f
extern "C" f32 lbl_eu_80666E10;  // 0.024543693f (2*pi/256)
extern "C" f32 lbl_eu_80666E24;  // 0.15f
extern "C" f32 lbl_eu_80666E68;  // 3.0f
extern "C" f32 lbl_eu_80666E6C;  // 5.0f
extern "C" f32 lbl_eu_80666E70;  // 0.35f
extern "C" f32 lbl_eu_80666E74;  // 0.01f (squared-distance epsilon)
// (ml::pi comes from CfObjectEne.hpp; an extra extern "C" copy here
// breaks the C++-linkage declaration when the header is included twice.)
extern "C" const s16 sStatusCountTable[];
// --- shared battle-helper externs used by the updateBattleAction / processBattleMove ports ---
extern "C" u16 getReloadParam0();                              // current chapter/episode
extern "C" void* bindIndexedEffect(u32 cls, int param);          // object factory
extern "C" f32 getArtsFacing(void*);                         // table[clamp(idx,0,4)] @0x80501978
extern "C" f32 getArtsClampedRate(void*);                         // table[clamp(idx,0,5)] @0x80501990
f32 getArtsScaledSpeed(const u8* self);                         // per-frame-scaled table @0x805019A8
extern "C" f32 getArtsGaugeRate(void*);                         // table[clamp(idx,0,5)] @0x805019C0
extern "C" void CCharVoiceMan_EnqueueDamageVoice(void*, void*, void*);          // battle-voice enqueue (3 args)
extern "C" void CCharVoiceMan_ClearPlayerVoiceCount(void*);                        // battle-voice clear
extern "C" void CCharVoiceMan_EnqueueHurtVoice(void*, void*, void*);          // battle-voice enqueue (3 args)
void SysWinLog_PollSequenceProgress(CScenarioLogOwner*, s32, CScenarioLogArg2*); // battle-voice (3 args)
extern "C" f32 Atan2FIdx__Q24nw4r4mathFff(f32 y, f32 x);      // nw4r math
extern "C" void seedReloadTmr(s32);
extern "C" s32 CPartsChange_SubmitSpawnRequest(u32, u16);
extern "C" void CCharVoiceMan_EnqueueBattleEventVoice(void*, void*, void*);
extern "C" void CCharVoiceMan_HookPendingBattleVoice(void*);
extern "C" void CCharVoiceMan_EnqueueBreakAndGaugeVoice(void*, void*, int);
extern "C" void CCharVoiceMan_PlayPairVoice(void*, void*);
extern "C" void CCharVoiceMan_TriggerBattleOrPlainVoice(void*);
extern "C" void CCharVoiceMan_TickPlayerVoiceCount(void*);
#include "kyoshin/code_8028130C.hpp"
void SysWinLog_DrivePair(CScenarioLogOwner*, CScenarioLogOwner*);
extern "C" void* getLiveParamPtr(void*);
extern "C" void attachEffectById(void*, void*, u32, u32);
extern "C" void SuddenCommuDispatchAttacker(void*, void*, void*, void*);
extern "C" int IdTable_GetSumMax(void*, int, u32*, u32*);
extern "C" int IdTable_QuerySumMaxFloat(void*, int, u32*, u32*, u32*);
extern "C" int* CtrlObjectParam_GetSlotTableBase();
extern "C" void broadcastBattleDamage(void*, void*, s32, s32, s32, s32, void*);
extern "C" s32 CBattleMan_ApplyHitStop9(void*, BattleObjAccessor*, void*, void*);
extern "C" s32 CBattleMan_ApplyHitStop10(void*, BattleObjAccessor*, void*, void*);
extern "C" void CBattleMan_SendActorEvent(cf::CBattleManager*, BattleObjAccessor*);
extern "C" void BtlDmg_FilterNotifyDamage(void*, u32, int);
extern "C" void BtlDmg_SetField779(u8);
extern "C" void BtlDmg_SetByte778(u8);
extern "C" void BtlDmg_SetByte777(u8);
extern "C" void BtlDmg_SetDamageType(u8);
extern "C" void BtlDmg_SetDamageDir(u8);
extern "C" void BtlDmg_NotifyDamage(void*, u32);
extern const u32 sDamageRateThresholds[3];  // {0,0,100} (3rd used as int)
extern "C" const float sDamageRates[3];// {8.0f,4.0f,2.0f} rate table
// --- forward decls for same-TU battle functions defined later in this file ---
extern "C" void processBattleMove(cf::CBattleManager*, void*, void*, void*);
extern "C" void spreadChainArtsHits(void*, void*, void*, void*);
extern "C" void emitBattleEvents(void*, void*, void*, void*);
extern "C" s32 emitBattleMoveEvents(void*, void*, void*, void*, void*);
extern "C" void unregisterBattleActor(void*, void*);   // canonical (void*,void*) form
extern "C" f32 CBattleMan_ApplyStatusRate(u8* obj, void* target);
// Arts-data auxiliary dispatch (vtable pointer at sub+0x84, this = sub);
// defined later in this file (owner: CAttackParam-compatible count slot).
static inline s32 artsSubGetMax(void* sub);
static inline f32 artsSubGetF14(void* sub);
// Retail bool-normalize for (byte & mask): neg/or/rlwinm -> 0/1 (not >>n&1).
#define bmByteBit01(byte, mask) \
    ((s32)((u32)(-(s32)(u8)((byte) & (mask)) | (u8)((byte) & (mask))) >> 31))
// Word bit-test -> double cntlzw/rlwinm (CtrlAct / CfObjectMove pattern).
#define bmFlagBit(flags, mask) \
    (((u32)__cntlzw((u32)__cntlzw((flags) & (mask)) >> 5) >> 5) != 0)
struct BattleEvent {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u16 eventType;
    u16 _pad_0E;
    s32 field_10;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    s16 _pad_1A;
    s32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    u16 field_2C;
    union { u16 prevEventType; u16 field_2E; };
    u32 field_30;
};

struct BattleEventData {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    union { u16 eventType; u16 field_0C; };
    u16 _pad_0E;
    s32 field_10;
    s16 field_14;
    s16 field_16;
    s16 field_18;
    s16 _pad_1A;
    s32 field_1C;
    f32 field_20;
    f32 field_24;
    f32 field_28;
    u16 field_2C;
    union { u16 prevEventType; u16 field_2E; };
    u32 field_30;
};
struct BattleEventWorkspace {
    BattleEvent tailEvent;
    BattleEvent case291Event;
    BattleEvent case290Event;
    BattleEvent case267SecondaryEvent;
    BattleEvent case267PrimaryEvent;
    BattleEvent case251Event;
    BattleEvent case250Event;
    BattleEvent case287Event;
    BattleEvent case261Event;
    BattleEvent case260PlayerEvent;
    BattleEvent case260ActorEvent;
    BattleEvent case249Event;
    BattleEvent case259Event;
    BattleEvent case258Event;
    BattleEvent case257Event;
    BattleEvent case236Event;
    BattleEvent case275Event;
    BattleEvent case274Event;
    BattleEvent case246Event;
    BattleEvent case242Event;
    BattleEvent case241Event;
    BattleEvent case240Event;
    BattleEvent case239Event;
    BattleEvent case244ActorEvent;
    BattleEvent case244PlayerEvent;
    BattleEvent case254Event;
    BattleEvent case243Event;
    BattleEvent case253Event;
    BattleEvent case238ActorEvent;
    BattleEvent case238PlayerEvent;
    BattleEvent case237Event;
    BattleEvent case235Event;
    BattleEvent case215Event;
    BattleEvent case227Event;
    BattleEvent case4Followup;
    BattleEvent case17Followup;
};

struct BattleTargetData;
struct EC918_BattleObjAccessor;
extern "C" s32 func_800EC918(
    void* self,
    void* pc,
    EC918_BattleObjAccessor* accessor,
    BattleEvent* event,
    BattleTargetData* target
);

void CBattleMan_RunBattleEvent(u32 a, u32 b, void* c, u32 d) {
    func_800EC918(
        (void*)a,
        0,
        (EC918_BattleObjAccessor*)b,
        (BattleEvent*)c,
        (BattleTargetData*)d
    );
}
struct BattleTargetData {
    u8 _pad_00[0x50];
    void* artsData;     // +0x50
    f32 field_54;       // +0x54
    f32 field_58;       // +0x58
    f32 field_5C;       // +0x5C
    f32 field_60;       // +0x60
    f32 field_64;       // +0x64
    f32 field_68;       // +0x68
    s16 field_6C;       // +0x6C
    s16 field_6E;       // +0x6E
    s16 field_70;       // +0x70
    s16 field_72;       // +0x72
    u32 field_74;       // +0x74
    u32 field_78;       // +0x78
    u32 field_7C;       // +0x7C
    u16 field_80;       // +0x80
    u8 _pad_82[0xA8 - 0x82];
    u32 field_A8;       // +0xA8
    u32 field_AC;
    u32 field_B0;
    u32 field_B4;
    u32 field_B8;
};

struct SubAccessor {
    void* vtbl;
};

struct EC918_BattleObjAccessor {
    void* vtbl;
    void* vtbl2;
    SubAccessor subObject;
    u8 _pad_0C[0x1524];
    u32 field_1530;
    u8 _pad_1534[0x15E4 - 0x1534];
    u32 field_15E4;
    u8 _pad_15E8[0x15F0 - 0x15E8];
    u32 field_15F0;
    u8 _pad_15F4[0x3374 - 0x15F4];
    u32 field_3374;
    u8 _pad_3378[0x3E9C - 0x3378];
    u8 field_3E9C;
    u8 _pad_3E9D[0x3ED4 - 0x3E9D];
    void* field_3ED4;
    u8 _pad_3ED8[0x3F00 - 0x3ED8];
    u32 field_3F00;
    u8 _pad_3F04[0x3F10 - 0x3F04];
    u32 field_3F10;
    u8 _pad_3F14[0x3F28 - 0x3F14];
    u16 field_3F28;
    u8 _pad_3F2A[0x456C - 0x3F2A];
    u16 field_456C;
};

// ---- Helper: modulo 100 via mulhw 0x51EC851F pattern ----
static inline s32 mod100(s32 x) {
    return x % 100;
}

// ================================================================
// vcall helpers + constants + externs for func_800EC918 switch cases
// ================================================================
#pragma push
#pragma inline_max_size(10000)
#pragma inline_max_total_size(100000)

static __inline cf::CfObjectMove* getObjectMove(void* actorAccessor) {
    cf::CfObjectMove* move = (cf::CfObjectMove*)actorAccessor;
    if (move != nullptr) {
        move = static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)move);
    }
    return move;
}

extern "C" int mtRand__Q22ml4mathFii(int a, int b);
extern "C" void* getFP__FPCc(const char* str);
// Bdat_GetTable_AA34: already declared (void form) by a visible shared header.

extern "C" s32 CItem_rollBoxEntry(s32 a, u16 b);
extern "C" s32 spawnBankByte(void);
extern "C" void CCharVoiceMan_EnqueueArtsVoice(void* pc, s32 v);
extern void getCfObjectPc(cf::CfObjectMove* moveObj);
extern "C" u8 lbl_eu_80573EEC[];

#define k0_0f lbl_eu_80666DDC
#define k0_01f 0.01f
#define k0_25f 0.25f
#define k0_5 0.5
#define kM0_5 -0.5
#define k1_0f 1.0f
#define k1_25f 1.25f
#define k2_0f 2.0f
#define k2_5f 2.5f
#define k5_0f 5.0f
#define k10_0f 10.0f
#define k15_0f 15.0f
#define k25_0f 25.0f
#define k50_0f 50.0f
#define k75_0f 75.0f
#define k99_0f 99.0f
#define k100_0f 100.0f

extern "C" void* __ct__800FB044(void* list, f32 f, void* obj, int kind);
extern "C" float calcBattleDamage(void* obj, void* target, void* source);

// Move/SubObj/StatusEntry/Actor/EventData deleted: use CActorParam_UnkStruct1 / UnkStruct2 / CfStatusEntry / CfObjectActor / BattleEvent

static __inline u32 copyBattleEvent(
    BattleEvent& destination,
    const BattleEvent& source
) {
    u32* destinationWords = (u32*)&destination;
    const u32* sourceWords = (const u32*)&source;

    for (s32 i = 0; i < 12; i += 2) {
        u32 first = sourceWords[i];
        u32 second = sourceWords[i + 1];
        destinationWords[i + 1] = second;
        destinationWords[i] = first;
    }

    u32 last = sourceWords[12];
    destinationWords[12] = last;
    return last;
}

static __inline void copyBattleEventPair(
    BattleEvent& first,
    BattleEvent& second,
    BattleEvent& source,
    u32 secondFlags
) {
    u32* words = (u32*)&source;
    u32 word00 = words[0];
    u32 word04 = words[1];
    u32 word08 = words[2];
    u32 word0C = words[3];
    u32 word10 = words[4];
    u32 word14 = words[5];
    u32 word18 = words[6];
    u32 word1C = words[7];
    u32 word20 = words[8];
    u32 word24 = words[9];
    u32 word28 = words[10];
    u32 word2C = words[11];
    u32 word30 = words[12];

    u32* firstWords = (u32*)&first;
    firstWords[0] = word00; firstWords[1] = word04; firstWords[2] = word08;
    firstWords[3] = word0C; firstWords[4] = word10; firstWords[5] = word14;
    firstWords[6] = word18; firstWords[7] = word1C; firstWords[8] = word20;
    firstWords[9] = word24; firstWords[10] = word28; firstWords[11] = word2C;
    firstWords[12] = word30;

    u32 secondWord30 = word30 | secondFlags;
    if (secondFlags != 0) {
        words[12] = secondWord30;
    }

    u32* secondWords = (u32*)&second;
    secondWords[0] = word00; secondWords[1] = word04; secondWords[2] = word08;
    secondWords[3] = word0C; secondWords[4] = word10; secondWords[5] = word14;
    secondWords[6] = word18; secondWords[7] = word1C; secondWords[8] = word20;
    secondWords[9] = word24; secondWords[10] = word28; secondWords[11] = word2C;
    secondWords[12] = secondWord30;
}

static __inline u32 battleCategoryFlag70(cf::CActorParam_UnkStruct3* categoryData, u32 category) {
    switch (category) {
    case 1:
    case 2:
    case 3: return (categoryData->unk70[0].unk0 >> 7) & 1;
    case 4: return categoryData->unk70[0].unk0 & 1;
    case 5: return (categoryData->unk70[0].unk0 >> 1) & 1;
    case 6: return (categoryData->unk70[0].unk0 >> 2) & 1;
    case 7: return (categoryData->unk70[0].unk0 >> 3) & 1;
    case 8: return (categoryData->unk70[0].unk0 >> 4) & 1;
    case 9: return (categoryData->unk70[0].unk0 >> 5) & 1;
    default:
    case 0: return 0;
    }
}

static __inline u32 battleCategoryFlag71(cf::CActorParam_UnkStruct3* categoryData, u32 category) {
    switch (category) {
    case 1:
    case 2:
    case 3: return (categoryData->unk70[1].unk0 >> 7) & 1;
    case 4: return categoryData->unk70[1].unk0 & 1;
    case 5: return (categoryData->unk70[1].unk0 >> 1) & 1;
    case 6: return (categoryData->unk70[1].unk0 >> 2) & 1;
    case 7: return (categoryData->unk70[1].unk0 >> 3) & 1;
    case 8: return (categoryData->unk70[1].unk0 >> 4) & 1;
    case 9: return (categoryData->unk70[1].unk0 >> 5) & 1;
    default:
    case 0: return 0;
    }
}

static __inline s32 battleCategoryStat(cf::CActorParam_UnkStruct3* categoryData, u32 category) {
    switch (category) {
    case 4: return categoryData->field_64;
    case 5: return categoryData->field_66;
    case 6: return categoryData->field_68;
    case 7: return categoryData->field_6A;
    case 8: return categoryData->field_6C;
    case 9: return categoryData->field_6E;
    default: return 0;
    }
}

extern "C" s32 func_800EC918(
    void* self,
    void* pc,
    EC918_BattleObjAccessor* acc,
    BattleEvent* evt,
    BattleTargetData* tgt
) {
    f32 f28 = lbl_eu_80666DDC;
    void* artsData = nullptr;
    BattleEventWorkspace eventWorkspace;

    if (tgt != nullptr) {
        artsData = tgt->artsData;
    }

    // evt->field_2C = tgt->field_80
    if (tgt != nullptr) {
        evt->field_2C = tgt->field_80;
    }

    // ---- Guard 1: game flag 0x1000 ----
    {
        extern void* getInstance__Q22cf13CfGameManagerFv();
        getInstance__Q22cf13CfGameManagerFv();
        if (isGlobalCamFlagSet(0x10000000)) {
            return 0;
        }
    }

    // ---- Guard 2: actor vfunc 0x2BC ----
    {
        if (((cf::CActorParam*)acc)->CActorParam_isBattleLocked()) {
            return 0;
        }
    }

    // ---- Guard 3: actor state disallows battle-event processing ----
    if (acc->field_3374 & 0x08000000) {
        return 0;
    }

    // ---- Guard 4: Confirmation gate (flag 0x04000000) ----
    {
        extern void* getInstance__Q22cf13CfGameManagerFv();
        getInstance__Q22cf13CfGameManagerFv();
        if (isGlobalCamFlagSet(0x04000000)) {
            extern const void* lbl_eu_8052B784[];
            extern const void* lbl_eu_8052B790[];
            bool rejectsConfirmation = false;

            {
                u32 firstHandler[3];
                firstHandler[0] = (u32)lbl_eu_8052B784[0];
                firstHandler[1] = (u32)lbl_eu_8052B784[1];
                firstHandler[2] = (u32)lbl_eu_8052B784[2];
                if (__ptmf_cmpr(&((cf::CBattleManager*)self)->mVision.mPtmf, firstHandler)) {
                    u32 confirmationHandler[3];
                    confirmationHandler[0] = (u32)lbl_eu_8052B790[0];
                    confirmationHandler[1] = (u32)lbl_eu_8052B790[1];
                    confirmationHandler[2] = (u32)lbl_eu_8052B790[2];
                    if (__ptmf_cmpr(&((cf::CBattleManager*)self)->mVision.mPtmf, confirmationHandler)) {
                        rejectsConfirmation = true;
                    }
                }
            }

            if (rejectsConfirmation) {
                return 0;
            }

            evt->field_30 |= 0x10;
        }
    }

    // ---- Entry fixups ----
    if (isBattleEventKind3(evt->eventType)) {
        if (evt->field_00 == 0 || ((cf::CfObjectActor*)acc)->field_0x74 == evt->field_00) {
            u32 stagingId = *(u32*)((cf::CActorParam*)acc)->unk1528;
            if (stagingId != 0) {
                ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(stagingId);
            }
        }
    }

    // ---- Counter-attack block ----
    if (artsData != nullptr && tgt != nullptr && pc != nullptr) {
        f28 = CBattleMan_ApplyStatusRate((u8*)pc, tgt);

        if (evt->prevEventType == 0 || (evt->field_30 & 0x2)) {
            if (isBattleStatusKind1(evt->eventType)) {
                u16 artsId = ((cf::CAttackParamPrefix*)artsData)->unk46;
                s32 level = ((cf::CActorParam*)pc)->CActorParam_getActorLevel();
                s32 product = (s32)artsId * (level + 14);
                f32 fprod = (f32)(s32)product;
                ((cf::CfObjectActor*)acc)->CActorParam_addHate(pc, lbl_eu_80666DDC, f28 * fprod, lbl_eu_80666DDC);

            } else if (isBattleEventKind3(evt->eventType)) {
                u16 artsId = ((cf::CAttackParamPrefix*)artsData)->unk46;
                s32 level = ((cf::CActorParam*)pc)->CActorParam_getActorLevel();
                s32 product = (s32)artsId * (level + 14);
                s32 damage = (s32)(f28 * (f32)(s32)product);
                broadcastBattleDamage(
                    self,
                    pc,
                    0,
                    damage,
                    0,
                    1,
                    (void*)(uintptr_t)acc->field_3F10
                );

            } else if (isBattleStatusKind0(evt->eventType)) {
                if (evt->prevEventType == 0) {
                    u16 artsId = ((cf::CAttackParamPrefix*)artsData)->unk46;
                    s32 level = ((cf::CActorParam*)pc)->CActorParam_getActorLevel();
                    s32 product = (s32)artsId * (level + 14);
                    s32 damage = (s32)(f28 * (f32)(s32)product);
                    broadcastBattleDamage(
                        self,
                        pc,
                        0,
                        damage,
                        0,
                        1,
                        (void*)(uintptr_t)acc->field_3F10
                    );
                }
            }
        }
    }

    // ---- subAccessor vfunc 0x80 ----
    {
        if (((cf::CBattleState*)&acc->subObject)->CBattleState_getEventMask(evt->eventType)) {
            return CBattleMan_ApplyHitStop9(
                self, (BattleObjAccessor*)acc, evt, tgt);
        }
    }

    // ---- subAccessor vfunc 0x88 ----
    {
        if (((cf::CBattleState*)&acc->subObject)->CBattleState_getStatusMask(evt->eventType)) {
            bool resisted = false;
            s32 resistanceChance = 0x32;

            if (evt->eventType == 0x12) {
                s32 f15E4 = ((cf::CActorParam*)acc)->unk15E4;
                if (f15E4 == 1) resistanceChance = 0x14;
                if (f15E4 == 2) resistanceChance -= 0x14;
                if (f15E4 == 3) resistanceChance += 0x14;
                if (f15E4 == 4) resistanceChance += 0x32;
                if (f15E4 == 5) resistanceChance += 0x32;

                if (((cf::CfObjectActor*)acc)->unk3374 & 0x100) {
                    resistanceChance += 0x1E;
                }

                u8 byte1AA = ((cf::CBattleManager*)self)->mChain.mHead.field_2;
                if (byte1AA >= 1) {
                    if (byte1AA <= 24) {
                        resistanceChance = 0;
                    }
                }
            }

            s32 rollVal;
            if (tgt != nullptr) {
                rollVal = mod100((s32)tgt->field_B0);
            } else {
                rollVal = mtRand__Q22ml4mathFi(100);
            }

            if (rollVal < resistanceChance) {
                resisted = true;
                if (tgt != nullptr) {
                    tgt->field_74 |= 0x80000010;
                }
            }

            if (resisted) {
                return CBattleMan_ApplyHitStop10(
                    self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
    }

    // ---- Status check: 0x27 ----
    if (isBattleStatusActive((&acc->subObject), 0x27)) {
        if (!(acc == (EC918_BattleObjAccessor*)pc && evt->eventType == 0x11)) {
            if (isBattleEventKind1(evt->eventType)) {
                cf::CBattleStateEntry* entry27 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x27);
                if (entry27 != nullptr) {
                    s32 rollVal2;
                    if (tgt != nullptr) {
                        rollVal2 = (s32)tgt->field_B4;
                    } else {
                        rollVal2 = mtRand__Q22ml4mathFi(100);
                    }
                    s32 rate = entry27->unk10;
                    if (rollVal2 < rate) {
                        return CBattleMan_ApplyHitStop10(
                            self, (BattleObjAccessor*)acc, evt, tgt);
                    }
                }
            }
        }
    }

    // ---- Status check: 0xCE ----
    if (isBattleStatusActive((&acc->subObject), 0xCE)) {
        if (tgt != nullptr && (tgt->field_78 & 0x1000)) {
            if (isBattleStatusKind1(evt->eventType)) {
                cf::CBattleStateEntry* entryCE = (cf::CBattleStateEntry*)findTopBattleStatusEntry((&acc->subObject), 0xCE);
                if (entryCE != nullptr && artsData != nullptr) {
                    s32 artsVal = ((cf::CObjectState*)artsData)->checkStateFlags(0);
                    s32 entryVal = entryCE->unk10;
                    if (entryVal >= artsVal) {
                        tgt->field_74 |= 0x80002000;
                        return CBattleMan_ApplyHitStop10(
                            self, (BattleObjAccessor*)acc, evt, tgt);
                    }
                }
            }
        }
    }

    // ---- classifyBattleCommand result gate ----
    s32 resultGate = classifyBattleCommand(evt->eventType);

    cf::CActorParam_UnkStruct3* vf224Result;
    {
        vf224Result = (cf::CActorParam_UnkStruct3*)((cf::CActorParam*)acc)->CActorParam_getBattleParams();
    }

    // Categories 1-3 share the high bit. Categories 4-9 use their own
    // stat/flag slots; a sufficiently high stat forces the gate on.
    {
        u32 blocked = 0;
        if ((u32)(resultGate - 1) <= 2) {
            blocked = battleCategoryFlag71(vf224Result, resultGate);
        } else if (resultGate != 0) {
            if (battleCategoryStat(vf224Result, resultGate) >= 0x96) {
                blocked = 1;
            } else {
                blocked = battleCategoryFlag71(vf224Result, resultGate);
            }
        }

        if (blocked) return 0;
    }

    {
        u32 triggersResistance = 0;
        if ((u32)(resultGate - 1) <= 2) {
            triggersResistance = battleCategoryFlag70(vf224Result, resultGate);
        } else if (resultGate != 0) {
            if (battleCategoryStat(vf224Result, resultGate) >= 0x64) {
                triggersResistance = 1;
            } else {
                triggersResistance = battleCategoryFlag70(vf224Result, resultGate);
            }
        }

        if (triggersResistance) {
            s32 roll = tgt != nullptr
                ? mod100((s32)tgt->field_AC)
                : mtRand__Q22ml4mathFi(100);
            if (roll < 0x32) {
                return CBattleMan_ApplyHitStop10(
                    self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
    }

    // ---- Stat multiplier: 0x75 ----
    if (pc != nullptr) {
        void* statObj = ((cf::CActorParam*)pc)->CActorParam_getStatusTable();
        if (statObj != nullptr) {
            if (isBattleEventKind3(evt->eventType)) {
                u32 stackVal;
                if (IdTable_GetSum(((cf::CActorParam*)pc)->CActorParam_getStatusTable(), 0x75, (s32*)&stackVal)) {
                    f32 statMul = (f32)(s32)stackVal;
                    evt->field_20 += statMul;
                }
            }
        }
    }

    // ---- Stat multiplier: 0x7C ----
    {
        void* accStatObj = (void*)((cf::CActorParam*)(acc))->CActorParam_getStatusTable();
        if (accStatObj != nullptr) {
            if (isBattleStatusKind1(evt->eventType)) {
                u32 stackVal2;
                if (IdTable_GetSum(((cf::CActorParam*)acc)->CActorParam_getStatusTable(), 0x7C, (s32*)&stackVal2)) {
                    if (tgt != nullptr) {
                        if ((s32)tgt->field_B4 < (s32)stackVal2) {
                            return CBattleMan_ApplyHitStop10(
                                self, (BattleObjAccessor*)acc, evt, tgt);
                        }
                    } else {
                        if (mtRand__Q22ml4mathFi(100) < (s32)stackVal2) {
                            return CBattleMan_ApplyHitStop10(
                                self, (BattleObjAccessor*)acc, evt, tgt);
                        }
                    }
                }
            }
        }
    }

    // ---- isBattleStatusKind0 check + CCharVoiceMan_EnqueueBattleEventVoice ----
    if (isBattleStatusKind0(evt->eventType)) {
        CCharVoiceMan_EnqueueBattleEventVoice(acc, pc, tgt);
    }

    // ================================================================
    // MAIN DISPATCH: switch (evt->eventType - 4)
    // jumptable at lbl_eu_8052B79C; default falls to shared tail.
    // ================================================================

    switch (evt->eventType) {

    case 17: {
        // Skill roll: status 0x22 (resist/evade gate)
        if (isBattleStatusActive((&acc->subObject), 0x22)) {
            if (acc != (EC918_BattleObjAccessor*)pc) {  // cmplw r24,r23; beq
                cf::CBattleStateEntry* entry22 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x22);
                s32 rollVal;
                if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                else rollVal = mtRand__Q22ml4mathFi(100);
                s32 rate = entry22->unk10;
                if (rollVal < rate) {
                    return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
                }
            }
        }

        // Clear status 0x11 through the sub-acc
        if (isBattleStatusActive((&acc->subObject), 0x11)) {
            ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(0x11);   // lwz r12,8(r24); lwz r12,0x20(r12); bctrl
        }

        s32 initialValue = evt->field_10;
        if (initialValue != 0) break;         // bne .L_800F4000
        if (evt->field_14 != 0) break;       // bne .L_800F4000

        // Preserve the original for the follow-up, then process the primary
        // event in place.
        copyBattleEvent(eventWorkspace.case17Followup, *evt);
        evt->field_10 = ((cf::CfObjectActor*)pc)->field_0x74;

        if (tgt != nullptr && (tgt->field_78 & 1)) {  // clrlwi. 0,0,31
            evt->field_14 = 1;
        }

        s32 subResult = func_800EC918(self, pc, acc, evt, tgt);
        if (!subResult) {                       // beq 0x800EE0B4
            return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
        }

        // acc->3ED4 holder slot 0x70 (CfObject UVF8; retail passes the
        // +0x3F10 effect-target in r4 - hidden arg, base decl is deficient
        // and widening it breaks CfObjectModel/Eff/Pc no-arg callers).
        ((cf::CfObject*)acc->field_3ED4)->CfObject_notifyEventDone();

        // If the acc's sub-identifier object == player 0 -> HUD refresh
        cf::CfObjectMove* subIdent = getObjectMove(acc);
        if (subIdent == getPlayer__Q22cf13CfGameManagerFi(0)) {
            void* hud = Selector_GetInstance();
            Selector_UpdateFilter((cf::CfObjectSelectorObj*)hud, (unsigned int)((cf::CfObjectActor*)pc)->field_0x74);
        }

        // Second half: only when the tgt lacks bit0 of field_78
        if (tgt == nullptr || !(tgt->field_78 & 1)) {
            eventWorkspace.case17Followup.eventType = 0x11;
            eventWorkspace.case17Followup.field_30 |= 0x800;
            eventWorkspace.case17Followup.field_10 = ((cf::CfObjectActor*)acc)->field_0x74;

            func_800EC918(
                self,
                pc,
                (EC918_BattleObjAccessor*)pc,
                &eventWorkspace.case17Followup,
                tgt
            );

            // pc->3ED4 holder slot 0x70 (CfObject UVF8; r4 hidden arg, see above).
            ((cf::CfObject*)((void*)((cf::CfObjectActor*)pc)->mSubObj38))->CfObject_notifyEventDone();

            cf::CfObjectMove* pcSubIdent = getObjectMove(pc);
            if (pcSubIdent == getPlayer__Q22cf13CfGameManagerFi(0)) {
                void* hud = Selector_GetInstance();
                Selector_UpdateFilter((cf::CfObjectSelectorObj*)hud, (unsigned int)((cf::CfObjectActor*)acc)->field_0x74);
            }
        }
        return 1;                               // li r3,1; b .L_800F41D4
    }

    case 4: {
        if (isBattleStatusActive((&acc->subObject), 0x15)) {
            cf::CBattleStateEntry* entry15 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x15);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            s32 rate = entry15->unk10;
            if (rollVal < rate) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        // .L_800EE200
        evt->prevEventType = evt->eventType;      // lhz r3,0xc(r25); sth r3,0x2e(r25)
        evt->field_30 = copyBattleEvent(eventWorkspace.case4Followup, *evt) | 2;
        eventWorkspace.case4Followup.eventType = 5;
        func_800EC918(self, pc, acc, &eventWorkspace.case4Followup, tgt);
        break;                                  // b .L_800F4000
    }

    case 9: {
        if (isBattleStatusActive((&acc->subObject), 0x1A)) {
            cf::CBattleStateEntry* entry1A = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x1A);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < entry1A->unk10) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 6: {
        if (isBattleStatusActive((&acc->subObject), 0x17)) {
            cf::CBattleStateEntry* entry17 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x17);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < entry17->unk10) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 7: {
        if (isBattleStatusActive((&acc->subObject), 0x18)) {
            cf::CBattleStateEntry* entry18 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x18);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < entry18->unk10) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 19: {
        if (isBattleStatusActive((&acc->subObject), 0x24)) {
            cf::CBattleStateEntry* entry24 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x24);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < entry24->unk10) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 11: {
        if (isBattleStatusActive((&acc->subObject), 0x1C)) {
            cf::CBattleStateEntry* entry1C = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x1C);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < entry1C->unk10) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 12: {
        if (isBattleStatusActive((&acc->subObject), 0x1D)) {
            cf::CBattleStateEntry* entry1D = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x1D);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < entry1D->unk10) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                  // b .L_800F4000
    }

    case 147: {
        if (evt->field_20 == lbl_eu_80666DDC) {            // fcmpu cr0, f1(0.0), f0; bne tail
            evt->field_20 = k5_0f;               // lfs f0, 80666E6C; stfs 0x20(r25)
        }
        break;                                  // b .L_800F4000
    }

    case 280: {
        u32 fmt = (*(u16*)((u8*)acc + 0x456C)) >> 4;   // lhz + srawi r3,r3,4
        u16 artsId = (u16)evt->field_10;                     // clrlwi r4, r0, 16
        CPartsChange_SubmitSpawnRequest(fmt, artsId);
        break;                                  // b .L_800F4000
    }

    case 299: {
        getInstance__Q22cf13CfGameManagerFv();  // bl (result unused in this case)
        if (!isGlobalCamFlagSet(0x04000000)) {   // lis r3, 0x400  (0x04000000)
            seedReloadTmr(evt->field_10);        // lwz r3,0x10(r25); bl seedReloadTmr
        }
        break;                                  // b .L_800F4000
    }

    case 227: {
        if (pc == nullptr) break;               // cmpwi r23,0; beq .L_800F4000

        // r15 = pc->CActorParam_getBattleStats() (stat object); r16 = flag
        void* pcStat = ((cf::CActorParam*)pc)->CActorParam_getBattleStats();      // 0x800EE538
        s32 flag = 0;                           // li r16, 0

        // acc->vf324()[0] == 7 (u16)
        if (*(u16*)((cf::CActorParam*)acc)->CActorParam_getActorModeFlags() == 7) flag = 1;  // lhz + cmplwi 7

        // acc->vf108() - pc->vf108() > 10
        if ((((cf::CActorParam*)acc)->CActorParam_getActorLevel() - ((cf::CActorParam*)pc)->CActorParam_getActorLevel()) > 10) flag = 1;  // subf + cmpwi 0xa

        if (!flag) {
            // Gate: obj4 = acc->+0x4; val = obj4->vf30()[0]
            int val;
            {
                void* obj4 = *(void**)((u8*)acc + 4);
                val = *(u32*)((cf::CObjectState*)obj4)->getStateData();
            }
            if (testActorStatus((void*)acc, &val, 0x802) == 0 &&   // bl testActorStatus; cmpwi; bne skip
                !isBattleStatusActive((&acc->subObject), 0x32)) {      // has 0x32 -> skip gate
                // f28 base by acc->field_15E4 (0 -> 0.0, 6+ -> 0.0)
                f32 f28 = lbl_eu_80666DDC;
                s32 state = acc->field_15E4;
                if (state == 1) {
                    f28 = k75_0f;
                } else if (state == 2) {
                    f28 = k50_0f;
                } else if (state == 3) {
                    f28 = k25_0f;
                } else if (state == 4) {
                    f28 = k15_0f;
                } else if (state == 5) {
                    f28 = k10_0f;
                }
                // pcStat->+0x50 artsData: f28 += 2.5 * (f32)artsLevel
                void* pcArts = ((cf::CActorParam_UnkStruct1*)pcStat)->unk50;   // lwz r3, 0x50(r15)
                if (pcArts != nullptr) {
                    f28 += k2_5f * (f32)((cf::CObjectState*)pcArts)->checkStateFlags(0);   // fmadds f28, f0(2.5), f1, f28
                }
                s32 rollVal;
                if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                else rollVal = mtRand__Q22ml4mathFi(100);
                if ((s32)f28 <= rollVal) flag = 1;      // fctiwz; cmpw; bgt skip; li r16,1
            } else {
                flag = 1;                               // .L_800EE6CC
            }
        }

        copyBattleEvent(eventWorkspace.case227Event, *evt);
        eventWorkspace.case227Event.eventType = 0x10;
        eventWorkspace.case227Event.field_20 = (f32)(s32)evt->field_10;
        eventWorkspace.case227Event.field_10 = 3;

        if (flag) {                             // cmpwi r16,0; beq .L_800EE798
            eventWorkspace.case227Event.eventType = 6;
            if (func_800EC918(self, pc, acc, &eventWorkspace.case227Event, tgt)) {
                break;
            }
            return 0;
        }

        // .L_800EE798 -- flag == 0 path
        eventWorkspace.case227Event.field_30 |= 0x2000;
        if (func_800EC918(self, pc, acc, &eventWorkspace.case227Event, tgt)) {
            break;
        }
        eventWorkspace.case227Event.eventType = 6;
        if (func_800EC918(self, pc, acc, &eventWorkspace.case227Event, tgt)) {
            break;
        }
        return 0;                               // li r3,0; b .L_800F41D4
    }

    case 16: {
        // obj4 = acc->+0x4; val = obj4->vf30()[0]; testActorStatus((void*)acc, &val, 0xA)
        int val;
        {
            void* obj4 = *(void**)((u8*)acc + 4);
            val = *(u32*)((cf::CObjectState*)obj4)->getStateData();
        }
        if (testActorStatus((void*)acc, &val, 0xA)) {       // beq .L_800EE84C
            void* accStat = ((cf::CActorParam*)acc)->CActorParam_getBattleStats();   // acc->CActorParam_getBattleStats()
            if (((cf::CActorParam_UnkStruct1*)accStat)->mFlagsArray[1].flags & 0x08000000) {  // rlwinm. 0,0,4,4
                return 0;                               // li r3,0; b .L_800F41D4
            }
        }

        // acc->field_3F00 & 0x2 -> walk self->0x48 list; empty -> return 0
        if (acc->field_3F00 & 0x2) {               // rlwinm. 0,0,30,30; beq
            void* head = *(void**)((u8*)(uintptr_t)self + 0x48);   // lwz r5, 0x48(r22)
            s32 count = 0;                              // li r4, 0
            void* node = *(void**)head;                 // lwz r3, 0(r5)
            while (node != head) {                      // .L_800EE868/.L_800EE870
                node = *(void**)node;                   // lwz r3, 0(r3)
                count++;                                // addi r4, r4, 1
            }
            if (count == 0) {                           // cmpwi r4,0; bne
                return 0;                               // li r3,0; b .L_800F41D4
            }
        }

        // .L_800EE888 -- pc != nullptr main branch; r14 = tgt ? tgt : pc->CActorParam_getBattleStats()
        if (pc != nullptr) {
            cf::CActorParam_UnkStruct1* targetStats;
            if (tgt != nullptr) targetStats = (cf::CActorParam_UnkStruct1*)tgt; // mr r14, r26
            else targetStats = (cf::CActorParam_UnkStruct1*)((cf::CActorParam*)pc)->CActorParam_getBattleStats();              // pc->CActorParam_getBattleStats()

            if ((targetStats->mFlagsArray[0].flags & 0x4) ||     // rlwinm. 0,3,29,29; bne .L_800EEADC
                (targetStats->mFlagsArray[0].flags & 0x2)) {     // rlwinm. 0,3,30,30; bne .L_800EEADC
                return 0;                               // .L_800EEADC: li r3,0
            }

            if (!(evt->field_10 & 0x4)) {                // rlwinm. 0,0,29,29; bne .L_800EE950
                // Menu gate: vf324()[0] & 0x8, status 0x3A, status 0x32
                s32 menuFlag = 0;                       // li r15, 0
                if (*(u16*)((cf::CActorParam*)acc)->CActorParam_getActorModeFlags() & 0x8) menuFlag = 1;  // rlwinm. 0,0,28,28
                if (isBattleStatusActive((&acc->subObject), 0x3A)) menuFlag = 1;
                if (isBattleStatusActive((&acc->subObject), 0x32)) menuFlag = 1;
                if (menuFlag) {                         // cmpwi r15,0; beq .L_800EE950
                    return CBattleMan_ApplyHitStop9(
                        self, (BattleObjAccessor*)acc, evt, (BattleTargetData*)targetStats);
                }
            }

            // .L_800EE950 -- roll gate (runs even when field_10 & 0x4)
            s32 rollFlag = 0;                           // li r15, 0
            if (!(evt->field_10 & 0x5)) {                // andi. r0, r0, 0x5; bne .L_800EE9C8
                if (isBattleStatusActive((&acc->subObject), 0x21)) {
                    cf::CBattleStateEntry* entry21 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x21);
                    s32 rollVal;
                    if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                    else rollVal = mtRand__Q22ml4mathFi(100);
                    if (rollVal < entry21->unk10) rollFlag = 1;
                }
                if ((acc->field_3374 & 0x80) &&    // rlwinm. 0,0,24,24; beq
                    !(targetStats->mFlagsArray[0].flags & 0x4000)) {  // rlwinm. 0,0,17,17; bne
                    rollFlag = 1;
                }
            }
            if (!(evt->field_10 & 0x6)) {                // rlwinm. 0,0,29,30; bne .L_800EEA2C
                if (!isBattleStatusActive((&acc->subObject), 0x12) &&
                    !isBattleStatusActive((&acc->subObject), 0x10)) {
                    if (acc->field_3F00 & 0x4) rollFlag = 1;  // rlwinm. 0,0,29,29
                }
            }
            if (rollFlag) {                             // cmpwi r15,0; beq .L_800EEA2C
                return CBattleMan_ApplyHitStop10(
                    self, (BattleObjAccessor*)acc, evt, (BattleTargetData*)targetStats);
            }

            // .L_800EEA2C -- pc skills 0x9C (multiply) / 0x9D (divide)
            if (isBattleStatusActive(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9C)) {
                cf::CBattleStateEntry* entry9C = (cf::CBattleStateEntry*)findBattleStatusEntry(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9C);
                evt->field_20 *= 1.0f + (f32)(s32)(entry9C->unk10) / 100.0f;
            } else if (isBattleStatusActive(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9D)) {
                cf::CBattleStateEntry* entry9D = (cf::CBattleStateEntry*)findBattleStatusEntry(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9D);
                evt->field_20 /= 1.0f + (f32)(s32)(entry9D->unk10) / 100.0f;
            }
        } else {
            // .L_800EEAE4 -- pc == nullptr branch
            if (!(evt->field_10 & 0x4)) {                // rlwinm. 0,0,29,29; bne .L_800EEB68
                s32 menuFlag = 0;                       // li r14, 0
                if (*(u16*)((cf::CActorParam*)acc)->CActorParam_getActorModeFlags() & 0x8) menuFlag = 1;
                if (isBattleStatusActive((&acc->subObject), 0x3A)) menuFlag = 1;
                if (isBattleStatusActive((&acc->subObject), 0x32)) menuFlag = 1;
                if (menuFlag) {                         // cmpwi r14,0; beq .L_800EEB68
                    return CBattleMan_ApplyHitStop9(self, (BattleObjAccessor*)acc, evt, tgt);
                }
            }

            // .L_800EEB68 -- roll gate (no 0x3374 check here)
            s32 rollFlag = 0;                           // li r15, 0
            if (!(evt->field_10 & 0x5)) {                // andi. r0, r0, 0x5; bne .L_800EEBC4
                if (isBattleStatusActive((&acc->subObject), 0x21)) {
                    cf::CBattleStateEntry* entry21 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x21);
                    s32 rollVal;
                    if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                    else rollVal = mtRand__Q22ml4mathFi(100);
                    if (rollVal < entry21->unk10) rollFlag = 1;
                }
            }
            if (!(evt->field_10 & 0x6)) {                // rlwinm. 0,0,29,30; bne .L_800EEC28
                if (!isBattleStatusActive((&acc->subObject), 0x12) &&
                    !isBattleStatusActive((&acc->subObject), 0x10)) {
                    if (acc->field_3F00 & 0x4) rollFlag = 1;
                }
            }
            if (rollFlag) {                             // cmpwi r15,0; beq .L_800EEC28
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        // .L_800EEC28 -- clear event head, fall to shared tail
        evt->field_04 = 0;                               // li r0,0; stw 0x4(r25)
        evt->field_00 = 0;                               // stw r0, 0x0(r25)
        break;                                          // b .L_800F4000
    }

    case 15: {
        int val;
        {
            void* obj4 = *(void**)((u8*)acc + 4);
            val = *(u32*)((cf::CObjectState*)obj4)->getStateData();
        }
        if (testActorStatus((void*)acc, &val, 0xA)) {       // beq .L_800EEC94
            void* accStat = ((cf::CActorParam*)acc)->CActorParam_getBattleStats();
            if (((cf::CActorParam_UnkStruct1*)accStat)->mFlagsArray[1].flags & 0x08000000) {
                return 0;                               // li r3,0; b .L_800F41D4
            }
        }

        if (acc->field_3F00 & 0x2) {               // rlwinm. 0,0,30,30; beq .L_800EECD0
            void* head = *(void**)((u8*)(uintptr_t)self + 0x48);
            s32 count = 0;
            void* node = *(void**)head;
            while (node != head) {
                node = *(void**)node;
                count++;
            }
            if (count == 0) {
                return 0;                               // li r3,0; b .L_800F41D4
            }
        }

        // .L_800EECD0 -- pc != nullptr main branch
        if (pc != nullptr) {
            cf::CActorParam_UnkStruct1* targetStats;
            if (tgt != nullptr) targetStats = (cf::CActorParam_UnkStruct1*)tgt; // mr r14, r26
            else targetStats = (cf::CActorParam_UnkStruct1*)((cf::CActorParam*)pc)->CActorParam_getBattleStats();              // pc->CActorParam_getBattleStats()

            if ((targetStats->mFlagsArray[0].flags & 0x4) ||     // rlwinm. 0,3,29,29; bne .L_800EEF2C
                (targetStats->mFlagsArray[0].flags & 0x2)) {     // rlwinm. 0,3,30,30; bne .L_800EEF2C
                return 0;                               // .L_800EEF2C: li r3,0
            }

            // field_30 & 0x80 -> skip everything, clear + tail (.L_800EF0A4)
            if (!(evt->field_30 & 0x80)) {               // rlwinm. 0,0,24,24; bne .L_800EF0A4
                if (!(evt->field_10 & 0x4)) {            // rlwinm. 0,0,29,29; bne .L_800EEDA4
                    // Menu gate: vf320()[0] & 0x8, status 0x3B, status 0x32
                    s32 menuFlag = 0;                   // li r15, 0
                    if (*(u16*)((cf::CActorParam*)acc)->CActorParam_getStateFlags() & 0x8) menuFlag = 1;  // rlwinm. 0,0,28,28
                    if (isBattleStatusActive((&acc->subObject), 0x3B)) menuFlag = 1;
                    if (isBattleStatusActive((&acc->subObject), 0x32)) menuFlag = 1;
                    if (menuFlag) {                     // cmpwi r15,0; beq .L_800EEDA4
                        return CBattleMan_ApplyHitStop9(
                            self, (BattleObjAccessor*)acc, evt, (BattleTargetData*)targetStats);
                    }
                }

                // .L_800EEDA4 -- roll gate (runs even when field_10 & 0x4)
                s32 rollFlag = 0;                       // li r15, 0
                if (!(evt->field_10 & 0x5)) {            // andi. r0, r0, 0x5; bne .L_800EEE1C
                    if (isBattleStatusActive((&acc->subObject), 0x20)) {
                        cf::CBattleStateEntry* entry20 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x20);
                        s32 rollVal;
                        if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                        else rollVal = mtRand__Q22ml4mathFi(100);
                        if (rollVal < entry20->unk10) rollFlag = 1;
                    }
                    if ((acc->field_3374 & 0x80) &&      // .L_800EEE00
                        !(targetStats->mFlagsArray[0].flags & 0x4000)) {
                        rollFlag = 1;
                    }
                }
                if (!(evt->field_10 & 0x6)) {            // rlwinm. 0,0,29,30; bne .L_800EEE7C
                    if (!isBattleStatusActive((&acc->subObject), 0x10)) {
                        if (acc->field_3F00 & 0x4) rollFlag = 1;
                    }
                }
                if (rollFlag) {                         // cmpwi r15,0; beq .L_800EEE7C
                    targetStats->mFlagsArray[0].flags |= 0x80000010;  // oris 0x8000; ori 0x10
                    return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
                }

                // .L_800EEE7C -- pc skills 0x9A (multiply) / 0x9B (divide)
                if (isBattleStatusActive(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9A)) {
                    cf::CBattleStateEntry* entry9A = (cf::CBattleStateEntry*)findBattleStatusEntry(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9A);
                    evt->field_20 *= 1.0f + (f32)(s32)(entry9A->unk10) / 100.0f;
                } else if (isBattleStatusActive(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9B)) {
                    cf::CBattleStateEntry* entry9B = (cf::CBattleStateEntry*)findBattleStatusEntry(&((EC918_BattleObjAccessor*)pc)->subObject, 0x9B);
                    evt->field_20 /= 1.0f + (f32)(s32)(entry9B->unk10) / 100.0f;
                }
            }
        } else {
            // .L_800EEF34 -- pc == nullptr branch
            if (isBattleStatusActive((&acc->subObject), 0x32)) {
                return 0;                               // bne .L_800EF09C
            }
            if (!(evt->field_30 & 0x80)) {               // rlwinm. 0,0,24,24; bne .L_800EF0A4
                if (!(evt->field_10 & 0x4)) {            // rlwinm. 0,0,29,29; bne .L_800EEFD8
                    s32 menuFlag = 0;                   // li r14, 0
                    if (*(u16*)((cf::CActorParam*)acc)->CActorParam_getStateFlags() & 0x8) menuFlag = 1;
                    if (isBattleStatusActive((&acc->subObject), 0x3B)) menuFlag = 1;
                    if (isBattleStatusActive((&acc->subObject), 0x32)) menuFlag = 1;
                    if (menuFlag) {                     // cmpwi r14,0; beq .L_800EEFD8
                        return CBattleMan_ApplyHitStop9(self, (BattleObjAccessor*)acc, evt, tgt);
                    }
                }

                // .L_800EEFD8 -- roll gate (no 0x3374 check)
                s32 rollFlag = 0;                       // li r15, 0
                if (!(evt->field_10 & 0x5)) {            // andi. r0, r0, 0x5; bne .L_800EF034
                    if (isBattleStatusActive((&acc->subObject), 0x20)) {
                        cf::CBattleStateEntry* entry20 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x20);
                        s32 rollVal;
                        if (tgt != nullptr) rollVal = (s32)tgt->field_B4;
                        else rollVal = mtRand__Q22ml4mathFi(100);
                        if (rollVal < entry20->unk10) rollFlag = 1;
                    }
                }
                if (!(evt->field_10 & 0x6)) {            // rlwinm. 0,0,29,30; bne .L_800EF0A4
                    if (!isBattleStatusActive((&acc->subObject), 0x10)) {
                        if (acc->field_3F00 & 0x4) rollFlag = 1;
                    }
                }
                if (rollFlag) {                         // cmpwi r15,0; beq .L_800EF0A4
                    if (tgt != nullptr) {            // cmpwi r26,0; beq .L_800EF084
                        tgt->field_74 |= 0x80000010;  // oris 0x8000; ori 0x10; stw
                    }
                    return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
                }
            }
        }

        // .L_800EF0A4 -- clear event head, fall to shared tail
        evt->field_04 = 0;                               // li r0,0; stw 0x4(r25)
        evt->field_00 = 0;                               // stw r0, 0x0(r25)
        break;                                          // b .L_800F4000
    }

    case 102: {
        if (isBattleStatusActive((&acc->subObject), 0x86)) {
            cf::CBattleStateEntry* entry86 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x86);
            if (entry86->unk10 >= 0x64) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        // .L_800EF0F8 -- field_10 = (s32)((f64)field_10 * round(sum)/100)
        if (evt->field_08 != 0x8000 && tgt != nullptr) {  // cmplwi 0x8000; beq / cmpwi r26; beq
            f32 sum = tgt->field_5C + tgt->field_60;  // fadds f1, f2, f1
            s32 roundVal;
            if (sum < lbl_eu_80666DDC) {
                roundVal = 0;
            } else if (tgt->field_74 & 0x80) {
                roundVal = 0;                               // li r0, 0
            } else {
                roundVal = (s32)((f64)sum + (sum > lbl_eu_80666DDC ? 0.5 : -0.5));
            }
            evt->field_10 = (s32)((f32)(s32)evt->field_10 *
                                 ((f32)(s32)roundVal / 100.0f));  // fsubs+fdivs+fmuls+fctiwz
        }

        // .L_800EF1A4 -- pc skill 0x85: field_10 += field_10/100 * entry
        if (pc != nullptr && isBattleStatusActive(&((EC918_BattleObjAccessor*)pc)->subObject, 0x85)) {
            cf::CBattleStateEntry* entry85 = (cf::CBattleStateEntry*)findBattleStatusEntry(&((EC918_BattleObjAccessor*)pc)->subObject, 0x85);
            s32 entryVal = entry85->unk10;
            evt->field_10 = (s32)(((f32)(s32)evt->field_10 / 100.0f) *
                                 (f32)(s32)entryVal + (f32)(s32)evt->field_10);  // fmadds + fctiwz
        }
        break;                                          // b .L_800F4000
    }

    case 103: {
        if (isBattleStatusActive((&acc->subObject), 0x88)) {
            cf::CBattleStateEntry* entry88 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x88);
            if (entry88->unk10 >= 0x64) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        if (evt->field_08 != 0x8000 && tgt != nullptr) {
            f32 sum = tgt->field_5C + tgt->field_60;
            s32 roundVal;
            if (sum < lbl_eu_80666DDC) {
                roundVal = 0;
            } else if (tgt->field_74 & 0x80) {
                roundVal = 0;
            } else {
                roundVal = (s32)((f64)sum + (sum > lbl_eu_80666DDC ? 0.5 : -0.5));
            }
            evt->field_10 = (s32)((f32)(s32)evt->field_10 *
                                 ((f32)(s32)roundVal / 100.0f));  // fsubs+fdivs+fmuls+fctiwz
        }

        if (pc != nullptr && isBattleStatusActive(&((EC918_BattleObjAccessor*)pc)->subObject, 0x87)) {
            cf::CBattleStateEntry* entry87 = (cf::CBattleStateEntry*)findBattleStatusEntry(&((EC918_BattleObjAccessor*)pc)->subObject, 0x87);
            s32 entryVal = entry87->unk10;
            evt->field_10 = (s32)(((f32)(s32)evt->field_10 / 100.0f) *
                                 (f32)(s32)entryVal + (f32)(s32)evt->field_10);
        }
        if (pc != nullptr && ((cf::CActorParam*)pc)->CActorParam_getStatusTable() != nullptr) {
            s32 bonus;
            if (IdTable_GetSum(((cf::CActorParam*)pc)->CActorParam_getStatusTable(), 0x80, &bonus)) {
                evt->field_20 += (f32)bonus;
            }
        }
        break;                                          // b .L_800F4000
    }

    case 104: {
        if (isBattleStatusActive((&acc->subObject), 0x8A)) {
            cf::CBattleStateEntry* entry8A = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x8A);
            if (entry8A->unk10 >= 0x64) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        if (evt->field_08 != 0x8000 && tgt != nullptr) {
            f32 sum = tgt->field_5C + tgt->field_60;
            s32 roundVal;
            if (sum < lbl_eu_80666DDC) {
                roundVal = 0;
            } else if (tgt->field_74 & 0x80) {
                roundVal = 0;
            } else {
                roundVal = (s32)((f64)sum + (sum > lbl_eu_80666DDC ? 0.5 : -0.5));
            }
            evt->field_10 = (s32)((f32)(s32)evt->field_10 *
                                 ((f32)(s32)roundVal / 100.0f));  // fsubs+fdivs+fmuls+fctiwz
        }

        if (pc != nullptr && isBattleStatusActive(&((EC918_BattleObjAccessor*)pc)->subObject, 0x89)) {
            cf::CBattleStateEntry* entry89 = (cf::CBattleStateEntry*)findBattleStatusEntry(&((EC918_BattleObjAccessor*)pc)->subObject, 0x89);
            s32 entryVal = entry89->unk10;
            evt->field_10 = (s32)(((f32)(s32)evt->field_10 / 100.0f) *
                                 (f32)(s32)entryVal + (f32)(s32)evt->field_10);
        }
        if (pc != nullptr && ((cf::CActorParam*)pc)->CActorParam_getStatusTable() != nullptr) {
            s32 bonus;
            if (IdTable_GetSum(((cf::CActorParam*)pc)->CActorParam_getStatusTable(), 0x7F, &bonus)) {
                evt->field_20 += (f32)bonus;
            }
        }
        break;                                          // b .L_800F4000
    }

    case 105: {
        if (isBattleStatusActive((&acc->subObject), 0x8C)) {
            cf::CBattleStateEntry* entry8C = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x8C);
            if (entry8C->unk10 >= 0x64) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        if (evt->field_08 != 0x8000 && tgt != nullptr) {
            f32 sum = tgt->field_5C + tgt->field_60;
            s32 roundVal;
            if (sum < lbl_eu_80666DDC) {
                roundVal = 0;
            } else if (tgt->field_74 & 0x80) {
                roundVal = 0;
            } else {
                roundVal = (s32)((f64)sum + (sum > lbl_eu_80666DDC ? 0.5 : -0.5));
            }
            evt->field_10 = (s32)((f32)(s32)evt->field_10 *
                                 ((f32)(s32)roundVal / 100.0f));  // fsubs+fdivs+fmuls+fctiwz
        }

        if (pc != nullptr && isBattleStatusActive(&((EC918_BattleObjAccessor*)pc)->subObject, 0x8B)) {
            cf::CBattleStateEntry* entry8B = (cf::CBattleStateEntry*)findBattleStatusEntry(&((EC918_BattleObjAccessor*)pc)->subObject, 0x8B);
            s32 entryVal = entry8B->unk10;
            evt->field_10 = (s32)(((f32)(s32)evt->field_10 / 100.0f) *
                                 (f32)(s32)entryVal + (f32)(s32)evt->field_10);
        }
        if (pc != nullptr && ((cf::CActorParam*)pc)->CActorParam_getStatusTable() != nullptr) {
            s32 bonus;
            if (IdTable_GetSum(((cf::CActorParam*)pc)->CActorParam_getStatusTable(), 0x81, &bonus)) {
                evt->field_20 += (f32)bonus;
            }
        }
        break;                                          // b .L_800F4000
    }

    case 106: {
        if (pc != nullptr && isBattleStatusActive(&((EC918_BattleObjAccessor*)pc)->subObject, 0x4E)) {
            cf::CBattleStateEntry* entry4E = (cf::CBattleStateEntry*)findBattleStatusEntry(&((EC918_BattleObjAccessor*)pc)->subObject, 0x4E);
            s32 entryVal = entry4E->unk10;
            evt->field_10 = (s32)(((f32)(s32)evt->field_10 / 100.0f) *
                                 (f32)(s32)entryVal + (f32)(s32)evt->field_10);  // fmadds + fctiwz
        }
        break;                                          // b .L_800F4000
    }

    case 51: {
        if (artsData != nullptr) {                    // cmpwi r16,0; beq .L_800EF880
            // (chunk 0) s32 artsLevel = ((cf::CObjectState*)artsData)->checkStateFlags(0);
            s32 artsLevel = ((cf::CObjectState*)artsData)->checkStateFlags(0);

            // 0x800EF860..0x800EF87C (chunk 1):
            //   lbz r4, 0x6f(r16); subi r3, r3, 1; lha r0, 0x4a(r16)
            //   mullw r3, r4, r3; add r0, r0, r3; mulli r0, r0, 0xa
            //   sth r0, 0x14(r25); b .L_800F4000
            evt->field_14 = (s16)(((s16)((cf::CAttackParamPrefix*)artsData)->unk4A +
                                  (s32)((cf::CAttackParamPrefix*)artsData)->unk6C[3] *
                                      (artsLevel - 1)) * 10);
        } else {
            // .L_800EF880: acc->CActorParam_getDamageScale() float scale -> field_14
            //   f1 = vf12C(acc); (f32)(s32)field_10 / 100.0f * f1; fctiwz
            evt->field_14 = (s16)((f32)(s32)evt->field_10 / 100.0f *
                                 ((cf::CActorParam*)acc)->CActorParam_getDamageScale());
        }
        break;                                      // b .L_800F4000
    }

    case 206: {
        if (evt->field_10 != 0) break;               // bne .L_800F4000
        if (artsData != nullptr) {                  // beq .L_800F4000
            evt->field_10 = ((cf::CObjectState*)artsData)->checkStateFlags(0);          // stw r3, 0x10(r25)
        }
        break;                                      // b .L_800F4000
    }

    case 54: {
        if (artsData != nullptr) {                  // beq .L_800F4000
            // subi r3, r3, 1; lha r0, 0x14(r25); extsh; add; sth
            evt->field_14 = (s16)((s32)evt->field_14 + (((cf::CObjectState*)artsData)->checkStateFlags(0) - 1));
        }
        break;                                      // b .L_800F4000
    }

    case 82: {
        if (((cf::CActorParam*)acc)->CActorParam_getStatusTable() != nullptr) {  // beq .L_800F4000
            if (Counter_TestBit(((cf::CActorParam*)acc)->CActorParam_getStatusTable(), 0x7B)) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
            if (Counter_TestBit(((cf::CActorParam*)acc)->CActorParam_getStatusTable(), 0x66)) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                      // b .L_800F4000
    }

    case 84: {
        if (((cf::CActorParam*)acc)->CActorParam_getStatusTable() != nullptr) {  // beq .L_800F4000
            if (Counter_TestBit(((cf::CActorParam*)acc)->CActorParam_getStatusTable(), 0x7D)) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
            if (Counter_TestBit(((cf::CActorParam*)acc)->CActorParam_getStatusTable(), 0x66)) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                      // b .L_800F4000
    }

    case 83:
    case 87: {
        if (((cf::CActorParam*)acc)->CActorParam_getStatusTable() != nullptr) {  // beq .L_800F4000
            if (Counter_TestBit(((cf::CActorParam*)acc)->CActorParam_getStatusTable(), 0x66)) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                      // b .L_800F4000
    }

    case 60:

    case 61: {
        if (((cf::CActorParam*)acc)->CActorParam_getStatusTable() != nullptr) {  // beq .L_800F4000
            if (Counter_TestBit(((cf::CActorParam*)acc)->CActorParam_getStatusTable(), 0x66)) {
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }
        break;                                      // b .L_800F4000
    }

    case 197: {
        if (acc != nullptr &&                  // cmpwi r24,0; beq tail
            ((cf::CActorParam*)acc)->CActorParam_isBattleLocked() == 0) {        // bne tail
            f32 factor = 100.0f * CBattleMan_ApplyStatusRate((u8*)pc, tgt);  // fmuls f4, f0(100), f1
            // fctiwz((f64)(s32)field * factor) for each field
            evt->field_10 = (s32)((f32)(s32)evt->field_10 * factor);  // stw
            evt->field_14 = (s16)((f32)(s32)evt->field_14 * factor);  // sth
            evt->field_16 = (s16)((f32)(s32)evt->field_16 * factor);  // sth
        }
        break;                                      // b .L_800F4000
    }

    case 198: {
        f32 factor = 100.0f * CBattleMan_ApplyStatusRate((u8*)pc, tgt);
        evt->field_10 = (s32)((f32)(s32)evt->field_10 * factor);
        evt->field_14 = (s16)((f32)(s32)evt->field_14 * factor);
        evt->field_16 = (s16)((f32)(s32)evt->field_16 * factor);
        break;                                      // b .L_800F4000
    }

    case 212: {
        if (evt->field_10 != 0) {                    // beq .L_800EFD60
            // field_10 != 0 path (0x800EFC60..0x800EFD5C)
            CfEnumListHolder holder;
            startEnumObjects(holder.get(), 0x80000000, 0);  // lis r4,0x8000
            for (u32 i = 0; i < holder.get()->mPtrCount; i++) {
                void* actorAcc = MapFx_ResolveOwner(getObjectAt(holder.get(), i));
                // ((cf::CActorParam*)actorAcc)->CActorParam_findGaugeEntry(acc->3F10) -> hit result (or 0)
                void* res = ((cf::CActorParam*)actorAcc)->CActorParam_findGaugeEntry(acc->field_3F10);
                if (res != nullptr) {               // beq .L_800EFD38
                    // Apply the percentage independently to each vector component.
                    *(f32*)((u8*)res + 0x10) *=
                        lbl_eu_80666DD4 - (f32)(s32)evt->field_10 / 100.0f;
                    *(f32*)((u8*)res + 0x00) *=
                        lbl_eu_80666DD4 - (f32)(s32)evt->field_10 / 100.0f;
                    *(f32*)((u8*)res + 0x04) *=
                        lbl_eu_80666DD4 - (f32)(s32)evt->field_10 / 100.0f;
                }
            }
        } else {
            // .L_800EFD60: field_10 == 0 path
            CBattleMan_SendActorEvent((cf::CBattleManager*)self, (BattleObjAccessor*)pc);                // mr r3,r22; mr r4,r23; bl
        }
        break;                                      // b .L_800F4000
    }

    case 213:
    case 263: {
        if (((cf::CActorParam*)acc)->CActorParam_isBattleLocked() != 0) break;   // bne .L_800F4000
        if (isBattleStatusActive((&acc->subObject), 0x120)) break;  // bne .L_800F4000

        s32 dmgBase = evt->field_10;                     // lwz r17, 0x10(r25)
        if (evt->field_08 == 0x8000) {               // cmplwi r0,0x8000; bne .L_800EFDE4
            // calcBattleDamage(pc, acc, tgt); r17 = (s32)((f32)(s32)field_10 * f1)
            dmgBase = (s32)((f32)(s32)evt->field_10 * calcBattleDamage(pc, acc, tgt));
        }

        f32 f28 = CBattleMan_ApplyStatusRate((u8*)pc, tgt);        // .L_800EFDE4: fmr f28, f1
        if (pc != nullptr) {                        // cmpwi r23,0; beq .L_800EFE98
            u32 accObjId = acc->field_3F10;         // lwz r14, 0x3f10(r24)
            f32 prod = lbl_eu_80666DDC;
            if (artsData != nullptr) {              // beq .L_800EFE44
                // lhz r15, 0x46(r16); ((cf::CActorParam*)pc)->CActorParam_getActorLevel(); addi +0xe; mullw
                u16 artsId = ((cf::CAttackParamPrefix*)artsData)->unk46;
                s32 level = ((cf::CActorParam*)pc)->CActorParam_getActorLevel();
                prod = f28 * (f32)(s32)((s32)artsId * (level + 14));
            }
            // .L_800EFE44: prod = lbl_eu_80666DDC (when artsData == null)
            s32 broadcastArg6 = (s32)prod;                     // fctiwz; lwz 0x854(r1)
            // r5 = (s32)(1.25f * (f32)(s32)r17 * f28)
            s32 broadcastArg5 = (s32)(k1_25f * (f32)(s32)dmgBase * f28);
            // broadcastBattleDamage(self, pc, r5, r6, 0, 1, acc->3F10)
            broadcastBattleDamage((void*)(uintptr_t)self, pc, broadcastArg5, broadcastArg6, 0, 1,
                          (void*)(uintptr_t)accObjId);
        }

        // .L_800EFE98: ((cf::CfObjectActor*)acc)->CfObjectActor_takeDamageValue((f32)(s32)r17, tgt->field_74)
        {
            ((cf::CfObjectActor*)acc)->CfObjectActor_takeDamageValue((f32)(s32)dmgBase, tgt->field_74);  // lwz r4, 0x74(r26)
        }
        break;                                      // b .L_800F4000
    }

    case 214: {
        if (((cf::CActorParam*)acc)->CActorParam_isBattleLocked() != 0) {        // beq .L_800F4000 (body runs when != 0)
            // r4 = acc != 0 ? acc + 0x3E9C : acc(0); bindIndexedEffect(0x4C, r4)
            void* obj = static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)acc);
            bindIndexedEffect(0x4C, (int)(obj));

            s32 scaleInt = (s32)((cf::CActorParam*)acc)->CActorParam_getDamageScale();  // fctiwz; lwz 0x85c(r1)
            ((cf::CActorParam*)acc)->CActorParam_resetActorState(0);            // ecVfAC(acc, 0)

            BtlDmg_SetDamageType(3);                       // li r3,3; bl
            BtlDmg_SetDamageDir(0);                       // li r3,0; bl
            BtlDmg_NotifyDamage((void*)(uintptr_t)acc->field_3F10, (u32)scaleInt);  // lwz 3f10; mr r4,r14; bl
        }
        break;                                      // b .L_800F4000
    }

    case 215: {
        if (((cf::CActorParam*)acc)->CActorParam_isBattleLocked() != 0) break;   // bne .L_800F4000

        evt->prevEventType = evt->eventType;          // lhz + sth 0x2e
        evt->field_30 = copyBattleEvent(eventWorkspace.case215Event, *evt) | 2;

        for (u32 i = 0; i < 0x20; i++) {            // li r14,0; cmpwi r14,0x20; blt
            cf::CBattleStateEntry* evt2 = (cf::CBattleStateEntry*)static_cast<cf::CBattleState*>((cf::CfObjectActor*)acc)->CBattleState_getEventEntry(i);  // slot 0x5C
            // prevEventType classifier 0 AND (field_08 & 0x7000) == 0
            if (isBattleEventKind3(evt2->unk2E) == 0 &&
                (evt2->unk08 & 0x7000) == 0) {   // rlwinm 17-19
                static_cast<cf::CBattleState*>((cf::CfObjectActor*)acc)->CBattleState_applyEventEntry(evt2);   // slot 0x24
            }
        }

        eventWorkspace.case215Event.eventType = 0x27;                    // li r3,0x27; sth 0x770(r1)
        eventWorkspace.case215Event.field_10 = 0x64;                     // li r0,0x64; stw 0x774(r1)
        func_800EC918(self, pc, acc, &eventWorkspace.case215Event, tgt);
        break;                                      // b .L_800F4000
    }

    case 216: {
        if (((cf::CActorParam*)acc)->CActorParam_isBattleLocked() != 0) break;   // bne .L_800F4000

        s32 f10 = evt->field_10;                     // lwz r4, 0x10(r25)
        if (f10 == -1) {                            // cmpwi r4,-1; bne .L_800F019C
            // Start at a random slot and wrap through all 32 signed indices.
            s32 start = rand() % 32;
            for (s32 i = 0; i < 0x20; i++) {        // cmpwi r14,0x20; blt
                cf::CBattleStateEntry* evt2 = (cf::CBattleStateEntry*)static_cast<cf::CBattleState*>((cf::CfObjectActor*)acc)->CBattleState_fetchStatusEntry((start + i) % 32);
                if (evt2->unk0C != 0 &&
                    (evt2->unk2E == 0 ||
                     isBattleEventKind3(evt2->unk2E) == 0) &&
                    (evt2->unk08 & 0x7000) == 0) {
                    static_cast<cf::CBattleState*>((cf::CfObjectActor*)acc)->CBattleState_applyEventEntry(evt2);   // slot 0x24
                    if (evt->field_14 != 0) {        // lha 0x14; beq tail
                        // ((cf::CActorParam*)pc)->CActorParam_addArtsGauge((f32)(s32)field_14 * (vf15C(pc)/100))
                        f32 f = ((cf::CActorParam*)pc)->CActorParam_getArtsScale() / 100.0f;
                        ((cf::CActorParam*)pc)->CActorParam_addArtsGauge((f32)(s32)evt->field_14 * f);
                    }
                    break;   // retail: b .L_800F4000 (success exits the case)
                }
            }
        } else if (f10 == 0) {                     // cmpwi r4,0; bne .L_800F0274
            for (u32 i = 0; i < 0x20; i++) {
                cf::CBattleStateEntry* evt2 = (cf::CBattleStateEntry*)static_cast<cf::CBattleState*>((cf::CfObjectActor*)acc)->CBattleState_fetchStatusEntry(i);
                if (evt2->unk0C != 0 &&
                    (evt2->unk2E == 0 ||
                     isBattleEventKind3(evt2->unk2E) == 0) &&
                    (evt2->unk08 & 0x7000) == 0) {
                    if (evt->field_14 != 0) {        // lha 0x14; beq .L_800F024C
                        f32 f = ((cf::CActorParam*)pc)->CActorParam_getArtsScale() / 100.0f;
                        ((cf::CActorParam*)pc)->CActorParam_addArtsGauge((f32)(s32)evt->field_14 * f);
                    }
                    static_cast<cf::CBattleState*>((cf::CfObjectActor*)acc)->CBattleState_applyEventEntry(evt2);   // slot 0x24
                }
            }
        } else {
            // .L_800F0274: ((cf::CBattleState*)sub)->CBattleState_clearStatusId(field_10) -- clear that status id
            static_cast<cf::CBattleState*>((cf::CfObjectActor*)acc)->CBattleState_clearStatusId((u32)f10);  // slot 0x20
            if (evt->field_14 != 0) {                // lha 0x14; beq tail
                f32 f = ((cf::CActorParam*)pc)->CActorParam_getArtsScale() / 100.0f;
                ((cf::CActorParam*)pc)->CActorParam_addArtsGauge((f32)(s32)evt->field_14 * f);
            }
        }
        break;                                      // b .L_800F4000
    }

    case 217: {
        if (((cf::CActorParam*)acc)->CActorParam_isBattleLocked() != 0) break;   // bne .L_800F4000
        ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(0x111);              // slot 0x20, li r4,0x111
        ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(0x112);
        ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(0x113);
        ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(0x114);
        ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(0x115);
        break;                                      // b .L_800F4000
    }

    case 218: {
        void* hateOwner = MapFx_ResolveOwner(findObjectById(evt->field_00));  // bl 800B708C; bl 8016FE34
        f32 f28 = CBattleMan_ApplyStatusRate((u8*)pc, tgt);        // fmr f28, f1
        s32 level = ((cf::CActorParam*)pc)->CActorParam_getActorLevel();             // addi r4, r3, 0xe
        s32 product = evt->field_10 * (level + 14);  // mullw
        // ((cf::CActorParam*)acc)->CActorParam_addHate(r14, lbl_eu_80666DDC, f28 * (f32)(s32)product, lbl_eu_80666DDC)
        ((cf::CfObjectActor*)acc)->CActorParam_addHate(hateOwner, lbl_eu_80666DDC, f28 * (f32)(s32)product, lbl_eu_80666DDC);
        break;                                      // b .L_800F4000
    }

    case 219: {
        void* hateOwner = MapFx_ResolveOwner(findObjectById(evt->field_00));
        s32 level = ((cf::CActorParam*)pc)->CActorParam_getActorLevel();
        // r17 = field_10 * 100 + (level - 1) * 120   (mulli 0x64 / mulli 0x78)
        s32 hateBase = evt->field_10 * 100 + (level - 1) * 120;

        f32 f28 = CBattleMan_ApplyStatusRate((u8*)pc, tgt);        // fmr f28, f1
        if (pc != nullptr) {                        // cmpwi r23,0; beq .L_800F051C
            if (((cf::CActorParam*)pc)->CActorParam_getStatusTable() != nullptr) {    // beq .L_800F051C
                s32 sv;
                // IdTable_GetSum(((cf::CActorParam*)pc)->CActorParam_getStatusTable(), 0x67, &sv) -> f28 += sv/100
                if (IdTable_GetSum(((cf::CActorParam*)pc)->CActorParam_getStatusTable(), 0x67, &sv)) {
                    f28 += (f32)(s32)sv / 100.0f;
                }
                // .L_800F04B0: 0x68 bonus only when acc->3E9C ecVf4C() != pc->3F10
                if (IdTable_GetSum(((cf::CActorParam*)pc)->CActorParam_getStatusTable(), 0x68, &sv)) {
                    if ((u32)((cf::CfObjectActor*)acc)->CObjectParam_getSelfObjectId() != ((cf::CfObjectActor*)pc)->field_0x74) {
                        f28 += (f32)(s32)sv / 100.0f;
                    }
                }
            }
            // .L_800F051C: status 0xEE on pc -> +0.25
            if (isBattleStatusActive((SubAccessor*)((u8*)pc + 8), 0xEE)) {
                f28 += k0_25f;                      // lfs 80666E1C; fadds
            }
        }

        // .L_800F0538: r15 = (s32)((f32)(s32)r17 * f28)
        s32 hateTotal = (s32)((f32)(s32)hateBase * f28);

        if (isBattleStatusActive((SubAccessor*)((u8*)pc + 8), 0xED)) {
            // .L_800F0570: status 0xED path -- AOE via filtered actor list
            f32 f28b = (artsData != nullptr) ? ((cf::CAttackParamPrefix*)artsData)->unk60
                                             : lbl_eu_80666DDC;
            CfEnumListHolder holder;
            startEnumObjects(holder.get(), 0x100, 0);  // li r4,0x100; li r5,0

            void* idObj = ((cf::CfObjectActor*)pc)->CfObject_getPosVector();   // slot 0xAC

            // __ct__800FB044(list, f28b, idObj, 8)
            __ct__800FB044(holder.get(), f28b, idObj, 8);

            for (u32 i = 0; i < holder.get()->mPtrCount; i++) {
                void* actorAcc = MapFx_ResolveOwner(getObjectAt(holder.get(), i));
                // ((cf::CActorParam*)actorAcc)->CActorParam_addHate(pc, lbl_eu_80666DDC, lbl_eu_80666DDC, (f32)(s32)r15)
                ((cf::CfObjectActor*)actorAcc)->CActorParam_addHate(pc, lbl_eu_80666DDC, lbl_eu_80666DDC, (f32)(s32)hateTotal);
            }
        } else {
            // .L_800F0644: ((cf::CActorParam*)acc)->CActorParam_addHate(r14, lbl_eu_80666DDC, lbl_eu_80666DDC, (f32)(s32)r15)
            ((cf::CfObjectActor*)acc)->CActorParam_addHate(hateOwner, lbl_eu_80666DDC, lbl_eu_80666DDC, (f32)(s32)hateTotal);
        }

        // .L_800F0678: ((cf::CActorParam*)pc)->CActorParam_setTensionPoints(lbl_eu_80666DDC)
        ((cf::CActorParam*)pc)->CActorParam_setTensionPoints(lbl_eu_80666DDC);
        break;                                      // b .L_800F4000
    }

    case 220: {
        // Skill roll: status 0x26
        if (isBattleStatusActive((&acc->subObject), 0x26)) {
            cf::CBattleStateEntry* entry26 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x26);
            s32 rollVal;
            if (tgt != nullptr) rollVal = (s32)tgt->field_B4;  // cmpwi r26,0
            else rollVal = mtRand__Q22ml4mathFi(100);
            if (rollVal < entry26->unk10) {  // cmpw; bge skip
                return CBattleMan_ApplyHitStop10(self, (BattleObjAccessor*)acc, evt, tgt);
            }
        }

        // .L_800F06F4: tgt != 0 path
        if (tgt != nullptr) {
            // r5 = field_B0; 0x51EB851F fixed-point -> (s32)(field_B0 % 100)
            if ((s32)(tgt->field_B0 % 100) >= evt->field_10) {  // cmpw; blt .L_800F0734
                return 0;                           // li r3,0; b .L_800F41D4
            }
            // .L_800F0734
            if (artsData != nullptr && ((cf::CAttackParamPrefix*)artsData)->unk3C == 3) {
                if (pc != acc) {               // cmplw r23,r24; beq .L_800F0780
                    // vf11C(acc, -vf12C(acc))
                    ((cf::CActorParam*)acc)->CActorParam_addHp(-((cf::CActorParam*)acc)->CActorParam_getDamageScale());
                } else {
                    ((cf::CActorParam*)acc)->CActorParam_setHp(lbl_eu_80666DDC);  // lfs 0.0; slot 0x118
                }
            } else {
                // .L_800F079C
                if (pc != acc) {               // beq .L_800F07E4
                    if (evt->field_30 & 0x20000) {   // rlwinm 0,14,14; beq .L_800F084C
                        // ((cf::CfObjectActor*)acc)->CfObjectActor_takeDamageValue(-vf12C(acc), 0x90000000)
                        ((cf::CfObjectActor*)acc)->CfObjectActor_takeDamageValue(-((cf::CActorParam*)acc)->CActorParam_getDamageScale(), 0x90000000);
                    }
                } else {
                    ((cf::CActorParam*)acc)->CActorParam_setHp(lbl_eu_80666DDC);
                }
            }
        } else {
            // .L_800F0800: tgt == 0 path -- plain rand(100) gate
            if (mtRand__Q22ml4mathFi(100) >= evt->field_10) {
                return 0;                           // li r3,0; b .L_800F41D4
            }
            // .L_800F081C: ((cf::CfObjectActor*)acc)->CfObjectActor_takeDamageValue(-vf128(acc), 0x90000000)
            ((cf::CfObjectActor*)acc)->CfObjectActor_takeDamageValue(-((cf::CActorParam*)acc)->CActorParam_getHp(), 0x90000000);
        }

        // .L_800F084C: status 0x33 clear
        if (isBattleStatusActive((&acc->subObject), 0x33)) {
            ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(0x33);           // slot 0x20
        }
        if (tgt != nullptr) {
            tgt->field_74 |= 0x90000000;         // oris r0,r0,0x9000; stw 0x74
            isBattleStatusActive((&acc->subObject), 0xFC);  // results unused
            isBattleStatusActive((&acc->subObject), 0x100);
        }
        // .L_800F08A4: when acc == pc, flag 0x04000000 + event bit 0x800
        if (acc == pc) {                       // cmplw r24,r23; bne tail
            acc->field_3374 |= 0x04000000;     // oris 0x400
            evt->field_30 |= 0x800;                  // ori 0x800
        }
        break;                                      // b .L_800F4000
    }

    case 221: {
        if (pc == nullptr) break;                   // beq .L_800F4000

        // f28 = tgt->field_58 + calcBattleDamage(pc, acc, tgt)
        f32 f28 = tgt->field_58 + calcBattleDamage(pc, acc, tgt);

        // r17 = (s32)((f32)(s32)field_10 * vf12C(pc) / 100)
        s32 pcAddedHp = (s32)((f32)(s32)evt->field_10 * ((cf::CActorParam*)pc)->CActorParam_getDamageScale() / 100.0f);
        // r14 = (s32)((f32)(s32)field_14 * vf12C(pc) / 100)  (vf12C called again)
        s32 pcSubHp = (s32)((f32)(s32)evt->field_14 * ((cf::CActorParam*)pc)->CActorParam_getDamageScale() / 100.0f);
        // retail 0x800F097C: r17 is re-scaled by f28 (fmuls f0, f0, f28; fctiwz)
        pcAddedHp = (s32)((f32)(s32)pcAddedHp * f28);

        ((cf::CActorParam*)acc)->CActorParam_addHp((f32)(s32)pcAddedHp);   // slot 0x11C on acc
        ((cf::CActorParam*)pc)->CActorParam_addHp((f32)(s32)(-pcSubHp));      // neg r0,r14 on pc

        f32 dmg = CBattleMan_ApplyStatusRate((u8*)pc, tgt);        // fmr f28, f1 (overwrites f28)
        if (pc != nullptr) {                        // cmpwi r23,0; beq tail
            u32 accObjId = acc->field_3F10;        // lwz r14, 0x3f10(r24)
            f32 prod = lbl_eu_80666DDC;
            if (artsData != nullptr) {              // beq .L_800F0A40
                u16 artsId = ((cf::CAttackParamPrefix*)artsData)->unk46;  // lhz r15, 0x46(r16)
                s32 level = ((cf::CActorParam*)pc)->CActorParam_getActorLevel();
                prod = dmg * (f32)(s32)((s32)artsId * (level + 14));
            }
            s32 broadcastArg6 = (s32)prod;                     // fctiwz; lwz 0x854(r1)
            s32 broadcastArg5 = (s32)(k1_25f * (f32)(s32)pcAddedHp * dmg);
            broadcastBattleDamage((void*)(uintptr_t)self, pc, broadcastArg5, broadcastArg6, 0, 1,
                          (void*)(uintptr_t)accObjId);
        }
        break;                                      // b .L_800F4000
    }

    case 282: {
        if (pc == nullptr) break;                   // beq .L_800F4000
        ((cf::CActorParam*)acc)->CActorParam_addArtsGauge((f32)(s32)evt->field_10);  // slot 0x154
        break;                                      // b .L_800F4000
    }

    case 222: {
        // fmuls f1, f2(field_14), f1(vf12C); fdivs /100; fneg
        ((cf::CActorParam*)acc)->CActorParam_addHp(-((f32)(s32)evt->field_14 * ((cf::CActorParam*)acc)->CActorParam_getDamageScale() / 100.0f));
        ((cf::CActorParam*)acc)->CActorParam_addArtsGauge((f32)(s32)evt->field_10);
        break;                                      // b .L_800F4000
    }

    case 269: {
        ((cf::CActorParam*)acc)->CActorParam_addHp(-((f32)(s32)evt->field_14 * ((cf::CActorParam*)acc)->CActorParam_getHp() / 100.0f));
        ((cf::CActorParam*)acc)->CActorParam_addArtsGauge((f32)(s32)evt->field_10);
        break;                                      // b .L_800F4000
    }

    case 223: {
        if (pc != nullptr && tgt != nullptr) {   // beq .L_800F0CD0
            if (evt->field_08 == 0x8000) {           // cmplwi r0,0x8000; bne .L_800F0C98
                // r14 = field_10 + rand() % (field_14 - field_10 + 1)
                s32 eventHp = evt->field_10 + (rand() % (evt->field_14 - evt->field_10 + 1));
                ((cf::CActorParam*)acc)->CActorParam_addHp((f32)(s32)(-eventHp));  // neg r0,r14
                f32 f = calcBattleDamage(pc, pc, tgt);
                eventHp = (s32)((f32)(s32)eventHp * f);    // fctiwz
                ((cf::CActorParam*)pc)->CActorParam_addHp((f32)(s32)eventHp);
            } else {
                // .L_800F0C98: vf11C(pc, field_64); vf11C(acc, -field_60)
                ((cf::CActorParam*)pc)->CActorParam_addHp(tgt->field_64);   // lfs f1, 0x64(r26)
                ((cf::CActorParam*)acc)->CActorParam_addHp(-tgt->field_60);  // fneg f1, f0
            }
        } else {
            // .L_800F0CD0 (pc==0 or tgt==0): rand range + field_58 add
            s32 eventHp = evt->field_10 + (rand() % (evt->field_14 - evt->field_10 + 1));
            ((cf::CActorParam*)acc)->CActorParam_addHp((f32)(s32)(-eventHp));
            f32 f = calcBattleDamage(pc, pc, tgt);
            // r14 = (s32)((f32)(s32)r14 * (tgt->field_58 + f))
            eventHp = (s32)((f32)(s32)eventHp * (tgt->field_58 + f));
            ((cf::CActorParam*)pc)->CActorParam_addHp((f32)(s32)eventHp);
        }
        break;                                      // b .L_800F4000
    }

    case 224: {
        if (pc != nullptr && tgt != nullptr) {   // beq .L_800F0DCC
            ((cf::CActorParam*)pc)->CActorParam_addHp(tgt->field_64);
            ((cf::CActorParam*)acc)->CActorParam_addHp(-tgt->field_60);
        } else {
            // .L_800F0DCC: r14 = (s32)(0.01f * (f32)(s32)field_10 * vf12C(acc))
            s32 eventHp = (s32)(0.01f * (f32)(s32)evt->field_10 *
                           ((cf::CActorParam*)acc)->CActorParam_getDamageScale());
            ((cf::CActorParam*)acc)->CActorParam_addHp((f32)(s32)(-eventHp));
            f32 f = calcBattleDamage(pc, pc, tgt);
            eventHp = (s32)((f32)(s32)eventHp * (tgt->field_58 + f));
            ((cf::CActorParam*)pc)->CActorParam_addHp((f32)(s32)eventHp);
        }
        break;                                      // b .L_800F4000
    }

    case 225: {
        if (tgt != nullptr) {                    // beq .L_800F4000
            // f1 = 0.01f * field_5C * (f32)(s32)field_10
            f32 v = 0.01f * tgt->field_5C * (f32)(s32)evt->field_10;
            // fcmpo vs 0.0; fadd +/-0.5 (f64); fctiwz -- round to nearest
            s32 eventHp = (s32)((f64)v + ((v > lbl_eu_80666DDC) ? 0.5 : -0.5));
            f32 f = calcBattleDamage(pc, pc, tgt);
            s32 scaledHp = (s32)((f32)(s32)eventHp * f);
            ((cf::CActorParam*)pc)->CActorParam_addHp((f32)(s32)scaledHp);
        }
        break;                                      // b .L_800F4000
    }

    case 226: {
        if (pc == nullptr) break;                   // beq .L_800F4000
        if (acc == pc) break;                  // cmplw r24,r23; beq tail
        s32 gaugeAmt;
        if (tgt != nullptr) {                    // cmpwi r26,0; beq .L_800F0F90
            // divw remainder: r14 = field_10 + (field_B0 % (field_14-field_10+1))
            gaugeAmt = evt->field_10 +
                  ((s32)tgt->field_B0 % (evt->field_14 - evt->field_10 + 1));
        } else {
            gaugeAmt = evt->field_10 + (rand() % (evt->field_14 - evt->field_10 + 1));
        }
        // .L_800F0FB4
        ((cf::CActorParam*)acc)->CActorParam_addArtsGauge((f32)(s32)(-gaugeAmt));  // neg r0,r14; slot 0x154
        ((cf::CActorParam*)pc)->CActorParam_addArtsGauge((f32)(s32)gaugeAmt);
        break;                                      // b .L_800F4000
    }

    case 234: {
        evt->prevEventType = evt->eventType;          // sth r4, 0x2e(r25)  (r4 = eventType)
        evt->field_30 |= 2;                          // ori r0,r0,2; stw 0x30(r25)
        void* res = ((cf::CActorParam*)acc)->CActorParam_getArtsSet();       // slot 0x27C
        *(u16*)res = 1;                             // li r0,1; sth r0, 0x0(r3)
        break;                                      // b .L_800F4000
    }

    case 235: {
        if (isBattleStatusActive((&acc->subObject), 0xEB)) {
            ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(0xEB);           // slot 0x20
            return 0;                               // li r3,0; b .L_800F41D4
        }
        // .L_800F1070
        evt->prevEventType = evt->eventType;          // lhz + sth 0x2e
        evt->field_30 = copyBattleEvent(eventWorkspace.case235Event, *evt) | 2;
        eventWorkspace.case235Event.eventType = 0x32;                    // li r8,0x32; sth 0x73c(r1)
        eventWorkspace.case235Event.field_10 = 0;                        // li r0,0; stw 0x740(r1)
        func_800EC918(self, pc, acc, &eventWorkspace.case235Event, tgt);
        break;                                      // b .L_800F4000
    }

    case 237: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case237Event, *evt) | 2;

        eventWorkspace.case237Event.eventType = 0x58;                 // sth r0(0x58), +0x0C
        func_800EC918(self, pc, acc, &eventWorkspace.case237Event, tgt);

        eventWorkspace.case237Event.eventType = 0x95;                 // li r3,0x95
        eventWorkspace.case237Event.field_10 = 0x78;                  // li r0,0x78
        func_800EC918(self, pc, acc, &eventWorkspace.case237Event, tgt);

        eventWorkspace.case237Event.eventType = 0x3C;                 // li r3,0x3C
        eventWorkspace.case237Event.field_10 = 0x19;                  // li r0,0x19
        func_800EC918(self, pc, acc, &eventWorkspace.case237Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 238: {
        if (pc == nullptr) {
            break; // cmpwi r23,0; beq .L_800F4000
        }
        evt->prevEventType = evt->eventType;

        // eventWorkspace.case238PlayerEvent (stack 0x6C8): plain copy; field_30 keeps the pre-|2 value.
        copyBattleEventPair(eventWorkspace.case238PlayerEvent,
                            eventWorkspace.case238ActorEvent, *evt, 2);

        // eventWorkspace.case238ActorEvent (stack 0x694): copy with field_30 |= 2, eventType 0x11,
        // field_10 = pc->field_3F10.
        eventWorkspace.case238ActorEvent.eventType = 0x11;                    // li r14,0x11
        eventWorkspace.case238ActorEvent.field_10 = ((cf::CfObjectActor*)pc)->field_0x74;
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case238ActorEvent, tgt)) {
            return 0; // cmpwi r3,0; beq -> li r3,0; b .L_800F41D4
        }

        // acc->field_3ED4 holder slot 0x70 called TWICE (CfObject UVF8;
        // retail passes pc->field_3F10 in r4 - hidden arg, see above).
        // (retail has no null check here).
        ((cf::CfObject*)((void*)((cf::CfObjectActor*)acc)->mSubObj38))->CfObject_notifyEventDone();

        // eventType 0xEE recursion with pc=0 and acc=pc (!).
        eventWorkspace.case238PlayerEvent.eventType = 0xEE;                    // li r0,0xEE
        func_800EC918(self, 0, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case238PlayerEvent, tgt);

        // eventType 0x11 recursion with field_10 = acc->field_3F10,
        // field_30 |= 0x800; acc=pc again.
        eventWorkspace.case238PlayerEvent.eventType = 0x11;                    // sth r14, +0x0C
        eventWorkspace.case238PlayerEvent.field_10 = ((cf::CfObjectActor*)acc)->field_0x74;
        eventWorkspace.case238PlayerEvent.field_30 |= 0x800;                   // ori r0,r0,0x800
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case238PlayerEvent, tgt);
        return 1; // li r3,1; b .L_800F41D4
    }

    case 253: {
        evt->prevEventType = evt->eventType;
        s16 savedField16 = evt->field_16;         // lha r14, 0x16(r25)
        evt->field_30 = copyBattleEvent(eventWorkspace.case253Event, *evt) | 2;
        eventWorkspace.case253Event.eventType = 0x44;                 // li r0,0x44
        func_800EC918(self, pc, acc, &eventWorkspace.case253Event, tgt);

        // eventType 0x6F with field_10 = pcStat->+0x30 (s16) * 2
        eventWorkspace.case253Event.eventType = 0x6F;                 // li r0,0x6F
        {
            void* pcStat = ((cf::CActorParam*)pc)->CActorParam_getBattleParams();
            eventWorkspace.case253Event.field_10 = (s32)((cf::CActorParam_UnkStruct3*)pcStat)->field_30 * 2; // lha+slwi
        }
        func_800EC918(self, pc, acc, &eventWorkspace.case253Event, tgt);

        // eventType 0x97 with field_10=0x14, field_14=2,
        // field_16=original field_16, field_24=1.0f
        eventWorkspace.case253Event.eventType = 0x97;                 // li r3,0x97
        eventWorkspace.case253Event.field_10 = 0x14;                  // li r6,0x14
        eventWorkspace.case253Event.field_14 = 2;                     // li r0,0x2
        eventWorkspace.case253Event.field_16 = savedField16;          // sth r14, +0x16
        eventWorkspace.case253Event.field_24 = lbl_eu_80666DD4;                 // stfs f0(1.0f), +0x24
        func_800EC918(self, pc, acc, &eventWorkspace.case253Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 243: {
        // f28 = artsData ? artsData->+0x60 : lbl_eu_80666DDC   (r16 = artsData)
        // r14 = artsData ? artsData->+0x46 : 0      (arts id)
        f32 f28;
        u32 artsId;
        void* artsData = (tgt != nullptr) ? tgt->artsData : nullptr;
        if (artsData != nullptr) {
            f28 = ((cf::CAttackParamPrefix*)artsData)->unk60;
            artsId = ((cf::CAttackParamPrefix*)artsData)->unk46;
        } else {
            f28 = lbl_eu_80666DDC;
            artsId = 0;
        }

        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case243Event, *evt) | 2;
        eventWorkspace.case243Event.eventType = 0x58;                 // li r0,0x58

        // Single enum-list holder reused by all three passes (retail r1+0x58);
        // destructor once at the end.
        {
            CfEnumListHolder holder;

            // Pass 1 (0x800F14B4-0x800F15C0): filter (0x20, 0x800) then
            // __ct__800FB044(list, f28, pc->subIdent->vfAC(), 0);
            // apply event 0x58 to every listed actor via recursion.
            {
                cf::CfObjEnumList* list = holder.get();
                startEnumObjects(list, 0x20, 0x800);

                void* idObj = ((cf::CfObjectActor*)pc)->CfObject_getPosVector();

                list = holder.get();
                __ct__800FB044(list, f28, idObj, 0);

                for (u32 i = 0;; i++) {
                    cf::CfObjEnumList* l = holder.get();
                    u32 count = l->mPtrCount;
                    if (i >= count) break;
                    void* actorAcc = MapFx_ResolveOwner(getObjectAt(holder.get(), i));
                    func_800EC918(self, pc, (EC918_BattleObjAccessor*)actorAcc,
                                  &eventWorkspace.case243Event, tgt);
                }
            }

            // Pass 2 (0x800F15C4-0x800F1610): same list, subAcc->vf20(0x0B) on
            // every actor except the acc itself.
            {
                for (u32 i = 0;; i++) {
                    cf::CfObjEnumList* l = holder.get();
                    u32 count = l->mPtrCount;
                    if (i >= count) break;
                    void* actorAcc = MapFx_ResolveOwner(getObjectAt(holder.get(), i));
                    if (actorAcc == acc) continue; // cmplw r3,r24; beq
                    static_cast<cf::CBattleState*>((cf::CfObjectActor*)actorAcc)->CBattleState_clearStatusId(0x0B);
                }
            }

            // Damage factor: f29 = CBattleMan_ApplyStatusRate((u8*)pc, tgt).
            f32 f29 = CBattleMan_ApplyStatusRate((u8*)pc, tgt);

            // Pass 3 (0x800F1624-0x800F1714): refilter (0x100, 0) then
            // __ct__800FB044(list, f28, pc->subIdent->vfAC(), 8);
            // apply scaled arts damage to each listed actor.
            {
                cf::CfObjEnumList* list = holder.get();
                startEnumObjects(list, 0x100, 0);

                void* idObj = ((cf::CfObjectActor*)pc)->CfObject_getPosVector();

                list = holder.get();
                __ct__800FB044(list, f28, idObj, 8);

                for (u32 i = 0;; i++) {
                    cf::CfObjEnumList* l = holder.get();
                    u32 count = l->mPtrCount;
                    if (i >= count) break;
                    void* actorAcc = MapFx_ResolveOwner(getObjectAt(holder.get(), i));
                    if (actorAcc == acc) continue; // cmplw r3,r24; beq .L_800F1700

                    // skip actors whose subIdent->vf4C() == pc->field_3F10
                    {
                        u32 pcId = ((cf::CfObjectActor*)pc)->field_0x74;
                        if ((u32)((cf::CfObjectActor*)actorAcc)->CObjectParam_getSelfObjectId() == pcId) continue; // beq .L_800F1700

                        // level = pc->vf108(); product = artsId * (level + 14)
                        s32 level = ((cf::CActorParam*)pc)->CActorParam_getActorLevel();
                        s32 product = (s32)artsId * (level + 14); // mullw r0,r14,r0

                        // actor->vf2C4(pc, lbl_eu_80666DDC, f29 * product, lbl_eu_80666DDC)
                        ((cf::CfObjectActor*)actorAcc)->CActorParam_addHate(pc, lbl_eu_80666DDC, f29 * (f32)product, lbl_eu_80666DDC);
                    }
                }
            }
        }
        break; // b .L_800F4000 (shared tail)
    }

    case 254: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case254Event, *evt) | 2;
        eventWorkspace.case254Event.eventType = 0x28;                 // li r0,0x28
        func_800EC918(self, pc, acc, &eventWorkspace.case254Event, tgt);

        eventWorkspace.case254Event.eventType = 0x29;                 // li r0,0x29
        func_800EC918(self, pc, acc, &eventWorkspace.case254Event, tgt);

        eventWorkspace.case254Event.eventType = 0xC6;                 // li r3,0xC6
        eventWorkspace.case254Event.field_10 = 2;                     // li r6,0x2
        eventWorkspace.case254Event.field_14 = 0;                     // li r0,0x0
        eventWorkspace.case254Event.field_24 = lbl_eu_80666DD4;                 // stfs f0, +0x24
        func_800EC918(self, pc, acc, &eventWorkspace.case254Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 244: {
        if (pc == nullptr) {
            break; // cmpwi r23,0; beq .L_800F4000
        }
        evt->prevEventType = evt->eventType;

        copyBattleEventPair(eventWorkspace.case244PlayerEvent,
                            eventWorkspace.case244ActorEvent, *evt, 2);

        eventWorkspace.case244ActorEvent.eventType = 0x11;                    // li r14,0x11
        eventWorkspace.case244ActorEvent.field_10 = ((cf::CfObjectActor*)pc)->field_0x74;
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case244ActorEvent, tgt)) {
            return 0; // li r3,0; b .L_800F41D4
        }

        // acc->field_3ED4 holder slot 0x70 called ONCE (CfObject UVF8;
        // retail passes pc->field_3F10 in r4 - hidden arg, see above).
        ((cf::CfObject*)((void*)((cf::CfObjectActor*)acc)->mSubObj38))->CfObject_notifyEventDone();

        // eventType 0x60 recursion with pc=0, acc=pc.
        eventWorkspace.case244PlayerEvent.eventType = 0x60;                    // li r3,0x60
        eventWorkspace.case244PlayerEvent.field_10 = 0x19;                     // li r0,0x19
        func_800EC918(self, 0, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case244PlayerEvent, tgt);

        // eventType 0xF4 recursion with pc=0, acc=pc.
        eventWorkspace.case244PlayerEvent.eventType = 0xF4;                    // li r0,0xF4
        func_800EC918(self, 0, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case244PlayerEvent, tgt);

        // eventType 0x11 recursion: field_10 = acc->field_3F10,
        // field_30 |= 0x800, acc=pc.
        eventWorkspace.case244PlayerEvent.eventType = 0x11;                    // sth r14, +0x0C
        eventWorkspace.case244PlayerEvent.field_10 = ((cf::CfObjectActor*)acc)->field_0x74;
        eventWorkspace.case244PlayerEvent.field_30 |= 0x800;                   // ori r0,r0,0x800
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case244PlayerEvent, tgt);
        return 1; // li r3,1; b .L_800F41D4
    }

    case 239: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case239Event, *evt) | 2;
        eventWorkspace.case239Event.eventType = 0x2D;                 // li r0,0x2D
        func_800EC918(self, pc, acc, &eventWorkspace.case239Event, tgt);

        eventWorkspace.case239Event.eventType = 0xCA;                 // li r0,0xCA
        eventWorkspace.case239Event.field_10 = 0;                     // li r14,0x0
        func_800EC918(self, pc, acc, &eventWorkspace.case239Event, tgt);

        eventWorkspace.case239Event.eventType = 0xCD;                 // li r0,0xCD
        eventWorkspace.case239Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case239Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 240: {
        // Status 0xF1 gate: if the actor has this status, reject the event.
        if (isBattleStatusActive((&acc->subObject), 0xF1)) {
            return 0; // li r3,0; b .L_800F41D4
        }

        // amount = 0x23 / ((0x64 - stat[0x83]) / 100.0f), single precision
        s32 amount = 0x23;                       // li r14,0x23
        {
            void* statObj = ((cf::CActorParam*)acc)->CActorParam_getStatusTable();
            if (statObj != nullptr) {
                s32 sv;
                statObj = ((cf::CActorParam*)acc)->CActorParam_getStatusTable();
                if (IdTable_GetSum(statObj, 0x83, &sv)) {
                    f32 num = (f32)amount;
                    f32 den = (f32)(0x64 - sv) / 100.0f; // subfic + fdivs
                    amount = (s32)(num / den);         // fdivs + fctiwz
                }
            }
        }

        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case240Event, *evt) | 2;
        eventWorkspace.case240Event.eventType = 0x94;                 // li r8,0x94
        eventWorkspace.case240Event.field_10 = amount;                // stw r14, +0x10
        eventWorkspace.case240Event.field_14 = 0xF0;                  // li r0,0xF0
        eventWorkspace.case240Event.field_24 = lbl_eu_80666DD4;                 // stfs f0, +0x24
        func_800EC918(self, pc, acc, &eventWorkspace.case240Event, tgt);

        eventWorkspace.case240Event.eventType = 0x34;                 // li r3,0x34
        eventWorkspace.case240Event.field_10 = 0;                     // li r14,0x0
        eventWorkspace.case240Event.field_14 = 3;                     // li r0,0x3
        eventWorkspace.case240Event.field_24 = lbl_eu_80666DD4;
        func_800EC918(self, pc, acc, &eventWorkspace.case240Event, tgt);

        eventWorkspace.case240Event.eventType = 0x2E;                 // li r0,0x2E
        eventWorkspace.case240Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case240Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 241: {
        if (isBattleStatusActive((&acc->subObject), 0xF1)) {
            return 0; // li r3,0; b .L_800F41D4
        }

        // amount = 0x0F / ((0x64 - stat[0x83]) / 100.0f), single precision
        s32 amount = 0x0F;                       // li r14,0x0F
        {
            void* statObj = ((cf::CActorParam*)acc)->CActorParam_getStatusTable();
            if (statObj != nullptr) {
                s32 sv;
                if (IdTable_GetSum(statObj, 0x83, &sv)) {
                    f32 num = (f32)0x0F;
                    f32 den = (f32)(0x64 - sv) / 100.0f;
                    amount = (s32)(num / den);
                }
            }
        }

        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case241Event, *evt) | 2;
        eventWorkspace.case241Event.eventType = 0x94;                 // li r3,0x94
        eventWorkspace.case241Event.field_10 = amount;
        eventWorkspace.case241Event.field_14 = 0xF1;                  // li r0,0xF1
        eventWorkspace.case241Event.field_24 = lbl_eu_80666DD4;
        func_800EC918(self, pc, acc, &eventWorkspace.case241Event, tgt);

        eventWorkspace.case241Event.eventType = 0x34;                 // li r3,0x34
        eventWorkspace.case241Event.field_10 = 0;
        eventWorkspace.case241Event.field_14 = 3;                     // li r0,0x3
        eventWorkspace.case241Event.field_24 = lbl_eu_80666DD4;
        func_800EC918(self, pc, acc, &eventWorkspace.case241Event, tgt);

        eventWorkspace.case241Event.eventType = 0x2E;                 // li r0,0x2E
        eventWorkspace.case241Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case241Event, tgt);

        eventWorkspace.case241Event.eventType = 0xCB;                 // li r0,0xCB
        func_800EC918(self, pc, acc, &eventWorkspace.case241Event, tgt);

        eventWorkspace.case241Event.eventType = 0xCC;                 // li r0,0xCC
        func_800EC918(self, pc, acc, &eventWorkspace.case241Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 242: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case242Event, *evt) | 2;
        eventWorkspace.case242Event.eventType = 0xD4;                 // li r0,0xD4
        func_800EC918(self, pc, acc, &eventWorkspace.case242Event, tgt);

        eventWorkspace.case242Event.eventType = 0xC7;                 // li r3,0xC7
        eventWorkspace.case242Event.field_10 = 0;                     // li r0,0x0
        func_800EC918(self, pc, acc, &eventWorkspace.case242Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 246: {
        // rnd = mtRand(evt->field_14, evt->field_10)  [lha +0x14 / lwz +0x10]
        s32 rnd = mtRand__Q22ml4mathFii(evt->field_14, evt->field_10);

        // acc->vf154((f32)(-rnd));  pc->vf154((f32)rnd);
        {
            ((cf::CActorParam*)acc)->CActorParam_addArtsGauge((f32)(-rnd));
        }
        {
            ((cf::CActorParam*)pc)->CActorParam_addArtsGauge((f32)rnd);
        }

        // cap: if (pc->vf158() > 99.0f) pc->vf150(99.0f);
        {
            if (((cf::CActorParam*)pc)->CActorParam_getArtsGauge() > k99_0f) {
                ((cf::CActorParam*)pc)->CActorParam_setTensionPoints(k99_0f);
            }
        }

        evt->prevEventType = evt->eventType;
        evt->field_20 = (f32)rnd;                 // stfs f0, 0x20(r25)
        evt->field_30 = copyBattleEvent(eventWorkspace.case246Event, *evt) | 2;
        eventWorkspace.case246Event.eventType = 0xC3;                 // li r0,0xC3
        eventWorkspace.case246Event.field_10 = 0;                     // li r14,0x0
        func_800EC918(self, pc, acc, &eventWorkspace.case246Event, tgt);

        eventWorkspace.case246Event.eventType = 0x120;                // li r0,0x120
        eventWorkspace.case246Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case246Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 247: {
        evt->field_10 = 0;                        // li r0,0; stw r0, 0x10(r25)
        break; // b .L_800F4000 (shared tail)
    }

    case 274: {
        if (isBattleStatusActive((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case274Event, *evt) | 2;
        eventWorkspace.case274Event.eventType = 0x58;                 // li r8,0x58
        eventWorkspace.case274Event.field_10 = 0x19;                  // li r0,0x19
        func_800EC918(self, pc, acc, &eventWorkspace.case274Event, tgt);

        eventWorkspace.case274Event.eventType = 0x02;                 // li r3,0x2
        eventWorkspace.case274Event.field_10 = 0x32;                  // li r0,0x32
        func_800EC918(self, pc, acc, &eventWorkspace.case274Event, tgt);

        eventWorkspace.case274Event.eventType = 0x3B;                 // li r0,0x3B
        eventWorkspace.case274Event.field_10 = 0;                     // li r14,0x0
        func_800EC918(self, pc, acc, &eventWorkspace.case274Event, tgt);

        eventWorkspace.case274Event.eventType = 0x3A;                 // li r0,0x3A
        eventWorkspace.case274Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case274Event, tgt);

        eventWorkspace.case274Event.eventType = 0x15;                 // li r3,0x15
        eventWorkspace.case274Event.field_10 = 0x64;                  // li r0,0x64
        func_800EC918(self, pc, acc, &eventWorkspace.case274Event, tgt);

        // acc->vf150(lbl_eu_80666DDC)
        {
            ((cf::CActorParam*)acc)->CActorParam_setTensionPoints(lbl_eu_80666DDC);
        }

        // Target the actor owning the accessory/unit id; if unavailable,
        // pick a random actor from the party list.
        {
            void* obj = findObjectById((int)((cf::CfObjectActor*)acc)->CObjectParam_getSelfObjectId());
            void* acc = MapFx_ResolveOwner(obj);
            if (acc != nullptr) {
                if (((cf::CActorParam*)acc)->CActorParam_isBattleLocked() == 0) {
                    evt->field_10 = ((cf::CfObjectActor*)acc)->field_0x74;
                    break; // b .L_800F4000 (shared tail)
                }
            }
        }

        // fallback: random party member from enum list
        {
            CfEnumListHolder holder;
            cf::CfObjEnumList* list = holder.get();
            startEnumObjects(list, 0x20, 0x800);

            cf::CfObjEnumList* l = holder.get();
            s32 count = (s32)l->mPtrCount;
            if (count > 0) {
                s32 idx = rand() % count;        // divwu/mullw/subf
                void* item = getObjectAt(holder.get(), (u32)idx);
                evt->field_10 = *(u32*)((u8*)item + 0x74);
            }
        }
        break; // b .L_800F4000 (shared tail)
    }

    case 273: {
        if (isBattleStatusActive((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        evt->prevEventType = evt->eventType;
        evt->field_30 |= 2;

        // level = acc->vf108(); pick a count from level bands.
        {
            s32 level = ((cf::CActorParam*)acc)->CActorParam_getActorLevel();
            s32 count;
            if (level < 0x0A) {
                count = 0;
            } else if (level < 0x14) {
                count = rand() % 2 + 1;
            } else if (level < 0x1E) {
                count = rand() % 2 + 2;
            } else if (level < 0x28) {
                count = rand() % 2 + 2;
            } else if (level < 0x32) {
                count = rand() % 2 + 2;
            } else if (level < 0x3C) {
                count = rand() % 3 + 2;
            } else if (level < 0x46) {
                count = rand() % 3 + 2;
            } else if (level < 0x50) {
                count = rand() % 3 + 2;
            } else if (level < 0x5A) {
                count = rand() % 3 + 2;
            } else {
                s32 randomValue = rand();
                count = randomValue % 3;
                count += 2;
            }

            if (count != 0) {
                // if (acc->CActorParam_getActorType() == 5) --count;
                {
                    if (((cf::CActorParam*)acc)->CActorParam_getActorType() == 5) {
                        count--;
                    }
                }

                Bdat_GetTable_AA34();
                void* file = getFP__FPCc((const char*)((u8*)acc + 0x3F14));
                u16 colId = ((cf::CfObjectActor*)acc)->unk8C_3;

                // The BDAT columns are 16-bit growth values.
                u16 c0 = getBdatStringColumnValue(file, "Lv_up_str", colId);
                u16 c1 = getBdatStringColumnValue(file, "Lv_up_agi", colId);
                u16 c2 = getBdatStringColumnValue(file, "Lv_up_eth", colId);

                evt->field_10 = count;
                evt->field_14 = (s16)(count * c0);  // mullw + sth
                evt->field_16 = (s16)(count * c1);
                evt->field_18 = (s16)(count * c2);
            }
        }

        // acc->vf150(lbl_eu_80666DDC)
        {
            ((cf::CActorParam*)acc)->CActorParam_setTensionPoints(lbl_eu_80666DDC);
        }
        break; // b .L_800F4000 (shared tail)
    }

    case 275: {
        if (isBattleStatusActive((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case275Event, *evt) | 2;
        eventWorkspace.case275Event.eventType = 0x34;                 // li r9,0x34
        eventWorkspace.case275Event.field_10 = 0;                     // li r8,0x0
        eventWorkspace.case275Event.field_14 = 4;                     // li r0,0x4
        eventWorkspace.case275Event.field_24 = k2_0f;                 // stfs f0(2.0f), +0x24
        func_800EC918(self, pc, acc, &eventWorkspace.case275Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 276: {
        if (isBattleStatusActive((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        break; // beq .L_800F4000 (shared tail)
    }

    case 277: {
        if (isBattleStatusActive((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        evt->prevEventType = evt->eventType;
        evt->field_30 |= 2;
        break; // b .L_800F4000 (shared tail)
    }

    case 278: {
        if (isBattleStatusActive((&acc->subObject), 0x117)) {
            return 0; // li r3,0; b .L_800F41D4
        }
        break; // beq .L_800F4000 (shared tail)
    }

    case 236: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case236Event, *evt) | 2;
        eventWorkspace.case236Event.eventType = 0x44;
        eventWorkspace.case236Event.field_30 |= 0x800;
        func_800EC918(self, pc, acc, &eventWorkspace.case236Event, tgt);

        eventWorkspace.case236Event.eventType = 0x45;
        func_800EC918(self, pc, acc, &eventWorkspace.case236Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 281: {
        cf::PartyGaugeAddClamped(reinterpret_cast<cf::UnkClass_8018C5FC*>((void*)&((cf::CBattleManager*)self)->unk194), evt->field_10);
        break; // b .L_800F4000 (shared tail)
    }

    case 255: {
        return 1; // li r3,1; b .L_800F41D4
    }

    case 193: {
        evt->field_20 = k5_0f;                    // stfs f0(5.0f), 0x20(r25)
        break; // b .L_800F4000 (shared tail)
    }

    case 153: {
        // acc->vf154((f32)evt->field_10)
        {
            ((cf::CActorParam*)acc)->CActorParam_addArtsGauge((f32)evt->field_10);
        }
        break; // b .L_800F4000 (shared tail)
    }

    case 257: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case257Event, *evt) | 2;
        eventWorkspace.case257Event.eventType = 0xD4;                 // li r8,0xD4
        eventWorkspace.case257Event.field_10 = 0;                     // li r0,0x0
        func_800EC918(self, pc, acc, &eventWorkspace.case257Event, tgt);

        eventWorkspace.case257Event.eventType = 0x62;                 // li r3,0x62
        eventWorkspace.case257Event.field_10 = 0x64;                  // li r0,0x64
        func_800EC918(self, pc, acc, &eventWorkspace.case257Event, tgt);
        break; // b .L_800F4000 (shared tail)
    }

    case 258: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case258Event, *evt) | 2;
        eventWorkspace.case258Event.eventType = 0x02;                 // li r0,0x2
        func_800EC918(self, pc, acc, &eventWorkspace.case258Event, tgt);

        // sweep slots 0..31: purge events that are not class-0x45C00 and
        // whose field_08 lacks the 0x1C00 bits.
        for (s32 i = 0; i < 0x20; i++) {
            cf::CBattleStateEntry* r = (cf::CBattleStateEntry*)static_cast<cf::CBattleState*>((cf::CfObjectActor*)acc)->CBattleState_getEventEntry((u32)i);

            if (isBattleEventKind3(r->unk2E)) continue; // bne
            if (r->unk08 & 0x1C00) continue;       // rlwinm 17-19

            static_cast<cf::CBattleState*>((cf::CfObjectActor*)acc)->CBattleState_applyEventEntry((cf::CBattleStateEntry*)r);
        }
        break; // b .L_800F4000 (shared tail)
    }

    case 259: {
        if (pc == nullptr) {
            return 0; // beq .L_800F29CC
        }
        if (*(u32*)((u8*)pc + 0x1530) == 0) {
            return 0; // lwz r0, 0x1530(r23); cmpwi r0,0; beq .L_800F29CC
        }
        copyBattleEvent(eventWorkspace.case259Event, *evt);                            // (no prevEventType, no |2)
        eventWorkspace.case259Event.eventType = 0x0F;                 // li r8,0x0F
        eventWorkspace.case259Event.field_10 = 0x03;                  // li r0,0x3
        func_800EC918(self, pc, acc, &eventWorkspace.case259Event, tgt);
        return 1; // .L_800F29D4: li r3,1; b .L_800F41D4
    }

    case 283: {
        {
            CfEnumListHolder actorList;
            startEnumObjects(actorList.get(), 0x80000000, 0);  // lis r4,0x8000 ; li r5,0
            for (s32 i = 0; i < (s32)actorList.get()->mPtrCount; i++) {
                void* actor = MapFx_ResolveOwner(getObjectAt(actorList.get(), i));
                void* result = ((cf::CActorParam*)actor)->CActorParam_findGaugeEntry(((cf::CfObjectActor*)acc)->field_0x74);
                if (result != nullptr) {
                    // Each vector component is evaluated independently in the original.
                    ((cf::CfObjectActor*)actor)->CActorParam_addHate(pc, *(f32*)((u8*)result + 0x10) * ((f32)(s32)evt->field_10 / 100.0f), *(f32*)((u8*)result + 0x00) * ((f32)(s32)evt->field_10 / 100.0f), *(f32*)((u8*)result + 0x04) * ((f32)(s32)evt->field_10 / 100.0f));
                    *(f32*)((u8*)result + 0x10) *=
                        1.0f - ((f32)(s32)evt->field_10 / 100.0f);
                    *(f32*)((u8*)result + 0x00) *=
                        1.0f - ((f32)(s32)evt->field_10 / 100.0f);
                    *(f32*)((u8*)result + 0x04) *=
                        1.0f - ((f32)(s32)evt->field_10 / 100.0f);
                }
            }
        }
        break;                                         // b .L_800F4000
    }

    case 284: {
        if (acc->field_1530 != 0) {               // lwz r4,0x1530 ; beq tail
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), acc->field_1530);
            u16 prevType = entry->unk2E; // lhz r15, 0x2e
            if (prevType != 0) {
                for (s32 i = 0; i < 0x68; i++) {      // cmpwi 0x68
                    cf::CBattleStateEntry* item = (cf::CBattleStateEntry*)((cf::CBattleState*)&acc->subObject)->CBattleState_getEntryByIndex(i);
                    if (item->unk2E == prevType) {
                        if (item->unk20 > lbl_eu_80666DDC) {   // fcmpo vs 0.0
                            item->unk20 += evt->field_20;
                            if (item->unk0C == 0x11) {  // cmplwi 0x11
                                void* actor = MapFx_ResolveOwner(
                                    findObjectById(item->unk10));
                                if (actor != nullptr &&
                                    isBattleStatusActive((u8*)actor + 8, 0x11)) {
                                    cf::CBattleStateEntry* entry2 = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actor + 8, 0x11);
                                    if (entry2 != nullptr) {
                                        entry2->unk20 += evt->field_20;
                                    }
                                }
                            }
                        }
                    }
                }
            } else {
                if (entry->unk20 > lbl_eu_80666DDC) {
                    entry->unk20 += evt->field_20;
                }
            }
            // actor-list walker, filter (0x20, 0x800)
            CfEnumListHolder actorList;
            startEnumObjects(actorList.get(), 0x20, 0x800);
            for (s32 i = 0; i < (s32)actorList.get()->mPtrCount; i++) {
                void* actor = MapFx_ResolveOwner(getObjectAt(actorList.get(), i));
                if (actor != nullptr && actor != acc) {   // cmplw r3,r24 ; beq
                    SubAccessor* sub = (SubAccessor*)((u8*)actor + 8);
                    for (s32 j = 0; j < 0x20; j++) {  // vfunc 0x58 list
                        cf::CBattleStateEntry* item = (cf::CBattleStateEntry*)((cf::CBattleState*)sub)->CBattleState_fetchStatusEntry(j);
                        if (item->unk2E == prevType) {
                            // Reacquire the matching entry before mutating it.
                            item = (cf::CBattleStateEntry*)((cf::CBattleState*)sub)->CBattleState_fetchStatusEntry(j);
                            item->unk20 += evt->field_20;
                        }
                    }
                    for (s32 j = 0; j < 0x20; j++) {  // vfunc 0x60 list
                        cf::CBattleStateEntry* item = (cf::CBattleStateEntry*)((cf::CBattleState*)sub)->CBattleState_getStatusEntry(j);
                        if (item->unk2E == prevType) {
                            item = (cf::CBattleStateEntry*)((cf::CBattleState*)sub)->CBattleState_getStatusEntry(j);
                            item->unk20 += evt->field_20;
                        }
                    }
                }
            }
        }
        break;                                         // b .L_800F4000
    }

    case 285: {
        ((cf::CActorParam*)acc)->CActorParam_accumulateTension((u32)evt->field_10);
        break;                                         // b .L_800F4000
    }

    case 249: {
        evt->prevEventType = evt->eventType;      // sth r4, 0x2e(r25)
        evt->field_30 = copyBattleEvent(eventWorkspace.case249Event, *evt) | 2;
        if (pc == acc) {                   // cmplw r23,r24 ; bne .L_800F2E58
            const s32 eventTypes[5] = { 0x58, 0x59, 0x5A, 0x5B, 0x5C };
            eventWorkspace.case249Event.eventType = eventTypes[mtRand__Q22ml4mathFi(5)];
            func_800EC918(self, pc, acc, &eventWorkspace.case249Event, tgt);
        }
        // .L_800F2E58
        eventWorkspace.case249Event.eventType = 0xBF;
        eventWorkspace.case249Event.field_10 = 0;
        func_800EC918(self, pc, acc, &eventWorkspace.case249Event, tgt);
        if (pc == acc) {
            break;                              // beq .L_800F4000 -> shared tail
        }
        return 1;                               // li r3,1 ; b .L_800F41D4
    }

    case 260: {
        s32 roll = mtRand__Q22ml4mathFi(100);          // r16
        s32 chosen = 0;                                // r15
        s32 i = 3;                                     // r17 start index
        u32 f15F0 = acc->field_15F0;              // lwz r0,0x15f0(r24)
        if ((f15F0 == 0 || f15F0 == 2) && *(u32*)(lbl_eu_80573EEC + 0xD0) < 4) {
            i = 0;                                     // blt .L_800F2F04
        }
        s32 diff = ((cf::CActorParam*)acc)->CActorParam_getActorLevel() - ((cf::CActorParam*)pc)->CActorParam_getActorLevel();  // subf r0,r3,r14
        if (diff > 10) i = 5;                          // cmpwi 0xa ; ble skip
        const s32 weights[6] = { 2, 5, 40, 25, 25, 100 };
        for (; i < 6; i++) {                           // mtctr(6-i) ; bdnz
            roll -= weights[i];                        // subf. r16,r0,r16
            if (roll < 0) { chosen = i; break; }       // bge next ; mr r15,r17
        }
        s32 clearedValue = 0;
        u32 inheritedFlag = 0x10000;
        s32 selectedArts = 0;                                   // selected arts id
        s32 chainResult = -1;                                  // selected chain result
        s32 chainLevel = 0;                                   // selected chain level
    reselect:                                          // .L_800F2FAC
        switch (chosen) {
        case 0: chainLevel = 3; selectedArts = 0; break;               // .L_800F2FEC
        case 1: chainLevel = 2; selectedArts = 1; break;               // .L_800F2FF8
        case 2: chainLevel = 1; selectedArts = 2; break;               // .L_800F3004
        case 3: {                                      // .L_800F3010 arts radar
            if (pc != nullptr) {
                void* statObj = ((cf::CActorParam*)pc)->CActorParam_getStatusTable();
                if (statObj != nullptr) {
                    statObj = ((cf::CActorParam*)pc)->CActorParam_getStatusTable();      // retail calls vf290 twice
                    if (Counter_TestBit(statObj, 0x78)) {
                        for (s32 n = 1; n <= 8; n++) { // li r16,1 ; ble 8
                            void* obj = CtrlObjectParam_GetCharEntry((u16)n);
                            CtrlObjectParam_RunPartyArtsFlow(obj, (u32)((cf::CActorParam*)acc)->CActorParam_getSpentCurrency() >> 1, 0,
                                          ((cf::CActorParam*)acc)->CActorParam_getActorLevel(), 0, 1, 0);
                        }
                        chainResult = 1; selectedArts = 3;
                        break;
                    }
                }
            }
            chosen = 5;                                // .L_800F30CC: li r15,5
            goto reselect;                             // b .L_800F2FAC
        }
        case 4: {                                      // .L_800F30D4 arts radar (2)
            if (pc != nullptr) {
                void* statObj = ((cf::CActorParam*)pc)->CActorParam_getStatusTable();
                if (statObj != nullptr) {
                    statObj = ((cf::CActorParam*)pc)->CActorParam_getStatusTable();
                    if (Counter_TestBit(statObj, 0x79)) {
                        for (s32 n = 1; n <= 8; n++) {
                            void* obj = CtrlObjectParam_GetCharEntry((u16)n);
CtrlObjectParam_RunPartyArtsFlow(obj, 0, (void*)(uintptr_t)((u32)((cf::CActorParam*)acc)->CActorParam_getSecondCurrency() >> 1),
                                          ((cf::CActorParam*)acc)->CActorParam_getActorLevel(), 0, 1, 0);
                        }
                        chainResult = 2; selectedArts = 4;
                        break;
                    }
                }
            }
            chosen = 5;                                // .L_800F3190: li r15,5
            goto reselect;
        }
        case 5: {                                      // .L_800F3198
            // ---- build eventWorkspace.case260ActorEvent (stack 0x2EC) and eventWorkspace.case260PlayerEvent (stack 0x2B8) ----
            copyBattleEvent(eventWorkspace.case260ActorEvent, *evt);
            eventWorkspace.case260ActorEvent.eventType = clearedValue;              // sth r30(0), 0x2f8
            eventWorkspace.case260ActorEvent.field_10 = clearedValue;               // stw r30(0), 0x2fc
            eventWorkspace.case260ActorEvent.field_20 = lbl_eu_80666DDC;                       // stfs f29(0.0), 0x30c
            eventWorkspace.case260ActorEvent.field_08 = inheritedFlag;              // stw r31(0x10000), 0x2f4
            eventWorkspace.case260ActorEvent.field_30 |= 0x500;                     // stw r6(evt->30|0x500), 0x31c
            eventWorkspace.case260PlayerEvent.field_30 =
                copyBattleEvent(eventWorkspace.case260PlayerEvent,
                                eventWorkspace.case260ActorEvent) | 0x4;
            getCfObjectPc(getObjectMove(pc));
            switch (rand() % 4) {                      // bl rand ; %4 idiom
            case 0: {                                  // .L_800F32A0 knockback
                s32 k = (s32)(0.5f * ((cf::CActorParam*)pc)->CActorParam_getDamageScale());  // f30 = 0.5 (80666DE8)
                ((cf::CActorParam*)acc)->CActorParam_addHp((float)(f64)(s32)(-k));  // neg r0,r15
                f32 d = calcBattleDamage(pc, pc, tgt);
                ((cf::CActorParam*)acc)->CActorParam_addHp((float)(f64)(s32)((s32)((f32)(s32)k * d)));
                chainResult = 3; selectedArts = 8;
                break;
            }
            case 1: {                                  // .L_800F334C
                eventWorkspace.case260ActorEvent.eventType = 0x52;                  // r17 = 0x52
                eventWorkspace.case260ActorEvent.field_10 = 10;                     // r18 = 0xa
                eventWorkspace.case260PlayerEvent.eventType = 0x58;                  // r19 = 0x58
                eventWorkspace.case260PlayerEvent.field_18 = 50;                     // r21 = 0x32
                {
                    void* accArts = ((cf::CActorParam*)acc)->CActorParam_getArtsDataBlock();
                    s32 v1 = *(s16*)((u8*)accArts + 0x1C);
                    f32 t = 0.1f * (f32)(s32)v1;       // f28 = 0.1 (80666E84)
                    s32 pct = (s32)((f64)t + (t > lbl_eu_80666DDC ? 0.5 : -0.5));  // E58/E60
                    void* pcArts = ((cf::CActorParam*)pc)->CActorParam_getArtsDataBlock();
                    s32 v2 = *(s16*)((u8*)pcArts + 0x1C);
                    eventWorkspace.case260PlayerEvent.field_10 = pct * 100 / v2;     // mulli 0x64 ; divw
                }
                chainResult = 4; selectedArts = 5;
                break;
            }
            case 2: {                                  // .L_800F33DC
                eventWorkspace.case260ActorEvent.eventType = 0x53;                  // r20 = 0x53
                eventWorkspace.case260ActorEvent.field_10 = 10;
                eventWorkspace.case260PlayerEvent.eventType = 0x59;                  // r14 = 0x59
                eventWorkspace.case260PlayerEvent.field_18 = 50;
                {
                    void* accArts = ((cf::CActorParam*)acc)->CActorParam_getArtsDataBlock();
                    s32 v1 = *(s16*)((u8*)accArts + 0x1E);
                    f32 t = 0.1f * (f32)(s32)v1;
                    s32 pct = (s32)((f64)t + (t > lbl_eu_80666DDC ? 0.5 : -0.5));
                    void* pcArts = ((cf::CActorParam*)pc)->CActorParam_getArtsDataBlock();
                    s32 v2 = *(s16*)((u8*)pcArts + 0x1E);
                    eventWorkspace.case260PlayerEvent.field_10 = pct * 100 / v2;
                }
                chainResult = 5; selectedArts = 7;
                break;
            }
            case 3: {                                  // .L_800F346C
                eventWorkspace.case260ActorEvent.eventType = 0x54;
                eventWorkspace.case260ActorEvent.field_10 = 10;
                eventWorkspace.case260PlayerEvent.eventType = 0x5A;
                eventWorkspace.case260PlayerEvent.field_18 = 50;
                {
                    void* accArts = ((cf::CActorParam*)acc)->CActorParam_getArtsDataBlock();
                    s32 v1 = *(s16*)((u8*)accArts + 0x20);
                    f32 t = 0.1f * (f32)(s32)v1;
                    s32 pct = (s32)((f64)t + (t > lbl_eu_80666DDC ? 0.5 : -0.5));
                    void* pcArts = ((cf::CActorParam*)pc)->CActorParam_getArtsDataBlock();
                    s32 v2 = *(s16*)((u8*)pcArts + 0x20);
                    eventWorkspace.case260PlayerEvent.field_10 = pct * 100 / v2;
                }
                chainResult = 6; selectedArts = 6;
                break;
            }
            }
            // .L_800F3500
            if (eventWorkspace.case260ActorEvent.eventType != 0) {                  // lhz r0,0x2f8 ; beq .L_800F355C
                if (!func_800EC918(self, pc, acc, &eventWorkspace.case260ActorEvent, tgt)) return 0;
                if (!func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case260PlayerEvent, tgt)) return 0;  // acc=pc
            }
            break;
        }
        }
        // .L_800F355C
        if (chainLevel != 0) {
            if (!CItem_rollBoxEntry(chainLevel, acc->field_3F28)) {
                chosen = 5;                            // li r15,5 ; b .L_800F2FAC
                goto reselect;
            }
            chainResult = 0;
        }
        CUICfManager_queueMultiGetItemMenu((u8)chainResult);
        CCharVoiceMan_EnqueueArtsVoice(pc, selectedArts);
        return 1;                                      // li r3,1 ; b .L_800F41D4
    }

    case 261: {
        evt->prevEventType = evt->eventType;
        evt->field_30 |= 0x2;
        if (isBattleStatusActive((&acc->subObject), 0x112)) {
            void* artsData = *(void**)((u8*)tgt + 0x50);
            s32 artsVal;
            artsVal = artsSubGetMax(artsData);
            s32 rate = artsVal * 5 + 50;               // slwi 2 ; add ; addi 0x32
            if (((cf::CActorParam*)pc)->CActorParam_getStatusCount() == 1) rate -= 25;   // subi r14,r14,0x19
            if (((cf::CActorParam*)pc)->CActorParam_getStatusCount() == 0) rate = 0;     // li r14,0
            if (mtRand__Q22ml4mathFi(100) < rate) {    // cmpw ; bge skip
                ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(0x112);         // vfunc 0x20: clear status
            }
        }
        copyBattleEvent(eventWorkspace.case261Event, *evt);
        eventWorkspace.case261Event.eventType = 7;                             // sth r0(7), 0x290
        eventWorkspace.case261Event.field_10 = ((cf::CfObjectActor*)pc)->field_0x74;      // stw pc->3F10, 0x294
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case261Event, tgt)) return 0;
        break;                                         // b .L_800F4000
    }

    case 264: {
        for (s32 i = 0; i < 0x20; i++) {
            cf::CBattleStateEntry* item = (cf::CBattleStateEntry*)((cf::CBattleState*)&acc->subObject)->CBattleState_getEventEntry(i);
            if (!isBattleEventKind3(item->unk2E)) {
                if (!(item->unk08 & 0xE000)) {   // rlwinm 0,17,19
                    ((cf::CBattleState*)&acc->subObject)->CBattleState_applyEventEntry((cf::CBattleStateEntry*)item);
                }
            }
        }
        break;                                         // b .L_800F4000
    }

    case 287: {
        s32 artsNo = spawnBankByte();                  // bl spawnBankByte
        if (artsNo < 0) return 0;                      // bge .L_800F3768 ; li r3,0
        if (mtRand__Q22ml4mathFi(100) < evt->field_10) {
            spawnBankByte();                           // bl again; return ignored
        }
        const s32 artsNumbers[6] = { 6, 4, 7, 8, 9, 5 };
        const s32 eventTypes[6] = { 0x125, 0x126, 0x127, 0x128, 0x129, 0x12A };
        memset(&eventWorkspace.case287Event, 0, sizeof(eventWorkspace.case287Event));                  // memset(0x250, 0, 0x34)
        eventWorkspace.case287Event.field_00 = ((cf::CfObjectActor*)pc)->field_0x74;      // stw pc->3F10, 0x250
        for (s32 i = 0; i < 6; i++) {                  // mtctr 6 ; bdnz
            if (artsNo != artsNumbers[i]) continue;           // cmpw r14,r0 ; bne .L_800F38EC
            if (isBattleStatusActive((u8*)pc + 8, eventTypes[i])) break;   // bne -> return 1
            for (s32 idx = 0; idx <= 1; idx++) {       // r17=0 ; cmpwi 1 ; ble
                for (s32 slot = 0; slot < 8; slot++) { // r18=0 ; cmpwi 8 ; blt
                    void* artsParam = getArtsParamRC2(((cf::CActorParam*)pc)->CActorParam_getArtsSet(), idx, slot);
                    if (((cf::CAttackParamPrefix*)artsParam)->unk3C == 4 &&
                        ((cf::CAttackParamPrefix*)artsParam)->unk40 == (u16)artsNo) {
                        eventWorkspace.case287Event.eventType = eventTypes[i];       // lwzx ; sth 0x25c
                        s32 vf = artsSubGetMax(artsParam);
                        eventWorkspace.case287Event.field_10 =
                                     (s32)(s16)((cf::CAttackParamPrefix*)artsParam)->unk4A
                                     + (s32)((cf::CAttackParamPrefix*)artsParam)->unk6C[3] * (vf - 1);
                        eventWorkspace.case287Event.field_14 =
                                     (s16)((cf::CAttackParamPrefix*)artsParam)->unk4C;   // sth 0x264
                        func_800EC918((void*)(uintptr_t)cf::CBattleManager::spInstance, pc, acc, &eventWorkspace.case287Event, nullptr);
                        break;                         // b .L_800F38DC (outer cont.)
                    }
                }
            }
            break;                                     // b .L_800F38F8 (return 1)
        }
        return 1;                                      // li r3,1 ; b .L_800F41D4
    }

    case 250: {
        for (s32 i = 0; i < 0x20; i++) {        // li r14,0 ; cmpwi 0x20 ; blt
            cf::CBattleStateEntry* item = (cf::CBattleStateEntry*)((cf::CBattleState*)&acc->subObject)->CBattleState_getEventEntry(i);
            if (!isBattleEventKind3(item->unk2E)) {
                if (!(item->unk08 & 0xE000)) {   // rlwinm 0,17,19
                    ((cf::CBattleState*)&acc->subObject)->CBattleState_applyEventEntry((cf::CBattleStateEntry*)item);
                }
            }
        }
        evt->prevEventType = evt->eventType;      // lhz r3,0xc(r25) ; sth r3,0x2e(r25)
        evt->field_30 = copyBattleEvent(eventWorkspace.case250Event, *evt) | 2;
        eventWorkspace.case250Event.eventType = 0x34;                   // sth r0(0x34), 0x228(r1)
        func_800EC918(self, pc, acc, &eventWorkspace.case250Event, tgt);
        break;                                  // b .L_800F4000
    }

    case 251: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case251Event, *evt) | 2;
        eventWorkspace.case251Event.eventType = 2;
        func_800EC918(self, pc, acc, &eventWorkspace.case251Event, tgt);
        eventWorkspace.case251Event.eventType = 0x60;
        eventWorkspace.case251Event.field_10 = 25;                      // li r14,0x19 ; stw r14,0x1f8
        func_800EC918(self, pc, acc, &eventWorkspace.case251Event, tgt);
        eventWorkspace.case251Event.eventType = 0x3C;
        eventWorkspace.case251Event.field_10 = 25;
        func_800EC918(self, pc, acc, &eventWorkspace.case251Event, tgt);
        break;                                  // b .L_800F4000
    }

    case 267: {
        evt->prevEventType = evt->eventType;
        copyBattleEventPair(eventWorkspace.case267PrimaryEvent,
                            eventWorkspace.case267SecondaryEvent, *evt, 0);
        evt->field_30 |= 0x2;
        eventWorkspace.case267SecondaryEvent.eventType = 0x11;                          // sth r14(0x11), 0x18c
        eventWorkspace.case267SecondaryEvent.field_10 = ((cf::CfObjectActor*)pc)->field_0x74;      // stw pc->3F10, 0x190
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case267SecondaryEvent, tgt)) return 0;
        // .L_800F3BC0 (acc->3ED4 holder slot 0x70: CfObject UVF8, r4 hidden).
        ((cf::CfObject*)acc->field_3ED4)->CfObject_notifyEventDone();
        eventWorkspace.case267PrimaryEvent.eventType = 0x62;                          // sth r3(0x62), 0x1c0
        eventWorkspace.case267PrimaryEvent.field_10 = 0x32;                           // stw r0(0x32), 0x1c4
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case267PrimaryEvent, tgt);     // acc arg = pc
        eventWorkspace.case267PrimaryEvent.eventType = 0x121;
        eventWorkspace.case267PrimaryEvent.field_10 = 5;
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case267PrimaryEvent, tgt);
        eventWorkspace.case267PrimaryEvent.eventType = 0x11;
        eventWorkspace.case267PrimaryEvent.field_10 = ((cf::CfObjectActor*)acc)->field_0x74;  // ORIGINAL acc
        eventWorkspace.case267PrimaryEvent.field_30 |= 0x800;                         // ori 0x800, 0x1e4
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case267PrimaryEvent, tgt);
        acc = (EC918_BattleObjAccessor*)pc;       // mr r24,r23
        break;                                         // b .L_800F4000
    }

    case 290: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case290Event, *evt) | 2;
        eventWorkspace.case290Event.eventType = 0x54;                          // sth r0(0x54), 0x158
        eventWorkspace.case290Event.field_30 |= 0x400;                         // ori 0x400, 0x17c
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case290Event, tgt)) return 0;
        // .L_800F3D10
        eventWorkspace.case290Event.eventType = 0x5A;
        cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)matchBattleStatusEntry((u8*)pc + 8, 0x5A, evt->field_00, evt->field_04, 0);
        if (entry != nullptr) {
            entry->unk10 += 5;           // addi r0,r4,0x5
            entry->unk20 = evt->field_20; // stfs evt->20
            break;                                     // b .L_800F4000
        }
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case290Event, tgt);     // acc arg = pc
        break;                                         // b .L_800F4000
    }

    case 291: {
        evt->prevEventType = evt->eventType;
        evt->field_30 = copyBattleEvent(eventWorkspace.case291Event, *evt) | 2;
        eventWorkspace.case291Event.eventType = 0x52;                          // sth r0(0x52), 0x124
        eventWorkspace.case291Event.field_30 |= 0x400;                         // ori 0x400, 0x148
        if (!func_800EC918(self, pc, acc, &eventWorkspace.case291Event, tgt)) return 0;
        // .L_800F3E1C
        eventWorkspace.case291Event.eventType = 0x58;
        cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)matchBattleStatusEntry((u8*)pc + 8, 0x58, evt->field_00, evt->field_04, 0);
        if (entry != nullptr) {
            entry->unk10 += 5;
            entry->unk20 = evt->field_20;
            break;                                     // b .L_800F4000
        }
        func_800EC918(self, pc, (EC918_BattleObjAccessor*)pc, &eventWorkspace.case291Event, tgt);     // acc arg = pc
        break;                                         // b .L_800F4000
    }

    case 292: {
        for (s32 i = 0; i < 0x20; i++) {               // vfunc 0x58 list
            if (!isBattleEventKind3(*(u16*)((u8*)((cf::CBattleState*)&acc->subObject)->CBattleState_fetchStatusEntry(i) + 0x2E))) {
                if (tgt != nullptr &&
                    *(u16*)((u8*)((cf::CBattleState*)&acc->subObject)->CBattleState_fetchStatusEntry(i) + 0x2C) == tgt->field_80) {
                    continue;                          // beq .L_800F3F04
                }
                ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(*(u16*)((u8*)((cf::CBattleState*)&acc->subObject)->CBattleState_fetchStatusEntry(i) + 0x0C));
            }
        }
        for (s32 i = 0; i < 0x20; i++) {               // vfunc 0x5C list
            if (!isBattleEventKind3(*(u16*)((u8*)((cf::CBattleState*)&acc->subObject)->CBattleState_getEventEntry(i) + 0x2E))) {
                if (tgt != nullptr &&
                    *(u16*)((u8*)((cf::CBattleState*)&acc->subObject)->CBattleState_getEventEntry(i) + 0x2C) == tgt->field_80) {
                    continue;                          // beq .L_800F3F9C
                }
                ((cf::CBattleState*)&acc->subObject)->CBattleState_clearStatusId(*(u16*)((u8*)((cf::CBattleState*)&acc->subObject)->CBattleState_getEventEntry(i) + 0x0C));
            }
        }
        break;                                         // b .L_800F4000
    }

    case 50: {
        if (pc != nullptr) {
            if ((((cf::CfObjectActor*)pc)->unk64 & 0x2) && (((cf::CfObjectActor*)pc)->unk8C_3 == 0x2)) {
                if (isBattleStatusActive(
                        &((EC918_BattleObjAccessor*)pc)->subObject, 0x32)) {
                    SubAccessor* playerStatus =
                        &((EC918_BattleObjAccessor*)pc)->subObject;
                    ((cf::CBattleState*)playerStatus)->CBattleState_clearStatusId(0x32);
                    return 0;   // li r3,0 ; b .L_800F41D4
                }
            }
        }
        break;   // beq .L_800F4000 -> shared tail
    }

    default:
        break;
    }

    // ================================================================
    // SHARED TAIL (.L_800F4000)
    // ================================================================

    // ---- Part 1: isBattleEventKind3 -> synthesize type 0x34 event ----
    if (isBattleEventKind3(evt->eventType)) {
        cf::CBattleStateEntry* data90 = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x90);
        if (data90 != nullptr) {
            copyBattleEvent(eventWorkspace.tailEvent, *evt);
            eventWorkspace.tailEvent.field_00 = ((cf::CfObjectActor*)acc)->field_0x74;
            eventWorkspace.tailEvent.field_04 = 0x90;
            eventWorkspace.tailEvent.eventType = 0x34;
            eventWorkspace.tailEvent.field_10 = data90->unk10;
            eventWorkspace.tailEvent.field_14 = 0;
            eventWorkspace.tailEvent.field_20 = k2_0f;
            eventWorkspace.tailEvent.field_30 |= data90->unk30;
            func_800EC918(self, pc, acc, &eventWorkspace.tailEvent, tgt);
        }
    }

    // ---- Part 2: slot 0x9E multiplier (isBattleStatusKind0 gate) ----
    if (pc != nullptr) {
        if (isBattleStatusKind0(evt->eventType)) {
            cf::CBattleStateEntry* data9E = (cf::CBattleStateEntry*)findBattleStatusEntry((&acc->subObject), 0x9E);
            if (data9E != nullptr) {
                if (evt->field_08 == 0x8000) {
                    s32 val9E = data9E->unk10;
                    evt->field_20 *= (1.0f + (f32)(s32)val9E / 100.0f);
                }
            }
        }
    }

    // ---- Part 3: slot 0x9F multiplier (isBattleStatusKind1 gate, on pc+8) ----
    if (pc != nullptr) {
        if (isBattleStatusKind1(evt->eventType)) {
            cf::CBattleStateEntry* data9F = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)pc + 8, 0x9F);
            if (data9F != nullptr) {
                if (evt->field_08 == 0x8000) {
                    s32 val9F = data9F->unk10;
                    evt->field_20 *= (1.0f + (f32)(s32)val9F / 100.0f);
                }
            }
        }
    }

    // ---- Part 4: apply event to actor (vfunc 0x18) ----
    {
        ((cf::CBattleState*)&acc->subObject)->CBattleState_enterStatusEntry((cf::CBattleStateEntry*)evt);
    }

    return 1;
}

#pragma pop

// Retail switch-slot reservation, run 1 (decomp .data 0x5DC-0x704, 0x128
// zero bytes, anchored by the owning run's function address so MWCC keeps the
// reservation in .data (all-zero aggregates migrate to .bss despite section/
// used): pads the func_800EC918 switch block out so the B784/B790 PMF
// triplets land at retail 0x704/0x710. Covers the still-elided WIP switch
// tables (func_800DCB54 x8, updateBattleAction x4, processBattleMove x5,
// emitBattleMoveEvents x4) plus MWCC shape drift (two 0x20 tables, E08E8 0x38 for
// retail 0x68). Volatile so MWCC keeps the zeros in .data (never .bss).
// Shrink by 4 per restored table slot as switches are reimplemented.
__attribute__((used)) const void* cbm_data_reserve_run1a[9] = {
    (const void*)func_800EC918,
    (const void*)0,
    (const void*)0
};
__attribute__((used)) const void* cbm_data_reserve_run1b[0x41] = {
    (const void*)func_800EC918,
    (const void*)0,
    (const void*)0
};
// lbl_eu_8052B784 / lbl_eu_8052B790 (0xC each): confirmation-handler PMF
// triplets {delta=0, vindex=-1, func}, copied wholesale and compared with
// __ptmf_cmpr (non-virtual PMF, so dispatch stays a direct call).
__attribute__((used)) const void* lbl_eu_8052B784[3] = {
    (const void*)0,
    (const void*)-1,
    (const void*)runVisionTierWindows
};
__attribute__((used)) const void* lbl_eu_8052B790[3] = {
    (const void*)0,
    (const void*)-1,
    (const void*)gateVisionByFusionParam
};

// The remaining functions in this translation unit still use provisional
// pointer types for battle data. Keep their call sites concise while routing
// them through func_800EC918's recovered, strongly typed interface.
#define func_800EC918(manager, player, accessor, event, target)                 \
    func_800EC918(                                                             \
        (manager),                                                             \
        (player),                                                              \
        (EC918_BattleObjAccessor*)(accessor),                                  \
        (BattleEvent*)(event),                                                 \
        (BattleTargetData*)(target)                                            \
    )

// Thunk: dispatch through secondary vtable at +0x8, calling vtable[0x20] on
// the sub-object, passing (subobj, arg).
void CBattleMan_ClearActorStatus(void* ignored, void* self, void* arg) {
    ((cf::CBattleState*)&static_cast<EC918_BattleObjAccessor*>(self)->subObject)->CBattleState_clearStatusId((u32)(uintptr_t)arg);
}
int CBattleMan_GetUnk94Count(void* self, int key) { const unsigned char* item = static_cast<const unsigned char*>(self) + 0x94; for (int i = 0; i < 32; ++i) { if (*reinterpret_cast<const int*>(item) == key) return item[4]; item += 8; } return 0; }
void CBattleMan_MarkList48Flag40(void* this_) {
    Func800F4004_Self* self = static_cast<Func800F4004_Self*>(this_);
    SimpleListNode* anchor = self->listHead;
    SimpleListNode* node = anchor->next;
    while (node != self->listHead) {
        unsigned char* object = static_cast<unsigned char*>(node->data);
        *reinterpret_cast<unsigned int*>(object + 0x3f04) |= 0x40;
        node = node->next;
    }
}
void cf::CBattleManager::CBattleMan_ZeroStateMask() {
    unk84 = 0;
}

namespace cf {
    // CChainCombo's dtor is inline in CChainCombo.hpp: retail only keeps the
    // weak copy at 0x800D9704 and the vtable in CChainCombo.o's .data run.

    CChainMember::~CChainMember() {
    }

    CChainChance::~CChainChance() {
    }

    CChainEffect::~CChainEffect() {
    }

    CChainTime::~CChainTime() {
        // MWCC emits the +0x14 vptr store for the virtual destructor
        // (__vt__Q22cf10CChainTime); the body only resets the timer.
        resetChainTime();
    }

    UnkClass_800D8DBC::~UnkClass_800D8DBC() {
    }
}

// Searches for a matching value in this->field_B8 entries and this->+8 array.
// Returns 1 if field_78 bit 0 is clear, or if a matching non-zero element is
// found without a live actor bound to it. Returns 0 otherwise.
extern "C" s32 findFreeBattleSlot(D7D24_Obj* self) {
    s32 result = 0;
    s32 i;
    u8* p;
    u32* kp;
    u32* entryBase;
    // Retail 0x800D880C shape notes: the 16-word scan is a counted loop that
    // MWCC unrolls 8x into a two-trip CTR loop, keeping the dead induction
    // update (li r4,0 / addi r4,r4,7). The key cursor (kp) and the byte scan
    // cursor (p) stay distinct locals so the match flag lands in r0; the two
    // forward gotos are control-flow exit gates (multi-exit guards), which
    // let the mid/return paths read result from r31 (see MWCC_PATTERNS
    // "Counted loop unrolls to CTR + dead induction counter"). The second
    // pass keeps the self+j*4 entry temp (e) for the retail rlwinm/add shape.
    // kp mirrors entryBase (fixed key base); p is the advancing scan cursor.
    // i is the counted-loop index MWCC converts to CTR (dead update kept).
    // s holds the per-check scan word so the scan side loads first.

    // If bit 0 of field_78 is clear, report 1
    if ((self->field_78 & 1) == 0) {
        result = 1;
    } else {
        entryBase = self->field_B8;
        if (entryBase != nullptr) {
            u32 found;
            kp = entryBase;
            p = (u8*)entryBase;
            for (i = 0; i < 16; i++) {
                u32 s = *(u32*)(p + 8);
                if (s == kp[1]) { found = 1; goto out; }
                p += 4;
            }
            found = 0;
        out:
            if (found) {
                if (findObjectById((s32)kp[1]) != 0) goto done;
            }

            // Second pass: walk the u32 array at self+8 (up to 16 entries); stop at
            // the first non-zero word and report whether it equals self->field_4.
            // Shared done gate reads result once from r31.
            {
                u8* scan = (u8*)self;
                for (s32 j = 0; j < 16; j++) {
                    if (*(u32*)(scan + 8) != 0) {
                        u8* e = (u8*)self + j * 4;
                        if (*(u32*)(e + 8) == self->field_4) {
                            result = 1;
                        }
                        goto done;
                    }
                    scan += 4;
                }
            }
        }
    }
done:
    return result;
}

extern f32 lbl_80666DD4; // 1.0f

extern f32 lbl_80666DD8; // 0.001f
// NOTE: retail symbol names for these battle helpers are C-linkage
// --- shared battle-helper externs used by the updateBattleAction / processBattleMove ports ---

// Calculates accumulated damage/healing value from various status effects.
// Returns a float clamped to >= 0.
extern "C" f32 CBattleMan_ApplyStatusRate(u8* obj, void* target) {
    if (obj == nullptr || target == nullptr) return 1.0f;

    f32 result = 1.0f;
    BattleTargetData* targetData = (BattleTargetData*)target;
    u32 targetFlags = targetData->field_78;
    u32 stackVal;

    if (targetFlags & 0x200) {
        if (isBattleStatusActive(obj + 8, 0xB4)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(obj + 8, 0xB4);
            result -= 0.001f * (f32)(s32)entry->unk10;
        }
    } else if (targetFlags & 0xC00) {
        if (isBattleStatusActive(obj + 8, 0xB5)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(obj + 8, 0xB5);
            result -= 0.001f * (f32)(s32)entry->unk10;
        }
    }

    if (isBattleStatusActive(obj + 8, 0xB3)) {
        cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(obj + 8, 0xB3);
        result += 0.001f * (f32)(s32)entry->unk10;
    }

    if (isBattleStatusActive(obj + 8, 0xB6)) {
        cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(obj + 8, 0xB6);
        result -= 0.001f * (f32)(s32)entry->unk10;
    }

    // Fetch the stat object through CActorParam_getStatusTable (vtable slot
    // 0x290); each IdTable_GetSum receives its return value as arg1
    if (((cf::CActorParam*)(void*)obj)->CActorParam_getStatusTable() != nullptr) {
        // Reload flags from target (retail: lwz r0, 0x78(r31))
        if (targetData->field_78 & 0xC00) {
            if (IdTable_GetSum(((cf::CActorParam*)(void*)obj)->CActorParam_getStatusTable(), 0x64, &stackVal)) {
                result -= 0.001f * (f32)(s32)stackVal;
            }
        }

        if (targetData->field_5C > lbl_eu_80666DDC) {
            if (IdTable_GetSum(((cf::CActorParam*)(void*)obj)->CActorParam_getStatusTable(), 0x6F, &stackVal)) {
                result += 0.001f * (f32)(s32)stackVal;
            }
        }

        if (IdTable_GetSum(((cf::CActorParam*)(void*)obj)->CActorParam_getStatusTable(), 0x71, &stackVal)) {
            if (((EC918_BattleObjAccessor*)obj)->field_1530 != 0) {
                result -= 0.001f * (f32)(s32)stackVal;
            }
        }

        if (IdTable_GetSum(((cf::CActorParam*)(void*)obj)->CActorParam_getStatusTable(), 0x70, &stackVal)) {
            if (((cf::CActorParam*)(void*)obj)->unk3374 & 0x200000) {
                result += 0.001f * (f32)(s32)stackVal;
            }
        }
    }

    if (result < lbl_eu_80666DDC) result = lbl_eu_80666DDC;
    return result;
}

// Arts-data category (+0x3C) / art type (+0x40), read as int (retail cmpwi).
static inline int artsCategory(const BattleSrcObjAccessor* arts) { return arts->type_3c; }
static inline int artsType(const BattleSrcObjAccessor* arts) { return arts->field_40; }

// Calculates the cumulative damage multiplier from status / chain / arts bonuses.
extern "C" float calcBattleDamage(void* obj, void* target, void* source){
    float result = 1.0f;
    s32 v;
    BattleSrcObjAccessor* src;
    D81A8_StatusListView* status = (D81A8_StatusListView*)target;

    // Linked source arts object at +0x50 (or null if there is no source).
    if (source != nullptr) {
        src = ((D81A8_SourceView*)source)->link;
    } else {
        src = nullptr;
    }

    // ---- Target-side bonuses ----
    if (target != nullptr) {
        if (isBattleStatusActive(&status->statusList, 0x8f) != 0) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(&status->statusList, 0x8f);
            result += 0.01f * (f32)((BDSessionEntry*)entry)->field_10;
        } else if (isBattleStatusActive(&status->statusList, 0x8e) != 0) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(&status->statusList, 0x8e);
            result += 0.01f * (f32)((BDSessionEntry*)entry)->field_10;
        }

        if (((cf::CActorParam*)target)->CActorParam_getStatusTable() != nullptr && src != nullptr &&
            (src->field_78 & 0x40000000) && artsCategory(src) == 3) {
            if (IdTable_GetSum(((cf::CActorParam*)target)->CActorParam_getStatusTable(), 0x3e, &v) != 0) {
                result += 0.01f * (f32)v;
            }
        }
    }

    // ---- Attacker-side bonuses ----
    if (obj != nullptr) {
        // Chain gauge when the source is a chain art (or absent).
        if (src == nullptr || artsCategory(src) == 3) {
            if (CChain_HasMemberEntry(&cf::CBattleManager::getInstance()->mChain, (CChainBattleObjTail*)obj) != 0) {
                result += ((CBattleManager_20AC*)cf::CBattleManager::getInstance())->field_20AC;
            }
        }

        if (((cf::CActorParam*)obj)->CActorParam_getStatusTable() != nullptr) {
            if (src != nullptr && (src->field_78 & 0x40000000) && artsCategory(src) == 3) {
                if (IdTable_GetSum(((cf::CActorParam*)obj)->CActorParam_getStatusTable(), 0x3d, &v) != 0) {
                    result += 0.01f * (f32)v;
                }
            }

            // Art type 5 on the source: 0x42 value
            if (src != nullptr && (src->field_78 & 0x40000000) && artsType(src) == 5) {
                if (IdTable_GetSum(((cf::CActorParam*)obj)->CActorParam_getStatusTable(), 0x42, &v) != 0) {
                    result += 0.01f * (f32)v;
                }
            }

            // Generic 0x3f value against the target's move-flag / hit-rate gate
            if (src != nullptr && (src->field_78 & 0x40000000)) {
                if (IdTable_GetSum(((cf::CActorParam*)obj)->CActorParam_getStatusTable(), 0x3f, &v) != 0 &&
                    target != nullptr &&
                    ((cf::CfObjectActor*)obj)->CfObjectActor_sharesMoveFlags((cf::CfObjectActor*)target) != 0 &&
                    ((cf::CActorParam*)target)->CActorParam_getHitRate() < 0.5f) {
                    result += 0.01f * (f32)v;
                }
            }
        }
    }

    return result;
}
// Per-frame battle-manager update: drains the art-countdown timer (unk88),
// resets the sub-systems, and every full second (accumulator at +0x283D4)
// sweeps mActorList1, unregistering every dead actor from all three lists
// and handing it to unregisterBattleActor when the actor-id gate passes.
extern "C" void updateBattleManager(cf::CBattleManager* self) {
    // Frame-delta scaled to seconds (1/30 s per frame).
    f32 delta = Scn_GetFrameDelta(gmScene) * (1.0f / 30.0f);

    if (self->CheckUnk84Flag(0x10)) {
        self->CBattleMan_ClearStateMask(0x10);
        gmGameFlags |= 0x10000000;
    }

    driveVisionRingFx(&self->mVision);
    PartyGaugeTickDecay(&self->unk194);
    updatePtStateTimer(&self->unk19C);
    self->mChain.UpdatePerFrame();
    ChainTmr_Update_D20C(&self->unk20C8);
    SuddenCommuDriveFrame(&self->mSuddenCommu);
    SysWinBuff_EmptyStubB20(&self->mSuddenCommu.unk28);
    SysWinLog_PollBattleEnd();

    if (self->unk88 > lbl_eu_80666DDC) {
        self->unk88 -= delta;
        if (self->unk88 < lbl_eu_80666DDC) {
            self->CBattleMan_ResetTimer();
        } else {
            refreshBattleActors(self, self->unk90, self->unk8C);
        }
    }

    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x4000000) == 0) {
        // Once per second, drop actors that left the field and hand dead
        // actors to unregisterBattleActor (one per sweep; the list mutates).
        self->mVision.unk261C4.unk74 += delta;
        if (self->mVision.unk261C4.unk74 >= 1.0f) {
            _reslist_node<cf::CfObjectActor*>* node = self->mActorList1.mStartNodePtr->mNext;
            while (node != self->mActorList1.mStartNodePtr) {
                cf::CfObjectActor* obj = node->mItem;
                if (isGimmickActive__Q22cf13CfGameManagerFv(static_cast<cf::CfObjectMove*>(obj)) == 0) {
                    self->mActorList1.remove(obj);
                    _reslist_node<cf::CfObjectActor*>* n;
                    for (n = self->mActorList2.mStartNodePtr->mNext; n != self->mActorList2.mStartNodePtr; n = n->mNext) {
                        cf::CfObjectActor* item = n->mItem;
                        if (obj == item) {
                            self->mActorList2.remove(item);
                            break;
                        }
                    }
                    for (n = self->mActorList3.mStartNodePtr->mNext; n != self->mActorList3.mStartNodePtr; n = n->mNext) {
                        cf::CfObjectActor* item = n->mItem;
                        if (obj == item) {
                            self->mActorList3.remove(item);
                            break;
                        }
                    }
                    break;
                }
                if (obj != nullptr) {
                    int state = *(u32*)((cf::CObjectState*)((cf::CfActorField04*)obj)->field_0x04)->getStateData();
                    if (testActorStatus(obj, &state, 0x802)) {
                        unregisterBattleActor(self, obj);
                        break;
                    }
                }
                node = node->mNext;
            }
            self->mVision.unk261C4.unk74 = lbl_eu_80666DDC;
        }
    }
}

// Battle-actor registration: actors already in mActorList1 only refresh the
// busy check; unregistered idle actors are pushed into the lists, notified
// through their vtable holders and the battle-event chain, and reset the
// sudden-commu voice state when they become the lone list-2 member.
// Iterator distance (std::distance-style; the MSL here ships no <iterator>
// distance). Iterators are taken by value, so retail keeps both copies in the frame.
template <typename It>
static inline int iterDistance(It first, It last) {
    int n = 0;
    while (first != last) {
        ++first;
        n++;
    }
    return n;
}

extern "C" void registerBattleActor(void* selfV, void* actorV) {
    cf::CfObjectActor* actor = (cf::CfObjectActor*)actorV;
    cf::CBattleManager* self = (cf::CBattleManager*)selfV;
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x4000000) != 0) return;

    if (actor->unk3374 & 0x08000000) return;
    if (actor->unk3374 & 0x00002000) return;

    // Is the actor already registered in mActorList1?
    _reslist_node<cf::CfObjectActor*>* head1 = self->mActorList1.mStartNodePtr;
    _reslist_node<cf::CfObjectActor*>* cur = head1->mNext;
    while (cur != head1 && cur->mItem != actor) cur = cur->mNext;

    if (cur == head1 && actor->CActorParam_isBattleLocked() == 0) {
        // Register the actor in the lists.
        self->mActorList1.push_back(actor);
        CChain_chkActorList(&self->mChain, actor);
        if (actor->unk64 & 0x2) {
            self->mActorList2.push_back(actor);
        } else {
            self->mActorList3.push_back(actor);
        }

        // Notify the vtable-holder sub-object.
        ((cf::CfObject*)actor->mSubObj38)->CfObject_forwardSubObject();
        broadcastBattleDamage(self, actor, 0, 0, 0, 0, 0);

        // Notify every registered battle event (vtable slot 0xC).
        _reslist_node<cf::IBattleEvent*>* ev =
            self->mBattleEventList.mStartNodePtr->mNext;
        while (ev != self->mBattleEventList.mStartNodePtr) {
            ((cf::IBattleEvent*)ev->mItem)->onActorRegister(actor);
            ev = ev->mNext;
        }

        // If mActorList2 holds at most one actor, reset the voice state.
        int count = iterDistance(self->mActorList2.begin(), self->mActorList2.end());
        if (count <= 1 && (actor->unk64 & 0x2)) {
            CCharVoiceMan_ResetAndAppend();
            SysWinLog_ResetState();
            self->mSuddenCommu.resetPairTimer();
        }

        if (actor->unk64 & 0x4) {
            ((cf::CfObject*)actor->mSubObj38)->CfObject_notifyActiveMember();
            CCharVoiceMan_LatchPendingVoiceId(actor);
        }
    } else {
        // Already registered or busy: retail re-runs the check call.
        actor->CActorParam_isBattleLocked();
    }
}
void unregisterBattleActor(void* mgrV, void* targetV){
    cf::CfObjectActor* target = (cf::CfObjectActor*)targetV;
    cf::CBattleManager* mgr = (cf::CBattleManager*)mgrV;
    cf::CfGameManager::getInstance();
    if(isGlobalCamFlagSet(0x4000000) != 0) return;

    // Only proceed if the target is actually a member of mActorList1.
    if(mgr->mActorList1.find(target) != mgr->mActorList1.end()){
        mgr->mActorList1.removeAll(target);
        CChain_NotifyStateChange(&mgr->mChain, target);

        // Check target flag bit 0x02 at +0x3F00 to decide list
        if(target->unk64 & 0x02){
            mgr->mActorList2.remove(target);
        }else{
            mgr->mActorList3.remove(target);
        }
    }else{
        return;
    }

    // Call vfunc at +0x3ED4 object's vtable[0x7C]
((cf::CfObject*)target->mSubObj38)->CfObject_notifyMemberRemoved();

    // Call target's vtable[0x2E4]
    ((cf::CActorParam*)target)->CActorParam_resetGaugeEntries();

    // If target flag bit 0x04 at +0x3F00 is set, do extra cleanup
    if(target->unk64 & 0x04){
        ((cf::CActorParam*)target)->CActorParam_setupActorState();
        ((cf::CActorParam*)target)->CActorParam_refreshBattleStatus();
        // Call secondary vtable at +0x3E9C, vtable[0x50]
        ((cf::CfObjectActor*)target)->CObjectParam_signalActionEnd(0);
    }

    // Notify all battle events (retail re-reads the sentinel each pass)
    {
        _reslist_node<cf::IBattleEvent*>* cur = mgr->mBattleEventList.mStartNodePtr->mNext;
        while(cur != mgr->mBattleEventList.mStartNodePtr){
            cf::IBattleEvent* event = cur->mItem;
            event->onActorUnregister(target);
            cur = cur->mNext;
        }
    }

    // Drain mActorList1 sentinel loop
    {
        _reslist_node<cf::CfObjectActor*>* cur = mgr->mActorList1.mStartNodePtr->mNext;
        _reslist_node<cf::CfObjectActor*>* head = mgr->mActorList1.mStartNodePtr;
        while(cur != head) cur = cur->mNext;
    }

    // Global list state gates (sequential, with CBattleMan_OnActorsEmpty between)
    if(((cf::CBattleManager*)cf::CBattleManager::spInstance)->mActorList3.size() != 0) return;
    CBattleMan_OnActorsEmpty(mgr);
    if(!(target->unk64 & 0x04)){
        if(((cf::CBattleManager*)cf::CBattleManager::spInstance)->mActorList1.size() != 0) return;
    }

    // Player cleanup loop
    for(int i = 0; i < 3; i++){
        void* player = getPlayer__Q22cf13CfGameManagerFi(i);
        void* action = MapFx_ResolveOwner(player);
        if(action != nullptr){
            ((cf::CObjectState*)((cf::CActorState*)action)->unk4)->clearStateFlags8(0x400000);
            ((cf::CObjectState*)((cf::CActorState*)action)->unk4)->clearStateFlags8(0x800000);
            ((cf::CObjectState*)((cf::CActorState*)action)->unk4)->clearStateFlags8(0x80000000);
            dispatchItemLoad(action, 0x8000000);
            dispatchItemLoad(action, 0x10000000);
        }
    }

    // Call virtual setPartyMaskFlag(mgr, 1, 1)
    mgr->setPartyMaskFlag(1, 1);

    // Clear global flag (rlwinm mask 29..27 clears bit 28)
    gmPauseFlags &= ~0x8;
    CChain_ForwardVoiceClear(&mgr->mChain);

    resetEnumeratedBattleActors();
}

// Actor/Sub/SubObj/Block deleted: use CfObjectActor / CActorParam_UnkStruct1 / UnkStruct2

extern "C" void* getCfObjectPc__FPQ22cf12CfObjectMove(void*);
extern "C" void CCharVoiceMan_CheckAutoTalk(void*);
extern "C" void CChain_StartArtsCombo(void*, void*);
extern "C" void CCharVoiceMan_EnqueueDownedActorVoice(void*);
extern "C" void SysWinLog_AdvanceOwnerLadder(void*);
extern "C" void CCharVoiceMan_EnqueueFreqAndLevelVoice(void*);
extern "C" void rebuildByDistanceCone(void*, void*, f32, f32, f32, s32);
extern "C" void __ct__800FBA18(void*, f32, f32, f32, s32);
extern "C" void rebuildByDualCone(void*, void*, f32, f32, f32, f32, f32, s32);
extern "C" void dispatchBattleAI(void*, void*, void*, void*);
extern "C" f32 FrSqrt__Q24nw4r4mathFf(f32);
extern "C" void Warning__Q24nw4r2dbFPCciPCce(const char*, int, const char*, ...);
extern "C" void* getObjectIdAt(void*, int);
extern f64 lbl_eu_80666E08;
extern f64 lbl_eu_80666DE0;
extern f32 lbl_eu_80666DF0;
extern f32 lbl_eu_80666DF4;
extern f32 lbl_eu_80666DF8;
extern f32 lbl_eu_80666DFC;
extern f32 lbl_eu_80666E14;
extern f32 lbl_eu_80666E28;
extern f32 lbl_eu_80666E18;
extern "C" s32 getCurrentSlotIndex__Q22cf13CfGameManagerFv(void);
extern "C" u32 getAnimModelId(void*);

void initBattleActorState(void* self_, void* actor_) {
    cf::CBattleManager* mgr = (cf::CBattleManager*)self_;
    cf::CfObjectActor* actor = (cf::CfObjectActor*)actor_;
    extern const void* lbl_eu_8052B110[];
    extern const u32 sBattleEffectParamTable[];
    // int->double conversion temporaries (0x4330 trick), pre-seeded once.
    cf::CfActorF64Conv convA;
    cf::CfActorF64Conv convB;
    convA.w[0] = 0x43300000;
    convB.w[0] = 0x43300000;

    // ptmf gate: process only while the manager's 0x28354 event handler is not
    // the default handler recorded in lbl_eu_8052B110.
    u32 defaultHandler[3];
    defaultHandler[0] = (u32)lbl_eu_8052B110[0];
    defaultHandler[1] = (u32)lbl_eu_8052B110[1];
    defaultHandler[2] = (u32)lbl_eu_8052B110[2];
    if (__ptmf_cmpr(&((cf::CBattleManager*)mgr)->mVision.mPtmf, defaultHandler) == 0) {
        return;
    }

    // Register the actor into the manager list when missing.
    {
        SimpleListNode* head = (SimpleListNode*)((cf::CBattleManager*)mgr)->mActorList1.mStartNodePtr;
        SimpleListNode* cur = head->next;
        while (cur != head && cur->data != actor) cur = cur->next;
        if (cur == head) {
            registerBattleActor(mgr, (cf::CfObjectActor*)actor);
        }
    }

    // Linked move record (vt 0x2A4) and its payload at +0x50.
    cf::CActorParam_UnkStruct1* sub = (cf::CActorParam_UnkStruct1*)((cf::CActorParam*)actor)->CActorParam_getMoveRecord();
    cf::CActorParam_UnkStruct2* subObj = (cf::CActorParam_UnkStruct2*)sub->unk50;
    if (subObj == nullptr) return;
    if (sub->unk48 == 0) return;

    // Dispatch target: actor flag bit1 selects the arts-data path.
    void* action;
    if (actor->unk64 & 0x02) {
        cf::CfObjectActor* artsOwner = (cf::CfObjectActor*)getCfObjectPc__FPQ22cf12CfObjectMove(
            actor ? (void*)static_cast<cf::CfObjectMove*>(actor) : (void*)actor);
        if (sub->unk4 != 0) {
            action = (void*)sub->unk4;
        } else {
            action = (void*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)artsOwner)->CObjectParam_getSelfObjectId();
        }
        if (action == nullptr) return;
        if ((void*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)artsOwner) ==
            getPlayer__Q22cf13CfGameManagerFi(0)) {
            ((cf::CBattleManager*)mgr)->CheckUnk84Flag(1);
        }
    } else {
        action = (void*)(uintptr_t)((cf::CObjectParam*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor))->CObjectParam_getSelfObjectId();
    }

    // The action's stats must carry one of the two active bits.
    void* stats = findObjectById((int)(u32)action);
    if (stats == nullptr) return;
    u32 statFlags = *(u32*)((u8*)stats + 0x64);
    if (!(statFlags & 0x04) && !(statFlags & 0x02)) return;
    void* res = MapFx_ResolveOwner(stats);

    // Reset the move record when the 0x08000000 flag is clear.
    if (!(sub->mFlagsArray[1].flags & 0x08000000)) {
        sub->mFlagsArray[0].flags = 0;
        sub->mFlagsArray[1].flags = (sub->mFlagsArray[1].flags & 0xFFFF) | (sub->mFlagsArray[1].flags & 0x00F00000);
        std::memset((u8*)sub + 0x08, 0, 0x40);
        *(f32*)((u8*)sub + 0x68) = ((cf::CActorParam*)res)->CActorParam_getHp();
    }

    CCharVoiceMan_CheckAutoTalk(actor);
    CChain_StartArtsCombo((u8*)&((cf::CBattleManager*)mgr)->mChain, actor);
    dispatchBattleAI((void*)cf::CBattleManager::spInstance, actor, res, sub);

    // 24-word splash parameter table copied pairwise from .rodata.
    u32 splash[24];
    {
        const u32* src = sBattleEffectParamTable;
        u32* dst = splash;
        for (s32 i = 0; i < 12; i++) {
            dst[0] = src[0];
            dst[1] = src[1];
            dst += 2;
            src += 2;
        }
    }

    {
        CfEnumListHolder holder;

        // Range value: scaled target-id (u16 at subObj+0x64).
        convA.w[1] = (u32)subObj->field_64 ^ 0x80000000;
        f32 range = (f32)(convA.d - lbl_eu_80666E08) * ml::deg2rad;
        f32 power = subObj->field_60;
        u32 tblWord = *(u32*)((u8*)splash + subObj->field_5C * 8 +
                              ((actor->unk64 & 0x02) ? 0 : 8));
        u16 alt5E = subObj->field_5E;

        // Skill modifier 0x77 raises the base power inside its window.
        void* skill = ((cf::CActorParam*)actor)->CActorParam_getStatusTable();
        if (skill != nullptr) {
            u32 sv;
            if (IdTable_GetSum(skill, 0x77, &sv) != 0) {
                if (subObj->unk78 & 0x40000000) {
                    if (power <= 20.0f && power >= k15_0f) {
                        convB.w[1] = (u32)(s32)sv ^ 0x80000000;
                        power = power + (f32)(convB.d - lbl_eu_80666DE0);
                        if (power > k25_0f) {
                            power = k25_0f;
                        }
                    }
                }
            }
        }

        // Doubled power for the flagged actor class.
        if ((subObj->unk78 & 0x40000000) && (actor->unk64 & 0x04) &&
            subObj->field_2C != lbl_eu_80666DDC) {
            power *= k2_0f;
        }

        // Hit position: vt 0x12C(0x64) sample, else vt 0xAC vector. Ease the y
        // component halfway toward the sub-record's y, then measure the drop.
        ml::CVec3 hitPos;
        {
    void* sample = static_cast<cf::CfObject*>(actor)->CfObject_getPosSample(0x64);
            if (sample != nullptr) {
                hitPos.x = *(f32*)((u8*)sample + 0x0C);
                hitPos.y = *(f32*)((u8*)sample + 0x1C);
                hitPos.z = *(f32*)((u8*)sample + 0x2C);
            } else {
                hitPos = *(ml::CVec3*)static_cast<cf::CfObject*>(actor)->CfObject_getPosVector();
            }
            void* q = static_cast<cf::CfObject*>(actor)->CfObject_getPosVector();
            hitPos.y = hitPos.y - (hitPos.y - *(f32*)((u8*)q + 0x04)) * 0.5f;
            void* q2 = static_cast<cf::CfObject*>(actor)->CfObject_getPosVector();
            f32 dy = hitPos.y - *(f32*)((u8*)q2 + 0x04);
            if (dy != lbl_eu_80666DDC) {
                f32 distSq = power * power + dy * dy;
                if (distSq < lbl_eu_80666DDC) {
                    Warning__Q24nw4r2dbFPCciPCce(lbl_eu_80526324, 0x273,
                                                 lbl_eu_80526300);
                }
                if (distSq > lbl_eu_80666DDC) {
                    distSq = distSq * FrSqrt__Q24nw4r4mathFf(distSq);
                }
                power = distSq;
            }
        }

        // Chapter-gated chain gauge overrides the hit position with the facing vec.
    f32 gauge = ((cf::CfObjectActor*)actor)->CfObjectActor_getAdjustedFacing();
        {
            u8 chapter = cf::CBattleManager::spInstance->mChain.mHead.field_2;
            if (chapter >= 1 && chapter <= 0x18) {
                hitPos = *(ml::CVec3*)static_cast<cf::CfObject*>(actor)->CfObject_getPosVector();
            }
        }

        // Splash-kind selection: entry 0x95 of the actor's status table forces
        // kind 1 and rescales the range by its entry value.
        s32 splashKind = 0;
        if (!(subObj->unk78 & 0x40000000)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(static_cast<cf::CBattleState*>(actor), 0x95);
            if (entry != nullptr) {
                convA.w[1] = (u32)entry->unk10 ^ 0x80000000;
                range = (f32)(convA.d - lbl_eu_80666DE0) * ml::deg2rad;
                splashKind = 1;
            }
        }
        // Damage variant: 8 unless flagged actor / special ids say otherwise.
        s32 dmgKind = 8;
        if (actor->unk64 & 0x02) {
            dmgKind = 0;
        }
        if (actor->unk64 & 0x04) {
            switch (actor->unk8C_3) {
            case 0x8B3:
            case 0x8B4:
            case 0x8A0:
            case 0x6C5:
            case 0x5F2:
            case 0x607:
            case 0x60B:
            case 0x11C:
            case 0x9C5:
                dmgKind = 0;
                break;
            }
        }

        switch (splashKind) {
        case 1:
            startEnumObjects(holder.get(), tblWord, 0x1000);
            rebuildByDistanceCone(holder.get(), &hitPos, power, range, gauge, dmgKind);
            __ct__800FB044(holder.get(), power, &hitPos, 0);
            break;
        case 2:
            startEnumObjects(holder.get(), tblWord, 0x1000);
            rebuildByDistanceCone(holder.get(), &hitPos, power, range,
                          gauge + ml::pi, dmgKind);
            __ct__800FB044(holder.get(), power, &hitPos, 0);
            break;
        case 3:
            startEnumObjects(holder.get(), tblWord, 0x1000);
            rebuildByDualCone(holder.get(), &hitPos, power, range,
                          gauge + ml::pi * 0.5f, range,
                          gauge - ml::pi * 0.5f, dmgKind);
            __ct__800FB044(holder.get(), power, &hitPos, 0);
            break;
        case 4:
            startEnumObjects(holder.get(), tblWord, 0x1000);
            __ct__800FB044(holder.get(), power, &hitPos, dmgKind);
            break;
        case 5: {
            convB.w[1] = (u32)subObj->field_64 ^ 0x80000000;
            range = (f32)(convB.d - lbl_eu_80666E08) / 100.0f;
            startEnumObjects(holder.get(), tblWord, 0x1000);
            __ct__800FBA18(holder.get(), power, range, gauge, dmgKind);
            __ct__800FB044(holder.get(), power, &hitPos, 0);
            break;
        }
        case 6: {
            convB.w[1] = (u32)subObj->field_64 ^ 0x80000000;
            range = (f32)(convB.d - lbl_eu_80666E08) / 100.0f;
            void* pos = ((cf::CfObject*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)res))->CfObject_getPosVector();
            startEnumObjects(holder.get(), tblWord, 0x1000);
            __ct__800FB044(holder.get(), range, pos, dmgKind);
            break;
        }
        case 7:
            startEnumObjects(holder.get(), tblWord, 0x1000);
            break;
        default: {
            // empty the enum list.
            cf::CfObjEnumList* list = holder.get();
            list->mPtrCount = 0;
            list->mObjInfoCount = 0;
            if (subObj->field_5C == 2) {
                appendObjectById(list, (u32)actor->field_0x74);
            }
            break;
        }
        }

        // Target-type 2: make sure the actor itself ends up on the enum list.
        if (subObj->field_5C == 2) {
            bool registered = false;
            for (u32 i = 0;; i++) {
                cf::CfObjEnumList* list = holder.get();
                if (i >= list->mPtrCount) break;
                if (getObjectIdAt(list, i) == (void*)actor->field_0x74) {
                    registered = true;
                    break;
                }
            }
            if (!registered) {
                appendObjectById(holder.get(), (u32)actor->field_0x74);
            }
        }

    }

    // Snapshot the 0xBC state block through vt 0x298, then publish it back via
    // vt 0x2A0 (vt 0x2B4 fires for side effects only when its flag is clear).
    if (!(sub->mFlagsArray[1].flags & 0x08000000)) {
        ((cf::CActorParam*)actor)->CActorParam_initBattleStats();
    }
    cf::CActorParam_UnkStruct1* snapshot = (cf::CActorParam_UnkStruct1*)((cf::CActorParam*)actor)->CActorParam_getBattleStats();
    *snapshot = *(cf::CActorParam_UnkStruct1*)sub;
    *(cf::CActorParam_UnkStruct1*)((cf::CActorParam*)actor)->CActorParam_getStagingStats() = *snapshot;

    getInstance__Q22cf13CfGameManagerFv();
    if (!isGlobalCamFlagSet(0x04000000)) {
        if (!isBattleStatusActive(static_cast<cf::CBattleState*>(actor), 0x2F)) {
            subObj->field_80 = artsSubGetF14(subObj);
        }
        if (!isBattleStatusActive(static_cast<cf::CBattleState*>(actor), 0x30)) {
            ((cf::CActorParam*)actor)->CActorParam_addHitCharge(lbl_eu_80666DDC);
        }
        if (!isBattleStatusActive(static_cast<cf::CBattleState*>(actor), 0x31)) {
            convB.w[1] = (u32)(s32)subObj->field_34 ^ 0x80000000;
            f32 penalty = (f32)(convB.d - lbl_eu_80666DE0);
            if (penalty < lbl_eu_80666DDC) {
                if (isBattleStatusActive(static_cast<cf::CBattleState*>(actor), 0x12C)) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(static_cast<cf::CBattleState*>(actor), 0x12C);
                    if (entry != nullptr) {
                        convA.w[1] = (u32)entry->unk10 ^ 0x80000000;
                        penalty *= lbl_eu_80666DD4 -
                                   (f32)(convA.d - lbl_eu_80666DE0) / 100.0f;
                    }
                }
            }
            ((cf::CActorParam*)actor)->CActorParam_addArtsGauge(-penalty);
        }
    }

    if (((cf::CfObjectActor*)actor)->unk8C_3 == 1 && (actor->unk64 & 0x02) &&
        (subObj->unk78 & 0x8000)) {
        *(u16*)((cf::CActorParam*)actor)->CActorParam_getArtsSet() = 0;
        static_cast<cf::CBattleState*>(actor)->CBattleState_clearStatusId(0xEA);
    }

    if (subObj->unk78 & 0x40000000) {
        if (actor->unk64 & 0x02) {
            if (subObj->field_28 != 1) {
                bindIndexedEffect(0x7A, (int)(actor ? (void*)static_cast<cf::CfObjectMove*>(actor) : (void*)actor));
                u32 voiceId = 0;
                if (((cf::CActorParam*)actor)->CActorParam_getStatusTable() != 0) {
                    void* skill2 = ((cf::CActorParam*)actor)->CActorParam_getStatusTable();
                    IdTable_GetSum(skill2, 0x3B, &voiceId);
                }
                func_800F3970((void*)cf::CBattleManager::spInstance, actor, nullptr, 0x0B, (s32)voiceId);
            }
        } else {
            if (subObj->unk78 & 0x01) {
                bindIndexedEffect(0x7D, (int)(actor ? (void*)static_cast<cf::CfObjectMove*>(actor) : (void*)actor));
            } else {
                bindIndexedEffect(0x7C, (int)(actor ? (void*)static_cast<cf::CfObjectMove*>(actor) : (void*)actor));
            }
        }
        SysWinLog_AdvanceOwnerLadder(actor);
        CCharVoiceMan_EnqueueFreqAndLevelVoice(actor);
    } else {
        CCharVoiceMan_EnqueueDownedActorVoice(actor);
        if (sub->mFlagsArray[1].flags & 0x02000000) {
            BtlDmg_FilterNotifyDamage((void*)actor->field_0x74, 2, 0x10);
        } else if (sub->mFlagsArray[1].flags & 0x01000000) {
            BtlDmg_FilterNotifyDamage((void*)actor->field_0x74, 1, 0x10);
        }
    }

    if (((cf::CActorParam*)actor)->CActorParam_getStatusTable() != 0) {
        if (findFreeBattleSlot((D7D24_Obj*)sub) != 0) {
            void* skill3 = ((cf::CActorParam*)actor)->CActorParam_getStatusTable();
            if (Counter_TestBit(skill3, 0x3C) != 0) {
                if (sub->mFlagsArray[1].flags & 0x00100000) {
                    if (subObj->field_28 != 1) {
                        ((cf::CActorParam*)actor)->CActorParam_resetTensionState(((cf::CActorParam*)actor)->CActorParam_getStatusCount() + 1);
                    }
                } else if (sub->mFlagsArray[1].flags & 0x00200000) {
                    if (subObj->unk78 & 0x8000) {
                        ((cf::CActorParam*)actor)->CActorParam_resetTensionState(((cf::CActorParam*)actor)->CActorParam_getStatusCount() + 1);
                    }
                }
            }
        }
    }
}
// World position of an actor (its CfObjectMove sub-object).
static inline ml::CVec3* actorPos(cf::CfObjectActor* actor) {
    return static_cast<cf::CfObjectMove&>(*actor).CfObject_getPosVector();
}

// Half-width of an actor's frontal hit cone.
static inline f32 halfHitRange(cf::CActorParam* actor) {
    f32 range = actor->CActorParam_getHitRange();
    return range * lbl_eu_80666DE8;
}

// Bounds of an actor's side hit window: the half cone widened by the
// window center (CActorParam_getHitCenter).
static inline f32 sideConeMin(cf::CActorParam* actor) {
    f32 center = actor->CActorParam_getHitCenter();
    f32 range = actor->CActorParam_getHitRange();
    return -(range * lbl_eu_80666DE8 + center);
}
static inline f32 sideConeMax(cf::CActorParam* actor) {
    f32 center = actor->CActorParam_getHitCenter();
    f32 range = actor->CActorParam_getHitRange();
    return range * lbl_eu_80666DE8 + center;
}

// Bearing of (x, z) in radians (FIdx -> rad), relative to the given facing.
static inline f32 relativeBearing(f32 x, f32 z, f32 facing) {
    f32 bearing = lbl_eu_80666E10 * Atan2FIdx__Q24nw4r4mathFff(x, z);
    return bearing - facing;
}

// dispatchBattleAI: per-move ai dispatch. Fills the 13-entry ai table in arg4,
// computes the bearing to the enemy (nw4r paired-single vector math), sets
// the aim-sector flags, then dispatches to the arts-specific helpers by the
// linked sub-object's type.
extern "C" void dispatchBattleAI(void* mgr, void* obj, void* enemy, void* arg4){
    if(enemy == nullptr) return;

    DB0FC_MoveTable* move = (DB0FC_MoveTable*)arg4;
    if(move->field_78 & 0x10) return;

    // 13-entry random ai table; the two vt[0x28] flags pin entries 0 and 4.
    for(int i = 0; i < 13; i++){
        if(move->field_78 & 0x40) continue;
        move->table[i] = mtRand__Q22ml4mathFi(100);
        if(((cf::CBattleManager*)mgr)->CheckUnk84Flag(0x2000) != 0){
            if(i == 4 || i == 0) move->table[i] = 0x63;
        }else if(((cf::CBattleManager*)mgr)->CheckUnk84Flag(0x4000) != 0){
            if(i == 4 || i == 0) move->table[i] = 0;
        }
    }

    move->field_78 |= 0x40000040;
    void* sub = *(void**)((u8*)move + 0x50);
    if(sub == nullptr) return;

    // Bearing from the move source to the enemy (paired-single subtract).
    ml::CVec3* objPos = actorPos((cf::CfObjectActor*)obj);
    ml::CVec3 diff = *actorPos((cf::CfObjectActor*)enemy) - *objPos;

    // Relative angle, wrapped into [-pi, pi).
    f32 angle;
    if(((cf::CfObjectActor*)enemy)->unk64 & 0x02){
        f32 bearing = lbl_eu_80666E10 * Atan2FIdx__Q24nw4r4mathFff(diff.x, diff.z);
        angle = bearing - ((cf::CfObjectActor*)enemy)->CfObject_getMoveHeadAngle();
        while(ml::pi <= angle) angle -= ml::tau;
        while(angle < -ml::pi) angle += ml::tau;
    }else{
        void* target = ((cf::CfObject*)((u8*)enemy + 0x3E9C))->CfObject_getCurrentTarget();
        angle = relativeBearing(diff.x, diff.z, *(f32*)((u8*)target + 0xC));
        while(ml::pi <= angle) angle -= ml::tau;
        while(angle < -ml::pi) angle += ml::tau;
    }

    // Aim-sector flags from the enemy's facing cone.
    if(-halfHitRange((cf::CActorParam*)enemy) <= angle &&
       angle <= halfHitRange((cf::CActorParam*)enemy)){
        move->field_74 |= 0x82000000;
    }else{
        if(sideConeMin((cf::CActorParam*)enemy) <= angle &&
           angle <= sideConeMax((cf::CActorParam*)enemy)){
            move->field_74 |= 0x84000000;
        }else{
            move->field_74 |= 0x81000000;
        }
    }

    // Dispatch by the linked sub-object's move type.
    switch(((BattleSubObjAccessor*)sub)->type_3c){
    case 1:
    case 2:
        setupBattleMove(mgr, obj, enemy, arg4);
        break;
    case 3:
        handleBattleMoveEvent(mgr, obj, enemy, arg4);
        break;
    case 4:
    case 5:
    case 6:
CBattleMan_HandleMoveType45(mgr, obj, enemy, arg4);
        break;
    }

    move->field_78 |= 0x40000010;
    // u16 dispatch counter at +0x80 of the move block.
    *(u16*)&move->pad_80 = ((cf::CBattleManager*)mgr)->mVision.unk261C4.unk70;
    ((cf::CBattleManager*)mgr)->mVision.unk261C4.unk70++;
}

// Same-TU battle helper (retail-unmangled name; defined later in this file).
extern "C" void prepareBattleMove(void*, void*, void*, void*);

// func_800DCB54 is defined later in this TU with a BattleMoveData* param;
// forward-declare the incomplete type so the declaration matches and the
// call site below stays consistent with the retail tail.
struct BattleMoveData;
extern "C" void func_800DCB54(void* self, void* attacker, void* target, BattleMoveData* move);

// Party slot (1-based) an arts record belongs to.
static inline int artsOwnerSlot(const cf::CAttackParam* arts) { return arts->unk44; }

// Battle move/arts setup: initialises the move block, routes to prepareBattleMove
// or the combo path from the arts object's flag, then runs the damage-gap
// checks (status windows, arts-type gates) that set the counter/knockback
// flags on move->field_74 before the shared func_800DCB54 tail.
extern "C" void setupBattleMove(void* self, void* obj, void* enemyArg, void* arg4) {
    if (enemyArg == 0) return;

    BattleObjAccessor* bob = (BattleObjAccessor*)obj;
    DB4FC_EnemyObj* enemy = (DB4FC_EnemyObj*)enemyArg;
    DB4FC_MoveBlock* move = (DB4FC_MoveBlock*)arg4;

    move->field_58 = 1.0f;
    cf::CAttackParam* arts = (cf::CAttackParam*)move->artsData;
    move->field_54 = 1.0f;
    move->field_5C = lbl_eu_80666DDC;
    move->field_60 = lbl_eu_80666DDC;
    move->field_64 = lbl_eu_80666DDC;

    if (bob->field_3f00 & 0x2) {
        if (move->field_78 & 0x400) {
            refreshPtStateAccum(&((cf::CBattleManager*)self)->unk19C);
        }
    }

    if (arts->unk78 & 0x8) {
        move->field_74 |= 0x80000001;
    } else {
        prepareBattleMove(self, obj, enemyArg, arg4);
    }

if (CChain_HasMemberEntry(&((cf::CBattleManager*)self)->mChain, (CChainBattleObjTail*)obj) != 0) {
        move->field_74 |= 0x80004000;
    }

    if (move->field_78 & 0x20) return;

    if ((move->field_74 & 1) && !(move->field_74 & 2) &&
        !(enemy->field_3374 & 0x1000) &&
((cf::CfObject*)((u8*)enemy + 0x3E9C))->CfObject_getMoveFactor() &&
        isBattleStatusActive(&enemy->statusBase, 0x32) == 0) {
        s32 flag = 0;

            if (isBattleStatusActive(&enemy->statusBase, 0xCE) && (move->field_78 & 0x800)) {
                cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findTopBattleStatusEntry(&enemy->statusBase, 0xCE);
                s32 val = arts->CAttackParam_getArtsFlagByte();
                if (entry->unk10 >= val) flag = 1;
            }

        if (flag == 0 &&
            (!(arts->unk78 & 0x2000) ||
             artsOwnerSlot(arts) == ((cf::CActorParam*)bob)->CActorParam_getStatsSlot() + 1)) {
            int type = arts->unk58;
            if (type == 1) {
                if (!isBattleStatusActive(&enemy->statusBase, 0xF) &&
                    !isBattleStatusActive(&enemy->statusBase, 0x10)) {
                    if (isBattleStatusActive(&enemy->statusBase, 0x29)) {
                        cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry(&enemy->statusBase, 0x29);
                        if (move->field_B4 >= e->unk10) {
                            move->field_74 |= 0x80010000;
                        }
                    } else {
                        move->field_74 |= 0x80010000;
                    }
                }
            } else if (type == 2) {
                int canApply = 1;
                if (((cf::CActorParam*)enemy)->CActorParam_getStatusTable()) {
                    if (Counter_TestBit(((cf::CActorParam*)enemy)->CActorParam_getStatusTable(), 0x76)) canApply = 0;
                }
                if (isBattleStatusActive(&enemy->statusBase, 0x28)) {
                    cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry(&enemy->statusBase, 0x28);
                    if (move->field_B4 < e->unk10) canApply = 0;
                }
                if (canApply) move->field_74 |= 0x80020000;
            }
        }
    }

    func_800DCB54(self, obj, enemyArg, (BattleMoveData*)arg4);
}
// Battle event handler: the damage-gap checks of setupBattleMove (status
// windows, arts-type gates) that set the counter/knockback flags on
// move->field_74, followed by the emitBattleEvents tail.
void handleBattleMoveEvent(void* mgr, void* attacker, void* enemyArg, void* arg4) {
    if (enemyArg == nullptr) return;

    DB4FC_EnemyObj* enemy = (DB4FC_EnemyObj*)enemyArg;
    DB4FC_MoveBlock* move = (DB4FC_MoveBlock*)arg4;
    cf::CAttackParam* arts = (cf::CAttackParam*)move->artsData;

    move->field_78 |= 0x40000100;

    if (!(move->field_74 & 2) &&
        !(enemy->field_3374 & 0x1000) &&
        ((cf::CfObject*)((u8*)enemy + 0x3E9C))->CfObject_getMoveFactor() &&
        isBattleStatusActive(&enemy->statusBase, 0x32) == 0) {
        s32 flag = 0;

        if (isBattleStatusActive(&enemy->statusBase, 0xCE) && (move->field_78 & 0x800)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findTopBattleStatusEntry(&enemy->statusBase, 0xCE);
            s32 val = arts->CAttackParam_getArtsFlagByte();
            if (entry->unk10 >= val) flag = 1;
        }

        if (flag == 0) {
            int type = arts->unk58;
            if (type == 1) {
                if (!isBattleStatusActive(&enemy->statusBase, 0xF) &&
                    !isBattleStatusActive(&enemy->statusBase, 0x10)) {
                    if (isBattleStatusActive(&enemy->statusBase, 0x29)) {
                        cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry(&enemy->statusBase, 0x29);
                        if (move->field_B4 >= e->unk10) {
                            move->field_74 |= 0x80010000;
                        }
                    } else {
                        move->field_74 |= 0x80010000;
                    }
                }
            } else if (type == 2) {
                int canApply = 1;
                if (((cf::CActorParam*)enemy)->CActorParam_getStatusTable()) {
                    if (Counter_TestBit(((cf::CActorParam*)enemy)->CActorParam_getStatusTable(), 0x76)) canApply = 0;
                }
                if (isBattleStatusActive(&enemy->statusBase, 0x28)) {
                    cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry(&enemy->statusBase, 0x28);
                    if (move->field_B4 < e->unk10) canApply = 0;
                }
                if (canApply) move->field_74 |= 0x80020000;
            }
        }
    }

    emitBattleEvents(mgr, attacker, enemyArg, arg4);
}
extern void spawnPcEffect(u16 type, int slotSel);

void CBattleMan_HandleMoveType45(void* self, BattleObjAccessor* obj, void* arg1, BattleMoveObjAccessor* arg2) {
    BattleSubObjAccessor* subObj = static_cast<BattleSubObjAccessor*>(*(void**)((u8*)arg2 + 0x50));
    if (subObj == nullptr) return;
    
    if (obj->field_3f00 & 0x2) {
        if (obj->field_3f28 == 7) {
            if (arg2->field_78 & 0x400) {
                int result = getFirstFreeSlot();
                if (result != -1) {
                    spawnPcEffect(subObj->field_40, -1);
                }
            }
        }
    }
    
    // Retail range-checks type in {5,6} via (type-5) <= 1 unsigned
    if ((u32)(subObj->type_3c - 5) <= 1) {
        setupBattleMove(self, obj, arg1, arg2);
    }
}
// prepareBattleMove (retail 0x800DBACC, 0x838 bytes): target-selection / hit-intent
// gate for the shared move dispatchers. Runs the status + art-type gates on the
// target, computes hit / critical intent from the sub-object stats and
// level/party modifiers, then sets the appropriate field_74 flag (1 = hit,
// 2 = miss, 3 = dodge-class) or falls through to the damage-splash tail.

// Target deleted: use CfObjectActor

// Sub deleted: use CActorParam_UnkStruct2

// Move deleted: use CActorParam_UnkStruct1

void prepareBattleMove(void* self, BattleObjAccessor* arg1, void* arg2, void* move_) {
    cf::CActorParam_UnkStruct1* move = (cf::CActorParam_UnkStruct1*)move_;
    if (arg2 == nullptr) return;

    cf::CfObjectActor* atk = (cf::CfObjectActor*)arg1;
    cf::CfObjectActor* tgt = (cf::CfObjectActor*)arg2;

    void* prm1 = ((cf::CActorParam*)arg1)->CActorParam_getBattleParams();
    void* prm2 = ((cf::CActorParam*)arg2)->CActorParam_getBattleParams();
    cf::CActorParam_UnkStruct2* sub = move->unk50;

    s32 res = 0;
    if (sub->unk78 & 0x8) res = 2;
    if ((atk->unk64 & 0x4) && (sub->unk78 & 0x1)) res = 2;
    if (move->mFlagsArray[1].flags & 0x1000) res = 2;

    s32 hitVal = 0;
    if (res == 0) {
        // Status gate chain: any check firing blocks the action outright.
        int sv;
        bool blocked;
        sv = *(s32*)((cf::CObjectState*)tgt->CActorState::unk4)->getStateData();
        blocked = testActorStatus(tgt, &sv, 0x805) != 0;
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0x9) != 0;
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0xb) != 0;
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0xf) != 0;
        if (!blocked) { sv = *(s32*)((cf::CObjectState*)tgt->CActorState::unk4)->getStateData(); blocked = testActorStatus(tgt, &sv, 0x4) != 0; }
        if (!blocked) { sv = *(s32*)((cf::CObjectState*)tgt->CActorState::unk4)->getStateData(); blocked = testActorStatus(tgt, &sv, 0x19) != 0; }
        if (!blocked) { sv = *(s32*)((cf::CObjectState*)tgt->CActorState::unk4)->getStateData(); blocked = testActorStatus(tgt, &sv, 0x1a) != 0; }
        if (!blocked) { sv = *(s32*)((cf::CObjectState*)tgt->CActorState::unk4)->getStateData(); blocked = testActorStatus(tgt, &sv, 0x1b) != 0; }
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0x32) != 0;
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0xf8) != 0;
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0xeb) != 0;
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0xf0) != 0;
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0xf1) != 0;
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0xf8) != 0;
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0xf7) != 0;
        if (!blocked) blocked = isBattleStatusActive((u8*)arg2 + 8, 0x106) != 0;
        // Extra-object page gate.
        if (!blocked && ((void*)((cf::CfObjectActor*)tgt)->mTargetC4) == nullptr) blocked = true;
        if (!blocked && getAnimModelId(((void*)((cf::CfObjectActor*)tgt)->mTargetC4)) == 0x31) blocked = true;

        if (!blocked) {
            f32 chance = lbl_eu_80666DDC;
            if (sub->field_3C == 1 || sub->field_3C == 5) {
                f32 lo = lbl_eu_80666DDC;   // floor
                f32 hi = 95.0f;   // cap
                f32 mod = lbl_eu_80666DD4;  // party modifier
                chance = lo;
                if (((cf::CActorParam*)arg1)->CActorParam_getStatusTable() != 0) {
                    u32 q1;
                    if (IdTable_GetSum(((cf::CActorParam*)arg1)->CActorParam_getStatusTable(), 0x1D, &q1)) {
                        mod -= (f32)q1 / 100.0f;
                    }
                    u32 q2;
                    if (IdTable_GetSum(((cf::CActorParam*)arg1)->CActorParam_getStatusTable(), 0x7E, &q2) && (move->mFlagsArray[0].flags & 0x40)) {
                        chance = lo - (f32)q2;
                    }
                }
                if ((atk->unk64 & 0x2) && ((cf::CActorParam*)arg1)->CActorParam_getStatusCount() == 0) res = 1;
                if ((atk->unk64 & 0x2) && ((cf::CActorParam*)arg1)->CActorParam_getStatusCount() == 1) chance += k50_0f * mod;
                if (isBattleStatusActive((u8*)arg2 + 8, 0x116)) res = 1;
                s32 lv = ((cf::CActorParam*)arg2)->CActorParam_getActorLevel() - ((cf::CActorParam*)arg1)->CActorParam_getActorLevel();
                if (lv < -10) lv = -10;
                if (lv > 10) lv = 10;
                if ((u32)(lv + 5) > 10) chance += (f32)(lv * 20);
                else if ((u32)(lv + 2) > 4) chance += (f32)(lv * 8);
                chance -= (f32)*(s16*)((u8*)prm2 + 0x36);
                if (atk->unk64 & 0x2) chance += k50_0f * (f32)*(s16*)((u8*)prm1 + 0x36);
                if (getCurrentSlotIndex__Q22cf13CfGameManagerFv() == 4) {
                    if (atk->unk64 & 0x2) {
                        if (isBattleStatusActive((u8*)arg1 + 8, 0xa2)) {
                            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg1 + 8, 0xa2);
                            chance -= (f32)e->unk10;
                        }
                    } else if (tgt->unk64 & 0x2) {
                        if (isBattleStatusActive((u8*)arg2 + 8, 0xa2)) {
                            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg2 + 8, 0xa2);
                            s32 ev = e->unk10;
                            chance += (f32)(ev / 2);
                        }
                    }
                }
                if (isBattleStatusActive((u8*)arg2 + 8, 0xd1)) chance += 200.0f;
                if (isBattleStatusActive((u8*)arg2 + 8, 0xf5)) {
                    chance += k50_0f;
                    if (((cf::CActorParam*)arg2)->CActorParam_getHitRate() < 0.15f) chance += k25_0f;
                } else if (isBattleStatusActive((u8*)arg1 + 8, 0xf5)) {
                    chance -= k50_0f;
                    if (((cf::CActorParam*)arg2)->CActorParam_getHitRate() < 0.15f) chance -= k25_0f;
                }
                if (isBattleStatusActive((u8*)arg2 + 8, 0x114)) chance += 200.0f;
                f32 f1 = chance - (f32)(*(s16*)((u8*)sub + 0x36) * artsSubGetMax(sub));
                f1 = f1 + (f32)(*(s16*)((u8*)prm2 + 0x1E) - *(s16*)((u8*)prm1 + 0x1E));
                if (f1 < lo) f1 = lo;
                if (f1 > hi) f1 = hi;
                if (res == 0) {
                    if (move->unk84[1] % 100 < (s32)f1) res = 1;
                }
                hitVal = (s32)f1;
            } else {
                f32 lo = chance;              // floor
                f32 hi = 90.0f;     // cap
                s32 e0 = ((cf::CActorParam*)arg2)->CActorParam_getActorType();
                if (e0 == 2 || e0 == 0xa) {
                    u32 f3 = atk->unk3374;
                    if ((f3 & 0x4000) && !(f3 & 0x8000)) res = 3;
                }
                if ((atk->unk64 & 0x2) && ((cf::CActorParam*)arg1)->CActorParam_getStatusCount() == 0) res = 3;
                if (isBattleStatusActive((u8*)arg2 + 8, 0x116)) res = 3;
                if ((atk->unk64 & 0x2) && ((cf::CActorParam*)arg1)->CActorParam_getStatusCount() == 1) chance += k25_0f;
                void* v290 = ((cf::CActorParam*)arg1)->CActorParam_getStatusTable();
                if (v290 != nullptr) {
                    if (Counter_TestBit(((cf::CActorParam*)arg1)->CActorParam_getStatusTable(), 0x82)) {
                        chance -= (f32)(*(s16*)((u8*)sub + 0x36) * artsSubGetMax(sub));
                    }
                }
                s32 lv = ((cf::CActorParam*)arg2)->CActorParam_getActorLevel() - ((cf::CActorParam*)arg1)->CActorParam_getActorLevel();
                if (lv < -10) lv = -10;
                if (lv > 10) lv = 10;
                if ((u32)(lv + 5) > 10) chance += (f32)(lv * 11);
                else if ((u32)(lv + 2) > 4) chance += (f32)(lv * 6);
                if (atk->unk64 & 0x2) chance -= (f32)(*(s16*)((u8*)sub + 0x36) * artsSubGetMax(sub));
                if (chance < lo) chance = lo;
                if (chance > hi) chance = hi;
                if (res == 0) {
                    if (move->unk84[1] % 100 < (s32)chance) res = 3;
                }
                hitVal = (s32)chance;
            }
        }
    }
    if (res == 0) res = 2;
    // Apply the intent flag: 1 = hit, 2 = miss/blocked, 3 = dodge-class.
    if (res == 1) move->mFlagsArray[0].flags |= 0x80000004;
    else if (res == 2) move->mFlagsArray[0].flags |= 0x80000001;
    else if (res == 3) move->mFlagsArray[0].flags |= 0x80000008;

    if (move->mFlagsArray[1].flags & 0x1000) return;

    bool splash = false;
    if (sub->field_3C == 1 || sub->field_3C == 5) {
        if (!(sub->unk78 & 0x8) && (move->mFlagsArray[0].flags & 1)) {
            s32 e0 = ((cf::CActorParam*)arg2)->CActorParam_getActorType();
            u32 f3374 = atk->unk3374;
            if (e0 == 1) {
                if (!isBattleStatusActive((u8*)arg2 + 8, 0x9) && !isBattleStatusActive((u8*)arg2 + 8, 0xf) &&
                    !isBattleStatusActive((u8*)arg2 + 8, 0x10)) {
                    if (!isBattleStatusActive((u8*)arg1 + 8, 0xea) && !isBattleStatusActive((u8*)arg1 + 8, 0xcf) &&
                        !isBattleStatusActive((u8*)arg1 + 8, 0xd0) && !(f3374 & 0x4000)) {
                        if (move->unk84[1] < 100) splash = true;
                    }
                }
            } else if (e0 == 2) {
                if (!isBattleStatusActive((u8*)arg2 + 8, 0x9) && !isBattleStatusActive((u8*)arg2 + 8, 0xf) &&
                    !isBattleStatusActive((u8*)arg2 + 8, 0x10)) {
                    if (!(f3374 & 0x8000) && !isBattleStatusActive((u8*)arg1 + 8, 0xd0)) {
                        if (move->unk84[1] < 100) splash = true;
                    }
                }
            } else if (e0 == 0xa) {
                if ((f3374 & 0x4000) && !(f3374 & 0x8000) && move->unk84[1] < 100) splash = true;
            }
        }
    } else if (sub->field_3C == 2 || sub->field_3C == 6) {
        if (isBattleStatusActive((u8*)arg2 + 8, 0xf7)) {
            if (((cf::CfObjectActor*)arg2)->CfObjectActor_sharesMoveFlags((cf::CfObjectActor*)arg1) == 0) {
                if (move->mFlagsArray[0].flags & 1) {
                    findBattleStatusEntry((u8*)arg2 + 8, 0xf7);
                    u32 f78 = move->mFlagsArray[1].flags;
                    if ((f78 & 0x200) || (!(f78 & 0x400) && !(f78 & 0x800))) {
                        move->mFlagsArray[0].flags |= 0x80000080;
                    }
                }
            }
        }
    }

    if (splash) {
        // Guaranteed-damage tail: set ratio fields, tag the move, bump the
        // shared dispatch counter.
        move->unk54 = lbl_eu_80666DD4;
        move->mFlagsArray[0].flags = (move->mFlagsArray[0].flags | 0x80000000) | 0x41;
        move->unk5C = move->unk54 * move->unk58;
        if (isBattleStatusActive((u8*)arg2 + 8, 0x1)) move->unk5C = lbl_eu_80666DDC;
        move->mFlagsArray[1].flags |= 0x40000020;
        move->unk80 = ((cf::CBattleManager*)self)->mVision.unk261C4.unk70++;
        return;
    }

    if (!(move->mFlagsArray[0].flags & 1)) return;

    // Critical-proc gate chain.
    s32 res2 = 0;
    int sv;
    bool blocked2;
    sv = *(s32*)((cf::CObjectState*)tgt->CActorState::unk4)->getStateData();
    blocked2 = testActorStatus(tgt, &sv, 0x805) != 0;
    if (!blocked2) { sv = *(s32*)((cf::CObjectState*)tgt->CActorState::unk4)->getStateData(); blocked2 = testActorStatus(tgt, &sv, 0x4) != 0; }
    if (!blocked2) { sv = *(s32*)((cf::CObjectState*)tgt->CActorState::unk4)->getStateData(); blocked2 = testActorStatus(tgt, &sv, 0x19) != 0; }
    if (!blocked2) { sv = *(s32*)((cf::CObjectState*)tgt->CActorState::unk4)->getStateData(); blocked2 = testActorStatus(tgt, &sv, 0x1a) != 0; }
    if (!blocked2) { sv = *(s32*)((cf::CObjectState*)tgt->CActorState::unk4)->getStateData(); blocked2 = testActorStatus(tgt, &sv, 0x1b) != 0; }
    if (!blocked2) blocked2 = isBattleStatusActive((u8*)arg2 + 8, 0x9) != 0;
    if (!blocked2) blocked2 = isBattleStatusActive((u8*)arg2 + 8, 0xb) != 0;
    if (!blocked2) blocked2 = isBattleStatusActive((u8*)arg2 + 8, 0xf) != 0;
    if (!blocked2) blocked2 = isBattleStatusActive((u8*)arg2 + 8, 0xf0) != 0;
    if (!blocked2) blocked2 = isBattleStatusActive((u8*)arg2 + 8, 0xf1) != 0;
    if (!blocked2) blocked2 = isBattleStatusActive((u8*)arg2 + 8, 0xf8) != 0;
    if (blocked2) res2 = 1;
    if (res2 == 0 && sub->field_3C != 1) res2 = 1;
    if (res2 == 0 && isBattleStatusActive((u8*)arg2 + 8, 0xeb) && !(move->mFlagsArray[1].flags & 0x800)) res2 = 2;
    if (res2 == 0 && isBattleStatusActive((u8*)arg2 + 8, 0x10c) && !(move->mFlagsArray[1].flags & 0x800)) res2 = 2;
    if (res2 == 0 && (atk->unk64 & 0x4) && (sub->unk78 & 0x1)) res2 = 1;
    if (sub->unk78 & 0x8) res2 = 1;
    if (res2 == 0) {
        s32 thr;
        if (tgt->unk64 & 0x2) {
            s32 d = ((cf::CActorParam*)arg2)->CActorParam_getActorLevel() - ((cf::CActorParam*)arg1)->CActorParam_getActorLevel();
            if ((u32)(d + 2) > 4) d = d * 5;
            thr = *(u8*)((u8*)sub + 0x55) + d;
        } else {
            thr = *(u8*)((u8*)sub + 0x55) + hitVal;
        }
        if (thr < 0) thr = 0;
        if (thr > 0x5f) thr = 0x5f;
        if (move->unk84[2] % 100 < thr) res2 = 2;
    }
    if (res2 == 2) move->mFlagsArray[0].flags |= 0x80000002;
}
struct BattleMoveSubData {
    u8 pad_00[0x38];
    s16 field_38;           // +0x38
    s16 field_3A;           // +0x3A
    u16 type_3C;            // +0x3C move type (1/5 -> main block)
    u16 field_3E;
    u16 field_40;           // +0x40 art type (0..9, dispatch switches)
    u8 pad_42[0x48 - 0x42];
    u16 field_48;           // +0x48
    u8 pad_4A[0x5E - 0x4A];
    u16 field_5E;           // +0x5E
    u8 pad_60[0x6C - 0x60];
    u8 field_6C;            // +0x6C
    u8 field_6D;            // +0x6D
    u8 pad_6E[0x72 - 0x6E];
    u8 field_72;            // +0x72 skill bitfield byte
    u8 field_73;            // +0x73 skill bitfield byte
    u8 pad_74[0x78 - 0x74];
    u32 field_78;           // +0x78 flags
    u8 pad_7C[0x84 - 0x7C];
    void** field_84;        // +0x84 sub-vtable (slot 0x0C used)
};

// Move/event data (r31).
struct BattleMoveData {
    u8 pad_00[0x50];
    void* field_50;         // +0x50 sub-object (BattleMoveSubData)
    f32 field_54;           // +0x54 multiplicative damage ratio
    f32 field_58;           // +0x58 additive damage ratio
    f32 field_5C;           // +0x5C base damage
    f32 field_60;           // +0x60 damage variance
    f32 field_64;           // +0x64 (init 0.0)
    f32 field_68;           // +0x68 (dot/status damage accumulator)
    f32 field_6C;           // +0x6C (hp delta accumulator)
    s16 field_70;           // +0x70 (dot tick counter)
    u8 pad_72[0x74 - 0x72];
    u32 field_74;           // +0x74 tagged pointer / flag word
    u32 field_78;           // +0x78 flags
    u8 pad_7C[0x94 - 0x7C];
    u32 field_94;           // +0x94 (used in % math for variance)
    u32 field_98;           // +0x98 (100-based proc chance)
    u8 pad_9C[0xA8 - 0x9C];
    u32 field_A8;           // +0xA8 (100-based proc chance)
    u8 pad_AC[0xB0 - 0xAC];
    u32 field_B0;           // +0xB0 (100-based proc chance)
};

// Param struct returned by vf0x224 (r23 = attacker's, r22 = target's).
struct BattleParamData {
    u8 pad_00[0x1E];
    s16 field_1E;           // +0x1E
    u8 pad_20[0x24 - 0x20];
    f32 field_24;           // +0x24
    f32 field_28;           // +0x28
    s16 field_2C;           // +0x2C
    s16 field_2E;           // +0x2E
    s16 field_30;           // +0x30
    s16 field_32;           // +0x32
    u8 pad_34[0x38 - 0x34];
    s16 field_38;           // +0x38
    s16 field_3A;           // +0x3A
    u8 pad_3C[0x54 - 0x3C];
    u8 field_54;            // +0x54
    u8 pad_55[0x60 - 0x55];
    s16 field_60;           // +0x60 (range, main block)
    s16 field_62;           // +0x62 (range, alt block)
    s16 field_64;           // +0x64
    s16 field_66;           // +0x66
    s16 field_68;           // +0x68
    s16 field_6A;           // +0x6A
    s16 field_6C;           // +0x6C
    s16 field_6E;           // +0x6E
    u8 field_70;            // +0x70 skill bitfield byte
    u8 field_71;
    u8 field_72;            // +0x72 skill bitfield byte
    u8 field_73;            // +0x73 skill bitfield byte
};

// Secondary vtable at +0x3E9C, slot 0xAC returns a position/velocity vec.
struct BattleVec {
    f32 x;                  // +0x00 (ps pair)
    f32 y;                  // +0x04 (also read as f32 at +0x4)
    f32 z;                  // +0x08
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names kept verbatim)
// ---------------------------------------------------------------------------

// (Former VFn* manual-slot typedefs removed: slots 0x224/0x20C/0x328/0x2A4/
// 0x108/0xE0/0x290/0x308/0x158/0x12C/0x2BC/0x30/0x54/0x5C are
// CActorParam Func100/94/165/132/29/19/127/157/49/38/138,
// CObjectState Func11 and CBattleState Func20/22; call sites use the real
// virtuals directly.)

// (Former d_vf290/d_vfAC manual-dispatch helpers removed: slot 0x290 is
// CActorParam::CActorParam_getStatusTable and slot 0xAC on the +0x3E9C
// move sub-object is CfObject::CfObject_getPosVector; call sites use
// the real virtuals directly.)

// ---------------------------------------------------------------------------
// func_800DCB54
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names kept verbatim)
// ---------------------------------------------------------------------------

// Static tables decoded from retail .data (see notes for full listing).

// ---------------------------------------------------------------------------
extern "C" void func_800DCB54(void* self, void* attacker, void* target,
                   BattleMoveData* move) {
    // ----------------------------------------------------------
    // 0x5150 prologue / early guards
    // ----------------------------------------------------------
    // Declare sub before pool so MWCC colors sub=r21, pool=r24 (retail).
    BattleMoveSubData* sub;
    // Shared out-param for testActorStatus (lifetimes don't overlap). One slot
    // keeps frame at -336; five separate locals grew it to -352.
    s32 outVal;
    // Pool base (retail lis/addi r24 = sBattleEventTable before beq).
    u8* pool = (u8*)sBattleEventTable;
    // Plain u8* bases: register+const folds per use -- no CSE into r15/r16.
    u8* atkB = (u8*)attacker;
    u8* tgtB = (u8*)target;

    if (target == 0) return;                        // 0x5154 cmpwi r5,0

    // 0x51C0: load tag, init ratios (58/54/5C/60/64), then bit0 gate
    {
        u32 tag = move->field_74;
        f32 zero = lbl_eu_80666DDC;
        f32 one = lbl_eu_80666DD4;
        move->field_58 = one;                       // 0x51D0
        move->field_54 = one;                       // 0x51D4
        move->field_5C = zero;                      // 0x51D8
        move->field_60 = zero;                      // 0x51DC
        move->field_64 = zero;                      // 0x51E0
        if (!(tag & 0x1)) return;                   // 0x51CC/0x51E4
    }

    // 0x51E8: grab params (sub loaded before first vcall -- retail r21)
    sub = (BattleMoveSubData*)move->field_50;
    BattleParamData* atkParam =
        (BattleParamData*)(BattleParamData*)((cf::CActorParam*)(attacker))->CActorParam_getBattleParams();
    BattleParamData* tgtParam =
        (BattleParamData*)(BattleParamData*)((cf::CActorParam*)(target))->CActorParam_getBattleParams();
    {
        void* res20C = ((cf::CActorParam*)(target))->CActorParam_getArtsDataBlock();

        // 0x5230: type gate -- only 1 and 5 take the main block (lhz + signed cmpi)
        if ((s32)sub->type_3C == 1 || (s32)sub->type_3C == 5) {
        // MAIN BLOCK (types 1/5), 0x800DD730
        // ================================================================
        f32 f26;
        f32 f28;
        {
            f26 = (f32)*(s16*)((u8*)res20C + 0x60) / 100.0f;
            f28 = (f32)tgtParam->field_60 / 100.0f - f26;
        }
        s32 atkValue = atkParam->field_2C;
        s32 tgtValue = tgtParam->field_2E;
        // --- hit-flag conditional zero of f26 (0x800DD78C) ---
        // Reload field_74 after float convert (retail lwz after fdivs).
        {
            u32 tag = move->field_74;
            // Retail: field_74 bits 6/5/7 gate hf bits 0x2/0x4/0x1 (rlwinm 30/29/31).
            if (tag & 0x2000000) {
                u16 hf = *(u16*)((cf::CActorParam*)(target))->CActorParam_getBattleHitFlags();
                if (!(hf & 0x2)) f26 = lbl_eu_80666DDC;
            } else if (tag & 0x4000000) {
                u16 hf = *(u16*)((cf::CActorParam*)(target))->CActorParam_getBattleHitFlags();
                if (!(hf & 0x4)) f26 = lbl_eu_80666DDC;
            } else if (tag & 0x1000000) {
                u16 hf = *(u16*)((cf::CActorParam*)(target))->CActorParam_getBattleHitFlags();
                if (!(hf & 0x1)) f26 = lbl_eu_80666DDC;
            }
        }
        f28 += f26;                                     // 0x800DD804

        // 0x800DD80C: sub->field_78 bit24 (0x80) -> average r25
        if (sub->field_78 & 0x80) {
            atkValue = (atkValue + atkParam->field_1E) / 2;
        }

        // --- clamp [-1.0, 0.9] ---
        if (f28 < -1.0f) f28 = -1.0f;
        else if (f28 > 0.9f) f28 = 0.9f;

        // --- zero ratio under data-map flags ---
        // Retail: li r26,0 then f28=(f32)r26 via xoris/fsubs (not lfs 0.0).
        if (isBattleStatusActive(tgtB + 8, 0x13)) {
            tgtValue = 0;
            f28 = (f32)(s32)tgtValue;
        }
        if (bmFlagBit(*(u32*)(atkB + 0x3F00), 0x4) &&
            (move->field_78 & 0x800)) {
            tgtValue = 0;
            f28 = (f32)(s32)tgtValue;
        }

        // --- hit quality flags (0x800DD8B8) ---
        // Literal-first compares -> retail fcmpo operand order (f0,f28).
        if (lbl_eu_80666DDC < f28 && f28 < 0.5f) {
            move->field_74 |= 0x80100000;
        } else if (0.5f <= f28 && f28 < 1.0f) {
            move->field_74 |= 0x80200000;
        }

        // --- guard path: move->field_78 bit21 (0x200) ---
        if (move->field_78 & 0x200) {
            s32 vf = artsSubGetMax(sub);
            s32 rangeValue = sub->field_38 + (s32)sub->field_6C * (vf - 1);
            if (bmFlagBit(*(u32*)(atkB + 0x3F00), 0x2)) {
                // Dual fctiwz spills (retail sp+168/176): keep both floats live.
                f32 f2 = atkParam->field_24;
                f32 atkValueF = (f32)(s32)atkValue;
                f32 f1 = atkParam->field_28 - f2;
                u32 rollValue = move->field_94;
                f32 dF = lbl_eu_80666DD4 + f1;
                s32 d = (s32)dF;
                f32 sumF = atkValueF + f2;
                atkValue = (s32)sumF + ((s32)rollValue % d);
            } else {
                rangeValue = (s32)((f32)(s32)rangeValue *
                           (f32)(s32)(((s32)move->field_94 % 21) + 90) /
                           100.0f);
            }
            move->field_54 =
                (f32)(s32)(atkValue - tgtValue) * (f32)(s32)rangeValue / 100.0f;

            if (isBattleStatusActive(atkB + 8, 0xC8)) {
                findBattleStatusEntry(atkB + 8, 0xC8);       // presence (retail always fetches)
                move->field_54 *= 0.5f;
            }
            if (move->field_54 < lbl_eu_80666DD4)
                move->field_54 = lbl_eu_80666DD4;
            move->field_54 = move->field_54 - move->field_54 * f28;
            goto main_tail;
        }

        // --- non-guard: move->field_78 bits 20-21 (0x600) ---
        if (move->field_78 & 0x600) {
            // 0x800DDAB0: same r25 variance as above
            if (bmFlagBit(*(u32*)(atkB + 0x3F00), 0x2)) {
                f32 f2 = atkParam->field_24;
                f32 atkValueF = (f32)(s32)atkValue;
                f32 f1 = atkParam->field_28 - f2;
                u32 rollValue = move->field_94;
                f32 dF = 1.0f + f1;
                s32 d = (s32)dF;
                f32 sumF = atkValueF + f2;
                atkValue = (s32)sumF + ((s32)rollValue % d);
            }
            // three sub-vtable calls; lo/hi stay live across call3 (retail r18/r19)
            s32 lo = sub->field_38 + (s32)sub->field_6C * (artsSubGetMax(sub) - 1);
            s32 hi = sub->field_3A + (s32)sub->field_6D * (artsSubGetMax(sub) - 1);
            s32 roll = (s32)move->field_94;
            s32 vmax = artsSubGetMax(sub);
            s32 span = hi - lo + 1;
            s32 picked = sub->field_38 + (s32)sub->field_6C * (vmax - 1) +
                         (s32)(roll % span);
            move->field_54 =
                (f32)(s32)(atkValue - tgtValue) * (f32)(s32)picked / 100.0f;       // 0x800DDBE4

            // 0x800DDBF0: clamp + apply ratio
            if (move->field_54 < lbl_eu_80666DD4)
                move->field_54 = lbl_eu_80666DD4;
            move->field_54 = move->field_54 - move->field_54 * f28;

            // 0x800DDC10: 0x93 -> field_58 += 0.5 ; 0xC0 -> += val/10 - 1
            if (isBattleStatusActive(atkB + 8, 0x93)) {
                move->field_58 += 0.5f;
            }
            if (isBattleStatusActive(atkB + 8, 0xC0)) {
                cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(atkB + 8, 0xC0);
                move->field_58 +=
                    (f32)(s32)(u32)entry->unk10 / 10.0f - 1.0f;
            }
            // MAIN: no 0x4E/0x55/0x56 status block (ALT-only at 0x800DEF20).
        }

    main_tail:;
        // ================================================================
        // MAIN-BLOCK TAIL (0x800DDC80): combo / level-diff / vf0x308
        // ================================================================
        // --- 0x800DDC80: target vf0x2A4 (combo) checks ---
        {
            void* p = ((cf::CActorParam*)(target))->CActorParam_getMoveRecord();
            if (*(u32*)((u8*)p + 0x78) & 0x400) {
                void* f4 = *(void**)(tgtB + 0x4);
                outVal = *(s32*)(((cf::CObjectState*)(f4))->getStateData());
                // Retail: cmpi r3,0 (no bool-normalize neg/or).
                if (testActorStatus(target, &outVal, 0x806) == 0 &&
                    !(*(u16*)(tgtB + 0x3E6C) & 0x20))
                    goto main_combo_skip;
                void* sub2 = ((cf::CActorParam*)(target))->CActorParam_getMoveRecord();
                if (*(void**)((u8*)sub2 + 0x50) == 0) goto main_combo_skip;
                void* sub3 = ((cf::CActorParam*)(target))->CActorParam_getMoveRecord();
                void* sub4 = *(void**)((u8*)sub3 + 0x50);
                // MAIN: bonus applies when combo target type != 1
                if ((s32)*(u16*)((u8*)sub4 + 0x3C) == 1) goto main_combo_skip;
                void* sub5 = ((cf::CActorParam*)(target))->CActorParam_getMoveRecord();
                void* sub6 = *(void**)((u8*)sub5 + 0x50);
                if ((s32)*(u8*)((u8*)sub6 + 0x42) == 1) goto main_combo_skip;
                move->field_58 += 0.5f;                 // 0x800DDD48
            }
        main_combo_skip:;
        }

        // --- 0x800DDD58: level-difference multiplier (main thresholds) ---
        {
            s32 tl = (s32)((cf::CActorParam*)(target))->CActorParam_getActorLevel();
            s32 al = (s32)((cf::CActorParam*)(attacker))->CActorParam_getActorLevel();
            s32 diff = tl - al;
            if (diff < -2) {
                if (diff >= -5) move->field_54 *= 1.25f;
                else if (diff >= -9) move->field_54 *= 1.5f;
                else move->field_54 *= 2.0f;
            } else {
                if (diff >= 6) move->field_54 *= 0.5f;
                else if (diff >= 3) move->field_54 *= 0.75f;
            }
        }

        // --- 0x800DDE18: field_74 bit15 (0x4000) -> vf0x308 table ---
        // Retail: u32-copy 5 words to stack, vcall, lfsx from the copy.
        if (move->field_74 & 0x4000) {
            u32* src = (u32*)(pool + 0x150);
            u32 tbl[5];
            tbl[0] = src[0];
            tbl[1] = src[1];
            tbl[2] = src[2];
            tbl[3] = src[3];
            tbl[4] = src[4];
            s32 idx = ((cf::CActorParam*)(attacker))->CActorParam_getStatusCount();
            move->field_58 += ((f32*)tbl)[idx];
        }

        // --- 0x800DDE7C: art-type dispatch (guard: !data 0x13) ---
        if (!isBattleStatusActive(tgtB + 8, 0x13)) {
            u16 artType = sub->field_40;
            s32 r0 = 0;

            // ---- dispatch A (0x800DDE90): byte 0x72, negate, thr 200 ----
            // Retail layout: early JT first (<=2), then stat/thr + fail JT.
            if ((u32)(artType - 1) <= 2) {
                if (artType != 0) {
                    switch (artType) {
                        case 1: case 2: case 3:
                            r0 = bmByteBit01(tgtParam->field_72, 0x80); break;
                        case 4: r0 = bmByteBit01(tgtParam->field_72, 0x01); break;
                        case 5: r0 = bmByteBit01(tgtParam->field_72, 0x02); break;
                        case 6: r0 = bmByteBit01(tgtParam->field_72, 0x04); break;
                        case 7: r0 = bmByteBit01(tgtParam->field_72, 0x08); break;
                        case 8: r0 = bmByteBit01(tgtParam->field_72, 0x10); break;
                        case 9: r0 = bmByteBit01(tgtParam->field_72, 0x20); break;
                        default: r0 = 0; break;
                    }
                }
            } else if (artType != 0) {
                s16 val = 0;
                switch (artType) {
                    case 4: val = tgtParam->field_64; break;
                    case 5: val = tgtParam->field_66; break;
                    case 6: val = tgtParam->field_68; break;
                    case 7: val = tgtParam->field_6A; break;
                    case 8: val = tgtParam->field_6C; break;
                    case 9: val = tgtParam->field_6E; break;
                }
                if (val >= 200) {
                    r0 = 1;
                } else {
                    switch (artType) {
                        case 1: case 2: case 3:
                            r0 = bmByteBit01(tgtParam->field_72, 0x80); break;
                        case 4: r0 = bmByteBit01(tgtParam->field_72, 0x01); break;
                        case 5: r0 = bmByteBit01(tgtParam->field_72, 0x02); break;
                        case 6: r0 = bmByteBit01(tgtParam->field_72, 0x04); break;
                        case 7: r0 = bmByteBit01(tgtParam->field_72, 0x08); break;
                        case 8: r0 = bmByteBit01(tgtParam->field_72, 0x10); break;
                        case 9: r0 = bmByteBit01(tgtParam->field_72, 0x20); break;
                        default: r0 = 0; break;
                    }
                }
            }
            if (r0 != 0) {
                move->field_54 *= -1.0f;
                move->field_74 |= 0x80000800;
                goto main_post;
            }

            // ---- dispatch B (0x800DE0F4): byte 0x70, halve, thr 100 ----
            if ((u32)(artType - 1) <= 2) {
                if (artType != 0) {
                    switch (artType) {
                        case 1: case 2: case 3:
                            r0 = bmByteBit01(tgtParam->field_70, 0x80); break;
                        case 4: r0 = bmByteBit01(tgtParam->field_70, 0x01); break;
                        case 5: r0 = bmByteBit01(tgtParam->field_70, 0x02); break;
                        case 6: r0 = bmByteBit01(tgtParam->field_70, 0x04); break;
                        case 7: r0 = bmByteBit01(tgtParam->field_70, 0x08); break;
                        case 8: r0 = bmByteBit01(tgtParam->field_70, 0x10); break;
                        case 9: r0 = bmByteBit01(tgtParam->field_70, 0x20); break;
                        default: r0 = 0; break;
                    }
                }
            } else if (artType != 0) {
                s16 val = 0;
                switch (artType) {
                    case 4: val = tgtParam->field_64; break;
                    case 5: val = tgtParam->field_66; break;
                    case 6: val = tgtParam->field_68; break;
                    case 7: val = tgtParam->field_6A; break;
                    case 8: val = tgtParam->field_6C; break;
                    case 9: val = tgtParam->field_6E; break;
                }
                if (val >= 100) {
                    r0 = 1;
                } else {
                    switch (artType) {
                        case 1: case 2: case 3:
                            r0 = bmByteBit01(tgtParam->field_70, 0x80); break;
                        case 4: r0 = bmByteBit01(tgtParam->field_70, 0x01); break;
                        case 5: r0 = bmByteBit01(tgtParam->field_70, 0x02); break;
                        case 6: r0 = bmByteBit01(tgtParam->field_70, 0x04); break;
                        case 7: r0 = bmByteBit01(tgtParam->field_70, 0x08); break;
                        case 8: r0 = bmByteBit01(tgtParam->field_70, 0x10); break;
                        case 9: r0 = bmByteBit01(tgtParam->field_70, 0x20); break;
                        default: r0 = 0; break;
                    }
                }
            }
            if (r0 != 0) {
                move->field_54 *= 0.5f;
                goto main_post;
            }

            // ---- dispatch C (0x800DE344): early 0x72, thr path 0x73, +0.25 ----
            if ((u32)(artType - 1) <= 2) {
                if (artType != 0) {
                    switch (artType) {
                        case 1: case 2: case 3:
                            r0 = bmByteBit01(tgtParam->field_72, 0x80); break;
                        case 4: r0 = bmByteBit01(tgtParam->field_72, 0x01); break;
                        case 5: r0 = bmByteBit01(tgtParam->field_72, 0x02); break;
                        case 6: r0 = bmByteBit01(tgtParam->field_72, 0x04); break;
                        case 7: r0 = bmByteBit01(tgtParam->field_72, 0x08); break;
                        case 8: r0 = bmByteBit01(tgtParam->field_72, 0x10); break;
                        case 9: r0 = bmByteBit01(tgtParam->field_72, 0x20); break;
                        default: r0 = 0; break;
                    }
                }
            } else if (artType != 0) {
                s16 val = 0;
                switch (artType) {
                    case 4: val = tgtParam->field_64; break;
                    case 5: val = tgtParam->field_66; break;
                    case 6: val = tgtParam->field_68; break;
                    case 7: val = tgtParam->field_6A; break;
                    case 8: val = tgtParam->field_6C; break;
                    case 9: val = tgtParam->field_6E; break;
                }
                if (val < 0) {
                    r0 = 1;
                } else {
                    switch (artType) {
                        case 1: case 2: case 3:
                            r0 = bmByteBit01(tgtParam->field_73, 0x80); break;
                        case 4: r0 = bmByteBit01(tgtParam->field_73, 0x01); break;
                        case 5: r0 = bmByteBit01(tgtParam->field_73, 0x02); break;
                        case 6: r0 = bmByteBit01(tgtParam->field_73, 0x04); break;
                        case 7: r0 = bmByteBit01(tgtParam->field_73, 0x08); break;
                        case 8: r0 = bmByteBit01(tgtParam->field_73, 0x10); break;
                        case 9: r0 = bmByteBit01(tgtParam->field_73, 0x20); break;
                        default: r0 = 0; break;
                    }
                }
            }
            if (r0 != 0) {
                move->field_58 += 0.25f;
            }

        }
    main_post:;
        // ================================================================
        // 0x800DE590 (post-dispatch, shared): position-diff, 0xCE reaction,
        // crit block.
        // ================================================================
        // --- 0x800DE590: attacker/target chain-pos diff -> field_58 ---
        if (*(u32*)(tgtB + 0x3374) & 0x20000) {
            BattleVec* vT = (BattleVec*)((cf::CfObject*)(tgtB + 0x3E9C))->CfObject_getPosVector();
            BattleVec* vA = (BattleVec*)((cf::CfObject*)(atkB + 0x3E9C))->CfObject_getPosVector();
            f32 d = vA->y - vT->y;
            if (d < lbl_eu_80666DDC) d = -d;
            if (d <= 10.0f) move->field_58 += 2.0f;         // 0x800DE5E8
        }

        // --- 0x800DE5F8: 0xCE reaction block ---
        if (isBattleStatusActive(tgtB + 8, 0xCE)) {
            if (move->field_78 & 0x800) {
                cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findTopBattleStatusEntry(tgtB + 8, 0xCE);
                if ((s32)(u32)entry->unk10 >= artsSubGetMax(sub)) {
                    move->field_54 = 1.0f;
                    move->field_74 |= 0x80002000;
                }
            }
        }

        // --- 0x800DE660: field_74 bit1 (0x2) -> direct ratio clamp, else crit ---
        if (move->field_74 & 0x2) {
            // 0x800DE66C: field_54 *= (0.5 - (f32)targetParam->field_54)
            move->field_54 *= 0.5f - (f32)tgtParam->field_54;
        } else {
            // ================================================================
            // CRIT BLOCK (0x800DE698..0x800DE8DC) -- r20 accumulation
            // ================================================================
        s32 critRate = atkParam->field_38;
        // Retail: isolate bit -> double cntlzw bool -> lhax from pool+0x208 (post-call).
        if (bmFlagBit(*(u32*)(atkB + 0x3F00), 0x2)) {
            s32 idx = ((cf::CActorParam*)(attacker))->CActorParam_getStatusCount();
            s16* critTbl = (s16*)(pool + 0x208);
            critRate += critTbl[idx];
        }
        bool critBoost = false;
        if ((sub->field_78 & 0x4) &&
            (*(u32*)(tgtB + 0x3374) & 0x100)) {   // bit23 -> 0x100
            critBoost = true;
            critRate += 0x19;
        }
        if (sub->field_78 & 0x10) {
            f32 f1 = ((cf::CActorParam*)(attacker))->CActorParam_getArtsGauge();
            critRate = (s32)((f32)(s32)critRate + f1 * 0.5f);
        }
        if (sub->field_78 & 0x2) {
            critRate = 100;
        }
        {
            void* st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
            if (st != 0) {
                st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
                if (Counter_TestBit(st, 0x59) && (move->field_78 & 0x2000000)) {
                    critRate = 100;
                }
                st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
                if (Counter_TestBit(st, 0x5A) && (move->field_78 & 0x1000000)) {
                    critRate = 100;
                }
            }
        }
        if (isBattleStatusActive(tgtB + 8, 0xC2)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(tgtB + 8, 0xC2);
            critRate -= (s32)(u32)entry->unk10;
        }
        // 0x800DE804: signed (field_98 % 100) < r20 -> crit (mulhw), else skip
        if ((s32)move->field_98 % 100 < critRate) {
            f32 f26 = 0.25f;
            if (isBattleStatusActive(tgtB + 8, 0xF) ||
                isBattleStatusActive(tgtB + 8, 0x10)) {
                f26 += 0.25f;                            // 0.5 total
            }
            if (critBoost) f26 += 0.25f;
            if ((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable() != 0) {
                u32 sv;
                if (IdTable_GetSum((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable(), 0x51, &sv)) {
                    f26 += (f32)(s32)sv / 100.0f;
                }
            }
            move->field_58 += f26;
            move->field_74 |= 0x80000100;
        }

        }
        } else {
            // ============================================================
            // ALT BLOCK (0x800DE8FC): same shape as main, uses +0x62 ranges,
            // r23+0x30 / r22+0x32 hit positions.
            // ============================================================
            f32 f26;
            f32 f28;
            {
                f26 = (f32)*(s16*)((u8*)res20C + 0x62) / 100.0f;
                f28 = (f32)tgtParam->field_62 / 100.0f - f26;
            }
            s32 atkValue = atkParam->field_30;
            s32 tgtValue = tgtParam->field_32;

            // --- hit-flag conditional zero of f26 (0x800DE948) ---
            {
                u32 tag = move->field_74;
                if (tag & 0x2000000) {
                    u16 hf = *(u16*)((cf::CActorParam*)(target))->CActorParam_getBattleHitFlags();
                    if (!(hf & 0x2)) f26 = lbl_eu_80666DDC;
                } else if (tag & 0x4000000) {
                    u16 hf = *(u16*)((cf::CActorParam*)(target))->CActorParam_getBattleHitFlags();
                    if (!(hf & 0x4)) f26 = lbl_eu_80666DDC;
                } else if (tag & 0x1000000) {
                    u16 hf = *(u16*)((cf::CActorParam*)(target))->CActorParam_getBattleHitFlags();
                    if (!(hf & 0x1)) f26 = lbl_eu_80666DDC;
                }
            }
            f28 += f26;                                 // 0x800DE9D0

            // --- clamp [-1.0, 0.9] ---
            if (f28 < -1.0f) f28 = -1.0f;
            else if (f28 > 0.9f) f28 = 0.9f;

            // --- zero ratio under data-map flags ---
            if (isBattleStatusActive(tgtB + 8, 0x13)) { // 0x800DE9F8
                tgtValue = 0;
                f28 = (f32)(s32)tgtValue;
            }
            if (bmFlagBit(*(u32*)(atkB + 0x3F00), 0x4) &&
                (move->field_78 & 0x800)) {             // 0x800DEA24
                tgtValue = 0;
                f28 = (f32)(s32)tgtValue;
            }

            // --- hit quality flags (0x800DEA64) ---
            if (lbl_eu_80666DDC < f28 && f28 < 0.5f) {
                move->field_74 |= 0x80400000;
            } else if (0.5f <= f28 && f28 < 1.0f) {
                move->field_74 |= 0x80800000;
            }

            // --- guard path: move->field_78 bit21 (0x200) ---
            if (move->field_78 & 0x200) {
                // 0x800DEAC0: variance computation
                s32 vf = artsSubGetMax(sub);
                s32 rangeValue = sub->field_38 + (s32)sub->field_6C * (vf - 1);
                if (bmFlagBit(*(u32*)(atkB + 0x3F00), 0x2)) {
                    f32 f2 = atkParam->field_24;
                    f32 atkValueF = (f32)(s32)atkValue;
                    f32 f1 = atkParam->field_28 - f2;
                    u32 rollValue = move->field_94;
                    f32 dF = 1.0f + f1;
                    s32 d = (s32)dF;
                    f32 sumF = atkValueF + f2;
                    atkValue = (s32)sumF + ((s32)rollValue % d);
                } else {
                    // 0x800DEB60: (f94 % 21) + 90 over 100
                    rangeValue = (s32)((f32)(s32)rangeValue *
                               (f32)(s32)(((s32)move->field_94 % 21) + 90) / 100.0f);
                }
                move->field_54 = (f32)(s32)(atkValue - tgtValue) * (f32)(s32)rangeValue / 100.0f;

                if (isBattleStatusActive(atkB + 8, 0xC8)) {
                    findBattleStatusEntry(atkB + 8, 0xC8);
                    move->field_54 *= 0.5f;
                }
                if (move->field_54 < 1.0f) move->field_54 = 1.0f;
                move->field_54 = move->field_54 - move->field_54 * f28;
                goto ratio_done;
            }

            // --- non-guard: move->field_78 bits 20-21 (0x600) ---
            if (move->field_78 & 0x600) {
                // 0x800DEC58: same r25 variance as above
                if (bmFlagBit(*(u32*)(atkB + 0x3F00), 0x2)) {
                    f32 f2 = atkParam->field_24;
                    f32 atkValueF = (f32)(s32)atkValue;
                    f32 f1 = atkParam->field_28 - f2;
                    u32 rollValue = move->field_94;
                    f32 dF = 1.0f + f1;
                    s32 d = (s32)dF;
                    f32 sumF = atkValueF + f2;
                    atkValue = (s32)sumF + ((s32)rollValue % d);
                }
                // three sub-vtable calls; lo/hi stay live across call3 (retail r18/r19)
                s32 lo = sub->field_38 + (s32)sub->field_6C * (artsSubGetMax(sub) - 1);
                s32 hi = sub->field_3A + (s32)sub->field_6D * (artsSubGetMax(sub) - 1);
                s32 roll = (s32)move->field_94;
                s32 vmax = artsSubGetMax(sub);
                s32 span = hi - lo + 1;
                s32 picked = sub->field_38 + (s32)sub->field_6C * (vmax - 1) +
                             (s32)(roll % span);
                move->field_54 =
                    (f32)(s32)(atkValue - tgtValue) * (f32)(s32)picked / 100.0f;   // 0x800DED94

                // --- 0x800DED98: vf0xE0-based half-damage selection ---
                f32 f26b = 1.0f;
                if (bmFlagBit(*(u32*)(atkB + 0x3F00), 0x2)) {
                    s32 etype = (s32)((cf::CActorParam*)(target))->CActorParam_getActorType();
                    u16 f28t = (*(u16*)(atkB + 0x3F28));
                    if (etype == 1) {
                        if (f28t != 1 && f28t != 8 &&
                            !isBattleStatusActive(atkB + 8, 0xCF) &&
                            !isBattleStatusActive(atkB + 8, 0xD0) &&
                            !(*(u32*)(atkB + 0x3374) & 0x4000)) {
                            f26b = 0.5f;
                        }
                    } else {
                        s32 etype2 = (s32)((cf::CActorParam*)(target))->CActorParam_getActorType();
                        if (etype2 == 2) {
                            if (f28t != 8 &&
                                !isBattleStatusActive(atkB + 8, 0xD0) &&
                                !(*(u32*)(atkB + 0x3374) & 0x8000)) {
                                f26b = 0.5f;
                            }
                        }
                    }
                }
                move->field_54 *= f26b;                 // 0x800DEE70

                // --- 0x800DEE7C: 0x10A bonus ---
                if ((move->field_78 & 0x800) &&
                    isBattleStatusActive(atkB + 8, 0x10A)) {
                    move->field_58 += 1.0f;
                }

                // 0x800DEEAC: clamp + apply ratio
                if (move->field_54 < 1.0f) move->field_54 = 1.0f;
                move->field_54 = move->field_54 - move->field_54 * f28;

                // 0x800DEEC4: 0xC0 skill -> field_58 += val/10 - 1
                if (isBattleStatusActive(atkB + 8, 0xC0)) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(atkB + 8, 0xC0);
                    move->field_58 +=
                        (f32)(s32)(u32)entry->unk10 / 10.0f - 1.0f;
                }
                // ALT only (0x800DEF20): attacker status 0x4E / 0x55 / 0x56
                {
                    void* st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
                    if (st != 0) {
                        u32 sv;
                        st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
                        if (IdTable_GetSum(st, 0x4E, &sv)) {
                            move->field_58 += (f32)(s32)sv / 100.0f;
                        }
                        if (sub->field_40 == 6) {
                            st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
                            if (IdTable_GetSum(st, 0x55, &sv)) {
                                move->field_58 += (f32)(s32)sv / 100.0f;
                            }
                        }
                        if (sub->field_40 == 8) {
                            st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
                            if (IdTable_GetSum(st, 0x56, &sv)) {
                                move->field_58 += (f32)(s32)sv / 100.0f;
                            }
                        }
                    }
                }
            }
        ratio_done:;
            // ============================================================
            // ALT-BLOCK TAIL (0x800DF050): combo / level-diff / vf0x308
            // ============================================================
            // --- 0x800DF050: target vf0x2A4 (combo) checks ---
            {
                void* p = ((cf::CActorParam*)(target))->CActorParam_getMoveRecord();
                if (*(u32*)((u8*)p + 0x78) & 0x400) {
                    void* f4 = *(void**)(tgtB + 0x4);
                    outVal = *(s32*)(((cf::CObjectState*)(f4))->getStateData());
                    if (testActorStatus(target, &outVal, 0x806) == 0 &&
                        !(*(u16*)(tgtB + 0x3E6C) & 0x20))
                        goto alt_combo_skip;
                    void* sub2 = ((cf::CActorParam*)(target))->CActorParam_getMoveRecord();
                    if (*(void**)((u8*)sub2 + 0x50) == 0) goto alt_combo_skip;
                    void* sub3 = ((cf::CActorParam*)(target))->CActorParam_getMoveRecord();
                    void* sub4 = *(void**)((u8*)sub3 + 0x50);
                    // ALT: bonus applies when combo target type == 1
                    if ((s32)*(u16*)((u8*)sub4 + 0x3C) != 1) goto alt_combo_skip;
                    void* sub5 = ((cf::CActorParam*)(target))->CActorParam_getMoveRecord();
                    void* sub6 = *(void**)((u8*)sub5 + 0x50);
                    if ((s32)*(u8*)((u8*)sub6 + 0x42) == 1) goto alt_combo_skip;
                    move->field_58 += 0.5f;             // 0x800DF11C
                }
            alt_combo_skip:;
            }

            // --- 0x800DF128: level-difference multiplier (alt thresholds) ---
            {
                s32 tl = (s32)((cf::CActorParam*)(target))->CActorParam_getActorLevel();
                s32 al = (s32)((cf::CActorParam*)(attacker))->CActorParam_getActorLevel();
                s32 diff = tl - al;
                if (diff >= 7) move->field_54 *= 0.5f;
                else if (diff >= 4) move->field_54 *= 0.75f;
                else if (diff <= -7) move->field_54 *= 1.5f;
                else if (diff <= -4) move->field_54 *= 1.25f;
            }

            // --- 0x800DF1C4: field_74 bit15 (0x4000) -> vf0x308 table ---
            if (move->field_74 & 0x4000) {
                u32* src = (u32*)(pool + 0x150);
                u32 tbl[5];
                tbl[0] = src[0];
                tbl[1] = src[1];
                tbl[2] = src[2];
                tbl[3] = src[3];
                tbl[4] = src[4];
                s32 idx = ((cf::CActorParam*)(attacker))->CActorParam_getStatusCount();
                move->field_58 += ((f32*)tbl)[idx];
            }

            // --- 0x800DF228: art-type dispatch (guard: !data 0x13) ---
            if (!isBattleStatusActive(tgtB + 8, 0x13)) {
                    u16 artType = sub->field_40;
                s32 r0 = 0;

                // ---- dispatch A' (0x800DF23C): byte 0x72, negate, thr 200 ----
                if ((u32)(artType - 1) <= 2) {
                    if (artType != 0) {
                        switch (artType) {
                            case 1: case 2: case 3:
                                r0 = bmByteBit01(tgtParam->field_72, 0x80); break;
                            case 4: r0 = bmByteBit01(tgtParam->field_72, 0x01); break;
                            case 5: r0 = bmByteBit01(tgtParam->field_72, 0x02); break;
                            case 6: r0 = bmByteBit01(tgtParam->field_72, 0x04); break;
                            case 7: r0 = bmByteBit01(tgtParam->field_72, 0x08); break;
                            case 8: r0 = bmByteBit01(tgtParam->field_72, 0x10); break;
                            case 9: r0 = bmByteBit01(tgtParam->field_72, 0x20); break;
                            default: r0 = 0; break;
                        }
                    }
                } else if (artType != 0) {
                    s16 val = 0;
                    switch (artType) {
                        case 4: val = tgtParam->field_64; break;
                        case 5: val = tgtParam->field_66; break;
                        case 6: val = tgtParam->field_68; break;
                        case 7: val = tgtParam->field_6A; break;
                        case 8: val = tgtParam->field_6C; break;
                        case 9: val = tgtParam->field_6E; break;
                    }
                    if (val >= 200) {
                        r0 = 1;
                    } else {
                        switch (artType) {
                            case 1: case 2: case 3:
                                r0 = bmByteBit01(tgtParam->field_72, 0x80); break;
                            case 4: r0 = bmByteBit01(tgtParam->field_72, 0x01); break;
                            case 5: r0 = bmByteBit01(tgtParam->field_72, 0x02); break;
                            case 6: r0 = bmByteBit01(tgtParam->field_72, 0x04); break;
                            case 7: r0 = bmByteBit01(tgtParam->field_72, 0x08); break;
                            case 8: r0 = bmByteBit01(tgtParam->field_72, 0x10); break;
                            case 9: r0 = bmByteBit01(tgtParam->field_72, 0x20); break;
                            default: r0 = 0; break;
                        }
                    }
                }
                if (r0 != 0) {
                    move->field_54 *= -1.0f;
                    move->field_74 |= 0x80000800;
                    goto alt_post;
                }

                // ---- dispatch B' (0x800DF4A0): byte 0x70, halve, thr 100 ----
                if ((u32)(artType - 1) <= 2) {
                    if (artType != 0) {
                        switch (artType) {
                            case 1: case 2: case 3:
                                r0 = bmByteBit01(tgtParam->field_70, 0x80); break;
                            case 4: r0 = bmByteBit01(tgtParam->field_70, 0x01); break;
                            case 5: r0 = bmByteBit01(tgtParam->field_70, 0x02); break;
                            case 6: r0 = bmByteBit01(tgtParam->field_70, 0x04); break;
                            case 7: r0 = bmByteBit01(tgtParam->field_70, 0x08); break;
                            case 8: r0 = bmByteBit01(tgtParam->field_70, 0x10); break;
                            case 9: r0 = bmByteBit01(tgtParam->field_70, 0x20); break;
                            default: r0 = 0; break;
                        }
                    }
                } else if (artType != 0) {
                    s16 val = 0;
                    switch (artType) {
                        case 4: val = tgtParam->field_64; break;
                        case 5: val = tgtParam->field_66; break;
                        case 6: val = tgtParam->field_68; break;
                        case 7: val = tgtParam->field_6A; break;
                        case 8: val = tgtParam->field_6C; break;
                        case 9: val = tgtParam->field_6E; break;
                    }
                    if (val >= 100) {
                        r0 = 1;
                    } else {
                        switch (artType) {
                            case 1: case 2: case 3:
                                r0 = bmByteBit01(tgtParam->field_70, 0x80); break;
                            case 4: r0 = bmByteBit01(tgtParam->field_70, 0x01); break;
                            case 5: r0 = bmByteBit01(tgtParam->field_70, 0x02); break;
                            case 6: r0 = bmByteBit01(tgtParam->field_70, 0x04); break;
                            case 7: r0 = bmByteBit01(tgtParam->field_70, 0x08); break;
                            case 8: r0 = bmByteBit01(tgtParam->field_70, 0x10); break;
                            case 9: r0 = bmByteBit01(tgtParam->field_70, 0x20); break;
                            default: r0 = 0; break;
                        }
                    }
                }
                if (r0 != 0) {
                    move->field_54 *= 0.5f;
                    goto alt_post;
                }

                // ---- dispatch C' (0x800DF6F0): early 0x72, thr path 0x73, +0.25 ----
                if ((u32)(artType - 1) <= 2) {
                    if (artType != 0) {
                        switch (artType) {
                            case 1: case 2: case 3:
                                r0 = bmByteBit01(tgtParam->field_72, 0x80); break;
                            case 4: r0 = bmByteBit01(tgtParam->field_72, 0x01); break;
                            case 5: r0 = bmByteBit01(tgtParam->field_72, 0x02); break;
                            case 6: r0 = bmByteBit01(tgtParam->field_72, 0x04); break;
                            case 7: r0 = bmByteBit01(tgtParam->field_72, 0x08); break;
                            case 8: r0 = bmByteBit01(tgtParam->field_72, 0x10); break;
                            case 9: r0 = bmByteBit01(tgtParam->field_72, 0x20); break;
                            default: r0 = 0; break;
                        }
                    }
                } else if (artType != 0) {
                    s16 val = 0;
                    switch (artType) {
                        case 4: val = tgtParam->field_64; break;
                        case 5: val = tgtParam->field_66; break;
                        case 6: val = tgtParam->field_68; break;
                        case 7: val = tgtParam->field_6A; break;
                        case 8: val = tgtParam->field_6C; break;
                        case 9: val = tgtParam->field_6E; break;
                    }
                    if (val < 0) {
                        r0 = 1;
                    } else {
                        switch (artType) {
                            case 1: case 2: case 3:
                                r0 = bmByteBit01(tgtParam->field_73, 0x80); break;
                            case 4: r0 = bmByteBit01(tgtParam->field_73, 0x01); break;
                            case 5: r0 = bmByteBit01(tgtParam->field_73, 0x02); break;
                            case 6: r0 = bmByteBit01(tgtParam->field_73, 0x04); break;
                            case 7: r0 = bmByteBit01(tgtParam->field_73, 0x08); break;
                            case 8: r0 = bmByteBit01(tgtParam->field_73, 0x10); break;
                            case 9: r0 = bmByteBit01(tgtParam->field_73, 0x20); break;
                            default: r0 = 0; break;
                        }
                    }
                }
                if (r0 != 0) {
                    move->field_58 += 0.25f;
                }


                // ALT only (0x800DF93C): sub->field_40 == 6 -> attacker 0x4E * 0.01
                if (sub->field_40 == 6) {
                    if (isBattleStatusActive(atkB + 8, 0x4E)) {
                        cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(atkB + 8, 0x4E);
                        move->field_58 +=
                            0.01f * (f32)(s32)(u32)entry->unk10;
                    }
                }

            }
        alt_post:;
            // ================================================================
            // ALT post-dispatch (0x800DF990): pos-diff, 0xCE, crit.
            // ================================================================
            if (*(u32*)(tgtB + 0x3374) & 0x20000) {
                BattleVec* vT = (BattleVec*)((cf::CfObject*)(tgtB + 0x3E9C))->CfObject_getPosVector();
                BattleVec* vA = (BattleVec*)((cf::CfObject*)(atkB + 0x3E9C))->CfObject_getPosVector();
                f32 d = vA->y - vT->y;
                if (d < lbl_eu_80666DDC) d = -d;
                if (d <= 10.0f) move->field_58 += 2.0f;
            }

            if (isBattleStatusActive(tgtB + 8, 0xCE)) {
                if (move->field_78 & 0x800) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findTopBattleStatusEntry(tgtB + 8, 0xCE);
                    if ((s32)(u32)entry->unk10 >= artsSubGetMax(sub)) {
                        move->field_54 = 1.0f;
                        move->field_74 |= 0x80002000;
                    }
                }
            }

            // ALT: field_74 bit3 (0x8) skips crit entirely (no field_54 clamp).
            if (!(move->field_74 & 0x8)) {
                // CRIT BLOCK ALT -- base atkParam->field_3A (not field_38)
                s32 critRate = atkParam->field_3A;
                if (bmFlagBit(*(u32*)(atkB + 0x3F00), 0x2)) {
                    s32 idx = ((cf::CActorParam*)(attacker))->CActorParam_getStatusCount();
                    s16* critTbl = (s16*)(pool + 0x208);
                    critRate += critTbl[idx];
                }
                bool critBoost = false;
                if ((sub->field_78 & 0x4) &&
                    (*(u32*)(tgtB + 0x3374) & 0x100)) {
                    critBoost = true;
                    critRate += 0x19;
                }
                if (sub->field_78 & 0x10) {
                    f32 f1 = ((cf::CActorParam*)(attacker))->CActorParam_getArtsGauge();
                    critRate = (s32)((f32)(s32)critRate + f1 * 0.5f);
                }
                if (sub->field_78 & 0x2) {
                    critRate = 100;
                }
                // ALT crit: no 0x59/0x5A status checks (MAIN-only).
                if (isBattleStatusActive(tgtB + 8, 0xC2)) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(tgtB + 8, 0xC2);
                    critRate -= (s32)(u32)entry->unk10;
                }
                // ALT gate: rem < r20 OR (rem >= r20 && has 0x10) -> crit
                s32 rem = (s32)move->field_98 % 100;
                if (rem < critRate || isBattleStatusActive(tgtB + 8, 0x10)) {
                    f32 f26 = 0.25f;
                    if (isBattleStatusActive(tgtB + 8, 0xF) ||
                        isBattleStatusActive(tgtB + 8, 0x10)) {
                        f26 += 0.25f;
                    }
                    if (critBoost) f26 += 0.25f;
                    if ((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable() != 0) {
                        u32 sv;
                        if (IdTable_GetSum((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable(), 0x51, &sv)) {
                            f26 += (f32)(s32)sv / 100.0f;
                        }
                    }
                    move->field_58 += f26;
                    move->field_74 |= 0x80000100;
                }
            }


        dispatch_alt_done:;
        }
    }

post_dispatch:;
    // pos/CE/crit duplicated at end of MAIN (main_post) and ALT (alt_post);
    // retail emits both copies rather than a shared fallthrough.

    // ================================================================
    // 0x800DFC5C: 0xF/0x83 and 0x10/0x84 data-map pairs
    // ================================================================
    if (isBattleStatusActive(tgtB + 8, 0xF)) {
        if (isBattleStatusActive(atkB + 8, 0x83)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(atkB + 8, 0x83);
            move->field_58 +=
                (f32)(s32)(u32)entry->unk10 / 100.0f;
        }
    }
    if (isBattleStatusActive(tgtB + 8, 0x10)) {
        if (isBattleStatusActive(atkB + 8, 0x84)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(atkB + 8, 0x84);
            move->field_58 +=
                (f32)(s32)(u32)entry->unk10 / 100.0f;
        }
    }

    // ================================================================
    // 0x800DFD1C: field_74 bit25 (0x2000000) -- party-list membership bonus
    // ================================================================
    if (move->field_74 & 0x2000000) {
        void* f4 = *(void**)(atkB + 0x4);
        outVal = *(s32*)(((cf::CObjectState*)(f4))->getStateData());
        // Retail: lis r5,2048 -> 0x08000000; plain cmpi (no bool ok).
        if (testActorStatus(attacker, &outVal, 0x08000000) != 0) {
            // walk self->mActorList1 sentinel (self+8) looking for target
            void* sentinel = ((cf::CBattleManager*)self)->mActorList1.mStartNodePtr;
            void* cur = *(void**)sentinel;
            s32 found = 0;
            while (cur != sentinel) {
                if (*(void**)((u8*)cur + 0x8) == target) { found = 1; break; }
                cur = *(void**)cur;
            }
            if (found != 0) {
                move->field_58 += 1.0;                  // f64 1.0 -> fadd
                move->field_78 |= 0x40000002;
            }
        }
        if (isBattleStatusActive(atkB + 8, 0xB2)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(atkB + 8, 0xB2);
            move->field_58 +=
                (f32)(s32)(u32)entry->unk10 / 100.0f;
        }
    }

    // ================================================================
    // 0x800DFE08: member check 0x08000000 + attacker skill values
    // ================================================================
    {
        void* f4 = *(void**)(atkB + 0x4);
        outVal = *(s32*)(((cf::CObjectState*)(f4))->getStateData());
        if (testActorStatus(attacker, &outVal, 0x08000000) != 0) {
            if (isBattleStatusActive(atkB + 8, 0xB1)) {
                cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(atkB + 8, 0xB1);
                move->field_58 +=
                    (f32)(s32)(u32)entry->unk10 / 100.0f;
            }
            {
                void* st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
                if (st != 0) {
                    u32 sv;
                    st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
                    if (IdTable_GetSum(st, 0x52, &sv)) {
                        move->field_58 += (f32)(s32)sv / 100.0f;
                    }
                }
            }
            {
                // Separate null gate for 0x53 (retail 0x800DFEF8) -- not shared with 0x52.
                void* st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
                if (st != 0) {
                    u32 sv;
                    st = (void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable();
                    if (IdTable_GetSum(st, 0x53, &sv) &&
                        *(u32*)(atkB + 0x1530) != 0 &&
                        (s32)((cf::CActorParam*)(target))->CActorParam_getActorType() == 9) {
                        move->field_58 += (f32)(s32)sv / 100.0f;
                    }
                }
            }
            if (bmFlagBit(*(u32*)(atkB + 0x3F00), 0x2) &&
                (*(u16*)(atkB + 0x3F28)) == 5 &&
                (move->field_78 & 0x400)) {
                f32 f1 = ((cf::CActorParam*)(attacker))->CActorParam_getArtsGauge();
                move->field_58 += f1 / 150.0f;
            }
            if (isBattleStatusActive(tgtB + 8, 0x9)) {
                move->field_58 += 0.5f;
                if ((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable() != 0) {
                    u32 sv;
                    if (IdTable_GetSum((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable(), 0x58, &sv)) {
                        move->field_58 += (f32)(s32)sv / 100.0f;
                    }
                }
                move->field_74 |= 0x80000100;
            }
        }
    }

    // ================================================================
    // 0x800E0090: sub->field_48 == 0x108 -- count active skill entries
    // ================================================================
    if (sub->field_48 == 0x108) {
        s32 count = 0;
        for (f32 i = lbl_eu_80666DDC; i < 32.0f; i += 1.0f) {
            void* e = ((cf::CBattleState*)(tgtB + 8))->CBattleState_getEventEntry((int)((u32)(s32)i));
            if (*(u16*)((u8*)e + 0xC) != 0) count++;
        }
        move->field_58 += (f32)(s32)count;
    }

    // ================================================================
    // 0x800E010C: 0x10C / 0xFD penalty
    // ================================================================
    if (isBattleStatusActive(atkB + 8, 0x10C) ||
        isBattleStatusActive(atkB + 8, 0xFD)) {
        move->field_58 -= 0.25f;
    }

    // ================================================================
    // 0x800E0144: target vf0x290 damage-resist skills
    // ================================================================
    // Retail calls getStatusTable once for the null check, then again per
    // query (CSE would drop a bcctrl -- force the reload like siblings).
    {
        void* st = (void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable();
        if (st != 0) {
            u32 sv;
            if (move->field_78 & 0x400) {
                st = (void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable();
                if (IdTable_GetSum(st, 0x4E, &sv) && sub->field_40 == 6) {
                    move->field_58 += (f32)(s32)sv / 100.0f;
                }
                st = (void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable();
                if (IdTable_GetSum(st, 0x4E, &sv) && sub->field_40 == 8) {
                    move->field_58 += (f32)(s32)sv / 100.0f;
                }
            }
            if (sub->type_3C == 1 || sub->type_3C == 5) {
                st = (void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable();
                if (IdTable_GetSum(st, 0x4F, &sv)) {
                    move->field_58 *= 1.0f - (f32)(s32)sv / 100.0f;
                }
            } else {
                st = (void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable();
                if (IdTable_GetSum(st, 0x50, &sv)) {
                    move->field_58 *= 1.0f - (f32)(s32)sv / 100.0f;
                }
                st = (void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable();
                if (IdTable_GetSum(st, 0x54, &sv) && sub->field_5E != 0) {
                    move->field_58 *= 1.0f - (f32)(s32)sv / 100.0f;
                }
            }
        }
    }

    // ================================================================
    // 0x800E0364: chain membership -> self->field_20A8 ; event dispatch
    // ================================================================
    if (CChain_HasMemberEntry((cf::CChain*)((u8*)&((cf::CBattleManager*)self)->mChain), (CChainBattleObjTail*)attacker)) {
        move->field_58 += *(f32*)((u8*)self + 0x20A8);
    }
    emitBattleEvents(self, attacker, target, move);        // 0x800E0388

    // ================================================================
    // 0x800E039C: field_5C clamp
    // ================================================================
    if (isBattleStatusActive(tgtB + 8, 0x1)) {
        move->field_5C = lbl_eu_80666DDC;
    } else if (!(move->field_74 & 0x8000) && move->field_5C < 1.0f) {
        move->field_5C = 1.0f;
    }

    // ================================================================
    // 0x800E03DC: field_68 rounding block (f64 0.5/-0.5 rounding)
    // ================================================================
    if (!(move->field_74 & 0x80)) {
        if (move->field_74 & 0x8000) {
            f32 sum = move->field_5C + move->field_60;
            s32 v = 0;
            if (sum >= lbl_eu_80666DDC) {
                v = (s32)((f64)sum + (sum > lbl_eu_80666DDC ? 0.5 : -0.5));
            }
            if (v > 0) {
                f32 sum2 = move->field_5C + move->field_60;
                s32 v2 = 0;
                if (sum2 >= lbl_eu_80666DDC) {
                    v2 = (s32)((f64)sum2 + (sum2 > lbl_eu_80666DDC ? 0.5 : -0.5));
                }
                move->field_68 -= (f32)(s32)v2;
            }
        } else {
            f32 sum2 = move->field_5C + move->field_60;
            s32 v2 = 0;
            if (sum2 >= lbl_eu_80666DDC) {
                v2 = (s32)((f64)sum2 + (sum2 > lbl_eu_80666DDC ? 0.5 : -0.5));
            }
            move->field_68 -= (f32)(s32)v2;
        }
    }

    // ================================================================
    // 0x800E04B8: status blocks 0x92 / 0xFC / 0x100 (dot/counter)
    // ================================================================
    if (!(move->field_78 & 0x1000) &&
        ((cf::CActorParam*)(target))->CActorParam_isBattleLocked() == 0 &&
        move->field_5C > lbl_eu_80666DDC) {
        // ---- 0x92 counter block (0x800E04F0) ----
        if (isBattleStatusActive(tgtB + 8, 0x92)) {
            s32 extra = 0;
            if (isBattleStatusActive(tgtB + 8, 0xA1)) {
                cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry(tgtB + 8, 0xA1);
                extra = (s32)(u32)e->unk10;
            }
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(tgtB + 8, 0x92);
            if (entry != 0 && move->field_68 <= lbl_eu_80666DDC) {
                s32 procRate = (s32)(u32)entry->unk10;
                if (entry->unk08 == 0x2000) procRate += extra;
                if ((s32)move->field_A8 % 100 < procRate) {
                    move->field_74 |= 0xA0000000;
                    move->field_68 = 1.0f;
                }
            }
        }
        // ---- 0xFC block (0x800E05B0) ----
        if (!(move->field_78 & 0x2000) &&
            isBattleStatusActive(tgtB + 8, 0xFC)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(tgtB + 8, 0xFC);
            if (entry != 0 && move->field_68 <= lbl_eu_80666DDC) {
                move->field_78 |= 0x40002000;
                f32 f1 = ((cf::CActorParam*)(target))->CActorParam_getDamageScale();
                move->field_68 = 0.01f * (f32)(s32)(u32)entry->unk10 * f1;
            }
        }
        // ---- 0x100 block (0x800E064C) ----
        if (!(move->field_78 & 0x2000) &&
            isBattleStatusActive(tgtB + 8, 0x100)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(tgtB + 8, 0x100);
            if (entry != 0 && move->field_68 <= lbl_eu_80666DDC) {
                move->field_78 |= 0x40002000;
                f32 f1 = ((cf::CActorParam*)(target))->CActorParam_getDamageScale();
                move->field_68 = 0.01f * (f32)(s32)(u32)entry->unk10 * f1;
            }
        }
    }

    // ================================================================
    // 0x800E06D8: status blocks 0x36 / 0x37 / 0x11E (dot ticks)
    // ================================================================
    if (!(move->field_78 & 0x1000) &&
        ((cf::CActorParam*)(target))->CActorParam_isBattleLocked() == 0) {
        // ---- 0x36 block (0x800E070C) ----
        if (!(move->field_78 & 0x4000) &&
            isBattleStatusActive(tgtB + 8, 0x36)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(tgtB + 8, 0x36);
            if (entry != 0 && move->field_68 > lbl_eu_80666DDC) {
                s32 v = (s32)(u32)entry->unk10;
                f32 f1 = calcBattleDamage(0, target, 0);
                v = (s32)((f32)(s32)v * f1);
                move->field_68 += (f32)(s32)v;
                if (entry->unk14 > 0) {
                    if (move->field_70 == 0) {
                        move->field_70 = entry->unk14;
                    }
                    move->field_70 -= 1;
                    if (move->field_70 <= 0) {
                        move->field_70 = 0;
                        move->field_78 |= 0x40004000;
                    }
                }
            }
        }
        // ---- 0x37 block (0x800E07E4) ----
        if (isBattleStatusActive(tgtB + 8, 0x37)) {
            s32 extra = 0;
            if (isBattleStatusActive(tgtB + 8, 0xA1)) {
                cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry(tgtB + 8, 0xA1);
                extra = (s32)(u32)e->unk10;
            }
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(tgtB + 8, 0x37);
            if (entry != 0 && move->field_68 > lbl_eu_80666DDC) {
                s32 procRate = entry->unk1A;
                if (entry->unk08 == 0x2000) procRate += extra;
                if ((s32)move->field_A8 % 100 < procRate) {
                    s32 v = (s32)(u32)entry->unk10;
                    f32 f1 = calcBattleDamage(0, target, 0);
                    v = (s32)((f32)(s32)v * f1);
                    move->field_68 += (f32)(s32)v;
                }
            }
        }
        // ---- 0x11E block (0x800E08E4) ----
        if (!(move->field_78 & 0x4000) &&
            isBattleStatusActive(tgtB + 8, 0x11E)) {
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry(tgtB + 8, 0x11E);
            if (entry != 0 && move->field_68 > lbl_eu_80666DDC) {
                f32 f1 = ((cf::CActorParam*)(target))->CActorParam_getDamageScale();
                s32 v = (s32)((f32)(s32)entry->unk14 * f1 / 100.0f);
                f1 = calcBattleDamage(0, target, 0);
                v = (s32)((f32)(s32)v * f1);
                move->field_68 += (f32)(s32)v;
                if ((s32)(u32)entry->unk10 > 0) {
                    if (move->field_70 == 0) {
                        move->field_70 = (s16)(u32)entry->unk10;
                    }
                    move->field_70 -= 1;
                    if (move->field_70 <= 0) {
                        move->field_70 = 0;
                        move->field_78 |= 0x40004000;
                    }
                }
            }
        }
    }

    // ================================================================
    // 0x800E09FC: field_68 <= 0 -> "dot expired" flag
    // ================================================================
    if (move->field_68 <= lbl_eu_80666DDC) {
        move->field_78 |= 0x40001000;
    }

    // ================================================================
    // 0x800E0A20: 0x106 block -- final rounded damage to field_6C
    // ================================================================
    if (isBattleStatusActive(tgtB + 8, 0x106)) {
        findBattleStatusEntry(tgtB + 8, 0x106);
        if (!(move->field_78 & 0x800)) {
            // Reload sub from move (retail lwz 0x50(r31) before each virt).
            void* sub106 = move->field_50;
            if (sub106 == 0) return;                    // 0x800E0A50

            // rem = field_B0 % 100 first -- must live across virt calls (r18).
            s32 rem = (s32)move->field_B0 % 100;

            // Retail: copy 3 words from pool+0x280, virt, overwrite [0],
            // virt, overwrite [1], copy rates from pool+0x28C. Struct copy
            // keeps the initial lwz/stw (scalar assigns get DCE'd).
            struct Triple { u32 a, b, c; };
            Triple steps = *(Triple*)(pool + 0x280);
            s32 vmax = artsSubGetMax(sub106);
            steps.a = (u32)(vmax / 2 + 3);
            vmax = artsSubGetMax(move->field_50);
            steps.b = (u32)(vmax + 0xA);

            Triple rates = *(Triple*)(pool + 0x28C);

            f32 mult = 1.0f;
            {
                u32* pTh = &steps.a;
                s32 idx = 0;
                s32 count = 3;
                while (count != 0) {
                    rem -= (s32)*pTh;
                    if (rem < 0) {
                        mult = ((f32*)&rates.a)[idx];
                        break;
                    }
                    pTh++;
                    idx++;
                    count--;
                }
            }
            if ((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable() != 0) {
                u32 sv;
                if (IdTable_GetSum((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable(), 0x5B, &sv)) {
                    mult *= 1.0f + (f32)(s32)sv / 100.0f;
                }
            }
            // rounded damage (0x800E0BA0..0x800E0CA0)
            f32 sum = move->field_5C + move->field_60;
            s32 v = 0;
            if (sum >= lbl_eu_80666DDC && !(move->field_74 & 0x80)) {
                v = (s32)((f64)sum + (sum > lbl_eu_80666DDC ? 0.5 : -0.5));
            }
            f32 f1 = (f32)(s32)v;
            s32 roundedValue = (s32)((f64)f1 + (f1 > lbl_eu_80666DDC ? 0.5 : -0.5));
            f1 = (f32)(s32)roundedValue * mult;
            s32 r0 = (s32)((f64)f1 + (f1 > lbl_eu_80666DDC ? 0.5 : -0.5));
            move->field_6C -= (f32)(s32)r0;
        }
    }

    // ================================================================
    // 0x800E0CA4: guards before the art loop
    // ================================================================
    if (isBattleStatusActive(tgtB + 8, 0x10)) {
        if (!(*(u32*)(tgtB + 0x3374) & 0x40000)) return;  // bit13
        void* f4 = *(void**)(tgtB + 0x4);
        outVal = *(s32*)(((cf::CObjectState*)(f4))->getStateData());
        if (testActorStatus(target, &outVal, 0x17) == 0) return;
    }
    if (isBattleStatusActive(tgtB + 8, 0x117)) return;
    if (move->field_74 & 0x80) return;

    // ================================================================
    // 0x800E0D4C: art loop -- slots 0x40..0x67 (64 entries)
    // ================================================================
    // Nested block: buffers share stack with earlier 0x106 Triples (retail),
    // but pointers stay live across art-loop calls -> _savegpr_18.
    {
    f32 magArg[3];
    f32 delta[3];
    f32* pMag = magArg;
    f32* pDel = delta;
    for (u32 idx = 0x40; idx < 0x68; idx++) {
        cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)((cf::CBattleState*)(tgtB + 8))->CBattleState_getEntryByIndex((int)(idx));
        u16 type = *(u16*)((u8*)entry + 0xC);
        s32 skillId = 0;
        switch (type) {                                 // jt 0x8052B1BC
            case 0x6E: case 0x78: skillId = (s32)classifyBattleCommand(0x6E); break;
            case 0x6F: case 0x79: skillId = (s32)classifyBattleCommand(0x6F); break;
            case 0x70: case 0x7A: skillId = (s32)classifyBattleCommand(0x70); break;
            case 0x71: case 0x7B: skillId = (s32)classifyBattleCommand(0x71); break;
            default: skillId = 0; break;
        }
        if (skillId == 0) continue;                     // 0x800E0DCC
        if (entry->unk14 == 0) continue;  // 0x800E0DD4

        // distance check via VEC3Sub + stack copy + PSVECMag (0x800E0DE0)
        {
            nw4r::math::VEC3* vT = (nw4r::math::VEC3*)((cf::CfObject*)(tgtB + 0x3E9C))->CfObject_getPosVector();
            nw4r::math::VEC3* vA = (nw4r::math::VEC3*)((cf::CfObject*)(atkB + 0x3E9C))->CfObject_getPosVector();
            nw4r::math::VEC3Sub((nw4r::math::VEC3*)pDel, vA, vT);
            pMag[0] = pDel[0];
            pMag[1] = pDel[1];
            pMag[2] = pDel[2];
            if (PSVECMag((const struct Vec*)pMag) > (f32)(s32)entry->unk14) {
                skillId = 0;
            }
        }
        if (skillId == 0) continue;

        // attacker param + entry value (0x800E0E74)
        BattleParamData* atkP =
            (BattleParamData*)(BattleParamData*)((cf::CActorParam*)(attacker))->CActorParam_getBattleParams();
        s32 val = (s32)entry->unk10;
        f32 f27 = 1.0f;
        if ((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable() != 0) {
            if (findBattleStatusEntry(tgtB + 8, 0x6F) != 0 ||
                findBattleStatusEntry(tgtB + 8, 0x79) != 0) {
                u32 sv;
                if (IdTable_GetSum((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable(), 0x5B, &sv)) {
                    val = (s32)((f32)(s32)val *
                                (1.0f + (f32)(s32)sv / 100.0f));
                }
            }
        }
        // 0x800E0F3C: 0x8D / vf0x290 0x57 reductions
        if (isBattleStatusActive(atkB + 8, 0x8D)) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry(atkB + 8, 0x8D);
            f27 = 1.0f - 0.01f * (f32)(s32)(u32)e->unk10;
        }
        if ((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable() != 0) {
            u32 sv;
            if (IdTable_GetSum((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable(), 0x57, &sv)) {
                f27 = 1.0f - 0.01f * (f32)(s32)sv;
            }
        }
        val = (s32)((f32)(s32)val * f27);               // 0x800E0FD0
        if (val < 1) val = 0;
        if (isBattleStatusActive(atkB + 8, 0x1)) val = 0;

        // skill dispatch (0x800E1014): same (id-1) CF as art, but >>n&1 (no neg/or)
        s32 r0 = 0;
        if ((u32)(skillId - 1) <= 2) {
            if (skillId != 0) {
                switch (skillId) {
                    case 1: case 2: case 3:
                        r0 = (atkP->field_72 >> 7) & 1; break;
                    case 4: r0 = atkP->field_72 & 1; break;
                    case 5: r0 = (atkP->field_72 >> 1) & 1; break;
                    case 6: r0 = (atkP->field_72 >> 2) & 1; break;
                    case 7: r0 = (atkP->field_72 >> 3) & 1; break;
                    case 8: r0 = (atkP->field_72 >> 4) & 1; break;
                    case 9: r0 = (atkP->field_72 >> 5) & 1; break;
                    default: r0 = 0; break;
                }
            }
        } else if (skillId != 0) {
            s16 stat = 0;
            switch (skillId) {
                case 4: stat = atkP->field_64; break;
                case 5: stat = atkP->field_66; break;
                case 6: stat = atkP->field_68; break;
                case 7: stat = atkP->field_6A; break;
                case 8: stat = atkP->field_6C; break;
                case 9: stat = atkP->field_6E; break;
            }
            if (stat >= 200) {
                r0 = 1;
            } else {
                switch (skillId) {
                    case 1: case 2: case 3:
                        r0 = (atkP->field_72 >> 7) & 1; break;
                    case 4: r0 = atkP->field_72 & 1; break;
                    case 5: r0 = (atkP->field_72 >> 1) & 1; break;
                    case 6: r0 = (atkP->field_72 >> 2) & 1; break;
                    case 7: r0 = (atkP->field_72 >> 3) & 1; break;
                    case 8: r0 = (atkP->field_72 >> 4) & 1; break;
                    case 9: r0 = (atkP->field_72 >> 5) & 1; break;
                    default: r0 = 0; break;
                }
            }
        }
        if (r0 != 0) {
            val = -val;                                 // 0x800E11A0 negate
        } else {
            if ((u32)(skillId - 1) <= 2) {
                if (skillId != 0) {
                    switch (skillId) {
                        case 1: case 2: case 3:
                            r0 = (atkP->field_70 >> 7) & 1; break;
                        case 4: r0 = atkP->field_70 & 1; break;
                        case 5: r0 = (atkP->field_70 >> 1) & 1; break;
                        case 6: r0 = (atkP->field_70 >> 2) & 1; break;
                        case 7: r0 = (atkP->field_70 >> 3) & 1; break;
                        case 8: r0 = (atkP->field_70 >> 4) & 1; break;
                        case 9: r0 = (atkP->field_70 >> 5) & 1; break;
                        default: r0 = 0; break;
                    }
                }
            } else if (skillId != 0) {
                s16 stat = 0;
                switch (skillId) {
                    case 4: stat = atkP->field_64; break;
                    case 5: stat = atkP->field_66; break;
                    case 6: stat = atkP->field_68; break;
                    case 7: stat = atkP->field_6A; break;
                    case 8: stat = atkP->field_6C; break;
                    case 9: stat = atkP->field_6E; break;
                }
                if (stat >= 100) {
                    r0 = 1;
                } else {
                    switch (skillId) {
                        case 1: case 2: case 3:
                            r0 = (atkP->field_70 >> 7) & 1; break;
                        case 4: r0 = atkP->field_70 & 1; break;
                        case 5: r0 = (atkP->field_70 >> 1) & 1; break;
                        case 6: r0 = (atkP->field_70 >> 2) & 1; break;
                        case 7: r0 = (atkP->field_70 >> 3) & 1; break;
                        case 8: r0 = (atkP->field_70 >> 4) & 1; break;
                        case 9: r0 = (atkP->field_70 >> 5) & 1; break;
                        default: r0 = 0; break;
                    }
                }
            }
            if (r0 != 0) {
                val = (s32)((f32)(s32)val * 0.5f);      // 0x800E1344 halve
            }
        }


        // 0x800E1358: apply
        if (val != 0) {
            move->field_6C -= (f32)(s32)val;
        }
    }
    } // art-loop buffer block
    // 0x800E1388 epilogue: restore + blr (void return)
}

extern f64 lbl_eu_80666E08;   // 0x4330000000000000 sibling magic
extern f32 lbl_eu_80666E24;   // 0.15f


// Status-list entry overlay (findBattleStatusEntry result): flags word +0x08,
// payload word +0x10, tick s16 +0x14, aux s16 +0x1A.
// Move-state block overlay carried in arg4.
// Battle-object overlay (attacker/target views).
// Arts sub-object overlay (the pointer at move->0x50).
// Actor vtable mirror exposing exactly the slots retail dispatches through
// (0xE0/0x128/0x12C/0x130/0x224/0x290/0x2A8/0x2BC/0x5C0); the placeholder
// slots keep MWCC's dispatch routed through r12 like retail.
// Vtable of the object at *(subObj->field_84): value slot 0xC.
// Vtable of the embedded move sub-object at actor+0x3E9C: id slot 0x4C.
// emitBattleEvents (retail 0x800E13D0, 0x125C bytes): battle-event emission and
// move-state update shared by the move-type dispatchers. Resolves the event
// source (arg1 vs arg2 via the 0xF7 status / 0x5C0 self-check), runs the
// arts-type jump table, emits the event via emitBattleMoveEvents, then applies the
// counter / dot / proc-rate damage math on the move block.
// Superseded draft kept as dead code (never called; dropped by the linker).
static void func_staleMove(void* self, void* arg1, void* arg2, BattleTargetData* move) {
    if (arg2 == nullptr) return;

    // 0xF7 status + 0x5C0 self-target check: when no confirmation is pending
    // the event source falls back to arg1.
    if (!(move->field_74 & 0x08)) {
        if (isBattleStatusActive((u8*)arg2 + 8, 0xF7)) {
            if (((cf::CfObjectActor*)(arg2))->CfObjectActor_sharesMoveFlags((cf::CfObjectActor*)arg1) == 0) {
                findBattleStatusEntry((u8*)arg2 + 8, 0xF7);
                u32 f78 = move->field_78;
                if ((f78 & 0x200) || ((f78 & 0x400) && !(f78 & 0x800))) {
                    move->field_74 |= 0x80000080;
                }
            }
        }
    }

    // Event-source swap happens inside the above block (retail keeps arg2
    // until the source-swap condition, then reuses r28).
    u32 f74 = move->field_74;
    if (!(f74 & 0x08)) {
        u32 f78b = move->field_78;
        if ((f78b & 0x200) || ((f78b & 0x400) && !(f78b & 0x800))) {
            move->field_74 |= 0x80000080;
        }
    }

    move->field_60 = lbl_eu_80666DDC;
    move->field_64 = lbl_eu_80666DDC;
    cf::CActorParam_UnkStruct2* subObj = (cf::CActorParam_UnkStruct2*)move->artsData;      // r31

    ((cf::CActorParam*)(arg1))->CActorParam_getBattleParams();
    ((cf::CActorParam*)(arg2))->CActorParam_getBattleParams();

    if (move->field_74 & 0x08) {
        if (!(subObj->unk78 & 0x1000)) goto done1744_1B8C;
    }

    s32 shouldEmit = 0;
    if (subObj->unk78 & 0x2000) {
        u8 slot = subObj->field_44;
        if (slot != (u8)(((cf::CActorParam*)(arg1))->CActorParam_getStatsSlot() + 1)) {
            goto done1744_1B8C;
        }
    }

    // Arts-type jump table (byte +0x66 of the sub-object).
    u8 type = subObj->field_66;
    if (type <= 0x19) {
        switch (type) {
        case 0x00: {
            if (((cf::CfObjectActor*)arg1)->unk3374 & 0x8000) {
                s32 e0 = (s32)((cf::CActorParam*)(arg2))->CActorParam_getActorType();
                if (e0 == 1 || e0 == 2) shouldEmit = 1;
            } else {
                if ((s32)((cf::CActorParam*)(arg2))->CActorParam_getActorType() == 1) shouldEmit = 1;
            }
            break;
        }
        case 0x01:
            if ((s32)((cf::CActorParam*)(arg2))->CActorParam_getActorType() == 3) shouldEmit = 1;
            break;
        case 0x02:
            if (move->field_74 & 0x1000000) shouldEmit = 1;
            break;
        case 0x03:
            if (move->field_74 & 0x4000000) shouldEmit = 1;
            break;
        case 0x04:
            if (move->field_74 & 0x2000000) shouldEmit = 1;
            break;
        case 0x05:
            if (isBattleStatusActive((u8*)arg1 + 8, 0xC1)) shouldEmit = 1;
            break;
        case 0x06:
            if (isBattleStatusActive((u8*)arg2 + 8, 0x10)) shouldEmit = 1;
            break;
        case 0x07:
            if (isBattleStatusActive((u8*)arg2 + 8, 0x0F)) shouldEmit = 1;
            break;
        case 0x08: {
            cf::CfObjectMove* m = static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)arg2);
            void* r = (void*)(uintptr_t)((cf::CObjectParam*)m)->CObjectParam_getSelfObjectId();
            if (r == (void*)((cf::CfObjectActor*)arg1)->field_0x74) shouldEmit = 1;
            break;
        }
        case 0x09:
            if (move->field_5C >= ((cf::CActorParam*)(arg2))->CActorParam_getHp()) shouldEmit = 1;
            break;
        case 0x0A:
            if (subObj->field_67 != 0) shouldEmit = 1;
            break;
        case 0x0B:
            if (((u16)getReloadParam0()) == (u16)(type - 0x0A)) shouldEmit = 1;
            break;
        case 0x0C: {
            u32 m74 = move->field_74;
            if (m74 & 0x4000000) {
                s32 e0 = (s32)((cf::CActorParam*)(arg2))->CActorParam_getActorType();
                if (e0 == 1 || e0 == 2) shouldEmit = 1;
            }
            break;
        }
        case 0x0D:
            if (((cf::CActorParam*)(arg2))->CActorParam_getHitRate() < 0.15f) shouldEmit = 1;
            break;
        default:
            break;
        }
    }

done1744_1B8C:;
    if (subObj->unk78 & 0x1000) {
        if (findFreeBattleSlot((D7D24_Obj*)move) == 0) shouldEmit = 0;
    }
    if (!(move->field_74 & 0x10)) {
        move->field_5C = move->field_54 * move->field_58;
    }

    if (shouldEmit == 0) goto postEmit;

    // ---- event emission (dispatch on subObj->0x67) ----
    BattleEvent ev;
    std::memset(&ev, 0, sizeof(ev));
    u8 emitType = subObj->field_67;
    switch (emitType) {
    case 0x03: {
        if (move->field_78 & 0x10) break;
        cf::CfActorF64Conv conv;
        conv.w[0] = 0x43300000;
        s16 hp = subObj->field_68;
        conv.w[1] = (u32)(hp - 1) ^ 0x80000000;
        f32 add = (f32)(conv.d - lbl_eu_80666DE0);
        move->field_58 = move->field_58 + add;
        move->field_5C = move->field_54 * move->field_58;
        if ((void*)((cf::CActorParam*)(arg1))->CActorParam_getStatusTable() != 0) {
            void* p = (void*)((cf::CActorParam*)(arg1))->CActorParam_getStatusTable();
            if (Counter_TestBit(p, 0x9C) != 0) {
                if (subObj->field_66 == 3) {
                    move->field_58 = move->field_58 + 1.0f;
                }
            }
        }
        break;
    }
    case 0x07:
        ev.field_00 = ((cf::CfObjectActor*)arg1)->field_0x74;
        ev.field_04 = (u32)(uintptr_t)subObj;
        ev.eventType = 0x10;
        ev.field_10 = 0;
        {
            cf::CfActorF64Conv conv;
            conv.w[0] = 0x43300000;
            conv.w[1] = (u32)(subObj->field_68) ^ 0x80000000;
            ev.field_20 = (f32)(conv.d - lbl_eu_80666DE0);
        }
        ev.field_24 = lbl_eu_80666DDC;
        if (subObj->unk78 & 0x1000) {
            emitBattleMoveEvents(self, arg1, arg1, &ev, move);
        } else {
            emitBattleMoveEvents(self, arg1, arg2, &ev, move);
        }
        break;
    case 0x08:
        ev.field_00 = ((cf::CfObjectActor*)arg1)->field_0x74;
        ev.field_04 = (u32)(uintptr_t)subObj;
        ev.eventType = 0x0F;
        ev.field_10 = 0;
        {
            cf::CfActorF64Conv conv;
            conv.w[0] = 0x43300000;
            conv.w[1] = (u32)(subObj->field_68) ^ 0x80000000;
            ev.field_20 = (f32)(conv.d - lbl_eu_80666DE0);
        }
        ev.field_24 = lbl_eu_80666DDC;
        if (subObj->unk78 & 0x1000) {
            emitBattleMoveEvents(self, arg1, arg1, &ev, move);
        } else {
            emitBattleMoveEvents(self, arg1, arg2, &ev, move);
        }
        break;
    case 0x0B: {
        ev.field_00 = ((cf::CfObjectActor*)arg1)->field_0x74;
        ev.field_04 = (u32)(uintptr_t)subObj;
        ev.eventType = 0x10;
        ev.field_10 = 3;
        ev.field_20 = 3.0f;
        ev.field_24 = lbl_eu_80666DDC;
        {
            CfEnumListHolder holder;
            startEnumObjects(holder.get(), 0x8000, 0);
            for (u32 i = 0; i < holder.get()->mPtrCount; i++) {
                void* o = MapFx_ResolveOwner(getObjectAt(holder.get(), i));
                if (o != arg2) emitBattleMoveEvents(self, arg1, o, &ev, move);
            }
        }
        break;
    }
    case 0x0D:
        ev.field_00 = ((cf::CfObjectActor*)arg1)->field_0x74;
        ev.field_04 = (u32)(uintptr_t)subObj;
        ev.eventType = 0x10;
        ev.field_10 = 3;
        ev.field_20 = 3.0f;
        ev.field_24 = lbl_eu_80666DDC;
        emitBattleMoveEvents(self, arg1, arg2, &ev, move);
        break;
    case 0x06:
        ev.field_00 = ((cf::CfObjectActor*)arg1)->field_0x74;
        ev.field_04 = (u32)(uintptr_t)subObj;
        ev.eventType = 0xDC;
        ev.field_10 = subObj->field_68;
        if (subObj->unk78 & 0x1000) {
            emitBattleMoveEvents(self, arg1, arg1, &ev, move);
        } else {
            emitBattleMoveEvents(self, arg1, arg2, &ev, move);
        }
        break;
    case 0x01: {
        ev.field_00 = ((cf::CfObjectActor*)arg1)->field_0x74;
        ev.field_04 = (u32)(uintptr_t)subObj;
        ev.eventType = subObj->field_68;
        ev.field_10 = subObj->field_6A;
        s32 sv = artsSubGetMax(subObj) - 1;
        cf::CfActorF64Conv conv;
        conv.w[0] = 0x43300000;
        conv.w[1] = (u32)(u8)(subObj->field_6E) ^ 0x80000000;
        f32 f1 = (f32)(conv.d - lbl_eu_80666E08);
        conv.w[1] = (u32)sv ^ 0x80000000;
        f32 f2 = (f32)(conv.d - lbl_eu_80666DE0);
        ev.field_20 = subObj->field_50 + f1 / 10.0f * f2;
        ev.field_24 = subObj->field_54;
        if (subObj->unk78 & 0x1000) {
            emitBattleMoveEvents(self, arg1, arg1, &ev, move);
        } else {
            emitBattleMoveEvents(self, arg1, arg2, &ev, move);
        }
        break;
    }
    default:
        break;
    }

postEmit:
    // Recompute the multiplier after emission.
    if (!(move->field_74 & 0x10)) {
        move->field_5C = move->field_54 * move->field_58;
    }

    // Second sub-object event block (+0x48 id).
    if (!(move->field_74 & 0x08) || (subObj->unk78 & 0x400)) {
        if (!(subObj->unk78 & 0x2000) ||
            (subObj->field_44 ==
             (u8)(((cf::CActorParam*)(arg1))->CActorParam_getStatsSlot() + 1))) {
            if (subObj->field_48 != 0) {
                BattleEvent ev2;
                std::memset(&ev2, 0, sizeof(ev2));
                ev2.field_00 = ((cf::CfObjectActor*)arg1)->field_0x74;
                ev2.field_04 = (u32)(uintptr_t)subObj;
                ev2.eventType = subObj->field_48;
                s32 sv2 = artsSubGetMax(subObj) - 1;
                ev2.field_10 = subObj->field_4A + (u32)(subObj->field_6F) * (u32)sv2;
                ev2.field_14 = (u16)subObj->field_4C;
                cf::CfActorF64Conv conv;
                conv.w[0] = 0x43300000;
                conv.w[1] = (u32)(u8)(subObj->field_6E) ^ 0x80000000;
                f32 f1 = (f32)(conv.d - lbl_eu_80666E08);
                conv.w[1] = (u32)sv2 ^ 0x80000000;
                f32 f2 = (f32)(conv.d - lbl_eu_80666DE0);
                ev2.field_20 = subObj->field_50 + f1 / 10.0f * f2;
                ev2.field_24 = subObj->field_54;
                if (subObj->unk78 & 0x400) {
                    emitBattleMoveEvents(self, arg1, arg1, &ev2, move);
                } else {
                    emitBattleMoveEvents(self, arg1, arg2, &ev2, move);
                }
            }
        }
    }

    // Counter damage gate: protect against hitting an already-active enemy.
    {
        s32 ival = 0;
        f32 sum = move->field_5C + move->field_60;
        if (sum >= lbl_eu_80666DDC && !(move->field_74 & 0x80)) {
            ival = (s32)((f64)sum + (sum <= lbl_eu_80666DDC ? -0.5 : 0.5));
        }
        if (ival > 0) {
            if (!(move->field_74 & 0x10)) {
                if (isBattleStatusActive((u8*)arg2 + 8, 0xF7)) {
                    if (((cf::CfObjectActor*)(arg2))->CfObjectActor_sharesMoveFlags((cf::CfObjectActor*)arg1) == 0) {
                        u32 m78 = move->field_78;
                        if ((m78 & 0x200) || ((m78 & 0x400) && !(m78 & 0x800))) {
                            move->field_74 |= 0x80000080;
                        }
                    }
                }
            }

            // X/Y gate: both the move and the sub-state must agree.
            u32 g74 = move->field_74;
            if (g74 & 0x8000) {
                if (move->field_78 & 0x8000) goto counterHit;
            } else {
                if (move->field_78 & 0x8000) {
                    if (g74) goto counterHit;
                } else {
                    goto counterHit;
                }
            }
            // (fallthrough = blocked)
            if (g74 & 0x8000) {
                if (!(move->field_78 & 0x8000)) goto counterHit;
            } else {
                if (move->field_78 & 0x8000) goto counterHit;
                goto blocked;
            }
        blocked:
            goto afterCounter;
        counterHit:
            // 0x33 status: per-hit counter bleed from the move block.
            if (!isBattleStatusActive((u8*)arg2 + 8, 0x33)) goto afterCounter;
            cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg2 + 8, 0x33);
            if (entry == nullptr) goto afterCounter;

            if (!(move->field_78 & 0x4000000)) {
                s32 steps = entry->unk14;
                if (steps <= 0) goto afterCounter;
                if (move->field_72 == -1) goto afterCounter;
                if (move->field_72 == 0) {
                    move->field_72 = (s16)steps;
                }
                s32 ival2 = 0;
                f32 sum2 = move->field_5C + move->field_60;
                if (sum2 >= lbl_eu_80666DDC && !(move->field_74 & 0x80)) {
                    ival2 = (s32)((f64)sum2 + (sum2 <= lbl_eu_80666DDC ? -0.5 : 0.5));
                }
                move->field_74 |= 0x80008000;
                cf::CfActorF64Conv conv;
                conv.w[0] = 0x43300000;
                conv.w[1] = (u32)(s16)(move->field_72) ^ 0x80000000;
                f32 f72 = (f32)(conv.d - lbl_eu_80666DE0);
                conv.w[1] = (u32)ival2 ^ 0x80000000;
                f32 fiv = (f32)(conv.d - lbl_eu_80666DE0);
                move->field_64 = fiv;
                f32 sum3 = move->field_5C - f72 + move->field_60;
                move->field_5C = move->field_5C - f72;
                s32 ival3 = 0;
                if (sum3 >= lbl_eu_80666DDC && !(move->field_74 & 0x80)) {
                    ival3 = (s32)((f64)sum3 + (sum3 <= lbl_eu_80666DDC ? -0.5 : 0.5));
                }
                if (ival3 > 0) {
                    move->field_72 = -1;
                    move->field_78 |= 0x40008000;
                } else {
                    f32 neg = -move->field_5C;
                    move->field_72 = (s16)(s32)neg;
                }
            } else {
                s32 steps = entry->unk14;
                if (steps <= 0) goto afterCounter;
                s32 ival = 0;
                f32 sum = move->field_5C + move->field_60;
                if (sum >= lbl_eu_80666DDC && !(move->field_74 & 0x80)) {
                    ival = (s32)((f64)sum + (sum <= lbl_eu_80666DDC ? -0.5 : 0.5));
                }
                move->field_74 |= 0x80008000;
                cf::CfActorF64Conv conv;
                conv.w[0] = 0x43300000;
                conv.w[1] = (u32)ival ^ 0x80000000;
                move->field_64 = (f32)(conv.d - lbl_eu_80666DE0);
                conv.w[1] = (u32)(s16)steps ^ 0x80000000;
                f32 fs = (f32)(conv.d - lbl_eu_80666DE0);
                move->field_5C = move->field_5C - fs;
            }
        }
    }

afterCounter:
    // Arts-type 3 dot/regeneration block.
    if (subObj->field_3C == 3) {
        s32 ival = 0;
        f32 sum = move->field_5C + move->field_60;
        if (sum >= lbl_eu_80666DDC && !(move->field_74 & 0x80)) {
            ival = (s32)((f64)sum + (sum <= lbl_eu_80666DDC ? -0.5 : 0.5));
        }
        if (ival > 0) {
            if (!(move->field_78 & 0x10)) {
                cf::CfActorF64Conv conv;
                conv.w[0] = 0x43300000;
                conv.w[1] = (u32)ival ^ 0x80000000;
                f32 fv = (f32)(conv.d - lbl_eu_80666DE0);
                move->field_68 = move->field_68 - fv;
            }
            if (!(move->field_78 & 0x1000)) {
                if (((cf::CActorParam*)(arg2))->CActorParam_isBattleLocked() == 0) {
                    // 0x92 proc-rate block.
                    if (isBattleStatusActive((u8*)arg2 + 8, 0x92)) {
                        s32 bonus = 0;
                        if (isBattleStatusActive((u8*)arg2 + 8, 0xA1)) {
                            cf::CBattleStateEntry* eA1 = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg2 + 8, 0xA1);
                            bonus = eA1->unk10;
                        }
                        cf::CBattleStateEntry* e92 = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg2 + 8, 0x92);
                        if (e92 != nullptr && move->field_68 <= lbl_eu_80666DDC) {
                            u32 need = (u32)e92->unk10;
                            if (e92->unk08 == 0x2000) need += bonus;
                            if (need > move->field_A8 % 100) {
                                move->field_74 |= 0xA0000000;
                                move->field_68 = lbl_eu_80666DD4;
                            }
                        }
                    }
                    if (!(move->field_78 & 0x2000)) {
                        // 0xFC / 0x100 fixed-rate bleeds.
                        if (isBattleStatusActive((u8*)arg2 + 8, 0xFC)) {
                            cf::CBattleStateEntry* eFC = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg2 + 8, 0xFC);
                            if (eFC != nullptr && move->field_68 <= lbl_eu_80666DDC) {
                                move->field_78 |= 0x40002000;
                                ((cf::CActorParam*)(arg2))->CActorParam_getDamageScale();
                                move->field_68 =
                                    move->field_68 * (f32)(eFC->unk10) * 0.01f;
                            }
                        }
                        if (isBattleStatusActive((u8*)arg2 + 8, 0x100)) {
                            cf::CBattleStateEntry* e100 = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg2 + 8, 0x100);
                            if (e100 != nullptr && move->field_68 <= lbl_eu_80666DDC) {
                                move->field_78 |= 0x40002000;
                                ((cf::CActorParam*)(arg2))->CActorParam_getDamageScale();
                                move->field_68 =
                                    move->field_68 * (f32)(e100->unk10) * 0.01f;
                            }
                        }
                        // 0x36 growth block.
                        if (isBattleStatusActive((u8*)arg2 + 8, 0x36)) {
                            cf::CBattleStateEntry* e36 = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg2 + 8, 0x36);
                            if (e36 != nullptr && move->field_68 > lbl_eu_80666DDC) {
                                s32 v36 = e36->unk10;
                                calcBattleDamage((void*)0, arg2, (void*)0);
                                cf::CfActorF64Conv conv;
                                conv.w[0] = 0x43300000;
                                conv.w[1] = (u32)v36 ^ 0x80000000;
                                f32 fv = (f32)(conv.d - lbl_eu_80666DE0);
                                f32 prod = fv * move->field_68;
                                f32 rnd = (f32)(s32)prod;
                                conv.w[1] = (u32)(s32)rnd ^ 0x80000000;
                                move->field_68 =
                                    move->field_68 + (f32)(conv.d - lbl_eu_80666DE0);
                            }
                        }
                    }
                    if (!(move->field_78 & 0x4000)) {
                        // 0x11E protection block.
                        if (isBattleStatusActive((u8*)arg2 + 8, 0x11E)) {
                            cf::CBattleStateEntry* e11E = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg2 + 8, 0x11E);
                            if (e11E != nullptr && move->field_68 > lbl_eu_80666DDC) {
                                f32 f68 = move->field_68;
                                ((cf::CActorParam*)(arg2))->CActorParam_getDamageScale();
                                s32 s14 = e11E->unk14;
                                cf::CfActorF64Conv conv;
                                conv.w[0] = 0x43300000;
                                conv.w[1] = (u32)s14 ^ 0x80000000;
                                f32 fs = (f32)(conv.d - lbl_eu_80666DE0);
                                s32 rnd = (s32)(fs * f68 / 100.0f);
                                conv.w[1] = (u32)rnd ^ 0x80000000;
                                f32 fadd = (f32)(conv.d - lbl_eu_80666DE0);
                                move->field_68 = move->field_68 + fadd;
                                if (e11E->unk10 > 0) {
                                    if (move->field_70 == 0) {
                                        move->field_70 = (s16)e11E->unk10;
                                    }
                                    s32 tick = move->field_70 - 1;
                                    move->field_70 = (s16)tick;
                                    if (tick <= 0) {
                                        move->field_70 = 0;
                                        move->field_78 |= 0x40004000;
                                    }
                                }
                            }
                        }
                        // 0x37 proc-rate block (same shape as 0x92).
                        if (isBattleStatusActive((u8*)arg2 + 8, 0x37)) {
                            s32 bonus = 0;
                            if (isBattleStatusActive((u8*)arg2 + 8, 0xA1)) {
                                cf::CBattleStateEntry* eA1 = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg2 + 8, 0xA1);
                                bonus = eA1->unk10;
                            }
                            cf::CBattleStateEntry* e37 = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)arg2 + 8, 0x37);
                            if (e37 != nullptr && move->field_68 > lbl_eu_80666DDC) {
                                s32 need = e37->unk1A;
                                if (e37->unk08 == 0x2000) need += bonus;
                                u32 dotValue = e37->unk10;
                                calcBattleDamage((void*)0, arg2, (void*)0);
                                cf::CfActorF64Conv conv;
                                conv.w[0] = 0x43300000;
                                conv.w[1] = (u32)dotValue ^ 0x80000000;
                                f32 fv = (f32)(conv.d - lbl_eu_80666DE0);
                                f32 prod = fv * move->field_68;
                                f32 rnd = (f32)(s32)prod;
                                conv.w[1] = (u32)(s32)rnd ^ 0x80000000;
                                move->field_68 =
                                    move->field_68 + (f32)(conv.d - lbl_eu_80666DE0);
                            }
                        }
                    }
                }
            }
        }
    }

    // Final clamp: move stays active until the accumulated dot drains.
    if (move->field_68 <= lbl_eu_80666DDC) {
        move->field_78 |= 0x40001000;
    }
}

// emitBattleEvents (retail 0x800E13D0, 0x125C bytes): battle-event emission and
// move-state update shared by the move-type dispatchers. Resolves the event
// source via the 0xF7 status / 0x5C0 self-check, runs the arts-type jump
// table, emits events through emitBattleMoveEvents, then applies counter / dot /
// proc-rate damage math on the move block.
void emitBattleEvents(void* self_, void* attacker_, void* target_, void* move_) {
    // Two int->float conversion slots (Gekko 2^52-bias trick). Only the
    // sign-flipped low words change per conversion; the 0x43300000 high words
    // are written once up front and both slots stay live for the whole body.
    cf::CfActorF64Conv cvtA;
    cvtA.w[0] = 0x43300000;

    void* self = self_;          // r26
    void* attacker = attacker_;  // r27
    void* target = target_;      // r28

    cf::CfActorF64Conv cvtB;
    cvtB.w[0] = 0x43300000;

    cf::CActorParam_UnkStruct1* move = (cf::CActorParam_UnkStruct1*)move_;   // r29

    if (target == nullptr) return;

    if (!(move->mFlagsArray[0].flags & 0x08)) {
        if (isBattleStatusActive((u8*)target + 8, 0xF7)) {
            if (((cf::CfObjectActor*)target)->CfObjectActor_sharesMoveFlags((cf::CfObjectActor*)attacker) == 0) {
                findBattleStatusEntry((u8*)target + 8, 0xF7);
                if ((move->mFlagsArray[1].flags & 0x200) ||
                    ((move->mFlagsArray[1].flags & 0x400) && !(move->mFlagsArray[1].flags & 0x800))) {
                    move->mFlagsArray[0].flags |= 0x80000080;
                }
            }
        }
    }

    move->unk60 = lbl_eu_80666DDC;
    cf::CActorParam_UnkStruct2* subObj = (cf::CActorParam_UnkStruct2*)move->unk50;   // r31
    move->unk64 = lbl_eu_80666DDC;
    ((cf::CfObjectActor*)attacker)->CActorParam_getBattleParams();
    ((cf::CfObjectActor*)target)->CActorParam_getBattleParams();

    s32 hit;
    if (move->mFlagsArray[0].flags & 0x08) {
        if (!(subObj->unk78 & 0x800)) goto lateBlock;
    }
    hit = 0;
    if (subObj->unk78 & 0x2000) {
        if (subObj->field_44 != (u8)(((cf::CfObjectActor*)attacker)->CActorParam_getStatsSlot() + 1)) {
            goto lateBlock;
        }
    }

    // Arts-type jump table (byte +0x66 of the sub-object).
    switch (subObj->field_66) {
    case 0x00:
        if (((cf::CfObjectActor*)attacker)->unk3374 & 0x8000) {
            s32 e0 = ((cf::CfObjectActor*)target)->CActorParam_getActorType();
            if (e0 == 1 || e0 == 2) hit = 1;
        } else {
            if (((cf::CfObjectActor*)target)->CActorParam_getActorType() == 1) hit = 1;
        }
        break;
    case 0x01:
        if (((cf::CfObjectActor*)target)->CActorParam_getActorType() == 3) hit = 1;
        break;
    case 0x02:
        if (move->mFlagsArray[0].flags & 0x01000000) hit = 1;
        break;
    case 0x03:
        if (move->mFlagsArray[0].flags & 0x04000000) hit = 1;
        break;
    case 0x04:
        if (move->mFlagsArray[0].flags & 0x02000000) hit = 1;
        break;
    case 0x05:
        if (isBattleStatusActive((u8*)attacker + 8, 0xC1)) hit = 1;
        break;
    case 0x06:
        if (isBattleStatusActive((u8*)target + 8, 0x10)) hit = 1;
        break;
    case 0x07:
        if (isBattleStatusActive((u8*)target + 8, 0x0F)) hit = 1;
        break;
    case 0x08:
        if ((u32)((cf::CObjectParam*)((u8*)target + 0x3E9C))->CObjectParam_getSelfObjectId() ==
            ((cf::CfObjectActor*)attacker)->field_0x74) {
            hit = 1;
        }
        break;
    case 0x09:
        if (move->unk5C >= ((cf::CfObjectActor*)target)->CActorParam_getHp()) hit = 1;
        break;
    case 0x0A:
        if (subObj->field_67 != 0) hit = 1;
        break;
    case 0x0B:
        if ((u16)getReloadParam0() == (u16)(subObj->field_66 - 0x0A)) hit = 1;
        break;
    case 0x0C:
        if (move->mFlagsArray[0].flags & 0x04000000) {
            s32 e0 = ((cf::CfObjectActor*)target)->CActorParam_getActorType();
            if (e0 == 1 || e0 == 2) hit = 1;
        }
        break;
    case 0x0D:
        if (((cf::CfObjectActor*)target)->CActorParam_getHitRate() < 0.15f) hit = 1;
        break;
    default:
        break;
    }

postSwitch:
    if (subObj->unk78 & 0x800) {
        if (findFreeBattleSlot((D7D24_Obj*)move) == 0) hit = 0;
    }
    if (!(move->mFlagsArray[0].flags & 0x10000000)) {
        move->unk5C = move->unk54 * move->unk58;
    }

    if (hit == 0) goto lateBlock;

    {
        // ---- event emission (dispatch on subObj->0x67) ----
        BattleEvent ev;
        std::memset(&ev, 0, sizeof(ev));
        switch (subObj->field_67) {
        case 0x03: {
            // HP adjustment path: no event broadcast, just the move update.
            if (move->mFlagsArray[1].flags & 0x10) break;
            cvtA.w[1] = (u32)(subObj->field_68 - 1) ^ 0x80000000;
            move->unk58 = move->unk58 + (f32)(cvtA.d - lbl_eu_80666DE0);
            move->unk5C = move->unk54 * move->unk58;
            if (((cf::CfObjectActor*)attacker)->CActorParam_getStatusTable() != 0) {
                void* chain = (void*)((cf::CfObjectActor*)attacker)->CActorParam_getStatusTable();
                if (Counter_TestBit(chain, 0x9C) != 0) {
                    if (subObj->field_66 == 3) {
                        move->unk58 = move->unk58 + lbl_eu_80666DD4;
                    }
                }
            }
            break;
        }
        case 0x07:
            ev.field_00 = (u32)(void*)((cf::CfObjectActor*)attacker)->field_0x74;
            ev.field_04 = (u32)subObj;
            ev.eventType = 0x10;
            ev.field_10 = 0;
            ev.field_24 = lbl_eu_80666DDC;
            cvtB.w[1] = (u32)subObj->field_68 ^ 0x80000000;
            ev.field_20 = (f32)(cvtB.d - lbl_eu_80666DE0);
            if (subObj->unk78 & 0x1000) {
                emitBattleMoveEvents(self, attacker, attacker, &ev, move);
            } else {
                emitBattleMoveEvents(self, attacker, target, &ev, move);
            }
            break;
        case 0x08:
            ev.field_00 = (u32)(void*)((cf::CfObjectActor*)attacker)->field_0x74;
            ev.field_04 = (u32)subObj;
            ev.eventType = 0x0F;
            ev.field_10 = 0;
            ev.field_24 = lbl_eu_80666DDC;
            cvtB.w[1] = (u32)subObj->field_68 ^ 0x80000000;
            ev.field_20 = (f32)(cvtB.d - lbl_eu_80666DE0);
            if (subObj->unk78 & 0x1000) {
                emitBattleMoveEvents(self, attacker, attacker, &ev, move);
            } else {
                emitBattleMoveEvents(self, attacker, target, &ev, move);
            }
            break;
        case 0x0B: {
            // Broadcast to every enumerated actor except the target.
            ev.field_00 = (u32)(void*)((cf::CfObjectActor*)attacker)->field_0x74;
            ev.field_04 = (u32)subObj;
            ev.eventType = 0x10;
            ev.field_10 = 3;
            ev.field_20 = 3.0f;
            ev.field_24 = lbl_eu_80666DDC;
            {
                CfEnumListHolder holder;
                startEnumObjects(holder.get(), 0x8000, 0);
                for (u32 i = 0; i < ((cf::CVisionEnumList*)holder.get())->count; i++) {
                    void* o = MapFx_ResolveOwner(getObjectAt(holder.get(), i));
                    if (o != target) {
                        emitBattleMoveEvents(self, attacker, o, &ev, move);
                    }
                }
            }
            break;
        }
        case 0x0D:
            ev.field_00 = (u32)(void*)((cf::CfObjectActor*)attacker)->field_0x74;
            ev.field_04 = (u32)subObj;
            ev.eventType = 0x10;
            ev.field_10 = 3;
            ev.field_20 = 3.0f;
            ev.field_24 = lbl_eu_80666DDC;
            emitBattleMoveEvents(self, attacker, target, &ev, move);
            break;
        case 0x06:
            ev.field_00 = (u32)(void*)((cf::CfObjectActor*)attacker)->field_0x74;
            ev.field_04 = (u32)subObj;
            ev.eventType = 0xDC;
            ev.field_10 = (u32)subObj->field_68;
            if (subObj->unk78 & 0x1000) {
                emitBattleMoveEvents(self, attacker, attacker, &ev, move);
            } else {
                emitBattleMoveEvents(self, attacker, target, &ev, move);
            }
            break;
        case 0x01: {
            ev.field_00 = (u32)(void*)((cf::CfObjectActor*)attacker)->field_0x74;
            ev.field_04 = (u32)subObj;
            ev.eventType = (u16)subObj->field_68;
            ev.field_10 = (u32)subObj->field_6A;
            s32 sv = artsSubGetMax(subObj) - 1;
            cvtA.w[1] = (u32)subObj->field_6E ^ 0x80000000;
            f32 t1 = (f32)(cvtA.d - lbl_eu_80666E08);
            cvtB.w[1] = (u32)sv ^ 0x80000000;
            f32 t2 = (f32)(cvtB.d - lbl_eu_80666DE0);
            ev.field_20 = subObj->field_50 + t1 / k10_0f * t2;
            ev.field_24 = subObj->field_54;
            if (subObj->unk78 & 0x1000) {
                emitBattleMoveEvents(self, attacker, attacker, &ev, move);
            } else {
                emitBattleMoveEvents(self, attacker, target, &ev, move);
            }
            break;
        }
        default:
            break;
        }
    }

lateBlock:
    if (!(move->mFlagsArray[0].flags & 0x10000000)) {
        move->unk5C = move->unk54 * move->unk58;
    }

    // Second sub-object event block (+0x48 id).
    if (move->mFlagsArray[0].flags & 0x08) {
        if (!(subObj->unk78 & 0x400)) goto tail2;
    }
    if (subObj->unk78 & 0x2000) {
        if (subObj->field_44 != (u8)(((cf::CfObjectActor*)attacker)->CActorParam_getStatsSlot() + 1)) goto tail2;
    }
    if (subObj->field_48 == 0) goto tail2;

    {
        BattleEvent ev2;
        std::memset(&ev2, 0, sizeof(ev2));
        ev2.field_00 = (u32)(void*)((cf::CfObjectActor*)attacker)->field_0x74;
        ev2.field_04 = (u32)subObj;
        ev2.eventType = subObj->field_48;
        s32 sv2 = artsSubGetMax(subObj) - 1;
        ev2.field_10 = (u32)subObj->field_4A + (u32)subObj->field_6F * (u32)sv2;
        ev2.field_14 = (u16)subObj->field_4C;
        cvtA.w[1] = (u32)subObj->field_6E ^ 0x80000000;
        f32 u1 = (f32)(cvtA.d - lbl_eu_80666E08);
        cvtB.w[1] = (u32)sv2 ^ 0x80000000;
        f32 u2 = (f32)(cvtB.d - lbl_eu_80666DE0);
        ev2.field_20 = subObj->field_50 + u1 / k10_0f * u2;
        ev2.field_24 = subObj->field_54;
        if (subObj->unk78 & 0x400) {
            emitBattleMoveEvents(self, attacker, attacker, &ev2, move);
        } else {
            emitBattleMoveEvents(self, attacker, target, &ev2, move);
        }
    }

tail2:
    // Counter damage gate: protect against hitting an already-active enemy.
    {
        s32 cnt = 0;
        f32 csum = move->unk5C + move->unk60;
        if (csum >= lbl_eu_80666DDC && !(move->mFlagsArray[0].flags & 0x80)) {
            cnt = (s32)((f64)csum + (csum <= lbl_eu_80666DDC ? -0.5 : 0.5));
        }
        if (cnt > 0) {
            if (!(move->mFlagsArray[0].flags & 0x10000000)) {
                if (isBattleStatusActive((u8*)target + 8, 0xF7)) {
                    if (((cf::CfObjectActor*)target)->CfObjectActor_sharesMoveFlags((cf::CfObjectActor*)attacker) == 0) {
                        if ((move->mFlagsArray[1].flags & 0x200) ||
                            ((move->mFlagsArray[1].flags & 0x400) && !(move->mFlagsArray[1].flags & 0x800))) {
                            move->mFlagsArray[0].flags |= 0x80000080;
                        }
                    }
                }
            }
            if ((move->mFlagsArray[0].flags & 0x8000) || !(move->mFlagsArray[1].flags & 0x8000)) {
                if (!isBattleStatusActive((u8*)target + 8, 0x33)) {
                    goto dotBlock;
                }
                cf::CfStatusEntry* entry = (cf::CfStatusEntry*)findBattleStatusEntry(
                    (u8*)target + 8, 0x33);
                if (entry == nullptr) goto dotBlock;

                if (move->mFlagsArray[1].flags & 0x04000000) {
                    // per-hit counter bleed tracked in move->unk72
                    s16 steps = entry->field_0x14;
                    if (steps <= 0) goto dotBlock;
                    if (move->unk72 == -1) goto dotBlock;
                    if (move->unk72 == 0) {
                        move->unk72 = steps;
                    }
                    s32 cnt2 = 0;
                    f32 s2 = move->unk5C + move->unk60;
                    if (s2 >= lbl_eu_80666DDC && !(move->mFlagsArray[0].flags & 0x80)) {
                        cnt2 = (s32)((f64)s2 + (s2 <= lbl_eu_80666DDC ? -0.5 : 0.5));
                    }
                    cvtB.w[1] = (u32)cnt2 ^ 0x80000000;
                    cvtA.w[1] = (u32)(s16)move->unk72 ^ 0x80000000;
                    move->unk64 = (f32)(cvtB.d - lbl_eu_80666DE0);
                    f32 dec = move->unk5C - (f32)(cvtA.d - lbl_eu_80666DE0);
                    f32 news = dec + move->unk60;
                    move->unk5C = dec;
                    move->mFlagsArray[0].flags |= 0x80008000;
                    s32 cnt3 = 0;
                    if (news >= lbl_eu_80666DDC && !(move->mFlagsArray[0].flags & 0x80)) {
                        cnt3 = (s32)((f64)news + (news <= lbl_eu_80666DDC ? -0.5 : 0.5));
                    }
                    if (cnt3 > 0) {
                        move->unk72 = -1;
                        move->mFlagsArray[1].flags |= 0x40008000;
                    } else {
                        move->unk72 = (s16)(s32)-move->unk5C;
                    }
                } else {
                    s16 steps = entry->field_0x14;
                    if (steps <= 0) goto dotBlock;
                    s32 cntB = 0;
                    f32 sB = move->unk5C + move->unk60;
                    if (sB >= lbl_eu_80666DDC && !(move->mFlagsArray[0].flags & 0x80)) {
                        cntB = (s32)((f64)sB + (sB <= lbl_eu_80666DDC ? -0.5 : 0.5));
                    }
                    cvtB.w[1] = (u32)cntB ^ 0x80000000;
                    move->unk64 = (f32)(cvtB.d - lbl_eu_80666DE0);
                    cvtA.w[1] = (u32)steps ^ 0x80000000;
                    move->mFlagsArray[0].flags |= 0x80008000;
                    move->unk5C = move->unk5C - (f32)(cvtA.d - lbl_eu_80666DE0);
                }
            }
        }
    }

dotBlock:
    {
        // Arts-type 3 dot/regeneration block.
        if (subObj->field_3C != 3) return;
        s32 dv = 0;
        f32 ds = move->unk5C + move->unk60;
        if (ds >= lbl_eu_80666DDC && !(move->mFlagsArray[0].flags & 0x80)) {
            dv = (s32)((f64)ds + (ds <= lbl_eu_80666DDC ? -0.5 : 0.5));
        }
        if (dv <= 0) return;

        if (!(move->mFlagsArray[1].flags & 0x10)) {
            cvtB.w[1] = (u32)dv ^ 0x80000000;
            move->unk68 = move->unk68 - (f32)(cvtB.d - lbl_eu_80666DE0);
        }
        if (move->mFlagsArray[1].flags & 0x1000) goto finalClamp;
        if (((cf::CfObjectActor*)target)->CActorParam_isBattleLocked() != 0) goto finalClamp;

        // 0x92 proc-rate block.
        if (isBattleStatusActive((u8*)target + 8, 0x92)) {
            s32 bonus = 0;
            if (isBattleStatusActive((u8*)target + 8, 0xA1)) {
                bonus = ((cf::CfStatusEntry*)findBattleStatusEntry(
                    (u8*)target + 8, 0xA1))->field_0x10;
            }
            cf::CfStatusEntry* e92 = (cf::CfStatusEntry*)findBattleStatusEntry(
                (u8*)target + 8, 0x92);
            if (e92 != nullptr && move->unk68 <= lbl_eu_80666DDC) {
                s32 need = e92->field_0x10;
                if (e92->field_0x8 == 0x2000) need += bonus;
                if ((s32)(move->unk84[9] % 100) < need) {
                    move->mFlagsArray[0].flags |= 0xA0000000;
                    move->unk68 = lbl_eu_80666DD4;
                }
            }
        }
        if (!(move->mFlagsArray[1].flags & 0x2000)) {
            // 0xFC fixed-rate bleed.
            if (isBattleStatusActive((u8*)target + 8, 0xFC)) {
                cf::CfStatusEntry* eFC = (cf::CfStatusEntry*)findBattleStatusEntry(
                    (u8*)target + 8, 0xFC);
                if (eFC != nullptr && move->unk68 <= lbl_eu_80666DDC) {
                    move->mFlagsArray[1].flags |= 0x40002000;
                    f32 mult = ((cf::CfObjectActor*)target)->CActorParam_getDamageScale();
                    cvtA.w[1] = (u32)eFC->field_0x10 ^ 0x80000000;
                    move->unk68 = move->unk68 *
                        ((f32)(cvtA.d - lbl_eu_80666DE0) * mult);
                }
            }
        }
        if (!(move->mFlagsArray[1].flags & 0x2000)) {
            // 0x100 fixed-rate bleed.
            if (isBattleStatusActive((u8*)target + 8, 0x100)) {
                cf::CfStatusEntry* e100 = (cf::CfStatusEntry*)findBattleStatusEntry(
                    (u8*)target + 8, 0x100);
                if (e100 != nullptr && move->unk68 <= lbl_eu_80666DDC) {
                    move->mFlagsArray[1].flags |= 0x40002000;
                    f32 mult = ((cf::CfObjectActor*)target)->CActorParam_getDamageScale();
                    cvtB.w[1] = (u32)e100->field_0x10 ^ 0x80000000;
                    move->unk68 = move->unk68 *
                        ((f32)(cvtB.d - lbl_eu_80666DE0) * mult);
                }
            }
        }
        if (!(move->mFlagsArray[1].flags & 0x4000)) {
            // 0x36 growth block.
            if (isBattleStatusActive((u8*)target + 8, 0x36)) {
                cf::CfStatusEntry* e36 = (cf::CfStatusEntry*)findBattleStatusEntry(
                    (u8*)target + 8, 0x36);
                if (e36 != nullptr && move->unk68 > lbl_eu_80666DDC) {
                    s32 v36 = e36->field_0x10;
                    calcBattleDamage(nullptr, target, nullptr);
                    cvtA.w[1] = (u32)v36 ^ 0x80000000;
                    f32 fv = (f32)(cvtA.d - lbl_eu_80666DE0);
                    f32 prod = fv * move->unk68;
                    cvtB.w[1] = (u32)(s32)prod ^ 0x80000000;
                    move->unk68 = move->unk68 + (f32)(cvtB.d - lbl_eu_80666DE0);
                }
            }
        }
        // 0x37 proc-rate block (same shape as 0x92).
        if (isBattleStatusActive((u8*)target + 8, 0x37)) {
            s32 bonus = 0;
            if (isBattleStatusActive((u8*)target + 8, 0xA1)) {
                bonus = ((cf::CfStatusEntry*)findBattleStatusEntry(
                    (u8*)target + 8, 0xA1))->field_0x10;
            }
            cf::CfStatusEntry* e37 = (cf::CfStatusEntry*)findBattleStatusEntry(
                (u8*)target + 8, 0x37);
            if (e37 != nullptr && move->unk68 > lbl_eu_80666DDC) {
                s32 need = e37->field_0x1A;
                if (e37->field_0x8 == 0x2000) need += bonus;
                if ((s32)(move->unk84[9] % 100) < need) {
                    s32 v37 = e37->field_0x10;
                    calcBattleDamage(nullptr, target, nullptr);
                    cvtA.w[1] = (u32)v37 ^ 0x80000000;
                    f32 fv = (f32)(cvtA.d - lbl_eu_80666DE0);
                    f32 prod = fv * move->unk68;
                    cvtB.w[1] = (u32)(s32)prod ^ 0x80000000;
                    move->unk68 = move->unk68 + (f32)(cvtB.d - lbl_eu_80666DE0);
                }
            }
        }
        if (!(move->mFlagsArray[1].flags & 0x4000)) {
            // 0x11E protection block.
            if (isBattleStatusActive((u8*)target + 8, 0x11E)) {
                cf::CfStatusEntry* e11E = (cf::CfStatusEntry*)findBattleStatusEntry(
                    (u8*)target + 8, 0x11E);
                if (e11E != nullptr && move->unk68 > lbl_eu_80666DDC) {
                    f32 mult = ((cf::CfObjectActor*)target)->CActorParam_getDamageScale();
                    cvtA.w[1] = (u32)e11E->field_0x14 ^ 0x80000000;
                    f32 fsm = (f32)(cvtA.d - lbl_eu_80666DE0);
                    s32 q1 = (s32)((fsm * mult) / 100.0f);
                    calcBattleDamage(nullptr, target, nullptr);
                    cvtB.w[1] = (u32)q1 ^ 0x80000000;
                    f32 prod2 = (f32)(cvtB.d - lbl_eu_80666DE0) * (fsm * mult);
                    cvtA.w[1] = (u32)(s32)prod2 ^ 0x80000000;
                    move->unk68 = move->unk68 + (f32)(cvtA.d - lbl_eu_80666DE0);
                    if (e11E->field_0x10 > 0) {
                        if (move->unk70 == 0) {
                            move->unk70 = (s16)e11E->field_0x10;
                        }
                        s32 tick = move->unk70 - 1;
                        move->unk70 = (s16)tick;
                        if (tick <= 0) {
                            move->unk70 = 0;
                            move->mFlagsArray[1].flags |= 0x40004000;
                        }
                    }
                }
            }
        }
    }

finalClamp:
    // Final clamp: move stays active until the accumulated dot drains.
    if (move->unk68 <= lbl_eu_80666DDC) {
        move->mFlagsArray[1].flags |= 0x40001000;
    }
}

// E921SlotAcc deleted: use cf::CActorParam_UnkStruct1

// 0xBC-byte move snapshot block copied by value across the damage-preview
// paths. The retail copy codegen is: single words at +0x00/+0x04, eight
// 8-byte chunks +0x08..+0x44, three singles +0x48..+0x50, seven floats
// +0x54..+0x6C, two s16s, a word pair +0x74/+0x78, a single +0x7C, a u16
// +0x80, six word pairs +0x84..+0xB0, then two singles +0xB4/+0xB8.
// CActorParam_UnkStruct1 deleted

// Vtable mirror for the battle-actor dispatch slots used below. Never
// instantiated -- casting a retail object to this type and calling the named
// virtuals makes MWCC route the dispatch through r12 (retail order) instead of
// a general scratch register (cf. ActorVtableMirror / CVisionBattleObj).
struct E2A9C_BattleMoveData {
    u32  field_00;      // 0x00
    u32  field_04;      // 0x04
    u32  unk08[16];     // 0x08
    u32  field_48;      // 0x48
    u32  field_4C;      // 0x4C (retail prologue initialises to -1)
    void* field_50;     // 0x50 sub-object
    f32  field_54;      // 0x54
    f32  field_58;      // 0x58
    f32  field_5C;      // 0x5C (damage amount / knockback)
    f32  field_60;      // 0x60
    f32  field_64;      // 0x64
    f32  field_68;      // 0x68
    f32  field_6C;      // 0x6C
    s16  field_70;      // 0x70
    s16  field_72;      // 0x72
    struct { u32  field_74; u32  field_78; };
    u32  field_7C;      // 0x7C
    u16  field_80;      // 0x80 (0x82: alignment padding, not copied)
    struct {
        u32  field_84;      // 0x84
        u32  field_88;      // 0x88
        u32  field_8C;      // 0x8C
        u32  field_90;      // 0x90
        u32  field_94;      // 0x94
        u32  field_98;      // 0x98
        u32  field_9C;      // 0x9C
        u32  field_A0;      // 0xA0
        u32  field_A4;      // 0xA4
        u32  field_A8;      // 0xA8 (attack power threshold)
        u32  field_AC;      // 0xAC
        u32  field_B0;      // 0xB0 (combo count, %100 used)
    };
    u32  field_B4;      // 0xB4
    u32  field_B8;      // 0xB8
};

// previewBattleDamage (retail 0x800E9D04, 0x934 bytes): battle damage-preview setup.
// Computes the damage preview for the party's selected arts: resolves the
// target sub-object/action, clears all player selection flags, fills the
// actor stat block and per-member move snapshots, then accumulates the
// rounded damage sum into *outDamage.
static inline s32 roundedMoveDamage(E2A9C_BattleMoveData* slot) {
    f32 sum = slot->field_5C + slot->field_60;
    if (sum < lbl_eu_80666DDC) return 0;
    if (slot->field_74 & 0x80) return 0;
    return (s32)(sum + (sum > lbl_eu_80666DDC ? 0.5 : -0.5));
}
static inline void accumulateMoveDamage(f32& total, E2A9C_BattleMoveData* slot) {
    total += (f32)roundedMoveDamage(slot);
}
void previewBattleDamage(void* self, ScMain* actor, cf::CActorParam* obj, f32* outDamage, s32* outFlag) {

    // r23: selected target sub-object from obj's move slot; r31: its action.
    void* subObj = *(void**)((u8*)obj->CActorParam_getBattleStats() + 0x50);
    cf::CActorParam* action = (cf::CActorParam*)MapFx_ResolveOwner((void*)(intptr_t)findObjectById__Fi(
        *(s32*)((u8*)obj->CActorParam_getBattleStats() + 4)));

    *outDamage = lbl_eu_80666DDC;
    *outFlag = 0;
    std::memset(actor->zero_828, 0, sizeof(actor->zero_828));

    if (action == nullptr) return;
    if (subObj == nullptr) return;

    // Talent-gauge-full one-round path: the preview becomes the computed value.
    if (actor->flags_824 & 0x20000) {  // bit14 (rlwinm 14,14)
        *outDamage = (f32)(s32)(intptr_t)ScMain_GetSummedInt(actor);
        return;
    }

    // Party size / arts-count gate for the preview loop.
    if ((s32)*(u8*)((u8*)subObj + 0x43) == 1) {
        *outFlag = *(u8*)((u8*)subObj + 0x44);
    } else {
        *outFlag = 1;
    }

    // Clear the selection flag on the action, the 3 party members, and obj.
    action->CActorParam_resetArtsStatus(0);
    for (s32 i = 0; i < 3; i++) {
        cf::CActorParam* player =
            (cf::CActorParam*)MapFx_ResolveOwner(getPlayer__Q22cf13CfGameManagerFi(i));
        if (player != nullptr && player != action) {
            player->CActorParam_resetArtsStatus(0);
        }
    }
    obj->CActorParam_resetArtsStatus(0);
    action->CActorParam_resetArtsStatus(0);

    actor->field_88 &= ~0x1000;

    // Store the base damage into the target move slots (0x6C/0x68) and zero
    // the tick counters (each store re-fetches the slot via 0x298).
    {
        f32 dmg = obj->CActorParam_getHp();
        ((E2A9C_BattleMoveData*)obj->CActorParam_getBattleStats())->field_6C = dmg;
    }
    {
        f32 dmg = action->CActorParam_getHp();
        ((E2A9C_BattleMoveData*)obj->CActorParam_getBattleStats())->field_68 = dmg;
    }
    ((E2A9C_BattleMoveData*)obj->CActorParam_getBattleStats())->field_70 = 0;
    ((E2A9C_BattleMoveData*)obj->CActorParam_getBattleStats())->field_72 = 0;

    // Reset each party member's move slot.
    for (s32 i = 0; i < *outFlag; i++) {
        E2A9C_BattleMoveData* slot = (E2A9C_BattleMoveData*)obj->CActorParam_getStatsEntry(i);
        slot->field_74 = 0;
        slot->field_78 = (slot->field_78 & 0xFFFF0000) | (slot->field_78 & 0x00000FC0);
        slot->field_54 = lbl_eu_80666DDC;
        slot->field_58 = lbl_eu_80666DDC;
        slot->field_5C = lbl_eu_80666DDC;
        slot->field_60 = lbl_eu_80666DDC;
        slot->field_64 = lbl_eu_80666DDC;
    }

    // Per-party-member preview loop: run the ai dispatcher, snapshot the
    // stat block / per-member move data, accumulate the rounded damage.
    f32 zero2 = lbl_eu_80666DDC;
    s32 idx = 0;
    while (idx < *outFlag) {
        E2A9C_BattleMoveData* slot = (E2A9C_BattleMoveData*)obj->CActorParam_getBattleStats();   // r29
        slot->field_78 |= 0x44000000;
        dispatchBattleAI((void*)(intptr_t)cf::CBattleManager::spInstance, obj, action, slot);
        slot->field_78 &= ~0x04000000;

        if (obj->CActorParam_getStatsSlot() == 0) {
            // Snapshot into the actor's displayed stat block (0x10..0xC8).
            *(E2A9C_BattleMoveData*)(&actor->field_10) =
                *(const E2A9C_BattleMoveData*)obj->CActorParam_getBattleStats();
        } else {
            u32 fl = ((E2A9C_BattleMoveData*)obj->CActorParam_getBattleStats())->field_74;
            if (fl & 0x80000000) {
                actor->flags_84 |= fl;
            } else {
                actor->field_88 |= fl;
            }
        }

        // Per-member move snapshot at actor + v2A8()*0xBC + 0xCC.
        *(E2A9C_BattleMoveData*)(&actor->subs[obj->CActorParam_getStatsSlot()]) =
            *(const E2A9C_BattleMoveData*)slot;

        if (slot->field_78 & 0x1000) {
            *(u32*)&actor->zero_828[0] = 0;
            actor->field_88 |= 0x40001000;
            *(u32*)&actor->zero_828[4] = 0;
        }

        // Round base+variance to the nearest int and accumulate.
        accumulateMoveDamage(*outDamage, slot);
        obj->CActorParam_pushBattleStats();

        if (slot->field_6C < zero2) break;
        idx++;
    }

    // Final snapshot into the 0x2A4 target block.
    obj->CActorParam_resetStatsSlot();
    cf::CActorParam* src = (cf::CActorParam*)obj->CActorParam_getBattleStats();
    void* dst = obj->CActorParam_getMoveRecord();
    *(E2A9C_BattleMoveData*)dst = *(const E2A9C_BattleMoveData*)src;

    if (*outDamage < lbl_eu_80666DDC) *outDamage = lbl_eu_80666DDC;
    if (*outFlag > 1) actor->f_6C = *outDamage;
}
// ---------------------------------------------------------------------------
// AI action slot (0x20 bytes) installed via aiRingPrepend. Byte-precise to the
// retail store sequence (memset + 7 stores).
// ---------------------------------------------------------------------------
struct BattleAIActionSlot {
    u32  unk00;         // 0x00
    u8   pad04[2];      // 0x04
    u8   b_06;          // 0x06 = 0x25
    u8   pad07[6];      // 0x07
    u8   b_0D;          // 0x0D = 0x54 / 0x55
    u8   b_0E;          // 0x0E = 0x64
    u8   pad0F;         // 0x0F
    u16  h_10;          // 0x10 = 0
    s16  h_12;          // 0x12 = -1
    f32  f_14;          // 0x14 = lbl_eu_80666DDC
    u8   pad18[8];      // 0x18
};

// Snapshot block laid out at sp+0xF4 in retail: the move-data copy, then the
// target pointer (+0x1B0) and the action (+0x1B4) right behind it.
struct E1B5C_Snapshot {
    cf::CActorParam_UnkStruct1 data;
    void* target;
    void* action;
};

// Workspace block handed to dispatchBattleAI / dispatchBattleMove (retail lays it
// out at sp+0x38). Field pattern: src words at +0x00..0x50, a zeroed float
// pack at +0x54..0x64, damage/variance at +0x68/+0x6C, s16s at +0x70/+0x72,
// flags at +0x74/+0x78, and the shared word tail at +0x80..0xB8.
struct E1B5C_Workspace {
    u32 v00; u32 v04; u32 v08; u32 v0C; u32 v10; u32 v14; u32 v18; u32 v1C;
    u32 v20; u32 v24; u32 v28; u32 v2C; u32 v30; u32 v34; u32 v38; u32 v3C;
    u32 v40; u32 v44; u32 v48; u32 v4C;
    void* v50;      // +0x50 src[0x50]
    f32 v54;        // +0x54 0.0
    f32 v58;        // +0x58 0.0
    f32 v5C;        // +0x5C 0.0
    f32 v60;        // +0x60 0.0
    f32 v64;        // +0x64 0.0
    f32 v68;        // +0x68 src[0x5C]
    f32 v6C;        // +0x6C src[0x60]
    s16 v70;        // +0x70 src[0x70]
    s16 v72;        // +0x72 src[0x72]
    u32 v74;        // +0x74 0
    u32 v78;        // +0x78 flags
    u32 v7C;        // +0x7C 0
    u16 v80;        // +0x80 src[0x80]
    u16 pad82;      // +0x82
    u32 v84; u32 v88; u32 v8C; u32 v90; u32 v94; u32 v98; u32 v9C;
    u32 vA0; u32 vA4; u32 vA8; u32 vAC; u32 vB0; u32 vB4;
    void* vB8;      // +0xB8 &copy
};

// evalBattleTarget (retail 0x800E1B5C, 0xAF0 bytes): party-chain / target-table
// refresh. Verifies the actor's two move objects reference the same target,
// snapshots the source move data, broadcasts the arts action to the party
// (via the vision enum list or the 2x8 target-slot scan + chain handoff), then
// re-runs the 16-slot fallback building per-target event workspaces.
extern "C" void CChain_RegisterCombo(void*, void*);
extern "C" void ChainTmr_CheckArts_D478(void*, void*);
extern "C" void SysWinBuff_EmptyStubB24(void*, void*);
extern "C" void CCharVoiceMan_HookBattleSlotVoice(void*, void*);
extern "C" void dispatchBattleMove(void*, void*, void*, void*);

void evalBattleTarget(void* mgr, void* actor) {
    cf::CActorParam* act = (cf::CActorParam*)actor;
    cf::CfObjectActor* av = (cf::CfObjectActor*)actor;

    // NOTE: dst declared first - MWCC colors locals in declaration order and
    // retail allocates it above src.
    cf::CActorParam_UnkStruct1* dst;
    cf::CActorParam_UnkStruct1* src = (cf::CActorParam_UnkStruct1*)act->CActorParam_getBattleStats();     // r30: move obj A
    dst = (cf::CActorParam_UnkStruct1*)act->CActorParam_getStagingStats();                     // move obj B

    // Both move objects must name the same target id / sub-object.
    if ((s32)src->w48 != (s32)dst->w48) return;
    if (src->w50 != dst->w50) return;
    if (av->mTargetC4 == 0) return;

    cf::CActorParam* action =
        (cf::CActorParam*)MapFx_ResolveOwner(
            (void*)(intptr_t)findObjectById__Fi((s32)src->w04));

    // Snapshot the full move block (sp+0xF4..0x1AC); the target pointer
    // (sp+0x1B0) and the action (sp+0x1B4) sit right behind it.
    E1B5C_Snapshot snap;
    snap.target = (void*)src->w50;
    snap.data = *src;
    snap.action = action;
    void* targetPtr = snap.target;
    cf::CActorParam_UnkStruct1& copy = snap.data;

    if (action == nullptr) goto skipScan;
    if (((cf::CfObjectActor*)action)->mTargetC4 == 0) goto skipScan;
    if ((((cf::CfObjectActor*)action)->unk64 & 0x4) == 0) {
        if ((((cf::CfObjectActor*)action)->unk64 & 0x2) == 0) return;
    }
    if (targetPtr == nullptr) return;

    if (((cf::CActorParam*)action)->CActorParam_isBattleLocked() == 0) {
        getInstance__Q22cf13CfGameManagerFv();
        if (!isGlobalCamFlagSet(0x400)) {
            // Broadcast path: target has the 0x8000 flag - poke every party
            // member's 0x118 hook with the shared float constant.
            if (((E1B5C_TargetObj*)targetPtr)->flags_78 & 0x8000) {
                CfEnumListHolder holder;
                startEnumObjects(holder.get(), 0x20, 0);
                for (u32 i = 0; i < holder.get()->mPtrCount;
                     i++) {
                    cf::CActorParam* o = (cf::CActorParam*)MapFx_ResolveOwner(
                        getObjectAt(holder.get(), i));
                    o->CActorParam_setHp(lbl_eu_80666DDC);
                }
                return;
            }
        }
    }

scan2:
    // 2x8 target-slot scan: does any slot reference src->field_04?
    {
        u32 slotId = src->w04;
        bool found = false;
        for (s32 blk = 0; blk < 2 && !found; blk++) {
            u32* base = (u32*)((u8*)src + 8 + blk * 0x20);
            for (s32 j = 0; j < 8; j++) {
                if (base[j] == slotId) {
                    found = true;
                    break;
                }
            }
        }

        if (found) {
            s32 ok = 1;
            if (av->unk64 & 0x2) {
                if (av->unk8C_3 == 7) {
                    if (((E1B5C_MoveFlags*)src)->flags_78 & 0x1000) {
                        if (((cf::CActorParam*)actor)->CActorParam_getStatsSlot() != 1) ok = 0;
                    }
                }
            }
            if (ok) {
                CChain_RegisterCombo((u8*)&((cf::CBattleManager*)mgr)->mChain, actor);
                ChainTmr_CheckArts_D478((u8*)&((cf::CBattleManager*)mgr)->unk20C8, actor);
            }
            SysWinBuff_EmptyStubB24(&((cf::CBattleManager*)mgr)->mSuddenCommu.unk28, actor);
            CCharVoiceMan_HookBattleSlotVoice(actor, action);
            dispatchBattleMove(mgr, actor, src, action);
            if (((cf::CfObjectActor*)action)->unk64 & 0x4) {
            void* h = ((cf::CfObjectActor*)action)->mSubObj38;
((cf::CfObject*)(h))->CfObject_notifyActiveMember();
            }
        }
    }

skipScan:
    getInstance__Q22cf13CfGameManagerFv();
    if (!isGlobalCamFlagSet(0x400)) {
        // Per-target fallback: rebuild a workspace from the snapshot for each
        // of the 16 party-slot ids and run the shared move dispatchers on it.
        E1B5C_Snapshot* snapP = &snap;
        u32* idp = (u32*)((u8*)&snap + 8);
        for (s32 i = 0; i < 16; i++, idp++) {
            if (*idp == 0) continue;
            void* pl = MapFx_ResolveOwner((void*)(intptr_t)findObjectById__Fi((s32)*idp));
            cf::CfObjectActor* pv = (cf::CfObjectActor*)pl;
            if (pl == nullptr || ((cf::CActorParam*)pl)->CActorParam_isBattleLocked() != 0) {
                *idp = 0;
                continue;
            }
            if (pl == (void*)action) continue;

            E1B5C_Workspace ws;
            ws.v00 = copy.w00;
            ws.v04 = copy.w04;
            ws.v08 = (u32)copy.q08;
            ws.v0C = (u32)(copy.q08 >> 32);
            ws.v10 = (u32)copy.q10;
            ws.v14 = (u32)(copy.q10 >> 32);
            ws.v18 = (u32)copy.q18;
            ws.v1C = (u32)(copy.q18 >> 32);
            ws.v20 = (u32)copy.q20;
            ws.v24 = (u32)(copy.q20 >> 32);
            ws.v28 = (u32)copy.q28;
            ws.v2C = (u32)(copy.q28 >> 32);
            ws.v30 = (u32)copy.q30;
            ws.v34 = (u32)(copy.q30 >> 32);
            ws.v38 = (u32)copy.q38;
            ws.v3C = (u32)(copy.q38 >> 32);
            ws.v40 = (u32)copy.q40;
            ws.v44 = (u32)(copy.q40 >> 32);
            ws.v48 = copy.w48;
            ws.v4C = copy.w4C;
            ws.v50 = (void*)(uintptr_t)copy.w50;
            ws.v54 = lbl_eu_80666DDC;
            ws.v58 = lbl_eu_80666DDC;
            ws.v5C = lbl_eu_80666DDC;
            ws.v60 = lbl_eu_80666DDC;
            ws.v64 = lbl_eu_80666DDC;
            ws.v68 = copy.f68;
            ws.v6C = copy.f6C;
            ws.v70 = copy.s70;
            ws.v72 = copy.s72;
            ws.v74 = 0;
            ws.v78 = ((*(u32*)&copy.f6C) & ~0xFFu) | 0x40000001;
            ws.v7C = 0;
            ws.v80 = copy.u80;
            ws.v84 = (u32)copy.q84;
            ws.v88 = (u32)(copy.q84 >> 32);
            ws.v8C = (u32)copy.q8C;
            ws.v90 = (u32)(copy.q8C >> 32);
            ws.v94 = (u32)copy.q94;
            ws.v98 = (u32)(copy.q94 >> 32);
            ws.v9C = (u32)copy.q9C;
            ws.vA0 = (u32)(copy.q9C >> 32);
            ws.vA4 = (u32)copy.qA4;
            ws.vA8 = (u32)(copy.qA4 >> 32);
            ws.vAC = (u32)copy.qAC;
            ws.vB0 = (u32)(copy.qAC >> 32);
            ws.vB4 = copy.wB4;
            ws.vB8 = snapP;                     // +0xB8 = &snapshot

            ws.v04 = pv->field_0x74;       // tag with the actor id

            dispatchBattleAI(mgr, actor, pl, &ws);
            dispatchBattleMove(mgr, actor, pl, &ws);

            // Register the target into the actor list if not present already.
            SimpleListNode* sentinel = *(SimpleListNode**)((u8*)mgr + 8);
            SimpleListNode* cur = sentinel->next;
            while (cur != sentinel && cur->data != pl) cur = cur->next;
            if (cur == sentinel && (pv->unk64 & 0x4) &&
                !(pv->unk3374 & 0x08000000)) {
                registerBattleActor((cf::CBattleManager*)mgr, (cf::CfObjectActor*)pl);
            }
        }
    }

    // Status reset: clear status 0xC0 through the actor's status object when
    // the target is a type-1/2 object with the 0x400 move flag.
    if (isBattleStatusActive((u8*)actor + 8, 0xC0)) {
        if (targetPtr != nullptr) {
            E1B5C_TargetObj* tp = (E1B5C_TargetObj*)targetPtr;
            if ((u32)(tp->type_3c - 1) <= 1) {
                if (((E1B5C_MoveFlags*)src)->flags_78 & 0x400) {
                    ((cf::CBattleState*)((u8*)actor + 8))->CBattleState_clearStatusId(0xC0);
                }
            }
        }
    }

    // Player-actor special case: install an AI action slot for player 0.
    if (av->unk8C_3 == 5) {
        if (av->unk64 & 0x2) {
            void* pcSub = actor != nullptr ? (void*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor) : actor;
            if (pcSub == getPlayer__Q22cf13CfGameManagerFi(0)) {
                if (targetPtr != nullptr) {
                    E1B5C_TargetObj* tp = (E1B5C_TargetObj*)targetPtr;
                    if ((tp->flags_78 & 0x40000000) && tp->id_48 == 9) {
                        BattleAIActionSlot slot;
                        std::memset((u8*)&slot + 4, 0, 0xE);
                        std::memset(&slot, 0, sizeof(slot));
                        slot.unk00 = av->field_0x74;
                        slot.b_06 = 0x25;
                        slot.b_0D = 1;
                        slot.b_0E = 0x64;
                        slot.h_10 = 0;
                        slot.h_12 = 0;
                        slot.f_14 = k5_0f;
                        aiRingPrepend((void*)static_cast<cf::CAIAction*>((cf::CfObjectActor*)actor), &slot);
                    }
                }
            }
        }
    }

    if (av->unk64 & 0x2) {
        if (((cf::CBattleManager*)(mgr))->CheckUnk84Flag(1)) {
            ((cf::CBattleManager*)(mgr))->CBattleMan_ClearStateMask(1);
        }
    }

    // Status-id gate: run testActorStatus against the actor's live status id.
    {
        void* stHolder = *(void**)((u8*)actor + 4);
        void* stPtr = ((cf::CObjectState*)(stHolder))->getStateData();
        u32 stVal = *(u32*)stPtr;
        if (testActorStatus(actor, (int*)&stVal, 0x800)) {
            ((cf::CObjectState*)(stHolder))->clearStateFlags8(0x800);
        }
    }

    cf::CBattleManager* bm = (cf::CBattleManager*)mgr;
    void* vision = &((cf::CBattleManager*)mgr)->mVision;
    if (cf::CBattleManager::spInstance != 0) {
        if (getNonZeroSlot1(vision) != nullptr) {
            ((cf::CVision*)(vision))->func_801A5444((cf::CVisionObjV*)actor, (cf::CVisionFusionV*)action);
        }
    }

    // Re-run the dispatcher when the selected arts level exceeds the actor's.
    if (src->w04 != 0) {
        if (copy.w48 == src->w48) {
            DB4FC_ArtsObj* arts = (DB4FC_ArtsObj*)(void*)src->w50;
            act->CActorParam_pushBattleStats();    // result discarded in retail
            if (arts != nullptr) {
                s32 lv = arts->field_44;
                s32 cur = act->CActorParam_getStatsSlot();
                if (lv > cur) {
                    cf::CActorParam_UnkStruct1* src2 = (cf::CActorParam_UnkStruct1*)act->CActorParam_getBattleStats();
                    dispatchBattleAI(mgr, actor, action, src2);
                }
            }
        }
    }
}

// ================ updateBattleAction reconstruction ================

// Calculates accumulated damage/healing value from various status effects.
// Returns a float clamped to >= 0.


// ---------------------------------------------------------------------------
// Battle event payload built on the stack and handed to func_800EC918.
// 0x34 bytes; fields are filled per call site.
// ---------------------------------------------------------------------------
// ---------------------------------------------------------------------------
// vtable dispatch helpers (r23/r24 are CfObjectActor-derived; every slot
// below goes through the owning-class real virtual directly)
// ---------------------------------------------------------------------------

// (Former e_vf/vf/subVf manual-dispatch helpers removed
// goes through the owning-class real virtual directly at the call site.)


// Arts-data auxiliary dispatch (vtable pointer at sub+0x84, this = sub).
// Slot 0x0C returns the s32 count/index (CAttackParam::getMax shape, cf.
// CtrlAct.cpp - the arts rows share that vtable for the count slot).
// NOTE: this shape (vtable from sub+0x84, this = sub) is not expressible
// as a C++ virtual call (which reads the vtable at this+0), so it stays a
// named manual dispatch; the owner and arity are recovered, not padded.
static inline s32 artsSubGetMax(void* sub) {
    return ((s32(*)(void*))(*(void***)((u8*)sub + 0x84))[0x0C / 4])(sub);
}
// Slot 0x14 sibling (returns f32); same owner/shape caveat as above.
static inline f32 artsSubGetF14(void* sub) {
    return ((f32(*)(void*))(*(void***)((u8*)sub + 0x84))[0x14 / 4])(sub);
}

// status id of an object = *(CObjectState::Func11(unk4-holder)) -- slot 0x30
// on the move sub-object reached through CActorState::unk4.
static inline u32 getStatusId(void* o) {
    void* holder = (void*)((cf::CActorState*)o)->unk4;
    void* p = ((cf::CObjectState*)holder)->getStateData();
    return *(u32*)p;
}
static inline bool hasStatus(void* o, int id) {
    int sid = (int)getStatusId(o);
    return testActorStatus((u8*)o, &sid, id) != 0;
}

// helpers for the small repeated shapes
static void emitEvent(cf::CBattleManager* self, void* attacker, void* target,
                      E2A9C_BattleMoveData* move, const BattleEventData& ev) {
    func_800EC918(self, attacker, target, (void*)&ev, move);
}
static void installSlot(void* obj, u32 unk00, u8 typeByte) {
    BattleAIActionSlot slot;
    std::memset(&slot, 0, sizeof(slot));
    slot.unk00 = unk00;
    slot.b_06 = 0x25;
    slot.b_0D = typeByte;
    slot.b_0E = 0x64;
    slot.h_10 = 0;
    slot.h_12 = -1;
    slot.f_14 = lbl_eu_80666DDC;
    aiRingPrepend((void*)((u8*)obj + 0x3380), (void*)&slot);
}

// retail int->double helper (emits xoris/lfd/fsubs 2^52)
static inline f64 i2d(s32 x) { return (f64)x; }

// round-to-int with the retail +-0.5 double bias, gated by the 0x80 flag
static inline s32 roundF(f32 x, u32 flags) {
    if (x < lbl_eu_80666DDC || (flags & 0x00000080)) return 0;
    return (s32)((f64)x + (x <= lbl_eu_80666DDC ? -0.5 : 0.5));
}

// element sound table at sBattleEventTable + 0x168 (12-byte entries)
static inline u32* elementTable(u32 idx) {
    return (u32*)&sBattleEventTable[0x168 / sizeof(BattleTableEntry) + idx];
}

// distance between the +0x3E9C sub-object positions (retail uses psq_l/ps_sub
// via VEC3Sub, then PSVECMag)
static bool distLE(void* attacker, void* target, s32 range) {
    nw4r::math::VEC3* pT = (nw4r::math::VEC3*)((cf::CfObjectActor*)target)->CfObject_getPosVector();
    nw4r::math::VEC3* pA = (nw4r::math::VEC3*)((cf::CfObjectActor*)attacker)->CfObject_getPosVector();
    nw4r::math::VEC3 d;
    nw4r::math::VEC3Sub(&d, pT, pA);
    return PSVECMag((const struct Vec*)&d) <= (f32)i2d(range);
}

// signed position delta, same PS math
static nw4r::math::VEC3 actorDelta(void* attacker, void* target) {
    nw4r::math::VEC3* pT = (nw4r::math::VEC3*)((cf::CfObjectActor*)target)->CfObject_getPosVector();
    nw4r::math::VEC3* pA = (nw4r::math::VEC3*)((cf::CfObjectActor*)attacker)->CfObject_getPosVector();
    nw4r::math::VEC3 d;
    nw4r::math::VEC3Sub(&d, pT, pA);
    return d;
}

// talent-class bit from byte +off of the talent stat object (retail jump
// tables 0x8052B4DC/0x8052B4B4/0x8052B48C/0x8052B464)
static s32 talentBit(void* tgt, u32 off, s32 talent) {
    u8 b = *(u8*)((u8*)tgt + off);
    switch (talent) {
    case 1: return (b & 0x80) != 0;
    case 2: return (b & 0x01) != 0;
    case 3: return (b & 0x02) != 0;
    case 4: return (b & 0x04) != 0;
    case 5: return (b & 0x08) != 0;
    case 6: return (b & 0x10) != 0;
    case 7: return (b & 0x20) != 0;
    default: return 0;
    }
}

// 0x52A4: case-0x72 inline talent emission (note arg order: self, target,
// attacker, ev, localMove)
static void talentEmitSpecial(cf::CBattleManager* self, void* attacker, void* target,
                              void* entry, E2A9C_BattleMoveData* localMove) {
    BattleEventData ev;
    std::memset(&ev, 0, sizeof(ev));
    ev.field_00 = ((cf::CfObjectActor*)attacker)->field_0x74;
    ev.field_0C = *(u16*)((u8*)entry + 0x16);
    ev.field_10 = (u32)*(s16*)((u8*)entry + 0x18);
    ev.field_20 = k10_0f;   // 10.0
    ev.field_24 = lbl_eu_80666DDC;   // 0.0
    ev.field_30 = 0x20000;
    func_800EC918(self, target, attacker, &ev, localMove);
    bindIndexedEffect(0x42, (int)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)attacker));
    cf::CfSoundMan::playActorSound(0, 0x1B6, 0, 0, 0.6f);
}

// ============================================================================
// Main candidate
// ============================================================================
extern "C" void updateBattleAction(cf::CBattleManager* self, cf::CfObjectActor* attackerIn,
                   cf::CfObjectActor* targetIn, E2A9C_BattleMoveData* move) {
    void* attacker = attackerIn;
    void* target = targetIn;

    // retail prologue initialises the localMove buffer: all zero except
    // field_4C = -1 (0x2E0..0x398 layout, three chunked memsets)
    E2A9C_BattleMoveData localMove;
    std::memset(&localMove, 0, sizeof(localMove));
    localMove.field_4C = (u32)-1;

    if (target == nullptr) return;

    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x400) == 0) {
        if (((cf::CActorParam*)(target))->CActorParam_isBattleLocked() == 0) return;
        if (isBattleStatusActive((u8*)target + 8, 0xF8)) return;
    }
    if (((cf::CfObjectActor*)target)->unk3374 & 0x08000000) return;
    if (*(u32*)((u8*)attacker + 0x3F60) == 0) return;
    if (*(u32*)((u8*)target + 0x3F60) == 0) return;

    void* sub = move->field_50;

    // ---- 0xCE "reaction" block ----
    if (isBattleStatusActive((u8*)target + 8, 0xCE) &&
        (move->field_78 & 0x00000800) && !(move->field_74 & 0x00002000)) {
        cf::CBattleStateEntry* sub2 = (cf::CBattleStateEntry*)findTopBattleStatusEntry((u8*)target + 8, 0xCE);
        if ((u32)sub2->unk10 >= (u32)artsSubGetMax(sub)) {
            move->field_54 = lbl_eu_80666DD4;
            move->field_5C = lbl_eu_80666DD4;
            move->field_74 = (move->field_74 | 0x80002000) & ~0x00030000;
        }
    }

    // ---- 0xEB "knockback" block ----
    if (isBattleStatusActive((u8*)target + 8, 0xEB) && !(move->field_74 & 0x2)) {
        u16 type = *(u16*)((u8*)sub + 0x3C);
        if (type == 1 || type == 5) {
            move->field_74 &= ~0x00030000;
            if (!(move->field_78 & 0x00000800)) {
                move->field_74 |= 0x80000002;
                move->field_54 = move->field_54 * 0.5f;
                move->field_5C = move->field_54 * move->field_58;
            }
        }
    }

    if (move->field_74 & 0x1) {
        // ====================================================================
        // PATH A -- standard attack (0x3820..0x6360)
        // ====================================================================
        f32 f26 = lbl_eu_80666DD4;   // global ratio
        f32 f27 = lbl_eu_80666DD4;   // combo ratio (retail loads 1.0 only inside
                                     // the 0x106 block; value is register-garbage
                                     // otherwise -- declared here for safety)

        if (isBattleStatusActive((u8*)target + 8, 0xF7)) {
            if ((u32)((cf::CfObjectActor*)(target))->CfObjectActor_sharesMoveFlags((cf::CfObjectActor*)attacker) == 0) {
                findBattleStatusEntry((u8*)target + 8, 0xF7);
                u32 f = move->field_78;
                if ((f & 0x00000200) || ((f & 0x00000400) && !(f & 0x00000800))) {
                    move->field_74 |= 0x80000080;
                    target = attacker;                 // redirect onto attacker
                } else {
                    move->field_74 &= ~0x00000080;
                }
            } else {
                move->field_74 &= ~0x00000080;
            }
        }

        if (isBattleStatusActive((u8*)target + 8, 0x106)) {
            findBattleStatusEntry((u8*)target + 8, 0x106);
            if (!(move->field_78 & 0x00000800)) {
                if (move->field_50 == nullptr) return;

                f27 = lbl_eu_80666DD4;
                s32 a = artsSubGetMax(move->field_50);
                s32 b = artsSubGetMax(move->field_50);
                s32 rem = (s32)(move->field_B0 % 100);
                // retail: pool = {8.0,4.0,2.0} (dwords at +0x2A4 read as f32),
                // thresholds = {a/2+3, b+10, 100} (ints at +0x298, 0/1 patched)
                f32 pool[3] = { 8.0f, 4.0f, 2.0f };
                s32 thr[3] = { a / 2 + 3, b + 10, 100 };
                for (int i = 0; i < 3; i++) {
                    if (rem - thr[i] < 0) { f27 = pool[i]; break; }
                    rem -= thr[i];
                }

                if ((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable()) {
                    u32 tmp;
                    if (IdTable_GetSum((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable(), 0x5B, &tmp)) {
                        f27 = f27 + 0.01f * (f32)i2d((s32)tmp);
                    }
                }
            }
        }

        // 0x3A18..0x3B08: r3 = round(field_5C + field_60); if <= 0 use field_64
        s32 roundedValue = roundF(move->field_5C + move->field_60, move->field_74);
        if (roundedValue <= 0) roundedValue = roundF(move->field_64, move->field_74);

        // self-inflicted HP adjust (0x3B08)
        s32 amt;
        {
            f32 d = (f32)i2d(roundedValue) * f27;
            amt = (s32)((f64)d + (d > lbl_eu_80666DDC ? 0.5 : -0.5));
            ((cf::CActorParam*)(attacker))->CActorParam_applyDamage((float)((f64)(-amt)), (int)(2), (int)(0), (int)(0));
        }
        CCharVoiceMan_EnqueueDamageVoice(attacker, target, (void*)amt);
        CCharVoiceMan_ClearPlayerVoiceCount(target);
SysWinLog_PollSequenceProgress((CScenarioLogOwner*)target, (s32)(move->field_5C * f27), (CScenarioLogArg2*)move);
        ((cf::CActorParam*)(attacker))->CActorParam_applyEventStatus((cf::CActorParam12Arg*)(move));

        // 0x3BF4
        if (!(move->field_78 & 0x00000200)) {
            if (move->field_50 == nullptr) return;
            u32 subFlags = *(u32*)((u8*)sub + 0x78);
            if (!(subFlags & 0x00002000) ||
                (u32)(((cf::CActorParam*)(attacker))->CActorParam_getStatsSlot() + 1) != *(u8*)((u8*)sub + 0x44)) {
                processBattleMove(self, attacker, target, move);
            }
        }

        {
            f32 d = move->field_5C * f26;
            s32 appliedDamage = (s32)((f64)d + (d > lbl_eu_80666DDC ? 0.5 : -0.5));
            if (move->field_50 == nullptr) return;
            if ((move->field_74 & 0x2) || (move->field_74 & 0x60)) CCharVoiceMan_HookPendingBattleVoice(attacker);

            // ---- 0x3CC8: per-tag attack parameter emission ----
            if (move->field_50 == nullptr) return;          // 0x3CB0
            if (move->field_78 & 0x00000200) {
                s32 a0Bonus = 0;
                if (isBattleStatusActive((u8*)attacker + 8, 0xA0)) {
                    a0Bonus = *(s32*)((u8*)findBattleStatusEntry((u8*)attacker + 8, 0xA0) + 0x10);
                }

#define EMIT_TAG(tag, evType, extraField)                                        \
    if (isBattleStatusActive((u8*)attacker + 8, tag)) {                                \
        cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)attacker + 8, tag);                    \
        s32 val = entry->unk1A;                                   \
        if (entry->unk08 == 0x4000) val += a0Bonus;                   \
        if (move->field_A8 < (u32)val) {                                        \
            BattleEventData ev;                                                 \
            std::memset(&ev, 0, sizeof(ev));                                    \
            ev.field_00 = ((cf::CfObjectActor*)attacker)->field_0x74;                      \
            ev.field_08 = 0x18000;                                              \
            ev.field_0C = evType;                                               \
            ev.field_10 = (u32)entry->unk10;                           \
            ev.field_20 = k10_0f;                                      \
            extraField;                                                         \
            emitEvent(self, attacker, target, move, ev);                        \
        }                                                                       \
    }

                EMIT_TAG(0xA3, 0x52, (void)0)
                EMIT_TAG(0xA4, 0x53, (void)0)
                EMIT_TAG(0xA5, 0x54, (void)0)
                EMIT_TAG(0xAC, 0x3C, (void)0)
                EMIT_TAG(0xAD, 0x3D, (void)0)
                EMIT_TAG(0x7E, 0x66, ev.field_24 = k2_0f;)
                EMIT_TAG(0x7F, 0x67, ev.field_24 = k2_0f;)
                EMIT_TAG(0x80, 0x68, ev.field_24 = k2_0f;)
                EMIT_TAG(0x81, 0x69, ev.field_24 = k2_0f;)

                // 0x82 -- unconditional, 0.35/0.35
                if (isBattleStatusActive((u8*)attacker + 8, 0x82)) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)attacker + 8, 0x82);
                    BattleEventData ev;
                    std::memset(&ev, 0, sizeof(ev));
                    ev.field_00 = ((cf::CfObjectActor*)attacker)->field_0x74;
                    ev.field_08 = 0x18000;
                    ev.field_0C = 0x6A;
                    ev.field_10 = (u32)entry->unk10;
                    ev.field_20 = 0.35f;
                    ev.field_24 = 0.35f;
                    emitEvent(self, attacker, target, move, ev);
                }

                // 0xA8 -- extra u16 at +0x14
                if (isBattleStatusActive((u8*)attacker + 8, 0xA8)) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)attacker + 8, 0xA8);
                    s32 val = entry->unk1A;
                    if (entry->unk08 == 0x4000) val += a0Bonus;
                    if (move->field_A8 < (u32)val) {
                        BattleEventData ev;
                        std::memset(&ev, 0, sizeof(ev));
                        ev.field_00 = ((cf::CfObjectActor*)attacker)->field_0x74;
                        ev.field_08 = 0x18000;
                        ev.field_0C = 0xDF;
                        ev.field_10 = (u32)entry->unk10;
                        ev.field_14 = (u16)(u32)entry->unk10;
                        ev.field_20 = k10_0f;
                        emitEvent(self, attacker, target, move, ev);
                    }
                }

                // 0xA9 -- constant value 0x4B, type 7
                if (isBattleStatusActive((u8*)attacker + 8, 0xA9)) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)attacker + 8, 0xA9);
                    s32 val = entry->unk1A;
                    if (entry->unk08 == 0x4000) val += a0Bonus;
                    if (move->field_A8 < (u32)val) {
                        BattleEventData ev;
                        std::memset(&ev, 0, sizeof(ev));
                        ev.field_00 = ((cf::CfObjectActor*)attacker)->field_0x74;
                        ev.field_08 = 0x18000;
                        ev.field_0C = 0x07;
                        ev.field_10 = 0x4B;
                        ev.field_20 = k10_0f;
                        emitEvent(self, attacker, target, move, ev);
                    }
                }

                EMIT_TAG(0xAA, 0x12, (void)0)

                // 0xA6 -- constant value 0x32, type 4
                if (isBattleStatusActive((u8*)attacker + 8, 0xA6)) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)attacker + 8, 0xA6);
                    s32 val = entry->unk1A;
                    if (entry->unk08 == 0x4000) val += a0Bonus;
                    if (move->field_A8 < (u32)val) {
                        BattleEventData ev;
                        std::memset(&ev, 0, sizeof(ev));
                        ev.field_00 = ((cf::CfObjectActor*)attacker)->field_0x74;
                        ev.field_08 = 0x18000;
                        ev.field_0C = 0x04;
                        ev.field_10 = 0x32;
                        ev.field_20 = k10_0f;
                        emitEvent(self, attacker, target, move, ev);
                    }
                }

                EMIT_TAG(0xA7, 0x06, (void)0)

#undef EMIT_TAG

                // 0xCF / 0xD0 "warp" event -- only when !(field_74 & 0x40)
                if (!(move->field_74 & 0x40)) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)attacker + 8, 0xCF);
                    if (entry == nullptr) entry = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)attacker + 8, 0xD0);
                    if (entry != nullptr) {
                        u32 st = ((cf::CActorParam*)(target))->CActorParam_getActorType();
                        if (st == 1 || st == 2) {
                            BattleEventData ev;
                            std::memset(&ev, 0, sizeof(ev));
                            ev.field_00 = ((cf::CfObjectActor*)attacker)->field_0x74;
                            ev.field_0C = 0x7D;
                            ev.field_10 = (u32)entry->unk10;
                            ev.field_20 = 0.35f;
                            ev.field_24 = 0.35f;
                            emitEvent(self, attacker, target, move, ev);
                        }
                    }
                }

                // 0x6D event -- requires move->field_B0 < out2
                if ((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable()) {
                    u32 out1, out2;
                    if (IdTable_GetSumMax((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable(), 0x6D, &out1, &out2)) {
                        if (move->field_B0 < out2) {
                            BattleEventData ev;
                            std::memset(&ev, 0, sizeof(ev));
                            ev.field_00 = ((cf::CfObjectActor*)attacker)->field_0x74;
                            ev.field_0C = 0xE2;
                            ev.field_10 = out1;
                            emitEvent(self, attacker, target, move, ev);
                        }
                    }
                }
            }

            // ================================================================
            // 0x474C: local copy + player index + HP application
            // ================================================================
            localMove = *move;
            s32 attackerKind = 0;

            if (((cf::CfObjectActor*)attacker)->unk64 & 0x2) {
                void* sub2 = (attacker != nullptr) ? (u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)attacker) : attacker;
                if (sub2 != cf::CfGameManager::getPlayer(0)) {
                    CfEnumListHolder holder;
                    cf::CfObjEnumList* list = holder.get();
                    startEnumObjects(list, 0x20, 0);
                    list = holder.get();
                    if (list->mPtrCount == 2) {
                        attackerKind = 1;
                    } else {
                        int* table = CtrlObjectParam_GetSlotTableBase();
                        u16 id = ((cf::CfObjectActor*)attacker)->unk8C_3;
                        if (id != (u32)table[2]) attackerKind = 1;
                        else if (id != (u32)table[3]) attackerKind = 2;
                    }
                }
            }

            u32 mvType = 0;
            if (move->field_74 & 0x00001000) mvType = 7;
            else if (move->field_74 & 0x2) mvType = 5;
            else if (move->field_74 & 0x10) mvType = 6;
            else if (move->field_74 & 0x00000800) mvType = 3;

            ((cf::CActorParam*)(target))->CActorParam_applyDamage((float)((f64)(-appliedDamage)), (int)(mvType), (int)((u32)attackerKind), (int)(move->field_74));
            CCharVoiceMan_EnqueueDamageVoice(target, attacker, (void*)(-appliedDamage));
            CCharVoiceMan_ClearPlayerVoiceCount(attacker);
SysWinLog_PollSequenceProgress((CScenarioLogOwner*)attacker, appliedDamage, (CScenarioLogArg2*)move);
            if (move->field_50 == nullptr) return;

            if (appliedDamage >= 0) SuddenCommuDispatchAttacker(&self->mSuddenCommu, attacker, target, &localMove);

            // ---- 0x4A44: element-type sound selection ----
            {
                void* elem = getLiveParamPtr((u8*)(*(u32*)((u8*)attacker + 0x3F60)) + 0x10);
                if (elem != nullptr) {
                    void* who = (*(u8*)((u8*)elem + 0xC) & 0x80) ? target : attacker;
                    u8 b8 = *(u8*)((u8*)getLiveParamPtr((u8*)(*(u32*)((u8*)attacker + 0x3F60)) + 0x10) + 0x08);
                    u16 hA = *(u16*)((u8*)getLiveParamPtr((u8*)(*(u32*)((u8*)attacker + 0x3F60)) + 0x10) + 0x0A);
                    attachEffectById(who, attacker, hA, b8);
                } else {
                    u32 sf = *(u32*)((u8*)sub + 0x78);
                    u32* tbl = elementTable(*(u16*)((u8*)sub + 0x40));
                    if (sf & 0x100) {
                        attachEffectById(target, attacker, tbl[1], 0);
                    } else if (sf & 0x200) {
                        attachEffectById(target, attacker, tbl[2], 0);
                    } else {
                        attachEffectById(target, attacker, tbl[0], 0);
                    }
                }
            }

            // ---- 0x4B30: r14 guard (whole 0x4B50..0x5090 chain runs only
            // when isGlobalCamFlagSet(0x400) == 0; otherwise only e_vfC4 runs) ----
            bool flag_3C8 = false;                          // 0x3C8 stack byte
            if (isGlobalCamFlagSet(0x400) == 0) {
                bool isSelfTarget = true;
                if ((u32)((cf::CfObjectActor*)target)->CObjectParam_getSelfObjectId() != ((cf::CfObjectActor*)attacker)->field_0x74) {
                    isSelfTarget = false;
                }
                int sid = (int)getStatusId(target);
                if (testActorStatus((u8*)target, &sid, 0x802)) isSelfTarget = false;
                sid = (int)getStatusId(target);
                if (testActorStatus((u8*)target, &sid, 0x801)) {
                    isSelfTarget = false;
                } else {
                    if (isBattleStatusActive((u8*)target + 8, 0x7) ||
                        isBattleStatusActive((u8*)target + 8, 0xB) ||
                        isBattleStatusActive((u8*)target + 8, 0x9) ||
                        isBattleStatusActive((u8*)target + 8, 0xF) ||
                        isBattleStatusActive((u8*)target + 8, 0x10)) {
                        isSelfTarget = false;
                    }
                }

                // ---- 0x4C48 / 0x4CC0 ----
                if (((cf::CActorParam*)(target))->CActorParam_isBattleLocked() != 0) {
                    if (!(move->field_78 & 0x1)) dispatchItemLoad(attacker, 0x400000);
                    if (((cf::CBattleManager*)self)->mChain.mHead.field_2 >= 1 && ((cf::CBattleManager*)self)->mChain.mHead.field_2 <= 0x18) {
                        cf::PartyGaugeAddClamped(reinterpret_cast<cf::UnkClass_8018C5FC*>(&self->unk194), 0x64);
                    }
SysWinLog_DrivePair((CScenarioLogOwner*)target, (CScenarioLogOwner*)attacker);
                    isSelfTarget = false;
                } else {
                    if ((localMove.field_74 & 0x100) && (((cf::CfObjectActor*)target)->unk64 & 0x2)) {
                        func_800F3970((void*)cf::CBattleManager::spInstance, target, 0, 0x16, 0);
                    }
                }

                // ---- 0x4D00: AI slot install (target side) ----
                // retail stores the 0x3C8 flag = (((cf::CActorParam*)(target))->CActorParam_getGradeByte() < field_A0) here,
                // used later at 0x5B50.
                if (isSelfTarget && (localMove.field_78 & 0x200)) {
                    if (((cf::CActorParam*)(target))->CActorParam_getGradeByte() < localMove.field_A0) {
                        flag_3C8 = true;
                        installSlot(target, ((cf::CfObjectActor*)attacker)->field_0x74, 0x54);
                    }
                }

                // ---- 0x4DAC: attacker status / chain events ----
                {
                    int sid2 = (int)getStatusId(attacker);
                    if (testActorStatus((u8*)attacker, &sid2, 0x800)) {
                        if (localMove.field_74 & 0x02000000) {
                            if (self->CheckUnk84Flag(1) != 0) {
                                func_800F3970((void*)cf::CBattleManager::spInstance, attacker, target, 2, 0);
                            }
                        }
                    }
                    if (localMove.field_74 & 0x100) {
                        if (((cf::CfObjectActor*)attacker)->unk64 & 0x2) {
                            u32 v = 0;
                            if ((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable()) IdTable_GetSum((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable(), 0x3A, &v);
                            func_800F3970((void*)cf::CBattleManager::spInstance, attacker, target, 3, (s32)v);
                            cf::PartyGaugeAddClamped(reinterpret_cast<cf::UnkClass_8018C5FC*>(&self->unk194), 0xA);
                        }
                    }
                    if (((cf::CfObjectActor*)attacker)->unk64 & 0x2) {
                        if ((localMove.field_74 & 0x2) || (localMove.field_74 & 0x40)) {
                            if ((u32)((cf::CActorParam*)(attacker))->CActorParam_getStatusCount() == 4) cf::PartyGaugeAddClamped(reinterpret_cast<cf::UnkClass_8018C5FC*>(&self->unk194), 0xA);
                        }
                    }
                    if (localMove.field_78 & 0xC00) {
                        if (isBattleStatusActive((u8*)target + 8, 0x10)) {
                            func_800F3970((void*)cf::CBattleManager::spInstance, attacker, target, 4, 0);
                        }
                        if (isBattleStatusActive((u8*)target + 8, 0xF) ||
                            isBattleStatusActive((u8*)target + 8, 0x9)) {
                            func_800F3970((void*)cf::CBattleManager::spInstance, attacker, target, 5, 0);
                        }
                    }
                }

                // ---- 0x4F84 ----
                {
                    u32 v = 0;
                    if ((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable()) IdTable_GetSum((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable(), 0x74, &v);
                    if (localMove.field_74 & 0x18) {
                        func_800F3970((void*)cf::CBattleManager::spInstance, attacker, 0, 0xD, (s32)v);
                    }
                    if (localMove.field_74 & 0x2) {
                        func_800F3970((void*)cf::CBattleManager::spInstance, attacker, 0, 0xE, (s32)v);
                    }
                    if (isBattleStatusActive((u8*)target + 8, 0xF)) {
                        cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)attacker + 8, 0xBD);
                        if (e != nullptr) ((cf::CActorParam*)(attacker))->CActorParam_accumulateTension((int)((u32)e->unk10));
                    }
                }

                ((cf::CActorParam*)(attacker))->CActorParam_applyStatusHeal((cf::CActorParam11Arg*)(&localMove));
                ((cf::CActorParam*)(target))->CActorParam_applyEventStatus((cf::CActorParam12Arg*)(&localMove));
            } else {
                ((cf::CActorParam*)(target))->CActorParam_applyEventStatus((cf::CActorParam12Arg*)(&localMove));                 // 0x5094
            }

            // ---- 0x50AC: talent-art gate ----
            bool runTalent = true;
            if (isBattleStatusActive((u8*)target + 8, 0x10)) {
                if (!(((cf::CfObjectActor*)target)->unk3374 & 0x00040000)) {
                    runTalent = false;                 // -> 0x5A00
                } else {
                    int sid = (int)getStatusId(target);
                    if (testActorStatus((u8*)target, &sid, 0x17)) runTalent = false;
                }
            }
            if (runTalent) {
                if (isBattleStatusActive((u8*)target + 8, 0x117)) runTalent = false;
                if (localMove.field_74 & 0x80) runTalent = false;
            }

            if (runTalent) {
                // ============================================================
                // 0x5120..0x59F4: talent/skill loop
                // ============================================================
                for (u32 idx = 0x40; idx < 0x68; idx++) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)static_cast<cf::CBattleState*>((cf::CfObjectActor*)target)->CBattleState_getEntryByIndex((int)(idx));
                    u32 type = entry->unk0C;
                    s32 talent = 0;

                    switch (type) {
                    case 0x6E:
                    case 0x78:
                        talent = classifyBattleCommand(0x6E);
                        break;
                    case 0x6F:
                    case 0x79:
                        talent = classifyBattleCommand(0x6F);
                        break;
                    case 0x70:
                    case 0x7A:
                        talent = classifyBattleCommand(0x70);
                        break;
                    case 0x71:
                    case 0x7B:
                        talent = classifyBattleCommand(0x71);
                        break;
                    case 0x72:
                    case 0x7C:
                        if (((cf::CBattleManager*)self)->mChain.mHead.field_2 >= 1 &&
                            ((cf::CBattleManager*)self)->mChain.mHead.field_2 <= 0x18) {
                            break;                     // r25 stays 0
                        }
                        if (entry->unk14 == 0) {
                            talentEmitSpecial(self, attacker, target, entry, &localMove);
                            break;                     // 0x52A4
                        }
                        if (distLE(attacker, target, entry->unk14)) {
                            talentEmitSpecial(self, attacker, target, entry, &localMove);
                            break;                     // 0x52A4
                        }
                        break;
                    default:
                        break;
                    }

                    // 0x532C..0x53CC: range re-check for r25 from 0x6E..0x71
                    if (talent != 0) {
                        if (entry->unk14 != 0) {
                            if (!distLE(attacker, target, entry->unk14)) {
                                talent = 0;
                            }
                        }
                        if (talent == 0) continue;
                    } else {
                        continue;
                    }

                    // ---- main emission 0x53D4 ----
                    {
                        void* tgt = ((cf::CActorParam*)(attacker))->CActorParam_getBattleParams();   // talent stat object
                        s32 value = (s32)entry->unk10;
                        f32 ratio = lbl_eu_80666DD4;

                        if ((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable()) {
                            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)target + 8, 0x6F);
                            if (e == nullptr) e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)target + 8, 0x79);
                            if (e != nullptr) {
                                u32 tmp;
                                if ((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable() && IdTable_GetSum((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable(), 0x5B, &tmp)) {
                                    value = (s32)((f32)i2d(value) *
                                        (1.0f + (f32)i2d((s32)tmp) / 100.0f));
                                }
                            }
                        }
                        if (isBattleStatusActive((u8*)attacker + 8, 0x8D)) {
                            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)attacker + 8, 0x8D);
                            ratio = ratio - 0.01f * (f32)i2d(e->unk10);
                        }
                        if ((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable()) {
                            u32 tmp;
                            if (IdTable_GetSum((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable(), 0x57, &tmp)) {
                                ratio = ratio - 0.01f * (f32)i2d((s32)tmp);
                            }
                        }
                        value = (s32)((f32)i2d(value) * ratio);
                        if (value < 1) value = 0;
                        if (isBattleStatusActive((u8*)attacker + 8, 0x1)) value = 0;

                        // talent classification A (byte +0x72 / s16 +0x64..)
                        s32 r0 = talentBit(tgt, 0x72, talent);
                        if (talent >= 4 && talent <= 9) {
                            if (*(s16*)((u8*)tgt + 0x64 + (talent - 4) * 2) >= 0xC8) r0 = 1;
                        }
                        if (r0 != 0) {
                            value = -value;
                        } else {
                            r0 = talentBit(tgt, 0x70, talent);
                            if (talent >= 4 && talent <= 9) {
                                if (*(s16*)((u8*)tgt + 0x64 + (talent - 4) * 2) >= 0x64) r0 = 1;
                            }
                            if (r0 != 0) {
                                value = (s32)((f32)i2d(value) * 0.5f);
                            }
                        }
                        if (value != 0) {
                            ((cf::CActorParam*)(attacker))->CActorParam_applyDamage((float)((f64)(-value)), (int)(2), (int)(0), (int)(0));
                        }
                        // hit SFX
                        s32 hitSnd = 0;
                        switch (talent) {
                        case 4: hitSnd = 0x42; break;
                        case 5: hitSnd = 0x4C; break;
                        case 6: hitSnd = 0x44; break;
                        case 7: hitSnd = 0x46; break;
                        case 8: hitSnd = 0x4A; break;
                        case 9: hitSnd = 0x48; break;
                        default: hitSnd = 0; break;
                        }
                        bindIndexedEffect((u32)hitSnd,
                            (int)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)attacker));
                        u32 snd = 0;
                        switch (talent) {
                        case 1: snd = 0x1AD; break;
                        case 2: snd = 0x1AA; break;
                        case 3: snd = 0x1B0; break;
                        case 4: snd = 0x1B6; break;
                        case 5: snd = 0x1BA; break;
                        case 6: snd = 0x1B5; break;
                        case 7: snd = 0x1B7; break;
                        case 8: snd = 0x1B8; break;
                        case 9: snd = 0x1B9; break;
                        default: snd = 0; break;
                        }
                        cf::CfSoundMan::playActorSound(0, snd, 0, 0, 0.6f);
                    }
                }
            }

            // ---- 0x5A00 ----
            if (((cf::CActorParam*)(target))->CActorParam_isBattleLocked() != 0) {
                CCharVoiceMan_PlayPairVoice(attacker, target);
                CCharVoiceMan_TriggerBattleOrPlainVoice(target);
            } else {
                bool gaugeVoice = true, breakVoice = true;
                if (!(localMove.field_74 & 0x100)) {
                    int sid = (int)getStatusId(target);
                    if (testActorStatus((u8*)target, &sid, 0x19) == 0) breakVoice = false;
                }
                if (!breakVoice) {
                    int sid = (int)getStatusId(target);
                    if (testActorStatus((u8*)target, &sid, 0x1A) == 0) gaugeVoice = false;
                }
                CCharVoiceMan_EnqueueBreakAndGaugeVoice(target, attacker, gaugeVoice ? 1 : 0);
            }

            applyBattleDamage((cf::CfObjectPc*)attacker, (cf::CfObjectEne*)target);

            bool hasStatus806 = false;
            if ((((cf::CfObjectActor*)target)->unk64 & 0x4) &&
                (((cf::CfObjectActor*)target)->unk8C_3 >= 0x9C5 &&
                 ((cf::CfObjectActor*)target)->unk8C_3 <= 0x9C7)) {
                int sid = (int)getStatusId(target);
                if (testActorStatus((u8*)target, &sid, 0x806)) hasStatus806 = true;
            }

            // ---- 0x5B50: chain/status branch ----
            if (flag_3C8 || hasStatus806 || hasStatus(target, 0x16) || hasStatus(target, 0x18) ||
                hasStatus(target, 0x19) || hasStatus(target, 0x1A) ||
                hasStatus(target, 0x1B)) {
                // -> 0x6360
            } else if (localMove.field_74 & 0x2) {
                if (!hasStatus(target, 0xA) && !hasStatus(target, 0xB)) {
                    CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)attacker), 0x2D, 0, -1, 1);
                    if (CChain_HasMemberEntry(&self->mChain, (CChainBattleObjTail*)target) == 0) {
                        CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target), 0x2E, 0, -1, 1);
                    }
                }
            } else if (localMove.field_74 & 0x60) {
                if (!hasStatus(target, 0xA) && !hasStatus(target, 0xB)) {
                    CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)attacker), 0x2D, 0, -1, 1);
                    if (localMove.field_74 & 0x40) {
                        bindIndexedEffect(0xB0, (int)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target));
                    }
                }
            } else {
                if (!hasStatus(target, 0x1000) && !hasStatus(target, 0x7) &&
                    !hasStatus(target, 0x8) && !hasStatus(target, 0x4)) {
                    if (localMove.field_74 & 0x10000) {
                        // knockback
                        if (!hasStatus(target, 0xA) && !hasStatus(target, 0xB)) {
                            void* st = (void*)((cf::CfObjectActor*)target)->mTargetC4;
                            if (!(*(u16*)((u8*)st + 0x530) & 0x1)) {
                                ((cf::CfObjectActor*)target)->CfObject_setMoveHeadAngle(ml::pi + ((cf::CfObjectActor*)(attacker))->CfObject_getMoveHeadAngle());
                            }
                            nw4r::math::VEC3 d = actorDelta(attacker, target);
                            // retail: horizontal dist^2 = x^2+z^2 only (y stored, unused)
                            f32 dist2 = d.x * d.x + d.z * d.z;
                            void* st2 = (void*)((cf::CfObjectActor*)target)->mTargetC4;
                            if (dist2 <= 0.010000001f) {
                                *(f32*)((u8*)st2 + 0x46C) = ((cf::CfObjectActor*)(attacker))->CfObject_getMoveHeadAngle();
                            } else {
                                *(f32*)((u8*)st2 + 0x46C) =
                                    0.024543693f * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
                            }
                            if (((cf::CBattleManager*)self)->mChain.mHead.field_2 < 1 || ((cf::CBattleManager*)self)->mChain.mHead.field_2 > 0x18) {
                                *(f32*)((u8*)st2 + 0x468) = getArtsFacing(sub);
                            } else {
                                *(f32*)((u8*)st2 + 0x468) = lbl_eu_80666DDC;
                            }
                            CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target), 0x1D, 0, -1, 1);
                        }
                    } else if (localMove.field_74 & 0x20000) {
                        // ether hit
                        if (!hasStatus(target, 0xA) && !hasStatus(target, 0xB)) {
                            void* st = (void*)((cf::CfObjectActor*)target)->mTargetC4;
                            if (!(*(u16*)((u8*)st + 0x530) & 0x1)) {
                                ((cf::CfObjectActor*)target)->CfObject_setMoveHeadAngle(ml::pi + ((cf::CfObjectActor*)(attacker))->CfObject_getMoveHeadAngle());
                            }
                            nw4r::math::VEC3 d = actorDelta(attacker, target);
                            f32 dist2 = d.x * d.x + d.z * d.z;   // horizontal only
                            void* st2 = (void*)((cf::CfObjectActor*)target)->mTargetC4;
                            if (dist2 <= 0.010000001f) {
                                *(f32*)((u8*)st2 + 0x478) = ((cf::CfObjectActor*)(attacker))->CfObject_getMoveHeadAngle();
                            } else {
                                *(f32*)((u8*)st2 + 0x478) =
                                    0.024543693f * Atan2FIdx__Q24nw4r4mathFff(d.x, d.z);
                            }
                            if (((cf::CBattleManager*)self)->mChain.mHead.field_2 < 1 || ((cf::CBattleManager*)self)->mChain.mHead.field_2 > 0x18) {
                                *(f32*)((u8*)st2 + 0x470) = getArtsClampedRate(sub);
*(f32*)((u8*)st2 + 0x474) = getArtsScaledSpeed((const u8*)sub);
                            } else {
                                *(f32*)((u8*)st2 + 0x470) = lbl_eu_80666DDC;
f32 c0 = getArtsScaledSpeed((const u8*)sub);
                                *(f32*)((u8*)st2 + 0x474) = c0 < lbl_eu_80666DDC ? lbl_eu_80666DDC : c0;
                            }
                            f32 f134 = getArtsGaugeRate(sub);
                            if (f134 != lbl_eu_80666DDC) {
                                BattleEventData ev;
                                std::memset(&ev, 0, sizeof(ev));
                                ev.field_00 = ((cf::CfObjectActor*)attacker)->field_0x74;
                                ev.field_04 = (u32)sub;
                                ev.field_0C = 0xF;
                                ev.field_2E = 0xF;
                                if (emitBattleMoveEvents(self, attacker, target, &ev, &localMove) != 0) {
                                    ((cf::CActorParam*)target)->CActorParam_setGaugeRateB(f134);
                                    ((cf::CActorParam*)target)->CActorParam_addSecondGauge(1);
                                }
                            }
                            CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target), 0x1E, 0, -1, 1);
                        }
                    } else {
                        if (!hasStatus(target, 0x800) && !hasStatus(target, 0x1D) &&
                            !hasStatus(target, 0x801)) {
                            if (appliedDamage > 0) {
                                CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target), 0x4, 0, -1, 1);
                            }
                        }
                    }
                }
            }

            // ---- 0x6360 ----
            if (((cf::CActorParam*)(target))->CActorParam_isBattleLocked() != 0) {
                spreadChainArtsHits(self, target, attacker, &localMove);
                unregisterBattleActor(self, target);
            }
        }
    } else {
        // ====================================================================
        // PATH B -- field_74 bits 2/3 (0x63A0..0x6C6C)
        // ====================================================================
        if (move->field_74 & 0xC) {
            localMove = *move;
            s32 attackerKind = 0;

            if (((cf::CfObjectActor*)attacker)->unk64 & 0x2) {
                void* sub2 = (attacker != nullptr) ? (u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)attacker) : attacker;
                if (sub2 != cf::CfGameManager::getPlayer(0)) {
                    CfEnumListHolder holder;
                    cf::CfObjEnumList* list = holder.get();
                    startEnumObjects(list, 0x20, 0);
                    list = holder.get();
                    if (list->mPtrCount == 2) {
                        attackerKind = 1;
                    } else {
                        int* table = CtrlObjectParam_GetSlotTableBase();
                        u16 id = ((cf::CfObjectActor*)attacker)->unk8C_3;
                        if (id != (u32)table[2]) attackerKind = 1;
                        else if (id != (u32)table[3]) attackerKind = 2;
                    }
                }
            }

            BtlDmg_SetField779(0);
            BtlDmg_SetByte778(0);
            BtlDmg_SetDamageType((move->field_74 & 0x8) ? 8 : 4);
            BtlDmg_SetDamageDir(attackerKind);
            BtlDmg_NotifyDamage((void*)((cf::CfObjectActor*)target)->field_0x74, 0);

            {
                u32 v = 0;
                if ((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable()) IdTable_GetSum((void*)((cf::CActorParam*)(attacker))->CActorParam_getStatusTable(), 0x74, &v);
                func_800F3970((void*)cf::CBattleManager::spInstance, attacker, 0, 0xC, (s32)v);
            }
            SuddenCommuDispatchPartner(&self->mSuddenCommu, (CSuddenCommuActor*)attacker, (CSuddenCommuActor*)target);

            bool idMatch = true;
            if ((u32)((cf::CfObjectActor*)target)->CObjectParam_getSelfObjectId() != ((cf::CfObjectActor*)attacker)->field_0x74) {
                idMatch = false;
            }
            {
                int sid = (int)getStatusId(target);
                if (testActorStatus((u8*)target, &sid, 0x801) ||
                    isBattleStatusActive((u8*)target + 8, 0x7) ||
                    isBattleStatusActive((u8*)target + 8, 0xB) ||
                    isBattleStatusActive((u8*)target + 8, 0x9) ||
                    isBattleStatusActive((u8*)target + 8, 0xF) ||
                    isBattleStatusActive((u8*)target + 8, 0x10)) {
                    idMatch = false;
                }
            }
            if (idMatch && (localMove.field_78 & 0x200)) {
                if (((cf::CActorParam*)(target))->CActorParam_getGradeByte() < localMove.field_A0) {
                    installSlot(target, ((cf::CfObjectActor*)attacker)->field_0x74, 0x54);
                }
            }

            if (!hasStatus(target, 0x801) && !hasStatus(target, 0x16) &&
                !hasStatus(target, 0x18) && !hasStatus(target, 0x19) &&
                !hasStatus(target, 0x1A) && !hasStatus(target, 0x1B)) {
                if (CChain_HasMemberEntry(&self->mChain, (CChainBattleObjTail*)target) == 0) {
                    s32 chainSlot = 0;
                    if (localMove.field_78 & 0x200) {
                        chainSlot = *(u8*)((u8*)sub + 0x77);
                        if (chainSlot >= 3) chainSlot -= 3;
                    } else if (localMove.field_78 & 0x400) {
                        if (rand() & 1) chainSlot = 1;
                    }
                    *(s32*)((u8*)((cf::CfObjectActor*)target)->CfObject_getCurrentTarget() + 0x1C) = chainSlot;
                    if (!hasStatus(target, 0x802)) {
                        CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target), 0x2C, 0, -1, 1);
                    }
                }
            }
        } else {
            if (move->field_78 & 0x00000800) {
                localMove = *move;                    // 0x6AF4
            }
            // else: localMove keeps the prologue zeroed value (0x6AE8 -> 0x6C6C)
        }
    }

    // ========================================================================
    // 0x6C6C tail (shared)
    // ========================================================================
    if (localMove.field_74 & 0x1) {
        if ((localMove.field_78 & 0x200) &&
            !(localMove.field_78 & 0x01000000) &&
            !(localMove.field_78 & 0x02000000)) {
            if (!isBattleStatusActive((u8*)target + 8, 0x7)) {
                if (((cf::CActorParam*)(attacker))->CActorParam_getRankByte() < localMove.field_A0) {
                    // note: retail uses *target*->field_3F10 here, installed
                    // on *attacker*+0x3380
                    installSlot(attacker, ((cf::CfObjectActor*)target)->field_0x74, 0x55);
                }
            }
        }
    }

    f32 f25 = CBattleMan_ApplyStatusRate((u8*)attacker, &localMove);
    f32 f26 = lbl_eu_80666DD4;
    if ((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable()) {
        u32 tmp;
        if (IdTable_GetSum((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable(), 0x6E, &tmp)) {
            f26 = f26 + (f32)i2d((s32)tmp) / 100.0f;
        }
    }
    if (isBattleStatusActive((u8*)target + 8, 0xFE)) f26 = lbl_eu_80666DDC;
    if (f25 < lbl_eu_80666DDC) f25 = lbl_eu_80666DDC;
    if (f26 < lbl_eu_80666DDC) f26 = lbl_eu_80666DDC;

    s32 hateBase = (s32)localMove.field_5C;
    if (localMove.field_74 & 0x8000) {
        if (localMove.field_5C < lbl_eu_80666DDC) hateBase = (s32)localMove.field_64;
        else hateBase = (s32)(localMove.field_64 - localMove.field_5C);
    }

    ((cf::CfObjectActor*)(target))->CActorParam_addHate(attacker, f25 * 0.5f * (f32)i2d(hateBase), lbl_eu_80666DDC, lbl_eu_80666DDC);
    if (!(localMove.field_74 & 0x8000) || localMove.field_5C >= lbl_eu_80666DDC) {
        ((cf::CfObjectActor*)(attacker))->CActorParam_addHate(target, f26 * f25 * (f32)i2d(-hateBase), lbl_eu_80666DDC, lbl_eu_80666DDC);
    }

    if ((u32)((cf::CfObjectActor*)target)->CObjectParam_getSelfObjectId() == 0) {
        ((cf::CfObjectActor*)target)->CObjectParam_signalActionEnd((int)(((cf::CfObjectActor*)attacker)->field_0x74));
    }

    if (hasStatus(target, 0x1000)) {
        ((cf::CObjectState*)(*(void**)((u8*)target + 4)))->clearStateFlags8((int)(0x1000));
    }
}

struct BtlMove {
    u8 pad_00[0x04];
    u32 mSubId;        // +0x04 -> findObjectById
    u8 pad_08[0x48];
    void* mSub;        // +0x50  (r28)
    u8 pad_54[0x08];
    f32 mDmg;          // +0x5C
    f32 mHeal;         // +0x60
    u8 pad_64[0x10];
    u32 mFlags74;      // +0x74  (0x80,0x8,0x100,0x2,0x60,0x40,0x10000,0x4000,0x4000000,0x2000000)
    u32 mFlags78;      // +0x78  (0x1,0x200,0x400,0x800)
    u8 pad_7C[0x04];
    u16 mId80;         // +0x80  (consumed by func_800EC918)
    u8 pad_82[0x2E];
    u32 mB0;           // +0xB0  (dmg % 100 source)
};

// r28: type-3 sub-param object (fields used by this function)
struct BtlSubParam {
    u8 pad_00[0x3C];
    u16 mType3C;       // +0x3C  == 3
    u8 pad_3E[0x0A];
    u16 mEvId48;       // +0x48
    s16 mVal4A;        // +0x4A
    s16 mVal4C;        // +0x4C
    f32 mF50;          // +0x50
    f32 mF54;          // +0x54
    u8 pad_58[0x02];
    u16 mIdx5A;        // +0x5A  (getArtsFacing/8C/C0/134 table index)
    u8 pad_5C[0x0A];
    u8 mType66;        // +0x66
    u8 mType67;        // +0x67
    s16 mVal68;        // +0x68
    s16 mVal6A;        // +0x6A
    u8 pad_6C[0x02];
    u8 mVal6E;         // +0x6E
    u8 mVal6F;         // +0x6F
    u8 pad_70[0x08];
    u32 mFlags78;      // +0x78  (0x400, 0x800)
    u8 pad_7C[0x08];
    void* mSub84;      // +0x84  sub-obj, vtable[0x0C]() -> s32 count
};

// r19: battle-status struct from actor vtable[0x224]
struct BtlStatusPc {
    u8 pad_00[0x64];
    s16 mS64;          // +0x64
    s16 mS66;          // +0x66
    s16 mS68;          // +0x68
    s16 mS6A;          // +0x6A
    s16 mS6C;          // +0x6C
    s16 mS6E;          // +0x6E
    u8 mF70;           // +0x70 flags
    u8 pad_71;
    u8 mF72;           // +0x72 flags
};

// target->+0x3F60 heading/move data (fields written by this function)
// status entry returned by the target's CBattleState vtable[0x54]
// (mirrors cf::CBattleStateEntry field layout for the used members)
struct BtlStatusEntry {
    u32 unk00;
    u32 unk04;
    u32 unk08;
    u16 unk0C;   // +0x0C id
    u32 unk10;   // +0x10 value (s32)
    s16 unk14;   // +0x14 range
    s16 unk16;   // +0x16 event id
    s16 unk18;   // +0x18 event value
};

struct BtlMoveData {
    u8 pad_00[0x468];
    f32 mFacing468;    // +0x468
    f32 mFacing46C;    // +0x46C
    f32 mSpeed470;     // +0x470
    f32 mSpeed474;     // +0x474
    f32 mFacing478;    // +0x478
    u8 pad_47C[0xB4];
    u16 mFlags530;     // +0x530 (bit0 gating the heading-set)
};

// 0x34-byte battle-event struct consumed by func_800EC918
struct BtlEvent {
    u32 mTargetId;     // +0x00 (actor->0x3F10)
    void* mParam;      // +0x04 (sub, or 0)
    u32 mPad08;        // +0x08
    u16 mEvId;         // +0x0C
    u16 mPad0E;        // +0x0E
    s32 mVal10;        // +0x10
    s32 mVal14;        // +0x14
    u32 mPad18;        // +0x18
    f32 mF1C;          // +0x1C
    f32 mF20;          // +0x20
    f32 mF24;          // +0x24
    u32 mPad28;        // +0x28
    u32 mPad2C;        // +0x2C
    u32 mFlags30;      // +0x30
};

struct Vec3f {
    f32 x, y, z;
};

// ---------------------------------------------------------------------------
// Externs for processBattleMove (retail symbols; only NEW ones - duplicates of
// file-scope / header decls above are omitted to avoid C-linkage overloads)
// ---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

// Retail: fcmpo f,0.0; ble -> -0.5 : +0.5 (DOUBLE constants); fadd (double);
// fctiwz.  Round half away from zero (up for positives).
static inline s32 roundF(f32 f) {
    return (s32)((f64)f + (f > lbl_eu_80666DDC ? 0.5 : -0.5));
}

// r22->0x1AA (u8, inside CChain at +0x1A8): 1..0x18 (unsigned) gate.
// Used at 0x7678, 0x8D24, 0x8E80.
static inline bool chapterInRange(void* self) {
    u8 v = ((cf::CBattleManager*)self)->mChain.unk0[2];
    return v >= 1 && v <= 0x18;
}

// (Former e64_VFn* manual-slot typedefs removed: slots 0x2BC/0x290/0x5C0/
// 0x2C4/0x120/0x11C/0x12C/0x154/0x184/0x1A4/0x224/0x5B4/0x128/0x130/
// 0x2F8/0x0C4/0xAC/0x4C/0xC4/0x20C are CActorParam Func138/127,
// CfObjectActor Func9, CActorParam Func140/35/34/38/48/60/68/100,
// CfObjectActor Func6, CActorParam Func37/39/153/12, CfObject UVF23/29
// and CfObjectMove Func18; the +8/+4 slots are CBattleState Func20 and
// CObjectState Func11/Func7. Call sites use the real virtuals directly.)

// bit/switch helpers on r19 (BtlStatusPc) ------------------------------------
// flag72: switch(cat){1:&0x80 2:&0x01 3:&0x02 4:&0x04 5:&0x08 6:&0x10 7:&0x20}
// retail instantiates this switch TWICE (tables 0x8052B680 / 0x8052B658)
static inline int flag72(const BtlStatusPc* bs, int cat) {
    switch (cat) {
        case 1: return (bs->mF72 & 0x80) != 0;
        case 2: return (bs->mF72 & 0x01) != 0;
        case 3: return (bs->mF72 & 0x02) != 0;
        case 4: return (bs->mF72 & 0x04) != 0;
        case 5: return (bs->mF72 & 0x08) != 0;
        case 6: return (bs->mF72 & 0x10) != 0;
        case 7: return (bs->mF72 & 0x20) != 0;
        default: return 0;
    }
}
// flag70: same bit pattern on mF70 (tables 0x8052B630 / 0x8052B608)
static inline int flag70(const BtlStatusPc* bs, int cat) {
    switch (cat) {
        case 1: return (bs->mF70 & 0x80) != 0;
        case 2: return (bs->mF70 & 0x01) != 0;
        case 3: return (bs->mF70 & 0x02) != 0;
        case 4: return (bs->mF70 & 0x04) != 0;
        case 5: return (bs->mF70 & 0x08) != 0;
        case 6: return (bs->mF70 & 0x10) != 0;
        case 7: return (bs->mF70 & 0x20) != 0;
        default: return 0;
    }
}
// value16: switch(cat){4:mS64 5:mS66 6:mS68 7:mS6A 8:mS6C 9:mS6E}
static inline s16 value16(const BtlStatusPc* bs, int cat) {
    switch (cat) {
        case 4: return bs->mS64;
        case 5: return bs->mS66;
        case 6: return bs->mS68;
        case 7: return bs->mS6A;
        case 8: return bs->mS6C;
        case 9: return bs->mS6E;
        default: return 0;
    }
}
// effect class for the actor (0x7ED8 switch): {4:0x42 5:0x4C 6:0x44 7:0x46
//                                             8:0x4A 9:0x48}
static inline u32 effectClass(int cat) {
    switch (cat) {
        case 4: return 0x42;
        case 5: return 0x4C;
        case 6: return 0x44;
        case 7: return 0x46;
        case 8: return 0x4A;
        case 9: return 0x48;
        default: return 0;
    }
}
// sound id (0x7F54 switch): {1:0x1AD 2:0x1AA 3:0x1B0 4:0x1B6 5:0x1BA
//                            6:0x1B5 7:0x1B7 8:0x1B8 9:0x1B9}
static inline u32 statusSound(int cat) {
    switch (cat) {
        case 1: return 0x1AD;
        case 2: return 0x1AA;
        case 3: return 0x1B0;
        case 4: return 0x1B6;
        case 5: return 0x1BA;
        case 6: return 0x1B5;
        case 7: return 0x1B7;
        case 8: return 0x1B8;
        case 9: return 0x1B9;
        default: return 0;
    }
}

// full 3D distance used by the range checks (retail PSVECMag of (b-a))
static inline f32 dist3D(void* a, void* b) {
    Vec3f* pa = (Vec3f*)((cf::CfObjectActor*)a)->CfObject_getPosVector();
    Vec3f* pb = (Vec3f*)((cf::CfObjectActor*)b)->CfObject_getPosVector();
    Vec3f d = { pb->x - pa->x, pb->y - pa->y, pb->z - pa->z };
    return PSVECMag((const struct Vec*)&d);
}

// mActorList1 (reslist at self+0x4) membership walk + "node == sentinel"
// check, as compiled at 0x70A0-0x70D8 (the subf/or/srwi pair is the inlined
// reslist node-vs-sentinel test, cf. ScnFilterListIter_notEqual).
static inline bool inActorList1(void* self, void* item) {
    struct Node { Node* mNext; Node* mPrev; void* mItem; };
    Node* sentinel = (Node*)((cf::CBattleManager*)self)->mActorList1.mStartNodePtr;
    Node* cur = sentinel->mNext;
    while (cur != sentinel) {
        if (cur->mItem == item) break;
        cur = cur->mNext;
    }
    return cur != sentinel;
}

// "power" = round-half-up(dmg + heal), guarded by < 0 and flag 0x80.
// Retail re-computes this 3x (0x72E4, 0x7410, 0x74A0) - keep as call.
static inline s32 calcPower(const void* mv) {
    const BtlMove* move = (const BtlMove*)mv;
    f32 sum = move->mDmg + move->mHeal;
    if (sum < lbl_eu_80666DDC || (move->mFlags74 & 0x80)) return 0;
    return roundF(sum);
}

// ---------------------------------------------------------------------------
// The reconstructed function
// ---------------------------------------------------------------------------
extern "C" void CfSoundMan_playActorSound(u32 a, u32 b, u32 c, u32 d, f32 vol);

extern "C" void processBattleMove(cf::CBattleManager* self, void* actor, void* arg5, void* move) {
    // r22 = self, r23 = actor, r5 = arg5 (null-gated only), r25 = move

    if (arg5 == nullptr) return;                      // 0x800E7024

    BtlMove* bm = (BtlMove*)move;
    BtlSubParam* sub = (BtlSubParam*)bm->mSub;      // r28 (0x7094)
    void* target;                                     // r24
    if (bm->mFlags74 & 0x80) {
        target = actor;                               // 0x7034
    } else {
        target = MapFx_ResolveOwner(findObjectById((s32)bm->mSubId));  // 0x703C-0x7048
    }

    if (((cf::CActorParam*)(target))->CActorParam_isBattleLocked() != 0) return;   // 0x704C-0x7064
    if (isBattleStatusActive((u8*)target + 8, 0xF8)) return;                             // 0x7068-0x7078

    emitBattleEvents(self, actor, target, move);         // 0x707C-0x708C

    if (bm->mFlags78 & 0x1) {                       // 0x7090-0x70D8
        if (!inActorList1(self, target)) {
            if ((u32)((cf::CfObjectActor*)(target))->CfObjectActor_sharesMoveFlags((cf::CfObjectActor*)actor) != 0)
                return;                               // 0x70DC-0x70F8
        }
    }

    if (sub->mType3C != 3) goto postLoop;             // 0x70FC-0x7104

    // ---- initial damage gate ----------------------------------------------
    {
        f32 sum = bm->mDmg + bm->mHeal;           // 0x7108-0x7118
        s32 dmg;
        if (sum < lbl_eu_80666DDC || (bm->mFlags74 & 0x80)) {
            dmg = 0;                                  // 0x7120 / 0x7134
        } else {
            dmg = roundF(sum);                        // 0x713C-0x7158
        }
        if (dmg <= 0) goto postLoop;                  // 0x715C-0x7160
    }

    // ---- status 0x106 branch: rate + knockback + voice ---------------------
    if (isBattleStatusActive((u8*)target + 8, 0x106)) {      // 0x7164-0x7174
        findBattleStatusEntry((u8*)target + 8, 0x106);        // 0x7178-0x7180
        if (!(bm->mFlags78 & 0x800)) {              // 0x7184-0x718C
            if (sub == nullptr) return;               // 0x7190-0x7198 (reload)

            // rate selection from dmg%100 vs thresholds (0x719C-0x7274)
            f32 rate = lbl_eu_80666DD4;               // 1.0f (0x71C0)
            s32 x = artsSubGetMax(sub);   // 0x71B4-0x71EC
            s32 y = artsSubGetMax(sub);   // 0x71FC-0x7210
            // NOTE: retail materialises the 3-entry threshold array at
            // 0xC4/0xC8/0xCC = { x/2+3, y+10, *(u32*)&sDamageRateThresholds[2] (== 2) };
            // the 3rd entry comes from the rodata table, not an immediate.
            u32 th[3] = { (u32)(x / 2 + 3), (u32)(y + 10), *(const u32*)&sDamageRateThresholds[2] };
            u32 rem = bm->mB0 % 100;                // 0x71A0-0x71E4 (mulhw magic)
            for (int i = 0; i < 3; i++) {             // 0x724C-0x7270
                rem -= th[i];
                if ((s32)rem < 0) {
                    rate = sDamageRates[i];           // {8,4,2}
                    break;
                }
            }

            if ((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable() != nullptr) {  // 0x7274-0x728C
                u32 v;
                if (IdTable_GetSum((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable(),
                                  0x5B, &v)) {        // 0x7290-0x72B0
                    rate *= 1.0f + (f32)(u32)v / 100.0f;   // 0x72B8-0x72E0 (100.0f)
                }
            }

            s32 power = calcPower(move);              // 0x72E4-0x7348
            // retail also computes (mFlags74 & 0x7FFFFFFD) & 0x7FFFFBE5 into a
            // dead register here (0x7304-0x7308) - source-side artifact.

            // knockback (0x734C-0x740C): -(s32)(a + (a + (a>0 ? 0.5 : -0.5)))
            {
                f32 a = (f32)(s32)power * rate;
                f64 r = (f64)a + (a > lbl_eu_80666DDC ? 0.5 : -0.5);
                s32 kb = -(s32)((f64)a + r);
                ((cf::CActorParam*)(actor))->CActorParam_applyDamage((f32)kb, 2, 0, 0);
            }

            // voice event A (0x7410-0x7494)
            {
                s32 p = calcPower(move);              // re-computed
                CCharVoiceMan_EnqueueDamageVoice(actor, (void*)target, (void*)(size_t)(s32)(rate * (f32)(-p)));
            }
            CCharVoiceMan_ClearPlayerVoiceCount(target);                    // 0x7498

            // voice event B (0x74A0-0x7520)
            {
                s32 p = calcPower(move);              // re-computed
SysWinLog_PollSequenceProgress((CScenarioLogOwner*)target, (s32)(rate * (f32)(s32)p), (CScenarioLogArg2*)move);
            }
            ((cf::CActorParam*)(actor))->CActorParam_applyEventStatus((cf::CActorParam12Arg*)(move));   // 0x7524-0x7538
        }
    }

    // ---- gates before the status loop (0x753C-0x75AC) ----------------------
    if (isBattleStatusActive((u8*)target + 8, 0x10)) {       // 0x753C-0x754C
        if (!(((cf::CfObjectActor*)target)->unk3374 & 0x40000)) goto postLoop;   // 0x7550-0x7558
        {
            void* sub4 = *(void**)((u8*)target + 0x04);
            int v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();   // 0x755C-0x7570
            if (testActorStatus(target, &v, 0x17) == 0) goto postLoop;        // 0x7574-0x758C
        }
    }
    if (isBattleStatusActive((u8*)target + 8, 0x117)) goto postLoop;   // 0x7590-0x759C
    if (bm->mFlags74 & 0x80) goto postLoop;                   // 0x75A0-0x75AC

    // ---- main status loop: CBattleState entries 0x40..0x67 ----------------
    for (u32 id = 0x40; id < 0x68; id++) {            // r27 (0x75F4-0x7FD4)
        BtlStatusEntry* st =
            (BtlStatusEntry*)((cf::CBattleState*)((u8*)target + 8))->CBattleState_getEntryByIndex((int)(id));
        u16 sid = st->unk0C;
        s32 cat = 0;                                  // r26

        if (sid >= 0x6E && sid <= 0x7C) {
            switch (sid) {
                case 0x6E: cat = classifyBattleCommand(0x6E); break;   // 0x7638
                case 0x6F: cat = classifyBattleCommand(0x6F); break;   // 0x7648
                case 0x70: cat = classifyBattleCommand(0x70); break;   // 0x7658
                case 0x71: cat = classifyBattleCommand(0x71); break;   // 0x7668
                case 0x72: {                                  // 0x7678
                    if (chapterInRange(self)) break;           // -> skip event
                    if (st->unk14 == 0 || dist3D(actor, target) <= (f32)(s16)st->unk14) {
                        // build the 0x72 event (0x7734-0x77B8)
                        BtlEvent ev;
                        std::memset(&ev, 0, sizeof(ev));
                        ev.mTargetId = ((cf::CfObjectActor*)actor)->field_0x74;
                        ev.mParam = nullptr;
                        ev.mEvId = (u16)st->unk16;
                        ev.mVal10 = (s32)(s16)st->unk18;
                        ev.mF20 = k10_0f;              // 10.0f
                        ev.mF24 = lbl_eu_80666DDC;              // lbl_eu_80666DDC
                        ev.mFlags30 |= 0x20000;
                        func_800EC918(self, actor, target, &ev, move);
                        bindIndexedEffect(0x42, (u32)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor));
                        CfSoundMan_playActorSound(0, 0x1B6, 0, 0, 0.6f);
                    }
                    break;
                }
                default: break;                       // 0x73..0x7C
            }
        }

        // distance re-check for category statuses (0x77BC-0x785C)
        if (cat == 0 || st->unk14 == 0) goto nextStatus;
        if (dist3D(actor, target) > (f32)(s16)st->unk14) cat = 0;
        if (cat == 0) goto nextStatus;

        // ---- apply (0x7864-0x7FD0) ----------------------------------------
        {
            BtlStatusPc* bs = (BtlStatusPc*)((cf::CActorParam*)(actor))->CActorParam_getBattleParams();  // r19
            s32 val = st->unk10;                      // r16 (0x7884: st->0x10)
            f32 rate = lbl_eu_80666DD4;               // f25 = 1.0f

            if ((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable() != nullptr) {   // 0x7894-0x789C
                if (findBattleStatusEntry((u8*)target + 8, 0x6F) != nullptr ||
                    findBattleStatusEntry((u8*)target + 8, 0x79) != nullptr) {                    // 0x78A0-0x78C4
                    u32 v;
                    if (IdTable_GetSum((void*)((cf::CActorParam*)(target))->CActorParam_getStatusTable(),
                                      0x5B, &v)) {   // 0x78C8-0x78E8
                        val = (s32)((f32)(s32)val * (1.0f + (f32)(u32)v / 100.0f)); // 0x78F0-0x7928
                    }
                }
            }
            if (isBattleStatusActive((u8*)actor + 8, 0x8D)) {                                     // 0x792C-0x793C
                BtlStatusEntry* e = (BtlStatusEntry*)findBattleStatusEntry((u8*)actor + 8, 0x8D);
                rate -= 0.01f * (f32)(s32)e->unk10;   // 0.001f (0x7940-0x7960)
            }
            if ((void*)((cf::CActorParam*)(actor))->CActorParam_getStatusTable() != nullptr) {      // 0x7964-0x797C
                u32 v;
                if (IdTable_GetSum((void*)((cf::CActorParam*)(actor))->CActorParam_getStatusTable(),
                                  0x57, &v)) {       // 0x7980-0x79A0
                    rate -= 0.01f * (f32)(s32)v;       // 0x79A8-0x79BC
                }
            }
            val = (s32)((f32)(s32)val * rate);        // 0x79C0-0x79DC
            if (val < 1) val = 0;                     // 0x79E0-0x79E8
            if (isBattleStatusActive((u8*)actor + 8, 0x01)) val = 0;    // 0x79EC-0x7A00

            // sign decision (0x7A04-0x7C40)
            int r0;
            if (cat >= 1 && cat <= 3) {
                r0 = flag72(bs, cat);                 // table 0x8052B680
            } else if (cat >= 4 && cat <= 9) {
                s16 vv = value16(bs, cat);            // 0x7AF0
                r0 = (vv >= 0xC8) ? 1 : flag72(bs, cat);        // 0x7B58 / table 0x8052B658
            } else {
                r0 = 0;
            }
            if (r0) {
                val = -val;                           // 0x7C38
            } else {
                // halve decision (0x7C44-0x7E98)
                if (cat >= 1 && cat <= 3) {
                    r0 = flag70(bs, cat);             // table 0x8052B630
                } else if (cat >= 4 && cat <= 9) {
                    s16 vv = value16(bs, cat);        // 0x7D30
                    r0 = (vv >= 0x64) ? 1 : flag70(bs, cat);     // 0x7D98 / table 0x8052B608
                } else {
                    r0 = 0;
                }
                if (r0) val = (s32)((f32)(s32)val * 0.5f);   // *0.5f (0x7E88)
            }

            if (val != 0) {                           // 0x7E9C-0x7ED4
                ((cf::CActorParam*)(actor))->CActorParam_applyDamage(-(f32)val, 2, 0, 0);
            }
            bindIndexedEffect(effectClass(cat), (u32)(actor ? (size_t)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor) : 0));  // 0x7ED8-0x7F50
            CfSoundMan_playActorSound(0, statusSound(cat), 0, 0, 0.6f);     // 0x7F54-0x7FD0
        }

nextStatus:
        ;
    }

postLoop:
    // ---- post-loop: target switch + per-sub event dispatch (0x7FE0-0x8FEC) --
    if (!(bm->mFlags74 & 0x8)) {                    // 0x7FE0-0x7FE8
        if (isBattleStatusActive((u8*)target + 8, 0xF7)) {   // 0x7FEC-0x7FFC
            if ((u32)((cf::CfObjectActor*)(target))->CfObjectActor_sharesMoveFlags((cf::CfObjectActor*)actor) == 0) {  // 0x8000-0x801C
                findBattleStatusEntry((u8*)target + 8, 0xF7); // 0x8020-0x8028
                u32 f78 = bm->mFlags78;             // 0x802C
                if ((f78 & 0x200) || ((f78 & 0x400) && !(f78 & 0x800))) {   // 0x8030-0x8044
                    bm->mFlags74 |= 0x80000000 | 0x80;   // 0x8048-0x8058
                    target = actor;                   // 0x804C
                }
            }
        }
    }

    {
        int hit = 0;                                  // r14
        f32 f24 = lbl_eu_80666DDC;                    // lbl_eu_80666DDC (0x8064)

        if (!(bm->mFlags74 & 0x8) || (sub->mFlags78 & 0x800)) {   // 0x805C-0x806C: rlwinm 28,28 / bit 20
            // switch on sub->mType66 (0..0x19), table 0x8052B578 (0x807C-0x82B4)
            switch (sub->mType66) {
                case 0:   // 0x80A0
                    if (((cf::CfObjectActor*)actor)->unk3374 & 0x10000) {
                        if ((s32)((cf::CActorParam*)(target))->CActorParam_getActorType() == 1 ||
                            (s32)((cf::CActorParam*)(target))->CActorParam_getActorType() == 2)
                            hit = 1;
                    } else {
                        if ((s32)((cf::CActorParam*)(target))->CActorParam_getActorType() == 1)
                            hit = 1;
                    }
                    break;
                case 1:   // 0x8110
                    if ((s32)((cf::CActorParam*)(target))->CActorParam_getActorType() == 3)
                        hit = 1;
                    break;
                case 2:   // 0x8134
                    if (bm->mFlags74 & 0x100) hit = 1;
                    break;
                case 3:   // 0x8148
                    if (bm->mFlags74 & 0x4000000) hit = 1;
                    break;
                case 4:   // 0x815C
                    if (bm->mFlags74 & 0x2000000) hit = 1;
                    break;
                case 5:   // 0x8170
                    if (isBattleStatusActive((u8*)actor + 8, 0xC1)) hit = 1;
                    break;
                case 6:   // 0x818C
                    if (isBattleStatusActive((u8*)target + 8, 0x10)) hit = 1;
                    break;
                case 7:   // 0x81A8
                    if (isBattleStatusActive((u8*)target + 8, 0x0F)) hit = 1;
                    break;
                case 8:   // 0x81C4
                    if ((void*)(uintptr_t)((cf::CfObjectActor*)target)->CObjectParam_getSelfObjectId()
                            == (void*)((cf::CfObjectActor*)actor)->field_0x74)
                        hit = 1;
                    break;
                case 9:   // 0x81EC
                    if (bm->mDmg >= ((cf::CActorParam*)(target))->CActorParam_getHp()) {
                        hit = 1;
                        ((cf::CfObjectActor*)actor)->CfObjectMove_transferSlotBits((u32)(0x2B));
                    }
                    break;
                case 10:  // 0x8230
                    if (sub->mType67 != 0) hit = 1;
                    break;
                case 11:  // 0x8244
                    if ((u16)getReloadParam0() == (u16)(sub->mType66 - 0x0A)) hit = 1;
                    break;
                case 12:  // 0x8260
                    if ((bm->mFlags74 & 0x4000000) &&
                        (s32)((cf::CActorParam*)(target))->CActorParam_getActorType() == 1)
                        hit = 1;
                    break;
                case 13:  // 0x8290
                    if (((cf::CActorParam*)(target))->CActorParam_getHitRate() < 0.15f)  // 0.15f
                        hit = 1;
                    break;
                default:  // 0x14..0x19
                    break;
            }
        }

        if (sub->mFlags78 & 0x800) {                  // 0x82B4-0x82BC
            if (findFreeBattleSlot((D7D24_Obj*)move) == 0) hit = 0;    // 0x82C0-0x82D0
        }

        if (hit != 0) {                               // 0x82D4-0x8300
            u8 t = sub->mType66;
            if ((u32)(t - 0x0B) > 0x0C && t != 0) {   // not in [0xB,0x17] and != 0
                bm->mFlags74 |= 0x80000000 | 0x400;
            }
        }

        if (hit != 0) {                               // 0x8304-0x836C
            if ((((cf::CfObjectActor*)actor)->unk64 & 0x2) && sub->mType66 != 0) {
                cf::PartyGaugeAddClamped(reinterpret_cast<cf::UnkClass_8018C5FC*>((u8*)&((cf::CBattleManager*)self)->unk194), 0x14);   // 0x8324-0x833C
            }
            if (sub->mType67 == 9) {                  // 0x8340-0x836C
                f24 += (f32)(s32)sub->mVal68 / k10_0f;   // 10.0f
            }
        }

        // ---- event build (0x8370-0x84EC) ----------------------------------
        if (sub->mEvId48 != 0 && (!(bm->mFlags74 & 0x8) || (sub->mFlags78 & 0x400))) {
            BtlEvent ev;
            std::memset(&ev, 0, sizeof(ev));          // 0x8394-0x83A0
            ev.mTargetId = ((cf::CfObjectActor*)actor)->field_0x74;
            ev.mParam = sub;
            ev.mEvId = sub->mEvId48;
            {
                s32 v = artsSubGetMax(sub);
                ev.mVal10 = (s32)sub->mVal4A + (s32)sub->mVal6F * (v - 1);   // 0x83BC-0x83E4
            }
            ev.mVal14 = (s32)(s16)sub->mVal4C;        // 0x83E8-0x83EC
            {
                s32 v = artsSubGetMax(sub);
                ev.mF20 = f24 + (f32)(s32)(v - 1) / k10_0f * (f32)(u32)sub->mVal6E
                          + sub->mF50;                // 0x83F0-0x8448
            }
            ev.mF24 = sub->mF54;                      // 0x844C-0x8450

            switch (ev.mEvId) {
                case 0xD5:                            // 0x846C
                case 0x107:
                    ev.mVal10 = -(s32)bm->mDmg;     // fneg+fctiwz (0x846C-0x848C)
                    CCharVoiceMan_EnqueueHurtVoice(target, actor, move);
                    break;
                case 0xCF:                            // 0x8498
                    if (((cf::CfObjectActor*)actor)->unk3374 & 0x10000)
                        ev.mEvId = 0xD0;
                    break;
                default:
                    break;
            }

            if (sub->mFlags78 & 0x400)                // 0x84AC-0x84B4
                func_800EC918(self, actor, actor, &ev, move);    // 0x84B8-0x84CC
            else
                func_800EC918(self, actor, target, &ev, move);   // 0x84D4-0x84E8
        }

        // ---- sub->mType67 switch (0x84EC-0x8974) --------------------------
        if (hit != 0 && sub->mType67 <= 0x0D) {
            BtlEvent ev;
            std::memset(&ev, 0, sizeof(ev));          // 0x84F4-0x8500
            switch (sub->mType67) {                   // table 0x8052B540
                case 0:                               // 0x8528
                    ((cf::CActorParam*)(actor))->CActorParam_addArtsGauge((f32)(s32)sub->mVal68);
                    break;
                case 1:                               // 0x8558
                    ev.mTargetId = ((cf::CfObjectActor*)actor)->field_0x74;
                    ev.mParam = sub;
                    ev.mEvId = 0x10;
                    ev.mVal10 = 0;
                    ev.mF20 = (f32)(s32)sub->mVal68;
                    ev.mF24 = lbl_eu_80666DDC;        // lbl_eu_80666DDC
                    if (sub->mFlags78 & 0x800)
                        func_800EC918(self, actor, actor, &ev, move);
                    else
                        func_800EC918(self, actor, target, &ev, move);
                    break;
                case 2:                               // 0x85DC (all-actors)
                    ev.mTargetId = ((cf::CfObjectActor*)actor)->field_0x74;
                    ev.mParam = sub;
                    ev.mEvId = 0x10;
                    ev.mVal10 = 3;
                    ev.mF20 = 3.0f;        // 3.0f
                    ev.mF24 = lbl_eu_80666DDC;        // lbl_eu_80666DDC
                    {
                        CfEnumListHolder holder;
                        startEnumObjects(holder.get(), 0x80000000, 0);
                        for (u32 i = 0; ; i++) {
                            cf::CfObjEnumList* list = holder.get();
                            u32 count = list->mPtrCount;
                            if (i >= count) break;
                            void* elem = MapFx_ResolveOwner(getObjectAt(holder.get(), i));
                            if (elem != target)
                                func_800EC918(self, actor, elem, &ev, move);
                        }
                    }
                    break;
                case 3:                               // 0x8688
                    ev.mTargetId = ((cf::CfObjectActor*)actor)->field_0x74;
                    ev.mParam = sub;
                    ev.mEvId = 0x10;
                    ev.mVal10 = 3;
                    ev.mF20 = 3.0f;        // 3.0f
                    ev.mF24 = lbl_eu_80666DDC;        // lbl_eu_80666DDC
                    func_800EC918(self, actor, target, &ev, move);
                    break;
                case 4:                               // 0x86D0
                    ((cf::CActorParam*)(actor))->CActorParam_accumulateTension((int)(sub->mVal68));
                    break;
                case 5:                               // 0x86EC
                    ev.mTargetId = ((cf::CfObjectActor*)actor)->field_0x74;
                    ev.mParam = sub;
                    ev.mEvId = 0x0F;
                    ev.mVal10 = 0;
                    ev.mF20 = (f32)(s32)sub->mVal68;
                    ev.mF24 = lbl_eu_80666DDC;
                    if (sub->mFlags78 & 0x800)
                        func_800EC918(self, actor, actor, &ev, move);
                    else
                        func_800EC918(self, actor, target, &ev, move);
                    break;
                case 6:                               // 0x8770 (SP drain)
                    if (sub->mFlags78 & 0x800) {
                        f32 f2 = ((cf::CActorParam*)(actor))->CActorParam_getDamageScale() / 100.0f;
                        ((cf::CActorParam*)(actor))->CActorParam_addHp(-(f32)(s32)sub->mVal68 * f2);
                    } else {
                        f32 f2 = ((cf::CActorParam*)(actor))->CActorParam_getDamageScale() / 100.0f;
                        ((cf::CActorParam*)(target))->CActorParam_addHp(-(f32)(s32)sub->mVal68 * f2);
                    }
                    break;
                case 7:                               // 0x8824
                    ev.mTargetId = ((cf::CfObjectActor*)actor)->field_0x74;
                    ev.mParam = sub;
                    ev.mEvId = 0xDC;
                    ev.mVal10 = (s32)(s16)sub->mVal68;
                    if (sub->mFlags78 & 0x800)
                        func_800EC918(self, actor, actor, &ev, move);
                    else
                        func_800EC918(self, actor, target, &ev, move);
                    break;
                case 8:                               // 0x8884
                    ev.mTargetId = ((cf::CfObjectActor*)actor)->field_0x74;
                    ev.mParam = sub;
                    ev.mEvId = (u16)sub->mVal68;
                    ev.mVal10 = (s32)(s16)sub->mVal6A;
                    {
                        s32 v = artsSubGetMax(sub);
                        ev.mF20 = (f32)(s32)(v - 1) / k10_0f * (f32)(u32)sub->mVal6E
                                  + sub->mF50;       // 0x88A4-0x88F0
                    }
                    ev.mF24 = sub->mF54;
                    if (sub->mFlags78 & 0x800)
                        func_800EC918(self, actor, actor, &ev, move);
                    else
                        func_800EC918(self, actor, target, &ev, move);
                    break;
                case 9:                               // 0x8940
                    ev.mTargetId = ((cf::CfObjectActor*)actor)->field_0x74;
                    ev.mParam = sub;
                    ev.mEvId = 0xC1;
                    ev.mF20 = k5_0f;        // 5.0f
                    func_800EC918(self, actor, actor, &ev, move);
                    break;
                default:
                    break;
            }
        }

        // ---- type-3 status apply (0x8974-0x8FEC) ---------------------------
        if (sub->mType3C == 3) {
            // gates: 7x testActorStatus(target, &v, mask); v re-fetched each time
            {
                void* sub4 = *(void**)((u8*)target + 0x04);
                int v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                if (testActorStatus(target, &v, 0x0A)) goto afterType3;
                v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                if (testActorStatus(target, &v, 0x0B)) goto afterType3;
                v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                if (testActorStatus(target, &v, 0x16)) goto afterType3;
                v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                if (testActorStatus(target, &v, 0x18)) goto afterType3;
                v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                if (testActorStatus(target, &v, 0x19)) goto afterType3;
                v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                if (testActorStatus(target, &v, 0x1A)) goto afterType3;
                v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                if (testActorStatus(target, &v, 0x1B)) goto afterType3;
            }

            if (bm->mFlags74 & 0x2) {               // 0x8AEC
                CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor), 0x2D, 0, -1, 1);
                if (CChain_HasMemberEntry((cf::CChain*)((u8*)&((cf::CBattleManager*)self)->mChain), (CChainBattleObjTail*)target) == 0)
                    CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target), 0x2E, 0, -1, 1);
            } else if (bm->mFlags74 & 0x60) {       // 0x8B40
                CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor), 0x2D, 0, -1, 1);
                if (bm->mFlags74 & 0x40)
                    bindIndexedEffect(0xB0, (u32)(target ? (size_t)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target) : 0));
            } else {
                {
                    void* sub4 = *(void**)((u8*)target + 0x04);
                    int v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                    if (testActorStatus(target, &v, 0x1000)) goto afterType3;
                    v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                    if (testActorStatus(target, &v, 0x07)) goto afterType3;
                    v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                    if (testActorStatus(target, &v, 0x08)) goto afterType3;
                }

                if (bm->mFlags74 & 0x10000) {       // 0x8C24
                    BtlMoveData* md = *(BtlMoveData**)((u8*)target + 0x3F60);
                    if (!(md->mFlags530 & 1)) {
                        f32 f1 = ((cf::CfObjectActor*)(actor))->CfObject_getMoveHeadAngle();
                        ((cf::CfObjectActor*)target)->CfObject_setMoveHeadAngle(ml::pi + f1);
                    }
                    // face the actor (0x8C70-0x8D24)
                    {
                        Vec3f* pa = (Vec3f*)((cf::CfObjectActor*)actor)->CfObject_getPosVector();
                        Vec3f* pt = (Vec3f*)((cf::CfObjectActor*)target)->CfObject_getPosVector();
                        f32 dx = pt->x - pa->x;
                        f32 dz = pt->z - pa->z;
                        BtlMoveData* md2 = *(BtlMoveData**)((u8*)target + 0x3F60);
                        if (dx * dx + dz * dz <= 0.010000001f) {
                            md2->mFacing46C = ((cf::CfObjectActor*)(actor))->CfObject_getMoveHeadAngle();
                        } else {
                            md2->mFacing46C = Atan2FIdx__Q24nw4r4mathFff(dx, dz) * 0.024543693f;
                        }
                        if (chapterInRange(self)) {
                            md2->mFacing468 = lbl_eu_80666DD4;   // 1.0f (0x8D5C)
                        } else {
                            md2->mFacing468 = getArtsFacing(sub);   // 0x8D48
                        }
                    }
                    CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target), 0x1D, 0, -1, 1);   // 0x8D68
                } else if (bm->mFlags74 & 0x4000) { // 0x8D84
                    BtlMoveData* md = *(BtlMoveData**)((u8*)target + 0x3F60);
                    if (!(md->mFlags530 & 1)) {
                        f32 f1 = ((cf::CfObjectActor*)(actor))->CfObject_getMoveHeadAngle();
                        ((cf::CfObjectActor*)target)->CfObject_setMoveHeadAngle(ml::pi + f1);
                    }
                    // approach values (0x8DCC-0x8EF4)
                    {
                        Vec3f* pa = (Vec3f*)((cf::CfObjectActor*)actor)->CfObject_getPosVector();
                        Vec3f* pt = (Vec3f*)((cf::CfObjectActor*)target)->CfObject_getPosVector();
                        f32 dx = pt->x - pa->x;
                        f32 dz = pt->z - pa->z;
                        BtlMoveData* md2 = *(BtlMoveData**)((u8*)target + 0x3F60);
                        if (dx * dx + dz * dz <= 0.010000001f) {
                            md2->mFacing478 = ((cf::CfObjectActor*)(actor))->CfObject_getMoveHeadAngle();
                        } else {
                            md2->mFacing478 = Atan2FIdx__Q24nw4r4mathFff(dx, dz) * 0.024543693f;
                        }
                        if (chapterInRange(self)) {
f32 v = getArtsScaledSpeed((const u8*)sub);
                            if (v >= lbl_eu_80666DD4) v = lbl_eu_80666DD4;   // min(v,1.0f) (0x8ED4)
                            md2->mSpeed470 = lbl_eu_80666DDC;
                            md2->mSpeed474 = v;
                        } else {
                            md2->mSpeed470 = getArtsClampedRate(sub);
md2->mSpeed474 = getArtsScaledSpeed((const u8*)sub);
                        }
                    }
                    if (getArtsGaugeRate(sub) != lbl_eu_80666DDC) {   // 0x8EF4-0x8F38
                        // NOTE: retail calls getArtsGaugeRate TWICE (0x8EF4 and
                        // 0x8F08); the 2nd result is passed as f1 to vtbl 0x1A4.
                        ((cf::CActorParam*)target)->CActorParam_setGaugeRateB(getArtsGaugeRate(sub));
                        ((cf::CActorParam*)target)->CActorParam_addSecondGauge(1);
                    }
                    CfObjectMove_setAnimModeArgs((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target), 0x1E, 0, -1, 1);   // 0x8F3C
                } else {
                    void* sub4 = *(void**)((u8*)target + 0x04);
                    int v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                    if (testActorStatus(target, &v, 0x800)) goto afterType3;
                    v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                    if (testActorStatus(target, &v, 0x1D)) goto afterType3;
                    v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
                    if (testActorStatus(target, &v, 0x801)) goto afterType3;
                }
            }
        }

afterType3:
        // ---- final (0x8FEC-0x9080) -----------------------------------------
        if ((u32)((cf::CfObjectActor*)(actor))->CfObjectActor_sharesMoveFlags((cf::CfObjectActor*)target) == 0) {
            ((cf::CfObjectActor*)(target))->CActorParam_addHate(actor, lbl_eu_80666DD4, lbl_eu_80666DD4, lbl_eu_80666DD4);   // (1,1,1) 0x900C-0x9030
        } else {
            void* sub4 = *(void**)((u8*)target + 0x04);
            int v = *(u32*)((cf::CObjectState*)(sub4))->getStateData();
            if (testActorStatus(target, &v, 0x1000)) {   // 0x9034-0x9060
                ((cf::CObjectState*)(sub4))->clearStateFlags8((int)(0x1000));   // 0x9068-0x907C
            }
        }
    }
}

// Retail switch-slot reservation, run 2 (decomp .data 0x9F8-0xC60, 0x268 zero
// bytes, covering the retail 0xC5C gap word too, anchored like run 1): pads
// the remaining switch block out so the MWCC vtable run lands at retail
// 0xC60. Same shrink-as-restored rule as run 1.
__attribute__((used)) __attribute__((used)) const void* cbm_data_reserve_run2a[9] = {
    (const void*)processBattleMove,
    (const void*)0,
    (const void*)0
};
__attribute__((used)) const void* cbm_data_reserve_run2b[0x91] = {
    (const void*)processBattleMove,
    (const void*)0,
    (const void*)0
};

// ============================================================================
// followUpBattleMove / CBattleManager_preCalcTotalDamage / spreadChainArtsHits /
// dispatchBattleMove -- move-type dispatchers and chain-arts spread helpers
// ============================================================================

// ---- vtable slots used only by these five functions ------------------------
// (Former vfBC/vf29C/e_vf304/e_vf150/e_vfA4/e_vf2A4/e_vf298/e_vf2AC/e_vf2B4/
// e_vf2B8/e_vf128 manual-dispatch helpers removed: slots 0xBC/0x29C/0x304/
// 0x150/0xA4/0x2A4/0x298/0x2AC/0x2B4/0x2B8/0x128 are CActorParam
// Func10/130/156/47/4/132/129/134/136/137/37; call sites use the real
// virtuals directly.)

// ---- small object views (byte-precise offsets) -----------------------------
struct E85F0_Move {         // move block view used by followUpBattleMove
    u8 pad_00[0x50];
    void* sub;              // +0x50
    u8 pad_54[0x24];
    u32 flags78;            // +0x78
    u8 pad_7C[0x14];
    u32 field_90;           // +0x90
};
struct E484_TypeObj {       // enum-cast result: u16 type at +0x8C, +0x9C ptr
    u8 pad_00[0x8C];
    u16 type8C;             // +0x8C
    u8 pad_8E[0x9C - 0x8E];
    void* field_9C;         // +0x9C
};
struct E484_VisionPair {    // getNonZeroSlot1 result: two u32 ids
    u32 field_00;
    u32 field_04;
};
struct E484_Global6C {      // getEffectTask result: f32 at +0x6C
    u8 pad_00[0x6C];
    f32 field_6C;           // +0x6C
};

// ---- imports (retail-unmangled names) --------------------------------------
extern "C" void CCharVoiceMan_FlagBattleActiveVoice();
extern "C" void dispatchScenarioHelp(void*);
extern "C" void updateBattleHelpState(void*, void*);
extern "C" void CfObjectMove_relaySubB0Slot54(void*, int, int, u32, f32, f32);
void Scn_SetTimeScale(CScn* scene, float value);
extern "C" void* getEffectTask();
extern "C" void followUpBattleMove(void*, void*, void*, void*);
extern "C" void dispatchBattleMove(void*, void*, void*, void*);
extern "C" void CBattleManager_preCalcTotalDamage(void*, void*, f32*, u32*);
extern "C" void spreadChainArtsHits(void*, void*, void*, void*);
// lbl_eu_80664A10 (CHelpManager-ish singleton) is declared by an included header
extern f32 lbl_eu_80666E78;     // 30.0f
extern u32 sArtsPowerTable[];   // {80,60,40,25,10,...} arts-power table

// ---- followUpBattleMove (0x800E90D8) --------------------------------------------
// Move-type 4/5/6 follow-up: validates the giver, fires the 0x12E status (or
// its default event), mirrors the first move block, then either runs the
// counter/chain logic or recurses into processBattleMove.
extern "C" void followUpBattleMove(void* self, void* actor, void* target, void* move) {
    if (target == nullptr) return;                           // 0x800E9100
    if (!(((BattleObjAccessor*)actor)->field_3f00 & 0x2)) return; // 0x800E9104
    void* sub = ((E85F0_Move*)move)->sub;                    // loaded before the +0x3f28 test
    if (((BattleObjAccessor*)actor)->field_3f28 != 7) return; // 0x800E9114

    // One scratch home serves the D7D24 probe, both 0x10A status reads and
    // the 0x12E entry pointer (retail reuses one callee-saved register for
    // all of them).
    void* stat = (void*)findFreeBattleSlot((D7D24_Obj*)move);
    if (((E85F0_Move*)move)->flags78 & 0x800) {              // 0x800E9130
        if (((cf::CActorParam*)(actor))->CActorParam_getStatsSlot() == 0) {                           // 0x800E9138 chain-arts pass
            stat = (void*)isBattleStatusActive((u8*)actor + 8, 0x10A);
            // Retail parks the chain type in the move pointer's register --
            // mirrored here by reusing the parameter slot.
            move = (void*)(uintptr_t)startPcEffect((u32)(uintptr_t)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)target), -1, (s32)(uintptr_t)stat, 0);
            s32 type = (s32)(uintptr_t)move;
            // 0x800E91C4..0x800E92A0: art id picked per chain type; the
            // neg/or/srwi pair in retail computes (stat/0x10A result != 0).
            switch (type) {
            case 6:
                CfObjectMove_relaySubB0Slot54((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor), 0x67 + (stat != nullptr), 0, 0, 0.6f, 30.0f);
                break;
            case 4:
                CfObjectMove_relaySubB0Slot54((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor), 0x69 + (stat != nullptr), 0, 0, 0.6f, 30.0f);
                break;
            case 7:
                CfObjectMove_relaySubB0Slot54((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor), 0x6B + (stat != nullptr), 0, 0, 0.6f, 30.0f);
                break;
            case 8:
                CfObjectMove_relaySubB0Slot54((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor), 0x6D + (stat != nullptr), 0, 0, 0.6f, 30.0f);
                break;
            case 9:
                CfObjectMove_relaySubB0Slot54((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor), 0x6F + (stat != nullptr), 0, 0, 0.6f, 30.0f);
                break;
            case 5:
                CfObjectMove_relaySubB0Slot54((u8*)static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)actor), 0x71 + (stat != nullptr), 0, 0, 0.6f, 30.0f);
                break;
            }

            if (isBattleStatusActive((u8*)actor + 8, 0x12E)) {      // 0x800E92C8
                stat = findBattleStatusEntry((u8*)actor + 8, 0x12E); // entry ptr shares the scratch home
                cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)stat;
                if (e->unk10 != type) ((cf::CActorParam*)(actor))->CActorParam_addArtsGauge(k5_0f);
                e->unk10 = type;
                e->unk20 = k5_0f;
            } else {
                BtlEvent ev;
                std::memset(&ev, 0, sizeof(ev));
                ev.mTargetId = ((cf::CfObjectActor*)actor)->field_0x74;
                ev.mParam = sub;
                ev.mEvId = 0x12E;
                ev.mVal10 = type;
                ev.mF20 = k5_0f;
                func_800EC918((void*)self, (EC918_BattleObjAccessor*)actor,
                              (EC918_BattleObjAccessor*)actor, (BattleEvent*)&ev, 0);
            }
            ((cf::CActorParam*)(actor))->CActorParam_addArtsGauge(k10_0f);                 // 10.0 (0x800E9368)
            void* srcB = ((cf::CActorParam*)(actor))->CActorParam_getStatsEntry((int)(0));                    // 0x800E9384
            void* dstB = ((cf::CActorParam*)(actor))->CActorParam_getStatsEntry((int)(1));
            *((E2A9C_BattleMoveData*)dstB) = *((E2A9C_BattleMoveData*)srcB);
            return;                                          // b .L_800E9644
        }
        // 0x800E9538: second pass -- the move flags are re-read here
        if (((cf::CActorParam*)(actor))->CActorParam_getStatsSlot() == 1) {                           // 0x800E9544
            stat = (void*)isBattleStatusActive((u8*)actor + 8, 0x10A);
            if (sub != nullptr) {                            // r30 == move->unk50
                if ((u32)(((cf::CAttackParamPrefix*)sub)->unk3C - 5) <= 1) {   // types 5,6
                    updateBattleAction((cf::CBattleManager*)self, (cf::CfObjectActor*)actor,
                                  (cf::CfObjectActor*)target, (E2A9C_BattleMoveData*)move);
                } else {
                    processBattleMove((cf::CBattleManager*)self, actor, target, move);
                }
                if (stat != nullptr) {                       // 0x800E95B4
                    u32 tbl[5];
                    tbl[0] = sArtsPowerTable[0];
                    tbl[1] = sArtsPowerTable[1];
                    tbl[2] = sArtsPowerTable[2];
                    tbl[3] = sArtsPowerTable[3];
                    tbl[4] = sArtsPowerTable[4];
                    s32 idx = (s32)(u32)((cf::CActorParam*)(actor))->CActorParam_getStatusCount();           // 0x800E95BC
                    if ((s32)((E85F0_Move*)move)->field_90 < (s32)tbl[idx]) { // 0x800E9600
                        ((cf::CActorParam*)(actor))->CActorParam_setTensionPoints(lbl_eu_80666DDC);
                    }
                }
            }
            return;
        }
        return;                                              // bne .L_800E9644
    }
    processBattleMove((cf::CBattleManager*)self, actor, target, move); // 0x800E9630
}

// Damage a finalised move block deals: field_5C + field_60 rounded half away
// from zero; negative totals and blocked hits (field_74 & 0x80) deal none.

// ---- CBattleManager_preCalcTotalDamage (0x800E965C) -------------------------
// Sums the pending party damage: for each of the outCount entries it lets
// dispatchBattleAI finalise the 0x298 move block, adds the rounded
// field_5C+field_60 amount, then restores a clean copy of the block.
extern "C" void CBattleManager_preCalcTotalDamage(void* self, void* actor, f32* outDamage, u32* outCount) {
    // r3 (self) is not used by the retail body
    void* obj = actor;                                       // r27

    void* sub = ((E2A9C_BattleMoveData*)((cf::CActorParam*)(obj))->CActorParam_getMoveRecord())->field_50;         // r31
    void* action = MapFx_ResolveOwner(
        (void*)(intptr_t)findObjectById((s32)((BtlMove*)((cf::CActorParam*)(obj))->CActorParam_getMoveRecord())->mSubId)); // r30

    *outDamage = lbl_eu_80666DDC;                            // 0x800E96C4
    *outCount = 0;
    if (action == nullptr) return;
    if (sub == nullptr) return;

    // Hit count: 1 per sub-object flag, else a single hit.
    if ((s32)*(u8*)((u8*)sub + 0x43) == 1) *outCount = *(u8*)((u8*)sub + 0x44);
    else *outCount = 1;

    // Clear the selection flag on the action, the 3 party members, and obj.
    ((cf::CActorParam*)(action))->CActorParam_resetArtsStatus((void*)(0));                                       // 0x800E9708
    for (s32 i = 0; i < 3; i++) {                            // 0x800E9724
        void* p = MapFx_ResolveOwner(getPlayer__Q22cf13CfGameManagerFi(i));
        if (p != nullptr && p != action) ((cf::CActorParam*)(p))->CActorParam_resetArtsStatus((void*)(0));
    }
    ((cf::CActorParam*)(obj))->CActorParam_resetArtsStatus((void*)(0));                                          // 0x800E9760
    ((cf::CActorParam*)(action))->CActorParam_resetArtsStatus((void*)(0));                                       // 0x800E9778
    ((cf::CActorParam*)(obj))->CActorParam_initBattleStats();                                            // 0x800E9790

    // Seed the workspace block with the target's current move data.
    *(E2A9C_BattleMoveData*)(void*)((cf::CActorParam*)(obj))->CActorParam_getBattleStats() = *(E2A9C_BattleMoveData*)((cf::CActorParam*)(obj))->CActorParam_getMoveRecord();

    // Store the base damage into the workspace (0x6C/0x68) and zero ticks.
    ((E2A9C_BattleMoveData*)(void*)((cf::CActorParam*)(obj))->CActorParam_getBattleStats())->field_6C = ((cf::CActorParam*)(obj))->CActorParam_getHp();
    ((E2A9C_BattleMoveData*)(void*)((cf::CActorParam*)(obj))->CActorParam_getBattleStats())->field_68 = ((cf::CActorParam*)(action))->CActorParam_getHp();

    ((E2A9C_BattleMoveData*)(void*)((cf::CActorParam*)(obj))->CActorParam_getBattleStats())->field_70 = 0;     // 0x800E99B0
    ((E2A9C_BattleMoveData*)(void*)((cf::CActorParam*)(obj))->CActorParam_getBattleStats())->field_72 = 0;

    // Accumulation loop: run the dispatcher per hit and add the rounded
    // damage. The int->float conversion is left to the compiler: its own
    // 2^52 pool constant is the value retail binds to lbl_eu_80666DE0.

    s32 idx = 0;
    while (idx < (s32)*outCount) {                           // 0x800E99F8
        E2A9C_BattleMoveData* slot = (E2A9C_BattleMoveData*)(void*)((cf::CActorParam*)(obj))->CActorParam_getBattleStats();
        slot->field_78 |= 0x44000000;
        dispatchBattleAI((void*)cf::CBattleManager::spInstance, obj, action, slot);
        slot->field_78 = (slot->field_78 & ~0x04000000) | 0x48000000;

        *outDamage += (f32)roundedMoveDamage(slot);

        ((cf::CActorParam*)(obj))->CActorParam_pushBattleStats();                                        // 0x800E9AB4
        idx++;
    }

    // Restore a clean move-data copy into the target block.
    ((cf::CActorParam*)(obj))->CActorParam_resetStatsSlot();                                            // 0x800E9AD4
    ((E2A9C_BattleMoveData*)(void*)((cf::CActorParam*)(obj))->CActorParam_getBattleStats())->field_70 = 0;
    ((E2A9C_BattleMoveData*)(void*)((cf::CActorParam*)(obj))->CActorParam_getBattleStats())->field_72 = 0;

    E2A9C_BattleMoveData* srcB = (E2A9C_BattleMoveData*)(void*)((cf::CActorParam*)(obj))->CActorParam_getBattleStats();
    E2A9C_BattleMoveData* dstB = (E2A9C_BattleMoveData*)((cf::CActorParam*)(obj))->CActorParam_getMoveRecord();
    *dstB = *srcB;

    if (*outDamage < lbl_eu_80666DDC) *outDamage = lbl_eu_80666DDC;
  // 0x800E9CCC
}

// ---- spreadChainArtsHits (0x800EA63C) --------------------------------------------
// Chain-arts hit spread: refreshes the arts rows, then for each player action
// found in the (0x20, 0x800) enum list emits chain events (0x35 arts / 0x13
// counter) and re-installs the AI slot via BtlDmg_FilterNotifyDamage.
extern "C" void spreadChainArtsHits(void* self, void* target, void* attacker, void* move) {
    u32 out14, out10;
    f32 out0c;
    BattleEventData ev;
    u32 sid;
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x04000000)) {                       // 0x800EA664 (lis r3,0x400)
        ((cf::CActorParam*)target)->CActorParam_refreshBattleStatus();
        return;
    }

    // 0x800EA68C: status-id via the +4 holder, then the 0x2000000 gate
    {
        void* holder4 = *(void**)((u8*)target + 0x04);
        sid = *(u32*)((cf::CObjectState*)holder4)->getStateData();
        if (testActorStatus(target, (int*)&sid, 0x02000000)) return;
    }

((cf::CActorParam*)target)->CActorParam_resetGaugesAndRefresh();                            // slot 0xC8 (0x800EA6C0)

    if (attacker != nullptr && (((BattleObjAccessor*)target)->field_3f00 & 0x4)) {
        // 0x800EA6E8: find target in the self+8 list, then the 0x800EA71C call
        SimpleListNode* s = (SimpleListNode*)((cf::CBattleManager*)self)->mActorList1.mStartNodePtr;
        SimpleListNode* c;
        for (c = s->next; c != s && c->data != target; c = c->next) {}
        // retail: the D9978 call fires when the target was NOT found in the list
        if (c == s) {
            registerBattleActor((cf::CBattleManager*)self, (cf::CfObjectActor*)target);
        }
    }

    if (attacker == nullptr) {                               // 0x800EA720
        s32 subId = ((cf::CfObjectActor*)target)->CObjectParam_getSelfObjectId();
        attacker = MapFx_ResolveOwner(findObjectById(subId));
    }

    if (attacker != nullptr &&
        (((BattleObjAccessor*)attacker)->field_3f00 & 0x2) &&
        (((BattleObjAccessor*)target)->field_3f00 & 0x4)) {  // 0x800EA748
        if (!(((EC918_BattleObjAccessor*)target)->field_3374 & 0x04000000)) {
            // 0x800EA778: refresh the eight arts rows
            for (s32 i = 1; i <= 8; i++) {
                s32 flag = (move != nullptr)
                    ? (s32)((((E2A9C_BattleMoveData*)move)->field_74 >> 14) & 1)
                    : 0;
                void* arts = CtrlObjectParam_GetCharEntry((u16)i);
CtrlObjectParam_RunPartyArtsFlow(arts, (s32)(uintptr_t)((cf::CActorParam*)target)->CActorParam_getSpentCurrency(),
(void*)(uintptr_t)((cf::CActorParam*)target)->CActorParam_getSecondCurrency(),
                              (s32)(uintptr_t)((cf::CActorParam*)target)->CActorParam_getActorLevel(), flag, 0, 0);
            }
        }

        // 0x800EA80C: count the mActorList3 members (sentinel at self+0x48)
        {
            _reslist_node<cf::CfObjectActor*>* cur;
            s32 cnt = 0;
            _reslist_node<cf::CfObjectActor*>* sentinel = ((cf::CBattleManager*)self)->mActorList3.mStartNodePtr;
            for (cur = sentinel->mNext; cur != sentinel; cur = cur->mNext) { cnt++; }
            if (cnt <= 1) {
                CCharVoiceMan_FlagBattleActiveVoice();
                SysWinLog_PollBattleStart();
                dispatchScenarioHelp((void*)lbl_eu_80664A10);
            } else if (((BattleObjAccessor*)attacker)->field_3f00 & 0x2) {
                // ---- 0x800EA854: enum-list walk ----
                CfEnumListHolder holder;
                startEnumObjects(holder.get(), 0x20, 0x800);

                const s32 cArt35 = 0x35;
                const s32 cVal1000 = 0x1000;
                const s32 cEvType2 = 0x2;
                const s32 cSel5A = 0x5A;
                const s32 cSel13 = 0x13;

                for (u32 i = 0; i < ((cf::CVisionEnumList*)holder.get())->count; i++) {
                    void* obj = MapFx_ResolveOwner(getObjectAt(holder.get(), i));
                    void* listActor = obj;
                    if (obj == attacker) {                       // 0x800EA8A0
                        if (((cf::CActorParam*)obj)->CActorParam_getStatusTable() &&       // called twice (retail)
                            IdTable_QuerySumMaxFloat((void*)(uintptr_t)((cf::CActorParam*)obj)->CActorParam_getStatusTable(),
                                          0x35, &out14, &out10, (u32*)&out0c) &&
                            (s32)mtRand__Q22ml4mathFi(100) < (s32)out10) {
                            // 0x800EA908: propagate the (0x35, 0x1000) chain event
                            // to every listed actor.
                            std::memset(&ev, 0, sizeof(ev));
                            ev.field_10 = out14;
                            ev.field_20 = out0c;
                            ev.field_0C = (u16)cEvType2;
                            for (u32 j = 0; j < ((cf::CVisionEnumList*)holder.get())->count; j++) {
                                void* sub = MapFx_ResolveOwner(getObjectAt(holder.get(), j));
                                ev.field_00 = (u32)(uintptr_t)((BattleObjAccessor*)sub)->field_3f10;
                                ev.field_04 = (u32)cArt35;
                                ev.field_08 = (u32)cVal1000;
                                ev.field_30 |= 0x1800;
                                func_800EC918((void*)cf::CBattleManager::spInstance, 0,
                                              (EC918_BattleObjAccessor*)sub,
                                              (BattleEvent*)&ev, 0);
                                BtlDmg_FilterNotifyDamage(((BattleObjAccessor*)sub)->field_3f10, cArt35, 9);
                            }
                        }
                    }
                    if (listActor != attacker) {                       // 0x800EA9A8
                        if (((cf::CActorParam*)listActor)->CActorParam_getStatusTable() &&
                            IdTable_QuerySumFloat((void*)(uintptr_t)((cf::CActorParam*)listActor)->CActorParam_getStatusTable(),
                                          0x13, &out14, &out0c)) {
                            // 0x800EA9F8: counter event (0x5A / 0x13)
                            std::memset(&ev, 0, sizeof(ev));
                            ev.field_10 = out14;
                            ev.field_20 = out0c;
                            ev.field_0C = (u16)cSel5A;
                            ev.field_00 = (u32)(uintptr_t)((BattleObjAccessor*)listActor)->field_3f10;
                            ev.field_04 = (u32)cSel13;
                            ev.field_08 = (u32)cVal1000;
                            ev.field_30 |= 0x1800;
                            func_800EC918((void*)cf::CBattleManager::spInstance, 0,
                                          (EC918_BattleObjAccessor*)listActor,
                                          (BattleEvent*)&ev, 0);
                            BtlDmg_FilterNotifyDamage(((BattleObjAccessor*)listActor)->field_3f10, cSel13, 9);
                        }
                    }
                }
            }
        }
    } else {
        // 0x800EAA88: target is a player -- rescue via the help manager
        if (((BattleObjAccessor*)target)->field_3f00 & 0x2) {
            updateBattleHelpState((void*)lbl_eu_80664A10, target);
        }
    }

    // 0x800EAAA0: shared tail
    CCharVoiceMan_TriggerBattleOrPlainVoice(target);
    dispatchItemLoad(target, 0x2000000);
}

// ---- dispatchBattleMove (0x800E307C) --------------------------------------------
// Move dispatcher for sub-object types 1..6: mirrors the move block onto the
// stack, routes to followUpBattleMove / updateBattleAction / processBattleMove, then applies
// status/event follow-ups keyed on the local copy's field_78 bits.
extern "C" void dispatchBattleMove(void* self, void* actor, void* target, void* move) {
    void* giver = actor;       // the giver
    void* victim = target;
    void* moveArg = move;

    if (victim == nullptr) return;                              // 0x800E30A4

    if (!(((BtlMove*)moveArg)->mFlags78 & 0x10)) {               // 0x800E30A8
        dispatchBattleAI(self, actor, target, move);
    }

    void* sub = ((BtlMove*)moveArg)->mSub;                       // r31 (0x800E30B8)
    if (sub == nullptr) return;
    if (((cf::CAttackParamPrefix*)sub)->unk78 & 0x8000) return;         // 0x800E30C4

    if (((cf::CBattleManager*)self)->CheckUnk84Flag(0x10)) return;                           // 0x800E30D0
    ((cf::CActorParam*)(giver))->CActorParam_applyTargetEvent((cf::CActorParam10Arg*)(moveArg));                                          // 0x800E30F0

    E2A9C_BattleMoveData local = *((E2A9C_BattleMoveData*)moveArg);  // 0x800E3108 copy

    u16 type = ((cf::CAttackParamPrefix*)sub)->unk3C;                    // 0x800E3288
    if ((u32)(type - 4) <= 2) {                              // types 4,5,6
        followUpBattleMove(self, actor, target, move);
    } else if ((u32)(type - 1) <= 1) {                       // types 1,2
        updateBattleAction((cf::CBattleManager*)self, (cf::CfObjectActor*)actor,
                      (cf::CfObjectActor*)target, (E2A9C_BattleMoveData*)move);
    } else if (type == 3) {
        processBattleMove((cf::CBattleManager*)self, actor, target, move);
    }

    if (local.field_78 & 0x2) {                              // 0x800E32F4
        if ((u32)((cf::CActorParam*)(giver))->CActorParam_getStatusCount() == 0) {
            ((cf::CActorParam*)(giver))->CActorParam_resetTensionState((int)(2));
        }
    }

    if (local.field_78 & 0xC00) {                            // 0x800E3334
        bool canCounter = false;
        if (sub != nullptr) {
            if (((cf::CAttackParamPrefix*)sub)->unk78 & 0x1) {
                u16 id28 = ((cf::CAttackParamPrefix*)sub)->unk28;
                if ((u16)(id28 - 0x2A) <= 2 || (u16)(id28 - 0x3D) <= 1 || id28 == 1) {
                    canCounter = true;
                }
            }
        }
        if (!canCounter) {
            // 0x800E3394: 0x91 counter status event
            if (isBattleStatusActive((u8*)giver + 8, 0x91)) {
                BtlStatusEntry* entry = (BtlStatusEntry*)findBattleStatusEntry((u8*)giver + 8, 0x91);
                BattleEventData ev;
                std::memset(&ev, 0, sizeof(ev));
                ev.field_00 = ((cf::CfObjectActor*)giver)->field_0x74;
                ev.field_08 = 0x00008000;
                ev.field_0C = 0xD5;
                ev.field_10 = entry->unk10;
                if (((BtlMove*)moveArg)->mSubId != 0) {
                    func_800EC918((void*)self, (EC918_BattleObjAccessor*)giver,
                                  (EC918_BattleObjAccessor*)giver, (BattleEvent*)&ev, moveArg);
                } else {
                    func_800EC918((void*)self, (EC918_BattleObjAccessor*)giver,
                                  (EC918_BattleObjAccessor*)giver, (BattleEvent*)&ev, &local);
                }
            }
        }
    }

    if (local.field_78 & 0xC00) {                            // 0x800E342C
        if (isBattleStatusActive((u8*)giver + 8, 0xC1)) {
            if ((((cf::CAttackParamPrefix*)sub)->unk78 & 0x2000) &&
                ((u32)((cf::CAttackParamPrefix*)sub)->unk44 != ((cf::CActorParam*)(giver))->CActorParam_getStatsSlot() + 1) &&
                (local.field_74 & 1) && !(local.field_74 & 0x2) &&
                !(local.field_74 & 0x40)) {
                // guarded status -- leave the entry untouched
            } else {
                cf::CBattleStateEntry* entryC1 = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)giver + 8, 0xC1);
                if (entryC1->unk04 != (u32)sub) {
                    ((cf::CBattleState*)((u8*)giver + 8))->CBattleState_clearStatusId(0xC1);
                }
            }
        }
    }

    if (local.field_78 & 0x800) {                            // 0x800E34C8
        if (isBattleStatusActive((u8*)victim + 8, 0xCE)) {
            if (giver != victim) {                                // 0x800E34E8
                bool keep = true;
                if (((cf::CAttackParamPrefix*)sub)->unk78 & 0x2000) {
                    if ((u32)((cf::CAttackParamPrefix*)sub)->unk44 != ((cf::CActorParam*)(giver))->CActorParam_getStatsSlot() + 1) {
                        keep = false;                        // bne -> out
                    }
                }
                if (keep && ((local.field_74 & 1) || (local.field_78 & 0x100))) {
                    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)findTopBattleStatusEntry((u8*)victim + 8, 0xCE);
                    if (entry->unk2C != local.field_80) {      // 0x800E3548
                        ((cf::CBattleState*)((u8*)victim + 8))->CBattleState_applyEventEntry((cf::CBattleStateEntry*)entry);
                    }
                }
            }
        }
    }
}

// Iterates through one of the actor lists (mActorList3 for the flags&2 path,
// else mActorList2) and broadcasts the arts-damage triple (arg2/arg3/arg4 as
// floats) to every idle actor; when arg5 == 1 the broadcast is gated on the
// actor's sub-object id (vtable slot 0x4C) matching arg6.
// Spreads hate (arg2/arg3/arg4 as floats) from target to every idle actor on
// the opposing list (mActorList3 when target is a party member, else
// mActorList2); with checkId == 1 only actors whose sub-object id matches id.
extern "C" void broadcastBattleDamage(void* mgr, void* target, s32 arg2, s32 arg3, s32 arg4,
                              s32 checkId, void* id) {
    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x4000000)) {
        return;
    }

    if (isBattleStatusActive((u8*)target + 8, 0x101)) {
        if (arg2 != 0) arg2 = 1;
        if (arg3 != 0) arg3 = 1;
        if (arg4 != 0) arg4 = 1;
    }

    if (((BattleObjAccessor*)target)->field_3f00 & 0x2) {
        for (reslist<cf::CfObjectActor*>::iterator it = ((cf::CBattleManager*)mgr)->mActorList3.begin();
             it != ((cf::CBattleManager*)mgr)->mActorList3.end(); ++it) {
            cf::CfObjectActor* obj = *it;
            if (obj != target && obj->CActorParam_isBattleLocked() == 0) {
                if (checkId == 1) {
                    if ((void*)static_cast<cf::CfObjectMove&>(*obj).CObjectParam_getSelfObjectId() == id) {
                        obj->CActorParam_addHate((cf::CfObjectActor*)target, (f32)arg2, (f32)arg3, (f32)arg4);
                    }
                } else {
                    obj->CActorParam_addHate((cf::CfObjectActor*)target, (f32)arg2, (f32)arg3, (f32)arg4);
                }
            }
        }
    } else {
        for (reslist<cf::CfObjectActor*>::iterator it = ((cf::CBattleManager*)mgr)->mActorList2.begin();
             it != ((cf::CBattleManager*)mgr)->mActorList2.end(); ++it) {
            cf::CfObjectActor* obj = *it;
            if (obj != target && obj->CActorParam_isBattleLocked() == 0) {
                if (checkId == 1) {
                    if ((void*)static_cast<cf::CfObjectMove&>(*obj).CObjectParam_getSelfObjectId() == id) {
                        obj->CActorParam_addHate((cf::CfObjectActor*)target, (f32)arg2, (f32)arg3, (f32)arg4);
                    }
                } else {
                    obj->CActorParam_addHate((cf::CfObjectActor*)target, (f32)arg2, (f32)arg3, (f32)arg4);
                }
            }
        }
    }
}
// Mirror of the actor vtable used to reach slot 0x2C8 as a direct virtual
// call: MWCC then routes the dispatch through r12 (retail order) instead of a
// general temp register. The 177 placeholder slots (0x04..0x2C4) are never
// called - only the 0x2C8 slot is used, against the actor's real vtable.
// Iterates through a list of actors based on flags in arg1.
// For each actor not matching arg1, calls vtable[0x2C8] with arg1->+0x3f10.
extern "C" void CBattleMan_SendActorEvent(cf::CBattleManager* mgr, BattleObjAccessor* arg1) {
    if (arg1 == nullptr) return;

    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(0x4000000)) return;

    // Iterate through one of the actor lists, calling vtable[0x2C8] on each
    // actor that doesn't match arg1, with arg1->field_3f10 as the argument.
    // The list choice depends on arg1->field_3f00 bit 0x02.
    if (arg1->field_3f00 & 0x02) {
        _reslist_node<cf::CfObjectActor*>* cur = mgr->mActorList3.mStartNodePtr->mNext;
        while (cur != mgr->mActorList3.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            if (actor != (cf::CfObjectActor*)arg1) {
                // Direct vtable[0x2C8] dispatch so MWCC uses r12 (retail order).
                ((cf::CActorParam*)actor)->CActorParam_resetGaugeEntry((void*)arg1->field_3f10);
            }
            cur = cur->mNext;
        }
    } else {
        _reslist_node<cf::CfObjectActor*>* cur = mgr->mActorList2.mStartNodePtr->mNext;
        while (cur != mgr->mActorList2.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            if (actor != (cf::CfObjectActor*)arg1) {
                // Direct vtable[0x2C8] dispatch so MWCC uses r12 (retail order).
                ((cf::CActorParam*)actor)->CActorParam_resetGaugeEntry((void*)arg1->field_3f10);
            }
            cur = cur->mNext;
        }
    }
}
// Mirror vtable routing slots 0x88 and 0x5C4 through MWCC's canonical r12
// virtual-call sequence (cf. ActorVtableMirror). All other slots are unused
// placeholders declared against the enum-cast objects' real vtables.
// ---- refreshBattleActors (0x800EAF6C) --------------------------------------------
// Update dispatch for the battle-manager refresh flags (bits 0x1 player list,
// 0x2 player pairs, 0x4 vision actors, 0x8 enemy scene, 0x10 raygun, 0x20 /
// 0x40 targeted filters). Each enabled step re-filters an enum list created by
// __ct__16CfEnumListHolderFv and broadcasts the given value over vtable slot 0x88 on the
// matching objects.
extern "C" void refreshBattleActors(cf::CBattleManager* self, f32 value, int flags) {

    if (flags & 0x8) {                                       // 0x800EAF78
        Scn_SetTimeScale(gmScene, value);
    }

    CfEnumListHolder h1;
    {
        CfEnumListHolder h2;

        if (flags & 0x1) {                                       // 0x800EAFAC
            startEnumObjects(h1.get(), 0x408, 0);
            for (u32 i = 0; i < ((cf::CVisionEnumList*)h1.get())->count; i++) {
                void* o = getObjectAt(h1.get(), i);
                void* c = __dynamic_cast(o, 0, &__RTTI__Q22cf13CfObjectModel, &__RTTI__Q22cf8CfObject, 0);
                ((cf::CfObject*)c)->CfObject_pushRefreshValue((float)value);
            }
        }

        if (flags & 0x2) {                                       // 0x800EB020
            startEnumObjects(h1.get(), (0x40 << 16) + 0x320, 1);
            startEnumObjects(h2.get(), 0x80000, 0);
            for (u32 i = 0; i < ((cf::CVisionEnumList*)h1.get())->count; i++) {
                void* o = getObjectAt(h1.get(), i);
                void* actorObj = __dynamic_cast(o, 0, &__RTTI__Q22cf13CfObjectModel, &__RTTI__Q22cf8CfObject, 0);
                ((cf::CfObject*)actorObj)->CfObject_pushRefreshValue((float)value);
                for (u32 j = 0; j < ((cf::CVisionEnumList*)h2.get())->count; j++) {
                    void* o2 = getObjectAt(h2.get(), j);
                    void* typeObj = __dynamic_cast(o2, 0, &__RTTI__Q22cf11CfObjectEff, &__RTTI__Q22cf8CfObject, 0);
                    if (((E484_TypeObj*)typeObj)->field_9C == actorObj)
                        ((cf::CfObject*)typeObj)->CfObject_pushRefreshValue((float)value);
                    void* a = MapFx_ResolveOwner(findObjectById((s32)(uintptr_t)actorObj));
                    if (a != nullptr) {
                        void* b = (void*)(uintptr_t)((cf::CfObjectActor*)a)->mActionSrcHandle;
                        if (b != nullptr) {
                            void* c = MapFx_ResolveOwner(findObjectById((s32)(uintptr_t)b));
                            if (c != nullptr) c = (u8*)c + 0x3E9C;
                            if (((E484_TypeObj*)typeObj)->field_9C == c)
                                ((cf::CfObject*)typeObj)->CfObject_pushRefreshValue((float)value);
                        }
                    }
                }
            }
        }

        if (flags & 0x10) {                                      // 0x800EB170
            ((E484_Global6C*)getEffectTask())->field_6C = value;
        }

        if (flags & 0x20) {                                      // 0x800EB180
            startEnumObjects(h1.get(), 0x80000, 0);
            for (u32 i = 0; i < ((cf::CVisionEnumList*)h1.get())->count; i++) {
                void* o = getObjectAt(h1.get(), i);
                void* typeObj = __dynamic_cast(o, 0, &__RTTI__Q22cf11CfObjectEff, &__RTTI__Q22cf8CfObject, 0);
                // int local over the u16 field: retail emits a zero-extending
                // lhz but signed cmpi chains.
                int t = ((E484_TypeObj*)typeObj)->type8C;
                // 0x800EB1CC: four type ranges trigger the broadcast. Written as
                // a sparse switch so MWCC lowers it to the same binary-search
                // comparison tree as retail.
                switch (t) {
                case 0x3D: case 0x3E: case 0x3F: case 0x40: case 0x41:
                case 0x42: case 0x43: case 0x44: case 0x45: case 0x46:
                case 0x47: case 0x48: case 0x49: case 0x4A: case 0x4B:
                case 0x4C:
                case 0x51: case 0x52: case 0x53: case 0x54: case 0x55:
                case 0x56: case 0x57: case 0x58: case 0x59:
                case 0x65: case 0x66:
                case 0x79: case 0x7A: case 0x7B: case 0x7C: case 0x7D:
                    ((cf::CfObject*)typeObj)->CfObject_pushRefreshValue((float)value);
                    break;
                default:
                    break;
                }
            }
        }

        if (flags & 0x40) {                                      // 0x800EB244
            startEnumObjects(h1.get(), 0x80000, 0);
            for (u32 i = 0; i < ((cf::CVisionEnumList*)h1.get())->count; i++) {
                void* o = getObjectAt(h1.get(), i);
                void* typeObj = __dynamic_cast(o, 0, &__RTTI__Q22cf11CfObjectEff, &__RTTI__Q22cf8CfObject, 0);
                int t = ((E484_TypeObj*)typeObj)->type8C;
                if ((u32)(t - 0xA1) <= 3 || (u32)(t - 0xC0) <= 2 || t == 0xB8)
                    ((cf::CfObject*)typeObj)->CfObject_pushRefreshValue((float)value);
            }
        }

        if (flags & 0x4) {                                       // 0x800EB2DC
            void* vision;
            if (cf::CBattleManager::spInstance != 0) {
                vision = getNonZeroSlot1((u8*)&((cf::CBattleManager*)self)->mVision);
            } else {
                vision = nullptr;
            }
            if (vision != nullptr) {
                E484_VisionPair* vp = (E484_VisionPair*)vision;
                void* ownerA = MapFx_ResolveOwner(findObjectById((s32)vp->field_00));
                void* ownerB = MapFx_ResolveOwner(findObjectById((s32)vp->field_04));
    ((cf::CfObjectActor*)ownerA)->CfObject_pushRefreshValue((float)value);
    ((cf::CfObjectActor*)ownerB)->CfObject_pushRefreshValue((float)value);
                startEnumObjects(h2.get(), 0x80000, 0);
                for (u32 i = 0; i < ((cf::CVisionEnumList*)h2.get())->count; i++) {
                    void* typeObj = __dynamic_cast(getObjectAt(h2.get(), i), 0, &__RTTI__Q22cf11CfObjectEff, &__RTTI__Q22cf8CfObject, 0);
                    // Match the cast object's +0x9C pointer against either vision
                    // actor (or its embedded move sub-object at +0x3E9C).
                    if (((E484_TypeObj*)typeObj)->field_9C == static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)ownerA) ||
                        ((E484_TypeObj*)typeObj)->field_9C == static_cast<cf::CfObjectMove*>((cf::CfObjectActor*)ownerB))
                        ((cf::CfObject*)typeObj)->CfObject_pushRefreshValue((float)value);
                    if (ownerA != nullptr) {
                        void* b = (void*)(uintptr_t)((cf::CfObjectActor*)ownerA)->mActionSrcHandle;
                        if (b != nullptr) {
                            void* c = MapFx_ResolveOwner(findObjectById((s32)(uintptr_t)b));
                            if (c != nullptr) c = (u8*)c + 0x3E9C;
                            if (((E484_TypeObj*)typeObj)->field_9C == c)
                                ((cf::CfObject*)typeObj)->CfObject_pushRefreshValue((float)value);
                        }
                    }
                }
            }
        }

    }
}
extern void BtlDmg_FilterNotifyDamage(void* ptr, u32 id, int arg);
// Struct for the third arg of CBattleMan_FireActorEvent918: fields at +0x00, +0x04, +0x08, +0x30
struct Func800EA9A8_Arg5 {
    u32 field_00;
    u32 field_04;
    u32 field_08;
    u8 pad_0C[0x24];
    u32 field_30;
};

void CBattleMan_FireActorEvent918(void* self, BattleObjAccessor* arg4, Func800EA9A8_Arg5* arg5, void* arg6) {
    arg5->field_00 = (u32)arg4->field_3f10;
    arg5->field_04 = (u32)arg6;
    arg5->field_08 = 0x1000;
    arg5->field_30 |= 0x1800;
    func_800EC918((void*)cf::CBattleManager::spInstance, 0, (void*)arg4, (void*)arg5, 0);
    if (arg6 != nullptr) {
        BtlDmg_FilterNotifyDamage(arg4->field_3f10, (u32)arg6, 9);
    }
}
// The 4th argument: battle command event. 0x34 bytes.
struct BattleCmdEvent {
    u32 field_00;       // +0x00 (cleared by cmd 15/16 handlers)
    u32 field_04;       // +0x04 pointer
    u32 field_08;       // +0x08 pointer
    u16 cmd;            // +0x0C
    u16 pad_0E;         // +0x0E
    u32 field_10;       // +0x10 flags / value
    u32 field_14;       // +0x14 (low s16 used by cmd 221/222 as a range end)
    u32 field_18;       // +0x18
    u32 field_1C;       // +0x1C
    f32 field_20;       // +0x20
    f32 field_24;       // +0x24
    f32 field_28;       // +0x28
    u16 field_2C;       // +0x2C (cmd 15/16 copy target)
    u16 field_2E;       // +0x2E
    u32 field_30;       // +0x30 flags
};

// The battle actor layout (actorA/actorB). Only offsets touched here.
struct BattleActor {
    u8  pad_00[0x04];
    void* sub4;         // +0x04 object with vtable slot 0x30
    void* sub8;         // +0x08 object with vtable slots 0x80/0x88/0x5C
    u8  pad_0C[0x15D8]; // up to +0x15E4
    u32 type_15E4;      // +0x15E4 "enemy type" used by the power table
    u8  pad_15E8[0x1D8C]; // up to +0x3374
    u32 flags_3374;     // +0x3374
    u8  pad_3378[0xB88];  // up to +0x3F00
    u32 flags_3F00;     // +0x3F00
    u8  pad_3F04[0x0C];
    u32 field_3F10;     // +0x3F10
    u8  pad_3F14[0x14];
    u16 field_3F28;     // +0x3F28
};

// ---------------------------------------------------------------------------
// Small static helpers (semantic clarity; each maps 1:1 to retail code).
// ---------------------------------------------------------------------------

// Heap-range validity test used twice in the cmd-227 handler and once for the
// stat block in the same handler:
//   ((u32)p & 3) == 0 && (u32)p >= 0x80000000 && (u32)p < 0x93800000
static bool IsValidPtr(const void* p) {
    u32 v = (u32)p;
    return (v & 3) == 0 && v >= 0x80000000u && v < 0x93800000u;
}

// (Former ActorVfunc/Sub4/Sub8 manual-dispatch helpers removed: every slot
// now goes through the owning-class real virtual directly at the call site:
// CActorParam Func100/138/127/129/29/38/49/157/163/164, CObjectState Func11
// via CActorState::unk4, CBattleState Func31/33/22 on the +8 sub-object.)

// ---------------------------------------------------------------------------
// The state check (inlined twice in retail with different constants):
//   call A: byteOff=113, thresh=150   -> gates fail_0x1000
//   call B: byteOff=112, thresh=100   -> gates the % 50 check
// sel is the classifyBattleCommand(cmd) classifier (0..9), obj is actorB->vfunc_224()
// result. Bit table for sel 0..6: {6, 31, 0, 1, 2, 3, 4} (bit 31 is always 0
// for the u8 field; kept faithfully).
// ---------------------------------------------------------------------------
static u32 BattleStateCheck(u32 sel, void* obj, u32 byteOff, s32 thresh) {
    // Bit table for sel 0..6 is {6, 31, 0, 1, 2, 3, 4}; retail pools no such
    // storage, so keep it as branch selects (a static array here would emit
    // a phantom .sdata2 object with no retail counterpart).
    u8* p = (u8*)obj;
#define KY_BITSHIFT(sel) \
    ((u8)((sel) == 0 ? 6 : (sel) == 1 ? 31 : (sel) == 2 ? 0 : (sel) == 3 ? 1 : \
                        (sel) == 4 ? 2 : (sel) == 5 ? 3 : 4))

    if (sel <= 3) {
        return (p[byteOff] >> KY_BITSHIFT(sel)) & 1;
    }
    if (sel <= 9) {
        s16 v = *(s16*)(p + 0x64 + (sel - 4) * 2);   // +100..+110
        if (v < thresh) {
            if (sel <= 6) return (p[byteOff] >> KY_BITSHIFT(sel)) & 1;
            return 0;
        }
        return 1;
    }
    return 0;
#undef KY_BITSHIFT
}

// ---------------------------------------------------------------------------
// emitBattleMoveEvents
// ---------------------------------------------------------------------------
extern "C" s32 emitBattleMoveEvents(void* mgr /*r24*/, void* actorA /*r25*/, void* actorB /*r26*/,
                  void* evtArg /*r27*/, void* statArg /*r28*/) {
    BattleCmdEvent* evt = (BattleCmdEvent*)evtArg;
    cf::CActorParam_UnkStruct1* stat = (cf::CActorParam_UnkStruct1*)statArg;
    // ---- Prologue: classifier object of actorA (used by cmd 213/263). ----
    void* a224 = ((cf::CActorParam*)(actorA))->CActorParam_getBattleParams();   // r30 (kept across the whole body)
    ((cf::CActorParam*)(actorB))->CActorParam_getBattleParams();                // result discarded by retail

    // ---- Sub-object / event fixups. ----
    void* subObj = (void*)evt->field_04;  // r29
    if (subObj == nullptr && stat != nullptr) {
        subObj = stat->unk50;
    }
    if (stat != nullptr) {
        evt->field_2C = stat->unk80;
    }

    // ---- Game-state gate. ----
    getInstance__Q22cf13CfGameManagerFv();
    if (isGlobalCamFlagSet(0x10000000)) return 0;

    // ---- Actor gates. ----
    if (((cf::CActorParam*)(actorB))->CActorParam_isBattleLocked()) return 0;
    if (stat != nullptr && (stat->mFlagsArray[1].flags & 0x1000)) return 0;

    // ---- Sub8 art-state gates (pre-switch). ----
    if (((cf::CBattleState*)((u8*)(actorB) + 8))->CBattleState_getEventMask((u32)(evt->cmd))) goto fail_1000;

    if (((cf::CBattleState*)((u8*)(actorB) + 8))->CBattleState_getStatusMask((u32)(evt->cmd))) {
        // ---- "Hit chance" threshold block. ----
        s32 threshold = 50;                                // r31
        if (evt->cmd <= 18) {
            s32 type = ((BattleActor*)actorB)->type_15E4;
            if (type == 1) {
                threshold = 20;
            } else if (type == 2) {
                threshold -= 20;
            } else if (type == 3) {
                threshold += 20;
            } else if (type == 4) {
                threshold += 50;
            } else if (type == 5) {
                threshold += 50;
            }
            if (((BattleActor*)actorB)->flags_3374 & 0x100) threshold += 30;
            u8 b = ((cf::CBattleManager*)mgr)->mChain.mHead.field_2;
            if (b >= 1 && b <= 24) threshold = 0;
        }
        if (stat != nullptr) {
            if ((u32)(stat->unk84[11] % 100) < (u32)threshold) goto fail_10;
        } else {
            if (mtRand__Q22ml4mathFi(100) < threshold) return 0;
        }
    }

    // ---- Skill 39 block (cmd 17 / same-actor exception). ----
    if (isBattleStatusActive((u8*)actorB + 8, 39) != nullptr) {
        if (actorB != actorA || evt->cmd != 17) {
            if (isBattleEventKind1(evt->cmd)) {
                cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 39);
                if (e != nullptr) {
                    u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
                    if (roll < (u32)e->unk10) goto fail_10;
                }
            }
        }
    }

    // ---- Skill 206 block (physical-attack + stat bit 11 gate). ----
    if (isBattleStatusActive((u8*)actorB + 8, 206) != nullptr) {
        if (stat != nullptr && (stat->mFlagsArray[1].flags & 0x800)) {
            if (isBattleStatusKind1(evt->cmd)) {
                cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findTopBattleStatusEntry((u8*)actorB + 8, 206);
                if ((u32)e->unk10 >= (u32)artsSubGetMax(subObj)) {
                    stat->mFlagsArray[0].flags |= 0x80002000;
                    goto fail_10;
                }
            }
        }
    }

    // ---- Classifier state checks (two inlined copies). ----
    {
        u32 sel = (u32)classifyBattleCommand(evt->cmd);   // r31
        void* b224 = ((cf::CActorParam*)(actorB))->CActorParam_getBattleParams();       // r3, reused by both checks
        if (BattleStateCheck(sel, b224, 113, 150) != 0) goto fail_1000;
        if (BattleStateCheck(sel, b224, 112, 100) != 0) {
            if (stat != nullptr) {
                if ((stat->unk84[10] % 100) < 50) goto fail_10;
            } else {
                if (mtRand__Q22ml4mathFi(100) < 50) goto fail_10;
            }
        }
    }

    // ---- Physical-attack + stat-124 value block. ----
    if ((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable() != nullptr) {
        if (isBattleStatusKind1(evt->cmd)) {
            u32 val;
            if (IdTable_GetSum((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable(), 124, &val)) {
                u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
                if (roll < val) goto fail_10;
            }
        }
    }

    // ========================================================================
    // Big dispatch on the command id (u16 at evt+0x0C).
    // ========================================================================
    switch (evt->cmd) {

    // ---- cmd 4: skill 21 roll. ----
    case 4:
        if (isBattleStatusActive((u8*)actorB + 8, 21) != nullptr) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 21);
            u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < (u32)e->unk10) goto fail_10;
        }
        break;

    // ---- cmd 5: no-op (retail -> default). ----
    case 5:
        break;

    // ---- cmd 6: skill 23 roll. ----
    case 6:
        if (isBattleStatusActive((u8*)actorB + 8, 23) != nullptr) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 23);
            u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < (u32)e->unk10) goto fail_10;
        }
        break;

    // ---- cmd 7: skill 24 roll. ----
    case 7:
        if (isBattleStatusActive((u8*)actorB + 8, 24) != nullptr) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 24);
            u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < (u32)e->unk10) goto fail_10;
        }
        break;

    // ---- cmd 9: skill 26 roll. ----
    case 9:
        if (isBattleStatusActive((u8*)actorB + 8, 26) != nullptr) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 26);
            u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < (u32)e->unk10) goto fail_10;
        }
        break;

    // ---- cmd 8: no-op (retail -> default). ----
    case 8:
        break;
    // ---- cmd 10: no-op (retail -> default). ----
    case 10:
        break;

    // ---- cmd 11: skill 28 roll. ----
    case 11:
        if (isBattleStatusActive((u8*)actorB + 8, 28) != nullptr) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 28);
            u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < (u32)e->unk10) goto fail_10;
        }
        break;

    // ---- cmd 12: skill 29 roll. ----
    case 12:
        if (isBattleStatusActive((u8*)actorB + 8, 29) != nullptr) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 29);
            u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < (u32)e->unk10) goto fail_10;
        }
        break;

    // ---- cmd 16: "break-art" handler (retail 0x141D0). ----
    case 16: {
        int val;
        if (testActorStatus(actorB, &val, 0xA) != 0) {
            if (*(u32*)((u8*)(void*)((cf::CActorParam*)(actorB))->CActorParam_getBattleStats() + 0x78) & 0x08000000) return 0;
        }
        if (((BattleActor*)actorB)->flags_3F00 & 2) {
            // mActorList3 node count (sentinel at mgr+0x48).
            void* sentinel = (void*)((cf::CBattleManager*)mgr)->mActorList3.mStartNodePtr;
            s32 count = 0;
            for (void* cur = *(void**)sentinel; cur != sentinel; cur = *(void**)cur) count++;
            if (count == 0) return 0;
        }
        if (actorA != nullptr) {
            cf::CActorParam_UnkStruct1* tgt = stat != nullptr ? stat : (cf::CActorParam_UnkStruct1*)((cf::CActorParam*)(actorA))->CActorParam_getBattleStats();   // r31
            if (tgt->mFlagsArray[0].flags & 4) return 0;
            if (tgt->mFlagsArray[0].flags & 2) return 0;
            int val2 = *(s32*)((cf::CObjectState*)((cf::CActorState*)(actorB))->unk4)->getStateData();
            if (testActorStatus(actorB, &val2, 0x802) != 0) return 0;
            if (!(evt->field_30 & 0x80)) {
                if (!(evt->field_10 & 4)) {
                    u32 r = 0;                                   // r29
                    if (*(u16*)((cf::CActorParam*)(actorB))->CActorParam_getStateFlags() & 8) r = 1;
                    if (isBattleStatusActive((u8*)actorB + 8, 59) != nullptr) r = 1;
                    if (isBattleStatusActive((u8*)actorB + 8, 50) != nullptr) r = 1;
                    if (r) goto fail_10;
                }
                if (!(evt->field_10 & 5)) {
                    u32 r = 0;                                   // r29 (re-reset per retail)
                    if (isBattleStatusActive((u8*)actorB + 8, 32) != nullptr) {
                        cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 32);
                        u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
                        if (roll < (u32)e->unk10) r = 1;
                    }
                    if (((BattleActor*)actorB)->flags_3374 & 0x80) {
                        if (tgt->mFlagsArray[0].flags & 0x4000) r = 1;
                    }
                    if (!(evt->field_10 & 6)) {
                        if (isBattleStatusActive((u8*)actorB + 8, 16) == nullptr) {
                            if (((BattleActor*)actorB)->flags_3F00 & 4) r = 1;
                        }
                        if (r) goto fail_10;
                    }
                }
                // Skill 154/155 power scaling of evt->+0x20.
                if (isBattleStatusActive((u8*)actorA + 8, 154) != nullptr) {
                    s32 v = *(s32*)((u8*)findBattleStatusEntry((u8*)actorA + 8, 154) + 0x10);
                    f32 ratio = (f32)((f64)(s32)v / (f64)100.0f);
                    evt->field_20 = evt->field_20 * (lbl_eu_80666DD4 + ratio);
                } else if (isBattleStatusActive((u8*)actorA + 8, 155) != nullptr) {
                    s32 v = *(s32*)((u8*)findBattleStatusEntry((u8*)actorA + 8, 155) + 0x10);
                    f32 ratio = (f32)((f64)(s32)v / (f64)100.0f);
                    evt->field_20 = evt->field_20 / (lbl_eu_80666DD4 + ratio);
                }
            }
        } else {
            int val3 = *(s32*)((cf::CObjectState*)((cf::CActorState*)(actorB))->unk4)->getStateData();
            if (testActorStatus(actorB, &val3, 0x802) != 0) return 0;
            if (isBattleStatusActive((u8*)actorB + 8, 50) != nullptr) return 0;
            if (!(evt->field_30 & 0x80)) {
                if (!(evt->field_10 & 4)) {
                    u32 r = 0;
                    if (*(u16*)((cf::CActorParam*)(actorB))->CActorParam_getStateFlags() & 8) r = 1;
                    if (isBattleStatusActive((u8*)actorB + 8, 59) != nullptr) r = 1;
                    if (isBattleStatusActive((u8*)actorB + 8, 50) != nullptr) r = 1;
                    if (r) goto fail_10;
                }
                if (!(evt->field_10 & 5)) {
                    if (isBattleStatusActive((u8*)actorB + 8, 32) != nullptr) {
                        cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 32);
                        u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
                        if (roll < (u32)e->unk10) goto fail_10;
                    }
                }
                if (!(evt->field_10 & 6)) {
                    u32 r = 0;
                    if (isBattleStatusActive((u8*)actorB + 8, 16) == nullptr) {
                        if (((BattleActor*)actorB)->flags_3F00 & 4) r = 1;
                    }
                    if (r) goto fail_10;
                }
            }
        }
        evt->field_00 = 0;
        evt->field_04 = 0;
        break;
    }

    // ---- cmd 15: "talent art" handler (retail 0x13E48). ----
    case 15: {
        int val;
        if (testActorStatus(actorB, &val, 0xA) != 0) {
            if (*(u32*)((u8*)(void*)((cf::CActorParam*)(actorB))->CActorParam_getBattleStats() + 0x78) & 0x08000000) return 0;
        }
        if (((BattleActor*)actorB)->flags_3F00 & 2) {
            void* sentinel = (void*)((cf::CBattleManager*)mgr)->mActorList3.mStartNodePtr;
            s32 count = 0;
            for (void* cur = *(void**)sentinel; cur != sentinel; cur = *(void**)cur) count++;
            if (count == 0) return 0;
        }
        if (actorA != nullptr) {
            cf::CActorParam_UnkStruct1* tgt = stat != nullptr ? stat : (cf::CActorParam_UnkStruct1*)((cf::CActorParam*)(actorA))->CActorParam_getBattleStats();   // r31
            if (tgt->mFlagsArray[0].flags & 4) return 0;
            if (tgt->mFlagsArray[0].flags & 2) return 0;
            int val2 = *(s32*)((cf::CObjectState*)((cf::CActorState*)(actorB))->unk4)->getStateData();
            if (testActorStatus(actorB, &val2, 0x802) != 0) return 0;
            if (!(evt->field_10 & 4)) {
                u32 r = 0;                                   // r29
                if (*(u16*)((cf::CActorParam*)(actorB))->CActorParam_getActorModeFlags() & 8) r = 1;
                if (isBattleStatusActive((u8*)actorB + 8, 58) != nullptr) r = 1;
                if (isBattleStatusActive((u8*)actorB + 8, 50) != nullptr) r = 1;
                if (r) goto fail_1000;
            }
            if (!(evt->field_10 & 5)) {
                u32 r = 0;                                   // r29 (re-reset per retail)
                if (isBattleStatusActive((u8*)actorB + 8, 33) != nullptr) {
                    cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 33);
                    u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
                    if (roll < (u32)e->unk10) r = 1;
                }
                if (((BattleActor*)actorB)->flags_3374 & 0x80) {
                    if (tgt->mFlagsArray[0].flags & 0x4000) r = 1;
                }
                if (!(evt->field_10 & 6)) {
                    if (isBattleStatusActive((u8*)actorB + 8, 18) == nullptr &&
                        isBattleStatusActive((u8*)actorB + 8, 16) == nullptr) {
                        if (((BattleActor*)actorB)->flags_3F00 & 4) r = 1;
                    }
                    if (r) goto fail_10;
                }
            }
        } else {
            int val3 = *(s32*)((cf::CObjectState*)((cf::CActorState*)(actorB))->unk4)->getStateData();
            if (testActorStatus(actorB, &val3, 0x802) != 0) return 0;
            if (!(evt->field_10 & 4)) {
                u32 r = 0;
                if (*(u16*)((cf::CActorParam*)(actorB))->CActorParam_getActorModeFlags() & 8) r = 1;
                if (isBattleStatusActive((u8*)actorB + 8, 58) != nullptr) r = 1;
                if (isBattleStatusActive((u8*)actorB + 8, 50) != nullptr) r = 1;
                if (r) goto fail_1000;
            }
            if (!(evt->field_10 & 5)) {
                if (isBattleStatusActive((u8*)actorB + 8, 33) != nullptr) {
                    cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 33);
                    u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
                    if (roll < (u32)e->unk10) goto fail_10;
                }
            }
            if (!(evt->field_10 & 6)) {
                u32 r = 0;
                if (isBattleStatusActive((u8*)actorB + 8, 18) == nullptr &&
                    isBattleStatusActive((u8*)actorB + 8, 16) == nullptr) {
                    if (((BattleActor*)actorB)->flags_3F00 & 4) r = 1;
                }
                if (r) goto fail_10;
            }
        }
        evt->field_00 = 0;
        evt->field_04 = 0;
        break;
    }

    // ---- cmd 17: skill 34 roll (same-actor exception). ----
    case 17:
        if (isBattleStatusActive((u8*)actorB + 8, 34) != nullptr) {
            if (actorB != actorA) {
                cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 34);
                u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
                if (roll < (u32)e->unk10) goto fail_10;
            }
        }
        break;

    // ---- cmd 18: no-op (retail -> default). ----
    case 18:
        break;
        if (isBattleStatusActive((u8*)actorB + 8, 34) != nullptr) {
            if (actorB != actorA) {
                cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 34);
                u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
                if (roll < (u32)e->unk10) goto fail_10;
            }
        }
        break;

    // ---- cmd 19: skill 36 roll. ----
    case 19:
        if (isBattleStatusActive((u8*)actorB + 8, 36) != nullptr) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 36);
            u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < (u32)e->unk10) goto fail_10;
        }
        break;

    // ---- cmds 60, 61: retail 0x800ECCCC. ----
    case 60:
    case 61:
        if ((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable() != nullptr) {
            if (Counter_TestBit((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable(), 0x66)) goto fail_10;
        }
        if (isBattleStatusActive((u8*)actorB + 8, 0x26) != nullptr) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 0x26);
            u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < (u32)e->unk10) goto fail_10;
        }
        if (stat != nullptr) {
            if ((u32)(stat->unk84[11] % 100) >= evt->field_10) goto fail_10;
            if (actorA != actorB) {
                f32 v = ((cf::CActorParam*)(actorB))->CActorParam_getDamageScale();
                if (stat->unk5C < v) stat->unk5C = v;
            }
            stat->mFlagsArray[0].flags |= 0x90000000;
            isBattleStatusActive((u8*)actorB + 8, 252);
            isBattleStatusActive((u8*)actorB + 8, 256);
        }
        break;

    // ---- cmd 82: retail 0x800ECBB8. ----
    case 82:
        if ((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable() != nullptr) {
            if (Counter_TestBit((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable(), 0x7B)) goto fail_10;
            if (Counter_TestBit((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable(), 0x66)) goto fail_10;
        }
        break;

    // ---- cmds 83, 87: retail 0x800ECC88. ----
    case 83:
    case 87:
        if ((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable() != nullptr) {
            if (Counter_TestBit((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable(), 0x66)) goto fail_10;
        }
        if (isBattleStatusActive((u8*)actorB + 8, 0x26) != nullptr) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 0x26);
            u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < (u32)e->unk10) goto fail_10;
        }
        if (stat != nullptr) {
            if ((u32)(stat->unk84[11] % 100) >= evt->field_10) goto fail_10;
            if (actorA != actorB) {
                f32 v = ((cf::CActorParam*)(actorB))->CActorParam_getDamageScale();
                if (stat->unk5C < v) stat->unk5C = v;
            }
            stat->mFlagsArray[0].flags |= 0x90000000;
            isBattleStatusActive((u8*)actorB + 8, 252);
            isBattleStatusActive((u8*)actorB + 8, 256);
        }
        break;

    // ---- cmd 84: retail 0x800ECC20. ----
    case 84:
        if ((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable() != nullptr) {
            if (Counter_TestBit((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable(), 0x7D)) goto fail_10;
            if (Counter_TestBit((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable(), 0x66)) goto fail_10;
        }
        break;

    // ---- cmd 102: retail 0x800ECAF8: skills 134, 136. ----
    case 102:
        if (isBattleStatusActive((u8*)actorB + 8, 134) != nullptr) {
            if (*(s32*)((u8*)findBattleStatusEntry((u8*)actorB + 8, 134) + 0x10) >= 100) goto fail_10;
        }
        if (isBattleStatusActive((u8*)actorB + 8, 136) != nullptr) {
            if (*(s32*)((u8*)findBattleStatusEntry((u8*)actorB + 8, 136) + 0x10) >= 100) goto fail_10;
        }
        break;

    // ---- cmd 103: retail 0x800ECB28: skill 136. ----
    case 103:
        if (isBattleStatusActive((u8*)actorB + 8, 136) != nullptr) {
            if (*(s32*)((u8*)findBattleStatusEntry((u8*)actorB + 8, 136) + 0x10) >= 100) goto fail_10;
        }
        break;

    // ---- cmd 104: retail 0x800ECB58: skills 138, 140. ----
    case 104:
        if (isBattleStatusActive((u8*)actorB + 8, 138) != nullptr) {
            if (*(s32*)((u8*)findBattleStatusEntry((u8*)actorB + 8, 138) + 0x10) >= 100) goto fail_10;
        }
        if (isBattleStatusActive((u8*)actorB + 8, 140) != nullptr) {
            if (*(s32*)((u8*)findBattleStatusEntry((u8*)actorB + 8, 140) + 0x10) >= 100) goto fail_10;
        }
        break;

    // ---- cmd 105: retail 0x800ECB88: skill 140 + status 0x7B/0x66. ----
    case 105:
        if (isBattleStatusActive((u8*)actorB + 8, 140) != nullptr) {
            if (*(s32*)((u8*)findBattleStatusEntry((u8*)actorB + 8, 140) + 0x10) >= 100) goto fail_10;
        }
        if ((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable() != nullptr) {
            if (Counter_TestBit((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable(), 0x7B)) goto fail_10;
            if (Counter_TestBit((void*)((cf::CActorParam*)(actorB))->CActorParam_getStatusTable(), 0x66)) goto fail_10;
        }
        break;

    // ---- cmds 213, 263: knock-back / damage multiplier calculation. ----
    case 213:
    case 263: {
        if (stat == nullptr) break;
        // f29 = 0.01f * (f64)(s32)stat->unk84[0]   (kept live across the vfunc calls)
        f64 scaled = (f64)0.01f * (f64)(s32)stat->unk84[0];
        // first vfunc_C: art level of sub-object +0x84 vtable[0x0C]
        s32 lvl1 = artsSubGetMax(subObj) - 1;
        s32 v1 = *(s16*)((u8*)subObj + 0x38) + *(u8*)((u8*)subObj + 0x6C) * lvl1;       // r31
        // second vfunc_C: recompute level (retail calls it again)
        s32 lvl2 = artsSubGetMax(subObj) - 1;
        s32 v2 = *(s16*)((u8*)subObj + 0x3A) + *(u8*)((u8*)subObj + 0x6D) * lvl2;       // r0
        // scaled *= (v2 - v1)
        f64 power = scaled * (f64)(s32)(v2 - v1);
        // round-half-away-from-zero (retail: +0.5 / -0.5 doubles + fctiwz)
        f64 rnd = power > 0.0 ? 0.5 : -0.5;
        s32 rounded = (s32)(power + rnd);                                // f29 (as double) afterwards
        // recompute v2 with a THIRD vfunc_C call, then store the new power.
        s32 lvl3 = artsSubGetMax(subObj) - 1;
        s32 v2b = *(s16*)((u8*)subObj + 0x3A) + *(u8*)((u8*)subObj + 0x6D) * lvl3;
        stat->unk5C = (f32)((f64)(s32)*(s16*)((u8*)a224 + 0x30) *
                               ((f64)0.01f * ((f64)(s32)rounded + (f64)(s32)v2b)));
        // accumulate from the damage helper.
        f32 dmg = calcBattleDamage(actorA, actorB, stat);
        f32 f31 = stat->unk58 + dmg;
        if (((BattleActor*)actorA)->flags_3F00 & 2) {
            if (((BattleActor*)actorA)->field_3F28 == 5) {
                s32 v = (s32)(((cf::CActorParam*)(actorA))->CActorParam_getArtsGauge() + (f64)(s32)*(s16*)((u8*)subObj + 0x34));
                if (v < 0) v = 0;
                if (v > 100) v = 100;
                f31 += (f64)(s32)(50 - v) / (f64)200.0f;
            }
        }
        if (*(u16*)((u8*)subObj + 0x48) == 263) {
            s32 count = 0;
            for (f32 x = lbl_eu_80666DDC; x < 32.0f; x += lbl_eu_80666DD4) {
                void* o = (void*)((cf::CBattleState*)((u8*)(actorB) + 8))->CBattleState_getEventEntry((int)((s32)(f64)x));
                if (*(u16*)((u8*)o + 0xC) != 0) count++;
            }
            f31 += (f64)0.1f * (f64)(s32)count;
        }
        if (((BattleActor*)actorA)->flags_3F00 & 2) {
            s32 roll = stat->unk84[5] % 100;
            s32 need = sStatusCountTable[((cf::CActorParam*)(actorA))->CActorParam_getStatusCount()];
            if (roll < need) {
                f31 += k0_25f;
                stat->mFlagsArray[0].flags |= 0x80000200;
            }
        }
        stat->unk5C = (stat->unk5C * f31) * -1.0f;   // retail: fmuls f1,f1,f31; fmuls f0,f1,-1.0f
        break;
    }

    // ---- cmd 220: skill 38 roll + stat/vision updates. ----
    case 220:
        if (isBattleStatusActive((u8*)actorB + 8, 38) != nullptr) {
            cf::CBattleStateEntry* e = (cf::CBattleStateEntry*)findBattleStatusEntry((u8*)actorB + 8, 38);
            u32 roll = stat != nullptr ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
            if (roll < (u32)e->unk10) goto fail_10;
        }
        if (stat != nullptr) {
            if ((stat->unk84[11] % 100) >= evt->field_10) return 0;
            // Both arms of the sub-object type split are identical in retail
            // (0x148C0 and 0x14904); kept as two blocks to mirror the codegen.
            if (subObj != nullptr && *(u16*)((u8*)subObj + 0x3C) == 3) {
                if (actorA != actorB) {
                    f32 v = ((cf::CActorParam*)(actorB))->CActorParam_getDamageScale();
                    if (stat->unk5C < v) stat->unk5C = v;
                }
            } else {
                if (actorA != actorB) {
                    f32 v = ((cf::CActorParam*)(actorB))->CActorParam_getDamageScale();
                    if (stat->unk5C < v) stat->unk5C = v;
                }
            }
        } else {
            if (mtRand__Q22ml4mathFi(100) >= (s32)evt->field_10) goto fail_10;
        }
        if (stat != nullptr) {
            stat->mFlagsArray[0].flags |= 0x90000000;
            isBattleStatusActive((u8*)actorB + 8, 252);
            isBattleStatusActive((u8*)actorB + 8, 256);
        }
        break;

    // ---- cmd 223: damage span calculation (retail 0x800ECE70). ----
    case 223: {
        if (stat == nullptr) break;
        s32 span = (s32)(s16)evt->field_14 - (s32)evt->field_10 + 1;
        s32 base = (s32)evt->field_10 + (stat->unk84[11] % span);       // r29
        stat->unk60 = (f32)(f64)(s32)base;
        f32 dmg = calcBattleDamage(actorA, actorA, stat);
        s32 v = (s32)((f64)(s32)base * (f64)(stat->unk58 + dmg));
        stat->unk64 = (f32)(f64)(s32)v;
        break;
    }

    // ---- cmd 224: damage calculation via sub-object art levels (0x14A1C). ----
    case 224: {
        if (stat == nullptr) break;
        s32 base = *(s16*)((u8*)subObj + 0x4A) +
                   *(u8*)((u8*)subObj + 0x6F) * (artsSubGetMax(subObj) - 1);   // r29
        s32 v = (s32)((f64)0.01f * (f64)(s32)base * (f64)((cf::CActorParam*)(actorB))->CActorParam_getDamageScale());
        stat->unk60 = (f32)(f64)(s32)v;
        f32 dmg = calcBattleDamage(actorA, actorA, stat);
        s32 v2 = (s32)((f64)(s32)v * (f64)(stat->unk58 + dmg));
        stat->unk64 = (f32)(f64)(s32)v2;
        break;
    }

    // ---- cmd 227: chain-break / recursive re-dispatch. ----
    case 227: {
        if (((u32)actorA & 3) != 0) break;
        if ((u32)actorA < 0x80000000u) break;
        if ((u32)actorA >= 0x93800000u) break;
        if (((u32)actorB & 3) != 0) break;
        if ((u32)actorB < 0x80000000u) break;
        if ((u32)actorB >= 0x93800000u) break;

        void* a298 = (void*)((cf::CActorParam*)(actorA))->CActorParam_getBattleStats();                       // r31
        u32 brk = 0;                                             // r29
        if (*(u16*)((cf::CActorParam*)(actorB))->CActorParam_getActorModeFlags() == 7) brk = 1;
        s32 d = (s32)((cf::CActorParam*)(actorB))->CActorParam_getActorLevel() - (s32)((cf::CActorParam*)(actorA))->CActorParam_getActorLevel();   // r30/r3
        if (d > 10) brk = 1;

        if (brk == 0) {
            int val = *(s32*)((cf::CObjectState*)((cf::CActorState*)(actorB))->unk4)->getStateData();
            if (testActorStatus(actorB, &val, 0x802) != 0) brk = 1;
            else if (isBattleStatusActive((u8*)actorB + 8, 50) != nullptr) brk = 1;
            else {
                // power table by enemy type (retail f29 loads).
                f32 power = lbl_eu_80666DDC;                     // lbl_eu_80666DDC
                s32 type = ((BattleActor*)actorB)->type_15E4;
                if (type == 1) power = k75_0f;          // 75
                else if (type == 2) power = k50_0f;     // 50
                else if (type == 3) power = k25_0f;     // 25
                else if (type == 4) power = k15_0f;     // 15
                else if (type == 5) power = k10_0f;     // 10
                void* sub = *(void**)((u8*)a298 + 0x50);
                if (IsValidPtr(sub)) {
                    power += k2_5f * (f64)(s32)artsSubGetMax(sub);   // 2.5f, fmadds
                }
                u32 roll = IsValidPtr(stat) ? stat->unk84[12] : (u32)mtRand__Q22ml4mathFi(100);
                if ((u32)(s32)(f64)power <= (u32)roll) brk = 1;
            }
        }

        // Build the recursive event copy (retail copies field-by-field and
        // converts evt->field_10 via the 0x4330 double trick).
        BattleCmdEvent buf;
        buf.field_00 = evt->field_00;
        buf.field_04 = evt->field_04;
        buf.field_08 = evt->field_08;
        buf.field_10 = 3;
        buf.field_14 = evt->field_14;
        buf.field_18 = evt->field_18;
        buf.field_1C = evt->field_1C;
        buf.field_20 = (f32)(f64)(s32)evt->field_10;
        buf.field_24 = evt->field_24;
        buf.field_28 = evt->field_28;
        buf.field_2C = evt->field_2C;
        buf.field_2E = evt->field_2E;
        buf.field_30 = evt->field_30;
        buf.cmd = 16;

        if (brk) {
            buf.cmd = 6;
            if (!emitBattleMoveEvents(mgr, actorA, actorB, &buf, stat)) goto fail_10;
        } else {
            buf.field_30 |= 0x800;
            if (emitBattleMoveEvents(mgr, actorA, actorB, &buf, stat) == 0) {
                buf.cmd = 6;
                buf.field_30 &= ~0x400;
                if (!emitBattleMoveEvents(mgr, actorA, actorB, &buf, stat)) goto fail_10;
            }
        }
        break;
    }

    // ---- no-op cases and the default dispatch straight to 0x14DD4. ----
    default:
        break;
    }

    // ---- Shared default block (retail 0x14DD4): vision sub-object command
    // recording. Reached by every case `break` AND the default case.
    {
        void* vobj = nullptr;
        if (cf::CBattleManager::spInstance != 0) {
            vobj = getNonZeroSlot1((u8*)&((cf::CBattleManager*)mgr)->mVision);            // &mVision
        }
        if (vobj != nullptr && actorA != nullptr) {
            if (((BattleActor*)actorA)->field_3F10 == *(u32*)((u8*)vobj + 0) &&
                ((BattleActor*)actorB)->field_3F10 == *(u32*)((u8*)vobj + 4)) {
                if (evt->field_2E == 0 || !(evt->field_30 & 2)) {
                    if (evt->cmd != 220 && isBattleStatusKind1(evt->cmd)) {
                        if (*(u32*)((u8*)vobj + 0x828) == 0) {
                            *(u32*)((u8*)vobj + 0x828) = evt->cmd;
                        } else {
                            *(u32*)((u8*)vobj + 0x82C) = evt->cmd;
                        }
                    }
                }
            }
        }
    }
    return 1;

fail_10:
    // retail 0x14E7C
    if (stat != nullptr) stat->mFlagsArray[0].flags |= 0x80000010;
    return 0;

fail_1000:
    // retail 0x14E9C
    if (stat != nullptr) stat->mFlagsArray[0].flags |= 0x80001000;
    return 0;
}


// Battle damage menu functions (CMenuBattleDamage)
extern void BtlDmg_SetField779(u8 val);
extern void BtlDmg_SetByte778(u8 val);
extern void BtlDmg_SetByte777(u8 val);
extern void BtlDmg_SetDamageType(u8 val);
extern void BtlDmg_SetDamageDir(u8 val);
extern void BtlDmg_NotifyDamage(void* ptr, u32 val);

// Handles battle event. Sets flags on arg4, checks arg3 conditions,
// and updates the battle damage display.
// Returns 0 always (retail: li r3, 0 before blr).
extern "C" s32 CBattleMan_ApplyHitStop9(void* arg1, BattleObjAccessor* accessor, void* arg3, void* arg4) {
    // arg1 (r3) is unused by the logic but was stored in r31 per retail
    BattleEvent* evt = (BattleEvent*)arg3;
    BattleTargetData* target = (BattleTargetData*)arg4;

    if (target != nullptr) {
        u32* flags = &target->field_74;
        *flags = *flags | 0x80000000 | 0x1000;
    }

    u32 arg3Flags = evt->field_30;
    if (arg3Flags & 0x800) return 0;
    if (arg3Flags & 0x2000) return 0;

    void* subObj = (target != nullptr) ? target->artsData : nullptr;
    if (subObj == nullptr || (s32)((BattleSrcObjAccessor*)subObj)->type_3c == 3) {
        BtlDmg_SetField779(0);
        BtlDmg_SetByte778(0);
        BtlDmg_SetByte777(0);
        BtlDmg_SetDamageType(9);
        BtlDmg_SetDamageDir(0);
        BtlDmg_NotifyDamage(accessor->field_3f10, 0);
    }
    return 0;
}

// Similar to CBattleMan_ApplyHitStop9 but with additional checks on arg3->+0x2e and arg3->+0x30.
// Sets flags on arg4, checks conditions, and updates the battle damage display.
// Returns 0 always.
extern "C" s32 CBattleMan_ApplyHitStop10(void* arg1, BattleObjAccessor* accessor, void* arg3, void* arg4) {
    BattleEvent* evt = (BattleEvent*)arg3;
    BattleTargetData* target = (BattleTargetData*)arg4;

    if (target != nullptr) {
        u32* flags = &target->field_74;
        *flags = *flags | 0x80000000 | 0x10;

        // Check if bit 0x1000 is now set (retail: rlwinm. 0, 19, 19)
        if (*flags & 0x1000) return 0;
    }

    u32 arg3Flags = evt->field_30;
    if (arg3Flags & 0x800) return 0;
    if (arg3Flags & 0x2000) return 0;

    // Check arg3->+0x2e: if non-zero AND 0x402 bits not set, skip to ret0
    u16 val2e = evt->field_2E;
    if (val2e != 0 && (arg3Flags & 0x402) == 0) {
        // goto ret0 (skip function calls)
    } else {
        void* subObj = (target != nullptr) ? target->artsData : nullptr;
        if (subObj == nullptr || (s32)((BattleSrcObjAccessor*)subObj)->type_3c == 3) {
            BtlDmg_SetField779(0);
            BtlDmg_SetByte778(0);
            BtlDmg_SetByte777(0);
            BtlDmg_SetDamageType(8);
            BtlDmg_SetDamageDir(0);
            BtlDmg_NotifyDamage(accessor->field_3f10, 0);
        }
    }

    return 0;
}
// Stack buffer for CBattleMan_ClearEventNotify: 0x34 bytes, accessed as:
// +0x00: padding
// +0x0C: u16 (stores arg3)
// +0x30: u32 (bit 0 set)
struct Func800F38E0_StackBuf {
    u8 pad_00[0x0C];
    u16 field_0C;
    u8 pad_0E[0x22];
    u32 field_30;
};

void CBattleMan_ClearEventNotify(void* self, u32 arg2, u16 arg3) {
    Func800F38E0_StackBuf buf;
    std::memset(&buf, 0, sizeof(buf));
    buf.field_0C = arg3;
    buf.field_30 |= 1;
    func_800EC918(self, 0, (void*)arg2, &buf, 0);
}
// Dispatches battle event calls to actor objects based on the dispatch-table
// entry at sBattleEventTable[idx]: selector 0 -> obj1, 1 -> obj2, 2 -> both,
// anything else -> every entry of the manager's second actor list.
// Signature matches the single shared decl on CBattleManager.hpp; the
// typed views are recovered up front (pointer casts emit no code).
// f3970_tbl is an UNDEF alias for sBattleEventTable used only by this
// function (see note at its use); UNIT_RULES add_symbols anchors it at
// the table's address for the link.
extern BattleTableEntry f3970_tbl[];
void func_800F3970(void* self, void* obj1, void* obj2, s32 idx, s32 addVal) {
    // Declaration order drives the saved-register colors. Signed selector so
    // the dispatch compares with retail cmpwi (cmpli would come from unsigned).
    // NOTE: indexed through f3970_tbl (an UNDEF alias of sBattleEventTable,
    // anchored by UNIT_RULES add_symbols): with the table defined in-TU,
    // MWCC -ipa miscompiles entry->selector to arr[0] here, so this TU's
    // dispatch keeps the extern-array codegen while linking to the real table.
    const BattleTableEntry* entry = &f3970_tbl[idx];
    s32 selector = entry->selector;
    s32 val3 = entry->val;
    s32 val2 = -1;

    if (val3 == 0x3e7) {
        val2 = 4;
        val3 = 0;
    }

    val3 += addVal;

    // Read the byte once: retail keeps it in a saved register across the
    // lookup call (extsb re-uses it for both the test and the multiply).
    s8 byteVal = f3970_tbl[idx].byteVal;
    if (byteVal != 0) {
        // Cross-actor id lookup (CfGameManager); feeds a /1000 multiplier.
        s32 result = getTableValueByPair__Q22cf13CfGameManagerFv(
            ((BattleObjAccessor*)obj1)->field_3f28,
            ((BattleObjAccessor*)obj2)->field_3f28);
        if (result != -1) {
            // mulhw 0x10624DD3 with >>38 is the signed /1000 magic (not /100).
            val3 += (result / 1000) * (s32)byteVal;
        }
    }

    if (selector == 0) {
        if (val2 != -1) {
            ((cf::CActorParam*)obj1)->CActorParam_resetTensionState(val2);
            ((cf::CActorParam*)obj1)->CActorParam_raiseTensionValue(val3);
        } else {
            ((cf::CActorParam*)obj1)->CActorParam_accumulateTension(val3);
        }
    } else if (selector == 1) {
        if (val2 != -1) {
            ((cf::CActorParam*)obj2)->CActorParam_resetTensionState(val2);
            ((cf::CActorParam*)obj2)->CActorParam_raiseTensionValue(val3);
        } else {
            ((cf::CActorParam*)obj2)->CActorParam_accumulateTension(val3);
        }
    } else if (selector == 2) {
        if (val2 != -1) {
            ((cf::CActorParam*)obj1)->CActorParam_resetTensionState(val2);
            ((cf::CActorParam*)obj1)->CActorParam_raiseTensionValue(val3);
        } else {
            ((cf::CActorParam*)obj1)->CActorParam_accumulateTension(val3);
        }

        if (val2 != -1) {
            ((cf::CActorParam*)obj2)->CActorParam_resetTensionState(val2);
            ((cf::CActorParam*)obj2)->CActorParam_raiseTensionValue(val3);
        } else {
            ((cf::CActorParam*)obj2)->CActorParam_accumulateTension(val3);
        }
    } else {
        // Iterate every node of mActorList2, dispatching to each item.
        _reslist_node<cf::CfObjectActor*>* cur =
            ((cf::CBattleManager*)self)->mActorList2.mStartNodePtr->mNext;
        while (cur != ((cf::CBattleManager*)self)->mActorList2.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            if (val2 != -1) {
                ((cf::CActorParam*)actor)->CActorParam_resetTensionState(val2);
                ((cf::CActorParam*)actor)->CActorParam_raiseTensionValue(val3);
            } else {
                ((cf::CActorParam*)actor)->CActorParam_accumulateTension(val3);
            }
            cur = cur->mNext;
        }
    }
}
void CBattleMan_SetPartyFlagReset(cf::CBattleManager* mgr, u32 arg) {
    mgr->setPartyMaskFlag(2, arg);
    if (arg != 0) {
        std::memset(&mgr->unk94, 0, sizeof(cf::CBattleManager_Struct2));
    }
}
// Searches through unk94 slot array for a matching key. If found, increments count.
// If not found, inserts into the first empty slot (key==0) with count=1.
void CBattleMan_BumpUnk94Key(cf::CBattleManager* mgr, s32 key) {
    // Gate: vtable slot 0x28 (CheckUnk84Flag) with mask 2.
    if (!mgr->CheckUnk84Flag(2)) return;

    // Scan all 32 slots for a matching key (increment its count), else
    // remember the first empty slot and insert there.
    cf::CBattleManager_Struct1* slot = mgr->unk94.unk0;
    s32 best = -1;

    for (s32 i = 0; i < 32; i++) {
        if (slot->key == key) {
            slot->count++;
            return;
        }
        if (slot->key == 0 && best < 0) {
            best = i;
        }
        slot++;
    }

    // Insert into the first empty slot: store the key and bump the count
    // (retail lbz/addi/stb read-modify-write).
    if (best == -1) return;
    cf::CBattleManager_Struct1* chosen = &mgr->unk94.unk0[best];
    chosen->key = key;
    chosen->count++;
}
// Searches for an actor with field_0x15F0 matching arg1.
// Pass 1 scans mActorList3 directly; pass 2 counts mActorList3 (empty list
// falls through); pass 3 scans mActorList1, resolving each actor's move
// sub-object probe through findObjectById/MapFx_ResolveOwner.
// Returns 1 if found, 0 otherwise.
s32 CBattleMan_FindActorByKind(cf::CBattleManager* mgr, s32 arg1) {
    // Pass 1: search mActorList3 for a direct type-id match.
    _reslist_node<cf::CfObjectActor*>* end = mgr->mActorList3.mStartNodePtr;
    _reslist_node<cf::CfObjectActor*>* cur = end->mNext;
    while (cur != end) {
        if ((s32)cur->mItem->unk15F0 == arg1)
            return 1;
        cur = cur->mNext;
    }

    // Pass 2: count mActorList3; a non-empty list means no candidate exists.
    // Pass 3 only runs when the list was empty; both fall through to the
    // shared `return 0`.
    s32 count = 0;
    BMSentinelView* view3 = (BMSentinelView*)mgr;
    _reslist_node<cf::CfObjectActor*>* sent3 = view3->sentinel;
    cur = sent3->mNext;
    while (cur != sent3) {
        count++;
        cur = cur->mNext;
    }

    if (count == 0) {
        // Pass 3: scan mActorList1 via each actor's move sub-object probe.
        cur = mgr->mActorList1.mStartNodePtr->mNext;
        while (cur != mgr->mActorList1.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            s32 vresult = ((cf::CfObjectActor*)actor)->CObjectParam_getSelfObjectId();
            if (vresult != 0) {
                cf::CfObjectActor* found =
                    (cf::CfObjectActor*)MapFx_ResolveOwner(findObjectById(vresult));
                if (found != nullptr && (found->unk64 & 4) &&
                    (s32)found->unk15F0 == arg1) {
                    return 1;
                }
            }
            cur = cur->mNext;
        }
    }

    return 0;
}


void CBattleMan_RaiseBattleFlag10(cf::CBattleManager* mgr) {
    mgr->CBattleMan_ClearStateMask(0x10);
    gmGameFlags |= 0x10000000;
}

void CBattleMan_DropBattleFlag10(cf::CBattleManager* mgr) {
    mgr->CBattleMan_ClearStateMask(0x10);
    gmGameFlags &= ~0x10000000;
}
// Performs battle cleanup: chain maintenance, enum list iteration with virtual calls,
// actor list flag updates, and action cleanup.
// Retail linker names for helpers called from CBattleMan_RefreshChainEnum / CBattleMan_OnActorsEmpty.
extern "C" void CPartsChange_ResetBattleEntry(void*, u32, u32);
extern "C" void CItem_clearSharedBox();
extern "C" s32 CfRes_checkFlags_48000();
class IUIWindow;
extern "C" IUIWindow* UIWin_CreateB4790Win(u32, u32);
extern "C" void CCharVoiceMan_EnqueueItemVoice(void*);


void CBattleMan_RefreshChainEnum(cf::CBattleManager* mgr) {
    // Chain maintenance
    extern void CChain_ForwardVoiceClear(cf::CChain*);
    CChain_ForwardVoiceClear(&mgr->mChain);

    // Setup enum list holder
    {
        CfEnumListHolder holder;
        cf::CfObjEnumList* list = holder.get();
        startEnumObjects(list, 0x20, 0);

        // Loop with check at bottom, using unsigned comparison to match retail cmplw.
        // element declared first: retail claims r31 (first-born saved) for it,
        // r30 for the counter.
        void* element = nullptr;
        u32 i = 0;
        goto check;
    loop:
        {
            element = MapFx_ResolveOwner(getObjectAt(holder.get(), i));
            // Retail uses canonical r12 virtual dispatch (slots 0xB0/0xB8/0x314).
            ((cf::CActorParam*)element)->CActorParam_setupActorState();
            ((cf::CActorParam*)element)->CActorParam_refreshBattleStatus();
            ((cf::CActorParam*)element)->CActorParam_initStatusCounter();
            i++;
        }
    check:
        {
            cf::CfObjEnumList* list = holder.get();
            if (i < list->mPtrCount) goto loop;
        }

    }

    // Iterate through mActorList3 (sentinel at offset 0x48)
    // Retail: stores flags unconditionally, then checks null for moveBase calc
    {
        _reslist_node<cf::CfObjectActor*>* cur =
            mgr->mActorList3.mStartNodePtr->mNext;
        while (cur != mgr->mActorList3.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            // Retail: store flags first, then branch over the moveBase
            // adjustment only (adjusted-this pattern: the call still happens
            // with the raw pointer when actor is null).
            ((cf::CfObjectActor*)actor)->mFlags68 |= 0x40;

            {
                void* base = (actor != nullptr) ? (void*)((u8*)actor + 0x3e9c) : (void*)actor;
                void* result = getEffOwner__(base);
                if (result != nullptr) {
                    *(u32*)((u8*)result + 0x3f08) |= 0x8000000;
                    CPartsChange_ResetBattleEntry(result, 0, 0);
                }
            }

            cur = cur->mNext;
        }
    }

    // Iterate through mActorList2 (sentinel at offset 0x28)
    {
        _reslist_node<cf::CfObjectActor*>* cur =
            mgr->mActorList2.mStartNodePtr->mNext;
        while (cur != mgr->mActorList2.mStartNodePtr) {
            cf::CfObjectActor* actor = cur->mItem;
            CfObjectMove_setAnimModeArgs((u8*)actor + 0x3e9c, 0x31, 0, -1, 1);
            cur = cur->mNext;
        }
    }
}
// Handles battle finish / state transition logic.
// Checks various battle state flags and performs cleanup.
void CBattleMan_OnActorsEmpty(cf::CBattleManager* mgr) {
    extern u8 lbl_eu_80573EEC[];
    struct EECView { u8 _pad[0xD0]; u32 field_d0; };
    struct Player8CView { u8 _00[0x8c]; u16 field_8c; };
    if (reinterpret_cast<EECView*>(lbl_eu_80573EEC)->field_d0 == 0) return;

    cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet(4)) goto early_return;

    cf::CfGameManager::getInstance();
    if (CfRes_checkFlags_48000()) goto early_return;

    cf::CfGameManager::getInstance();
    extern s32 CfRes_getE24Bit22();
    if (CfRes_getE24Bit22()) goto early_return;

    // Main path: set up enum list and iterate
    {
        // __ct__16CfEnumListHolderFv / get__16CfEnumListHolderFv / __dt__16CfEnumListHolderFv come from
        // CVision.hpp as extern "C" (void* params); no local re-declaration.
        extern void startEnumObjects(void*, u32, u32);

        CfEnumListHolder holder;
        cf::CfObjEnumList* list = holder.get();
        startEnumObjects(list, 0x20, 0x800);

        list = holder.get();
        if (list->mPtrCount != 0) {
UIWin_CreateB4790Win((u32)(uintptr_t)lbl_eu_80573EEC, 0);

            CItem_clearSharedBox();

            for (int i = 0; i < 3; i++) {
                extern void* getPlayer__Q22cf13CfGameManagerFi(int);
                void* player = getPlayer__Q22cf13CfGameManagerFi(i);
                if (player != nullptr) {
                    if (((Player8CView*)player)->field_8c == 6) {
                        void* action = MapFx_ResolveOwner(player);
                        CCharVoiceMan_EnqueueItemVoice(action);
                        goto cleanup;
                    }
                }
            }
        } else {
            CItem_clearSharedBox();
        }

cleanup:;
    }
    return;

early_return:
    CItem_clearSharedBox();
}

// (.data absorb deleted: typified above as B080/B110/B784/B790 +
// switch-slot reservations; the MWCC vtable run closes the section.)
// (rodata absorb deleted: typified above as tables + strings)
// Retail cf::CBattleManager::spInstance is an 8-byte .sbss object: the singleton pointer above
// + one trailing word that no code in this TU names (zero at boot). A plain
// tentative definition is enough: MWCC auto-routes small zero-init data to
// .sbss, and an explicit section declspec is rejected here (error 33044).
// Typed so the section raw-matches without an absorb blob.
// (sdata absorb deleted: typified above as six .sdata pointer tables)
unsigned int lbl_eu_80663F00_tail[1];
// (sdata2 absorb deleted: typified above as individual .sdata2 defs)
