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
extern OCData lbl_eu_80525D58[];
void ocCfpRegist() {
    vmOCRegist(lbl_eu_80525D58);
}

// ----- Retail data sections (all defined by this TU) -----
// .rodata time-selector names (unsized; MWCC 4-packs them to 0x20),
// .sdata2 property/entry names (each sized to its 8-byte retail slot for a
// deterministic 0x20), .data OC property/selector tables + the OCData root
// (80525D58, the frontier label). Property/selector length words are 0 in
// retail; null entries terminate each table. The .data pieces after the
// first land on 8-byte boundaries (retail layout; MWCC places them so).
// Function-pointer casts bridge the VM table types (OCGetSetFunc et al.)
// and the plain (VMThread*) handlers; only addresses are emitted.
__declspec(section ".rodata") __attribute__((used))
char lbl_eu_804FA9D0[] = "getTimeIdxMin";
__declspec(section ".rodata") __attribute__((used)) char lbl_eu_804FA9E0[0x10] = "getTimeIdxMax";
__declspec(section ".sdata2") __attribute__((used)) char lbl_eu_80665D98[8] = "hour";
__declspec(section ".sdata2") __attribute__((used)) char lbl_eu_80665DA0[8] = "min";
__declspec(section ".sdata2") __attribute__((used)) char lbl_eu_80665DA8[8] = "timeIdx";
__declspec(section ".sdata2") __attribute__((used)) char lbl_eu_80665DB0[8] = "cfp";
__declspec(section ".data") __attribute__((used)) OCProperty lbl_eu_80525CF0[4] = {
    { lbl_eu_80665D98, (OCGetSetFunc)getCtrlWordA33C, (OCGetSetFunc)setCtrlConfigA, 0 },
    { lbl_eu_80665DA0, (OCGetSetFunc)getCtrlWordA37C, (OCGetSetFunc)setCtrlConfigB, 0 },
    { lbl_eu_80665DA8, (OCGetSetFunc)getCtrlWordDiv3, 0, 0 },
    { 0, 0, 0, 0 },
};
__declspec(section ".data") __attribute__((used)) OCSelector lbl_eu_80525D30[3] = {
    { lbl_eu_804FA9D0, (OCSelectorFunc)getTimeIdxMin, 0 },
    { lbl_eu_804FA9E0, (OCSelectorFunc)getTimeIdxMax, 0 },
    { 0, 0, 0 },
};
__declspec(section ".data") __attribute__((used)) OCData lbl_eu_80525D58[1] = {
    { lbl_eu_80665DB0, (OCCtorFunc)getType9Value, lbl_eu_80525CF0, lbl_eu_80525D30 },
};