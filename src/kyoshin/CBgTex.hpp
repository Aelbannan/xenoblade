#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CEventFile.hpp"

#include <nw4r/lyt.h>

// IWorkEvent-compatible vtable (split1 .data).
extern "C" void* lbl_eu_80533DC8[];

/* Background layout texture widget. Layout-compatible with IWorkEvent (vptr @0)
for CDeviceFile::readFile, but not a C++ IWorkEvent subclass (avoids weak stubs).

US retail strips most member manglings (__ct__CBgTex, func_801C3A24, ...); dtor and
OnFileEvent keep C++ names. See CBgTex.cpp. */
struct CBgTex {
    void* mVtbl; // 0x0 - lbl_eu_80533DC8
    UnkClass_8045F564 mMemRegion; // 0x4
    CFileHandle* mFileHandle; // 0x14
    nw4r::lyt::Layout* mLayout; // 0x18
    bool mLayoutReady; // 0x1C
    u8 mLoaded; // 0x1D
    u8 mPtmMode; // 0x1E

    CBgTex(u8 arg);
    void func_801C3A24();
    bool func_801C3C14();
    void func_801C3D54();
    void func_801C3D7C(nw4r::lyt::DrawInfo* drawInfo);
    void func_801C3D9C();
    u8 func_801C3E34();
    void func_801C3E3C();
    ~CBgTex();
    bool OnFileEvent(CEventFile* pEventFile);
};
