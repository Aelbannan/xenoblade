#pragma once

#include <types.h>
#include <nw4r/lyt.h>

class CBaseCur;

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Pane-visibility helper (retail C-linkage name; same convention as
// CSysWin.hpp / CKizunagram.hpp / CEquipItemBox.hpp).
extern "C" bool isPaneVisible(nw4r::lyt::Pane*);

// Pane-name table (retail .data quad) used by CCur22's pane helpers.
extern const char* lbl_eu_80534828[];
