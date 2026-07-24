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
};

// C-linkage accessors matching retail unmangled symbols.
extern "C" u8 func_8025DA40(CPcKizunagram* pKizunagram);
extern "C" u8 func_8025DA48(CPcKizunagram* pKizunagram);
