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

extern "C" void func_8006ACB0(void* self, float value) {
    *(float*)((uintptr_t)self + 0x280) = value;
}
extern "C" float func_8006ACB8(void* self) {
    return *(float*)((char*)self + 0x280);
}

void func_8006A82C(){}
void func_8006A8C4(){}
void func_8006A90C(){}
void func_8006A9F8(){}
void func_8006AA34(){}
void func_8006AA84(){}
void func_8006AAEC(){}
void func_8006AB40(){}
void func_8006AB94(){}
void func_8006AC10(){}
void func_8006AC60(){}
void func_8006ACC0(){}
