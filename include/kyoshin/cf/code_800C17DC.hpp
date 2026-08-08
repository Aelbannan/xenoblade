#pragma once

#include <types.h>

// Hard-symbol callback-table record (scaffold_hard_symbols).
struct Unk805739F8 {
    unsigned char pad[0x20];
    unsigned int field20;
    unsigned char field24;
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" Unk805739F8 lbl_eu_805739F8;
