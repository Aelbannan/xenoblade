// Auto-scaffolded catalog TU for kyoshin/CPresentWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPresentWin.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <stdio.h>

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

// Retail uses the -O4,s shape (_savegpr_29 at only 3 saved GPRs).
#pragma push
#pragma optimize_for_size on
// Present-window rank display (state 2): look up the player's time for the
// current item, show the appropriate rank/result panes + textures, then hand
// control to the closing animation.
void func_8022DD90(CPresentWin* self) {
    if (self->mField37 != 2) return;

    u32 itemId = func_80139358(self->mField34) & 0xFFFF;

    u8 rank = func_8022E868(self, self->mField33);
    if (rank == 8) rank = 3;

    // String-pool base kept in a callee-saved register by retail.
    char* pool = lbl_eu_8050A84C;

    char buf[0x20];
    sprintf(buf, pool + 0x12a, rank);

    int res = func_801362C0((const char*)lbl_eu_80664104, buf, (const char*)itemId);

    if ((s8)res < 0) {
        // Time not met: hide the result panes, show the failure panes and a
        // texture matching how far off the player was.
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x133, 1), 1);
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x13e, 1), 0);
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x149, 1), 1);

        void* tex;
        if ((s8)res <= -0x32) {
            tex = func_801355F4()->GetResource(0x74696D67, pool + 0x153, NULL);
        } else if ((s8)res <= -0x10) {
            tex = func_801355F4()->GetResource(0x74696D67, pool + 0x169, NULL);
        } else {
            tex = func_801355F4()->GetResource(0x74696D67, pool + 0x17f, NULL);
        }
        if (tex) {
            func_80137E7C(self->mpLayout, pool + 0x149, tex);
        }
    } else {
        // Time met: show the result panes and a texture matching the rank.
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x133, 1), 0);
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x13e, 1), 1);
        func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x195, 1), 1);

        void* tex;
        if ((s8)res >= 0x12c) {
            tex = func_801355F4()->GetResource(0x74696D67, pool + 0x19f, NULL);
        } else if ((s8)res >= 0x32) {
            tex = func_801355F4()->GetResource(0x74696D67, pool + 0x153, NULL);
        } else if ((s8)res >= 0x15) {
            tex = func_801355F4()->GetResource(0x74696D67, pool + 0x1b5, NULL);
        } else if ((s8)res >= 0x10) {
            tex = func_801355F4()->GetResource(0x74696D67, pool + 0x169, NULL);
        } else {
            tex = func_801355F4()->GetResource(0x74696D67, pool + 0x17f, NULL);
        }
        if (tex) {
            func_80137E7C(self->mpLayout, pool + 0x195, tex);
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
    char* s1 = func_80136190(pool + 0xc5, pool + 0xd3, 0x2c);
    func_80136B4C(self->mpLayout, pool + 0xd8, s1, 0);
    func_80136B4C(self->mpLayout, pool + 0x100, pool + 0xc4, 0);

    void* tex = self->mAccessor->GetResource(0x74696D67, pool + 0x117, NULL);
    if (tex) {
        func_80137E7C(self->mpLayout, pool + 0x10d, tex);
    }

    self->mField37 = 4;
    self->mField38 = 0;
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, false);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, true);
    self->mpAnimTrans1->SetFrame(lbl_eu_80668628);
}
#pragma pop

// Store the anim index (+0x34), call the anim loader, then bind the loaded
// frame (+0xB7 offset string) through the +8 layout.
extern "C" void* func_801394D4(u32);
extern "C" void func_8022E204(void* self, u16 idx) {
    *(u16*)((u8*)self + 0x34) = idx;
    void* anim = func_801394D4(idx);
    func_80136B4C((nw4r::lyt::Layout*)*(void**)((u8*)self + 8), lbl_eu_8050A84C + 0xB7, (char*)anim, 0);
}

// Constructor: install the retail vtable, zero every field, except mField38
// which starts at 1.
CPresentWin::CPresentWin() {
    mVtable = lbl_eu_805366D8;
    mAccessor = NULL;
    mpLayout = NULL;
    mpAnimTrans0 = NULL;
    mpAnimTrans1 = NULL;
    mPane14 = NULL;
    mPane18 = NULL;
    mPane1C = NULL;
    mPane20 = NULL;
    mPane24 = NULL;
    mPane28 = NULL;
    mPane2C = NULL;
    mField30 = 0;
    mField31 = 0;
    mField32 = 0;
    mField33 = 0;
    mField34 = 0;
    mField36 = 0;
    mField37 = 0;
    mField38 = 1;
    mDataCount = 0;
}

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
#pragma push
#pragma optimize_for_size on
void func_8022E3AC(CPresentWin* self) {
    self->mField31 = (u8)((self->mField31 ^ 1) != 0);
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
#pragma pop

u8 func_8022E488(CPresentWin* self) { return self->mField31; }

u8 func_8022E490(CPresentWin* self) { return self->mField36; }

// Get the world position of the selected pane of another present window and
// scale its x by that window's label pane (+0x14) horizontal scale factor.
void func_8022E498(nw4r::math::VEC3* out, CPresentWin* src) {
    func_801375A0(out, src->mField31 != 0 ? src->mPane1C : src->mPane18);
    out->x = out->x * src->mPane14->GetScale().x;
}

// retail: lbz r4,0x32(r3); b func_8022E868
extern "C" u8 func_8022E4FC(CPresentWin* self) { return func_8022E868(self, self->mField32); }

// retail: lbz r4,0x33(r3); b func_8022E868
extern "C" u8 func_8022E504(CPresentWin* self) { return func_8022E868(self, self->mField33); }

extern "C" __declspec(noinline) void func_8022E50C(CPresentWin* self) {
    if (func_80137444(self->mpAnimTrans0, lbl_eu_8066862C) != 0) {
        self->mField37 = 2;
        self->mField38 = 1;
    }
}

extern const f32 lbl_eu_8066862C;
extern "C" __declspec(noinline) void func_8022E558(CPresentWin* self) {
    if (func_80137510((nw4r::lyt::AnimTransform*)*(void**)((u8*)self + 0xC), lbl_eu_8066862C)) {
        *((u8*)self + 0x37) = 0;
        *((u8*)self + 0x38) = 1;
        *((u8*)self + 0x31) = 0;
        *((u8*)self + 0x36) = 0;
        *((u8*)self + 0x30) = 0;
    }
}

// Frame-update handler: when animTrans1 finishes, switch to state 2, enable
// both animations, show the window and reset the two labelled panes.
extern "C" __declspec(noinline) void func_8022E5B0(CPresentWin* self) {
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

// Retail uses the stmw/lmw block-save frame for these two (optimize_for_size
// shape; see KB kyoshin/CCur func_801D2E4C).
#pragma push
#pragma optimize_for_size on
// Repaint helper for the mField32 (primary) item: look up the item name and
// message id from the BDAT table, bind the name to pane20's text box and the
// message texture to mPane28.
extern "C" __declspec(noinline) void func_8022E698(CPresentWin* self) {
    u8 idx = func_8022E868(self, self->mField32);
    char* name = func_8013639C(lbl_eu_80664090, &lbl_eu_8050A84C[0x1cb], idx);
    func_80136D74((nw4r::lyt::Layout*)self->mPane20, name, 0);
    u32 msgId = func_80136254(lbl_eu_80664090, &lbl_eu_8050A84C[0x1d0], idx) & 0xFFFF;
    void* tex = self->mAccessor->GetResource(0x74696D67, func_80138F78(msgId), NULL);
    if (tex != NULL) {
        func_80137F88(self->mPane28, tex);
    }
}

// Same repaint for the mField33 (secondary) item, targeting mPane24/mPane2C.
extern "C" __declspec(noinline) void func_8022E744(CPresentWin* self) {
    u8 idx = func_8022E868(self, self->mField33);
    char* name = func_8013639C(lbl_eu_80664090, &lbl_eu_8050A84C[0x1cb], idx);
    func_80136D74((nw4r::lyt::Layout*)self->mPane24, name, 0);
    u32 msgId = func_80136254(lbl_eu_80664090, &lbl_eu_8050A84C[0x1d0], idx) & 0xFFFF;
    void* tex = self->mAccessor->GetResource(0x74696D67, func_80138F78(msgId), NULL);
    if (tex != NULL) {
        func_80137F88(self->mPane2C, tex);
    }
}

// Retail uses the stmw/lmw block-save frame (MWCC optimize_for_size shape).
// Retail calls this from func_8022DB7C rather than inlining it.
extern "C" __declspec(noinline) void func_8022E7F0(CPresentWin* self) {
    self->mDataCount = 0;
    u8 count = code80135FDC_getByte_64077();
    for (u8 i = 0; i < count; i++) {
        u8 val = func_801392B4(i);
        if (val <= 8) {
            u8 idx = self->mDataCount;
            self->mDataArray[idx] = val;
            self->mDataCount = idx + 1;
        }
    }
}
#pragma pop


// Unload: wait for the VI, drop the active flag, destroy the layout through
// its vtable slot-2 virtual (guarded by the retail double null check) and
// clear the layout + accessor pointers.
extern "C" __declspec(noinline) void func_8022DB04(CPresentWin* self) {
    waitForDrawDone__9CDeviceVIFv();
    self->mField30 = 0;
    if (self->mpLayout != NULL) {
        nw4r::lyt::Layout* p = self->mpLayout;
        if (p != NULL) {
            ((CPresentLayoutSlot2View*)p)->mSlot2(1);
        }
        self->mpLayout = NULL;
    }
    self->mAccessor = NULL;
}

// Step the selected labelled pane counter backwards, wrapping through
// mDataCount; when the two counters collide after a step, step again. Then
// tail-call the repaint helper for the active path.
extern "C" __declspec(noinline) void func_8022E254(CPresentWin* self) {
    if (self->mField36 != 0) return;
    if (self->mField31 != 0) {
        self->mField33 = (u8)(self->mField33 - 1);
        if ((s8)self->mField33 < 0) self->mField33 = self->mDataCount - 1;
        if ((s8)self->mField33 == (s8)self->mField32) {
            self->mField33 = (u8)(self->mField33 - 1);
            if ((s8)self->mField33 < 0) self->mField33 = self->mDataCount - 1;
        }
        func_8022E744(self);
    } else {
        self->mField32 = (u8)(self->mField32 - 1);
        if ((s8)self->mField32 < 0) self->mField32 = self->mDataCount - 1;
        func_8022E698(self);
    }
}

extern "C" __declspec(noinline) u8 func_8022E868(CPresentWin* self, u32 r4) {
    u8 limit = self->mDataCount;
    if (r4 >= limit) return 0;
    return self->mDataArray[r4];
}

#pragma push
#pragma optimize_for_size on
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
        // Texture dimensions are read before the pane lookup so the loads
        // schedule above the call (retail interleaves them).
        CPresentTexDims* dims = ((CPresentTexObj*)tex2)->mChain->mDims;
        u16 h = dims->mH;
        u16 w = dims->mW;
        nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->FindPaneByName(&lbl_eu_8050A84C[0x10d], 1);
        if (pane) {
            float src[2];
            // u32->float via the 2^52 magic: build 0x43300000_<v> on the stack
            // and subtract the NAMED .sdata2 double so the pool reloc stays on
            // lbl_eu_80668620 (a plain (f32) cast synthesizes a TU-local @N).
            union {
                struct { u32 hi; u32 lo; } w;
                double d;
            } conv[2];
            conv[0].w.lo = h;
            conv[0].w.hi = 0x43300000u;
            conv[1].w.lo = w;
            conv[1].w.hi = 0x43300000u;
            src[0] = (f32)(conv[0].d - lbl_eu_80668620);
            src[1] = (f32)(conv[1].d - lbl_eu_80668620);
            func_80124288(pane, src);
        }
    }
    func_80138078(0xd);
}
#pragma pop

#pragma push
#pragma optimize_for_size on
// Init: load the layout + animation transforms, bind the font, enable the
// default animation and look up the 7 pane handles used by the window.
void func_8022D614(CPresentWin* self, nw4r::lyt::ArcResourceAccessor* accessor) {
    // Shared u32->float scratch (2^52 magic): hi words are written up front
    // so the stores schedule into the prologue, exactly as retail.
    union {
        struct { u32 hi; u32 lo; } w[2];
        double d[2];
    } conv;
    conv.w[0].hi = 0x43300000u;
    conv.w[1].hi = 0x43300000u;

    self->mAccessor = accessor;

    // String-pool base kept in a callee-saved register by retail.
    char* pool = lbl_eu_8050A84C;

    func_80136E84(&self->mpLayout, accessor, pool);
    func_80136F08(self->mpLayout, &self->mpAnimTrans0, self->mAccessor, pool + 0x19);
    func_80136F08(self->mpLayout, &self->mpAnimTrans1, self->mAccessor, pool + 0x35);

    void* fontObj = func_80452C10__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->mpLayout);
    u32 fontResult = ((CPresentFontView*)fontObj)->sf9();
    // No persistent rootPane local: retail reloads GetRootPane() per use,
    // keeping callee-saved pressure down to r29-r31.
    func_8013676C(self->mpLayout->GetRootPane(), fontResult);

    self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, true);
    self->mpLayout->Animate(0);

    self->mPane14 = self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x51, 1);
    self->mPane18 = self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x60, 1);
    self->mPane1C = self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x6d, 1);
    self->mPane20 = self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x7a, 1);
    self->mPane24 = self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x87, 1);
    self->mPane28 = self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x94, 1);
    self->mPane2C = self->mpLayout->GetRootPane()->FindPaneByName(pool + 0xa1, 1);

    func_80124270(self->mpLayout->GetRootPane()->FindPaneByName(pool + 0xae, 1), 0);
    func_80136B4C(self->mpLayout, pool + 0xb7, pool + 0xc4, 0);
    func_80136D74((nw4r::lyt::Layout*)self->mPane20, pool + 0xc4, 0);
    func_80136D74((nw4r::lyt::Layout*)self->mPane24, pool + 0xc4, 0);

    char* s1 = func_80136190(pool + 0xc5, pool + 0xd3, 0x2b);
    func_80136B4C(self->mpLayout, pool + 0xd8, s1, 0);

    const char* msgName1 = func_80086F9C__Q22cf13CfGameManagerFv(-1) ? pool + 0xe4 : pool + 0xed;
    char* handle1 = func_80138F78(func_8013606C(pool + 0xc5, msgName1, 0x2b));
    nw4r::lyt::ArcResourceAccessor* mgr1 = func_801355F4();
    void* tex1 = mgr1->GetResource(0x74696D67, handle1, NULL);
    if (tex1) {
        func_80137E7C(self->mpLayout, pool + 0x10d, tex1);
        // Texture dimensions are read before the pane lookup so the loads
        // schedule above the virtual call (retail interleaves them).
        CPresentTexDims* dims1 = ((CPresentTexObj*)tex1)->mChain->mDims;
        u16 h1 = dims1->mH;
        u16 w1 = dims1->mW;
        nw4r::lyt::Pane* pane1 = self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x10d, 1);
        if (pane1) {
            float src1[2];
            conv.w[0].lo = h1;
            conv.w[1].lo = w1;
            src1[0] = (f32)(conv.d[0] - lbl_eu_80668620);
            src1[1] = (f32)(conv.d[1] - lbl_eu_80668620);
            func_80124288(pane1, src1);
        }
    }

    char* s2 = func_80136190(pool + 0xc5, pool + 0xd3, 0x9b);
    func_80136B4C(self->mpLayout, pool + 0x100, s2, 0);

    const char* msgName2 = func_80086F9C__Q22cf13CfGameManagerFv(-1) ? pool + 0xe4 : pool + 0xed;
    char* handle2 = func_80138F78(func_8013606C(pool + 0xc5, msgName2, 0x9b));
    nw4r::lyt::ArcResourceAccessor* mgr2 = func_801355F4();
    void* tex2 = mgr2->GetResource(0x74696D67, handle2, NULL);
    if (tex2) {
        func_80137E7C(self->mpLayout, pool + 0x10d, tex2);
        CPresentTexDims* dims2 = ((CPresentTexObj*)tex2)->mChain->mDims;
        u16 h2 = dims2->mH;
        u16 w2 = dims2->mW;
        nw4r::lyt::Pane* pane2 = self->mpLayout->GetRootPane()->FindPaneByName(pool + 0x10d, 1);
        if (pane2) {
            float src2[2];
            conv.w[0].lo = h2;
            conv.w[1].lo = w2;
            src2[0] = (f32)(conv.d[0] - lbl_eu_80668620);
            src2[1] = (f32)(conv.d[1] - lbl_eu_80668620);
            func_80124288(pane2, src2);
        }
    }
}
#pragma pop

extern "C" void func_8022DA58(CPresentWin* self) {
    if (self->mField30 == 0) {
        return;
    }
    switch (self->mField37) {
    case 1:
        func_8022E50C(self);
        break;
    case 3:
        func_8022E558(self);
        break;
    case 4:
        func_8022E5B0(self);
        break;
    }
    self->mpLayout->Animate(0);
}
