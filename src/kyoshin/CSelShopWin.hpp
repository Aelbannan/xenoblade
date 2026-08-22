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
#include <nw4r/lyt/lyt_drawInfo.h>
#include <nw4r/math/math_types.h>

// CSelShopWin vtable (lbl_eu_805365A8)
extern "C" void* lbl_eu_805365A8[];

// Retail imports (defined outside this TU). Animation frame constant.
// `const` routes it into the readonly sdata2 pool so MWCC hoists the lfs above
// the frame stores (CExchangeWin func_8022D1F8/D244 pattern, MWCC_CASES).
extern const float lbl_eu_80668600;
// u32->float cast-magic constant (2^52) shared with the retail sdata2 pool.
extern const double lbl_eu_80668608;
// Shop window arc file path string (split1 .rodata).
extern "C" char lbl_eu_8050A62C[];
// Animate the attached anim-transform forward to a frame (returns true when done).
u32 func_80137444(nw4r::lyt::AnimTransform*, float);
// Animate the attached anim-transform in reverse (returns true when done).
// Retail symbol is unmangled (C linkage).
extern "C" {
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
}
// Draw a nw4r layout with the given draw info.
void func_80137038(nw4r::lyt::Layout*, nw4r::lyt::DrawInfo*, int, int);
// Fire a per-window sound/effect event by id.
void func_80138078(u32);
// Sum the translate of a pane and its ancestors into an output VEC3. C-ABI.
extern "C" void func_80137924(nw4r::math::VEC3*, nw4r::lyt::Pane*, nw4r::lyt::Pane*, nw4r::lyt::Pane*);

// OnFileEvent imports (retail symbols, unmangled / pre-mangled C-ABI names).
extern "C" {
void* func_801355F4();
void* func_80138DA4(const char*);
void func_80124288(nw4r::lyt::Pane*, float*);
void* func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
void* getPlayer__Q22cf13CfGameManagerFi(int);
int func_80086F9C__Q22cf13CfGameManagerFv(int);
}
// C++-mangled retail helper (actor id -> action source).
void* func_800B708C(int id);

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
    void func_8022C830(nw4r::lyt::DrawInfo* drawInfo);
    void func_8022C85C();
    void func_8022C8E0();
    void func_8022C908();
    void func_8022C930(int);
    void func_8022CA6C();
    bool OnFileEvent(CEventFile* pEventFile);

    u8 func_8022C8D0();
    u8 func_8022C8D8();

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
