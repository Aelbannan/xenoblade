#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include "kyoshin/cf/CfPadTask.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
// CActorParam.hpp pulls CBattleState.hpp, which declares
// getBdatStringColumnValue(void*, const char*, int); the unity-helpers header
// (included later) declares the canonical u32(void*, const char*, s32) form and
// MWCC forbids overloading an extern "C" function. Rename the CBattleState copy
// away; this TU uses the unity-helpers declaration.
#define getBdatStringColumnValue cbattleStateGetBdatStringColumnValue
#include "kyoshin/cf/object/CActorParam.hpp"
#undef getBdatStringColumnValue
#include "kyoshin/cf/object/CObjectParam.hpp"
// CfObjectMove.hpp declares func_80081900__Q22cf13CfGameManagerFv as void* and
// getPlayer__Q22cf13CfGameManagerFi as void*(int) to avoid pulling the
// unity-helpers header; CfGameManagerUnityHelpers.hpp (included later) declares
// them with proper return types (Unk8187CData* / CfObjectMove*(s32)). extern
// "C" redeclaration with a different return type is illegal, so rename
// CfObjectMove.hpp's copies away.
#define func_80081900__Q22cf13CfGameManagerFv cfObjectMoveFunc_80081900
#define getPlayer__Q22cf13CfGameManagerFi cfObjectMoveGetPlayer
#include "kyoshin/cf/object/CfObjectMove.hpp"
#undef func_80081900__Q22cf13CfGameManagerFv
#undef getPlayer__Q22cf13CfGameManagerFi
// CfObjectMap.hpp's import block declares func_80496288 as float(CScn*),
// func_800AA33C as int(char*,...) and getBdatStringColumnValue with s32,
// clashing with the proper extern "C" versions already in scope from
// CfObjectMove.hpp / IResInfo.hpp / CActorParam.hpp#CBattleState.hpp (MWCC
// forbids overloading an extern "C" function). Rename CfObjectMap.hpp's
// copies out of the way; this TU uses the properly-typed declarations from
// those headers.
#define func_80496288 cfObjectMapFunc_80496288
#define func_800AA33C cfObjectMapFunc_800AA33C
#define getBdatStringColumnValue cfObjectMapGetBdatStringColumnValue
#include "kyoshin/cf/object/CfObjectMap.hpp"
#undef func_80496288
#undef func_800AA33C
#undef getBdatStringColumnValue
#include "kyoshin/cf/voice/CCharVoice.hpp"
#include "kyoshin/code_801862C0.hpp"

#include "monolib/core/CPadManager.hpp"
#include "monolib/core/CView.hpp"
#include "monolib/math/CVec3.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/FixStr.hpp"
#include <string.h>

extern "C" CScn* lbl_eu_80663E14;
extern "C" void func_800B9404(void* object);
// TU-local decl: func_8007EEF0 is a tail-call thunk (addi r3,r3,0x1c; b func_8009D790)
// that passes r4 through, so the 1-arg form is intentional here. The retail
// 2-arg form (data + index) is declared in pluginCfs.cpp.
extern "C" void func_8009D790(CfGameManagerData1C* data);

// --- imports for the func_8007xxxx / func_8008xxxx target bodies ---
// (retail C-linkage names; signature params are informational, the emitted
// reloc symbol is the unmangled name)
extern "C" void func_801414CC__Q22cf6CfBdatFv();
extern "C" void* __ct__cf_CfTFile(void* self);
extern "C" void* __ct__80075298(void* self);
extern "C" void* __ct__cf_CfMapEffectManager(void* self);
extern "C" void* __ct__80186578(void* self);
extern "C" void* __ct__801BF76C(void* self);
extern "C" void func_80188584(void* self);
extern "C" void func_801A9CCC(void* self);
extern "C" void func_80069EE0();
extern "C" void func_80069EA8();
extern "C" void func_8016E13C();
extern "C" void func_8006CC0C();
extern "C" float func_8016E9CC();
extern "C" void func_80189390(const void* text);
extern "C" void func_8006A03C(u32 first, u32 second, u32 third);
extern "C" void func_80068AA4();
extern "C" void func_801BF9A4();
extern "C" int func_800426F0__9CTaskGameFv();
extern "C" void func_800C1EB8();
extern "C" void func_eu_80053FC8();
extern "C" void func_8007BAFC(CfCamEventManager* manager);
extern "C" void func_80189450();
extern "C" void func_8018986C(int value, float first);
extern "C" void func_80189318(int value, float first);
extern "C" void func_801C0118(u32 first, u32 second);
extern "C" void func_8013D26C(u32 value);
extern "C" void func_80068C38();
extern "C" void func_80164DB8();
extern "C" void func_801896A8(s32 index, float f1, float f2);
extern "C" void func_eu_8049AB50(void* scene, u32 value);

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
    func_800B9404(unk90);
    unk90 = nullptr;
    if (unkA8 != nullptr) {
        func_801A9FC0(unkA8);
    }
    if (unkA0 != 0) {
        func_8016EEB0(unkA0);
    }
    if (field_0xA4 != nullptr) {
        func_80186664(field_0xA4);
    }
    if (unkB4 != nullptr) {
        func_800754C0(unkB4);
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

// Bdat-defined object returned by func_80081CB8 / func_800B9548 (func_80086778):
// only the u32 field at 0x64 and the u16 flag at 0x158 are read/written, plus a
// virtual dispatch through vtable slot 0x158.
struct BdatObjView {
    u8 pad_00[0x64];
    u32 field_0x64;          // 0x64
    u8 pad_68[0x158 - 0x68];
    u16 field_0x158;         // 0x158
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
cf::CfObjectMove** func_8007C6B4__Q22cf13CfGameManagerFv(cf::CfObjectMove** slots, int index);

void __ct__Q22cf13CfGameManagerFv(cf::CfGameManager* self);

void* __register_global_object(void* object, void* destructor, void* registration);
void func_80189424(float value);
}

extern "C" u32 func_8009CF8C(u32 resourceId);
extern void func_801BFFAC(float value, float fadeTime);
extern const float lbl_eu_8066A208;
extern const float lbl_eu_8066650C;
extern const float lbl_eu_80666510;
extern const float lbl_eu_80666548;

#pragma dont_inline on
unsigned int func_8007DCA8(unsigned int first, unsigned int second) {
    return (first << 27) | (second << 20);
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void func_8007E4CC__Q22cf13CfGameManagerFv(u32 mask) {
    lbl_eu_80663E24 &= ~mask;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" u32 func_8007EEE0__Q22cf13CfGameManagerFv(const UnkEEE0Data* data, u16 index) {
    return data->entries_0x2[index];
}

extern "C" u32 func_8007F044__Q22cf13CfGameManagerFv(const UnkF044Data* data, u32 index) {
    return data->field_0x888[index];
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void func_8007F8C0__Q22cf13CfGameManagerFv(UnkF8C0Node* destination,
                                                        const UnkF8C0Source* source) {
    destination->field_0x0 = source->field_0x4->field_0x0;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void func_80081CA0__Q22cf13CfGameManagerFv(Unk81CA0Data* data, u16 index,
                                                        u32 offset) {
    u32 value = index * 16;
    value += offset;
    data->field_0x6D0 = value;
}
#pragma dont_inline reset

#pragma dont_inline on
bool func_8007CBD4(u32 mask) {
    return (lbl_eu_80663E24 & mask) != 0;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" bool func_8007F0AC__Q22cf13CfGameManagerFv(const UnkF0ACData* data) {
    return data->flag_0x4 != 0;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" bool func_80082FCC__Q22cf13CfGameManagerFv(const cf::CfObject* data, u32 mask) {
    return (data->mFlags68 & mask) != 0;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" bool func_8007F900__Q22cf13CfGameManagerFv(const u32* first, const u32* second) {
    return *first != *second;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void func_8007F990__Q22cf13CfGameManagerFv(u32 mask, bool enable) {
    if (enable) {
        lbl_eu_80663E28 |= mask;
    } else {
        lbl_eu_80663E28 &= ~mask;
    }
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void func_80082544__Q22cf13CfGameManagerFv(s32 minimum, s32* value,
                                                        s32 maximum) {
    if (*value < minimum) {
        *value = minimum;
    } else if (*value > maximum) {
        *value = maximum;
    }
}
#pragma dont_inline reset

// The func_80083D50 wrapper body is visible in UnityHelpers and -inline auto
// would inline it at call sites, but retail calls it as a real function
// (bl func_80083D50__Q22cf13CfGameManagerFv in func_80084F50/
// func_80085FB8), so rename the visible definition away and import the
// retail symbol directly (declared in CfGameManager.hpp).
#define func_80083D50__Q22cf13CfGameManagerFv unityHelpersWrapFunc_80083D50
#define func_800827E4__Q22cf13CfGameManagerFv unityHelpersWrapFunc_800827E4
#include "kyoshin/cf/CfGameManagerUnityHelpers.hpp"
#undef func_80083D50__Q22cf13CfGameManagerFv
#undef func_800827E4__Q22cf13CfGameManagerFv

namespace cf {

void CfGameManager::func_8007C140() {
    if (this == nullptr) {
        lbl_eu_80663E24 |= 8;
    }

    lbl_eu_80663E28 &= ~8;
    lbl_eu_80663E3C = 0;
    lbl_eu_80663E68 = lbl_eu_80666498;
    lbl_eu_80663E5C = 0;
    lbl_eu_80663E58 = lbl_eu_80666498;
    lbl_eu_80661BCC = -1;
    func_80189424(lbl_eu_8066649C);
}

void CfGameManager::func_8007C0F8() {
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
    return *func_8007C6B4__Q22cf13CfGameManagerFv(getInstance()->unk94, playerIndex);
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
extern "C" cf::CfObjectMove** func_8007C6B4__Q22cf13CfGameManagerFv(cf::CfObjectMove** slots,
                                                                              int index) {
    return &slots[index];
}
#pragma dont_inline off

#pragma dont_inline on
extern "C" void func_8007C188__Q22cf13CfGameManagerFv(u32 flags) {
    lbl_eu_80663E24 |= flags;
}
#pragma dont_inline reset

extern "C" void func_8007D190__Q22cf13CfGameManagerFv(u32 flags) {
    lbl_eu_80663E24 &= ~flags;
}

#pragma dont_inline on
extern "C" void func_8007D794__Q22cf13CfGameManagerFv(Unk87588Data* data) {
    data->field_0x0 = 0;
    data->field_0x20 = 0;
}
#pragma dont_inline reset

#pragma dont_inline on
bool cf::CfGameManager::func_8007E1B4() {
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    return (lbl_eu_80663E24 & 0x10000) != 0;
}
#pragma dont_inline reset
// cf::CfGameManager::func_8007E218 - full teardown of the CfGameManager
// singleton: mirrors the destructor's subsystem detach/delete sequence, then
// resets the global state and removes the layered CProcess hooks. The
// vtable+0x08 dispatch with delete flag 1 is MWCC's delete-on-cast-dtor
// shape (the if + delete-expansion null guards produce the paired beq; see
// CfGameManager.hpp CfVt08Dtor).
void cf::CfGameManager::func_8007E218() {
    lbl_eu_80663E28 |= 0x200000;
    func_8012F87C(0);
    func_80135568(0);
    func_802A1DA8();
    func_800D91D0__Q22cf14CBattleManagerFv();
    func_80295924();
    __dt__80157150();
    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    cf::CfGameManager* manager = &lbl_eu_80571758;
    func_800B9404(manager->unk90);
    manager->unk90 = nullptr;
    if (manager->unkA8 != nullptr) {
        func_801A9FC0(manager->unkA8);
    }
    if (manager->unkA0 != 0) {
        func_8016EEB0(manager->unkA0);
    }
    if (manager->field_0xA4 != nullptr) {
        func_80186664(manager->field_0xA4);
    }
    if (manager->unkB4 != nullptr) {
        func_800754C0(manager->unkB4);
    }
    manager->unkB0 = nullptr;
    if (manager->unkA8 != nullptr) {
        __dt__801A9F78(manager->unkA8, 1);
        manager->unkA8 = nullptr;
    }
    if (manager->unkA0 != 0) {
        delete reinterpret_cast<CfVt08Dtor*>(manager->unkA0);
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
    func_800B92D8();
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
    func_80068AC8();
    func_800FE68C();
    __dt__800FDEF8();
    func_8007C5B8__Q22cf13CfGameManagerFv(manager);
    func_8009CE88();
    // Player-file slots at the top of the manager (offset 0x00): cancel the
    // in-flight file read, then release the object through its vtable+0x08
    // destructor and null the slot (retail's reload + paired null tests come
    // from the library call potentially clearing the slot).
    CfGameManagerSlotArray* slots = reinterpret_cast<CfGameManagerSlotArray*>(manager);
    for (u32 i = 0; i < 3; ++i) {
        if (slots->slots[i] != nullptr) {
            func_80069A18(slots->slots[i]);
            if (slots->slots[i] != nullptr) {
                delete reinterpret_cast<CfVt08Dtor*>(slots->slots[i]);
                slots->slots[i] = nullptr;
            }
        }
    }
    if (manager->unkAC != nullptr) {
        __dt__Q22cf17UnkClass_8018EF3CFv(manager->unkAC, 1);
        manager->unkAC = nullptr;
    }
    lbl_eu_80663E04 = 0;
    CfRes_callFunc_67E78(func_801BF93C__Fv());
    func_800620F0();
    func_800A7D9C();
    if (lbl_eu_80663E1C != nullptr) {
        Remove__8CProcessFv(lbl_eu_80663E1C);
        lbl_eu_80663E1C = nullptr;
    }
    if (lbl_eu_80663E18 != nullptr) {
        Remove__8CProcessFv(lbl_eu_80663E18);
        lbl_eu_80663E18 = nullptr;
    }
    func_8003AA8C__5CBdatFUl(1);
    if (lbl_eu_80663E20 != nullptr) {
        Remove__8CProcessFv(lbl_eu_80663E20);
        lbl_eu_80663E20 = nullptr;
    }
}

#pragma dont_inline on
UnkClass_80083298* cf::CfGameManager::func_80083298() {
    return getInstance()->unk90;
}
#pragma dont_inline reset
extern "C" void func_eu_800874CC(){ func_800B76CC(); }
struct VoiceSource;

extern "C" void func_eu_800874D0()
{
    func_800B76F4();
}

extern "C" void func_eu_800874D4(void){
    func_800B781C();
}

extern "C" VoiceSource* func_eu_800874D8(VoiceSource* source)
{
    return func_800B7854(source);
}
void cf::CfGameManager::func_80086B5C(int arg1, int arg2, int arg3) {}
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
    if (func_800829B8()) {
        if (func_800FF778__9CMainMenuFv() || getInstance__11CSysWinBuffFv() != nullptr ||
            isInitialized__10CMenuPauseFv()) {
            return 0xFFFFFFFF;
        }
    }
    return lbl_eu_80663DF8;
}
bool cf::CfGameManager::func_80086F9C() {
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
extern "C" void func_80087588(Unk87588Data* data) {
    data->field_0x0 = 0;
    data->field_0x20 = 0;
    data->field_0x24 = 0;
    data->field_0x44 = 0;
    data->field_0x48 = lbl_eu_80666548;
}
extern u16 lbl_eu_80664772;
bool cf::CfGameManager::func_80085840() {
    if (lbl_eu_80663E14 == nullptr) {
        return true;
    }
    return func_80496044(lbl_eu_80663E14);
}

bool cf::CfGameManager::func_8008585C() {
    if (lbl_eu_80663E14 == nullptr) {
        return false;
    }
    return func_804960A8(lbl_eu_80663E14);
}

bool cf::CfGameManager::func_80084B68() {
    if ((lbl_eu_80663E24 & 0x8000) != 0 && !func_80068E44(0x20)) {
        return false;
    }
    return func_800B45A0();
}

bool cf::CfGameManager::func_80084BAC() {
    if ((lbl_eu_80663E24 & 0x48000) != 0) {
        if (func_80068E44(0x20)) {
            return true;
        }
    }
    return false;
}

bool cf::CfGameManager::func_80084BF4() {
    return (lbl_eu_80663E24 & 0x48000) != 0;
}

#pragma dont_inline on
bool cf::CfGameManager::func_800829B8() {
    if (testResInfoFlag(0x04000000)) {
        return false;
    }

    getInstance();
    return func_80496288(lbl_eu_80663E14) <= lbl_eu_8066A208;
}
#pragma dont_inline reset

#pragma dont_inline on
bool cf::CfGameManager::func_80082680() {
    return lbl_eu_80664772 != 0;
}
#pragma dont_inline reset

bool cf::CfGameManager::func_8007F91C() {
    return lbl_eu_80663E3C > 0;
}

u16 cf::CfGameManager::func_8007E030() {
    return field_0x18;
}

#pragma dont_inline on
cf::CfObject** cf::CfGameManager::func_8007E0C8() {
    return &field_0x4;
}
#pragma dont_inline reset

void cf::CfGameManager::func_80087334() {
    field_0x4->CfObject_UnkVirtualFunc52();
}

void cf::CfGameManager::func_80087348() {
    field_0x4->CfObject_UnkVirtualFunc53();
}

void cf::CfGameManager::func_80087364() {
    field_0x4->CfObject_UnkVirtualFunc48();
}

void cf::CfGameManager::func_80087378() {
    field_0x4->CfObject_UnkVirtualFunc46();
}

void cf::CfGameManager::func_80087390() {
    field_0x4->CfObject_UnkVirtualFunc56();
}

void cf::CfGameManager::func_800873AC() {
    field_0x4->CfObject_UnkVirtualFunc58();
}

void cf::CfGameManager::func_800873D4() {
    field_0x4->CfObject_UnkVirtualFunc31();
}

void cf::CfGameManager::func_800873E8() {
    reinterpret_cast<Unk80EE4Data*>(field_0x4)->vfunc_0xC4();
}

void cf::CfGameManager::func_800873FC() {
    field_0x4->CfObject_UnkVirtualFunc23();
}

void cf::CfGameManager::func_80087410() {
    field_0x4->CfObject_UnkVirtualFunc19();
}

#pragma dont_inline on
void cf::CfGameManager::func_80082258() {
    func_80141B20(this);
    func_800B1E2C(this);
}
#pragma dont_inline reset

void cf::CfGameManager::func_80085878() {
    if (func_80083298() != nullptr && &func_80083298()->field_0xF0 != nullptr) {
        func_8047BDA0__17UnkClass_8047BB54Fv(&func_80083298()->field_0xF0);
    }
}

u32 cf::CfGameManager::func_80082900() {
    UnkClass_80083298* object = func_80083298();
    return object != nullptr ? object->vfunc_0x74() : 0;
}

void cf::CfGameManager::func_80083304() {
    getInstance()->func_8007C8C8();
}

// cf::CfGameManager::func_8007C8C8 - per-frame scene/presentation tick and
// the scene-load / object-queue dump. Returns the func_80190840 result.
// Flag bits use the local convention (value bit = 31 - PPC bit): E24 0x80,
// 0x1000, 0x400000, 0x40000|0x8000, 0x2000000|0x400; E28 0x1000000, 0x200,
// 0x8000000.
void* cf::CfGameManager::func_8007C8C8() {
    void* ret = func_80190840(this->unkAC, &lbl_eu_80663E04);
    if (lbl_eu_80663E60 != 0) {
        func_801889D0(lbl_eu_80663E60);
    }
    func_800D9354(getInstance__Q22cf14CBattleManagerFv());
    if (lbl_eu_80663E24 & 0x80) {
        func_80295A88(lbl_eu_80664A10);
    }
    func_802A1610();
    if (lbl_eu_80663E14 != nullptr &&
        func_80496288(lbl_eu_80663E14) > lbl_eu_80666498) {
        ++lbl_eu_80663D90;
    }
    if (!(lbl_eu_80663E28 & 0x1000000)) {
        func_8006A75C();
    }
    if (func_80082900__Q22cf13CfGameManagerFv() != 0) {
        ++lbl_eu_80663E6C;
        if (!(lbl_eu_80663E6C & 1)) {
            if (lbl_eu_80663E24 & 0x1000) {
                func_80186D20(this->field_0xA4);
            }
        } else if ((lbl_eu_80663E6C & 1) == 1 && (lbl_eu_80663E24 & 0x1000)) {
            func_8016F140(this->unkA0);
        }
        if ((lbl_eu_80663E24 & 0x1000) && !(lbl_eu_80663E28 & 0x1000000)) {
            func_8016F144(this->unkA0);
        }
        this->func_8007CDA8();
    }
    if ((lbl_eu_80663E24 & 0xAFA40000) == 0 ||
        (lbl_eu_80663E28 & 0x1000000) != 0) {
        func_800FE68C();
        func_800FE104();
        if (func_80082900__Q22cf13CfGameManagerFv() != 0) {
            if (lbl_eu_80663E24 & 0x1000) {
                func_8019FD2C();
            }
            if (((lbl_eu_80663E24 & 0x2000000) != 0 ||
                 (lbl_eu_80663E24 & 0x400) != 0) &&
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
                if (func_8007CBD4(0x1000)) {
                    cf::CfObjectMove* player = getPlayer(0);
                    if (player != nullptr) {
                        func_8016EC58(player->CfObject_UnkVirtualFunc23());
                    }
                }
            }
            func_80069F2C();
            if (func_800829B8() == 0 &&
                cf::CfPadTask::func_801C1BC0() == 0) {
                int sceneReady = lbl_eu_80663E14 == nullptr
                                     ? 1
                                     : func_80496044(lbl_eu_80663E14);
                if (sceneReady != 0 &&
                    (lbl_eu_80663E24 & 0x400000) == 0 &&
                    (lbl_eu_80663E24 & 0xAFA40000) == 0 &&
                    (lbl_eu_80663E24 & (0x2000000 | 0x400)) == 0 &&
                    (lbl_eu_80663E24 & (0x40000 | 0x8000)) == 0 &&
                    (lbl_eu_80663E28 & 0x200) == 0 &&
                    func_8010CE48() &&
                    (lbl_eu_80663E28 & 0x8000000) == 0 &&
                    (lbl_eu_80663E24 & 0x80) != 0) {
                    func_800827E4__Q22cf13CfGameManagerFv();
                }
            }
        }
    }
done:
    u32 v1 = func_8006A3BC();
    u32 v2 = func_8006A37C();
    u32 v3 = func_8006A33C();
    func_80462CD8__8CTaskLODFv((u16)v3, (u16)v2, (u16)v1);
    func_8007CBEC__Q22cf13CfGameManagerFv();
    return ret;
}
void cf::CfGameManager::func_8007FBFC() {
    if (CView::getCurrentView() == lbl_eu_80663E10) {
        func_800B9340();
    }
}

void cf::CfGameManager::func_8007FC2C() {
    if (CView::getCurrentView() == lbl_eu_80663E10) {
        func_800B9364();
    }
}

void cf::CfGameManager::func_8007E4DC() {
    func_801BFFAC(lbl_eu_8066650C, lbl_eu_8066649C);
    func_80189510(lbl_eu_80666510);
    func_801895F4(lbl_eu_8066649C);
}

void cf::CfGameManager::func_80082060() {
    func_80079B34(getInstance()->unkB4);
}

bool cf::CfGameManager::func_80082104() {
    return func_80079DBC(getInstance()->unkB4);
}

#pragma dont_inline on
UnkClass_800821F8* cf::CfGameManager::func_800821F8() {
    return getInstance()->unkB0;
}
#pragma dont_inline reset

cf::CfObject* cf::CfGameManager::func_8008221C() {
    CfCamEventManager* manager = getInstance()->unkB4;
    return manager != nullptr ? func_8006E5A4(manager) : nullptr;
}

u16 cf::CfGameManager::func_80082770() {
    s32 oldIndex = field_0x44;
    field_0x44++;
    field_0x44 %= field_0x4C;
    field_0x48--;
    return field_0x40[oldIndex];
}

u32 cf::CfGameManager::func_800828DC() {
    return getInstance()->unkA0;
}

void cf::CfGameManager::func_8007EEF0() {
    func_8009D790(&field_0x1C);
}

bool cf::CfObjectMap::func_8007D814() {
    return field_0x8E >= 2;
}

void cf::CfGameManager::func_8007D834() {
    if (lbl_eu_80663E24 & 0x1000) {
        func_80186D20(field_0xA4);
    }
}

void cf::CfObject::CfObject_UnkVirtualFunc33(float amount) {
    CfObject_UnkVirtualFunc32();
}

extern "C" void CfObject_UnkVirtualFunc33__Q22cf8CfObjectFv(
    cf::CfObject* object) {
    object->CfObject_UnkVirtualFunc32();
}

#pragma dont_inline on
void cf::CfGameManager::func_80083550() {
    lbl_eu_80663E24 |= 0x10;
}
#pragma dont_inline reset

#pragma dont_inline on
void cf::CfGameManager::func_800835FC() {
    lbl_eu_80663E24 |= 0x20;
}
#pragma dont_inline reset

#pragma dont_inline on
void cf::CfGameManager::func_80083878() {
    lbl_eu_80663E24 |= 0x40;
}
#pragma dont_inline reset

void cf::CfGameManager::func_80083CC8() {
    lbl_eu_80663E24 &= ~0x20;
}

#pragma dont_inline on
bool cf::CfGameManager::func_80087250() {
    return func_8009CF8C(0x3508) != 0;
}
#pragma dont_inline reset

#pragma dont_inline on
void* cf::CfGameManager::func_8007C4B4() { return this; }
#pragma dont_inline reset
void cf::CfGameManager::func_8007EF44() {}
void cf::CfGameManager::func_80087330() {}
void cf::CfObject::CfObject_UnkVirtualFunc46() {}
void cf::CfObject::CfObject_UnkVirtualFunc47() {}

void cf::CfGameManager::func_8007EF48() {}
void cf::CfGameManager::func_8007FE18() {}
void cf::CfGameManager::func_8007FE1C() {}
void cf::CfGameManager::func_8007FE20() {}
extern "C" void* func_8007F0A4__Q22cf13CfGameManagerFv() { return nullptr; }
extern "C" void* func_8007F114__Q22cf13CfGameManagerFv() { return nullptr; }
extern "C" void* func_8007FE24__Q22cf13CfGameManagerFv() { return nullptr; }
cf::CfObject* cf::CfObject::CfObject_UnkVirtualFunc53() {
    return nullptr;
}
extern u32 lbl_eu_80661BC0;
u32* cf::CfObject::CfObject_UnkVirtualFunc58() {
    return &lbl_eu_80661BC0;
}
// lwz getters - return global vars
extern u32 lbl_eu_80664188;
u32 cf::CfGameManager::func_80083458() { return lbl_eu_80664188; }
extern u32 lbl_eu_80664184;
u32 cf::CfGameManager::func_80083460() { return lbl_eu_80664184; }
extern "C" u32 func_80086B1C__Q22cf13CfGameManagerFv() { return lbl_eu_80664184; }
extern u32 lbl_eu_8066409C;
extern "C" u32 func_80086B24__Q22cf13CfGameManagerFv() { return lbl_eu_8066409C; }
extern void* lbl_eu_806640A8;
extern "C" u32 func_80086B2C__Q22cf13CfGameManagerFv() { return (u32)lbl_eu_806640A8; }
extern u32 lbl_eu_806640A4;
extern "C" u32 func_80086B34__Q22cf13CfGameManagerFv() { return lbl_eu_806640A4; }
extern u32 lbl_eu_806640F4;
extern "C" u32 func_80086B3C__Q22cf13CfGameManagerFv() { return lbl_eu_806640F4; }
extern u32 lbl_eu_80663D90;
extern "C" u32 func_80086DB4__Q22cf13CfGameManagerFv() { return lbl_eu_80663D90; }
extern u8 lbl_eu_80663E34;
extern "C" u8 func_8007F9BC__Q22cf13CfGameManagerFv() { return lbl_eu_80663E34; }
extern u8 lbl_eu_80663E5D;
u8 cf::CfGameManager::func_80080E20() { return lbl_eu_80663E5D; }
extern u8 lbl_eu_8066476D;
u8 cf::CfGameManager::func_80080E28() { return lbl_eu_8066476D; }
void* cf::CfGameManager::func_8007DA00() { return lbl_eu_8065FC18; }

#pragma dont_inline on
extern "C" Unk8187CData* func_80080F40__Q22cf13CfGameManagerFv(u32 first, u32 second, u32 third) {
    return reinterpret_cast<Unk8187CData*>(func_800B985C(reinterpret_cast<UnkClass_800AA714*>(first), reinterpret_cast<UnkClass_800AA714*>(second), third));
}
#pragma dont_inline reset

void cf::CfGameManager::func_80080F44() { func_800B9404(this); }

void cf::CfGameManager::func_80081CB8() { func_800B9548(); }

extern "C" void* func_800B97A0(void* object);
#pragma dont_inline on
extern "C" Unk80EE4Data* func_80081D88__Q22cf13CfGameManagerFv(
    void* object, u32 value) {
    return static_cast<Unk80EE4Data*>(func_800B97A0(object));
}
#pragma dont_inline reset

void cf::CfGameManager::func_80082254() { func_800B93D0(); }

extern "C" void func_8026178C(void* object);
void cf::CfGameManager::func_80082EC0() { func_8026178C(this); }

void cf::CfGameManager::func_80086B04() { func_800B6BA4__Fv(); }

extern "C" void func_800B6BC8();
void cf::CfGameManager::func_80086B08() { func_800B6BC8(); }

extern "C" void func_800B6BEC();
void cf::CfGameManager::func_80086B0C() { func_800B6BEC(); }

void cf::CfGameManager::func_80086B10() { func_800B6C10(); }

extern "C" void func_800B6C34();
void cf::CfGameManager::func_80086B14() { func_800B6C34(); }

extern "C" void func_800B8920();
void cf::CfGameManager::func_80086B18() { func_800B8920(); }

void cf::CfGameManager::func_80086B44() { func_80141B54(); }

void cf::CfGameManager::func_80086B48() { func_80069EA0(); }

void cf::CfGameManager::func_80086D90() { func_8006A12C(); }

void cf::CfGameManager::func_80086D94() { func_8006A1A0(); }

extern "C" void func_80086D98__Q22cf13CfGameManagerFv(u16* first, u16* second) { func_8006A234(first, second); }

#pragma dont_inline on
bool cf::CfGameManager::func_80086D9C() { return func_8006A2E0(); }
#pragma dont_inline reset

extern "C" u32 func_8006A33C();
u32 cf::CfGameManager::func_80086DA0() { return func_8006A33C(); }

void cf::CfGameManager::func_80086DA4() { func_8006A37C(); }

void cf::CfGameManager::func_80086DA8() { func_8006A3BC(); }

void cf::CfGameManager::func_80086DAC() { func_8006A3FC(); }

void cf::CfGameManager::func_80086DB0() { func_8006A404(); }

extern "C" u32 func_8006A6D0();
u32 cf::CfGameManager::func_80086DBC() { return func_8006A6D0(); }

#pragma dont_inline on
u16 cf::CfGameManager::func_8007F8B8() {
    ItemListEntryView* object = reinterpret_cast<ItemListEntryView*>(this);
    return object->itemId_0x8C;
}
#pragma dont_inline reset

extern u16 lbl_eu_80663E3A;
extern "C" u16 func_8007F9AC__Q22cf13CfGameManagerFv() { return lbl_eu_80663E3A; }

extern "C" void func_8007F9B4__Q22cf13CfGameManagerFv(u16 value) {
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

extern "C" void func_80081264__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x4 = value; }
extern "C" void func_8008126C__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 0); }
extern "C" void func_80081274__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x8 = value; }
extern "C" void func_8008127C__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 1); }
extern "C" void func_80081284__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x10 = value; }
extern "C" void func_8008128C__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 2); }
extern "C" void func_80081294__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { *reinterpret_cast<u32*>(reinterpret_cast<u8*>(data) + 0xC) = value; }
extern "C" void func_8008129C__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 3); }
extern "C" void func_800812A4__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x14 = value; }
extern "C" void func_800812AC__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 4); }
extern "C" u16 func_800812B4__Q22cf13CfGameManagerFv(Unk812Data* data) { return data->field_0xE; }
extern "C" u16 func_800812BC__Q22cf13CfGameManagerFv(const u8* data) { return *reinterpret_cast<const u16*>(data + 0x10); }
extern "C" u16 func_800812C4__Q22cf13CfGameManagerFv(const u8* data) { return *reinterpret_cast<const u16*>(data + 0x12); }
extern "C" u16 func_800812CC__Q22cf13CfGameManagerFv(const u8* data) { return *reinterpret_cast<const u16*>(data + 0x14); }
extern "C" u16 func_800812D4__Q22cf13CfGameManagerFv(const u8* data) { return *reinterpret_cast<const u16*>(data + 0x16); }
extern "C" void func_800812DC__Q22cf13CfGameManagerFv(UnkClass_8009EC9C* data) { func_8009E120(data, 5); }
extern "C" u16 func_800812E4__Q22cf13CfGameManagerFv(Unk812Data* data) { return data->field_0xC; }
extern "C" void func_800812EC__Q22cf13CfGameManagerFv(Unk812Data* data, u16 value) { data->field_0x24 = value; }
extern "C" void func_800812F4__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x0 = value; }
extern "C" void func_80081330__Q22cf13CfGameManagerFv(Unk812Data* data, u32 value) { data->field_0x18 = value; }
extern "C" void func_80081338__Q22cf13CfGameManagerFv(Unk812Data* data, u16 value) { data->field_0x20 = value; }
extern "C" void func_80081340__Q22cf13CfGameManagerFv(Unk812Data* data, u16 value) { data->field_0x22 = value; }
extern "C" void func_80081348__Q22cf13CfGameManagerFv(Unk812Data* data, u16 value) { data->field_0x1C = value; }
extern "C" void func_80081350__Q22cf13CfGameManagerFv(Unk812Data* data, u16 value) { data->field_0x1E = value; }

Unk817A8Object* cf::CfGameManager::func_800817A8() {
    return *reinterpret_cast<Unk817A8Object**>(reinterpret_cast<u8*>(this) + 8);
}

#pragma dont_inline on
extern "C" void func_80081874__Q22cf13CfGameManagerFv(Unk8187CData* data, u32 value) { *reinterpret_cast<u32*>(reinterpret_cast<u8*>(data) + 0x64) = value; }
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void func_80081CB0__Q22cf13CfGameManagerFv(u8* data, u16 value) { *reinterpret_cast<u16*>(data + 0x8C) = value; }
#pragma dont_inline reset

#pragma dont_inline on
UNKWORD cf::CfGameManager::func_800822F4() { return func_8009CF8C(0x20); }
#pragma dont_inline reset

#pragma dont_inline on
extern "C" u32 func_80082614__Q22cf13CfGameManagerFv(u32 data) { return func_8009CF8C(data + 0x312C); }
#pragma dont_inline reset
extern "C" u32 func_80082694__Q22cf13CfGameManagerFv(u8* data) { return func_8009CF8C(reinterpret_cast<u32>(data + 0x40)); }


extern "C" s32 func_80083290__Q22cf13CfGameManagerFv(const u8* data) {
    return *reinterpret_cast<const s16*>(data + 0x532);
}

extern u8 lbl_eu_80664298;
void cf::CfGameManager::func_80083468(u32 value) { lbl_eu_80664298 = value; }

extern "C" void* func_80083C70__Q22cf13CfGameManagerFv(u8* data) { return data + 0x219C; }

void cf::CfGameManager::func_80085838() { func_80496034(lbl_eu_80663E14); }

float cf::CfObject::CfObject_UnkVirtualFunc56() {
    return lbl_eu_80666498;
}

u32 cf::CfGameManager::func_80087424() {
    return mObjectFlags;
}

extern u32 lbl_eu_80663E24;
bool cf::CfGameManager::func_8007CBC8() {
    return (lbl_eu_80663E24 & 0x80) != 0;
}

#pragma dont_inline on
u32 cf::CfGameManager::func_8007EEF8() {
    const u32* packed = reinterpret_cast<const u32*>(this);
    return *packed >> 20;
}
#pragma dont_inline reset

#pragma dont_inline on
void** cf::CfGameManager::func_8007F8D0() {
    UnkF8C0Node* iterator = reinterpret_cast<UnkF8C0Node*>(this);
    ItemListNode* node = reinterpret_cast<ItemListNode*>(iterator->field_0x0);
    return reinterpret_cast<void**>(&node->object);
}

#pragma dont_inline reset

extern void __fill_mem(void*, int, int);

extern u32 lbl_eu_80663E24;
bool cf::CfGameManager::func_800817B0() {
    return (*reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(this) + 0x82C) >> 2) & 1;
}

#pragma dont_inline on
extern "C" void func_80082940__Q22cf13CfGameManagerFv(void*, void* first, void* second) { func_80141C6C(first, second); }
#pragma dont_inline reset

#pragma dont_inline on
u32 cf::CfGameManager::func_80083100() {
    return (field_0x4EC >> 16) & 1;
}

u32 cf::CfGameManager::func_8008310C() {
    return (field_0x4EC >> 7) & 1;
}
#pragma dont_inline reset

extern "C" u32 func_80083284__Q22cf13CfGameManagerFv(const u8* data) {
    return (*reinterpret_cast<const u32*>(data + 0x4EC) >> 20) & 1;
}

extern "C" void CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv(u8* data) {
    *reinterpret_cast<u32*>(data + 0x34) = 0;
}

extern u32 lbl_eu_80663E24;
#pragma dont_inline on
bool cf::CfGameManager::func_80083538() {
    return (lbl_eu_80663E24 & 0x20) != 0;
}

bool cf::CfGameManager::func_80083544() {
    return (lbl_eu_80663E24 & 0x10) != 0;
}
#pragma dont_inline reset

extern u32 lbl_eu_80663E28;
bool cf::CfGameManager::func_80087244() {
    return (lbl_eu_80663E28 & 0x100000) != 0;
}

cf::CfObjectSub54* cf::CfGameManager::func_800873C8() {
    return &field_0x4->mSubObj54;
}

void cf::CfGameManager::func_8008743C() {
    mObjectFlags = 0;
}

extern "C" void CActorParam_UnkVirtualFunc89__Q22cf11CActorParamFv(u8* data, u32 value) {
    *reinterpret_cast<u32*>(data + 0x1608) = value;
}

extern "C" u32 CActorParam_UnkVirtualFunc91__Q22cf11CActorParamFv(const u8* data) {
    return *reinterpret_cast<const u32*>(data + 0x1608);
}

cf::UnkClass_CActorParam15E0* cf::CActorParam::CActorParam_UnkVirtualFunc127() {
    return field_0x15D8;
}


void cf::CfGameManager::func_8007C6C0() {}

// cf::CfGameManager::func_8007C6C0 - full init/reset of the singleton state:
// stores the scene pointer, wipes the flag words / Bdat text entries / pad
// slot, then allocates the per-player file streams, camera manager, map-
// effect manager and the remaining subsystems. Declared extern "C" because
// the retail signature carries a hidden r4 scene argument under an Fv name.
extern "C" void func_8007C6C0__Q22cf13CfGameManagerFv(cf::CfGameManager* self, void* scene) {
    lbl_eu_80663E14 = static_cast<CScn*>(scene);
    self->func_8007C5B8();

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
    func_801414CC__Q22cf6CfBdatFv();

    for (s32 i = 0; i < 3; ++i) {
        u32 heap = func_80061FFC();
        void* memory = allocate__Q23mtl10MemManagerFUlUl(0xE5C, heap);
        void* object = memory;
        if (memory != nullptr) {
            object = __ct__cf_CfTFile(memory);
        }
        self->unk94[i] = static_cast<cf::CfObjectMove*>(object);
    }

    u32 heap = func_80061FFC();
    void* memory = allocate__Q23mtl10MemManagerFUlUl(0x4E4, heap);
    void* object = memory;
    if (memory != nullptr) {
        object = __ct__80075298(memory);
    }
    self->unkB4 = static_cast<CfCamEventManager*>(object);

    heap = func_80061FFC();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x13C, heap);
    object = memory;
    if (memory != nullptr) {
        object = __ct__cf_CfMapEffectManager(memory);
    }
    self->unkA0 = reinterpret_cast<u32>(object);

    heap = func_80061FFC();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x180, heap);
    object = memory;
    if (memory != nullptr) {
        object = __ct__80186578(memory);
    }
    self->field_0xA4 = static_cast<u8*>(object);

    heap = func_80061FFC();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x2944, heap);
    object = memory;
    if (memory != nullptr) {
        object = __ct__801BF76C(memory);
    }
    lbl_eu_80663E64 = reinterpret_cast<u32>(object);

    heap = func_80061FFC();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x39C, heap);
    object = memory;
    if (memory != nullptr) {
        func_80188584(memory);
    }
    lbl_eu_80663E60 = reinterpret_cast<u32>(object);

    heap = func_80061FFC();
    memory = allocate__Q23mtl10MemManagerFUlUl(0x1C20, heap);
    object = memory;
    if (memory != nullptr) {
        func_801A9CCC(memory);
    }
    self->unkA8 = static_cast<UnkClass_8007E864*>(object);

    if (lbl_eu_80665958 != nullptr) {
        reinterpret_cast<Unk65958Object*>(lbl_eu_80665958)->field_0x7C =
            func_80061FFC();
    }
    func_80069EE0();
    func_80069EA8();
    func_8016E13C();
    func_8006CC0C();
}
// cf::CfGameManager::func_8007CF64 - per-frame camera/target-state tick:
// updates the active text-display entry (lbl_eu_805716F8 or lbl_eu_80571658)
// when the player side indicator changes, then clears the 0x8 flag.
extern "C" void func_8007CF64__Q22cf13CfGameManagerFv(cf::CfGameManager* self) {
    if ((lbl_eu_80663E24 & 0x4) != 0) {
        return;
    }
    if (cf::CfGameManager::func_800829B8()) {
        return;
    }
    if ((lbl_eu_80663E24 & 0x80) == 0) {
        return;
    }

    cf::CfObjectMove* player = cf::CfGameManager::getPlayer(0);
    if (player == nullptr) {
        return;
    }
    if (player->CfObject_UnkVirtualFunc9() == 0) {
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

    if ((u32)direction != (u32)lbl_eu_80663DFC || flag3 != 0) {
        float value = lbl_eu_80663E68 - func_80069EA0();
        lbl_eu_80663E68 = value;
        if (value <= lbl_eu_80666498) {
            lbl_eu_80663E68 = lbl_eu_80666498;
            lbl_eu_80663DFC = static_cast<u8>(direction);
            float fade = func_8016E9CC();
            s32 updated = 0;
            if ((lbl_eu_80663E24 & 0x40) != 0) {
                BdatTextEntry* entry = &lbl_eu_805716F8;
                if (entry->textLength != 0) {
                    updated = 1;
                    if (lbl_eu_80663E3E > 0) {
                        if (direction != 0) {
                            func_80189390(entry);
                        } else {
                            func_80189390(&entry->secondaryText);
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
                            func_80189390(entry);
                        } else {
                            func_80189390(&entry->secondaryText);
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
    char* base = lbl_eu_804FB824;
    for (u16 dataId = 1; dataId <= 13; ++dataId) {
        UnkClass_8009EC9C* data = static_cast<UnkClass_8009EC9C*>(func_8009EC9C(dataId));
        reinterpret_cast<u16*>(data->field_0x0)[0] = dataId;
        for (s32 col = 0; col <= 5; ++col) {
            u16 value = 0;
            if (col == 0) {
                value = static_cast<u16>(getBdatStringColumnValue(lbl_eu_80663E74, base + 0x28, dataId));
            } else if (col == 1) {
                value = static_cast<u16>(getBdatStringColumnValue(lbl_eu_80663E74, base + 0x31, dataId));
            } else if (col == 2) {
                value = static_cast<u16>(getBdatStringColumnValue(lbl_eu_80663E74, base + 0x3A, dataId));
            } else if (col == 3) {
                value = static_cast<u16>(getBdatStringColumnValue(lbl_eu_80663E74, base + 0x42, dataId));
            } else if (col == 4) {
                value = static_cast<u16>(getBdatStringColumnValue(lbl_eu_80663E74, base + 0x4C, dataId));
            } else if (col == 5) {
                value = static_cast<u16>(getBdatStringColumnValue(lbl_eu_80663E74, base + 0x55, dataId));
            }
            s16 result;
            s16 extra;
            bool active = false;
            if (value != 0 && lbl_eu_80663E40 != 0 && dataId <= 7) {
                func_80158420(value, &result, 1, &extra);
                func_8009E0C4(data, col, value);
                func_8009DBF4(data, col, reinterpret_cast<void*>(static_cast<s32>(result)));
                active = true;
            }
            if (!active) {
                func_8009DBF4(data, col, reinterpret_cast<void*>(-1));
                func_8009E0C4(data, col, value);
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
        reinterpret_cast<cf::CtrlObjectParamSlots*>(func_8009ECB0());
    func_8009E3C0();
    u16 match[9];
    const u32* slotWords = reinterpret_cast<const u32*>(party);
    for (int i = 0; i < 9; ++i) {
        match[i] = static_cast<u16>(slotWords[i + 1]);
    }

    const s16* list = static_cast<const s16*>(inList);

    // Camera-manager dispatch (getter invoked twice: null test + use).
    if (cf::CfGameManager::getInstance()->func_8008221C()) {
        cf::CfObject* cam = cf::CfGameManager::getInstance()->func_8008221C();
        void** vt = *reinterpret_cast<void***>(cam);
        typedef void (*V5CFn)(void*, s32);
        reinterpret_cast<V5CFn>(vt[0x5C / 4])(cam, 0);
    }

    if (!lbl_eu_80663E70) {
        __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
        __register_global_object(&lbl_eu_80571758, __dt__Q22cf13CfGameManagerFv,
                                 lbl_eu_80571748);
        lbl_eu_80663E70 = 1;
    }
    for (int i = 0; i < 3; ++i) {
        if (lbl_eu_80571758.unk94[i] != 0) {
            // func_80080F44 is an Fv-hidden-arg helper whose `this` is the
            // player object (calls func_800B9404(this)) - dispatch via cast.
            reinterpret_cast<cf::CfGameManager*>(lbl_eu_80571758.unk94[i])
                ->func_80080F44();
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
                func_8009E740(party, x);
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
                    if (!func_8009E56C(party, x, 1)) {
                        func_8009E56C(party, x, 2);
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
        if (i != 3 && func_8008235C__Q22cf13CfGameManagerFv(i)) {
            u16 c = static_cast<u16>(
                func_800A082C(func_8009EC9C(static_cast<u16>(i))));
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

    u32 itembase = func_8009CF8C(0x10a);
    s32 seed = static_cast<s32>(func_80291BF8());

    for (int i = 0; i < 9; ++i) {
        s16 x = list[i];
        if (x != 0 &&
            func_800823A4__Q22cf13CfGameManagerFv(static_cast<u16>(x), 1)) {
            UnkClass_8009EC9C* obj =
                reinterpret_cast<UnkClass_8009EC9C*>(
                    func_8009EC9C(static_cast<u16>(x)));
            if (leader != 0 && x != 0x0b && func_8009CF8C(0x3508) == 0) {
                func_800A0860(obj, static_cast<u16>(leader));
                func_8009EF9C(obj, 0);
                func_800A21F8(obj, 0, lbl_eu_805276F0[static_cast<u16>(x)], 0);
                if (x >= 3 && x <= 8) {
                    u16 c = static_cast<u16>(func_800A082C(obj));
                    func_800A2974(obj,
                                  static_cast<u16>(c + seed + itembase - 1));
                }
            } else {
                u16 c = static_cast<u16>(func_800A082C(obj));
                if (x == 3) {
                    UnkClass_8009EC9C* obj8 =
                        reinterpret_cast<UnkClass_8009EC9C*>(func_8009EC9C(8));
                    u16 c8 = static_cast<u16>(func_800A082C(obj8));
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
                func_8025EE7C(&obj->itemCounts_0x3534,
                              *reinterpret_cast<u32*>(
                                  reinterpret_cast<u8*>(obj3) + 0x3DD0));
                u8* sub3 = reinterpret_cast<u8*>(obj3) + 0x17C;
                void** vt3 = *reinterpret_cast<void***>(sub3);
                typedef void* (*V200Fn)(u8*);
                void* r = reinterpret_cast<V200Fn>(vt3[0x200 / 4])(sub3);
                u8* sub2 = reinterpret_cast<u8*>(obj) + 0x17C;
                void** vt2 = *reinterpret_cast<void***>(sub2);
                typedef void (*V1F8Fn)(u8*, void*);
                reinterpret_cast<V1F8Fn>(vt2[0x1F8 / 4])(sub2, r);
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
// cf::CfGameManager::func_80084F50 - in-battle presentation reset: on the
// E24 bit-16 "title/battle setup" flag, resets the battle manager
// (party-gauge + 0x94 payload), clears the presentation masks, reloads the
// bdat event table, then either kicks the event-video UI or re-populates the
// file-event table through func_8009ECB0 / func_8009D5FC.
void cf::CfGameManager::func_80084F50() {
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
        func_8018C8F4(getInstance__Q22cf14CBattleManagerFv()->field_0x194, 0);
        CBattleManagerView* battle = getInstance__Q22cf14CBattleManagerFv();
        memset(battle->cleared_0x94, 0, 0x100);
        reinterpret_cast<CBattleManagerVt1C*>(battle)->m1C(2, 0);
    }
    func_80135FDC();
    manager->func_8007D84C();
    lbl_eu_80663E24 &= ~0x400000;
    lbl_eu_80663E28 &= ~0x100000;
    lbl_eu_80570CF0.field_0x48 = 0;
    lbl_eu_80570CF0.field_0x44 = 0;
    lbl_eu_80663ED8 = lbl_eu_80666498;
    lbl_eu_80663EDC = lbl_eu_80666498;
    func_802062BC();
    func_80164CFC();
    func_802959AC(lbl_eu_80664A10);
    func_802A1F9C();
    if (func_8023C1C0() != 0) {
        if (!lbl_eu_80663E70) {
            __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
            __register_global_object(&lbl_eu_80571758,
                                     __dt__Q22cf13CfGameManagerFv,
                                     lbl_eu_80571748);
            lbl_eu_80663E70 = 1;
        }
        if (lbl_eu_80571758.unkAC != nullptr) {
            func_80061870(lbl_eu_80571758.unkAC, 0x1E, 0, 0, 0, 0);
        }
    } else {
        for (s32 i = 1; i <= 13; ++i) {
            func_8009F6D4(func_8009EC9C(static_cast<u16>(i)));
        }
        __dt__8023E448();
        func_801592EC();
        if (lbl_eu_80664774 != 0) {
            // Retail materializes the resource-loaded test as a bool
            // (neg/or/rlwinm booleanize) before the short-circuit &&, so the
            // != result is captured in a local first.
            bool hasVisionPack = func_8009CF8C(0x3508) != 0;
            if (hasVisionPack && func_800822F4__Q22cf13CfGameManagerFv() == 0) {
                u16 buffer[9];
                memset(buffer, 0, sizeof(buffer));
                buffer[0] = 4;
                buffer[1] = 9;
                buffer[2] = 10;
                func_8007F1FC__Q22cf13CfGameManagerFv(buffer, 1);
            }
        }
        UnkC1B4Data* video = reinterpret_cast<UnkC1B4Data*>(func_8023C1B4());
        lbl_eu_80663E40 = (u16)func_8009ECB0()[1];
        func_8009E474(func_8009ECB0(), lbl_eu_80663E40);
        CfFileEventIdsView* ids = func_8009D5FC();
        func_80083D50__Q22cf13CfGameManagerFv(
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
        func_80189318(1, lbl_eu_80666498);
        func_80189424(lbl_eu_80666498);
        func_801C0118(0x20, 0xf);
        func_8013D26C(0);
        func_80068C38();
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
            func_80061870(lbl_eu_80571758.unkAC, 6, 2, 0, 0, 0);
        }
        if (!lbl_eu_80663E70) {
            __ct__Q22cf13CfGameManagerFv(&lbl_eu_80571758);
            __register_global_object(&lbl_eu_80571758,
                                     __dt__Q22cf13CfGameManagerFv,
                                     lbl_eu_80571748);
            lbl_eu_80663E70 = 1;
        }
        if (lbl_eu_80571758.unkAC != nullptr) {
            func_80061870(lbl_eu_80571758.unkAC, 0x12, 0, 0, 0, 0);
        }
        func_eu_8049AB50(lbl_eu_80663E14, 0);
    } else if ((lbl_eu_80663E24 & 0x2000000) != 0 ||
               (lbl_eu_80663E24 & 0x400) != 0) {
        func_80164DB8();
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
    func_801896A8(1, lbl_eu_8066649C, lbl_eu_80666498);
    ItemListManager* list = func_800B6BA4__Fv();
    ItemListNode* node = list->sentinel->next;
    while (node != list->sentinel) {
        Unk80EE4Data* object = static_cast<Unk80EE4Data*>(
            func_800BFC68__FPQ22cf12CfObjectMove(node->object));
        if (object != nullptr) {
            object->vfunc_0xA8(true);
            object->vfunc_0xB8();
        }
        node = node->next;
    }
    lbl_eu_80663E28 &= ~8;
}
void cf::CfGameManager::func_80085978() {}
void cf::CfGameManager::func_80085FB8() {}
// cf::CfGameManager::func_80086778 - one-time bdat table scan (guarded by the
// E35 flag). Loads the current bdat table (lbl_eu_806640A0), walks a row range,
// tallies rows by the FD column byte, and for rows whose B3-column byte matches
// lbl_eu_80664184 spawns collision objects via func_80081CB8 and positions them
// with the func_800AC1xx collision helpers.
void cf::CfGameManager::func_80086778() {
    if (lbl_eu_80663E35 != 0) {
        return;
    }
    func_8003AA34();
    void* bdat = lbl_eu_806640A0;
    int rowStart = (int)func_8003B41C(bdat);
    int rowCount = (int)func_8003B1EC(bdat);
    lbl_eu_80663E35 = 1;
    lbl_eu_80663E36 = 0;
    lbl_eu_80663E38 = 0;
    const char* base = lbl_eu_804FB824;
    int targetCode = (int)lbl_eu_80664184;

    for (int i = 0; i < rowCount; ++i) {
        int row = rowStart + i;
        u8 code = (u8)*((const char*)getBdatStringColumnValue(bdat, base + 0xb3, row));
        u8 v = (u8)*((const char*)getBdatStringColumnValue(bdat, base + 0xfd, row));
        if (v == 0) {
            ++lbl_eu_80663E36;
        } else if (v == 1) {
            ++lbl_eu_80663E38;
        }
        if (code != targetCode) {
            continue;
        }
        s16 sA = *(const s16*)(const char*)getBdatStringColumnValue(bdat, base + 0x106, row);
        s16 sB = *(const s16*)(const char*)getBdatStringColumnValue(bdat, base + 0x10b, row);
        s16 sC = *(const s16*)(const char*)getBdatStringColumnValue(bdat, base + 0x110, row);
        union { double d; u32 w[2]; } cvt1, cvt2;
        cvt1.w[0] = 0x43300000;
        cvt2.w[0] = 0x43300000;
        ml::CVec3 v48;
        cvt1.w[1] = (u32)sA ^ 0x80000000;
        v48.x = (float)(cvt1.d - *reinterpret_cast<const double*>(&lbl_eu_80666530));
        cvt2.w[1] = (u32)sB ^ 0x80000000;
        v48.y = (float)(cvt2.d - *reinterpret_cast<const double*>(&lbl_eu_80666530));
        cvt1.w[1] = (u32)sC ^ 0x80000000;
        v48.z = (float)(cvt1.d - *reinterpret_cast<const double*>(&lbl_eu_80666530));
        u16 uD = *(const u16*)(const char*)getBdatStringColumnValue(bdat, base + 0x115, row);
        u16 uE = *(const u16*)(const char*)getBdatStringColumnValue(bdat, base + 0x11b, row);
        cvt2.w[1] = (u32)uD;
        float fD = (float)(cvt2.d - *reinterpret_cast<const double*>(&lbl_eu_80666528));
        cvt1.w[1] = (u32)uE;
        float fE = (float)(cvt1.d - *reinterpret_cast<const double*>(&lbl_eu_80666528));

        // The bdat object is obtained by calling the matched void member
        // func_80081CB8 with the bdat pointer in r3; its tail-call to
        // func_800B9548 leaks the resulting object through r3.
        typedef Unk80EE4Data* (*BdatThunk)(cf::CfGameManager*);
        union { void (cf::CfGameManager::*m)(); BdatThunk f; } cvt;
        cvt.m = &cf::CfGameManager::func_80081CB8;
        Unk80EE4Data* obj = cvt.f(reinterpret_cast<cf::CfGameManager*>(bdat));
        if (obj == nullptr) {
            continue;
        }
        BdatObjView* view = reinterpret_cast<BdatObjView*>(obj);
        if (fD > lbl_eu_8066A208) {
            ml::CVec3 v3c(v48.x, v48.y + fE, v48.z);
            func_800AC110(obj, &v48, &v3c, fD);
        } else {
            u16 uD2 = *(const u16*)(const char*)getBdatStringColumnValue(bdat, base + 0x121, row);
            float fD2 = U16ToF32(uD2);
            u16 uE2 = *(const u16*)(const char*)getBdatStringColumnValue(bdat, base + 0x127, row);
            float fE2 = U16ToF32(uE2);
            u16 uG = *(const u16*)(const char*)getBdatStringColumnValue(bdat, base + 0x12d, row);
            ml::CVec3 v30(fD2, fE, fE2);
            func_800AC1BC(obj, &v48, &v30, (float)uG * lbl_eu_8066A210);
        }
        func_800AC450(obj, (u16)row, code);
        // virtual dispatch through vtable slot 0x158 (arg 0)
        void** vtbl = *reinterpret_cast<void***>(obj);
        typedef void (*V158Fn)(BdatObjView*, u32);
        reinterpret_cast<V158Fn>(vtbl[0x158 / 4])(view, 0);
        view->field_0x64 &= ~0x10000;
        if (v == 0) {
            view->field_0x158 |= 0x100;
        } else if (v == 1) {
            view->field_0x158 |= 0x200;
        }
    }
}
