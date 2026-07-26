// Auto-scaffolded catalog TU for kyoshin/plugin/pluginBtl
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void startObserve() {}

extern "C" void endObserve() {}

extern "C" void defeatingCount() {}

extern "C" void isEnd() {}

extern "C" void end() {}

extern "C" void attack() {}

extern "C" void attackEne() {}

extern "C" void selectTgt() {}

extern "C" void vision() {}

extern "C" void voiceEvent() {}

extern "C" void isVoiceEvent() {}

extern "C" void unlockMonadoArts() {}

extern "C" void setTensionLv() {}

extern "C" void setTP() {}

extern "C" void breakVision() {}

extern "C" void setPTG() {}

extern "C" void getPTG() {}

extern "C" void test() {}

extern "C" void pluginBtlRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_805038B0[];
    extern char lbl_eu_80532360[];
    vmPluginRegist((void*)((char*)lbl_eu_805038B0 + 0xe), (void*)lbl_eu_80532360);
}
