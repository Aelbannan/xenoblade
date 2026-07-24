#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"

#include "monolib/core/CPadManager.hpp"

extern "C" {
u32 lbl_eu_80661BC8;
cf::CfPadData lbl_eu_80571500;
s8 lbl_eu_80663E70;
cf::CfGameManager lbl_eu_80571758;
u8 lbl_eu_80571748[0x10];
CPad* lbl_eu_80663E0C;
CPad lbl_eu_80570D40[8];
u32 lbl_eu_80663E14;
u32 lbl_eu_80663E60;
u32 lbl_eu_80663E64;
u32 lbl_eu_80663E24;
u32 lbl_eu_80663E28;

// symbols.txt: Fv, but callers leave the index in r4 (see func_80082D54).
cf::CfObjectMove** func_8007C6B4__Q22cf13CfGameManagerFv(cf::CfObjectMove** slots, int index);

void __ct__Q22cf13CfGameManagerFv(cf::CfGameManager* self);
void __dt__Q22cf13CfGameManagerFv();
void* __register_global_object(void* object, void* destructor, void* registration);
}

namespace cf {

CfGameManager::CfGameManager() {
    unk28 = 0;
    unk68 = 0;
    unk6C = 0;
    unk7C = 0;
    unk8C = 0;
    unkA0 = 0;
    unkA4 = 0;
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
    unk4 = 0;
    unk8 = 0;
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

CfObjectMove* CfGameManager::func_80082D54(int playerIndex) {
    return *func_8007C6B4__Q22cf13CfGameManagerFv(getInstance()->unk94, playerIndex);
}

} // namespace cf

#pragma dont_inline on
extern "C" cf::CfObjectMove** func_8007C6B4__Q22cf13CfGameManagerFv(cf::CfObjectMove** slots,
                                                                    int index) {
    return &slots[index];
}
#pragma dont_inline off

// LLM-HARNESS-BEGIN: us-8007eb50
extern "C" void func_8007E1B4__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-8007eb50
// LLM-HARNESS-BEGIN: us-8007ebb4
extern "C" void func_8007E218__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-8007ebb4
// LLM-HARNESS-BEGIN: us-800802cc
extern "C" void func_8007F930__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-800802cc
// LLM-HARNESS-BEGIN: us-80083c1c
extern "C" void func_80083298__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-80083c1c
// LLM-HARNESS-BEGIN: us-800874cc
extern "C" void func_800B76CC();
extern "C" void func_eu_800874CC() { func_800B76CC(); }
// LLM-HARNESS-END: us-800874cc
// LLM-HARNESS-BEGIN: us-800874d0
extern "C" void func_800B76F4();

extern "C" void func_eu_800874D0()
{
    func_800B76F4();
}
// LLM-HARNESS-END: us-800874d0
// LLM-HARNESS-BEGIN: us-800874d4
extern "C" void func_800B781C(void);

extern "C" void func_eu_800874D4(void) {
    func_800B781C();
}
// LLM-HARNESS-END: us-800874d4
// LLM-HARNESS-BEGIN: us-800874d8
extern "C" void func_800B7854();

extern "C" void func_eu_800874D8()
{
    func_800B7854();
}
// LLM-HARNESS-END: us-800874d8
// LLM-HARNESS-BEGIN: us-80087534
extern "C" void func_80086B5C__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-80087534
// LLM-HARNESS-BEGIN: us-80087798
extern "C" void enablePadFlags__Q22cf13CfGameManagerFUlb() {}
// LLM-HARNESS-END: us-80087798
// LLM-HARNESS-BEGIN: us-8008791c
extern "C" void getEnabledInputFlags__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-8008791c
// LLM-HARNESS-BEGIN: us-80087974
extern "C" void func_80086F9C__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-80087974
// LLM-HARNESS-BEGIN: us-80087988
extern "C" void getPad__Q22cf13CfGameManagerFi(int) {}
// LLM-HARNESS-END: us-80087988
// LLM-HARNESS-BEGIN: us-800879a0
extern "C" void setPad__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-800879a0
// LLM-HARNESS-BEGIN: us-80087be8
extern "C" void setCurrentPadPtr__Q22cf13CfGameManagerFPC4CPadUl() {}
// LLM-HARNESS-END: us-80087be8
// LLM-HARNESS-BEGIN: us-80087f60
extern "C" void func_80087588() {}
// LLM-HARNESS-END: us-80087f60
// LLM-HARNESS-BEGIN: us-8007f8e4
extern "C" void func_8007EF48__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-8007f8e4
// LLM-HARNESS-BEGIN: us-800807b8
extern "C" void func_8007FE18__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-800807b8
// LLM-HARNESS-BEGIN: us-800807bc
extern "C" void func_8007FE1C__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-800807bc
// LLM-HARNESS-BEGIN: us-800807c0
extern "C" void func_8007FE20__Q22cf13CfGameManagerFv() {}
// LLM-HARNESS-END: us-800807c0
// LLM-HARNESS-BEGIN: us-8007fa40
extern "C" int func_8007F0A4__Q22cf13CfGameManagerFv() { return 0; }
// LLM-HARNESS-END: us-8007fa40
// LLM-HARNESS-BEGIN: us-8007fab0
extern "C" int func_8007F114__Q22cf13CfGameManagerFv() { return 0; }
// LLM-HARNESS-END: us-8007fab0
// LLM-HARNESS-BEGIN: us-800807c4
extern "C" int func_8007FE24__Q22cf13CfGameManagerFv() { return 0; }
// LLM-HARNESS-END: us-800807c4
// LLM-HARNESS-BEGIN: us-80087d34
extern "C" int CfObject_UnkVirtualFunc53__Q22cf8CfObjectFv() { return 0; }
// LLM-HARNESS-END: us-80087d34
// LLM-HARNESS-BEGIN: us-80087d98
extern "C" int CfObject_UnkVirtualFunc58__Q22cf8CfObjectFv() { return 0; }
// LLM-HARNESS-END: us-80087d98
// lwz getters — return global vars
// LLM-HARNESS-BEGIN: us-80083e14
extern u32 lbl_eu_80664188;
extern "C" int func_80083458__Q22cf13CfGameManagerFv() { return lbl_eu_80664188; }
// LLM-HARNESS-END: us-80083e14
// LLM-HARNESS-BEGIN: us-80083e1c
extern u32 lbl_eu_80664184;
extern "C" int func_80083460__Q22cf13CfGameManagerFv() { return lbl_eu_80664184; }
// LLM-HARNESS-END: us-80083e1c
// LLM-HARNESS-BEGIN: us-800874f4
extern "C" int func_80086B1C__Q22cf13CfGameManagerFv() { return lbl_eu_80664184; }
// LLM-HARNESS-END: us-800874f4
// LLM-HARNESS-BEGIN: us-800874fc
extern u32 lbl_eu_8066409C;
extern "C" int func_80086B24__Q22cf13CfGameManagerFv() { return lbl_eu_8066409C; }
// LLM-HARNESS-END: us-800874fc
// LLM-HARNESS-BEGIN: us-80087504
extern u32 lbl_eu_806640A8;
extern "C" int func_80086B2C__Q22cf13CfGameManagerFv() { return lbl_eu_806640A8; }
// LLM-HARNESS-END: us-80087504
// LLM-HARNESS-BEGIN: us-8008750c
extern u32 lbl_eu_806640A4;
extern "C" int func_80086B34__Q22cf13CfGameManagerFv() { return lbl_eu_806640A4; }
// LLM-HARNESS-END: us-8008750c
// LLM-HARNESS-BEGIN: us-80087514
extern u32 lbl_eu_806640F4;
extern "C" int func_80086B3C__Q22cf13CfGameManagerFv() { return lbl_eu_806640F4; }
// LLM-HARNESS-END: us-80087514
// LLM-HARNESS-BEGIN: us-8008778c
extern u32 lbl_eu_80663D90;
extern "C" int func_80086DB4__Q22cf13CfGameManagerFv() { return lbl_eu_80663D90; }
// LLM-HARNESS-END: us-8008778c
// LLM-HARNESS-BEGIN: us-80080358
extern u8 lbl_eu_80663E34;
extern "C" u8 func_8007F9BC__Q22cf13CfGameManagerFv() { return lbl_eu_80663E34; }
// LLM-HARNESS-END: us-80080358
// LLM-HARNESS-BEGIN: us-800817c0
extern u8 lbl_eu_80663E5D;
extern "C" u8 func_80080E20__Q22cf13CfGameManagerFv() { return lbl_eu_80663E5D; }
// LLM-HARNESS-END: us-800817c0
// LLM-HARNESS-BEGIN: us-800817c8
extern u8 lbl_eu_8066476D;
extern "C" u8 func_80080E28__Q22cf13CfGameManagerFv() { return lbl_eu_8066476D; }
// LLM-HARNESS-END: us-800817c8
