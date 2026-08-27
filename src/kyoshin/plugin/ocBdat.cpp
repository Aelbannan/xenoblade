// Auto-scaffolded catalog TU for kyoshin/plugin/ocBdat
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/code_801862C0.hpp"
#include "monolib/vm/yvm2.h"
#include <string.h>

// Defining TU; canonical declaration lives in plugin/ocBdat.hpp
// (retail signature u32 (void*, const char*, s32)).
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* col, s32 index) {
    BdatHeader* hdr = static_cast<BdatHeader*>(bdat);
    char* base = reinterpret_cast<char*>(hdr);
    void* colEntry = func_8003B4B0(hdr, col);
    if (colEntry == 0) {
        return 0;
    }
    if (bdat == 0) {
        return 0;
    }
    s32 rowIdx = index - hdr->rowBase;
    s32 ok;
    if (hdr->maxRow < rowIdx || rowIdx < 0) {
        ok = 0;
    } else {
        index = rowIdx;
        ok = 1;
    }
    if (ok == 0) {
        return 0;
    }
    u16 colRel = static_cast<BdatColEntry*>(colEntry)->colHdrRel;
    char* colHdr = base + colRel;
    if (static_cast<u8>(colHdr[0]) != 1) {
        return 0;
    }
    u16 stride = hdr->stride;
    u16 dataOff = hdr->dataOff;
    s32 rowBytes = index * stride;
    u16 colDataOff = *reinterpret_cast<u16*>(colHdr + 0x2);
    u8 elemType = static_cast<u8>(colHdr[1]);
    char* dataPtr = base + dataOff;
    dataPtr += rowBytes;
    dataPtr += colDataOff;
    return func_8003B6A0(hdr, dataPtr, elemType);
}

#pragma dont_inline on
// func_eu_8003B720: decode obfuscated BDAT entries. Each flagged entry holds
// two running-XOR key bytes at +0x16 and two [start,end) byte ranges (+0x18/
// +0x1C relative to the entry, and +0x06/+0x0A u16 ranges); bytes are XORed
// with alternating key bytes which advance by the original byte value. The
// second range uses the complemented seeds. Clears the flag when done.
extern "C" void func_eu_8003B720(void* tblVoid) {
    char* base = static_cast<char*>(tblVoid);
    u32 count = *reinterpret_cast<u32*>(base);
    u32* offsets = reinterpret_cast<u32*>(base + 8);
    for (u32 i = 0; i < count; i++) {
        char* entry = base + offsets[i];
        if ((entry[4] & 2) == 0) {
            continue;
        }

        u16 keySeed = *reinterpret_cast<u16*>(entry + 0x16);
        u8 keyHi = keySeed >> 8;
        u8 keyLo = keySeed & 0xFF;
        u8 keyHi2 = ~keyHi;
        u8 keyLo2 = ~keyLo;
        s16 start = *reinterpret_cast<u32*>(entry + 0x18);
        s16 end = *reinterpret_cast<u32*>(entry + 0x1C);
        int total = start + end;

        s16 pos = start;
        if (start < total) {
            if ((end + 1) / 2 > 8) {
                u32 n = (u32)(end - 1) >> 4;
                while (n != 0) {
                    u8 b0 = entry[pos];
                    entry[pos] = b0 ^ keyHi;
                    keyHi += b0;
                    u8 b1 = entry[pos + 1];
                    entry[pos + 1] = b1 ^ keyLo;
                    keyLo += b1;
                    pos += 16;
                    n--;
                }
            }
            while (pos < total) {
                u8 b0 = entry[pos];
                entry[pos] = b0 ^ keyHi;
                keyHi += b0;
                u8 b1 = entry[pos + 1];
                entry[pos + 1] = b1 ^ keyLo;
                keyLo += b1;
                pos += 2;
            }
        }

        int pos2 = *reinterpret_cast<u16*>(entry + 6);
        int end2 = *reinterpret_cast<u16*>(entry + 0xA);
        if (pos2 < end2) {
            if ((end2 - pos2 + 1) / 2 > 8) {
                u32 n = (u32)(end2 - pos2 - 1) >> 4;
                while (n != 0) {
                    u8 b0 = entry[pos2];
                    entry[pos2] = b0 ^ keyHi2;
                    keyHi2 += b0;
                    u8 b1 = entry[pos2 + 1];
                    entry[pos2 + 1] = b1 ^ keyLo2;
                    keyLo2 += b1;
                    pos2 += 16;
                    n--;
                }
            }
            while (pos2 < end2) {
                u8 b0 = entry[pos2];
                entry[pos2] = b0 ^ keyHi2;
                keyHi2 += b0;
                u8 b1 = entry[pos2 + 1];
                entry[pos2 + 1] = b1 ^ keyLo2;
                keyLo2 += b1;
                pos2 += 2;
            }
        }

        entry[4] &= static_cast<char>(~2);
    }
}
#pragma dont_inline reset


extern "C" void* func_8003AA34() {
    if (!lbl_eu_80663D10) {
        lbl_eu_80663D10 = 1;
    }
    //retail returns &lbl_eu_80663D14 via SDA21 (li r3,0 + R_PPC_EMB_SDA21 reloc)
    return &lbl_eu_80663D14;
}

void* CBdat::clearEntries() {
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

void* CBdat::func_8003AA78(u32 idx, void* p) {
    lbl_eu_805705D0[idx] = p;
    func_eu_8003B720(p);
    return p;
}

void CBdat::getEntry(u32 idx) {
    lbl_eu_805705D0[idx] = 0;
}

#pragma dont_inline on
void* getFP(const char* pName) {
    void** tableSlot;
    BdatNameIndexHdr* hdr;
    s32 tableIdx;
    char* entry;
    s32 lo;
    s32 hi;
    s32 mid;
    s32 cmp;

    tableSlot = lbl_eu_805705D0;
    tableIdx = 0;
    while (tableIdx < 7) {
        hdr = static_cast<BdatNameIndexHdr*>(*tableSlot);
        if (hdr != 0) {
            char* base = reinterpret_cast<char*>(hdr);
            lo = 0;
            hi = hdr->count;
            while (lo < hi) {
                mid = (lo + hi) / 2;
                // Parenthesised (mid*4 + 8) makes MWCC form the address as
                // add rD, scaled, base (retail operand order); hdr->offsets[mid]
                // would emit add rD, base, scaled (one-byte diff).
                entry = base + *(u32*)(base + (mid * 4 + 8));
                cmp = strcmp(entry + *reinterpret_cast<u16*>(entry + 6), pName);
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

#pragma dont_inline on
// func_8003B6A0: read one BDAT column element of the given type from data,
// widening to u32. Type 6 is an offset resolved against the BDAT base.
extern "C" u32 func_8003B6A0(void* bdat, void* data, u32 elemType) {
    u32 val = 0;
    switch (elemType) {
    case 0:
        *(u8*)&val = *(u8*)data;
        break;
    case 1:
        *(u16*)&val = *(u16*)data;
        break;
    case 2:
        *(u32*)&val = *(u32*)data;
        break;
    case 3:
        *(u8*)&val = *(u8*)data;
        break;
    case 4:
        *(s16*)&val = *(s16*)data;
        break;
    case 5:
        *(u32*)&val = *(u32*)data;
        break;
    case 6:
        *(u32*)&val = reinterpret_cast<uintptr_t>(bdat) + *static_cast<u32*>(data);
        break;
    case 7:
    case 8:
        *(u32*)&val = *(u32*)data;
        break;
    }
    return val;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" void* func_8003B4B0(void* bdat, const char* col){
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

    if (bdat == 0) {
        return 0;
    }
    BdatHeader* hdr = static_cast<BdatHeader*>(bdat);
    char* base = reinterpret_cast<char*>(hdr);
    colArg = col;
    bucketCount = hdr->bucketCount;
    hashBase = base + hdr->hashBaseOff;
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
    rem %= bucketCount;
    bucketOff = *reinterpret_cast<u16*>(hashBase + rem * 2);
    if (bucketOff == 0) {
        return 0;
    }
    entry = base + bucketOff;
    while (1) {
        if (strcmp(colArg, entry + 4) == 0) {
            return entry;
        }
        nextOff = *reinterpret_cast<u16*>(entry + 2);
        if (nextOff == 0) {
            break;
        }
        entry = base + nextOff;
    }
    return 0;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" u32 func_8003AD98(void* bdat, const char* col, s32 row, s32 index){
    const char* colArg;
    s32 rowArg;
    s32 indexArg;
    void* colEntry;
    s32 rowBase;
    s32 maxRow;
    s32 rowIdx;
    s32 ok;

    if (bdat == 0) {
        return 0;
    }
    BdatHeader* hdr = static_cast<BdatHeader*>(bdat);
    colArg = col;
    rowArg = row;
    indexArg = index;
    colEntry = func_8003B4B0(hdr, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdat == 0) {
        return 0;
    }
    rowBase = hdr->rowBase;
    maxRow = hdr->maxRow;
    rowIdx = rowArg - rowBase;
    if (maxRow < rowIdx || rowIdx < 0) {
        ok = 0;
    } else {
        rowArg = rowIdx;
        ok = 1;
    }
    if (ok == 0) {
        return 0;
    }
    return func_8003B748(hdr, colEntry, rowArg, indexArg);
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" u32 func_8003AFC0(void* bdat, const char* col) {
    const char* colArg;
    void* colEntry;
    char* colHdr;

    if (bdat == 0) {
        return 0;
    }
    BdatHeader* hdr = static_cast<BdatHeader*>(bdat);
    char* base = reinterpret_cast<char*>(hdr);
    colArg = col;
    colEntry = func_8003B4B0(hdr, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdat == 0) {
        return 0;
    }
    u16 colRel = static_cast<BdatColEntry*>(colEntry)->colHdrRel;
    colHdr = base + colRel;
    if (static_cast<u8>(colHdr[0]) != 2) {
        return 0;
    }
    return *reinterpret_cast<u16*>(colHdr + 0x4);
}
#pragma dont_inline reset

extern "C" u32 func_8003B1EC(void* p) {
    if (p == 0) {
        return 0;
    }
    return static_cast<BdatHeader*>(p)->maxRow;
}

#pragma dont_inline on
extern "C" u32 func_8003B434(void* table, void*, void* col, s32 row) {
    void* colArg;
    s32 rowArg;
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

    if (table == 0) {
        return 0;
    }
    BdatHeader* hdr = static_cast<BdatHeader*>(table);
    char* base = reinterpret_cast<char*>(hdr);
    colArg = col;
    rowArg = row;
    rowIdx = rowArg - hdr->rowBase;
    if (hdr->maxRow < rowIdx) {
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
    colRel = static_cast<BdatColEntry*>(colArg)->colHdrRel;
    colHdr = base + colRel;
    if (static_cast<u8>(colHdr[0]) != 1) {
        return 0;
    }
    stride = hdr->stride;
    dataOff = hdr->dataOff;
    rowBytes = rowArg * stride;
    colDataOff = *reinterpret_cast<u16*>(colHdr + 0x2);
    dataBase = base + dataOff;
    elemType = static_cast<u8>(colHdr[1]);
    dataBase += rowBytes;
    dataPtr = dataBase + colDataOff;
    return func_8003B6A0(hdr, dataPtr, elemType);
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" u32 func_8003B204(void* bdat, const char* col) {
    const char* colArg;
    void* colEntry;
    char* colHdr;

    if (bdat == 0) {
        return 0;
    }
    BdatHeader* hdr = static_cast<BdatHeader*>(bdat);
    char* base = reinterpret_cast<char*>(hdr);
    colArg = col;
    colEntry = func_8003B4B0(hdr, colArg);
    if (colEntry == 0) {
        return 0;
    }
    if (bdat == 0) {
        return 0;
    }
    u16 colRel = static_cast<BdatColEntry*>(colEntry)->colHdrRel;
    colHdr = base + colRel;
    return static_cast<u8>(colHdr[1]);
}
#pragma dont_inline reset

extern "C" u32 func_8003B41C(void* p) {
    if (p == 0) {
        return 0;
    }
    return static_cast<BdatHeader*>(p)->rowBase;
}

#pragma dont_inline on
extern "C" u32 func_eu_8003B488(void* bdat, const char* col1, s32 row, const char* col2){
    const char* col1Arg;
    s32 rowArg;
    const char* col2Arg;
    s32 rowBase;
    s32 maxRow;
    s32 rowIdx;
    s32 ok;
    void* col1Entry;
    void* col2Entry;
    BdatColHdrFlag* flagHdr;
    char* col1Hdr;
    u32 val;
    u32 mask;
    u8 shift;

    if (bdat == 0) {
        return 0;
    }
    BdatHeader* hdr = static_cast<BdatHeader*>(bdat);
    char* base = reinterpret_cast<char*>(hdr);
    rowArg = row;
    col1Arg = col1;
    col2Arg = col2;
    rowBase = hdr->rowBase;
    maxRow = hdr->maxRow;
    rowIdx = rowArg - rowBase;
    if (maxRow < rowIdx || rowIdx < 0) {
        ok = 0;
    } else {
        rowArg = rowIdx;
        ok = 1;
    }
    if (ok == 0) {
        return 0;
    }
    col1Entry = func_8003B4B0(hdr, col1Arg);
    if (col1Entry == 0) {
        return 0;
    }
    col2Arg = col2;
    col2Entry = func_8003B4B0(hdr, col2Arg);
    if (col2Entry == 0) {
        return 0;
    }
    u16 col2Rel = static_cast<BdatColEntry*>(col2Entry)->colHdrRel;
    flagHdr = reinterpret_cast<BdatColHdrFlag*>(base + col2Rel);
    if (flagHdr->type != 3 ||
        static_cast<u16>(reinterpret_cast<uintptr_t>(col1Entry) -
                         reinterpret_cast<uintptr_t>(hdr)) !=
            flagHdr->colEntryRel) {
        return 0;
    }
    val = 0;
    u16 col1Rel = static_cast<BdatColEntry*>(col1Entry)->colHdrRel;
    col1Hdr = base + col1Rel;
    if (col1Hdr[0] == 1) {
        u16 stride = hdr->stride;
        u16 dataOff = hdr->dataOff;
        s32 rowBytes = rowArg * stride;
        u16 colDataOff = *reinterpret_cast<u16*>(col1Hdr + 0x2);
        u32 elemType = static_cast<u8>(col1Hdr[1]);
        char* dataPtr = base + dataOff + rowBytes + colDataOff;
        val = func_8003B6A0(hdr, dataPtr, elemType);
    }
    mask = flagHdr->mask;
    shift = flagHdr->shift;
    return (val & mask) >> shift;
    return 0;
}
#pragma dont_inline reset


extern "C" u32 func_8003B748(void* table, void* col, s32 row, s32 index){
    BdatHeader* hdr = static_cast<BdatHeader*>(table);
    char* base = reinterpret_cast<char*>(hdr);
    u16 colOff = static_cast<BdatColEntry*>(col)->colHdrRel;
    u8* colHdr = reinterpret_cast<u8*>(base) + colOff;
    if (colHdr[0] != 2) {
        return 0;
    }
    s32 count = *reinterpret_cast<u16*>(colHdr + 0x4);
    if (count <= index) {
        return 0;
    }
    u16 stride = hdr->stride;
    u16 dataOff = hdr->dataOff;
    s32 rowStride = row * stride;
    s32 elemType = static_cast<s32>(colHdr[1]);
    char* dataBase = base + dataOff;
    u16 colDataOff = *reinterpret_cast<u16*>(colHdr + 0x2);
    dataBase += rowStride;
    dataBase += colDataOff;
    // Retail: (elemType-6) <= 1 (types 6/7) jumps straight into the shared
    // 1/4, 2/5, 3 switch runs otherwise (retail `bc 4,1` = branch if NOT GT
    // straight into the shared *4 block).
    if (static_cast<u32>(elemType - 6) > 1) {
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
            goto scale4;
        default:
            break;
        }
    } else {
    scale4:
        dataBase += index * 4;
    }
    return func_8003B6A0(table, dataBase, elemType);
}


#pragma dont_inline on
extern "C" void func_8003B800(VMArg* out, void* data, u32 type){
    VMArg* outArg;
    void* dataArg;
    s32 typeArg;
    u32 local;

    outArg = out;
    dataArg = data;
    typeArg = static_cast<s32>(type);
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
        local = static_cast<u32>(*reinterpret_cast<s8*>(dataArg));
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

#pragma dont_inline on
extern "C" int getVal(VMThread* t, void* bdat){
    VMThread* thread;
    void* bdatTbl;
    const char* col;
    s32 idx;
    u32 dataSlot;
    u32 type;
    VMArg result;
    u32 value;

    thread = t;
    bdatTbl = bdat;
    col = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    idx = vmArgIntGet(3, vmArgPtrGet(thread, 2));
    dataSlot = getBdatStringColumnValue(bdatTbl, col, idx);
    type = func_8003B204(bdatTbl, col);
    value = dataSlot;
    func_8003B800(&result, &value, type);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" int getArrayVal(VMThread* t, void* bdat){
    VMThread* thread;
    void* bdatTbl;
    s32 row;
    const char* col;
    s32 index;
    u32 dataSlot;
    u32 type;
    VMArg result;
    u32 value;

    thread = t;
    bdatTbl = bdat;
    col = vmArgStringGet(2, vmArgPtrGet(thread, 1));
    row = vmArgIntGet(3, vmArgPtrGet(thread, 2));
    index = vmArgIntGet(4, vmArgPtrGet(thread, 3));
    dataSlot = func_8003AD98(bdatTbl, col, row, index);
    type = func_8003B204(bdatTbl, col);
    value = dataSlot;
    func_8003B800(&result, &value, type);
    vmRetValSet(thread, &result);
    return 1;
}
#pragma dont_inline reset

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

#pragma dont_inline on
extern "C" int getIdCount(VMThread* t, void* bdat) {
    VMArg result;

    result.type = VM_TYPE_INT;
    result.value.uintVal = func_8003B1EC(bdat);
    vmRetValSet(t, &result);
    return 1;
}
#pragma dont_inline reset

#pragma dont_inline on
extern "C" int getIdTop(VMThread* t, void* bdat) {
    VMArg result;

    result.type = VM_TYPE_INT;
    result.value.uintVal = func_8003B41C(bdat);
    vmRetValSet(t, &result);
    return 1;
}
#pragma dont_inline reset

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

extern "C" void ocBdatRegist() {
    vmOCRegist(&lbl_eu_80524E40);
}
