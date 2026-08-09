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
extern char lbl_eu_8056DD08[];

void* func_804823A0(u8* self) { return (void*)((u8*)self + 0x11d8); }

void __ct__CScnItemLightNw4r(void*, int);
extern "C" void func_80482398(u8* self, int val) {
    __ct__CScnItemLightNw4r(self, (s32)(s16)val);
}

// func_80482388: tail-call which forwards the owning CScnItemLightNw4r's
// light-environment handle (+0x5C), this light item's env (+0x14) and the
// owner into func_804936AC.
void func_80482388(CScnItemLight* self) {
    func_804936AC(self->mpNw4r->field_0x5c, self->mEnv, self->mpNw4r);
}

// func_80482400: push this light item's nw4r g3d LightSetting (+0x944) into
// the scene root's LightSetting (ScnRoot::mLightSetting at +0x2878) via
// LightSetting::Import, then clear the active flag.
void func_80482400(CScnItemLightNw4r* self) {
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