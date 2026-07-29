// Auto-scaffolded catalog TU for kyoshin/COption
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/COption.hpp"
#include "kyoshin/CBaseCur.hpp"
#include <nw4r/lyt.h>

// External labels and functions used by COption methods
extern float lbl_eu_80668C10;
extern char lbl_eu_805103D8[];

u32 func_80137444(nw4r::lyt::AnimTransform*, float);
void func_801D216C(void*, u8);
void func_801D2174(CBaseCur*);
void func_80124270(nw4r::lyt::Pane*, u32);
void func_8029E254(COption*);

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

void func_8029D0C0(){}

// Check if an animation transform has finished; if so, finalize state
// and activate sub-cursors before calling the shared tail handler.
void COption::func_8029D10C() {
    if (func_80137444(mAnimTransform24, lbl_eu_80668C10) != 0) {
        field_0x29 = 3;
        field_0x2B = 1;
        func_801D216C(&mSubCur1, 1);
        func_801D216C(&mSubCur2, 1);
        func_8029E254(this);
    }
}

void func_8029D178(){}

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

void func_8029E144(){}

void func_8029E1CC(){}

void func_8029E254(){}

void func_8029E3F8(){}

void COption::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern void func_80296A04__FP14Class_80296898(void*);
extern u8 lbl_eu_80577308[];
void sinit_8029E7D8(){
    func_80296A04__FP14Class_80296898(lbl_eu_80577308);
}
