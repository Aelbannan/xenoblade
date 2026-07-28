#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"
#include "kyoshin/cf/CfPadTask.hpp"
#include "kyoshin/cf/IResInfo.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/cf/object/CActorParam.hpp"
#include "kyoshin/cf/object/CObjectParam.hpp"
#include "kyoshin/cf/object/CfObjectMove.hpp"
#include "kyoshin/cf/object/CfObjectMap.hpp"
#include "kyoshin/code_801862C0.hpp"

#include "monolib/core/CPadManager.hpp"
#include "monolib/scn/CScn.hpp"

// destructor defined inline in CfGameManager.hpp
// Manually-named destructor wrapper for __dt__Q22cf13CfGameManagerFv
extern "C" void __dt__Q22cf13CfGameManagerFv(void* self) {
    static_cast<cf::CfGameManager*>(self)->~CfGameManager();
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

class ResourceHandle;
extern "C" ResourceHandle* func_8009CF8C(u32 resourceId);
extern "C" void func_8009D790(CfGameManagerData1C* data);
extern "C" float func_80496288(CScn* scene);
extern "C" bool func_80496044(CScn* scene);
extern "C" bool func_804960A8(CScn* scene);
extern const float lbl_eu_8066A208;

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

CfObjectMove* CfGameManager::getPlayer(int playerIndex) {
    return *func_8007C6B4__Q22cf13CfGameManagerFv(getInstance()->unk94, playerIndex);
}

} // namespace cf

#pragma dont_inline on
cf::CfObjectMove** cf::CfGameManager::func_8007C6B4(cf::CfObjectMove** slots,
                                                                    int index) {
    return &slots[index];
}
#pragma dont_inline off

void cf::CfGameManager::func_8007C188(unsigned long flags) {}

void cf::CfGameManager::func_8007D190(unsigned long flags) {}

void cf::CfGameManager::func_8007D794() {}

bool cf::CfGameManager::func_8007E1B4() { return false; }
void cf::CfGameManager::func_8007E218() {}
void cf::CfGameManager::func_8007F930(bool arg1) {}
UNKTYPE* cf::CfGameManager::func_80083298() { return nullptr; }
extern "C" void func_800B76CC();
void func_eu_800874CC(){ func_800B76CC(); }
extern "C" void func_800B76F4();

extern "C" void func_eu_800874D0()
{
    func_800B76F4();
}
extern "C" void func_800B781C(void);

void func_eu_800874D4(void){
    func_800B781C();
}
extern "C" void func_800B7854();

extern "C" void func_eu_800874D8()
{
    func_800B7854();
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
u32 cf::CfGameManager::getEnabledInputFlags() { return 0; }
bool cf::CfGameManager::func_80086F9C() {
    return CfPadTask::getDetectedPadType() == PAD_TYPE_CLASSIC;
}
CPad* cf::CfGameManager::getPad(int channel) {
    return &lbl_eu_80570D40[channel & 7];
}
void cf::CfGameManager::setPad(int r3, CPad* pPad, u32 r5) {}
void cf::CfGameManager::setCurrentPadPtr(const CPad* pad, u32 channel) {
    if (pad != nullptr && lbl_eu_80661BC8 != 0xFFFFFFFF && lbl_eu_80661BC8 != channel) {
        return;
    }

    lbl_eu_80663E0C = const_cast<CPad*>(pad);
}
void func_80087588(){}
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

bool cf::CfGameManager::func_80084BF4() {
    return (lbl_eu_80663E24 & 0x48000) != 0;
}

bool cf::CfGameManager::func_800829B8() {
    if (testResInfoFlag(0x04000000)) {
        return false;
    }

    getInstance();
    return func_80496288(lbl_eu_80663E14) <= lbl_eu_8066A208;
}

bool cf::CfGameManager::func_80082680() {
    return lbl_eu_80664772 != 0;
}

bool cf::CfGameManager::func_8007F91C() {
    return lbl_eu_80663E3C > 0;
}

u16 cf::CfGameManager::func_8007E030() {
    return field_0x18;
}

cf::CfObject** cf::CfGameManager::func_8007E0C8() {
    return &field_0x4;
}

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
    field_0x4->CfObject_UnkVirtualFunc29();
}

void cf::CfGameManager::func_800873FC() {
    field_0x4->CfObject_UnkVirtualFunc23();
}

void cf::CfGameManager::func_80087410() {
    field_0x4->CfObject_UnkVirtualFunc19();
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

void cf::CfObject::CfObject_UnkVirtualFunc33() {
    CfObject_UnkVirtualFunc32();
}

void cf::CfGameManager::func_80083550() {
    lbl_eu_80663E24 |= 0x10;
}

void cf::CfGameManager::func_800835FC() {
    lbl_eu_80663E24 |= 0x20;
}

void cf::CfGameManager::func_80083878() {
    lbl_eu_80663E24 |= 0x40;
}

void cf::CfGameManager::func_80083CC8() {
    lbl_eu_80663E24 &= ~0x20;
}

bool cf::CfGameManager::func_80087250() {
    return func_8009CF8C(0x3508) != nullptr;
}

void cf::CfGameManager::func_8007C4B4() {}
void cf::CfGameManager::func_8007EF44() {}
void cf::CfGameManager::func_80087330() {}
void cf::CfObject::CfObject_UnkVirtualFunc46() {}
void cf::CfObject::CfObject_UnkVirtualFunc47() {}

void cf::CfGameManager::func_8007EF48() {}
void cf::CfGameManager::func_8007FE18() {}
void cf::CfGameManager::func_8007FE1C() {}
void cf::CfGameManager::func_8007FE20() {}
void cf::CfGameManager::func_8007F0A4() {}
void cf::CfGameManager::func_8007F114() {}
void cf::CfGameManager::func_8007FE24() {}
cf::CfObject* cf::CfObject::CfObject_UnkVirtualFunc53() {
    return nullptr;
}
extern u32 lbl_eu_80661BC0;
u32* cf::CfObject::CfObject_UnkVirtualFunc58() {
    return &lbl_eu_80661BC0;
}
// lwz getters — return global vars
extern u32 lbl_eu_80664188;
void cf::CfGameManager::func_80083458() {}
extern u32 lbl_eu_80664184;
void cf::CfGameManager::func_80083460() {}
void cf::CfGameManager::func_80086B1C() {}
extern u32 lbl_eu_8066409C;
void cf::CfGameManager::func_80086B24() {}
extern u32 lbl_eu_806640A8;
void cf::CfGameManager::func_80086B2C() {}
extern u32 lbl_eu_806640A4;
void cf::CfGameManager::func_80086B34() {}
extern u32 lbl_eu_806640F4;
void cf::CfGameManager::func_80086B3C() {}
extern u32 lbl_eu_80663D90;
void cf::CfGameManager::func_80086DB4() {}
extern u8 lbl_eu_80663E34;
void cf::CfGameManager::func_8007F9BC() {}
extern u8 lbl_eu_80663E5D;
void cf::CfGameManager::func_80080E20() {}
extern u8 lbl_eu_8066476D;
void cf::CfGameManager::func_80080E28() {}
extern "C" u32 lbl_eu_8065FC18[];
void cf::CfGameManager::func_8007DA00() {}

extern "C" void func_800B985C();
void cf::CfGameManager::func_80080F40() {}

void cf::CfGameManager::func_80080F44() {}

void cf::CfGameManager::func_80081CB8() {}

void cf::CfGameManager::func_80081D88() {}

void cf::CfGameManager::func_80082254() {}

void cf::CfGameManager::func_80082EC0() {}

extern "C" void func_800B6BA4__Fv();
void cf::CfGameManager::func_80086B04() {}

extern "C" void func_800B6BC8();
void cf::CfGameManager::func_80086B08() {}

extern "C" void func_800B6BEC();
void cf::CfGameManager::func_80086B0C() {}

extern "C" void func_800B6C10();
void cf::CfGameManager::func_80086B10() {}

extern "C" void func_800B6C34();
void cf::CfGameManager::func_80086B14() {}

extern "C" void func_800B8920();
void cf::CfGameManager::func_80086B18() {}

extern "C" void func_80141B54();
void cf::CfGameManager::func_80086B44() {}

extern "C" void func_80069EA0();
void cf::CfGameManager::func_80086B48() {}

extern "C" void func_8006A12C();
void cf::CfGameManager::func_80086D90() {}

extern "C" void func_8006A1A0();
void cf::CfGameManager::func_80086D94() {}

extern "C" void func_8006A234();
void cf::CfGameManager::func_80086D98() {}

extern "C" void func_8006A2E0();
void cf::CfGameManager::func_80086D9C() {}

extern "C" void func_8006A33C();
void cf::CfGameManager::func_80086DA0() {}

extern "C" void func_8006A37C();
void cf::CfGameManager::func_80086DA4() {}

extern "C" void func_8006A3BC();
void cf::CfGameManager::func_80086DA8() {}

extern "C" void func_8006A3FC();
void cf::CfGameManager::func_80086DAC() {}

extern "C" void func_8006A404();
void cf::CfGameManager::func_80086DB0() {}

extern "C" void func_8006A6D0();
void cf::CfGameManager::func_80086DBC() {}

void cf::CfGameManager::func_8007F8B8() {}

extern u16 lbl_eu_80663E3A;
void cf::CfGameManager::func_8007F9AC() {}

extern u16 lbl_eu_80663E3A;
void cf::CfGameManager::func_8007F9B4() {}

void cf::CfGameManager::func_80081264() {}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_8008126C() {}

void cf::CfGameManager::func_80081274() {}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_8008127C() {}

void cf::CfGameManager::func_80081284() {}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_8008128C() {}

void cf::CfGameManager::func_80081294() {}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_8008129C() {}

void cf::CfGameManager::func_800812A4() {}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_800812AC() {}

void cf::CfGameManager::func_800812B4() {}

void cf::CfGameManager::func_800812BC() {}

void cf::CfGameManager::func_800812C4() {}

void cf::CfGameManager::func_800812CC() {}

void cf::CfGameManager::func_800812D4() {}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_800812DC() {}

void cf::CfGameManager::func_800812E4() {}

void cf::CfGameManager::func_800812EC() {}

void cf::CfGameManager::func_800812F4() {}

void cf::CfGameManager::func_80081330() {}

void cf::CfGameManager::func_80081338() {}

void cf::CfGameManager::func_80081340() {}

void cf::CfGameManager::func_80081348() {}

void cf::CfGameManager::func_80081350() {}

void cf::CfGameManager::func_800817A8() {}

void cf::CfGameManager::func_80081874() {}

void cf::CfGameManager::func_80081988() {}

void cf::CfGameManager::func_80081CB0() {}

UNKWORD cf::CfGameManager::func_800822F4() { return 0; }

void cf::CfGameManager::func_80082354() {}
void cf::CfGameManager::func_80082614() {}
void cf::CfGameManager::func_80082694() {}

void cf::CfGameManager::func_80082768() {}

void cf::CfGameManager::func_80083290() {}

extern u8 lbl_eu_80664298;
void cf::CfGameManager::func_80083468() {}

void cf::CfGameManager::func_80083C70() {}

extern "C" void func_80496034(u32);
void cf::CfGameManager::func_80085838() {}

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

void cf::CfGameManager::func_8007EEF8() {}

void cf::CfGameManager::func_8007F8D0() {}

void cf::CfGameManager::func_8007F8F4() {}

extern void __fill_mem(void*, int, int);
void cf::CfGameManager::func_80081258() {}

extern u32 lbl_eu_80663E24;
void cf::CfGameManager::func_800817B0() {}

extern "C" void func_80141C6C(void*, void*);
void cf::CfGameManager::func_80082940() {}

void cf::CfGameManager::func_80083100() {}

void cf::CfGameManager::func_8008310C() {}

void cf::CfGameManager::func_80083284() {}

void cf::CObjectParam::CObjectParam_UnkVirtualFunc4() {
}

extern u32 lbl_eu_80663E24;
bool cf::CfGameManager::func_80083538() {
    return (lbl_eu_80663E24 & 0x20) != 0;
}

bool cf::CfGameManager::func_80083544() {
    return (lbl_eu_80663E24 & 0x10) != 0;
}

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

void cf::CActorParam::CActorParam_UnkVirtualFunc89() {
}

void cf::CActorParam::CActorParam_UnkVirtualFunc91() {
}

void cf::CActorParam::CActorParam_UnkVirtualFunc127() {
}

