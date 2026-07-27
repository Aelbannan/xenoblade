// Translation unit for kyoshin/plugin/pluginCfs
// Plugin script functions for the CFS (Common File System / script) subsystem.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/plugin/pluginCfs.hpp"

void setMapJumpArea(){}

void func_80047814__Q22cf13CfObjectPointFv(void* self, void* src) {
    u32 a = *(u32*)((u8*)src + 0);
    u32 b = *(u32*)((u8*)src + 4);
    u32 c = *(u32*)((u8*)src + 8);
    *(u32*)((u8*)self + 0x3C) = a;
    *(u32*)((u8*)self + 0x40) = b;
    *(u32*)((u8*)self + 0x44) = c;
}

void setMapJumpAreaBox(){}

void setWarpArea(){}

void setMapPreloadArea(){}

void mapJump(){}

void setMapPreloadArea2(){}

void setEventArea(){}

void delEventArea(){}

void setTownArea(){}

void addPopID(){}

void setTimeSpeed(){}

void setPopSheet(){}

void changeWalker(){}

void eventStart(){}

unsigned long battleEventStart() {
    extern unsigned long lbl_eu_80663E24;
    lbl_eu_80663E24 |= 0x40000;
    return 0;
}

void eventEnd(){}

void delAttr(){}

void setMono(){}

void setMapDispID(){}

void loadCfEvent(){}

void waitCfEvent(){}

void isMainParty(){}

void isResvParty(){}

void addParty(){}

void makeParty(){}

void cf::CfObject::CfObject_UnkVirtualFunc31() { }

void makeGuestParty(){}

void delParty(){}

void addItem(){}

void delItem(){}

void totalItem(){}

void equipItem(){}

void equipWeapon(){}

void getWeaponSlot(){}

bool func_8004A2E8() {
    return false;
}

void setWeaponSlot(){}

void func_8004A400() {
    // Intentionally empty
}

void waitEventRes(){}

void setFade(){}

void applyPcPrm(){}

void setDispOffArea(){}

void setScheduleType(){}

void setWeather(){}

void setWeatherArea(){}

void setGimmick(){}

void setElvGim(){}

void setActMapObj(){}

void getMapID(){}

void clearGimmickJump(){}

void partyMember(){}

void clearPartyGauge(){}

void waitPop(){}

void partyWarp(){}

void* CfObject_UnkVirtualFunc48__Q22cf12CfObjectMoveFv(void* self) {
    void* ptr = *(void**)((u8*)self + 0xC4);
    if (ptr == NULL) {
        return *(void**)((u8*)self + 0x6C0);
    }
    return *(void**)((u8*)ptr + 8);
}

void setMoney(){}

void addMoney(){}

void isTimeSkip(){}

void delHoldBox(){}

void getWeaponID(){}

int clearTbox() {
    extern void* func_800B07E8__Fv();
    extern void func_800B1AF4(void*);
    void* ptr = func_800B07E8__Fv();
    if (ptr) func_800B1AF4(ptr);
    return 0;
}

// Forward declarations for functions called from this unit
void func_80043628();
void func_801579A4();
void func_800A3304();

int returnTitle() {
    func_80043628();
    return 0;
}

int dispLoading() {
    extern void func_8004302C(int, int);
    func_8004302C(1, 0);
    return 0;
}

void addItemLimit(){}

int clearItemLimit() {
    func_801579A4();
    return 0;
}

int clearEquipGem() {
    func_800A3304();
    return 0;
}

void setPcCtrl(){}

unsigned long setFieldVision() {
    extern unsigned long lbl_eu_80663E28;
    lbl_eu_80663E28 |= 0x20000000;
    return 0;
}

void saveNamedCount(){}

void isPal(){}

void setIgnorePal(){}

void isVoiceJP(){}

void pluginCfsRegist(){
    extern void vmPluginRegist(void*, void*);
    vmPluginRegist((void*)0x804FB046, (void*)0x80525F98);
}
