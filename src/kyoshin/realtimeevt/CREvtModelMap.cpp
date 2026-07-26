// Auto-scaffolded catalog TU for kyoshin/realtimeevt/CREvtModelMap
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CREvtModelMap() {}

extern "C" void __ct__80180B00(void* self) {}

extern "C" void func_80180C60() {}

extern "C" void func_80180C88() {}

extern "C" void func_80180CBC() {}

extern "C" void func_80180DCC() {}

extern "C" void func_80180E1C() {}

extern "C" void func_80180E60() {}

extern "C" void func_80180EBC() {}

extern "C" void func_8018140C() {}

extern "C" void func_8018152C() {}

extern "C" void func_801815AC() {}

extern "C" void func_801818BC() {}

extern "C" void func_8018196C() {}

extern "C" void func_80181988(void* self) {}

// Adjusting thunk: upcasts from IWorkEvent sub-object (at offset +0x38 within CREvtModelMap)
// to the full CREvtModelMap, then tail-calls the real event handler.
extern "C" void OnFileEvent__13CREvtModelMapFP10CEventFile(void* self) {
    ((void(*)(void*))func_80181988)(static_cast<char*>(self) - 0x38);
}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x38 within CREvtModelMap)
// to the full object, then tail-calls the constructor.
extern "C" void func_80181A54(void* self) {
    ((void(*)(void*))__ct__80180B00)(static_cast<char*>(self) - 0x38);
}
