#include "kyoshin/plugin/ocBuiltin.hpp"
#include <cstring>

extern "C" {
void vmBuiltinOCRegist(OCData* pOC);
// Built-in OC data block registered by ocBuiltinRegist (0x80524BF8, size 0x10)
extern OCData lbl_eu_80524BF8;
}

int isExistProperty(VMThread* pThread, int r4, OCData* pOC) {
    VMArg arg;
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    const char* name = vmArgStringGet(2, ptr);
    u32 result = vmPropertySearch(pOC, name);
    arg.type = ((u32)__cntlzw((result >> 31) ^ 1) >> 5) + 1;
    vmRetValSet(pThread, &arg);
    return 1;
}

int isExistSelector(VMThread* pThread, int r4, OCData* pOC) {
    VMArg arg;
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    const char* name = vmArgStringGet(2, ptr);
    u32 result = vmSelectorSearch(pOC, name);
    arg.type = ((u32)__cntlzw((result >> 31) ^ 1) >> 5) + 1;
    vmRetValSet(pThread, &arg);
    return 1;
}

int getOCName(VMThread* pThread, int r4, OCData* pOC) {
    VMArg arg;
    arg.type = VM_TYPE_STRING;
    arg.unk2 = strlen(pOC->name);
    arg.value.pointerVal = (void*)pOC->name;
    vmRetValSet(pThread, &arg);
    return 1;
}

extern "C" void ocBuiltinRegist() {
    vmBuiltinOCRegist(&lbl_eu_80524BF8);
}
