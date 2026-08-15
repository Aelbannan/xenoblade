#pragma once

#include <types.h>

#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/CEventFile.hpp"

#include <nw4r/lyt.h>

// Shared string pool (split1 .rodata).
extern char lbl_eu_80505370[];

// SDA globals (retail names; defined in common/sbss splits).
extern u32 lbl_eu_80664184;
extern s32 lbl_eu_80664460;
extern nw4r::lyt::ArcResourceAccessor* lbl_eu_80664464;

// IWorkEvent-compatible vtable for CBgTex (split1 .data; 0x88 = 34 slots):
// [rtti @0, toffset @4, dtor @8 (__dt__6CBgTexFv), 31 event handlers @0xC].
// Only its address is stored here - CDeviceFile::readFile dispatches through it.
extern u32 lbl_eu_80533DC8[0x22];

struct CBgTexVtbl {
    u32 rtti;         // 0x00 - typeinfo (0x80662650)
    u32 toffset;      // 0x04
    u32 dtor;         // 0x08 - __dt__6CBgTexFv
    u32 handlers[31]; // 0x0C..0x88 - IWorkEvent event handlers
};

/* Sets mVtbl before the mem-region member is constructed (retail ctor order).
Same pattern as CBatteryVtblBase in CBattery.hpp: an inline base ctor makes
MWCC emit the vptr store first, then the member ctors. */
struct CBgTexVtblBase {
    CBgTexVtbl* mVtbl; // 0x0 - lbl_eu_80533DC8

    CBgTexVtblBase() {
        mVtbl = reinterpret_cast<CBgTexVtbl*>(lbl_eu_80533DC8);
    }
};

/* Background layout texture widget. Layout-compatible with IWorkEvent (vptr @0)
for CDeviceFile::readFile, but not a C++ IWorkEvent subclass (avoids weak stubs).

US retail strips most member manglings (__ct__CBgTex, func_801C3A24, ...); dtor and
OnFileEvent keep C++ names. See CBgTex.cpp. */
struct CBgTex : public CBgTexVtblBase {
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
