#include "kyoshin/cf/CfCamEvent.hpp"

extern "C" {
s8 lbl_eu_80663DB0;
char* lbl_eu_80663DB4;
s8 lbl_eu_80663DB8;
char* lbl_eu_80663DBC;
s8 lbl_eu_80663DC0;
char* lbl_eu_80663DC4;

extern char lbl_eu_80527130[];
extern char lbl_eu_80527108[];
extern char lbl_eu_805270D8[];
}

extern "C" char** CfCamEvent_initCamIntfInstances(cf::CfCamEvent* self) {
    if (!lbl_eu_80663DB0) {
        lbl_eu_80663DB4 = lbl_eu_80527130;
        lbl_eu_80663DB0 = 1;
    }
    if (!lbl_eu_80663DB8) {
        lbl_eu_80663DBC = lbl_eu_80527108;
        lbl_eu_80663DB8 = 1;
    }
    if (!lbl_eu_80663DC0) {
        lbl_eu_80663DC4 = lbl_eu_805270D8;
        lbl_eu_80663DC0 = 1;
    }

    s32 type = self->unk8;
    char** result = &lbl_eu_80663DBC;
    if (type == 8) {
        return &lbl_eu_80663DBC;
    }
    if (type == 9) {
        return &lbl_eu_80663DB4;
    }
    if (type == 10) {
        return &lbl_eu_80663DC4;
    }
    return result;
}

// LLM-HARNESS-BEGIN: us-8006b708
extern "C" bool func_8006ACB0() { return false; }
// LLM-HARNESS-END: us-8006b708
// LLM-HARNESS-BEGIN: us-8006b710
extern "C" bool func_8006ACB8() { return false; }
// LLM-HARNESS-END: us-8006b710

// LLM-HARNESS-BEGIN: us-8006b284
extern "C" void func_8006A82C() {}
// LLM-HARNESS-END: us-8006b284
// LLM-HARNESS-BEGIN: us-8006b31c
extern "C" void func_8006A8C4() {}
// LLM-HARNESS-END: us-8006b31c
// LLM-HARNESS-BEGIN: us-8006b364
extern "C" void func_8006A90C() {}
// LLM-HARNESS-END: us-8006b364
// LLM-HARNESS-BEGIN: us-8006b450
extern "C" void func_8006A9F8() {}
// LLM-HARNESS-END: us-8006b450
// LLM-HARNESS-BEGIN: us-8006b48c
extern "C" void func_8006AA34() {}
// LLM-HARNESS-END: us-8006b48c
// LLM-HARNESS-BEGIN: us-8006b4dc
extern "C" void func_8006AA84() {}
// LLM-HARNESS-END: us-8006b4dc
// LLM-HARNESS-BEGIN: us-8006b544
extern "C" void func_8006AAEC() {}
// LLM-HARNESS-END: us-8006b544
// LLM-HARNESS-BEGIN: us-8006b598
extern "C" void func_8006AB40() {}
// LLM-HARNESS-END: us-8006b598
// LLM-HARNESS-BEGIN: us-8006b5ec
extern "C" void func_8006AB94() {}
// LLM-HARNESS-END: us-8006b5ec
// LLM-HARNESS-BEGIN: us-8006b668
extern "C" void func_8006AC10() {}
// LLM-HARNESS-END: us-8006b668
// LLM-HARNESS-BEGIN: us-8006b6b8
extern "C" void func_8006AC60() {}
// LLM-HARNESS-END: us-8006b6b8
// LLM-HARNESS-BEGIN: us-8006b718
extern "C" void func_8006ACC0() {}
// LLM-HARNESS-END: us-8006b718
