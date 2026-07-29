// Auto-scaffolded catalog TU for kyoshin/COption
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/COption.hpp"

CCur19::CCur19(nw4r::lyt::ArcResourceAccessor* arcResAcc) : CBaseCur(arcResAcc) {
    mVtable = lbl_eu_805396D0;
}

u8 func_8029C790(void* self) { return static_cast<COptionFull*>(self)->field_2B; }










void func_8029C5C8(){}

void func_8029C66C(){}

void func_8029C734(){}


u8 func_8029C798(void* self) { return static_cast<COptionFull*>(self)->field_30; }

u8 func_8029C7A0(void* self) { return static_cast<COptionFull*>(self)->field_32; }

void func_8029C7A8(){}

void func_8029C8C4(){}

void func_8029C9E8(){}

void func_8029CABC(){}

void func_8029CB9C(){}

void func_8029CC30(){}

void func_8029CC9C(){}

void func_8029CDB0(){}

void func_8029CF7C(){}

void func_8029D054(){}

// External nw4r animation helpers
int func_80137444(nw4r::lyt::AnimTransform*, float);
int func_80137510(nw4r::lyt::AnimTransform*, float);

// Called when AnimTransform at +0x20 finishes its animation
void func_8029D0C0(CCur19* self) {
    if (func_80137444(self->mpAnimTrans2, 1.0f)) {
        self->mState = 2;
        func_8029E1CC(self);
    }
}

void func_8029D10C(){}

// Called when AnimTransform at +0x24 finishes its animation
void func_8029D178(CCur19* self) {
    if (func_80137510(self->mpAnimTrans3, 1.0f)) {
        self->mState = 5;
        func_8029E144(self);
    }
}

void func_8029D1C4(){}

void func_8029D210(){}

void func_8029D278(){}

void func_8029D2F0(){}

void func_8029D358(){}

void func_8029D3C0(){}

void func_8029D420(){}

void func_8029D634(){}

void func_8029D7E8(){}

void func_8029D7FC(){}

void func_8029D96C(){}

void func_8029D990(){}

void func_8029DD6C(){}

void func_8029E144(CCur19* self){}

void func_8029E1CC(CCur19* self){}

void func_8029E254(){}

void func_8029E3F8(){}

void COption::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern void func_80296A04__FP14Class_80296898(void*);
extern u8 lbl_eu_80577308[];
void sinit_8029E7D8(){
    func_80296A04__FP14Class_80296898(lbl_eu_80577308);
}
