// Auto-scaffolded catalog TU for kyoshin/realtimeevt/CREvtModelObj
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/realtimeevt/CREvtModelObj.hpp"
void __ct__CREvtModelObj(){}

void __ct__80181B74(void* self){}

void func_80181C90(){}

void func_80181DDC(){}

void func_80181F28(){}

void func_80182084(){}

void func_80182100(){}

void func_80182178(){}

void func_80182B2C(){}

void func_80183264(void) {}

void func_80183268(){}

void func_801832D4(){}

void func_8018351C(){}

int func_801835BC(void* self) { return 1; }

void func_801835C4(void* self) {
    *(unsigned long*)((char*)self + 0x18) &= ~0x20;
}

void func_801835D4(void* self){}

void func_801836CC(void* self, unsigned long value)
{
    if (*(unsigned long*)((char*)self + 0x7c) == value) {
        *(unsigned long*)((char*)self + 0x7c) = 0;
    }
}

void func_801836E4(){}

extern "C" void OnFileEvent__13CREvtModelObjFP10CEventFile(CREvtModelObj* self) { ((void(*)(void*))func_801835D4)((char*)self - 0x38); }

extern "C" void func_8018370C(void* self) { ((void(*)(void*))__ct__80181B74)((char*)self - 0x38); }

extern "C" void func_80183714(void* self) { ((void(*)(void*))func_801836CC)((char*)self - 0x3c); }

extern "C" void func_8018371C(void* self) { ((void(*)(void*))__ct__80181B74)((char*)self - 0x3c); }
