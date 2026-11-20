// Auto-scaffolded catalog TU for kyoshin/plugin/ocCfp
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocCfp.hpp"

extern "C" {
unsigned short getControllerWordA33C__Q22cf13CfGameManagerFv();
unsigned short getControllerWordA37C__Q22cf13CfGameManagerFv();
void loadControllerConfigA__Q22cf13CfGameManagerFv(unsigned short);
void loadControllerConfigB__Q22cf13CfGameManagerFv(unsigned short);
}

extern "C" int getType9Value(VMThread* ths, int a, int val){
    VMArg arg;
    arg.type = 9;
    arg.unk2 = val;
    arg.value.intVal = 0;
    vmRetValSet(ths, &arg);
    return 1;
}

extern "C" int getCtrlWordA33C(VMThread* ths){
    VMArg arg;
    arg.type = 3;
    arg.value.intVal = getControllerWordA33C__Q22cf13CfGameManagerFv();
    vmRetValSet(ths, &arg);
    return 1;
}

extern "C" int getCtrlWordA37C(VMThread* ths){
    VMArg arg;
    arg.type = 3;
    arg.value.intVal = getControllerWordA37C__Q22cf13CfGameManagerFv();
    vmRetValSet(ths, &arg);
    return 1;
}

extern "C" int getCtrlWordDiv3(VMThread* ths){
    VMArg arg;
    arg.type = 3;
    arg.value.intVal = getControllerWordA33C__Q22cf13CfGameManagerFv() / 3;
    vmRetValSet(ths, &arg);
    return 1;
}

extern "C" int setCtrlConfigA(VMThread* ths){
    VMArg* prop = (VMArg*)vmOCPropertyGet(ths);
    u32 val = prop->value.uintVal;
    loadControllerConfigA__Q22cf13CfGameManagerFv(val & 0xFFFF);
    return 0;
}

extern "C" int setCtrlConfigB(VMThread* ths){
    VMArg* prop = (VMArg*)vmOCPropertyGet(ths);
    u32 val = prop->value.uintVal;
    loadControllerConfigB__Q22cf13CfGameManagerFv(val & 0xFFFF);
    return 0;
}

extern "C" int getTimeIdxMin(VMThread* ths){
    VMArg* argPtr = vmArgPtrGet(ths, 1);
    int idx = vmArgIntGet(2, argPtr);
    VMArg arg;
    if (idx > 7) {
        arg.type = 3;
        arg.value.intVal = 0;
        vmOCExceptionThrow(ths);
        return 0;
    }
    arg.type = 3;
    arg.value.intVal = idx * 3;
    vmRetValSet(ths, &arg);
    return 1;
}

extern "C" int getTimeIdxMax(VMThread* ths){
    VMArg* argPtr = vmArgPtrGet(ths, 1);
    int idx = vmArgIntGet(2, argPtr);
    VMArg arg;
    if (idx > 7) {
        arg.type = 3;
        arg.value.intVal = 0;
        vmOCExceptionThrow(ths);
        return 0;
    }
    int val = idx + 1;
    arg.value.intVal = val * 3;
    arg.type = 3;
    vmRetValSet(ths, &arg);
    return 1;
}

extern "C" {
BOOL vmOCRegist(OCData* pOC);
}

// ----- Retail data tables (all defined by this TU) -----
// name-guess: VM "cfp" property table registered by ocCfpRegist; retail
// .data object 0x80525CF0 (three entries plus a null terminator).
static OCProperty sCfpProperties[4] = {
    { "hour", (OCGetSetFunc)getCtrlWordA33C, (OCGetSetFunc)setCtrlConfigA, 0 },
    { "min", (OCGetSetFunc)getCtrlWordA37C, (OCGetSetFunc)setCtrlConfigB, 0 },
    { "timeIdx", (OCGetSetFunc)getCtrlWordDiv3, 0, 0 },
    { 0, 0, 0, 0 },
};

// name-guess: VM "cfp" selector table registered by ocCfpRegist; retail
// .data object 0x80525D30 (two entries plus a null terminator).
static OCSelector sCfpSelectors[3] = {
    { "getTimeIdxMin", (OCSelectorFunc)getTimeIdxMin, 0 },
    { "getTimeIdxMax", (OCSelectorFunc)getTimeIdxMax, 0 },
    { 0, 0, 0 },
};

// name-guess: root OCData for the script-facing "cfp" object; retail
// .data object 0x80525D58.
static OCData sCfpData[1] = {
    { "cfp", (OCCtorFunc)getType9Value, sCfpProperties, sCfpSelectors },
};

void ocCfpRegist() {
    vmOCRegist(sCfpData);
}
