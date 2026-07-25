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

// symbols.txt: Fv, but callers leave the index in r4 (see getPlayer).
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

CfObjectMove* CfGameManager::getPlayer(int playerIndex) {
    return *func_8007C6B4__Q22cf13CfGameManagerFv(getInstance()->unk94, playerIndex);
}

} // namespace cf

#pragma dont_inline on
extern "C" cf::CfObjectMove** func_8007C6B4__Q22cf13CfGameManagerFv(cf::CfObjectMove** slots,
                                                                    int index) {
    return &slots[index];
}
#pragma dont_inline off

// LLM-HARNESS-BEGIN: us-8007cb24
extern "C" void func_8007C188__Q22cf13CfGameManagerFv(unsigned long flags) {
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 |= flags;
}
// LLM-HARNESS-END: us-8007cb24

// LLM-HARNESS-BEGIN: us-8007db2c
extern "C" void func_8007D190__Q22cf13CfGameManagerFv(unsigned long flags) {
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 &= ~flags;
}
// LLM-HARNESS-END: us-8007db2c

// LLM-HARNESS-BEGIN: us-8007e130
extern "C" void func_8007D794__Q22cf13CfGameManagerFv(void* self) {
    *(unsigned char*)self = 0;
    *(unsigned long*)((char*)self + 0x20) = 0;
}
// LLM-HARNESS-END: us-8007e130

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
extern "C" void* func_80083298__Q22cf13CfGameManagerFv() {
    extern void* getInstance__Q22cf13CfGameManagerFv();
    return *(void**)((char*)getInstance__Q22cf13CfGameManagerFv() + 0x90);
}
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
// LLM-HARNESS-BEGIN: us-8007e39c
extern "C" u32 lbl_eu_8065FC18[];
extern "C" void* func_8007DA00__Q22cf13CfGameManagerFv() { return (void*)lbl_eu_8065FC18; }
// LLM-HARNESS-END: us-8007e39c

// LLM-HARNESS-BEGIN: us-800818e0
extern "C" void func_800B985C();
extern "C" void func_80080F40__Q22cf13CfGameManagerFv() {
    func_800B985C();
}
// LLM-HARNESS-END: us-800818e0

// LLM-HARNESS-BEGIN: us-800818e4
extern "C" void func_80080F44__Q22cf13CfGameManagerFv() {
    extern void func_800B9404(void);
    func_800B9404();
}
// LLM-HARNESS-END: us-800818e4

// LLM-HARNESS-BEGIN: us-8008263c
extern "C" void func_80081CB8__Q22cf13CfGameManagerFv() {
    extern void func_800B9548(void);
    func_800B9548();
}
// LLM-HARNESS-END: us-8008263c

// LLM-HARNESS-BEGIN: us-8008270c
extern "C" void func_80081D88__Q22cf13CfGameManagerFv() {
    extern void func_800B97A0(void);
    func_800B97A0();
}
// LLM-HARNESS-END: us-8008270c

// LLM-HARNESS-BEGIN: us-80082bd8
extern "C" void func_80082254__Q22cf13CfGameManagerFv() {
    extern void func_800B93D0(void);
    func_800B93D0();
}
// LLM-HARNESS-END: us-80082bd8

// LLM-HARNESS-BEGIN: us-80083844
extern "C" void func_80082EC0__Q22cf13CfGameManagerFv() {
    extern void func_8026178C(void);
    func_8026178C();
}
// LLM-HARNESS-END: us-80083844

// LLM-HARNESS-BEGIN: us-800874dc
extern "C" void func_800B6BA4__Fv();
extern "C" void func_80086B04__Q22cf13CfGameManagerFv() {
    func_800B6BA4__Fv();
}
// LLM-HARNESS-END: us-800874dc

// LLM-HARNESS-BEGIN: us-800874e0
extern "C" void func_800B6BC8();
extern "C" void func_80086B08__Q22cf13CfGameManagerFv() {
    func_800B6BC8();
}
// LLM-HARNESS-END: us-800874e0

// LLM-HARNESS-BEGIN: us-800874e4
extern "C" void func_800B6BEC();
extern "C" void func_80086B0C__Q22cf13CfGameManagerFv() {
    func_800B6BEC();
}
// LLM-HARNESS-END: us-800874e4

// LLM-HARNESS-BEGIN: us-800874e8
extern "C" void func_800B6C10();
extern "C" void func_80086B10__Q22cf13CfGameManagerFv() {
    func_800B6C10();
}
// LLM-HARNESS-END: us-800874e8

// LLM-HARNESS-BEGIN: us-800874ec
extern "C" void func_800B6C34();
extern "C" void func_80086B14__Q22cf13CfGameManagerFv() {
    func_800B6C34();
}
// LLM-HARNESS-END: us-800874ec

// LLM-HARNESS-BEGIN: us-800874f0
extern "C" void func_800B8920();
extern "C" void func_80086B18__Q22cf13CfGameManagerFv() {
    func_800B8920();
}
// LLM-HARNESS-END: us-800874f0

// LLM-HARNESS-BEGIN: us-8008751c
extern "C" void func_80141B54();
extern "C" void func_80086B44__Q22cf13CfGameManagerFv() {
    func_80141B54();
}
// LLM-HARNESS-END: us-8008751c

// LLM-HARNESS-BEGIN: us-80087520
extern "C" void func_80069EA0();
extern "C" void func_80086B48__Q22cf13CfGameManagerFv() {
    func_80069EA0();
}
// LLM-HARNESS-END: us-80087520

// LLM-HARNESS-BEGIN: us-80087768
extern "C" void func_8006A12C();
extern "C" void func_80086D90__Q22cf13CfGameManagerFv() {
    func_8006A12C();
}
// LLM-HARNESS-END: us-80087768

// LLM-HARNESS-BEGIN: us-8008776c
extern "C" void func_8006A1A0();
extern "C" void func_80086D94__Q22cf13CfGameManagerFv() {
    func_8006A1A0();
}
// LLM-HARNESS-END: us-8008776c

// LLM-HARNESS-BEGIN: us-80087770
extern "C" void func_8006A234();
extern "C" void func_80086D98__Q22cf13CfGameManagerFv() {
    func_8006A234();
}
// LLM-HARNESS-END: us-80087770

// LLM-HARNESS-BEGIN: us-80087774
extern "C" void func_8006A2E0();
extern "C" void func_80086D9C__Q22cf13CfGameManagerFv() {
    func_8006A2E0();
}
// LLM-HARNESS-END: us-80087774

// LLM-HARNESS-BEGIN: us-80087778
extern "C" void func_8006A33C();
extern "C" void func_80086DA0__Q22cf13CfGameManagerFv() {
    func_8006A33C();
}
// LLM-HARNESS-END: us-80087778

// LLM-HARNESS-BEGIN: us-8008777c
extern "C" void func_8006A37C();
extern "C" void func_80086DA4__Q22cf13CfGameManagerFv() {
    func_8006A37C();
}
// LLM-HARNESS-END: us-8008777c

// LLM-HARNESS-BEGIN: us-80087780
extern "C" void func_8006A3BC();
extern "C" void func_80086DA8__Q22cf13CfGameManagerFv() {
    func_8006A3BC();
}
// LLM-HARNESS-END: us-80087780

// LLM-HARNESS-BEGIN: us-80087784
extern "C" void func_8006A3FC();
extern "C" void func_80086DAC__Q22cf13CfGameManagerFv() {
    func_8006A3FC();
}
// LLM-HARNESS-END: us-80087784

// LLM-HARNESS-BEGIN: us-80087788
extern "C" void func_8006A404();
extern "C" void func_80086DB0__Q22cf13CfGameManagerFv() {
    func_8006A404();
}
// LLM-HARNESS-END: us-80087788

// LLM-HARNESS-BEGIN: us-80087794
extern "C" void func_8006A6D0();
extern "C" void func_80086DBC__Q22cf13CfGameManagerFv() {
    func_8006A6D0();
}
// LLM-HARNESS-END: us-80087794

// LLM-HARNESS-BEGIN: us-80080254
extern "C" u16 func_8007F8B8__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0x8c);
}
// LLM-HARNESS-END: us-80080254

// LLM-HARNESS-BEGIN: us-80080348
extern u16 lbl_eu_80663E3A;
extern "C" u16 func_8007F9AC__Q22cf13CfGameManagerFv() {
    return lbl_eu_80663E3A;
}
// LLM-HARNESS-END: us-80080348

// LLM-HARNESS-BEGIN: us-80080350
extern u16 lbl_eu_80663E3A;
extern "C" void func_8007F9B4__Q22cf13CfGameManagerFv(u16 val) {
    lbl_eu_80663E3A = val;
}
// LLM-HARNESS-END: us-80080350

// LLM-HARNESS-BEGIN: us-80081c04
extern "C" void func_80081264__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x4) = val;
}
// LLM-HARNESS-END: us-80081c04

// LLM-HARNESS-BEGIN: us-80081c0c
extern "C" void func_8009E120(u32, u32);
extern "C" void func_8008126C__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 0);
}
// LLM-HARNESS-END: us-80081c0c

// LLM-HARNESS-BEGIN: us-80081c14
extern "C" void func_80081274__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x8) = val;
}
// LLM-HARNESS-END: us-80081c14

// LLM-HARNESS-BEGIN: us-80081c1c
extern "C" void func_8009E120(u32, u32);
extern "C" void func_8008127C__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 1);
}
// LLM-HARNESS-END: us-80081c1c

// LLM-HARNESS-BEGIN: us-80081c24
extern "C" void func_80081284__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x10) = val;
}
// LLM-HARNESS-END: us-80081c24

// LLM-HARNESS-BEGIN: us-80081c2c
extern "C" void func_8009E120(u32, u32);
extern "C" void func_8008128C__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 2);
}
// LLM-HARNESS-END: us-80081c2c

// LLM-HARNESS-BEGIN: us-80081c34
extern "C" void func_80081294__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0xc) = val;
}
// LLM-HARNESS-END: us-80081c34

// LLM-HARNESS-BEGIN: us-80081c3c
extern "C" void func_8009E120(u32, u32);
extern "C" void func_8008129C__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 3);
}
// LLM-HARNESS-END: us-80081c3c

// LLM-HARNESS-BEGIN: us-80081c44
extern "C" void func_800812A4__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x14) = val;
}
// LLM-HARNESS-END: us-80081c44

// LLM-HARNESS-BEGIN: us-80081c4c
extern "C" void func_8009E120(u32, u32);
extern "C" void func_800812AC__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 4);
}
// LLM-HARNESS-END: us-80081c4c

// LLM-HARNESS-BEGIN: us-80081c54
extern "C" u16 func_800812B4__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0xe);
}
// LLM-HARNESS-END: us-80081c54

// LLM-HARNESS-BEGIN: us-80081c5c
extern "C" u16 func_800812BC__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0x10);
}
// LLM-HARNESS-END: us-80081c5c

// LLM-HARNESS-BEGIN: us-80081c64
extern "C" u16 func_800812C4__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0x12);
}
// LLM-HARNESS-END: us-80081c64

// LLM-HARNESS-BEGIN: us-80081c6c
extern "C" u16 func_800812CC__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0x14);
}
// LLM-HARNESS-END: us-80081c6c

// LLM-HARNESS-BEGIN: us-80081c74
extern "C" u16 func_800812D4__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0x16);
}
// LLM-HARNESS-END: us-80081c74

// LLM-HARNESS-BEGIN: us-80081c7c
extern "C" void func_8009E120(u32, u32);
extern "C" void func_800812DC__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 5);
}
// LLM-HARNESS-END: us-80081c7c

// LLM-HARNESS-BEGIN: us-80081c84
extern "C" u16 func_800812E4__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0xc);
}
// LLM-HARNESS-END: us-80081c84

// LLM-HARNESS-BEGIN: us-80081c8c
extern "C" void func_800812EC__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x24) = val;
}
// LLM-HARNESS-END: us-80081c8c

// LLM-HARNESS-BEGIN: us-80081c94
extern "C" void func_800812F4__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x0) = val;
}
// LLM-HARNESS-END: us-80081c94

// LLM-HARNESS-BEGIN: us-80081cb4
extern "C" void func_80081330__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x18) = val;
}
// LLM-HARNESS-END: us-80081cb4

// LLM-HARNESS-BEGIN: us-80081cbc
extern "C" void func_80081338__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x20) = val;
}
// LLM-HARNESS-END: us-80081cbc

// LLM-HARNESS-BEGIN: us-80081cc4
extern "C" void func_80081340__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x22) = val;
}
// LLM-HARNESS-END: us-80081cc4

// LLM-HARNESS-BEGIN: us-80081ccc
extern "C" void func_80081348__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x1c) = val;
}
// LLM-HARNESS-END: us-80081ccc

// LLM-HARNESS-BEGIN: us-80081cd4
extern "C" void func_80081350__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x1e) = val;
}
// LLM-HARNESS-END: us-80081cd4

// LLM-HARNESS-BEGIN: us-8008212c
extern "C" u32 func_800817A8__Q22cf13CfGameManagerFv(void* self) {
    return *(u32*)((u8*)self + 0x8);
}
// LLM-HARNESS-END: us-8008212c

// LLM-HARNESS-BEGIN: us-800821f8
extern "C" void func_80081874__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x64) = val;
}
// LLM-HARNESS-END: us-800821f8

// LLM-HARNESS-BEGIN: us-8008230c
extern "C" void func_80081988__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0xa8) = val;
}
// LLM-HARNESS-END: us-8008230c

// LLM-HARNESS-BEGIN: us-80082634
extern "C" void func_80081CB0__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x8c) = val;
}
// LLM-HARNESS-END: us-80082634

// LLM-HARNESS-BEGIN: us-80082c78
extern "C" void func_8009CF8C(u32);
extern "C" void func_800822F4__Q22cf13CfGameManagerFv() {
    func_8009CF8C(0x20);
}
// LLM-HARNESS-END: us-80082c78

// LLM-HARNESS-BEGIN: us-80082cd8
extern "C" void func_8009CF8C(u32);
extern "C" void func_80082354__Q22cf13CfGameManagerFv(u32 self) {
    func_8009CF8C(self + 0x220);
}
// LLM-HARNESS-END: us-80082cd8

// LLM-HARNESS-BEGIN: us-80082f98
extern "C" void func_8009CF8C(u32);
extern "C" void func_80082614__Q22cf13CfGameManagerFv(u32 self) {
    func_8009CF8C(self + 0x312c);
}
// LLM-HARNESS-END: us-80082f98

// LLM-HARNESS-BEGIN: us-80083018
extern "C" void func_8009CF8C(u32);
extern "C" void func_80082694__Q22cf13CfGameManagerFv(u32 self) {
    func_8009CF8C(self + 0x40);
}
// LLM-HARNESS-END: us-80083018

// LLM-HARNESS-BEGIN: us-800830ec
extern "C" u32 func_80082768__Q22cf13CfGameManagerFv(void* self) {
    return *(u32*)((u8*)self + 0x48);
}
// LLM-HARNESS-END: us-800830ec

// LLM-HARNESS-BEGIN: us-80083c14
extern "C" s16 func_80083290__Q22cf13CfGameManagerFv(void* self) {
    return *(s16*)((u8*)self + 0x532);
}
// LLM-HARNESS-END: us-80083c14

// LLM-HARNESS-BEGIN: us-80083e24
extern u8 lbl_eu_80664298;
extern "C" void func_80083468__Q22cf13CfGameManagerFv(u8 val) {
    lbl_eu_80664298 = val;
}
// LLM-HARNESS-END: us-80083e24

// LLM-HARNESS-BEGIN: us-8008462c
extern "C" void* func_80083C70__Q22cf13CfGameManagerFv(void* self) {
    return (u8*)self + 0x219c;
}
// LLM-HARNESS-END: us-8008462c

// LLM-HARNESS-BEGIN: us-80086200
extern u32 lbl_eu_80663E14;
extern "C" void func_80496034(u32);
extern "C" void func_80085838__Q22cf13CfGameManagerFv() {
    func_80496034(lbl_eu_80663E14);
}
// LLM-HARNESS-END: us-80086200

// LLM-HARNESS-BEGIN: us-80087d7c
extern "C" float CfObject_UnkVirtualFunc56__Q22cf8CfObjectFv() {
    extern float lbl_eu_80666498;
    return lbl_eu_80666498;
}
// LLM-HARNESS-END: us-80087d7c

// LLM-HARNESS-BEGIN: us-80087dfc
extern "C" u32 func_80087424__Q22cf13CfGameManagerFv(void* self) {
    return *(u32*)((u8*)self + 0x8);
}
// LLM-HARNESS-END: us-80087dfc

// LLM-HARNESS-BEGIN: us-8007d564
extern u32 lbl_eu_80663E24;
extern "C" u32 func_8007CBC8__Q22cf13CfGameManagerFv() {
    return (lbl_eu_80663E24 >> 7) & 1;
}
// LLM-HARNESS-END: us-8007d564

// LLM-HARNESS-BEGIN: us-8007f894
extern "C" u32 func_8007EEF8__Q22cf13CfGameManagerFv(void* self) {
    return *(u32*)((u8*)self + 0x0) >> 20;
}
// LLM-HARNESS-END: us-8007f894

// LLM-HARNESS-BEGIN: us-8008026c
extern "C" void* func_8007F8D0__Q22cf13CfGameManagerFv(void* self) {
    return (u8*)*(u32**)((u8*)self + 0x0) + 0x8;
}
// LLM-HARNESS-END: us-8008026c

// LLM-HARNESS-BEGIN: us-80080290
extern "C" void func_8007F8F4__Q22cf13CfGameManagerFv(void* self, void* src) {
    *(u32*)((u8*)self + 0x0) = *(u32*)((u8*)src + 0x4);
}
// LLM-HARNESS-END: us-80080290

// LLM-HARNESS-BEGIN: us-80081bf8
extern void __fill_mem(void*, int, int);
extern "C" void func_80081258__Q22cf13CfGameManagerFv(void* self) {
    __fill_mem(self, 0, 0x28);
}
// LLM-HARNESS-END: us-80081bf8

// LLM-HARNESS-BEGIN: us-80082134
extern u32 lbl_eu_80663E24;
extern "C" u32 func_800817B0__Q22cf13CfGameManagerFv(void* self) {
    return (*(u32*)((u8*)self + 0x82c) >> 2) & 1;
}
// LLM-HARNESS-END: us-80082134

// LLM-HARNESS-BEGIN: us-800832c4
extern "C" void func_80141C6C(void*, void*);
extern "C" void func_80082940__Q22cf13CfGameManagerFv(void* self, void* a, void* b) {
    func_80141C6C(a, b);
}
// LLM-HARNESS-END: us-800832c4

// LLM-HARNESS-BEGIN: us-80083a84
extern "C" u32 func_80083100__Q22cf13CfGameManagerFv(void* self) {
    u32 val = *(u32*)((u8*)self + 0x4ec);
    return (val >> 16) & 1;
}
// LLM-HARNESS-END: us-80083a84

// LLM-HARNESS-BEGIN: us-80083a90
extern "C" u32 func_8008310C__Q22cf13CfGameManagerFv(void* self) {
    u32 val = *(u32*)((u8*)self + 0x4ec);
    return (val >> 7) & 1;
}
// LLM-HARNESS-END: us-80083a90

// LLM-HARNESS-BEGIN: us-80083c08
extern "C" u32 func_80083284__Q22cf13CfGameManagerFv(void* self) {
    u32 val = *(u32*)((u8*)self + 0x4ec);
    return (val >> 20) & 1;
}
// LLM-HARNESS-END: us-80083c08

// LLM-HARNESS-BEGIN: us-80080fe0
extern "C" void CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv(void* self) {
    *(u32*)((u8*)self + 0x34) = 0;
}
// LLM-HARNESS-END: us-80080fe0

// LLM-HARNESS-BEGIN: us-80083ef4
extern u32 lbl_eu_80663E24;
extern "C" u32 func_80083538__Q22cf13CfGameManagerFv() {
    return (lbl_eu_80663E24 >> 5) & 1;
}
// LLM-HARNESS-END: us-80083ef4

// LLM-HARNESS-BEGIN: us-80083f00
extern u32 lbl_eu_80663E24;
extern "C" u32 func_80083544__Q22cf13CfGameManagerFv() {
    return (lbl_eu_80663E24 >> 4) & 1;
}
// LLM-HARNESS-END: us-80083f00

// LLM-HARNESS-BEGIN: us-80087c1c
extern u32 lbl_eu_80663E28;
extern "C" u32 func_80087244__Q22cf13CfGameManagerFv() {
    return (lbl_eu_80663E28 >> 20) & 1;
}
// LLM-HARNESS-END: us-80087c1c

// LLM-HARNESS-BEGIN: us-80087da0
extern "C" void* func_800873C8__Q22cf13CfGameManagerFv(void* self) {
    return (u8*)*(u32*)((u8*)self + 0x4) + 0x54;
}
// LLM-HARNESS-END: us-80087da0

// LLM-HARNESS-BEGIN: us-80087e14
extern "C" void func_8008743C__Q22cf13CfGameManagerFv(void* self) {
    *(u32*)((u8*)self + 0x8) = 0;
}
// LLM-HARNESS-END: us-80087e14

// LLM-HARNESS-BEGIN: us-800801bc
extern "C" void CActorParam_UnkVirtualFunc89__Q22cf11CActorParamFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x1608) = val;
}
// LLM-HARNESS-END: us-800801bc

// LLM-HARNESS-BEGIN: us-800801c4
extern "C" u32 CActorParam_UnkVirtualFunc91__Q22cf11CActorParamFv(void* self) {
    return *(u32*)((u8*)self + 0x1608);
}
// LLM-HARNESS-END: us-800801c4

// LLM-HARNESS-BEGIN: us-8008383c
extern "C" u32 CActorParam_UnkVirtualFunc127__Q22cf11CActorParamFv(void* self) {
    return *(u32*)((u8*)self + 0x15E0);
}
// LLM-HARNESS-END: us-8008383c

