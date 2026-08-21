// CriWare SUD (Super Video Object Data) helper library.
//
// These routines support parsing of the .sud container header: they identify
// the stream flavour (divx field / CCS) from the format tag at offset 0x12/0x13
// and walk the embedded entry table in lbl_eu_8051D47C.

#include <harness_catalog.h>
#include <string.h>

// Init-once guard counter and the string-table pointer it initialises.
extern s32 lbl_eu_8061A4D0;
extern void* lbl_eu_8061A4D4;
extern const char lbl_eu_8051D448[];
extern const char lbl_eu_8051D47C[];

/**
 * One-time initialisation: points the module's string table pointer at the
 * default table. The guard counter makes repeated calls a no-op.
 */
void SUD_Init(void) {
    if (lbl_eu_8061A4D0 >= 1)
        return;
    lbl_eu_8061A4D4 = (void*)lbl_eu_8051D448;
    lbl_eu_8061A4D0 += 1;
}

/**
 * Returns 1 when the buffer's format tag (offset 0x12) marks a "div field"
 * style stream, 0 otherwise (also 0 for null/empty buffers).
 */
s32 SUD_AnalyTypeDivField(const char* buf, s32 len) {
    if (!buf || len < 0)
        return 0;
    return strncmp(buf + 0x12, lbl_eu_8051D47C + 0x15, 1) == 0 ? 1 : 0;
}

/**
 * Returns 1 when the buffer's format tag (offset 0x13) marks a CCS stream,
 * 0 otherwise (also 0 for null/empty buffers).
 */
s32 SUD_AnalyTypeCcs(const char* buf, s32 len) {
    if (!buf || len < 0)
        return 0;
    return strncmp(buf + 0x13, lbl_eu_8051D47C + 0x17, 1) == 0 ? 1 : 0;
}

/**
 * Scans `id` bytes of `data` for an entry matching the table signature
 * (marker byte at tbl+0x1f plus the 8-byte key at tbl). On a hit, stores the
 * match offset in *out1 and 35 in *out2 unless the offset lies in the top
 * half of the address space (in which case *out2 is 0).
 */
void SUD_SearchSudDat(char* data, s32 id, s32* out1, s32* out2) {
    const char* tbl;
    s32 i;

    *out1 = 0;
    *out2 = 0;
    if (data == NULL || id < 1)
        return;

    tbl = lbl_eu_8051D47C;
    i = 0;
    do {
        if (memcmp(data, tbl + 0x1f, 1) == 0 && memcmp(data, tbl, 8) == 0) {
            *out1 = (s32)data;
            // cntlzw bit 4 is set iff the top byte of the pointer is zero.
            *out2 = ((__cntlzw((u32)data) >> 5) & 1) ? 0 : 35;
        }
        data++;
        i++;
    } while (i < id);
}
