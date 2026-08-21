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

// retail: lwz r3, lbl_eu_80663C74; blr
extern "C" u32 func_804E6C78() { return lbl_eu_80663C74; }

void func_804E6C80() {}
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
namespace AUBlob { extern "C" void __dt__9CMdlAnmUVFv(); }
extern "C" u32 lbl_eu_8066B348;    // foreign .sdata2 string
extern "C" u32 lbl_eu_80663C74; // this unit's sdata

// [.data] 0x805701C0-0x805701D0 (16B): CMdlAnmUV vtable
extern "C" u32 lbl_eu_805701C0[4] = {
    (u32)&lbl_eu_80663C78, 0x00000000,
    (u32)&AUBlob::__dt__9CMdlAnmUVFv, 0x00000000,
};

// [.rodata] 0x805247D0-0x805247E8 (24B)
extern "C" __declspec(align(4)) const char lbl_eu_805247D0[0x18] = {
    0x43,0x4D,0x64,0x6C,0x41,0x6E,0x6D,0x55,0x56,0x00,0x00,0x00,
    0x25,0x73,0x25,0x64,0x25,0x64,0x00,0x00,0x00,0x00,0x00,0x00,
};

// [.sdata] 0x80663C74-0x80663C88 (20B, align 4 -- all u32 members)
// [.sdata] 0x80663C74-0x80663C88 (20B). Retail align is 4 with the 8B label
// 80663C78 at +4; MWCC 8-aligns the 8B u32[2] (->0x18) -- needs UNIT_RULES
// set_data_align=(('.sdata',4)) + layout fix. Best-effort definition:
__asm__(".section .sdata,\"aw\"\n"
".global lbl_eu_80663C74\n lbl_eu_80663C74: \n\t.long lbl_eu_8066B348\n"
".global lbl_eu_80663C78\n lbl_eu_80663C78: \n\t.long lbl_eu_805247D0\n"
".global lbl_eu_80663C7C\n lbl_eu_80663C7C: \n\t.long 0\n"
".global lbl_eu_80663C80\n lbl_eu_80663C80: \n\t.long 0x72656600\n"
".global lbl_eu_80663C84\n lbl_eu_80663C84: \n\t.long 0x72656600\n"
".text\n");
