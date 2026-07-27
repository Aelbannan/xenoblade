// Auto-scaffolded catalog TU for kyoshin/plugin/pluginEve
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

void getFlag(){}

void setFlag(){}

void getAwardFlagF16(){}

void addAwardFlagF16(){}

void setAwardFlagF1(){}

void realtimeEventStart(){}

void realtimeEventPlay(){}

void func_80086490__Q22cf13CfGameManagerFv();

int realtimeEventEnd(VMThread* pThread) {
    func_80086490__Q22cf13CfGameManagerFv();
    return 0;
}

void waitRealtimeEvent(){}

void onTalk(){}

void onTalkEnd(){}

void fadeIn(){}

void fadeOut(){}

void fadeWait(){}

void checkEvent(){}

{
extern u32 lbl_eu_80663E28;
}
int clearEventSkip() {
    lbl_eu_80663E28 &= ~0x100000;
    return 0;
}

void isEvent(){}

void isTalkEvent(){}

void isVisionEvent(){}

extern "C" void pluginEveRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_804FACF0[];
    extern char lbl_eu_80525EF8[];
    vmPluginRegist((void*)lbl_eu_804FACF0, (void*)lbl_eu_80525EF8);
}
