#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/CfPadData.hpp"

#include "monolib/core/CPadManager.hpp"

// destructor defined inline in CfGameManager.hpp
// Manually-named destructor wrapper for __dt__Q22cf13CfGameManagerFv
extern "C" void __dt__Q22cf13CfGameManagerFv(void* self) {
    static_cast<cf::CfGameManager*>(self)->~CfGameManager();
}

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
cf::CfObjectMove** cf::CfGameManager::func_8007C6B4(cf::CfObjectMove** slots,
                                                                    int index) {
    return &slots[index];
}
#pragma dont_inline off

void cf::CfGameManager::func_8007C188(unsigned long flags) {
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 |= flags;
}

void cf::CfGameManager::func_8007D190(unsigned long flags) {
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 &= ~flags;
}

void cf::CfGameManager::func_8007D794() {
    this->unk0 = 0;
    *(u32*)&this->unkC[0x14] = 0;
}

void cf::CfGameManager::func_8007E1B4() {}
void cf::CfGameManager::func_8007E218() {}
void cf::CfGameManager::func_8007F930() {}
void* cf::CfGameManager::func_80083298() {
    extern void* getInstance__Q22cf13CfGameManagerFv();
    return static_cast<cf::CfGameManager*>(getInstance__Q22cf13CfGameManagerFv())->unk90;
}
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
void cf::CfGameManager::func_80086B5C() {}
void cf::CfGameManager::enablePadFlags() {}
void cf::CfGameManager::getEnabledInputFlags() {}
void cf::CfGameManager::func_80086F9C() {}
void cf::CfGameManager::getPad(int) {}
void cf::CfGameManager::setPad() {}
void cf::CfGameManager::setCurrentPadPtr() {}
void func_80087588(){}
void cf::CfGameManager::func_8007EF48() {}
void cf::CfGameManager::func_8007FE18() {}
void cf::CfGameManager::func_8007FE1C() {}
void cf::CfGameManager::func_8007FE20() {}
int cf::CfGameManager::func_8007F0A4() { return 0; }
int cf::CfGameManager::func_8007F114() { return 0; }
int cf::CfGameManager::func_8007FE24() { return 0; }
int cf::CfObject::CfObject_UnkVirtualFunc53() { return 0; }
int cf::CfObject::CfObject_UnkVirtualFunc58() { return 0; }
// lwz getters — return global vars
extern u32 lbl_eu_80664188;
int cf::CfGameManager::func_80083458() { return lbl_eu_80664188; }
extern u32 lbl_eu_80664184;
int cf::CfGameManager::func_80083460() { return lbl_eu_80664184; }
int cf::CfGameManager::func_80086B1C() { return lbl_eu_80664184; }
extern u32 lbl_eu_8066409C;
int cf::CfGameManager::func_80086B24() { return lbl_eu_8066409C; }
extern u32 lbl_eu_806640A8;
int cf::CfGameManager::func_80086B2C() { return lbl_eu_806640A8; }
extern u32 lbl_eu_806640A4;
int cf::CfGameManager::func_80086B34() { return lbl_eu_806640A4; }
extern u32 lbl_eu_806640F4;
int cf::CfGameManager::func_80086B3C() { return lbl_eu_806640F4; }
extern u32 lbl_eu_80663D90;
int cf::CfGameManager::func_80086DB4() { return lbl_eu_80663D90; }
extern u8 lbl_eu_80663E34;
u8 cf::CfGameManager::func_8007F9BC() { return lbl_eu_80663E34; }
extern u8 lbl_eu_80663E5D;
u8 cf::CfGameManager::func_80080E20() { return lbl_eu_80663E5D; }
extern u8 lbl_eu_8066476D;
u8 cf::CfGameManager::func_80080E28() { return lbl_eu_8066476D; }
extern "C" u32 lbl_eu_8065FC18[];
void* cf::CfGameManager::func_8007DA00() { return (void*)lbl_eu_8065FC18; }

extern "C" void func_800B985C();
void cf::CfGameManager::func_80080F40() {
    func_800B985C();
}

void cf::CfGameManager::func_80080F44() {
    extern void func_800B9404(void);
    func_800B9404();
}

void cf::CfGameManager::func_80081CB8() {
    extern void func_800B9548(void);
    func_800B9548();
}

void cf::CfGameManager::func_80081D88() {
    extern void func_800B97A0(void);
    func_800B97A0();
}

void cf::CfGameManager::func_80082254() {
    extern void func_800B93D0(void);
    func_800B93D0();
}

void cf::CfGameManager::func_80082EC0() {
    extern void func_8026178C(void);
    func_8026178C();
}

extern "C" void func_800B6BA4__Fv();
void cf::CfGameManager::func_80086B04() {
    func_800B6BA4__Fv();
}

extern "C" void func_800B6BC8();
void cf::CfGameManager::func_80086B08() {
    func_800B6BC8();
}

extern "C" void func_800B6BEC();
void cf::CfGameManager::func_80086B0C() {
    func_800B6BEC();
}

extern "C" void func_800B6C10();
void cf::CfGameManager::func_80086B10() {
    func_800B6C10();
}

extern "C" void func_800B6C34();
void cf::CfGameManager::func_80086B14() {
    func_800B6C34();
}

extern "C" void func_800B8920();
void cf::CfGameManager::func_80086B18() {
    func_800B8920();
}

extern "C" void func_80141B54();
void cf::CfGameManager::func_80086B44() {
    func_80141B54();
}

extern "C" void func_80069EA0();
void cf::CfGameManager::func_80086B48() {
    func_80069EA0();
}

extern "C" void func_8006A12C();
void cf::CfGameManager::func_80086D90() {
    func_8006A12C();
}

extern "C" void func_8006A1A0();
void cf::CfGameManager::func_80086D94() {
    func_8006A1A0();
}

extern "C" void func_8006A234();
void cf::CfGameManager::func_80086D98() {
    func_8006A234();
}

extern "C" void func_8006A2E0();
void cf::CfGameManager::func_80086D9C() {
    func_8006A2E0();
}

extern "C" void func_8006A33C();
void cf::CfGameManager::func_80086DA0() {
    func_8006A33C();
}

extern "C" void func_8006A37C();
void cf::CfGameManager::func_80086DA4() {
    func_8006A37C();
}

extern "C" void func_8006A3BC();
void cf::CfGameManager::func_80086DA8() {
    func_8006A3BC();
}

extern "C" void func_8006A3FC();
void cf::CfGameManager::func_80086DAC() {
    func_8006A3FC();
}

extern "C" void func_8006A404();
void cf::CfGameManager::func_80086DB0() {
    func_8006A404();
}

extern "C" void func_8006A6D0();
void cf::CfGameManager::func_80086DBC() {
    func_8006A6D0();
}

u16 cf::CfGameManager::func_8007F8B8() {
    return static_cast<u16>(this->unk8C & 0xFFFF);
}

extern u16 lbl_eu_80663E3A;
u16 cf::CfGameManager::func_8007F9AC() {
    return lbl_eu_80663E3A;
}

extern u16 lbl_eu_80663E3A;
void cf::CfGameManager::func_8007F9B4(u16 val) {
    lbl_eu_80663E3A = val;
}

void cf::CfGameManager::func_80081264(u32 val) {
    this->unk4 = val;
}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_8008126C() {
    func_8009E120(reinterpret_cast<u32>(this), 0);
}

void cf::CfGameManager::func_80081274(u32 val) {
    this->unk8 = val;
}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_8008127C() {
    func_8009E120(reinterpret_cast<u32>(this), 1);
}

void cf::CfGameManager::func_80081284(u32 val) {
    *(u32*)&this->unkC[4] = val;
}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_8008128C() {
    func_8009E120(reinterpret_cast<u32>(this), 2);
}

void cf::CfGameManager::func_80081294(u32 val) {
    *(u32*)this->unkC = val;
}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_8008129C() {
    func_8009E120(reinterpret_cast<u32>(this), 3);
}

void cf::CfGameManager::func_800812A4(u32 val) {
    *(u32*)&this->unkC[8] = val;
}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_800812AC() {
    func_8009E120(reinterpret_cast<u32>(this), 4);
}

u16 cf::CfGameManager::func_800812B4() {
    return *(u16*)&this->unkC[2];
}

u16 cf::CfGameManager::func_800812BC() {
    return *(u16*)&this->unkC[4];
}

u16 cf::CfGameManager::func_800812C4() {
    return *(u16*)&this->unkC[6];
}

u16 cf::CfGameManager::func_800812CC() {
    return *(u16*)&this->unkC[8];
}

u16 cf::CfGameManager::func_800812D4() {
    return *(u16*)&this->unkC[10];
}

extern "C" void func_8009E120(u32, u32);
void cf::CfGameManager::func_800812DC() {
    func_8009E120(reinterpret_cast<u32>(this), 5);
}

u16 cf::CfGameManager::func_800812E4() {
    return *(u16*)this->unkC;
}

void cf::CfGameManager::func_800812EC(u16 val) {
    *(u16*)&this->unkC[24] = val;
}

void cf::CfGameManager::func_800812F4(u32 val) {
    this->unk0 = val;
}

void cf::CfGameManager::func_80081330(u32 val) {
    *(u32*)&this->unkC[12] = val;
}

void cf::CfGameManager::func_80081338(u16 val) {
    *(u16*)&this->unkC[20] = val;
}

void cf::CfGameManager::func_80081340(u16 val) {
    *(u16*)&this->unkC[22] = val;
}

void cf::CfGameManager::func_80081348(u16 val) {
    *(u16*)&this->unkC[16] = val;
}

void cf::CfGameManager::func_80081350(u16 val) {
    *(u16*)&this->unkC[18] = val;
}

u32 cf::CfGameManager::func_800817A8() {
    return this->unk8;
}

void cf::CfGameManager::func_80081874(u32 val) {
    *(u32*)&this->unk29[59] = val;
}

void cf::CfGameManager::func_80081988(u32 val) {
    this->unkA8 = val;
}

void cf::CfGameManager::func_80081CB0(u16 val) {
    *(u16*)&this->unk8C = val;
}

extern "C" void func_8009CF8C(u32);
void cf::CfGameManager::func_800822F4() {
    func_8009CF8C(0x20);
}

extern "C" void func_8009CF8C(u32);
void cf::CfGameManager::func_80082354() {
    func_8009CF8C(reinterpret_cast<u32>(this) + 0x220);
}

extern "C" void func_8009CF8C(u32);
void cf::CfGameManager::func_80082614() {
    func_8009CF8C(reinterpret_cast<u32>(this) + 0x312c);
}

extern "C" void func_8009CF8C(u32);
void cf::CfGameManager::func_80082694() {
    func_8009CF8C(reinterpret_cast<u32>(this) + 0x40);
}

u32 cf::CfGameManager::func_80082768() {
    return *(u32*)&this->unk29[31];
}

s16 cf::CfGameManager::func_80083290() {
    return *(s16*)((u8*)this + 0x532);
}

extern u8 lbl_eu_80664298;
void cf::CfGameManager::func_80083468(u8 val) {
    lbl_eu_80664298 = val;
}

void* cf::CfGameManager::func_80083C70() {
    return (u8*)this + 0x219c;
}

extern u32 lbl_eu_80663E14;
extern "C" void func_80496034(u32);
void cf::CfGameManager::func_80085838() {
    func_80496034(lbl_eu_80663E14);
}

float cf::CfObject::CfObject_UnkVirtualFunc56() {
    extern float lbl_eu_80666498;
    return lbl_eu_80666498;
}

u32 cf::CfGameManager::func_80087424() {
    return this->unk8;
}

extern u32 lbl_eu_80663E24;
u32 cf::CfGameManager::func_8007CBC8() {
    return (lbl_eu_80663E24 >> 7) & 1;
}

u32 cf::CfGameManager::func_8007EEF8() {
    return this->unk0 >> 20;
}

void* cf::CfGameManager::func_8007F8D0() {
    return reinterpret_cast<u8*>(reinterpret_cast<u32*>(this->unk0)) + 0x8;
}

void cf::CfGameManager::func_8007F8F4(void* src) {
    this->unk0 = *(u32*)((const u8*)src + 0x4);
}

extern void __fill_mem(void*, int, int);
void cf::CfGameManager::func_80081258() {
    __fill_mem(this, 0, 0x28);
}

extern u32 lbl_eu_80663E24;
u32 cf::CfGameManager::func_800817B0() {
    return (*(u32*)((u8*)this + 0x82c) >> 2) & 1;
}

extern "C" void func_80141C6C(void*, void*);
void cf::CfGameManager::func_80082940(void* a, void* b) {
    func_80141C6C(a, b);
}

u32 cf::CfGameManager::func_80083100() {
    u32 val = *(u32*)((u8*)this + 0x4ec);
    return (val >> 16) & 1;
}

u32 cf::CfGameManager::func_8008310C() {
    u32 val = *(u32*)((u8*)this + 0x4ec);
    return (val >> 7) & 1;
}

u32 cf::CfGameManager::func_80083284() {
    u32 val = *(u32*)((u8*)this + 0x4ec);
    return (val >> 20) & 1;
}

void cf::CObjectParam::CObjectParam_UnkVirtualFunc4() {
    *(u32*)((u8*)this + 0x34) = 0;
}

extern u32 lbl_eu_80663E24;
u32 cf::CfGameManager::func_80083538() {
    return (lbl_eu_80663E24 >> 5) & 1;
}

extern u32 lbl_eu_80663E24;
u32 cf::CfGameManager::func_80083544() {
    return (lbl_eu_80663E24 >> 4) & 1;
}

extern u32 lbl_eu_80663E28;
u32 cf::CfGameManager::func_80087244() {
    return (lbl_eu_80663E28 >> 20) & 1;
}

void* cf::CfGameManager::func_800873C8() {
    return (u8*)this->unk4 + 0x54;
}

void cf::CfGameManager::func_8008743C() {
    this->unk8 = 0;
}

void cf::CActorParam::CActorParam_UnkVirtualFunc89(u32 val) {
    *(u32*)((u8*)this + 0x1608) = val;
}

u32 cf::CActorParam::CActorParam_UnkVirtualFunc91() {
    return *(u32*)((u8*)this + 0x1608);
}

u32 cf::CActorParam::CActorParam_UnkVirtualFunc127() {
    return *(u32*)((u8*)this + 0x15E0);
}

