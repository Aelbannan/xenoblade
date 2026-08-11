#pragma once

#include <types.h>
#include <nw4r/lyt.h>

#include "kyoshin/CBaseCur.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"

/* Cursor widget embedded at +0x34 (loads layout "curs22"). Its constructor
   and destructor are defined in the CCur unit (__ct__CCur22 / __dt__6CCur22Fv);
   only the dtor declaration is needed here so CPartyState's destructor emits
   the member-destruction call with the retail symbol name. */
class CCur22 : public CBaseCur {
public:
    ~CCur22();
};

class CPartyState {
public:
    CPartyState();
    ~CPartyState();
    void OnFileEvent();
    u8 func_801FD17C();
    u8 func_801FD184();
    u8 func_801FD18C();
    u32 func_801FD580();
    u8 func_801FD5F4();
    u8 func_801FD5FC();

    u8 _00[0x04];                          // 0x00
    UnkClass_8045F564 mMemRegion;          // 0x04 (0x10 bytes)
    u8 _14[0x1C - 0x14];                   // 0x14
    u32 field_0x1C;                        // 0x1C
    u8 _20[0x24 - 0x20];                   // 0x20
    nw4r::lyt::AnimTransform* mAnimTrans0; // 0x24
    u8 field_0x28;                         // 0x28
    u8 _29[0x2C - 0x29];                   // 0x29
    u32 field_0x2C;                        // 0x2C (advance/state word)
    u8 field_0x30;                         // 0x30
    u8 field_0x31;                         // 0x31
    u8 _32[0x34 - 0x32];                   // 0x32
    CCur22 mCur22;                         // 0x34 (0x16 bytes)
    u8 _4A[0x4C - 0x4A];                   // 0x4A
    u8 field_0x4C;                         // 0x4C
    s8 field_0x4D;                         // 0x4D
    u8 field_0x4E;                         // 0x4E
    u8 _4F[0x57 - 0x4F];                   // 0x4F
    u8 field_0x57;                         // 0x57
};

/* ---- imports (declared here, never locally in the .cpp) ---- */

// UI sound effect; retail symbol is the C++ mangled func_80138078__FUl.
void func_80138078(u32);

// nw4r anim-transform frame checker; retail symbol is the C++ mangled
// func_80137444__FPQ34nw4r3lyt13AnimTransformf.
u32 func_80137444(nw4r::lyt::AnimTransform*, float);

// C-linkage helpers from other units (retail symbols are unmangled).
// func_801D216C / func_80139198 are declared in CEquipItemBox.hpp.
extern "C" void func_801D2BFC(CBaseCur*, u8);
extern "C" u8 code80135FDC_getByte_64077();

// Internal helpers of this unit (unmangled retail symbols).
extern "C" void func_801FD8F8(CPartyState*);
extern "C" void func_801FDA7C(CPartyState*);

// Animation target frame for func_801FD76C (.sdata2, retail lbl_eu_80668218).
extern f32 lbl_eu_80668218;
