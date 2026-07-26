// Auto-scaffolded catalog TU for kyoshin/cf/CfObjectEnumList
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

extern "C" void __ct__cf_CfObjEnumList() {}

extern "C" void func_800F49F8() {}

extern "C" void func_800F4A98() {}

extern "C" void func_800F4B5C() {}

extern "C" void func_800F6D50() {}

extern "C" void func_800F6E08() {}

extern "C" void* func_800F6E98(void* this_ptr, int index) {
    void** array = *reinterpret_cast<void***>(reinterpret_cast<char*>(this_ptr) + 0x20 + index * 4);
    return *array;
}

extern "C" void* func_800F6EAC(void* self, unsigned long idx) {
    void** arr = (void**)((uint8_t*)self + 0x20);
    void* ptr = arr[idx];
    return *(void**)((uint8_t*)ptr + 4);
}

extern "C" void* func_800F6EC0(void* this_ptr, int index) {
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(this_ptr) + index * 4 + 0x20);
}

extern "C" void func_800F6ED0() {}

extern "C" void func_800F7DEC() {}

extern "C" void func_800F8794() {}

extern "C" void func_800F8890() {}

extern "C" void func_800F89DC() {}

extern "C" void func_800F9AEC() {}

extern "C" void __ct__800FA9B4() {}

extern "C" void __ct__800FAE3C() {}

extern "C" void __ct__800FB044() {}

extern "C" void func_800FB270() {}

extern "C" void func_800FB5AC() {}

extern "C" void __ct__800FBA18() {}

extern "C" void __ct__800FBF08() {}

extern "C" void __ct__800FC040() {}

extern "C" void __ct__800FC19C() {}

extern "C" void __ct__800FC32C() {}

extern "C" void __ct__800FC4FC() {}

extern "C" void __ct__800FC648() {}

extern "C" void __ct__800FC790() {}

extern "C" void __ct__800FC8DC() {}

extern "C" void __ct__800FCA24() {}

extern "C" void __ct__800FCB80() {}

extern "C" void __ct__800FCD10() {}

extern "C" void __ct__800FCF1C() {}

extern "C" void __ct__800FD0B4() {}

extern "C" void __ct__800FD250() {}

extern "C" void func_800FD2FC() {}

extern "C" void func_800FD378() {}

extern "C" void func_800FD3FC() {}

extern "C" void func_800FD68C(void* self) {}

extern "C" void func_800FD698(void* self) { ((void(*)(void*))func_800FD68C)((char*)self - 0x604); }

extern "C" void func_800F4798(void* self);
extern "C" void func_800FD6A0(void* self) { ((void(*)(void*))func_800F4798)((char*)self - 0x604); }

extern "C" void __dt__Q22cf19CfObjectSelectorObjFv() {}
