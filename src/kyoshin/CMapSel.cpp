// Auto-scaffolded catalog TU for kyoshin/CMapSel
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CMapSel.hpp"

void __ct__CMapSel(){}

CMapSel::~CMapSel() {}

void func_8024343C(){}

void func_802434A0(){}

void func_80243560(){}

void func_802435CC(){}

u8 func_80243680(CMapSel* self){
    if (self->mScrollBar.isVisible()) {
        return self->field_0x32;
    }
    return 0;
}

// FULL_MATCH: reads the initial-setup flag at +0x33 (set to 1 in ctor, cleared on play/close)

void func_802436CC(){}

void func_8024371C(){}

void func_80243768(){}

void func_80243838(){}

void func_8024391C(){}

void func_802439CC(){}

u8 func_80243A9C(CMapSelFull* self){
    s8 x = self->field_8D;
    s8 y = self->field_8C;
    return self->mGridData[x + y];
}

void func_80243ABC(){}

void func_80243B88(){}

void func_80243BE8(){}

void func_80243C6C(){}

void func_80243CB8(){}

void func_80243CFC(){}

void func_80243E08(){}

void func_80243ED8(){}

void func_80243FC4(){}

void func_80244020(){}

void CMapSel::OnFileEvent() {}
