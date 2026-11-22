#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/menu/CMenuPause.hpp"
#include "kyoshin/menu/MenuSndApi.hpp"


#include "monolib/device/CDeviceVI.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/core/CPadManager.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/code_80135FDC.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUICfManager_Res.hpp"
#include "kyoshin/cf/CfSoundMan.hpp"

extern u32 gmPauseFlags;

// Forward declaration for the pause-exit input handler (defined below). Retail
// emits this under C linkage (`Pause_HandleExitInput`), so it must stay unmangled.
extern "C" void Pause_HandleExitInput(CMenuPause* p);

// ---- Declarations private to this unit ----

// C-linkage imports (retail symbol names -- these identifiers are pre-mangled
// or unmangled in retail, so they must keep C linkage to bind to the literal
// name).


// Texture object returned by ArcResourceAccessor::GetResource for the 'timg'
// tag. mChain (+0x08) -> mDims (+0x00) carries the 2D dimension header.
struct CMenuPauseTexDims {
    u16 field_0x0; // +0x00
    u16 field_0x2; // +0x02
};
struct CMenuPauseTexChain {
    CMenuPauseTexDims* mDims; // +0x00
};
struct CMenuPauseTexObj {
    u8 _0[0x8];
    CMenuPauseTexChain* mChain; // +0x08
};

extern "C" {
    void func_800853C8__Q22cf13CfGameManagerFv();
    // setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).

    // CMenuPause factory constructor imports.
    void __ct__17UnkClass_8045F564Fv(UnkClass_8045F564*);
    u32 getWorkMem__17CWorkThreadSystemFv();

    // Init() resource/layout helpers.
    void* getHandleMEM2__Q23mtl10MemManagerFv();
    void* __ct__14Class_8045F858FP17UnkClass_8045F564(void*, void*);
    void __dt__14Class_8045F858Fv(void*, int);
    void setMemInitFlag__Q23mtl10MemManagerFb(bool);
    void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);
    void validateHeap__17UnkClass_8045F564Fv(void*);
    void addRenderCB__4CScnFP10IScnRenderUlUl(void*, void*, u32, u32);
}

CMenuPause::~CMenuPause() {}

void CMenuPause::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(true);
    MenuSnd_SetSlotsPause_8890(1);

    // Scoped MEM2 region guard, then build the layout + animations.
    mMemRegion.createRegion((int)mtl::MemManager::getHandleMEM2(), 0x2000,
                            "CMenuPause", 0);
    Class_8045F858 regionGuard(&mMemRegion);
    mtl::MemManager::setMemInitFlag(false);

    buildLayout(&mLayout, CUICfManager_getArcResourceAccessor(), "mf70_cf60_syswin13.brlyt");
    bindLayoutAnimTransform(mLayout,
                  &mAnim,
                  CUICfManager_getArcResourceAccessor(), "mf70_cf60_syswin13_in.brlan");

    // Bind the font and hand the loaded font object over to the root pane.
    nw4r::lyt::Pane* rootPane = mLayout->GetRootPane();
    IDeviceFontInfo* fontInfo =
        static_cast<IDeviceFontInfo*>(CDeviceFont::getFontInfo(1, mLayout));
    ApplyFontToTxtPanes(rootPane, fontInfo->getFont());

    mLayout->SetAnimationEnable(
        mAnim, true);
    mLayout->Animate(0);

    // Fill the two message-table text fields.
    char* title = BdatTouchStringCell("MNU_sysmes", "name", 8);
    LayoutSetTextBoxFmtValue(mLayout, "txt_titlemes", title, 0);
    char* help0 = BdatTouchStringCell("MNU_kyeassign", "help", 0x6b);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes00", help0, 0);

    // ---- Cursor texture palette variants (classic vs remote pad). ----
    // Each: pick a message name by pad type, load the "timg" texture
    // resource, bind it (PaneSetTexPaletteByName) and set the pane size from the
    // texture's 2D dimension header (u16 w/h converted via the 2^52 trick).
    {
        u16 msg = BdatGetU16ByTableKey(
            "MNU_kyeassign",
            cf::CfGameManager::isClassicController(-1) ? "fileID_2"
                                                      : "fileID_1",
            0x6b);
        char* handle = MakeTplNameSysFile(msg);
        void* tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, handle, NULL);
        if (tex != NULL) {
            PaneSetTexPaletteByName(mLayout, (const char*)((void*)"pic_btn00"), (TPLPalette*)tex);
            // u16 -> f32 via MWCC's own 0x43300000 double-trick conversion:
            // the width/height reads are hoisted ahead of FindPaneByName and
            // the Size copy lands as two stfs pairs (pane + stack scratch).
            CMenuPauseTexDims* dims =
                ((CMenuPauseTexObj*)tex)->mChain->mDims;
            u16 texHeight = dims->field_0x2;
            u16 texWidth = dims->field_0x0;
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()
                                         ->FindPaneByName("pic_btn00",
                                                          true);
            if (pane != NULL) {
                pane->SetSize(nw4r::lyt::Size(texHeight, texWidth));
            }
        }
    }
    // Middle message-table text field between the two cursor variants.
    char* help1 = BdatTouchStringCell("MNU_kyeassign", "help", 0x6c);
    LayoutSetTextBoxFmtValue(mLayout, "txt_sysmes01", help1, 0);
    {
        u16 msg = BdatGetU16ByTableKey(
            "MNU_kyeassign",
            cf::CfGameManager::isClassicController(-1) ? "fileID_2"
                                                      : "fileID_1",
            0x6c);
        char* handle = MakeTplNameSysFile(msg);
        void* tex = CUICfManager_getArcResourceAccessor()->GetResource(0x74696D67, handle, NULL);
        if (tex != NULL) {
            PaneSetTexPaletteByName(mLayout, (const char*)((void*)"pic_btn01"), (TPLPalette*)tex);
            CMenuPauseTexDims* dims =
                ((CMenuPauseTexObj*)tex)->mChain->mDims;
            u16 texHeight = dims->field_0x2;
            u16 texWidth = dims->field_0x0;
            nw4r::lyt::Pane* pane = mLayout->GetRootPane()
                                         ->FindPaneByName("pic_btn01",
                                                          true);
            if (pane != NULL) {
                pane->SetSize(nw4r::lyt::Size(texHeight, texWidth));
            }
        }
    }

    mState = 1;
    playUISound(0xd);

    mField74 = 0;

    // Register as a scene render callback; IScnRender subobject lives at +0x5c.
    mScene->addRenderCB(this, 0x12, 0);

    mMemRegion.validateHeap();
}


void CMenuPause::Term() {
    CDeviceVI::waitForDrawDone();

    mScene->removeRenderCB(this);

    if (mLayout != NULL) {
        delete mLayout;
        mLayout = NULL;
    }

    mMemRegion.deleteRegion();

    sInstance = nullptr;
    MenuSnd_SetSlotsPause_8890(0);
    setPresentationFlag__Q22cf13CfGameManagerFv(false);
}

void CMenuPause::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) goto exit;
    if ((gmPauseFlags & (1u << 21)) != 0) goto exit;
    // Branch-over-branch guard: `goto body` with the `exit` label + return
    // placed BEFORE `body` keeps MWCC from folding the bit test to a single
    // `bne` -- it emits retail's `beq body; b exit` (docs/MWCC_CASES.md
    // 5824 / 4707).
    goto body;
exit:
    return;
body:
    if (!IsMenuState621F0()) {
        goto exit;
    }
    if (mLayout == NULL) {
        goto exit;
    }

    switch (mState) {
    case 1:
        // Opening animation: tick it forward; when it finishes arms state 2.
        if (advanceAnimTransform(mAnim,
                          1.0f) != 0) {
            mState = 2;
        }
        break;
    case 2:
        // Wait for a cancel/confirm input to leave the pause menu.
        Pause_HandleExitInput(this);
        break;
    case 3:
        // Closing animation: when it finishes, reset to state 0 / clear flag.
        if (AnimRewindFrame(mAnim,
                          1.0f) != 0) {
            mState = 0;
            mField54 = 1;
        }
        break;
    }

    // Pump the layout animation once.
    mLayout->Animate(0);
}

void CMenuPause::cbRenderBefore(CScn*) {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) goto done;
    // Branch-over-branch guard, same trick as Move(): keeps MWCC from folding
    // the bit test into the first bne -- retail emits `beq body; b done`.
    if ((gmPauseFlags & (1u << 21)) != 0) goto done;
    goto body;
done:
    return;
body:
    if (!IsMenuState621F0()) {
        goto done;
    }

    {
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        if (mLayout != NULL) {
            drawLayout(mLayout, &drawInfo, 0, 1);
        }
    }
}

CMenuPause* CMenuPause::sInstance;

// Only one pause menu exists at a time.
CMenuPause* CMenuPause::create(CProcess* parent, CScn* scene) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuPause* menu = new (CWorkThreadSystem::getWorkMem()) CMenuPause(scene);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

unsigned long CMenuPause::isInitialized() {
    return sInstance != nullptr;
}

extern "C" unsigned long Pause_IsActive() {
    CMenuPause* p = CMenuPause::sInstance;
    if (p == NULL) {
        return 0;
    }
    return p->mState != 3;
}

extern "C" void Pause_HandleExitInput(CMenuPause* p) {
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 first;
    u32 second;
    // Call the raw mangled import so the -1 channel argument reaches r3.
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        first = (pad->mPressedButtonFlags >> 21) & 1;
        second = (pad->mPressedButtonFlags >> 22) & 1;
    } else {
        first = (pad->mPressedButtonFlags >> 4) & 1;
        second = (pad->mPressedButtonFlags >> 5) & 1;
    }
    if (first) {
        MenuStateClear64064();
        func_800853C8__Q22cf13CfGameManagerFv();
        CfSoundMan_PlayRequest(0, 3, 0, 0.6f);
        CfSoundMan_PlayRequest(0, 0xe, 0, 0.6f);
        p->mState = 3;
    } else if (second) {
        playUISound(6);
        playUISound(0xe);
        p->mState = 3;
    }
}
