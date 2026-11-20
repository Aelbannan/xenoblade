// The header declares the 2-arg (delete-flag) form of the UnkClass_8018EF3C
#include "kyoshin/cf/CtrlObjectParamSymbols.hpp"
#include "kyoshin/menu/code_8018C5FC.hpp"
#include "kyoshin/code_8025FB10.hpp"
// ctor for teardownGameManager; init() calls the 1-arg placement form. Rename the
// 2-arg copy away while including the header, then declare the 1-arg form.
#define __ct__Q22cf17UnkClass_8018EF3CFv ct_UnkClass_8018EF3C_2arg
#include "kyoshin/cf/CBattleManager.hpp"
#include "monolib/coli/code_804B2FF0.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObjectSelectorObj.hpp"
#include "kyoshin/cf/CfTFile.hpp"
#include "kyoshin/cf/CfCamEventApi.hpp"
#include "kyoshin/menu/MenuSndApi.hpp"
#include "kyoshin/cf/CfMapEffectManager.hpp"

#include "monolib/scn/CScn.hpp"
#undef __ct__Q22cf17UnkClass_8018EF3CFv
#include "kyoshin/cf/CfPadData.hpp"
#include "kyoshin/cf/CfPadTask.hpp"
#include "kyoshin/cf/CfTaskMain.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"
// CfObjectMove.hpp declares createEffectForPlayer__Q22cf13CfGameManagerFv as void* and
// getPlayer__Q22cf13CfGameManagerFi as void*(int) to avoid pulling the
// unity-helpers header; CfGameManagerUnityHelpers.hpp (included later) declares
// them with proper return types (Unk8187CData* / CfObjectMove*(s32)). extern
// "C" redeclaration with a different return type is illegal, so rename
// CfObjectMove.hpp's copies away.
#define createEffectForPlayer__Q22cf13CfGameManagerFv cfObjectMoveFunc_80081900
#define getPlayer__Q22cf13CfGameManagerFi cfObjectMoveGetPlayer
#include "kyoshin/cf/object/CfObjectMove.hpp"
#undef createEffectForPlayer__Q22cf13CfGameManagerFv
#undef getPlayer__Q22cf13CfGameManagerFi
// CfObjectMap.hpp's import block declares Tok_FormatStr as int(char*,...)
// clashing with the proper extern "C"
// versions already in scope from CfObjectMove.hpp / IResInfo.hpp
// (MWCC forbids overloading an extern "C"
// function). Rename CfObjectMap.hpp's copies out of the way; this TU uses the
// properly-typed declarations from those headers. (Scn_GetFrameDelta now has a
// single unified decl.)
#define Tok_FormatStr cfObjectMapFunc_800AA33C
#include "kyoshin/cf/object/CfObjectMap.hpp"
#undef Tok_FormatStr
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include "kyoshin/code_801862C0.hpp"

// slot-0x40 virtual CtrlPlayerSub3ED4::vf14, int(int), same 0x400-probe
// idiom as CtrlPc.cpp's mask-3 resets). Include-only: owned elsewhere.
#define cfCam_setClear04 ctrlPcFunc_8006BBF4
#define appendObjectById ctrlPcFunc_800F6D50
#define GetZealObjPtr ctrlPcFunc_8017FD44
#define GetZealResult ctrlPcFunc_8017FD4C
#define getArtsParamRC2 ctrlPcGetArtsParamRC2
#include "kyoshin/cf/CtrlPc.hpp"
#undef cfCam_setClear04
#undef appendObjectById
#undef GetZealObjPtr
#undef GetZealResult
#undef getArtsParamRC2
// Full battle-manager class for resetBattlePresentation's real slot-0x1C call
// (setPartyMaskFlag) and +0x94/+0x194 member access. Include-only: the class
// and its TU are owned elsewhere; nothing here is redeclared.
#define CItem_getNameIdFromFam cbattleFunc_80158018
#define isGlobalCamFlagSet__Fi cbattleIsGlobalCamFlagSet
#define CtrlObjectParam_FindSlotTypeIndex cbattleFunc_8009E344
#define lookupEffectForResource__Q22cf13CfGameManagerFv cbattleLookupEffectForResource
#define __dt__Q22cf16CfObjectImplMoveFv cbattle_dt_CfObjectImplMove
#define getPlayer__Q22cf13CfGameManagerFi cbattleGetPlayer
#define lookupWorkAtAddr cbattleFunc_800B8920
#define syncBattleState__Q22cf13CfGameManagerFv cbattleSyncBattleState
#define getReslistBE8 cbattleFunc_800B6C34
#define addTableValueWithClamp__Q22cf13CfGameManagerFv cbattleAddTableValueWithClamp
#define createBattleActor__Q22cf13CfGameManagerFv cbattleCreateBattleActor
#include "kyoshin/cf/CBattleManager.hpp"
#undef createBattleActor__Q22cf13CfGameManagerFv
#undef CtrlObjectParam_FindSlotTypeIndex
#undef lookupEffectForResource__Q22cf13CfGameManagerFv
#undef __dt__Q22cf16CfObjectImplMoveFv
#undef getPlayer__Q22cf13CfGameManagerFi
#undef lookupWorkAtAddr
#undef syncBattleState__Q22cf13CfGameManagerFv
#undef getReslistBE8
#undef addTableValueWithClamp__Q22cf13CfGameManagerFv
#undef isGlobalCamFlagSet__Fi
#undef CItem_getNameIdFromFam

#include "monolib/core/CPadManager.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/FixStr.hpp"
#include <string.h>
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (ml::epsilon)
#include "kyoshin/cf/CfSoundMan.hpp"
#include "kyoshin/cf/CfNandManager.hpp"  // gCfNandRestoredEventFlag (owner)
#include "kyoshin/code_800AA008.hpp"  // Tok_ParseFile (owner)
#include "kyoshin/cf/CTaskEnvironment.hpp"  // Scn_SetPalFixFlag (owner)

extern "C" u32 getEffectFlagFromMgr(void* mgr);
extern "C" void setGimmick(void* obj, bool enable);
extern "C" void refreshGimmickObject(void* obj);
extern "C" void setVoiceActionActive(void* act, bool enable);

// Local complete type for Scn_QueryUnk80State's result (canonical name per
// CfGameManager.hpp's fwd-decl; layout matches CTaskGame.hpp's view;
// local def avoids pulling CTaskGame.hpp into this TU).
struct CTaskGameCamView {
    float field_0;
    float field_4;
    float field_8;
    float field_C;
};

// No owner header yet for these unmangled helpers.
extern void clearReloadGlobals();
extern "C" s32 MiniMapHasGlobalData();
extern "C" void gmFileObject(void* object);
extern "C" void MenuSnd_RestartBGM_88D34(const char* name, int flag, float f1, float f2);
extern "C" void Scn_SetPalFixFlag(u8* scene, unsigned char value);


// TU-local decl: loadBdatGroupData is a tail-call thunk (addi r3,r3,0x1c; b CtrlObjectParam_ResolveEquipItem)
// that passes r4 through, so the 1-arg form is intentional here. The retail
// 2-arg form (data + index) is declared in pluginCfs.cpp.
extern "C" void CtrlObjectParam_ResolveEquipItem(CfGameManagerData1C* data);
extern "C" void CtrlObjectParam_WriteEquipSlotValue(void* self, unsigned long index, void* value);

// --- imports for the func_8007xxxx / func_8008xxxx target bodies ---
// (retail C-linkage names; signature params are informational, the emitted
// reloc symbol is the unmangled name)
extern "C" void loadBdatFileTable__Q22cf6CfBdatFv();
extern "C" void* __ct__cf_CfTFile(void* self);
extern "C" void* __ct__80075298(void* self);
extern "C" void* __ct__80186578(void* self);
extern "C" void* __ct__801BF76C(void* self);
extern "C" void MenuSnd_ClearSlot_88584(void* self);
extern "C" void BattleWork_InitStateWork(void* self);

extern "C" void cfCam_seedFollowD();
extern "C" float getReloadDelay();
extern "C" void CfT_PlayClockAccumulate(u32 first, u32 second, u32 third);
extern "C" void dispatchLODArgs__8CTaskLODFv(u16 first, u16 second, u16 third);
extern "C" void CfScript_UpdateMgr();
extern "C" void CfScript_ClearReq0();
extern "C" void CfScript_ClearReq1();
extern "C" void CfScript_ClearReq2();
void CfScript_InitMgr();
void CfScript_InitPath(int);
extern "C" void CmText_DispatchCallbacks();
extern "C" void clrAnimGate();
extern "C" void CamEvtCheckAimAngles(CfCamEventManager* manager);
extern "C" void MenuSnd_CanPlayGate_9450();
extern "C" void MenuSnd_SilenceByName_8986C(int value, float first);
extern "C" void UIWin_SweepQueue(u32 value);
extern "C" void CfScript_SleepReq2();
extern "C" void evtUpdateSequenceKick();

extern "C" void* __ct__Q22cf17UnkClass_8018EF3CFv(void* self);
extern "C" void func_8007DCB8__Q22cf13CfGameManagerFv();

// --- imports for func_80086B5C ---
extern "C" void* MapFx_ResolveOwner(void* source);
// Retail symbol is Fv but callers pass three values (see setPad precedent).
extern "C" void func_80086B5C__Q22cf13CfGameManagerFv(int arg1, int arg2,
                                                        int flag);
// Definition lives (renamed away) in CfGameManagerUnityHelpers.hpp; retail
// calls it out-of-line. Its own definition below precedes every use.
// Circular gimmick list
// CfGimmick.hpp's CfGimmickList; kept TU-local to avoid pulling the header).
struct Func800B6BECNode {
    Func800B6BECNode* next;  // 0x00
    u8 gap04[4];
    void* object;            // 0x08
};
struct Func800B6BECList {
    void* field_0x0;
    Func800B6BECNode* head;  // 0x04 sentinel
};
extern "C" Func800B6BECList* getReslistB68();

// Real cf::CfObjectColl tree for func_80085978's copyCollPosition (+0xB8 Fv)
// call: Coll's no-arg override of that slot, shared with the main object tree.
// Include-only: CfObjectColl.cpp / .hpp owned elsewhere; nothing redefined.
#include "kyoshin/cf/object/CfObjectColl.hpp"

// --- imports for func_80080F48 / func_80085FB8 / func_800838F4 ---
class UnkClass_8009EC9C;
extern "C" u32 bdatPcPartsRes(u32 param1, s16 param2, u32 param3, u32 param4);
extern "C" u16 CItem_getNameIdFromFam(u16 value);
extern "C" u32 bdatMountValue(u32 value, u32 mode);
extern "C" u32 CfRes_extractBits20_7(void* field);
extern "C" u32 CfRes_packFourFields(u32 first, u32 second, u32 third, u32 fourth);
extern "C" u32 CfObjectMove_lookupBdatCol7(u32 id, u32 flag);
extern "C" u32 CfObjectMove_lookupBdatCol11(u32 id, u32 flag);
extern "C" void CamEvtClearSlotBytes(void* manager);
extern "C" void func_801AAD08();
extern u8 gSoundSystemInitFlag;
extern "C" void cfCam_setClear04(void* manager, u32 mask, s32 flag);
extern "C" void loadLayoutScales();
extern "C" void setTrackedTarget(float value, u32 a, u32 b);
extern "C" void cfCam_copyBlock16(void* dst, void* src);
extern "C" void disableFlag20__Q22cf13CfGameManagerFv();
extern "C" void cfCam_syncFollowD();
extern "C" void CfScript_MakeName(char* dest, const char* src1, const char* src2,
                              const char* src3);
struct Unk407C8Color {
    u32 w[4];
};
extern "C" Unk407C8Color* CTaskGame_setVec4(Unk407C8Color* self, float f1, float f2,
                                        float f3, float f4);

// data globals referenced by the target bodies (defined in other TUs)
extern u16 gmGlobalFlag46;
extern u8 gmEventFlag35;
extern u16 gEventWindowFlags;
extern u8 gmLastDirection;
extern float gmNandFloat54;
extern u16 gmCounter36;
extern u16 gmCounter38;
extern void* lbl_eu_806640A0;
extern const double lbl_eu_80666528;

cf::CfGameManager::~CfGameManager() {
    gmFileObject(unk90);
    unk90 = nullptr;
    if (unkA8 != nullptr) {
        BattleWork_InitCtrlWork((CtrlWork*)unkA8);
    }
    if (unkA0 != 0) {
        unkA0->releaseHandle();
    }
    if (field_0xA4 != nullptr) {
        ArtsSelect_FreeSlots(field_0xA4);
    }
    if (unkB4 != nullptr) {
        CamEvtReleaseAllSlots(unkB4);
    }
    unkB0 = nullptr;
    gmScene = nullptr;
}

namespace {

struct BdatTextEntry {
    char text[0x20];
    u32 textLength;
    char secondaryText[0x20];
    u32 secondaryTextLength;
    float value;
    u8 enabled;
    u8 padding[3];
};

struct Unk87588Data {
    u8 field_0x0;
    u8 field_0x1[0x1F];
    u32 field_0x20;
    u8 field_0x24;
    u8 field_0x25[0x1F];
    u32 field_0x44;
    float field_0x48;
};

struct UnkEEE0Data {
    u16 field_0x0;
    u16 entries_0x2[1];
};

struct UnkF044Data {
    u8 field_0x0[0x888];
    u32 field_0x888[1];
};

struct UnkF8C0Node {
    u32 field_0x0;
};

struct UnkF8C0Source {
    u32 field_0x0;
    UnkF8C0Node* field_0x4;
};

struct Unk81CA0Data {
    u8 field_0x0[0x6D0];
    u16 field_0x6D0;
};

struct UnkF0ACData {
    u8 field_0x0[4];
    u16 field_0x4_bits : 15;
    u16 flag_0x4 : 1;
};

struct Unk82FCCData {
    u8 field_0x0[0x68];
    u32 flags_0x68;
};

} // namespace

extern "C" {
u32 gmActivePadChannel;
cf::CfPadData gmPadData;
s8 gmInitFlag;
cf::CfGameManager gmInstance;
u8 gmStaticRegistration[0x10];
CPad* gmCurrentPad;
CPad gmPadTable[8];
CScn* gmScene;
CView* gmCurrentView;
u32 gmSubManager60;
u32 gmSubManager64;
u32 gmGameFlags;
u32 gmPauseFlags;
u32 gmEnabledPadFlags;
s16 gmTimerCounter;
u8 gmGameStateFlag5C;
float gmTimeScale;
float gmFloat68;
s16 gmTransitionIndex;
float lbl_eu_80666498;
float lbl_eu_8066649C;
BdatTextEntry gmBdatTextEntries[2];
BdatTextEntry gmBdatNameEntry;

// symbols.txt: Fv, but callers leave the index in r4 (see getPlayer).
cf::CfObjectMove** getPlayerSlotPtr__Q22cf13CfGameManagerFv(cf::CfObjectMove** slots, int index);

void __ct__Q22cf13CfGameManagerFv(cf::CfGameManager* self);

void* __register_global_object(void* object, void* destructor, void* registration);
}

extern "C" u32 CtrlRemote_TouchBitByArg(u32 resourceId);
extern void setMasterVolume(float value, float fadeTime);
extern const float lbl_eu_80666548;

// func_80085978 imports.
// MiniMapResetLayoutAnims: declared (s32()) by CfGameManager.hpp - local void form conflicts (10505)
// bindPadSubobjects: declared (s32) by CfGameManager.hpp - local u32 form conflicts (10197)
extern "C" void UIBattleMarkFlags82();
extern "C" void UIBattleRemoveAll(u32);
// Retail C-linkage imports (signatures verbatim from the defining TUs /
// CfGameManager.ctx.c; kept file-local so no shared header is pulled in).
extern "C" void MenuSnd_RampMasterVol_889D0(u32 object);
extern "C" void updateHelpManager(cf::CHelpManager* object);
extern "C" void CCharVoiceMan_Tick();
extern "C" void refreshNpcRowFlags();
extern "C" void CfT_PlayClockUpdate();
extern "C" void CfT_PlayClockAdvanceInt();
extern "C" void bdatTotalRows();
extern "C" void bdatSetResRow(void*, void*);
// CtrlObjectParam helpers: retail calls them by flat name (cf. CfObjectPc.cpp
// calling RefreshCharEntry the same way); signatures match the defining TU.
extern "C" void CtrlObjectParam_RefreshCharEntry(void* self, u32 arg2);
extern "C" int CtrlObjectParam_ValidateSlotRange();
extern "C" int CtrlObjectParam_InsertSlotEntry(void* self, int value, int type, int index);
extern "C" void CtrlObjectParam_CompactSlotArrays(u8* self);
extern "C" void CtrlObjectParam_RampEntryArtsLevel(void* self, u16 val);
extern "C" void CtrlObjectParam_ApplyArtsRankUp(void* self, u32 value, u32 a, u32 b);
extern "C" void CtrlObjectParam_WriteArtsCounter(void* self, u16 arg2);
extern "C" void CollObjSetupVecsState4(void* self, const void* pA, const void* pB, float f);
extern "C" void mineResetAll();
extern "C" void resetHelpManager(cf::CHelpManager* object);
extern "C" void setInputDisableTime__Q22cf9CfPadTaskFf(float value);
extern float lbl_eu_806669C8;

unsigned int packResourceId(unsigned int first, unsigned int second) {
    return (first << 27) | (second << 20);
}

extern "C" void clearGameFlagMask__Q22cf13CfGameManagerFv(u32 mask) {
    gmGameFlags &= ~mask;
}

extern "C" u32 getBdatEntryIndex__Q22cf13CfGameManagerFv(const UnkEEE0Data* data, u16 index) {
    return data->entries_0x2[index];
}

extern "C" u32 getItemSlotCount__Q22cf13CfGameManagerFv(const UnkF044Data* data, u32 index) {
    return data->field_0x888[index];
}

extern "C" void copyItemSourceNode__Q22cf13CfGameManagerFv(UnkF8C0Node* destination,
                                                        const UnkF8C0Source* source) {
    destination->field_0x0 = source->field_0x4->field_0x0;
}

extern "C" void setPackedSlotId__Q22cf13CfGameManagerFv(Unk81CA0Data* data, u16 index,
                                                        u32 offset) {
    u32 value = index * 16;
    value += offset;
    data->field_0x6D0 = value;
}

bool isGameFlagMaskSet(u32 mask) {
    return (gmGameFlags & mask) != 0;
}

extern "C" bool isItemEnabled__Q22cf13CfGameManagerFv(const UnkF0ACData* data) {
    return data->flag_0x4 != 0;
}

extern "C" bool isObjectFlagMaskSet__Q22cf13CfGameManagerFv(const cf::CfObject* data, u32 mask) {
    return (data->mFlags68 & mask) != 0;
}

extern "C" bool compareFlagValues__Q22cf13CfGameManagerFv(const u32* first, const u32* second) {
    return *first != *second;
}

extern "C" void togglePauseFlag__Q22cf13CfGameManagerFv(u32 mask, bool enable) {
    if (enable) {
        gmPauseFlags |= mask;
    } else {
        gmPauseFlags &= ~mask;
    }
}

extern "C" void clampValueToRange__Q22cf13CfGameManagerFv(s32 minimum, s32* value,
                                                        s32 maximum) {
    if (*value < minimum) {
        *value = minimum;
    } else if (*value > maximum) {
        *value = maximum;
    }
}

// The queueSceneEventA wrapper body is visible in UnityHelpers and -inline auto
// would inline it at call sites, but retail calls it as a real function
// (bl queueSceneEventA__Q22cf13CfGameManagerFv in resetBattlePresentation/
// func_80085FB8), so rename the visible definition away and import the
// retail symbol directly (declared in CfGameManager.hpp).
// (func_8008064C's CfCamPosSource traffic goes through the unity-helpers
// header's own forward decl + vtable-slot dispatch; no TU-local view needed.)

// .sdata2 float constants used by func_8008064C in the unity-helpers header.
extern const float lbl_eu_80666540;
extern const float lbl_eu_80666544;

#define queueSceneEventA__Q22cf13CfGameManagerFv unityHelpersWrapFunc_80083D50
#define flushEventQueue__Q22cf13CfGameManagerFv unityHelpersWrapFunc_800827E4
#define syncBdatDataCache__Q22cf13CfGameManagerFv unityHelpersWrapFunc_8007DECC
#define setPresentationFlag__Q22cf13CfGameManagerFv unityHelpersWrapFunc_8008294C
#include "kyoshin/CPcKizunagram.hpp"
#include "kyoshin/cf/CfGameManagerUnityHelpers.hpp"
#include "libs/monolib/src/scn/CScn_8049603C.hpp"
extern "C" void Scn_ReleaseUnk80(void*, u32, const void*);
#undef queueSceneEventA__Q22cf13CfGameManagerFv
#undef flushEventQueue__Q22cf13CfGameManagerFv
#undef syncBdatDataCache__Q22cf13CfGameManagerFv
#undef setPresentationFlag__Q22cf13CfGameManagerFv

// Retail-named definitions for the four unity-helpers wrappers above (whose
// bodies compile under wrap names so -inline auto cannot pull them into
// other unity-helper callers). Bodies match the wrappers exactly.
extern "C" bool syncBdatDataCache__Q22cf13CfGameManagerFv(s32 value, s32* current,
                                                        u32 size) {
    bool changed = false;
    if (value > 0) {
        UnkClass_8009EC9C* data = (UnkClass_8009EC9C*)CtrlObjectParam_GetCharEntry(static_cast<u16>(value));
        s32 destination = CfRes_acquireSlot(value, 0, 0);
        if (destination >= 0 && destination != *current) {
            func_8009E120((UnkClass_8009EC9C*)CtrlObjectParam_GetCharEntry(static_cast<u16>(value)), 0);
            CfRes_tryResolveByBits(destination,
                          func_8009E120((UnkClass_8009EC9C*)CtrlObjectParam_GetCharEntry(static_cast<u16>(value)), 0),
                          size);
            CfRes_tryResolveByBits(destination,
                          func_8009E120((UnkClass_8009EC9C*)CtrlObjectParam_GetCharEntry(static_cast<u16>(value)), 1),
                          size);
            CfRes_tryResolveByBits(destination,
                          func_8009E120((UnkClass_8009EC9C*)CtrlObjectParam_GetCharEntry(static_cast<u16>(value)), 2),
                          size);
            CfRes_tryResolveByBits(destination,
                          func_8009E120((UnkClass_8009EC9C*)CtrlObjectParam_GetCharEntry(static_cast<u16>(value)), 3),
                          size);
            CfRes_tryResolveByBits(destination,
                          func_8009E120((UnkClass_8009EC9C*)CtrlObjectParam_GetCharEntry(static_cast<u16>(value)), 4),
                          size);
            CfRes_tryResolveByBits(destination,
                          resolveWpnTok__Q22cf6CfBdatFUl(data->textId_0x18), size);
            *current = destination;
            changed = true;
        } else if (destination >= 0 && destination == *current) {
            cfResNopValueSink(0);
        }
    }
    return changed;
}

extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable) {
    cf::CfGameManager::getInstance();
    if (enable) {
        enableGameFlag__Q22cf13CfGameManagerFv(0x40000000);
        Scn_SetTimeScale(gmScene, lbl_eu_80666498);
    } else {
        clearGameFlagMask__Q22cf13CfGameManagerFv(0x40000000);
        Scn_SetTimeScale(gmScene, lbl_eu_8066649C);
    }
    CfSoundMan_PauseAllRecords(enable, 10);
}

extern "C" void queueSceneEventA__Q22cf13CfGameManagerFv(
    u32 first, u32 second, u32 third, u32 fourth, float value) {
    func_800838F4__Q22cf13CfGameManagerFv(0, first, second, third, fourth, value);
}

extern "C" void flushEventQueue__Q22cf13CfGameManagerFv() {
    while (getEventQueueCount__Q22cf13CfGameManagerFv(&gmEventQueue) != 0) {
        u16 value = popEventQueue__Q22cf13CfGameManagerFv(&gmEventQueue);
        setEventFlag__Q22cf13CfGameManagerFv(value, true);
    }
}

namespace cf {

void CfGameManager::resetGameFlags() {
    if (this == nullptr) {
        gmGameFlags |= 8;
    }

    gmPauseFlags &= ~8;
    gmTimerCounter = 0;
    gmFloat68 = lbl_eu_80666498;
    gmGameStateFlag5C = 0;
    gmTimeScale = lbl_eu_80666498;
    gmTransitionIndex = -1;
    MenuSnd_StopSlot1_9424(lbl_eu_8066649C);
}

void CfGameManager::clearBdatTextEntries() {
    gmBdatTextEntries[0].text[0] = 0;
    gmBdatTextEntries[0].textLength = 0;
    gmBdatTextEntries[0].secondaryText[0] = 0;
    gmBdatTextEntries[0].secondaryTextLength = 0;
    gmBdatTextEntries[1].text[0] = 0;
    gmBdatTextEntries[1].textLength = 0;
    gmBdatTextEntries[1].secondaryText[0] = 0;
    gmBdatTextEntries[1].secondaryTextLength = 0;
    gmBdatNameEntry.text[0] = 0;
    gmBdatNameEntry.textLength = 0;
    gmBdatNameEntry.secondaryText[0] = 0;
    gmBdatNameEntry.secondaryTextLength = 0;
}

CfGameManager::CfGameManager() {
    unk28 = 0;
    unk68 = 0;
    unk6C = 0;
    unk7C = 0;
    unk8C = 0;
    unkA0 = 0;
    field_0xA4 = 0;
    unkA8 = 0;
    unkAC = 0;
    unkB0 = 0;
    unkB4 = 0;
    gmScene = 0;
    gmSubManager60 = 0;
    gmSubManager64 = 0;
    gmGameFlags = 0;
    gmPauseFlags = 0;
    unk0 = 0;
    field_0x4 = 0;
    mObjectFlags = 0;
}

CfGameManager* CfGameManager::getInstance() {
    if (!gmInitFlag) {
        // Call mangled ctor directly: placement new inserts a null check retail lacks.
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv, gmStaticRegistration);
        gmInitFlag = 1;
    }
    return &gmInstance;
}

u32 CfGameManager::getCurrentPadChannel() {
    return gmActivePadChannel;
}

CfPadData* CfGameManager::getCfPadData() {
    return &gmPadData;
}

CPad* CfGameManager::getCurrentPad() {
    u32 channel = gmActivePadChannel;
    if (channel == 0xFFFFFFFF) {
        CPad* pad = gmCurrentPad;
        if (pad != 0) {
            return pad;
        }
        return gmPadTable;
    }
    CPad* pad = gmCurrentPad;
    if (pad != 0) {
        return pad;
    }
    return &gmPadTable[channel & 7];
}

CfObjectMove* CfGameManager::getPlayer(int playerIndex) {
    return *getPlayerSlotPtr__Q22cf13CfGameManagerFv(getInstance()->unk94, playerIndex);
}

} // namespace cf

// 0x4330-prefixed s16/u16 -> f32 conversion (retail loads the magic double from
// .sdata2 and subtracts it; keep the named labels so the reloc targets match).
static float S16ToF32(s16 v) {
    union { double d; u32 w[2]; } u;
    u.w[0] = 0x43300000;
    u.w[1] = (u32)v ^ 0x80000000;
    // lbl_eu_80666530 is declared float by the unity-helpers header; when read
    // as an 8-byte double here it is the s16->f32 magic 0x4330000080000000.
    return (float)(u.d - *reinterpret_cast<const double*>(&lbl_eu_80666530));
}
static float U16ToF32(u16 v) {
    union { double d; u32 w[2]; } u;
    u.w[0] = 0x43300000;
    u.w[1] = (u32)v;
    return (float)(u.d - lbl_eu_80666528);
}

extern "C" cf::CfObjectMove** getPlayerSlotPtr__Q22cf13CfGameManagerFv(cf::CfObjectMove** slots,
                                                                              int index) {
    return &slots[index];
}

extern "C" void enableGameFlag__Q22cf13CfGameManagerFv(u32 flags) {
    gmGameFlags |= flags;
}

extern "C" void disableGameFlag__Q22cf13CfGameManagerFv(u32 flags) {
    gmGameFlags &= ~flags;
}

extern "C" void clearTempTextBuffer__Q22cf13CfGameManagerFv(Unk87588Data* data) {
    data->field_0x0 = 0;
    data->field_0x20 = 0;
}

bool cf::CfGameManager::isManagerInitialized() {
    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    return (gmGameFlags & 0x10000) != 0;
}
// cf::CfGameManager::teardownGameManager - full teardown of the CfGameManager
// singleton: mirrors the destructor's subsystem detach/delete sequence, then
// resets the global state and removes the layered CProcess hooks. The
// vtable+0x08 dispatch with delete flag 1 is MWCC's delete-on-cast-dtor
// shape (the if + delete-expansion null guards produce the paired beq; see
// CfGameManager.hpp CfDtor08).
void cf::CfGameManager::teardownGameManager() {
    gmPauseFlags |= 0x200000;
    UIBattleRemoveAll(0);
    CUICfManager_setFlagState(0);
    CCharVoiceMan_DestroySingleton();
    getBattleStateB__Q22cf14CBattleManagerFv();
    destroyHelpManager();
    __dt__80157150();
    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    cf::CfGameManager* manager = &gmInstance;
    gmFileObject(manager->unk90);
    manager->unk90 = nullptr;
    if (manager->unkA8 != nullptr) {
        BattleWork_InitCtrlWork((CtrlWork*)manager->unkA8);
    }
    if (manager->unkA0 != 0) {
        manager->unkA0->releaseHandle();
    }
    if (manager->field_0xA4 != nullptr) {
        ArtsSelect_FreeSlots(manager->field_0xA4);
    }
    if (manager->unkB4 != nullptr) {
        CamEvtReleaseAllSlots(manager->unkB4);
    }
    manager->unkB0 = nullptr;
    if (manager->unkA8 != nullptr) {
        __dt__801A9F78(manager->unkA8, 1);
        manager->unkA8 = nullptr;
    }
    if (manager->unkA0 != 0) {
        delete manager->unkA0;
        manager->unkA0 = 0;
    }
    if (manager->field_0xA4 != nullptr) {
        __dt__801865C4(manager->field_0xA4, 1);
        manager->field_0xA4 = nullptr;
    }
    if (manager->unkB4 != nullptr) {
        __dt__8007540C(manager->unkB4, 1);
        manager->unkB4 = nullptr;
    }
    gmTeardown();
    if (gmSubManager60 != 0) {
        __dt__801886EC(reinterpret_cast<void*>(gmSubManager60), 1);
        gmSubManager60 = 0;
    }
    if (gmSubManager64 != 0) {
        __dt__801BF874(reinterpret_cast<void*>(gmSubManager64), 1);
        gmSubManager64 = 0;
    }
    gmScene = nullptr;
    gmGameFlags &= ~0x30000;
    CfScript_ResetMgr();
    Selector_GetInstance();
    __dt__800FDEF8();
    resetManagerState__Q22cf13CfGameManagerFv(manager);
    CtrlRemote_InitSharedBufVoid();
    // Player-file slots at the top of the manager (offset 0x00): cancel the
    // in-flight file read, then release the object through its vtable+0x08
    // destructor and null the slot (retail's reload + paired null tests come
    // from the library call potentially clearing the slot).
    CfGameManagerSlotArray* slots = reinterpret_cast<CfGameManagerSlotArray*>(manager);
    for (u32 i = 0; i < 3; ++i) {
        if (slots->slots[i] != nullptr) {
            CfTFile_CancelResetFull((CfTFile*)slots->slots[i]);
            if (slots->slots[i] != nullptr) {
                delete reinterpret_cast<CfDtor08*>(slots->slots[i]);
                slots->slots[i] = nullptr;
            }
        }
    }
    if (manager->unkAC != nullptr) {
        __dt__Q22cf17UnkClass_8018EF3CFv(manager->unkAC, 1);
        manager->unkAC = nullptr;
    }
    gmCommandBufferState = 0;
    CfRes_callFunc_67E78(stopSoundMan__Fv());
    CfRes_callFunc_67FE0();
    KyoshinHeap_Shutdown();
    if (gmTaskMain != nullptr) {
        Remove__8CProcessFv(gmTaskMain);
        gmTaskMain = nullptr;
    }
    if (gmResManager != nullptr) {
        Remove__8CProcessFv(gmResManager);
        gmResManager = nullptr;
    }
    CBdat::getEntry(1);
    if (gmPadTask != nullptr) {
        Remove__8CProcessFv(gmPadTask);
        gmPadTask = nullptr;
    }
}

UnkClass_80083298* cf::CfGameManager::getGameSubManager() {
    return getInstance()->unk90;
}
extern "C" void triggerVoiceDown(){ firstReslistB48(); }
struct VoiceSource;

extern "C" void getVoiceSourcePtr()
{
    firstReslistB68();
}

extern "C" void resetVoiceSystem(void){
    typedef cf::CfObject* (*NextReslistB48NoArg)();  // DEBT(callcast): retail calls nextReslistB48 with no r3 setup but owner decl takes an arg (LOSTFIX proof)
    ((NextReslistB48NoArg)nextReslistB48)();
}

extern "C" VoiceSource* getVoiceSourceForSystem(VoiceSource* source)
{
    return (VoiceSource*)insertReslistB68((cf::CfObject*)source);
}
// cf::CfGameManager::func_80086B5C - battle-start pad/object reset. Refreshes
// the party-slot pair, optionally (flag != 0) rotates the slot index into the
// 0..0x17 ring and refreshes every gimmick object's 0x3E9C container, then
// pumps the pad manager a fixed number of frames (5, or 8/16 depending on
// arg1 % 3) before clearing the bit-26 presentation flag.
extern "C" void func_80086B5C__Q22cf13CfGameManagerFv(int arg1, int arg2,
                                                        int flag) {
    CfT_DispatchFrameCounts();
    u16 outA;
    u16 outB;
    CfT_PlayClockSnapshot(&outA, &outB);
    CfT_PlayClockAccumulate(arg1, arg2, 0);
    if (flag != 0) {
        gmPauseFlags |= 0x04000100;
        // Wrap arg1-3 into the 0..0x17 slot ring.
        int slot = arg1 - 3;
        if (slot < 0) {
            slot += 0x18;
        }
        if (slot >= 0x18) {
            slot -= 0x18;
        }
        CfT_PlayClockAccumulate((u16)slot, arg2, 0);
        // Refresh every gimmick object's 0x3E9C container.
        Func800B6BECList* list = getReslistB68();
        for (Func800B6BECNode* node = list->head->next; node != list->head;
             node = node->next) {
            void* rawObj = MapFx_ResolveOwner(node->object);
            if (rawObj == nullptr) {
                continue;
            }
            // +0x3E9C CfObjectMove subobject (CfObjectActor.cpp spells the
            // same (u8*)self + 16028): refresh it directly, then dispatch
            // its real slots 0x10 (CObjectState::setStateBitFlag) and 0x20
            // (CObjectState::clearStateFlags8).
            cf::CfObjectMove* container = reinterpret_cast<cf::CfObjectMove*>(
                reinterpret_cast<u8*>(rawObj) + 0x3E9C);
            CfObjectMove_setAnimModeArgs(reinterpret_cast<u8*>(container), 1, 0, -1, 1);
            container->setStateBitFlag(4);
            container->clearStateFlags8(0x1000);
        }
    }
    for (int i = 0; i < 5; ++i) {
        CfScript_UpdateMgr();
        gmDispatchPair(cf::CfGameManager::getPlayer(0), 1);
    }
    CfT_PlayClockAccumulate(arg1, arg2, 0);
    Func800B6BECList* list = getReslistB68();
    for (Func800B6BECNode* node = list->head->next; node != list->head;
         node = node->next) {
        void* rawObj = MapFx_ResolveOwner(node->object);
        if (rawObj == nullptr) {
            continue;
        }
        // Same +0x3E9C CfObjectMove subobject refresh as above.
        cf::CfObjectMove* container = reinterpret_cast<cf::CfObjectMove*>(
            reinterpret_cast<u8*>(rawObj) + 0x3E9C);
        CfObjectMove_setAnimModeArgs(reinterpret_cast<u8*>(container), 1, 0, -1, 1);
        container->setStateBitFlag(4);
        container->clearStateFlags8(0x1000);
    }
    gmPauseFlags &= ~0x00800000;
    // 8 pump frames when arg1 divides evenly by 3, else 16.
    int count = (arg1 % 3 == 0) ? 8 : 16;
    for (int i = 0; i < count; ++i) {
        CfScript_UpdateMgr();
        gmDispatchPair(cf::CfGameManager::getPlayer(0), 1);
    }
    gmPauseFlags &= ~0x04000000;
    setPresentationFlag__Q22cf13CfGameManagerFv(false);
}
void cf::CfGameManager::enablePadFlags(u32 inputFlags, bool enable) {
    u32 enabledFlags = gmEnabledPadFlags & ~inputFlags;
    if (enable) {
        enabledFlags = gmEnabledPadFlags | inputFlags;
    }
    gmEnabledPadFlags = enabledFlags;

    if (gmCurrentPad == nullptr) {
        return;
    }
    if (enable) {
        return;
    }

    gmCurrentPad->mPressedButtonFlags &= enabledFlags;
    gmCurrentPad->mTurboPressButtonFlags &= enabledFlags;
    gmCurrentPad->mReleasedButtonFlags &= enabledFlags;
    gmCurrentPad->mHeldButtonFlags &= enabledFlags;
    gmCurrentPad->mLongHoldButtonFlags &= enabledFlags;
    gmCurrentPad->mShortPressButtonFlags &= enabledFlags;
    gmPadData.mPressedButtonFlags &= enabledFlags;
    gmPadData.mTurboPressButtonFlags &= enabledFlags;
}
u32 cf::CfGameManager::getEnabledInputFlags() {
    if (isSceneLoading()) {
        if (isAnyMenuOpen__9CMainMenuFv() || getInstance__11CSysWinBuffFv() != nullptr ||
            isInitialized__10CMenuPauseFv()) {
            return 0xFFFFFFFF;
        }
    }
    return gmEnabledPadFlags;
}
bool cf::CfGameManager::isClassicController(s16 selector) {
    return CfPadTask::getDetectedPadType() == PAD_TYPE_CLASSIC;
}
CPad* cf::CfGameManager::getPad(int channel) {
    return &gmPadTable[channel & 7];
}
extern "C" void setPad__Q22cf13CfGameManagerFv(u32 channel, CPad* pad, u32 arg3) {
    gmPadTable[channel & 7] = *pad;
    if (gmActivePadChannel == 0xFFFFFFFF && channel <= 3 && arg3 != 0) {
        gmActivePadChannel = channel;
    }
}
void cf::CfGameManager::setCurrentPadPtr(const CPad* pad, u32 channel) {
    if (pad != nullptr && gmActivePadChannel != 0xFFFFFFFF && gmActivePadChannel != channel) {
        return;
    }

    gmCurrentPad = const_cast<CPad*>(pad);
}
extern "C" void clearTextBufferExtra(Unk87588Data* data) {
    data->field_0x0 = 0;
    data->field_0x20 = 0;
    data->field_0x24 = 0;
    data->field_0x44 = 0;
    data->field_0x48 = lbl_eu_80666548;
}
extern u16 gNandTeardownFlag;
bool cf::CfGameManager::isSceneReadyForInput() {
    if (gmScene == nullptr) {
        return true;
    }
    return Scn_IsDefaultScale(gmScene);
}

bool cf::CfGameManager::isSceneActive() {
    if (gmScene == nullptr) {
        return false;
    }
    return Scn_IsScaleAtLeastOne(gmScene);
}

bool cf::CfGameManager::isFieldTransitionReady() {
    if ((gmGameFlags & 0x8000) != 0 && !CfScript_TestMask(0x20)) {
        return false;
    }
    return probeReadyLists();
}

bool cf::CfGameManager::isFieldBlockedByFlag() {
    if ((gmGameFlags & 0x48000) != 0) {
        if (CfScript_TestMask(0x20)) {
            return true;
        }
    }
    return false;
}

bool cf::CfGameManager::isAnyFieldFlagSet() {
    return (gmGameFlags & 0x48000) != 0;
}

bool cf::CfGameManager::isSceneLoading() {
    if (testResInfoFlag(0x04000000)) {
        return false;
    }

    getInstance();
    return Scn_GetFrameDelta(gmScene) <= ml::epsilon;
}

bool cf::CfGameManager::isAllEventsDone() {
    return gNandTeardownFlag != 0;
}

bool cf::CfGameManager::isTimerActive() {
    return gmTimerCounter > 0;
}

u16 cf::CfGameManager::getField18Value() {
    return field_0x18;
}

cf::CfObject** cf::CfGameManager::getField04Ptr() {
    return &field_0x4;
}

// Retail Fv symbol: callers leave r4 = name for slot 0x120; forced-name form
// keeps the Fv linker name while forwarding the live argument onto the real
// CfObject slot (findNodeMatrix alias, same arity, returns void* for the
// tail-call). CfObjectImplPc's vtable serves this entry at +0x68.
extern "C" void* dispatchObjectFunc52b__Q22cf13CfGameManagerFv(
    cf::CfGameManager* self, const char* name) {
    return self->field_0x4->CfObject_findNodeMatrix(name);
}

void cf::CfGameManager::dispatchObjectFunc53(u32 idx) {
    field_0x4->CfObject_getIndexedNodeMatrix(idx);
}

void cf::CfGameManager::dispatchObjectFunc48() {
    field_0x4->CfObject_getCurrentTarget();
}

// Retail Fv symbol: callers leave r4 = arg for slot 0x108; forced-name form
// keeps the Fv linker name while forwarding the live argument onto the real
// CfObject::setMoveTargetPtr slot. CfObjectImplPc's vtable serves this entry
// at +0x60.
extern "C" void dispatchObjectFunc46__Q22cf13CfGameManagerFv(
    cf::CfGameManager* self, void* arg) {
    self->field_0x4->CfObject_setMoveTargetPtr(arg);
}

void cf::CfGameManager::dispatchObjectFunc56() {
    field_0x4->CfObject_getHeightDiff();
}

void cf::CfGameManager::dispatchObjectFunc58() {
    field_0x4->CfObject_getMoveRateScale();
}

void cf::CfGameManager::dispatchObjectFunc31() {
    field_0x4->CfObject_getMoveHeadAngle();
}

// Retail Fv symbol: callers leave f1 = value for slot 0xC4; forced-name form
// keeps the Fv linker name while forwarding the live argument onto the real
// CfObject::setMoveHeadAngle slot. CfObjectImplPc's vtable serves this entry
// at +0x4C.
extern "C" void dispatchObjectFuncC4__Q22cf13CfGameManagerFv(
    cf::CfGameManager* self, float value) {
    self->field_0x4->CfObject_setMoveHeadAngle(value);
}

void cf::CfGameManager::dispatchObjectFunc23() {
    field_0x4->CfObject_getPosVector();
}

// Retail Fv symbol: callers leave r4 = vec for UVF19; forced-name form keeps
// the Fv linker name while forwarding the live argument.
extern "C" void dispatchObjectFunc19__Q22cf13CfGameManagerFv(
    cf::CfGameManager* self, const ml::CVec3* vec) {
    self->field_0x4->CfObject_setMoveTargetVec(vec);
}

// Retail .text:0x80142524, unmangled global (the CfBdat static of the same name
// is a different, mangled symbol). Called with `this` in r3 at both retail call
// sites; no definition exists in this tree yet, so it stays a link-frontier item.
extern "C" void* bdatFetchCache(void* self);

void cf::CfGameManager::syncFieldAndBattle() {
    bdatFetchCache(this);
    setMgrFixStrName((const char*)this);
}

void cf::CfGameManager::cleanupMapEffects() {
    if (getGameSubManager() != nullptr && &getGameSubManager()->field_0xF0 != nullptr) {
        flushMpfBuffer__17UnkClass_8047BB54Fv(&getGameSubManager()->field_0xF0);
    }
}

u32 cf::CfGameManager::getEffectFlagState() {
    UnkClass_80083298* object = getGameSubManager();
    return object != nullptr ? object->getEffectFlags() : 0;
}

void cf::CfGameManager::tickGameManager() {
    getInstance()->func_8007C8C8();
}

// cf::CfGameManager::func_8007C8C8 - per-frame scene/presentation tick and
// the scene-load / object-queue dump. Returns the CfCmd_PumpRingBuf result.
// Flag bits use the local convention (value bit = 31 - PPC bit): E24 0x80,
// 0x1000, 0x400000, 0x40000|0x8000, 0x2000000|0x400; E28 0x1000000, 0x200,
// 0x8000000.
int CfCmd_PumpRingBuf(void* buf, u32* outFlag);
void NpcMove_ClearRowBmp();
extern "C" void gmNotifyCA0(void* object);
void KyoshinHeap_Init();
void gmCallInit1120();
void CmText_InitCallbackTable();
void CmText_RegisterCallback(void (*fn)(), int);
// (memberized: KizunagramSetActiveSlot -> CPcKizunagramBig::SetActiveSlot; declaration moved into class CPcKizunagramBig)
extern "C" void MiniMapResetLayoutAnims();

void* cf::CfGameManager::func_8007C8C8() {
    void* ret = (void*)CfCmd_PumpRingBuf(this->unkAC, &gmCommandBufferState);
    if (gmSubManager60 != 0) {
        MenuSnd_RampMasterVol_889D0(gmSubManager60);
    }
    // updateBattleManager takes cf::CBattleManager* (decl on CBattleManager.hpp);
    // explicit cast - the singleton getter returns void* and MWCC rejects
    // the implicit void* conversion.
    updateBattleManager((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv());
    if (gmGameFlags & 0x80) {
        updateHelpManager(lbl_eu_80664A10);
    }
    CCharVoiceMan_Tick();
    if (gmScene != nullptr &&
        Scn_GetFrameDelta(gmScene) > lbl_eu_80666498) {
        ++gCfTEventFrameCounter;
    }
    if (!(gmPauseFlags & 0x1000000)) {
        CfT_AdvanceFrameTimer();
    }
    if (getEffectFlagState__Q22cf13CfGameManagerFv() != 0) {
        ++gmFrameCounter;
        if (!(gmFrameCounter & 1)) {
            if (gmGameFlags & 0x1000) {
                ArtsSelect_SyncRows(this->field_0xA4);
            }
        } else if ((gmFrameCounter & 1) == 1 && (gmGameFlags & 0x1000)) {
            this->unkA0->forwardReload();
        }
        if ((gmGameFlags & 0x1000) && !(gmPauseFlags & 0x1000000)) {
            this->unkA0->updateSceneFx();
        }
        this->updateCameraState();
    }
    if ((gmGameFlags & 0xAFA40000) == 0 ||
        (gmPauseFlags & 0x1000000) == 0) {
        Selector_GetInstance();
        Selector_FrameUpdate();
        if (getEffectFlagState__Q22cf13CfGameManagerFv() != 0) {
            if (gmGameFlags & 0x1000) {
                refreshNpcRowFlags();
            }
            // Retail loads E24 once and tests bit 26 (rlwinm 6,6) OR'd
            // with bit 21 (rlwimi 21,21).
            u32 e24bits = gmGameFlags;
            if ((((e24bits & 0x04000000) | (e24bits & 0x400)) != 0) &&
                (gmPauseFlags & 0x1000000) == 0) {
                goto done;
            }
            if (!gmInitFlag) {
                __ct__Q22cf13CfGameManagerFv(&gmInstance);
                __register_global_object(&gmInstance,
                                         __dt__Q22cf13CfGameManagerFv,
                                         gmStaticRegistration);
                gmInitFlag = 1;
            }
            cf::CfGameManager* gmn = &gmInstance;
            if (gmn != nullptr) {
                if (!gmInitFlag) {
                    __ct__Q22cf13CfGameManagerFv(gmn);
                    __register_global_object(gmn, __dt__Q22cf13CfGameManagerFv,
                                             gmStaticRegistration);
                    gmInitFlag = 1;
                }
                if (isGameFlagMaskSet(0x1000)) {
                    cf::CfObjectMove* player = getPlayer(0);
                    if (player != nullptr) {
                        updateReloadP1(player->CfObject_getPosVector());
                    }
                }
            }
            CfT_PlayClockUpdate();
            if (isSceneLoading() == 0 &&
                cf::CfPadTask::isInputDisabled() == 0) {
                int sceneReady = gmScene == nullptr
                                     ? 1
                                     : Scn_IsDefaultScale(gmScene);
                if (sceneReady != 0 &&
                    (gmGameFlags & 0x400000) == 0 &&
                    (gmGameFlags & 0xAFA40000) == 0 &&
                    ((gmGameFlags & 0x04000000) |
                     (gmGameFlags & 0x400)) == 0 &&
                    (gmGameFlags & (0x40000 | 0x8000)) == 0 &&
                    (gmPauseFlags & 0x200) == 0 &&
                    BpsStateSingletonToInt() &&
                    (gmPauseFlags & 0x8000000) == 0 &&
                    (gmGameFlags & 0x80) != 0) {
                    flushEventQueue__Q22cf13CfGameManagerFv();
                }
            }
        }
    }
done:
    u32 v1 = CfT_FrameCountC();
    u32 v2 = CfT_FrameCountB();
    u32 v3 = CfT_FrameCountA();
    dispatchLODArgs__8CTaskLODFv((u16)v3, (u16)v2, (u16)v1);
    func_8007CBEC__Q22cf13CfGameManagerFv();
    return ret;
}
void cf::CfGameManager::refreshCameraIfCurrentView() {
    if (CView::getCurrentView() == gmCurrentView) {
        gmCallNopA();
    }
}

void cf::CfGameManager::resetCameraIfCurrentView() {
    if (CView::getCurrentView() == gmCurrentView) {
        gmCallNopB();
    }
}

void cf::CfGameManager::fadeOutGameEffects() {
    setMasterVolume(0.65f, lbl_eu_8066649C);
    MenuSnd_SetMasterAndPush_9510(0.7f);
    MenuSnd_SetSeVolPush_95F4(lbl_eu_8066649C);
}

void cf::CfGameManager::notifyCameraManager() {
    CamEvtStepShk(getInstance()->unkB4);
}

bool cf::CfGameManager::isCameraReady() {
    return CamEvtShkActv(getInstance()->unkB4);
}

UnkClass_800821F8* cf::CfGameManager::getCameraDataBlock() {
    return getInstance()->unkB0;
}

cf::CfObject* cf::CfGameManager::getActiveCameraObject() {
    CfCamEventManager* manager = getInstance()->unkB4;
    return manager != nullptr ? cfCam_getActiveObj(manager) : nullptr;
}

u16 cf::CfGameManager::popEventQueue() {
    s32 oldIndex = field_0x44;
    field_0x44++;
    field_0x44 %= field_0x4C;
    field_0x48--;
    return field_0x40[oldIndex];
}

cf::CfMapEffectManager* cf::CfGameManager::getMapEffectManager() {
    return getInstance()->unkA0;
}

void cf::CfGameManager::loadBdatGroupData() {
    CtrlObjectParam_ResolveEquipItem(&field_0x1C);
}

bool cf::CfObjectMap::isObjectMapReady() {
    return field_0x8E >= 2;
}

void cf::CfGameManager::notifyObjectMapChange() {
    if (gmGameFlags & 0x1000) {
        ArtsSelect_SyncRows(field_0xA4);
    }
}

void cf::CfObject::CfObject_applyMoveYaw(float amount) {
    CfObject_syncMoveHead(amount);
}

// Retail Fv linker name kept for hand-built vtables (CfCollSphereImpl /
// CfObjectModel). Member emits CfObject_applyMoveYaw__...Ff.
extern "C" void CfObject_applyMoveYaw__Q22cf8CfObjectFf(
    cf::CfObject* object, float amount) {
    object->CfObject_syncMoveHead(amount);
}

void cf::CfGameManager::enableFlag10() {
    gmGameFlags |= 0x10;
}

void cf::CfGameManager::enableFlag20() {
    gmGameFlags |= 0x20;
}

void cf::CfGameManager::enableFlag40() {
    gmGameFlags |= 0x40;
}

void cf::CfGameManager::disableFlag20() {
    gmGameFlags &= ~0x20;
}

bool cf::CfGameManager::isVisionPackLoaded() {
    return CtrlRemote_TouchBitByArg(0x3508) != 0;
}

void* cf::CfGameManager::getSelf() { return this; }
void cf::CfGameManager::stubEmptyA() {}
void cf::CfGameManager::dispatchObjectFunc52() {}
void cf::CfObject::CfObject_setMoveTargetPtr(void*) {}

// Retail Fv base impl (us/jp/eu 0x80087D64: bare blr) for vtable slot 0x108.
// Same dual-symbol shape as CfObject_applyMoveYaw above: the C++
// member def emits the FPv override-chain symbol (referenced by the
// CfCollSphere/CfObjectEff hand vtables); this forced-name extern "C"
// form emits the retail Fv linker name (referenced by the CfObjectModel
// hand vtables). Empty body: retail is a single blr.
extern "C" void CfObject_setMoveTargetPtr__Q22cf8CfObjectFPv(
    cf::CfObject*) {}
void cf::CfObject::CfObject_createMoveTarget(int) {}

void cf::CfGameManager::stubEmptyB() {}
void cf::CfGameManager::stubEmptyC() {}
void cf::CfGameManager::stubEmptyD() {}
void cf::CfGameManager::stubEmptyE() {}
extern "C" void* getNullPtrA__Q22cf13CfGameManagerFv() { return nullptr; }
extern "C" void* getNullPtrB__Q22cf13CfGameManagerFv() { return nullptr; }
extern "C" void* getNullPtrC__Q22cf13CfGameManagerFv() { return nullptr; }
// Slot +0x124 base: retail Fv returns nullptr. Forced-name keeps the Unk
// linker spelling for hand-built vtables; virtual is CfObject_getIndexedNodeMatrix.
extern "C" cf::CfObject* CfObject_getIndexedNodeMatrix__Q22cf8CfObjectCFUl(
    cf::CfObject*) {
    return nullptr;
}
extern u32 gObjectMoveRateScale;
u32* cf::CfObject::CfObject_getMoveRateScale() {
    return &gObjectMoveRateScale;
}
// lwz getters - return global vars
extern u32 gCachedFldMapRow;
u32 cf::CfGameManager::getGlobalWord64188() { return gCachedFldMapRow; }
extern u32 lbl_eu_80664184;
u32 cf::CfGameManager::getGlobalWord64184() { return lbl_eu_80664184; }
extern "C" u32 getGlobalWord64184Dup__Q22cf13CfGameManagerFv() { return lbl_eu_80664184; }
extern u32 lbl_eu_8066409C;
extern "C" u32 getGlobalPtr6409C__Q22cf13CfGameManagerFv() { return lbl_eu_8066409C; }
extern void* lbl_eu_806640A8;
extern "C" u32 getGlobalPtr640A8__Q22cf13CfGameManagerFv() { return (u32)lbl_eu_806640A8; }
extern u32 lbl_eu_806640A4;
extern "C" u32 getGlobalWord640A4__Q22cf13CfGameManagerFv() { return lbl_eu_806640A4; }
extern void* lbl_eu_806640F4;
extern "C" u32 getGlobalWord640F4__Q22cf13CfGameManagerFv() { return (u32)lbl_eu_806640F4; }

extern "C" u32 getFieldD90Value__Q22cf13CfGameManagerFv() { return gCfTEventFrameCounter; }
extern u8 gmGlobalFlag34;
extern "C" u8 getGlobalFlag34__Q22cf13CfGameManagerFv() { return gmGlobalFlag34; }
extern u8 gmVoiceFlag5D;
u8 cf::CfGameManager::getVoiceFlag5D() { return gmVoiceFlag5D; }
extern u8 gNandTeardownByte;
u8 cf::CfGameManager::getVoiceFlag6D() { return gNandTeardownByte; }
void* cf::CfGameManager::getResetDataPtr() { return lbl_eu_8065FC18; }

// cf::CfGameManager::func_8007D84C - battle-exit / scene reset. Tears down
// the mode subsystems, wipes the manager's per-mode state block, clears the
// presentation globals and the E24/E28 flag masks.
extern "C" void gmCallInit1954();
extern "C" void resetBattleState__Q22cf14CBattleManagerFv(void* battle);
extern "C" void CfRes_updateEntry(u32 a, u32 b, u32 c, u32 d);
extern "C" void CfRes_getInstPtr170();
extern "C" void EffSys_Init4F9C(void* object);
extern "C" float lbl_eu_80667700;
void cf::CfGameManager::func_8007D84C() {
    extern u16 gmEventCounterA;
    func_801AAD08();
    gSoundSystemInitFlag = 0;
    ArtsSelect_Clear(ArtsSelect_GetContainer());
    CfSoundMan_StopAllModes(1);
    CfSoundMan_ClearFxEffect(0, 0);
    CfSoundMan_ClearFxEffect(1, 0);
    UIBattleRemoveAll(0);
    CUICfManager_setFlagState(0);
    if ((gEventSequenceFlags & 0x40) != 0) {
        gEventSequenceFlags &= ~0x40;
    }
    memset(unk94, 0, sizeof(unk94));
    unk90 = nullptr;
    float zeroF = lbl_eu_80666498;
    gmGlobalFlag50 = 0;
    field_0x86 = 0;
    gmEventCounterA = 0;
    gmTimerCounter = 0;
    gmTransitionCounter = 0;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x80) = 0;
    gmGlobalFlag34 = 0;
    gmEventFlag35 = 0;
    gmFloat68 = zeroF;
    gmCommandCounter = 0;
    gmBaseTimeScale = zeroF;
    if (unkA8 != nullptr) {
        BattleWork_InitCtrlWork((CtrlWork*)unkA8);
    }
    if (unkA0 != 0) {
        unkA0->releaseHandle();
    }
    if (field_0xA4 != nullptr) {
        ArtsSelect_FreeSlots(field_0xA4);
    }
    CfCamEventManager* camManager = unkB4;
    {
        ml::CVec3* zeroVec = &zero__Q22ml5CVec3;
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x18) = zeroVec->x;
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x1C) = zeroVec->y;
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x20) = zeroVec->z;
    }
    if (camManager != nullptr) {
        CamEvtReleaseAllSlots(camManager);
    }
    unkB0 = nullptr;
    CfResPcFileHost_clear(unkAC);
    gmResetSubMgrs();
    gmCallInit1954();
    void* resetObject = lbl_eu_8065FC18;
    if (resetObject != 0) {
        EffSys_Init4F9C(resetObject);
    }
    CfRes_getInstPtr170();
    CfRes_updateEntry(0, 0, 0, 0);
    Selector_GetInstance();
    __dt__800FDEF8();
    resetBattleState__Q22cf14CBattleManagerFv(
        getInstance__Q22cf14CBattleManagerFv());
    CfScript_ClearReq0();
    CfScript_ClearReq1();
    CfScript_ClearReq2();
    gmGameFlags = gmGameFlags & 0x505B0208;
    gmPauseFlags = gmPauseFlags & 0xFBFFF430;
    NpcMove_ClearRowBmp();
    cf::CfMapEffectManager::setGlobalFloat(lbl_eu_80667700);
}

extern "C" Unk8187CData* lookupResourceByKeys__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third) {
    return reinterpret_cast<Unk8187CData*>(spawnFromRecord(reinterpret_cast<UnkClass_800AA714*>(first), reinterpret_cast<UnkClass_800AA714*>(second), third));
}

void cf::CfGameManager::clearPlayerEffect() { gmFileObject(this); }

// Static member (same mangled Fv symbol as retail): the body is a bare
// tail-call to gmSpawnMask8000, so the returned object rides in r3.
// Retail calls this out-of-line from func_80086778 - keep it a real function.
void* cf::CfGameManager::createBdatCollisionObj() { return gmSpawnMask8000(); }

void* mapKindToSpawn(void* object, u32 arg);
extern "C" Unk80EE4Data* createEffectForResource__Q22cf13CfGameManagerFv(
    void* object, u32 value) {
    return static_cast<Unk80EE4Data*>(mapKindToSpawn(object, value));
}

void cf::CfGameManager::resetFieldState() { gmNotifyCA0(this); }

typedef int (*CounterTestBitNoArg)(void*);  // DEBT(callcast): retail passes only r3 to Counter_TestBit; canonical arity differs (LOSTFIX proof)
void cf::CfGameManager::cleanupPlayerEffectList() { ((CounterTestBitNoArg)Counter_TestBit)(this); }

void cf::CfGameManager::getGimmickListHead() { getListB28__Fv(); }

extern "C" void* getReslistB48();
void cf::CfGameManager::getFirstGimmick() { getReslistB68(); }
void cf::CfGameManager::getGimmickList() { getReslistB48(); }

extern "C" void* getReslistB48();
void cf::CfGameManager::spawnGimmickEntity() { getReslistB88(); }

void cf::CfGameManager::clearGimmickFlags() { getReslistBE8(); }

void cf::CfGameManager::isGimmickActive() { lookupWorkAtAddr(this); }

int cf::CfGameManager::initPadSubSystem() { bdatTotalRows(); }

float cf::CfGameManager::updatePadState() { return CfT_PlayRateGet(); }

void cf::CfGameManager::loadControllerConfigA() { CfT_PlayClockAdvanceFrame((u32)this); }

void cf::CfGameManager::loadControllerConfigB() { CfT_PlayClockAdvanceInt(); }

extern "C" void getControllerValues__Q22cf13CfGameManagerFv(u16* first, u16* second) { CfT_PlayClockSnapshot(first, second); }

bool cf::CfGameManager::isControllerReady() { return CfT_FrameInWindow(); }

u32 cf::CfGameManager::getControllerWordA33C() { return CfT_FrameCountA(); }

u32 cf::CfGameManager::getControllerWordA37C() { return CfT_FrameCountB(); }

void cf::CfGameManager::getControllerWordA3BC() { CfT_FrameCountC(); }

void cf::CfGameManager::resetControllerState() { CfT_FrameCounterStore(); }

void cf::CfGameManager::clearControllerState() { CfT_FrameCounterLoad(); }

u32 cf::CfGameManager::getCurrentSlotIndex() { return CfT_DispatchFrameCounts(); }

u16 cf::CfGameManager::getItemId() {
    ItemListEntryView* object = reinterpret_cast<ItemListEntryView*>(this);
    return object->itemId_0x8C;
}

extern u16 gmEventCounterA;
extern "C" u16 getEventCounterA__Q22cf13CfGameManagerFv() { return gmEventCounterA; }

extern "C" void setEventCounterA__Q22cf13CfGameManagerFv(u16 value) {
    gmEventCounterA = value;
}

struct Unk812Data {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u16 field_0xC;
    u16 field_0xE;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u16 field_0x1C;
    u16 field_0x1E;
    u16 field_0x20;
    u16 field_0x22;
    u16 field_0x24;
};

extern "C" void setResourceField04__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x4 = value; }
extern "C" void getBdatTextId0__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 0); }
extern "C" void setResourceField08__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x8 = value; }
extern "C" void getBdatTextId1__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 1); }
extern "C" void setResourceField10__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x10 = value; }
extern "C" void getBdatTextId2__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 2); }
extern "C" void setResourceField0C__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { *reinterpret_cast<u32*>(reinterpret_cast<u8*>(data) + 0xC) = value; }
extern "C" void getBdatTextId3__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 3); }
extern "C" void setResourceField14__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x14 = value; }
extern "C" void getBdatTextId4__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 4); }
extern "C" u16 getResourceField0E__Q22cf13CfGameManagerFv(Unk812Data* data) { return data->field_0xE; }
extern "C" u16 getResourceWord10__Q22cf13CfGameManagerFv(const u8* data) { return *reinterpret_cast<const u16*>(data + 0x10); }
extern "C" u16 getResourceWord12__Q22cf13CfGameManagerFv(const u8* data) { return *reinterpret_cast<const u16*>(data + 0x12); }
extern "C" u16 getResourceWord14__Q22cf13CfGameManagerFv(const u8* data) { return *reinterpret_cast<const u16*>(data + 0x14); }
extern "C" u16 getResourceWord16__Q22cf13CfGameManagerFv(const u8* data) { return *reinterpret_cast<const u16*>(data + 0x16); }
extern "C" void getBdatTextId5__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 5); }
extern "C" u16 getResourceField0C_2__Q22cf13CfGameManagerFv(Unk812Data* data) { return data->field_0xC; }
extern "C" void setResourceField24__Q22cf13CfGameManagerFv(Unk812Data* data, u16 value) { data->field_0x24 = value; }
extern "C" void setResourceField00__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x0 = value; }
extern "C" void setResourceField18__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x18 = value; }
extern "C" void setResourceField20__Q22cf13CfGameManagerFv(Unk812Data* data, u16 value) { data->field_0x20 = value; }
extern "C" void setResourceField22__Q22cf13CfGameManagerFv(Unk812Data* data, u16 value) { data->field_0x22 = value; }
extern "C" void setResourceField1C__Q22cf13CfGameManagerFv(Unk812Data* data, u16 value) { data->field_0x1C = value; }
extern "C" void setResourceField1E__Q22cf13CfGameManagerFv(Unk812Data* data, u16 value) { data->field_0x1E = value; }

Unk817A8Object* cf::CfGameManager::getManagerObjectA8() {
    return (Unk817A8Object*)this->mObjectFlags;
}

extern "C" void setObjectFlags64__Q22cf13CfGameManagerFv(Unk8187CData* data, u32 value) { data->flags_0x64 = value; }

extern "C" void setObjectItemId__Q22cf13CfGameManagerFv(u8* data, u16 value) { *reinterpret_cast<u16*>(data + 0x8C) = value; }

UNKWORD cf::CfGameManager::getQueuedFileEventCount() { return CtrlRemote_TouchBitByArg(0x20); }

extern "C" u32 isEventFlagActive__Q22cf13CfGameManagerFv(u32 data) { return CtrlRemote_TouchBitByArg(data + 0x312C); }
extern "C" u32 getEventValue40__Q22cf13CfGameManagerFv(u8* data) { return CtrlRemote_TouchBitByArg(reinterpret_cast<u32>(data + 0x40)); }


extern "C" s32 getStateTimer532__Q22cf13CfGameManagerFv(const u8* data) {
    return *reinterpret_cast<const s16*>(data + 0x532);
}

extern u8 gFixedMapItemFlag;
void cf::CfGameManager::setGlobalByte64298(u32 value) { gFixedMapItemFlag = value; }

extern "C" void* getDataArray219C__Q22cf13CfGameManagerFv(u8* data) { return data + 0x219C; }

void cf::CfGameManager::resetCameraManager() { Scn_GetUnk80Handle(gmScene); }

// Slot +0x130 base: retail Fv returns the shared float const. Forced-name
// keeps the Unk linker spelling for hand-built vtables; virtual is
// CfObject_getHeightDiff.
extern "C" float CfObject_getHeightDiff__Q22cf8CfObjectCFv(cf::CfObject*) {
    return lbl_eu_80666498;
}

u32 cf::CfGameManager::getObjectFlags() {
    return mObjectFlags;
}

bool cf::CfGameManager::isGameFlagEnabled() {
    return (gmGameFlags & 0x80) != 0;
}

u32 cf::CfGameManager::getPackedResourceBits() {
    const u32* packed = reinterpret_cast<const u32*>(this);
    return *packed >> 20;
}

void** cf::CfGameManager::getObjectNodePtr() {
    UnkF8C0Node* iterator = reinterpret_cast<UnkF8C0Node*>(this);
    ItemListNode* node = reinterpret_cast<ItemListNode*>(iterator->field_0x0);
    return reinterpret_cast<void**>(&node->object);
}


extern void __fill_mem(void*, int, int);

bool cf::CfGameManager::isManagerFlag82C() {
    return (*reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(this) + 0x82C) >> 2) & 1;
}

extern "C" void linkEffectObjects__Q22cf13CfGameManagerFv(void*, void* first, void* second) { bdatSetResRow(first, second); }

u32 cf::CfGameManager::isField4ECFlag16() {
    return (field_0x4EC >> 16) & 1;
}

u32 cf::CfGameManager::isField4ECFlag07() {
    return (field_0x4EC >> 7) & 1;
}

extern "C" u32 isStateFlag20Set__Q22cf13CfGameManagerFv(const u8* data) {
    return (*reinterpret_cast<const u32*>(data + 0x4EC) >> 20) & 1;
}

extern "C" void CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv(u8* data) {
    // Slot +0x48 base: clear +0x34. Real API alias is CObjectParam_clearExtraWord;
    // forced-name Unk Fv spelling kept for hand-built vtables.
    *reinterpret_cast<u32*>(data + 0x34) = 0;
}

bool cf::CfGameManager::isFlag20Set() {
    return (gmGameFlags & 0x20) != 0;
}

bool cf::CfGameManager::isFlag10Set() {
    return (gmGameFlags & 0x10) != 0;
}

extern u32 gmPauseFlags;
bool cf::CfGameManager::isFlag010000Set() {
    return (gmPauseFlags & 0x100000) != 0;
}

cf::CfObjectSub54* cf::CfGameManager::getObjectSub54Ptr() {
    return &field_0x4->mSubObj54;
}

void cf::CfGameManager::clearObjectFlagsAll() {
    mObjectFlags = 0;
}

// Slot +0x1F8/+0x200 base impls: store/load +0x1608. Virtuals are
// CActorParam_setSecondCurrency / getSecondCurrency; forced-name Unk Fv
// spellings kept for hand-built CREvtModel vtables.
extern "C" void CActorParam_setSecondCurrency__Q22cf11CActorParamFUl(u8* data, u32 value) {
    *reinterpret_cast<u32*>(data + 0x1608) = value;
}

extern "C" u32 CActorParam_getSecondCurrency__Q22cf11CActorParamFv(const u8* data) {
    return *reinterpret_cast<const u32*>(data + 0x1608);
}

extern "C" cf::UnkClass_CActorParam15E0* CActorParam_getStatusTable__Q22cf11CActorParamFv(
    u8* data) {
    return *reinterpret_cast<cf::UnkClass_CActorParam15E0**>(data + 0x15E0);
}



void cf::CfGameManager::func_8007C6C0() {}

// Heap object allocated for the manager's unkAC slot; the manager back-
// pointer lives at +0x408.
struct UnkClass_8018EF3CObj {
    u8 field_0x0[0x408];
    cf::CfGameManager* field_0x408;
};
extern "C" void func_8007C6C0__Q22cf13CfGameManagerFv(cf::CfGameManager* self,
                                                      void* scene);

// UnkClass_8007DAE0::init - boot-time construction of the CfGameManager
// singleton and its process/subsystem graph. The lazy-ctor blocks are manual
// expansions of getInstance() (retail inlines all occurrences).
cf::CfGameManager* UnkClass_8007DAE0::init(CScnNw4r* scene, CView* view,
                                           u16 arg) {
    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    gmScene = reinterpret_cast<CScn*>(scene);
    cf::CfGameManager* mgr = &gmInstance;
    gmCurrentView = view;
    __ct__8009D604();
    __dt__801570F0();
    gmCommandBufferState = 0;
    if (mgr->unkAC == nullptr) {
        void* memory = allocate__Q23mtl10MemManagerFUlUl(0x40C, CfRes_getAllocHandle());
        void* object = memory;
        if (memory != nullptr) {
            object = __ct__Q22cf17UnkClass_8018EF3CFv(memory);
        }
        mgr->unkAC = static_cast<UnkClass_80085334*>(object);
        static_cast<UnkClass_8018EF3CObj*>(object)->field_0x408 = mgr;
    } else {
        CfResPcFileHost_clear(mgr->unkAC);
    }
    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    if (!(gmGameFlags & 0x10000)) {
        // First-boot path: tear down the battle manager, build the help manager
        // and the layered CProcess graph, then run the full subsystem reset.
        getBattleStateA__Q22cf14CBattleManagerFv();
    __ct__Q22cf12CHelpManagerFv(nullptr);
    CCharVoiceMan_CreateSingleton();
    CProcess* rtRoot = GetRootProcRealTime__12CTaskManagerFv();
    gmPadTask = create__Q22cf9CfPadTaskFP8CProcess(rtRoot, scene);
    CProcess* gameRoot = GetRootProcGame__12CTaskManagerFv();
    gmResManager = __ct__Q22cf5CfResFv(gameRoot, scene);
    gameRoot = GetRootProcGame__12CTaskManagerFv();
    gmTaskMain = cf::CfTaskMain::create(gameRoot, reinterpret_cast<CScn*>(scene));
    KyoshinHeap_Init();
    NpcMove_ClearRowBmp();
    gmCallInit1120();
    CfScript_InitMgr();
    func_8007C6C0__Q22cf13CfGameManagerFv(mgr, scene);
    CmText_InitCallbackTable();
    CmText_RegisterCallback(&cf::CfGameManager::tickGameManager, 0);
    gmInitArg = arg;
    // Bare Fv-name call: retail leaves r3 untouched here.
    func_8007DCB8__Q22cf13CfGameManagerFv();
    CtrlObjectParam_MoveValueToHead(CtrlObjectParam_GetSlotTableBase(), gmInitArg);
    s32 fallback = -1;
    syncBdatDataCache__Q22cf13CfGameManagerFv(gmInitArg, &fallback, 4);
    CfRes_tryRefreshSlot220(0x70100000, 4);
        CfRes_tryRefreshByBits(0x90100000, 5);
        CfScript_InitPath(1);
    }
    return mgr;
}

// cf::CfGameManager::func_8008566C - screen-tint color update carrying hidden
// (first, vector, mode) arguments under the retail Fv name.
extern "C" void func_8008566C__Q22cf13CfGameManagerFv(u32 first,
                                                      const UnkFloat4* vec,
                                                      u32 mode) {
    if ((gmPauseFlags & 0x1000000) != 0) {
        return;
    }
    if (mode != 0 && Scn_GetUnk80Handle(gmScene) != false && first != 0) {
        CTaskGameCamView* res =
            (CTaskGameCamView*)Scn_QueryUnk80State(gmScene); // cast: CfObjectMove.hpp's void* form is also in scope
        if (res->field_C <= ml::epsilon) {
            float temp[4];
            temp[0] = vec->field_0x0;
            temp[1] = vec->field_0x4;
            temp[2] = vec->field_0x8;
            temp[3] = vec->field_0xC;
            temp[3] = lbl_eu_80666498;
            Scn_ReleaseUnk80(gmScene, 0, temp);
        }
    }
    if (first == 0 || mode != 0) {
        Scn_ReleaseUnk80(gmScene, first,
                      const_cast<float*>(&vec->field_0x0));
        return;
    }
    // Scale path: truncate the scaled RGBA floats to bytes and push them to
    // the tint block on unkAC.
    float scale = 255.0f;
    u8 rgba[4];
    rgba[0] = (u8)(s32)(scale * vec->field_0x0);
    rgba[1] = (u8)(s32)(scale * vec->field_0x4);
    rgba[2] = (u8)(s32)(scale * vec->field_0x8);
    rgba[3] = (u8)(s32)(scale * vec->field_0xC);
    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    cf::CfGameManager* mgr = &gmInstance;
    if (mgr->unkAC != nullptr) {
        CfRes_pushRec(reinterpret_cast<u32>(mgr->unkAC), 0x10, first,
                      *reinterpret_cast<const u32*>(rgba), 0, 0);
    }
}

// cf::CfGameManager::func_8007C6C0 - full init/reset of the singleton state:
// stores the scene pointer, wipes the flag words / Bdat text entries / pad
// slot, then allocates the per-player file streams, camera manager, map-
// effect manager and the remaining subsystems. Declared extern "C" because
// the retail signature carries a hidden r4 scene argument under an Fv name.
extern "C" void func_8007C6C0__Q22cf13CfGameManagerFv(cf::CfGameManager* self, void* scene) {
    gmScene = static_cast<CScn*>(scene);
    self->resetManagerState();

    // rlwinm-mask flag clears (retail: two separate rlwinms with the E24
    // oris interleaved; mixed =/&= forms keep the two ANDs distinct)
    gmPauseFlags = gmPauseFlags & ~0x200000;
    gmGameFlags |= 0x10000;
    gmPauseFlags &= ~0x8000;
    float zero = lbl_eu_80666498;
    gmBaseTimeScale = zero;
    gmCurrentPad = nullptr;
    self->unk6C = 0;
    self->unk7C = 0;
    gmBdatNameText.text[0] = 0;
    gmBdatNameText.textLength = 0;
    gmEventQueue.field_0x48 = 0;
    gmEventQueue.field_0x44 = 0;
    gmInitArg = 0;
    gmQueuedEventCount = 0;
    gmGlobalFlag50 = 0;
    gmEventCounterA = 0;
    gmTimerCounter = 0;
    gmTransitionCounter = 0;
    gmGlobalFlag46 = 0;
    gmFlagPairs48[0] = 0;
    gmFlagPairs48[1] = 0;
    gmFlagPairs4C[0] = 0;
    gmFlagPairs4C[1] = 0;
    *reinterpret_cast<u32*>(self->unk80) = 0;
    gmCommandCounter = 0;
    gmGlobalFlag34 = 0;
    gmEventFlag35 = 0;
    gmTransitionIndex = -1;
    gmGameStateFlag5C = 0;
    gmTimeScale = zero;
    loadBdatFileTable__Q22cf6CfBdatFv();

    for (s32 i = 0; i < 3; ++i) {
        u32 heap = CfRes_getAllocHandle();
        void* memory = allocate__Q23mtl10MemManagerFUlUl(0xE5C, heap);
        void* object = memory;
        if (memory != nullptr) {
            object = __ct__cf_CfTFile(memory);
        }
        self->unk94[i] = static_cast<cf::CfObjectMove*>(object);
    }

    u32 heap = CfRes_getAllocHandle();
    void* memory = allocate__Q23mtl10MemManagerFUlUl(0x4E4, heap);
    void* object = memory;
    if (memory != nullptr) {
        object = __ct__80075298(memory);
    }
    self->unkB4 = static_cast<CfCamEventManager*>(object);

    heap = CfRes_getAllocHandle();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x13C, heap);
    self->unkA0 = new (memory) cf::CfMapEffectManager;

    heap = CfRes_getAllocHandle();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x180, heap);
    object = memory;
    if (memory != nullptr) {
        object = __ct__80186578(memory);
    }
    self->field_0xA4 = static_cast<u8*>(object);

    heap = CfRes_getAllocHandle();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x2944, heap);
    object = memory;
    if (memory != nullptr) {
        object = __ct__801BF76C(memory);
    }
    gmSubManager64 = reinterpret_cast<u32>(object);

    heap = CfRes_getAllocHandle();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x39C, heap);
    object = memory;
    if (memory != nullptr) {
        MenuSnd_ClearSlot_88584(memory);
    }
    gmSubManager60 = reinterpret_cast<u32>(object);

    heap = CfRes_getAllocHandle();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x1C20, heap);
    object = memory;
    if (memory != nullptr) {
        BattleWork_InitStateWork(memory);
    }
    self->unkA8 = static_cast<UnkClass_8007E864*>(object);

    if (sEneColiProbe != nullptr) {
        reinterpret_cast<Unk65958Object*>(sEneColiProbe)->field_0x7C =
            CfRes_getAllocHandle();
    }
    func_80069EA8(CfT_IsTvPal());
    clearReloadGlobals();
    cfCam_seedFollowD();
}
// cf::CfGameManager::func_8007CF64 - per-frame camera/target-state tick:
// updates the active text-display entry (gmBdatNameEntry or gmBdatTextEntries)
// when the player side indicator changes, then clears the 0x8 flag.
extern "C" void func_8007CF64__Q22cf13CfGameManagerFv(cf::CfGameManager* self) {
    if ((gmGameFlags & 0x4) != 0 || cf::CfGameManager::isSceneLoading() ||
        (gmGameFlags & 0x80) == 0) {
        return;
    }

    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player == nullptr || player->CfObject_isMoveActiveNow() == 0) {
        return;
    }
    if ((gEventWindowFlags & 1) != 0) {
        gmFloat68 = lbl_eu_80666498;
        return;
    }
    if ((gmPauseFlags & 0x800000) != 0) {
        return;
    }

    // side indicator: (E24 bit 3) forced on when the counted players change
    // to/from 4; the direction byte is sign-compressed through the
    // 2 | ~(X-1) arithmetic (retail bit-twiddle, kept verbatim).
    s32 flag3 = (gmGameFlags >> 3) & 1;
    u32 count = CfT_DispatchFrameCounts();
    s32 t = count - 1;
    u32 z = (2 | ~t) - ((u32)(2 - t) >> 1);
    s32 direction = z >> 31;
    s32 updated = 0;

    if ((u32)direction != (u32)gmLastDirection || flag3 != 0) {
        float value = gmFloat68 - CfT_PlayRateGet();
        gmFloat68 = value;
        if (value <= lbl_eu_80666498) {
            gmFloat68 = lbl_eu_80666498;
            gmLastDirection = static_cast<u8>(direction);
            float fade = getReloadDelay();
            if ((gmGameFlags & 0x40) != 0) {
                BdatTextEntry* entry = &gmBdatNameEntry;
                if (entry->textLength != 0) {
                    updated = 1;
                    if (gmTransitionCounter > 0) {
                        if (direction != 0) {
                            MenuSnd_StopSlot0Store_9390((const char*)entry);
                        } else {
                            MenuSnd_StopSlot0Store_9390((const char*)&entry->secondaryText);
                        }
                    } else if (direction != 0) {
                        UnkTextAreaView* area =
                            reinterpret_cast<UnkTextAreaView*>(entry);
                        MenuSnd_RestartBGM_88D34((const char*)area,
                                     (int)area->enabled_0x4C, area->value_0x48,
                                     fade);
                    } else {
                        UnkTextAreaView* area = reinterpret_cast<UnkTextAreaView*>(
                            &entry->secondaryText);
                        MenuSnd_RestartBGM_88D34((const char*)area,
                                     (int)area->enabled_0x4C, area->value_0x48,
                                     fade);
                    }
                    gmFloat68 = 60.0f;
                }
            } else {
                BdatTextEntry* entry = &gmBdatTextEntries[0];
                if (entry->textLength != 0) {
                    updated = 1;
                    if (gmTransitionCounter > 0) {
                        if (direction != 0) {
                            MenuSnd_StopSlot0Store_9390((const char*)entry);
                        } else {
                            MenuSnd_StopSlot0Store_9390((const char*)&entry->secondaryText);
                        }
                    } else if (direction != 0) {
                        UnkTextAreaView* area =
                            reinterpret_cast<UnkTextAreaView*>(entry);
                        MenuSnd_RestartBGM_88D34((const char*)area,
                                     (int)area->enabled_0x4C, area->value_0x48,
                                     fade);
                    } else {
                        UnkTextAreaView* area = reinterpret_cast<UnkTextAreaView*>(
                            &entry->secondaryText);
                        MenuSnd_RestartBGM_88D34((const char*)area,
                                     (int)area->enabled_0x4C, area->value_0x48,
                                     fade);
                    }
                    gmFloat68 = 60.0f;
                }
            }
            if (updated != 0) {
                gmGameFlags &= ~8;
            }
        }
    }
}
void cf::CfGameManager::func_8007D1A0() {}
void cf::CfGameManager::func_8007DCB8() {
    if (!gmBdatTableReady) {
        gmBdatTablePtr = lbl_eu_80664090;
        gmBdatTableReady = 1;
    }
    void* table = gmBdatTablePtr;
    char* base = lbl_eu_804FB824;
    for (s32 dataId = 1; dataId <= 13; ++dataId) {
        UnkClass_8009EC9C* data = static_cast<UnkClass_8009EC9C*>(CtrlObjectParam_GetCharEntry(static_cast<u16>(dataId)));
        reinterpret_cast<u16*>(data->field_0x0)[0] = static_cast<u16>(dataId);
        for (s32 col = 0; col <= 5; ++col) {
            u16 value = 0;
            if (col == 0) {
                value = static_cast<u16>(getBdatStringColumnValue(table, base + 0x28, dataId));
            } else if (col == 1) {
                value = static_cast<u16>(getBdatStringColumnValue(table, base + 0x31, dataId));
            } else if (col == 2) {
                value = static_cast<u16>(getBdatStringColumnValue(table, base + 0x3A, dataId));
            } else if (col == 3) {
                value = static_cast<u16>(getBdatStringColumnValue(table, base + 0x42, dataId));
            } else if (col == 4) {
                value = static_cast<u16>(getBdatStringColumnValue(table, base + 0x4C, dataId));
            } else if (col == 5) {
                value = static_cast<u16>(getBdatStringColumnValue(table, base + 0x55, dataId));
            }
            s16 result;
            s16 extra;
            s32 active = 0;
            if (value != 0 && gmInitArg != 0 && dataId <= 7) {
                CItem_findOrCreateRec(value, &result, 1, &extra);
                CtrlObjectParam_WriteU16RowEntry((cf::CtrlObjectParamU16RowTable*)data, col, value);
                CtrlObjectParam_WriteEquipSlotValue(data, col, reinterpret_cast<void*>(static_cast<s32>(result)));
                active = 1;
            }
            if (active == 0) {
                CtrlObjectParam_WriteEquipSlotValue(data, col, reinterpret_cast<void*>(-1));
                CtrlObjectParam_WriteU16RowEntry((cf::CtrlObjectParamU16RowTable*)data, col, value);
            }
        }
        CtrlObjectParam_RefreshCharEntry(data, 0);
    }
}
// Retail symbol is Fv but callers pass (row id, mode) in r3/r4 - declared
// extern "C" with a matching body so the Fv name is preserved while the
// hidden args stay ABI-visible.
extern "C" void func_8007E9CC__Q22cf13CfGameManagerFv(u16 value, u32 mode) {}

// cf::CfGameManager::func_8007F1FC - party roster refresh. Mirrors the 9
// party-slot ids into a local u16 list, clears the three live object slots,
// then reconciles the passed list against the roster (drop dead rows, insert
// new rows into the slot object). Afterwards averages the per-row item counts
// (rows 1..8, skipping 3) into a "leader" seed, then re-creates / re-slots
// each input row through the item-instance factory.
extern "C" void func_8007F1FC__Q22cf13CfGameManagerFv(void* inList, s32 mode) {
    cf::CtrlObjectParamSlots* party =
        reinterpret_cast<cf::CtrlObjectParamSlots*>(CtrlObjectParam_GetSlotTableBase());
    CtrlObjectParam_ValidateSlotRange();
    u16 match[9];
    const u32* slotWords = reinterpret_cast<const u32*>(party);
    for (int i = 0; i < 9; ++i) {
        match[i] = static_cast<u16>(slotWords[i + 1]);
    }

    const s16* list = static_cast<const s16*>(inList);

    // Camera-manager dispatch (getter invoked twice: null test + use).
    // Slot 0x5C is CfObject::syncEnableState (slot owner: cf::CfObject;
    // enable-state propagation hook; base impl + hand vtables spell the Fv
    // name in CfObjectEff/CfObjectModel/CfCollSphereImpl, so the virtual
    // keeps its Unk spelling and this calls the inline alias).
    // Retail passes r4 (resetBattleGauge forwards its data arg, here 0) -
    // the no-arg decl is a known cross-TU residual.
    // NOTE (W58): the alias does NOT inline here - the unit compiles with
    // -inline noauto, which also blocks ALL inlining INTO the caller
    // (repo-proven: MWCC_CASES ResTev::GXSetTevOrder). Decomp emits a `bl`
    // to the alias where retail has the virtual `bctrl`; recorded as a
    // non-pad matching residual (function is 2%-matched for 340 unrelated
    // structural reasons). A future virtual-rename wave (symbols
    // rename-all incl. the hand vtables) collapses it to `bctrl`.
    if (cf::CfGameManager::getInstance()->getActiveCameraObject()) {
        cf::CfGameManager::getInstance()->getActiveCameraObject()->CfObject_syncEnableState();
    }

    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    for (int i = 0; i < 3; ++i) {
        if (gmInstance.unk94[i] != 0) {
            // clearPlayerEffect is an Fv-hidden-arg helper whose `this` is the
            // player object (calls gmFileObject(this)) - dispatch via cast.
            reinterpret_cast<cf::CfGameManager*>(gmInstance.unk94[i])
                ->clearPlayerEffect();
            gmInstance.unk94[i] = 0;
        }
    }

    // Drop rows no longer present in the input list.
    for (int i = 0; i < 9; ++i) {
        s16 x = match[i];
        if (x != 0) {
            bool listed = false;
            if (x > 0) {
                for (int c = 0; c < 9; ++c) {
                    if (x == list[c]) {
                        listed = true;
                        break;
                    }
                }
            }
            if (!listed) {
                func_8007E9CC__Q22cf13CfGameManagerFv(static_cast<u16>(x), 0);
                CtrlObjectParam_ClearSlotValue(party, x);
            }
        }
    }

    // Insert rows in the input list that are not yet in the roster.
    for (int i = 0; i < 9; ++i) {
        s16 x = list[i];
        if (x != 0) {
            bool listed = false;
            if (x > 0) {
                for (int c = 0; c < 9; ++c) {
                    if (x == match[c]) {
                        listed = true;
                        break;
                    }
                }
            }
            if (!listed) {
                CItem_famFlag(x, 1);
                func_8007E9CC__Q22cf13CfGameManagerFv(static_cast<u16>(x), 1);
                if (mode) {
                    if (!CtrlObjectParam_InsertSlotValue(party, x, 1)) {
                        CtrlObjectParam_InsertSlotValue(party, x, 2);
                    }
                } else if (i < 3) {
                    CtrlObjectParam_InsertSlotEntry(party, x, 1, i);
                } else {
                    CtrlObjectParam_InsertSlotEntry(party, x, 2, i - 3);
                }
            }
        }
    }

    // Average the item counts of rows 1..8 (skip 3); leader = rounded avg + K.
    float total = lbl_eu_80666498;
    int count = 0;
    for (int i = 1; i <= 8; ++i) {
        if (i != 3 && cf::CfGameManager::isResourceFlagSet(i)) {
            u16 c = static_cast<u16>(
                CtrlObjectParam_GetArtsDataWord(CtrlObjectParam_GetCharEntry(static_cast<u16>(i))));
            ++count;
            total += U16ToF32(c);
        }
    }
    int leader = 0;
    if (count != 0) {
        float countF = S16ToF32(static_cast<s16>(count));
        float avg = total / countF;
        double k =
            (avg > lbl_eu_80666498) ? 0.5 : -0.5;
        leader = static_cast<int>(static_cast<double>(
            total / S16ToF32(static_cast<s16>(count))) + k);
    }

    u32 itembase = CtrlRemote_TouchBitByArg(0x10a);
    s32 seed = static_cast<s32>(countCollepedia());

    for (int i = 0; i < 9; ++i) {
        s16 x = list[i];
        if (x != 0 &&
            setResourceFlag__Q22cf13CfGameManagerFv(static_cast<u16>(x), 1)) {
            UnkClass_8009EC9C* obj =
                reinterpret_cast<UnkClass_8009EC9C*>(
                    CtrlObjectParam_GetCharEntry(static_cast<u16>(x)));
            if (leader != 0 && x != 0x0b && CtrlRemote_TouchBitByArg(0x3508) == 0) {
                CtrlObjectParam_RampEntryArtsLevel(obj, static_cast<u16>(leader));
                CtrlObjectParam_RefreshCharEntry(obj, 0);
                CtrlObjectParam_ApplyArtsRankUp(obj, 0, lbl_eu_805276F0[static_cast<u16>(x)], 0);
                if (x >= 3 && x <= 8) {
                    u16 c = static_cast<u16>(CtrlObjectParam_GetArtsDataWord(obj));
                    CtrlObjectParam_WriteArtsCounter(obj,
                                  static_cast<u16>(c + seed + itembase - 1));
                }
            } else {
                u16 c = static_cast<u16>(CtrlObjectParam_GetArtsDataWord(obj));
                if (x == 3) {
                    UnkClass_8009EC9C* obj8 =
                        reinterpret_cast<UnkClass_8009EC9C*>(CtrlObjectParam_GetCharEntry(8));
                    u16 c8 = static_cast<u16>(CtrlObjectParam_GetArtsDataWord(obj8));
                    if (c < c8) {
                        c = c8;
                    }
                }
                if (x >= 3 && x <= 8) {
                    CtrlObjectParam_WriteArtsCounter(obj,
                                  static_cast<u16>(itembase + seed + c - 1));
                }
            }
            if (x == 8) {
                UnkClass_8009EC9C* obj3 =
                    reinterpret_cast<UnkClass_8009EC9C*>(CtrlObjectParam_GetCharEntry(3));
                reinterpret_cast<CPcKizunagramBig*>(&obj->itemCounts_0x3534)->SetActiveSlot(*reinterpret_cast<u32*>( reinterpret_cast<u8*>(obj3) + 0x3DD0));
                // Slot owner: CActorParam is embedded at +0x17C (vtable) -
                // CtrlObjectParam.hpp; 0x200 is getSecondCurrency (() -> u32),
                // 0x1F8 is setSecondCurrency (u32). Both called via the
                // inline behavior aliases (same arity; MWCC inlines them
                // into the identical virtual dispatch when auto-inlining is
                // enabled - cf. resetBattleGauge's inlined syncEnableState).
                // NOTE (W58): same no-auto-inline caveat as the 0x5C site
                // above - `bl` to the aliases where retail has `bctrl`;
                // recorded non-pad residual, collapses on virtual-rename.
                cf::CActorParam* sub3 = reinterpret_cast<cf::CActorParam*>(
                    reinterpret_cast<u8*>(obj3) + 0x17C);
                u32 r = sub3->CActorParam_getSecondCurrency();
                cf::CActorParam* sub2 = reinterpret_cast<cf::CActorParam*>(
                    reinterpret_cast<u8*>(obj) + 0x17C);
                sub2->CActorParam_setSecondCurrency(r);
            }
        }
    }
    if (mode) {
        CtrlObjectParam_CompactSlotArrays(reinterpret_cast<u8*>(party));
    }
}
void cf::CfGameManager::func_8007F9C4() {}
void cf::CfGameManager::func_80080888() {}
void cf::CfGameManager::func_8008413C(u16, u32) {}
void cf::CfGameManager::func_80084654() {}
// cf::CfGameManager::resetBattlePresentation - in-battle presentation reset: on the
// E24 bit-16 "title/battle setup" flag, resets the battle manager
// (party-gauge + 0x94 payload), clears the presentation masks, reloads the
// bdat event table, then either kicks the event-video UI or re-populates the
// file-event table through CtrlObjectParam_GetSlotTableBase / CtrlRemote_GetFileEventIds.
void cf::CfGameManager::resetBattlePresentation() {
    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    if ((gmGameFlags & 0x10000) == 0) {
        return;
    }
    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    cf::CfGameManager* manager = &gmInstance;
    // Only the last getInstance result is kept in a saved register (live
    // across the memset); the first two are consumed in scratch.
    if (getInstance__Q22cf14CBattleManagerFv() != nullptr) {
        cf::PartyGaugeSetClamped(reinterpret_cast<cf::UnkClass_8018C5FC*>((u8*)&((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv())->unk194), 0);
        cf::CBattleManager* battle = (cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv();
        memset(&battle->unk94, 0, 0x100);
        battle->setPartyMaskFlag(2, 0);
    }
    MenuStateInitFlags();
    manager->func_8007D84C();
    gmGameFlags &= ~0x400000;
    gmPauseFlags &= ~0x100000;
    gmEventQueue.field_0x48 = 0;
    gmEventQueue.field_0x44 = 0;
    lbl_eu_80663ED8 = lbl_eu_80666498;
    lbl_eu_80663EDC = lbl_eu_80666498;
    mineResetAll();
    evtTeardownActive();
    resetHelpManager(lbl_eu_80664A10);
    CCharVoiceMan_ClearFieldFlag();
    if (NandQueueBusy() != 0) {
        if (!gmInitFlag) {
            __ct__Q22cf13CfGameManagerFv(&gmInstance);
            __register_global_object(&gmInstance,
                                     __dt__Q22cf13CfGameManagerFv,
                                     gmStaticRegistration);
            gmInitFlag = 1;
        }
        if (gmInstance.unkAC != nullptr) {
            CfRes_pushRec(reinterpret_cast<u32>(gmInstance.unkAC), 0x1E, 0, 0, 0, 0);
        }
    } else {
        for (s32 i = 1; i <= 13; ++i) {
            CtrlObjectParam_ActivateCharRow(CtrlObjectParam_GetCharEntry(static_cast<u16>(i)));
        }
        __dt__8023E448();
        CItem_rerankAllKinds();
        if (gCfNandRestoredEventFlag != 0) {
            // Retail materializes the resource-loaded test as a bool
            // (neg/or/rlwinm booleanize) before the short-circuit &&, so the
            // != result is captured in a local first.
            bool hasVisionPack = CtrlRemote_TouchBitByArg(0x3508) != 0;
            if (hasVisionPack && getQueuedFileEventCount__Q22cf13CfGameManagerFv() == 0) {
                u16 buffer[9];
                memset(buffer, 0, sizeof(buffer));
                buffer[0] = 4;
                buffer[1] = 9;
                buffer[2] = 10;
                func_8007F1FC__Q22cf13CfGameManagerFv(buffer, 1);
            }
        }
        UnkC1B4Data* video = reinterpret_cast<UnkC1B4Data*>(NandPartySnap());
        gmInitArg = (u16)CtrlObjectParam_GetSlotTableBase()[1];
        CtrlObjectParam_MoveValueToHead(CtrlObjectParam_GetSlotTableBase(), gmInitArg);
        CfFileEventIdsView* ids = CtrlRemote_GetFileEventIds();
        queueSceneEventA__Q22cf13CfGameManagerFv(
            ids->field_0x2, ids->field_0x0, reinterpret_cast<u32>(video),
            reinterpret_cast<u32>(ids->field_0x4), video->value_0xC);
        u16 eventDone = gCfNandRestoredEventFlag;
        gmPauseFlags =
            (eventDone != 0) ? (gmPauseFlags | 0x8000)
                             : (gmPauseFlags & ~0x8000);
        gCfNandRestoredEventFlag = 0;
    }
    gmGameFlags |= 0x100000;
    gmPauseFlags |= 0x20;
}
// cf::CfGameManager::func_800853C8 - title/menu-reset hook: on the
// 0x40000 flag it resets volume/fade state and the bdat text area, then
// walks the item-list refreshing every object; the 0x40000-clear path
// just re-runs the list walk after an explicit event-tear-down.
extern "C" void func_800853C8__Q22cf13CfGameManagerFv() {
    UnkArea70CE0* area = reinterpret_cast<UnkArea70CE0*>(gmResetWorkArea);
    if ((gmGameFlags & 0x40000) != 0) {
        if (!gmInitFlag) {
            __ct__Q22cf13CfGameManagerFv(&gmInstance);
            __register_global_object(&gmInstance,
                                     __dt__Q22cf13CfGameManagerFv,
                                     gmStaticRegistration);
            gmInitFlag = 1;
        }
        MenuSnd_SilenceByName_8986C(0, lbl_eu_80666498);
        MenuSnd_StopSlot0Clear_9318(1, lbl_eu_80666498);
        MenuSnd_StopSlot1_9424(lbl_eu_80666498);
        CfSoundMan_StopMaskedSlots(0x20, 0xf);
        UIWin_SweepQueue(0);
        CfScript_SleepReq2();
        if (gmInstance.field_0x88 != -1) {
            UnkFloat4 value;
            value.field_0x0 = lbl_eu_80666498;
            value.field_0x4 = lbl_eu_80666498;
            value.field_0x8 = lbl_eu_80666498;
            value.field_0xC = lbl_eu_8066649C;
            func_8008566C__Q22cf13CfGameManagerFv(0xf, &value, 1);
            gmGameFlags |= 0x100;
        }
        if (!gmInitFlag) {
            __ct__Q22cf13CfGameManagerFv(&gmInstance);
            __register_global_object(&gmInstance,
                                     __dt__Q22cf13CfGameManagerFv,
                                     gmStaticRegistration);
            gmInitFlag = 1;
        }
        if (gmInstance.unkAC != nullptr) {
            CfRes_pushRec(reinterpret_cast<u32>(gmInstance.unkAC), 6, 2, 0, 0, 0);
        }
        if (!gmInitFlag) {
            __ct__Q22cf13CfGameManagerFv(&gmInstance);
            __register_global_object(&gmInstance,
                                     __dt__Q22cf13CfGameManagerFv,
                                     gmStaticRegistration);
            gmInitFlag = 1;
        }
        if (gmInstance.unkAC != nullptr) {
            CfRes_pushRec(reinterpret_cast<u32>(gmInstance.unkAC), 0x12, 0, 0, 0, 0);
        }
        Scn_SetPalFixFlag((u8*)gmScene, 0);
    } else if ((gmGameFlags & 0x2000000) != 0 ||
               (gmGameFlags & 0x400) != 0) {
        evtUpdateSequenceKick();
    }

    gmPauseFlags |= 0x100000;
    gEventWindowFlags &= ~0x8000;
    area->entryA.field_0x0[0] = 0;
    area->entryA.field_0x20 = 0;
    area->entryA.field_0x24[0] = 0;
    area->entryA.field_0x44 = 0;
    area->entryA.field_0x50 = 0;
    area->entryA.field_0x70 = 0;
    area->entryA.field_0x74 = 0;
    area->entryA.field_0x94 = 0;
    area->entryB.field_0x0[0] = 0;
    area->entryB.field_0x20 = 0;
    area->entryB.field_0x24[0] = 0;
    area->entryB.field_0x44 = 0;
    MenuSnd_SetBalancePush_96A8(1, lbl_eu_8066649C, lbl_eu_80666498);
    ItemListManager* list = getListB28__Fv();
    ItemListNode* node = list->sentinel->next;
    while (node != list->sentinel) {
        Unk80EE4Data* object = static_cast<Unk80EE4Data*>(
            getCfObjectPc__FPQ22cf12CfObjectMove(node->object));
        if (object != nullptr) {
            setGimmick(object, true);
            refreshGimmickObject(object);
        }
        node = node->next;
    }
    gmPauseFlags &= ~8;
}
// cf::CfGameManager::func_80085978 - mode-dependent global reset. With the
// argument set it runs the "enter" path: pad/camera/event teardown, per-slot
// resource resets, item-list virtual sweep and presentation reload; with it
// clear it runs the lighter reset path (pad-task fade, pad flag clears,
// conditional manager notifications). Both paths share the singleton guard
// blocks and end in the field_0x80000-conditional manager notification.
// Retail Fv symbol taking the mode flag directly in r3 (retail asm does
// `mr r31, r3` then tests it; script plugin calls it with a single bool).
extern "C" void func_80085978__Q22cf13CfGameManagerFv(int param) {
    // Retail reads E24 here for the gate bit only (not kept live across the
    // guard); the update below re-reads it.
    bool gateFunc10 = (gmGameFlags & 0x400000) != 0;
    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    // Second E24 read feeds the update; masked form computed unconditionally,
    // overridden on the set path.
    u32 e24Now = gmGameFlags;
    u32 e24Next = e24Now & ~0x00400000;
    if (param != 0) {
        e24Next = e24Now | 0x00400000;
    }
    gmGameFlags = e24Next;
    if (param != 0) {
        // Enter path.
        if (gateFunc10) {
            CfRes_callFunc_67F10(0);
        }
        setInputDisableTime__Q22cf9CfPadTaskFf(2.0f);
        CPad* pad = gmCurrentPad;
        float zero = lbl_eu_80666498;
        if (pad != nullptr) {
            *(u8*)(reinterpret_cast<u8*>(pad) + 0x58) = 0;
            *(float*)(reinterpret_cast<u8*>(pad) + 0x60) = zero;
            *(u8*)(reinterpret_cast<u8*>(pad) + 0x59) = 0;
            *(float*)(reinterpret_cast<u8*>(pad) + 0x64) = zero;
            *(float*)(reinterpret_cast<u8*>(pad) + 0x68) = zero;
            *(float*)(reinterpret_cast<u8*>(pad) + 0x6c) = zero;
        }
        UIBattleMarkFlags82();
        if (MiniMapHasGlobalData() == 0) {
            if (!gmInitFlag) {
                __ct__Q22cf13CfGameManagerFv(&gmInstance);
                __register_global_object(&gmInstance,
                                         __dt__Q22cf13CfGameManagerFv,
                                         gmStaticRegistration);
                gmInitFlag = 1;
            }
            if (gmInstance.unkAC != nullptr) {
                CfRes_pushRec(reinterpret_cast<u32>(gmInstance.unkAC), 6, 4,
                              0, 0, 0);
            }
            if (!gmInitFlag) {
                __ct__Q22cf13CfGameManagerFv(&gmInstance);
                __register_global_object(&gmInstance,
                                         __dt__Q22cf13CfGameManagerFv,
                                         gmStaticRegistration);
                gmInitFlag = 1;
            }
            if (gmInstance.unkAC != nullptr) {
                CfRes_pushRec(reinterpret_cast<u32>(gmInstance.unkAC), 0x20,
                              0, 0, 0, 0);
            }
        }
        bindPadSubobjects(1);
        if ((gmPauseFlags & 0x08000000) != 0) {
            if (!gmInitFlag) {
                __ct__Q22cf13CfGameManagerFv(&gmInstance);
                __register_global_object(&gmInstance,
                                         __dt__Q22cf13CfGameManagerFv,
                                         gmStaticRegistration);
                gmInitFlag = 1;
            }
            if (gmInstance.unkAC != nullptr) {
                CfRes_pushRec(reinterpret_cast<u32>(gmInstance.unkAC), 0x1f,
                              0, 0, 0, 0);
            }
            gmPauseFlags &= ~0x08000000;
        }
    } else {
        // Reset path.
        if (MiniMapHasGlobalData() != 0) {
            MiniMapResetLayoutAnims();
        }
        UIBattleRemoveAll(0);
        gmBaseTimeScale = lbl_eu_80666498;
        if (gateFunc10) {
            CfRes_callFunc_67F10(0);
        }
        bindPadSubobjects(0);
        if (!gmInitFlag) {
            __ct__Q22cf13CfGameManagerFv(&gmInstance);
            __register_global_object(&gmInstance,
                                     __dt__Q22cf13CfGameManagerFv,
                                     gmStaticRegistration);
            gmInitFlag = 1;
        }
        if (gmInstance.unkAC != nullptr) {
            CfRes_markKey(reinterpret_cast<UnkClass_80085334*>(
                              gmInstance.unkAC),
                          0x20);
        }
        if (!gmInitFlag) {
            __ct__Q22cf13CfGameManagerFv(&gmInstance);
            __register_global_object(&gmInstance,
                                     __dt__Q22cf13CfGameManagerFv,
                                     gmStaticRegistration);
            gmInitFlag = 1;
        }
        if (gmInstance.unkAC != nullptr) {
            CfRes_markKey(reinterpret_cast<UnkClass_80085334*>(
                              gmInstance.unkAC),
                          0x15);
        }
        if ((gmGameFlags & 0x200) != 0) {
            gmGameFlags &= ~0x200;
        }
        if ((gmPauseFlags & 0x800) != 0) {
            gmPauseFlags &= ~0x800;
        }
        for (s32 i = 1; i <= 13; ++i) {
            CtrlObjectParam_ActivateCharRow(CtrlObjectParam_GetCharEntry(static_cast<u16>(i)));
        }

        // Item-list sweep through getEffOwner's flag view. Retail table
        // lbl_eu_80528600 (cf::CfObjectColl) has copyCollPosition at +0xB8;
        // call through the Coll tree (macro-included above) so MWCC emits a
        // no-arg bctrl (main-tree applyMoveOffset(vec,float) would load r4/f1).
        Func800B6BECList* list = (Func800B6BECList*)getReslistB48();
        Func800B6BECNode* node = list->head->next;
        while (node != list->head) {
            cf::CfObjectColl* obj =
                static_cast<cf::CfObjectColl*>(getEffOwner(node->object));
            if (obj != nullptr) {
                obj->copyCollPosition();
            }
            node = node->next;
        }
        lbl_eu_80663ED8 = lbl_eu_806669C8;
        CfSoundMan_StopSlotByMode(0, 0x1bb, 0xf);
    }

    // Shared tail: on E24 bit 0x80000 notify the manager, with different
    // call shapes depending on the mode argument.
    if ((gmGameFlags & 0x80000) != 0) {
        if (!gmInitFlag) {
            __ct__Q22cf13CfGameManagerFv(&gmInstance);
            __register_global_object(&gmInstance,
                                     __dt__Q22cf13CfGameManagerFv,
                                     gmStaticRegistration);
            gmInitFlag = 1;
        }
        if (param != 0) {
            if (gmInstance.unkAC != nullptr) {
                CfRes_markKey(reinterpret_cast<UnkClass_80085334*>(
                                  gmInstance.unkAC),
                              0x1c);
            }
        } else {
            if (gmInstance.unkAC != nullptr) {
                CfRes_pushRec(reinterpret_cast<u32>(gmInstance.unkAC), 0x1c,
                              0x28, 0, 0, 0);
            }
        }
    }
}
// cf::CfGameManager::func_80085FB8 - battle/scene reset hook. Masks the
// enabled-input flags into every pad flagset, resets the camera/event
// managers, refreshes the party slot objects' 0x3E9C containers, then walks
// the item list refreshing each object before clearing the presentation bits.
void cf::CfGameManager::func_80085FB8() {
    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    cf::CfGameManager* mgr = &gmInstance;

    // 0xFFA00000 - 0x231: keep-everything-except mask applied to all pads.
    u32 mask = gmEnabledPadFlags & 0xFF9FFDCF;
    gmEnabledPadFlags = mask;
    CPad* pad = gmCurrentPad;
    if (pad != nullptr) {
        pad->mPressedButtonFlags &= mask;
        pad->mTurboPressButtonFlags &= mask;
        pad->mReleasedButtonFlags &= mask;
        pad->mHeldButtonFlags &= mask;
        pad->mLongHoldButtonFlags &= mask;
        pad->mShortPressButtonFlags &= mask;
        gmPadData.mPressedButtonFlags &= mask;
        gmPadData.mTurboPressButtonFlags &= mask;
    }
    CfRes_callFunc_67FE0();
    CamEvtClearSlotBytes(mgr->unkB4);
    CamEvtSearchStateVal(0);
    func_801AAD08();
    gSoundSystemInitFlag = 0;
    getInstance__Fv();
    walkReslistByMask(nullptr, 0x10000);
    if (!gmInitFlag) {
        __ct__Q22cf13CfGameManagerFv(&gmInstance);
        __register_global_object(&gmInstance, __dt__Q22cf13CfGameManagerFv,
                                 gmStaticRegistration);
        gmInitFlag = 1;
    }
    gmGameFlags &= 0xFFFB7FFF;

    // Retail dispatches vtable slot 0x158 (CfObjectMove::setPointEnabled,
    // u32 arg) on each party slot object whose +0x64 word has bit 1 set.
    for (int i = 0; i < 3; ++i) {
        cf::CfObjectMove* slotObj = mgr->unk94[i];
        if (slotObj == nullptr || (slotObj->unk64 & 2) == 0) {
            continue;
        }
        slotObj->setPointEnabled(1);
    }

    getInstance__Fv();
    setTrackedTarget(lbl_eu_80666498, 0, 0);
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (!lookupWorkAtAddr(this)) {
        player = nullptr;
    }
    if ((gmPauseFlags & 0x20000000) == 0) {
        assignCameraTarget__Q22cf13CfGameManagerFv(reinterpret_cast<u32>(player));
    }
    CfObject* cam = this->getActiveCameraObject();
    if (cam != nullptr && player != nullptr) {
        // Slot-0x40 probe on the object at player+0x38 (mSubObj38): the
        // field player's sub-object answers the 0x400 flag query through
        // the same slot (CtrlPlayerSub3ED4::vf14, int(int)) that CtrlPc.cpp
        // uses for its mask-3 camera resets; on failure reset the
        // camera-event manager mode.
        CtrlPlayerSub3ED4* sub =
            reinterpret_cast<CtrlPlayerSub3ED4*>(*reinterpret_cast<void**>(reinterpret_cast<u8*>(player) + 0x38));
        if (sub->vf14(0x400) == 0) {
            cfCam_setClear04(cam, 3, 1);
        }
    }

    UnkClass_80083298* unk90 = cf::CfGameManager::getGameSubManager();
    if (unk90 != nullptr) {
        if (cf::CfGameManager::getGameSubManager() != nullptr) {
            flushMpfBuffer__17UnkClass_8047BB54Fv(
                &cf::CfGameManager::getGameSubManager()->field_0xF0);
        }
    }
    loadLayoutScales();
    ArtsSelect_NotifyAll(reinterpret_cast<void*>(1));
    if ((gmPauseFlags & 0x20000000) == 0) {
        ItemListManager* list = getListB28__Fv();
        ItemListNode* node = list->sentinel->next;
        while (node != list->sentinel) {
            Unk80EE4Data* object = static_cast<Unk80EE4Data*>(
                getCfObjectPc__FPQ22cf12CfObjectMove(node->object));
            if (object != nullptr) {
                setGimmick(object, true);
                refreshGimmickObject(object);
            }
            node = node->next;
        }
        // rlwinm mb=29,me=27 wrap mask: everything except bit 28.
        gmPauseFlags &= ~0x10000000;
    }
    // rlwinm mb=3,me=1 wrap mask: everything except bit 2.
    gmPauseFlags &= ~0x20000000;
}
// cf::CfGameManager::func_80086778 - one-time bdat table scan (guarded by the
// E35 flag). Loads the current bdat table (lbl_eu_806640A0), walks a row range,
// tallies rows by the FD column byte, and for rows whose B3-column byte matches
// lbl_eu_80664184 spawns collision objects via createBdatCollisionObj and positions them
// with the func_800AC1xx collision helpers.
void cf::CfGameManager::func_80086778() {
    if (gmEventFlag35 != 0) {
        return;
    }
    Bdat_GetTable_AA34();
    void* bdat = lbl_eu_806640A0;
    int rowStart = (int)Bdat_GetRowBase_B41C(bdat);
    int rowCount = (int)Bdat_GetMaxRow_B1EC(bdat);
    gmEventFlag35 = 1;
    gmCounter36 = 0;
    gmCounter38 = 0;
    const char* base = lbl_eu_804FB824;
    u16 targetCode = (u16)lbl_eu_80664184;
    // 0x43300000-prefixed conversion blocks and the magic doubles are hoisted
    // out of the row loop like retail's stack slots 0x58/0x60 and preloaded
    // f28/f29 constants.
    union { double d; u32 w[2]; } cvtA, cvtB;
    cvtA.w[0] = 0x43300000;
    cvtB.w[0] = 0x43300000;
    const double s16magic = *reinterpret_cast<const double*>(&lbl_eu_80666530);
    const double u16magic = lbl_eu_80666528;
    const float threshold = ml::epsilon;
    const float scale = ml::deg2rad;

    for (int i = 0; i < rowCount; ++i) {
        int row = rowStart + i;
        // Each bdat string-column result rides through a u32 local so MWCC
        // spills it to its own stack slot and re-extracts the byte/halfword
        // afterwards, matching retail's 0x8..0x2c spill pattern.
        u32 rawB3 = getBdatStringColumnValue(bdat, base + 0xb3, row);
        u32 rawFD = getBdatStringColumnValue(bdat, base + 0xfd, row);
        u8 code = (u8)rawB3;
        u8 v = (u8)rawFD;
        if (v == 0) {
            ++gmCounter36;
        } else if (v == 1) {
            ++gmCounter38;
        }
        if (code != targetCode) {
            continue;
        }
        ml::CVec3 v48;
        u32 raw106 = getBdatStringColumnValue(bdat, base + 0x106, row);
        cvtA.w[1] = (u32)(s16)raw106 ^ 0x80000000;
        v48.x = (float)(cvtA.d - s16magic);
        u32 raw10b = getBdatStringColumnValue(bdat, base + 0x10b, row);
        cvtB.w[1] = (u32)(s16)raw10b ^ 0x80000000;
        v48.y = (float)(cvtB.d - s16magic);
        u32 raw110 = getBdatStringColumnValue(bdat, base + 0x110, row);
        cvtA.w[1] = (u32)(s16)raw110 ^ 0x80000000;
        v48.z = (float)(cvtA.d - s16magic);
        u32 raw115 = getBdatStringColumnValue(bdat, base + 0x115, row);
        cvtB.w[1] = (u32)(u16)raw115;
        float fD = (float)(cvtB.d - u16magic);
        u32 raw11b = getBdatStringColumnValue(bdat, base + 0x11b, row);
        cvtA.w[1] = (u32)(u16)raw11b;
        float fE = (float)(cvtA.d - u16magic);

        // Static member call: no this pointer is materialized, so r3 still holds
        // the previous getBdatStringColumnValue result when the bl issues -
        // the register inheritance retail relies on. The tail-call inside
        // createBdatCollisionObj leaks the bdat object back through r3.
        // gmSpawnMask8000's object is a collision object (CollObjSetupVecsState4/AC1BC
        // take cf::CfObjectColl*): dispatch the real slot-0x158 virtual
        // (CfObject::setPointEnabled) through the CfObject base, clear bit 16
        // of the +0x64 word (CfObject::unk64), and set the +0x158 flag
        // halfword (CfObjectColl::field_0x158) per the row kind.
        cf::CfObject* obj = reinterpret_cast<cf::CfObject*>(cf::CfGameManager::createBdatCollisionObj());
        if (obj == nullptr) {
            continue;
        }
        if (fD > threshold) {
            ml::CVec3 v3c(v48.x, v48.y + fE, v48.z);
            CollObjSetupVecsState4(obj, &v48, &v3c, fD);
        } else {
            u32 raw121 = getBdatStringColumnValue(bdat, base + 0x121, row);
            cvtB.w[1] = (u32)(u16)raw121;
            float fD2 = (float)(cvtB.d - u16magic);
            u32 raw127 = getBdatStringColumnValue(bdat, base + 0x127, row);
            cvtA.w[1] = (u32)(u16)raw127;
            float fE2 = (float)(cvtA.d - u16magic);
            u32 raw12d = getBdatStringColumnValue(bdat, base + 0x12d, row);
            ml::CVec3 v30(fD, fE, fE2);
            cvtB.w[1] = (u32)(u16)raw12d;
            float radius = (float)(cvtB.d - u16magic) * scale;
            CollObjSetupRotBoxState4Timed((cf::CfObjectColl*)obj, &v48, &v30, radius);
        }
        ::CollObjPackResIdWords((cf::CfObjectColl*)obj, (u32)row, code);
        obj->setPointEnabled(0);
        obj->unk64 &= ~0x10000;
        u16& pointFlag = *reinterpret_cast<u16*>(reinterpret_cast<u8*>(obj) + 0x158);
        if (v == 0) {
            pointFlag |= 0x100;
        } else if (v == 1) {
            pointFlag |= 0x200;
        }
    }
}

// func_80080F48 - resource-slot query. Fills the caller's CfResStackObject
// (Unk812Data layout) either directly from the bdat row getters (mode != 0)
// or through the per-index level lookup table, then resolves the slot ids.
extern "C" void func_80080F48__Q22cf13CfGameManagerFv(
    u32 value, CfResStackObject* resource, u32 mode, u32 flag) {
    UnkClass_8009EC9C* table =
        reinterpret_cast<UnkClass_8009EC9C*>(CtrlObjectParam_GetCharEntry(value));
    Unk812Data* data = reinterpret_cast<Unk812Data*>(resource);
    clearResourceStack__Q22cf13CfGameManagerFv(reinterpret_cast<UnkReset28Data*>(table));
    if (mode != 0) {
        setResourceField04__Q22cf13CfGameManagerFv(data, func_8009E120(table, 0));
        setResourceField08__Q22cf13CfGameManagerFv(data, func_8009E120(table, 1));
        setResourceField10__Q22cf13CfGameManagerFv(data, func_8009E120(table, 2));
        setResourceField0C__Q22cf13CfGameManagerFv(data, func_8009E120(table, 3));
        setResourceField14__Q22cf13CfGameManagerFv(data, func_8009E120(table, 4));
    } else {
        setResourceField04__Q22cf13CfGameManagerFv(
            data, bdatPcPartsRes(value, (s16)getResourceField0E__Q22cf13CfGameManagerFv(
                                       reinterpret_cast<Unk812Data*>(table)),
                                 1, 0));
        setResourceField08__Q22cf13CfGameManagerFv(
            data,
            bdatPcPartsRes(value, (s16)getResourceWord10__Q22cf13CfGameManagerFv(reinterpret_cast<const u8*>(table)),
                          2, 0));
        setResourceField10__Q22cf13CfGameManagerFv(
            data,
            bdatPcPartsRes(value, (s16)getResourceWord12__Q22cf13CfGameManagerFv(reinterpret_cast<const u8*>(table)),
                          3, 0));
        setResourceField0C__Q22cf13CfGameManagerFv(
            data,
            bdatPcPartsRes(value, (s16)getResourceWord14__Q22cf13CfGameManagerFv(reinterpret_cast<const u8*>(table)),
                          4, 0));
        setResourceField14__Q22cf13CfGameManagerFv(
            data,
            bdatPcPartsRes(value, (s16)getResourceWord16__Q22cf13CfGameManagerFv(reinterpret_cast<const u8*>(table)),
                          5, 0));
    }

    void* handle = (void*)(uintptr_t)func_8009E120(table, 5);
    u32 aFull = CItem_getNameIdFromFam(getResourceField0C_2__Q22cf13CfGameManagerFv(data));
    u32 kind = (u32)(u16)bdatMountValue((u32)(u16)aFull, 1);
    u32 b = 0;
    if (kind == 3) {
        b = aFull;
    } else if (kind == 1) {
        b = aFull;
        aFull = 0;
    }

    u32 cur = CfRes_extractBits20_7(handle);
    if (mode != 0) {
        setResourceField24__Q22cf13CfGameManagerFv(data, 1);
        setResourceField00__Q22cf13CfGameManagerFv(data, CfRes_packFourFields(8, value, 0, 0));
    } else {
        // `value` doubles as the row id here; the +1-0xC wrap test covers
        // rows 11..12.
        if ((value == 4 || (u16)(value + 1 - 0xc) <= 1) && cur == 0x2b) {
            setResourceField00__Q22cf13CfGameManagerFv(data, packThreeFieldIds__Q22cf13CfGameManagerFv(value, cur, 0x14));
        } else {
            setResourceField00__Q22cf13CfGameManagerFv(data, packThreeFieldIds__Q22cf13CfGameManagerFv(value, 0, 0x14));
        }
    }
    setResourceField18__Q22cf13CfGameManagerFv(data, (u32)handle);
    setResourceField20__Q22cf13CfGameManagerFv(data, (u16)aFull);
    setResourceField22__Q22cf13CfGameManagerFv(data, (u16)b);
    if (flag != 0) {
        setResourceField1C__Q22cf13CfGameManagerFv(data, (u16)CfObjectMove_lookupBdatCol7((u16)aFull, 0));
        setResourceField1E__Q22cf13CfGameManagerFv(data, (u16)CfObjectMove_lookupBdatCol7((u16)b, 1));
    } else {
        setResourceField1C__Q22cf13CfGameManagerFv(data, (u16)CfObjectMove_lookupBdatCol11((u16)aFull, 0));
        setResourceField1E__Q22cf13CfGameManagerFv(data, (u16)CfObjectMove_lookupBdatCol11((u16)b, 1));
    }
}

// func_800838F4 - battle-start presentation driver. mode selects which screen
// region gets the tint/color refresh (0 = none, 1/2 = the two sub-screens);
// `value` is the fade factor stored back into the manager.
extern "C" void func_800838F4__Q22cf13CfGameManagerFv(u32 mode, u32 first,
                                                        u32 second, u32 third,
                                                        u32 fourth, float value) {
    cf::CfGameManager* mgr = cf::CfGameManager::getInstance();
    if (isGlobalCamFlagSet__Fi(4)) {
        return;
    }
    func_80084CA4__Q22cf13CfGameManagerFv(first, second, 1, false);
    if (getInstance__Q22cf14CBattleManagerFv() != nullptr) {
        if (((cf::CBattleManager*)nullptr)->CBattleMan_FetchVisionObj() != nullptr) {
            // Slot owner: cf::CVision owns 0x20 (vt_20(u32)) - CVision.hpp;
            // battle+0x219C is CBattleManager::mVision (CBattleManager.hpp).
            cf::CVision* sub = reinterpret_cast<cf::CVision*>(
                getDataArray219C__Q22cf13CfGameManagerFv(
                    reinterpret_cast<u8*>(getInstance__Q22cf14CBattleManagerFv())));
            sub->vt_20(1);
            CfRes_getE14();
            Scn_SetTimeScale(reinterpret_cast<CScn*>(CfRes_getE14()), lbl_eu_8066649C);
        }
    }
    cf::CfGameManager::enablePadFlags(0x1FE01EFF, false);

    if (mode == 0) {
        UnkFloat4 bufA;
        UnkFloat4 cand;
        UnkFloat4 hit;
        UnkFloat4 miss;
        CfRes_getE14();
        cfCam_copyBlock16(&bufA, Scn_QueryUnk80State(reinterpret_cast<CScn*>(CfRes_getE14())));
        CTaskGame_setVec4(reinterpret_cast<Unk407C8Color*>(&cand), lbl_eu_8066649C,
                      lbl_eu_8066649C, lbl_eu_8066649C, lbl_eu_8066649C);
        if (compareFloatVectors__Q22cf13CfGameManagerFv(&bufA, &cand)) {
            CTaskGame_setVec4(reinterpret_cast<Unk407C8Color*>(&hit), lbl_eu_8066649C,
                          lbl_eu_8066649C, lbl_eu_8066649C, lbl_eu_8066649C);
            func_8008566C__Q22cf13CfGameManagerFv(0x1e, &hit, 1);
        } else {
            CTaskGame_setVec4(reinterpret_cast<Unk407C8Color*>(&miss), lbl_eu_80666498,
                          lbl_eu_80666498, lbl_eu_80666498, lbl_eu_8066649C);
            func_8008566C__Q22cf13CfGameManagerFv(0, &miss, 1);
        }
        CfSoundMan_StopAllModes(0);
    } else if (mode == 1) {
        UnkFloat4 bufA;
        UnkFloat4 cand;
        UnkFloat4 hit;
        UnkFloat4 miss;
        CfRes_getE14();
        cfCam_copyBlock16(&bufA, Scn_QueryUnk80State(reinterpret_cast<CScn*>(CfRes_getE14())));
        CTaskGame_setVec4(reinterpret_cast<Unk407C8Color*>(&cand), lbl_eu_8066649C,
                      lbl_eu_8066649C, lbl_eu_8066649C, lbl_eu_8066649C);
        if (compareFloatVectors__Q22cf13CfGameManagerFv(&bufA, &cand)) {
            CTaskGame_setVec4(reinterpret_cast<Unk407C8Color*>(&hit), lbl_eu_8066649C,
                          lbl_eu_8066649C, lbl_eu_8066649C, lbl_eu_8066649C);
            func_8008566C__Q22cf13CfGameManagerFv(0x1e, &hit, 1);
        } else {
            CTaskGame_setVec4(reinterpret_cast<Unk407C8Color*>(&miss), lbl_eu_80666498,
                          lbl_eu_80666498, lbl_eu_80666498, lbl_eu_8066649C);
            func_8008566C__Q22cf13CfGameManagerFv(0x1e, &miss, 1);
        }
        CfSoundMan_StopAllModes(0x1c);
        queueCameraRequest__Q22cf13CfGameManagerFv(0x18, 0, 0, 0, 0);
    } else if (mode == 2) {
        UnkFloat4 tint;
        queueCameraRequest__Q22cf13CfGameManagerFv(6, 0x23, 0, 0, 0);
        queueCameraRequest__Q22cf13CfGameManagerFv(0x21, 0x1c, 0, 0, 0);
        CTaskGame_setVec4(reinterpret_cast<Unk407C8Color*>(&tint), lbl_eu_80666498,
                      lbl_eu_80666498, lbl_eu_80666498, lbl_eu_8066649C);
        func_8008566C__Q22cf13CfGameManagerFv(0x1e, &tint, 0);
        queueCameraRequest__Q22cf13CfGameManagerFv(0x18, 0, 0, 0, 0);
    }

    disableFlag20__Q22cf13CfGameManagerFv();
    gmQueuedEventCount = 0;
    CfRes_callFunc_67E78(0);
    CfRes_callFunc_67FE0();
    enableGameFlag__Q22cf13CfGameManagerFv(4);
    disableGameFlag__Q22cf13CfGameManagerFv(0x80);
    queueCameraRequest__Q22cf13CfGameManagerFv(0xb, first, second, 0, 0);
    copyVec3Words(reinterpret_cast<ml::CVec3*>(&mgr->field_0xC),
                  reinterpret_cast<const ml::CVec3*>(first));

    // Fade-factor bookkeeping: zero the halfword mirrors, compare the incoming
    // factor against the shared threshold and stash it in the manager.
    float threshold = 360.0f;
    gmFlagPairs48[0] = 0;
    gmFlagPairs4C[0] = 0;
    struct Mgr24View {
        u8 pad[8];
        float value24;
    };
    bool hitSlot = false;
    if (threshold == value) {
        cf::CfObjectMove** slot = getPlayerSlotPtr__Q22cf13CfGameManagerFv(mgr->unk94, 0);
        if (*slot != nullptr) {
            hitSlot = true;
        }
    }
    float out = value;
    if (hitSlot) {
        cf::CfObjectMove** slot = getPlayerSlotPtr__Q22cf13CfGameManagerFv(mgr->unk94, 0);
        // Slot owner: cf::CfObject owns 0xD8 (getScaledAngle, ()->float).
        out = static_cast<cf::CfObject*>(*slot)->CfObject_getScaledAngle();
    }
    reinterpret_cast<Mgr24View*>(&mgr->field_0x1C)->value24 = out;

    CfScript_MakeName(reinterpret_cast<char*>(&mgr->unk28),
                  reinterpret_cast<const char*>(first),
                  reinterpret_cast<const char*>(second),
                  reinterpret_cast<const char*>(fourth));
    copyBdatTextEntryFull__Q22cf13CfGameManagerFv(&gmBdatTextEntries[0],
                                           &gmBdatTextEntries[1]);
    cfCam_syncFollowD();
}
