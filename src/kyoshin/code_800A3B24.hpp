#pragma once

#include "monolib/math/CVec3.hpp"

// Canonical C++ declarations for the math helpers defined in code_800A3B24.cpp.
void VecMath_RandomAngleOffset(ml::CVec3* out, int seed);
ml::CVec3* VecMath_NormalizeInPlace(ml::CVec3* v);
float VecMath_SafeSqrtF(float x);
void VecMath_WrapAnglesPi(ml::CVec3* v);
bool VecMath_IsTurnLeftXZ(const ml::CVec3& a, const ml::CVec3& b, const ml::CVec3& c);
