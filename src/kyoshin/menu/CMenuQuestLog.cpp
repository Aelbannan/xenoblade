// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQuestLog
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__CMenuQuestLog() {}

extern "C" void __dt__13CMenuQuestLogFv(void* self) {}

extern "C" void Init__13CMenuQuestLogFv() {}

extern "C" void __ct__UnkClass_8011C974() {}

extern "C" void func_8011C998() {}

extern "C" void Term__13CMenuQuestLogFv() {}

extern "C" void Move__13CMenuQuestLogFv() {}

extern "C" void cbRenderBefore__13CMenuQuestLogFv(void* self) {}

extern "C" void func_8011CCE0() {}

extern "C" void func_8011CD5C() {}

extern "C" void func_8011CD6C() {}

extern "C" void func_8011CDF4() {}

extern "C" void func_8011CE44() {}

extern "C" void func_8011D03C() {}

extern "C" bool isIdle__11CTitleAHelpFv(void*);
extern "C" bool func_80227CCC(void*);
extern "C" unsigned int func_80228394(void*);
extern "C" void func_80229768(void*, unsigned short);
extern "C" void func_80229510(void*);

extern "C" void func_8011D08C(void* self)
{
    unsigned char* base = static_cast<unsigned char*>(self);
    if (isIdle__11CTitleAHelpFv(base + 0x80) && func_80227CCC(base + 0xB8)) {
        unsigned int value = func_80228394(base + 0xB8);
        func_80229768(base + 0x2240, static_cast<unsigned short>(value));
        func_80229510(base + 0x2240);
        base[0x2284] = 5;
    }
}

extern "C" void func_8011D0FC() {}

extern "C" void func_8011D158() {}

extern "C" void func_8011D1A8() {}

extern "C" void func_8011D22C() {}

extern "C" void func_8011D298() {}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x58 within CMenuQuestLog)
// to the full CMenuQuestLog, then tail-calls cbRenderBefore.
extern "C" void func_8011D2E8(void* self) {
    ((void(*)(void*))cbRenderBefore__13CMenuQuestLogFv)(static_cast<char*>(self) - 0x58);
}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x58 within CMenuQuestLog)
// to the full CMenuQuestLog, then tail-calls the destructor.
extern "C" void func_8011D2F0(void* self) {
    ((void(*)(void*))__dt__13CMenuQuestLogFv)(static_cast<char*>(self) - 0x58);
}

extern "C" void __dt__8011D2F8() {}
