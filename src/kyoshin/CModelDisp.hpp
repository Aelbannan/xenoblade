#pragma once

#include <types.h>

// Sub-object struct within CModelDisp, stride 0xFF0
// Kept void*: the only includer (CModelDisp.cpp) has a pre-existing compile
// failure (undefined mSubObj) and mixed void*-assignment usage; unverifiable.
struct CModelDispSub {
    void* field_00;                  // 0x00
    void* field_04;                  // 0x04
    void* mpController;              // 0x08 - pointer to controller (vcalled)
    void* mObj0C;                    // 0x0C - object with vtable (vcall at 0xE0)
    void* field_10;                  // 0x10
    u8 _14[0x53C];                   // 0x14 to 0x54F
    void* field_550;                 // 0x550 - controller-like pointer
    void* field_554;                 // 0x554 - controller-like pointer
    u8 mBuffer[0x4B4];               // 0x558 - buffer region (base for func_8004B9D4)
    void* mResultA;                  // 0xA0C - getNextChainObj result
    u8 _A10[0x80];                   // 0xA10 to 0xA8F
    void* field_A90;                 // 0xA90 - second controller-like pointer
    u8 _A94[0x4B4];                  // 0xA94 to 0xF47
    void* mResultB;                  // 0xF48 - getNextChainObj result
    u8 _F4C[0x7C];                   // 0xF4C to 0xFC7
    void* mSlotPtrA;                 // 0xFC8
    void* mSlotPtrB;                 // 0xFCC
    u32 mFlagFD0;                    // 0xFD0 - flag
    u32 mFlagFD4;                    // 0xFD4 - flag
    u8 _FD8[0x18];                   // 0xFD8 - padding to 0xFF0
};

// CModelDisp is not polymorphic in retail: the vtable pointer at +0x00 is a
// plain data member (lbl_eu_80535390) written by the constructor - the retail
// dtor performs no vtable reset, so no compiler-generated vtable is wanted.
class CModelDisp {
public:
    CModelDisp(u8* initParam);
    ~CModelDisp();

    u32* mVtbl;                  // 0x00 - vtable pointer (lbl_eu_80535390)
    void* mInitParam;            // 0x04 - init parameter
    CModelDispSub mSubs[3];      // 0x08 - three sub-objects
    u8 field_2FD8;               // 0x2FD8
    u8 _pad2FD9[3];              // 0x2FD9
    f32 field_2FDC;              // 0x2FDC - display alpha/transition value
    f32 field_2FE0;              // 0x2FE0 - accumulator timer
    u8 field_2FE4;               // 0x2FE4
};

// --- retail data symbols (imports; global scope keeps names unmangled) ---
extern u32 lbl_eu_80535390[];     // CModelDisp vtable (.data)
extern f32 lbl_eu_806681E8;       // .sdata2 float -> field_2FDC
extern f32 lbl_eu_806681EC;       // .sdata2 float -> field_2FE0

// --- C-linkage runtime helper and sub-object ctor/dtor (retail names) ---
// The sub-object ctor/dtor are retail-named symbols (func_801FBEB8 /
// __dt__801FBF0C), not C++ member symbols; C linkage keeps the __construct_array
// data relocs byte-identical to retail (same pattern as code_801C2C14.cpp).
extern "C" void __construct_array(CModelDispSub* array,
                                  void (*ctor)(CModelDispSub*),
                                  u8* (*dtor)(CModelDispSub*, int),
                                  int size, int count);
extern "C" void func_801FBEB8(CModelDispSub* sub);
extern "C" u8* __dt__801FBF0C(CModelDispSub* obj, int flag);
