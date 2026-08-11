// Auto-scaffolded catalog TU for kyoshin/menu/CMenuItemExchange
// Replace stubs with high-level C/C++ during decomp.

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

void CMenuItemExchange::Init() {}

/* Copy-construct a CBgTex field-by-field from src into dest (retail
 * func_801BE108): the +0x4 mem region through the shared UnkClass_8011C974
 * helper, then the file-handle / layout pointers and the three status bytes.
 * Returns dest (copy-ctor idiom, retail restores r3 to dest before the tail). */
CBgTex* func_801BE108(CBgTex* dest, CBgTex* src) {
    __ct__UnkClass_8011C974(&dest->mMemRegion, &src->mMemRegion);
    dest->mFileHandle = src->mFileHandle;
    dest->mLayout = src->mLayout;
    dest->mLayoutReady = src->mLayoutReady;
    dest->mLoaded = src->mLoaded;
    dest->mPtmMode = src->mPtmMode;
    return dest;
}

void func_801BE16C(){}

void func_801BE208(){}

void func_801BE590(){}

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
