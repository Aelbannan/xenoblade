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
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Retail imports (defined outside this TU). Animation frame constant.
// `const` routes it into the readonly sdata2 pool so MWCC hoists the lfs above
// the frame stores (CExchangeWin ExWin_AdvanceEnter/D244 pattern, MWCC_CASES).
extern const float lbl_eu_80668600;
// u32->float cast-magic constant (2^52) shared with the retail sdata2 pool.
extern const double lbl_eu_80668608;
// Shop window arc file path string (split1 .rodata).
extern "C" char lbl_eu_8050A62C[];
// Animate the attached anim-transform forward to a frame (returns true when done).
// Animate the attached anim-transform in reverse (returns true when done).
// Retail symbol is unmangled (C linkage).
// Draw a nw4r layout with the given draw info.
// Fire a per-window sound/effect event by id.
// Sum the translate of a pane and its ancestors into an output VEC3. C-ABI.

// OnFileEvent imports (retail symbols, unmangled / pre-mangled C-ABI names).
extern "C" {
void writePanePos(nw4r::lyt::Pane*, float*);
void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
void* getPlayer__Q22cf13CfGameManagerFi(int);
}
// C++-mangled retail helper (actor id -> action source).
void* findObjectById(int id);

/* Shop window widget; receives its arc through IWorkEvent::OnFileEvent. */
class CSelShopWin : public IWorkEvent {
public:
    CSelShopWin();
    virtual ~CSelShopWin();
    void loadShopWinArc();
    void driveShopWinAnim();
    void drawShopWin(nw4r::lyt::DrawInfo* drawInfo);
    void teardownShopWin();
    void showShopWin();
    void hideShopWin();
    void getShopWinPanePos(int);
    void markShopWinBuilt();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    int isShopWinLoaded();
    int isShopWinAnimActive();

    // +0x00: IWorkEvent vptr
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
    void advanceShopWinOpen();
    void rewindShopWinClose();
};
