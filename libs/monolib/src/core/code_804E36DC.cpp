// Auto-scaffolded catalog TU for monolib/src/core/code_804E36DC
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include "monolib/core/code_804E36DC.hpp"

void func_804E536C();
void func_804DF2F0(u32 r3, u32 r4);
extern "C" float lbl_eu_8066B2E4;

void func_804E36DC(){}

void func_804E39E8(){}

void func_804E3B08(){}

void func_804E3B6C(){}

void func_804E3CCC(UnkStruct_804E36DC* self) {
    self->field_0x0 &= 0xFFFF7FFF;
    func_804E536C();
}

void func_804E3CDC(UnkStruct_804E36DC* self, float f1, float f2) {
    if (f1 == lbl_eu_8066B2E4) return;
    if (f2 == lbl_eu_8066B2E4) return;
    self->field_0x90 = f1;
    self->field_0x94 = f2;
}

void func_804E3CFC(UnkStruct_804E36DC* self) {
    func_804DF2F0(self->field_0xC, self->field_0x4);
}

void func_804E3D0C(UnkStruct_804E36DC* self, u32 value) {
    for (int i = 0; i < 4; i++) {
        if (self->field_0xDC[i] == 0) {
            self->field_0xDC[i] = value;
            return;
        }
    }
}

void func_804E3D48(UnkStruct_804E36DC* self, u32 value) {
    for (int i = 0; i < 4; i++) {
        if (self->field_0xDC[i] == value) {
            self->field_0xDC[i] = 0;
            return;
        }
    }
}

void func_804E3D88(){}

void func_804E3E2C(){}

void func_804E3EB4(){}

void func_804E3EF4(){}

void func_804E3FB0(){}

void func_804E4094(){}

void func_804E41B4(){}

void func_804E424C(){}

void __dt__804E42F4(){}
