#pragma once

#include <types.h>

// Lean public header for kyoshin/code_80135FDC (kept separate from the fat code_80135FDC.hpp):
// the C-linkage BDAT direct cell accessors. Retail names are unmangled.
// bdat = loaded BDAT file, column = column name, row = row id.
extern "C" {
u16 BdatGetU16Direct(const void* bdat, const void* column, int row);
}
