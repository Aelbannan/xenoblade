// Auto-scaffolded catalog TU for kyoshin/menu/CMenuQuestLog
// Replace stubs with high-level C/C++ during decomp.


#include "kyoshin/menu/CMenuQuestLog.hpp"
#include "kyoshin/CMainMenu.hpp" // CMainMenu_GetInstancePtr

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"  // cf::CfGameManager::getCurrentPad / isClassicController
#include "kyoshin/code_80135FDC.hpp"     // IsMenuState621F0 / SetWideLocAdjust
#include "monolib/core/CPadManager.hpp"   // CPad::mPressedButtonFlags
#include "monolib/util/MemManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"

#include <revolution/GX.h>
#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/CTitleAHelp.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"

// Retail constructor: out-of-line, so the factory (createQuestLogMenu) emits a
// real `bl` to it. The IUICf/CTTask base chain and the IScnRender vptr store
// are emitted by the compiler.
CMenuQuestLog::CMenuQuestLog(CProcess* parent, u32 arg2)
    : mScene(parent), mBgTex(0), mTitleAHelp(0, 0), mQstLogList(0),
      field_2280(arg2), mState(0) {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);
    code80135FDC_postIncByte_64080();
}

CMenuQuestLog::~CMenuQuestLog() {}

// ---------------------------------------------------------------------------
// ---- CMenuQuestLog::Init (us-8011d08c) -------------------------------------
// Re-initialise each embedded widget by assigning a freshly constructed
// temporary (same pattern as CMenuCollepedia::Init), then register the
// render-callback sub-object on the owning scene.
// ---------------------------------------------------------------------------
void CMenuQuestLog::Init() {
    mBgTex = CBgTex(0);
    BgTex_Acquire_3C14(&mBgTex);

    char* name = BdatTouchStringCell(lbl_eu_804FE518, lbl_eu_804FE518 + 0xa, 1);
    mTitleAHelp = CTitleAHelp(name, 0x3c);
    mTitleAHelp.CTitleAHelp_load();

    mQstLogList = CQstLogList((u16)field_2280);
    mQstLogList.LoadArc();

    mQstLogInfo = CQstLogInfo();
    QstLogInfo_LoadArcHandle(&mQstLogInfo);

    // Register the render callback (IScnRender base at +0x58).
    reinterpret_cast<CScn*>(mScene)->addRenderCB(this, 0x10, 0);
}

// ---------------------------------------------------------------------------
// ---- CMenuQuestLog::Term (us-8011d528) -------------------------------------
// Wait for the VI, deregister the render callback (this-adjusting IScnRender
// view at +0x58), tear down the embedded widgets, clear the singleton, then
// re-enable the cf load once if the boot-time byte flag is still clear.
// ---------------------------------------------------------------------------
void CMenuQuestLog::Term() {
    CDeviceVI::waitForDrawDone();
    reinterpret_cast<CScn*>(mScene)->removeRenderCB(this);
    BgTex_Release_3D9C(&mBgTex);
    mTitleAHelp.teardown();
    mQstLogList.Teardown();
    QstLogInfo_TeardownLayout(&mQstLogInfo);
    spQuestLogMenuInstance[0] = 0;
    DecMenuCounter64080();
    if (code80135FDC_getByte_64080() == 0)
        setPresentationFlag__Q22cf13CfGameManagerFv(false);
    CUICfManager_setTimeout30();
}

void CMenuQuestLog::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto exit;
    }
    // Branch-over-branch guard (same shape as cbRenderBefore): exit label
    // before body label emits retail's `beq body; b exit`.
    if (!(gmPauseFlags & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    // Close request: classic controller bit 23 / wii bit 10 plays the close
    // sound and flags the closing state.
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 close;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        close = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        close = (pad->mPressedButtonFlags >> 10) & 1;
    }
    if (close != 0) {
        if (CMainMenu_GetInstancePtr()) {
            ArtsInfo_SetReadyFlag();
        }
        playUISound(6);
        mState = 0xa;
        mField54 = 1;
    }

    // State machine (jumptable dispatch on the state byte).
    switch (mState) {
    case 0:
        openQuestLogMenu(this);
        break;
    case 1:
        gateQuestLogList(this);
        break;
    case 2:
        handleQuestLogInput(this);
        break;
    case 3:
        flagQuestLogReady(this);
        break;
    case 4:
        selectQuestLogEntry(this);
        break;
    case 5:
        startQuestLogInfo(this);
        break;
    case 6:
        finishQuestLogInfo(this);
        break;
    case 7:
        closeQuestLogMenu(this);
        break;
    case 8:
        resortQuestLogList(this);
        break;
    case 9:
        reopenQuestLogList(this);
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    mTitleAHelp.updateHelp();
    mQstLogList.FrameUpdate();
    QstLogInfo_StepStateMachine(&mQstLogInfo);
}

void CMenuQuestLog::cbRenderBefore(CScn*) {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set()) {
        goto exit;
    }
    // Branch-over-branch guard keeps MWCC from folding the bit test into a
    // single branch -- emits retail's `beq body; b exit`.
    if (!(gmPauseFlags & (1u << 21))) {
        goto body;
    }
    goto exit;
exit:
    return;
body:
    if (IsMenuState621F0() == 0) {
        goto exit;
    }
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    {
        nw4r::lyt::DrawInfo drawInfo;
        SetWideLocAdjust(&drawInfo);
        BgTex_Draw_3D7C(&mBgTex, &drawInfo);
        mQstLogList.Draw(&drawInfo);
        QstLogInfo_DrawLayoutTail(&mQstLogInfo, &drawInfo);
        mTitleAHelp.drawHelp(&drawInfo);
    }
}

// ---------------------------------------------------------------------------
// ---- createQuestLogMenu (us-8011d7bc) ------------------------------------------
// Factory: lazily allocate + construct the single quest-log menu instance and
// register it as a child of `self`. Returns the stored singleton (or 0 if it
// already exists). Regist is called even when the allocation failed, matching
// retail.
// ---------------------------------------------------------------------------
// CProcess::Regist is declared in monolib/work/CProcess.hpp.

extern "C" CMenuQuestLog* createQuestLogMenu(CProcess* self, CProcess* parent, u32 arg2) {
    if (spQuestLogMenuInstance[0] != 0) {
        return 0;
    }
    CMenuQuestLog* obj = new (CWorkThreadSystem::getWorkMem()) CMenuQuestLog(parent, arg2);
    spQuestLogMenuInstance[0] = obj;
    obj->Regist(self, false);
    return spQuestLogMenuInstance[0];
}

// (spQuestLogMenuInstance != 0) - retail lwz sda21; subic; subfe
extern "C" bool isQuestLogMenuActive() { return spQuestLogMenuInstance[0] != 0; }

// State-machine step for state 1: once the background layout, title help,
// list and info widget are all ready, open the title help + list, set state 1
// and play sound 0x6d.
extern "C" void openQuestLogMenu(CMenuQuestLog* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex)) {
        if (self->mTitleAHelp.isInitialized()) {
            if (self->mQstLogList.ScrollFlag()) {
                if (self->mQstLogInfo.QstLogInfo_GetLoadedMark()) {
                    self->mTitleAHelp.func_801C412C();
                    self->mQstLogList.BeginSortOpen();
                    self->mState = 1;
                    playUISound(0x6d);
                }
            }
        }
    }
}

// Gate the quest-log list state: when the help sub-object is idle and the
// list accepts input, raise the state byte to 2.
extern "C" void gateQuestLogList(CMenuQuestLog* self) {
    if (self->mTitleAHelp.func_801C4124() && self->mQstLogList.IsSortEnabled())
        self->mState = 2;
}

// Retail handleQuestLogInput is a large real handler (us-8011d920); keep this
// placeholder out-of-line so Move's case 2 still emits a real bl.
// State-2 input handler: decodes the pad (classic vs wiimote bit positions)
// and drives the list widget: confirm opens the info pane, up/down scroll,
// cancel/dir/confirm/menu shortcuts, and the sort trigger re-sorts + updates
// the title-help mode depending on whether sorting is available.
extern "C" void handleQuestLogInput(CMenuQuestLog* self) {
    CQuestLogPadData* pad = (CQuestLogPadData*)cf::CfGameManager::getCfPadData();
    int cancel, trigger3, trigger1, trigger2, confirm, menu, dir;
    // The bit positions differ between Classic Controller and Wiimote.
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPressedButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = ((f & 0x10000) | (f & 0x8)) != 0;
        confirm = (f & 0x2001) != 0;
        menu = (f & 0x4002) != 0;
        trigger1 = (p >> 21) & 1;
        trigger2 = (p >> 22) & 1;
        trigger3 = (p >> 24) & 1;
    } else {
        u32 f = pad->mTurboPressButtonFlags;
        u32 p = pad->mPressedButtonFlags;
        cancel = (f & 0x8004) != 0;
        dir = ((f & 0x10000) | (f & 0x8)) != 0;
        confirm = (f & 0x2001) != 0;
        menu = (f & 0x4002) != 0;
        trigger1 = (p >> 4) & 1;
        trigger2 = (p >> 5) & 1;
        trigger3 = (p >> 11) & 1;
    }

    if (trigger1) {
        self->mQstLogList.ApplySortSelection();
        if (!self->mQstLogList.IsSortEnabled()) {
            self->mTitleAHelp.markReplayClose();
            self->mQstLogList.EndSortMode(0);
            self->mState = 4;
        } else {
            self->mTitleAHelp.func_801C41E8(0x3c);
        }
    } else if (trigger2) {
        if (self->mQstLogList.IsSortDescending()) {
            self->mQstLogList.ConfirmSort(0);
            self->mTitleAHelp.func_801C41E8(0x3c);
        } else {
            self->mTitleAHelp.beginClose();
            self->mQstLogList.EndSortMode(1);
            self->mState = 3;
        }
    } else if (cancel) {
        self->mQstLogList.ScrollUp();
    } else if (dir) {
        self->mQstLogList.ScrollDown();
    } else if (confirm) {
        self->mQstLogList.PageUp();
    } else if (menu) {
        self->mQstLogList.PageDown();
    } else if (trigger3) {
        self->mQstLogList.ToggleSort();
        self->mTitleAHelp.func_801C41E8(self->mQstLogList.IsSortDescending() ? 0x3e : 0x3c);
    }
}

// Same gate as gateQuestLogList but toggles the +0x54 byte to 1.
extern "C" void flagQuestLogReady(CMenuQuestLog* self) {
    if (self->mTitleAHelp.func_801C4124() && self->mQstLogList.IsSortEnabled())
        self->mField54 = 1;
}

extern "C" void selectQuestLogEntry(CMenuQuestLog* self)
{
    CMenuQuestLog* base = self;
    if (base->mTitleAHelp.func_801C4124() && base->mQstLogList.IsSortEnabled()) {
        unsigned int value = base->mQstLogList.GetSelectedQuestLo();
        base->mQstLogInfo.QstLogInfo_SetQuestIdSlot(static_cast<unsigned short>(value));
        QstLogInfo_LoadCommonArchive(&base->mQstLogInfo);
        base->mState = 5;
    }
}

// ---------------------------------------------------------------------------
// ---- startQuestLogInfo (us-8011dbd8) ------------------------------------------
// When the quest log info is done, set the title-help mode to 0x3d, close the
// title help, finish the log info and advance to state 6.
// ---------------------------------------------------------------------------
extern "C" void startQuestLogInfo(CMenuQuestLog* self) {
    if (self->mQstLogInfo.QstLogInfo_GetLoadedMark()) {
        self->mTitleAHelp.func_801C41E8(0x3d);
        self->mTitleAHelp.reopenFromClose();
        QstLogInfo_Start(&self->mQstLogInfo);
        self->mState = 6;
    }
}

// ---------------------------------------------------------------------------
// ---- finishQuestLogInfo (us-8011dc34) ------------------------------------------
// When the title help is idle and the quest log info is done, advance to
// state 7.
// ---------------------------------------------------------------------------
extern "C" void finishQuestLogInfo(CMenuQuestLog* self) {
    if (self->mTitleAHelp.func_801C4124() && self->mQstLogInfo.QstLogInfo_GetReadyMark()) {
        self->mState = 7;
    }
}

// ---------------------------------------------------------------------------
// ---- closeQuestLogMenu (us-8011dc84) ------------------------------------------
// State 7 handler: on a close-button press (classic bit 23 / wii bit 10),
// close the title help, finish the log info and advance to state 8.
// ---------------------------------------------------------------------------
extern "C" void closeQuestLogMenu(CMenuQuestLog* self) {
    CPad* pad = cf::CfGameManager::getCurrentPad();
    // Close-button press: classic-controller bits 22-23 vs wii bits 4-5,
    // normalized to bool (retail's rlwinm + subic/subfe shape).
    bool close;
    if (cf::CfGameManager::isClassicController(-1)) {
        close = pad->mPressedButtonFlags & 0x600000;
    } else {
        close = pad->mPressedButtonFlags & 0x30;
    }
    if (close) {
        self->mTitleAHelp.markReplayClose();
        QstLogInfo_Resume(&self->mQstLogInfo);
        self->mState = 8;
    }
}

// ---------------------------------------------------------------------------
// ---- resortQuestLogList (us-8011dd08) ------------------------------------------
// When the title help is idle and the quest log info is done, set the
// title-help mode to 0x3c, close the title help, re-sort the list and advance
// to state 9.
// ---------------------------------------------------------------------------
extern "C" void resortQuestLogList(CMenuQuestLog* self) {
    if (self->mTitleAHelp.func_801C4124() && self->mQstLogInfo.QstLogInfo_GetReadyMark()) {
        self->mTitleAHelp.func_801C41E8(0x3c);
        self->mTitleAHelp.reopenFromClose();
        self->mQstLogList.BeginSortOpen();
        self->mState = 9;
    }
}

// ---------------------------------------------------------------------------
// ---- reopenQuestLogList (us-8011dd74) ------------------------------------------
// When the title help is idle and sorting is enabled, advance to state 2.
// ---------------------------------------------------------------------------
extern "C" void reopenQuestLogList(CMenuQuestLog* self) {
    if (self->mTitleAHelp.func_801C4124() && self->mQstLogList.IsSortEnabled()) {
        self->mState = 2;
    }
}

// sbss singleton (retail 0x8: pointer + emitted zero word).
CMenuQuestLog* spQuestLogMenuInstance[2] = { 0, 0 };

// --- hand-built retail data (splits: .rodata 804FE508-804FE6F0,
// .data 8052CC10-8052CD50, .sdata 80661E70-80661E78) ---
// .rodata menu-text pool (BdatTouchStringCell reads +0x0/+0xA in Init).
extern "C" __attribute__((used, aligned(8)))
const char lbl_eu_804FE518[0x10] = "MNU_quest\0name";
// .rodata marker-name table (each 0xF content+null, MWCC zero-pads to 0x10;
// referenced by the .data string-pointer table below).
// .data string-pointer table (28 marker addresses + 8 zero words) and the
// resource table below need the retail names visible here.
// .data marker-pointer table (retail lbl_eu_8052CCA8, 28 addresses + zero fill).
struct QuestLogStrTable { const char* p[28]; u32 reserved[8]; };
__attribute__((used, aligned(4)))
struct QuestLogStrTable lbl_eu_8052CCA8 = {
    {
        "markerlist0000", "markerlist0101", "markerlist0201", "markerlist0301",
        "markerlist0401", "markerlist0402", "markerlist0501", "markerlist0601",
        "markerlist0701", "markerlist0801", "markerlist0901", "markerlist1001",
        "markerlist1101", "markerlist1201", "markerlist1202", "markerlist1301",
        "markerlist1401", "markerlist1501", "markerlist1601", "markerlist1601",
        "markerlist1701", "markerlist1801", "markerlist1901", "markerlist2001",
        "markerlist2101", "markerlist2201", "markerlist2301", "markerlist2401",
    },
    { 0 },
};
// .rodata float table (read cross-TU by CMenuSymbolMark as f32s).
extern "C" __attribute__((used, aligned(8)))
const float lbl_eu_804FE6D8[6] = { -0.2f, 1.81f, -260.26f, -0.18f, 1.76f, -338.3f };

// .data float table (retail lbl_eu_8052CD38).
extern "C" __attribute__((used, aligned(4)))
float lbl_eu_8052CD38[6] = { 1.0f, 1.0f, 1.0f, 0.9f, 0.7f, 0.9f };
