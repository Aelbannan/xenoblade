// Auto-scaffolded catalog TU for kyoshin/COption
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/COption.hpp"
#include "kyoshin/code_80135FDC.hpp"
#include "kyoshin/CScrollBar.hpp"
#include "kyoshin/CSysWin.hpp"

extern float lbl_eu_80668C10;

void __dt__8CBaseCurFv(CBaseCur*, int);
int CScrollBar_isVisible(CScrollBar*);
u32 CSysWin_isReady(CSysWinFull*);

u8 func_8029C790(void* self) { return static_cast<COptionFull*>(self)->field_2B; }










void func_8029C5C8(){}

void func_8029C66C(){}

// Returns field_0x2A only when the scrollbar is visible and
// the syswin is ready; otherwise returns 0.
u8 func_8029C734(COptionFull* self) {
    if (!CScrollBar_isVisible((CScrollBar*)self->mScrollBar)) {
        return 0;
    }
    if (!CSysWin_isReady((CSysWinFull*)self->mSysWin)) {
        return 0;
    }
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

void func_8029D0C0(){}

void func_8029D10C(){}

void func_8029D178(){}

// Checks if an animation at field_0x20 has reached a target frame;
// if so, resets field_0x29 and marks field_2B as active.
void func_8029D1C4(COptionFull* self) {
    float f = lbl_eu_80668C10;
    if (func_80137510(self->field_0x20, f)) {
        self->field_0x29 = 0;
        self->field_2B = 1;
    }
}

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

// Complete object destructor for CCur19 (CBaseCur subclass).
// Standard MWCC virtual dtor: null-check, call base dtor with flag 0,
// conditionally operator delete, return this.
CCur19* __dt__8029BF18(CCur19* _this, int flags) {
    if (_this) {
        __dt__8CBaseCurFv(_this, 0);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern void func_80296A04__FP14Class_80296898(void*);
extern u8 lbl_eu_80577308[];
void sinit_8029E7D8(){
    func_80296A04__FP14Class_80296898(lbl_eu_80577308);
}
