// Auto-scaffolded catalog TU for kyoshin/CSysWinScenarioLog

#define IWORK_EVENT_INLINE_DTOR
#define CTTASK_INLINE_DTOR
#define IUIWINDOW_INLINE_DTOR
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "kyoshin/cf/CfMapItemManager.hpp"
#include <types.h>

#include "kyoshin/CSysWinScenarioLog.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/device/CFileHandle.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/device/CDeviceVI.hpp"

#include <revolution/gx/GXPixel.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_arcResourceAccessor.h>
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::CSysWinScenarioLog (us-80280934)
// Runs CProcess's ctor, lets the inlined CTTask<IUIWindow>/IUIWindow ctors write
// the temp vptr + the two null ptmf hooks + the window control words, then the
// composite table with its +0x24 / +0xAC sub-vptrs, the embedded
// UnkClass_8045F564 region and the window fields; finally bumps the open-window
// bookkeeping counters.
// ---------------------------------------------------------------------------
CSysWinScenarioLog::CSysWinScenarioLog(CScn* scene)
    : mFileHandle74(NULL), mFileHandle78(NULL), mMemRegion(), mScene(scene), mState(0),
      mField94(NULL), mpAccessor(NULL), mpLayout(NULL), mpAnim(NULL) {
    setPresentationFlag__Q22cf13CfGameManagerFv(true);
    code80135FDC_postIncByte_64080();
}

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::~CSysWinScenarioLog (us-80280a38)
// D1-inlined-into-D2 virtual dtor: the compiler destroys mMemRegion@0x7c, then
// the CProcess base (through the inlined CTTask<IUIWindow>/IUIWindow dtors) and
// the conditional operator delete.
// ---------------------------------------------------------------------------
CSysWinScenarioLog::~CSysWinScenarioLog() {}

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::Init (us-80280aac)
// Register this window's IScnRender sub-object (+0x70) with the owning scene,
// then kick off both file loads against the IWorkEvent sub-object (+0x6C):
// the layout archive from MEM2 and the BDAT common archive (+0x18 into the
// string pool).
// ---------------------------------------------------------------------------
void CSysWinScenarioLog::Init() {
    // Null-this tolerant sub-object selection: seed with the raw pointer,
    // then overwrite with the embedded interface offset when this is live.
    // A fresh selection per call site matches retail, which re-materializes
    // the +0x6c IWorkEvent offset before each file request.
    IScnRender* render = this;
    mScene->addRenderCB(render, 0xD, 0);

    IWorkEvent* ev74 = this;
    mFileHandle74 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), "menu/jp/ScenarioLog.arc", ev74, 0, 0);

    IWorkEvent* ev78 = this;
    mFileHandle78 = CDeviceFile::readCommonArchiveFile(
        KyoshinHeap_GetField44(), "/common/jp/bdat_menu_slog.bin", ev78, 0, 0);
}

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::Term (us-80280b60)
// Tear down the scenario-log window: wait for the VI, release the two file
// handles and the BDAT archive, destroy the layout pair (vtable slot-2 call
// with arg 1, guarded like retail's double-beq), free the arc accessor, clear
// the mem region + globals, deregister the render callback from the scene, and
// drop the open-window bookkeeping counter once per boot.
// ---------------------------------------------------------------------------
void CSysWinScenarioLog::Term() {
    CDeviceVI::waitForDrawDone();
    closeFileHandle(&mFileHandle74);
    closeFileHandle(&mFileHandle78);
    CBdat::getEntry(2);

    if (mpLayout != 0) {
        nw4r::lyt::Layout* p = mpLayout;
        if (p != 0) {
            ((CSysWinLayoutSlot2View*)p)->mSlot2(1);
        }
        mpLayout = 0;
    }
    if (mField94 != 0) {
        nw4r::lyt::Layout* p = mField94;
        if (p != 0) {
            ((CSysWinLayoutSlot2View*)p)->mSlot2(1);
        }
        mField94 = 0;
    }
    releaseArcResourceAccessor(mpAccessor);
    mpAccessor = 0;
    lbl_eu_8066490C = 0;
    mMemRegion.deleteRegion();
    lbl_eu_80664908 = 0;

    IScnRender* render = this;
    mScene->removeRenderCB(render);

    DecMenuCounter64080();
    if (code80135FDC_getByte_64080() == 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(false);
    }
}

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::Move (us-80280c54)
// Advance the scenario-log window's opening/closing state machine.
// Gate layout mirrors retail: the first guard folds to a direct branch to the
// shared epilogue, the pause-bit guard renders as the branch-over-branch
// (beq forward + b end, cbRenderBefore pattern), and the remaining guards are
// separate single-condition early returns (direct conditional branches).
// ---------------------------------------------------------------------------
void CSysWinScenarioLog::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() != 0 ||
        (gmPauseFlags & 0x200000) != 0) {
        return;
    }
    if (IsMenuState621F0() == 0) {
        return;
    }
    if (MenuTutorialIsCreated() != 0) {
        return;
    }
    if (mpLayout == 0) {
        return;
    }
    {
        switch (mState) {
        case 0:
            SysWinLog_OpenContent(this);
            break;
        case 1:
            if (advanceAnimTransform(mpAnim, 1.0f) != 0) {
                mState = 2;
            }
            break;
        case 2: {
            CSysWinPadView* pad = (CSysWinPadView*)cf::CfGameManager::getCurrentPad();
            bool pressed;
            if (cf::CfGameManager::isClassicController(-1) != 0) {
                pressed = (pad->mButtons & 0x4600000) != 0;
            } else {
                pressed = (pad->mButtons & 0x1030) != 0;
            }
            if (pressed) {
                mState = 3;
                playUISound(6);
                playUISound(0xE);
            }
            break;
        }
        case 3:
            if (AnimRewindFrame(mpAnim, 1.0f) != 0) {
                mField67 = 0;
                mField64 = 1;
            }
            break;
        }
        // Layout virtual update (vtable slot 0x38) with a null argument.
        ((CSysWinLayoutHook*)mpLayout)->mAt38(0);
    }
}

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::cbRenderBefore (us-80280dc0)
// Draw the scenario-log layout behind the scene while the game is live: same
// gate chain as Move() but testing bit 10 of the pause word, then Z-off and
// a layout draw with a stack DrawInfo.
// noinline keeps the this-adjusting vtable thunk SysWinLog_RenderThunk70 a two-instruction
// subi + tail-call instead of a full inline copy.
// ---------------------------------------------------------------------------
__declspec(noinline) void CSysWinScenarioLog::cbRenderBefore() {
    // OR-combined guards with early return (MWCC_CASES control_flow pattern):
    // the first disjunct folds to a direct branch to the shared epilogue and
    // the second becomes the branch-over-branch gate (beq forward + b end).
    if (CTaskGame::getInstance()->isFlag01Set() != 0 ||
        (gmPauseFlags & 0x200000) != 0) {
        return;
    }
    // Separate single-condition early returns fold to direct branches
    // (an || chain would put a spurious branch-over-branch on the last
    // disjunct).
    if (IsMenuState621F0() == 0) {
        return;
    }
    if (MenuTutorialIsCreated() != 0) {
        return;
    }
    if (mpLayout == 0) {
        return;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawLayout(mpLayout,
                  &drawInfo, 0, 1);
}

// ---------------------------------------------------------------------------
// SysWinLog_Create (us-80280e6c)
// Allocate the CSysWinScenarioLog singleton from work memory, construct it,
// store it, and register it as a child of `parent`. Returns null when already
// created. (Regist is called even if the allocation failed, matching retail.)
// ---------------------------------------------------------------------------
CSysWinScenarioLog* SysWinLog_Create(CProcess* parent, u8* param) {
    if (lbl_eu_80664908 != 0) {
        return nullptr;
    }
    CSysWinScenarioLog* obj = new (mtl::MemManager::allocate(0xA4, CWorkThreadSystem::getWorkMem()))
        CSysWinScenarioLog((CScn*)param);
    lbl_eu_80664908 = (u32)obj;
    obj->Regist(parent, false);
    return (CSysWinScenarioLog*)lbl_eu_80664908;
}

extern "C" bool SysWinLog_BuildLayout(CSysWinScenarioLog* self, CFileHandle* fh);

// ---------------------------------------------------------------------------
// SysWinLog_OpenContent (us-80280ef0)
// Open the scenario-log window content (Move state 0): guard on the layout
// being loaded and the BDAT file pointer being set, mark the window busy,
// pick the first message row whose id reaches the message-count threshold,
// stamp the four text fields, resolve the texture for the current game mode
// and size the picture pane from its dims.
// ---------------------------------------------------------------------------
extern "C" void SysWinLog_OpenContent(CSysWinScenarioLog* self) {
    if (self->mpLayout == 0) {
        return;
    }
    if (lbl_eu_8066490C == 0) {
        return;
    }
    self->mState = 1;
    playUISound(0xd);
    // (u32) cast pins the extern "C" decl (pluginUi.hpp's C++ int overload
    // would otherwise mangle the reloc to CtrlRemote_TouchBitByArg__Fi).
    u16 maxId = (u16)CtrlRemote_TouchBitByArg((u32)0x20);
    int chosen = 1;
    u16 count = (u16)Bdat_GetMaxRow_B1EC((void*)lbl_eu_8066490C);
    int i = 1;
    while ((u16)i <= count) {
        if ((u16)BdatGetU16Direct((void*)lbl_eu_8066490C, "flag",
                               (u16)i) >= maxId) {
            chosen = i;
            break;
        }
        i++;
    }
    char* s = BdatTouchStringCell("MNU_main", "name", 0x19);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_titlemes", s, 0);
    char* t = BdatGetPtrDirect((void*)lbl_eu_8066490C, "help",
                            (u16)chosen);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_sysmes01", t, (u32)self->mField94);
    char* u = BdatTouchStringCell("MNU_kyeassign", "help", 0x2c);
    LayoutSetTextBoxFmtValue(self->mpLayout, "txt_close00", u, 0);
    const char* sel = cf::CfGameManager::isClassicController(-1) != 0
                          ? "fileID_2"
                          : "fileID_1";
    u16 msgId = (u16)BdatGetU16ByTableKey("MNU_kyeassign", sel, 0x2c);
    void* tex = MakeTplNameSysFile(msgId);
    nw4r::lyt::ArcResourceAccessor* mgr = CUICfManager_getArcResourceAccessor();
    void* mat = mgr->GetResource(0x74696d67, (const char*)tex, 0);
    if (mat == 0) {
        return;
    }
    PaneSetTexPaletteByName(self->mpLayout, "pic_btn00", (TPLPalette*)mat);
    CSysWinTexMeta* meta = (CSysWinTexMeta*)mat;
    u16 w = meta->field_0x8->field_0x2;
    u16 h = meta->field_0x8->field_0x0;
    void* pane =
        ((CSysWinPaneView*)self->mpLayout->GetRootPane())->mSetText(
            "pic_btn00", 1);
    if (pane == 0) {
        return;
    }
    CSysWinF64Conv convW;
    CSysWinF64Conv convH;
    CSysWinPaneView* pv = (CSysWinPaneView*)pane;
    convW.w[1] = w;
    convW.w[0] = 0x43300000;
    pv->field_0x4C = (f32)(convW.d - lbl_eu_80668AD8);
    convH.w[1] = h;
    convH.w[0] = 0x43300000;
    pv->field_0x50 = (f32)(convH.d - lbl_eu_80668AD8);
}

// ---------------------------------------------------------------------------
// SysWinLog_BuildLayout (us-80281104)
// File-event handler for the scenario-log window. When the exchanged file
// matches mFileHandle74: create the scratch region, take the file data,
// allocate + construct the tag processor, build the arc accessor + layout +
// animation, bind the font and animate, then return 1. When it matches
// mFileHandle78 instead: release the BDAT archive and store the loaded FP in
// lbl_eu_8066490C, return 1. Otherwise return 0.
// ---------------------------------------------------------------------------
extern "C" bool SysWinLog_BuildLayout(CSysWinScenarioLog* self, CFileHandle* fh) {
    void* data = fh->mData;
    if (self->mFileHandle74 == data) {
        self->mMemRegion.createRegion(mtl::MemManager::getHandleMEM2(), 0x8000,
                                      "CSysWinScenarioLog", 0);
        Class_8045F858 memHost(&self->mMemRegion);
        void* fileData = self->mFileHandle74->getData();
        mtl::MemManager::setMemInitFlag(false);
        void* tag = mtl::MemManager::allocate(0x858, CLibLayout::getAllocHandle());
        if (tag != 0) {
            tag = __ct__CTagProcessor(tag);
        }
        self->mField94 = (nw4r::lyt::Layout*)tag;
        self->mpAccessor = CLibLayout::createArcResourceAccessor();
        self->mpAccessor->Attach(fileData, "arc");
        buildLayout(&self->mpLayout, self->mpAccessor, "mf70_cf60_syswin14.brlyt");
        bindLayoutAnimTransform(self->mpLayout, &self->mpAnim, self->mpAccessor, "mf70_cf60_syswin14_in.brlan");
        nw4r::lyt::Pane* rootPane = self->mpLayout->GetRootPane();
        void* fontObj = CDeviceFont::getFontInfo(1, self->mpLayout);
        u32 fontHandle = ((CSysWinFontObjView*)fontObj)->getFontHandle();
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontHandle);
        self->mpLayout->SetAnimationEnable(self->mpAnim, true);
        self->mpLayout->Animate(0);
        void* pane = ((CSysWinPaneView*)self->mpLayout->GetRootPane())->mSetText(
            "txt_sysmes01", 1);
        ((CSysWinPaneView*)pane)->field_0xF8 = (u32)self->mField94;
        self->mFileHandle74 = 0;
        self->mMemRegion.validateHeap();
        return 1;
    }
    if (self->mFileHandle78 == data) {
        void* fileData78 = self->mFileHandle78->getData();
        CBdat::setBdatEntry(2, fileData78);
        Bdat_GetTable_AA34();
        lbl_eu_8066490C = (u32)getFP("MNU_scenario");
        self->mFileHandle78 = 0;
        return 1;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// CSysWinScenarioLog::OnFileEvent: the IWorkEvent slot (+0x34 of the sub-vtable)
// forwards straight to the file-event handler, so the compiler folds the body
// into the +0x6C adjusting thunk and emits nothing else.
// ---------------------------------------------------------------------------
bool CSysWinScenarioLog::OnFileEvent(CEventFile* pEventFile) {
    return SysWinLog_BuildLayout(this, (CFileHandle*)pEventFile);
}
