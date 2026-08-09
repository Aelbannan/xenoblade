// monolib/src/scn/CMdlAnmUV - UV-animation helper object.
#include <harness_catalog.h>
#include "libs/monolib/src/scn/CMdlAnmUV.hpp"

CMdlAnmUV::CMdlAnmUV() {
    field_04 = 0;
    field_38 = 0;
    field_3c = 0;
    field_3d = 0;
}

CMdlAnmUV::~CMdlAnmUV() {}

void func_804E72D0() {}

// retail: lwz r3, lbl_eu_80663C74; blr
extern "C" u32 func_804E6C78() { return (u32)lbl_eu_80663C74; }

void func_804E6C80() {}