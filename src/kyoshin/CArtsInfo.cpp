// Auto-scaffolded catalog TU for kyoshin/CArtsInfo
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/CArtsInfo.hpp"

u8 CArtsInfo_getField48(CArtsInfo* self) { return self->field_0x48; }







u8 CArtsInfo_getField49(CArtsInfo* self) { return self->field_0x49; }





void func_80235DD8(){}

void CArtsInfo_setField54(CArtsInfo* self, u8 val) { self->field_0x54 = val; }

void CArtsInfo_setField55(CArtsInfo* self, u8 val) { self->field_0x55 = val; }

void CArtsInfo_setField56(CArtsInfo* self, u8 val) { self->field_0x56 = val; }

void CArtsInfo_setField58(CArtsInfo* self, u16 val) { self->field_0x58 = val; }

void func_80235EA4(){}

void func_80235EF0(){}

void func_80235F14(){}

u32 CArtsInfo_getField5A(CArtsInfo* self) {
    s8 val = self->field_0x5A;
    u32 result = __cntlzw(val);
    return result >> 5;
}

int CArtsInfo_isField44GE6(CArtsInfo* self) {
    int field = self->field_0x44;
    return field >= 6 ? 1 : 0;
}

void func_80235F6C(){}

void func_80236020(){}

void func_8023606C(){}

void func_80236120(){}

void func_8023616C(){}

void func_80236220(){}

void func_802362D4(){}

void func_80236334(){}

void func_80236408(){}

void func_80236454(){}

void func_80236508(){}

void func_802369C0(){}

void func_80236CF4(){}

void func_80236DB8(){}

void func_80236DF0(){}

void func_80236E28(){}

void func_80236E6C(){}

void func_80237050(){}

void func_802370A8(){}

void func_80237100(){}

void func_8023719C(){}

void func_80237238(){}

void func_80237394(){}

void func_802374F0(){}

void func_8023754C(){}

void func_802375A8(){}

void func_80237A0C(){}

void func_80237B88(){}

void func_80237D58(){}

void func_80237E24(){}

void func_80238038(){}

void func_80238298(){}

void func_802384F4(){}

void func_80238904(){}

void func_80239030(){}

void func_8023916C(){}

void func_8023939C(){}

void func_8023959C(){}

void func_802397F4(){}

void func_80239964(){}

void func_80239AA0(){}

void func_80239BDC(){}

void func_80239D20(){}

void func_80239EFC(){}

void func_80239FC4(){}

void func_8023A148(){}

void func_8023A210(){}

void func_8023A2D8(){}

void func_8023A398(){}

void func_8023A460(){}

void func_8023A55C(){}

void func_8023A60C(){}

void func_8023A6BC(){}

void func_8023A76C(){}

void func_8023A81C(){}

void func_8023A8CC(){}

void func_8023A97C(){}

void func_8023AA2C(){}

void func_8023AADC(){}

void func_8023AB8C(){}

void func_8023AD5C(){}

void func_8023AE24(){}

void func_8023AF60(){}

void func_8023B074(){}

void func_8023B12C(){}

void func_8023B280(){}

void func_8023B368(){}

void func_eu_8023D490(){}

void CArtsInfo_initialize(CArtsInfo* self) {
    if (self->field_0x20 == 0) return;
    if (self->field_0x4C == 0) return;
    if (self->field_0x50 != 0) {
        self->field_0x48 = 1;
        self->field_0x40 = 1;
    }
}

void CArtsInfo::OnFileEvent() {}

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8023BC8C(){}
