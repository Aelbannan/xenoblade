// Auto-scaffolded catalog TU for monolib/src/scn/CVirtualLightObj
// Replace stubs with high-level C/C++ during decomp.

#include <harness_catalog.h>
#include <PowerPC_EABI_Support/Runtime/MWCPlusLib.h>
#include "libs/monolib/src/scn/CVirtualLightObj.hpp"

// Sets byte at offset 0x1174.
extern "C" void func_804954AC(CVirtualLightObj* self, u8 val) {
    self->mUnk1174 = val;
}

// ---------------------------------------------------------------------------
// CVirtualLightObj constructor: default light state - white color, unit
// intensity, disabled slot index (-1). The retail vtable is an unnamed data
// symbol (lbl_eu_8056E8B8), so the pointer is stored manually instead of
// going through the compiler's vtable machinery.
// ---------------------------------------------------------------------------
CVirtualLightObj::CVirtualLightObj() {
    mVtable = (u8*)lbl_eu_8056E8B8;
    mField04 = lbl_eu_8066AA88;
    mField08 = lbl_eu_8066AA88;
    mField0C = lbl_eu_8066AA88;
    mField10 = lbl_eu_8066AA88;
    mField14 = lbl_eu_8066AA88;
    mByte18 = 1;
    mByte19 = 0;
    mField1C = lbl_eu_8066AA88;
    mField20 = lbl_eu_8066AA88;
    mField24 = lbl_eu_8066AA88;
    mField28 = lbl_eu_8066AA88;
    mField2C = 0;
    mField30 = -1;
}

// ---------------------------------------------------------------------------
// Query whether this virtual light still carries its default (white) state.
// Returns the "default" sentinel while the color fields are untouched, and
// the "white" sentinel once any field has been customized.
// ---------------------------------------------------------------------------
extern "C" f32 func_8049488C(CVirtualLightObj* self) {
    if (self->mField14 <= lbl_eu_8066AA8C) {
        return lbl_eu_8066AA8C;
    }
    if (lbl_eu_8066AA8C == self->mField1C &&
        lbl_eu_8066AA8C == self->mField20 &&
        lbl_eu_8066AA8C == self->mField24) {
        return lbl_eu_8066AA8C;
    }
    if (lbl_eu_8066AA8C == self->mField04 &&
        lbl_eu_8066AA8C == self->mField08 &&
        lbl_eu_8066AA8C == self->mField0C) {
        return lbl_eu_8066AA8C;
    }
    return lbl_eu_8066AA88;
}

// ---------------------------------------------------------------------------
// Resolve the virtual light's current color into out. Lights whose type byte
// is 0 use the static black color; otherwise the two color triples (ambient
// at +0x04, diffuse at +0x1C) are scaled by the intensity at +0x14 and the
// alpha is the product of +0x10 and +0x28.
// ---------------------------------------------------------------------------
extern "C" void func_804947EC(ml::CCol4* out, CVirtualLightObj* in) {
    if (in->mByte18 == 0) {
        *out = ml::CCol4::black;
    } else {
        *out = ml::CCol4(in->mField04 * in->mField1C * in->mField14,
                         in->mField08 * in->mField20 * in->mField14,
                         in->mField0C * in->mField24 * in->mField14,
                         in->mField10 * in->mField28);
    }
}

void func_804952C4() {}

void func_804954B4() {}

// ---------------------------------------------------------------------------
// Select light slot `idx` on the environment: reset the slot color, clear
// every CLight referenced by the slot's bank, reset the slot state, and (for
// the first slot) release the shared "current" index.
// ---------------------------------------------------------------------------
extern "C" void func_80495644(CLightEnv* self, u32 idx) {
    self->mField1180 = 0;
    self->mSlotColors[idx].x = lbl_eu_8066AAA8;
    self->mSlotColors[idx].y = lbl_eu_8066AAA8;
    self->mSlotColors[idx].z = lbl_eu_8066AAA8;
    self->mSlotColors[idx].w = lbl_eu_8066AA98;
    for (u32 i = 0; i < self->mSlotCounts[idx]; i++) {
        func_804C03A0(reinterpret_cast<u8*>(&self->mSlotPtrs[idx][i]), 0);
    }
    self->mSlotFields[idx] = 0;
    if (idx == 0) {
        self->mField1170 = -1;
    }
    self->mField117C = idx;
}

void func_804956F8(void) {}

void func_80495704() {}

void func_804957E4() {}

void func_804958B8() {}

void func_804959E8() {}

void func_80495AF4() {}

extern "C" void func_804948F4() {}
extern "C" void func_80494A64() {}
extern "C" void func_80494C30() {}
extern "C" void func_80494D84() {}
extern "C" void func_80494F10() {}
extern "C" void func_804950F4() {}

// ---------------------------------------------------------------------------
// CLightEnv deleting destructor. Destroys the four CLight[8] banks in reverse
// declaration order (highest address first) followed by the 0x20 nw4r
// LightObj array at +0x0, then frees the object when the deleting flag is
// positive. Emitted as a stand-alone function because the retail symbol is
// fragment-anchored to 0x80495200 (no recoverable class name).
// ---------------------------------------------------------------------------
extern "C" void* __dt__80495200(CLightEnv* self, int deleting) {
    if (self != nullptr) {
        // MWCC runtime helpers: __destroy_arr walks the array calling the
        // given destructor in reverse order.
        extern void __dt__Q34nw4r3g3d8LightObjFv(void*, int);
        extern void __dt__6CLightFv(void*, int);
        __destroy_arr(self->mSlotLights[3],
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__6CLightFv))),
                      0x40, 8);
        __destroy_arr(self->mSlotLights[2],
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__6CLightFv))),
                      0x40, 8);
        __destroy_arr(self->mSlotLights[1],
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__6CLightFv))),
                      0x40, 8);
        __destroy_arr(self->mSlotLights[0],
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__6CLightFv))),
                      0x40, 8);
        __destroy_arr(self->mGxLights,
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__Q34nw4r3g3d8LightObjFv))),
                      0x44, 0x20);

        // When the deleting flag is positive, free the object via operator delete.
        if (deleting > 0) {
            extern void __dl__FPv(void*);
            __dl__FPv(self);
        }
    }
    return self;
}
