// Decompiled for monolib/src/scn/CScnItemLightNw4r

#include <types.h>
#include "libs/monolib/src/scn/CScnItemLightNw4r.hpp"
#include "libs/monolib/src/scn/CScnItemLight.hpp"
#include <monolib/util/FixStr.hpp>
#include "monolib/util/MemManager.hpp"

extern "C" {
    extern char lbl_eu_80523ECC[];  // "Global(%d)" format string
    extern "C++" u32 CScnItemPool_hasFreeSlot(u8* pool, s32 kind);
    extern mtl::ALLOC_HANDLE Scn_CallUnk8C_V8(CScnLightItemHost* scene);
    extern "C++" u32 CScnItemPool_registerOtherList(u8* pool, u8* item);
}

extern "C" CScnItemLightNw4r* __ct__CScnItemLightNw4r(CScnLightItemHost* ths,
                                                      int param2, void* param3) {
    if (CScnItemPool_hasFreeSlot((u8*)ths->mPool, 3) == 0) {
        return 0;
    }
    ml::FixStr<32> str(true);
    str.format(lbl_eu_80523ECC, param2);
    CScnItemLightNw4r* light = new (Scn_CallUnk8C_V8(ths))
        CScnItemLightNw4r(ths, (char*)str.c_str(), param2, param3);
    ((u32 (*)(u8*, void*, u32))CScnItemPool_registerOtherList)((u8*)ths->mPool, light, 0);
    return light;
}
