#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)

// Retail names the pooled "bar_00" pane string lbl_eu_80549D1C (symbols.txt
// .data:0x80549D1C); update_controller's FindPaneByName("bar_00") emits a
// reloc against it. Pin the name via an undefined extern declaration (resolved
// at link time from symbols.txt) so the reloc matches without adding a .data
// object that would shift the TU string pool.
extern "C" char lbl_eu_80549D1C[16];
