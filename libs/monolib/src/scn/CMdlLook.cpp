// Auto-scaffolded catalog TU for monolib/src/scn/CMdlLook
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>

struct CMdlLook {
    ~CMdlLook();
};


void __ct__CMdlLook(){}

CMdlLook::~CMdlLook() {}

void func_804E7B38(){}

void func_804E8220(){}

void func_804E8284(u8* self, float a, float b) { *(float*)(self + 0x58) = a; *(float*)(self + 0x5C) = b; }

void func_804E8290(){}

void func_804E830C(){}

void func_804E8AAC(){}

// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// forward refs (in-block definitions + foreign data labels)
extern "C" const char lbl_eu_80524810[0x9];
extern "C" u32 lbl_eu_8066B3C8;
extern "C" u32 lbl_eu_8066B3C0;
extern "C" u32 lbl_eu_8066B3B8;
extern "C" u32 lbl_eu_80663CA0[0x2];
// foreign function pointers (namespace-scoped extern "C" so the
// declarations cannot collide with in-scope class members; the
// C linkage keeps the emitted reloc names unmangled)
namespace D2Blob {
extern "C" void __dt__8CMdlLookFv();
}

// [.data] 0x805701E0-0x805701FC (28 bytes)
extern "C" u32 lbl_eu_805701E0[0x4] = { (u32)&lbl_eu_80663CA0, 0x00000000, (u32)&D2Blob::__dt__8CMdlLookFv, 0x00000000 };
extern "C" u32 lbl_eu_805701F0[0x3] = { (u32)&lbl_eu_8066B3B8, (u32)&lbl_eu_8066B3C0, (u32)&lbl_eu_8066B3C8 };

// [.rodata] 0x80524810-0x80524830 (32 bytes)
extern "C" __declspec(align(4)) const char lbl_eu_80524810[0x9] = { 0x43, 0x4D, 0x64, 0x6C, 0x4C, 0x6F, 0x6F, 0x6B, 0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_8052481C[0x14] = { 0x4A, 0x55, 0x73, 0x70, 0x41, 0x00, 0x4A, 0x55, 0x68, 0x65, 0x61, 0x64, 0x00, 0x6D, 0x6F, 0x76, 0x65, 0x00, 0x00, 0x00 };

// [.sdata] 0x80663CA0-0x80663CB0 (16 bytes)
extern "C" u32 lbl_eu_80663CA0[0x2] = { (u32)&lbl_eu_80524810, 0x00000000 };
extern "C" u32 lbl_eu_80663CA8[0x1] = { 0x72656600 };
extern "C" u32 lbl_eu_80663CAC[0x1] = { 0x72656600 };

// DECOMP_FORCEACTIVE keeps the blob symbols alive under -ipa file.
DECOMP_FORCEACTIVE(CMdlLook_cpp, lbl_eu_80524810);
DECOMP_FORCEACTIVE(CMdlLook_cpp, lbl_eu_8066B3C8);
DECOMP_FORCEACTIVE(CMdlLook_cpp, lbl_eu_8066B3C0);
DECOMP_FORCEACTIVE(CMdlLook_cpp, lbl_eu_8066B3B8);
DECOMP_FORCEACTIVE(CMdlLook_cpp, lbl_eu_80663CA0);
DECOMP_FORCEACTIVE(CMdlLook_cpp, lbl_eu_805701E0);
DECOMP_FORCEACTIVE(CMdlLook_cpp, lbl_eu_805701F0);
DECOMP_FORCEACTIVE(CMdlLook_cpp, lbl_eu_8052481C);
DECOMP_FORCEACTIVE(CMdlLook_cpp, lbl_eu_80663CA8);
DECOMP_FORCEACTIVE(CMdlLook_cpp, lbl_eu_80663CAC);
