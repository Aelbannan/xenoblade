// Auto-scaffolded catalog TU for kyoshin/CPresentWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPresentWin.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <stdio.h>

// Forward decls for cross-referenced unit functions (bodies below). Retail
// symbols are unmangled, so the definitions keep C linkage.
extern "C" void func_8022E698(CPresentWin* self);
extern "C" void func_8022E744(CPresentWin* self);
extern "C" u8 func_8022E868(CPresentWin* self, u32 r4);

// Destructor - base object destructor (D2); MWCC emits D1 with delete check
CPresentWin::~CPresentWin() {
}

// Draw the present window layout if active and in the right state
void func_8022DAD8(CPresentWin* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mField30 == 0) return;
    if (self->mField37 == 0) return;
    func_80137038(self->mpLayout, drawInfo, 0, 1);
}

u8 func_8022DB6C(CPresentWin* self) { return self->mField30; }


u8 func_8022DB74(CPresentWin* self) { return self->mField38; }

// State-machine transition: if state == 2, advance to state 3 and clear sub-state
void func_8022DD68(CPresentWin* self) {
    if (self->mField37 != 2) return;
    self->mField37 = 3;
    self->mField38 = 0;
    func_80138078(0xe);
}

// Present-window rank display (state 2): look up the player's time for the
// current item, show the appropriate rank/result panes + textures, then hand
// control to the closing animation.
void func_8022DD90(CPresentWin* self) {
    if (self->mField37 != 2) return;

    u32 itemId = func_80139358(self->mField34) & 0xFFFF;

    u8 rank = func_8022E868(self, self->mField33);
    if (rank == 8) rank = 3;

    char buf[0x20];
    sprintf(buf, &lbl_eu_8050A84C[0x12a], rank);

    int res = func_801362C0((const char*)lbl_eu_80664104, buf, (const char*)itemId);

    if ((s8)res < 0) {
        // Time not met: hide the result panes, show the failure panes and a
        // texture matching how far off the player was.
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x133], 1), 1);
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x13e], 1), 0);
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x149], 1), 1);

        void* tex;
        if ((s8)res <= -0x32) {
            tex = func_801355F4()->GetResource(0x74696D67, &lbl_eu_8050A84C[0x153], NULL);
        } else if ((s8)res <= -0x10) {
            tex = func_801355F4()->GetResource(0x74696D67, &lbl_eu_8050A84C[0x169], NULL);
        } else {
            tex = func_801355F4()->GetResource(0x74696D67, &lbl_eu_8050A84C[0x17f], NULL);
        }
        if (tex) {
            func_80137E7C(self->mpLayout, &lbl_eu_8050A84C[0x149], tex);
        }
    } else {
        // Time met: show the result panes and a texture matching the rank.
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x133], 1), 0);
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x13e], 1), 1);
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x195], 1), 1);

        void* tex;
        if ((s8)res >= 0x12c) {
            tex = func_801355F4()->GetResource(0x74696D67, &lbl_eu_8050A84C[0x19f], NULL);
        } else if ((s8)res >= 0x32) {
            tex = func_801355F4()->GetResource(0x74696D67, &lbl_eu_8050A84C[0x153], NULL);
        } else if ((s8)res >= 0x15) {
            tex = func_801355F4()->GetResource(0x74696D67, &lbl_eu_8050A84C[0x1b5], NULL);
        } else if ((s8)res >= 0x10) {
            tex = func_801355F4()->GetResource(0x74696D67, &lbl_eu_8050A84C[0x169], NULL);
        } else {
            tex = func_801355F4()->GetResource(0x74696D67, &lbl_eu_8050A84C[0x17f], NULL);
        }
        if (tex) {
            func_80137E7C(self->mpLayout, &lbl_eu_8050A84C[0x195], tex);
        }
    }

    // Rank sound + collection bookkeeping (skipped entirely for a 0 result).
    if ((s8)res >= 1) {
        func_80138078(0x34);
    } else if ((s8)res == 0) {
        goto tail;
    } else {
        func_80138078(0x36);
    }
    {
        u8 idxA = func_8022E868(self, self->mField32);
        u8 idxB = func_8022E868(self, self->mField33);
        func_8013A95C(idxA, idxB, (s8)res);
    }

tail:
    // Rebuild the item labels and message texture, then start the close
    // animation.
    char* s1 = func_80136190(&lbl_eu_8050A84C[0xc5], &lbl_eu_8050A84C[0xd3], 0x2c);
    func_80136B4C(self->mpLayout, &lbl_eu_8050A84C[0xd8], s1, 0);
    func_80136B4C(self->mpLayout, &lbl_eu_8050A84C[0x100], &lbl_eu_8050A84C[0xc4], 0);

    void* tex = self->mAccessor->GetResource(0x74696D67, &lbl_eu_8050A84C[0x117], NULL);
    if (tex) {
        func_80137E7C(self->mpLayout, &lbl_eu_8050A84C[0x10d], tex);
    }

    self->mField37 = 4;
    self->mField38 = 0;
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, false);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, true);
    self->mpAnimTrans1->SetFrame(lbl_eu_80668628);
}

void func_8022E204(){}

extern "C" __declspec(noinline) void func_8022E254(CPresentWin* self) {}

// Advance the present-window frame counter. mField31 selects which counter
// advances; mField32/mField33 wrap around at mDataCount. If the two counters
// collide after an advance, the secondary one steps again.
void func_8022E2F8(CPresentWin* self) {
    if (self->mField36 != 0) return;
    if (self->mField31 != 0) {
        self->mField33 = (u8)(self->mField33 + 1);
        if ((s8)self->mField33 >= self->mDataCount) self->mField33 = 0;
        if ((s8)self->mField33 == (s8)self->mField32) {
            self->mField33 = (u8)(self->mField33 + 1);
            if ((s8)self->mField33 >= self->mDataCount) self->mField33 = 0;
        }
        func_8022E744(self);
    } else {
        self->mField32 = (u8)(self->mField32 + 1);
        if ((s8)self->mField32 >= self->mDataCount) self->mField32 = 0;
        func_8022E698(self);
    }
}

// retail: tail-branch to func_8022E254 (passes `this` through in r3)
extern "C" void func_8022E3A8(CPresentWin* self) { func_8022E254(self); }

// Toggle mField31 (which animation path is active). When toggling ON, skip the
// frame if the secondary counter would collide; when OFF, repaint the label
// and (re)load the texture bound to mPane2C.
void func_8022E3AC(CPresentWin* self) {
    self->mField31 = (u32)(self->mField31 ^ 1) > 0;
    if (self->mField31) {
        if ((s8)self->mField33 == (s8)self->mField32) {
            self->mField33 = (u8)(self->mField32 + 1);
            if ((s8)self->mField33 >= self->mDataCount) self->mField33 = 0;
        }
        func_8022E744(self);
    } else {
        func_80136D74((nw4r::lyt::Layout*)self->mPane24, &lbl_eu_8050A84C[0xc4], 0);
        void* tex = self->mAccessor->GetResource(0x74696D67, &lbl_eu_8050A84C[0x117], NULL);
        if (tex) {
            func_80137F88(self->mPane2C, tex);
        }
    }
}

u8 func_8022E488(CPresentWin* self) { return self->mField31; }

u8 func_8022E490(CPresentWin* self) { return self->mField36; }

void func_8022E498(){}

// retail: lbz r4,0x32(r3); b func_8022E868
extern "C" u8 func_8022E4FC(CPresentWin* self) { return func_8022E868(self, self->mField32); }

// retail: lbz r4,0x33(r3); b func_8022E868
extern "C" u8 func_8022E504(CPresentWin* self) { return func_8022E868(self, self->mField33); }

void func_8022E50C(){}

void func_8022E558(){}

// Frame-update handler: when animTrans1 finishes, switch to state 2, enable
// both animations, show the window and reset the two labelled panes.
void func_8022E5B0(CPresentWin* self) {
    if (func_80137444(self->mpAnimTrans1, lbl_eu_8066862C) != 0) {
        self->mField37 = 2;
        self->mField38 = 1;
        self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, false);
        self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, true);
        self->mField36 = 1;
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x133], 1), 0);
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x13e], 1), 0);
    }
}

// Stubs for the two frame-advance helpers (not targets this session). Their
// bodies MUST stay non-inlinable: the retail callers tail-call them (`b`), so
// an inlined empty body would collapse the call sites' control flow.
extern "C" __declspec(noinline) void func_8022E698(CPresentWin* self) {}
extern "C" __declspec(noinline) void func_8022E744(CPresentWin* self) {}

void func_8022E7F0(CPresentWin* self) {
    self->mDataCount = 0;
    u8 count = code80135FDC_getByte_64077();
    for (u8 i = 0; i < count; i++) {
        u8 val = func_801392B4(i);
        if (val <= 8) {
            self->mDataArray[self->mDataCount] = val;
            self->mDataCount++;
        }
    }
}

extern "C" __declspec(noinline) u8 func_8022E868(CPresentWin* self, u32 r4) {
    u8 limit = self->mDataCount;
    if (r4 >= limit) return 0;
    return self->mDataArray[r4];
}

// Open handler: mark the window active, rebuild the item data, repaint the
// labels, load the "timg" texture for mPane2C and the message texture, then
// position the message pane at the texture's dimensions.
void func_8022DB7C(CPresentWin* self) {
    if (self->mField37 != 0) return;
    self->mField37 = 1;
    self->mField38 = 0;
    self->mField30 = 1;
    func_8022E7F0(self);
    func_8022E698(self);

    func_80136D74((nw4r::lyt::Layout*)self->mPane24, &lbl_eu_8050A84C[0xc4], 0);
    void* tex = self->mAccessor->GetResource(0x74696D67, &lbl_eu_8050A84C[0x117], NULL);
    if (tex) {
        func_80137F88(self->mPane2C, tex);
    }

    char* s1 = func_80136190(&lbl_eu_8050A84C[0xc5], &lbl_eu_8050A84C[0xd3], 0x2b);
    func_80136B4C(self->mpLayout, &lbl_eu_8050A84C[0xd8], s1, 0);
    char* s2 = func_80136190(&lbl_eu_8050A84C[0xc5], &lbl_eu_8050A84C[0xd3], 0x9b);
    func_80136B4C(self->mpLayout, &lbl_eu_8050A84C[0x100], s2, 0);

    const char* msgName = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                              ? &lbl_eu_8050A84C[0xe4]
                              : &lbl_eu_8050A84C[0xed];
    char* handle = func_80138F78(func_8013606C(&lbl_eu_8050A84C[0xc5], msgName, 0x9b));
    nw4r::lyt::ArcResourceAccessor* mgr = func_801355F4();
    void* tex2 = mgr->GetResource(0x74696D67, handle, NULL);
    if (tex2) {
        func_80137E7C(self->mpLayout, &lbl_eu_8050A84C[0x10d], tex2);
        nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x10d], 1);
        if (pane) {
            CPresentTexDims* dims = ((CPresentTexObj*)tex2)->mChain->mDims;
            u16 h = dims->mH;
            u16 w = dims->mW;
            float src[2];
            src[0] = (float)(u32)h;
            src[1] = (float)(u32)w;
            func_80124288(pane, src);
        }
    }
    func_80138078(0xd);
}

// Init: load the layout + animation transforms, bind the font, enable the
// default animation and look up the 7 pane handles used by the window.
void func_8022D614(CPresentWin* self, nw4r::lyt::ArcResourceAccessor* accessor) {
    self->mAccessor = accessor;
    func_80136E84(&self->mpLayout, accessor, lbl_eu_8050A84C);
    func_80136F08(self->mpLayout, &self->mpAnimTrans0, accessor, &lbl_eu_8050A84C[0x19]);
    func_80136F08(self->mpLayout, &self->mpAnimTrans1, accessor, &lbl_eu_8050A84C[0x35]);

    nw4r::lyt::Pane* rootPane = self->mpLayout->GetRootPane();
    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->mpLayout);
    u32 fontResult = ((CPresentFontView*)fontObj)->sf9();
    func_8013676C(rootPane, fontResult);

    self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, true);
    self->mpLayout->Animate(0);

    self->mPane14 = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x51], 1);
    self->mPane18 = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x60], 1);
    self->mPane1C = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x6d], 1);
    self->mPane20 = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x7a], 1);
    self->mPane24 = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x87], 1);
    self->mPane28 = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x94], 1);
    self->mPane2C = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0xa1], 1);

    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0xae], 1), 0);
    func_80136B4C(self->mpLayout, &lbl_eu_8050A84C[0xb7], &lbl_eu_8050A84C[0xc4], 0);
    func_80136D74((nw4r::lyt::Layout*)self->mPane20, &lbl_eu_8050A84C[0xc4], 0);
    func_80136D74((nw4r::lyt::Layout*)self->mPane24, &lbl_eu_8050A84C[0xc4], 0);

    char* s1 = func_80136190(&lbl_eu_8050A84C[0xc5], &lbl_eu_8050A84C[0xd3], 0x2b);
    func_80136B4C(self->mpLayout, &lbl_eu_8050A84C[0xd8], s1, 0);

    const char* msgName1 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                               ? &lbl_eu_8050A84C[0xe4]
                               : &lbl_eu_8050A84C[0xed];
    char* handle1 = func_80138F78(func_8013606C(&lbl_eu_8050A84C[0xc5], msgName1, 0x2b));
    nw4r::lyt::ArcResourceAccessor* mgr1 = func_801355F4();
    void* tex1 = mgr1->GetResource(0x74696D67, handle1, NULL);
    if (tex1) {
        func_80137E7C(self->mpLayout, &lbl_eu_8050A84C[0x10d], tex1);
        nw4r::lyt::Pane* pane1 = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x10d], 1);
        if (pane1) {
            CPresentTexDims* dims1 = ((CPresentTexObj*)tex1)->mChain->mDims;
            u16 h1 = dims1->mH;
            u16 w1 = dims1->mW;
            float src1[2];
            src1[0] = (float)(u32)h1;
            src1[1] = (float)(u32)w1;
            func_80124288(pane1, src1);
        }
    }

    char* s2 = func_80136190(&lbl_eu_8050A84C[0xc5], &lbl_eu_8050A84C[0xd3], 0x9b);
    func_80136B4C(self->mpLayout, &lbl_eu_8050A84C[0x100], s2, 0);

    const char* msgName2 = func_80086F9C__Q22cf13CfGameManagerFv(-1)
                               ? &lbl_eu_8050A84C[0xe4]
                               : &lbl_eu_8050A84C[0xed];
    char* handle2 = func_80138F78(func_8013606C(&lbl_eu_8050A84C[0xc5], msgName2, 0x9b));
    nw4r::lyt::ArcResourceAccessor* mgr2 = func_801355F4();
    void* tex2 = mgr2->GetResource(0x74696D67, handle2, NULL);
    if (tex2) {
        func_80137E7C(self->mpLayout, &lbl_eu_8050A84C[0x10d], tex2);
        nw4r::lyt::Pane* pane2 = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x10d], 1);
        if (pane2) {
            CPresentTexDims* dims2 = ((CPresentTexObj*)tex2)->mChain->mDims;
            u16 h2 = dims2->mH;
            u16 w2 = dims2->mW;
            float src2[2];
            src2[0] = (float)(u32)h2;
            src2[1] = (float)(u32)w2;
            func_80124288(pane2, src2);
        }
    }
}

void func_8022DA58() {}
