#pragma once

#include <types.h>

namespace cf {

class CfObjectImplMove {
public:
    virtual ~CfObjectImplMove();

    void func_800CFFA0(unsigned int* param);
    void func_800CFFBC();
    void func_800CFFC4();
};

} // namespace cf

// Internal driver struct for CfObjectImplMove.
// Function func_800CD5C0 accesses a sub-object at +0x14 and reads a uint at +0xc4.
struct CfObjectImplMoveSubObj;
struct CfObjectImplMoveData {
    u8 _00_13[0x14];              // 0x00-0x13
    CfObjectImplMoveSubObj* mSubObj;                // 0x14, sub-object with field at +0xc4

    void func_800CD5C0(unsigned int a, unsigned int b);
};

// Driver sub-object reached through CfObjectImplMoveObj::mSubObj: func_800CAB00
// reads the event id at +0x98, func_800CD5C0 matches mSomeId at +0xc4.
struct CfObjectImplMoveSubObj {
    u8 _00_97[0x98];              // 0x00-0x97
    unsigned int field_0x98;      // 0x98 (event / handle id)
    u8 _9C_C3[0x28];              // 0x9c-0xc3
    unsigned int mSomeId;         // 0xc4
};

// Embedded sub-object (own vtable) at +0x3e9c of CfActorObj. Slots 0x00-0x48
// are unused; func_800CE8AC calls the slot-0x4c virtual, which MWCC dispatches
// through r12 (matching the retail lwzu r12/lwz r12,0x4c/mtctr/bctrl shape).
struct CfEmbeddedSubObj_3E9C {
    virtual void* vfn00();
    virtual void* vfn01();
    virtual void* vfn02();
    virtual void* vfn03();
    virtual void* vfn04();
    virtual void* vfn05();
    virtual void* vfn06();
    virtual void* vfn07();
    virtual void* vfn08();
    virtual void* vfn09();
    virtual void* vfn10();
    virtual void* vfn11();
    virtual void* vfn12();
    virtual void* vfn13();   // slot 0x4c
    virtual void* vfn14();
    virtual void* vfn15();
    virtual void* vfn16();
    virtual void* vfn17();
    virtual void* vfn18();
    virtual void* vfn19();
};

// Object reached via CfObjectImplMoveObj::field_0x18; carries an embedded
// sub-object (own vtable) at +0x3e9c.
struct CfActorObj {
    u8 _00_3E9B[0x3e9c];          // 0x00-0x3e9b
    CfEmbeddedSubObj_3E9C sub;    // 0x3e9c
};

// Main object manipulated by this TU's func_* helpers: vtable at 0x00,
// embedded callback source at 0x0c (passed as `this+0xc` to event dispatch),
// driver sub-object at 0x14, actor object at 0x18, init fields at 0x1c-0x24.
struct CfObjectImplMoveObj {
    void** vtbl;                  // 0x00
    u8 _04_0B[0x8];               // 0x04-0x0b
    u8 field_0x0C[0x8];           // 0x0c-0x13 embedded callback source
    CfObjectImplMoveSubObj* mSubObj;  // 0x14
    CfActorObj* field_0x18;       // 0x18
    u8* field_0x1C;               // 0x1c (opaque object pointer)
    u32 field_0x20;               // 0x20
    f32 field_0x24;               // 0x24
};

// --- imports referenced by this TU (retail symbol names) ---

extern f32 lbl_eu_80666C60;              // float constant, SDA21 (r2)

// C++-mangled retail helper func_800B708C__Fi (actor id -> source).
extern void* func_800B708C(int id);

// Unmangled C-ABI imports (retail symbols have no C++ mangling).
extern "C" void* func_8016FE34(void* source);
extern "C" void func_80482AB8(u32 id, void* source);

// The CfObjectImplMove destructor, called directly by the func_800CFFBC /
// func_800CFFC4 adjusting thunks. The C++ member form would emit the 2-arg
// dtor ABI (li r4, delete-flag) plus __FPv mangling; the retail thunks are
// 1-arg tail calls to the unmangled symbol, so keep C linkage here.
extern "C" void __dt__Q22cf16CfObjectImplMoveFv(void* self);
