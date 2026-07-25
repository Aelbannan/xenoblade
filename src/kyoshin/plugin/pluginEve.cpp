// Auto-scaffolded catalog TU for kyoshin/plugin/pluginEve
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// LLM-HARNESS-BEGIN: us-80046ec8
extern "C" void getFlag() {}
// LLM-HARNESS-END: us-80046ec8

// LLM-HARNESS-BEGIN: us-80046f80
extern "C" void setFlag() {}
// LLM-HARNESS-END: us-80046f80

// LLM-HARNESS-BEGIN: us-800470f0
extern "C" void getAwardFlagF16() {}
// LLM-HARNESS-END: us-800470f0

// LLM-HARNESS-BEGIN: us-8004714c
extern "C" void addAwardFlagF16() {}
// LLM-HARNESS-END: us-8004714c

// LLM-HARNESS-BEGIN: us-800471f4
extern "C" void setAwardFlagF1() {}
// LLM-HARNESS-END: us-800471f4

// LLM-HARNESS-BEGIN: us-8004722c
extern "C" void realtimeEventStart() {}
// LLM-HARNESS-END: us-8004722c

// LLM-HARNESS-BEGIN: us-800472c0
extern "C" void realtimeEventPlay() {}
// LLM-HARNESS-END: us-800472c0

// LLM-HARNESS-BEGIN: us-8004731c
extern "C" void func_80086490__Q22cf13CfGameManagerFv();

extern "C" int realtimeEventEnd(VMThread* pThread) {
    func_80086490__Q22cf13CfGameManagerFv();
    return 0;
}
// LLM-HARNESS-END: us-8004731c

// LLM-HARNESS-BEGIN: us-80047340
extern "C" void waitRealtimeEvent() {}
// LLM-HARNESS-END: us-80047340

// LLM-HARNESS-BEGIN: us-80047380
extern "C" void onTalk() {}
// LLM-HARNESS-END: us-80047380

// LLM-HARNESS-BEGIN: us-8004743c
extern "C" void onTalkEnd() {}
// LLM-HARNESS-END: us-8004743c

// LLM-HARNESS-BEGIN: us-800474f8
extern "C" void fadeIn() {}
// LLM-HARNESS-END: us-800474f8

// LLM-HARNESS-BEGIN: us-800475fc
extern "C" void fadeOut() {}
// LLM-HARNESS-END: us-800475fc

// LLM-HARNESS-BEGIN: us-800476fc
extern "C" void fadeWait() {}
// LLM-HARNESS-END: us-800476fc

// LLM-HARNESS-BEGIN: us-8004773c
extern "C" void checkEvent() {}
// LLM-HARNESS-END: us-8004773c

// LLM-HARNESS-BEGIN: us-80047780
extern "C" {
extern u32 lbl_eu_80663E28;
}
extern "C" int clearEventSkip() {
    lbl_eu_80663E28 &= ~0x100000;
    return 0;
}
// LLM-HARNESS-END: us-80047780

// LLM-HARNESS-BEGIN: us-80047794
extern "C" void isEvent() {}
// LLM-HARNESS-END: us-80047794

// LLM-HARNESS-BEGIN: us-800477d0
extern "C" void isTalkEvent() {}
// LLM-HARNESS-END: us-800477d0

// LLM-HARNESS-BEGIN: us-8004780c
extern "C" void isVisionEvent() {}
// LLM-HARNESS-END: us-8004780c

// LLM-HARNESS-BEGIN: us-80047848
extern "C" void pluginEveRegist() {
    extern void vmPluginRegist(void*, void*);
    extern char lbl_eu_804FACF0[];
    extern char lbl_eu_80525EF8[];
    vmPluginRegist((void*)lbl_eu_804FACF0, (void*)lbl_eu_80525EF8);
}
// LLM-HARNESS-END: us-80047848
