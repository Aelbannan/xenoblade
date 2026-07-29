#pragma once

#include <types.h>
#include "monolib/scn/CLight.hpp"
#include <nw4r/g3d/g3d_light.h>

// Light environment data container class.
//
// This class manages the lighting state for the scene environment, containing:
//   - 8 CLight objects at offset 0x000 (0x40 bytes each = 0x200 total)
//   - 8 nw4r::g3d::LightObj objects at offset 0x200 (0x44 bytes each = 0x220 total)
//   - 0x420-0x428: vec3 (directional color base, defaults to 0)
//   - 0x42C-0x434: vec3 (directional color mul, defaults to 1)
//   - 0x438-0x440: vec3 (ambient base color)
//   - 0x444-0x44C: vec3 (ambient multiplier)
//   - 0x450-0x458: vec3 (computed ambient output)
//   - 0x45C-0x464: vec3 (directional base color)
//   - 0x468: float (lerp factor t)
//   - 0x46C: float (lerp speed)
//   - 0x470-0x478: vec3 (directional output color, defaults to 1)
//   - 0x47C: float (directional output alpha, defaults to 1)
//   - 0x480: s32 mActiveLightCount
//   - 0x484: u32 mFlags (bitfield)
//
// Total class size: 0x488 bytes.
//
// Used as a base or member by CScnEnvLgtCtrl and CVirtualLightObj.
class CScnEnvLgtData {
public:
    // 8 CLight objects (0x40 bytes each).
    CLight mLights[8];
    // 8 nw4r::g3d::LightObj objects (0x44 bytes each).
    nw4r::g3d::LightObj mLightObjs[8];
    // Directional color base (vec3 at 0x420, defaults to 0).
    f32 mDirColorBase2[3]; // 0x420
    // Directional color multiplier (vec3 at 0x42C, defaults to 1).
    f32 mDirColorMul[3]; // 0x42C
    // Ambient color base (vec3 at 0x438).
    f32 mAmbColorBase[3]; // 0x438
    // Ambient multiplier (vec3 at 0x444).
    f32 mAmbMul[3]; // 0x444
    // Computed ambient color (vec3 at 0x450).
    f32 mAmbColor[3]; // 0x450
    // Directional base color (vec3 at 0x45C).
    f32 mDirColorBase[3]; // 0x45C
    // Lerp factor in [0,1] (0x468).
    f32 mLerpT; // 0x468
    // Lerp speed (0x46C).
    f32 mLerpSpeed; // 0x46C
    // Directional output color (vec3 at 0x470, defaults to 1).
    f32 mDirColor[3]; // 0x470
    // Directional output alpha (0x47C, defaults to 1).
    f32 mDirColorA; // 0x47C
    // Number of active lights.
    s32 mActiveLightCount; // 0x480
    // Bitfield of control flags (bit 0x80 = ?, bit 0x100 = ?).
    u32 mFlags; // 0x484
};

// CScnEnvLgtData constructor.
//
// Constructs 8 CLight and 8 nw4r::g3d::LightObj sub-objects in place,
// initializes all float fields to 0/1 defaults, clears FLAG_ENABLE_LIGHT
// on every LightObj, and wires each CLight.mpLightObj to its paired
// LightObj. Resets mActiveLightCount to 0.
//
// Marked extern "C" so the compiler emits the address-anchored
// __ct__804BF5A0 symbol (the class has no vtable/RTTI to anchor a
// normal C++ mangled name).
extern "C" void __ct__804BF5A0(CScnEnvLgtData* self);

// ---------------------------------------------------------------------------
// No-op constructor for trivial sub-object initialization via __construct_array.
// Used by sinit_804BF540 to construct an array of 32 objects (0x24 bytes each)
// at lbl_eu_8065F428. Since BSS zero-init suffices, the constructor body is empty.
// ---------------------------------------------------------------------------
extern "C" void func_804BF59C(void);

// Copy-assignment-like operation: disables all FLAG_ENABLE_LIGHT bits on
// 'this', then compacts enabled CLight entries from 'rhs' into 'this' (up
// to mActiveLightCount). Copies ambient color/mul, lerp speed, and flags
// from rhs, then calls the post-update hook.
extern "C" void func_804BF774(CScnEnvLgtData* self, const CScnEnvLgtData* rhs);

// ---------------------------------------------------------------------------
// Post-light-update hook called after modifying light parameters.
// Takes 'this' pointer in r3. No-op in this base implementation;
// intended to be overridden by derived classes for post-processing.
// Called from CScnEnvLgtCtrl (0x804C573C) and CVirtualLightObj (0x80499C80).
// ---------------------------------------------------------------------------
extern "C" void func_804BF940(void);

// Light-state update routine. Scans mFlags bit 0 to pick a directional
// color (mDirColor[0] vs mDirColor[1]) and the matching curve table index,
// scales mAmbColor by the chosen directional value, then iterates all 8
// CLight slots. For each enabled slot, samples the curve at
// mAmbMul[i] * 2 and multiplies it by the per-slot unk10 color scaled by
// mDirColorA. Clamps the result components to >= 0 and forwards the
// vec3 to func_804C07F0 for the per-slot light update.
extern "C" void func_804BFFB8(CScnEnvLgtData* self);

// Top-level state update: selects between full reset, soft reset, or
// interpolation paths based on mFlags bits 3..6 plus the supplied
// 'reset' flag and 'mode' argument. The reset path (r6 == 0 or bit 4
// of mFlags clear) zeros mAmbMul/mDirColorMul, mLerpSpeed, and clears
// the relevant control bits. The interpolation path lerps mAmbMul
// toward mDirColorMul at rate mLerpSpeed, updating mLerpT. 'vec' is a
// directional color sample consumed when bit 6 of mFlags is set.
extern "C" void func_804BFA70(CScnEnvLgtData* self, const ml::CVec3* vec, int mode, int reset);


// Sets bit 0x80 of mFlags when 'enable' is non-zero, and clears bit 0x100
// either way.
extern "C" void func_804C0228(CScnEnvLgtData* self, int enable);

// Sets bit 0x100 of mFlags when 'enable' is non-zero, and clears bit 0x80
// either way.
extern "C" void func_804C0254(CScnEnvLgtData* self, int enable);
