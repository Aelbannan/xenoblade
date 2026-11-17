#pragma once

#include <types.h>

namespace nw4r {
namespace lyt {
class ArcResourceAccessor;
} // namespace lyt
namespace ut {
class PackedFont;
} // namespace ut
} // namespace nw4r

// Lean public header for the shared UI resources owned by kyoshin/CUICfManager
// (the singleton's arc accessor and its three packed fonts at +0x60/+0x9C/
// +0xD8). Single canonical declaration of each accessor -- do NOT re-declare
// these anywhere else. All return 0 while the manager does not exist.

// Retail symbol getPackedFont__Fv (C++ linkage). Callers that hand the font
// to setLayoutTextBoxFont pass it as a word.
nw4r::ut::PackedFont* getPackedFont();

extern "C" {
nw4r::ut::PackedFont* CUICfManager_getPackedFont9C();
nw4r::ut::PackedFont* CUICfManager_getPackedFontD8();
nw4r::lyt::ArcResourceAccessor* CUICfManager_getArcResourceAccessor();
} // extern "C"
