// Auto-scaffolded catalog TU for kyoshin/plugin/ocBdat
// Mangled extern stubs for llm-harness / coop selection.
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "monolib/vm/yvm2.h"
#include <string.h>

extern "C" {
extern s8 lbl_eu_80663D10;
extern u32 lbl_eu_80663D14;
extern void* lbl_eu_805705D0[8];

BOOL vmOCRegist(OCData* pOC);
extern OCData lbl_eu_80524E40;

void* func_8003B4B0(void* bdat, const char* col);
u32 func_8003B6A0(void* base, void* data, u32 type);
u32 func_8003B748(void* table, void* col, s32 row, s32 index);
u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index);
u32 func_8003AD98(void* bdat, const char* col, s32 row, s32 index);
u32 func_8003AFC0(void* bdat, const char* col);
u32 func_8003B204(void* bdat, const char* col);
u32 func_eu_8003B488(void* bdat, const char* col1, s32 row, const char* col2);
void func_8003B800(VMArg* out, void* data, u32 type);
}

extern "C" void* func_eu_8003B720(void* p);

// LLM-HARNESS-BEGIN: us-8003ae14
extern "C" void* func_8003AA34() {
    if (!lbl_eu_80663D10) {
        lbl_eu_80663D10 = 1;
    }
    return &lbl_eu_80663D14;
}
// LLM-HARNESS-END: us-8003ae14

// LLM-HARNESS-BEGIN: us-8003ae30
void* CBdat::func_8003AA50() {
    void** table = lbl_eu_805705D0;
    table[0] = 0;
    table[1] = 0;
    table[2] = 0;
    table[3] = 0;
    table[4] = 0;
    table[5] = 0;
    table[6] = 0;
    return table;
}
// LLM-HARNESS-END: us-8003ae30

// LLM-HARNESS-BEGIN: us-8003ae5c
void* CBdat::func_8003AA78(u32 idx, void* p) {
    lbl_eu_805705D0[idx] = p;
    return func_eu_8003B720(p);
}
// LLM-HARNESS-END: us-8003ae5c

// LLM-HARNESS-BEGIN: us-8003ae74
void CBdat::func_8003AA8C(u32 idx) {
    lbl_eu_805705D0[idx] = 0;
}
// LLM-HARNESS-END: us-8003ae74

// LLM-HARNESS-BEGIN: us-8003ae8c
#pragma dont_inline on
void* getFP(const char* pName) {
    void** tableSlot;
    const char* name;
    s32 tableIdx;
    void* table;
    s32 hi;
    s32 lo;
    u32 sum;
    u32 adj;
    s32 mid;
    u32 entryOff;
    char* entry;
    u16 nameOff;
    const char* entryName;
    s32 cmp;

    tableSlot = lbl_eu_805705D0;
    name = pName;
    tableIdx = 0;
    while (tableIdx < 7) {
        table = *tableSlot;
        if (table != 0) {
            lo = 0;
            hi = *reinterpret_cast<s32*>(table);
            while (lo < hi) {
                sum = static_cast<u32>(lo) + static_cast<u32>(hi);
                adj = sum + (sum >> 31);
                mid = adj >> 1;
                entryOff = *reinterpret_cast<u32*>(reinterpret_cast<char*>(table) + (adj & ~1u) + 8u);
                entry = reinterpret_cast<char*>(table) + entryOff;
                nameOff = *reinterpret_cast<u16*>(entry + 6);
                entryName = entry + nameOff;
                cmp = strcmp(entryName, name);
                if (cmp < 0) {
                    lo = static_cast<s32>(mid + 1);
                } else if (cmp > 0) {
                    hi = static_cast<s32>(mid);
                } else {
                    return entry;
                }
            }
        }
        tableIdx++;
        tableSlot++;
    }
    return 0;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003ae8c

// LLM-HARNESS-BEGIN: us-8003af48
#pragma dont_inline on
extern "C" void* func_8003B4B0(void* bdat, const char* col) {
    void* bdatArg;
    const char* colArg;
    u16 bucketCount;
    char* hashBase;
    s32 hashLen;
    s32 pos;
    s32 hash;
    u32 rem;
    u16 bucketOff;
    char* entry;
    u16 nextOff;

    bdatArg = bdat;
    colArg = col;
    if (bdatArg == 0) {
        return 0;
    }
    bucketCount = *reinterpret_cast<u16*>(reinterpret_cast<char*>(bdatArg) + 0xC);
    hashBase = reinterpret_cast<char*>(bdatArg) + *reinterpret_cast<u16*>(reinterpret_cast<char*>(bdatArg) + 0xA);
    hashLen = static_cast<s32>(strlen(colArg));
    if (hashLen > 8) {
        hashLen = 8;
    }
    hash = 0;
    pos = 0;
    while (pos < hashLen) {
        hash = hash * 7 + static_cast<s8>(colArg[pos]);
        pos++;
    }
    rem = static_cast<u32>(hash);
    rem -= (rem / bucketCount) * bucketCount;
    bucketOff = *reinterpret_cast<u16*>(hashBase + rem * 2);
    if (bucketOff == 0) {
        return 0;
    }
    entry = reinterpret_cast<char*>(bdatArg) + bucketOff;
    while (1) {
        if (strcmp(colArg, entry + 4) == 0) {
            break;
        }
        nextOff = *reinterpret_cast<u16*>(entry + 2);
        if (nextOff == 0) {
            entry = 0;
            break;
        }
        entry = reinterpret_cast<char*>(bdatArg) + nextOff;
    }
    return entry;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003af48

// LLM-HARNESS-BEGIN: us-8003b148
#pragma dont_inline on
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index) {
    void* bdatArg;
    const char* colArg;
    s32 indexArg;
    void* colEntry;
    char* tbl;
    s32 rowBase;
    s32 maxRow;
    s32 rowIdx;
    u16 colRel;
    char* colHdr;
    u16 stride;
    u16 dataOff;
    s32 rowBytes;
    u16 colDataOff;
    u32 elemType;
    char* dataPtr;

    bdatArg = bdat;
    colArg = col;
    indexArg = index;
    colEntry = func_8003B4B0(bdatArg, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdatArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(bdatArg);
    rowBase = *reinterpret_cast<u16*>(tbl + 0x12);
    maxRow = *reinterpret_cast<u16*>(tbl + 0x10);
    rowIdx = indexArg - rowBase;
    if (rowIdx < 0 || maxRow < rowIdx) {
        return 0;
    }
    indexArg = rowIdx;
    colRel = *reinterpret_cast<u16*>(colEntry);
    colHdr = tbl + colRel;
    if (static_cast<u8>(colHdr[0]) != 1) {
        return 0;
    }
    stride = *reinterpret_cast<u16*>(tbl + 0x8);
    dataOff = *reinterpret_cast<u16*>(tbl + 0xE);
    rowBytes = indexArg * stride;
    colDataOff = *reinterpret_cast<u16*>(colHdr + 0x2);
    elemType = static_cast<u8>(colHdr[1]);
    dataPtr = tbl + dataOff + rowBytes + colDataOff;
    return func_8003B6A0(bdatArg, dataPtr, elemType);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b148

// LLM-HARNESS-BEGIN: us-8003b220
#pragma dont_inline on
extern "C" u32 func_8003AD98(void* bdat, const char* col, s32 row, s32 index) {
    void* bdatArg;
    const char* colArg;
    s32 rowArg;
    s32 indexArg;
    void* colEntry;
    char* tbl;
    s32 rowBase;
    s32 maxRow;
    s32 rowIdx;
    s32 ok;

    bdatArg = bdat;
    colArg = col;
    rowArg = row;
    indexArg = index;
    if (bdatArg == 0) {
        return 0;
    }
    colEntry = func_8003B4B0(bdatArg, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdatArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(bdatArg);
    rowBase = *reinterpret_cast<u16*>(tbl + 0x12);
    maxRow = *reinterpret_cast<u16*>(tbl + 0x10);
    rowIdx = rowArg - rowBase;
    if (maxRow < rowIdx) {
        ok = 0;
    } else if (rowIdx < 0) {
        ok = 0;
    } else {
        rowArg = rowIdx;
        ok = 1;
    }
    if (ok == 0) {
        return 0;
    }
    return func_8003B748(bdatArg, colEntry, rowArg, indexArg);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b220

// LLM-HARNESS-BEGIN: us-8003b2e4
#pragma dont_inline on
extern "C" u32 func_8003AFC0(void* bdat, const char* col) {
    void* bdatArg;
    const char* colArg;
    void* colEntry;
    char* tbl;
    char* colHdr;

    bdatArg = bdat;
    colArg = col;
    if (bdatArg == 0) {
        return 0;
    }
    colEntry = func_8003B4B0(bdatArg, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdatArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(bdatArg);
    colHdr = tbl + *reinterpret_cast<u16*>(colEntry);
    if (static_cast<u8>(colHdr[0]) != 2) {
        return 0;
    }
    return *reinterpret_cast<u16*>(colHdr + 0x4);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b2e4

// LLM-HARNESS-BEGIN: us-8003b360
extern "C" u32 func_8003B1EC(void* p) {
    if (p == 0) {
        return 0;
    }
    return *reinterpret_cast<u16*>(reinterpret_cast<char*>(p) + 0x10);
}
// LLM-HARNESS-END: us-8003b360

// LLM-HARNESS-BEGIN: us-8003b378
extern "C" u32 func_8003B6A0(void* base, void* data, u32 type);

#pragma dont_inline on
extern "C" u32 func_8003B434(void* table, void*, void* col, s32 row) {
    void* tableArg;
    void* colArg;
    s32 rowArg;
    char* tbl;
    s32 rowBase;
    s32 maxRow;
    s32 rowIdx;
    s32 ok;
    u16 colRel;
    char* colHdr;
    u16 stride;
    u16 dataOff;
    s32 rowBytes;
    u16 colDataOff;
    char* dataBase;
    u8 elemType;
    char* dataPtr;

    tableArg = table;
    colArg = col;
    rowArg = row;
    if (tableArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(tableArg);
    rowBase = *reinterpret_cast<u16*>(tbl + 0x12);
    maxRow = *reinterpret_cast<u16*>(tbl + 0x10);
    rowIdx = rowArg - rowBase;
    if (maxRow < rowIdx) {
        goto bounds_fail;
    }
    if (rowIdx >= 0) {
        goto bounds_success;
    }
bounds_fail:
    ok = 0;
    goto bounds_check;
bounds_success:
    rowArg = rowIdx;
    ok = 1;
bounds_check:
    if (ok == 0) {
        return 0;
    }
    colRel = *reinterpret_cast<u16*>(colArg);
    colHdr = tbl + colRel;
    if (static_cast<u8>(colHdr[0]) != 1) {
        return 0;
    }
    stride = *reinterpret_cast<u16*>(tbl + 0x8);
    dataOff = *reinterpret_cast<u16*>(tbl + 0xE);
    rowBytes = rowArg * stride;
    colDataOff = *reinterpret_cast<u16*>(colHdr + 0x2);
    dataBase = tbl + dataOff;
    elemType = static_cast<u8>(colHdr[1]);
    dataBase += rowBytes;
    dataPtr = dataBase + colDataOff;
    return func_8003B6A0(tableArg, dataPtr, elemType);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b378

// LLM-HARNESS-BEGIN: us-8003b408
#pragma dont_inline on
extern "C" u32 func_8003B204(void* bdat, const char* col) {
    void* bdatArg;
    const char* colArg;
    void* colEntry;
    char* tbl;
    char* colHdr;

    bdatArg = bdat;
    colArg = col;
    if (bdatArg == 0) {
        return 0;
    }
    colEntry = func_8003B4B0(bdatArg, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdatArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(bdatArg);
    colHdr = tbl + *reinterpret_cast<u16*>(colEntry);
    return static_cast<u8>(colHdr[1]);
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b408

// LLM-HARNESS-BEGIN: us-8003b470
extern "C" u32 func_8003B41C(void* p) {
    if (p == 0) {
        return 0;
    }
    return *reinterpret_cast<u16*>(reinterpret_cast<char*>(p) + 0x12);
}
// LLM-HARNESS-END: us-8003b470

// LLM-HARNESS-BEGIN: us-8003b488
#pragma dont_inline on
extern "C" u32 func_eu_8003B488(void* bdat, const char* col1, s32 row, const char* col2) {
    void* bdatArg;
    const char* col1Arg;
    s32 rowArg;
    const char* col2Arg;
    char* tbl;
    s32 rowBase;
    s32 maxRow;
    s32 rowIdx;
    s32 ok;
    void* col1Entry;
    void* col2Entry;
    char* flagHdr;
    char* col1Hdr;
    u32 val;
    u32 mask;
    u8 shift;

    bdatArg = bdat;
    col1Arg = col1;
    rowArg = row;
    col2Arg = col2;
    if (bdatArg == 0) {
        return 0;
    }
    tbl = reinterpret_cast<char*>(bdatArg);
    rowBase = *reinterpret_cast<u16*>(tbl + 0x12);
    maxRow = *reinterpret_cast<u16*>(tbl + 0x10);
    rowIdx = rowArg - rowBase;
    if (maxRow < rowIdx) {
        ok = 0;
    } else if (rowIdx < 0) {
        ok = 0;
    } else {
        rowArg = rowIdx;
        ok = 1;
    }
    if (ok == 0) {
        return 0;
    }
    col1Entry = func_8003B4B0(bdatArg, col1Arg);
    if (col1Entry == 0) {
        return 0;
    }
    col2Entry = func_8003B4B0(bdatArg, col2Arg);
    if (col2Entry == 0) {
        return 0;
    }
    flagHdr = tbl + *reinterpret_cast<u16*>(col2Entry);
    if (static_cast<u8>(flagHdr[0]) != 3) {
        return 0;
    }
    if (static_cast<u16>(reinterpret_cast<char*>(col1Entry) - tbl) != *reinterpret_cast<u16*>(flagHdr + 0x6)) {
        return 0;
    }
    val = 0;
    col1Hdr = tbl + *reinterpret_cast<u16*>(col1Entry);
    if (static_cast<u8>(col1Hdr[0]) == 1) {
        u16 stride = *reinterpret_cast<u16*>(tbl + 0x8);
        u16 dataOff = *reinterpret_cast<u16*>(tbl + 0xE);
        s32 rowBytes = rowArg * stride;
        u16 colDataOff = *reinterpret_cast<u16*>(col1Hdr + 0x2);
        u32 elemType = static_cast<u8>(col1Hdr[1]);
        char* dataPtr = tbl + dataOff + rowBytes + colDataOff;
        val = func_8003B6A0(bdatArg, dataPtr, elemType);
    }
    mask = *reinterpret_cast<u32*>(flagHdr + 0x2);
    shift = static_cast<u8>(flagHdr[1]);
    return (val & mask) >> shift;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b488

// LLM-HARNESS-BEGIN: us-8003b5d8
#pragma dont_inline on
extern "C" u32 func_8003B6A0(void* base, void* data, u32 type) {
    volatile u32 local;
    local = 0;
    switch (type) {
    case 0:
        break;
    case 1:
        *reinterpret_cast<volatile u8*>(&local) = *reinterpret_cast<u8*>(data);
        break;
    case 2:
        *reinterpret_cast<volatile u16*>(&local) = *reinterpret_cast<u16*>(data);
        break;
    case 3:
        local = *reinterpret_cast<u32*>(data);
        break;
    case 4:
        *reinterpret_cast<volatile u8*>(&local) = *reinterpret_cast<u8*>(data);
        break;
    case 5:
        *reinterpret_cast<volatile s16*>(&local) = *reinterpret_cast<s16*>(data);
        break;
    case 6:
        local = *reinterpret_cast<u32*>(data);
        break;
    case 7:
        local = reinterpret_cast<u32>(base) + *reinterpret_cast<u32*>(data);
        break;
    case 8:
        local = *reinterpret_cast<u32*>(data);
        break;
    default:
        break;
    }
    return local;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b5d8

// LLM-HARNESS-BEGIN: us-8003b670
extern "C" u32 func_8003B748(void* table, void* col, s32 row, s32 index) {
    char* tbl = reinterpret_cast<char*>(table);
    u16 colOff = *reinterpret_cast<u16*>(col);
    char* colHdr = tbl + colOff;
    if (static_cast<u8>(colHdr[0]) != 2) {
        return 0;
    }
    s32 count = *reinterpret_cast<u16*>(colHdr + 0x4);
    if (count <= index) {
        return 0;
    }
    u16 stride = *reinterpret_cast<u16*>(tbl + 0x8);
    u16 dataOff = *reinterpret_cast<u16*>(tbl + 0xE);
    s32 rowStride = row * stride;
    u32 elemType = static_cast<u8>(colHdr[1]);
    char* dataBase = tbl + dataOff;
    u16 colDataOff = *reinterpret_cast<u16*>(colHdr + 0x2);
    dataBase += rowStride;
    dataBase += colDataOff;
    // Retail: subi/cmplwi then ble to *4; fall-through is the cmpwi switch.
    if (elemType - 6 > 1) {
        switch (elemType) {
        case 1:
        case 4:
            dataBase += index;
            break;
        case 2:
        case 5:
            dataBase += index * 2;
            break;
        case 3:
            dataBase += index * 4;
            break;
        default:
            break;
        }
    } else {
        dataBase += index * 4;
    }
    return func_8003B6A0(table, dataBase, elemType);
}
// LLM-HARNESS-END: us-8003b670

// LLM-HARNESS-BEGIN: us-8003b720
#pragma dont_inline on
extern "C" void* func_eu_8003B720(void* p) {
    u32 mem1 = 0x80000000u;
    u32* entryList = reinterpret_cast<u32*>(reinterpret_cast<char*>(p) + 8);
    u32 idx = 0;
    // Separate load vs xor-result temps (retail r23 / r31) to improve stmw/regalloc.
    u32 xorTmp;

    // Reload count at loop_check so MWCC cannot steal CTR for the outer walk
    // (inners are addic./bne; retail uses CTR on inners and cmplw on outer).
    goto loop_check;
loop_body:
    {
        u8* ent = reinterpret_cast<u8*>(p) + *entryList;
        if ((ent[4] & 2) != 0) {
            s32 start = *reinterpret_cast<s32*>(ent + 0x18);
            s32 len = *reinterpret_cast<s32*>(ent + 0x1C);
            u32 keyWord = *reinterpret_cast<u16*>(ent + 0x16);
            s32 end = start + len;
            u32 byteHi = static_cast<u8>(keyWord >> 8);
            u32 byteLo = static_cast<u8>(keyWord);
            // nor interleaved with cmpw (cr1) before masking keys.
            u32 norHi = ~byteHi;
            s32 phase1Empty = (start >= end);
            u32 norLo = ~byteLo;
            u32 kHi = static_cast<u8>(norHi);
            u32 kLo = static_cast<u8>(norLo);
            if (phase1Empty)
                goto phase2;

            {
                s32 lim16 = end - 0x10;
                s32 half = (len + 1) / 2;
                if (half > 8) {
                    s32 f24 = 0;
                    s32 f25 = 0;
                    s32 f26 = 0;
                    s32 f27 = 0;
                    s32 f23 = 0;
                    s32 lim = static_cast<s32>(mem1) - 3;
                    if (start > end)
                        goto gate1b;
                    if (end > lim)
                        goto gate1b;
                    f23 = 1;
                gate1b:
                    if (f23 == 0)
                        goto gate1c;
                    lim = static_cast<s32>(mem1) - 3;
                    if (start > lim)
                        goto gate1c;
                    f27 = 1;
                gate1c:
                    if (f27 == 0)
                        goto gate1d;
                    if (static_cast<u32>(start) + 0x80000000u == 0)
                        goto gate1d;
                    f26 = 1;
                gate1d:
                    if (f26 == 0)
                        goto gate1e;
                    {
                        s32 tmp = 1;
                        s32 signEnd = end & static_cast<s32>(0x80000000u);
                        s32 signNegStart = (-start) & static_cast<s32>(0x80000000u);
                        if (signEnd == signNegStart) {
                            if (signEnd != (len & static_cast<s32>(0x80000000u))) {
                                tmp = 0;
                            }
                        }
                        if (tmp == 0)
                            goto gate1e;
                        f25 = 1;
                    }
                gate1e:
                    if (f25 == 0)
                        goto gate1f;
                    {
                        s32 tmp = 1;
                        if ((len & static_cast<s32>(0x80000000u)) == 0) {
                            len += 1;
                            if ((len & static_cast<s32>(0x80000000u)) != 0) {
                                tmp = 0;
                            }
                        }
                        if (tmp == 0)
                            goto gate1f;
                        f24 = 1;
                    }
                gate1f:
                    if (f24 == 0)
                        goto residual1;
                    {
                        // Peak size 0x540: do-while(--n) → addic./bne (retail mtctr soft-cap).
                        u32 n = static_cast<u32>(lim16 + 0xF - start) >> 4;
                        if (start < lim16) {
                            do {
                                xorTmp = ent[start];
                                u8* cur = ent + start;
                                ent[start] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                start += 16;
                                xorTmp = cur[1];
                                cur[1] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[2];
                                cur[2] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[3];
                                cur[3] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[4];
                                cur[4] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[5];
                                cur[5] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[6];
                                cur[6] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[7];
                                cur[7] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[8];
                                cur[8] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[9];
                                cur[9] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[10];
                                cur[10] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[11];
                                cur[11] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[12];
                                cur[12] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[13];
                                cur[13] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[14];
                                cur[14] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[15];
                                cur[15] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                            } while (--n != 0);
                        }
                    }
                }
            residual1:
                {
                    u32 n = static_cast<u32>(end + 1 - start) >> 1;
                    if (start < end) {
                        do {
                            xorTmp = ent[start];
                            u8* cur = ent + start;
                            ent[start] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                            start += 2;
                            xorTmp = cur[1];
                            cur[1] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                        } while (--n != 0);
                    }
                }
            }

        phase2:
            kHi = static_cast<u8>(norHi);
            start = *reinterpret_cast<u16*>(ent + 6);
            end = *reinterpret_cast<u16*>(ent + 0xA);
            kLo = static_cast<u8>(norLo);
            if (start < end) {
                s32 span = end - start;
                s32 lim16 = end - 0x10;
                s32 span1 = span + 1;
                s32 half = span1 / 2;
                if (half > 8) {
                    s32 f28 = 0;
                    s32 f31 = 0;
                    s32 f26 = 0;
                    s32 f25 = 0;
                    s32 f24 = 0;
                    s32 lim = static_cast<s32>(mem1) - 3;
                    if (start > end)
                        goto gate2b;
                    if (end > lim)
                        goto gate2b;
                    f24 = 1;
                gate2b:
                    if (f24 == 0)
                        goto gate2c;
                    lim = static_cast<s32>(mem1) - 3;
                    if (start > lim)
                        goto gate2c;
                    f25 = 1;
                gate2c:
                    if (f25 == 0)
                        goto gate2d;
                    if (static_cast<u32>(start) + 0x80000000u == 0)
                        goto gate2d;
                    f26 = 1;
                gate2d:
                    if (f26 == 0)
                        goto gate2e;
                    {
                        s32 tmp = 1;
                        s32 signEnd = end & static_cast<s32>(0x80000000u);
                        s32 signNegStart = (-start) & static_cast<s32>(0x80000000u);
                        if (signEnd == signNegStart) {
                            if (signEnd != (span & static_cast<s32>(0x80000000u))) {
                                tmp = 0;
                            }
                        }
                        if (tmp == 0)
                            goto gate2e;
                        f31 = 1;
                    }
                gate2e:
                    if (f31 == 0)
                        goto gate2f;
                    {
                        // Retail: test span sign, then span1 sign (no mutate).
                        s32 tmp = 1;
                        if ((span & static_cast<s32>(0x80000000u)) == 0) {
                            if ((span1 & static_cast<s32>(0x80000000u)) != 0) {
                                tmp = 0;
                            }
                        }
                        if (tmp == 0)
                            goto gate2f;
                        f28 = 1;
                    }
                gate2f:
                    if (f28 == 0)
                        goto residual2;
                    {
                        u32 n = static_cast<u32>(lim16 + 0xF - start) >> 4;
                        if (start < lim16) {
                            do {
                                xorTmp = ent[start];
                                u8* cur = ent + start;
                                ent[start] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                start += 16;
                                xorTmp = cur[1];
                                cur[1] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[2];
                                cur[2] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[3];
                                cur[3] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[4];
                                cur[4] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[5];
                                cur[5] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[6];
                                cur[6] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[7];
                                cur[7] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[8];
                                cur[8] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[9];
                                cur[9] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[10];
                                cur[10] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[11];
                                cur[11] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[12];
                                cur[12] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[13];
                                cur[13] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                                xorTmp = cur[14];
                                cur[14] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                                xorTmp = cur[15];
                                cur[15] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                            } while (--n != 0);
                        }
                    }
                }
            residual2:
                {
                    u32 n = static_cast<u32>(end + 1 - start) >> 1;
                    if (start < end) {
                        do {
                            xorTmp = ent[start];
                            u8* cur = ent + start;
                            ent[start] = static_cast<u8>(xorTmp ^ kHi);
                                kHi += xorTmp;
                            start += 2;
                            xorTmp = cur[1];
                            cur[1] = static_cast<u8>(xorTmp ^ kLo);
                                kLo += xorTmp;
                        } while (--n != 0);
                    }
                }
            }

            ent[4] = ent[4] & 0xFD;
        }
        entryList += 1;
        idx += 1;
    }
loop_check:
    if (idx < *reinterpret_cast<u32*>(p)) {
        goto loop_body;
    }
    return p;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003b720

// LLM-HARNESS-BEGIN: us-8003bc60
#pragma dont_inline on
extern "C" void func_8003B800(VMArg* out, void* data, u32 type) {
    VMArg* outArg;
    void* dataArg;
    u32 typeArg;
    u32 local;

    outArg = out;
    dataArg = data;
    typeArg = type;
    local = 0;
    switch (typeArg) {
    case 0:
        break;
    case 1:
        local = *reinterpret_cast<u8*>(dataArg);
        break;
    case 2:
        local = *reinterpret_cast<u16*>(dataArg);
        break;
    case 3:
        local = *reinterpret_cast<u32*>(dataArg);
        break;
    case 4:
        local = static_cast<u32>(static_cast<s32>(static_cast<s8>(*reinterpret_cast<u8*>(dataArg))));
        break;
    case 5:
        local = static_cast<u32>(static_cast<s32>(*reinterpret_cast<s16*>(dataArg)));
        break;
    case 6:
        local = *reinterpret_cast<u32*>(dataArg);
        break;
    case 7:
        outArg->type = VM_TYPE_STRING;
        outArg->unk2 = static_cast<u16>(strlen(*reinterpret_cast<const char* const*>(dataArg)));
        outArg->value.pointerVal = *reinterpret_cast<void* const*>(dataArg);
        break;
    case 8:
        outArg->type = VM_TYPE_FIXED;
        outArg->value.uintVal = *reinterpret_cast<u32*>(dataArg);
        break;
    default:
        break;
    }
    if (typeArg != 7 && typeArg != 8) {
        outArg->type = VM_TYPE_INT;
        outArg->value.uintVal = local;
    }
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003bc60

// LLM-HARNESS-BEGIN: us-8003bd4c
#pragma dont_inline on
extern "C" int bdat(VMThread* t, void* /*unused*/, u16 unk) {
    VMThread* thread;
    u16 unkArg;
    const char* name;
    VMArg result;

    thread = t;
    unkArg = unk;
    name = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    result.type = VM_TYPE_OC;
    result.unk2 = unkArg;
    result.value.pointerVal = getFP(name);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003bd4c

// LLM-HARNESS-BEGIN: us-8003bdb8
#pragma dont_inline on
extern "C" int getVal(VMThread* t, void* bdat) {
    VMThread* thread;
    void* bdatTbl;
    const char* col;
    s32 idx;
    u32 dataSlot;
    u32 type;
    VMArg result;

    thread = t;
    bdatTbl = bdat;
    col = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    idx = vmArgIntGet(3, vmArgPtrGet(thread, 2));
    dataSlot = getBdatStringColumnValue(bdatTbl, col, idx);
    type = func_8003B204(bdatTbl, col);
    func_8003B800(&result, &dataSlot, type);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003bdb8

// LLM-HARNESS-BEGIN: us-8003be70
#pragma dont_inline on
extern "C" int getArrayVal(VMThread* t, void* bdat) {
    VMThread* thread;
    void* bdatTbl;
    const char* col;
    s32 row;
    s32 index;
    u32 dataSlot;
    u32 type;
    VMArg result;

    thread = t;
    bdatTbl = bdat;
    col = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    row = vmArgIntGet(3, vmArgPtrGet(thread, 2));
    index = vmArgIntGet(4, vmArgPtrGet(thread, 3));
    dataSlot = func_8003AD98(bdatTbl, col, row, index);
    type = func_8003B204(bdatTbl, col);
    func_8003B800(&result, &dataSlot, type);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003be70

// LLM-HARNESS-BEGIN: us-8003bf48
#pragma dont_inline on
extern "C" int getArrayCount(VMThread* t, void* bdat) {
    VMThread* thread;
    void* bdatTbl;
    const char* col;
    VMArg result;

    thread = t;
    bdatTbl = bdat;
    col = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    result.type = VM_TYPE_INT;
    result.value.uintVal = func_8003AFC0(bdatTbl, col);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003bf48

// LLM-HARNESS-BEGIN: us-8003bfb8
#pragma dont_inline on
extern "C" int getVarType(VMThread* t, void* bdat) {
    VMThread* thread;
    void* bdatTbl;
    const char* col;
    VMArg result;

    thread = t;
    bdatTbl = bdat;
    col = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    result.type = VM_TYPE_INT;
    result.value.uintVal = func_8003B204(bdatTbl, col);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003bfb8

// LLM-HARNESS-BEGIN: us-8003c028
#pragma dont_inline on
extern "C" int getIdCount(VMThread* t, void* bdat) {
    VMArg result;

    result.type = VM_TYPE_INT;
    result.value.uintVal = func_8003B1EC(bdat);
    vmRetValSet(t, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003c028

// LLM-HARNESS-BEGIN: us-8003c074
#pragma dont_inline on
extern "C" int getIdTop(VMThread* t, void* bdat) {
    VMArg result;

    result.type = VM_TYPE_INT;
    result.value.uintVal = func_8003B41C(bdat);
    vmRetValSet(t, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003c074

// LLM-HARNESS-BEGIN: us-8003c0c0
#pragma dont_inline on
extern "C" int getFlagVal(VMThread* t, void* bdat) {
    VMThread* thread;
    void* bdatTbl;
    const char* col1;
    s32 row;
    const char* col2;
    VMArg result;

    thread = t;
    bdatTbl = bdat;
    col1 = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    row = vmArgIntGet(3, vmArgPtrGet(thread, 2));
    col2 = vmArgStringGet(4, vmArgPtrGet(thread, 3));
    result.type = VM_TYPE_INT;
    result.value.uintVal = func_eu_8003B488(bdatTbl, col1, row, col2);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset
// LLM-HARNESS-END: us-8003c0c0

// LLM-HARNESS-BEGIN: us-8003c180
extern "C" void ocBdatRegist() {
    vmOCRegist(&lbl_eu_80524E40);
}
// LLM-HARNESS-END: us-8003c180
