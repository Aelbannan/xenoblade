// Auto-scaffolded catalog TU for kyoshin/plugin/ocCfp
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocCfp.hpp"

extern "C" {
unsigned short func_80086DA0__Q22cf13CfGameManagerFv();
unsigned short func_80086DA4__Q22cf13CfGameManagerFv();
void func_80086D90__Q22cf13CfGameManagerFv(unsigned short);
void func_80086D94__Q22cf13CfGameManagerFv(unsigned short);
}

extern "C" int func_80045560(VMThread* self, int a, int val){
    VMArg arg;
    arg.type = 9;
    arg.unk2 = val;
    arg.value.intVal = 0;
    vmRetValSet(self, &arg);
    return 1;
}

extern "C" int func_8004559C(VMThread* self){
    VMArg arg;
    arg.type = 3;
    arg.value.intVal = func_80086DA0__Q22cf13CfGameManagerFv();
    vmRetValSet(self, &arg);
    return 1;
}

extern "C" int func_800455E8(VMThread* self){
    VMArg arg;
    arg.type = 3;
    arg.value.intVal = func_80086DA4__Q22cf13CfGameManagerFv();
    vmRetValSet(self, &arg);
    return 1;
}

extern "C" int func_80045634(VMThread* self){
    VMArg arg;
    arg.type = 3;
    arg.value.intVal = func_80086DA0__Q22cf13CfGameManagerFv() / 3;
    vmRetValSet(self, &arg);
    return 1;
}

extern "C" int func_80045694(VMThread* self){
    void* prop = vmOCPropertyGet(self);
    u32 val = *(u32*)((char*)prop + 4);
    func_80086D90__Q22cf13CfGameManagerFv(val & 0xFFFF);
    return 0;
}

extern "C" int func_800456C4(VMThread* self){
    void* prop = vmOCPropertyGet(self);
    u32 val = *(u32*)((char*)prop + 4);
    func_80086D94__Q22cf13CfGameManagerFv(val & 0xFFFF);
    return 0;
}

extern "C" int getTimeIdxMin(VMThread* self){
    VMArg* argPtr = vmArgPtrGet(self, 1);
    int idx = vmArgIntGet(2, argPtr);
    VMArg arg;
    if (idx > 7) {
        arg.type = 3;
        arg.value.intVal = 0;
        vmOCExceptionThrow(self);
        return 0;
    }
    arg.type = 3;
    arg.value.intVal = idx * 3;
    vmRetValSet(self, &arg);
    return 1;
}

extern "C" int getTimeIdxMax(VMThread* self){
    VMArg* argPtr = vmArgPtrGet(self, 1);
    int idx = vmArgIntGet(2, argPtr);
    VMArg arg;
    if (idx > 7) {
        arg.type = 3;
        arg.value.intVal = 0;
        vmOCExceptionThrow(self);
        return 0;
    }
    int val = idx + 1;
    arg.value.intVal = val * 3;
    arg.type = 3;
    vmRetValSet(self, &arg);
    return 1;
}

extern "C" {
BOOL vmOCRegist(OCData* pOC);
}
extern OCData lbl_eu_80525D58[];
void ocCfpRegist() {
    vmOCRegist(lbl_eu_80525D58);
}