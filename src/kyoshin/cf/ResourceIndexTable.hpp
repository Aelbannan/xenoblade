#pragma once

#include <types.h>

// Resource-gating table: 8x8 s8 row/col lookup used by
// func_80082418__Q22cf13CfGameManagerFv (see CfGameManagerUnityHelpers.hpp).
// Extracted from the CfGameManager unity fragment so the class-typed label
// lbl_eu_804FB7B0 has a self-contained type home.
struct ResourceIndexTable {
    s8 values[8][8];
};
