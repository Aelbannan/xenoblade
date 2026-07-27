// Auto-scaffolded catalog TU for kyoshin/realtimeevt/CREvtModelMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/realtimeevt/CREvtModelMap.hpp"
void __ct__CREvtModelMap(){}

void __ct__80180B00(void* self){}

void func_80180C60(){}

void func_80180C88(){}

void func_80180CBC(){}

void func_80180DCC(){}

void func_80180E1C(){}

void func_80180E60(){}

void func_80180EBC(){}

void func_8018140C(){}

void func_8018152C(){}

void func_801815AC(){}

void func_801818BC(){}

void func_8018196C(){}

void func_80181988(void* self){}

// Adjusting thunk: upcasts from IWorkEvent sub-object (at offset +0x38 within CREvtModelMap)
// to the full CREvtModelMap, then tail-calls the real event handler.
void OnFileEvent__13CREvtModelMapFP10CEventFile(void* self) {
    ((void(*)(void*))func_80181988)(static_cast<char*>(self) - 0x38);
}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x38 within CREvtModelMap)
// to the full object, then tail-calls the constructor.
void func_80181A54(void* self) {
    ((void(*)(void*))__ct__80180B00)(static_cast<char*>(self) - 0x38);
}
