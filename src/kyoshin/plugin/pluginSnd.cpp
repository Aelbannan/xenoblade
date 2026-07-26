// Auto-scaffolded catalog TU for kyoshin/plugin/pluginSnd
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void playBgm() {}

extern "C" void stopBgm() {}

extern "C" void setFieldBgm() {}

extern "C" void setTownBgm() {}

extern "C" void stopFieldBgm() {}

extern "C" void stopTownBgm() {}

extern "C" void forceFieldBgm() {}

extern "C" void setVolBgm() {}

extern "C" void playVoice() {}

extern "C" void stopVoice() {}

extern "C" void waitVoice() {}

extern "C" void playSeCommon() {}

extern "C" void playSeMap() {}

extern "C" void volSeMap() {}

extern "C" void stopSeCommon() {}

extern "C" void stopSeMap() {}

extern "C" void seCamPos() {}

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
