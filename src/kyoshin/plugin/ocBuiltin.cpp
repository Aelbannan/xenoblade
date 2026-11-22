#include "kyoshin/plugin/ocBuiltin.hpp"
#include <cstring>

extern "C" {
void vmBuiltinOCRegist(OCData* pOC);
}

int isExistProperty(VMThread* pThread, int handle, OCData* pOC) {
    VMArg arg;
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    const char* name = vmArgStringGet(2, ptr);
    u32 result = vmPropertySearch(pOC, name);
    arg.type = ((u32)__cntlzw((result >> 31) ^ 1) >> 5) + 1;
    vmRetValSet(pThread, &arg);
    return 1;
}

int isExistSelector(VMThread* pThread, int handle, OCData* pOC) {
    VMArg arg;
    VMArg* ptr = vmArgPtrGet(pThread, 1);
    const char* name = vmArgStringGet(2, ptr);
    u32 result = vmSelectorSearch(pOC, name);
    arg.type = ((u32)__cntlzw((result >> 31) ^ 1) >> 5) + 1;
    vmRetValSet(pThread, &arg);
    return 1;
}

int getOCName(VMThread* pThread, int handle, OCData* pOC) {
    VMArg arg;
    arg.type = VM_TYPE_STRING;
    arg.unk2 = strlen(pOC->name);
    arg.value.pointerVal = (void*)pOC->name;
    vmRetValSet(pThread, &arg);
    return 1;
}

// name-guess: script-visible selectors of the "builtin" OC plugin; retail
// .data object 0x80524BC8 (three entries plus the retail null terminator).
OCSelector sBuiltinSelectors[4] = {
    { "isExistProperty", (OCSelectorFunc)isExistProperty, 0 },
    { "isExistSelector", (OCSelectorFunc)isExistSelector, 0 },
    { "getOCName", (OCSelectorFunc)getOCName, 0 },
    { 0, 0, 0 },
};

// name-guess: root OCData registered by ocBuiltinRegist; retail .data object
// 0x80524BF8.
OCData sBuiltinData[1] = {
    { "builtin", 0, 0, sBuiltinSelectors },
};

extern "C" void ocBuiltinRegist() {
    vmBuiltinOCRegist(sBuiltinData);
}
