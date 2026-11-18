// Auto-scaffolded catalog TU for kyoshin/CTutorial
// Replace stubs with high-level C/C++ during decomp.

#define IWORK_EVENT_INLINE_DTOR
#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/CTutorial.hpp"


#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceFont.hpp"
#include "monolib/lib/CLibLayout.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/device/CFileHandle.hpp"
#include <nw4r/lyt.h>

#include <functions.hpp>
#include "kyoshin/CTutorialImports.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"

// Tex palette binder (const char* pane name; code_80135FDC.hpp types it void*).

// (Layout teardown goes through `delete`, whose deleting-dtor vcall
// matches retail. Font dispatch goes through IDeviceFontInfo::getFont.)

// Constructor: install the IWorkEvent vtable, construct the two memory
// regions (in declaration order), null every pointer, then set the state
// bytes - 0x47 defaults to visible; the two ctor args land at 0x48 (locale)
// and 0x53 (region/flags).
CTutorial::CTutorial(u8 param_1, u8 param_2) {
    mFileHandle0 = nullptr;
    mFileHandle1 = nullptr;
    mFileHandle2 = nullptr;
    mAccessor0 = nullptr;
    mAccessor1 = nullptr;
    mpLayout = nullptr;
    mpAnimTrans0 = nullptr;
    mpAnimTrans1 = nullptr;
    field_44 = 0;
    field_45 = 0;
    field_46 = 0;
    field_47 = 1;
    field_48 = param_1;
    field_4C = nullptr;
    field_50 = 0;
    field_51 = 0;
    field_52 = 0;
    field_53 = param_2;
}

// Compiler-generated complete-object dtor: destroy the two memory regions in
// reverse declaration order, then free the object when the delete flag is set.
CTutorial::~CTutorial() {}

// Load the three tutorial data files: the layout arc, then (when the region
// gate 0x53 is set) a locale-specific data file whose path is sprintf-ed, and
// finally a game data file whose path depends on the CfGameManager flag.
void CTutorial::Tutorial_LoadTutorialFiles() {
    char buf[0x40];
    mFileHandle0 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), "menu/jp/Tutorial.arc",
        reinterpret_cast<IWorkEvent*>(this), 0, 0);
    if (field_53 != 0) {
        // Format string has no conversions - retail passes no varargs here
        // (the stale r5 in the retail asm is leftover from the previous call).
        sprintf(buf, "/common/jp/bdat_menu_ttrl.bin");
        mFileHandle1 = CDeviceFile::readFile(
            mtl::MemManager::getHandleMEM2(), buf,
            reinterpret_cast<IWorkEvent*>(this), 0, 0);
    }
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        sprintf(buf, "menu/jp/tpl/c_ttrl_%03d.arc", field_48);
    } else {
        sprintf(buf, "menu/jp/tpl/r_ttrl_%03d.arc", field_48);
    }
    mFileHandle2 = CDeviceFile::readFile(
        mtl::MemManager::getHandleMEM2(), buf,
        reinterpret_cast<IWorkEvent*>(this), 0, 0);
    field_46 = 0;
}

void CTutorial::Tutorial_Move() {
    if (field_44 == 0) {
        return;
    }
    switch (field_45) {
    case 1:
        Tutorial_StartAdvanceAnim();
        break;
    case 2:
        Tutorial_FinishAdvanceAnim();
        break;
    case 4:
        Tutorial_StartRewindAnim();
        break;
    case 5:
        Tutorial_FinishRewindAnim();
        break;
    default:
        break;
    }
    mpLayout->Animate(0);
}

/* Draw the tutorial layout when the active gate (0x44) is set. r4 (the draw
 * info) is passed straight through to the layout draw helper. */
void CTutorial::Tutorial_DrawLayoutGated(nw4r::lyt::DrawInfo* drawInfo) {
    if (field_44 != 0) {
        drawLayout(mpLayout, drawInfo, 0, 1);
    }
}

// Unload: wait for draw done, release BDAT index 4 when the region gate is
// clear, close the three file handles, destroy + clear the layout, release
// both accessors, free the data buffer, clear the shared BDAT pointer, then
// tear down both memory regions in declaration order.
void CTutorial::Tutorial_TeardownTutorial() {
    CDeviceVI::waitForDrawDone();
    if (field_53 != 0) {
        CBdat::getEntry(4);
    }
    closeFileHandle(&mFileHandle0);
    closeFileHandle(&mFileHandle1);
    closeFileHandle(&mFileHandle2);
    field_44 = 0;
    nw4r::lyt::Layout* layout = mpLayout;
    if (layout != nullptr) {
        delete layout;
        mpLayout = nullptr;
    }
    releaseArcResourceAccessor(mAccessor0);
    mAccessor0 = nullptr;
    releaseArcResourceAccessor(mAccessor1);
    mAccessor1 = nullptr;
    if (field_4C != nullptr) {
        mtl::MemManager::deallocate(field_4C);
        field_4C = nullptr;
    }
    lbl_eu_80664A30 = 0;
    mRegion0.deleteRegion();
    mRegion1.deleteRegion();
}

bool CTutorial::Tutorial_GetField46Mark() { return this->field_46; }
bool CTutorial::Tutorial_GetField47Mark() { return this->field_47; }
bool CTutorial::Tutorial_GetField52Mark() { return this->field_52; }

/* Idempotent start: if not already running, mark state 1 and fire the 0x8
 * UI sound/effect event. */
void CTutorial::Tutorial_StartIfIdle() {
    if (field_45 == 0) {
        field_45 = 1;
        field_47 = 0;
        playUISound(8);
    }
}

/* Advance-anim fully finished: state 4, hide, swap back to anim0 (rewind
 * transform) and re-tick the layout, then play the completion sound. */
void CTutorial::Tutorial_AdvanceState3To4() {
    if (field_45 == 3) {
        field_45 = 4;
        field_47 = 0;
        mpLayout->SetAnimationEnable(mpAnimTrans1, false);
        mpLayout->SetAnimationEnable(mpAnimTrans0, true);
        mpLayout->Animate(0);
        playUISound(9);
    }
}

// Page-navigation helper: mark complete when the counter already sits on the
// last page, else advance it clamped to bound-1, then refresh and play the
// confirm sound.
void CTutorial::Tutorial_NextPage() {
    if ((s8)field_50 == (s8)field_51 - 1) {
        field_52 = 1;
        return;
    }
    field_50 = field_50 + 1;
    if ((s8)(u8)(field_50) >= (s8)field_51) {
        field_50 = field_51 - 1;
    }
    Tutorial_RefreshPage();
    playUISound(8);
}

// Page-counter tick: play the confirm sound while the counter is nonzero,
// decrement it (u8-domain), clamp negatives to zero, then refresh.
void CTutorial::Tutorial_PrevPage() {
    if (field_50 != 0)
        playUISound(8);
    u8 next = (u8)field_50 - 1;
    field_50 = (s8)next;
    if ((s8)next < 0)
        field_50 = 0;
    Tutorial_RefreshPage();
}

u8 CTutorial::Tutorial_GetPageState() {
    s8 last = field_51;
    s8 cur = field_50;
    if (cur == last - 1) {
        if (last == 1) {
            return 2;
        }
        return 3;
    }
    return cur != 0;
}

// Advance-anim start: disable anim0 / enable anim1, mark state 2, and re-bind
// both transforms onto the layout before kicking an animation tick.
void CTutorial::Tutorial_StartAdvanceAnim() {
    if (advanceAnimTransform(mpAnimTrans0, 1.0f) != 0) {
        field_45 = 2;
        mpLayout->SetAnimationEnable(mpAnimTrans0, false);
        mpLayout->SetAnimationEnable(mpAnimTrans1, true);
        mpLayout->Animate(0);
    }
}

/* Advance-animation (0x40) reached the end frame: state 3, visible. */
__declspec(noinline) void CTutorial::Tutorial_FinishAdvanceAnim() {
    if (advanceAnimTransform(mpAnimTrans1, 1.0f) != 0) {
        field_45 = 3;
        field_47 = 1;
    }
}

// Rewind-anim start: mirror of Tutorial_StartAdvanceAnim with the transforms swapped.
void CTutorial::Tutorial_StartRewindAnim() {
    if (AnimRewindFrame(mpAnimTrans1, 1.0f) != 0) {
        field_45 = 5;
        mpLayout->SetAnimationEnable(mpAnimTrans1, false);
        mpLayout->SetAnimationEnable(mpAnimTrans0, true);
        mpLayout->Animate(0);
    }
}

/* Rewind-animation (0x3C) reached the start frame: state 0, visible. */
__declspec(noinline) void CTutorial::Tutorial_FinishRewindAnim() {
    if (AnimRewindFrame(mpAnimTrans0, 1.0f) != 0) {
        field_45 = 0;
        field_47 = 1;
    }
}

// Tutorial data reload: validate the loaded resources, resolve the BDAT
// text-table pointer when the region gate (0x53) is clear, then reset the
// page counters and refresh.
void CTutorial::Tutorial_ReloadTutorialData() {
    if (field_53 != 0) {
        if (mAccessor0 == nullptr || field_4C == nullptr ||
            mAccessor1 == nullptr)
            return;
    } else {
        if (mAccessor0 == nullptr || mAccessor1 == nullptr)
            return;
        // Resolve the tutorial BDAT table handle by its pooled tag string.
        Bdat_GetTable_AA34();
        lbl_eu_80664A30 = (u32)getFP("MNU_ttrl");
    }
    field_46 = 1;
    field_44 = 1;
    field_50 = 0;
    field_51 =
        (s8)(u8)BdatGetU8Direct((const void*)lbl_eu_80664A30, (const char*)("page"), field_48);
    Tutorial_RefreshPage();
}

/* Refresh the tutorial page: set both text panes from the current/bound page
 * counters, sprintf the locale-specific texture name, fetch it from the
 * loaded game-data archive (mAccessor1)
 * and bind it onto the layout. */
void CTutorial::Tutorial_RefreshPage() {
    char buf[0x20];
    // Sign-extend before increment: retail computes extsb(field_50)+1.
    setLayoutTextBoxNumber(mpLayout, "txt_pg00", field_50 + 1);
    setLayoutTextBoxNumber(mpLayout, "txt_pg01", field_51);
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        sprintf(buf, "cttrl_i%03d_%02d.tpl", field_48, field_50 + 1);
    } else {
        sprintf(buf, "rttrl_i%03d_%02d.tpl", field_48, field_50 + 1);
    }
    void* tex = mAccessor1->GetResource(0x74696d67, buf, 0);
    if (tex != NULL) {
        PaneSetTexPaletteByName(mpLayout, (const char*)("pic_cap"), (TPLPalette*)tex);
    }
}

// Font object returned by CDeviceFont::getFontInfo dispatches through
// IDeviceFontInfo::getFont (vtable slot 9, +0x24).

/* File-completion dispatcher: whichever of the three requested files arrived
 * determines the branch. Handle 0 (layout arc) builds the whole layout stack;
 * handle 1 (locale data) registers the BDAT table; handle 2 (game data)
 * attaches its archive then refreshes. Returns whether the event was ours. */
bool CTutorial::OnFileEvent(CEventFile* pEventFile) {
    if (mFileHandle0 == pEventFile->mFileHandle) {
        // Scratch heap region (RAII Class_8045F858 guard).
        mRegion0.createRegion(mtl::MemManager::getHandleMEM2(), 0x1800,
                              "CTutorial", 0);
        Class_8045F858 regionHost(&mRegion0);

        // Take ownership of the loaded arc buffer.
        void* arcData = mFileHandle0->getData();
        mtl::MemManager::setMemInitFlag(false);

        mAccessor0 = CLibLayout::createArcResourceAccessor();
        mAccessor0->Attach(arcData, "arc");

        buildLayout(&mpLayout, mAccessor0, "mf85_opt01_tut01.brlyt");
        bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mAccessor0,
                      "mf85_opt01_tut01_in.brlan");
        bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mAccessor0,
                      "mf85_opt01_tut01_info_in.brlan");

        // Bind the shared font onto the root pane (retail loads the root pane
        // before fetching the font handle from vtable slot 9).
        nw4r::lyt::Pane* rootPane = mpLayout->GetRootPane();
        u32 fontResult = (u32)static_cast<IDeviceFontInfo*>(
                             CDeviceFont::getFontInfo(1, mpLayout))->getFont();
        ApplyFontToTxtPanes(rootPane, (const nw4r::ut::Font*)fontResult);

        u32 lang = (u32)getPackedFont();
        if (lang != 0) {
            setLayoutTextBoxFont(mpLayout, "txt_pg00", lang);
            setLayoutTextBoxFont(mpLayout, "txt_pg01", lang);
        }

        // Start paused on the rewind transform (anim0 enabled, anim1 not).
        mpLayout->SetAnimationEnable(mpAnimTrans1, false);
        mpLayout->SetAnimationEnable(mpAnimTrans0, true);
        mpLayout->Animate(0);

        mFileHandle0 = nullptr;
        mRegion0.validateHeap();
        return true;
    }
    if (mFileHandle1 == pEventFile->mFileHandle) {
        // Locale data: hand the buffer to the BDAT system, resolve the shared
        // tutorial table, then refresh the page state.
        void* data = mFileHandle1->mData;
        mFileHandle1->mData = nullptr;
        field_4C = (u8*)data;
        CBdat::setBdatEntry(4, data);
        Bdat_GetTable_AA34();
        lbl_eu_80664A30 = (u32)getFP("MNU_ttrl");
        Tutorial_ReloadTutorialData();
        mFileHandle1 = nullptr;
        return true;
    }
    if (mFileHandle2 == pEventFile->mFileHandle) {
        // Game data archive: attach to a fresh accessor, then refresh.
        mRegion1.createRegion(mtl::MemManager::getHandleMEM2(), 0x1000,
                              "CTutorialTex", 0);
        Class_8045F858 regionHost(&mRegion1);

        void* arcData = mFileHandle2->getData();
        mtl::MemManager::setMemInitFlag(false);

        mAccessor1 = CLibLayout::createArcResourceAccessor();
        mAccessor1->Attach(arcData, "arc");

        Tutorial_ReloadTutorialData();
        mFileHandle2 = nullptr;
        mRegion1.validateHeap();
        return true;
    }
    return false;
}

// Reset a block of UI flags (0x3340..0x33BE) owned by this tutorial, then set
// the owner id 0x270 entry to a heap/direct-address classification mask.
#pragma optimize_for_size off
void CTutorial::Tutorial_ResetSharedFlags() {
    s16 i = 0x3340;
    while (i < 0x33bf) {
        CtrlRemote_SetSharedBit(i, (u32)this);
        i++;
    }
    // (-a | a) >> 31 (arithmetic) is -1 for any nonzero address, 0 for null;
    // masked to the 0xFE window.
    s32 addr = (s32)this;
    CtrlRemote_SetSharedBit(0x270, ((-addr | addr) >> 31) & 0xfe);
}
#pragma optimize_for_size reset
