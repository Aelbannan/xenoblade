// Decomp of kyoshin/menu/CMenuShopBuy.
// Shop buy menu screen process (CProcess-header + IScnRender MI layout).

#include <nw4r/lyt/lyt_drawInfo.h>
#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CTaskGame.hpp"

#include "kyoshin/menu/CMenuShopBuy.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <revolution/gx/GXPixel.h>
#include "kyoshin/CTitleAHelpImports.hpp"
void playUISound(u32);

// Same-TU blob copy helper (defined below); Init uses it twice.
extern "C" void ShopBuy_CopyData(ShopBuyData* dst, const ShopBuyData* src);

/*
 * Constructor. The IUICf base (CTTask + mField54/mField55) and the IScnRender
 * sub-object are constructed by the compiler; the members are initialised in
 * declaration order, then the item box line is built in place (raw storage),
 * then the scalar tail (state 0 + float init).
 */
CMenuShopBuy::CMenuShopBuy(CScn* scene, u32 arg)
    : mScene(scene), mField60(arg), mBgTex(0), mTitleAHelp(0, 0) {
    new (mItemBoxLine) CItemBoxLine(0, 0);

    mState = 0;
    mFloat6FC = lbl_eu_80667A20;
}

/*
 * Destructor: tears down the item box line (raw storage, so it is not a
 * member). The CBgTex / CTitleAHelp members and the IUICf / IScnRender bases
 * are destroyed by the compiler after this body.
 */
CMenuShopBuy::~CMenuShopBuy() {
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->CItemBoxLine::~CItemBoxLine();
}

void CMenuShopBuy::Init() {
    setPresentationFlag__Q22cf13CfGameManagerFv(1);

    // Build a temporary CBgTex (retail unmangled ctor) and copy its data
    // fields into the member, then destroy the temp (CMenuMapSelect pattern).
    // All member/temp addresses stay inline so MWCC recomputes the addi per
    // use (retail keeps only r31=self and r30 scratch - no cached pointers).
    mBgTex = CBgTex(0);

    BgTex_Acquire_3C14(&mBgTex);

    mTitleAHelp = CTitleAHelp(BdatTouchStringCell(lbl_eu_80503B20, lbl_eu_80503B20 + 9, 1), 0);

    mTitleAHelp.CTitleAHelp_load();

    // Build a temporary CItemBoxLine (retail unmangled ctor with the u16
    // arg0 and file-id 1) and copy its whole data region (vtable excluded)
    // field-by-field into the embedded member, then destroy the temp. The
    // typed temp and union member keep every access self/sp-relative so MWCC
    // needs no cached base pointers (retail saves only r30/r31).
    ShopBuyItemBoxLine tmpLine;
    new (&tmpLine) CItemBoxLine((u16)mField60, 1);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mItemBox.mIBLData.mRegion04, &tmpLine.mData.mRegion04);
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mItemBox.mIBLData.mRegion14, &tmpLine.mData.mRegion14);
    mItemBox.mIBLData.field_0x20[0] = tmpLine.mData.field_0x20[0];
    mItemBox.mIBLData.field_0x20[1] = tmpLine.mData.field_0x20[1];
    mItemBox.mIBLData.field_0x20[2] = tmpLine.mData.field_0x20[2];
    mItemBox.mIBLData.field_0x20[3] = tmpLine.mData.field_0x20[3];
    mItemBox.mIBLData.field_0x20[4] = tmpLine.mData.field_0x20[4];
    mItemBox.mIBLData.field_0x20[5] = tmpLine.mData.field_0x20[5];
    mItemBox.mIBLData.field_0x20[6] = tmpLine.mData.field_0x20[6];
    mItemBox.mIBLData.field_0x20[7] = tmpLine.mData.field_0x20[7];
    mItemBox.mIBLData.field_0x20[8] = tmpLine.mData.field_0x20[8];
    mItemBox.mIBLData.field_0x20[9] = tmpLine.mData.field_0x20[9];
    mItemBox.mIBLData.field_0x48 = tmpLine.mData.field_0x48;
    mItemBox.mIBLData.field_0x4C = tmpLine.mData.field_0x4C;
    mItemBox.mIBLData.field_0x50 = tmpLine.mData.field_0x50;
    mItemBox.mIBLData.field_0x54 = tmpLine.mData.field_0x54;
    mItemBox.mIBLData.unk59 = tmpLine.mData.unk59;
    mItemBox.mIBLData.tabEntries10 = tmpLine.mData.tabEntries10;
    mItemBox.mIBLData.pad6410 = tmpLine.mData.pad6410;
    MenuShopSellCopyCursor(&mItemBox.mIBLData.cursors[0], &tmpLine.mData.cursors[0]);
    MenuShopSellCopyCursor(&mItemBox.mIBLData.cursors[1], &tmpLine.mData.cursors[1]);
    MenuShopSellCopyCursor(&mItemBox.mIBLData.cursors[2], &tmpLine.mData.cursors[2]);
    MenuShopSellCopyCursor(&mItemBox.mIBLData.cursors[3], &tmpLine.mData.cursors[3]);
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mItemBox.mIBLData.mRegionD4, &tmpLine.mData.mRegionD4);
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mItemBox.mIBLData.mRegionE4, &tmpLine.mData.mRegionE4);
    mItemBox.mIBLData.field_0xF0[0] = tmpLine.mData.field_0xF0[0];
    mItemBox.mIBLData.field_0xF0[1] = tmpLine.mData.field_0xF0[1];
    mItemBox.mIBLData.field_0xF0[2] = tmpLine.mData.field_0xF0[2];
    mItemBox.mIBLData.field_0xF0[3] = tmpLine.mData.field_0xF0[3];
    mItemBox.mIBLData.field_0xF0[4] = tmpLine.mData.field_0xF0[4];
    mItemBox.mIBLData.field_0xF0[5] = tmpLine.mData.field_0xF0[5];
    mItemBox.mIBLData.field_0xF0[6] = tmpLine.mData.field_0xF0[6];
    mItemBox.mIBLData.field_0xF0[7] = tmpLine.mData.field_0xF0[7];
    mItemBox.mIBLData.field_0xF0[8] = tmpLine.mData.field_0xF0[8];
    mItemBox.mIBLData.field_0xF0[9] = tmpLine.mData.field_0xF0[9];
    mItemBox.mIBLData.field_0xF0[10] = tmpLine.mData.field_0xF0[10];
    mItemBox.mIBLData.field_0xF0[11] = tmpLine.mData.field_0xF0[11];
    mItemBox.mIBLData.field_0xF0[12] = tmpLine.mData.field_0xF0[12];
    mItemBox.mIBLData.field_0xF0[13] = tmpLine.mData.field_0xF0[13];
    mItemBox.mIBLData.field_0xF0[14] = tmpLine.mData.field_0xF0[14];
    mItemBox.mIBLData.field_0xF0[15] = tmpLine.mData.field_0xF0[15];
    mItemBox.mIBLData.field_0xF0[16] = tmpLine.mData.field_0xF0[16];
    mItemBox.mIBLData.field_0xF0[17] = tmpLine.mData.field_0xF0[17];
    mItemBox.mIBLData.field_0xF0[18] = tmpLine.mData.field_0xF0[18];
    mItemBox.mIBLData.field_0xF0[19] = tmpLine.mData.field_0xF0[19];
    mItemBox.mIBLData.field_0xF0[20] = tmpLine.mData.field_0xF0[20];
    mItemBox.mIBLData.field_0xF0[21] = tmpLine.mData.field_0xF0[21];
    mItemBox.mIBLData.field_0xF0[22] = tmpLine.mData.field_0xF0[22];
    mItemBox.mIBLData.field_0xF0[23] = tmpLine.mData.field_0xF0[23];
    mItemBox.mIBLData.field_0xF0[24] = tmpLine.mData.field_0xF0[24];
    mItemBox.mIBLData.field_0xF0[25] = tmpLine.mData.field_0xF0[25];
    mItemBox.mIBLData.field_0xF0[26] = tmpLine.mData.field_0xF0[26];
    mItemBox.mIBLData.field_0x15C = tmpLine.mData.field_0x15C;
    mItemBox.mIBLData.field_0x160 = tmpLine.mData.field_0x160;
    mItemBox.mIBLData.field_0x164 = tmpLine.mData.field_0x164;
    mItemBox.mIBLData.field_0x165 = tmpLine.mData.field_0x165;
    mItemBox.mIBLData.field_0x166 = tmpLine.mData.field_0x166;
    mItemBox.mIBLData.chunk168 = tmpLine.mData.chunk168;
    mItemBox.mIBLData.chunk170 = tmpLine.mData.chunk170;
    mItemBox.mIBLData.field_0x178 = tmpLine.mData.field_0x178;
    ShopBuy_CopyData(&mItemBox.mIBLData.blob17C, &tmpLine.mData.blob17C);

    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mItemBox.mIBLData.mRegion2DC, &tmpLine.mData.mRegion2DC);
    mItemBox.mIBLData.field_0x2EC[0] = tmpLine.mData.field_0x2EC[0];
    mItemBox.mIBLData.field_0x2EC[1] = tmpLine.mData.field_0x2EC[1];
    mItemBox.mIBLData.field_0x2EC[2] = tmpLine.mData.field_0x2EC[2];
    mItemBox.mIBLData.field_0x2EC[3] = tmpLine.mData.field_0x2EC[3];
    mItemBox.mIBLData.field_0x2EC[4] = tmpLine.mData.field_0x2EC[4];
    mItemBox.mIBLData.field_0x2EC[5] = tmpLine.mData.field_0x2EC[5];
    mItemBox.mIBLData.field_0x304[0] = tmpLine.mData.field_0x304[0];
    mItemBox.mIBLData.field_0x304[1] = tmpLine.mData.field_0x304[1];
    mItemBox.mIBLData.field_0x304[2] = tmpLine.mData.field_0x304[2];
    mItemBox.mIBLData.field_0x304[3] = tmpLine.mData.field_0x304[3];
    mItemBox.mIBLData.field_0x304[4] = tmpLine.mData.field_0x304[4];
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mItemBox.mIBLData.mRegion310, &tmpLine.mData.mRegion310);
    mItemBox.mIBLData.field_0x320[0] = tmpLine.mData.field_0x320[0];
    mItemBox.mIBLData.field_0x320[1] = tmpLine.mData.field_0x320[1];
    mItemBox.mIBLData.field_0x320[2] = tmpLine.mData.field_0x320[2];
    mItemBox.mIBLData.field_0x320[3] = tmpLine.mData.field_0x320[3];
    mItemBox.mIBLData.field_0x330[0] = tmpLine.mData.field_0x330[0];
    mItemBox.mIBLData.field_0x330[1] = tmpLine.mData.field_0x330[1];
    mItemBox.mIBLData.field_0x330[2] = tmpLine.mData.field_0x330[2];
    mItemBox.mIBLData.field_0x330[3] = tmpLine.mData.field_0x330[3];
    mItemBox.mIBLData.field_0x334[0] = tmpLine.mData.field_0x334[0];
    mItemBox.mIBLData.field_0x334[1] = tmpLine.mData.field_0x334[1];
    mItemBox.mIBLData.field_0x334[2] = tmpLine.mData.field_0x334[2];
    mItemBox.mIBLData.field_0x334[3] = tmpLine.mData.field_0x334[3];
    mItemBox.mIBLData.field_0x334[4] = tmpLine.mData.field_0x334[4];
    mItemBox.mIBLData.field_0x348 = tmpLine.mData.field_0x348;
    __as__17UnkClass_8045F564FRC17UnkClass_8045F564(&mItemBox.mIBLData.mRegion350, &tmpLine.mData.mRegion350);
    mItemBox.mIBLData.field_0x360[0] = tmpLine.mData.field_0x360[0];
    mItemBox.mIBLData.field_0x360[1] = tmpLine.mData.field_0x360[1];
    mItemBox.mIBLData.field_0x360[2] = tmpLine.mData.field_0x360[2];
    mItemBox.mIBLData.field_0x360[3] = tmpLine.mData.field_0x360[3];
    mItemBox.mIBLData.field_0x360[4] = tmpLine.mData.field_0x360[4];
    mItemBox.mIBLData.field_0x374[0] = tmpLine.mData.field_0x374[0];
    mItemBox.mIBLData.field_0x374[1] = tmpLine.mData.field_0x374[1];
    mItemBox.mIBLData.field_0x374[2] = tmpLine.mData.field_0x374[2];
    mItemBox.mIBLData.field_0x374[3] = tmpLine.mData.field_0x374[3];
    mItemBox.mIBLData.field_0x374[4] = tmpLine.mData.field_0x374[4];
    mItemBox.mIBLData.field_0x374[5] = tmpLine.mData.field_0x374[5];
    mItemBox.mIBLData.field_0x374[6] = tmpLine.mData.field_0x374[6];
    mItemBox.mIBLData.field_0x374[7] = tmpLine.mData.field_0x374[7];
    mItemBox.mIBLData.field_0x374[8] = tmpLine.mData.field_0x374[8];
    mItemBox.mIBLData.field_0x374[9] = tmpLine.mData.field_0x374[9];
    mItemBox.mIBLData.field_0x374[10] = tmpLine.mData.field_0x374[10];
    mItemBox.mIBLData.field_0x374[11] = tmpLine.mData.field_0x374[11];
    mItemBox.mIBLData.field_0x374[12] = tmpLine.mData.field_0x374[12];
    mItemBox.mIBLData.field_0x374[13] = tmpLine.mData.field_0x374[13];
    mItemBox.mIBLData.field_0x374[14] = tmpLine.mData.field_0x374[14];
    mItemBox.mIBLData.field_0x374[15] = tmpLine.mData.field_0x374[15];
    mItemBox.mIBLData.field_0x374[16] = tmpLine.mData.field_0x374[16];
    mItemBox.mIBLData.field_0x374[17] = tmpLine.mData.field_0x374[17];
    mItemBox.mIBLData.unk38C = tmpLine.mData.unk38C;
    mItemBox.mIBLData.unk38E = tmpLine.mData.unk38E;
    mItemBox.mIBLData.field_0x38C = tmpLine.mData.field_0x38C;
    mItemBox.mIBLData.field_0x38E = tmpLine.mData.field_0x38E;
    mItemBox.mIBLData.field_0x390 = tmpLine.mData.field_0x390;
    mItemBox.mIBLData.chunk392 = tmpLine.mData.chunk392;
    mItemBox.mIBLData.field_0x39A[0] = tmpLine.mData.field_0x39A[0];
    mItemBox.mIBLData.field_0x39A[1] = tmpLine.mData.field_0x39A[1];
    mItemBox.mIBLData.field_0x39A[2] = tmpLine.mData.field_0x39A[2];
    mItemBox.mIBLData.field_0x39A[3] = tmpLine.mData.field_0x39A[3];
    mItemBox.mIBLData.field_0x39A[4] = tmpLine.mData.field_0x39A[4];
    mItemBox.mIBLData.field_0x39A[5] = tmpLine.mData.field_0x39A[5];
    mItemBox.mIBLData.data3A0 = tmpLine.mData.data3A0;
    mItemBox.mIBLData.tabCount = tmpLine.mData.tabCount;
    mItemBox.mIBLData.field_0x432 = tmpLine.mData.field_0x432;
    mItemBox.mIBLData.field_0x433 = tmpLine.mData.field_0x433;
    mItemBox.mIBLData.len454 = strlen(tmpLine.mData.str434);
    strcpy(mItemBox.mIBLData.str434, tmpLine.mData.str434);
    mItemBox.mIBLData.len4D8 = strlen(tmpLine.mData.str458);
    strcpy(mItemBox.mIBLData.str458, tmpLine.mData.str458);
    ShopBuy_CopyData(&mItemBox.mIBLData.blob4DC, &tmpLine.mData.blob4DC);
    __dt__12CItemBoxLineFv(reinterpret_cast<CItemBoxLine*>(&tmpLine), -1);

    // Reset the real member and register the render callback (same
    // `if (this)` IScnRender idiom as Term).
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->ResetTabBytes();
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->PushTabByte(2);
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->PushTabByte(4);
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->PushTabByte(5);
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->PushTabByte(6);
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->PushTabByte(7);
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->PushTabByte(8);
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->PushTabByte(0xd);
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->LoadFiles();

    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, this, 0xd, 0);
}

// Tear down the shop buy menu: detach the render callback, release the
// background/title widgets and the item box line, clear the singleton, and
// hand control back to the game manager.
void CMenuShopBuy::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    removeRenderCB__4CScnFP10IScnRender(mScene, this);

    BgTex_Release_3D9C(&mBgTex);
    teardown(&mTitleAHelp);
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->UnloadFiles();

    lbl_eu_806642F8 = 0;
    setPresentationFlag__Q22cf13CfGameManagerFv(0);
}

void CMenuShopBuy::Move() {
    // Single short-circuit OR so MWCC emits: func test -> bne exit;
    // bit test -> beq continue / b exit (CSystemWindow::Move shape).
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0:
        ShopBuy_OpenPhase(this);
        break;
    case 1:
        ShopBuy_AdvancePhase(this);
        break;
    case 2:
        ShopBuy_HandleInput(this);
        break;
    case 3:
        ShopBuy_MarkClosing(this);
        break;
    default:
        break;
    }

    BgTex_Tick_3D54(&mBgTex);
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->UpdateStates();
    updateHelp(&mTitleAHelp);
}

void CMenuShopBuy::cbRenderBefore() {
    // Same single-OR guard shape as Move / CSystemWindow::cbRenderBefore.
    if (CTaskGame::getInstance()->isFlag01Set() ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (IsMenuState621F0() == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    nw4r::lyt::DrawInfo drawInfo;
    SetWideLocAdjust(&drawInfo);
    BgTex_Draw_3D7C(&mBgTex, &drawInfo);
    reinterpret_cast<CItemBoxLine*>(mItemBoxLine)->DrawLayout(&drawInfo);
    drawHelp(&mTitleAHelp, &drawInfo);
}

// Copy the 0x159-byte shop-buy data blob field-by-field. Each sub-struct
// assignment comes out as a mtctr/lwzu/stwu counted loop; the u32 slot group
// is unrolled into 6 word pairs and the byte is a single lbz/stb.
extern "C" void ShopBuy_CopyData(ShopBuyData* dst, const ShopBuyData* src) {
    dst->m00 = src->m00;
    dst->m18 = src->m18;
    // Six independent word slots: MWCC schedules these as interleaved
    // r8/r5 load/store pairs rather than a counted copy loop.
    dst->mA8[0] = src->mA8[0];
    dst->mA8[1] = src->mA8[1];
    dst->mA8[2] = src->mA8[2];
    dst->mA8[3] = src->mA8[3];
    dst->mA8[4] = src->mA8[4];
    dst->mA8[5] = src->mA8[5];
    dst->mC0 = src->mC0;
    dst->mD8 = src->mD8;
    dst->mD9 = src->mD9;
}

// Shop-buy menu factory: if no instance exists yet, allocate 0x700 bytes from
// the work heap, construct the CMenuShopBuy, publish it to the singleton flag
// and register it as a CProcess child of `parent`. Returns the singleton (or 0
// when one already exists).
extern "C" CMenuShopBuy* ShopBuy_CreateMenu(CProcess* parent, CScn* scene, u32 arg) {
    if (lbl_eu_806642F8 != 0)
        return 0;

    CMenuShopBuy* obj = (CMenuShopBuy*)mtl::MemManager::allocate(
        0x700, CWorkThreadSystem::getWorkMem());
    // Placement new supplies the null check; reassigning from its result keeps
    // obj in volatile r3 (retail never spills it to a callee-saved register).
    obj = new (obj) CMenuShopBuy(scene, arg);
    lbl_eu_806642F8 = (u32)obj;
    obj->Regist(parent, 0);
    return (CMenuShopBuy*)lbl_eu_806642F8;
}

// (lbl_eu_806642F8 != 0)
extern "C" bool ShopBuy_HasInstance() { return lbl_eu_806642F8 != 0; }

// Phase 0 -> 1 (open): once the background, title bar and item box line are
// all ready, run the open sequence (title bar + item box line) and play the
// open sound, then advance the phase byte at 0x6F8.
extern "C" __declspec(noinline) void ShopBuy_OpenPhase(CMenuShopBuy* self) {
    if (BgTex_IsLoaded_3E34(&self->mBgTex) != 0) {
        if (isInitialized(&self->mTitleAHelp) != 0) {
            if (reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->GetSelectReady() != 0) {
                func_801C412C(&self->mTitleAHelp);
                reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->OpenLine();
                self->mState = 1;
                playUISound(0x6d);
            }
        }
    }
}

// Phase 1 -> 2 (advance): once the title bar is idle and the item box line
// is ready, move to the next phase.
extern "C" __declspec(noinline) void ShopBuy_AdvancePhase(CMenuShopBuy* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->IsReadyFlag() != 0) {
            self->mState = 2;
        }
    }
}

// Phase 2 (interaction): poll the pad and drive the item-box line. The two
// branches differ only in the flag layout (input-enabled vs pointer-input
// controller); each handler ends by jumping to the shared focus-refresh.
extern "C" __declspec(noinline) void ShopBuy_HandleInput(CMenuShopBuy* self) {
    if (MenuTutorialIsCreated() != 0) return;

    // Timer clamped to lbl_eu_80667A28, reset to lbl_eu_80667A20 on confirm.
    self->mFloat6FC += lbl_eu_80667A24;
    if (self->mFloat6FC > lbl_eu_80667A28) self->mFloat6FC = lbl_eu_80667A28;

    ShopBuyPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    if (isClassicController__Q22cf13CfGameManagerFv(-1) != 0) {
        // Pointer input enabled: turbo flags move the cursor, pressed flags
        // run the confirm/cancel flow. The item-box pointer is recomputed per
        // call (addi r3,r31,0xbc) exactly like retail - no cached local.
        reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->ResetCursorB8(0);
        if ((pad->mTurboFlags & 0x02000000) &&
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->IsBusy() == 0 &&
            code80135FDC_getByte_64077() > 1) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->Info2SelectPrev();
        } else if ((pad->mTurboFlags & 0x04000000) &&
                   reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->IsBusy() == 0 &&
                   code80135FDC_getByte_64077() > 1) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->Info2SelectNext();
        } else if (pad->mPressedFlags & 0x00200000) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->ResolveEntry();
        } else if (pad->mPressedFlags & 0x00400000) {
            if (reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->IsBusy() != 0) {
                reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->ConfirmOverlayOrHint();
            } else {
                beginClose(&self->mTitleAHelp);
                reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->LeaveState3();
                self->mState = 3;
            }
        } else if (pad->mTurboFlags & 0x8004) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->CursorPageBack();
        } else if ((pad->mTurboFlags & 0x10000) != 0 || (pad->mTurboFlags & 0x8) != 0) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->CursorPageDown();
        } else if (pad->mTurboFlags & 0x2001) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->CursorPageUp();
        } else if (pad->mTurboFlags & 0x4002) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->CursorPageDownRow();
        } else if (pad->mPressedFlags & 0x200) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->ShowDetailWin();
        } else if (pad->mPressedFlags & 0x400) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->RefreshTabSelect();
        }
    } else {
        // Pointer input disabled: pad-held/pressed bits drive the flow, and
        // the inner A/B chain exits straight to the shared refresh.
        reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->ResetCursorB8(0);
        if ((pad->mHeldFlags & 0x1000) &&
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->IsBusy() == 0 &&
            code80135FDC_getByte_64077() > 1) {
            if (self->mFloat6FC > lbl_eu_80667A24) playUISound(2);
            self->mFloat6FC = lbl_eu_80667A20;
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->ResetCursorB8(1);
            if (pad->mTurboFlags & 0x2001) {
                reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->Info2SelectPrev();
            } else if (pad->mTurboFlags & 0x4002) {
                reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->Info2SelectNext();
            }
        } else if (pad->mPressedFlags & 0x10) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->ResolveEntry();
        } else if (pad->mPressedFlags & 0x20) {
            if (reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->IsBusy() != 0) {
                reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->ConfirmOverlayOrHint();
            } else {
                beginClose(&self->mTitleAHelp);
                reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->LeaveState3();
                self->mState = 3;
            }
        } else if (pad->mTurboFlags & 0x8004) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->CursorPageBack();
        } else if ((pad->mTurboFlags & 0x10000) != 0 || (pad->mTurboFlags & 0x8) != 0) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->CursorPageDown();
        } else if (pad->mTurboFlags & 0x2001) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->CursorPageUp();
        } else if (pad->mTurboFlags & 0x4002) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->CursorPageDownRow();
        } else if (pad->mPressedFlags & 0x200) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->ShowDetailWin();
        } else if (pad->mPressedFlags & 0x40) {
            reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->RefreshTabSelect();
        }
    }
    func_801C41E8(&self->mTitleAHelp,
                  reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->ResolveFocusId());
}

// Close: same idle+ready guard as ShopBuy_AdvancePhase, but marks the closing state
// byte at 0x54 instead of advancing the phase byte.
extern "C" __declspec(noinline) void ShopBuy_MarkClosing(CMenuShopBuy* self) {
    if (self->mTitleAHelp.isIdle() != 0) {
        if (reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)->IsReadyFlag() != 0) {
            self->mField54 = 1;
        }
    }
}
