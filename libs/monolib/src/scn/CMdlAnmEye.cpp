// Auto-scaffolded catalog TU for monolib/src/scn/CMdlAnmEye
#include <harness_catalog.h>
#include "libs/monolib/src/scn/CMdlAnmEye.hpp"

CMdlAnmEye::~CMdlAnmEye() {}

CMdlAnmEye::CMdlAnmEye() {
    field_04 = 0;
    field_1C = 0;
    field_20 = 0;
    field_24 = 0;
    field_28 = lbl_eu_8066B360;
    value2C = 1;
}

void func_804E77BC(u8* self, u32 val) {
    ((CMdlAnmEye*)self)->value2C = val;
}

void func_804E75B8() {}
void func_804E77C4() {}