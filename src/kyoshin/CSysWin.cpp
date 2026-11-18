#define IWORK_EVENT_INLINE_DTOR
// CSysWin - system message window widget (kyoshin/CSysWin unit).

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/CSysWin.hpp"
#include "kyoshin/CTagProcessor.hpp"
#include "kyoshin/CQuestWindow_Pane.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

#include "kyoshin/code_80135FDC.hpp"

#include "monolib/device.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/IWorkEvent.hpp"

#include <new>
#include "kyoshin/code_80135FDC_State.hpp"

// ---------------------------------------------------------------------------
// Unit-local external declarations (retail symbol names kept verbatim).
// ---------------------------------------------------------------------------

// C-linkage helpers defined in code_80135FDC.cpp (retail keeps them unmangled).
extern "C" void writePanePos(nw4r::lyt::Pane*, float*);
// Copies two floats (VEC2); leaves its destination pointer in r3.
extern "C" void TagCopyVec2f(float* dst, float* src);

// Member ctor/dtor of the memory-region helper: the retail ctor/dtor calls
// are out of line (the class has no inline definition in this tree).
extern "C" void __dt__17UnkClass_8045F564Fv(UnkClass_8045F564*, int);
extern "C" UnkClass_8045F564* __ct__17UnkClass_8045F564Fv(UnkClass_8045F564* self);

// CTagProcessor lives in its own TU (kyoshin/CTagProcessor.cpp); its ctor is
// referenced by its literal retail symbol.
class CTagProcessor;
extern "C" void* __ct__CTagProcessor(void*);

// cf::CfGameManager helper. Retail's symbol carries the no-arg Fv suffix but
// the call site passes an argument (the active window kind), so declare the
// retail symbol verbatim under C linkage.
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);

// Five-entry window-label index table (bdat row key per window kind). Retail
// keeps it as a function-local static and copies it by value, reading a word
// plus a trailing byte from .sdata2.
struct SysWinLabelKeys {
    u8 key[5];
};

// View into the object returned by CDeviceFont::getFontInfo: its vtable+0x24
// slot yields the font handle pushed into the layout's text panes. All-pure, so
// no vtable is emitted.
class CSysWinFontView {
public:
    virtual void v00() = 0;  // +0x08
    virtual void v01() = 0;  // +0x0C
    virtual void v02() = 0;  // +0x10
    virtual void v03() = 0;  // +0x14
    virtual void v04() = 0;  // +0x18
    virtual void v05() = 0;  // +0x1C
    virtual void v06() = 0;  // +0x20
    virtual u32 v07() = 0;   // +0x24 - font handle
};

// ---------------------------------------------------------------------------

CSysWin::CSysWin(u8 kind)
    : mFileHandle(nullptr),
      mTagProcessor(nullptr),
      mArcAccessor(nullptr),
      mLayout(nullptr),
      mAnimTrans(nullptr),
      field_28(1),
      field_2C(nullptr),
      field_30(nullptr),
      field_34(0),
      field_35(0),
      field_36(1),
      field_37(kind),
      field_38(5),
      field_39(0) {}

CSysWin::~CSysWin() {}

int CSysWin::isReady() { return field_28; }

int CSysWin::isOpened() { return field_34; }

int CSysWin::isActive() { return field_36; }

// us-8022d52c - load the System.arc file (IWorkEvent callback = this)
// into the window's file-handle slot, mark the handle, and clear the ready flag.
void CSysWin::loadSystemArc() {
    u32 allocHandle = mtl::MemManager::getHandleMEM2();
    mFileHandle = CDeviceFile::readFile(allocHandle, "menu/jp/SysWin.arc", this, 0, 0);
    CDeviceFile::setHandleFlag1(mFileHandle);
    field_28 = 0;
}

// us-8022d580 - per-frame update: run the phase handler for the
// current animation phase, then advance the layout animation (open-flag gate).
// Goto-chain dispatch (MWCC_CASES §7d2): switch/if-else shapes don't reproduce
// retail's linear equality chain with bodies appended after the tests; the int
// temp keeps the compares signed (cmpi) without an extsb.
void CSysWin::dispatchPhase() {
    if (field_34 != 0) {
        int phase = field_35;
        if (phase == 1) goto case1;
        if (phase == 2) goto case2;
        if (phase == 3) goto case3;
        goto tail;
    case1:
        phase1To2Advance();
        goto tail;
    case2:
        syncKindAdvance();
        goto tail;
    case3:
        rewindToPhase0();
    tail:
        mLayout->Animate(0);
    }
}

// us-8022d600 - draw the system-window layout when it is open and
// animated (field_34/field_35 guards), passing through the caller's DrawInfo.
void CSysWin::drawLayout(nw4r::lyt::DrawInfo* drawInfo) {
    if (field_34 == 0) return;
    if (field_35 == 0) return;
    return ::drawLayout(mLayout, drawInfo, 0, 1);
}

// us-8022d62c - tear down the window layout and free its resources.
// The `if (x != 0) { delete x; x = 0; }` shape reproduces retail's double-beq
// prologue check (same as CBattery::releaseLayout).
void CSysWin::termLayout() {
    closeFileHandle(&mFileHandle);
    nw4r::lyt::Layout* layout = mLayout;
    field_34 = 0;
    mAnimTrans = 0;
    if (layout != 0) {
        delete layout;
        mLayout = 0;
    }
    releaseArcResourceAccessor(mArcAccessor);
    mArcAccessor = 0;
    if (mTagProcessor != 0) {
        delete mTagProcessor;
        mTagProcessor = 0;
    }
    mMemRegion.deleteRegion();
}

// us-8022d6f0 - open the window (only when idle): mark phase 1 and
// play the opening UI sound.
void CSysWin::openPhase1() {
    if (field_35 != 0) return;
    field_35 = 1;
    field_36 = 0;
    field_34 = 1;
    return playUISound(0xd);
}

// us-8022d71c - advance the close animation: mark phase 3, clear the
// active flag and play the closing UI sound.
// retail: if (field_35 == 2) { field_35 = 3; field_36 = 0; tail playUISound(0xE) }
void CSysWin::advancePhase3() {
    if (field_35 == 2) {
        field_35 = 3;
        field_36 = 0;
        playUISound(0xE);
    }
}

// us-8022d744 - switch the window's visible pane for the given kind:
// hide both panes first, then re-show the one selected by the 4-way switch
// (each case keeps its own call site in retail - no fall-through merging).
void CSysWin::switchKindPane(int kind) {
    setPaneVisible(field_2C, 0);
    setPaneVisible(field_30, 0);
    field_37 = (u8)kind;
    switch (kind) {
    case 0:
        setPaneVisible(field_2C, 1);
        break;
    case 1:
        setPaneVisible(field_2C, 1);
        break;
    case 2:
        setPaneVisible(field_30, 1);
        break;
    case 3:
        setPaneVisible(field_30, 1);
        break;
    }
}

// us-8022d7ec - set the system-window content for one of the four
// window kinds. Finds the shared label pane once, then per kind: writes the
// label texts (cases 0/2/3 use the incoming strings), binds the tag processor
// into the label pane(s), sizes the icon pane from the label pane's height
// (threshold ladder), and repositions the icon pane.
void CSysWin::setKindContent(char* textA, char* textB) {
    nw4r::lyt::Pane* pane0 =
        mLayout->GetRootPane()->FindPaneByName("win_main", true);
    // Goto-chain dispatch (cf. dispatchPhase): retail's linear equality chain
    // with case bodies appended after the tests; field_37 loads as an int so the
    // compares stay plain cmpwi without an extsb.
    int kind = field_37;
    if (kind == 0) goto case0;
    if (kind == 1) goto case1;
    if (kind == 2) goto case2;
    if (kind == 3) goto case3;
    goto end;
case0:
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes01", textA,
                             reinterpret_cast<u32>(mTagProcessor));
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes02_00", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes02_01", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes03", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes04_00", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes04_01", "", 0);
    {
        nw4r::lyt::Pane* pane =
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes01", true);
        // TagProcCalcPageLayout's retail signature takes opaque view pointers
        TagProcCalcPageLayout(mTagProcessor, reinterpret_cast<CTagMsgView*>(pane),
                              reinterpret_cast<CTagOutView*>(pane0));
        f32 size[2];
        TagCopyVec2f(size, (f32*)&pane0->GetSize());
        f32 h = size[1];
        if (h >= 170.0f) {
            nw4r::math::VEC3 v;
            CBaseCur::SetPaneTranslate(field_2C, code80135FDC_setVec3(&v, 0.0f, -23.0f, 0.0f));
        } else if (h >= 150.0f) {
            nw4r::math::VEC3 v;
            CBaseCur::SetPaneTranslate(field_2C, code80135FDC_setVec3(&v, 0.0f, -14.0f, 0.0f));
        } else if (h >= 130.0f) {
            nw4r::math::VEC3 v;
            CBaseCur::SetPaneTranslate(field_2C, code80135FDC_setVec3(&v, 0.0f, -5.0f, 0.0f));
        } else if (h >= 110.0f) {
            nw4r::math::VEC3 v;
            CBaseCur::SetPaneTranslate(field_2C, code80135FDC_setVec3(&v, 0.0f, 4.0f, 0.0f));
        } else if (h >= 90.0f) {
            nw4r::math::VEC3 v;
            CBaseCur::SetPaneTranslate(field_2C, code80135FDC_setVec3(&v, 0.0f, 13.0f, 0.0f));
        } else {
            nw4r::math::VEC3 v;
            CBaseCur::SetPaneTranslate(field_2C, code80135FDC_setVec3(&v, 0.0f, 22.0f, 0.0f));
        }
    }
    goto end;
case1:
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes02_00", textA,
                             reinterpret_cast<u32>(mTagProcessor));
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes02_01", textB,
                             reinterpret_cast<u32>(mTagProcessor));
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes01", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes03", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes04_00", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes04_01", "", 0);
    {
        nw4r::lyt::Pane* paneA =
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes02_01", true);
        TagProcCalcPageLayout(mTagProcessor, reinterpret_cast<CTagMsgView*>(paneA),
                              reinterpret_cast<CTagOutView*>(pane0));
        f32 posA[2];
        TagCopyVec2f(posA, (f32*)&pane0->GetSize());
        nw4r::lyt::Pane* paneB =
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes02_00", true);
        TagProcCalcPageLayout(mTagProcessor, reinterpret_cast<CTagMsgView*>(paneB),
                              reinterpret_cast<CTagOutView*>(pane0));
        f32 posB[2];
        TagCopyVec2f(posB, (f32*)&pane0->GetSize());
        f32 pick[2];
        TagCopyVec2f(pick, posA[0] > posB[0] ? posA : posB);
        pick[1] = 90.0f;
        writePanePos(pane0, pick);
        nw4r::math::VEC3 v;
        CBaseCur::SetPaneTranslate(field_2C, code80135FDC_setVec3(&v, 0.0f, 13.0f, 0.0f));
    }
    goto end;
case2:
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes03", textA,
                             reinterpret_cast<u32>(mTagProcessor));
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes01", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes02_00", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes02_01", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes04_00", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes04_01", "", 0);
    {
        nw4r::lyt::Pane* pane =
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes03", true);
        // TagProcCalcPageLayout's retail signature takes opaque view pointers
        TagProcCalcPageLayout(mTagProcessor, reinterpret_cast<CTagMsgView*>(pane),
                              reinterpret_cast<CTagOutView*>(pane0));
        f32 size[2];
        TagCopyVec2f(size, (f32*)&pane0->GetSize());
        f32 h = size[1];
        if (h >= 130.0f) {
            nw4r::math::VEC3 v;
            CBaseCur::SetPaneTranslate(field_30, code80135FDC_setVec3(&v, 0.0f, -82.0f, 0.0f));
        } else if (h >= 110.0f) {
            nw4r::math::VEC3 v;
            CBaseCur::SetPaneTranslate(field_30, code80135FDC_setVec3(&v, 0.0f, -70.0f, 0.0f));
        } else if (h >= 90.0f) {
            nw4r::math::VEC3 v;
            CBaseCur::SetPaneTranslate(field_30, code80135FDC_setVec3(&v, 0.0f, -58.0f, 0.0f));
        } else {
            nw4r::math::VEC3 v;
            CBaseCur::SetPaneTranslate(field_30, code80135FDC_setVec3(&v, 0.0f, -46.0f, 0.0f));
        }
    }
    goto end;
case3:
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes04_00", textA,
                             reinterpret_cast<u32>(mTagProcessor));
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes04_01", textB,
                             reinterpret_cast<u32>(mTagProcessor));
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes01", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes02_00", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes02_01", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes03", "", 0);
    {
        nw4r::lyt::Pane* paneA =
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes04_01", true);
        TagProcCalcPageLayout(mTagProcessor, reinterpret_cast<CTagMsgView*>(paneA),
                              reinterpret_cast<CTagOutView*>(pane0));
        f32 posA[2];
        TagCopyVec2f(posA, (f32*)&pane0->GetSize());
        nw4r::lyt::Pane* paneB =
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes04_00", true);
        TagProcCalcPageLayout(mTagProcessor, reinterpret_cast<CTagMsgView*>(paneB),
                              reinterpret_cast<CTagOutView*>(pane0));
        f32 posB[2];
        TagCopyVec2f(posB, (f32*)&pane0->GetSize());
        f32 pick[2];
        TagCopyVec2f(pick, posA[0] > posB[0] ? posA : posB);
        pick[1] = 70.0f;
        writePanePos(pane0, pick);
        nw4r::math::VEC3 v;
        CBaseCur::SetPaneTranslate(field_30, code80135FDC_setVec3(&v, 0.0f, -46.0f, 0.0f));
    }
end:
    ;
}

// us-8022de64 - set the two window label texts from the shared
// string pool (offsets +0x77 / +0x82) into the layout.
void CSysWin::setTwoTextValues(char* textA, char* textB) {
    LayoutSetTextBoxFmtValue(mLayout, "txt_slct01", textA, 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_slct02", textB, 0);
}

// us-8022dec0 - window-kind advance: pick the label/message set for
// the kind, set both window label texts, then bind the icon texture (from the
// arc 'timg' resource) and size the two icon panes to the texture dims. The
// 5-byte label index table is read from .sdata2 (word + trailing byte).
void CSysWin::applyKind(u8 kind) {
    if (kind >= 5) return;
    static const SysWinLabelKeys sLabelKeys = {{0x2b, 0x2c, 0x2d, 0x30, 0x31}};
    SysWinLabelKeys table = sLabelKeys;
    u8 idx = table.key[kind];
    field_38 = kind;
    char* str = BdatTouchStringCell("MNU_kyeassign", "help", idx);
    LayoutSetTextBoxFmtValue(mLayout, "txt_close00", str, 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_close01", str, 0);
    u32 gm = isClassicController__Q22cf13CfGameManagerFv(-1);
    field_39 = (u8)gm;
    u16 keyVal = BdatGetU16ByTableKey(
        "MNU_kyeassign",
        ((u8)gm != 0) ? "fileID_2" : "fileID_1", idx);
    char* texName = MakeTplNameSysFile(keyVal);
    TPLPalette* pal = (TPLPalette*)mArcAccessor->GetResource(
        0x74696d67u, texName, 0);
    if (pal != 0) {
        PaneSetTexPaletteByName(mLayout, "pic_btn00", pal);
        PaneSetTexPaletteByName(mLayout, "pic_btn01", pal);
        TPLHeader* hdr = pal->descriptorArray->textureHeader;
        u16 texW = hdr->width;
        u16 texH = hdr->height;
        nw4r::lyt::Pane* pane1 =
            mLayout->GetRootPane()->FindPaneByName("pic_btn00", true);
        if (pane1 != 0) {
            f32 sz[2];
            sz[0] = (f32)texW;
            sz[1] = (f32)texH;
            writePanePos(pane1, sz);
        }
        nw4r::lyt::Pane* pane2 =
            mLayout->GetRootPane()->FindPaneByName("pic_btn01", true);
        if (pane2 != 0) {
            f32 sz[2];
            sz[0] = (f32)texW;
            sz[1] = (f32)texH;
            writePanePos(pane2, sz);
        }
    }
}

// us-8022e0ac - format a pane name (idx+1), find two panes in the
// second window's layout, and compute the second pane's absolute screen
// position into out (ancestor translate sum via ScalePaneWorldPos). Same shape as
// getShopWinPanePos / clpCalcCurPos: each GetRootPane() is a fresh load (retail
// reloads window->mLayout per use).
void CSysWin::getPaneScreenPos(nw4r::math::VEC3* out, CSysWin* window, u8 idx) {
    char buf[0x28];
    sprintf(buf, "txt_slct%02d", idx + 1);
    nw4r::lyt::Pane* pane1 = window->mLayout->GetRootPane()->FindPaneByName(buf, true);
    nw4r::lyt::Pane* pane2 =
        window->mLayout->GetRootPane()->FindPaneByName("nul_proportion", true);
    ScalePaneWorldPos(out, pane1, pane2, window->mLayout->GetRootPane());
}

// us-8022e150 - advance the window animation; mark it active (phase 2)
// when the anim transform consumed the step.
void CSysWin::phase1To2Advance() {
    if (advanceAnimTransform(mAnimTrans, 1.0f) != 0) {
        field_35 = 2;
        field_36 = 1;
    }
}

// us-8022e19c - advance the window when the game-manager's active
// kind differs from the window's current kind (window kind must be < 5).
void CSysWin::syncKindAdvance() {
    if (field_38 >= 5) return;
    if (field_39 == (u32)isClassicController__Q22cf13CfGameManagerFv(-1)) return;
    applyKind(field_38);
}

// us-8022e1f0 - rewind the window animation; clear the open flag
// (phase 0) once the reverse anim finishes.
void CSysWin::rewindToPhase0() {
    if (AnimRewindFrame(mAnimTrans, 1.0f) != 0) {
        field_35 = 0;
        field_36 = 1;
        field_34 = 0;
    }
}

// us-8022e240 - layout/kind setup after a file load (called by
// OnFileEvent once the System window layout is built): find the two label
// panes, write all six label texts, bind the tag processor into each label
// pane, then re-apply the current kind's pane visibility via switchKindPane.
void CSysWin::setupAfterLoad() {
    if (mLayout == 0) return;
    field_28 = 1;
    field_2C = mLayout->GetRootPane()->FindPaneByName("nul_close00", true);
    field_30 = mLayout->GetRootPane()->FindPaneByName("nul_slct", true);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes01", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes02_00", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes02_01", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes03", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes04_00", "", 0);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes04_01", "", 0);
    // TextBox::mpTagProcessor at +0xF8 (MonolithSoft nw4r extension).
    struct TextBoxLayout {
        u8 _[0xF8];
        void* mpTagProcessor;
    };
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes01", true));
        void* tag = mTagProcessor;
        t->mpTagProcessor = tag;
    }
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes02_00", true));
        void* tag = mTagProcessor;
        t->mpTagProcessor = tag;
    }
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes02_01", true));
        void* tag = mTagProcessor;
        t->mpTagProcessor = tag;
    }
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes03", true));
        void* tag = mTagProcessor;
        t->mpTagProcessor = tag;
    }
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes04_00", true));
        void* tag = mTagProcessor;
        t->mpTagProcessor = tag;
    }
    {
        TextBoxLayout* t = reinterpret_cast<TextBoxLayout*>(
            mLayout->GetRootPane()->FindPaneByName("txt_sysmes04_01", true));
        void* tag = mTagProcessor;
        t->mpTagProcessor = tag;
    }
    switchKindPane(field_37);
}

// us-8022e43c - build the System window layout from the loaded arc
// file (the mFileHandle match gate). Mirrors the CSelShopWin load path: mem
// region, tag processor, resource accessor, layout + anim transform, font
// binding, then hand off to setupAfterLoad and release the handle/region. The
// Class_8045F858 stack guard destructs at scope end (after validateHeap).
bool CSysWin::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle == pEventFile->mFileHandle) {
        u32 handle = mtl::MemManager::getHandleMEM2();
        mMemRegion.createRegion(handle, 0x3000, "CSysWin", 1);
        Class_8045F858 regionGuard(&mMemRegion);
        u8* archive = reinterpret_cast<u8*>(mFileHandle->getData());
        mtl::MemManager::setMemInitFlag(false);
        CTagProcessor* tag = (CTagProcessor*)mtl::MemManager::allocate(
            0x858, CLibLayout::getAllocHandle());
        if (tag != 0) {
            tag = (CTagProcessor*)__ct__CTagProcessor(tag);
        }
        mTagProcessor = tag;
        mArcAccessor = CLibLayout::createArcResourceAccessor();
        mArcAccessor->Attach(archive, "arc");
        buildLayout(&mLayout, mArcAccessor, "mf70_cf60_syswin01.brlyt");
        bindLayoutAnimTransform(mLayout, &mAnimTrans, mArcAccessor, "mf70_cf60_syswin01_in.brlan");
        nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
        CSysWinFontView* font = (CSysWinFontView*)CDeviceFont::getFontInfo(1, mLayout);
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)font->v07());
        mLayout->SetAnimationEnable(mAnimTrans, true);
        mLayout->Animate(0);
        setupAfterLoad();
        mFileHandle = 0;
        mMemRegion.validateHeap();
        return true;
    }
    return false;
}
