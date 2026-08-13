#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include "kyoshin/cf/CfPadTask.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CfObjectMap.hpp"
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
extern "C" float func_80496288(void* scene);
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

#include "kyoshin/cf/CfGameManagerUnityHelpers.hpp"

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
void cf::CfGameManager::func_8007E218() {}

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
void cf::CfGameManager::func_8007CF64() const {}
void cf::CfGameManager::func_8007D1A0() {}
void cf::CfGameManager::func_8007DCB8() {}
void cf::CfGameManager::func_8007E9CC() {}
void cf::CfGameManager::func_8007F1FC() {}
void cf::CfGameManager::func_8007F9C4() {}
void cf::CfGameManager::func_80080888() {}
void cf::CfGameManager::func_8008413C() {}
void cf::CfGameManager::func_80084654() {}
void cf::CfGameManager::func_80084F50() {}
void cf::CfGameManager::func_800853C8() {}
void cf::CfGameManager::func_80085978() {}
void cf::CfGameManager::func_80085FB8() {}
void cf::CfGameManager::func_80086778() {}
