// CriWare SUD (Super Video Object Data) helper library.
//
// These routines support parsing of the .sud container header: they identify
// the stream flavour (divx field / CCS) from the format tag at offset 0x12/0x13
// and walk the embedded entry table in lbl_eu_8051D47C.

#include <harness_catalog.h>
#include <string.h>

// Init-once guard counter and the string-table pointer it initialises.
extern s32 lbl_eu_8061A4D0;
extern const char* lbl_eu_8061A4D4;
extern const char lbl_eu_8051D448[];
extern const char lbl_eu_8051D47C[];

/**
 * One-time initialisation: points the module's string table pointer at the
 * default table. The guard counter makes repeated calls a no-op.
 */
void SUD_Init(void) {
    if (lbl_eu_8061A4D0 >= 1)
        return;
    const char* defaultTable = lbl_eu_8051D448;
    lbl_eu_8061A4D4 = defaultTable;
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
 * Scans the first `id` bytes of `data` for an entry matching the SUD table
 * signature: the marker byte at table+0x1f followed by the 8-byte key at
 * table. On a hit, stores the match offset in *outOffset and 35 in *outSize,
 * unless the offset lies in the top quarter of the address space (cntlzw bit
 * 4 clear), in which case *outSize is 0.
 */
void SUD_SearchSudDat(char* data, s32 id, s32* outOffset, s32* outSize) {
    const char* table;
    s32 i;

    *outOffset = 0;
    *outSize = 0;
    // Guard + bare block keeps MWCC's branch-over-branch lowering
    // (`bgt loop_init; b end` instead of a single `ble end`).
    if (!(data != NULL && id > 0))
        return;
    {
        table = lbl_eu_8051D47C;
        i = 0;
        while (i < id) {
            if (memcmp(data, table + 0x1f, 1) == 0 && memcmp(data, table, 8) == 0) {
                *outOffset = (s32)data;
                *outSize = ((__cntlzw((u32)data) >> 5) & 1) ? 0 : 35;
            }
            ++data;
            ++i;
        }
    }
}
