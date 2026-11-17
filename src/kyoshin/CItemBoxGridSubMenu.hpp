#pragma once

#include <types.h>

#include <nw4r/lyt.h>
#include "monolib/work/IWorkEvent.hpp"

// CItemBoxGridSubMenu -- submenu overlay for the item grid screen.
//
// Owns a small nw4r layout (one root pane + three text boxes) and tracks
// its own visibility/phase lifecycle independently of the parent grid.
//
// State field overview:
//   mAnimState     0=closed, 1=opening, 2=open, 3=closing
//   mSubState      Sub-menu action (1, 2, 3 = different sub-actions)
//   mSelectedIdx   Index into [mTxtBoxA, mTxtBoxB, mTxtBoxC] selected item

class CItemBoxGridSubMenu : public IWorkEvent {
public:
    nw4r::lyt::ArcResourceAccessor* mAccessor;       // 0x04 -- resource accessor
    nw4r::lyt::Layout* mLayout;                      // 0x08 -- layout
    nw4r::lyt::AnimTransform* mAnimDefault;          // 0x0C -- default animation
    nw4r::lyt::Pane* mRootPane;                      // 0x10 -- first child pane
    nw4r::lyt::TextBox* mTxtBoxA;                    // 0x14 -- textbox A
    nw4r::lyt::TextBox* mTxtBoxB;                    // 0x18 -- textbox B
    nw4r::lyt::TextBox* mTxtBoxC;                    // 0x1C -- textbox C
    u8 mIsVisible;                                   // 0x20 -- whether drawn/interactive
    u8 mIsOpened;                                    // 0x21 -- whether fully opened
    u8 mAnimState;                                   // 0x22 -- 0=closed, 1=opening, 2=open, 3=closing
    u8 mSubState;                                    // 0x23 -- 0=idle, 1/2/3 = sub-action
    s8 mSelectedIdx;                                 // 0x24 -- index of selected item, -1 = none

    CItemBoxGridSubMenu();
    virtual ~CItemBoxGridSubMenu();

    void GridSubMenu_InitLayout(nw4r::lyt::ArcResourceAccessor* accessor);
    void GridSubMenu_Move();
    void GridSubMenu_DrawLayoutGated(void* drawInfo);
    void GridSubMenu_TermLayout();
    void GridSubMenu_AdvanceState2To3();
    void GridSubMenu_ClearSelection();
    void GridSubMenu_FinishOpenAnim();
    void GridSubMenu_RewindSelection();

    int GridSubMenu_IsVisibleFlag();
    int GridSubMenu_IsOpenedFlag();
    void GridSubMenu_MoveSelectionPrev();
    void GridSubMenu_BeginOpenAnim();
    void GridSubMenu_MoveSelectionNext();
    void GridSubMenu_SetupOptions(int arg);
};

extern "C" void GridSubMenu_GetCursorPos(void* self, void* src);

