// monolib/src/scn/CMdlAnmUV - UV-animation helper object.
#include <harness_catalog.h>
#include "libs/monolib/src/scn/CMdlAnmUV.hpp"

CMdlAnmUV::CMdlAnmUV() {
    field_04 = 0;
    field_38 = 0;
    field_3c = 0;
    field_3d = 0;
}

extern "C" void __dt__9CMdlAnmUVFv(CMdlAnmUV* self) {}

void func_804E72D0() {}
void func_804E6C80() {}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace AUBlob { extern "C" void __dt__9CMdlAnmUVFv(); }
extern "C" u32 lbl_eu_8066B348;    // foreign .sdata2 string

// [.rodata] 0x805247D0-0x805247E8 (24B)
extern "C" __declspec(align(4)) const char lbl_eu_805247D0[0x18] = {
    0x43,0x4D,0x64,0x6C,0x41,0x6E,0x6D,0x55,0x56,0x00,0x00,0x00,
    0x25,0x73,0x25,0x64,0x25,0x64,0x00,0x00,0x00,0x00,0x00,0x00,
};

// [.sdata] 0x80663C74-0x80663C88 (20B, 5 words) -- defined as 5 separate scalars to satisfy header's scalar decl for lbl_eu_80663C74
// Header declares lbl_eu_80663C74 as u32; we define it and the following 4 words as separate globals placed contiguously in .sdata
extern "C" u32 lbl_eu_80663C74 = (u32)&lbl_eu_8066B348;
extern "C" u32 lbl_eu_80663C78 = (u32)&lbl_eu_805247D0;
extern "C" u32 lbl_eu_80663C7C = 0;
extern "C" u32 lbl_eu_80663C80 = 0x72656600;
extern "C" u32 lbl_eu_80663C84 = 0x72656600;

// retail: lwz r3, lbl_eu_80663C74; blr
extern "C" u32 func_804E6C78() { return lbl_eu_80663C74; }

// [.data] 0x805701C0-0x805701D0 (16B): CMdlAnmUV vtable
extern "C" u32 lbl_eu_805701C0[4] = {
    (u32)&lbl_eu_80663C78, 0x00000000,
    (u32)&AUBlob::__dt__9CMdlAnmUVFv, 0x00000000,
};
