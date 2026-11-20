#pragma once

// TU-private imports for the kyoshin/cf/chain TUs (moved out of the public
// CChain.hpp, which keeps the chain classes held by value in CBattleManager).

#include "kyoshin/cf/chain/CChain.hpp"
#include "kyoshin/cf/chain/CChainActorListImports.hpp"
#include "kyoshin/cf/chain/CChainTimer.hpp"
#include "kyoshin/cf/chain/CChainCombo.hpp"

// Forward decls for the CChain tail overlays (full defs live in
// CMenuBattleChain.hpp / CUIErrMesWin.hpp; CChain_ResetChainState only passes their
// addresses through to the reset helpers).
struct CBattleChainMenuState;
struct CErrMesEntry;
struct CErrMesOwner;

// CChain_PickVoiceId is defined in this TU (C linkage inherited from the extern
// "C" block below, so both the definition and the call reloc stay unmangled).
// Its battle-object params use cf::CChainBattleObj (field_3F28 chain type).

// Chain-voice data symbols (.sdata float / .sbss byte).
extern float lbl_eu_80668A18;
extern float lbl_eu_80668A1C;
extern float lbl_eu_80668A40;
extern float lbl_eu_80668A44;
extern float lbl_eu_80668A48;
extern float lbl_eu_80668A50;
extern float lbl_eu_80668A54;
extern u8 visionSharedStateFlags;
// Battle-manager mode-flag word (.sbss) probed by CChain_TickChain's chain-start
// gate (bit 22). Same declaration as CBattleManager.hpp's extern "C" row.

// Voice-timing threshold compared against the voice sub-object's slot-0x110
// result by CChainActor_CheckVoice (.sdata2 float; const lets MWCC schedule the
// pool load at retail's position, cf. MWCC_CASES SDA-pool hoist).
extern const f32 lbl_eu_80668A60;

// Chain facing-gate constants used by CChainActor_CheckFacing (.sdata2 floats): the
// Atan2FIdx scale, the +/- normalization window around the gauge angle, and
// the two factors of the facing tolerance product.
extern const f32 lbl_eu_80668A58;
extern const f32 lbl_eu_80668A5C;

// Chain-voice global state (retail .sdata lbl_eu_80662A20): -1 is the
// "not loaded" sentinel. The func_80276C*/func_8027715* helpers probe it with
// the unsigned (value + 0x10000) == 0xffff sentinel test (MWCC addis/cmplwi
// idiom, cf. CfCmd_Fwd16IfItem in code_8018F8D8.cpp).
extern s32 lbl_eu_80662A20;

// Chain-voice id tables picked by CChain_PickVoiceId (.sdata int arrays -> sda21
// addressing; lbl_eu_805381C8 is a 3-entry .data table -> lis/addi).
extern int lbl_eu_80662A24[1];
extern int lbl_eu_80662A28[2];
extern int lbl_eu_80662A30[2];
extern int lbl_eu_80662A38[2];
extern int lbl_eu_805381C8[3];

// C-linkage callees (retail symbols are unmangled globals - C linkage keeps
// the call relocs verbatim). Voice-manager helpers are defined in
// kyoshin/cf/voice/CCharVoiceMan.cpp; the chain-actor-list helpers in
// CChainActorList.cpp; the chain-menu / err-mes helpers in
// CMenuBattleChain.cpp / CUIErrMesWin.cpp.

// Circular gimmick object list consumed by getReslistB48 (layout mirror of
// the CfGimmick.hpp structs - that header's removeManagedObject C-import clashes
// with CChainEffect.hpp's void* variant, so its getReslistB48 C-import and
// the list structs are mirrored here instead).
struct CChainGimmickListNode {
    CChainGimmickListNode* next; //0x00
    u8 gap04[0x08 - 0x04];
    void* object;               //0x08
};
struct CChainGimmickList {
    void* field_00;             //0x00
    CChainGimmickListNode* head; //0x04
};

// Gauge-delta literals consumed by CChain_RegisterCombo (.sdata float / doubles).
extern float lbl_eu_80668A30;
extern double lbl_eu_80668A20;
extern double lbl_eu_80668A28;

// CfGameManager mode-flag word (.sbss; sda21 access).
extern u32 gmPauseFlags;

namespace cf { class CfObjectActor; }
#include "kyoshin/cf/CtrlRemoteApi.hpp"
#include "kyoshin/cf/CfCamSymbols.hpp"
extern "C" {
    void CCharVoiceMan_FlushAll();
    void CCharVoiceMan_PlayNodeById(u32 arg);
    int CCharVoiceMan_IsNodeActive(u32 arg);
    int CCharVoiceMan_AllocChainVoiceId();
    int CCharVoiceMan_ForwardNodeCall(int a, CChainBattleObjTail* b, int c);
    extern "C" void Chain_ClearAll(cf::CChainActorList* self);
    extern "C" cf::CChainActor* ChainList_FindKey(cf::CChainList* self, u32 key);
    void ChainList_Clear(cf::CChainList* self);
    extern "C" void trackChainPlayer(CBattleChainMenuState* self);
    int CChain_PickVoiceId(int mode, cf::CChainBattleObj* p1, cf::CChainBattleObj* p2);
    extern "C++" void func_802B48A0(CErrMesEntry* self);
    // Same-TU voice reset helper (defined in CChain.cpp); C linkage keeps
    // the call reloc from CChain_UpdatePerFrame verbatim.
    void CChain_RetireVoiceId();
    // Chain-actor-list helpers (CChainActorList.cpp). C linkage keeps the
    // call reloc names verbatim (MWCC otherwise mangles global functions).
    extern "C" void Chain_SweepDeadActors(cf::CChainActorList* self);
    extern "C" void Chain_RebuildLinks(cf::CChainActorList* self);
    extern "C" void ChainList_SweepDead(cf::CChainList* self);
    extern "C" void ChainList_UpdateAll(cf::CChainList* self);
    extern "C" void Chain_TickChanceCount(cf::CChainChanceS* self);
    extern "C" int  Chain_RemoveActor(cf::CChainActorList* self, u32 key);
    extern "C" int  Chain_CheckActivatable(cf::CChainActorList* self, u32 key);
    extern "C" int  ChainList_RemoveKey(cf::CChainList* self, u32 key);
    // Chain-start helpers used by CChain_TryActivateChain / CChain_AdvanceChainLink
    // (CChainActorList.cpp / CMenuBattleChain.cpp).
    extern "C" void Chain_CollectChainable(cf::CChainActorList* self, cf::CChainList* other, cf::CfObjectActor* target);
    extern "C" void ChainList_StepTargets(cf::CChainList* self, int target);
    void Chain_LinkList(cf::CChainList* self, int target, int index);
    extern "C" void Chain_EmitThresholdEvents(cf::CChainFlag* self);
    extern "C" void Chain_EmitCountEvents(cf::CChainFlag* self);
    extern "C" void reinitChainPlayer(CBattleChainMenuState* self);
    // Presentation/event flag-bit probe (code_800F42AC.cpp; retail symbol
    // carries the Fi suffix) and battle-manager sub-check (CBattleManager).
    bool func_8006EF04__Fi(int mask);

    // Arts-select slot probe (CMenuArtsSelect.cpp): unmangled at the retail
    // call site, so declared here with C linkage rather than through the
    // class member (which mangles to checkSlotUsable__15CMenuArtsSelectFl).
    int checkSlotUsable(void* menu, int index);

    // decl on kyoshin/cf/CBattleManager.hpp (extern "C", retail-unmangled
    // symbol); the C++-linkage copy here clashed with it (10197).
    // Voice-act / chain arts-combo helpers. aiRebootAction lives in
    // CVision.hpp (same signature, no clash); aiRingCompact and
    // updateChainCombo have no header declaration, so they are declared here.
    void aiRebootAction(void* self);
    void aiRingCompact(void* self);
    void updateChainCombo(cf::CChainCombo* self, cf::CfObjectActor* actor);
    // Same-TU helper (defined in CChain.cpp after its first caller).
    void CChain_TickChain(cf::CChain* self);
    // Same-TU chain-voice reset (defined in CChain.cpp); used by the ctor.
    void CChain_DiscardVoiceId(void* self);
    // Chain-menu / err-mes helpers (CMenuBattleChain.cpp / CUIErrMesWin.cpp).
    extern "C" void consumeChainToggles(CBattleChainMenuState* self);
    extern "C++" void func_802B48B8(CErrMesEntry* self);
    extern "C++" void ErrWin_RegEntryS1_48E4(CErrMesEntry* self, CErrMesOwner* owner);
    extern "C++" void ErrWin_RegEntryS2_4968(CErrMesEntry* self, CErrMesOwner* owner);
    extern "C++" void ErrWin_RegEntryS3_4A68(CErrMesEntry* self, CErrMesOwner* owner);
    // Error-message record register helpers (CUIErrMesWin.cpp) - same family
    // as func_802B48A0 / func_802B48B8 above (chain-state codes 1/2/3).
    // Battle-chain menu helpers (CMenuBattleChain.cpp).
    void requestAnimReset(CBattleChainMenuState* self);
    extern "C" int isChainReady(CBattleChainMenuState* self, u8* out);
    // Chain-state-machine helpers used by CChain_TickChain (same-TU or
    // CChainActorList.cpp / CUIErrMesWin.cpp / CMenuBattleChain.cpp).
    void CChain_ResetVoiceMgr();
    void CChain_AdvanceChainLink(cf::CChain* self);
    void CChain_TryExtend(cf::CChain* self);
    void CChain_DriveChainExtend(cf::CChain* self);
    extern "C" void Chain_FindOrCreateActor(cf::CChainActorList* self, u32 key);
    extern "C" void Chain_MergeList(cf::CChainActorList* self, cf::CChainList* list);
    extern "C" int  Chain_HasValidTarget(cf::CChainActorList* self);
    extern "C" int  ChainList_CheckCondition(cf::CChainList* self, int target, int check);
    extern "C" int  ChainList_HasCount(cf::CChainList* self, int key);
    void payoutChainCombo(cf::CChainCombo* self, cf::CfObjectActor* actor);
    extern "C" int  Chain_RollActivation(cf::CChainChance* self, cf::CChainBattleObj* target, cf::CChainBattleObj* source);
    extern "C" int  Chain_ReadPadAction(cf::CChainAction* self, u8* out);
    extern "C" void ChainCounter_Drain(cf::CChainCounter* self);
    extern "C" void ChainChance_Reset(cf::CChainChance* self);
    extern "C" void updateChainPlayer(CBattleChainMenuState* self);
    void requestArtsRef(CBattleChainMenuState* self);
    extern "C" bool isChainMenuBusy(CBattleChainMenuState* self);
    extern "C++" void ErrWin_ResetRecord_4B84(CErrMesEntry* self);
    void aiAppendToRing(u8* self, cf::CChainScratch20* scratch);
    void ChainEne_TouchBattle194_18F8();
}
