#pragma once

#include <types.h>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (gmScene; gmGameFlags)

class CScn;

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void TexMan_ApplyCamPersp_034C(void* a, void* b, int c);
// VecMath_PushOutCircleXZ: defined in kyoshin/code_800A3B24 (retail symbol is unmangled).
extern "C" void VecMath_PushOutCircleXZ(void* a, void* b, float c, void* d);
// Global CScn instance (defined CScn* in kyoshin/cf/CfGameManager.cpp)
