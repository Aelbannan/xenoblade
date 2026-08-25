// Auto-scaffolded catalog TU for kyoshin/cf/CInfoCf
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CInfoCf.hpp"

#include "kyoshin/menu/CMenuGCItem.hpp"
#include "monolib/core/CPadManager.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/device/CDeviceVI.hpp"
#include <revolution/gx/GXPixel.h>

// Global: UI state flag read by func_80166830.

// Body-copy helpers defined below in this TU (called by CMenuItem::Init).
// extern "C": the retail reloc names are unmangled (same convention as
// func_80167A18 in kyoshin/menu/CMenuItem.cpp).
extern "C" void func_80166E48(CInfoCfObjE48* dst, const CInfoCfObjE48* src);
extern "C" void func_80166F80(CInfoCfObjF80* dst, const CInfoCfObjF80* src);
extern "C" void func_801671D4(CInfoCfObjD4* dst, const CInfoCfObjD4* src);
extern "C" void func_80167260(CInfoCfObj60* dst, const CInfoCfObj60* src);
extern "C" void func_801672E4(CInfoCfObjE4* dst, const CInfoCfObjE4* src);
extern "C" void func_80167368(CInfoCfObj368* dst, const CInfoCfObj368* src);
// func_8016742C is declared by CItemBoxGrid.hpp (retail-unmangled name,
// void* ABI); the typed object views are passed straight through.
extern "C" void func_801674D0(CInfoCfObj4D0* dst, const CInfoCfObj4D0* src);

// The retail ctor symbol __ct__cf_CInfoCf is a C-linkage name (no C++ mangling
// markers), so it is emitted as a global function taking the instance (same
// convention as __ct__cf_CfTFile / __ct__cf_CREvtMem). It installs the
// externally-defined vtable, zeroes the flag word and the cached setting, and
// records the instance in the singleton global for the rest of the TU.
cf::CInfoCf* __ct__cf_CInfoCf(cf::CInfoCf* self) {
    self->vtable = lbl_eu_805309B0;
    self->mFlags = 0;
    self->mField08 = 0;
    lbl_eu_80664250 = self;
    return self;
}

// Destructor: clear the singleton pointer. The null check and delete-flag
// handling (conditional __dl__FPv) are emitted by MWCC for the member dtor.
cf::CInfoCf::~CInfoCf() {
    lbl_eu_80664250 = 0;
}

extern "C" void* func_8016676C() { return *(void**)((u8*)lbl_eu_80664250 + 0xC); }

extern "C" void* func_80166778() { return *(void**)((u8*)lbl_eu_80664250 + 0x10); }

// FULL_MATCH - no-op virtual function (vtable slot 2).  Immediately returns.
void func_80166784() {
}

// Toggle bits of the UI-state singleton's flag word: set bit 0x2, then
// immediately clear bit 0x1. The singleton pointer is re-read from the global
// between the two (retail reloads it), so both statements dereference the
// global directly rather than sharing a local.
void func_80166788() {
    lbl_eu_80664250->mFlags |= 0x2;
    lbl_eu_80664250->mFlags &= ~0x1;
}

void func_801667AC(cf::CInfoCf* self) {
    // Refresh the cached setting state: re-read the UI flag word, the option
    // and timer values, then rewrite the flag word (clear bit 0x1; set it back
    // when the cached setting matches the option-flag/pause combo).
    cf::CfGameManager::getInstance();
    self->mField08 = func_80166830();
    func_80062F60();
    self->mField0C = func_800A8CD4();
    u32 v = func_800A8DA4();
    u32 flags = self->mFlags & ~1;
    u32 f08 = self->mField08;
    self->mField10 = v;
    self->mFlags = flags;
    if ((f08 & 0x30800) == 0x30800 && (f08 & 0x65000004) == 0) {
        self->mFlags = flags | 1;
    }
}

// FULL_MATCH - Returns the global UI state flag value (lbl_eu_80663E24).
// noinline: retail calls this out-of-line from func_801667AC.
__declspec(noinline) u32 func_80166830() {
    return lbl_eu_80663E24;
}

// Destructor (retail D1 form; symbol keeps the CW member mangling in US).
// Written as an extern "C" free function (same scheme as ~CMenuGCItem) so the
// member subobjects (CItemBoxGrid @0xB8, CTitleAHelp @0x80, CBgTex @0x60) are
// destroyed explicitly in retail order with a non-deleting -1 flag, and the
// base-class destruction targets the library D1 symbol __dt__8CProcessFv
// (0x804474D0). The nested double null-check before the base call is an MWCC
// D2-inlined-into-D1 artifact (same shape as ~CSystemWindow / ~CMenuGCItem);
// the delete follows when the incoming deleting flag is > 0.
extern "C" CMenuItem* __dt__9CMenuItemFv(CMenuItem* _this, int flags) {
    if (_this != 0) {
        __dt__12CItemBoxGridFv(&_this->mItemBoxGrid, -1);
        __dt__11CTitleAHelpFv(&_this->mTitleAHelp, -1);
        __dt__6CBgTexFv(&_this->mBgTex, -1);
        if (_this != 0) {
            if (_this != 0) {
                __dt__8CProcessFv((CProcess*)_this, 0);
            }
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

void CMenuItem::Init() {
    // Gate the game-manager handoff until the menu flag is clear (retail
    // reads mField4AC5 before any widget setup).
    if (mField4AC5 == 0) {
        func_8008294C__Q22cf13CfGameManagerFv(1);
    }

    // --- Re-initialise the embedded CBgTex via a temporary ---
    u8 tempBgTex[0x20];
    __ct__CBgTex(reinterpret_cast<CBgTex*>(tempBgTex), 0);
    CBgTex* tmpBgTex = reinterpret_cast<CBgTex*>(tempBgTex);
    // Field-by-field body copy (retail copies the UnkClass head inline, not
    // via its __as__ operator=).
    mBgTex.mMemRegion.unk0 = tmpBgTex->mMemRegion.unk0;
    mBgTex.mMemRegion.unk4 = tmpBgTex->mMemRegion.unk4;
    mBgTex.mMemRegion.unk8 = tmpBgTex->mMemRegion.unk8;
    mBgTex.mMemRegion.unkC = tmpBgTex->mMemRegion.unkC;
    mBgTex.mFileHandle = tmpBgTex->mFileHandle;
    mBgTex.mLayout = tmpBgTex->mLayout;
    mBgTex.mLayoutReady = tmpBgTex->mLayoutReady;
    mBgTex.mLoaded = tmpBgTex->mLoaded;
    mBgTex.mPtmMode = tmpBgTex->mPtmMode;
    __dt__6CBgTexFv(tmpBgTex, -1);

    if (func_801C3C14(&mBgTex) != 0) {
        func_801C3A24(&mBgTex);
    }

    // --- Re-initialise the embedded CTitleAHelp via a temporary ---
    char* name = func_80136190(lbl_eu_8050303C, lbl_eu_8050303C + 9, 1);
    u8 tempTitle[0x38];
    __ct__CTitleAHelp(reinterpret_cast<CTitleAHelp*>(tempTitle), name, 1);
    CTitleAHelp* tmpTitle = reinterpret_cast<CTitleAHelp*>(tempTitle);
    mTitleAHelp.unk4.unk0 = tmpTitle->unk4.unk0;
    mTitleAHelp.unk4.unk4 = tmpTitle->unk4.unk4;
    mTitleAHelp.unk4.unk8 = tmpTitle->unk4.unk8;
    mTitleAHelp.unk4.unkC = tmpTitle->unk4.unkC;
    mTitleAHelp.mFileHandle = tmpTitle->mFileHandle;
    mTitleAHelp.mArcResourceAccessor = tmpTitle->mArcResourceAccessor;
    mTitleAHelp.mLayout = tmpTitle->mLayout;
    mTitleAHelp.mAnimTrans20 = tmpTitle->mAnimTrans20;
    mTitleAHelp.mAnimTrans24 = tmpTitle->mAnimTrans24;
    mTitleAHelp.unk28 = tmpTitle->unk28;
    mTitleAHelp.unk2c = tmpTitle->unk2c;
    mTitleAHelp.mName = tmpTitle->mName;
    mTitleAHelp.unk34 = tmpTitle->unk34;
    mTitleAHelp.unk35 = tmpTitle->unk35;
    mTitleAHelp.unk36 = tmpTitle->unk36;
    mTitleAHelp.unk37 = tmpTitle->unk37;
    __dt__11CTitleAHelpFv(tmpTitle, -1);

    CTitleAHelp_load(&mTitleAHelp);

    // --- Re-initialise the embedded CItemBoxGrid via a temporary ---
    u8 tempGrid[0x4a0c];
    __ct__CItemBoxGrid(reinterpret_cast<CItemBoxGrid*>(tempGrid),
                       mField4AC5 ? 3 : 0, 0, (u32)mScene, mField4AC6);

    // Copy the whole temporary into the member, region by region (retail
    // split schedule: inline head/tail + the func_80166xxx copy helpers).
    CItemBoxGridBodyView* dstBody =
        reinterpret_cast<CItemBoxGridBodyView*>(&mItemBoxGrid);
    const CItemBoxGridBodyView* srcBody =
        reinterpret_cast<const CItemBoxGridBodyView*>(tempGrid);
    // Head: 20 streamed words + byte + 2 words + 2 bytes + packed tail
    // (member-wise so MWCC inlines them; a whole-struct assignment would
    // emit an __as__ call).
    CItemBoxGridHead* dh = &dstBody->head;
    const CItemBoxGridHead* sh = &srcBody->head;
    dh->field_04 = sh->field_04;
    dh->field_08 = sh->field_08;
    dh->field_0C = sh->field_0C;
    dh->field_10 = sh->field_10;
    dh->field_14 = sh->field_14;
    dh->field_18 = sh->field_18;
    dh->field_1C = sh->field_1C;
    dh->field_20 = sh->field_20;
    dh->field_24 = sh->field_24;
    dh->field_28 = sh->field_28;
    dh->field_2C = sh->field_2C;
    dh->field_30 = sh->field_30;
    dh->field_34 = sh->field_34;
    dh->field_38 = sh->field_38;
    dh->field_3C = sh->field_3C;
    dh->field_40 = sh->field_40;
    dh->field_44 = sh->field_44;
    dh->field_48 = sh->field_48;
    dh->field_4C = sh->field_4C;
    dh->field_50 = sh->field_50;
    dh->field_54 = sh->field_54;
    dh->field_58 = sh->field_58;
    dh->field_5C = sh->field_5C;
    dh->field_60 = sh->field_60;
    dh->field_61 = sh->field_61;
    dh->tail.pair62 = sh->tail.pair62;
    dh->tail.field_6A = sh->tail.field_6A;
    dh->tail.field_6E = sh->tail.field_6E;
    dh->tail.field_6F = sh->tail.field_6F;
    func_80166E48(&dstBody->objE8, &srcBody->objE8);
    func_80166F80(&dstBody->obj1D8, &srcBody->obj1D8);
    func_801671D4(&dstBody->obj3E4, &srcBody->obj3E4);
    func_80167260(&dstBody->obj418, &srcBody->obj418);
    func_801672E4(&dstBody->obj440, &srcBody->obj440);
    func_80167368(&dstBody->obj468, &srcBody->obj468);
    func_8016742C(&dstBody->obj4AC, (void*)&srcBody->obj4AC);
    func_8016742C(&dstBody->obj4E8, (void*)&srcBody->obj4E8);
    dstBody->tail524.field_524 = srcBody->tail524.field_524;
    dstBody->tail524.field_525 = srcBody->tail524.field_525;
    dstBody->tail524.field_526 = srcBody->tail524.field_526;
    dstBody->tail524.field_527 = srcBody->tail524.field_527;
    dstBody->tail524.field_528 = srcBody->tail524.field_528;
    dstBody->tail524.field_529 = srcBody->tail524.field_529;
    dstBody->tail524.field_52A = srcBody->tail524.field_52A;
    dstBody->tail524.field_52C = srcBody->tail524.field_52C;
    dstBody->tail524.field_52D = srcBody->tail524.field_52D;
    dstBody->tail524.field_52E = srcBody->tail524.field_52E;
    dstBody->tail524.field_530 = srcBody->tail524.field_530;
    dstBody->tail524.pair534 = srcBody->tail524.pair534;
    dstBody->tail524.field_53C = srcBody->tail524.field_53C;
    dstBody->tail524.field_540 = srcBody->tail524.field_540;
    dstBody->tail524.field_541 = srcBody->tail524.field_541;
    dstBody->tail524.field_542 = srcBody->tail524.field_542;
    dstBody->tail524.field_543 = srcBody->tail524.field_543;
    dstBody->tail524.field_544 = srcBody->tail524.field_544;
    dstBody->tail524.field_545 = srcBody->tail524.field_545;
    dstBody->tail524.field_546 = srcBody->tail524.field_546;
    dstBody->tail524.field_547 = srcBody->tail524.field_547;
    dstBody->tail524.field_548 = srcBody->tail524.field_548;
    dstBody->tail524.field_549 = srcBody->tail524.field_549;
    func_801674D0(&dstBody->obj54C, &srcBody->obj54C);
    __dt__12CItemBoxGridFv(reinterpret_cast<CItemBoxGrid*>(tempGrid), -1);

    func_801CB480(&mItemBoxGrid);

    if (mField4AC5 == 0) {
        // Single-player item list ordering.
        PushToList(&mItemBoxGrid, 2);
        PushToList(&mItemBoxGrid, 4);
        PushToList(&mItemBoxGrid, 5);
        PushToList(&mItemBoxGrid, 6);
        PushToList(&mItemBoxGrid, 7);
        PushToList(&mItemBoxGrid, 8);
        PushToList(&mItemBoxGrid, 0xb);
        PushToList(&mItemBoxGrid, 0xd);
        PushToList(&mItemBoxGrid, 0xc);
        PushToList(&mItemBoxGrid, 3);
        PushToList(&mItemBoxGrid, 9);
        PushToList(&mItemBoxGrid, 0xa);
    } else {
        PushToList(&mItemBoxGrid, mField4AC5);
    }
    func_801CAA6C(&mItemBoxGrid);

    // Register the render callback (this or the IScnRender subobject).
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    mScene->addRenderCB(renderCB, 0xd, 0);
}

// Copy a 0xEB-byte body slice (+0x04..+0xEF). The 16-entry 8-byte table at
// +0x6C is copied by a counted lwzu/stwu loop (element base registers walk
// from 0x68 = 0x6C-4).
extern "C" void func_80166E48(CInfoCfObjE48* dst, const CInfoCfObjE48* src) {
    dst->body = src->body;
}

extern "C" void func_80166F80(CInfoCfObjF80* dst, const CInfoCfObjF80* src) {
    dst->head = src->head;
    dst->mid = src->mid;
}

// Copy a 0x21-byte body slice (7 words + 5 bytes at +0x04..+0x24), skipping
// the member vtable at +0x00. Member struct assignment reproduces the retail
// load-all-then-store-all shape and register allocation.
extern "C" void func_80167260(CInfoCfObj60* dst, const CInfoCfObj60* src) {
    dst->body = src->body;
}

// Copy an 8-word + 4-byte body slice (+0x04..+0x27).
extern "C" void func_801672E4(CInfoCfObjE4* dst, const CInfoCfObjE4* src) {
    dst->body = src->body;
}

// Copy a 10-word + 5-byte body slice (+0x04..+0x30).
extern "C" void func_801671D4(CInfoCfObjD4* dst, const CInfoCfObjD4* src) {
    dst->body = src->body;
}

// Copy a 0x3E-byte body slice (+0x04..+0x42). Retail load-all-then-store-all
// with one temp per field (22 temps fit the register file); the packed tail
// words at +0x39/+0x3D (odd offsets) are read with word casts from the raw
// tail slice.
extern "C" void func_80167368(CInfoCfObj368* dst, const CInfoCfObj368* src) {
    u32 v04 = src->body.field_04;
    u32 v08 = src->body.field_08;
    u32 v0C = src->body.field_0C;
    u32 v10 = src->body.field_10;
    u32 v14 = src->body.field_14;
    u32 v18 = src->body.field_18;
    u32 v1C = src->body.field_1C;
    u32 v20 = src->body.field_20;
    u32 v24 = src->body.field_24;
    u32 v28 = src->body.field_28;
    u32 v2C = src->body.field_2C;
    u8 v30 = src->body.field_30;
    u8 v31 = src->body.field_31;
    u8 v32 = src->body.field_32;
    u8 v33 = src->body.field_33;
    u16 v34 = src->body.field_34;
    u8 v36 = src->body.field_36;
    u8 v37 = src->body.field_37;
    u8 v38 = src->body.field_38;
    u32 v39 = *(u32*)&src->tail[0];
    u32 v3D = *(u32*)&src->tail[4];
    u8 v41 = src->tail[8];
    dst->body.field_04 = v04;
    dst->body.field_08 = v08;
    dst->body.field_0C = v0C;
    dst->body.field_10 = v10;
    dst->body.field_14 = v14;
    dst->body.field_18 = v18;
    dst->body.field_1C = v1C;
    dst->body.field_20 = v20;
    dst->body.field_24 = v24;
    dst->body.field_28 = v28;
    dst->body.field_2C = v2C;
    dst->body.field_30 = v30;
    dst->body.field_31 = v31;
    dst->body.field_32 = v32;
    dst->body.field_33 = v33;
    dst->body.field_34 = v34;
    dst->body.field_36 = v36;
    dst->body.field_37 = v37;
    dst->body.field_38 = v38;
    *(u32*)&dst->tail[0] = v39;
    *(u32*)&dst->tail[4] = v3D;
    dst->tail[8] = v41;
}

// CSysWin body copy: every field from +0x04..+0x39 except the 0x29-0x2B
// alignment gap (implicit padding in CInfoCfSysWinBody, not copied by the
// struct assignment - matching retail).
void func_8016742C(void* dstV, void* srcV) {
    CInfoCfObjSysWin* dst = (CInfoCfObjSysWin*)dstV;
    const CInfoCfObjSysWin* src = (const CInfoCfObjSysWin*)srcV;
    dst->body = src->body;
}

// Copy the 0x44C0-byte info-screen object (vtable slot included) region by
// region, mirroring retail's split schedule: u64 tables via counted lwzu/stwu
// loops (pointer walks keep MWCC from unrolling), then the packed string/data
// run as word/byte copies.
extern "C" void func_801674D0(CInfoCfObj4D0* dst, const CInfoCfObj4D0* src) {
    u8* d = (u8*)dst;
    const u8* s = (const u8*)src;
    u64* dq = (u64*)d;
    const u64* sq = (const u64*)s;
    for (int i = 0; i < 0x500; i++) {
        *dq++ = *sq++;
    }
    *(u16*)(d + 0x2800) = *(u16*)(s + 0x2800);
    d[0x2802] = s[0x2802];
    d[0x2803] = s[0x2803];
    d[0x2804] = s[0x2804];
    *(u32*)(d + 0x2805) = *(u32*)(s + 0x2805);
    *(u32*)(d + 0x2809) = *(u32*)(s + 0x2809);
    *(u32*)(d + 0x280d) = *(u32*)(s + 0x280d);
    *(u32*)(d + 0x2811) = *(u32*)(s + 0x2811);
    *(u32*)(d + 0x2815) = *(u32*)(s + 0x2815);
    *(u32*)(d + 0x2819) = *(u32*)(s + 0x2819);
    *(u32*)(d + 0x281d) = *(u32*)(s + 0x281d);
    *(u32*)(d + 0x2821) = *(u32*)(s + 0x2821);
    dq = (u64*)(d + 0x2825);
    sq = (const u64*)(s + 0x2825);
    for (int i = 0; i < 0x10; i++) {
        *dq++ = *sq++;
    }
    dq = (u64*)(d + 0x28a5);
    sq = (const u64*)(s + 0x28a5);
    for (int i = 0; i < 0x80; i++) {
        *dq++ = *sq++;
    }
    dq = (u64*)(d + 0x2cac);
    sq = (const u64*)(s + 0x2cac);
    for (int i = 0; i < 0x100; i++) {
        *dq++ = *sq++;
    }
    *(u16*)(d + 0x34ac) = *(u16*)(s + 0x34ac);
    dq = (u64*)(d + 0x34b4);
    sq = (const u64*)(s + 0x34b4);
    for (int i = 0; i < 0x100; i++) {
        *dq++ = *sq++;
    }
    *(u16*)(d + 0x3cb4) = *(u16*)(s + 0x3cb4);
    dq = (u64*)(d + 0x3cbc);
    sq = (const u64*)(s + 0x3cbc);
    for (int i = 0; i < 0x100; i++) {
        *dq++ = *sq++;
    }
    *(u16*)(d + 0x44bc) = *(u16*)(s + 0x44bc);
}

// Tear down the menu screen: wait for the draw callback to finish, remove the
// render callback from the owning scene, release each embedded widget, clear
// the singleton flag, and hand control back to the game manager when the
// menu-state byte is clear.
void CMenuItem::Term() {
    CDeviceVI::waitForDrawDone();

    // The `if (this)` is the MWCC idiom that splits mr r4,r31 / beq / addi r4,+0x58.
    IScnRender* renderCB = reinterpret_cast<IScnRender*>(this);
    if (this != NULL) {
        renderCB = &mIScnRender;
    }
    mScene->removeRenderCB(renderCB);

    func_801C3D9C(&mBgTex);
    func_801C40A0(&mTitleAHelp);
    func_801CAE9C(&mItemBoxGrid);

    lbl_eu_80664258 = 0;
    if (mField4AC5 == 0) {
        func_8008294C__Q22cf13CfGameManagerFv(0);
    }
}

void CMenuItem::Move() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;

    // Button input: the confirm trigger comes from a different pad bit in
    // co-op vs single-player mode.
    CPad* pad = cf::CfGameManager::getCurrentPad();
    u32 pressed;
    if (func_80086F9C__Q22cf13CfGameManagerFv(-1) != 0) {
        pressed = (pad->mPressedButtonFlags >> 23) & 1;
    } else {
        pressed = (pad->mPressedButtonFlags >> 10) & 1;
    }
    if (pressed != 0) {
        // Close a pending item-box interaction, then mark the menu done.
        if (func_801CB1E4(&mItemBoxGrid) == 0) {
            if (func_800FEDF8() != 0) {
                func_800FF914();
            }
            func_80138078__FUl(6);
            mState = 4;
            mPrefix[0x50] = 1;
        }
    }

    switch (mState) {
    case 0:
        // Once the bg texture, title bar and item grid are all ready,
        // start the panel intro animations and play the open cue.
        if (func_801C3E34(&mBgTex) != 0 && func_801C4114(&mTitleAHelp) != 0 &&
            func_801CB038(&mItemBoxGrid) != 0) {
            func_801C412C(&mTitleAHelp);
            func_801CB28C(&mItemBoxGrid);
            mState = 1;
            func_80138078__FUl(0x6d);
        }
        break;
    case 1:
        // Advance to phase 2 once the title bar is idle and the grid ready.
        if (isIdle__11CTitleAHelpFv(&mTitleAHelp) != 0 && GetField61(&mItemBoxGrid) != 0) {
            mState = 2;
        }
        break;
    case 2:
        func_80167A2C(this);
        break;
    case 3:
        // Same idle+ready check, but sets the phase flag at 0x54 instead.
        if (isIdle__11CTitleAHelpFv(&mTitleAHelp) != 0 && GetField61(&mItemBoxGrid) != 0) {
            mPrefix[0x50] = 1;
        }
        break;
    }

    func_801C3D54(&mBgTex);
    func_801CABC8(&mItemBoxGrid);
    func_801C3FF0(&mTitleAHelp);
}

void CMenuItem::cbRenderBefore() {
    CTaskGame::getInstance();
    if (CTaskGame::func_800426F0() || (lbl_eu_80663E28 & 0x200000))
        return;
    if (func_8013BE50() == 0) return;
    GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    // Raw-storage DrawInfo built/destroyed via C-ABI ct/dt calls (same
    // scheme as CMenuGCItem::cbRenderBefore; a C++ local would
    // virtual-dispatch its scope-exit destructor).
    u8 drawInfo[0x54];
    __ct__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0]);
    func_80137250((nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C3D7C(&mBgTex, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801CAD8C(&mItemBoxGrid, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    func_801C4080(&mTitleAHelp, (nw4r::lyt::DrawInfo*)&drawInfo[0]);
    __dt__Q34nw4r3lyt8DrawInfoFv(&drawInfo[0], -1);
}
