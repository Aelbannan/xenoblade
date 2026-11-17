// Decomp of kyoshin/menu/CMenuSave.
// Menu save/load screen process (IUICf task + IScnRender MI).

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuSave.hpp"

#include "kyoshin/CTaskGame.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include "monolib/scn/CScn.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include "monolib/util/MemManager.hpp"

#include <revolution/GX.h>
#include "kyoshin/CTitleAHelpImports.hpp"
void playUISound(u32);

class CSaveLoad;

// Flat view of cf::CfPadData exposing only the flag words handleSaveMenuInput reads:
// CPad::mPressedButtonFlags at +0x4 and CfPadData::mTurboPressButtonFlags at
// +0x104 (see kyoshin/cf/CfPadData.hpp for the full layout).
struct MenuSavePadData {
    u32 mPadHeldFlags;          // 0x00 - CPad::mHeldButtonFlags
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags
    u8 _pad[0xFC];              // 0x08-0x103 - rest of the pad data
    u32 mTurboPressButtonFlags; // 0x104
};

// Message table base used by Init's CTitleAHelp name lookup (+0xb string).
extern char lbl_eu_8050F7B0[];
// Render-gate mode bitfield (.sbss; bit 21 = realtime event busy).
extern u32 lbl_eu_80663E28;

// Retail-unmangled callee names (US strips mangling for these func_ helpers).
// isIdle/BgTex_IsLoaded_3E34/... are declared int (not u8) so callers compare with
// cmpwi directly (no byte mask), matching retail.
extern "C" int BgTex_IsLoaded_3E34(CBgTex* self);
extern "C" void BgTex_Release_3D9C(CBgTex* self);
extern "C" int isInitialized(CTitleAHelp* self);
extern "C" void teardown(CTitleAHelp* self);
extern "C" void func_801C412C(CTitleAHelp* self);
extern "C" void func_801C41E8(CTitleAHelp* self, u8 arg);
extern "C" int CSaveLoad_getIdle(CSaveLoad* self);
extern "C" int CSaveLoad_isReady(CSaveLoad* self);
extern "C" u8 func_8028FFD4(CSaveLoad* self);
extern "C" void CSaveLoad_reset(CSaveLoad* self);
extern "C" void CSaveLoad_open(CSaveLoad* self);
extern "C" void NandTeardown();
extern "C" void setPresentationFlag__Q22cf13CfGameManagerFv(u8 enable);

// Additional C-ABI imports used by the Move/cbRenderBefore/Init handlers
// (retail symbols are unmangled / non-namespace, so C linkage is required).
extern "C" int IsMenuState621F0();
extern "C" void openSaveMenu(CMenuSave* self);
extern "C" void advanceSaveMenuPhase(CMenuSave* self);
extern "C" void handleSaveMenuInput(CMenuSave* self);
extern "C" void flagSaveMenuReady(CMenuSave* self);
extern "C" void BgTex_Tick_3D54(CBgTex* self);
extern "C" void BgTex_Acquire_3C14(CBgTex* self);
extern "C" void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" void updateHelp(CTitleAHelp* self);
extern "C" void beginClose(CTitleAHelp* self);
extern "C" void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
// CTitleAHelp::CTitleAHelp_load() is declared in kyoshin/CTitleAHelp.hpp.
extern "C" void CSaveLoad_update(CSaveLoad* self);
extern "C" void CSaveLoad_draw(CSaveLoad* self, nw4r::lyt::DrawInfo* drawInfo);
extern "C" int CSaveLoad_isBusy(CSaveLoad* self);
extern "C" int CSaveLoad_getDone(CSaveLoad* self);
extern "C" void CSaveLoad_confirm(CSaveLoad* self);
extern "C" void CSaveLoad_cancel(CSaveLoad* self);
extern "C" void CSaveLoad_close(CSaveLoad* self);
extern "C" void CSaveLoad_openDel(CSaveLoad* self);
extern "C" void CSaveLoad_curUp(CSaveLoad* self);
extern "C" void CSaveLoad_curDown(CSaveLoad* self);
extern "C" void CSaveLoad_pageUp(CSaveLoad* self);
extern "C" void CSaveLoad_pageDown(CSaveLoad* self);
extern "C" void loadSaveData__9CSaveLoadFv(CSaveLoad* self);
extern "C" int CMainMenu_GetInstancePtr();
extern "C" void ArtsInfo_SetReadyFlag();
extern "C" int isClassicController__Q22cf13CfGameManagerFv(int arg);
extern "C" void __ct__6CBgTexFUc(void* self, int arg);
extern "C" void __ct__11CTitleAHelpFPcUc(void* self, char* name, int id);
extern "C" void __ct__CSaveLoad(void* self, u8 argA, u8 argB);
extern "C" void __as__17UnkClass_8045F564FRC17UnkClass_8045F564(void* dest, void* src);
extern "C" void NandAllocBufs();
extern "C" char* BdatTouchStringCell(char* a, char* b, int id);
extern "C" void InfoCfCopySysWin(void* dest, void* src);
extern "C" CSaveLoad* __dt__9CSaveLoadFv(CSaveLoad* self, int flags);
extern "C" CTitleAHelp* __dt__11CTitleAHelpFv(CTitleAHelp* self, int flags);
extern "C" CBgTex* __dt__6CBgTexFv(CBgTex* self, int flags);

// DrawInfo helper - retail emits the mangled nw4r form
// (SetWideLocAdjust__FPQ34nw4r3lyt8DrawInfo), so it is declared as a normal C++
// global function to let MWCC mangle it - NOT inside the extern "C" block.
void SetWideLocAdjust(nw4r::lyt::DrawInfo* drawInfo);

/*
 * Constructor. The CBgTex / CTitleAHelp members are real members built by the
 * initialiser list; the CSaveLoad region is raw storage, so its constructor is
 * called by hand. The trailing stores and the presentation flag follow it.
 */
CMenuSave::CMenuSave(CProcess* parent, u32 arg208, u32 arg209, u32 arg20A)
    : mParentRef(parent), mBgTex(0), mTitleAHelp(0, 0) {
    __ct__CSaveLoad(reinterpret_cast<CSaveLoad*>(mSaveLoad), 0, 0);

    mField208 = arg208;
    mField209 = arg209;
    mField20A = arg20A;
    mState = 0;
    if ((u8)arg20A != 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(1);
    }
}

/*
 * Destructor. The raw CSaveLoad region is torn down here; the compiler then
 * destroys mTitleAHelp, mBgTex and the IUICf base in reverse order.
 */
CMenuSave::~CMenuSave() {
    __dt__9CSaveLoadFv(reinterpret_cast<CSaveLoad*>(mSaveLoad), -1);
}

/*
 * Init - (re)initialise the save screen. Resets the CfGameManager init flag
 * and the media when requested, then rebuilds each embedded widget (CBgTex /
 * CTitleAHelp / CSaveLoad) by constructing a stack temporary and copying its
 * body into the embedded storage, then registers the render callback on the
 * parent scene (see CMenuKizunaTalkList::Init for the same pattern).
 */
void CMenuSave::Init() {
    if (mField20A != 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(1);
    }
    if (mField208 == 0) {
        NandAllocBufs();
    }

    // --- Re-initialise the background CBgTex via a temporary ---
    u8 tempBgTex[0x20];
    __ct__6CBgTexFUc(tempBgTex, 0);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0x64, tempBgTex + 0x4);
    *(u32*)((u8*)this + 0x74) = *(u32*)(tempBgTex + 0x14);
    *(u32*)((u8*)this + 0x78) = *(u32*)(tempBgTex + 0x18);
    *(u8*)((u8*)this + 0x7c)  = *(u8*)(tempBgTex + 0x1c);
    *(u8*)((u8*)this + 0x7d)  = *(u8*)(tempBgTex + 0x1d);
    *(u8*)((u8*)this + 0x7e)  = *(u8*)(tempBgTex + 0x1e);
    __dt__6CBgTexFv(reinterpret_cast<CBgTex*>(tempBgTex), -1);

    BgTex_Acquire_3C14(&mBgTex);

    // --- Re-initialise the embedded CTitleAHelp ---
    char* name = BdatTouchStringCell(lbl_eu_8050F7B0, lbl_eu_8050F7B0 + 0xb,
                                mField208 != 0 ? 0x49 : 0x48);

    u8 tempTitle[0x38];
    __ct__11CTitleAHelpFPcUc(tempTitle, name, mField208 != 0 ? 0x6b : 0x69);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0x84, tempTitle + 0x4);
    *(u32*)((u8*)this + 0x94) = *(u32*)(tempTitle + 0x14);
    *(u32*)((u8*)this + 0x98) = *(u32*)(tempTitle + 0x18);
    *(u32*)((u8*)this + 0x9c) = *(u32*)(tempTitle + 0x1c);
    *(u32*)((u8*)this + 0xa0) = *(u32*)(tempTitle + 0x20);
    *(u32*)((u8*)this + 0xa4) = *(u32*)(tempTitle + 0x24);
    *(u8*)((u8*)this + 0xa8)  = *(u8*)(tempTitle + 0x28);
    *(u32*)((u8*)this + 0xac) = *(u32*)(tempTitle + 0x2c);
    *(u32*)((u8*)this + 0xb0) = *(u32*)(tempTitle + 0x30);
    *(u8*)((u8*)this + 0xb4)  = *(u8*)(tempTitle + 0x34);
    *(u8*)((u8*)this + 0xb5)  = *(u8*)(tempTitle + 0x35);
    *(u8*)((u8*)this + 0xb6)  = *(u8*)(tempTitle + 0x36);
    *(u8*)((u8*)this + 0xb7)  = *(u8*)(tempTitle + 0x37);
    __dt__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(tempTitle), -1);

    mTitleAHelp.CTitleAHelp_load();

    // --- Re-initialise the embedded CSaveLoad via a temporary ---
    u8 tempSave[0x150];
    __ct__CSaveLoad(tempSave, mField208, mField209);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0xbc, tempSave + 0x4);
    *(u32*)((u8*)this + 0xcc) = *(u32*)(tempSave + 0x14);
    *(u32*)((u8*)this + 0xd0) = *(u32*)(tempSave + 0x18);
    *(u32*)((u8*)this + 0xd4) = *(u32*)(tempSave + 0x1c);
    *(u32*)((u8*)this + 0xd8) = *(u32*)(tempSave + 0x20);
    *(u32*)((u8*)this + 0xdc) = *(u32*)(tempSave + 0x24);
    *(u32*)((u8*)this + 0xe4) = *(u32*)(tempSave + 0x2c);
    *(u32*)((u8*)this + 0xe8) = *(u32*)(tempSave + 0x30);
    *(u32*)((u8*)this + 0xec) = *(u32*)(tempSave + 0x34);
    *(u32*)((u8*)this + 0xf0) = *(u32*)(tempSave + 0x38);
    *(u8*)((u8*)this + 0xf4)  = *(u8*)(tempSave + 0x3c);
    *(u8*)((u8*)this + 0xf5)  = *(u8*)(tempSave + 0x3d);
    *(u32*)((u8*)this + 0xfc) = *(u32*)(tempSave + 0x44);
    *(u32*)((u8*)this + 0x100) = *(u32*)(tempSave + 0x48);
    *(u32*)((u8*)this + 0x104) = *(u32*)(tempSave + 0x4c);
    *(u32*)((u8*)this + 0x108) = *(u32*)(tempSave + 0x50);
    *(u8*)((u8*)this + 0x10c)  = *(u8*)(tempSave + 0x54);
    *(u8*)((u8*)this + 0x10d)  = *(u8*)(tempSave + 0x55);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564((u8*)this + 0x114, tempSave + 0x5c);
    *(u32*)((u8*)this + 0x124) = *(u32*)(tempSave + 0x6c);
    *(u32*)((u8*)this + 0x128) = *(u32*)(tempSave + 0x70);
    *(u32*)((u8*)this + 0x12c) = *(u32*)(tempSave + 0x74);
    *(u32*)((u8*)this + 0x130) = *(u32*)(tempSave + 0x78);
    *(u8*)((u8*)this + 0x134)  = *(u8*)(tempSave + 0x7c);
    *(u8*)((u8*)this + 0x135)  = *(u8*)(tempSave + 0x7d);
    *(u8*)((u8*)this + 0x136)  = *(u8*)(tempSave + 0x7e);
    *(u8*)((u8*)this + 0x137)  = *(u8*)(tempSave + 0x7f);
    *(float*)((u8*)this + 0x138) = *(float*)(tempSave + 0x80);
    *(float*)((u8*)this + 0x13c) = *(float*)(tempSave + 0x84);
    *(float*)((u8*)this + 0x140) = *(float*)(tempSave + 0x88);
    *(float*)((u8*)this + 0x144) = *(float*)(tempSave + 0x8c);
    *(float*)((u8*)this + 0x148) = *(float*)(tempSave + 0x90);
    *(u8*)((u8*)this + 0x14c)  = *(u8*)(tempSave + 0x94);

    // Two embedded CSysWin-like regions are copied via their copy helper.
    InfoCfCopySysWin((u8*)this + 0x150, tempSave + 0x98);
    InfoCfCopySysWin((u8*)this + 0x18c, tempSave + 0xd4);

    // Tail state region (scrollbar/cursor/slot state).
    *(u32*)((u8*)this + 0x1c8) = *(u32*)(tempSave + 0x110);
    *(u32*)((u8*)this + 0x1cc) = *(u32*)(tempSave + 0x114);
    *(u32*)((u8*)this + 0x1d0) = *(u32*)(tempSave + 0x118);
    *(u8*)((u8*)this + 0x1d4)  = *(u8*)(tempSave + 0x11c);
    *(u8*)((u8*)this + 0x1d5)  = *(u8*)(tempSave + 0x11d);
    *(u8*)((u8*)this + 0x1d6)  = *(u8*)(tempSave + 0x11e);
    *(u8*)((u8*)this + 0x1d8)  = *(u8*)(tempSave + 0x120);
    *(u8*)((u8*)this + 0x1d9)  = *(u8*)(tempSave + 0x121);
    *(u8*)((u8*)this + 0x1da)  = *(u8*)(tempSave + 0x122);
    *(u8*)((u8*)this + 0x1db)  = *(u8*)(tempSave + 0x123);
    *(u8*)((u8*)this + 0x1dc)  = *(u8*)(tempSave + 0x124);
    *(s16*)((u8*)this + 0x1de) = *(s16*)(tempSave + 0x126);
    *(u8*)((u8*)this + 0x1e0)  = *(u8*)(tempSave + 0x128);
    *(u8*)((u8*)this + 0x1e1)  = *(u8*)(tempSave + 0x129);
    *(u8*)((u8*)this + 0x1e2)  = *(u8*)(tempSave + 0x12a);
    *(u8*)((u8*)this + 0x1e3)  = *(u8*)(tempSave + 0x12b);
    *(u8*)((u8*)this + 0x1e4)  = *(u8*)(tempSave + 0x12c);
    *(u8*)((u8*)this + 0x1e5)  = *(u8*)(tempSave + 0x12d);
    *(u8*)((u8*)this + 0x1e6)  = *(u8*)(tempSave + 0x12e);
    *(u8*)((u8*)this + 0x1e7)  = *(u8*)(tempSave + 0x12f);
    *(u32*)((u8*)this + 0x1e8) = *(u32*)(tempSave + 0x130);
    *(u32*)((u8*)this + 0x1ec) = *(u32*)(tempSave + 0x134);
    *(u32*)((u8*)this + 0x1f0) = *(u32*)(tempSave + 0x138);
    *(u32*)((u8*)this + 0x1f4) = *(u32*)(tempSave + 0x13c);
    *(u32*)((u8*)this + 0x1f8) = *(u32*)(tempSave + 0x140);
    *(u32*)((u8*)this + 0x1fc) = *(u32*)(tempSave + 0x144);
    *(u16*)((u8*)this + 0x200) = *(u16*)(tempSave + 0x148);
    *(u8*)((u8*)this + 0x202)  = *(u8*)(tempSave + 0x14a);
    *(u16*)((u8*)this + 0x203) = *(u16*)(tempSave + 0x14b);
    *(u8*)((u8*)this + 0x205)  = *(u8*)(tempSave + 0x14d);
    __dt__9CSaveLoadFv(reinterpret_cast<CSaveLoad*>(tempSave), -1);

    loadSaveData__9CSaveLoadFv(reinterpret_cast<CSaveLoad*>(mSaveLoad));

    // Register this screen as a render callback on the parent scene.
    reinterpret_cast<CScn*>(mParentRef)->addRenderCB(this, 0xd, 0);
}

void CMenuSave::Term() {
    CDeviceVI::waitForDrawDone();

    reinterpret_cast<CScn*>(mParentRef)->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    CSaveLoad_reset(reinterpret_cast<CSaveLoad*>(mSaveLoad));

    sInstance = nullptr;

    if (mField208 == 0) {
        NandTeardown();
    }
    if (mField20A != 0) {
        setPresentationFlag__Q22cf13CfGameManagerFv(0);
    }
}

// Per-frame update: gate on the task/busy flags, drive the 4-state FSM
// (phase handlers openSaveMenu/E4E0/E530/E768), then refresh the background,
// title/help bar and save-load panel each frame.
void CMenuSave::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0:
        openSaveMenu(this);
        break;
    case 1:
        advanceSaveMenuPhase(this);
        break;
    case 2:
        handleSaveMenuInput(this);
        break;
    case 3:
        flagSaveMenuReady(this);
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    updateHelp(&mTitleAHelp);
    CSaveLoad_update(reinterpret_cast<CSaveLoad*>(mSaveLoad));
}

// Render the save screen through a stack DrawInfo: gate on the task/busy
// flags and the scene-active check, then draw background, save-load panel and
// title/help bar in that order.
void CMenuSave::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::isFlag01Set() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    CSaveLoad_draw(reinterpret_cast<CSaveLoad*>(mSaveLoad),
                  &drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

CMenuSave* CMenuSave::sInstance;

/*
 * Factory: lazily allocate + construct the single save-menu instance and
 * register it as a child of `parent`. Returns the stored instance (or NULL if it
 * already exists). The last three args are forwarded to the constructor, with
 * `parentRef` becoming mParentRef.
 */
CMenuSave* CMenuSave::create(CProcess* parent, CProcess* parentRef, u32 arg208,
                             u32 arg209, u32 arg20A) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuSave* menu = new (CWorkThreadSystem::getWorkMem())
        CMenuSave(parentRef, arg208, arg209, arg20A);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

void stub_us_802908bc() {}

extern "C" unsigned long isSaveMenuActive(void) { return CMenuSave::sInstance != nullptr; }

/* Once the background, title bar and save-load panel are all ready, pull the
 * save-slot data, update the title bar, and play the open sound. */
extern "C" void openSaveMenu(CMenuSave* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0 &&
        isInitialized(&self->mTitleAHelp) != 0 &&
        CSaveLoad_isReady(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) != 0) {
        u8 slot = func_8028FFD4(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        func_801C41E8(&self->mTitleAHelp, slot);
        func_801C412C(&self->mTitleAHelp);
        CSaveLoad_open(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        self->mState = 1;
        playUISound(0x6d);
    }
}

/* Advance the save menu to phase 2 once the title bar is idle and the
 * save-load panel has finished its intro (writes the state byte at 0x20B). */
extern "C" void advanceSaveMenuPhase(CMenuSave* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        CSaveLoad_getIdle(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) != 0) {
        self->mState = 2;
    }
}

/*
 * handleSaveMenuInput - save/load panel input handling (state 2). Reads the current
 * cf pad data; depending on the co-op/single-player controller layout,
 * different pressed-button bits map to the same actions. The button tests use
 * the (inverted) == 0 form for the turbo-flag masks, matching retail's
 * subic/subfe bool idiom.
 */
extern "C" void handleSaveMenuInput(CMenuSave* self) {
    if (CSaveLoad_isReady(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) == 0) {
        return;
    }

    if (CSaveLoad_getDone(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) != 0) {
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
        }
        self->mState = 4;
        self->mField54 = 1;
    }

    MenuSavePadData* pad = (MenuSavePadData*)cf::CfGameManager::getCfPadData();
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        // Co-op layout: Classic-controller style bits.
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        if ((pressed >> 21) & 1) {
            CSaveLoad_confirm(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((pressed >> 22) & 1) {
            if (CSaveLoad_isBusy(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) == 0) {
                beginClose(&self->mTitleAHelp);
                CSaveLoad_close(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
                self->mState = 3;
            } else {
                CSaveLoad_cancel(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
            }
        } else if ((pressed >> 28) & 1) {
            CSaveLoad_openDel(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x8004) == 0) {
            CSaveLoad_curUp(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((((turbo >> 16) & 1) | (turbo & 8)) == 0) {
            CSaveLoad_curDown(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x2001) == 0) {
            CSaveLoad_pageUp(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x4002) == 0) {
            CSaveLoad_pageDown(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((pressed >> 23) & 1) {
            if (self->mField209 == 0 &&
                CSaveLoad_isBusy(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) == 0) {
                if (CMainMenu_GetInstancePtr() != 0) {
                    ArtsInfo_SetReadyFlag();
                }
                playUISound(6);
                self->mState = 4;
                self->mField54 = 1;
            }
        }
    } else {
        // Single-player layout: Wiimote-style bits.
        u32 turbo = pad->mTurboPressButtonFlags;
        u32 pressed = pad->mPadPressedFlags;
        if ((pressed >> 4) & 1) {
            CSaveLoad_confirm(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((pressed >> 5) & 1) {
            if (CSaveLoad_isBusy(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) == 0) {
                beginClose(&self->mTitleAHelp);
                CSaveLoad_close(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
                self->mState = 3;
            } else {
                CSaveLoad_cancel(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
            }
        } else if ((pressed >> 7) & 1) {
            CSaveLoad_openDel(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x8004) == 0) {
            CSaveLoad_curUp(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((((turbo >> 16) & 1) | (turbo & 8)) == 0) {
            CSaveLoad_curDown(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x2001) == 0) {
            CSaveLoad_pageUp(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((turbo & 0x4002) == 0) {
            CSaveLoad_pageDown(reinterpret_cast<CSaveLoad*>(self->mSaveLoad));
        } else if ((pressed >> 10) & 1) {
            if (self->mField209 == 0 &&
                CSaveLoad_isBusy(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) == 0) {
                if (CMainMenu_GetInstancePtr() != 0) {
                    ArtsInfo_SetReadyFlag();
                }
                playUISound(6);
                self->mState = 4;
                self->mField54 = 1;
            }
        }
    }

    func_801C41E8(&self->mTitleAHelp, func_8028FFD4(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)));
}

/* Same idle+advance check as advanceSaveMenuPhase, but advances the phase flag at
 * 0x54 instead of the state byte. */
extern "C" void flagSaveMenuReady(CMenuSave* self) {
    if (self->mTitleAHelp.isIdle() != 0 &&
        CSaveLoad_getIdle(reinterpret_cast<CSaveLoad*>(self->mSaveLoad)) != 0) {
        self->mField54 = 1;
    }
}
