#pragma once

#include <types.h>
#include "kyoshin/help/CHelp.hpp"

namespace cf {

class CHelp_LandMark : public CHelp {
public:
    CHelp_LandMark();
    ~CHelp_LandMark();

    void func_802B8280();
    void func_802B8290(u32 param1, u32 param2);
    u32 func_802B8328();
    void func_802B8388(u32 param1, u32 param2);
    void func_802B8390();

    // CHelp at +0x00..+0x0B (mOwner, mParam, mVtbl)
    // Second base-class subobject (invisible in single-inheritance C++)
    // Stores a vtable pointer for the second base at +0x0C
    void* mSecondBase; // +0x0C
    u32 field_10;         // +0x10
    s32 mTimer;           // +0x14
    u32 mSavedFlags;      // +0x18
};

} // namespace cf

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" int func_8009CF8C(void*);
extern "C" void* func_8009D414(void*);
extern "C" void func_8009D514(void*);
extern "C" void __dl__FPv(void*);
// CHelp base constructor with Fv symbol (retail name)
extern "C" void __ct__Q22cf5CHelpFv(cf::CHelp* self, void* owner, u32 param);
