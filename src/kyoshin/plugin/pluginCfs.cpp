// Translation unit for kyoshin/plugin/pluginCfs
// Plugin script functions for the CFS (Common File System / script) subsystem.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/plugin/pluginCfs.hpp"

extern "C" void setMapJumpArea() {}

extern "C" void func_80047814__Q22cf13CfObjectPointFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

extern "C" void setMapJumpAreaBox() {}

extern "C" void setWarpArea() {}

extern "C" void setMapPreloadArea() {}

extern "C" void mapJump() {}

extern "C" void setMapPreloadArea2() {}

extern "C" void setEventArea() {}

extern "C" void delEventArea() {}

extern "C" void setTownArea() {}

extern "C" void addPopID() {}

extern "C" void setTimeSpeed() {}

extern "C" void setPopSheet() {}

extern "C" void changeWalker() {}

extern "C" void eventStart() {}

extern "C" unsigned long battleEventStart() {
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 |= 0x40000;
    return 0;
}

extern "C" void eventEnd() {}

extern "C" void delAttr() {}

extern "C" void setMono() {}

extern "C" void setMapDispID() {}

extern "C" void loadCfEvent() {}

extern "C" void waitCfEvent() {}

extern "C" void isMainParty() {}

extern "C" void isResvParty() {}

extern "C" void addParty() {}

extern "C" void makeParty() {}

extern "C" bool CfObject_UnkVirtualFunc31__Q22cf8CfObjectFv() { return false; }

extern "C" void makeGuestParty() {}

extern "C" void delParty() {}

extern "C" void addItem() {}

extern "C" void delItem() {}

extern "C" void totalItem() {}

extern "C" void equipItem() {}

extern "C" void equipWeapon() {}

extern "C" void getWeaponSlot() {}

bool func_8004A2E8() {
    return false;
}

extern "C" void setWeaponSlot() {}

void func_8004A400() {
    // Intentionally empty
}

extern "C" void waitEventRes() {}

extern "C" void setFade() {}

extern "C" void applyPcPrm() {}

extern "C" void setDispOffArea() {}

extern "C" void setScheduleType() {}

extern "C" void setWeather() {}

extern "C" void setWeatherArea() {}

extern "C" void setGimmick() {}

extern "C" void setElvGim() {}

extern "C" void setActMapObj() {}

extern "C" void getMapID() {}

extern "C" void clearGimmickJump() {}

extern "C" void partyMember() {}

extern "C" void clearPartyGauge() {}

extern "C" void waitPop() {}

extern "C" void partyWarp() {}

extern "C" void* CfObject_UnkVirtualFunc48__Q22cf12CfObjectMoveFv(void* self) {
    void* ptr = *(void**)((u8*)self + 0xC4);
    if (ptr == NULL) {
        return *(void**)((u8*)self + 0x6C0);
    }
    return *(void**)((u8*)ptr + 8);
}

extern "C" void setMoney() {}

extern "C" void addMoney() {}

extern "C" void isTimeSkip() {}

extern "C" void delHoldBox() {}

extern "C" void getWeaponID() {}

extern "C" int clearTbox() {
    extern void* func_800B07E8__Fv();
    extern void func_800B1AF4(void*);
    void* ptr = func_800B07E8__Fv();
    if (ptr) func_800B1AF4(ptr);
    return 0;
}

// Forward declarations for functions called from this unit
extern "C" void func_80043628();
extern "C" void func_801579A4();
extern "C" void func_800A3304();

extern "C" int returnTitle() {
    func_80043628();
    return 0;
}

extern "C" int dispLoading() {
    extern void func_8004302C(int, int);
    func_8004302C(1, 0);
    return 0;
}

extern "C" void addItemLimit() {}

extern "C" int clearItemLimit() {
    func_801579A4();
    return 0;
}

extern "C" int clearEquipGem() {
    func_800A3304();
    return 0;
}

extern "C" void setPcCtrl() {}

extern "C" unsigned long setFieldVision() {
    extern unsigned long lbl_eu_80663E28;
    lbl_eu_80663E28 |= 0x20000000;
    return 0;
}

extern "C" void saveNamedCount() {}

extern "C" void isPal() {}

extern "C" void setIgnorePal() {}

extern "C" void isVoiceJP() {}

extern "C" void pluginCfsRegist() {
    extern void vmPluginRegist(void*, void*);
    vmPluginRegist((void*)0x804FB046, (void*)0x80525F98);
}
