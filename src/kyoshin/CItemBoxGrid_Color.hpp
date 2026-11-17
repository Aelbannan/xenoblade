#pragma once

#include <types.h>
#include <revolution/gx/GXTypes.h>

// Lean public header for the colour helper defined in kyoshin/CItemBoxGrid.cpp.
// Single canonical declaration -- do NOT re-declare it anywhere else (C
// linkage: a second declaration with different parameter types is a hard
// "illegal function overloading" error). Retail name is unmangled.
extern "C" {

// Copies the four s16 channels of a TEV colour (halfword by halfword).
void CopyVec4s(GXColorS10* dst, const GXColorS10* src);

} // extern "C"
