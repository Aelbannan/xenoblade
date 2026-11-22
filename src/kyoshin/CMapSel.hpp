#pragma once

#include <types.h>
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CEventFile.hpp"

namespace nw4r { namespace lyt { class Layout; class AnimTransform; class DrawInfo; class ArcResourceAccessor; } }
struct CFileHandle;

/* Map selection UI widget (retail RTTI "CMapSel", IWorkEvent base). Shows a
   10-row list of the maps the player may jump to.

   State machine (mState):
     0 = hidden, 1 = opening, 2 = open-in animation, 3 = active,
     4 = closing, 5 = post-close cleanup */
class CMapSel : public IWorkEvent {
public:
    CMapSel();
    virtual ~CMapSel();
    virtual bool OnFileEvent(CEventFile* pEventFile);

    void startLoad();
    void update();
    void draw(nw4r::lyt::DrawInfo* drawInfo);
    void term();
    int getVisibleFlag();
    int getInitFlag();
    void init();
    void beginClose();
    void moveUp();
    void moveDown();
    void movePageUp();
    void movePageDown();
    u8 getMapGridCell();
    void tickOpen();
    void tickOpenIn();
    void tickClose();
    void tickCloseEnd();
    void activate();
    void refreshGridText();
    void refreshImage();
    void loadSelectedMapResource();
    void requestImage();
    void updateCursor();

    UnkClass_8045F564 mMemRegion;                  // 0x04
    CFileHandle* mFileHandle;                      // 0x14 - layout archive
    CFileHandle* mFileHandle2;                     // 0x18 - map image
    nw4r::lyt::ArcResourceAccessor* mArcAccessor;  // 0x1C
    void* mAllocatedMem;                           // 0x20 - loaded map image
    nw4r::lyt::Layout* mLayout;                    // 0x24
    nw4r::lyt::AnimTransform* mAnimTransform1;     // 0x28
    nw4r::lyt::AnimTransform* mAnimTransform2;     // 0x2C
    u8 mActive;                                    // 0x30
    u8 mState;                                     // 0x31
    u8 mVisible;                                   // 0x32
    u8 mInitFlag;                                  // 0x33
    CScrollBar mScrollBar;                         // 0x34
    CCur18 mCursor;                                // 0x74
    s8 mSelY;                                      // 0x8C
    s8 mSelX;                                      // 0x8D
    u8 mGrid[0x20];                                // 0x8E - selectable map ids
    u8 mCount;                                     // 0xAE
    u8 mImagePending;                              // 0xAF
    float mTimer;                                  // 0xB0 - image load delay
};

// ---- imports folded from the former CMapSelImports.hpp ----
// TU-private retail imports and layout views for CMapSel.cpp (and the
// CMenuMapSelect implementation in code_802405F4.cpp). Kept out of
// kyoshin/CMapSel.hpp so the class header can be included next to
// kyoshin/CFloorMap.hpp.

#include "kyoshin/CMapSel.hpp"
#include "kyoshin/code_80135FDC_Bdat.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"

// Scrollbar destroy / layout-read helpers (unmangled retail symbols).

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 Bdat_GetMaxRow_B1EC(void*);
#include "kyoshin/cf/CtrlRemoteApi.hpp"
// Cursor / scrollbar / layout C-ABI helpers (unmangled retail symbols).
// cursor per-frame update
// Layout/text C-ABI helpers normally declared by code_80135FDC.hpp. CMapSel.cpp
// defines CODE_80135FDC_CPP before including that header, so those exports are
// skipped there and the declarations below apply (retail CMapSel callers pass
// the texture resource as PaneSetTexPaletteByName's third argument). Same guard as the legacy
// AnimRewindFrame pseudo-import so code_802405F4.cpp's define stays a no-op.
#ifndef KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
#endif
// Cursor position helper: fill output with the point derived from two panes.
// Grid-text helpers / cursor construction (C-ABI retail symbols).
extern "C" int sprintf(char*, const char*, ...);
// Map-select string pool (split1 .rodata): pane-name format at +0x1F, fixed
// pane name at +0x88.
extern "C" char lbl_eu_8050B4A8[];
// BDAT map-table pointers (.sbss, sda21-relative).
// Current map id (.sbss) - initial grid selection target.
extern u32 lbl_eu_80664184;
// Canonical AnimRewindFrame (nw4r::lyt::AnimTransform*, float) is declared in
// code_80135FDC.hpp. CMapSel.cpp includes that header and passes proper
// AnimTransform* pointers; the guard below stays for TUs that include both
// headers (identical redeclarations are harmless, but keep the old macro
// contract so code_802405F4.cpp's define stays a no-op).
#ifndef KYOSHIN_SKIP_CMAPSEL_LEGACY_LAYOUT_IMPORTS
#endif
