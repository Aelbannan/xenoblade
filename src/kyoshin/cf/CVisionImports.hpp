#pragma once

// TU-private imports for kyoshin/cf/CVision.cpp (moved out of the public
// CVision.hpp, which keeps the class and the types it holds by value).

#include "kyoshin/cf/CVision.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

// ---------------------------------------------------------------------------
// C-linkage imports: retail symbol names relocated from CVision.cpp.
// Keep `extern "C"` and the exact symbol names -- they ARE the retail names.
// ---------------------------------------------------------------------------
extern "C" void setChildF50G_(void* self, float v);
// CSchedule::Update is a member now; declared in monolib/core/CSchedule.hpp.

// Same-TU siblings (defined in CVision.cpp; C linkage keeps the call relocs
// and definitions at the unmangled retail names - CSuddenCommu.hpp idiom).
extern "C" void manageVisionSlotFx(cf::CVision* self, void* slot, void* r28);
extern "C" void routeVisionSlotFx(cf::CVision* self, void* r25, int r26, int r27, int r28);

// Class-spec table (8-byte stride, only low word used) for createVisionEffect.
struct CVisionClassTableEntry { u32 field_00; u32 field_04; };
extern "C" CVisionClassTableEntry visionEffectClassTable[4];

// sdata2 float constants used by setEffectScale / vision state checks.
extern "C" f32 lbl_eu_80667CD4;
extern "C" f32 lbl_eu_80667CF0;
extern "C" f32 lbl_eu_80667CD0;
extern "C" f32 lbl_eu_80667CE4;
extern "C" f32 lbl_eu_80667CEC;
extern "C" f32 lbl_eu_80667CF8;
extern "C" f32 lbl_eu_80667CFC;
extern "C" f32 lbl_eu_80667D04;
extern "C" f32 lbl_eu_80667CF4;
extern "C" f32 lbl_eu_80667D00;
extern "C" f32 lbl_eu_80667CE0;
extern "C" f32 lbl_eu_80667D08;
extern "C" f32 lbl_eu_80667D0C;
extern "C" f32 lbl_eu_80667D10;
extern "C" f64 lbl_eu_80667D28;
extern "C" f32 lbl_eu_80667D30;
extern "C" f32 lbl_eu_80667D34;
extern "C" f32 lbl_eu_80667D38;
extern "C" f32 lbl_eu_80667D3C;
extern "C" f32 lbl_eu_80667D40;
extern "C" f32 lbl_eu_80667D44;
extern "C" f32 lbl_eu_80667D48;
extern "C" f32 lbl_eu_80667CE8;

extern "C" void lookupEffectForResource__Q22cf13CfGameManagerFv(u32 a, u32 b, u32 c);
extern "C" void cfCam_pushStateToActive();
extern "C" void* getHandleMEM2__Q23mtl10MemManagerFv();
// CTaskGame_openVision is owned by kyoshin/CTaskGame.hpp (single unified decl).
extern "C" void aiActionClearBits0006(void* obj); // void return: CAIAction.cpp definition
extern "C" void aiRebootAction(void* obj);
extern "C" int findObjectById__Fi(int id);
// ScMain_* / Sc4798_* exports are declared once in kyoshin/cf/code_800F42AC.hpp.
extern "C" int CUICfManager_queueFactoryMenu(int id, float f);
// Character-data lookup + row predicate pair used by CVision.cpp's
// movie-gate checks (defined in CtrlObjectParam.cpp). Declared here because
// sibling TUs declare the same retail symbols with different signatures or
// define empty stubs; the variants in other TU-local headers are never
// included together with this one.
extern "C" bool CtrlObjectParam_FindSlotTypeIndex(int* a, u32 b, int* outA, int* outB);
extern "C" u8 CtrlObjectParam_GetRowColumnByte(void* row);
extern "C" void* CtrlObjectParam_GetCharEntry(u32 index);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int idx);
extern "C" void CfSoundMan_StopSlotByMode(u32 a, u32 b, u32 c);
extern "C" u8* getGlobalSda(void);
extern "C" void cfCam_copySavedState(int id);
extern u8 visionSharedStateFlags;
extern "C" void* __dynamic_cast(void* src, long offset, const void* src_type,
                                const void* dst_type, void* src2dst);

// Player-list enumeration (CfObjectEnumList).
extern "C" void startEnumObjects(void* list, u32 type, u32 filter);
extern "C" void* getObjectAt(void* list, u32 idx);
extern "C" void updateBattleEffectState__Q22cf13CfGameManagerFv(u32 a, u32 b);

// Typeinfo names for __dynamic_cast in the player-list loops.
extern "C" const void* __RTTI__Q22cf13CfObjectModel;
extern "C" const void* __RTTI__Q22cf8CfObject;

extern "C" void aiRingPrepend(void* a, void* b);
extern "C" int aiFetchAction(void* a, void* b);
extern "C" bool isVisionSlotAnimating(u32 flags);
extern "C" void* CBattleMan_FetchVisionObj(void* self);
extern "C" void CtrlObjectParam_RunPartyArtsFlow(void* a, int b, void* c, int d, int e, int f, int g);
extern "C" int Counter_TestBit(void* data, u32 mode);
extern "C" void Vision_CopyFxActor(void* self, void* dst);
extern "C" void requestItemLoad(void* obj, u32 flag);
extern "C" f64 lbl_eu_80667D18;
extern "C" f64 lbl_eu_80667D20;
extern "C" f32 lbl_eu_80667CDC;
extern "C" f32 lbl_eu_80667CD8;
extern "C" f32 lbl_eu_80667D50;
extern "C" f32 lbl_eu_80667D54;
extern "C" f64 lbl_eu_80667D58;

// Mode-dispatch helpers used by routeVisionSlotFx's per-mode FX setup.
extern "C" void restartVisionSlots(u32 a);
extern "C" void CCharVoiceMan_EnqueuePendingActionVoice(u32 a, u32 b, void* c);
extern "C" void SysWinLog_AdvanceTiers(u32 a);

// 5-word data table
extern "C" u32 visionEffectScaleWords[5];

// Flag word OR-ed with 0x8 in routeVisionSlotFx.
extern u32 gmPauseFlags;

// Scene handle whose frame-time drives the vision element decay.
namespace cf { class CScn; }

// Typeinfo name for __dynamic_cast in driveVisionFxUpdates's second list loop.
extern "C" const void* __RTTI__Q22cf11CfObjectEff;

// Callback ptmf tables (12-byte triplets).
// Callback triplet copied wholesale by gateVisionByFusionParam / forwardVisionShowRequests /
// runVisionResetWindows (retail emits a 12-byte struct copy: lis @ha / lwzu @l).
extern "C" cf::CVisionPtmf visionCallbackRunResetWindows;
extern "C" u32 visionCallbackPollSlotsAlt2[3];
extern "C" u32 visionCallbackPollSlotsAlt3[3];
extern "C" u32 visionCallbackDriveSubFxAlt[3];
extern "C" u32 visionCallbackPollSlotsAlt[3];
extern "C" u32 lbl_eu_805331E0[3];
extern "C" u32 lbl_eu_805331CC[3];
// ptmf-triplet table: entries used at byte offsets 0x114/0x120/0x12c.
extern "C" u32 visionCallbackPollSlots[3];
// 80533170/80533140/8053314C are 12-byte callback triplets installed in
// checkSecondFusionVision / manageVisionPlayerCallbacks.
extern "C" cf::CVisionPtmf visionCallbackFunc801A5260;
extern "C" cf::CVisionPtmf visionCallbackOpenForPairs;
extern "C" cf::CVisionPtmf visionCallbackRunIdleWindows;

extern "C" cf::CVisionPtmf visionCallbackEvalTrigger;
extern "C" cf::CVisionPtmf visionCallbackForwardShowRequests;
extern "C" u32 visionCallbackCheckSecondFusion[3];
extern "C" u32 visionCallbackTypeInfoTable[40];
extern "C" u32 visionCallbackResumeFromFader[3];
extern "C" u32 visionCallbackResumeFromFaderAlt[3];
extern "C" u32 visionCallbackManagePlayerCallbacks[3];
extern "C" f32 lbl_eu_80667CD8;
extern "C" void* __RTTI__Q22cf13CfObjectActor;
extern "C" void CtrlRemote_SetSharedBit(u32 a, u32 b);
// Return type must match CfGameManager.hpp's declaration (u32), or MWCC
// rejects the redeclaration when both headers are included in one TU.
// UIWin_BuildFlagBuf is owned by kyoshin/CUIWindowManager.hpp.

extern "C" void Vision_CopyEngageActor(void* obj, void* dst);
extern "C" void CfSoundMan_ApplyFxSendAll(u32 a, u32 b, f32 c);
extern "C" void CfSoundMan_ClearFxEffect(u32 a, u32 b);

extern "C" long __ptmf_test(void* ptmf);
extern "C" int __ptmf_cmpr(void* a, void* b);
extern "C" u32 __ptmf_null[3];
extern "C" u32 visionCallbackRunTierWindows[3];
extern "C" u32 visionCallbackDriveSubFx[3];
extern "C" int getAnimGate(void);
extern "C" s32 CfRes_getE24Bit22(void);
extern "C" u32 GetItemMulti_IsActiveFlag(void);
extern "C" void CBattleManager_preCalcTotalDamage(void* self, void* actor, f32* outDamage, u32* outCount);
extern "C" f32 lbl_eu_80667D4C;
extern "C" void* getInstance__Q22cf13CfGameManagerFv(void);

// Sub-object constructors / vtable data used by the slot constructor
// (retail __ct__801A33AC). resetMapMarker / initItemWorkState are C-linkage
// constructors defined in CfMapItemManager.cpp; the lbl_eu_8052xxxx symbols
// are the CArtsSet / CAttackSet / CVisionSlot vtables stored by the slot ctor.
// They are declared as byte arrays so MWCC takes their addresses with
// absolute lis/addi (ADDR16_HA/LO) instead of sda21, matching the retail.
extern "C" void resetMapMarker(void* self);
extern "C" void initItemWorkState(void* self);
// Declared larger than the 8-byte SDA threshold so MWCC takes their addresses
// with absolute lis/addi (ADDR16_HA/LO) instead of sda21, matching the retail.
extern "C" u8 lbl_eu_8052813C[0x40];
extern "C" u8 lbl_eu_8052F5C0[0x40];
extern "C" u8 lbl_eu_8052F5A8[0x40];
// Direct ctor calls for the sub-objects (placement new would add a null guard
// the retail does not have).
extern "C" void __ct__Q22cf12CBattleStateFv(void* self);
extern "C" void __ct__Q22cf9_sArtsSetFv(void* self);
extern "C" void __ct__Q22cf10CArtsParamFv(void* self);
extern "C" void __ct__Q22cf12CAttackParamFv(void* self);

