#pragma once

#include <types.h>
#include "monolib/vm/yvm2.h"

void* getFP(const char* pName);

#pragma pack(push, 1)

// Fixed header at the start of every bdat table.
struct BdatHeader {
    s32 count;        // +0x00
    u32 _pad04;       // +0x04
    u16 stride;       // +0x08: row stride in bytes
    u16 hashBaseOff;  // +0x0A: offset from table start to hash bucket table
    u16 bucketCount;  // +0x0C: number of hash buckets
    u16 dataOff;      // +0x0E: offset from table start to row data
    u16 maxRow;       // +0x10: maximum valid row index
    u16 rowBase;      // +0x12: minimum valid row index (rowBase)
};

// Column entry in bdat hash chain.
struct BdatColEntry {
    u16 colHdrRel;    // +0x00: relative offset to column header (from table start)
    u16 nextOff;      // +0x02: next entry offset (0 = end of chain)
    char name[1];     // +0x04: null-terminated name (variable length)
};

// Column header for type 1 (value).
struct BdatColHdrValue {
    u8 type;          // +0x00: 1
    u8 elemType;      // +0x01: element type enum
    u16 dataOff;      // +0x02: column data offset within row
};

// Column header for type 2 (array).
struct BdatColHdrArray {
    u8 type;          // +0x00: 2
    u8 elemType;      // +0x01: element type
    u16 dataOff;      // +0x02: column data offset within row
    u16 count;        // +0x04: array element count
};

// Column header for type 3 (flag).
struct BdatColHdrFlag {
    u8 type;          // +0x00: 3
    u8 shift;         // +0x01: right-shift amount
    u32 mask;         // +0x02: bitmask
    u16 colEntryRel;  // +0x06: relative offset to the value column entry
};

// Name-index table: s32 count at +0x00, then u32 at +0x04, then
// u32 entry offsets at +0x08 (retail loads each with lwz; the binary
// search indexes by `mid`). Entry offsets are u32 values from table start.
struct BdatNameIndexHdr {
    s32 count;       // +0x00
    u32 _pad;        // +0x04
    u32 offsets[];   // +0x08 (flexible array of u32 entry offsets)
};

#pragma pack(pop)

// Utility class for handling bdat files.
class CBdat {
public:
    static void* func_8003AA34();
    static void* clearEntries();
    static void* func_8003AA78(u32, void*);
    static void getEntry(u32 val);
    static void* getFP(const char* pName) { return ::getFP(pName); }
    static u32 func_8003B1EC(void* pData);
    static u32 func_8003B41C(void* pData);
};

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void* func_8003B4B0(void* bdat, const char* col);
// Canonical declaration of the shared bdat column reader (defining TU:
// ocBdat.cpp; retail signature `u32 (void*, const char*, s32)`). The third
// parameter MUST be spelled s32 (= signed long under MWCC), never int - the
// two are distinct types and extern "C" names cannot be overloaded.
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index);
extern "C" u32 func_8003AD98(void* bdat, const char* col, s32 row, s32 index);
extern "C" u32 func_eu_8003B488(void* bdat, const char* col1, s32 row, const char* col2);
extern "C" u32 func_8003B748(void* table, void* col, s32 row, s32 index);
extern "C" void func_8003B800(VMArg* out, void* data, u32 type);
extern "C" int getVal(VMThread* t, void* bdat);
extern "C" int getArrayVal(VMThread* t, void* bdat);
extern "C" {
extern s8 lbl_eu_80663D10;
extern u32 lbl_eu_80663D14;
extern void* lbl_eu_805705D0[8];

BOOL vmOCRegist(OCData* pOC);
extern OCData lbl_eu_80524E40;

u32 func_8003B6A0(void* base, void* data, u32 type);

u32 func_8003AFC0(void* bdat, const char* col);
u32 func_8003B204(void* bdat, const char* col);
}

#ifdef __cplusplus
extern "C" {
#endif

void ocBdatRegist();

#ifdef __cplusplus
}
#endif
