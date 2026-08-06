// Auto-scaffolded catalog TU for kyoshin/COption
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/COption.hpp"
#include "kyoshin/CBaseCur.hpp"
#include "kyoshin/code_80296898.hpp"
#include <nw4r/lyt.h>
#include <cstring>

// External labels and functions used by COption methods
extern float lbl_eu_80668C10;
extern char lbl_eu_805103D8[];

// CCur19 vtable and the Class_80296898 singleton for this TU.
extern "C" void* lbl_eu_805396D0[];
extern "C" Class_80296898 lbl_eu_80577308;
// Signed-index lookup table used by func_8029D96C.
extern "C" u8 lbl_eu_8053948C[0x30];

// CBaseCur base ctor/dtor and the C++ allocator free (C-linkage symbols).
extern "C" void __ct__8CBaseCurFv(CBaseCur*);
extern "C" void __dt__8CBaseCurFv(void*, int);
extern "C" void __dl__FPv(void*);

// Option window scroll-bar / system-window state checks (C-linkage).
extern "C" int CScrollBar_isVisible(void*);
extern "C" u32 CSysWin_isReady(void*);

u32 func_80137444(nw4r::lyt::AnimTransform*, float);
u32 func_80137510(nw4r::lyt::AnimTransform*, float);
void func_801D216C(void*, u8);
void func_801D2174(CBaseCur*);
void func_80124270(nw4r::lyt::Pane*, u32);
void func_8029E254(COption*);
void func_8029E1CC(COption*);
void func_8029E144(COption*);
void func_8029D420(COption*);
extern "C" void func_8029D634(COption*, u8);

u8 func_8029C790(void* self) { return static_cast<COptionFull*>(self)->field_2B; }










// Show pane "name1", hide pane "name2", then activate cursor.
extern "C" void func_8029C03C(COption* self) {
    nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x50, true);
    func_80124270(pane, 1);
    pane = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x5E, true);
    func_80124270(pane, 0);
    func_801D2174((CBaseCur*)self);
}

// Hide pane "name1", show pane "name2", then activate cursor.
extern "C" void func_8029C0C4(COption* self) {
    nw4r::lyt::Pane* pane = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x50, true);
    func_80124270(pane, 0);
    pane = self->mpLayout->GetRootPane()->FindPaneByName(lbl_eu_805103D8 + 0x5E, true);
    func_80124270(pane, 1);
    func_801D2174((CBaseCur*)self);
}

void func_8029C5C8(){}

void func_8029C66C(){}

// Returns the option-window visibility byte only if the embedded scroll bar
// is visible AND the system window is ready; otherwise 0.
extern "C" u8 func_8029C734(COptionWindow* self) {
    if (!CScrollBar_isVisible(&self->mScrollBar)) return 0;
    if (CSysWin_isReady(&self->mSysWin) == 0) return 0;
    return self->field_0x2A;
}
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

// Finish the option-panel animation at +0x20: when it has reached the target
// frame, set state 2 and run the shared tail handler func_8029E1CC.
extern "C" void func_8029D0C0(COption* self) {
    if (func_80137444(self->mAnimTransform20, lbl_eu_80668C10)) {
        self->field_0x29 = 2;
        func_8029E1CC(self);
    }
}

// Check if an animation transform has finished; if so, finalize state
// and activate sub-cursors before calling the shared tail handler.
extern "C" void func_8029D10C(COption* self) {
    if (func_80137444(self->mAnimTransform24, lbl_eu_80668C10) != 0) {
        self->field_0x29 = 3;
        self->field_0x2B = 1;
        func_801D216C(&self->mSubCur1, 1);
        func_801D216C(&self->mSubCur2, 1);
        func_8029E254(self);
    }
}

// Finish the +0x24 animation: set state 5 and run the shared handler func_8029E144.
extern "C" void func_8029D178(COption* self) {
    if (func_80137510(self->mAnimTransform24, lbl_eu_80668C10)) {
        self->field_0x29 = 5;
        func_8029E144(self);
    }
}

// Finish the +0x20 animation (different checker): clear state 0x29 and set 0x2B.
extern "C" void func_8029D1C4(COption* self) {
    if (func_80137510(self->mAnimTransform20, lbl_eu_80668C10)) {
        self->field_0x29 = 0;
        self->field_0x2B = 1;
    }
}

// CCur19 constructor: run the CBaseCur base constructor, then install the
// CCur19 vtable at +0x00.
extern "C" CBaseCur* __ct__CCur19(CBaseCur* _this) {
    __ct__8CBaseCurFv(_this);
    _this->mVtable = (void*)lbl_eu_805396D0;
    return _this;
}

// CCur19 destructor: tear down the CBaseCur base, then free the object only
// when the delete flag is positive (standard MWCC member-destructor shape).
extern "C" void* __dt__8029BF18(void* _this, int flags) {
    if (_this) {
        __dt__8CBaseCurFv(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

void func_8029D210(){}

void func_8029D278(){}

void func_8029D2F0(){}

void func_8029D358(){}

// If the +0x18 source is live, flag it, sync the singleton config (0x40 bytes)
// from getInstance() into this TU's global, then run func_8029D420.
extern "C" void func_8029D3C0(COption* self) {
    if (self->mpAnimTrans1 == 0) return;
    self->field_0x2A = 1;
    self->field_0x28 = 1;
    std::memcpy(&lbl_eu_80577308, Class_80296898::getInstance(), 0x40);
    func_8029D420(self);
}

void func_8029D420(COption* self) {}

// non-eliminable so sibling tail calls (e.g. func_8029D7E8) keep a real branch.
extern "C" void __declspec(noinline) func_8029D634(COption*, u8) {}

// Sum the bytes at 0x2C/0x2D into a u8 and pass to the state handler func_8029D634.
extern "C" void func_8029D7E8(COption* self) {
    func_8029D634(self, (u8)(self->field_0x2C + self->field_0x2D));
}

void func_8029D7FC(){}

// Look up a signed-index table entry via the sum of the signed 0x2D/0x2C bytes.
extern "C" u8 func_8029D96C(COption* self) {
    return lbl_eu_8053948C[(s8)self->field_0x2C + (s8)self->field_0x2D];
}

void func_8029D990(){}

void func_8029DD6C(){}

void func_8029E144(COption* self) {}

void func_8029E1CC(COption* self) {}

void func_8029E254(COption* self) {}

void func_8029E3F8(){}

void COption::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern "C" void func_80296A04__FP14Class_80296898(void*);

// sinit: constructor run for the Class_80296898 global at lbl_eu_80577308.
// Tail-calls the global constructor with the object address.
extern "C" void sinit_8029E7D8(){
    func_80296A04__FP14Class_80296898(&lbl_eu_80577308);
}
