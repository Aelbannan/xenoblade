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

extern "C" void func_8007C188__Q22cf13CfGameManagerFv(unsigned long flags) {
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 |= flags;
}

extern "C" void func_8007D190__Q22cf13CfGameManagerFv(unsigned long flags) {
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 &= ~flags;
}

extern "C" void func_8007D794__Q22cf13CfGameManagerFv(void* self) {
    *(unsigned char*)self = 0;
    *(unsigned long*)((char*)self + 0x20) = 0;
}

extern "C" void func_8007E1B4__Q22cf13CfGameManagerFv() {}
extern "C" void func_8007E218__Q22cf13CfGameManagerFv() {}
extern "C" void func_8007F930__Q22cf13CfGameManagerFv() {}
extern "C" void* func_80083298__Q22cf13CfGameManagerFv() {
    extern void* getInstance__Q22cf13CfGameManagerFv();
    return *(void**)((char*)getInstance__Q22cf13CfGameManagerFv() + 0x90);
}
extern "C" void func_800B76CC();
extern "C" void func_eu_800874CC() { func_800B76CC(); }
extern "C" void func_800B76F4();

extern "C" void func_eu_800874D0()
{
    func_800B76F4();
}
extern "C" void func_800B781C(void);

extern "C" void func_eu_800874D4(void) {
    func_800B781C();
}
extern "C" void func_800B7854();

extern "C" void func_eu_800874D8()
{
    func_800B7854();
}
extern "C" void func_80086B5C__Q22cf13CfGameManagerFv() {}
extern "C" void enablePadFlags__Q22cf13CfGameManagerFUlb() {}
extern "C" void getEnabledInputFlags__Q22cf13CfGameManagerFv() {}
extern "C" void func_80086F9C__Q22cf13CfGameManagerFv() {}
extern "C" void getPad__Q22cf13CfGameManagerFi(int) {}
extern "C" void setPad__Q22cf13CfGameManagerFv() {}
extern "C" void setCurrentPadPtr__Q22cf13CfGameManagerFPC4CPadUl() {}
extern "C" void func_80087588() {}
extern "C" void func_8007EF48__Q22cf13CfGameManagerFv() {}
extern "C" void func_8007FE18__Q22cf13CfGameManagerFv() {}
extern "C" void func_8007FE1C__Q22cf13CfGameManagerFv() {}
extern "C" void func_8007FE20__Q22cf13CfGameManagerFv() {}
extern "C" int func_8007F0A4__Q22cf13CfGameManagerFv() { return 0; }
extern "C" int func_8007F114__Q22cf13CfGameManagerFv() { return 0; }
extern "C" int func_8007FE24__Q22cf13CfGameManagerFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc53__Q22cf8CfObjectFv() { return 0; }
extern "C" int CfObject_UnkVirtualFunc58__Q22cf8CfObjectFv() { return 0; }
// lwz getters — return global vars
extern u32 lbl_eu_80664188;
extern "C" int func_80083458__Q22cf13CfGameManagerFv() { return lbl_eu_80664188; }
extern u32 lbl_eu_80664184;
extern "C" int func_80083460__Q22cf13CfGameManagerFv() { return lbl_eu_80664184; }
extern "C" int func_80086B1C__Q22cf13CfGameManagerFv() { return lbl_eu_80664184; }
extern u32 lbl_eu_8066409C;
extern "C" int func_80086B24__Q22cf13CfGameManagerFv() { return lbl_eu_8066409C; }
extern u32 lbl_eu_806640A8;
extern "C" int func_80086B2C__Q22cf13CfGameManagerFv() { return lbl_eu_806640A8; }
extern u32 lbl_eu_806640A4;
extern "C" int func_80086B34__Q22cf13CfGameManagerFv() { return lbl_eu_806640A4; }
extern u32 lbl_eu_806640F4;
extern "C" int func_80086B3C__Q22cf13CfGameManagerFv() { return lbl_eu_806640F4; }
extern u32 lbl_eu_80663D90;
extern "C" int func_80086DB4__Q22cf13CfGameManagerFv() { return lbl_eu_80663D90; }
extern u8 lbl_eu_80663E34;
extern "C" u8 func_8007F9BC__Q22cf13CfGameManagerFv() { return lbl_eu_80663E34; }
extern u8 lbl_eu_80663E5D;
extern "C" u8 func_80080E20__Q22cf13CfGameManagerFv() { return lbl_eu_80663E5D; }
extern u8 lbl_eu_8066476D;
extern "C" u8 func_80080E28__Q22cf13CfGameManagerFv() { return lbl_eu_8066476D; }
extern "C" u32 lbl_eu_8065FC18[];
extern "C" void* func_8007DA00__Q22cf13CfGameManagerFv() { return (void*)lbl_eu_8065FC18; }

extern "C" void func_800B985C();
extern "C" void func_80080F40__Q22cf13CfGameManagerFv() {
    func_800B985C();
}

extern "C" void func_80080F44__Q22cf13CfGameManagerFv() {
    extern void func_800B9404(void);
    func_800B9404();
}

extern "C" void func_80081CB8__Q22cf13CfGameManagerFv() {
    extern void func_800B9548(void);
    func_800B9548();
}

extern "C" void func_80081D88__Q22cf13CfGameManagerFv() {
    extern void func_800B97A0(void);
    func_800B97A0();
}

extern "C" void func_80082254__Q22cf13CfGameManagerFv() {
    extern void func_800B93D0(void);
    func_800B93D0();
}

extern "C" void func_80082EC0__Q22cf13CfGameManagerFv() {
    extern void func_8026178C(void);
    func_8026178C();
}

extern "C" void func_800B6BA4__Fv();
extern "C" void func_80086B04__Q22cf13CfGameManagerFv() {
    func_800B6BA4__Fv();
}

extern "C" void func_800B6BC8();
extern "C" void func_80086B08__Q22cf13CfGameManagerFv() {
    func_800B6BC8();
}

extern "C" void func_800B6BEC();
extern "C" void func_80086B0C__Q22cf13CfGameManagerFv() {
    func_800B6BEC();
}

extern "C" void func_800B6C10();
extern "C" void func_80086B10__Q22cf13CfGameManagerFv() {
    func_800B6C10();
}

extern "C" void func_800B6C34();
extern "C" void func_80086B14__Q22cf13CfGameManagerFv() {
    func_800B6C34();
}

extern "C" void func_800B8920();
extern "C" void func_80086B18__Q22cf13CfGameManagerFv() {
    func_800B8920();
}

extern "C" void func_80141B54();
extern "C" void func_80086B44__Q22cf13CfGameManagerFv() {
    func_80141B54();
}

extern "C" void func_80069EA0();
extern "C" void func_80086B48__Q22cf13CfGameManagerFv() {
    func_80069EA0();
}

extern "C" void func_8006A12C();
extern "C" void func_80086D90__Q22cf13CfGameManagerFv() {
    func_8006A12C();
}

extern "C" void func_8006A1A0();
extern "C" void func_80086D94__Q22cf13CfGameManagerFv() {
    func_8006A1A0();
}

extern "C" void func_8006A234();
extern "C" void func_80086D98__Q22cf13CfGameManagerFv() {
    func_8006A234();
}

extern "C" void func_8006A2E0();
extern "C" void func_80086D9C__Q22cf13CfGameManagerFv() {
    func_8006A2E0();
}

extern "C" void func_8006A33C();
extern "C" void func_80086DA0__Q22cf13CfGameManagerFv() {
    func_8006A33C();
}

extern "C" void func_8006A37C();
extern "C" void func_80086DA4__Q22cf13CfGameManagerFv() {
    func_8006A37C();
}

extern "C" void func_8006A3BC();
extern "C" void func_80086DA8__Q22cf13CfGameManagerFv() {
    func_8006A3BC();
}

extern "C" void func_8006A3FC();
extern "C" void func_80086DAC__Q22cf13CfGameManagerFv() {
    func_8006A3FC();
}

extern "C" void func_8006A404();
extern "C" void func_80086DB0__Q22cf13CfGameManagerFv() {
    func_8006A404();
}

extern "C" void func_8006A6D0();
extern "C" void func_80086DBC__Q22cf13CfGameManagerFv() {
    func_8006A6D0();
}

extern "C" u16 func_8007F8B8__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0x8c);
}

extern u16 lbl_eu_80663E3A;
extern "C" u16 func_8007F9AC__Q22cf13CfGameManagerFv() {
    return lbl_eu_80663E3A;
}

extern u16 lbl_eu_80663E3A;
extern "C" void func_8007F9B4__Q22cf13CfGameManagerFv(u16 val) {
    lbl_eu_80663E3A = val;
}

extern "C" void func_80081264__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x4) = val;
}

extern "C" void func_8009E120(u32, u32);
extern "C" void func_8008126C__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 0);
}

extern "C" void func_80081274__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x8) = val;
}

extern "C" void func_8009E120(u32, u32);
extern "C" void func_8008127C__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 1);
}

extern "C" void func_80081284__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x10) = val;
}

extern "C" void func_8009E120(u32, u32);
extern "C" void func_8008128C__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 2);
}

extern "C" void func_80081294__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0xc) = val;
}

extern "C" void func_8009E120(u32, u32);
extern "C" void func_8008129C__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 3);
}

extern "C" void func_800812A4__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x14) = val;
}

extern "C" void func_8009E120(u32, u32);
extern "C" void func_800812AC__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 4);
}

extern "C" u16 func_800812B4__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0xe);
}

extern "C" u16 func_800812BC__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0x10);
}

extern "C" u16 func_800812C4__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0x12);
}

extern "C" u16 func_800812CC__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0x14);
}

extern "C" u16 func_800812D4__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0x16);
}

extern "C" void func_8009E120(u32, u32);
extern "C" void func_800812DC__Q22cf13CfGameManagerFv(u32 self) {
    func_8009E120(self, 5);
}

extern "C" u16 func_800812E4__Q22cf13CfGameManagerFv(void* self) {
    return *(u16*)((u8*)self + 0xc);
}

extern "C" void func_800812EC__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x24) = val;
}

extern "C" void func_800812F4__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x0) = val;
}

extern "C" void func_80081330__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x18) = val;
}

extern "C" void func_80081338__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x20) = val;
}

extern "C" void func_80081340__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x22) = val;
}

extern "C" void func_80081348__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x1c) = val;
}

extern "C" void func_80081350__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x1e) = val;
}

extern "C" u32 func_800817A8__Q22cf13CfGameManagerFv(void* self) {
    return *(u32*)((u8*)self + 0x8);
}

extern "C" void func_80081874__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x64) = val;
}

extern "C" void func_80081988__Q22cf13CfGameManagerFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0xa8) = val;
}

extern "C" void func_80081CB0__Q22cf13CfGameManagerFv(void* self, u16 val) {
    *(u16*)((u8*)self + 0x8c) = val;
}

extern "C" void func_8009CF8C(u32);
extern "C" void func_800822F4__Q22cf13CfGameManagerFv() {
    func_8009CF8C(0x20);
}

extern "C" void func_8009CF8C(u32);
extern "C" void func_80082354__Q22cf13CfGameManagerFv(u32 self) {
    func_8009CF8C(self + 0x220);
}

extern "C" void func_8009CF8C(u32);
extern "C" void func_80082614__Q22cf13CfGameManagerFv(u32 self) {
    func_8009CF8C(self + 0x312c);
}

extern "C" void func_8009CF8C(u32);
extern "C" void func_80082694__Q22cf13CfGameManagerFv(u32 self) {
    func_8009CF8C(self + 0x40);
}

extern "C" u32 func_80082768__Q22cf13CfGameManagerFv(void* self) {
    return *(u32*)((u8*)self + 0x48);
}

extern "C" s16 func_80083290__Q22cf13CfGameManagerFv(void* self) {
    return *(s16*)((u8*)self + 0x532);
}

extern u8 lbl_eu_80664298;
extern "C" void func_80083468__Q22cf13CfGameManagerFv(u8 val) {
    lbl_eu_80664298 = val;
}

extern "C" void* func_80083C70__Q22cf13CfGameManagerFv(void* self) {
    return (u8*)self + 0x219c;
}

extern u32 lbl_eu_80663E14;
extern "C" void func_80496034(u32);
extern "C" void func_80085838__Q22cf13CfGameManagerFv() {
    func_80496034(lbl_eu_80663E14);
}

extern "C" float CfObject_UnkVirtualFunc56__Q22cf8CfObjectFv() {
    extern float lbl_eu_80666498;
    return lbl_eu_80666498;
}

extern "C" u32 func_80087424__Q22cf13CfGameManagerFv(void* self) {
    return *(u32*)((u8*)self + 0x8);
}

extern u32 lbl_eu_80663E24;
extern "C" u32 func_8007CBC8__Q22cf13CfGameManagerFv() {
    return (lbl_eu_80663E24 >> 7) & 1;
}

extern "C" u32 func_8007EEF8__Q22cf13CfGameManagerFv(void* self) {
    return *(u32*)((u8*)self + 0x0) >> 20;
}

extern "C" void* func_8007F8D0__Q22cf13CfGameManagerFv(void* self) {
    return (u8*)*(u32**)((u8*)self + 0x0) + 0x8;
}

extern "C" void func_8007F8F4__Q22cf13CfGameManagerFv(void* self, void* src) {
    *(u32*)((u8*)self + 0x0) = *(u32*)((u8*)src + 0x4);
}

extern void __fill_mem(void*, int, int);
extern "C" void func_80081258__Q22cf13CfGameManagerFv(void* self) {
    __fill_mem(self, 0, 0x28);
}

extern u32 lbl_eu_80663E24;
extern "C" u32 func_800817B0__Q22cf13CfGameManagerFv(void* self) {
    return (*(u32*)((u8*)self + 0x82c) >> 2) & 1;
}

extern "C" void func_80141C6C(void*, void*);
extern "C" void func_80082940__Q22cf13CfGameManagerFv(void* self, void* a, void* b) {
    func_80141C6C(a, b);
}

extern "C" u32 func_80083100__Q22cf13CfGameManagerFv(void* self) {
    u32 val = *(u32*)((u8*)self + 0x4ec);
    return (val >> 16) & 1;
}

extern "C" u32 func_8008310C__Q22cf13CfGameManagerFv(void* self) {
    u32 val = *(u32*)((u8*)self + 0x4ec);
    return (val >> 7) & 1;
}

extern "C" u32 func_80083284__Q22cf13CfGameManagerFv(void* self) {
    u32 val = *(u32*)((u8*)self + 0x4ec);
    return (val >> 20) & 1;
}

extern "C" void CObjectParam_UnkVirtualFunc4__Q22cf12CObjectParamFv(void* self) {
    *(u32*)((u8*)self + 0x34) = 0;
}

extern u32 lbl_eu_80663E24;
extern "C" u32 func_80083538__Q22cf13CfGameManagerFv() {
    return (lbl_eu_80663E24 >> 5) & 1;
}

extern u32 lbl_eu_80663E24;
extern "C" u32 func_80083544__Q22cf13CfGameManagerFv() {
    return (lbl_eu_80663E24 >> 4) & 1;
}

extern u32 lbl_eu_80663E28;
extern "C" u32 func_80087244__Q22cf13CfGameManagerFv() {
    return (lbl_eu_80663E28 >> 20) & 1;
}

extern "C" void* func_800873C8__Q22cf13CfGameManagerFv(void* self) {
    return (u8*)*(u32*)((u8*)self + 0x4) + 0x54;
}

extern "C" void func_8008743C__Q22cf13CfGameManagerFv(void* self) {
    *(u32*)((u8*)self + 0x8) = 0;
}

extern "C" void CActorParam_UnkVirtualFunc89__Q22cf11CActorParamFv(void* self, u32 val) {
    *(u32*)((u8*)self + 0x1608) = val;
}

extern "C" u32 CActorParam_UnkVirtualFunc91__Q22cf11CActorParamFv(void* self) {
    return *(u32*)((u8*)self + 0x1608);
}

extern "C" u32 CActorParam_UnkVirtualFunc127__Q22cf11CActorParamFv(void* self) {
    return *(u32*)((u8*)self + 0x15E0);
}

