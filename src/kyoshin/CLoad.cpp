// Auto-scaffolded catalog TU for kyoshin/CLoad
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" u8 func_802AE6AC(void* self) { return static_cast<CLoadFull*>(self)->field_2B; }







extern "C" u8 func_802AE6B4(void* self) { return static_cast<CLoadFull*>(self)->field_28; }

extern "C" u8 func_802AE6BC(void* self) { return static_cast<CLoadFull*>(self)->field_2C; }





extern "C" void func_802AE8E0(void* self) {
    auto* obj = static_cast<CLoadFull*>(self);
    if (obj->field_1C != 0) {
        obj->field_2B = 1;
        obj->field_28 = 1;
    }
}

void CLoad::OnFileEvent() {}
