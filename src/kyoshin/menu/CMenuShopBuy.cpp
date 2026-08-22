// Decomp of kyoshin/menu/CMenuShopBuy.
// Shop buy menu screen process (CProcess-header + IScnRender MI layout).

#include "kyoshin/harness_catalog.hpp"

#include "kyoshin/menu/CMenuShopBuy.hpp"

#include "monolib/device/CDeviceVI.hpp"
#include "monolib/util/MemManager.hpp"
#include "monolib/work/CWorkThreadSystem.hpp"
#include <revolution/gx/GXPixel.h>

extern "C" void cbRenderBefore__12CMenuShopBuyFv(void*);

// Same-TU blob copy helper (defined below); Init uses it twice.
extern "C" void func_8018BE74(ShopBuyData* dst, const ShopBuyData* src);

/*
 * Destructor (retail D2 form, symbol keeps its C++ mangling in US).
 *
 * Written with an explicit flags parameter so the base-class destruction can
 * target the retail CProcess dtor wrapper __dt__800FED0C: a real member
 * destructor would make MWCC auto-emit __dt__8CProcessFv (the library D1)
 * for the base call, which cannot match the retail reloc name at the call
 * site. Subobjects are destroyed in reverse construction order, then
 * conditional operator delete when flags > 0 (same shape as CMenuSave).
 */
extern "C" CMenuShopBuy* __dt__12CMenuShopBuyFv(CMenuShopBuy* _this, int flags) {
    if (_this != 0) {
        __dt__12CItemBoxLineFv(reinterpret_cast<CItemBoxLine*>(_this->mItemBoxLine), -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        __dt__6CBgTexFv(&_this->mBgTex, -1);
        __dt__800FED0C(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

/*
 * Constructor (retail unmangled `__ct__CMenuShopBuy`).
 *
 * CProcess base first, then the temp vtable + __ptmf_null callback slots, the
 * composite vtable + IScnRender sub-vtable at +0x58, the scene/arg slots, the
 * three embedded subobjects, and the scalar tail (state 0 + float init).
 */
extern "C" CMenuShopBuy* __ct__CMenuShopBuy(CMenuShopBuy* self, CScn* scene, u32 arg) {
    __ct__8CProcessFv(self);

    // vtable fixups: temp (CProcess) vtable first, then the composite vtable
    // and the IScnRender sub-vtable at +0x58 (same scheme as CMenuSave).
    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_8052BF70;
    self->ptmf0[0] = __ptmf_null[0];
    self->ptmf0[1] = __ptmf_null[1];
    self->ptmf0[2] = __ptmf_null[2];
    self->ptmf1[0] = __ptmf_null[0];
    self->ptmf1[1] = __ptmf_null[1];
    self->ptmf1[2] = __ptmf_null[2];
    self->mField54 = 0;
    self->mField55 = 0;

    *(u32*)((u8*)self + 0x10) = (u32)lbl_eu_805326C0;
    *(u32*)((u8*)self + 0x58) = (u32)lbl_eu_805326C0 + 0x24;
    self->mScene = scene;
    self->mField60 = arg;

    __ct__CBgTex(&self->mBgTex, 0);
    __ct__CTitleAHelp(&self->mTitleAHelp, 0, 0);
    __ct__CItemBoxLine(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine), 0, 0);

    self->mState = 0;
    self->mFloat6FC = lbl_eu_80667A20;
    return self;
}

void CMenuShopBuy::Init() {
    func_8008294C__Q22cf13CfGameManagerFv(1);

    // Build a temporary CBgTex (retail unmangled ctor) and copy its data
    // fields into the member, then destroy the temp (CMenuMapSelect pattern).
    // All member/temp addresses stay inline so MWCC recomputes the addi per
    // use (retail keeps only r31=self and r30 scratch - no cached pointers).
    u8 tmpBgRaw[0x20];
    __ct__CBgTex(reinterpret_cast<CBgTex*>(tmpBgRaw), 0);
    __ct__UnkClass_8011C974(&mBgTex.mMemRegion,
                            &reinterpret_cast<CBgTex*>(tmpBgRaw)->mMemRegion);
    mBgTex.mFileHandle = reinterpret_cast<CBgTex*>(tmpBgRaw)->mFileHandle;
    mBgTex.mLayout = reinterpret_cast<CBgTex*>(tmpBgRaw)->mLayout;
    mBgTex.mLayoutReady = reinterpret_cast<CBgTex*>(tmpBgRaw)->mLayoutReady;
    mBgTex.mLoaded = reinterpret_cast<CBgTex*>(tmpBgRaw)->mLoaded;
    mBgTex.mPtmMode = reinterpret_cast<CBgTex*>(tmpBgRaw)->mPtmMode;
    __dt__6CBgTexFv(reinterpret_cast<CBgTex*>(tmpBgRaw), -1);

    func_801C3C14(&mBgTex);

    // Build a temporary CTitleAHelp from the locale string and copy its data
    // fields into the member, then destroy the temp.
    u8 tmpTitleRaw[0x38];
    __ct__CTitleAHelp(reinterpret_cast<CTitleAHelp*>(tmpTitleRaw),
                      func_80136190(lbl_eu_80503B20, lbl_eu_80503B20 + 9, 1), 0);
    __ct__UnkClass_8011C974(&mTitleAHelp.unk4,
                            &reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->unk4);
    mTitleAHelp.mFileHandle = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->mFileHandle;
    mTitleAHelp.mArcResourceAccessor =
        reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->mArcResourceAccessor;
    mTitleAHelp.mLayout = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->mLayout;
    mTitleAHelp.mAnimTrans20 = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->mAnimTrans20;
    mTitleAHelp.mAnimTrans24 = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->mAnimTrans24;
    mTitleAHelp.unk28 = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->unk28;
    mTitleAHelp.unk2c = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->unk2c;
    mTitleAHelp.mName = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->mName;
    mTitleAHelp.unk34 = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->unk34;
    mTitleAHelp.unk35 = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->unk35;
    mTitleAHelp.unk36 = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->unk36;
    mTitleAHelp.unk37 = reinterpret_cast<CTitleAHelp*>(tmpTitleRaw)->unk37;
    __dt__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(tmpTitleRaw), -1);

    CTitleAHelp_load(&mTitleAHelp);

    // Build a temporary CItemBoxLine (retail unmangled ctor with the u16
    // arg0 and file-id 1) and copy its whole data region (vtable excluded)
    // field-by-field into the embedded member, then destroy the temp. The
    // typed temp and union member keep every access self/sp-relative so MWCC
    // needs no cached base pointers (retail saves only r30/r31).
    ShopBuyItemBoxLine tmpLine;
    __ct__CItemBoxLine(reinterpret_cast<CItemBoxLine*>(&tmpLine), (u16)mField60, 1);

    __ct__UnkClass_8011C974(&mItemBox.mIBLData.mRegion04, &tmpLine.mData.mRegion04);
    __ct__UnkClass_8011C974(&mItemBox.mIBLData.mRegion14, &tmpLine.mData.mRegion14);
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
    func_8018B0FC(&mItemBox.mIBLData.cursors[0], &tmpLine.mData.cursors[0]);
    func_8018B0FC(&mItemBox.mIBLData.cursors[1], &tmpLine.mData.cursors[1]);
    func_8018B0FC(&mItemBox.mIBLData.cursors[2], &tmpLine.mData.cursors[2]);
    func_8018B0FC(&mItemBox.mIBLData.cursors[3], &tmpLine.mData.cursors[3]);
    __ct__UnkClass_8011C974(&mItemBox.mIBLData.mRegionD4, &tmpLine.mData.mRegionD4);
    __ct__UnkClass_8011C974(&mItemBox.mIBLData.mRegionE4, &tmpLine.mData.mRegionE4);
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
    func_8018BE74(&mItemBox.mIBLData.blob17C, &tmpLine.mData.blob17C);

    __ct__UnkClass_8011C974(&mItemBox.mIBLData.mRegion2DC, &tmpLine.mData.mRegion2DC);
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
    __ct__UnkClass_8011C974(&mItemBox.mIBLData.mRegion310, &tmpLine.mData.mRegion310);
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
    __ct__UnkClass_8011C974(&mItemBox.mIBLData.mRegion350, &tmpLine.mData.mRegion350);
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
    func_8018BE74(&mItemBox.mIBLData.blob4DC, &tmpLine.mData.blob4DC);
    __dt__12CItemBoxLineFv(reinterpret_cast<CItemBoxLine*>(&tmpLine), -1);

    // Reset the real member and register the render callback (same
    // `if (this)` IScnRender idiom as Term).
    func_801EDA08(reinterpret_cast<CItemBoxLine*>(mItemBoxLine));
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(mItemBoxLine), 2);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(mItemBoxLine), 4);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(mItemBoxLine), 5);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(mItemBoxLine), 6);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(mItemBoxLine), 7);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(mItemBoxLine), 8);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(mItemBoxLine), 0xd);
    func_801ED31C(reinterpret_cast<CItemBoxLine*>(mItemBoxLine));

    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = &mIScnRender;
    }
    addRenderCB__4CScnFP10IScnRenderUlUl(mScene, cb, 0xd, 0);
}

// Tear down the shop buy menu: detach the render callback, release the
// background/title widgets and the item box line, clear the singleton, and
// hand control back to the game manager.
void CMenuShopBuy::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    IScnRender* cb = reinterpret_cast<IScnRender*>(this);
    if (this != 0) {
        cb = &mIScnRender;
    }
    removeRenderCB__4CScnFP10IScnRender(mScene, cb);

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_801ED618(reinterpret_cast<CItemBoxLine*>(mItemBoxLine));

    lbl_eu_806642F8 = 0;
    func_8008294C__Q22cf13CfGameManagerFv(0);
}

void CMenuShopBuy::Move() {
    // Single short-circuit OR so MWCC emits: func test -> bne exit;
    // bit test -> beq continue / b exit (CSystemWindow::Move shape).
    if (func_800426F0__9CTaskGameFv(getInstance__9CTaskGameFv()) ||
        (lbl_eu_80663E28 & 0x200000))
        return;

    switch (mState) {
    case 0:
        func_8018C190(this);
        break;
    case 1:
        func_8018C208(this);
        break;
    case 2:
        func_8018C258(this);
        break;
    case 3:
        func_8018C59C(this);
        break;
    default:
        break;
    }

    func_801C3D54(&mBgTex);
    func_801ED3E8(reinterpret_cast<CItemBoxLine*>(mItemBoxLine));
    func_801C3FF0(&mTitleAHelp);
}

void CMenuShopBuy::cbRenderBefore() {
    // Same single-OR guard shape as Move / CSystemWindow::cbRenderBefore.
    if (func_800426F0__9CTaskGameFv(getInstance__9CTaskGameFv()) ||
        (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via the C-ABI ct/dt calls so the
    // scope-exit destructor is not virtual-dispatched (same scheme as
    // CSystemWindow::cbRenderBefore).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C3D7C(&mBgTex, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801ED4FC(reinterpret_cast<CItemBoxLine*>(mItemBoxLine), (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C4080(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0], -1);
}

// Copy the 0x159-byte shop-buy data blob field-by-field. Each sub-struct
// assignment comes out as a mtctr/lwzu/stwu counted loop; the u32 slot group
// is unrolled into 6 word pairs and the byte is a single lbz/stb.
extern "C" void func_8018BE74(ShopBuyData* dst, const ShopBuyData* src) {
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
extern "C" CMenuShopBuy* func_8018C104(CProcess* parent, CScn* scene, u32 arg) {
    if (lbl_eu_806642F8 != 0)
        return 0;

    CMenuShopBuy* obj = (CMenuShopBuy*)mtl::MemManager::allocate(
        0x700, CWorkThreadSystem::getWorkMem());
    if (obj != 0) {
        // Reassign from the ctor return so obj stays in volatile r3 (retail
        // never spills it to a callee-saved register).
        obj = __ct__CMenuShopBuy(obj, scene, arg);
    }
    lbl_eu_806642F8 = (u32)obj;
    obj->Regist(parent, 0);
    return (CMenuShopBuy*)lbl_eu_806642F8;
}

// (lbl_eu_806642F8 != 0)
extern "C" bool func_8018C180() { return lbl_eu_806642F8 != 0; }

// Phase 0 -> 1 (open): once the background, title bar and item box line are
// all ready, run the open sequence (title bar + item box line) and play the
// open sound, then advance the phase byte at 0x6F8.
extern "C" __declspec(noinline) void func_8018C190(CMenuShopBuy* self) {
    if (func_801C3E34(&self->mBgTex) != 0) {
        if (func_801C4114(&self->mTitleAHelp) != 0) {
            if (func_801ED774(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0) {
                func_801C412C(&self->mTitleAHelp);
                func_801ED864(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
                self->mState = 1;
                func_80138078__FUl(0x6d);
            }
        }
    }
}

// Phase 1 -> 2 (advance): once the title bar is idle and the item box line
// is ready, move to the next phase.
extern "C" __declspec(noinline) void func_8018C208(CMenuShopBuy* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (func_801ED800(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0) {
            self->mState = 2;
        }
    }
}

// Phase 2 (interaction): poll the pad and drive the item-box line. The two
// branches differ only in the flag layout (input-enabled vs pointer-input
// controller); each handler ends by jumping to the shared focus-refresh.
extern "C" __declspec(noinline) void func_8018C258(CMenuShopBuy* self) {
    if (func_8029A658() != 0) return;

    // Timer clamped to lbl_eu_80667A28, reset to lbl_eu_80667A20 on confirm.
    self->mFloat6FC += lbl_eu_80667A24;
    if (self->mFloat6FC > lbl_eu_80667A28) self->mFloat6FC = lbl_eu_80667A28;

    ShopBuyPadData* pad = getCfPadData__Q22cf13CfGameManagerFv();
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        // Pointer input enabled: turbo flags move the cursor, pressed flags
        // run the confirm/cancel flow. The item-box pointer is recomputed per
        // call (addi r3,r31,0xbc) exactly like retail - no cached local.
        func_801EECE0(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine), 0);
        if ((pad->mTurboFlags & 0x02000000) &&
            func_801ED808(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) == 0 &&
            code80135FDC_getByte_64077() > 1) {
            func_801EECE8(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if ((pad->mTurboFlags & 0x04000000) &&
                   func_801ED808(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) == 0 &&
                   code80135FDC_getByte_64077() > 1) {
            func_801EED6C(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mPressedFlags & 0x00200000) {
            func_801EE788(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mPressedFlags & 0x00400000) {
            if (func_801ED808(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0) {
                func_801EE684(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
            } else {
                func_801C414C(&self->mTitleAHelp);
                func_801ED97C(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
                self->mState = 3;
            }
        } else if (pad->mTurboFlags & 0x8004) {
            func_801EDC94(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if ((pad->mTurboFlags & 0x10000) != 0 || (pad->mTurboFlags & 0x8) != 0) {
            func_801EDF40(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mTurboFlags & 0x2001) {
            func_801EE228(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mTurboFlags & 0x4002) {
            func_801EE448(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mPressedFlags & 0x200) {
            func_801EEDF8(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mPressedFlags & 0x400) {
            func_801EF050(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        }
    } else {
        // Pointer input disabled: pad-held/pressed bits drive the flow, and
        // the inner A/B chain exits straight to the shared refresh.
        func_801EECE0(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine), 0);
        if ((pad->mHeldFlags & 0x1000) &&
            func_801ED808(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) == 0 &&
            code80135FDC_getByte_64077() > 1) {
            if (self->mFloat6FC > lbl_eu_80667A24) func_80138078__FUl(2);
            self->mFloat6FC = lbl_eu_80667A20;
            func_801EECE0(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine), 1);
            if (pad->mTurboFlags & 0x2001) {
                func_801EECE8(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
            } else if (pad->mTurboFlags & 0x4002) {
                func_801EED6C(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
            }
        } else if (pad->mPressedFlags & 0x10) {
            func_801EE788(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mPressedFlags & 0x20) {
            if (func_801ED808(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0) {
                func_801EE684(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
            } else {
                func_801C414C(&self->mTitleAHelp);
                func_801ED97C(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
                self->mState = 3;
            }
        } else if (pad->mTurboFlags & 0x8004) {
            func_801EDC94(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if ((pad->mTurboFlags & 0x10000) != 0 || (pad->mTurboFlags & 0x8) != 0) {
            func_801EDF40(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mTurboFlags & 0x2001) {
            func_801EE228(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mTurboFlags & 0x4002) {
            func_801EE448(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mPressedFlags & 0x200) {
            func_801EEDF8(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        } else if (pad->mPressedFlags & 0x40) {
            func_801EF050(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine));
        }
    }
    func_801C41E8(&self->mTitleAHelp,
                  func_801EF0EC(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)));
}

// Close: same idle+ready guard as func_8018C208, but marks the closing state
// byte at 0x54 instead of advancing the phase byte.
extern "C" __declspec(noinline) void func_8018C59C(CMenuShopBuy* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0) {
        if (func_801ED800(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0) {
            self->mField54 = 1;
        }
    }
}

extern "C" void func_8018C5EC(void* self) {
    ((void(*)(void*))cbRenderBefore__12CMenuShopBuyFv)((char*)self - 0x58);
}

extern "C" void func_8018C5F4(void* self) {
    ((void(*)(void*))__dt__12CMenuShopBuyFv)((char*)self - 0x58);
}
