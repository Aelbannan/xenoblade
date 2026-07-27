// FULL_MATCH: func_802B0F08, func_802B0F10

#include "kyoshin/menu/CMenuGCItem.hpp"

#include "kyoshin/harness_catalog.hpp"

void CMenuGCItem::~CMenuGCItem() {}

void CMenuGCItem::Init() {}

void CMenuGCItem::Term() {}

void CMenuGCItem::Move() {}

void CMenuGCItem::cbRenderBefore() {}

void __ct__CMenuGCItem(){}

unsigned long func_802B0D10(){
    unsigned long v = *(unsigned long*)(lbl_eu_80664C00);
    return !!v;
}

void func_802B0D24(){}

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * IScnRender is a non-primary base at offset 0x58 within CMenuGCItem.
 * Adjusts the subobject pointer back to CMenuGCItem* before forwarding
 * to the real cbRenderBefore override.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__11CMenuGCItemFv
 */
void func_802B0F08(IScnRender* self) {
    ((void(*)(CMenuGCItem*))cbRenderBefore__11CMenuGCItemFv)(
        (CMenuGCItem*)((uintptr_t)self - 0x58));
}

/**
 * IScnRender vtable this-adjusting thunk for ~CMenuGCItem.
 *
 * Same adjustment as func_802B0F08 but forwards to the destructor.
 * r4 (the MWCC deletion flag) is preserved from the caller because the cast
 * to void(*)(CMenuGCItem*) declares only one parameter, leaving r4 unmodified
 * in the tail call.
 *
 * Retail: subi r3, r3, 0x58; b __dt__11CMenuGCItemFv
 */
void func_802B0F10(IScnRender* self){
    ((void(*)(CMenuGCItem*))__dt__11CMenuGCItemFv)(
        (CMenuGCItem*)((uintptr_t)self - 0x58));
}
