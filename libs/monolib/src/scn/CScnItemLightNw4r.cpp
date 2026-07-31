// Decompiled for monolib/src/scn/CScnItemLightNw4r

#include <types.h>
#include "libs/monolib/src/scn/CScnItemLightNw4r.hpp"
#include <monolib/util/FixStr.hpp>

extern "C" {
    extern char lbl_eu_80523ECC[];   // "Global(%d)" format string
    extern char lbl_eu_8056DD38[];   // CScnItemLightNw4r vtable
    extern void* func_8048C5B8(void* r3, u32 r4);
    extern void* func_80496018(void* r3);
    extern void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 r4);
    extern void __ct__CScnItemLight(void* self, void* r4, void* r5, void* r6, void* r7);
    extern void func_8048C630(void* r3, void* r4, u32 r5);
}

extern "C" void* __ct__CScnItemLightNw4r(CScnItemLightNw4r* self, int param2, void* param3) {
    if (func_8048C5B8(self->mField60, 3) == 0) {
        return 0;
    }
    ml::FixStr<32> str(true);
    str.format(lbl_eu_80523ECC, param2);
    void* alloc = allocate__Q23mtl10MemManagerFUlUl(0x1224, (u32)func_80496018(self));
    if (alloc != 0) {
        __ct__CScnItemLight(alloc, self, &str, (void*)param2, param3);
        *(void**)alloc = (void*)lbl_eu_8056DD38;
        *(u32*)((char*)alloc + 0x1220) = *(u32*)((char*)*(void**)((char*)self + 0x8C) + 0x10);
    }
    func_8048C630(self->mField60, alloc, 0);
    return alloc;
}
