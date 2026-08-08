#pragma once

#include <types.h>

#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <nw4r/lyt.h>

// IWorkEvent-compatible vtable (split1 .data).
extern "C" void* lbl_eu_80536EA8[];
// Shared string pool (split1 .rodata): fade layout arc filename.
extern char lbl_eu_8050B5A0[];
// SDA float - fade animation frame target.
extern f32 lbl_eu_80668750;

// UnkClass_8045F564 ctor, called by hand by CFade's ctor to reproduce the
// retail member-init order (vtable store first). See CBgTex.cpp.
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);

/* Fade overlay layout widget. Loads and animates a nw4r layout for fade-in/out
effects. Layout-compatible with IWorkEvent (vptr @ +0) for CDeviceFile::readFile,
but not a C++ IWorkEvent subclass - that would make MWCC emit a __vt__5CFade and
blow the US split budget (retail uses the shared lbl_eu_80536EA8 vtable). */
class CFade {
public:
    void* mVtbl;                             // 0x00 - lbl_eu_80536EA8
    UnkClass_8045F564 mMemRegion;            // 0x04
    CFileHandle* mFileHandle;                // 0x14
    nw4r::lyt::ArcResourceAccessor* mArcResAcc; // 0x18
    nw4r::lyt::Layout* mLayout;              // 0x1C
    nw4r::lyt::AnimTransform* mAnimTrans;    // 0x20

    u8 mIsLoaded;   // 0x24 - true after OnFileEvent completes loading
    u8 mFadeState;  // 0x25 - 0:idle, 1:fading in, 2:faded in, 3:fading out
    u8 mReady;      // 0x26 - true after layout is attached and visible
    u8 mVisible;    // 0x27 - visibility flag (toggled during fade animation)

    CFade();
    ~CFade();
    bool OnFileEvent(CEventFile* pEventFile);

    void func_8024439C();
    void func_802443E8();
    void func_80244460(nw4r::lyt::DrawInfo* drawInfo);
    void func_8024448C();
    u8 func_80244508();
    u8 func_80244510();
    void func_80244518();
    void func_80244538();
    void func_80244558();
    void func_802445A4();
};