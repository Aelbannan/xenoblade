// Auto-scaffolded catalog TU for kyoshin/plugin/pluginEve
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void getFlag() {}

extern "C" void setFlag() {}

extern "C" void getAwardFlagF16() {}

extern "C" void addAwardFlagF16() {}

extern "C" void setAwardFlagF1() {}

extern "C" void realtimeEventStart() {}

extern "C" void realtimeEventPlay() {}

extern "C" void func_80086490__Q22cf13CfGameManagerFv();

extern "C" int realtimeEventEnd(VMThread* pThread) {
    func_80086490__Q22cf13CfGameManagerFv();
    return 0;
}

extern "C" void waitRealtimeEvent() {}

extern "C" void onTalk() {}

extern "C" void onTalkEnd() {}

extern "C" void fadeIn() {}

extern "C" void fadeOut() {}

extern "C" void fadeWait() {}

extern "C" void checkEvent() {}

extern "C" {
extern u32 lbl_eu_80663E28;
}
extern "C" int clearEventSkip() {
    lbl_eu_80663E28 &= ~0x100000;
    return 0;
}

extern "C" void isEvent() {}

extern "C" void isTalkEvent() {}

extern "C" void isVisionEvent() {}

extern "C" void pluginEveRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_804FACF0[];
    extern char lbl_eu_80525EF8[];
    vmPluginRegist((void*)lbl_eu_804FACF0, (void*)lbl_eu_80525EF8);
}
