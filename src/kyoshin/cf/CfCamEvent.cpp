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
