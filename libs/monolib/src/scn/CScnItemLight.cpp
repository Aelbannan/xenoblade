// Decompiled for monolib/src/scn/CScnItemLight

#include <harness_catalog.h>
#include <string.h>
#include "libs/monolib/src/scn/CScnItemLight.hpp"
#include <nw4r/g3d/g3d_scnroot.h>

// Opaque retail helper functions resolved by the symbol map (C linkage so the
// emitted relocations keep the exact retail symbol names).
extern "C" {
    void* func_80496018(void* r3);
    void func_804950F4(u8* self, void* r4);
    void __dt__80495200(u8* self, int deleting);
    void func_804936AC(void* a, void* b, void* c);
}

// VTable for CScnItemLight (manually-managed data pointer at +0x00).
// char[] array type prevents SDA21 addressing so the address is materialised
// with lis+addi exactly like retail.
extern u32 lbl_eu_8056DD08[];

extern "C" void* func_804823A0(u8* self) { return (void*)((u8*)self + 0x11d8); }

void __ct__CScnItemLightNw4r(void*, int);
extern "C" void func_80482398(u8* self, int val) {
    __ct__CScnItemLightNw4r(self, (s32)(s16)val);
}

// func_80482388: tail-call which forwards the owning CScnItemLightNw4r's
// light-environment handle (+0x5C), this light item's env (+0x14) and the
// owner into func_804936AC.
extern "C" void func_80482388(CScnItemLight* self) {
    func_804936AC(self->mpNw4r->field_0x5c, self->mEnv, self->mpNw4r);
}

// func_80482400: push this light item's nw4r g3d LightSetting (+0x944) into
// the scene root's LightSetting (ScnRoot::mLightSetting at +0x2878) via
// LightSetting::Import, then clear the active flag.
extern "C" void func_80482400(CScnItemLightNw4r* self) {
    self->mpScnRoot->mLightSetting.Import(
        reinterpret_cast<const nw4r::g3d::LightSetting&>(self->mLightSetting));
    self->field_0x121c = 0;
}

CScnItemLight::CScnItemLight(CScnItemLightNw4r* pNw4r, char* name, u16 a2,
                             void* a3) {
    mpNw4r = pNw4r;
    field_0x8 = 3;
    field_0x0 = (void*)lbl_eu_8056DD08;
    field_0xc = a3;
    field_0x10 = a2;
    func_804950F4(mEnv, func_80496018(pNw4r));
    field_0x1218 = strlen(name);
    strcpy(mName, name);
    field_0x121c = 0;
}

// The base CScnItemLight dtor is declared `__declspec(noinline)` below, so this
// derived dtor must emit an external `bl __dt__13CScnItemLightFv` instead of
// inlining its body (retail keeps the base-dtor call out-of-line).
CScnItemLightNw4r::~CScnItemLightNw4r() {
    // base CScnItemLight destructor runs automatically (flag 0).
}

// `__declspec(noinline)` on the base dtor so the derived Nw4r dtor emits a
// real `bl __dt__13CScnItemLightFv` instead of inlining this body.
__declspec(noinline) CScnItemLight::~CScnItemLight() {
    __dt__80495200(mEnv, -1);
}
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056DD08-0x8056DD70 (104B): primary + secondary vtable groups.
// lbl_eu_806638C0/806638C8 (typeinfo pairs) and lbl_eu_806624D8 live in
// foreign TUs.
extern "C" u32 lbl_eu_806638C0;
extern "C" u32 lbl_eu_806638C8;
extern "C" u32 lbl_eu_806624D8;
extern "C" void __dt__13CScnItemLightFv();
extern "C" void __dt__17CScnItemLightNw4rFv();
extern "C" void func_80482040();
extern "C" void func_80482048();
extern "C" void func_80482288();
extern "C" u32 lbl_eu_8056DD08[8] = {
    (u32)&lbl_eu_806638C0, 0x00000000,
    (u32)&__dt__13CScnItemLightFv,
    (u32)&func_80482388,
    (u32)&func_80482048,
    (u32)&func_80482288,
    (u32)&func_804823A0,
    (u32)&func_80482040,
};
DECOMP_FORCEACTIVE(CScnItemLight_cpp, lbl_eu_8056DD08);
extern "C" u32 lbl_eu_8056DD28[4] = {
    (u32)&lbl_eu_806624D8, 0x00000000, 0x00000000, 0x00000000,
};
DECOMP_FORCEACTIVE(CScnItemLight_cpp, lbl_eu_8056DD28);
extern "C" u32 lbl_eu_8056DD38[8] = {
    (u32)&lbl_eu_806638C8, 0x00000000,
    (u32)&__dt__17CScnItemLightNw4rFv,
    (u32)&func_80482388,
    (u32)&func_80482048,
    (u32)&func_80482400,
    (u32)&func_804823A0,
    (u32)&func_80482040,
};
DECOMP_FORCEACTIVE(CScnItemLight_cpp, lbl_eu_8056DD38);
extern "C" u32 lbl_eu_8056DD58[6] = {
    (u32)&lbl_eu_806624D8, 0x00000000,
    (u32)&lbl_eu_806638C0, 0x00000000, 0x00000000, 0x00000000,
};
DECOMP_FORCEACTIVE(CScnItemLight_cpp, lbl_eu_8056DD58);

// [.rodata] 0x80523EA8-0x80523F18 (112B): RTTI names + debug format strings.
extern "C" __declspec(align(8)) const char lbl_eu_80523EA8[0xE] = { 0x43,0x53,0x63,0x6E,0x49,0x74,0x65,0x6D,0x4C,0x69,0x67,0x68,0x74,0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80523EB8[0x12] = { 0x43,0x53,0x63,0x6E,0x49,0x74,0x65,0x6D,0x4C,0x69,0x67,0x68,0x74,0x4E,0x77,0x34,0x72,0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80523ECC[0xB] = { 0x47,0x6C,0x6F,0x62,0x61,0x6C,0x28,0x25,0x64,0x29,0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80523ED8[0x1B] = { 0x6E,0x77,0x34,0x72,0x3A,0x3A,0x67,0x33,0x64,0x3A,0x3A,0x49,0x53,0x63,0x6E,0x4F,0x62,0x6A,0x43,0x61,0x6C,0x6C,0x62,0x61,0x63,0x6B,0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80523EF4[0x24] = { 0x6E,0x77,0x34,0x72,0x3A,0x3A,0x67,0x33,0x64,0x3A,0x3A,0x49,0x43,0x61,0x6C,0x63,0x57,0x6F,0x72,0x6C,0x64,0x43,0x61,0x6C,0x6C,0x62,0x61,0x63,0x6B,0x00,0x00,0x00,0x00,0x00,0x00 };  // +6B tail pad
DECOMP_FORCEACTIVE(CScnItemLight_cpp, lbl_eu_80523EA8);
DECOMP_FORCEACTIVE(CScnItemLight_cpp, lbl_eu_80523EB8);
DECOMP_FORCEACTIVE(CScnItemLight_cpp, lbl_eu_80523ECC);
DECOMP_FORCEACTIVE(CScnItemLight_cpp, lbl_eu_80523ED8);
DECOMP_FORCEACTIVE(CScnItemLight_cpp, lbl_eu_80523EF4);
