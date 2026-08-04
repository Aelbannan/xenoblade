#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CSLCur {
public:
    CSLCur();
    void func_8028EA74();
    void func_8028EEC0();

    // No vtable (no virtual functions)
    int mField0;    // 0x00
    int mField4;    // 0x04
    void* mField8;  // 0x08
    u8 mFieldC;     // 0x0C
    u8 mFieldD;     // 0x0D
    u8 mFieldE;     // 0x0E
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
    u32 mFileHandle;                                     // 0x14 - file handle from readFile
    char _pad_18[0x1C - 0x18];                          // 0x18-0x1B
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
    char _pad_11F[0x121 - 0x11F];                      // 0x11F-0x120
    u8 mField121;                                        // 0x121
    u8 mField122;                                        // 0x122
    u8 mField123;                                        // 0x123
    char _pad_124[0x129 - 0x124];                      // 0x124-0x128
    u8 mField129;                                        // 0x129
    u8 mField12A;                                        // 0x12A
    char _pad_12B[0x12D - 0x12B];                      // 0x12B-0x12C
    u8 mField12D;                                        // 0x12D
    char _pad_12E[0x13C - 0x12E];                      // 0x12E-0x13B
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
    void func_80291B18();
};

} // namespace cf
