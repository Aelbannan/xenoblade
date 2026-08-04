// FULL_MATCH: func_8029BECC, func_8029BED4

#include "kyoshin/menu/CMenuOption.hpp"

#include "kyoshin/harness_catalog.hpp"

void __ct__CMenuOption(){}

CMenuOption::~CMenuOption() {}

void CMenuOption::Init() {}

void CMenuOption::Term() {}

void CMenuOption::Move() {}

void CMenuOption::cbRenderBefore() {}

void func_8029BB24(){}


void func_8029BBB0(){}

void func_8029BC28(){}

void func_8029BC78(){}

void func_8029BE7C(){}

/**
 * IScnRender vtable this-adjusting thunk for cbRenderBefore.
 *
 * IScnRender is a non-primary base at offset 0x58 within CMenuOption.
 *
 * Retail: subi r3, r3, 0x58; b cbRenderBefore__11CMenuOptionFv
 */
void CMenuOption::func_8029BECC() {
    cbRenderBefore();
}

/**
 * IScnRender vtable this-adjusting thunk for ~CMenuOption.
 *
 * Same adjustment as func_8029BECC but forwards to the destructor.
 *
 * Retail: subi r3, r3, 0x58; b __dt__11CMenuOptionFv
 */
void CMenuOption::func_8029BED4() {
    this->~CMenuOption();
}

extern unsigned long lbl_eu_80664A38;
extern "C" unsigned long func_8029BBA0(void) { return lbl_eu_80664A38 != 0; }
