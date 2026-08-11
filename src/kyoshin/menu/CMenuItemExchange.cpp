// Auto-scaffolded catalog TU for kyoshin/menu/CMenuItemExchange
// High-level C/C++ reconstruction from retail PPC.

#include "kyoshin/menu/CMenuItemExchange.hpp"

#include "kyoshin/CBgTex.hpp"

extern "C" void __dt__17CMenuItemExchangeFv(void*, int);
extern "C" void cbRenderBefore__17CMenuItemExchangeFv(void*);

/**
 * Offset from the OC thunk interface (CMenuItemExchangeOC*) to the
 * containing CMenuItemExchange object. The OC system passes a pointer
 * to a sub-object embedded at this offset; thunks subtract this value
 * to recover the real this pointer before delegating.
 */
static const u32 CMENU_ITEM_EXCHANGE_OC_OFFSET = 0x6c;

void __ct__CMenuItemExchange(){}

CMenuItemExchange::~CMenuItemExchange() {}

// ---------------------------------------------------------------------------
// Target: CMenuItemExchange::Init (us-801bf844)
// Construct each embedded widget through a stack temporary (retail unmangled
// ctor), copy its body into the member with the retail copy helper, then
// destroy the temporary. Finishes by registering the OC render callback on
// the owning scene.
// ---------------------------------------------------------------------------
void CMenuItemExchange::Init() {
    func_8008294C__Q22cf13CfGameManagerFv(1);

    // Background layout widget: temp CBgTex -> copy -> destroy, then load.
    u8 tmpBgRaw[0x20];
    __ct__CBgTex(reinterpret_cast<CBgTex*>(tmpBgRaw), 0);
    func_801BE108(&mBgTex, reinterpret_cast<CBgTex*>(tmpBgRaw));
    __dt__6CBgTexFv(reinterpret_cast<CBgTex*>(tmpBgRaw), -1);

    func_801C3C14(&mBgTex);

    // Title/help bar: temp CTitleAHelp (named from the locale string) ->
    // copy -> destroy, then load.
    u8 tmpTitleRaw[0x38];
    __ct__CTitleAHelp(reinterpret_cast<CTitleAHelp*>(tmpTitleRaw),
                      func_80136190(lbl_eu_80505324, lbl_eu_80505324 + 9, 0x1a), 0);
    func_801BE16C(&mTitleAHelp, reinterpret_cast<CTitleAHelp*>(tmpTitleRaw));
    __dt__11CTitleAHelpFv(reinterpret_cast<CTitleAHelp*>(tmpTitleRaw), -1);

    CTitleAHelp_load(&mTitleAHelp);

    // Item-box line: temp CItemBoxLine -> copy -> destroy, then register the
    // tab entries (retail call order 2,4,5,6,7,8,0xb,3,0xa) and finish the
    // line setup.
    u8 tmpLineRaw[0x63C];
    __ct__CItemBoxLine(reinterpret_cast<CItemBoxLine*>(tmpLineRaw), (u16)mField74, 2);
    func_801BE208(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]),
                  reinterpret_cast<CItemBoxLine*>(tmpLineRaw));
    __dt__12CItemBoxLineFv(reinterpret_cast<CItemBoxLine*>(tmpLineRaw), -1);

    func_801EDA08(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]));
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 2);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 4);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 5);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 6);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 7);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 8);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 0xb);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 3);
    func_801EDA4C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]), 0xa);
    func_801ED31C(reinterpret_cast<CItemBoxLine*>(&mItemBoxLine[0]));

    // Item-box grid: temp CItemBoxGrid -> copy -> destroy, then register the
    // two tab lists.
    u8 tmpGridRaw[0x4a0c];
    __ct__CItemBoxGrid(reinterpret_cast<CItemBoxGrid*>(tmpGridRaw), 2, (u16)mField74, 0, 0);
    func_801BE590(&mItemBoxGrid, reinterpret_cast<CItemBoxGrid*>(tmpGridRaw));
    __dt__12CItemBoxGridFv(reinterpret_cast<CItemBoxGrid*>(tmpGridRaw), -1);

    func_801CB480(&mItemBoxGrid);
    PushToList(&mItemBoxGrid, 0xb);
    PushToList(&mItemBoxGrid, 0xa);

    // Register the OC render callback (adjusted-this idiom: null this stays
    // null, otherwise the +0x6c sub-object).
    IScnRender* render = reinterpret_cast<IScnRender*>(this);
    if (this) render = reinterpret_cast<IScnRender*>(&mOCVt);
    mScene->addRenderCB(render, 0xd, 0);
}

/* Copy-construct a CBgTex field-by-field from src into dest (retail
 * func_801BE108): the +0x4 mem region through the shared UnkClass_8011C974
 * helper, then the file-handle / layout pointers and the three status bytes.
 * Returns dest (copy-ctor idiom, retail restores r3 to dest before the tail). */
extern "C" CBgTex* func_801BE108(CBgTex* dest, CBgTex* src) {
    __ct__UnkClass_8011C974(&dest->mMemRegion, &src->mMemRegion);
    dest->mFileHandle = src->mFileHandle;
    dest->mLayout = src->mLayout;
    dest->mLayoutReady = src->mLayoutReady;
    dest->mLoaded = src->mLoaded;
    dest->mPtmMode = src->mPtmMode;
    return dest;
}

/* Copy-construct a CTitleAHelp field-by-field from src into dest (retail
 * func_801BE16C): the +0x4 mem region via UnkClass_8011C974, then the file
 * handle / layout / anim pointers and the trailing status bytes. */
extern "C" void func_801BE16C(CTitleAHelp* dest, CTitleAHelp* src) {
    __ct__UnkClass_8011C974(&dest->unk4, &src->unk4);
    dest->mFileHandle = src->mFileHandle;
    dest->mArcResourceAccessor = src->mArcResourceAccessor;
    dest->mLayout = src->mLayout;
    dest->mAnimTrans20 = src->mAnimTrans20;
    dest->mAnimTrans24 = src->mAnimTrans24;
    dest->unk28 = src->unk28;
    dest->unk2c = src->unk2c;
    dest->mName = src->mName;
    dest->unk34 = src->unk34;
    dest->unk35 = src->unk35;
    dest->unk36 = src->unk36;
    dest->unk37 = src->unk37;
}

extern "C" __declspec(noinline) void func_801BE208(CItemBoxLine* dest, CItemBoxLine* src) {
    __ct__UnkClass_8011C974(reinterpret_cast<u8*>(dest) + 4, reinterpret_cast<u8*>(src) + 4);
    __ct__UnkClass_8011C974(reinterpret_cast<u8*>(dest) + 0x14, reinterpret_cast<u8*>(src) + 0x14);
}

extern "C" __declspec(noinline) void func_801BE590(CItemBoxGrid* dest, CItemBoxGrid* src) {
    __ct__UnkClass_8011C974(reinterpret_cast<u8*>(dest) + 4, reinterpret_cast<u8*>(src) + 4);
    __ct__UnkClass_8011C974(reinterpret_cast<u8*>(dest) + 8, reinterpret_cast<u8*>(src) + 8);
}

void CMenuItemExchange::Term() {}

void CMenuItemExchange::Move() {}

void CMenuItemExchange::cbRenderBefore() {}

void func_801BEDE0(){}

// (lbl_eu_80664428 != 0)
extern "C" bool func_801BEE5C() { extern u32 lbl_eu_80664428; return lbl_eu_80664428 != 0; }

extern u8 lbl_eu_8066442C;
u8 func_801BEE6C() { return lbl_eu_8066442C; }

void func_801BEE74(){}

/* Exchange-menu phase gate: once the title/help bar is idle and the item-box
 * line is ready, advance the menu to phase 2 (field_5118). */
void func_801BEEF4(CMenuItemExchange* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_801ED800(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0) {
        self->field_5118 = 2;
    }
}

void func_801BEF44(){}

/* Exchange-menu phase gate: idle title/help bar plus ready item-box grid
 * advances the menu to phase 1 (field_5118). */
void func_801BF2E8(CMenuItemExchange* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_801ED800(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0 &&
        GetField61(&self->mItemBoxGrid) != 0) {
        self->field_64 = 1;
    }
}

void func_801BF348(){}

/* Exchange-menu phase gate: idle title/help bar plus ready item-box grid
 * advances the menu to phase 6 (field_5118). */
void func_801BF414(CMenuItemExchange* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        GetField61(&self->mItemBoxGrid) != 0) {
        self->field_5118 = 6;
    }
}

void func_801BF464(){}

void func_801BF6A0(){}

/* Exchange-menu phase gate: twin of func_801BEEF4 (idle help bar + ready
 * item-box line) advancing the menu to phase 2. */
void func_801BF70C(CMenuItemExchange* self) {
    if (isIdle__11CTitleAHelpFv(&self->mTitleAHelp) != 0 &&
        func_801ED800(reinterpret_cast<CItemBoxLine*>(self->mItemBoxLine)) != 0) {
        self->field_5118 = 2;
    }
}

/**
 * OC thunk for cbRenderBefore. Adjusts the OC wrapper pointer back to the
 * containing CMenuItemExchange and delegates to cbRenderBefore.
 */
extern "C" void func_801BF75C(CMenuItemExchangeOC* param) {
    ((void(*)(void*))cbRenderBefore__17CMenuItemExchangeFv)((char*)param - 0x6c);
}

/**
 * OC thunk for the destructor. Adjusts the OC wrapper pointer back to the
 * containing CMenuItemExchange and delegates to the real destructor.
 */
extern "C" void func_801BF764(CMenuItemExchangeOC* param) {
    ((void(*)(void*))__dt__17CMenuItemExchangeFv)((char*)param - 0x6c);
}
