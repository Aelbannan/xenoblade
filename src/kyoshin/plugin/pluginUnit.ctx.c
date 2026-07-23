

// LLM-HARNESS-BEGIN: us-8020123c
extern "C" void pluginUnitRegist() {
    extern const char lbl_eu_80507FC0[];
    extern const char lbl_eu_80535460[];
    extern void vmPluginRegist(const char*, const char*);
    vmPluginRegist(lbl_eu_80507FC0 + 0xb, lbl_eu_80535460);
}
// LLM-HARNESS-END: us-8020123c

// LLM-HARNESS-BEGIN: us-802005e4
extern "C" void getPcHp() {}
// LLM-HARNESS-END: us-802005e4
// LLM-HARNESS-BEGIN: us-80200680
extern "C" void getPcHpRate() {}
// LLM-HARNESS-END: us-80200680
// LLM-HARNESS-BEGIN: us-8020075c
extern "C" void getEneHp() {}
// LLM-HARNESS-END: us-8020075c
// LLM-HARNESS-BEGIN: us-802007f8
extern "C" void getEneHpRate() {}
// LLM-HARNESS-END: us-802007f8
// LLM-HARNESS-BEGIN: us-802008d4
extern "C" void setPcBtlState() {}
// LLM-HARNESS-END: us-802008d4
// LLM-HARNESS-BEGIN: us-80200ad4
extern "C" void clearPcBtlState() {}
// LLM-HARNESS-END: us-80200ad4
// LLM-HARNESS-BEGIN: us-80200b60
extern "C" void setEneBtlState() {}
// LLM-HARNESS-END: us-80200b60
// LLM-HARNESS-BEGIN: us-80200d50
extern "C" void clearEneBtlState() {}
// LLM-HARNESS-END: us-80200d50
// LLM-HARNESS-BEGIN: us-80200ddc
extern "C" void onPcArtsAttack() {}
// LLM-HARNESS-END: us-80200ddc
// LLM-HARNESS-BEGIN: us-80200f3c
extern "C" void onEneArtsAttack() {}
// LLM-HARNESS-END: us-80200f3c
// LLM-HARNESS-BEGIN: us-8020112c
extern "C" void synchro() {}
// LLM-HARNESS-END: us-8020112c
// LLM-HARNESS-BEGIN: us-802011c0
extern "C" void learnArts() {}
// LLM-HARNESS-END: us-802011c0
