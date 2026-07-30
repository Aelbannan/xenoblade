// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQuestLog
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuQuestLog.hpp"
void __ct__CMenuQuestLog(){}

CMenuQuestLog::~CMenuQuestLog() {}

void CMenuQuestLog::Init() {}

void __ct__UnkClass_8011C974(){}

void func_8011C998(){}

void CMenuQuestLog::Term() {}

void CMenuQuestLog::Move() {}

void CMenuQuestLog::cbRenderBefore() {}

void func_8011CCE0(){}

void func_8011CD5C(){}

void func_8011CD6C(){}

void func_8011CDF4(){}

void func_8011CE44(){}

void func_8011D03C(){}

bool isIdle__11CTitleAHelpFv(void*);
bool func_80227CCC(void*);
unsigned int func_80228394(void*);
void func_80229768(void*, unsigned short);
void func_80229510(void*);

void func_8011D08C(void* self)
{
    unsigned char* base = static_cast<unsigned char*>(self);
    if (isIdle__11CTitleAHelpFv(base + 0x80) && func_80227CCC(base + 0xB8)) {
        unsigned int value = func_80228394(base + 0xB8);
        func_80229768(base + 0x2240, static_cast<unsigned short>(value));
        func_80229510(base + 0x2240);
        base[0x2284] = 5;
    }
}

void func_8011D0FC(){}

void func_8011D158(){}

void func_8011D1A8(){}

void func_8011D22C(){}

void func_8011D298(){}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x58 within CMenuQuestLog)
// to the full CMenuQuestLog, then tail-calls cbRenderBefore.
void func_8011D2E8(void* self) {
    ((void(*)(void*))cbRenderBefore__13CMenuQuestLogFv)(static_cast<char*>(self) - 0x58);
}

// Adjusting thunk: upcasts from a base sub-object (at offset +0x58 within CMenuQuestLog)
// to the full CMenuQuestLog, then tail-calls the destructor.
void func_8011D2F0(void* self) {
    ((void(*)(void*))__dt__13CMenuQuestLogFv)(static_cast<char*>(self) - 0x58);
}

void __dt__8011D2F8(){}
