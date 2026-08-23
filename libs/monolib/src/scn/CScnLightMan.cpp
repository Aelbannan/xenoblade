// Decompiled for monolib/src/scn/CScnLightMan

#include <types.h>
#include "libs/monolib/src/scn/CScnLightMan.hpp"
#include "libs/monolib/src/scn/CScnEnvLgtCtrl.hpp"

extern u32 lbl_eu_8056E568[];   // CScnLightMan vtable
// Flat retail helpers (C linkage keeps the exact retail symbol names).
extern "C" CScnItemLight* func_80482398(u8* param, int val);
extern "C" CScnLightList* func_8048C698(u8* pool, int kind);  // scene-pool list accessor
extern "C" void func_804954AC(u8* self, int val);
extern "C" void func_804959E8(u8* self, int val);
// Retail references this helper under its unmangled C name.
extern "C" void func_804BF944(u8* self);

// Selects the active light: if the current pool light is armed, deactivate
// its param block (func_804959E8) and clear the arm flag.
// Flat retail symbol; declared extern "C" in CScnEnvLgtCtrl.hpp.
extern "C" void func_8048D160(CScnLightMan* self) {
    CScnItemLight* light = self->mLight;
    if (light->flag121C) {
        func_804959E8(&light->_14[0], 0);
        self->mLight->flag121C = 0;
    }
}

// Flat retail symbol (no class-size mangling), so it cannot be a regular
// mangled member ctor; returns self.
extern "C" CScnLightMan* __ct__CScnLightMan(CScnLightMan* self, CScnLightParam* param) {
    CScnItemLight* item;

    *(void**)self = (void*)lbl_eu_8056E568;
    self->mParam = param;
    item = func_80482398((u8*)param, 0);
    self->mLight = item;

    // Walk the scene pool's light-item list; keep the last item whose id is
    // zero as the active light (the one that gets armed by func_804954AC).
    CScnLightList* list = func_8048C698(self->mParam->mPool, 3);
    CScnLightNode* node = list->sentinel->next;
    while (node != list->sentinel) {
        item = node->item;
        func_804954AC(&item->_14[0], (u32)__cntlzw(item->id10) >> 5);
        if (item->id10 == 0) {
            self->mLight = item;
        }
        node = node->next;
    }
    return self;
}

CScnLightMan::~CScnLightMan() {}

// Arms the selected light item: runs the helper on arg2, then sets the arm
// flag (+0x121c) of the item pointer held at +0x8 of arg1.
extern "C" void func_8048D124(u8* arg1, u8* arg2) {
    func_804BF944(arg2);
    char* ptr = *(char**)(arg1 + 8);
    ptr[0x121c] = 1;
}

// Virtual dispatch thunk: invokes slot 3 of the object at +0x8 (flat retail
// symbol, declared in CScnRootNw4r.hpp). VTarget models the callee vtable:
// v_i sits at vtable offset 8+4*i. No virtual dtor: retail emits none here.
struct VTarget {
    virtual void v0() = 0;
    virtual void v1() = 0;
    virtual void v2() = 0;
    virtual void v3() = 0;
    virtual void v4() = 0;
    virtual void v5() = 0;
    virtual void v6() = 0;
    virtual void v7() = 0;
    virtual void v8() = 0;
    virtual void v9() = 0;
    virtual void v10() = 0;
};

extern "C" void func_8048D1B0(u8* self) {
    VTarget* obj = (VTarget*)*(void**)(self + 8);
    obj->v3();
}
// ===== Dissolved monolibdata2 (blob surgery) data owned by this TU =====
// [.data] 0x8056E568-0x8056E598 (48B): CScnLightMan vtable. The typeinfo pair
// lbl_eu_80663950 and the 8 foreign slot targets live in other TUs.
extern "C" u32 lbl_eu_80663950;
extern "C" void __dt__12CScnLightManFv();
extern "C" u32 lbl_eu_8066A958;
extern "C" u32 lbl_eu_8066A960;
extern "C" u32 lbl_eu_8066A968;
extern "C" u32 lbl_eu_8066A970;
extern "C" u32 lbl_eu_8066A978;
extern "C" u32 lbl_eu_8066A980;
extern "C" u32 lbl_eu_8066A988;
extern "C" u32 lbl_eu_8056E568[12] = {
    (u32)&lbl_eu_80663950, 0x00000000,
    (u32)&__dt__12CScnLightManFv, 0x00000000,
    (u32)&lbl_eu_8066A958,
    (u32)&lbl_eu_8066A960,
    (u32)&lbl_eu_8066A968,
    (u32)&lbl_eu_8066A970,
    (u32)&lbl_eu_8066A978,
    (u32)&lbl_eu_8066A980,
    (u32)&lbl_eu_8066A988,
    0x00000000,
};

// [.rodata] 0x80523F58-0x80523F98 (64B): RTTI name + texture name + default
// light colour table + "SHA" tag.
extern "C" __declspec(align(8)) const char lbl_eu_80523F58[0xD] = { 0x43,0x53,0x63,0x6E,0x4C,0x69,0x67,0x68,0x74,0x4D,0x61,0x6E,0x00 };
extern "C" __declspec(align(4)) const char lbl_eu_80523F68[0xD] = { 0x73,0x68,0x61,0x64,0x6F,0x77,0x54,0x65,0x78,0x5F,0x49,0x34,0x00 };
extern "C" __declspec(align(8)) const f32 lbl_eu_80523F78[7] = { 1.0f, 0.7f, 0.7f, 0.5f, 0.5f, 0.7f, 0.7f };
extern "C" __declspec(section ".rodata") const char lbl_eu_80523F94[0x4] = { 0x53,0x48,0x41,0x00 };
