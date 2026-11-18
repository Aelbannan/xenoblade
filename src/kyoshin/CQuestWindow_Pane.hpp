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

// Pane visibility: nw4r::lyt::Pane::SetVisible(bool). Callers pass genuine
// bool values (bool locals/params, comparisons, bool-returning helpers).
void setPaneVisible(nw4r::lyt::Pane* pane, bool visible);

} // extern "C"
