#pragma once

#include <types.h>

// Lean public header for kyoshin/code_80135FDC: the BDAT cell readers and the
// string helpers of that unit. Single canonical declaration of each helper --
// do NOT re-declare these anywhere else (C linkage: a second declaration with
// different parameter types is a hard "illegal function overloading" error).
// Retail names are unmangled (C linkage).
//
//   table  = BDAT table name (resolved through getFP)
//   bdat   = already-loaded BDAT file pointer
//   column = column name
//   row    = row id (1-based)
//
// Return widths follow the cell type (u8/u16/s8/s16). Retail callers that
// keep r3 unextended and mask only at the use site store the result in a
// local of the same narrow width (getEIBProgress, CArtsInfo_CalcDmg52/53,
// QstLogInfo_UpdateClearReasonPane); a conversion to a wider local or a cast
// at the call extends at receipt instead.
extern "C" {

// --- cells read through a table name ---
u8 BdatGetU8ByTableKey(const char* table, const char* column, u32 row);
u16 BdatGetU16ByTableKey(const char* table, const char* column, u32 row);
s8 BdatGetS8ByTableKey(const char* table, const char* column, u32 row);
s16 BdatGetS16ByTableKey(const char* table, const char* column, u32 row);
char* BdatTouchStringCell(const char* table, const char* column, int row);

// --- cells read through a loaded BDAT file ---
u8 BdatGetU8Direct(const void* bdat, const char* column, int row);
u16 BdatGetU16Direct(const void* bdat, const char* column, int row);
int BdatGetS8Direct(const void* bdat, const char* column, int row);
s16 BdatGetS16Direct(const void* bdat, const char* column, int row);
char* BdatGetPtrDirect(const void* bdat, const char* column, int row);

// --- string helpers ---
u32 Utf8ToWcsCopyFix(const char* src, u16* dst, u32 destLen);
void SjisDotToCommaEU(char* str);
int SplitStringTrim(char* src, char delim, char** outTokens);
int SplitWStringTrim(u16* src, u16 delim, u16** outTokens);
void WcsToUpperInPlace(u16* str);

} // extern "C"
