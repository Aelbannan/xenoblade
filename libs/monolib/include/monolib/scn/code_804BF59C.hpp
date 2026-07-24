#pragma once

// Light environment data container class.
//
// This class manages the lighting state for the scene environment, containing:
//   - 8 CLight objects at offset 0x000 (0x40 bytes each = 0x200 total)
//   - 8 nw4r::g3d::LightObj objects at offset 0x200 (0x44 bytes each = 0x220 total)
//   - Float parameters for ambient/directional color and intensity at 0x420+
//   - Active-light counter at 0x480 (int)
//   - Control flags at 0x484 (u32 bitfield)
//
// Total class size: >= 0x488 bytes.
//
// Used as a base or member by CScnEnvLgtCtrl and CVirtualLightObj.
// FULL_MATCH functions in this TU:
//   - func_804BF59C: No-op constructor for trivial sub-object array init
//   - func_804BF940: No-op post-light-update hook (virtual stub)

// ---------------------------------------------------------------------------
// No-op constructor for trivial sub-object initialization via __construct_array.
// Used by sinit_804BF540 to construct an array of 32 objects (0x24 bytes each)
// at lbl_eu_8065F428. Since BSS zero-init suffices, the constructor body is empty.
// ---------------------------------------------------------------------------
extern "C" void func_804BF59C(void);

// ---------------------------------------------------------------------------
// Post-light-update hook called after modifying light parameters.
// Takes 'this' pointer in r3. No-op in this base implementation;
// intended to be overridden by derived classes for post-processing.
// Called from CScnEnvLgtCtrl (0x804C573C) and CVirtualLightObj (0x80499C80).
// ---------------------------------------------------------------------------
extern "C" void func_804BF940(void);
