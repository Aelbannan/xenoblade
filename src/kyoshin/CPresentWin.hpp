#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CPresentWin {
public:
    CPresentWin();
    virtual ~CPresentWin();

    // +0x00: vtable
    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x04
    nw4r::lyt::Layout* mpLayout;              // 0x08
    nw4r::lyt::AnimTransform* mpAnimTrans0;    // 0x0C
    nw4r::lyt::AnimTransform* mpAnimTrans1;    // 0x10
    nw4r::lyt::Pane* mPane14;                  // 0x14
    nw4r::lyt::Pane* mPane18;                  // 0x18
    nw4r::lyt::Pane* mPane1C;                  // 0x1C
    nw4r::lyt::Pane* mPane20;                  // 0x20
    nw4r::lyt::Pane* mPane24;                  // 0x24
    nw4r::lyt::Pane* mPane28;                  // 0x28
    nw4r::lyt::Pane* mPane2C;                  // 0x2C
    u8 mField30;                               // 0x30
    u8 mField31;                               // 0x31
    u8 mField32;                               // 0x32
    u8 mField33;                               // 0x33
    u16 mField34;                              // 0x34-0x35 (item id read by func_8022DD90)
    u8 mField36;                               // 0x36
    u8 mField37;                               // 0x37
    u8 mField38;                               // 0x38
    u8 mDataArray[8];                          // 0x39-0x40 (indexed by func_8022E868)
    u8 mDataCount;                             // 0x41
};

// Mirror of the font object returned by CDeviceFont::func_80452C10.
// vtable slot 9 (offset 0x24) yields the u32 bound into the layout's font
// pane. Never instantiated, so no vtable is emitted; a genuine virtual call
// makes MWCC emit the retail r12 dispatch sequence (same trick as
// CMenuPTGaugeFont / CFontPanel).
struct CPresentFontView {
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0; // vtable offset 0x24
};

// Texture object returned by ArcResourceAccessor::GetResource for the 'timg'
// tag. mChain (+0x08) -> mDims (+0x00) carries the 2D dimension header.
struct CPresentTexDims;
struct CPresentTexChain {
    CPresentTexDims* mDims; // +0x00
};
struct CPresentTexDims {
    u16 mW; // +0x00
    u16 mH; // +0x02
};
struct CPresentTexObj {
    void* mVtable;            // +0x00
    void* _pad_04;            // +0x04
    CPresentTexChain* mChain; // +0x08
};

// --- imports (retail linker names) ---
extern char lbl_eu_8050A84C[]; // layout path + pane-name + message string pool
extern f32 lbl_eu_8066862C;    // animation advance delta
extern f64 lbl_eu_80668620;    // 2^52 double used by the u32->float conversion
extern f32 lbl_eu_80668628;    // frame reset value written into animTrans1
extern u32 lbl_eu_80664104;    // BDAT table handle used by the time/rank lookup

// Retail symbols that ARE the Itanium-mangled C++ names: declare under C
// linkage so calls bind to the literal mangled identifier.
extern "C" {
int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
void func_80137F88(void*, void*);
void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
}

// Unmangled retail helpers: the retail reloc at every call site is the bare
// identifier, so declare C linkage (a C++-linkage decl would emit a mangled
// reloc and drift at the reloc gate).
extern "C" {
nw4r::lyt::ArcResourceAccessor* func_801355F4();
void func_80124270(nw4r::lyt::Pane*, u32);
void func_80124288(nw4r::lyt::Pane*, float*);
extern "C" u32 func_80139358(u32);
int func_801362C0(const char*, const char*, const char*);
void func_8013A95C(u16, u16, s8);
}
void func_80138078(u32); // mangles to func_80138078__FUl
