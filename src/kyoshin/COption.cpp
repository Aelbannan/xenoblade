// Auto-scaffolded catalog TU for kyoshin/COption
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/COption.hpp"
#include "kyoshin/code_80296898.hpp"

// extern declarations for matched functions in other TUs
// CSysWin_isActive returns the field_36 byte from CSysWinFull
extern "C" int CSysWin_isActive(void*);
// func_801D216C sets mVisible on a CBaseCur subobject
extern "C" void func_801D216C(void*, int);

// Global buffer for game config data
extern u8 lbl_eu_80577308[];

// Forward declarations for callees in this TU
void func_8029D420(COption*);
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

void func_8029D10C(){}

void func_8029D178(){}

void func_8029D1C4(){}

// Called when the option menu transitions to a sub-state (CSysWin is active)
void func_8029D210(COption* self) {
    if (CSysWin_isActive(&self->syswin)) {
        self->field_0x29 = 3;
        self->field_0x2B = 1;
        func_801D216C(&self->cur1, 0);
        func_801D216C(&self->cur2, 1);
        func_8029E254(self);
    }
}

void func_8029D278(){}

// Similar to func_8029D210 but sets field_0x29 to 9 instead of 3
void func_8029D2F0(COption* self) {
    if (CSysWin_isActive(&self->syswin)) {
        self->field_0x29 = 9;
        self->field_0x2B = 1;
        func_801D216C(&self->cur1, 0);
        func_801D216C(&self->cur2, 1);
        func_8029E254(self);
    }
}

void func_8029D358(){}

// Initializes the option menu by copying game config and activating
void func_8029D3C0(COption* self) {
    if (self->field_0x18 != 0) {
        self->field_0x2A = 1;
        self->field_0x28 = 1;
        memcpy(lbl_eu_80577308, Class_80296898::getInstance(), 0x40);
        func_8029D420(self);
    }
}

void func_8029D420(COption*){}

void func_8029D634(){}

void func_8029D7E8(){}

void func_8029D7FC(){}

void func_8029D96C(){}

void func_8029D990(){}

void func_8029DD6C(){}

void func_8029E144(){}

void func_8029E1CC(){}

void func_8029E254(COption*){}

void func_8029E3F8(){}

void COption::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
extern void func_80296A04__FP14Class_80296898(void*);
void sinit_8029E7D8(){
    func_80296A04__FP14Class_80296898(lbl_eu_80577308);
}
