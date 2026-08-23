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
    // controller-like slot; retail treats it as a signed countdown value
    // (skips re-init while > 0, resets to 150 after func_8004B9D4)
    s32 field_550;                   // 0x550
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
extern const f32 lbl_eu_806681F0; // .sdata2 float (func_801FBFD8 vec arg)
extern const f32 lbl_eu_806681F4; // .sdata2 float (func_801FBFD8 vec arg)
extern const f32 lbl_eu_806681F8; // .sdata2 float -> field_2FDC increment

// (runtime imports and act-param view types come from
// kyoshin/menu/parts/CModelDispEquip.hpp, pulled in via harness_catalog.hpp)

// Filter-word tables copied to the stack by func_801FCBF4 (.rodata).
extern u32 lbl_eu_80507CDC[];
extern u32 lbl_eu_80507CD0[];
// Name-string buffer holding several NUL-terminated strings back to back
// (elements addressed at +0/+4/+8/+0xC by func_801FC3B0).
extern char lbl_eu_80507CF4[];
// .sdata2 floats used by func_801FC3B0's per-slot color selection.
extern const f32 lbl_eu_806681FC;
extern const f32 lbl_eu_80668200;
extern const f32 lbl_eu_80668204;
extern const f32 lbl_eu_80668208;
extern const f32 lbl_eu_8066820C;
extern const f32 lbl_eu_80668210;

// Overlay view of CActParamHolder words the shared view does not expose:
// the owner back-pointers at +0x10/+0x40 and the act-param flag word at
// +0x18 (bit 23 cleared, bits 0x160 set by func_801FC3B0).
struct CDispHolderWordView {
    u8 _00[0x10];
    void* owner10; // +0x10 back-pointer to the owning CModelDisp
    u8 _14[0x18 - 0x14];
    u32 flags18;   // +0x18 act-param flag word
    u8 _1C[0x40 - 0x1C];
    void* owner40; // +0x40 back-pointer to the owning CModelDisp
};

// Vtable dispatch at +0x40 on the equipment param objects (param->field_2C):
// takes the owning slot, returns a flag. 14 fillers land m40 at +0x40.
struct CModelDispParamSlot;
struct CDispVt40 {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D();
    virtual u32 m40(CModelDispParamSlot* owner); // #14 => +0x40
};

// Local flush-vtable view (+0xE0 slot) - one filler larger than the shared
// spelling so the method lands at byte offset 0xE0 with this compiler.
struct CDispFlushVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24(); virtual void m25(); virtual void m26(); virtual void m27();
    virtual void m28(); virtual void m29(); virtual void m2A(); virtual void m2B();
    virtual void m2C(); virtual void m2D(); virtual void m2E(); virtual void m2F();
    virtual void m30(); virtual void m31(); virtual void m32(); virtual void m33();
    virtual void m34(); virtual void m35();
    virtual void m38(); // lands at +0xE0
};

// --- C-linkage runtime helper and sub-object ctor/dtor (retail names) ---
// The sub-object ctor/dtor are retail-named symbols (func_801FBEB8 /
// __dt__801FBF0C), not C++ member symbols; C linkage keeps the __construct_array
// data relocs byte-identical to retail (same pattern as code_801C2C14.cpp).
// (__construct_array itself is declared in CModelDispEquip.hpp.)
extern "C" void* func_801FBEB8(CModelDispSub* sub);
extern "C" u8* __dt__801FBF0C(CModelDispSub* obj, int flag);

// Vtable dispatch on the object stored in a sub-object's field_00:
// +0xC8 (no args) and +0xC4 (three args). With MWCC's 2 reserved leading
// slots, offset = (declaredIndex+2)*4: 47 fillers put mC4 (index 47) at
// +0xC4 and mC8 (index 48) at +0xC8.
struct CModelDispSubVt {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03();
    virtual void m04(); virtual void m05(); virtual void m06(); virtual void m07();
    virtual void m08(); virtual void m09(); virtual void m0A(); virtual void m0B();
    virtual void m0C(); virtual void m0D(); virtual void m0E(); virtual void m0F();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13();
    virtual void m14(); virtual void m15(); virtual void m16(); virtual void m17();
    virtual void m18(); virtual void m19(); virtual void m1A(); virtual void m1B();
    virtual void m1C(); virtual void m1D(); virtual void m1E(); virtual void m1F();
    virtual void m20(); virtual void m21(); virtual void m22(); virtual void m23();
    virtual void m24(); virtual void m25(); virtual void m26(); virtual void m27();
    virtual void m28(); virtual void m29(); virtual void m2A(); virtual void m2B();
    virtual void m2C(); virtual void m2D(); virtual void m2E();
    virtual void mC4(void* ptr, void* arg, u32 flag); // index 47 => +0xC4
    virtual void mC8();                               // index 48 => +0xC8
};
