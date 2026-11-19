#pragma once

#include <types.h>

namespace nw4r {
namespace lyt {
class Pane;
} // namespace lyt
} // namespace nw4r

// Lean public header for the pane helpers. Single canonical declarations --
// do NOT re-declare them anywhere else (a second declaration with different
// parameter types or linkage is a hard "illegal function overloading" error).
extern "C" {

// Pane visibility: nw4r::lyt::Pane::SetVisible(bool). Callers pass genuine
// bool values (bool locals/params, comparisons, bool-returning helpers).
void setPaneVisible(nw4r::lyt::Pane* pane, bool visible);

} // extern "C"

// Defined in kyoshin/CTitleAHelp.cpp; retail emits the C++-mangled name.
bool isPaneVisible(nw4r::lyt::Pane* pane);
