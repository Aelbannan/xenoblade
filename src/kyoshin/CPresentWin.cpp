#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/CPresentWin
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/cf/CfBdat.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CPresentWin.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <stdio.h>
#include "kyoshin/code_80135FDC_State.hpp"

extern "C" {
void stepPresentOpenAnim(CPresentWin* self);
void stepPresentCloseAnim(CPresentWin* self);
void stepPresentResultAnim(CPresentWin* self);
void collectPresentData(CPresentWin* self);
}


extern const f32 CPresentAnimStepDelta;

// Constructor: zero every field, except mField38 which starts at 1.
CPresentWin::CPresentWin() {
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

// Destructor - base object destructor (D2); MWCC emits D1 with delete check
CPresentWin::~CPresentWin() {
}

// Init: load the layout + animation transforms, bind the font, enable the
// default animation and look up the 7 pane handles used by the window.
void initPresentWin(CPresentWin* self, nw4r::lyt::ArcResourceAccessor* accessor) {
    // Shared u32->float scratch (2^52 magic): hi words are written up front
    // so the stores schedule into the prologue, exactly as retail.
    union {
        struct { u32 hi; u32 lo; } w[2];
        double d[2];
    } conv;
    conv.w[0].hi = 0x43300000u;
    conv.w[1].hi = 0x43300000u;

    self->mAccessor = accessor;


    buildLayout(&self->mpLayout, accessor, "mf70_cf60_syswin07.brlyt");
    bindLayoutAnimTransform(self->mpLayout, &self->mpAnimTrans0, self->mAccessor, "mf70_cf60_syswin07_in.brlan");
    bindLayoutAnimTransform(self->mpLayout, &self->mpAnimTrans1, self->mAccessor, "mf70_cf60_syswin07_ef.brlan");

    void* fontObj = getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->mpLayout);
    u32 fontResult = (u32)static_cast<IDeviceFontInfo*>(fontObj)->getFont();
    // No persistent rootPane local: retail reloads GetRootPane() per use,
    // keeping callee-saved pressure down to r29-r31.
    ApplyFontToTxtPanes(self->mpLayout->GetRootPane(), (const nw4r::ut::Font*)fontResult);

    self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, false);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, true);
    self->mpLayout->Animate(0);

    self->mPane14 = self->mpLayout->GetRootPane()->FindPaneByName("nul_proportion", 1);
    self->mPane18 = self->mpLayout->GetRootPane()->FindPaneByName("nul_pcface01", 1);
    self->mPane1C = self->mpLayout->GetRootPane()->FindPaneByName("nul_pcface02", 1);
    self->mPane20 = self->mpLayout->GetRootPane()->FindPaneByName("txt_pcname01", 1);
    self->mPane24 = self->mpLayout->GetRootPane()->FindPaneByName("txt_pcname02", 1);
    self->mPane28 = self->mpLayout->GetRootPane()->FindPaneByName("pic_pcface01", 1);
    self->mPane2C = self->mpLayout->GetRootPane()->FindPaneByName("pic_pcface02", 1);

    setPaneVisible(self->mpLayout->GetRootPane()->FindPaneByName("nul_slct", 1), 0);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_sysmes00", "", 0);
    SetLayoutSelfText((nw4r::lyt::Layout*)self->mPane20, "", 0);
    SetLayoutSelfText((nw4r::lyt::Layout*)self->mPane24, "", 0);

    char* s1 = BdatTouchStringCell("MNU_kyeassign", "help", 0x2b);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_close00", s1, 0);

    const char* msgName1 = cf::CfGameManager::isClassicController(-1) ? "fileID_2" : "fileID_1";
    char* handle1 = MakeTplNameSysFile(BdatGetU16ByTableKey("MNU_kyeassign", msgName1, 0x2b));
    nw4r::lyt::ArcResourceAccessor* mgr1 = CUICfManager_getArcResourceAccessor();
    void* tex1 = mgr1->GetResource(0x74696D67, handle1, NULL);
    if (tex1) {
        PaneSetTexPaletteByName(self->mpLayout, "pic_btn00", (TPLPalette*)tex1);
        // Texture dimensions are read before the pane lookup so the loads
        // schedule above the virtual call (retail interleaves them).
        CPresentTexDims* dims1 = ((CPresentTexObj*)tex1)->mChain->mDims;
        u16 h1 = dims1->mH;
        u16 w1 = dims1->mW;
        nw4r::lyt::Pane* pane1 = self->mpLayout->GetRootPane()->FindPaneByName("pic_btn00", 1);
        if (pane1) {
            float src1[2];
            conv.w[0].lo = h1;
            conv.w[1].lo = w1;
            src1[0] = (f32)(conv.d[0] - CPresentU32ToFloatMagic);
            src1[1] = (f32)(conv.d[1] - CPresentU32ToFloatMagic);
            writePanePos(pane1, src1);
        }
    }

    char* s2 = BdatTouchStringCell("MNU_kyeassign", "help", 0x9b);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_change00", s2, 0);

    const char* msgName2 = cf::CfGameManager::isClassicController(-1) ? "fileID_2" : "fileID_1";
    char* handle2 = MakeTplNameSysFile(BdatGetU16ByTableKey("MNU_kyeassign", msgName2, 0x9b));
    nw4r::lyt::ArcResourceAccessor* mgr2 = CUICfManager_getArcResourceAccessor();
    void* tex2 = mgr2->GetResource(0x74696D67, handle2, NULL);
    if (tex2) {
        PaneSetTexPaletteByName(self->mpLayout, "pic_btn02", (TPLPalette*)tex2);
        CPresentTexDims* dims2 = ((CPresentTexObj*)tex2)->mChain->mDims;
        u16 h2 = dims2->mH;
        u16 w2 = dims2->mW;
        nw4r::lyt::Pane* pane2 = self->mpLayout->GetRootPane()->FindPaneByName("pic_btn02", 1);
        if (pane2) {
            float src2[2];
            conv.w[0].lo = h2;
            conv.w[1].lo = w2;
            src2[0] = (f32)(conv.d[0] - CPresentU32ToFloatMagic);
            src2[1] = (f32)(conv.d[1] - CPresentU32ToFloatMagic);
            writePanePos(pane2, src2);
        }
    }
}

extern "C" void updatePresentWin(CPresentWin* self) {
    if (self->mField30 == 0) {
        return;
    }
    switch (self->mField37) {
    case 1:
        stepPresentOpenAnim(self);
        break;
    case 3:
        stepPresentCloseAnim(self);
        break;
    case 4:
        stepPresentResultAnim(self);
        break;
    }
    self->mpLayout->Animate(0);
}

// Draw the present window layout if active and in the right state
void drawPresentWin(CPresentWin* self, nw4r::lyt::DrawInfo* drawInfo) {
    if (self->mField30 == 0) return;
    if (self->mField37 == 0) return;
    drawLayout(self->mpLayout, drawInfo, 0, 1);
}

// Unload: wait for the VI, drop the active flag, destroy the layout through
// its vtable slot-2 virtual (guarded by the retail double null check) and
// clear the layout + accessor pointers.
extern "C" void termPresentWin(CPresentWin* self) {
    waitForDrawDone__9CDeviceVIFv();
    self->mField30 = 0;
    if (self->mpLayout != NULL) {
        nw4r::lyt::Layout* p = self->mpLayout;
        delete p;
        self->mpLayout = NULL;
    }
    self->mAccessor = NULL;
}

u32 isPresentActive(CPresentWin* self) { return self->mField30; }

u32 isPresentSettled(CPresentWin* self) { return self->mField38; }

// Open handler: mark the window active, rebuild the item data, repaint the
// labels, load the "timg" texture for mPane2C and the message texture, then
// position the message pane at the texture's dimensions.
void openPresentWin(CPresentWin* self) {
    if (self->mField37 != 0) return;
    self->mField37 = 1;
    self->mField38 = 0;
    self->mField30 = 1;
    collectPresentData(self);
    repaintPresentItemA(self);

    SetLayoutSelfText((nw4r::lyt::Layout*)self->mPane24, "", 0);
    void* tex = self->mAccessor->GetResource(0x74696D67, "mf00_com00_dmy.tpl", NULL);
    if (tex) {
        PaneSetTexPalette(self->mPane2C, (TPLPalette*)tex);
    }

    char* s1 = BdatTouchStringCell("MNU_kyeassign", "help", 0x2b);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_close00", s1, 0);
    char* s2 = BdatTouchStringCell("MNU_kyeassign", "help", 0x9b);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_change00", s2, 0);

    const char* msgName = cf::CfGameManager::isClassicController(-1)
                              ? "fileID_2"
                              : "fileID_1";
    char* handle = MakeTplNameSysFile(BdatGetU16ByTableKey("MNU_kyeassign", msgName, 0x9b));
    nw4r::lyt::ArcResourceAccessor* mgr = CUICfManager_getArcResourceAccessor();
    void* tex2 = mgr->GetResource(0x74696D67, handle, NULL);
    if (tex2) {
        PaneSetTexPaletteByName(self->mpLayout, "pic_btn02", (TPLPalette*)tex2);
        // Texture dimensions are read before the pane lookup so the loads
        // schedule above the call (retail interleaves them).
        CPresentTexDims* dims = ((CPresentTexObj*)tex2)->mChain->mDims;
        u16 h = dims->mH;
        u16 w = dims->mW;
        nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->FindPaneByName("pic_btn02", 1);
        if (pane) {
            float src[2];
            // u32->float via the 2^52 magic: build 0x43300000_<v> on the stack
            // and subtract the NAMED .sdata2 double so the pool reloc stays on
            // CPresentU32ToFloatMagic (a plain (f32) cast synthesizes a TU-local @N).
            union {
                struct { u32 hi; u32 lo; } w;
                double d;
            } conv[2];
            conv[0].w.lo = h;
            conv[0].w.hi = 0x43300000u;
            conv[1].w.lo = w;
            conv[1].w.hi = 0x43300000u;
            src[0] = (f32)(conv[0].d - CPresentU32ToFloatMagic);
            src[1] = (f32)(conv[1].d - CPresentU32ToFloatMagic);
            writePanePos(pane, src);
        }
    }
    playUISound(0xd);
}

// State-machine transition: if state == 2, advance to state 3 and clear sub-state
void advancePresentState(CPresentWin* self) {
    if (self->mField37 != 2) return;
    self->mField37 = 3;
    self->mField38 = 0;
    playUISound(0xe);
}

// Retail uses the -O4,s shape (_savegpr_29 at only 3 saved GPRs).
// Present-window rank display (state 2): look up the player's time for the
// current item, show the appropriate rank/result panes + textures, then hand
// control to the closing animation.
void showPresentRank(CPresentWin* self) {
    if (self->mField37 != 2) return;

    u32 itemId = BdatGetItemId(self->mField34) & 0xFFFF;

    int rank = lookupPresentRank(self, self->mField33);
    if (rank == 8) rank = 3;


    char buf[0x20];
    sprintf(buf, "FSplus%d", rank);

    int res = BdatGetS8Direct((const char*)::cf::CfBdat::spItmCollectListFileData, (const char*)buf, (int)((const char*)itemId));

    if ((s8)res < 0) {
        // Time not met: hide the result panes, show the failure panes and a
        // texture matching how far off the player was.
        setPaneVisible(self->mpLayout->GetRootPane()->FindPaneByName("nul_anim_b", 1), 1);
        setPaneVisible(self->mpLayout->GetRootPane()->FindPaneByName("nul_anim_g", 1), 0);
        setPaneVisible(self->mpLayout->GetRootPane()->FindPaneByName("pic_kiz_b", 1), 1);

        void* tex;
        if ((s8)res <= -0x32) {
            tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf00_com00_kizp04.tpl", NULL);
        } else if ((s8)res <= -0x10) {
            tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf00_com00_kizp02.tpl", NULL);
        } else {
            tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf00_com00_kizp01.tpl", NULL);
        }
        if (tex) {
            PaneSetTexPaletteByName(self->mpLayout, "pic_kiz_b", (TPLPalette*)tex);
        }
    } else {
        // Time met: show the result panes and a texture matching the rank.
        setPaneVisible(self->mpLayout->GetRootPane()->FindPaneByName("nul_anim_b", 1), 0);
        setPaneVisible(self->mpLayout->GetRootPane()->FindPaneByName("nul_anim_g", 1), 1);
        setPaneVisible(self->mpLayout->GetRootPane()->FindPaneByName("pic_kiz_g", 1), 1);

        void* tex;
        if ((s8)res >= 0x12c) {
            tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf00_com00_kizp00.tpl", NULL);
        } else if ((s8)res >= 0x32) {
            tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf00_com00_kizp04.tpl", NULL);
        } else if ((s8)res >= 0x15) {
            tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf00_com00_kizp03.tpl", NULL);
        } else if ((s8)res >= 0x10) {
            tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf00_com00_kizp02.tpl", NULL);
        } else {
            tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, "mf00_com00_kizp01.tpl", NULL);
        }
        if (tex) {
            PaneSetTexPaletteByName(self->mpLayout, "pic_kiz_g", (TPLPalette*)tex);
        }
    }

    // Rank sound + collection bookkeeping (skipped entirely for a 0 result).
    if ((s8)res >= 1) {
        playUISound(0x34);
    } else if ((s8)res == 0) {
        goto tail;
    } else {
        playUISound(0x36);
    }
    {
        u8 idxA = lookupPresentRank(self, self->mField32);
        u8 idxB = lookupPresentRank(self, self->mField33);
        AdjustRelateValue(idxA, idxB, (s8)res);
    }

tail:
    // Rebuild the item labels and message texture, then start the close
    // animation.
    char* s1 = BdatTouchStringCell("MNU_kyeassign", "help", 0x2c);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_close00", s1, 0);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_change00", "", 0);

    void* tex = self->mAccessor->GetResource(0x74696D67, "mf00_com00_dmy.tpl", NULL);
    if (tex) {
        PaneSetTexPaletteByName(self->mpLayout, "pic_btn02", (TPLPalette*)tex);
    }

    self->mField37 = 4;
    self->mField38 = 0;
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, false);
    self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, true);
    self->mpAnimTrans1->SetFrame(CPresentFrameResetValue);
}

// Store the anim index (+0x34), call the anim loader, then bind the loaded
// frame ("txt_sysmes00" text box) through the +8 layout.
extern "C" void setPresentAnimText(CPresentWin* self, u16 idx) {
    self->mField34 = idx;
    void* anim = BuildItemNameStr(idx);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_sysmes00", (char*)anim, 0);
}

// Step the selected labelled pane counter backwards, wrapping through
// mDataCount; when the two counters collide after a step, step again. Then
// tail-call the repaint helper for the active path.
extern "C" void stepPresentCounterBack(CPresentWin* self) {
    if (self->mField36 != 0) return;
    if (self->mField31 != 0) {
        self->mField33 = (u8)(self->mField33 - 1);
        if ((s8)self->mField33 < 0) self->mField33 = self->mDataCount - 1;
        if ((s8)self->mField33 == (s8)self->mField32) {
            self->mField33 = (u8)(self->mField33 - 1);
            if ((s8)self->mField33 < 0) self->mField33 = self->mDataCount - 1;
        }
        repaintPresentItemB(self);
    } else {
        self->mField32 = (u8)(self->mField32 - 1);
        if ((s8)self->mField32 < 0) self->mField32 = self->mDataCount - 1;
        repaintPresentItemA(self);
    }
}

// Advance the present-window frame counter. mField31 selects which counter
// advances; mField32/mField33 wrap around at mDataCount. If the two counters
// collide after an advance, the secondary one steps again.
void stepPresentCounter(CPresentWin* self) {
    if (self->mField36 != 0) return;
    if (self->mField31 != 0) {
        self->mField33 = (u8)(self->mField33 + 1);
        if ((s8)self->mField33 >= self->mDataCount) self->mField33 = 0;
        if ((s8)self->mField33 == (s8)self->mField32) {
            self->mField33 = (u8)(self->mField33 + 1);
            if ((s8)self->mField33 >= self->mDataCount) self->mField33 = 0;
        }
        repaintPresentItemB(self);
    } else {
        self->mField32 = (u8)(self->mField32 + 1);
        if ((s8)self->mField32 >= self->mDataCount) self->mField32 = 0;
        repaintPresentItemA(self);
    }
}

// retail: tail-branch to stepPresentCounterBack (passes `this` through in r3)
extern "C" void dispatchCounterStepBack(CPresentWin* self) { stepPresentCounterBack(self); }

// Toggle mField31 (which animation path is active). When toggling ON, skip the
// frame if the secondary counter would collide; when OFF, repaint the label
// and (re)load the texture bound to mPane2C.
void togglePresentPath(CPresentWin* self) {
    self->mField31 = (u8)((self->mField31 ^ 1) != 0);
    if (self->mField31) {
        if ((s8)self->mField33 == (s8)self->mField32) {
            self->mField33 = (u8)(self->mField32 + 1);
            if ((s8)self->mField33 >= self->mDataCount) self->mField33 = 0;
        }
        repaintPresentItemB(self);
    } else {
        SetLayoutSelfText((nw4r::lyt::Layout*)self->mPane24, "", 0);
        void* tex = self->mAccessor->GetResource(0x74696D67, "mf00_com00_dmy.tpl", NULL);
        if (tex) {
            PaneSetTexPalette(self->mPane2C, (TPLPalette*)tex);
        }
    }
}

u32 getPresentPath(CPresentWin* self) { return self->mField31; }

u32 isPresentBusy(CPresentWin* self) { return self->mField36; }

// Get the world position of the selected pane of another present window and
// scale its x by that window's label pane (+0x14) horizontal scale factor.
void getPresentPaneWorldPos(nw4r::math::VEC3* out, CPresentWin* src) {
    AccumPaneWorldPos(out, src->mField31 != 0 ? src->mPane1C : src->mPane18);
    out->x = out->x * src->mPane14->GetScale().x;
}

// retail: lbz r4,0x32(r3); b lookupPresentRank
extern "C" u8 getPresentRankA(CPresentWin* self) { return lookupPresentRank(self, self->mField32); }

// retail: lbz r4,0x33(r3); b lookupPresentRank
extern "C" u8 getPresentRankB(CPresentWin* self) { return lookupPresentRank(self, self->mField33); }

extern "C" void stepPresentOpenAnim(CPresentWin* self) {
    if (advanceAnimTransform(self->mpAnimTrans0, CPresentAnimStepDelta) != 0) {
        self->mField37 = 2;
        self->mField38 = 1;
    }
}

extern "C" void stepPresentCloseAnim(CPresentWin* self) {
    if (AnimRewindFrame(self->mpAnimTrans0, CPresentAnimStepDelta)) {
        self->mField37 = 0;
        self->mField38 = 1;
        self->mField31 = 0;
        self->mField36 = 0;
        self->mField30 = 0;
    }
}

// Frame-update handler: when animTrans1 finishes, switch to state 2, enable
// both animations, show the window and reset the two labelled panes.
extern "C" void stepPresentResultAnim(CPresentWin* self) {
    if (advanceAnimTransform(self->mpAnimTrans1, CPresentAnimStepDelta) != 0) {
        self->mField37 = 2;
        self->mField38 = 1;
        self->mpLayout->SetAnimationEnable(self->mpAnimTrans1, false);
        self->mpLayout->SetAnimationEnable(self->mpAnimTrans0, true);
        self->mField36 = 1;
        setPaneVisible(self->mpLayout->GetRootPane()->FindPaneByName("nul_anim_b", 1), 0);
        setPaneVisible(self->mpLayout->GetRootPane()->FindPaneByName("nul_anim_g", 1), 0);
    }
}

// Retail uses the stmw/lmw block-save frame for these two (-O4,s unit).
// Repaint helper for the mField32 (primary) item: look up the item name and
// message id from the BDAT table, bind the name to pane20's text box and the
// message texture to mPane28.
extern "C" void repaintPresentItemA(CPresentWin* self) {
    u8 idx = lookupPresentRank(self, self->mField32);
    char* name = BdatGetPtrDirect(::cf::CfBdat::spBtlPcListFileData, "name", idx);
    SetLayoutSelfText((nw4r::lyt::Layout*)self->mPane20, name, 0);
    u32 msgId = BdatGetU16Direct(::cf::CfBdat::spBtlPcListFileData, "icon_type2", idx) & 0xFFFF;
    void* tex = self->mAccessor->GetResource(0x74696D67, MakeTplNameSysFile(msgId), NULL);
    if (tex != NULL) {
        PaneSetTexPalette(self->mPane28, (TPLPalette*)tex);
    }
}

// Same repaint for the mField33 (secondary) item, targeting mPane24/mPane2C.
extern "C" void repaintPresentItemB(CPresentWin* self) {
    u8 idx = lookupPresentRank(self, self->mField33);
    char* name = BdatGetPtrDirect(::cf::CfBdat::spBtlPcListFileData, "name", idx);
    SetLayoutSelfText((nw4r::lyt::Layout*)self->mPane24, name, 0);
    u32 msgId = BdatGetU16Direct(::cf::CfBdat::spBtlPcListFileData, "icon_type2", idx) & 0xFFFF;
    void* tex = self->mAccessor->GetResource(0x74696D67, MakeTplNameSysFile(msgId), NULL);
    if (tex != NULL) {
        PaneSetTexPalette(self->mPane2C, (TPLPalette*)tex);
    }
}

// Retail uses the stmw/lmw block-save frame (-O4,s unit).
// Retail calls this from openPresentWin rather than inlining it.
extern "C" void collectPresentData(CPresentWin* self) {
    self->mDataCount = 0;
    u8 count = code80135FDC_getByte_64077();
    for (u8 i = 0; i < count; i++) {
        u8 val = GetCollectedFlagByte(i);
        if (val <= 8) {
            u8 idx = self->mDataCount;
            self->mDataArray[idx] = val;
            self->mDataCount = idx + 1;
        }
    }
}

extern "C" u8 lookupPresentRank(CPresentWin* self, u32 index) {
    u8 limit = self->mDataCount;
    if (index >= limit) return 0;
    return self->mDataArray[index];
}
