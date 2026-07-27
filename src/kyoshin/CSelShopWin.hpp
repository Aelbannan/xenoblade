#pragma once

#include <types.h>
#include "monolib/core.hpp"
#include "monolib/work.hpp"
#include "monolib/lib.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>

// CSelShopWin vtable (lbl_eu_805365A8)
extern "C" void* lbl_eu_805365A8[];

/* Vtable base for CSelShopWin. Stores the vtable pointer at +0x00 so layout
matches retail (IWorkEvent-compatible vptr), without inheriting from IWorkEvent
which would emit weak stubs and blow the split budget. */
struct CSelShopWinVtblBase {
    void* mVtbl; // 0x0 - lbl_eu_805365A8

    CSelShopWinVtblBase() {
        mVtbl = lbl_eu_805365A8;
    }
};

/* Shop window widget. Layout-compatible with IWorkEvent (vptr @ +0) for
CDeviceFile::readFile. */
class CSelShopWin : public CSelShopWinVtblBase {
public:
    CSelShopWin();
    ~CSelShopWin();
    void func_8022C770();
    void func_8022C7C0();
    void func_8022C830();
    void func_8022C85C();
    void func_8022C8E0();
    void func_8022C908();
    void func_8022C930(int);
    void func_8022C9D4();
    void func_8022CA20();
    void func_8022CA6C();
    bool OnFileEvent(CEventFile* pEventFile);

    // +0x00: vtable (lbl_eu_805365A8 via CSelShopWinVtblBase)
    // +0x04: layout memory region
    UnkClass_8045F564 mMemRegion;
    // +0x14: file handle for loading arc resources
    CFileHandle* mFileHandle;
    // +0x18: NW4R layout arc resource accessor
    nw4r::lyt::ArcResourceAccessor* mAccessor;
    // +0x1C: NW4R layout instance
    nw4r::lyt::Layout* mLayout;
    // +0x20: animation transform
    nw4r::lyt::AnimTransform* mAnimTransform;
    // +0x24: layout is built and active
    u8 mIsLayoutBuilt;
    // +0x25: resources loaded and ready for interaction
    u8 mIsLoaded;
    // +0x26: animation state (0=idle, 1=opening, 2=open, 3=closing)
    u8 mAnimState;
    // +0x27: animation in progress flag
    u8 mAnimActive;
};
