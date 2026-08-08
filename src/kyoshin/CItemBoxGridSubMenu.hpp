#pragma once

#include <types.h>

#include <nw4r/lyt.h>

// CItemBoxGridSubMenu -- submenu overlay for the item grid screen.
//
// Owns a small nw4r layout (one root pane + three text boxes) and tracks
// its own visibility/phase lifecycle independently of the parent grid.
//
// State field overview:
//   mAnimState     0=closed, 1=opening, 2=open, 3=closing
//   mSubState      Sub-menu action (1, 2, 3 = different sub-actions)
//   mSelectedIdx   Index into [mTxtBoxA, mTxtBoxB, mTxtBoxC] selected item

struct CItemBoxGridSubMenu {
    void* mVtbl;                                     // 0x00
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

    ~CItemBoxGridSubMenu();

    u8 func_80208358();
    u8 func_80208360();
    void func_802083CC();
    void func_80208368();
    void func_8020844C();
    void func_802084D4(int arg);
};

void func_80207FC8(CItemBoxGridSubMenu* self, nw4r::lyt::ArcResourceAccessor* accessor);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_80138078__FUl(u32);
extern "C" char lbl_eu_805084BC[];
extern "C" const float lbl_eu_80668300; // 80.0f
extern "C" const float lbl_eu_80668304; // 0.0f
extern "C" const float lbl_eu_80668308; // 3.0f
extern "C" const float lbl_eu_8066830C; // 100.0f
extern "C" const float lbl_eu_80668310; // -17.0f
extern "C" const float lbl_eu_80668314; // 120.0f
extern "C" const float lbl_eu_80668318; // -37.0f
extern "C" const double lbl_eu_806682F8;
extern "C" u32 func_801355A0__Fv();
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int);
extern "C" void* func_801355F4();
extern "C" void func_80137038__FPQ34nw4r3lyt6LayoutPQ34nw4r3lyt8DrawInfoii(void*, void*, int, int);
