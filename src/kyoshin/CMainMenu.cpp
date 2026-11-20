#define IWORK_EVENT_INLINE_DTOR
// Auto-scaffolded catalog TU for kyoshin/CMainMenu
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/CSystemWindowApi.hpp"
#include "kyoshin/CUICfManager.hpp"
#include <types.h>
#include "monolib/lib/CLibLayout.hpp"
#include <string.h>
#include "kyoshin/CMainMenu.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/CUIWindowManagerApi.hpp"
#include "kyoshin/makecrystal/CMenuMakeCrystal.hpp"

// Minimal layout-identical view of CArtsInfo (only field_0x54 is used here).
// Full definition in kyoshin/CArtsInfo.hpp, which cannot be included from
// this TU (extern "C" BdatGetU16ByTableKey signature clash with code_80135FDC.hpp).
class CArtsInfo {
public:
    u8 _00[0x54];
    u8 field_0x54;
};

// Mirror of the font object returned by CDeviceFont::getFontInfo; vtable
// slot 9 (offset 0x24) yields the pane bound by ApplyFontToTxtPanes.
struct CMainMenuFontView {
    virtual void sf2() = 0;
    virtual void sf3() = 0;
    virtual void sf4() = 0;
    virtual void sf5() = 0;
    virtual void sf6() = 0;
    virtual void sf7() = 0;
    virtual void sf8() = 0;
    virtual u32 sf9() = 0; // vtable offset 0x24
};


// Gameplay-input gate for the menu-frame dispatch (defined in this TU).
// Takes the owner like retail (the caller emits mr r3,r31 before the call).
extern "C" int CMainMenu_IsInputBlocked(CMainMenu* self);

// Per-frame menu handlers (defined in this TU). extern "C" so every
// referencing reloc carries the exact retail (unmangled) symbol.
extern "C" void CMainMenu_RefreshFlags(CMainMenu* self);
extern "C" void CMainMenu_DispatchState(CMainMenu* self);
extern "C" void CMainMenu_HandleSubInput(CMainMenu* self);
extern "C" void CMainMenu_AdvanceIdle(CMainMenu* self);
extern "C" void CMainMenu_DispatchMenuState(CMainMenu* self);

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" char lbl_eu_804FCEBC[];              // rodata: menu resource names

// Menu singleton/state guards (defined in their owning menu TUs)
// Defined in CMenuItem.cpp with C++ linkage.
unsigned long ItemMenu_IsPresent();
extern "C" u32 isMapSelectActive();   // map-select menu active (CMenuMapSelect.cpp)
extern "C" u32 Colle_HasInstance();   // collepedia active (CMenuCollepedia.cpp)
extern "C" u32 KizunagramIsCreated();   // kizunagram active (CMenuKizunagram.cpp)
extern "C" u32 PlayAward_IsActive();   // play-award active (CMenuPlayAward.cpp)
extern "C" u32 KizunaList_IsPresent();   // kizuna-talk-list active (CMenuKizunaTalkList.cpp)
extern "C" u32 hasOptionMenu();   // option menu active (CMenuOption.cpp)
extern "C" u32 CMenuTutorialList_IsActive();   // tutorial-list active (CMenuTutorialList.cpp)
extern "C" u32 isSaveMenuActive();   // save-menu active (CMenuSave.cpp)
extern "C" u32 SkipTimer_IsActiveFlag();   // update menu active (CMenuUpdate.cpp)
extern "C" int CUICfManager_hasInUseSlot();   // menu-system close (CUICfManager.cpp)
extern "C" u32 menuPTStateIsActive();   // party-state screen active (CMenuPTState.cpp)
extern "C" u32 CMenuArtsSet_isCreated();   // arts-set menu active (CMenuArtsSet.cpp)

// Gameplay-input gate helpers (cf::CfGameManager / cf::CBattleManager)
extern "C" int isSceneLoading__Q22cf13CfGameManagerFv();
extern "C" int isAnyFieldFlagSet__Q22cf13CfGameManagerFv();

// split1 .sdata2 float constants (int->float magic f64 at 0x80666F10,
// then two 0.0f)
extern f64 lbl_eu_80666F10;   // 0x80666F10 - s32->f32 conversion constant (0x4330000080000000)
extern f32 lbl_eu_80666F18;   // 0x80666F18 - anim frame time (0.0f)
extern f32 lbl_eu_80666F1C;   // 0x80666F1C - HP death threshold (0.0f)

// Presentation/mode bitfield (sibling of gmGameFlags; split1 .sbss).
extern u32 gmPauseFlags;

// Resource/flag getter (resource id -> value; defined in CMiniMap.cpp).
#include "kyoshin/cf/CtrlRemoteApi.hpp"

// UnkClass_8045F564 embedded-object lifecycle (defined in split1; normally
// declared via kyoshin/CArtsInfo.hpp, which this TU cannot include).

// Mangled-identifier call forms previously supplied by CArtsInfo.hpp.

// Cursor constructors (defined in kyoshin/CCur.cpp)
// Font binding imports (CDeviceFont / CLibLayout)
extern "C" void* getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(u32, nw4r::lyt::Layout*);

// Menu pane-position/color helper (defined in code_80135FDC.cpp).
// Menu open/close gate (defined in code_80135FDC.cpp).
// System-window busy gate (defined in code_80135FDC.cpp).
// Another menu-open gate (kizuna-talk-list / message-log family).
extern "C" u32 hasPassiveSkillMenu();
// Message-log busy gate (CSysWinMsgLog.cpp).
extern "C" u32 SysWinLog_IsBusy();
// System window buffer singleton gate (CSysWinBuff.cpp).
class CSysWinBuff;
extern "C" CSysWinBuff* getInstance__11CSysWinBuffFv();
// Window focus gates (CUICfManager.cpp).
// Pad-enable/disable and mode gates (cf::CfGameManager).
// setPresentationFlag: canonical setPresentationFlag__Q22cf13CfGameManagerFv(bool).
#include "kyoshin/cf/CfGameManager.hpp"

// cf::CPad view: only the three flag words this unit reads.
struct CMainMenuPad {
    u32 mHeldButtonFlags;       // 0x00
    u32 mPressedButtonFlags;    // 0x04
    u32 mTurboPressButtonFlags; // 0x08
};
extern "C" CMainMenuPad* getCurrentPad__Q22cf13CfGameManagerFv();

// Cursor activate helper (defined in CCur.cpp).
// Cursor draw helper (defined in CCur.cpp; retail symbol is unmangled C linkage).

#include "kyoshin/CTaskGame.hpp"
// Cursor per-frame update helper (defined in CCur.cpp).
// Pane-config finish gate (defined in CUICfManager.cpp).

// Sub-menu pane-name table (14 words) and cursor->angle s16 table.
struct CMainMenuNameTable {
    const char* name[14];
};
extern u32 lbl_eu_804FCE50[];
extern s16 lbl_eu_804FCD60[];
// CBaseCur runtime vtable (stored over the stack temp after construction).

// Constructor data symbols (see __ct__CMainMenu in CMainMenu.cpp).
// CProcess base constructor (abstract class, out-of-line in retail).

// Player view structs for CMainMenu_IsInputBlocked (mirror CfGimmick.hpp layouts).
// getPlayer returns the +0x3E9C embedded spot object; the player base is the
// de-biased pointer (base = spot - 0x3E9C).
struct CMainMenuPlayerSub {
    u8 _00[0x3B4];
    u8 field_0x3B4[0x4EC - 0x3B4];  // 0x3B4 - sub-object passed to the spot vfn
    u32 field_4EC;              // 0x4EC - state flags
    u8 _4F0[0x530 - 0x4F0];
    u16 field_530;              // 0x530 - action flags
};

struct CMainMenuPlayerSpot {
    void** vtable;              // 0x00 - embedded +0x3E9C sub-object
    u8 _04[0xC4 - 0x04];
    CMainMenuPlayerSub* field_0xC4;   // 0xC4 - battle-state sub-object
};
struct CMainMenuPlayer {
    void** vtable;              // 0x00 - slot 0x128 yields HP (float)
    u8 _04[0x3E9C - 0x04];
    CMainMenuPlayerSpot spot;   // 0x3E9C (0xC8 bytes; field_0xC4 == player+0x3F60)
};
extern "C" CMainMenuPlayerSpot* getPlayer__Q22cf13CfGameManagerFi(int index);

// Typed head views for the raw back-adjust thunks/recoveries in CMainMenu.cpp:
// the +0x58/+0x5C sub-objects and the +0x3E9C player spot are addressed
// through sized byte-head pointer arithmetic ((Head*)p - 1) instead of raw
// (char*)-offset. Layout matches CMainMenu / CMainMenuPlayer exactly.
struct CMainMenuPlayerHead {
    u8 head_00[0x3E9C];              // bytes preceding CMainMenuPlayer::spot
};

// cf::CBattleManager battle-list view (sentinel node at +0x8, nodes at +0x0).
struct CMainMenuBattleNode {
    CMainMenuBattleNode* next;  // 0x00
};
struct CMainMenuBattleMgr {
    u8 _0[0x8];
    CMainMenuBattleNode* list;  // 0x8 - sentinel node
};
#include "kyoshin/cf/CBattleManager.hpp"

#include "kyoshin/code_801F3BE0.hpp"

// CSubCur view at +0xA8 (CBaseCur layout, 0x16 bytes). Cast-only iface for
// the vtable+0x10 dispatch (setRootPaneTranslate, takes a translate VEC3).

// Menu dispatch handlers (defined in CUICfManager.cpp / menu TUs)
extern "C" void CUICfManager_queueSkipTimerMenu();
extern "C" void CUICfManager_openSaveMenu();
#include "kyoshin/IUICf.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "monolib/device/CDeviceFile.hpp"
#include "monolib/device/CDeviceVI.hpp"          // waitForDrawDone
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CTTask.hpp"
#include "monolib/work/IWorkEvent.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/work/CEventFile.hpp"
#include "monolib/lib/UnkClass_8045F564.hpp"
#include <nw4r/lyt/lyt_layout.h>
#include <nw4r/lyt/lyt_pane.h>
#include <nw4r/math/math_types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)
// Real player base/spot types are in CfGimmick.hpp (CfPlayerBase::getHP at 0x128,
// CfPlayerSpot::partyOpen at 0x9C). Define minimal forward views here to avoid
// pulling the full CfGimmick header which conflicts on several extern "C" globals
// (CtrlRemote_TouchBitByArg/SysWinGetSingleton) already declared with different
// view structs in the menu headers.
class CfPlayerSpotReal {
public:
    virtual ~CfPlayerSpotReal();
    virtual void d01(); virtual void d02(); virtual void d03(); virtual void d04();
    virtual void d05(); virtual void d06(); virtual void d07(); virtual void d08();
    virtual void d09(); virtual void d10(); virtual void d11(); virtual void d12();
    virtual void d13(); virtual void d14(); virtual void d15(); virtual void d16();
    virtual void d17(); virtual void d18(); virtual void d19(); virtual void d20();
    virtual void d21(); virtual void d22(); virtual void d23(); virtual void d24();
    virtual void d25(); virtual void d26(); virtual void d27(); virtual void d28();
    virtual void d29(); virtual void d30(); virtual void d31(); virtual void d32();
    virtual void d33(); virtual void d34(); virtual void d35(); virtual void d36();
    virtual void partyOpen(u8* arg); // 0x9C
};
class CfPlayerBaseReal {
public:
    virtual ~CfPlayerBaseReal();
    virtual void d01(); virtual void d02(); virtual void d03(); virtual void d04();
    virtual void d05(); virtual void d06(); virtual void d07(); virtual void d08();
    virtual void d09(); virtual void d10(); virtual void d11(); virtual void d12();
    virtual void d13(); virtual void d14(); virtual void d15(); virtual void d16();
    virtual void d17(); virtual void d18(); virtual void d19(); virtual void d20();
    virtual void d21(); virtual void d22(); virtual void d23(); virtual void d24();
    virtual void d25(); virtual void d26(); virtual void d27(); virtual void d28();
    virtual void d29(); virtual void d30(); virtual void d31(); virtual void d32();
    virtual void d33(); virtual void d34(); virtual void d35(); virtual void d36();
    virtual void d37(); virtual void d38(); virtual void d39(); virtual void d40();
    virtual void d41(); virtual void d42(); virtual void d43(); virtual void d44();
    virtual void d45(); virtual void d46(); virtual void d47(); virtual void d48();
    virtual void d49(); virtual void d50(); virtual void d51(); virtual void d52();
    virtual void d53(); virtual void d54(); virtual void d55(); virtual void d56();
    virtual void d57(); virtual void d58(); virtual void d59(); virtual void d60();
    virtual void d61(); virtual void d62(); virtual void d63(); virtual void d64();
    virtual void d65(); virtual void d66(); virtual void d67(); virtual void d68();
    virtual void d69(); virtual void d70(); virtual void d71();
    virtual float getHP(); // 0x128
};

// C-linkage import (retail unmangled symbol).

// CBaseCur shared helpers (defined in CCur.cpp)

CMainCur::~CMainCur() {}

void CMainCur::initLayout() {
    buildLayout(&mpLayout, mArcResAcc, "mf01_main21_cursor.brlyt");
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans0, mArcResAcc, "mf01_main21_cursor_roop.brlan");
    bindLayoutAnimTransform(mpLayout, &mpAnimTrans1, mArcResAcc, "mf01_main21_cursor_on.brlan");
    mpLayout->UnbindAllAnimation();
    ((CBaseCur*)(this))->DeactivateTail();
}

// Moves the "Param" pane to the cursor position.
void CMainCur::setRootPaneTranslate(const nw4r::math::VEC3* pos) {
    nw4r::lyt::Pane* pane = mpLayout->GetRootPane()->FindPaneByName("txt_cursor", true);
    if (pane != NULL) {
        pane->SetSRTElement(0, pos->x);
        pane->SetSRTElement(1, pos->y);
        pane->SetSRTElement(2, pos->z);
    }
}

CMainMenu::CMainMenu(CScn* scene)
    : field_0x70(scene),
      field_0x74(nullptr),
      field_0x78(nullptr),
      field_0x7C(nullptr),
      field_0x80(nullptr),
      field_0x84(nullptr),
      field_0x88(nullptr),
      field_0x8C(nullptr),
      mMainCur(nullptr),
      mSubCur(nullptr),
      field_0xC0(3),
      field_0xC4(0),
      field_0xE0(0) {
    mMoveFunc = static_cast<MoveFunc>(&CMainMenu::moveWaitLoad);
}

CMainMenu::~CMainMenu() {}

CMainMenu* CMainMenu_GetInstancePtr() {
    return CMainMenu::sInstance;
}

static const char* sArcName = "menu/jp/MainMenu.arc";

// Requests the menu layout archive; OnFileEvent runs when it has loaded.
void CMainMenu::Init() {
    field_0x74 = CDeviceFile::readFile(mtl::MemManager::getHandleMEM2(), sArcName, this, 0, 0);
}

// Waits for the VI draw to complete, drops the render callback and the file
// handle, shuts both cursors down, deletes the layout, releases the arc
// accessor and the memory region, then clears the singleton.
void CMainMenu::Term() {
    CDeviceVI::waitForDrawDone();
    field_0x70->removeRenderCB(this);
    closeFileHandle(&field_0x74);
    mMainCur.cleanup();
    mSubCur.cleanup();
    if (field_0x7C != 0) {
        delete field_0x7C;
        field_0x7C = 0;
    }
    releaseArcResourceAccessor(field_0x78);
    field_0x78 = 0;
    mMemRegion.deleteRegion();
    sInstance = nullptr;
}

static const s16 sCursorPosX[] = {-192, -128, -64, 0, 64, 128, 192};

// First move function: wait until the layout archive has been consumed by
// OnFileEvent, then switch to the main update.
void CMainMenu::moveWaitLoad() {
    if (field_0x74 != nullptr) {
        return;
    }
    mMoveFunc = static_cast<MoveFunc>(&CMainMenu::moveMain);
}

void CMainMenu::moveMain() {
    CMainMenu* self = this;
    // Main-menu frame update: gate on the system window, refresh the menu
    // availability flags, then dispatch on the current state.
    if (IsMenuState621F0() == 0) {
        return;
    }
    if (gmGameFlags & 0xAFE40000) {
        self->mField54 = 1;
        return;
    }
    if (SysWinLog_IsBusy() != 0) return;
    if (getInstance__11CSysWinBuffFv() != 0) return;
    if (menuPTStateIsActive() != 0) return;
    if (ItemMenu_IsPresent() != 0) return;
    if (isQuestLogMenuActive() != 0) return;
    if (MakeCrystalIsCreated() != 0) return;
    if (CMenuArtsSet_isCreated() != 0) return;
    if (isMapSelectActive() != 0) return;
    if (Colle_HasInstance() != 0) return;
    if (KizunagramIsCreated() != 0) return;
    if (hasPassiveSkillMenu() != 0) return;
    if (PlayAward_IsActive() != 0) return;
    if (KizunaList_IsPresent() != 0) return;
    if (isSaveMenuActive() != 0) return;
    if (hasOptionMenu() != 0) return;
    if (SkipTimer_IsActiveFlag() != 0) return;
    if (CMenuTutorialList_IsActive() != 0) return;

    // Refresh cursor/sub-menu availability flags.
    CMainMenu_RefreshFlags(self);

    switch ((u32)self->field_0xE0) {
    case 0:
        // Enter: open sound + cursor-select state.
        playUISound(8);
        self->field_0xE0 = 1;
        break;
    case 1:
        // Cursor-select: once the intro animation finishes, set the current
        // cursor number (or its placeholder) and activate the cursor.
        if (advanceAnimTransform(self->field_0x80, 1.0f) == 0) {
            break;
        }
        // Strings hoisted above the branch like retail; MWCC colors the
        // temps across both arms.
        {
            char* s1;
            char* s2;
            int n = self->field_0xC0 + 1;
            if (n > 0) {
                s1 = BdatTouchStringCell("MNU_main", "name", n);
                s2 = BdatTouchStringCell("MNU_main", "help", n);
                LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", s1, 0);
                LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", s2, 0);
            } else {
                LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject",
                              "", 0);
                LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess",
                              "", 0);
            }
            self->mMainCur.SetVisible(1);
            self->field_0xE0 = 2;
        }
        break;
    case 2:
        // Cursor-active: run the input handler for the selected menu.
        CMainMenu_DispatchState(self);
        break;
    case 3:
        // Exit: once the close animation finishes, mark the menu idle.
        if (advanceAnimTransform(self->field_0x84, 1.0f) == 0) {
            break;
        }
        self->mField54 = 1;
        self->field_0xE0 = 0;
        break;
    case 4:
        // Screen-open dispatch: act on the cursor while no other window is up.
        if (CMainMenu_IsInputBlocked(self) == 0 && MenuTutorialIsCreated() == 0) {
            switch (self->field_0xC0) {
            case 0:
                CUICfManager_queueMapSelectMenu();
                break;
            case 3:
                CUICfManager_queuePTStateMenu();
                break;
            }
        }
        self->field_0xE0 = 2;
        break;
    case 5:
        CMainMenu_AdvanceIdle(self);
        break;
    case 6:
        CMainMenu_HandleSubInput(self);
        break;
    case 7:
        // Cursor-exit: like case 1 but driven by the exit animation.
        if (advanceAnimTransform(self->field_0x8C, 1.0f) == 0) {
            break;
        }
        // Strings hoisted above the branch like case 1.
        {
            char* s1;
            char* s2;
            int n = self->field_0xC0 + 1;
            if (n > 0) {
                s1 = BdatTouchStringCell("MNU_main", "name", n);
                s2 = BdatTouchStringCell("MNU_main", "help", n);
                LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", s1, 0);
                LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", s2, 0);
            } else {
                LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject",
                              "", 0);
                LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess",
                              "", 0);
            }
            self->field_0xE0 = 2;
        }
        break;
    case 8:
        CMainMenu_DispatchMenuState(self);
        break;
    }

    // Epilogue: animate the layout and advance both cursors.
    self->field_0x7C->Animate(0);
    self->mMainCur.Update();
    self->mSubCur.Update();
}

// File-event handler: fires when the menu
// layout arc finished loading. Same shape as COption::OnFileEvent: open a
// scratch heap region (RAII Class_8045F858 guard), detach the file handle,
// attach it to a fresh ArcResourceAccessor, build the layout + four
// animations, bind the device font, reset text panes and animation state,
// then build both cursors on the stack and copy them into their slots.
bool CMainMenu::OnFileEvent(CEventFile* pEventFile) {
    CMainMenu* self = this;
    if (self->field_0x74 == pEventFile->mFileHandle) {
        self->mMemRegion.createRegion(
            mtl::MemManager::getHandleMEM2(), 0xCC00, "CMainMenu", 0);
        Class_8045F858 memHost(&self->mMemRegion);

        // Take ownership of the loaded file data (getData clears the handle's
        // data pointer) and release the loader's lock.
        void* fileData = self->field_0x74->getData();
        mtl::MemManager::setMemInitFlag(false);

    self->field_0x78 = CLibLayout::createArcResourceAccessor();
            self->field_0x78->Attach(fileData, "arc");
            buildLayout(&self->field_0x7C, self->field_0x78, "mf01_main00_icon.brlyt");
            bindLayoutAnimTransform(self->field_0x7C, &self->field_0x80, self->field_0x78,
                          "mf01_main00_icon_in.brlan");
            bindLayoutAnimTransform(self->field_0x7C, &self->field_0x84, self->field_0x78,
                          "mf01_main00_icon_out.brlan");
            bindLayoutAnimTransform(self->field_0x7C, &self->field_0x88, self->field_0x78,
                          "mf01_main00_icon_sub_in.brlan");
            bindLayoutAnimTransform(self->field_0x7C, &self->field_0x8C, self->field_0x78,
                          "mf01_main00_icon_sub_out.brlan");

            // Bind the device font: ask the font object for its pane (vtable+0x24)
            // and push it onto the layout root.
            nw4r::lyt::Pane* rootPane = self->field_0x7C->GetRootPane();
            ApplyFontToTxtPanes(
                rootPane,
                (const nw4r::ut::Font*)(reinterpret_cast<CMainMenuFontView*>(
                    getFontInfo__11CDeviceFontFUlPQ34nw4r3lyt6Layout(1, self->field_0x7C))
                    ->sf9()));

            LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", "", 0);
            LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", "", 0);

            // Park all animations except the intro (anim0), which stays enabled.
            self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x80, true);
            self->field_0x80->SetFrame(0.0f);
            self->field_0x7C->Animate(0);

            mMainCur = CMainCur(field_0x78);
            mMainCur.initLayout();
            mSubCur = CSubCur(field_0x78);
            mSubCur.initLayout();

            CMainMenu_RefreshFlags(self);

            field_0x70->addRenderCB(this, 0xa, 0);

            self->field_0x74 = NULL;
            self->mMemRegion.validateHeap();
            return true;
    }
    return false;
}

// rodata block containing resource names:
// offset 0x00: brlyt name (e.g. "mf00_menu.brlyt")
// offset 0x19: brlan name (e.g. "mf00_menu_in.brlan")
// offset 0x37: brlan name (e.g. "mf00_menu_out.brlan")
// offset 0x53: pane name "Param"

// Render callback: draws the menu layout and its two embedded cursors.
// Gated on task-busy state, the global mode bitfield (bit 10), the
// system-window-active gate, and the presentation flag word.
void CMainMenu::cbRenderBefore() {
    // Single OR so MWCC emits the short-circuit branches: A -> bne exit,
    // B -> beq continue / b exit (same shape as CMiniMap::cbRenderBefore).
    if (CTaskGame::getInstance()->isFlag01Set() || (gmPauseFlags & 0x200000))
        return;
    if (!IsMenuState621F0())
        return;
    if (gmGameFlags & 0xAFE40000)
        return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Scope-exit dtor is auto-emitted as the retail direct call; an explicit
    // dtor call would emit a virtual dispatch.
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    drawLayout(field_0x7C, &drawInfo, 0, 1);
    mMainCur.DrawLayout(&drawInfo);
    mSubCur.DrawLayout(&drawInfo);
}

CMainMenu* CMainMenu::sInstance;

CMainMenu* CMainMenu::create(CProcess* parent, CScn* scene) {
    if (sInstance != nullptr) {
        return nullptr;
    }
    CMainMenu* menu = new (CWorkThreadSystem::getWorkMem()) CMainMenu(scene);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

// Returns 1 if isMenuOpen() is non-zero, else checks the singleton.
extern "C" int CMainMenu_IsOpen() {
    if (CMainMenu::isAnyMenuOpen() != 0) {
        return 1;
    }
    return CMainMenu::sInstance != nullptr ? 1 : 0;
}

int CMainMenu::isAnyMenuOpen() {
    // Menu-open gate: returns 1 while any menu screen is active (each callee
    // is a singleton/state guard), otherwise the tutorial-list state.
    if (menuPTStateIsActive()) return 1;
    if (ItemMenu_IsPresent()) return 1;
    if (isQuestLogMenuActive()) return 1;
    if (MakeCrystalIsCreated()) return 1;
    if (CMenuArtsSet_isCreated()) return 1;
    if (isMapSelectActive()) return 1;
    if (Colle_HasInstance()) return 1;
    if (KizunagramIsCreated()) return 1;
    if (hasPassiveSkillMenu()) return 1;
    if (PlayAward_IsActive()) return 1;
    if (KizunaList_IsPresent()) return 1;
    if (isSaveMenuActive()) return 1;
    if (hasOptionMenu()) return 1;
    if (SkipTimer_IsActiveFlag()) return 1;
    return CMenuTutorialList_IsActive();
}

// Returns 1 if isMenuOpen() is non-zero, else checks the global CMainMenu
// singleton's field_0xE0 for states 4 or 8 (active/invite).
extern "C" u32 CMainMenu_CheckOpenState() {
    if (CMainMenu::isAnyMenuOpen() != 0) {
        return 1;
    }
    CMainMenu* menu = CMainMenu::sInstance;
    if (menu == NULL) {
        return 0;
    }
    if (menu->field_0xE0 == 4) {
        return 1;
    }
    // cntlzw trick: returns 1 if field_0xE0 == 8, else 0
    return (menu->field_0xE0 == 8) ? 1 : 0;
}

extern "C" void ArtsInfo_SetReadyFlag(CArtsInfo* self) {
    self->field_0x54 = 1;
}

// Pane-flag setter used by the sub-menu open blocks: find the named pane
// through a fresh layout-root load (retail reloads it every lookup) and
// rewrite its enable bit (mFlag bit0). All lookups recurse except the last.
// Chained FindPaneByName→SetVisible so MWCC keeps the pane in r3 for the
// mFlag lbz/rlwinm/ori/stb sequence. Literal true/false arms constant-fold
// SetBit (false = rlwinm only; true adds ori 1).
#define MENU_SET_PANE_FLAG(self_, name_, on_, rec_)                              \
    do {                                                                         \
        if (on_) {                                                               \
            (*(nw4r::lyt::Pane**)((u8*)(self_)->field_0x7C + 0x10))              \
                ->FindPaneByName((name_), (rec_))                                \
                ->SetVisible(true);                                              \
        } else {                                                                 \
            (*(nw4r::lyt::Pane**)((u8*)(self_)->field_0x7C + 0x10))              \
                ->FindPaneByName((name_), (rec_))                                \
                ->SetVisible(false);                                             \
        }                                                                        \
    } while (0)

// Per-frame main-menu input handler (state 2): scroll the main cursor,
// open the selected sub-screen, or cancel back to the exit state.
void CMainMenu_DispatchState(CMainMenu* self) {
    if (CMainMenu_IsInputBlocked(self) != 0) {
        // Input locked: stop the open animations, run the close animation,
        // clear both number panes and drop to the exit state.
        self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, true);
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", "", 0);
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", "", 0);
        self->mMainCur.SetVisible(0);
        playUISound(9);
        self->field_0xE0 = 3;
        return;
    }
    if (MenuTutorialIsCreated() != 0) {
        return;
    }
    CMainMenuPad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
    // Held-button early-outs per input style.
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        if (pad->mHeldButtonFlags & 0x08000000) {
            return;
        }
    } else {
        if (pad->mHeldButtonFlags & 0x800) {
            return;
        }
    }

    // Join colors: r8=aPressed, r6=down, r7=up, r4=bPressed, r0=confirm.
    // Join-scope t,p with decl order bPressed,t,p,down,up,aPressed,confirm.
    u32 bPressed, t, p, down, up, aPressed, confirm;
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        p = pad->mPressedButtonFlags;
        t = pad->mTurboPressButtonFlags;
        u32 cHi = p & 0x400000;
        aPressed = (p >> 21) & 1;
        u32 cLo = p & 0x400;
        down = t & 1;
        confirm = (cHi | cLo) != 0;
        up = (t >> 1) & 1;
        bPressed = (p >> 26) & 1;
    } else {
        u32 t2 = pad->mTurboPressButtonFlags;
        u32 p2 = pad->mPressedButtonFlags;
        u32 c = p2 & 0x420;
        aPressed = (p2 >> 4) & 1;
        down = t2 & 1;
        confirm = c != 0;
        up = (t2 >> 1) & 1;
        bPressed = (p2 >> 12) & 1;
    }

    if (down != 0) {
        // Down: wrap the main cursor backward and reposition it.
        s32 idx = self->field_0xC0 - 1;
        self->field_0xC0 = idx;
        if (idx < 0) {
            self->field_0xC0 = 6;
        }
        nw4r::lyt::Pane* pane =
            self->mMainCur.mpLayout->GetRootPane()->FindPaneByName("txt_cursor", true);
        nw4r::math::VEC3 vec;
        if (pane != NULL) {
            vec = pane->GetTranslate();
        }
        vec.x = sCursorPosX[self->field_0xC0];
        self->mMainCur.setRootPaneTranslate(&vec);
        // Refresh the "N" counter panes for the new cursor index.
        int n = self->field_0xC0 + 1;
        if (n > 0) {
            char* s1 = BdatTouchStringCell("MNU_main", "name", n);
            char* s2 = BdatTouchStringCell("MNU_main", "help", n);
            LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", s1, 0);
            LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", s2, 0);
        } else {
            LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", "", 0);
            LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", "", 0);
        }
        playUISound(0x6a);
        return;
    }
    if (up != 0) {
        // Up: wrap the main cursor forward and reposition it.
        if (++self->field_0xC0 > 6) {
            self->field_0xC0 = 0;
        }
        nw4r::lyt::Pane* pane =
            self->mMainCur.mpLayout->GetRootPane()->FindPaneByName("txt_cursor", true);
        nw4r::math::VEC3 vec;
        if (pane != NULL) {
            vec = pane->GetTranslate();
        }
        vec.x = sCursorPosX[self->field_0xC0];
        self->mMainCur.setRootPaneTranslate(&vec);
        int n = self->field_0xC0 + 1;
        if (n > 0) {
            char* s1 = BdatTouchStringCell("MNU_main", "name", n);
            char* s2 = BdatTouchStringCell("MNU_main", "help", n);
            LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", s1, 0);
            LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", s2, 0);
        } else {
            LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", "", 0);
            LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", "", 0);
        }
        playUISound(0x6a);
        return;
    }
    // Retail: beq aPressed==0 → cancel/B tail; fallthrough = select path.
    if (aPressed != 0) {
        // Confirm: reject unavailable entries, otherwise open the selected menu.
        s32 sel = self->field_0xC0;
        if (self->field_0xC8[sel] != 0) {
            playUISound(5);
            return;
        }
        // Sub-menu open: reveal the four entries of the selected page (14 panes
        // total), swap to the sub-cursor open animation and park the state at 5.
        // Retail duplicates the anim tail inside every case, so each arm carries
        // its own copy here too.
        switch (sel) {
        case 0:
        case 3:
            // Header entries: activate the main cursor and enter screen-open state.
            self->mMainCur.SetActive();
            self->field_0xE0 = 4;
            playUISound(0x6b);
            return;
        case 1: {
            // Retail: first three entries on, rest off (0x16c is OFF).
            MENU_SET_PANE_FLAG(self, "pic_base02_01", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base02_02", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base02_03", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base03_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base03_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_03", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_04", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base06_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base06_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_03", 0, true);
            self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x88, true);
            self->field_0x88->SetFrame(0.0f);
            self->field_0xE0 = 5;
            playUISound(0x6c);
            return;
        }
        case 2: {
            // Retail: 0x16c+0x17a on (page-2 pair), rest off.
            MENU_SET_PANE_FLAG(self, "pic_base02_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base02_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base02_03", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base03_01", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base03_02", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_03", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_04", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base06_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base06_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_03", 0, true);
            self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x88, true);
            self->field_0x88->SetFrame(0.0f);
            self->field_0xE0 = 5;
            playUISound(0x6c);
            return;
        }
        case 4: {
            // Retail: 0x188..0x1b2 on (four), rest off.
            MENU_SET_PANE_FLAG(self, "pic_base02_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base02_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base02_03", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base03_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base03_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_01", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_02", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_03", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_04", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base06_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base06_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_03", 0, true);
            self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x88, true);
            self->field_0x88->SetFrame(0.0f);
            self->field_0xE0 = 5;
            playUISound(0x6c);
            return;
        }
        case 5: {
            // Retail: 0x1c0+0x1ce on, rest off.
            MENU_SET_PANE_FLAG(self, "pic_base02_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base02_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base02_03", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base03_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base03_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_03", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_04", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base06_01", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base06_02", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_03", 0, true);
            self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x88, true);
            self->field_0x88->SetFrame(0.0f);
            self->field_0xE0 = 5;
            playUISound(0x6c);
            return;
        }
        case 6: {
            // Retail: last three on (0x1dc..0x1f8); all FindPane recurse=true.
            MENU_SET_PANE_FLAG(self, "pic_base02_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base02_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base02_03", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base03_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base03_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_03", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base05_04", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base06_01", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base06_02", 0, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_01", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_02", 1, true);
            MENU_SET_PANE_FLAG(self, "pic_base07_03", 1, true);
            self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
            self->field_0x7C->SetAnimationEnable(self->field_0x88, true);
            self->field_0x88->SetFrame(0.0f);
            self->field_0xE0 = 5;
            playUISound(0x6c);
            return;
        }
        default:
            goto done;
        }
    }
    if (confirm != 0) {
        // Cancel: same tail as the input-lock branch above.
        self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, true);
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", "", 0);
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", "", 0);
        self->mMainCur.SetVisible(0);
        playUISound(9);
        self->field_0xE0 = 3;
        return;
    }
    if (bPressed != 0) {
        UIWin_CreateExtraWin();
    }
done:
    ;
}

extern "C" void CMainMenu_AdvanceIdle(CMainMenu* self) {
    // Per-frame menu advance: update the sub-cursor pane names/positions from
    // the current (state, sub-state) pair, then park the state at 6 (idle).
    if (advanceAnimTransform(self->field_0x88, 1.0f) == 0) {
        return;
    }
    self->field_0xC4 = 0;
    int idx = 0;
    switch (self->field_0xC0) {
    case 1: idx = 0x8; break;
    case 2: idx = 0xb; break;
    case 4: idx = 0xd; break;
    case 5: idx = 0x11; break;
    case 6: idx = 0x13; break;
    }
    if (idx == 0x13) {
        // All-states cursor: while any presentation/player condition holds the
        // cursor index is forced to 0x16 (no free roam).
        int cond;
        if (getUnk80664658() != 0 && (getUnk80664658()->mFlags & 0x8)) {
            cond = 1;
        } else if (gmPauseFlags & 0x40000000) {
            cond = 1;
        } else {
            CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
            if (spot == 0) goto math;
            if (getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4 == 0) goto math;
            u32 flags = getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4->field_4EC;
            if ((flags & 0x1000000) || (flags & 0x8000) == 0 || (flags & 0x100)) {
                cond = 1;
            } else {
            math:
                // Retail booleanizes this bound check arithmetically
                // (subi/orc/srwi/subf/srwi idiom for unsigned >=).
                cond = (CtrlRemote_TouchBitByArg(0x20) >= 0x18E);
            }
        }
        if (cond != 0) idx = 0x16;
    }
    char buf[0x20];
    sprintf(buf, "pic_base%02d_%02d", self->field_0xC0 + 1, self->field_0xC4 + 1);
    // First two lookups go through the root-pane virtual; the fourth argument
    // of ScalePaneWorldPos is the layout's raw +0x10 root pointer (no dispatch).
    // Declaration order nudges MWCC's callee-saved coloring.
    nw4r::lyt::Pane* pane1;
    nw4r::lyt::Pane* pane2;
    nw4r::lyt::Pane* root = *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10);
    pane1 = root->FindPaneByName(buf, true);
    pane2 = root->FindPaneByName("nul_proportion", true);
    nw4r::math::VEC3 vec;
    // Fourth argument reloads the raw root pointer (no dispatch).
    ScalePaneWorldPos(&vec, pane1, pane2,
                  *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10));
    self->mSubCur.setRootPaneTranslate(&vec);
    if (idx > 0) {
        char* s1 = BdatTouchStringCell("MNU_main", "name", idx);
        char* s2 = BdatTouchStringCell("MNU_main", "help", idx);
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", s1, 0);
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", s2, 0);
    } else {
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", "", 0);
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", "", 0);
    }
    self->mMainCur.SetVisible(0);
    self->mSubCur.SetVisible(1);
    self->field_0xE0 = 6;
}

extern "C" void CMainMenu_HandleSubInput(CMainMenu* self) {
    // Cursor-select sub-menu input: gate on the global input lock, then
    // handle sub-cursor movement / confirmation / cancel.

    if (CMainMenu_IsInputBlocked(self) != 0) {
        // Input locked: stop the intro animations and move to the exit state.
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, true);
        self->field_0x8C->SetFrame(0.0f);
        self->mMainCur.SetVisible(1);
        self->mSubCur.SetVisible(0);
        playUISound(6);
        self->field_0xE0 = 7;
        return;
    }
    if (MenuTutorialIsCreated() != 0) {
        return;
    }

    CMainMenuPad* pad = getCurrentPad__Q22cf13CfGameManagerFv();
    // First mode query: held-button early-outs per input style.
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        if (pad->mHeldButtonFlags & 0x08000000) {
            return;
        }
    } else {
        if (pad->mHeldButtonFlags & 0x800) {
            return;
        }
    }
    u32 down, up, aPressed, bPressed, confirm;
    // Second mode query picks the button extraction.
    if (cf::CfGameManager::isClassicController(-1) != 0) {
        u32 p = pad->mPressedButtonFlags;
        u32 t = pad->mTurboPressButtonFlags;
        int c = ((p >> 22) & 1) | ((p >> 10) & 1);
        aPressed = (p >> 21) & 1;
        down = t & 1;
        confirm = c != 0;
        up = (t >> 1) & 1;
        bPressed = (p >> 26) & 1;
    } else {
        u32 p = pad->mPressedButtonFlags;
        u32 t = pad->mTurboPressButtonFlags;
        int c = p & 0x420;
        aPressed = (p >> 4) & 1;
        down = t & 1;
        confirm = c != 0;
        up = (t >> 1) & 1;
        bPressed = (p >> 12) & 1;
    }

    if (down != 0) {
        // Down: wrap the sub-cursor backward, then move the cursor pane.
        switch (self->field_0xC0) {
        case 1:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 2;
            break;
        case 2:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 1;
            break;
        case 4:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 3;
            break;
        case 5:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 1;
            break;
        case 6:
            if (--self->field_0xC4 < 0) self->field_0xC4 = 2;
            break;
        }
        char buf[0x20];
        sprintf(buf, "pic_base%02d_%02d", self->field_0xC0 + 1, self->field_0xC4 + 1);
        // Raw +0x10 root-pane loads like retail (no accessor call).
        nw4r::lyt::Pane* pane1 =
            (*(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10))->FindPaneByName(buf, true);
        nw4r::lyt::Pane* pane2 =
            (*(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10))
                ->FindPaneByName("nul_proportion", true);
        nw4r::math::VEC3 vec;
        ScalePaneWorldPos(&vec, pane1, pane2,
                      *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10));
        self->mSubCur.setRootPaneTranslate(&vec);
        playUISound(0x6a);
        goto tail;
    }
    if (up != 0) {
        // Up: wrap the sub-cursor forward, then move the cursor pane.
        switch (self->field_0xC0) {
        case 1:
            if (++self->field_0xC4 > 2) self->field_0xC4 = 0;
            break;
        case 2:
            if (++self->field_0xC4 > 1) self->field_0xC4 = 0;
            break;
        case 4:
            if (++self->field_0xC4 > 3) self->field_0xC4 = 0;
            break;
        case 5:
            if (++self->field_0xC4 > 1) self->field_0xC4 = 0;
            break;
        case 6:
            if (++self->field_0xC4 > 2) self->field_0xC4 = 0;
            break;
        }
        char buf[0x20];
        sprintf(buf, "pic_base%02d_%02d", self->field_0xC0 + 1, self->field_0xC4 + 1);
        nw4r::lyt::Pane* pane1 =
            (*(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10))->FindPaneByName(buf, true);
        nw4r::lyt::Pane* pane2 =
            (*(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10))
                ->FindPaneByName("nul_proportion", true);
        nw4r::math::VEC3 vec;
        ScalePaneWorldPos(&vec, pane1, pane2,
                      *(nw4r::lyt::Pane**)((u8*)self->field_0x7C + 0x10));
        self->mSubCur.setRootPaneTranslate(&vec);
        playUISound(0x6a);
        goto tail;
    }
    if (aPressed != 0) {
        // Confirm: reject blocked items with a buzz, otherwise open the
        // selected sub-screen (party state for the special "free roam" item).
        u32 idx;
        switch (self->field_0xC0) {
        case 1: idx = (u8)self->field_0xC4; break;
        case 2: idx = (u8)(self->field_0xC4 + 3); break;
        case 4: idx = (u8)(self->field_0xC4 + 5); break;
        case 5: idx = (u8)(self->field_0xC4 + 9); break;
        case 6: idx = (u8)(self->field_0xC4 + 0xb); break;
        default: idx = 0; break;
        }
        if (self->field_0xC8[(u8)idx + 7] != 0) {
            playUISound(5);
            return;
        }
        if (self->field_0xC0 == 6 && self->field_0xC4 == 0) {
            // Free-roam entry: open the party screen via the spot object.
            // Retail biases the returned pointer back to the player base
            // before the null check and dispatches slot 0x9C with the address
            // of the sub-object's 0x3B4 region.
            setPresentationFlag__Q22cf13CfGameManagerFv(true);
            CMainMenuPlayer* player =
                (CMainMenuPlayer*)getPlayer__Q22cf13CfGameManagerFi(0);
            if (player != 0) {
                player = (CMainMenuPlayer*)((char*)player - 0x3E9C);
            }
            if (player != 0 && player->spot.field_0xC4 != 0) {
                ((CfPlayerSpotReal*)&player->spot)->partyOpen(&player->spot.field_0xC4->field_0x3B4[0]);
            }
        }
        playUISound(0x6b);
        self->field_0xE0 = 8;
        self->mSubCur.SetActive();
        goto tail;
    }
    if (confirm != 0) {
        // Cancel: stop the animations and move to the exit state.
        self->field_0x7C->SetAnimationEnable(self->field_0x80, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x84, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x88, false);
        self->field_0x7C->SetAnimationEnable(self->field_0x8C, true);
        self->field_0x8C->SetFrame(0.0f);
        self->mMainCur.SetVisible(1);
        self->mSubCur.SetVisible(0);
        playUISound(6);
        self->field_0xE0 = 7;
    } else if (bPressed != 0) {
        UIWin_CreateExtraWin();
    }

tail:
    // Common tail: pick the sub-cursor index for the selected menu and set
    // its text panes (free-roam gate identical to CMainMenu_AdvanceIdle).
    int idx = 0;
    switch (self->field_0xC0) {
    case 1: idx = self->field_0xC4 + 0x8; break;
    case 2: idx = self->field_0xC4 + 0xb; break;
    case 4: idx = self->field_0xC4 + 0xd; break;
    case 5: idx = self->field_0xC4 + 0x11; break;
    case 6:
        idx = self->field_0xC4 + 0x13;
        if (idx == 0x13) {
            int cond;
            if (getUnk80664658() != 0 && (getUnk80664658()->mFlags & 0x8)) {
                cond = 1;
            } else if (gmPauseFlags & 0x40000000) {
                cond = 1;
            } else {
                CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
                if (spot == 0) goto math;
                if (getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4 == 0) goto math;
                u32 flags = getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4->field_4EC;
                if ((flags & 0x1000000) || (flags & 0x8000) == 0 || (flags & 0x100)) {
                    cond = 1;
                } else {
                math:
                    u32 x = CtrlRemote_TouchBitByArg(0x20);
                    u32 a = x | ~0x18E;
                    u32 b = (x - 0x18E) >> 1;
                    cond = (a - b) >> 31;
                }
            }
            if (cond != 0) idx = 0x16;
            if (gmPauseFlags & 0x40000000) idx = 0x17;
        } else if (idx == 0x14 && (gmPauseFlags & 0x40000000)) {
            idx = 0x18;
        }
        break;
    }
    if (idx > 0) {
        char* s1 = BdatTouchStringCell("MNU_main", "name", idx);
        char* s2 = BdatTouchStringCell("MNU_main", "help", idx);
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", s1, 0);
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", s2, 0);
    } else {
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_suject", "", 0);
        LayoutSetTextBoxFmtValue(self->field_0x7C, "txt_helpmess", "", 0);
    }
}

extern "C" void CMainMenu_DispatchMenuState(CMainMenu* self) {
    // Per-frame menu-state dispatch: while no other screen is open, run the
    // handler for the current (state, sub-state) pair, then park the state
    // at 6 (idle) for the next frame.
    if (CMainMenu_IsInputBlocked(self) == 0 && MenuTutorialIsCreated() == 0) {
        switch (self->field_0xC0) {
        case 1:
            switch (self->field_0xC4) {
            case 0: CUICfManager_queueBaseMenuItem(); break;
            case 1: CUICfManager_queueMakeCrystalMenu(); break;
            case 2: CUICfManager_queueCollepediaMenu(); break;
            }
            break;
        case 2:
            switch (self->field_0xC4) {
            case 0: CUICfManager_queueArtsSetMenu(); break;
            case 1: CUICfManager_queuePassiveSkillMenu(); break;
            }
            break;
        case 4:
            switch (self->field_0xC4) {
            case 0: CUICfManager_queueQuestLogMenu(0); break;
            case 1: CUICfManager_queueKizunagramMenu(1); break;
            case 2: CUICfManager_queueKizunaTalkMenu(); break;
            case 3: CUICfManager_queuePlayAwardMenu(); break;
            }
            break;
        case 5:
            switch (self->field_0xC4) {
            case 0: CUICfManager_queueSkipTimerMenu(); break;
            case 1: CUICfManager_queueTutorialListMenu(0); break;
            }
            break;
        case 6:
            switch (self->field_0xC4) {
            case 0: CUICfManager_queueSaveMenu(0, 1); break;
            case 1: CUICfManager_openSaveMenu(); break;
            case 2: CUICfManager_queueOptionMenu(1); break;
            }
            break;
        }
    }
    self->field_0xE0 = 6;
}

extern "C" int CMainMenu_IsInputBlocked(CMainMenu* self) {
    (void)self;
    // Gameplay-input gate: returns 1 while input should be blocked (dead
    // player, active battle list, closing menus, or presentation flags).
    CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
    CMainMenuPlayer* player = (CMainMenuPlayer*)spot;
    if (spot != 0) {
        // De-bias the embedded +0x3E9C spot object back to the player base.
        player = (CMainMenuPlayer*)((CMainMenuPlayerHead*)spot - 1);
    }
    if (player != 0) {
        float hp = ((CfPlayerBaseReal*)player)->getHP();
        if (0.0f == hp) return 1;
        CMainMenuPlayerSub* sub = player->spot.field_0xC4;
        if (sub != 0) {
            int cond = (sub->field_4EC >> 15) & 1;
            if (isSceneLoading__Q22cf13CfGameManagerFv() != 0) cond = 1;
            // Retail shares one return-1 tail (L1C) for the flag/action/arts
            // gates; goto anchors that shared block at its retail position.
            if (cond == 0 && (sub->field_4EC & 2) == 0) goto ret1;
            if (sub->field_530 & 1) goto ret1;
            if ((sub->field_4EC & 0x100) == 0) goto battle;
        ret1:
            return 1;
        }
    }
battle:
    // Active battle objects (non-empty manager list). Declaration order
    // mirrors the matched SysWinLog_PollBattleStart loop so MWCC colors node r3 /
    // sentinel r5 like retail.
    CMainMenuBattleMgr* bm = (CMainMenuBattleMgr*)getInstance__Q22cf14CBattleManagerFv();
    CMainMenuBattleNode* node;
    int count;
    CMainMenuBattleNode* sentinel = bm->list;
    count = 0;
    node = sentinel->next;
    while (node != sentinel) {
        node = node->next;
        count++;
    }
    if (count != 0) return 1;
    if (hasQuestWindow()) return 1;
    if (SysWinGetSingleton()) return 1;
    if (getUnk80664658()->mFlags & 0x80000) return 1;
    // Presentation-flag / close gates share one return-1 tail (L1BC); the
    // volatile reads defeat MWCC's load CSE and keep both loads hoisted
    // before the masks (retail lwz r0 + lwz r3).
    u32 ev0 = *(volatile u32*)&gmGameFlags;
    u32 ev1 = *(volatile u32*)&gmGameFlags;
    if ((ev0 & 0x4) != 0) goto ret2;
    if ((ev1 & 0x400000) != 0) goto ret2;
    if (isAnyFieldFlagSet__Q22cf13CfGameManagerFv() == 0) goto end;
ret2:
    return 1;
end:
    return CUICfManager_hasInUseSlot();
}

extern "C" void CMainMenu_RefreshFlags(CMainMenu* self) {
    // Menu availability refresh: recompute cursor flags, sub-menu flags and
    // pane positions/colors from the current game state.
    self->field_0xC8[0] = 0;
    self->field_0xC8[1] = 0;
    self->field_0xC8[2] = 0;
    self->field_0xC8[3] = 0;
    self->field_0xC8[4] = 0;
    self->field_0xC8[5] = 0;
    self->field_0xC8[6] = 0;
    self->field_0xC8[7] = 0;

    // hasSave: a save-slot count other than 0xFE always enables the save
    // entry; the 0xFE case consults the shared save flag byte. The boolean is
    // materialized arithmetically (subfic/subi/or/srwi) and both branches
    // feed one store at the merge.
    u32 saveOk = CtrlRemote_TouchBitByArg(0x270) != 0xFE;
    u32 d0;
    if (saveOk != 0) {
        d0 = 1;
    } else {
        BuildSlotItemLists(1);
        d0 = (u8)code80135FDC_getByte_64077() < 2;
    }
    self->field_0xD0 = d0;

    self->field_0xD1 = CtrlRemote_TouchBitByArg(0x337c) == 0 ? 1 : 0;
    self->field_0xD2 = CtrlRemote_TouchBitByArg(0x3355) == 0 ? 1 : 0;
    self->field_0xD3 = CtrlRemote_TouchBitByArg(0x3356) == 0 ? 1 : 0;
    self->field_0xD4 = CtrlRemote_TouchBitByArg(0x337f) == 0 ? 1 : 0;
    self->field_0xD5 = CtrlRemote_TouchBitByArg(0x337a) == 0 ? 1 : 0;
    self->field_0xD6 = CtrlRemote_TouchBitByArg(0x3380) == 0 ? 1 : 0;
    self->field_0xD7 = 0;
    self->field_0xD8 = 0;
    self->field_0xD9 = 0;

    // Free-roam gate (same expression shape as CMainMenu_AdvanceIdle).
    int cond;
    if (getUnk80664658() != 0 && (getUnk80664658()->mFlags & 0x8)) {
        cond = 1;
    } else if (gmPauseFlags & 0x40000000) {
        cond = 1;
    } else {
        CMainMenuPlayerSpot* spot = getPlayer__Q22cf13CfGameManagerFi(0);
        if (spot == 0) goto math;
        if (getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4 == 0) goto math;
        u32 flags = getPlayer__Q22cf13CfGameManagerFi(0)->field_0xC4->field_4EC;
        if ((flags & 0x1000000) || (flags & 0x8000) == 0 || (flags & 0x100)) {
            cond = 1;
        } else {
        math:
            // Odd free-roam gate computed from the player-state resource
            // (retail bit-twiddling reproduced verbatim: subi/orc/srwi/subf).
            u32 x = CtrlRemote_TouchBitByArg(0x20);
            cond = (x >= 0x18Eu);
        }
    }
    self->field_0xDA = cond;

    self->field_0xDB = (gmPauseFlags >> 30) & 1;
    self->field_0xDC = 0;

    // Main cursor items 0..6: pane name is sprintf'ed, position is bright
    // when available ({0x3c,0x32,0x1e}) else dim ({0xff,0xff,0xff}).
    char buf[0x20];
    for (u32 i = 0; i < 7; i++) {
        sprintf(buf, "pic_icon%02d", (u8)i + 1);
        if (self->field_0xC8[(u8)i] != 0) {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0x1e;
            pos[1] = 0x32;
            pos[0] = 0x3c;
            PaneMatSetS16QuadByName(self->field_0x7C, buf, (const GXColorS10*)pos, 1);
        } else {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0xff;
            pos[1] = 0xff;
            pos[0] = 0xff;
            PaneMatSetS16QuadByName(self->field_0x7C, buf, (const GXColorS10*)pos, 1);
        }
    }

    // Sub-menu items 0..13: fixed pane names copied into a stack table;
    // availability flag bytes live at 0xCF+i (field_0xC8[7+i]).
    const char* names[14];
    *(CMainMenuNameTable*)names = *(CMainMenuNameTable*)lbl_eu_804FCE50;
    for (u32 i = 0; i < 14; i++) {
        if (self->field_0xC8[(u8)i + 7] != 0) {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0x1e;
            pos[1] = 0x32;
            pos[0] = 0x3c;
            PaneMatSetS16QuadByName(self->field_0x7C, names[(u8)i], (const GXColorS10*)pos, 1);
        } else {
            s16 pos[4];
            pos[3] = 0xff;
            pos[2] = 0xff;
            pos[1] = 0xff;
            pos[0] = 0xff;
            PaneMatSetS16QuadByName(self->field_0x7C, names[(u8)i], (const GXColorS10*)pos, 1);
        }
    }
}
