// Decompiled for monolib/src/scn/CScnItemLightNw4r

#include <types.h>
#include "libs/monolib/src/scn/CScnItemLightNw4r.hpp"
#include <monolib/util/FixStr.hpp>

// Freshly allocated light item (size 0x1224); +0x1220 holds ScnRoot*.
struct CScnItemLightNw4rAlloc {
    u8 _00[0x1220];
    nw4r::g3d::ScnRoot* mpScnRoot;  // +0x1220
};

extern "C" {
    extern char lbl_eu_80523ECC[];  // "Global(%d)" format string
    extern char lbl_eu_8056DD38[];  // CScnItemLightNw4r vtable
    extern u32 func_8048C5B8(u8* pool, s32 kind);
    extern u32 func_80496018(void* scene);
    extern void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 handle);
    extern void __ct__CScnItemLight(void* self, void* pNw4r, void* name, void* a2,
                                    void* a3);
    extern void func_8048C630(u8* pool, void* item, u32 value);
}

extern "C" CScnItemLightNw4rAlloc* __ct__CScnItemLightNw4r(CScnLightItemHost* self,
                                                           int param2, void* param3) {
    if (func_8048C5B8(self->mPool, 3) == 0) {
        return 0;
    }
    ml::FixStr<32> str(true);
    str.format(lbl_eu_80523ECC, param2);
    CScnItemLightNw4rAlloc* light =
        (CScnItemLightNw4rAlloc*)allocate__Q23mtl10MemManagerFUlUl(
            0x1224, func_80496018(self));
    if (light != 0) {
        __ct__CScnItemLight(light, self, &str, (void*)param2, param3);
        *(void**)light = (void*)lbl_eu_8056DD38;
        light->mpScnRoot = self->mRoot->mScnRoot;
    }
    func_8048C630(self->mPool, light, 0);
    return light;
}
