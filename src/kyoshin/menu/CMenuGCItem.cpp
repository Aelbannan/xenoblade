// FULL_MATCH: func_802B0F08, func_802B0F10

#include "kyoshin/menu/CMenuGCItem.hpp"

#include "kyoshin/harness_catalog.hpp"

void __ct__CMenuGCItem(){}

CMenuGCItem::~CMenuGCItem() {}

void CMenuGCItem::Init() {}

void CMenuGCItem::Term() {}

void CMenuGCItem::Move() {}

void CMenuGCItem::cbRenderBefore() {}

void func_8029BB24(){}

void func_8029BBA0(){}

void func_8029BBB0(){}

void func_8029BC28(){}

void func_8029BC78(){}

void func_8029BE7C(){}

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * IScnRender is a non-primary base at offset 0x58 within CMenuGCItem.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__11CMenuGCItemFv
 */
void CMenuGCItem::func_802B0F08() {
    cbRenderBefore();
}

/**
 * IScnRender vtable this-adjusting thunk for ~CMenuGCItem.
 *
 * Same adjustment as func_802B0F08 but forwards to the destructor.
 *
 * Retail: subi r3, r3, 0x58; b __dt__11CMenuGCItemFv
 */
void CMenuGCItem::func_802B0F10() {
    this->~CMenuGCItem();
}
