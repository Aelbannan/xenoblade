// Auto-scaffolded catalog TU for kyoshin/CLoad
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CLoad.hpp"

extern "C" u8 func_802AE6AC(void* self) { return static_cast<CLoadFull*>(self)->field_2B; }


extern "C" u8 func_802AE6B4(void* self) { return static_cast<CLoadFull*>(self)->field_28; }

extern "C" u8 func_802AE6BC(void* self) { return static_cast<CLoadFull*>(self)->field_2C; }


extern "C" void func_802AE8E0(void* self) {
    CLoadFull* obj = static_cast<CLoadFull*>(self);
    if (obj->field_1C != 0) {
        obj->field_2B = 1;
        obj->field_28 = 1;
    }
}

void CLoad::OnFileEvent() {}

extern "C" void func_802AE508() {}
extern "C" void func_802AE560() {}
extern "C" void func_802AE5F0() {}
extern "C" void func_802AE62C() {}
extern "C" void func_802AE6C4() {}
extern "C" void func_802AE758() {}
extern "C" void func_802AE7EC() {}
extern "C" void func_802AE894() {}
