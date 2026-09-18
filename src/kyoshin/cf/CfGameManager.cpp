// The header declares the 2-arg (delete-flag) form of the UnkClass_8018EF3C
// ctor for teardownGameManager; init() calls the 1-arg placement form. Rename the
// 2-arg copy away while including the header, then declare the 1-arg form.
#define __ct__Q22cf17UnkClass_8018EF3CFv ct_UnkClass_8018EF3C_2arg
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

#include "monolib/scn/CScnTimeApi.hpp"
#undef __ct__Q22cf17UnkClass_8018EF3CFv
#include "kyoshin/cf/CfPadData.hpp"
#include "kyoshin/cf/CfPadTask.hpp"
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
// CfObjectMap.hpp's import block declares func_800AA33C as int(char*,...)
// clashing with the proper extern "C"
// versions already in scope from CfObjectMove.hpp / IResInfo.hpp
// (MWCC forbids overloading an extern "C"
// function). Rename CfObjectMap.hpp's copies out of the way; this TU uses the
// properly-typed declarations from those headers. (Scn_GetFrameDelta now has a
// single unified decl.)
#define func_800AA33C cfObjectMapFunc_800AA33C
#include "kyoshin/cf/object/CfObjectMap.hpp"
#undef func_800AA33C
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include "kyoshin/code_801862C0.hpp"
// CtrlPc player-sub type for func_80085FB8's +0x38 sub-object probe (real
// slot-0x40 virtual CtrlPlayerSub3ED4::vf14, int(int), same 0x400-probe
// idiom as CtrlPc.cpp's mask-3 resets). Include-only: owned elsewhere.
// (Func renames: this TU's cfCam_setClear04 takes s32 flag and Selector_GetInstance
// returns void*; the header's int/CfObj90E4* copies are renamed away -
// same MWCC-10505 pattern as above.)
#define cfCam_setClear04 ctrlPcFunc_8006BBF4
#define Selector_GetInstance ctrlPcFunc_800FE68C
#define appendObjectById ctrlPcFunc_800F6D50
#define func_8017FD44 ctrlPcFunc_8017FD44
#define func_8017FD4C ctrlPcFunc_8017FD4C
#define getArtsParamRC2 ctrlPcGetArtsParamRC2
#include "kyoshin/cf/CtrlPc.hpp"
#undef Selector_GetInstance
#undef cfCam_setClear04
#undef appendObjectById
#undef func_8017FD44
#undef func_8017FD4C
#undef getArtsParamRC2
// Full battle-manager class for resetBattlePresentation's real slot-0x1C call
// (setPartyMaskFlag) and +0x94/+0x194 member access. Include-only: the class
// and its TU are owned elsewhere; nothing here is redeclared.
// (CSuddenCommu.hpp's import block declares getUnk80664658 with a different
// extern "C" return type than the TU-local decl; rename its copy away -
// same MWCC-10505 pattern as the CfObjectMove/CfObjectMap renames above.)
#define getUnk80664658 cbattleGetUnk80664658
#define CItem_getNameIdFromFam cbattleFunc_80158018
#define isGlobalCamFlagSet__Fi cbattleIsGlobalCamFlagSet
#define func_8009E344 cbattleFunc_8009E344
#define Counter_TestBit cbattleFunc_8026178C
#define lookupEffectForResource__Q22cf13CfGameManagerFv cbattleLookupEffectForResource
#define __dt__Q22cf16CfObjectImplMoveFv cbattle_dt_CfObjectImplMove
#define getPlayer__Q22cf13CfGameManagerFi cbattleGetPlayer
#define IdTable_SumValues cbattleFunc_8025FB10
#define lookupWorkAtAddr cbattleFunc_800B8920
#define syncBattleState__Q22cf13CfGameManagerFv cbattleSyncBattleState
#define getReslistBE8 cbattleFunc_800B6C34
#define addTableValueWithClamp__Q22cf13CfGameManagerFv cbattleAddTableValueWithClamp
#define createBattleActor__Q22cf13CfGameManagerFv cbattleCreateBattleActor
#include "kyoshin/cf/CBattleManager.hpp"
#undef createBattleActor__Q22cf13CfGameManagerFv
#undef Counter_TestBit
#undef func_8009E344
#undef lookupEffectForResource__Q22cf13CfGameManagerFv
#undef __dt__Q22cf16CfObjectImplMoveFv
#undef getPlayer__Q22cf13CfGameManagerFi
#undef IdTable_SumValues
#undef lookupWorkAtAddr
#undef syncBattleState__Q22cf13CfGameManagerFv
#undef getReslistBE8
#undef addTableValueWithClamp__Q22cf13CfGameManagerFv
#undef isGlobalCamFlagSet__Fi
#undef CItem_getNameIdFromFam
#undef getUnk80664658

#include "monolib/core/CPadManager.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/FixStr.hpp"
#include <string.h>
#include "monolib/math/FloatUtils.hpp"  // H3 label-owner decl (lbl_eu_8066A208)

extern "C" u32 getEffectFlagFromMgr(void* mgr);
extern "C" void setGimmickActive(void* obj, bool enable);
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

extern "C" void gmFileObject(void* object);
// TU-local decl: loadBdatGroupData is a tail-call thunk (addi r3,r3,0x1c; b CtrlObjectParam_ResolveEquipItem)
// that passes r4 through, so the 1-arg form is intentional here. The retail
// 2-arg form (data + index) is declared in pluginCfs.cpp.
extern "C" void CtrlObjectParam_ResolveEquipItem(CfGameManagerData1C* data);

// --- imports for the func_8007xxxx / func_8008xxxx target bodies ---
// (retail C-linkage names; signature params are informational, the emitted
// reloc symbol is the unmangled name)
extern "C" void loadBdatFileTable__Q22cf6CfBdatFv();
extern "C" void* __ct__cf_CfTFile(void* self);
extern "C" void* __ct__80075298(void* self);
extern "C" void* __ct__cf_CfMapEffectManager(void* self);
extern "C" void* __ct__80186578(void* self);
extern "C" void* __ct__801BF76C(void* self);
extern "C" void func_80188584(void* self);
extern "C" void func_801A9CCC(void* self);
extern "C" void CfT_IsTvPal();
extern "C" void func_80069EA8();
extern "C" void clearReloadGlobals();
extern "C" void cfCam_seedFollowD();
extern "C" float getReloadDelay();
extern "C" void MenuSnd_StopSlot0Store_9390(const void* text);
extern "C" void func_8006A03C(u32 first, u32 second, u32 third);
extern "C" void dispatchLODArgs__8CTaskLODFv(u16 first, u16 second, u16 third);
extern "C" void CfScript_UpdateMgr();
extern "C" void CfSoundMan_UpdateAllRecords();
extern "C" int isFlag01Set__9CTaskGameFv();
extern "C" void CmText_DispatchCallbacks();
extern "C" void clrAnimGate();
extern "C" void func_8007BAFC(CfCamEventManager* manager);
extern "C" void MenuSnd_CanPlayGate_9450();
extern "C" void func_8018986C(int value, float first);
extern "C" void MenuSnd_StopSlot0Clear_9318(int value, float first);
extern "C" void CfSoundMan_StopMaskedSlots(u32 first, u32 second);
extern "C" void func_8013D26C(u32 value);
extern "C" void CfScript_SleepReq2();
extern "C" void evtUpdateSequenceKick();
extern "C" void MenuSnd_SetBalancePush_96A8(s32 index, float f1, float f2);
extern "C" void Scn_SetPalFixFlag(void* scene, u32 value);
// 1-arg placement ctor form used by UnkClass_8007DAE0::init.
extern "C" void* __ct__Q22cf17UnkClass_8018EF3CFv(void* self);
extern "C" void func_8007DCB8__Q22cf13CfGameManagerFv();

// --- imports for func_80086B5C ---
extern "C" unsigned long func_8006A6D0();
extern "C" void* func_8016FE34(void* source);
// Retail symbol is Fv but callers pass three values (see setPad precedent).
extern "C" void func_80086B5C__Q22cf13CfGameManagerFv(int arg1, int arg2,
                                                        int flag);
// Definition lives (renamed away) in CfGameManagerUnityHelpers.hpp; retail
// calls it out-of-line.
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);

// Circular gimmick list returned by getReslistB68 (same shape as
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
// call. CfObjectColl.hpp recovers lbl_eu_80528600 with Coll's no-arg +0xB8
// override; the main cf::CfObject.hpp tree spells that slot as
// applyMoveOffset(vec,float). Macro-rename the Coll TU's intermediate bases
// so they do not ODR-clash with the already-included main object headers.
// Include-only: CfObjectColl.cpp / .hpp owned elsewhere; nothing redefined.
#define CObjectState GmColl_CObjectState
#define CObjectParam GmColl_CObjectParam
#define CfObject GmColl_CfObject
#define CfObjectPoint GmColl_CfObjectPoint
#include "kyoshin/cf/object/CfObjectColl.hpp"
#undef CfObjectPoint
#undef CfObject
#undef CObjectParam
#undef CObjectState

// --- imports for func_80080F48 / func_80085FB8 / func_800838F4 ---
class UnkClass_8009EC9C;
extern "C" u32 func_80141E90(u32 param1, s16 param2, u32 param3, u32 param4);
extern "C" u16 CItem_getNameIdFromFam(u16 value);
extern "C" u32 func_80142154(u32 value, u32 mode);
extern "C" u32 CfRes_extractBits20_7(void* field);
extern "C" u32 CfRes_packFourFields(u32 first, u32 second, u32 third, u32 fourth);
extern "C" u32 CfObjectMove_lookupBdatCol7(u32 id, u32 flag);
extern "C" u32 CfObjectMove_lookupBdatCol11(u32 id, u32 flag);
extern "C" void CamEvtClearSlotBytes(void* manager);
extern "C" void func_8007B0A0(s32 value);
extern "C" void func_801AAD08();
extern u8 lbl_eu_8066443A;
extern "C" void cfCam_setClear04(void* manager, u32 mask, s32 flag);
extern "C" void* CBattleMan_FetchVisionObj(void* bm);
extern "C" void func_800B06C8();
extern "C" void func_800B6800(float value, u32 a, u32 b);
extern "C" void cfCam_copyBlock16(void* dst, void* src);
extern "C" void CfSoundMan_StopAllModes(s32 value);
extern "C" void disableFlag20__Q22cf13CfGameManagerFv();
extern "C" void cfCam_syncFollowD();
extern "C" void CfScript_MakeName(char* dest, const char* src1, const char* src2,
                              const char* src3);
struct Unk407C8Color {
    u32 w[4];
};
extern "C" Unk407C8Color* CTaskGame_setVec4(Unk407C8Color* self, float f1, float f2,
                                        float f3, float f4);
extern const float lbl_eu_80666560;

// data globals referenced by the target bodies (defined in other TUs)
extern u16 lbl_eu_80663E46;
extern u8 lbl_eu_80663E35;
extern u16 lbl_eu_806642E0;
extern u8 lbl_eu_80663DFC;
extern const float lbl_eu_806664A4;
extern float lbl_eu_80663E54;
extern u16 lbl_eu_80663E36;
extern u16 lbl_eu_80663E38;
extern void* lbl_eu_806640A0;
extern const double lbl_eu_80666528;
extern const float lbl_eu_8066A210;

cf::CfGameManager::~CfGameManager() {
    gmFileObject(unk90);
    unk90 = nullptr;
    if (unkA8 != nullptr) {
        BattleWork_InitCtrlWork(unkA8);
    }
    if (unkA0 != 0) {
        MapFx_ReleaseHandle(unkA0);
    }
    if (field_0xA4 != nullptr) {
        func_80186664(field_0xA4);
    }
    if (unkB4 != nullptr) {
        CamEvtReleaseAllSlots(unkB4);
    }
    unkB0 = nullptr;
    lbl_eu_80663E14 = nullptr;
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
u32 lbl_eu_80661BC8;
cf::CfPadData lbl_eu_80571500;
s8 lbl_eu_80663E70;
cf::CfGameManager lbl_eu_80571758;
u8 lbl_eu_80571748[0x10];
CPad* lbl_eu_80663E0C;
CPad lbl_eu_80570D40[8];
CScn* lbl_eu_80663E14;
CView* lbl_eu_80663E10;
u32 lbl_eu_80663E60;
u32 lbl_eu_80663E64;
u32 lbl_eu_80663E24;
u32 lbl_eu_80663E28;
u32 lbl_eu_80663DF8;
s16 lbl_eu_80663E3C;
u8 lbl_eu_80663E5C;
float lbl_eu_80663E58;
float lbl_eu_80663E68;
s16 lbl_eu_80661BCC;
float lbl_eu_80666498;
float lbl_eu_8066649C;
BdatTextEntry lbl_eu_80571658[2];
BdatTextEntry lbl_eu_805716F8;

// symbols.txt: Fv, but callers leave the index in r4 (see getPlayer).
cf::CfObjectMove** getPlayerSlotPtr__Q22cf13CfGameManagerFv(cf::CfObjectMove** slots, int index);

void __ct__Q22cf13CfGameManagerFv(cf::CfGameManager* self);

void* __register_global_object(void* object, void* destructor, void* registration);
void MenuSnd_StopSlot1_9424(float value);
}

extern "C" u32 CtrlRemote_TouchBitByArg(u32 resourceId);
extern void setMasterVolume(float value, float fadeTime);
extern const float lbl_eu_8066650C;
extern const float lbl_eu_80666510;
extern const float lbl_eu_80666548;

// func_80085978 imports.
// MiniMapResetLayoutAnims: declared (s32()) by CfGameManager.hpp - local void form conflicts (10505)
extern "C" void CfSoundMan_StopSlotByMode(u32 a, u32 b, u32 c);
// bindPadSubobjects: declared (s32) by CfGameManager.hpp - local u32 form conflicts (10197)
extern "C" void UIBattleMarkFlags82();
extern "C" void setInputDisableTime__Q22cf9CfPadTaskFf(float value);
extern float lbl_eu_806669C8;

#pragma dont_inline on
unsigned int packResourceId(unsigned int first, unsigned int second) {
    return (first << 27) | (second << 20);
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void clearGameFlagMask__Q22cf13CfGameManagerFv(u32 mask) {
    lbl_eu_80663E24 &= ~mask;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" u32 getBdatEntryIndex__Q22cf13CfGameManagerFv(const UnkEEE0Data* data, u16 index) {
    return data->entries_0x2[index];
}

extern "C" u32 getItemSlotCount__Q22cf13CfGameManagerFv(const UnkF044Data* data, u32 index) {
    return data->field_0x888[index];
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void copyItemSourceNode__Q22cf13CfGameManagerFv(UnkF8C0Node* destination,
                                                        const UnkF8C0Source* source) {
    destination->field_0x0 = source->field_0x4->field_0x0;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void setPackedSlotId__Q22cf13CfGameManagerFv(Unk81CA0Data* data, u16 index,
                                                        u32 offset) {
    u32 value = index * 16;
    value += offset;
    data->field_0x6D0 = value;
}
#pragma dont_inline reset

#pragma dont_inline on
bool isGameFlagMaskSet(u32 mask) {
    return (lbl_eu_80663E24 & mask) != 0;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" bool isItemEnabled__Q22cf13CfGameManagerFv(const UnkF0ACData* data) {
    return data->flag_0x4 != 0;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" bool isObjectFlagMaskSet__Q22cf13CfGameManagerFv(const cf::CfObject* data, u32 mask) {
    return (data->mFlags68 & mask) != 0;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" bool compareFlagValues__Q22cf13CfGameManagerFv(const u32* first, const u32* second) {
    return *first != *second;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void togglePauseFlag__Q22cf13CfGameManagerFv(u32 mask, bool enable) {
    if (enable) {
        lbl_eu_80663E28 |= mask;
    } else {
        lbl_eu_80663E28 &= ~mask;
    }
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void clampValueToRange__Q22cf13CfGameManagerFv(s32 minimum, s32* value,
                                                        s32 maximum) {
    if (*value < minimum) {
        *value = minimum;
    } else if (*value > maximum) {
        *value = maximum;
    }
}
#pragma dont_inline reset

// The queueSceneEventA wrapper body is visible in UnityHelpers and -inline auto
// would inline it at call sites, but retail calls it as a real function
// (bl queueSceneEventA__Q22cf13CfGameManagerFv in resetBattlePresentation/
// func_80085FB8), so rename the visible definition away and import the
// retail symbol directly (declared in CfGameManager.hpp).
// (func_8008064C's CfCamPosSource traffic goes through the unity-helpers
// header's own forward decl + vtable-slot dispatch; no TU-local view needed.)

// .sdata2 float constants used by func_8008064C in the unity-helpers header.
extern const float lbl_eu_80666540;
extern const float lbl_eu_8066A1F8;
extern const float lbl_eu_80666544;

#define queueSceneEventA__Q22cf13CfGameManagerFv unityHelpersWrapFunc_80083D50
#define flushEventQueue__Q22cf13CfGameManagerFv unityHelpersWrapFunc_800827E4
#define syncBdatDataCache__Q22cf13CfGameManagerFv unityHelpersWrapFunc_8007DECC
#define setPresentationFlag__Q22cf13CfGameManagerFv unityHelpersWrapFunc_8008294C
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
        UnkClass_8009EC9C* data = (UnkClass_8009EC9C*)func_8009EC9C(static_cast<u16>(value));
        s32 destination = func_80063560(value, 0, 0);
        if (destination >= 0 && destination != *current) {
            func_8009E120((UnkClass_8009EC9C*)func_8009EC9C(static_cast<u16>(value)), 0);
            CfRes_tryResolveByBits(destination,
                          func_8009E120((UnkClass_8009EC9C*)func_8009EC9C(static_cast<u16>(value)), 0),
                          size);
            CfRes_tryResolveByBits(destination,
                          func_8009E120((UnkClass_8009EC9C*)func_8009EC9C(static_cast<u16>(value)), 1),
                          size);
            CfRes_tryResolveByBits(destination,
                          func_8009E120((UnkClass_8009EC9C*)func_8009EC9C(static_cast<u16>(value)), 2),
                          size);
            CfRes_tryResolveByBits(destination,
                          func_8009E120((UnkClass_8009EC9C*)func_8009EC9C(static_cast<u16>(value)), 3),
                          size);
            CfRes_tryResolveByBits(destination,
                          func_8009E120((UnkClass_8009EC9C*)func_8009EC9C(static_cast<u16>(value)), 4),
                          size);
            CfRes_tryResolveByBits(destination,
                          func_801422A8__Q22cf6CfBdatFUl(data->textId_0x18), size);
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
        Scn_SetTimeScale(lbl_eu_80663E14, lbl_eu_80666498);
    } else {
        clearGameFlagMask__Q22cf13CfGameManagerFv(0x40000000);
        Scn_SetTimeScale(lbl_eu_80663E14, lbl_eu_8066649C);
    }
    CfSoundMan_PauseAllRecords(enable, 10);
}

extern "C" __declspec(noinline) void queueSceneEventA__Q22cf13CfGameManagerFv(
    u32 first, u32 second, u32 third, u32 fourth, float value) {
    func_800838F4__Q22cf13CfGameManagerFv(0, first, second, third, fourth, value);
}

extern "C" void flushEventQueue__Q22cf13CfGameManagerFv() {
    while (getEventQueueCount__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0) != 0) {
        u16 value = popEventQueue__Q22cf13CfGameManagerFv(&lbl_eu_80570CF0);
        setEventFlag__Q22cf13CfGameManagerFv(value, true);
    }
}

namespace cf {

void CfGameManager::resetGameFlags() {
    if (this == nullptr) {
        lbl_eu_80663E24 |= 8;
    }

    lbl_eu_80663E28 &= ~8;
    lbl_eu_80663E3C = 0;
    lbl_eu_80663E68 = lbl_eu_80666498;
    lbl_eu_80663E5C = 0;
    lbl_eu_80663E58 = lbl_eu_80666498;
    lbl_eu_80661BCC = -1;
    MenuSnd_StopSlot1_9424(lbl_eu_8066649C);
}

void CfGameManager::clearBdatTextEntries() {
    lbl_eu_80571658[0].text[0] = 0;
    lbl_eu_80571658[0].textLength = 0;
    lbl_eu_80571658[0].secondaryText[0] = 0;
    lbl_eu_80571658[0].secondaryTextLength = 0;
    lbl_eu_80571658[1].text[0] = 0;
    lbl_eu_80571658[1].textLength = 0;
    lbl_eu_80571658[1].secondaryText[0] = 0;
    lbl_eu_80571658[1].secondaryTextLength = 0;
    lbl_eu_805716F8.text[0] = 0;
    lbl_eu_805716F8.textLength = 0;
    lbl_eu_805716F8.secondaryText[0] = 0;
    lbl_eu_805716F8.secondaryTextLength = 0;
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
    lbl_eu_80663E14 = 0;
    lbl_eu_80663E60 = 0;
    lbl_eu_80663E64 = 0;
    lbl_eu_80663E24 = 0;
    lbl_eu_80663E28 = 0;
    unk0 = 0;
    field_0x4 = 0;
    mObjectFlags = 0;
}

#pragma dont_inline on
CfGameManager* CfGameManager::getInstance() {
    if (!lbl_eu_80663E70) {
        // Call mangled ctor directly: placement new inserts a null check retail lacks.
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv, lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    return &lbl_eu_80571758;
}
#pragma dont_inline off

u32 CfGameManager::getCurrentPadChannel() {
    return lbl_eu_80661BC8;
}

CfPadData* CfGameManager::getCfPadData() {
    return &lbl_eu_80571500;
}

CPad* CfGameManager::getCurrentPad() {
    u32 channel = lbl_eu_80661BC8;
    if (channel == 0xFFFFFFFF) {
        CPad* pad = lbl_eu_80663E0C;
        if (pad != 0) {
            return pad;
        }
        return lbl_eu_80570D40;
    }
    CPad* pad = lbl_eu_80663E0C;
    if (pad != 0) {
        return pad;
    }
    return &lbl_eu_80570D40[channel & 7];
}

#pragma dont_inline on
CfObjectMove* CfGameManager::getPlayer(int playerIndex) {
    return *getPlayerSlotPtr__Q22cf13CfGameManagerFv(getInstance()->unk94, playerIndex);
}
#pragma dont_inline reset

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

#pragma dont_inline on
extern "C" cf::CfObjectMove** getPlayerSlotPtr__Q22cf13CfGameManagerFv(cf::CfObjectMove** slots,
                                                                              int index) {
    return &slots[index];
}
#pragma dont_inline off

#pragma dont_inline on
extern "C" void enableGameFlag__Q22cf13CfGameManagerFv(u32 flags) {
    lbl_eu_80663E24 |= flags;
}
#pragma dont_inline reset

extern "C" void disableGameFlag__Q22cf13CfGameManagerFv(u32 flags) {
    lbl_eu_80663E24 &= ~flags;
}

#pragma dont_inline on
extern "C" void clearTempTextBuffer__Q22cf13CfGameManagerFv(Unk87588Data* data) {
    data->field_0x0 = 0;
    data->field_0x20 = 0;
}
#pragma dont_inline reset

#pragma dont_inline on
bool cf::CfGameManager::isManagerInitialized() {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    return (lbl_eu_80663E24 & 0x10000) != 0;
}
#pragma dont_inline reset
// cf::CfGameManager::teardownGameManager - full teardown of the CfGameManager
// singleton: mirrors the destructor's subsystem detach/delete sequence, then
// resets the global state and removes the layered CProcess hooks. The
// vtable+0x08 dispatch with delete flag 1 is MWCC's delete-on-cast-dtor
// shape (the if + delete-expansion null guards produce the paired beq; see
// CfGameManager.hpp CfDtor08).
void cf::CfGameManager::teardownGameManager() {
    lbl_eu_80663E28 |= 0x200000;
    func_8012F87C(0);
    CUICfManager_setFlagState(0);
    CCharVoiceMan_DestroySingleton();
    getBattleStateB__Q22cf14CBattleManagerFv();
    destroyHelpManager();
    __dt__80157150();
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    cf::CfGameManager* manager = &lbl_eu_80571758;
    gmFileObject(manager->unk90);
    manager->unk90 = nullptr;
    if (manager->unkA8 != nullptr) {
        BattleWork_InitCtrlWork(manager->unkA8);
    }
    if (manager->unkA0 != 0) {
        MapFx_ReleaseHandle(manager->unkA0);
    }
    if (manager->field_0xA4 != nullptr) {
        func_80186664(manager->field_0xA4);
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
        delete reinterpret_cast<CfDtor08*>(manager->unkA0);
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
    if (lbl_eu_80663E60 != 0) {
        __dt__801886EC(reinterpret_cast<void*>(lbl_eu_80663E60), 1);
        lbl_eu_80663E60 = 0;
    }
    if (lbl_eu_80663E64 != 0) {
        __dt__801BF874(reinterpret_cast<void*>(lbl_eu_80663E64), 1);
        lbl_eu_80663E64 = 0;
    }
    lbl_eu_80663E14 = nullptr;
    lbl_eu_80663E24 &= ~0x30000;
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
            CfTFile_CancelResetFull(slots->slots[i]);
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
    lbl_eu_80663E04 = 0;
    CfRes_callFunc_67E78(stopSoundMan__Fv());
    CfRes_callFunc_67FE0();
    KyoshinHeap_Shutdown();
    if (lbl_eu_80663E1C != nullptr) {
        Remove__8CProcessFv(lbl_eu_80663E1C);
        lbl_eu_80663E1C = nullptr;
    }
    if (lbl_eu_80663E18 != nullptr) {
        Remove__8CProcessFv(lbl_eu_80663E18);
        lbl_eu_80663E18 = nullptr;
    }
    getEntry__5CBdatFUl(1);
    if (lbl_eu_80663E20 != nullptr) {
        Remove__8CProcessFv(lbl_eu_80663E20);
        lbl_eu_80663E20 = nullptr;
    }
}

#pragma dont_inline on
UnkClass_80083298* cf::CfGameManager::getGameSubManager() {
    return getInstance()->unk90;
}
#pragma dont_inline reset
extern "C" void* firstReslistB48();
extern "C" void nextReslistB48();
extern "C" void triggerVoiceDown(){ firstReslistB48(); }
struct VoiceSource;

extern "C" void getVoiceSourcePtr()
{
    firstReslistB68();
}

extern "C" void resetVoiceSystem(void){
    nextReslistB48();
}

extern "C" VoiceSource* getVoiceSourceForSystem(VoiceSource* source)
{
    return insertReslistB68(source);
}
// cf::CfGameManager::func_80086B5C - battle-start pad/object reset. Refreshes
// the party-slot pair, optionally (flag != 0) rotates the slot index into the
// 0..0x17 ring and refreshes every gimmick object's 0x3E9C container, then
// pumps the pad manager a fixed number of frames (5, or 8/16 depending on
// arg1 % 3) before clearing the bit-26 presentation flag.
extern "C" void func_80086B5C__Q22cf13CfGameManagerFv(int arg1, int arg2,
                                                        int flag) {
    func_8006A6D0();
    u16 outA;
    u16 outB;
    CfT_PlayClockSnapshot(&outA, &outB);
    func_8006A03C(arg1, arg2, 0);
    if (flag != 0) {
        lbl_eu_80663E28 |= 0x04000100;
        // Wrap arg1-3 into the 0..0x17 slot ring.
        int slot = arg1 - 3;
        if (slot < 0) {
            slot += 0x18;
        }
        if (slot >= 0x18) {
            slot -= 0x18;
        }
        func_8006A03C((u16)slot, arg2, 0);
        // Refresh every gimmick object's 0x3E9C container.
        Func800B6BECList* list = getReslistB68();
        for (Func800B6BECNode* node = list->head->next; node != list->head;
             node = node->next) {
            void* rawObj = func_8016FE34(node->object);
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
            container->CObjectState_setStateBitFlag(4);
            container->CObjectState_clearStateFlags8(0x1000);
        }
    }
    for (int i = 0; i < 5; ++i) {
        CfScript_UpdateMgr();
        gmDispatchPair(cf::CfGameManager::getPlayer(0), 1);
    }
    func_8006A03C(arg1, arg2, 0);
    Func800B6BECList* list = getReslistB68();
    for (Func800B6BECNode* node = list->head->next; node != list->head;
         node = node->next) {
        void* rawObj = func_8016FE34(node->object);
        if (rawObj == nullptr) {
            continue;
        }
        // Same +0x3E9C CfObjectMove subobject refresh as above.
        cf::CfObjectMove* container = reinterpret_cast<cf::CfObjectMove*>(
            reinterpret_cast<u8*>(rawObj) + 0x3E9C);
        CfObjectMove_setAnimModeArgs(reinterpret_cast<u8*>(container), 1, 0, -1, 1);
        container->CObjectState_setStateBitFlag(4);
        container->CObjectState_clearStateFlags8(0x1000);
    }
    lbl_eu_80663E28 &= ~0x00800000;
    // 8 pump frames when arg1 divides evenly by 3, else 16.
    int count = (arg1 % 3 == 0) ? 8 : 16;
    for (int i = 0; i < count; ++i) {
        CfScript_UpdateMgr();
        gmDispatchPair(cf::CfGameManager::getPlayer(0), 1);
    }
    lbl_eu_80663E28 &= ~0x04000000;
    setPresentationFlag__Q22cf13CfGameManagerFv(false);
}
void cf::CfGameManager::enablePadFlags(u32 inputFlags, bool enable) {
    u32 enabledFlags = lbl_eu_80663DF8 & ~inputFlags;
    if (enable) {
        enabledFlags = lbl_eu_80663DF8 | inputFlags;
    }
    lbl_eu_80663DF8 = enabledFlags;

    if (lbl_eu_80663E0C == nullptr) {
        return;
    }
    if (enable) {
        return;
    }

    lbl_eu_80663E0C->mPressedButtonFlags &= enabledFlags;
    lbl_eu_80663E0C->mTurboPressButtonFlags &= enabledFlags;
    lbl_eu_80663E0C->mReleasedButtonFlags &= enabledFlags;
    lbl_eu_80663E0C->mHeldButtonFlags &= enabledFlags;
    lbl_eu_80663E0C->mLongHoldButtonFlags &= enabledFlags;
    lbl_eu_80663E0C->mShortPressButtonFlags &= enabledFlags;
    lbl_eu_80571500.mPressedButtonFlags &= enabledFlags;
    lbl_eu_80571500.mTurboPressButtonFlags &= enabledFlags;
}
u32 cf::CfGameManager::getEnabledInputFlags() {
    if (isSceneLoading()) {
        if (isAnyMenuOpen__9CMainMenuFv() || getInstance__11CSysWinBuffFv() != nullptr ||
            isInitialized__10CMenuPauseFv()) {
            return 0xFFFFFFFF;
        }
    }
    return lbl_eu_80663DF8;
}
bool cf::CfGameManager::isClassicController() {
    return CfPadTask::getDetectedPadType() == PAD_TYPE_CLASSIC;
}
CPad* cf::CfGameManager::getPad(int channel) {
    return &lbl_eu_80570D40[channel & 7];
}
extern "C" void setPad__Q22cf13CfGameManagerFv(u32 channel, CPad* pad, u32 arg3) {
    lbl_eu_80570D40[channel & 7] = *pad;
    if (lbl_eu_80661BC8 == 0xFFFFFFFF && channel <= 3 && arg3 != 0) {
        lbl_eu_80661BC8 = channel;
    }
}
void cf::CfGameManager::setCurrentPadPtr(const CPad* pad, u32 channel) {
    if (pad != nullptr && lbl_eu_80661BC8 != 0xFFFFFFFF && lbl_eu_80661BC8 != channel) {
        return;
    }

    lbl_eu_80663E0C = const_cast<CPad*>(pad);
}
extern "C" void clearTextBufferExtra(Unk87588Data* data) {
    data->field_0x0 = 0;
    data->field_0x20 = 0;
    data->field_0x24 = 0;
    data->field_0x44 = 0;
    data->field_0x48 = lbl_eu_80666548;
}
extern u16 lbl_eu_80664772;
bool cf::CfGameManager::isSceneReadyForInput() {
    if (lbl_eu_80663E14 == nullptr) {
        return true;
    }
    return Scn_IsDefaultScale(lbl_eu_80663E14);
}

bool cf::CfGameManager::isSceneActive() {
    if (lbl_eu_80663E14 == nullptr) {
        return false;
    }
    return Scn_IsScaleAtLeastOne(lbl_eu_80663E14);
}

bool cf::CfGameManager::isFieldTransitionReady() {
    if ((lbl_eu_80663E24 & 0x8000) != 0 && !CfScript_TestMask(0x20)) {
        return false;
    }
    return probeReadyLists();
}

bool cf::CfGameManager::isFieldBlockedByFlag() {
    if ((lbl_eu_80663E24 & 0x48000) != 0) {
        if (CfScript_TestMask(0x20)) {
            return true;
        }
    }
    return false;
}

bool cf::CfGameManager::isAnyFieldFlagSet() {
    return (lbl_eu_80663E24 & 0x48000) != 0;
}

#pragma dont_inline on
bool cf::CfGameManager::isSceneLoading() {
    if (testResInfoFlag(0x04000000)) {
        return false;
    }

    getInstance();
    return Scn_GetFrameDelta(lbl_eu_80663E14) <= lbl_eu_8066A208;
}
#pragma dont_inline reset

#pragma dont_inline on
bool cf::CfGameManager::isAllEventsDone() {
    return lbl_eu_80664772 != 0;
}
#pragma dont_inline reset

bool cf::CfGameManager::isTimerActive() {
    return lbl_eu_80663E3C > 0;
}

u16 cf::CfGameManager::getField18Value() {
    return field_0x18;
}

#pragma dont_inline on
cf::CfObject** cf::CfGameManager::getField04Ptr() {
    return &field_0x4;
}
#pragma dont_inline reset

// Retail Fv symbol: callers leave r4 = name for slot 0x120; forced-name form
// keeps the Fv linker name while forwarding the live argument onto the real
// CfObject slot (findNodeMatrix alias, same arity, returns void* for the
// tail-call). CfObjectImplPc's vtable serves this entry at +0x68.
extern "C" void* dispatchObjectFunc52b__Q22cf13CfGameManagerFv(
    cf::CfGameManager* self, const char* name) {
    return self->field_0x4->CfObject_findNodeMatrix(name);
}

void cf::CfGameManager::dispatchObjectFunc53() {
    field_0x4->CfObject_getIndexedNodeMatrix();
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

#pragma dont_inline on
void cf::CfGameManager::syncFieldAndBattle() {
    func_80141B20(this);
    setMgrFixStrName(this);
}
#pragma dont_inline reset

void cf::CfGameManager::cleanupMapEffects() {
    if (getGameSubManager() != nullptr && &getGameSubManager()->field_0xF0 != nullptr) {
        flushMpfBuffer__17UnkClass_8047BB54Fv(&getGameSubManager()->field_0xF0);
    }
}

u32 cf::CfGameManager::getEffectFlagState() {
    UnkClass_80083298* object = getGameSubManager();
    return object != nullptr ? getEffectFlagFromMgr(object) : 0;
}

void cf::CfGameManager::tickGameManager() {
    getInstance()->func_8007C8C8();
}

// cf::CfGameManager::func_8007C8C8 - per-frame scene/presentation tick and
// the scene-load / object-queue dump. Returns the CfCmd_PumpRingBuf result.
// Flag bits use the local convention (value bit = 31 - PPC bit): E24 0x80,
// 0x1000, 0x400000, 0x40000|0x8000, 0x2000000|0x400; E28 0x1000000, 0x200,
// 0x8000000.
void* cf::CfGameManager::func_8007C8C8() {
    void* ret = CfCmd_PumpRingBuf(this->unkAC, &lbl_eu_80663E04);
    if (lbl_eu_80663E60 != 0) {
        func_801889D0(lbl_eu_80663E60);
    }
    // func_800D9354 takes cf::CBattleManager* (decl on CBattleManagerApi.hpp);
    // explicit cast - the singleton getter returns void* and MWCC rejects
    // the implicit void* conversion.
    func_800D9354((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv());
    if (lbl_eu_80663E24 & 0x80) {
        func_80295A88(lbl_eu_80664A10);
    }
    func_802A1610();
    if (lbl_eu_80663E14 != nullptr &&
        Scn_GetFrameDelta(lbl_eu_80663E14) > lbl_eu_80666498) {
        ++lbl_eu_80663D90;
    }
    if (!(lbl_eu_80663E28 & 0x1000000)) {
        CfT_AdvanceFrameTimer();
    }
    if (getEffectFlagState__Q22cf13CfGameManagerFv() != 0) {
        ++lbl_eu_80663E6C;
        if (!(lbl_eu_80663E6C & 1)) {
            if (lbl_eu_80663E24 & 0x1000) {
                func_80186D20(this->field_0xA4);
            }
        } else if ((lbl_eu_80663E6C & 1) == 1 && (lbl_eu_80663E24 & 0x1000)) {
            MapFx_ForwardReload(this->unkA0);
        }
        if ((lbl_eu_80663E24 & 0x1000) && !(lbl_eu_80663E28 & 0x1000000)) {
            MapFx_UpdateSceneFx(this->unkA0);
        }
        this->updateCameraState();
    }
    if ((lbl_eu_80663E24 & 0xAFA40000) == 0 ||
        (lbl_eu_80663E28 & 0x1000000) == 0) {
        Selector_GetInstance();
        func_800FE104();
        if (getEffectFlagState__Q22cf13CfGameManagerFv() != 0) {
            if (lbl_eu_80663E24 & 0x1000) {
                func_8019FD2C();
            }
            // Retail loads E24 once and tests bit 26 (rlwinm 6,6) OR'd
            // with bit 21 (rlwimi 21,21).
            u32 e24bits = lbl_eu_80663E24;
            if ((((e24bits & 0x04000000) | (e24bits & 0x400)) != 0) &&
                (lbl_eu_80663E28 & 0x1000000) == 0) {
                goto done;
            }
            if (!lbl_eu_80663E70) {
                __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
                __register_global_object(&lbl_eu_80571758,
                                         __dt__Q22cf13CfGameManagerFv,
                                         lbl_eu_80571748);
                lbl_eu_80663E70 = 1;
            }
            cf::CfGameManager* gmn = &lbl_eu_80571758;
            if (gmn != nullptr) {
                if (!lbl_eu_80663E70) {
                    __ct__Q22cf13CfGameManagerFv(gmn);
                    __register_global_object(gmn, __dt__Q22cf13CfGameManagerFv,
                                             lbl_eu_80571748);
                    lbl_eu_80663E70 = 1;
                }
                if (isGameFlagMaskSet(0x1000)) {
                    cf::CfObjectMove* player = getPlayer(0);
                    if (player != nullptr) {
                        func_8016EC58(player->CfObject_getPosVector());
                    }
                }
            }
            func_80069F2C();
            if (isSceneLoading() == 0 &&
                cf::CfPadTask::isInputDisabled() == 0) {
                int sceneReady = lbl_eu_80663E14 == nullptr
                                     ? 1
                                     : Scn_IsDefaultScale(lbl_eu_80663E14);
                if (sceneReady != 0 &&
                    (lbl_eu_80663E24 & 0x400000) == 0 &&
                    (lbl_eu_80663E24 & 0xAFA40000) == 0 &&
                    ((lbl_eu_80663E24 & 0x04000000) |
                     (lbl_eu_80663E24 & 0x400)) == 0 &&
                    (lbl_eu_80663E24 & (0x40000 | 0x8000)) == 0 &&
                    (lbl_eu_80663E28 & 0x200) == 0 &&
                    BpsStateSingletonToInt() &&
                    (lbl_eu_80663E28 & 0x8000000) == 0 &&
                    (lbl_eu_80663E24 & 0x80) != 0) {
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
    if (CView::getCurrentView() == lbl_eu_80663E10) {
        gmCallNopA();
    }
}

void cf::CfGameManager::resetCameraIfCurrentView() {
    if (CView::getCurrentView() == lbl_eu_80663E10) {
        gmCallNopB();
    }
}

void cf::CfGameManager::fadeOutGameEffects() {
    setMasterVolume(lbl_eu_8066650C, lbl_eu_8066649C);
    MenuSnd_SetMasterAndPush_9510(lbl_eu_80666510);
    MenuSnd_SetSeVolPush_95F4(lbl_eu_8066649C);
}

void cf::CfGameManager::notifyCameraManager() {
    func_80079B34(getInstance()->unkB4);
}

bool cf::CfGameManager::isCameraReady() {
    return func_80079DBC(getInstance()->unkB4);
}

#pragma dont_inline on
UnkClass_800821F8* cf::CfGameManager::getCameraDataBlock() {
    return getInstance()->unkB0;
}
#pragma dont_inline reset

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

u32 cf::CfGameManager::getMapEffectManager() {
    return getInstance()->unkA0;
}

void cf::CfGameManager::loadBdatGroupData() {
    CtrlObjectParam_ResolveEquipItem(&field_0x1C);
}

bool cf::CfObjectMap::isObjectMapReady() {
    return field_0x8E >= 2;
}

void cf::CfGameManager::notifyObjectMapChange() {
    if (lbl_eu_80663E24 & 0x1000) {
        func_80186D20(field_0xA4);
    }
}

void cf::CfObject::CfObject_applyMoveYaw(float amount) {
    CfObject_syncMoveHead();
}

// Retail Fv linker name kept for hand-built vtables (CfCollSphereImpl /
// CfObjectModel). Member emits CfObject_applyMoveYaw__...Ff.
extern "C" void CfObject_UnkVirtualFunc33__Q22cf8CfObjectFv(
    cf::CfObject* object) {
    object->CfObject_syncMoveHead();
}

#pragma dont_inline on
void cf::CfGameManager::enableFlag10() {
    lbl_eu_80663E24 |= 0x10;
}
#pragma dont_inline reset

#pragma dont_inline on
void cf::CfGameManager::enableFlag20() {
    lbl_eu_80663E24 |= 0x20;
}
#pragma dont_inline reset

#pragma dont_inline on
void cf::CfGameManager::enableFlag40() {
    lbl_eu_80663E24 |= 0x40;
}
#pragma dont_inline reset

void cf::CfGameManager::disableFlag20() {
    lbl_eu_80663E24 &= ~0x20;
}

#pragma dont_inline on
bool cf::CfGameManager::isVisionPackLoaded() {
    return CtrlRemote_TouchBitByArg(0x3508) != 0;
}
#pragma dont_inline reset

#pragma dont_inline on
void* cf::CfGameManager::getSelf() { return this; }
#pragma dont_inline reset
void cf::CfGameManager::stubEmptyA() {}
void cf::CfGameManager::dispatchObjectFunc52() {}
void cf::CfObject::CfObject_setMoveTargetPtr(void*) {}

// Retail Fv base impl (us/jp/eu 0x80087D64: bare blr) for vtable slot 0x108.
// Same dual-symbol shape as CfObject_applyMoveYaw above: the C++
// member def emits the FPv override-chain symbol (referenced by the
// CfCollSphere/CfObjectEff hand vtables); this forced-name extern "C"
// form emits the retail Fv linker name (referenced by the CfObjectModel
// hand vtables). Empty body: retail is a single blr.
extern "C" void CfObject_setMoveTargetPtr__Q22cf8CfObjectFv(
    cf::CfObject*) {}
void cf::CfObject::CfObject_createMoveTarget() {}

void cf::CfGameManager::stubEmptyB() {}
void cf::CfGameManager::stubEmptyC() {}
void cf::CfGameManager::stubEmptyD() {}
void cf::CfGameManager::stubEmptyE() {}
extern "C" void* getNullPtrA__Q22cf13CfGameManagerFv() { return nullptr; }
extern "C" void* getNullPtrB__Q22cf13CfGameManagerFv() { return nullptr; }
extern "C" void* getNullPtrC__Q22cf13CfGameManagerFv() { return nullptr; }
// Slot +0x124 base: retail Fv returns nullptr. Forced-name keeps the Unk
// linker spelling for hand-built vtables; virtual is CfObject_getIndexedNodeMatrix.
extern "C" cf::CfObject* CfObject_UnkVirtualFunc53__Q22cf8CfObjectFv(
    cf::CfObject*) {
    return nullptr;
}
extern u32 lbl_eu_80661BC0;
u32* cf::CfObject::CfObject_getMoveRateScale() {
    return &lbl_eu_80661BC0;
}
// lwz getters - return global vars
extern u32 lbl_eu_80664188;
u32 cf::CfGameManager::getGlobalWord64188() { return lbl_eu_80664188; }
extern u32 lbl_eu_80664184;
u32 cf::CfGameManager::getGlobalWord64184() { return lbl_eu_80664184; }
extern "C" u32 getGlobalWord64184Dup__Q22cf13CfGameManagerFv() { return lbl_eu_80664184; }
extern u32 lbl_eu_8066409C;
extern "C" u32 getGlobalPtr6409C__Q22cf13CfGameManagerFv() { return lbl_eu_8066409C; }
extern void* lbl_eu_806640A8;
extern "C" u32 getGlobalPtr640A8__Q22cf13CfGameManagerFv() { return (u32)lbl_eu_806640A8; }
extern u32 lbl_eu_806640A4;
extern "C" u32 getGlobalWord640A4__Q22cf13CfGameManagerFv() { return lbl_eu_806640A4; }
extern u32 lbl_eu_806640F4;
extern "C" u32 getGlobalWord640F4__Q22cf13CfGameManagerFv() { return lbl_eu_806640F4; }
extern u32 lbl_eu_80663D90;
extern "C" u32 getFieldD90Value__Q22cf13CfGameManagerFv() { return lbl_eu_80663D90; }
extern u8 lbl_eu_80663E34;
extern "C" u8 getGlobalFlag34__Q22cf13CfGameManagerFv() { return lbl_eu_80663E34; }
extern u8 lbl_eu_80663E5D;
u8 cf::CfGameManager::getVoiceFlag5D() { return lbl_eu_80663E5D; }
extern u8 lbl_eu_8066476D;
u8 cf::CfGameManager::getVoiceFlag6D() { return lbl_eu_8066476D; }
void* cf::CfGameManager::getResetDataPtr() { return lbl_eu_8065FC18; }

// cf::CfGameManager::func_8007D84C - battle-exit / scene reset. Tears down
// the mode subsystems, wipes the manager's per-mode state block, clears the
// presentation globals and the E24/E28 flag masks.
extern "C" void CfSoundMan_ClearFxEffect(u32 first, u32 second);
extern "C" void gmCallInit1954();
extern "C" void MapFx_SetGlobalFloat(float value);
extern "C" void func_800D9218__Q22cf14CBattleManagerFv(void* battle);
extern "C" void func_80066788(u32 a, u32 b, u32 c, u32 d);
extern "C" void CfRes_getInstPtr170();
extern "C" void EffSys_Init4F9C(void* object);
extern "C" float lbl_eu_80667700;
#pragma dont_inline on
void cf::CfGameManager::func_8007D84C() {
    extern u16 lbl_eu_80663E3A;
    func_801AAD08();
    lbl_eu_8066443A = 0;
    func_801862E0(ArtsSelect_GetContainer());
    CfSoundMan_StopAllModes(1);
    CfSoundMan_ClearFxEffect(0, 0);
    CfSoundMan_ClearFxEffect(1, 0);
    func_8012F87C(0);
    CUICfManager_setFlagState(0);
    if ((lbl_eu_80663EE0 & 0x40) != 0) {
        lbl_eu_80663EE0 &= ~0x40;
    }
    memset(unk94, 0, sizeof(unk94));
    unk90 = nullptr;
    float zeroF = lbl_eu_80666498;
    lbl_eu_80663E50 = 0;
    field_0x86 = 0;
    lbl_eu_80663E3A = 0;
    lbl_eu_80663E3C = 0;
    lbl_eu_80663E3E = 0;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(this) + 0x80) = 0;
    lbl_eu_80663E34 = 0;
    lbl_eu_80663E35 = 0;
    lbl_eu_80663E68 = zeroF;
    lbl_eu_80663E00 = 0;
    lbl_eu_80663E08 = zeroF;
    if (unkA8 != nullptr) {
        BattleWork_InitCtrlWork(unkA8);
    }
    if (unkA0 != 0) {
        MapFx_ReleaseHandle(unkA0);
    }
    if (field_0xA4 != nullptr) {
        func_80186664(field_0xA4);
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
    func_80066788(0, 0, 0, 0);
    Selector_GetInstance();
    __dt__800FDEF8();
    func_800D9218__Q22cf14CBattleManagerFv(
        getInstance__Q22cf14CBattleManagerFv());
    CfScript_ClearReq0();
    CfScript_ClearReq1();
    CfScript_ClearReq2();
    lbl_eu_80663E24 = lbl_eu_80663E24 & 0x505B0208;
    lbl_eu_80663E28 = lbl_eu_80663E28 & 0xFBFFF430;
    NpcMove_ClearRowBmp();
    MapFx_SetGlobalFloat(lbl_eu_80667700);
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" Unk8187CData* lookupResourceByKeys__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third) {
    return reinterpret_cast<Unk8187CData*>(spawnFromRecord(reinterpret_cast<UnkClass_800AA714*>(first), reinterpret_cast<UnkClass_800AA714*>(second), third));
}
#pragma dont_inline reset

void cf::CfGameManager::clearPlayerEffect() { gmFileObject(this); }

// Static member (same mangled Fv symbol as retail): the body is a bare
// tail-call to gmSpawnMask8000, so the returned object rides in r3.
// Retail calls this out-of-line from func_80086778 - keep it a real function.
#pragma dont_inline on
void* cf::CfGameManager::createBdatCollisionObj() { return gmSpawnMask8000(); }
#pragma dont_inline reset

extern "C" void* mapKindToSpawn(void* object);
#pragma dont_inline on
extern "C" Unk80EE4Data* createEffectForResource__Q22cf13CfGameManagerFv(
    void* object, u32 value) {
    return static_cast<Unk80EE4Data*>(mapKindToSpawn(object));
}
#pragma dont_inline reset

void cf::CfGameManager::resetFieldState() { gmNotifyCA0(); }

extern "C" void Counter_TestBit(void* object);
void cf::CfGameManager::cleanupPlayerEffectList() { Counter_TestBit(this); }

void cf::CfGameManager::getGimmickListHead() { getListB28__Fv(); }

extern "C" void* getReslistB48();
void cf::CfGameManager::getFirstGimmick() { getReslistB68(); }
void cf::CfGameManager::getGimmickList() { getReslistB48(); }

extern "C" void* getReslistB48();
extern "C" void* getReslistB88();

void cf::CfGameManager::spawnGimmickEntity() { getReslistB88(); }

extern "C" void getReslistBE8();
void cf::CfGameManager::clearGimmickFlags() { getReslistBE8(); }

extern "C" bool lookupWorkAtAddr();
void cf::CfGameManager::isGimmickActive() { lookupWorkAtAddr(); }

void cf::CfGameManager::initPadSubSystem() { func_80141B54(); }

void cf::CfGameManager::updatePadState() { CfT_PlayRateGet(); }

void cf::CfGameManager::loadControllerConfigA() { func_8006A12C(); }

void cf::CfGameManager::loadControllerConfigB() { func_8006A1A0(); }

extern "C" void getControllerValues__Q22cf13CfGameManagerFv(u16* first, u16* second) { CfT_PlayClockSnapshot(first, second); }

#pragma dont_inline on
bool cf::CfGameManager::isControllerReady() { return func_8006A2E0(); }
#pragma dont_inline reset

extern "C" u32 CfT_FrameCountA();
u32 cf::CfGameManager::getControllerWordA33C() { return CfT_FrameCountA(); }

void cf::CfGameManager::getControllerWordA37C() { CfT_FrameCountB(); }

void cf::CfGameManager::getControllerWordA3BC() { CfT_FrameCountC(); }

void cf::CfGameManager::resetControllerState() { CfT_FrameCounterStore(); }

void cf::CfGameManager::clearControllerState() { CfT_FrameCounterLoad(); }

extern "C" u32 func_8006A6D0();
u32 cf::CfGameManager::getCurrentSlotIndex() { return func_8006A6D0(); }

#pragma dont_inline on
u16 cf::CfGameManager::getItemId() {
    ItemListEntryView* object = reinterpret_cast<ItemListEntryView*>(this);
    return object->itemId_0x8C;
}
#pragma dont_inline reset

extern u16 lbl_eu_80663E3A;
extern "C" u16 getEventCounterA__Q22cf13CfGameManagerFv() { return lbl_eu_80663E3A; }

extern "C" void setEventCounterA__Q22cf13CfGameManagerFv(u16 value) {
    lbl_eu_80663E3A = value;
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
    return *reinterpret_cast<Unk817A8Object**>(reinterpret_cast<u8*>(this) + 8);
}

#pragma dont_inline on
extern "C" void setObjectFlags64__Q22cf13CfGameManagerFv(Unk8187CData* data, u32 value) { *reinterpret_cast<u32*>(reinterpret_cast<u8*>(data) + 0x64) = value; }
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void setObjectItemId__Q22cf13CfGameManagerFv(u8* data, u16 value) { *reinterpret_cast<u16*>(data + 0x8C) = value; }
#pragma dont_inline reset

#pragma dont_inline on
UNKWORD cf::CfGameManager::getQueuedFileEventCount() { return CtrlRemote_TouchBitByArg(0x20); }
#pragma dont_inline reset

#pragma dont_inline on
extern "C" u32 isEventFlagActive__Q22cf13CfGameManagerFv(u32 data) { return CtrlRemote_TouchBitByArg(data + 0x312C); }
#pragma dont_inline reset
extern "C" u32 getEventValue40__Q22cf13CfGameManagerFv(u8* data) { return CtrlRemote_TouchBitByArg(reinterpret_cast<u32>(data + 0x40)); }


#pragma dont_inline on
extern "C" s32 getStateTimer532__Q22cf13CfGameManagerFv(const u8* data) {
    return *reinterpret_cast<const s16*>(data + 0x532);
}
#pragma dont_inline reset

extern u8 lbl_eu_80664298;
void cf::CfGameManager::setGlobalByte64298(u32 value) { lbl_eu_80664298 = value; }

extern "C" void* getDataArray219C__Q22cf13CfGameManagerFv(u8* data) { return data + 0x219C; }

void cf::CfGameManager::resetCameraManager() { Scn_GetUnk80Handle(lbl_eu_80663E14); }

// Slot +0x130 base: retail Fv returns the shared float const. Forced-name
// keeps the Unk linker spelling for hand-built vtables; virtual is
// CfObject_getHeightDiff.
extern "C" float CfObject_UnkVirtualFunc56__Q22cf8CfObjectFv(cf::CfObject*) {
    return lbl_eu_80666498;
}

u32 cf::CfGameManager::getObjectFlags() {
    return mObjectFlags;
}

bool cf::CfGameManager::isGameFlagEnabled() {
    return (lbl_eu_80663E24 & 0x80) != 0;
}

#pragma dont_inline on
u32 cf::CfGameManager::getPackedResourceBits() {
    const u32* packed = reinterpret_cast<const u32*>(this);
    return *packed >> 20;
}
#pragma dont_inline reset

#pragma dont_inline on
void** cf::CfGameManager::getObjectNodePtr() {
    UnkF8C0Node* iterator = reinterpret_cast<UnkF8C0Node*>(this);
    ItemListNode* node = reinterpret_cast<ItemListNode*>(iterator->field_0x0);
    return reinterpret_cast<void**>(&node->object);
}

#pragma dont_inline reset

extern void __fill_mem(void*, int, int);

bool cf::CfGameManager::isManagerFlag82C() {
    return (*reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(this) + 0x82C) >> 2) & 1;
}

#pragma dont_inline on
extern "C" void linkEffectObjects__Q22cf13CfGameManagerFv(void*, void* first, void* second) { func_80141C6C(first, second); }
#pragma dont_inline reset

#pragma dont_inline on
u32 cf::CfGameManager::isField4ECFlag16() {
    return (field_0x4EC >> 16) & 1;
}

u32 cf::CfGameManager::isField4ECFlag07() {
    return (field_0x4EC >> 7) & 1;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" u32 isStateFlag20Set__Q22cf13CfGameManagerFv(const u8* data) {
    return (*reinterpret_cast<const u32*>(data + 0x4EC) >> 20) & 1;
}
#pragma dont_inline reset

extern "C" void CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv(u8* data) {
    // Slot +0x48 base: clear +0x34. Real API alias is CObjectParam_clearExtraWord;
    // forced-name Unk Fv spelling kept for hand-built vtables.
    *reinterpret_cast<u32*>(data + 0x34) = 0;
}

#pragma dont_inline on
bool cf::CfGameManager::isFlag20Set() {
    return (lbl_eu_80663E24 & 0x20) != 0;
}

bool cf::CfGameManager::isFlag10Set() {
    return (lbl_eu_80663E24 & 0x10) != 0;
}
#pragma dont_inline reset

extern u32 lbl_eu_80663E28;
bool cf::CfGameManager::isFlag010000Set() {
    return (lbl_eu_80663E28 & 0x100000) != 0;
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
extern "C" void CActorParam_UnkVirtualFunc89__Q22cf11CActorParamFv(u8* data, u32 value) {
    *reinterpret_cast<u32*>(data + 0x1608) = value;
}

extern "C" u32 CActorParam_UnkVirtualFunc91__Q22cf11CActorParamFv(const u8* data) {
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
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    lbl_eu_80663E14 = reinterpret_cast<CScn*>(scene);
    cf::CfGameManager* mgr = &lbl_eu_80571758;
    lbl_eu_80663E10 = view;
    __ct__8009D604();
    __dt__801570F0();
    lbl_eu_80663E04 = 0;
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
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    if (!(lbl_eu_80663E24 & 0x10000)) {
        // First-boot path: tear down the battle manager, build the help manager
        // and the layered CProcess graph, then run the full subsystem reset.
        getBattleStateA__Q22cf14CBattleManagerFv();
    __ct__Q22cf12CHelpManagerFv(nullptr);
    CCharVoiceMan_CreateSingleton();
    CProcess* rtRoot = GetRootProcRealTime__12CTaskManagerFv();
    lbl_eu_80663E20 = create__Q22cf9CfPadTaskFP8CProcess(rtRoot, scene);
    CProcess* gameRoot = GetRootProcGame__12CTaskManagerFv();
    lbl_eu_80663E18 = __ct__Q22cf5CfResFv(gameRoot, scene);
    gameRoot = GetRootProcGame__12CTaskManagerFv();
    lbl_eu_80663E1C = __ct__800697E8(gameRoot, scene);
    KyoshinHeap_Init();
    NpcMove_ClearRowBmp();
    gmCallInit1120();
    CfScript_InitMgr();
    func_8007C6C0__Q22cf13CfGameManagerFv(mgr, scene);
    CmText_InitCallbackTable();
    CmText_RegisterCallback(&cf::CfGameManager::tickGameManager, 0);
    lbl_eu_80663E40 = arg;
    // Bare Fv-name call: retail leaves r3 untouched here.
    func_8007DCB8__Q22cf13CfGameManagerFv();
    CtrlObjectParam_MoveValueToHead(CtrlObjectParam_GetSlotTableBase(), lbl_eu_80663E40);
    s32 fallback = -1;
    syncBdatDataCache__Q22cf13CfGameManagerFv(lbl_eu_80663E40, &fallback, 4);
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
    if ((lbl_eu_80663E28 & 0x1000000) != 0) {
        return;
    }
    if (mode != 0 && Scn_GetUnk80Handle(lbl_eu_80663E14) != false && first != 0) {
        CTaskGameCamView* res =
            (CTaskGameCamView*)Scn_QueryUnk80State(lbl_eu_80663E14); // cast: CfObjectMove.hpp's void* form is also in scope
        if (res->field_C <= lbl_eu_8066A208) {
            float temp[4];
            temp[0] = vec->field_0x0;
            temp[1] = vec->field_0x4;
            temp[2] = vec->field_0x8;
            temp[3] = vec->field_0xC;
            temp[3] = lbl_eu_80666498;
            Scn_ReleaseUnk80(lbl_eu_80663E14, 0, temp);
        }
    }
    if (first == 0 || mode != 0) {
        Scn_ReleaseUnk80(lbl_eu_80663E14, first,
                      const_cast<float*>(&vec->field_0x0));
        return;
    }
    // Scale path: truncate the scaled RGBA floats to bytes and push them to
    // the tint block on unkAC.
    float scale = lbl_eu_80666568;
    u8 rgba[4];
    rgba[0] = (u8)(s32)(scale * vec->field_0x0);
    rgba[1] = (u8)(s32)(scale * vec->field_0x4);
    rgba[2] = (u8)(s32)(scale * vec->field_0x8);
    rgba[3] = (u8)(s32)(scale * vec->field_0xC);
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    cf::CfGameManager* mgr = &lbl_eu_80571758;
    if (mgr->unkAC != nullptr) {
        func_80061870(reinterpret_cast<u32>(mgr->unkAC), 0x10, first,
                      *reinterpret_cast<const u32*>(rgba), 0, 0);
    }
}

// cf::CfGameManager::func_8007C6C0 - full init/reset of the singleton state:
// stores the scene pointer, wipes the flag words / Bdat text entries / pad
// slot, then allocates the per-player file streams, camera manager, map-
// effect manager and the remaining subsystems. Declared extern "C" because
// the retail signature carries a hidden r4 scene argument under an Fv name.
extern "C" void func_8007C6C0__Q22cf13CfGameManagerFv(cf::CfGameManager* self, void* scene) {
    lbl_eu_80663E14 = static_cast<CScn*>(scene);
    self->resetManagerState();

    // rlwinm-mask flag clears (retail: two separate rlwinms with the E24
    // oris interleaved; mixed =/&= forms keep the two ANDs distinct)
    lbl_eu_80663E28 = lbl_eu_80663E28 & ~0x200000;
    lbl_eu_80663E24 |= 0x10000;
    lbl_eu_80663E28 &= ~0x8000;
    float zero = lbl_eu_80666498;
    lbl_eu_80663E08 = zero;
    lbl_eu_80663E0C = nullptr;
    self->unk6C = 0;
    self->unk7C = 0;
    lbl_eu_80571628.text[0] = 0;
    lbl_eu_80571628.textLength = 0;
    lbl_eu_80570CF0.field_0x48 = 0;
    lbl_eu_80570CF0.field_0x44 = 0;
    lbl_eu_80663E40 = 0;
    lbl_eu_80663E30 = 0;
    lbl_eu_80663E50 = 0;
    lbl_eu_80663E3A = 0;
    lbl_eu_80663E3C = 0;
    lbl_eu_80663E3E = 0;
    lbl_eu_80663E46 = 0;
    lbl_eu_80663E48[0] = 0;
    lbl_eu_80663E48[1] = 0;
    lbl_eu_80663E4C[0] = 0;
    lbl_eu_80663E4C[1] = 0;
    *reinterpret_cast<u32*>(self->unk80) = 0;
    lbl_eu_80663E00 = 0;
    lbl_eu_80663E34 = 0;
    lbl_eu_80663E35 = 0;
    lbl_eu_80661BCC = -1;
    lbl_eu_80663E5C = 0;
    lbl_eu_80663E58 = zero;
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
    object = memory;
    if (memory != nullptr) {
        object = __ct__cf_CfMapEffectManager(memory);
    }
    self->unkA0 = reinterpret_cast<u32>(object);

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
    lbl_eu_80663E64 = reinterpret_cast<u32>(object);

    heap = CfRes_getAllocHandle();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x39C, heap);
    object = memory;
    if (memory != nullptr) {
        func_80188584(memory);
    }
    lbl_eu_80663E60 = reinterpret_cast<u32>(object);

    heap = CfRes_getAllocHandle();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x1C20, heap);
    object = memory;
    if (memory != nullptr) {
        func_801A9CCC(memory);
    }
    self->unkA8 = static_cast<UnkClass_8007E864*>(object);

    if (lbl_eu_80665958 != nullptr) {
        reinterpret_cast<Unk65958Object*>(lbl_eu_80665958)->field_0x7C =
            CfRes_getAllocHandle();
    }
    CfT_IsTvPal();
    func_80069EA8();
    clearReloadGlobals();
    cfCam_seedFollowD();
}
// cf::CfGameManager::func_8007CF64 - per-frame camera/target-state tick:
// updates the active text-display entry (lbl_eu_805716F8 or lbl_eu_80571658)
// when the player side indicator changes, then clears the 0x8 flag.
extern "C" void func_8007CF64__Q22cf13CfGameManagerFv(cf::CfGameManager* self) {
    if ((lbl_eu_80663E24 & 0x4) != 0 || cf::CfGameManager::isSceneLoading() ||
        (lbl_eu_80663E24 & 0x80) == 0) {
        return;
    }

    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player == nullptr || player->CfObject_isMoveActiveNow() == 0) {
        return;
    }
    if ((lbl_eu_806642E0 & 1) != 0) {
        lbl_eu_80663E68 = lbl_eu_80666498;
        return;
    }
    if ((lbl_eu_80663E28 & 0x800000) != 0) {
        return;
    }

    // side indicator: (E24 bit 3) forced on when the counted players change
    // to/from 4; the direction byte is sign-compressed through the
    // 2 | ~(X-1) arithmetic (retail bit-twiddle, kept verbatim).
    s32 flag3 = (lbl_eu_80663E24 >> 3) & 1;
    u32 count = func_8006A6D0();
    s32 t = count - 1;
    u32 z = (2 | ~t) - ((u32)(2 - t) >> 1);
    s32 direction = z >> 31;
    s32 updated = 0;

    if ((u32)direction != (u32)lbl_eu_80663DFC || flag3 != 0) {
        float value = lbl_eu_80663E68 - CfT_PlayRateGet();
        lbl_eu_80663E68 = value;
        if (value <= lbl_eu_80666498) {
            lbl_eu_80663E68 = lbl_eu_80666498;
            lbl_eu_80663DFC = static_cast<u8>(direction);
            float fade = getReloadDelay();
            if ((lbl_eu_80663E24 & 0x40) != 0) {
                BdatTextEntry* entry = &lbl_eu_805716F8;
                if (entry->textLength != 0) {
                    updated = 1;
                    if (lbl_eu_80663E3E > 0) {
                        if (direction != 0) {
                            MenuSnd_StopSlot0Store_9390(entry);
                        } else {
                            MenuSnd_StopSlot0Store_9390(&entry->secondaryText);
                        }
                    } else if (direction != 0) {
                        UnkTextAreaView* area =
                            reinterpret_cast<UnkTextAreaView*>(entry);
                        func_80188D34(reinterpret_cast<u32>(area),
                                     area->enabled_0x4C, area->value_0x48,
                                     fade);
                    } else {
                        UnkTextAreaView* area = reinterpret_cast<UnkTextAreaView*>(
                            &entry->secondaryText);
                        func_80188D34(reinterpret_cast<u32>(area),
                                     area->enabled_0x4C, area->value_0x48,
                                     fade);
                    }
                    lbl_eu_80663E68 = lbl_eu_806664A4;
                }
            } else {
                BdatTextEntry* entry = &lbl_eu_80571658[0];
                if (entry->textLength != 0) {
                    updated = 1;
                    if (lbl_eu_80663E3E > 0) {
                        if (direction != 0) {
                            MenuSnd_StopSlot0Store_9390(entry);
                        } else {
                            MenuSnd_StopSlot0Store_9390(&entry->secondaryText);
                        }
                    } else if (direction != 0) {
                        UnkTextAreaView* area =
                            reinterpret_cast<UnkTextAreaView*>(entry);
                        func_80188D34(reinterpret_cast<u32>(area),
                                     area->enabled_0x4C, area->value_0x48,
                                     fade);
                    } else {
                        UnkTextAreaView* area = reinterpret_cast<UnkTextAreaView*>(
                            &entry->secondaryText);
                        func_80188D34(reinterpret_cast<u32>(area),
                                     area->enabled_0x4C, area->value_0x48,
                                     fade);
                    }
                    lbl_eu_80663E68 = lbl_eu_806664A4;
                }
            }
            if (updated != 0) {
                lbl_eu_80663E24 &= ~8;
            }
        }
    }
}
void cf::CfGameManager::func_8007D1A0() {}
void cf::CfGameManager::func_8007DCB8() {
    if (!lbl_eu_80663E78) {
        lbl_eu_80663E74 = lbl_eu_80664090;
        lbl_eu_80663E78 = 1;
    }
    void* table = lbl_eu_80663E74;
    char* base = lbl_eu_804FB824;
    for (s32 dataId = 1; dataId <= 13; ++dataId) {
        UnkClass_8009EC9C* data = static_cast<UnkClass_8009EC9C*>(func_8009EC9C(static_cast<u16>(dataId)));
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
            if (value != 0 && lbl_eu_80663E40 != 0 && dataId <= 7) {
                func_80158420(value, &result, 1, &extra);
                CtrlObjectParam_WriteU16RowEntry(data, col, value);
                func_8009DBF4(data, col, reinterpret_cast<void*>(static_cast<s32>(result)));
                active = 1;
            }
            if (active == 0) {
                func_8009DBF4(data, col, reinterpret_cast<void*>(-1));
                CtrlObjectParam_WriteU16RowEntry(data, col, value);
            }
        }
        func_8009EF9C(data, 0);
    }
}
// Retail symbol is Fv but callers pass (row id, mode) in r3/r4 - declared
// extern "C" with a matching body so the Fv name is preserved while the
// hidden args stay ABI-visible.
extern "C" void func_8007E9CC__Q22cf13CfGameManagerFv(u16 value, u32 mode) {}

#pragma dont_inline on
// cf::CfGameManager::func_8007F1FC - party roster refresh. Mirrors the 9
// party-slot ids into a local u16 list, clears the three live object slots,
// then reconciles the passed list against the roster (drop dead rows, insert
// new rows into the slot object). Afterwards averages the per-row item counts
// (rows 1..8, skipping 3) into a "leader" seed, then re-creates / re-slots
// each input row through the item-instance factory.
extern "C" void func_8007F1FC__Q22cf13CfGameManagerFv(void* inList, s32 mode) {
    cf::CtrlObjectParamSlots* party =
        reinterpret_cast<cf::CtrlObjectParamSlots*>(CtrlObjectParam_GetSlotTableBase());
    func_8009E3C0();
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
    // NOTE (W58): the alias does NOT inline here - this function sits
    // inside `#pragma dont_inline on`, which blocks ALL inlining INTO it
    // (repo-proven: MWCC_CASES ResTev::GXSetTevOrder). Decomp emits a `bl`
    // to the alias where retail has the virtual `bctrl`; recorded as a
    // non-pad matching residual (function is 2%-matched for 340 unrelated
    // structural reasons). A future virtual-rename wave (symbols
    // rename-all incl. the hand vtables) collapses it to `bctrl`.
    if (cf::CfGameManager::getInstance()->getActiveCameraObject()) {
        cf::CfGameManager::getInstance()->getActiveCameraObject()->CfObject_syncEnableState();
    }

    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    for (int i = 0; i < 3; ++i) {
        if (lbl_eu_80571758.unk94[i] != 0) {
            // clearPlayerEffect is an Fv-hidden-arg helper whose `this` is the
            // player object (calls gmFileObject(this)) - dispatch via cast.
            reinterpret_cast<cf::CfGameManager*>(lbl_eu_80571758.unk94[i])
                ->clearPlayerEffect();
            lbl_eu_80571758.unk94[i] = 0;
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
                func_8015720C(x, 1);
                func_8007E9CC__Q22cf13CfGameManagerFv(static_cast<u16>(x), 1);
                if (mode) {
                    if (!CtrlObjectParam_InsertSlotValue(party, x, 1)) {
                        CtrlObjectParam_InsertSlotValue(party, x, 2);
                    }
                } else if (i < 3) {
                    func_8009E574(party, x, 1, i);
                } else {
                    func_8009E574(party, x, 2, i - 3);
                }
            }
        }
    }

    // Average the item counts of rows 1..8 (skip 3); leader = rounded avg + K.
    float total = lbl_eu_80666498;
    int count = 0;
    for (int i = 1; i <= 8; ++i) {
        if (i != 3 && isResourceFlagSet__Q22cf13CfGameManagerFv(i)) {
            u16 c = static_cast<u16>(
                CtrlObjectParam_GetArtsDataWord(func_8009EC9C(static_cast<u16>(i))));
            ++count;
            total += U16ToF32(c);
        }
    }
    int leader = 0;
    if (count != 0) {
        float countF = S16ToF32(static_cast<s16>(count));
        float avg = total / countF;
        double k =
            (avg > lbl_eu_80666498) ? lbl_eu_80666518 : lbl_eu_80666520;
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
                    func_8009EC9C(static_cast<u16>(x)));
            if (leader != 0 && x != 0x0b && CtrlRemote_TouchBitByArg(0x3508) == 0) {
                func_800A0860(obj, static_cast<u16>(leader));
                func_8009EF9C(obj, 0);
                func_800A21F8(obj, 0, lbl_eu_805276F0[static_cast<u16>(x)], 0);
                if (x >= 3 && x <= 8) {
                    u16 c = static_cast<u16>(CtrlObjectParam_GetArtsDataWord(obj));
                    func_800A2974(obj,
                                  static_cast<u16>(c + seed + itembase - 1));
                }
            } else {
                u16 c = static_cast<u16>(CtrlObjectParam_GetArtsDataWord(obj));
                if (x == 3) {
                    UnkClass_8009EC9C* obj8 =
                        reinterpret_cast<UnkClass_8009EC9C*>(func_8009EC9C(8));
                    u16 c8 = static_cast<u16>(CtrlObjectParam_GetArtsDataWord(obj8));
                    if (c < c8) {
                        c = c8;
                    }
                }
                if (x >= 3 && x <= 8) {
                    func_800A2974(obj,
                                  static_cast<u16>(itembase + seed + c - 1));
                }
            }
            if (x == 8) {
                UnkClass_8009EC9C* obj3 =
                    reinterpret_cast<UnkClass_8009EC9C*>(func_8009EC9C(3));
                KizunagramSetActiveSlot(&obj->itemCounts_0x3534,
                              *reinterpret_cast<u32*>(
                                  reinterpret_cast<u8*>(obj3) + 0x3DD0));
                // Slot owner: CActorParam is embedded at +0x17C (vtable) -
                // CtrlObjectParam.hpp; 0x200 is getSecondCurrency (() -> u32),
                // 0x1F8 is setSecondCurrency (u32). Both called via the
                // inline behavior aliases (same arity; MWCC inlines them
                // into the identical virtual dispatch outside dont_inline
                // regions - cf. resetBattleGauge's inlined syncEnableState).
                // NOTE (W58): same dont_inline caveat as the 0x5C site
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
        func_8009E838(reinterpret_cast<u8*>(party));
    }
}
#pragma dont_inline reset
void cf::CfGameManager::func_8007F9C4() {}
void cf::CfGameManager::func_80080888() {}
void cf::CfGameManager::func_8008413C() {}
void cf::CfGameManager::func_80084654() {}
// cf::CfGameManager::resetBattlePresentation - in-battle presentation reset: on the
// E24 bit-16 "title/battle setup" flag, resets the battle manager
// (party-gauge + 0x94 payload), clears the presentation masks, reloads the
// bdat event table, then either kicks the event-video UI or re-populates the
// file-event table through CtrlObjectParam_GetSlotTableBase / CtrlRemote_GetFileEventIds.
void cf::CfGameManager::resetBattlePresentation() {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    if ((lbl_eu_80663E24 & 0x10000) == 0) {
        return;
    }
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    cf::CfGameManager* manager = &lbl_eu_80571758;
    // Only the last getInstance result is kept in a saved register (live
    // across the memset); the first two are consumed in scratch.
    if (getInstance__Q22cf14CBattleManagerFv() != nullptr) {
        PartyGaugeSetClamped((u8*)&((cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv())->unk194, 0);
        cf::CBattleManager* battle = (cf::CBattleManager*)getInstance__Q22cf14CBattleManagerFv();
        memset(&battle->unk94, 0, 0x100);
        battle->setPartyMaskFlag(2, 0);
    }
    MenuStateInitFlags();
    manager->func_8007D84C();
    lbl_eu_80663E24 &= ~0x400000;
    lbl_eu_80663E28 &= ~0x100000;
    lbl_eu_80570CF0.field_0x48 = 0;
    lbl_eu_80570CF0.field_0x44 = 0;
    lbl_eu_80663ED8 = lbl_eu_80666498;
    lbl_eu_80663EDC = lbl_eu_80666498;
    func_802062BC();
    evtTeardownActive();
    func_802959AC(lbl_eu_80664A10);
    CCharVoiceMan_ClearFieldFlag();
    if (func_8023C1C0() != 0) {
        if (!lbl_eu_80663E70) {
            __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
            __register_global_object(&lbl_eu_80571758,
                                     __dt__Q22cf13CfGameManagerFv,
                                     lbl_eu_80571748);
            lbl_eu_80663E70 = 1;
        }
        if (lbl_eu_80571758.unkAC != nullptr) {
            func_80061870(reinterpret_cast<u32>(lbl_eu_80571758.unkAC), 0x1E, 0, 0, 0, 0);
        }
    } else {
        for (s32 i = 1; i <= 13; ++i) {
            CtrlObjectParam_ActivateCharRow(func_8009EC9C(static_cast<u16>(i)));
        }
        __dt__8023E448();
        CItem_rerankAllKinds();
        if (lbl_eu_80664774 != 0) {
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
        UnkC1B4Data* video = reinterpret_cast<UnkC1B4Data*>(func_8023C1B4());
        lbl_eu_80663E40 = (u16)CtrlObjectParam_GetSlotTableBase()[1];
        CtrlObjectParam_MoveValueToHead(CtrlObjectParam_GetSlotTableBase(), lbl_eu_80663E40);
        CfFileEventIdsView* ids = CtrlRemote_GetFileEventIds();
        queueSceneEventA__Q22cf13CfGameManagerFv(
            ids->field_0x2, ids->field_0x0, reinterpret_cast<u32>(video),
            reinterpret_cast<u32>(ids->field_0x4), video->value_0xC);
        u16 eventDone = lbl_eu_80664774;
        lbl_eu_80663E28 =
            (eventDone != 0) ? (lbl_eu_80663E28 | 0x8000)
                             : (lbl_eu_80663E28 & ~0x8000);
        lbl_eu_80664774 = 0;
    }
    lbl_eu_80663E24 |= 0x100000;
    lbl_eu_80663E28 |= 0x20;
}
// cf::CfGameManager::func_800853C8 - title/menu-reset hook: on the
// 0x40000 flag it resets volume/fade state and the bdat text area, then
// walks the item-list refreshing every object; the 0x40000-clear path
// just re-runs the list walk after an explicit event-tear-down.
extern "C" void func_800853C8__Q22cf13CfGameManagerFv() {
    UnkArea70CE0* area = reinterpret_cast<UnkArea70CE0*>(lbl_eu_80570CE0);
    if ((lbl_eu_80663E24 & 0x40000) != 0) {
        if (!lbl_eu_80663E70) {
            __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
            __register_global_object(&lbl_eu_80571758,
                                     __dt__Q22cf13CfGameManagerFv,
                                     lbl_eu_80571748);
            lbl_eu_80663E70 = 1;
        }
        func_8018986C(0, lbl_eu_80666498);
        MenuSnd_StopSlot0Clear_9318(1, lbl_eu_80666498);
        MenuSnd_StopSlot1_9424(lbl_eu_80666498);
        CfSoundMan_StopMaskedSlots(0x20, 0xf);
        func_8013D26C(0);
        CfScript_SleepReq2();
        if (lbl_eu_80571758.field_0x88 != -1) {
            UnkFloat4 value;
            value.field_0x0 = lbl_eu_80666498;
            value.field_0x4 = lbl_eu_80666498;
            value.field_0x8 = lbl_eu_80666498;
            value.field_0xC = lbl_eu_8066649C;
            func_8008566C__Q22cf13CfGameManagerFv(0xf, &value, 1);
            lbl_eu_80663E24 |= 0x100;
        }
        if (!lbl_eu_80663E70) {
            __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
            __register_global_object(&lbl_eu_80571758,
                                     __dt__Q22cf13CfGameManagerFv,
                                     lbl_eu_80571748);
            lbl_eu_80663E70 = 1;
        }
        if (lbl_eu_80571758.unkAC != nullptr) {
            func_80061870(reinterpret_cast<u32>(lbl_eu_80571758.unkAC), 6, 2, 0, 0, 0);
        }
        if (!lbl_eu_80663E70) {
            __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
            __register_global_object(&lbl_eu_80571758,
                                     __dt__Q22cf13CfGameManagerFv,
                                     lbl_eu_80571748);
            lbl_eu_80663E70 = 1;
        }
        if (lbl_eu_80571758.unkAC != nullptr) {
            func_80061870(reinterpret_cast<u32>(lbl_eu_80571758.unkAC), 0x12, 0, 0, 0, 0);
        }
        Scn_SetPalFixFlag(lbl_eu_80663E14, 0);
    } else if ((lbl_eu_80663E24 & 0x2000000) != 0 ||
               (lbl_eu_80663E24 & 0x400) != 0) {
        evtUpdateSequenceKick();
    }

    lbl_eu_80663E28 |= 0x100000;
    lbl_eu_806642E0 &= ~0x8000;
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
            setGimmickActive(object, true);
            refreshGimmickObject(object);
        }
        node = node->next;
    }
    lbl_eu_80663E28 &= ~8;
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
    bool gateFunc10 = (lbl_eu_80663E24 & 0x400000) != 0;
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    // Second E24 read feeds the update; masked form computed unconditionally,
    // overridden on the set path.
    u32 e24Now = lbl_eu_80663E24;
    u32 e24Next = e24Now & ~0x00400000;
    if (param != 0) {
        e24Next = e24Now | 0x00400000;
    }
    lbl_eu_80663E24 = e24Next;
    if (param != 0) {
        // Enter path.
        if (gateFunc10) {
            CfRes_callFunc_67F10(0);
        }
        setInputDisableTime__Q22cf9CfPadTaskFf(lbl_eu_80666548);
        CPad* pad = lbl_eu_80663E0C;
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
            if (!lbl_eu_80663E70) {
                __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
                __register_global_object(&lbl_eu_80571758,
                                         __dt__Q22cf13CfGameManagerFv,
                                         lbl_eu_80571748);
                lbl_eu_80663E70 = 1;
            }
            if (lbl_eu_80571758.unkAC != nullptr) {
                func_80061870(reinterpret_cast<u32>(lbl_eu_80571758.unkAC), 6, 4,
                              0, 0, 0);
            }
            if (!lbl_eu_80663E70) {
                __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
                __register_global_object(&lbl_eu_80571758,
                                         __dt__Q22cf13CfGameManagerFv,
                                         lbl_eu_80571748);
                lbl_eu_80663E70 = 1;
            }
            if (lbl_eu_80571758.unkAC != nullptr) {
                func_80061870(reinterpret_cast<u32>(lbl_eu_80571758.unkAC), 0x20,
                              0, 0, 0, 0);
            }
        }
        bindPadSubobjects(1);
        if ((lbl_eu_80663E28 & 0x08000000) != 0) {
            if (!lbl_eu_80663E70) {
                __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
                __register_global_object(&lbl_eu_80571758,
                                         __dt__Q22cf13CfGameManagerFv,
                                         lbl_eu_80571748);
                lbl_eu_80663E70 = 1;
            }
            if (lbl_eu_80571758.unkAC != nullptr) {
                func_80061870(reinterpret_cast<u32>(lbl_eu_80571758.unkAC), 0x1f,
                              0, 0, 0, 0);
            }
            lbl_eu_80663E28 &= ~0x08000000;
        }
    } else {
        // Reset path.
        if (MiniMapHasGlobalData() != 0) {
            MiniMapResetLayoutAnims();
        }
        func_8012F87C(0);
        lbl_eu_80663E08 = lbl_eu_80666498;
        if (gateFunc10) {
            CfRes_callFunc_67F10(0);
        }
        bindPadSubobjects(0);
        if (!lbl_eu_80663E70) {
            __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
            __register_global_object(&lbl_eu_80571758,
                                     __dt__Q22cf13CfGameManagerFv,
                                     lbl_eu_80571748);
            lbl_eu_80663E70 = 1;
        }
        if (lbl_eu_80571758.unkAC != nullptr) {
            func_80061D2C(reinterpret_cast<UnkClass_80085334*>(
                              lbl_eu_80571758.unkAC),
                          0x20);
        }
        if (!lbl_eu_80663E70) {
            __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
            __register_global_object(&lbl_eu_80571758,
                                     __dt__Q22cf13CfGameManagerFv,
                                     lbl_eu_80571748);
            lbl_eu_80663E70 = 1;
        }
        if (lbl_eu_80571758.unkAC != nullptr) {
            func_80061D2C(reinterpret_cast<UnkClass_80085334*>(
                              lbl_eu_80571758.unkAC),
                          0x15);
        }
        if ((lbl_eu_80663E24 & 0x200) != 0) {
            lbl_eu_80663E24 &= ~0x200;
        }
        if ((lbl_eu_80663E28 & 0x800) != 0) {
            lbl_eu_80663E28 &= ~0x800;
        }
        for (s32 i = 1; i <= 13; ++i) {
            CtrlObjectParam_ActivateCharRow(func_8009EC9C(static_cast<u16>(i)));
        }

        // Item-list sweep through func_800AD860's flag view. Retail table
        // lbl_eu_80528600 (cf::CfObjectColl) has copyCollPosition at +0xB8;
        // call through the Coll tree (macro-included above) so MWCC emits a
        // no-arg bctrl (main-tree applyMoveOffset(vec,float) would load r4/f1).
        Func800B6BECList* list = (Func800B6BECList*)getReslistB48();
        Func800B6BECNode* node = list->head->next;
        while (node != list->head) {
            cf::CfObjectColl* obj =
                static_cast<cf::CfObjectColl*>(func_800AD860(node->object));
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
    if ((lbl_eu_80663E24 & 0x80000) != 0) {
        if (!lbl_eu_80663E70) {
            __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
            __register_global_object(&lbl_eu_80571758,
                                     __dt__Q22cf13CfGameManagerFv,
                                     lbl_eu_80571748);
            lbl_eu_80663E70 = 1;
        }
        if (param != 0) {
            if (lbl_eu_80571758.unkAC != nullptr) {
                func_80061D2C(reinterpret_cast<UnkClass_80085334*>(
                                  lbl_eu_80571758.unkAC),
                              0x1c);
            }
        } else {
            if (lbl_eu_80571758.unkAC != nullptr) {
                func_80061870(reinterpret_cast<u32>(lbl_eu_80571758.unkAC), 0x1c,
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
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    cf::CfGameManager* mgr = &lbl_eu_80571758;

    // 0xFFA00000 - 0x231: keep-everything-except mask applied to all pads.
    u32 mask = lbl_eu_80663DF8 & 0xFF9FFDCF;
    lbl_eu_80663DF8 = mask;
    CPad* pad = lbl_eu_80663E0C;
    if (pad != nullptr) {
        pad->mPressedButtonFlags &= mask;
        pad->mTurboPressButtonFlags &= mask;
        pad->mReleasedButtonFlags &= mask;
        pad->mHeldButtonFlags &= mask;
        pad->mLongHoldButtonFlags &= mask;
        pad->mShortPressButtonFlags &= mask;
        lbl_eu_80571500.mPressedButtonFlags &= mask;
        lbl_eu_80571500.mTurboPressButtonFlags &= mask;
    }
    CfRes_callFunc_67FE0();
    CamEvtClearSlotBytes(mgr->unkB4);
    func_8007B0A0(0);
    func_801AAD08();
    lbl_eu_8066443A = 0;
    getInstance__Fv();
    walkReslistByMask(nullptr, 0x10000);
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    lbl_eu_80663E24 &= 0xFFFB7FFF;

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
    func_800B6800(lbl_eu_80666498, 0, 0);
    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (!lookupWorkAtAddr()) {
        player = nullptr;
    }
    if ((lbl_eu_80663E28 & 0x20000000) == 0) {
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
    func_800B06C8();
    func_80186C7C(reinterpret_cast<void*>(1));
    if ((lbl_eu_80663E28 & 0x20000000) == 0) {
        ItemListManager* list = getListB28__Fv();
        ItemListNode* node = list->sentinel->next;
        while (node != list->sentinel) {
            Unk80EE4Data* object = static_cast<Unk80EE4Data*>(
                getCfObjectPc__FPQ22cf12CfObjectMove(node->object));
            if (object != nullptr) {
                setGimmickActive(object, true);
                refreshGimmickObject(object);
            }
            node = node->next;
        }
        // rlwinm mb=29,me=27 wrap mask: everything except bit 28.
        lbl_eu_80663E28 &= ~0x10000000;
    }
    // rlwinm mb=3,me=1 wrap mask: everything except bit 2.
    lbl_eu_80663E28 &= ~0x20000000;
}
// cf::CfGameManager::func_80086778 - one-time bdat table scan (guarded by the
// E35 flag). Loads the current bdat table (lbl_eu_806640A0), walks a row range,
// tallies rows by the FD column byte, and for rows whose B3-column byte matches
// lbl_eu_80664184 spawns collision objects via createBdatCollisionObj and positions them
// with the func_800AC1xx collision helpers.
void cf::CfGameManager::func_80086778() {
    if (lbl_eu_80663E35 != 0) {
        return;
    }
    Bdat_GetTable_AA34();
    void* bdat = lbl_eu_806640A0;
    int rowStart = (int)Bdat_GetRowBase_B41C(bdat);
    int rowCount = (int)Bdat_GetMaxRow_B1EC(bdat);
    lbl_eu_80663E35 = 1;
    lbl_eu_80663E36 = 0;
    lbl_eu_80663E38 = 0;
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
    const float threshold = lbl_eu_8066A208;
    const float scale = lbl_eu_8066A210;

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
            ++lbl_eu_80663E36;
        } else if (v == 1) {
            ++lbl_eu_80663E38;
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
        // gmSpawnMask8000's object is a collision object (func_800AC110/AC1BC
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
            func_800AC110(obj, &v48, &v3c, fD);
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
            CollObjSetupRotBoxState4Timed(obj, &v48, &v30, radius);
        }
        CollObjPackResIdWords(obj, (u32)row, code);
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
        reinterpret_cast<UnkClass_8009EC9C*>(func_8009EC9C(value));
    Unk812Data* data = reinterpret_cast<Unk812Data*>(resource);
    clearResourceStack__Q22cf13CfGameManagerFv(reinterpret_cast<UnkReset28Data*>(table));
    if (mode != 0) {
        setResourceField04__Q22cf13CfGameManagerFv(data, (u32)func_8009E120(table, 0));
        setResourceField08__Q22cf13CfGameManagerFv(data, (u32)func_8009E120(table, 1));
        setResourceField10__Q22cf13CfGameManagerFv(data, (u32)func_8009E120(table, 2));
        setResourceField0C__Q22cf13CfGameManagerFv(data, (u32)func_8009E120(table, 3));
        setResourceField14__Q22cf13CfGameManagerFv(data, (u32)func_8009E120(table, 4));
    } else {
        setResourceField04__Q22cf13CfGameManagerFv(
            data, func_80141E90(value, (s16)getResourceField0E__Q22cf13CfGameManagerFv(
                                       reinterpret_cast<Unk812Data*>(table)),
                                 1, 0));
        setResourceField08__Q22cf13CfGameManagerFv(
            data,
            func_80141E90(value, (s16)getResourceWord10__Q22cf13CfGameManagerFv(reinterpret_cast<const u8*>(table)),
                          2, 0));
        setResourceField10__Q22cf13CfGameManagerFv(
            data,
            func_80141E90(value, (s16)getResourceWord12__Q22cf13CfGameManagerFv(reinterpret_cast<const u8*>(table)),
                          3, 0));
        setResourceField0C__Q22cf13CfGameManagerFv(
            data,
            func_80141E90(value, (s16)getResourceWord14__Q22cf13CfGameManagerFv(reinterpret_cast<const u8*>(table)),
                          4, 0));
        setResourceField14__Q22cf13CfGameManagerFv(
            data,
            func_80141E90(value, (s16)getResourceWord16__Q22cf13CfGameManagerFv(reinterpret_cast<const u8*>(table)),
                          5, 0));
    }

    void* handle = const_cast<void*>(func_8009E120(table, 5));
    u32 aFull = CItem_getNameIdFromFam(getResourceField0C_2__Q22cf13CfGameManagerFv(data));
    u32 kind = (u32)(u16)func_80142154((u32)(u16)aFull, 1);
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
        if (CBattleMan_FetchVisionObj(nullptr) != nullptr) {
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
    lbl_eu_80663E30 = 0;
    CfRes_callFunc_67E78(0);
    CfRes_callFunc_67FE0();
    enableGameFlag__Q22cf13CfGameManagerFv(4);
    disableGameFlag__Q22cf13CfGameManagerFv(0x80);
    queueCameraRequest__Q22cf13CfGameManagerFv(0xb, first, second, 0, 0);
    copyVec3Words(reinterpret_cast<ml::CVec3*>(&mgr->field_0xC),
                  reinterpret_cast<const ml::CVec3*>(first));

    // Fade-factor bookkeeping: zero the halfword mirrors, compare the incoming
    // factor against the shared threshold and stash it in the manager.
    float threshold = lbl_eu_80666560;
    lbl_eu_80663E48[0] = 0;
    lbl_eu_80663E4C[0] = 0;
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
    copyBdatTextEntryFull__Q22cf13CfGameManagerFv(&lbl_eu_80571658[0],
                                           &lbl_eu_80571658[1]);
    cfCam_syncFollowD();
}
