#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CFileHandle;

class CSLCur {
public:
    CSLCur();
    void func_8028EA74();
    void func_8028EEC0();

    // Vtable-like pointer set manually (initialized to lbl_eu_8053884C)
    void* mField0;    // 0x00
    void* mField4;    // 0x04 - ArcResourceAccessor* (or int from constructor)
    void* mField8;    // 0x08 - Layout* (or null)
    u8 mFieldC;       // 0x0C
    u8 mFieldD;       // 0x0D
    u8 mFieldE;       // 0x0E
    u8 _pad_0F;       // 0x0F (padding byte)
    u8 mField10;      // 0x10
    u8 mField11;      // 0x11
    u8 mField12;      // 0x12
    u8 mField13;      // 0x13
    u8 mField14;      // 0x14
    u8 mField15;      // 0x15
};

class CSaveLoad {
public:
    CSaveLoad();
    virtual ~CSaveLoad();
    void func_8028F23C();
    void OnFileEvent();

    u8 func_8028F664();
    u8 func_8028FEC4();

    // +0x00: vtable (implicit)
    char _pad_04[0x14 - 0x04];                         // 0x04-0x13
    CFileHandle* mFileHandle;                            // 0x14 - file handle from readFile
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;       // 0x18 - arc resource accessor (passed to func_80139124)
    nw4r::lyt::Layout* mLayout;                         // 0x1C - layout for animation control
    nw4r::lyt::AnimTransform* mAnimTransA;              // 0x20 - animation transform A
    nw4r::lyt::AnimTransform* mAnimTransB;              // 0x24 - animation transform B
    char _pad_28[0x3C - 0x28];                          // 0x28-0x3B
    u8 mField3C;                                         // 0x3C
    char _pad_3D[0x58 - 0x3D];                          // 0x3D-0x57
    u8 mScrollbar[0x98 - 0x58];                          // 0x58-0x97 - scrollbar state (passed to func_801F34F4/func_801F369C)
    char mSysWin98[0xD4 - 0x98];                           // 0x98-0xD3 - first CSysWin-like object
    char mSysWinD4[0x11C - 0xD4];                          // 0xD4-0x11B - second CSysWin-like object
    u8 mField11C;                                        // 0x11C
    u8 mField11D;                                        // 0x11D (padding/gap)
    u8 mField11E;                                        // 0x11E
    u8 _pad_11F;                                        // 0x11F
    u8 mField120;                                       // 0x120
    u8 mField121;                                        // 0x121
    u8 mField122;                                        // 0x122
    u8 mField123;                                        // 0x123
    u8 mField124;                                        // 0x124
    u8 _pad_125;                                         // 0x125
    u16 mField126;                                       // 0x126 (halfword cursor position)
    u8 mField128;                                        // 0x128
    u8 mField129;                                        // 0x129
    u8 mField12A;                                        // 0x12A
    char _pad_12B[0x12C - 0x12B];                      // 0x12B
    u8 mField12C;                                        // 0x12C
    u8 mField12D;                                        // 0x12D
    char _pad_12E[0x130 - 0x12E];                      // 0x12E-0x12F
    void* mField130;                                    // 0x130 - heap pointer to deallocate
    void* mField134;                                    // 0x134 - heap pointer to deallocate
    void* mField138;                                    // 0x138 - heap pointer to deallocate
    CSLCur mCur;                                         // 0x13C - cursor/save slot state
};

namespace cf {

struct CfAward {
    void* mVtbl;       // +0x00 - vtable pointer (points to lbl_eu_80538858 + 8)
    void* mSecondBase; // +0x04 - second vtable base (points to lbl_eu_80538858)

    // No virtual functions - vtable managed manually.
    // The destructor is a regular member function (not virtual).
    // MWCC generates the delete flag handling automatically.
    ~CfAward();
    // func_80291B18 is implemented as an extern "C" function (unmangled retail symbol)
};

} // namespace cf

// Abstract struct for CSysWin vtable dispatch at slot 34 (offset 0x88)
// MWCC adds 2 implicit entries (RTTI/dtor) before first user function.
// MWCC doesn't generate a vtable for pure abstract classes.
struct CSysWinProxy {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
    virtual void v11() = 0;
    virtual void v12() = 0;
    virtual void v13() = 0;
    virtual void v14() = 0;
    virtual void v15() = 0;
    virtual void v16() = 0;
    virtual void v17() = 0;
    virtual void v18() = 0;
    virtual void v19() = 0;
    virtual void v20() = 0;
    virtual void v21() = 0;
    virtual void v22() = 0;
    virtual void v23() = 0;
    virtual void v24() = 0;
    virtual void v25() = 0;
    virtual void v26() = 0;
    virtual void v27() = 0;
    virtual void v28() = 0;
    virtual void v29() = 0;
    virtual void v30() = 0;
    virtual void v31() = 0;
    virtual void v32() = 0; // vtable slot 34 = offset 0x88 (after 2 implicit entries)
};

// Opaque object whose first virtual (vtable offset 0x8 after RTTI pad slots) is a
// deleting-destructor-like virtual taking an int flag. Abstract (pure virtual)
// so MWCC emits no vtable.
class UnkObj {
public:
    virtual void vf2(int) = 0;
};

// Struct with ArcResourceAccessor at +0x00, Layout at +0x04, AnimTransform at +0x08.
// Used by func_8028ED70; compatible with UnkTwoPtr (UnkTwoPtr's _pad_00[4] absorbs the accessor).
struct UnkED70_Struct {
    nw4r::lyt::ArcResourceAccessor* mAccessor;  // +0x00
    nw4r::lyt::Layout* mLayout;                  // +0x04
    nw4r::lyt::AnimTransform* mAnimTrans;        // +0x08
};

// Opaque object whose vtable (after 2 RTTI pad slots) has virtuals at
// slots 11 (0x2C) and 14 (0x38) - used by func_8028EF74's dispatch.
// Abstract so MWCC emits no vtable.
class UnkVtblObj {
public:
    virtual void vf2(int) = 0;
    virtual void vf3(int) = 0;
    virtual void vf4(int) = 0;
    virtual void vf5(int) = 0;
    virtual void vf6(int) = 0;
    virtual void vf7(int) = 0;
    virtual void vf8(int) = 0;
    virtual void vf9(int) = 0;
    virtual void vf10(int) = 0;
    virtual void vf11(void* arg, int flag) = 0; // vtable 0x2C
    virtual void vf12(int) = 0;
    virtual void vf13(int) = 0;
    virtual void vf14(int) = 0;                 // vtable 0x38
};
