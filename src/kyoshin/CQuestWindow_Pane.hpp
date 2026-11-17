#pragma once

#include <types.h>

namespace nw4r {
namespace lyt {
class Pane;
} // namespace lyt
} // namespace nw4r

// Lean public header for the pane helper defined in kyoshin/CQuestWindow.cpp.
// Single canonical declaration -- do NOT re-declare it anywhere else (C
// linkage: a second declaration with different parameter types is a hard
// "illegal function overloading" error). Retail name is unmangled.
extern "C" {

// Pane visibility: clears bit 0 of the pane flag byte (+0xBB) and ORs in
// (u8)visible. Callers pass 0/1. `visible` is u32, not bool: retail callers
// pass raw words (u8 params, `a < b` as (a-b)>>31, int getters) with no bool
// normalisation, and the callee masks with clrlwi itself.
void setPaneVisible(nw4r::lyt::Pane* pane, u32 visible);

} // extern "C"
