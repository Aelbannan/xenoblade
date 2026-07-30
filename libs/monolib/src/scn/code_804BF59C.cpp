// Auto-scaffolded catalog TU for monolib/src/scn/code_804BF59C
// Replace stubs with high-level C/C++ during decomp.
//
// This TU defines a light-environment management class containing:
//   - 8 CLight objects (offset 0x00, 0x40 bytes each)
//   - 8 nw4r::g3d::LightObj objects (offset 0x200, 0x44 bytes each)
//   - Ambient/directional light parameters and flags (offset 0x420+)
//
// Used by CScnEnvLgtCtrl and CVirtualLightObj for scene lighting.

#include <harness_catalog.h>
#include <PowerPC_EABI_Support/Runtime/MWCPlusLib.h>
#include "monolib/scn/code_804BF59C.hpp"

// ---------------------------------------------------------------------------
// FULL_MATCH: No-op default constructor for trivial sub-object arrays.
//
// Used as a constructor callback by __construct_array in sinit_804BF540
// to initialize a BSS array of 32 objects (0x24 bytes each) at lbl_eu_8065F428.
// Since BSS provides zero-initialization, runtime construction is a no-op.
// ---------------------------------------------------------------------------
void func_804BF59C(void) {}

void __ct__804BF5A0(){}

// ---------------------------------------------------------------------------
// CScnEnvLgtData deleting destructor (destructor(4)).
//
// Takes the object pointer in r3 and the deletion flag in r4. If the object
// pointer is non-null, destroys the two member arrays in reverse declaration
// order (mLightObjs at this+0x200, mLights at this+0x0). When the deletion
// flag is positive, frees the object via operator delete (MWCC's __dl__FPv).
// Returns the object pointer in r3.
//
// The mangled symbol is fragment-anchored to 0x804BF6F4 (the class type-info
// address), so it is emitted as a stand-alone function rather than via a
// C++ class destructor definition. __destroy_arr is the MWCC runtime helper
// that walks an array invoking the supplied destructor in reverse order.
// ---------------------------------------------------------------------------
extern "C" void* __dt__804BF6F4(void* self, int deleting) {
    if (self != nullptr) {
        // Destroy the 8 nw4r::g3d::LightObj subobjects at this+0x200 (size 0x44 each).
        // The MWCC runtime helper __destroy_arr takes a pointer to a function
        // pointer; we declare local temporaries holding the destructor's address.
        extern void __dt__Q34nw4r3g3d8LightObjFv(void*, int);
        extern void __dt__6CLightFv(void*, int);
        __destroy_arr(static_cast<char*>(self) + 0x200,
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__Q34nw4r3g3d8LightObjFv))),
                      0x44, 8);

        // Destroy the 8 CLight subobjects at this+0x0 (size 0x40 each).
        __destroy_arr(self,
                      reinterpret_cast<ConstructorDestructor*>(
                          const_cast<void*>(reinterpret_cast<const void*>(&__dt__6CLightFv))),
                      0x40, 8);

        // When the deleting flag is positive, free the object via operator delete.
        if (deleting > 0) {
            extern void __dl__FPv(void*);
            __dl__FPv(self);
        }
    }
    return self;
}

void func_804BF774(){}

// ---------------------------------------------------------------------------
// Clears all 8 nw4r::g3d::LightObj subobjects in mLightObjs and resets the
// ambient/directional float fields plus the active-light count.
//
// For each LightObj, calls Clear() and then masks FLAG_ENABLE_LIGHT (bit 2)
// out of mFlag. The floating-point fields at 0x438-0x440 and 0x450-0x458
// are reset to 0.0f (lbl_eu_8066AF80), and mActiveLightCount at 0x480 is
// reset to 0.
//
// Implemented as a stand-alone function because CScnEnvLgtData does not
// declare this method; the catalog retains the original retail address
// 0x804C3A04 and only its body is reconstructed.
// ---------------------------------------------------------------------------
extern "C" void func_804BF8A8(CScnEnvLgtData* self) {
    // Walk each of the 8 LightObj entries: call Clear() on the cursor and
    // mask FLAG_ENABLE_LIGHT (LSB bit 2) out of mFlag. The retail codegen
    // keeps three pointers live (r28 = this, r30 = LightObj for the Clear()
    // call, r31 = the lagging base pointer used for the flag load/store)
    // plus the counter (r29).
    char* clearCursor = reinterpret_cast<char*>(&self->mLightObjs[0]);
    char* flagCursor = reinterpret_cast<char*>(self);
    for (u32 i = 0; i < 8; ++i) {
        reinterpret_cast<nw4r::g3d::LightObj*>(clearCursor)->Clear();
        // Clear the FLAG_ENABLE_LIGHT bit (LSB bit 2) from mFlag. The retail
        // encodes this as rlwinm r0,r0,0,30,28, which in PPC's MSB-first bit
        // numbering clears PPC bit 29 = LSB bit 2.
        *((u32*)(flagCursor + 0x200)) = *((u32*)(flagCursor + 0x200)) & 0xFFFFFFFBU;
        clearCursor += 0x44;
        flagCursor += 0x44;
    }

    // Reset the six float fields to 0.0f and the active-light count to 0.
    // The store order must match the retail (0x450 cluster first, then 0x438).
    extern const f32 lbl_eu_8066AF80;
    f32* floats450 = self->mAmbColor;  // 0x450
    floats450[0] = lbl_eu_8066AF80;
    floats450[1] = lbl_eu_8066AF80;
    floats450[2] = lbl_eu_8066AF80;
    f32* floats438 = self->mAmbColorBase;  // 0x438
    floats438[0] = lbl_eu_8066AF80;
    floats438[1] = lbl_eu_8066AF80;
    floats438[2] = lbl_eu_8066AF80;
    self->mActiveLightCount = 0;
}

// ---------------------------------------------------------------------------
// FULL_MATCH: Placeholder method called after updating light parameters.
//
// Passed 'this' in r3 from CScnEnvLgtCtrl and CVirtualLightObj call sites.
// Currently a no-op in the base implementation; likely overridden in derived
// classes for post-processing after light parameter updates.
// ---------------------------------------------------------------------------
void func_804BF940(void) {}

void func_804BF944(){}

void func_804BFA70(){}

void func_804BFEB4(){}

void func_804BFFB8(){}

// Sets bit 0x80 of mFlags when 'enable' is non-zero, and clears bit 0x100
// either way.
extern "C" void func_804C0228(CScnEnvLgtData* self, int enable) {
    if (enable != 0) {
        self->mFlags = (self->mFlags & ~0x100) | 0x80;
    } else {
        self->mFlags &= ~0x80;
    }
}

// Sets bit 0x100 of mFlags when 'enable' is non-zero, and clears bit 0x80
// either way.
extern "C" void func_804C0254(CScnEnvLgtData* self, int enable) {
    if (enable != 0) {
        self->mFlags = (self->mFlags & ~0x80) | 0x100;
    } else {
        self->mFlags &= ~0x100;
    }
}
