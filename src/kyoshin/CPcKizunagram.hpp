#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/work/CEventFile.hpp"

#include <nw4r/lyt.h>

// PC Kizunagram (affinity chart) per-character window.
// Layout-compatible with IWorkEvent (vptr @0) for CDeviceFile::readFile.
class CPcKizunagram {
public:
    void* mVtbl;                              // 0x00
    UnkClass_8045F564 mMemRegion;             // 0x04
    CFileHandle* mFileHandle;                 // 0x14
    nw4r::lyt::ArcResourceAccessor* mArcRes;  // 0x18
    nw4r::lyt::Layout* mLayout;               // 0x1C
    nw4r::lyt::AnimTransform* mAnimTransform; // 0x20
    u8 mStateByte1;                           // 0x24 — active flag
    u8 mStateByte2;                           // 0x25 — sub-state (0=hidden, 1=opening, 2=open, 3=closing)
    u8 mIsHidden;                             // 0x26 — set externally to hide/show
    u8 mIsOpen;                               // 0x27 — 1 when in stable visible state (state 2)
    u8 mField28;                              // 0x28
    u8 _pad29[3];                             // 0x29
    u8 mKizunaCur[0x18];                      // 0x2C — CPcKizunaCur sub-object
    u8 mByte44;                               // 0x44
    u8 _pad45[3];                             // 0x45
    f32 mFloat48;                             // 0x48
    CPcKizunagram();
    virtual ~CPcKizunagram();
    void OnFileEvent();
};

// C-linkage accessors matching retail unmangled symbols.
extern "C" u8 func_8025DA40(CPcKizunagram* pKizunagram);
extern "C" u8 func_8025DA48(CPcKizunagram* pKizunagram);

// ---------------------------------------------------------------------------
// CPcKizunaCur - the per-character cursor sub-object (0x18 bytes) embedded at
// offset 0x2C of CPcKizunagram. Layout-compatible with CBaseCur; the vtable
// pointer is stored explicitly (lbl_eu_805376AC) by the constructor.
// ---------------------------------------------------------------------------

// Vtable symbol for CPcKizunaCur.
extern "C" void* lbl_eu_805376AC[];

class CPcKizunaCur {
public:
    void* mVtable;                            // 0x00
    nw4r::lyt::ArcResourceAccessor* mAccessor; // 0x04
    nw4r::lyt::Layout* mpLayout;              // 0x08
    nw4r::lyt::AnimTransform* mpAnim0;        // 0x0C
    nw4r::lyt::AnimTransform* mpAnim1;        // 0x10
    u8 mField14;                              // 0x14
    u8 mField15;                              // 0x15
    u8 mField16;                              // 0x16

    CPcKizunaCur(nw4r::lyt::ArcResourceAccessor* accessor);
    ~CPcKizunaCur();
};

// ---------------------------------------------------------------------------
// Support types for the free functions in this TU (not part of CPcKizunagram).
// ---------------------------------------------------------------------------

// Large object func_8025EE7C / func_8025EE94 operate on (u32 field at 0x89C).
struct CPcKizunagramBig {
    u8 _00[0x89C];
    int field_0x89C;                          // 0x89C
};

// func_8025D6E0: nested pointer chain -> leaf struct holding a Vec3 at 0x2C.
struct CPcKizunaTreeLeaf {
    u8 _00[0x2C];
    f32 x;                                    // 0x2C
    f32 y;                                    // 0x30
    f32 z;                                    // 0x34
};

struct CPcKizunaTreeMid {
    u8 _00[0x10];
    CPcKizunaTreeLeaf* field10;               // 0x10
};

struct CPcKizunaTreeRoot {
    u8 _00[0x08];
    CPcKizunaTreeMid* field8;                 // 0x08
};

struct CPcKizunaVec3 {
    f32 x;
    f32 y;
    f32 z;
};
