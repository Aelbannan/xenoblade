// World map screen UI process (kyoshin/menu/CMenuMapSelect).

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/menu/CMenuMapSelect.hpp"

#include <monolib/work/CWorkThreadSystem.hpp>
#include <monolib/util/MemManager.hpp>
#include <monolib/core/CPadManager.hpp>
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/CTitleAHelpImports.hpp"
#include "kyoshin/CFloorMapImports.hpp"
#include <monolib/scn/CScn.hpp>
#include "kyoshin/CTaskGame.hpp"
#include <revolution/GX.h>
#include "kyoshin/code_80135FDC_State.hpp"
#include "kyoshin/code_80135FDC_Lyt.hpp"

extern u32 lbl_eu_80663E28; // .sbss global busy-flag word (bit 21)
extern u32 lbl_eu_80664790;
extern void CFade_StartLoad(CFade* fade);

void FloorMapCopy(CFloorMap* dest, const CFloorMap& src);
void setupFloorMapPhase(CMenuMapSelect* self);
void handleFloorMapInput(CMenuMapSelect* self);

/* Field-exact view of CFloorMap for the retail memberwise copy helper
   FloorMapCopy. Widths mirror the retail copy: words for data blocks, bytes
   for flags, floats for scalar state, and two pair-array loops (20 x 8 bytes
   at +0x150, 0x618 x 8 bytes at +0x20C). The vtable word (+0x00), the small
   inter-block gaps and the trailing word pair (+0x3340/+0x3344, stored in
   reverse order) are handled explicitly. */
struct CFloorMapCopyPair {
    u32 a;
    u32 b;
};

/* Retail lowers the 0xB4..0x207 and 0x208..0x32CB stretches as single
   struct assignments. Skipped gaps are unnamed storage (anonymous byte
   bitfields), so only named members expand to copies; the pair arrays
   become ctr loops whose walkers start at the substruct base - 4
   (displacement folds the array offset). */
struct CFloorMapCopyBlock1 {
    u8 fB4 : 8;
    u8 fB5 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;                 // +0xB6..0xBB skipped
    u32 fBC[9];             // +0xBC..0xDC
    u8 fE0 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u32 fE4[2];             // +0xE4..0xE8
    u8 fEC0 : 8;            // +0xEC..0xF1
    u8 fEC1 : 8;
    u8 fEC2 : 8;
    u8 fEC3 : 8;
    u8 fEC4 : 8;
    u8 fEC5 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;                 // +0xF2..0xF7 skipped
    u32 fF8[9];             // +0xF8..0x118
    u8 f11C : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u32 f120[2];            // +0x120..0x124
    u8 f128_0 : 8;          // +0x128..0x12D
    u8 f128_1 : 8;
    u8 f128_2 : 8;
    u8 f128_3 : 8;
    u8 f128_4 : 8;
    u8 f128_5 : 8;
    u8 : 8;
    u8 : 8;
    u32 f130[7];            // +0x130..0x148
    u8 f14C : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    CFloorMapCopyPair loop1[20]; // +0x150..0x1EF
    u8 f1F0 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u32 f1F4;               // +0x1F4
    u8 f1F8 : 8;
    u8 : 8;
    u8 : 8;
    u8 : 8;
    u32 f1FC;               // +0x1FC
    u32 f200;               // +0x200
    u8 f204 : 8;
    u8 f205 : 8;
    u8 f206 : 8;
    u8 f207 : 8;
};

struct CFloorMapCopyBlock2 {
    u8 f208 : 8;            // +0x208
    u8 : 8;
    u8 : 8;
    u8 : 8;
    CFloorMapCopyPair loop2[0x618]; // +0x20C..0x32CB
};

struct CFloorMapCopyView {
    u8 _00[4];              // +0x00 vptr (skipped)
    u32 f04[15];            // +0x04..0x3C
    u8 f40, f41, f42, f43;
    float f44[5];           // +0x44..0x54
    u8 f58;
    u8 _59;
    u16 f5A;
    u8 f5C, f5D;
    u8 _5E[6];              // +0x5E..0x63 (skipped)
    u32 f64[8];             // +0x64..0x80
    u8 f84, f85, f86, f87;
    float f88[5];           // +0x88..0x94
    u8 f9C;
    u8 _9D[3];
    u8 _A0[4];              // +0xA0..0xA3 (skipped)
    u32 fA4[3];             // +0xA4..0xAC
    u32 fB0;                // +0xB0
    CFloorMapCopyBlock1 block1;
    CFloorMapCopyBlock2 block2;
    u8 f32CC;
    u8 _32CD[3];
    u32 f32D4[4];           // +0x32D4..0x32E0
    u8 f32E4, f32E5;
    u8 _32E6[6];
    u32 f32EC[4];           // +0x32EC..0x32F8
    u8 f32FC, f32FD;
    u8 _32FE[6];
    u32 f3304[4];           // +0x3304..0x3310
    u8 f3314, f3315;
    u8 _3316[6];
    u32 f331C[4];           // +0x331C..0x3328
    u8 f332C, f332D;
    u8 _332E[2];
    u32 f3330[3];           // +0x3330..0x3338
    u8 f333C;
    u8 _333D[3];
    u32 f3340;
    u32 f3344;
};


// Out-of-line CBgTex / CTitleAHelp copy assignment (retail global symbols in
// CMenuItemExchange; not yet spelled as __as__ there).
extern "C" CBgTex* ItemEx_CopyBgTex(CBgTex* dest, CBgTex* src);
extern "C" CTitleAHelp* ItemEx_CopyTitle(CTitleAHelp* dest, CTitleAHelp* src);

extern "C" {
extern char lbl_eu_8050B498[];
void setPresentationFlag__Q22cf13CfGameManagerFv(bool enable);
void waitForDrawDone__9CDeviceVIFv();
void Scn_SetPauseFlag(CScn* scn, int flag);
void BgTex_Release_3D9C(CBgTex* self);
void teardown(CTitleAHelp* self);
void FloorMap_Teardown(CFloorMap* self);
void UIWin_ControlWindowLists(u32, u32);
u32 CtrlRemote_TouchBitByArg(u32 resourceId);
int CMainMenu_IsOpen();
void func_8008413C__Q22cf13CfGameManagerFv(u32, u32);
int BgTex_IsLoaded_3E34(CBgTex* self);
void BgTex_Tick_3D54(CBgTex* self);
void updateHelp(CTitleAHelp* self);
int isInitialized(CTitleAHelp* self);
void func_801C412C(CTitleAHelp* self);
void func_801C4654(CTitleAHelp* self, u32 arg);
void setInfoText(CTitleAHelp* self, char* text);
void func_801C46DC(CTitleAHelp* self, u32 arg);
void CFade_Update(CFade* self);
void CFade_FadeOut(CFade* self);
int FloorMap_GetField42Gated(CFloorMap* self);
void FloorMap_LoadStageLayout(CFloorMap* self);
int FloorMap_GetField40Flag(CFloorMap* self);
void FloorMap_UpdateFrame(CFloorMap* self);
void FloorMap_Draw(CFloorMap* self, nw4r::lyt::DrawInfo* drawInfo);
void BgTex_Draw_3D7C(CBgTex* self, nw4r::lyt::DrawInfo* drawInfo);
void drawHelp(CTitleAHelp* self, nw4r::lyt::DrawInfo* drawInfo);
void CFade_Draw(CFade* self, nw4r::lyt::DrawInfo* drawInfo);
}

// View of cf::CfPadData (incomplete in CfGameManager.hpp): embedded CPad at
// +0x00 and the turbo-press flag copy at +0x104.
struct CfPadDataView {
    u8 _00[0x4];
    u32 mPadPressedFlags;       // 0x04 - CPad::mPressedButtonFlags copy
    u8 _08[0x104 - 0x8];
    u32 mTurboPressButtonFlags; // 0x104
};

extern "C" {

// Callee helpers (retail-unmangled names).
// Use the C++ member (CfGameManager.hpp); no local pre-mangled copy.
int CMainMenu_GetInstancePtr();
void ArtsInfo_SetReadyFlag();
int CFade_IsVisible(CFade* self);
// (memberized: CFade_FadeIn -> CFade::In; declaration moved into class CFade)
// (memberized: FloorMap_SetMapTable -> CFloorMap::SetMapTable; declaration moved into class CFloorMap)
void FloorMap_LoadMapFiles(CFloorMap* self);

int FloorMap_ClassifyByte41(CFloorMap* self);
int FloorMap_CheckRowVisited(CFloorMap* self);
u8 FloorMap_GetBdatMaxRow(CFloorMap* self);
u8 FloorMap_GetField208Plain(CFloorMap* self);
u8 FloorMap_GetField208Gated(CFloorMap* self);
int FloorMap_IsSysWinBusy(CFloorMap* self);
void FloorMap_SetPaneFlag(CFloorMap* self, u32 arg);
u16 FloorMap_GetU16Field5A(CFloorMap* self);
void FloorMap_ConfirmSelect(CFloorMap* self);
void FloorMap_UpdateRowPanePos(CFloorMap* self);
void FloorMap_CursorNextFloor(CFloorMap* self);
void FloorMap_OpenRowWindow(CFloorMap* self);
void FloorMap_UpdateCursorMove(CFloorMap* self);
void FloorMap_UpdateWorldMap(CFloorMap* self);
void FloorMap_HandleMapDown(CFloorMap* self);
void FloorMap_NormalizeMapPos(CFloorMap* self);
void FloorMap_Log2Field208(CFloorMap* self);
void FloorMap_Log2Field333C(CFloorMap* self);
void FloorMap_ScrollDown(CFloorMap* self);
void FloorMap_ScrollUp(CFloorMap* self);
void FloorMap_ScrollRight(CFloorMap* self);
void FloorMap_ScrollLeft(CFloorMap* self);
void FloorMap_MoveToCursorRow(CFloorMap* self);

// CTitleAHelp help-bar id setter (retail-unmangled).
void func_801C41E8(CTitleAHelp* self, u8 arg);
}

// CTitleAHelp help-bar text reset (retail-unmangled name).
extern "C" void beginClose(CTitleAHelp* self);

// C++-linkage helper (retail emits the mangled form playUISound__FUl).

CMenuMapSelect* CMenuMapSelect::sInstance;

// Retail builds the IUICf task, the render base and the member widgets in the
// same function as the factory, so the constructor is defined inline here and
// is folded into create().
inline CMenuMapSelect::CMenuMapSelect(CProcess* parentRef)
    : mParent(parentRef), mBgTex(0), mTitleAHelp(0, 0), mState(0), mTimer(0.0f) {}

CMenuMapSelect::~CMenuMapSelect() {}

// Real cause of the out-of-line copies (MWCC_CASES "two rules", rule B):
// retail's mFloorMap = CFloorMap() assignment is a 0x494-byte implicit
// CFloorMap::operator= (FloorMapCopy, weak __as__9CFloorMapFRC9CFloorMap),
// which puts Init over the inlining budget, so every assignment in Init is
// called out of line. CFloorMap is not a real class yet (byte-array layout,
// hand-written FloorMapCopy), so the budget cannot be reproduced and
// dont_inline stands in for it. Remove once CFloorMap has its real members.
#pragma push
#pragma dont_inline on
void CMenuMapSelect::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    {
        CBgTex bgTex(0);
        ItemEx_CopyBgTex(&mBgTex, &bgTex);
    }
    mBgTex.BgTex_Acquire_3C14();

    char* helpStr = (char*)BdatTouchStringCell(lbl_eu_8050B498, lbl_eu_8050B498 + 8, 1);
    {
        CTitleAHelp titleHelp(helpStr, 0x4a);
        ItemEx_CopyTitle(&mTitleAHelp, &titleHelp);
    }
    mTitleAHelp.CTitleAHelp_load();

    mMapSel = CMapSel();
    mMapSel.startLoad();

    mFade = CFade();
    CFade_StartLoad(&mFade);

    FloorMapCopy(&mFloorMap, CFloorMap());

    static_cast<CScn*>(mParent)->addRenderCB(this, 0xd, 1);
}
#pragma pop


// Memberwise copy of a fully constructed CFloorMap into the process member
// (retail copy helper used by Init). Field widths follow the retail copy; the
// two large pair arrays use loop copies and the trailing word pair at
// +0x3340/+0x3344 is stored in reverse order.
void FloorMapCopy(CFloorMap* dest, const CFloorMap& src) {
    CFloorMapCopyView& d = *reinterpret_cast<CFloorMapCopyView*>(dest);
    const CFloorMapCopyView& s = *reinterpret_cast<const CFloorMapCopyView*>(&src);

    d.f04[0] = s.f04[0];
    d.f04[1] = s.f04[1];
    d.f04[2] = s.f04[2];
    d.f04[3] = s.f04[3];
    d.f04[4] = s.f04[4];
    d.f04[5] = s.f04[5];
    d.f04[6] = s.f04[6];
    d.f04[7] = s.f04[7];
    d.f04[8] = s.f04[8];
    d.f04[9] = s.f04[9];
    d.f04[10] = s.f04[10];
    d.f04[11] = s.f04[11];
    d.f04[12] = s.f04[12];
    d.f04[13] = s.f04[13];
    d.f04[14] = s.f04[14];
    d.f40 = s.f40;
    d.f41 = s.f41;
    d.f42 = s.f42;
    d.f43 = s.f43;
    d.f44[0] = s.f44[0];
    d.f44[1] = s.f44[1];
    d.f44[2] = s.f44[2];
    d.f44[3] = s.f44[3];
    d.f44[4] = s.f44[4];
    d.f58 = s.f58;
    d.f5A = s.f5A;
    d.f5C = s.f5C;
    d.f5D = s.f5D;
    d.f64[0] = s.f64[0];
    d.f64[1] = s.f64[1];
    d.f64[2] = s.f64[2];
    d.f64[3] = s.f64[3];
    d.f64[4] = s.f64[4];
    d.f64[5] = s.f64[5];
    d.f64[6] = s.f64[6];
    d.f64[7] = s.f64[7];
    d.f84 = s.f84;
    d.f85 = s.f85;
    d.f86 = s.f86;
    d.f87 = s.f87;
    d.f88[0] = s.f88[0];
    d.f88[1] = s.f88[1];
    d.f88[2] = s.f88[2];
    d.f88[3] = s.f88[3];
    d.f88[4] = s.f88[4];
    d.f9C = s.f9C;
    d.fA4[0] = s.fA4[0];
    d.fA4[1] = s.fA4[1];
    d.fA4[2] = s.fA4[2];
    d.fB0 = s.fB0;

    // Retail lowers these stretches as single struct assignments: named
    // members expand to per-field copies, unnamed storage is skipped, and
    // the pair arrays become ctr loops whose walkers start at the substruct
    // base (first displacement carries the array offset).
    d.block1 = s.block1;
    d.block2 = s.block2;

    d.f32CC = s.f32CC;
    d.f32D4[0] = s.f32D4[0];
    d.f32D4[1] = s.f32D4[1];
    d.f32D4[2] = s.f32D4[2];
    d.f32D4[3] = s.f32D4[3];
    d.f32E4 = s.f32E4;
    d.f32E5 = s.f32E5;
    d.f32EC[0] = s.f32EC[0];
    d.f32EC[1] = s.f32EC[1];
    d.f32EC[2] = s.f32EC[2];
    d.f32EC[3] = s.f32EC[3];
    d.f32FC = s.f32FC;
    d.f32FD = s.f32FD;
    d.f3304[0] = s.f3304[0];
    d.f3304[1] = s.f3304[1];
    d.f3304[2] = s.f3304[2];
    d.f3304[3] = s.f3304[3];
    d.f3314 = s.f3314;
    d.f3315 = s.f3315;
    d.f331C[0] = s.f331C[0];
    d.f331C[1] = s.f331C[1];
    d.f331C[2] = s.f331C[2];
    d.f331C[3] = s.f331C[3];
    d.f332C = s.f332C;
    d.f332D = s.f332D;
    d.f3330[0] = s.f3330[0];
    d.f3330[1] = s.f3330[1];
    d.f3330[2] = s.f3330[2];
    d.f333C = s.f333C;

    // Retail stores +0x3344 before +0x3340.
    u32 v3344 = s.f3344;
    u32 v3340 = s.f3340;
    d.f3344 = v3344;
    d.f3340 = v3340;
}

void CMenuMapSelect::Term() {
    waitForDrawDone__9CDeviceVIFv();
    Scn_SetPauseFlag(static_cast<CScn*>(mParent), 1);

    static_cast<CScn*>(mParent)->removeRenderCB(this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    mMapSel.term();
    (&mFade)->Unload();
    FloorMap_Teardown(&mFloorMap);

    lbl_eu_80664790 = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

// CMenuMapSelect state machine. States: 0 waits for bg/help/map resources,
// 1 waits for help idle then arms the map, 2 runs the world map handler,
// 3 waits help idle to raise mField54, 4 world map phase 2, 5 starts fade-in
// for the floor map, 6 waits for the fade, 7 floor map handler, 8 starts
// fade-out back to the world map, 9 waits for that fade, 10 exits the scene,
// 11 marks completion in mField54.
void CMenuMapSelect::Move() {
    if (CTaskGame::getInstance()->isFlag01Set() || (lbl_eu_80663E28 & 0x200000)) {
        return;
    }

    switch (mState) {
    case 0:
        if (BgTex_IsLoaded_3E34(&mBgTex) == 0) break;
        if (isInitialized(&mTitleAHelp) == 0) break;
        if (mMapSel.getVisibleFlag() == 0) break;
        func_801C412C(&mTitleAHelp);
        mMapSel.init();
        mState = 1;
        playUISound(0x6d);
        UIWin_ControlWindowLists(1, 0);
        Scn_SetPauseFlag(static_cast<CScn*>(mParent), 0);
        break;

    case 1:
        if (mTitleAHelp.isIdle() == 0) break;
        if (mMapSel.getInitFlag() == 0) break;
        mState = 2;
        break;

    case 2:
        handleWorldMapInput();
        break;

    case 3:
        if (mTitleAHelp.isIdle() == 0) break;
        if (mMapSel.getInitFlag() == 0) break;
        mField54 = 1;
        break;

    case 4:
        setupFloorMapPhase(this);
        break;

    case 5:
        if (FloorMap_GetField42Gated(&mFloorMap) == 0) break;
        func_801C4654(&mTitleAHelp, 0);
        setInfoText(&mTitleAHelp, BdatTouchStringCell(lbl_eu_8050B498, lbl_eu_8050B498 + 8, 3));
        if (CtrlRemote_TouchBitByArg((u32)0x3212) != 0) {
            func_801C46DC(&mTitleAHelp, 1);
        }
        func_801C41E8(&mTitleAHelp, 0);
        FloorMap_LoadStageLayout(&mFloorMap);
        playUISound(0x6d);
        CFade_FadeOut(&mFade);
        mState = 6;
        break;

    case 6:
        if (CFade_IsVisible(&mFade) == 0) break;
        mState = 7;
        break;

    case 7:
        handleFloorMapInput(this);
        break;

    case 8:
        if (CFade_IsVisible(&mFade) == 0) break;
        FloorMap_Teardown(&mFloorMap);
        func_801C4654(&mTitleAHelp, 1);
        setInfoText(&mTitleAHelp, BdatTouchStringCell(lbl_eu_8050B498, lbl_eu_8050B498 + 8, 3));
        func_801C46DC(&mTitleAHelp, 0);
        func_801C41E8(&mTitleAHelp, 0x4a);
        CFade_FadeOut(&mFade);
        mState = 9;
        break;

    case 9:
        if (CFade_IsVisible(&mFade) == 0) break;
        mState = 2;
        break;

    case 10:
        if (CFade_IsVisible(&mFade) == 0) break;
        func_8008413C__Q22cf13CfGameManagerFv(FloorMap_GetU16Field5A(&mFloorMap), 0);
        mState = 0xb;
        if (CMainMenu_IsOpen()) {
            CMainMenu_GetInstancePtr();
            ArtsInfo_SetReadyFlag();
        }
        break;

    case 11:
        mField54 = 1;
        break;
    }

    if (FloorMap_GetField40Flag(&mFloorMap) == 0) {
        BgTex_Tick_3D54(&mBgTex);
        mMapSel.update();
    }
    updateHelp(&mTitleAHelp);
    CFade_Update(&mFade);
    FloorMap_UpdateFrame(&mFloorMap);
}

// CMenuMapSelect::cbRenderBefore - scene render callback. Skips drawing while
// the game task is busy or the global bit-21 busy flag is set, then draws bg,
// map selector, floor map, title help and fade through a stack DrawInfo.
void CMenuMapSelect::cbRenderBefore() {
    if (CTaskGame::getInstance()->isFlag01Set() != 0 || (lbl_eu_80663E28 & 0x200000)) return;
    if (IsMenuState621F0() == 0) return;
    if (mState >= 0xb) return;

    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw 0x60-byte buffer: a typed local would make MWCC emit its own
    // construction; retail calls the ctor/dtor symbols directly (flag -1).
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    if (FloorMap_GetField40Flag(&mFloorMap) == 0) {
        BgTex_Draw_3D7C(&mBgTex, &drawInfo);
        mMapSel.draw(&drawInfo);
    }
    FloorMap_Draw(&mFloorMap, &drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
    CFade_Draw(&mFade, &drawInfo);
}

// ---------------------------------------------------------------------------
// us-802443d4 -- CMenuMapCreate: singleton factory.
// Allocates the process from the work heap, constructs it, registers it under
// the parent and returns the singleton (null if it already exists).
// ---------------------------------------------------------------------------
CMenuMapSelect* CMenuMapSelect::create(CProcess* parent, CProcess* parentRef) {
    if (sInstance != nullptr) {
        return nullptr;
    }

    CMenuMapSelect* menu = new (CWorkThreadSystem::getWorkMem()) CMenuMapSelect(parentRef);
    sInstance = menu;
    menu->Regist(parent, false);
    return sInstance;
}

// ---------------------------------------------------------------------------
// isMapSelectActive -- singleton guard
// ---------------------------------------------------------------------------
extern "C" u32 isMapSelectActive() {
    return CMenuMapSelect::sInstance != nullptr;
}

// ---------------------------------------------------------------------------
// us-80244520 -- handleWorldMapInput: world map input handler (phase gate).
// Reads turbo-press flags plus one controller-dependent trigger bit triple,
// then dispatches cursor movement / cancel / help / confirm actions.
// ---------------------------------------------------------------------------
void CMenuMapSelect::handleWorldMapInput() {
    CfPadDataView* padData = (CfPadDataView*)cf::CfGameManager::getCfPadData();

    // Trigger bits and masked-button booleans are extracted identically for
    // both layouts except for the three pressed-flag bit positions:
    // Classic controller uses bits 21-23, Wiimote/Nunchuk bits 4/5/10.
    u32 triggerBit1, triggerBit2, triggerBit3;
    u32 cancel, confirm, stickDir, menu;
    u32 turboFlags, pressedFlags, cancelVal, confirmVal, menuVal, stickVal;
    int classic = cf::CfGameManager::isClassicController(-1);
    if (classic != 0) {
        turboFlags = padData->mTurboPressButtonFlags;
        pressedFlags = padData->mPadPressedFlags;

        u32 cancelVal = turboFlags & 0x8004;
        u32 confirmVal = turboFlags & 0x2001;
        u32 menuVal = turboFlags & 0x4002;
        u32 stickVal = (turboFlags & 0x10000) | (turboFlags & 0x8);

        triggerBit1 = (pressedFlags >> 21) & 1;
        // (x | -x) >> 31 normalizes any non-zero mask to 1.
        cancel = (cancelVal | -(s32)cancelVal) >> 31;
        confirm = (confirmVal | -(s32)confirmVal) >> 31;
        menu = (menuVal | -(s32)menuVal) >> 31;
        stickDir = (stickVal | -(s32)stickVal) >> 31;
        triggerBit2 = (pressedFlags >> 22) & 1;
        triggerBit3 = (pressedFlags >> 23) & 1;
    } else {
        turboFlags = padData->mTurboPressButtonFlags;
        pressedFlags = padData->mPadPressedFlags;

        u32 cancelVal = turboFlags & 0x8004;
        u32 confirmVal = turboFlags & 0x2001;
        u32 menuVal = turboFlags & 0x4002;
        u32 stickVal = (turboFlags & 0x10000) | (turboFlags & 0x8);

        triggerBit1 = (pressedFlags >> 4) & 1;
        cancel = (cancelVal | -(s32)cancelVal) >> 31;
        confirm = (confirmVal | -(s32)confirmVal) >> 31;
        menu = (menuVal | -(s32)menuVal) >> 31;
        stickDir = (stickVal | -(s32)stickVal) >> 31;
        triggerBit2 = (pressedFlags >> 5) & 1;
        triggerBit3 = (pressedFlags >> 10) & 1;
    }

    if (triggerBit1 != 0) {
        // Cancel/back: fade out and leave the world map.
        (&this->mFade)->In();
        this->mState = 4;
        playUISound(3);
    } else if (triggerBit2 != 0) {
        // Show help overlay.
        beginClose(&this->mTitleAHelp);
        mMapSel.beginClose();
        this->mState = 3;
    } else if (cancel != 0) {
        mMapSel.moveUp();
    } else if (stickDir != 0) {
        mMapSel.moveDown();
    } else if (confirm != 0) {
        mMapSel.movePageUp();
    } else if (menu != 0) {
        mMapSel.movePageDown();
    } else if (triggerBit3 != 0) {
        // Confirm selection.
        if (CMainMenu_GetInstancePtr() != 0) {
            ArtsInfo_SetReadyFlag();
            playUISound(6);
        }
        this->mState = 11;
    }
}

// ---------------------------------------------------------------------------
// us-802446dc -- setupFloorMapPhase: world map phase 2 -> floor map setup.
// Once the fade reports ready, build a fresh CFloorMap in a stack temporary,
// assign it over the member (implicit operator= skips the vptr), select the
// current map and advance to phase 5.
// ---------------------------------------------------------------------------
void setupFloorMapPhase(CMenuMapSelect* self) {
    if (CFade_IsVisible(&self->mFade) != 0) {
        self->mFloorMap = CFloorMap();

        u8 sel = self->mMapSel.getMapGridCell();
        (&self->mFloorMap)->SetMapTable(sel);
        FloorMap_LoadMapFiles(&self->mFloorMap);
        self->mState = 5;
    }
}

// ---------------------------------------------------------------------------
// us-80244be0 -- handleFloorMapInput: floor map input handler (phase 5).
// Advances the animation timer, refreshes the help bar text, then dispatches
// zoom / cursor / selection actions based on the active controller layout.
// ---------------------------------------------------------------------------
void handleFloorMapInput(CMenuMapSelect* self) {
    if (FloorMap_ClassifyByte41(&self->mFloorMap) != 0) {
        return;
    }

    self->mTimer += 1.0f;
    if (self->mTimer > 10.0f) {
        self->mTimer = 10.0f;
    }

    if (cf::CfGameManager::isClassicController(-1) != 0) {
        // Classic controller layout.
        if (FloorMap_CheckRowVisited(&self->mFloorMap) != 0) {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4b);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4c);
            }
        } else {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4d);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4e);
            }
        }

        // Zoom is only allowed while the floor map is fully shown and idle.
        u32 zoomReady = 0;
        u32 zooming = 0;
        if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Gated(&self->mFloorMap) != 0) {
            zooming = 1;
        }
        if (zooming != 0 && FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
            zoomReady = 1;
        }

        FloorMap_SetPaneFlag(&self->mFloorMap, 0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        if ((pad->mTurboPressButtonFlags & 0x02000000) != 0 && zoomReady != 0) {
            FloorMap_UpdateRowPanePos(&self->mFloorMap);
            return;
        }
        if ((pad->mTurboPressButtonFlags & 0x04000000) != 0 && zoomReady != 0) {
            FloorMap_CursorNextFloor(&self->mFloorMap);
            return;
        }

        u32 pressed = pad->mPressedButtonFlags;
        if ((pressed & 0x00200000) != 0) {
            FloorMap_OpenRowWindow(&self->mFloorMap);
            if (FloorMap_GetU16Field5A(&self->mFloorMap) != 0) {
                (&self->mFade)->In();
                self->mState = 10;
            }
        } else if ((pressed & 0x00400000) != 0) {
            if (self->mFloorMap.GetActiveFlag58() != 0) {
                FloorMap_ConfirmSelect(&self->mFloorMap);
            } else if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                (&self->mFade)->In();
                self->mState = 8;
                playUISound(6);
            } else {
                FloorMap_OpenRowWindow(&self->mFloorMap);
            }
        } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
            FloorMap_UpdateCursorMove(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
            FloorMap_UpdateWorldMap(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
            FloorMap_HandleMapDown(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
            FloorMap_NormalizeMapPos(&self->mFloorMap);
        } else if ((pressed & 0x01000000) != 0) {
            FloorMap_Log2Field208(&self->mFloorMap);
        } else if ((pressed & 0x200) != 0) {
            FloorMap_Log2Field333C(&self->mFloorMap);
        } else if ((pressed & 0x800000) != 0) {
            if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                if (CMainMenu_GetInstancePtr() != 0) {
                    ArtsInfo_SetReadyFlag();
                    playUISound(6);
                }
                self->mState = 11;
            } else {
                FloorMap_OpenRowWindow(&self->mFloorMap);
            }
        }
    } else {
        // Wiimote/Nunchuk layout.
        if (FloorMap_CheckRowVisited(&self->mFloorMap) != 0) {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4b);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4c);
            }
        } else {
            if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Plain(&self->mFloorMap) != 0) {
                func_801C41E8(&self->mTitleAHelp, 0x4d);
            } else {
                func_801C41E8(&self->mTitleAHelp, 0x4e);
            }
        }

        u32 zooming = 0;
        u32 zoomReady = 0;
        if (FloorMap_GetBdatMaxRow(&self->mFloorMap) > 1 && FloorMap_GetField208Gated(&self->mFloorMap) != 0) {
            zooming = 1;
        }
        if (zooming != 0 && FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
            zoomReady = 1;
        }

        FloorMap_SetPaneFlag(&self->mFloorMap, 0);

        CPad* pad = cf::CfGameManager::getCurrentPad();
        u32 pressed = pad->mPressedButtonFlags;
        if ((pressed & 0x10) != 0) {
            FloorMap_OpenRowWindow(&self->mFloorMap);
            if (FloorMap_GetU16Field5A(&self->mFloorMap) != 0) {
                (&self->mFade)->In();
                self->mState = 10;
            }
        } else if ((pressed & 0x20) != 0) {
            if (self->mFloorMap.GetActiveFlag58() != 0) {
                FloorMap_ConfirmSelect(&self->mFloorMap);
            } else if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                (&self->mFade)->In();
                self->mState = 8;
                playUISound(6);
            } else {
                FloorMap_OpenRowWindow(&self->mFloorMap);
            }
        } else if ((pad->mHeldButtonFlags & 0x1000) != 0 && zooming != 0 &&
                   FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
            func_801C41E8(&self->mTitleAHelp, 0x4f);
            if (self->mTimer > 1.0f) {
                playUISound(2);
            }
            self->mTimer = 0.0f;
            FloorMap_SetPaneFlag(&self->mFloorMap, 1);
            if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
                FloorMap_UpdateRowPanePos(&self->mFloorMap);
            } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
                FloorMap_CursorNextFloor(&self->mFloorMap);
            }
        } else if ((pressed & 0x400) != 0) {
            if (FloorMap_IsSysWinBusy(&self->mFloorMap) == 0) {
                if (CMainMenu_GetInstancePtr() != 0) {
                    ArtsInfo_SetReadyFlag();
                    playUISound(6);
                }
                self->mState = 11;
            } else {
                FloorMap_OpenRowWindow(&self->mFloorMap);
            }
        } else if ((pad->mTurboPressButtonFlags & 0x4) != 0) {
            FloorMap_UpdateCursorMove(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x8) != 0) {
            FloorMap_UpdateWorldMap(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x80000000) != 0) {
            FloorMap_HandleMapDown(&self->mFloorMap);
        } else if ((pad->mTurboPressButtonFlags & 0x2) != 0) {
            FloorMap_NormalizeMapPos(&self->mFloorMap);
        } else if ((pressed & 0x800) != 0) {
            FloorMap_Log2Field208(&self->mFloorMap);
        } else if ((pressed & 0x200) != 0) {
            FloorMap_Log2Field333C(&self->mFloorMap);
        } else {
            // D-pad held: each direction moves the cursor once per press
            // transition; when no direction is held, confirm the selection.
            u32 flag = 1;
            if ((pad->mHeldButtonFlags & 0x8000) != 0) {
                FloorMap_ScrollDown(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x10000) != 0) {
                FloorMap_ScrollUp(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x2000) != 0) {
                FloorMap_ScrollRight(&self->mFloorMap);
                flag = 0;
            }
            if ((pad->mHeldButtonFlags & 0x4000) != 0) {
                FloorMap_ScrollLeft(&self->mFloorMap);
                flag = 0;
            }
            if (flag != 0) {
                FloorMap_MoveToCursorRow(&self->mFloorMap);
            }
        }
    }
}
