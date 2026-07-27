// Auto-scaffolded catalog TU for kyoshin/plugin/pluginSnd
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void playBgm(){}

void stopBgm(){}

void setFieldBgm(){}

void setTownBgm(){}

void stopFieldBgm(){}

void stopTownBgm(){}

void forceFieldBgm(){}

void setVolBgm(){}

void playVoice(){}

void stopVoice(){}

void waitVoice(){}

void playSeCommon(){}

void playSeMap(){}

void volSeMap(){}

void stopSeCommon(){}

void stopSeMap(){}

void seCamPos(){}

extern "C" int clearCamPos() {
    extern unsigned char lbl_eu_8066443A;
    extern void func_801AAD08();
    func_801AAD08();
    lbl_eu_8066443A = 0;
    return 0;
}

extern "C" void pluginSndRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_80504148[];
    extern char lbl_eu_805333F8[];
    vmPluginRegist((void*)((char*)lbl_eu_80504148 + 0x15), (void*)lbl_eu_805333F8);
}
